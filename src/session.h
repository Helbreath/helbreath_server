//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#pragma once
#include <memory>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include "streams.h"
#include <mutex>

using asio::ip::tcp;

class session
    : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket)
        : socket_(std::move(socket))
    {
    }

    void start()
    {
        do_read();
    }

    void set_on_message_received(std::function<void(stream_read &&)> callback)
    {
        on_message_received = std::move(callback);
    }

    void set_on_message_sent(std::function<void(int64_t)> callback)
    {
        on_message_sent = std::move(callback);
    }

    void set_on_connection_closed(std::function<void(std::shared_ptr<session>)> callback)
    {
        on_connection_closed = std::move(callback);
    }

    int64_t send_msg(const char * data, int64_t size, char key = 0)
    {
        if (size <= 0 || size > max_length)
        {
            return -1;
        }
        stream_write sw(size);
        memcpy(sw.data, data, size);
        do_write(std::move(sw));
        if (on_message_sent)
        {
            on_message_sent(size);
        }

        return size;
    }

    int64_t send_msg(stream_write & sw)
    {
        if (sw.size <= 0 || sw.size > max_length)
        {
            return -1;
        }
        stream_write sw_{ sw };
        do_write(std::move(sw_));
        if (on_message_sent)
        {
            on_message_sent(sw.size);
        }

        return sw.size;
    }

    void close(bool callback = true)
    {
        if (callback && on_connection_closed)
        {
            on_connection_closed(shared_from_this());
        }
        socket_.close();
    }

    tcp::socket & get_socket()
    {
        return socket_;
    }

private:
    void do_read()
    {
        std::lock_guard<std::mutex> lock(read_mtx);
        auto self(shared_from_this());
        socket_.async_read_some(asio::buffer(data_, max_length),
            [this, self](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    if (on_message_received)
                    {
                        stream_read read_stream(data_, length);
                        on_message_received(std::move(read_stream));
                    }
                    do_read();
                }
                else /*if (ec == asio::error::eof)*/
                {
                    if (on_connection_closed) on_connection_closed(shared_from_this());
                }
            });
    }

    void do_write(stream_write && sw)
    {
        std::lock_guard<std::mutex> lock(write_mtx);

        std::copy(sw.data, sw.data + sw.size, write_data_);

        auto self(shared_from_this());
        asio::async_write(socket_, asio::buffer(write_data_, sw.size),
            [this, self](std::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    do_read();
                }
                else
                {
                    std::cerr << "Send failed: " << ec.message() << std::endl;
                }
            });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length]{};
    char write_data_[max_length]{};
    std::mutex read_mtx{};
    std::mutex write_mtx{};

    std::function<void(stream_read &&)> on_message_received;
    std::function<void(int64_t)> on_message_sent;
    std::function<void(std::shared_ptr<session>)> on_connection_closed;
};
