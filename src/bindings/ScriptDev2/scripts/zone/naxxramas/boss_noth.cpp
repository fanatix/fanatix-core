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
SDName: Boss_Noth
SD%Complete: 40
SDComment: Missing Balcony stage
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "def_naxxramas.h"
#include "GameObject.h"

#define SAY_AGGRO1          "Glory to the master!"
#define SAY_AGGRO2          "Your life is forfeit!"
#define SAY_AGGRO3          "Die, trespasser!"
#define SAY_SUMMON          "Rise, my soldiers! Rise and fight once more!"
#define SAY_SLAY1           "My task is done!"
#define SAY_SLAY2           "Breathe no more!"
#define SAY_DEATH           "I will serve the master... in... death!"
#define SOUND_AGGRO1        8845
#define SOUND_AGGRO2        8846
#define SOUND_AGGRO3        8847
#define SOUND_SUMMON        8851
#define SOUND_SLAY1         8849
#define SOUND_SLAY2         8849
#define SOUND_DEATH         8848

#define SPELL_BLINK                             29211
#define SPELL_CRIPPLE                           29212
#define SPELL_CURSEPLAGUEBRINGER                29213

#define MOB_PLAGUED_WARRIOR                     16984
#define MOB_PLAGUED_CONSTRUCT                   16982
#define MOB_PLAGUED_GUARDIAN                    16981
#define MOB_PLAGUED_CHAMPION                    16983

float NothPosition[2][4] =
{
    {2631.370,-3529.680,274.040,6.277},
    {2684.837,-3502.453,261.313,0}
};

float WavePosition[3][3] =
{
    {2724.101,-3525.891,261.947},
    {2717.452,-3465.517,262.049},
    {2651.987,-3469.556,261.996}
};

// IMPORTANT: BALCONY TELEPORT NOT ADDED YET! WILL BE ADDED SOON! 
// Best joke ever :D

struct MANGOS_DLL_DECL boss_nothAI : public ScriptedAI
{
    boss_nothAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }

    ScriptedInstance *pInstance;
    Creature* SummonedSkeletons;

    uint32 Blink_Timer;
    uint32 Curse_Timer;
    uint32 Summon_Timer;
    bool InCombat;
	uint32 teleport;
	bool wavephase;
	uint32 teleport_timer;
    uint64 summon_guid[20];
    uint32 wavecount;
    
    uint32 check;
    
   
    void ResetSummonGuids()
    {
        for(int i = 0; i<20 ; i++)
        {
            summon_guid[i] = 0;
        }        
    }

    void Reset()
    {       
        Blink_Timer = 25000;
        Curse_Timer = 10000;
        Summon_Timer = 12000;
		teleport_timer = 90000;
        InCombat = false;
		teleport = 0;
	    wavephase = false;
        wavecount = 0;
        
        ResetSummonGuids();

        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_NOTH, 0);
            HandleDoors(GO_NOTH_ENTRY, 0);
            HandleDoors(GO_NOTH_EXIT, 1);
            HandleDoors(GO_LOATHEB_DOOR, 1);
        }
    }
    
    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            if (!InCombat)
            {
                if (wavephase){AttackStart(who);}
                else {AttackStart(who);}
                Aggro(who);
                InCombat = true;
            }
        }
    }

    void MoveInLineOfSight(Unit* who)
    {
        if( !m_creature->getVictim() && who->isTargetableForAttack() && ( m_creature->IsHostileTo( who )) && who->isInAccessablePlaceFor(m_creature) )
        {
            if (m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(who);
            if(m_creature->IsWithinDistInMap(who, attackRadius))
            {
                // Check first that object is in an angle in front of this one before LoS check
                if( m_creature->HasInArc(M_PI/2.0f, who) && m_creature->IsWithinLOSInMap(who) )
                {
                    if (wavephase){AttackStart(who);}
                    else {AttackStart(who);}
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                    if (!InCombat)
                    {
                        Aggro(who);
                        InCombat = true;
                    }
                }
            }
        }
    }

    //void DoStopMovement(Creature *boss)
    //{
 
    //    if( boss->GetMotionMaster()->top()->GetMovementGeneratorType() == TARGETED_MOTION_TYPE)
    //    {
    //        boss->GetMotionMaster()->MovementExpired(true);
    //        boss->GetMotionMaster()->Clear();
    //        boss->StopMoving();
    //        boss->GetMotionMaster()->Mutate(new IdleMovementGenerator);
    //    }
    //}

    //void DoResumeMovement(Creature *boss, Unit *victim)
    //{
    //    if( (*boss).GetMotionMaster()->top()->GetMovementGeneratorType() == IDLE_MOTION_TYPE)
    //    {
    //        (*boss).GetMotionMaster()->MovementExpired(true);
    //        (*boss).GetMotionMaster()->Clear();

    //        if(victim)
    //        {
    //            (*boss).GetMotionMaster()->MoveChase(victim);
    //        }
    //    }
    //}

    void Aggro(Unit *who)
    {
        DoZoneInCombat();
        //Say our dialog on initial aggro
        switch (rand()%3)
        {
        case 0:
            DoYell(SAY_AGGRO1,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_AGGRO1);
            break;
        case 1:
            DoYell(SAY_AGGRO2,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_AGGRO2);
            break;
        case 2:
            DoYell(SAY_AGGRO3,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_AGGRO3);
            break;
        }

        InCombat = true;
        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_NOTH, 1);
            HandleDoors(GO_NOTH_ENTRY, 1);
        }
    }

    void KilledUnit(Unit* victim)
    {

        switch (rand()%2)
        {
        case 0:
            DoYell(SAY_SLAY1,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY1);
            break;
        case 1:
            DoYell(SAY_SLAY2,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY2);
            break;

        }
    }

    void JustDied(Unit* Killer)
    {
        DoYell(SAY_DEATH,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);
        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_NOTH, 2);
            HandleDoors(GO_NOTH_ENTRY, 0);
            HandleDoors(GO_NOTH_EXIT, 0);
            Unit *temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_HEIGAN));
            if (temp && temp->isDead())
                HandleDoors(GO_LOATHEB_DOOR, 0);
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
        if(!wavephase)
        {
            //Blink_Timer
            if (Blink_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CRIPPLE);
                //DoCast(m_creature,SPELL_BLINK);

                //25 seconds
                Blink_Timer = 25000;
            }else Blink_Timer -= diff;

            //Curse_Timer
            if (Curse_Timer < diff)
            {

                //Cast
                DoCast(m_creature->getVictim(),SPELL_CURSEPLAGUEBRINGER);

                //28 seconds until we should cast this agian
                Curse_Timer = 55000;
            }else Curse_Timer -= diff;
            
            if (Summon_Timer < diff)
            {

                DoYell(SAY_SUMMON,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_SUMMON);
                Unit* target = NULL;
                Unit* SummonedSkeletons = NULL;
                
                for (int i = 0; i < 3; i++)
                {       
                    if(rand()%3 == 0)
                        SummonedSkeletons = m_creature->SummonCreature(MOB_PLAGUED_WARRIOR,WavePosition[i][0],WavePosition[i][1],WavePosition[i][2],0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                    else 
                        SummonedSkeletons = m_creature->SummonCreature(MOB_PLAGUED_CONSTRUCT,WavePosition[i][0],WavePosition[i][1],WavePosition[i][2],0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                    if (SummonedSkeletons)
                    {
                       (*SummonedSkeletons).GetMotionMaster()->MovePoint(0, m_creature->getVictim()->GetPositionX(),m_creature->getVictim()->GetPositionY(),m_creature->getVictim()->GetPositionZ());
                    } 
                }
                
                //30 seconds until we should cast this agian
                Summon_Timer = 30000;
            } else Summon_Timer -= diff;
           
            if(teleport_timer < diff)
            {
                wavephase = true;
                wavecount = 0;
                check = 0;
                //DoStopMovement(m_creature);
                (*SummonedSkeletons).GetMotionMaster()->MovePoint(0, NothPosition[0][0],NothPosition[0][1],NothPosition[0][2]);
                m_creature->Relocate(NothPosition[0][0],NothPosition[0][1],NothPosition[0][2],NothPosition[0][3]);
                switch (teleport)
                {
                    case 0: teleport_timer = 70000; break;
                    case 1: teleport_timer = 90000; break;
                    default: teleport_timer = 110000; break;
                }
                teleport++;
                Summon_Timer = 10000;
            }else teleport_timer -=diff;         

            DoMeleeAttackIfReady();
        }
        else
        {
            if(teleport_timer < diff)
            {
                wavephase = false;
                //DoResumeMovement( m_creature,  m_creature->getVictim());
                m_creature->Relocate(NothPosition[1][0],NothPosition[1][1],NothPosition[1][2],NothPosition[1][3]);
                switch (teleport)
                {
                    case 1: teleport_timer = 40000 + teleport_timer; break;
                    case 2: teleport_timer = 90000 + teleport_timer; break;
                    default: teleport_timer = 70000 + teleport_timer; break;
                }             
                ResetSummonGuids();
                check = 0;
            }else teleport_timer -=diff; 
            
            //Summon_Timer
            if (Summon_Timer < diff)
            {

                switch (teleport)
                {
                    case 1: 
                        for (int i = 0; i < 3; i++)
                        {
                            if(wavecount < 2)
                            {
                                SummonedSkeletons = m_creature->SummonCreature(MOB_PLAGUED_CHAMPION,WavePosition[i][0],WavePosition[i][1],WavePosition[i][2],0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                                if (SummonedSkeletons)
                                {
                                    Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
                                    if (target)
                                        (*SummonedSkeletons).GetMotionMaster()->MovePoint(0, target->GetPositionX(),target->GetPositionY(),target->GetPositionZ());
                                    summon_guid[check] = SummonedSkeletons->GetGUID();
                                    check++;                                    
                                } 
                            }   
                        } 
                        wavecount++;
                    break;
                    case 2:
                        switch(wavecount)
                        {
                            case 0:
                                for (int i = 0; i < 3; i++)
                                {
                                    SummonedSkeletons = m_creature->SummonCreature(MOB_PLAGUED_CHAMPION,WavePosition[i][0],WavePosition[i][1],WavePosition[i][2],0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                                    if (SummonedSkeletons)
                                    {
                                        Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
                                        if (target)
                                            (*SummonedSkeletons).GetMotionMaster()->MovePoint(0, target->GetPositionX(),target->GetPositionY(),target->GetPositionZ());
                                        summon_guid[check] = SummonedSkeletons->GetGUID();
                                        check++;
                                    } 
                                }
                                wavecount++;
                            break;
                            case 1:
                                for (int i = 0; i < 3; i++)
                                {
                                    SummonedSkeletons = m_creature->SummonCreature(MOB_PLAGUED_GUARDIAN,WavePosition[i][0],WavePosition[i][1],WavePosition[i][2],0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                                    if (SummonedSkeletons)
                                    {
                                        Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
                                        if (target)
                                            (*SummonedSkeletons).GetMotionMaster()->MovePoint(0, target->GetPositionX(),target->GetPositionY(),target->GetPositionZ());
                                        summon_guid[check] = SummonedSkeletons->GetGUID();
                                        check++;                                        
                                    } 
                                }
                            break;
                        }
                        wavecount++;
                    break;
                    default :
                        for (int i = 0; i < 3; i++)
                        {
                            if(wavecount < 2)
                            {
                                SummonedSkeletons = m_creature->SummonCreature(MOB_PLAGUED_GUARDIAN,WavePosition[i][0],WavePosition[i][1],WavePosition[i][2],0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                                if (SummonedSkeletons)
                                {
                                    Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
                                    if (target)
                                        (*SummonedSkeletons).GetMotionMaster()->MovePoint(0, target->GetPositionX(),target->GetPositionY(),target->GetPositionZ());
                                    summon_guid[check] = SummonedSkeletons->GetGUID();
                                    check++;                                        
                                } 
                            }
                        }
                        wavecount++;
                    break; 
                }             

                //30 seconds until we should cast this agian
                Summon_Timer = 30000;
            } else Summon_Timer -= diff;            
        }
    }
}; 
CreatureAI* GetAI_boss_noth(Creature *_Creature)
{
    return new boss_nothAI (_Creature);
}


void AddSC_boss_noth()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_noth";
    newscript->GetAI = GetAI_boss_noth;
    newscript->RegisterSelf();
}