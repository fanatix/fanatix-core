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
SDName: Boss_KelThuzud
SD%Complete: 0
SDComment: VERIFY SCRIPT
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "sc_creature.h"
#include "TargetedMovementGenerator.h"
#include "IdleMovementGenerator.h"
#include "def_naxxramas.h"

//Kel'thuzad. Still don't know where to use these...
//Dialog
//8878 dialog01 - Our preparations continue as planned, master.
//8881 lich_naxx_dialog01 - It is good that you serve me so faithfully. Soon, all will serve the Lich King and in the end, you shall be rewarded...so long as you do not falter.
//8879 dialog02 - I see no complications... Wait... What is this?
//8882 lich_naxx_dialog03 - Your security measures have failed! See to this interruption immediately!
//8880 dialog03 - Yes, master!
//Bigglesworth death
//No!!! A curse upon you, interlopers! The armies of the Lich King will hunt you down. You will not escape your fate...
//slay
//8818


#define NAXXRAMAS_MAP                533

float addspw[6][4] =
{
	{3783.27,-5062.69,143.71,3.61}, //LEFT FAR
	{3730.29,-5027.23,143.95,4.46}, //LEFT MIDDLE
	{3683.86,-5057.28,143.18,5.23}, //LEFT NEAR
	{3759.35,-5174.12,143.80,2.17}, //RIGHT FAR
	{3730.72,-5185.12,143.92,1.30}, //RIGHT MIDDLE
	{3665.12,-5138.67,143.18,0.60}  // RIGHT NEAR
};

//spells to be casted
#define SPELL_FROST_BOLT            28478
#define SPELL_FROST_BOLT_NOVA       28479
#define SPELL_CHAINS_OF_KELTHUZAD   28410
#define SPELL_MANA_DETONATION       27819
#define SPELL_SHADOW_FISURE         27810
#define SPELL_FROST_BLAST           27808

#define MOB_GUARDIAN_OF_THE_ICECROW 16441

//On Aggro
#define SAY_ARRIVAL1                "Pray for mercy!"
#define SOUND_ARRIVAL1              8809
#define SAY_ARRIVAL3                "Scream your dying breath!"
#define SOUND_ARRIVAL3              8810
#define SAY_ARRIVAL5                "The end is upon you!"
#define SOUND_ARRIVAL5              8811

//On Summon
#define SAY_REINFORCEMENTS1         "Minions, servants, soldiers of the cold dark! Obey the call of Kel'Thuzad!"
#define SOUND_REINFORCEMENTS1       8819
#define SAY_REINFORCEMENTS2         "Master, I require aid!"
#define SOUND_REINFORCEMENTS2       8816
#define SAY_LICH_NAXX_SUMMON        "Very well. Warriors of the frozen wastes, rise up! I command you to fight, kill and die for your master! Let none survive!"
#define SOUND_LICH_NAXX_SUMMON      8824

//Random 1/4 taunt said when player enters 300 yards
#define SAY_TAUNT01                 "Who dares violate the sactity of my domain? Be warned, all who traspass here are doomed"
#define SOUND_TAUNT01               8820
#define SAY_TAUNT02                 "Fools, you think yourselves triumphant? You have only taken one step closer to the abyss!"
#define SOUND_TAUNT02               8821
#define SAY_TAUNT03                 "I grow tired of these games. Proceed, and I will banish your souls to oblivion!"
#define SOUND_TAUNT03               8822
#define SAY_TAUNT04                 "You have no idea what horrors lie ahead. You have seen nothing! The frozen heart of Naxxramas awaits you!"
#define SOUND_TAUNT04               8823

//On kill unit
#define SAY_SLAY                    "The dark void awaits you!"
#define SOUND_SLAY                  8817

//Specials
#define SAY_FROST                   "I will freeze the blood in your veins!"
#define SOUND_FROST                 8815
#define SAY_CHAIN1                  "Your soul is bound to me, now!"
#define SOUND_CHAIN1                8812
#define SAY_CHAIN2                  "There will be no escape!"
#define SOUND_CHAIN2                8813
#define SAY_SPECIAL1                "Your petty magics are no challenge to the might of the scourge"
#define SOUND_SPECIAL1              9088
#define SAY_SPECIAL2                "Enough! I grow tired of these distractions!"
#define SOUND_SPECIAL2              9090
#define SAY_DISPEL                  "Fools, you have spread your powers too thin. Be Free, my minions!"
#define SOUND_DISPEL                9089

//On death
#define SAY_DEATH                   "Do not Rejoice... your victory is a hollow one... for I shall return with powers beyond your imagination!"
#define SOUND_DEATH                 8814

struct MANGOS_DLL_DECL boss_kelthuzadAI : public ScriptedAI
{
    boss_kelthuzadAI(Creature* c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }
	ScriptedInstance *pInstance;
    
    uint32 GuardiansOfIcecrown_Timer;
    uint32 FrostBolt_Timer;
    uint32 FrostBoltNova_Timer;
    uint32 ChainsOfKelthuzad_Timer;
    uint32 ManaDetonation_Timer;
    uint32 ShadowFisure_Timer;
    uint32 FrostBlast_Timer;
    uint32 ChainsOfKelthuzad_Targets;
    uint32 Phase1_Timer;
    
	bool phase1;
    bool phase2;
    bool phase3;
    bool InCombat;
    
    uint64 guardian[5];
    uint32 guardian_timer;
    uint32 guardian_counter;
    uint32 guardian_speech;
    bool guardian_b_speech;      

    void Reset()
    {
        FrostBolt_Timer = (rand()%60)*1000;             //It won't be more than a minute without cast it
        FrostBoltNova_Timer = 15000;                    //Cast every 15 seconds
        ChainsOfKelthuzad_Timer = (rand()%30+30)*1000;  //Cast no sooner than once every 30 seconds
        ManaDetonation_Timer = 20000;                   //Seems to cast about every 20 seconds
        ShadowFisure_Timer = 25000;                     //25 seconds
        FrostBlast_Timer = (rand()%30+30)*1000;         //Random time between 30-60 seconds

        Phase1_Timer = 300000; //Phase 1 lasts 5 minutes
		phase1 = false;
        phase2 = false;
        phase3 = false;
        InCombat = false; 
        guardian_counter = 0;
        

        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_KELTHUZAD, 0);
            HandleDoors(GO_KELTHUZAD_DOOR, 1);
        }
    }



    void KilledUnit()
    {
        if (rand()%5)
            return;

        DoYell(SAY_SLAY, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_SLAY);
    }

    void JustDied(Unit* Killer)
    {
        DoYell(SAY_DEATH,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);
        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_KELTHUZAD, 2);
            HandleDoors(GO_KELTHUZAD_DOOR, 0);
        }
    }



    void SayInitialAggro() //randomly select 1 of 3 say for aggro
    {
        switch(rand()%3)
            {
                case 0: DoYell(SAY_ARRIVAL1,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_ARRIVAL1);
                        break;
                case 1:    DoYell(SAY_ARRIVAL3,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_ARRIVAL3);
                        break;
                case 2:    DoYell(SAY_ARRIVAL5,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_ARRIVAL5);
						break;
            }
    }
    
    void SayRandomTaunt()
    {
        switch(rand()%4)
            {
                case 0: DoYell(SAY_TAUNT01,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_TAUNT01);
                        break;
                case 1: DoYell(SAY_TAUNT02,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_TAUNT02);
                        break;
                case 2: DoYell(SAY_TAUNT03,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_TAUNT03);
						break;
                case 3: DoYell(SAY_TAUNT04,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_TAUNT04);
						break;
            } 
    }

	void DoStopMovement(Creature *boss)
    {
 
        if( boss->GetMotionMaster()->top()->GetMovementGeneratorType() == TARGETED_MOTION_TYPE)
        {
            (*boss).GetMotionMaster()->MovementExpired(true);
            (*boss).GetMotionMaster()->Clear();
            boss->StopMoving();
            (*boss).GetMotionMaster()->MoveIdle();
        }
    }

	void DoResumeMovement(Creature *boss, Unit *victim)
	{
         if( (*boss).GetMotionMaster()->top()->GetMovementGeneratorType() == IDLE_MOTION_TYPE)
         {
             (*boss).GetMotionMaster()->MovementExpired(true);
             (*boss).GetMotionMaster()->Clear();
 
             if(victim)
             {
                   (*boss).GetMotionMaster()->MoveChase(victim);
             }
          }
	}


    void Aggro(Unit* who)
    {
       	DoYell(SAY_REINFORCEMENTS1,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_REINFORCEMENTS1);
       
        DoStopMovement(m_creature);
        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_KELTHUZAD, 1);
            HandleDoors(GO_KELTHUZAD_DOOR, 1);
        }
        InCombat = true;

    }

    void KelCast(const uint32 diff)
    {
        //Check for Frost Bolt

        if(FrostBolt_Timer < diff)
        {
            //time to cast
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
            DoCast(target,SPELL_FROST_BOLT);

            if(rand()%2 == 0)
            {
                DoYell(SAY_FROST,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_FROST);
            }

            //Cast again on time
            FrostBolt_Timer = (rand()%60)*1000;        
        }
        else FrostBolt_Timer -= diff;

        //Check for Frost Bolt Nova
        if(FrostBoltNova_Timer < diff)
        {
            //time to cast
            DoCast(m_creature->getVictim(),SPELL_FROST_BOLT_NOVA);

            if(rand()%2 == 0)
            {
                DoYell(SAY_FROST,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_FROST);
            }

                //cast again in 15 seconds
                FrostBoltNova_Timer = 15000;    

         }else FrostBoltNova_Timer -= diff;

            //Check for Chains Of Kelthuzad
         if(ChainsOfKelthuzad_Timer < diff)
         {
            //time to cast
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
            DoCast(target,SPELL_CHAINS_OF_KELTHUZAD);

            if(rand()%2 == 0)
                if(rand()%2 == 0)
                {
                    DoYell(SAY_CHAIN1,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_CHAIN1);
                }
                else
                {
                    DoYell(SAY_CHAIN2,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_CHAIN2);
                }

            //cast again on time
            ChainsOfKelthuzad_Timer = (rand()%30+30)*1000;        
        }
        else ChainsOfKelthuzad_Timer -= diff;

        //Check for Mana Detonation
        if(ManaDetonation_Timer < diff)
        {
            //time to cast
            //DoCast(m_creature->getVictim(),SPELL_MANA_DETONATION);

            if(rand()%2 == 0)
            {
                DoYell(SAY_SPECIAL1,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_SPECIAL1);
            }
            //cast again when it's time
            ManaDetonation_Timer = 20000;
        }
        else ManaDetonation_Timer -= diff;

        //Check for Shadow Fissure
        if(ShadowFisure_Timer < diff)
        {
            //time to cast
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
            DoCast(target,SPELL_SHADOW_FISURE);

            if(rand()%2 == 0)
            {
                DoYell(SAY_SPECIAL2,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_SPECIAL2);
            }
            //cast again when it's time
            ShadowFisure_Timer = 25000;    
        }
        else ShadowFisure_Timer -= diff;

        //Check for Frost Blast
        if(FrostBlast_Timer < diff)
        {
            //time to cast
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
            DoCast(target,SPELL_FROST_BLAST);

            if(rand()%2 == 0)
            {
                DoYell(SAY_FROST,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_FROST);
            }
            //cast again when it's time
            FrostBlast_Timer = (rand()%30+30)*1000;
        }
        else FrostBlast_Timer -= diff;

        DoMeleeAttackIfReady();
    };

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
        if(!m_creature->SelectHostilTarget())
            return;

        if(phase1 && !phase2 && !phase3) //Phase 1
        {
            if(Phase1_Timer < diff)
            {
                phase1 = false;
                phase2 = true;
                
                SayInitialAggro();

                DoResumeMovement(m_creature, m_creature->getVictim());
            }else Phase1_Timer -= diff;
        };

        if(!phase1 && phase2 && !phase3) //Phase 2
        {
            KelCast(diff);

            if((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 40)
            {
                DoYell(SAY_REINFORCEMENTS2,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_REINFORCEMENTS2);

                guardian_timer = 15000;
                guardian_speech = 5000;
                guardian_b_speech = false;
      
                phase2 = false;
                phase3 = true;
            };   
             
        };

        if(!phase1 && !phase2 && phase3) //Phase 3
        {
            if(!guardian_b_speech)
            {
                if (guardian_speech < diff)
                {
                    //DoYell(SAY_REINFORCEMENTS5,LANG_UNIVERSAL,NULL);
                    //DoPlaySoundToSet(m_creature,SOUND_REINFORCEMENTS5);
                    
                    guardian_b_speech = true;    
                    guardian_speech = 900000;    
                    guardian_timer = 5000;                    
                }else guardian_speech -= diff;
            }else
            {            
                if(guardian_counter < 5)
                {
                    if(guardian_timer < diff)
                    {
                        Unit* pUnit = NULL;
                        switch (rand()%2)
                        {
                            case 0: 
                                pUnit = m_creature->SummonCreature(MOB_GUARDIAN_OF_THE_ICECROW,addspw[0][0],addspw[0][1],addspw[0][2],addspw[0][3],TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,1000);
                                break;
                            case 1:                            
                                pUnit = m_creature->SummonCreature(MOB_GUARDIAN_OF_THE_ICECROW,addspw[3][0],addspw[3][1],addspw[3][2],addspw[3][3],TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,1000);
                                break;
                        }
                        
                        guardian[guardian_counter] = pUnit->GetGUID();
                        
                        guardian_timer = 5000;
                        guardian_counter++;
                    }else guardian_timer -= diff;
                }              
            }
            
            KelCast(diff);
        };
    }
};

CreatureAI* GetAI_boss_kelthuzadAI(Creature *_Creature)
{
    return new boss_kelthuzadAI (_Creature);
};

#define SPELL_BLOOD_TAP         28470

struct MANGOS_DLL_DECL mob_guardianAI : public ScriptedAI
{
    mob_guardianAI(Creature* c) : ScriptedAI(c) {Reset();}

    bool InCombat;    
    
    uint32 tap_timer;

    void Reset()
    {
        tap_timer = 5000;
    }

    void Aggro(Unit* who){}

    void UpdateAI(const uint32 diff) 
    {
        if(!m_creature->SelectHostilTarget())
            return;
            
        if(tap_timer < diff)
        {
            DoCast(m_creature,SPELL_BLOOD_TAP,true);
            tap_timer = 5000;
        }else tap_timer -= diff;
    
        DoMeleeAttackIfReady();
    }
    
};

CreatureAI* GetAI_mob_guardianAI(Creature *_Creature)
{
    return new mob_guardianAI (_Creature);
};

#define SPELL_DARK_BLAST        28458
struct MANGOS_DLL_DECL mob_soldier_frozen_wastesAI : public ScriptedAI
{
    mob_soldier_frozen_wastesAI(Creature* c) : ScriptedAI(c) {Reset();}

    bool InCombat;    

    void Reset()
    {
        DoCast(m_creature,SPELL_DARK_BLAST,true);
    }

    void Aggro(Unit* who){}

    void UpdateAI(const uint32 diff) 
    {
        if(!m_creature->SelectHostilTarget())
            return;
    
        DoMeleeAttackIfReady();
    }
    
};

CreatureAI* GetAI_mob_soldier_frozen_wastesAI(Creature *_Creature)
{
    return new mob_soldier_frozen_wastesAI (_Creature);
};

#define SPELL_ENRAGE                28468
#define SPELL_MORTAL_WOUND          28467
struct MANGOS_DLL_DECL mob_unstoppable_abominationAI : public ScriptedAI
{
    mob_unstoppable_abominationAI(Creature* c) : ScriptedAI(c) {Reset();}

    bool InCombat;
    bool enraged;
    uint32 wound_timer;

    void Reset()
    {
        enraged = false;
        wound_timer = 5000;
    }

    void Aggro(Unit* who){}

    void UpdateAI(const uint32 diff) 
    {
        if(!m_creature->SelectHostilTarget())
            return;
            
        if(wound_timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_MORTAL_WOUND);
            wound_timer = 5000;
        }else wound_timer -= diff;
        
        if(!enraged)
        {
            if((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 40)
            {
                DoCast(m_creature,SPELL_ENRAGE);
                enraged = true;
            }            
        }
    
        DoMeleeAttackIfReady();
    }
    
};

CreatureAI* GetAI_mob_unstoppable_abominationAI(Creature *_Creature)
{
    return new mob_unstoppable_abominationAI (_Creature);
};

#define SPELL_WAIL_OF_SOULS     28460
struct MANGOS_DLL_DECL mob_soul_weaverAI : public ScriptedAI
{
    mob_soul_weaverAI(Creature* c) : ScriptedAI(c) {Reset();}

    bool InCombat;

    uint32 wail_timer;

    void Reset()
    {
        wail_timer = 5000;
    }

    void Aggro(Unit* who){}

    void UpdateAI(const uint32 diff) 
    {
        if(!m_creature->SelectHostilTarget())
            return;
            
        if(wail_timer < diff)
        {
            DoCast(m_creature,SPELL_WAIL_OF_SOULS);
            wail_timer = 5000;
        }else wail_timer -= diff;
          
        DoMeleeAttackIfReady();
    }
    
};

CreatureAI* GetAI_mob_soul_weaverAI(Creature *_Creature)
{
    return new mob_soul_weaverAI (_Creature);
};

#define THE_LICH_KING     16980
struct MANGOS_DLL_DECL mob_lich_kingAI : public ScriptedAI
{
     mob_lich_kingAI(Creature* c) : ScriptedAI(c) {Reset();}

    bool InCombat;

    void Reset(){}

    void Aggro(Unit* who){}

    void UpdateAI(const uint32 diff) 
    {
        if(!m_creature->SelectHostilTarget())
            return;
    }    
};

CreatureAI* GetAI_mob_lich_kingAI(Creature *_Creature)
{
    return new  mob_lich_kingAI (_Creature);
};

void AddSC_boss_kelthuzad()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_kelthuzad";
    newscript->GetAI = GetAI_boss_kelthuzadAI;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name="mob_guardian";
    newscript->GetAI = GetAI_mob_guardianAI;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name="mob_soldier_frozen_wastes";
    newscript->GetAI = GetAI_mob_soldier_frozen_wastesAI;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name="mob_unstoppable_abomination";
    newscript->GetAI = GetAI_mob_unstoppable_abominationAI;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name="mob_soul_weaver";
    newscript->GetAI = GetAI_mob_soul_weaverAI;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name="mob_lich_king";
    newscript->GetAI = GetAI_mob_lich_kingAI;
    newscript->RegisterSelf();
    
   
}
