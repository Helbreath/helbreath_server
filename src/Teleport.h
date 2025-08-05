//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#pragma once

class CTeleport
{
public:
    CTeleport() = default;
    ~CTeleport() = default;

    char  m_cTeleportNum{};
    char  m_cTeleportNpcName[21]{};								// The npc with teleport ability.
    char  m_cSourceMap[10]{};										// The map the teleport npc is located in.
    char  m_cTargetMap[10]{};											// The map the player will be teleported to.
    short m_sDestinationX{ -1 }, m_sDestinationY{ -1 };						// The destination coordinates on the map.
    short m_sTeleportCost{ -1 };										// The amount of gold paid to teleport.
    short m_sTeleportMinLevel{ 0 }, m_sTeleportMaxLevel{ 0 };				// The required level range for the player.
    char  m_cTeleportSide[7]{};										// The side the player must be for a successful teleport.
    // both
    // elvine
    // aresden
    bool m_bTeleportHunt{ false }, m_bTeleportNtrl{ false }, m_bTeleportCrmnl{ false };	// Check if player is a Civilian, Neutral, or Criminal.
};
