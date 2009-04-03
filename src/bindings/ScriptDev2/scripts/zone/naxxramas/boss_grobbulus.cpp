/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* ScriptData
SDName: Boss_Grobbulus
SD%Complete: 0
SDComment: Place holder
SDCategory: Naxxramas
EndScriptData */

/*Poison Cloud 26590
Slime Spray 28157
Fallout slime 28218
Mutating Injection 28169
Enrages 26527*/

#include "precompiled.h"
#include "def_naxxramas.h"

#define SPELL_GROBBULUS_POISONCLOUD     28240
#define MOB_GROBBULUS_POISONCLOUD       16363
#define SPELL_POISONCLOUD_POISON        28156
#define SPELL_SLIME_SPRAY               28157
#define SPELL_MUTATING_INJECTION        28169
#define SPELL_SLIME_STREAM              28137

struct MANGOS_DLL_DECL boss_grobbulusAI : public ScriptedAI
{
    boss_grobbulusAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 cloud_timer;
    uint32 spray_timer;
    uint32 stream_timer;
    uint32 injection_timer;
    uint32 injectioneffect_timer;
    bool   injection;
    Unit* injection_target;

    bool InCombat;

    void Reset()
    {
        InCombat = false;

        cloud_timer = 15000;
        spray_timer = 20000;
        stream_timer = 10000;
        injection_timer = 45000;
        injection = false;
        injectioneffect_timer = 10000;

        injection_target = NULL;
        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_GROBBULUS, 0);
            HandleDoors(GO_PATCHWORK_EXIT, 0);
        }
    }

    void JustDied(Unit* Killer)
    {
        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_GROBBULUS, 2);
            HandleDoors(GO_PATCHWORK_EXIT, 0);
        }
    }

    void Aggro(Unit *who)
    {
        if (!InCombat)
        {                    
            InCombat = true;
            if(pInstance)
            {
                pInstance->SetData(ENCOUNT_GROBBULUS, 1);
                HandleDoors(GO_PATCHWORK_EXIT, 1);
            }
        }
    }

    //Open/Closes Doors 
    void HandleDoors(uint32 identifier, uint32 doorstate)
    {
        if(pInstance)
        {
            GameObject *door;
            door = GameObject::GetGameObject((*m_creature),pInstance->GetData64(identifier));
            if (door)
            {
                switch (doorstate)
                {
                case 0: //open
                    door->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
                    door->SetUInt32Value(GAMEOBJECT_BYTES_1, 0);
                    break;
                case 1: //close
                    door->SetUInt32Value(GAMEOBJECT_FLAGS, 0);
                    door->SetUInt32Value(GAMEOBJECT_BYTES_1, 1);
                    break;
                }
            }
            door = NULL;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (cloud_timer < diff)
        {
            DoCast(m_creature,SPELL_GROBBULUS_POISONCLOUD);
            cloud_timer = 30000;
        }else cloud_timer -= diff;

        if (spray_timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)
                DoCast(target, SPELL_SLIME_SPRAY);
            spray_timer = 20000;
        }else spray_timer -= diff;

        if (injection_timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)
            {
                DoCast(target, SPELL_MUTATING_INJECTION);
                injection_target = target;
                injection = true;
                injectioneffect_timer = 10000;
            }
            injection_timer = 30000;
        }else injection_timer -= diff;

        if (injection)
            if (injectioneffect_timer < diff)
            {
                if( injection_target && injection_target->HasAura(SPELL_MUTATING_INJECTION,0))
                    injection_target->CastSpell(injection_target,28158,true);
                injection = false;
                injectioneffect_timer = 10000;
            }else injectioneffect_timer -= diff;

        if (stream_timer < diff)
        {
            if(!(m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE)))
            {
                DoCast(m_creature->getVictim(), SPELL_SLIME_STREAM ,false);
            };
            stream_timer = 10000;
        }else stream_timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_grobbulus(Creature *_Creature)
{
    return new boss_grobbulusAI (_Creature);
}


struct MANGOS_DLL_DECL mob_grobbulus_poisoncloudAI : public ScriptedAI
{
    mob_grobbulus_poisoncloudAI(Creature *c) : ScriptedAI(c) 
    {
        Reset();
    }

    uint32 dead_timer;

    void Reset()
    {
        DoCast(m_creature,SPELL_POISONCLOUD_POISON);
        dead_timer = 60000;
    }
 
    void Aggro(Unit *who){}
    void MoveInLineOfSight(Unit *who) {}
    void AttackStart(Unit *who) {}


    void UpdateAI(const uint32 diff)
    {
        if (dead_timer < diff)
        {
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            dead_timer = 1000;
        }else dead_timer -= diff;
    }

};

CreatureAI* GetAI_mob_grobbulus_poisoncloud(Creature *_Creature)
{
    return new mob_grobbulus_poisoncloudAI (_Creature);
}


void AddSC_boss_grobbulus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_grobbulus";
    newscript->GetAI = GetAI_boss_grobbulus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_grobbulus_poisoncloud";
    newscript->GetAI = GetAI_mob_grobbulus_poisoncloud;
    newscript->RegisterSelf();
}
