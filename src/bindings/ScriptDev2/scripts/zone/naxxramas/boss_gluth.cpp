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
SDName: Boss_Gluth
SD%Complete: 100
SDComment: 
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "def_naxxramas.h"

#define SPELL_MORTALWOUND       25646    
#define SPELL_DECIMATE          28374              
#define SPELL_TERRIFYINGROAR    29685
#define SPELL_FRENZY            19812
#define SPELL_ENRAGE            28747

#define ADD_1X 3269.590
#define ADD_1Y -3161.287
#define ADD_1Z 297.423

#define ADD_2X 3277.797
#define ADD_2Y -3170.352
#define ADD_2Z 297.423

#define ADD_3X 3267.049
#define ADD_3Y -3172.820
#define ADD_3Z 297.423

#define ADD_4X 3252.157
#define ADD_4Y -3132.135
#define ADD_4Z 297.423

#define ADD_5X 3259.990
#define ADD_5Y -3126.590
#define ADD_5Z 297.423

#define ADD_6X 3259.815
#define ADD_6Y -3137.576
#define ADD_6Z 297.423

#define ADD_7X 3308.030
#define ADD_7Y -3132.135
#define ADD_7Z 297.423

#define ADD_8X 3303.046
#define ADD_8Y -3180.682
#define ADD_8Z 297.423

#define ADD_9X 3313.283
#define ADD_9Y -3180.766
#define ADD_9Z 297.423



struct MANGOS_DLL_DECL boss_gluthAI : public ScriptedAI
{
    boss_gluthAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 MortalWound_Timer;
    uint32 Decimate_Timer;
    uint32 TerrifyingRoar_Timer;
    uint32 Frenzy_Timer;
    uint32 Enrage_Timer;
    uint32 Summon_Timer;
    bool InCombat;

    void Reset()
    {       
        MortalWound_Timer = 8000;
        Decimate_Timer = 105000;
        TerrifyingRoar_Timer = 21000;
        Frenzy_Timer = 15000;
        Enrage_Timer = 304000;
        Summon_Timer = 40000;
        InCombat = false;

        if (pInstance)
        {
            pInstance->SetData(ENCOUNT_GLUTH, 0);
            HandleDoors(GO_GLUTH_EXIT, 1);
            HandleDoors(GO_THADDIUS_DOOR, 1);
        }
    }


    void JustDied(Unit* Killer)
    {
        if (pInstance)
        {
            pInstance->SetData(ENCOUNT_GLUTH, 2);
            HandleDoors(GO_GLUTH_EXIT, 0);
            Unit *temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_PATCHWERK));
            if (temp && temp->isDead())
                HandleDoors(GO_THADDIUS_DOOR, 0);
        }
    }

    void Aggro(Unit *who)
    {
        DoZoneInCombat();
        if (!InCombat)
        {
            if(pInstance)
                pInstance->SetData(ENCOUNT_GLUTH, 1);
            InCombat = true;
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

        //MortalWound_Timer
        if (MortalWound_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_MORTALWOUND);

            //10 seconds
            MortalWound_Timer = 10000;
        }else MortalWound_Timer -= diff;

        //Decimate_Timer
        if (Decimate_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_DECIMATE);

            Unit* Temp = NULL;
            std::list<HostilReference*>::iterator i = m_creature->getThreatManager().getThreatList().begin();
            for (; i != m_creature->getThreatManager().getThreatList().end(); ++i)
            {
                Temp = Unit::GetUnit((*m_creature),(*i)->getUnitGuid());
                if (Temp )
                    Temp->SetHealth(Temp->GetMaxHealth()* 0.05);
            }
            //105 seconds until we should cast this agian
            Decimate_Timer = 105000;
        }else Decimate_Timer -= diff;

        //TerrifyingRoar_Timer
        if (TerrifyingRoar_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_TERRIFYINGROAR);

            //20 seconds until we should cast this agian
            TerrifyingRoar_Timer = 20000;
        }else TerrifyingRoar_Timer -= diff;

        //Frenzy_Timer
        if (Frenzy_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_FRENZY);

            //10.5 seconds until we should cast this agian
            Frenzy_Timer = 10500;
        }else Frenzy_Timer -= diff;

        if (Enrage_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_ENRAGE);

            //61 seconds until we should cast this agian
            Enrage_Timer = 61000;
        }else Enrage_Timer -= diff;

        //Summon_Timer
        if (Summon_Timer < diff)
        {

            Unit* target = NULL;
            Unit* SummonedZombies = NULL;

            for (int i = 0; i < 6; i++);
            {
                switch (rand()%9)
                {
                case 0:
                    SummonedZombies = m_creature->SummonCreature(16360,ADD_1X,ADD_1Y,ADD_1Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                    if (SummonedZombies)
                    {
                        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                        if (target)
                            SummonedZombies->AddThreat(target,0.0f);
                    }
                    break;
                case 1:
                    SummonedZombies = m_creature->SummonCreature(16360,ADD_2X,ADD_2Y,ADD_2Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                    if (SummonedZombies)
                    {
                        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                        if (target)
                            SummonedZombies->AddThreat(target,0.0f);
                    }
                case 2:
                    SummonedZombies = m_creature->SummonCreature(16360,ADD_3X,ADD_3Y,ADD_3Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                    if (SummonedZombies)
                    {
                        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                        if (target)
                            SummonedZombies->AddThreat(target,0.0f);
                    }
                    break;
                case 3:
                    SummonedZombies = m_creature->SummonCreature(16360,ADD_4X,ADD_4Y,ADD_4Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                    if (SummonedZombies)
                    {
                        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                        if (target)
                            SummonedZombies->AddThreat(target,0.0f);
                    }
                    break;
                case 4:
                    SummonedZombies = m_creature->SummonCreature(16360,ADD_5X,ADD_5Y,ADD_5Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                    if (SummonedZombies)
                    {
                        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                        if (target)
                            SummonedZombies->AddThreat(target,0.0f);
                    }
                    break;
                case 5:
                    SummonedZombies = m_creature->SummonCreature(16360,ADD_6X,ADD_6Y,ADD_6Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                    if (SummonedZombies)
                    {
                        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                        if (target)
                            SummonedZombies->AddThreat(target,0.0f);
                    }
                    break;
                case 6:
                    SummonedZombies = m_creature->SummonCreature(16360,ADD_7X,ADD_7Y,ADD_7Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                    if (SummonedZombies)
                    {
                        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                        if (target)
                            SummonedZombies->AddThreat(target,0.0f);
                    }
                    break;
                case 7:
                    SummonedZombies = m_creature->SummonCreature(16360,ADD_8X,ADD_8Y,ADD_8Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000); 
                    if (SummonedZombies)
                    {
                        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                        if (target)
                            SummonedZombies->AddThreat(target,0.0f);
                    }
                    break;
                case 8:
                    SummonedZombies = m_creature->SummonCreature(16360,ADD_9X,ADD_9Y,ADD_9Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                    if (SummonedZombies)
                    {
                        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                        if (target)
                            SummonedZombies->AddThreat(target,0.0f);
                    }
                    break;
                }
            }

            //24 seconds until we should cast this agian
            Summon_Timer = 120000;
        } else Summon_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_gluth(Creature *_Creature)
{
    return new boss_gluthAI (_Creature);
}


void AddSC_boss_gluth()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_gluth";
    newscript->GetAI = GetAI_boss_gluth;
    newscript->RegisterSelf();
}
