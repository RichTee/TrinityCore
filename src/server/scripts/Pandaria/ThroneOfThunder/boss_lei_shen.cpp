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

/*Comment:
* This script is purely meant for educational purposes
* It should, and has, not be(en) compiled and has not been intended to work with any TC releases because of it
* Intended to be unfinished to befit personal purposes.
*/
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "throne_of_thunder.h"

/*
    The whole script needs sniffs, everything is based of assumptions.
*/
enum SAY
{
    // Lei-Shen
    SAY_INTRO = 0,
    SAY_AGGRO = 1,
    SAY_THUNDERSTRUCK_1 = 2,
    SAY_THUNDERSTRUCK_2 = 3,
    SAY_THUNDERSTRUCK_3 = 4,
    SAY_LIGHTNING_WHIP  = 5,
    SAY_VIOLENT_GALE_WINDS = 6,
    SAY_CONDUIT_CHARGE_1 = 7,
    SAY_CONDUIT_CHARGE_2 = 8,
    SAY_PHASE_2A = 9,
    SAY_PHASE_2B = 10,
    SAY_PHASE_3A = 11,
    SAY_PHASE_3B = 12,
    SAY_BERSERK = 13,
    SAY_SLAY_1  = 14,
    SAY_SLAY_2  = 15,
    SAY_SLAY_3  = 16,
    SAY_SLAY_4  = 17,
    SAY_SLAY_5  = 18,
    SAY_DEATH   = 19,

};

enum Spells
{
    // Lei-shen
    SPELL_DISCHARGE = 134820,
    SPELL_Decapitate = 134916,
    SPELL_THUNDERSTRUCK = 135095, // 2 versions, 134912
    SPELL_LIGHTNING_CRASH = 138272, // need sniff

    // Conduits
    SPELL_OVERCHARGE    = 135682,
    SPELL_STATIC_SHOCK = 135680,
    SPELL_DIFFUSION_CHAIN = 135681,
    SPELL_BOUNCING_BOLT = 135683,
};

enum Phases
{
    PHASE_ONE = 1,
    PHASE_TWO = 2,
    PHASE_INTERMISSION  = 3,
};

enum Events
{
    // Lei-Shen
    EVENT_BERSERK   = 1,
};

Position const conduitSpawnPoint[4] = { 
    { 1075.201f, -57.84896f, 55.42427f, 3.159046f }, // North, Static Shock
    { 1075.201f, -57.84896f, 55.42427f, 3.159046f }, // East, Diffusion Chain
    { 1075.201f, -57.84896f, 55.42427f, 3.159046f }, // South, Overcharge
    { 1075.201f, -57.84896f, 55.42427f, 3.159046f }, // West, Bouncing Bolt
};

// Needed movepoints ?

class boss_lei_shen : public CreatureScript
{
public:
    boss_lei_shen() : CreatureScript("boss_lei_shen") { }

    struct boss_lei_shenAI : public BossAI
    {
        boss_lei_shenAI(Creature* creature) : BossAI(creature, DATA_LEI_SHEN)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        void Reset() override
        {
            _Reset();

            instance->SetBossState(DATA_LEI_SHEN, NOT_STARTED);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

            me->SetReactState(REACT_PASSIVE);

            events.SetPhase(PHASE_ONE);
        }

        void EnterCombat(Unit* /*who*/) override
        {
            me->SetReactState(REACT_AGGRESSIVE);
            me->setActive(true);

            DoZoneInCombat();

            Talk(SAY_AGGRO);

            instance->SetBossState(DATA_LEI_SHEN, IN_PROGRESS);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
        }

        void JustDied(Unit* /*killer*/) override
        {
            instance->SetBossState(DATA_LEI_SHEN, DONE);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) override
        {
            // 65%
            // 30%
        }

        void KilledUnit(Unit* victim/*Killed*/) override
        {
            if (victim->GetTypeId() == TYPEID_PLAYER) {
                uint8 randomId = urand(1, 5);
                switch (randomId){
                    case 1:
                        Talk(SAY_SLAY_1);
                        break;
                    case 2:
                        Talk(SAY_SLAY_2);
                        break;
                    case 3:
                        Talk(SAY_SLAY_3);
                        break;
                    case 4:
                        Talk(SAY_SLAY_4);
                        break;
                    case 5:
                        Talk(SAY_SLAY_5);
                        break;
                    default:
                        Talk(SAY_SLAY_1);
                        break;
                }
            }
        }


        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            DoMeleeAttackIfReady();
            
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_lei_shenAI(creature);
    }
};

class npc_bouncing_bolt_conduit : public CreatureScript
{
    public:
        npc_bouncing_bolt_conduit() : CreatureScript("npc_bouncing_bolt_conduit") { }

        struct npc_bouncing_bolt_conduitAI : public ScriptedAI
        {

            npc_bouncing_bolt_conduitAI(Creature* creature) : ScriptedAI(creature)
            {
                disabled = false;
                level = 0;
            }

        private:
            EventMap events;
            bool disabled;
            int32 level;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_bouncing_bolt_conduitAI(creature);
        }
};

class npc_static_shock_conduit : public CreatureScript
{
    public:
        npc_static_shock_conduit() : CreatureScript("npc_static_shock_conduit") { }

        struct npc_static_shock_conduitAI : public ScriptedAI
        {

            npc_static_shock_conduitAI(Creature* creature) : ScriptedAI(creature)
            {
                disabled = false;
                level = 0;
            }

        private:
            EventMap events;
            bool disabled;
            int32 level;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_static_shock_conduitAI(creature);
        }
};

class npc_diffusion_chain_conduit : public CreatureScript
{
    public:
        npc_diffusion_chain_conduit() : CreatureScript("npc_diffusion_chain_conduit") { }

        struct npc_diffusion_chain_conduitAI : public ScriptedAI
        {

            npc_diffusion_chain_conduitAI(Creature* creature) : ScriptedAI(creature)
            {
                disabled = false;
                level = 0;
            }

        private:
            EventMap events;
            bool disabled;
            int32 level;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_diffusion_chain_conduitAI(creature);
        }
};

class npc_overcharge_conduit : public CreatureScript
{
    public:
        npc_overcharge_conduit() : CreatureScript("npc_overcharge_conduit") { }

        struct npc_overcharge_conduitAI : public ScriptedAI
        {

            npc_overcharge_conduitAI(Creature* creature) : ScriptedAI(creature)
            {
                disabled = false;
                level = 0;
            }

        private:
            EventMap events;
            bool disabled;
            int32 level;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_overcharge_conduitAI(creature);
        }
};

class npc_ball_lightning : public CreatureScript
{
    public:
        npc_ball_lightning() : CreatureScript("npc_ball_lightning") { }

        struct npc_ball_lightningAI : public ScriptedAI
        {

            npc_ball_lightningAI(Creature* creature) : ScriptedAI(creature)
            {

            }

        private:
            EventMap events;
        };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ball_lightningAI(creature);
    }
};

void ADDSC_boss_lei_shen()
{
    new boss_lei_shen();

    // Object-like
    new npc_bouncing_bolt_conduit();
    new npc_static_shock_conduit();
    new npc_diffusion_chain_conduit();
    new npc_overcharge_conduit();

    // NPC
    new npc_ball_lightning();
}