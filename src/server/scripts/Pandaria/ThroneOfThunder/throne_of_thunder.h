/*
* Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef THRONE_OF_THUNDER_H
#define THRONE_OF_THUNDER_H

#include "Map.h"
#include "CreatureAI.h"

#define DataHeader "TO"
#define ThroneOfThunderScriptName "instance_throne_of_thunder"

uint32 const EncounterCount = 12;

enum DataTypes
{
    DATA_JIN_ROKH_THE_BREAKER = 0,
    DATA_HORRIDON               = 1,
    DATA_COUNCIL_OF_ELDERS = 2,
    DATA_TORTOS = 3,
    DATA_MEGAERA = 4,
    DATA_JI_KUN = 5,
    DATA_DURUMU_THE_FORGOTTEN = 6,
    DATA_PRIMORDIUS = 7,
    DATA_DARK_ANIMUS = 8,
    DATA_IRON_QON = 9,
    DATA_TWIN_CONSORTS = 10,
    DATA_LEI_SHEN   = 11,
    DATA_RA_DEN     = 12,
};

enum CreatureIds
{
    // Lei-Shen
    NPC_BOUNCING_BOLT_CONDUIT   =   68698,
    NPC_STATIS_SHOCK_CONDUIT    =   69309,
    NPC_DIFFUSION_CHAIN_CONDUIT =   68696,
    NPC_OVERCHARGE_CONDUIT      =   68697,
};

template<class AI>
AI* GetThroneOfThunderAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, UKScriptName);
}

#endif // THRONE_OF_THUNDER_H
