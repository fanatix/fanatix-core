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
SDName: Boss_Gothik
SD%Complete: 0
SDComment: Placeholder
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "def_naxxramas.h"

#define SAY_START                   "Foolishly you have sought your own demise. Brazenly you have disregarded powers beyond your understanding. You have fought hard to invade the realm of the harvester. Now there is only one way out - to walk the lonely path of the damned."
#define SOUND_START                 8807
#define SAY_TELEPORT                "I have waited long enough! Now, you face the harvester of souls!"
#define SOUND_TELEPORT              8808
#define SAY_SLAY                    "Death is the only escape."
#define SOUND_SLAY                  8806
#define SAY_DEATH                   "I... am... undone!"
#define SOUND_DEATH                 8805

//Gothik
#define SPELL_HARVESTSOUL           28679
#define SPELL_SHADOWBOLT            19729

//Unrelenting Trainee 16124 --> Event AI
#define MOB_UNRELENTING_TRAINEE     16124
#define SPELL_EAGLECLAW             30285
#define SPELL_KNOCKDOWN_PASSIVE     6961

//Unrelenting Deathknight 16125
#define MOB_UNRELENTING_DEATHKNIGHT 16125
#define SPELL_CHARGE                22120
#define SPELL_SHADOW_MARK           27825

//Unrelenting Rider 16126
#define MOB_UNRELENTING_RIDER       16126
#define SPELL_UNHOLY_AURA           27987
#define SPELL_SHADOWBOLT            19729        //Search thru targets and find those who have the SHADOW_MARK to cast this on

//Spectral Trainee  --> Event AI
#define MOB_SPECTRAL_TRAINEE        16127                 
#define SPELL_ARCANE_EXPLOSION      27989

/*
insert into `eventai_scripts` (`id`,`creature_id`,`event_type`,`event_param1`,`event_param2`,`event_param3`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`)
values
(NULL,16127, 0,7500,0,0, 11,27989,1,0, 0,0,0,0 ,0,0,0,0);
*/

//Spectral Deathknight --> Event AI
#define MOB_SPECTRAL_DEATHKNIGHT    16148       
#define SPELL_WHIRLWIND             28334
#define SPELL_SUNDER_ARMOR          25051 //cannot find sunder that reduces armor by 2950
#define SPELL_CLEAVE                20677
#define SPELL_MANA_BURN             17631

/*
insert into `eventai_scripts` (`id`,`creature_id`,`event_type`,`event_param1`,`event_param2`,`event_param3`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`)
values
(NULL,16148, 0,15000,0,0, 11,28334,0,1, 0,0,0,0 ,0,0,0,0),
(NULL,16148, 0,5000,0,0, 11,25051,4,0, 0,0,0,0 ,0,0,0,0),
(NULL,16148, 0,7500,0,0, 11,20677,1,0, 0,0,0,0 ,0,0,0,0),
(NULL,16148, 0,7000,0,0, 11,17631,5,0, 0,0,0,0 ,0,0,0,0);
*/

//Spectral Rider --> Try Event AI
#define MOB_SPECTRAL_RIDER          16150
#define SPELL_LIFEDRAIN             24300
//USES SAME UNHOLY AURA AS UNRELENTING RIDER

/*
insert into `eventai_scripts` (`id`,`creature_id`,`event_type`,`event_param1`,`event_param2`,`event_param3`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`)
values
(NULL,16150, 0,10000,0,0, 11,24300,4,0, 0,0,0,0 ,0,0,0,0),
(NULL,16150, 1,1000,0,0, 11,27987,0,0, 0,0,0,0 ,0,0,0,0);
*/

//Spectral Horse --> Event AI
#define MOB_SPECTRAL_HORSE          16149
#define SPELL_STOMP                 27993

/*
insert into `eventai_scripts` (`id`,`creature_id`,`event_type`,`event_param1`,`event_param2`,`event_param3`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`)
values
(NULL,16149, 0,7500,0,0, 11,27993,1,0, 0,0,0,0 ,0,0,0,0);
*/

float lifeside_spawn[3][3] = 
{
    {2714.49, -3429.49, 269},
    {2692.22, -3429.49, 269},
    {2669.90, -3429.49, 269}
};

float deadside_spawn[5][3] = 
{
    {2665.31, -3340.27, 269},
    {2682.65, -3304.45, 269},
    {2699.89, -3322.92, 269},
    {2725.91, -3339.97, 269},
    {2733.40, -3349.32, 269}
};

float teleport_spots[2][3] = 
{
    {2691.11, -3317.14, 269},
    {2691.42, -3400.62, 269}
};

struct MANGOS_DLL_DECL boss_gothikAI : public ScriptedAI
{
    boss_gothikAI(Creature *c) : ScriptedAI(c)
    {  
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }

    ScriptedInstance *pInstance;
	
    uint32 phase1end_timer;
    uint32 harvestsouls_timer;
    uint32 shadowbolt_timer;
    uint32 teleport_timer;
    uint32 phase;
    uint32 pos;

    uint32 spawn_trainee;
    uint32 spawn_deathknight;
    uint32 spawn_rider; 

    bool InCombat;

    void JustDied(Unit* Killer)
    {
		DoYell(SAY_DEATH,LANG_UNIVERSAL,NULL);
		DoPlaySoundToSet(m_creature,SOUND_DEATH);
		
		if(pInstance)
		{
			pInstance->SetData(ENCOUNT_GOTHIK, 2);
			HandleDoors(GO_VACCUUM_ENTER, 0);
			HandleDoors(GO_VACCUUM_EXIT, 0);
			HandleDoors(GO_VACCUUM_COMBAT, 0);
			Unit *temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_RAZUVIOUS));
			if(temp && temp->isDead())
				HandleDoors(GO_DEATHKNIGHT_DOOR, 0);
		}
    }

    void KilledUnit()
    {
        DoYell(SAY_SLAY,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_SLAY);
    }

    void Reset()
    {
        m_creature->Relocate(2642.2,-3388.39,285.6); 
       (*m_creature).GetMotionMaster()->MoveTargetedHome();
        InCombat = false;
        phase1end_timer = 24000; 
        teleport_timer = 20000;//test
        harvestsouls_timer = 5000;//test
        shadowbolt_timer = 7500;//test
        phase = -1;    
        pos = -1; // teleport possition .. -1 Start , 0 life , 1 death
        spawn_trainee = 27000;
        spawn_deathknight = 77000;
        spawn_rider = 137000; 	

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		if(pInstance)
		{
			pInstance->SetData(ENCOUNT_GOTHIK, 0);
			HandleDoors(GO_VACCUUM_EXIT, 1);
			HandleDoors(GO_VACCUUM_COMBAT, 0);
			HandleDoors(GO_DEATHKNIGHT_DOOR, 1);
            Unit *temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_RAZUVIOUS));
			if(temp && temp->isDead())
			    HandleDoors(GO_VACCUUM_ENTER, 0);
		}
    }
	
	void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
                AttackStart(who);
                Aggro(who);
        }
    }
	
	void MoveInLineOfSight(Unit *who){}

    void Aggro(Unit *who)
    {
        if (!InCombat)
        {
            DoZoneInCombat();
            InCombat = true;
            DoYell(SAY_START,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_START);
            phase = 0; 
			phase1end_timer = 24000;  
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            HandleDoors(GO_VACCUUM_ENTER, 1);
            HandleDoors(GO_VACCUUM_COMBAT, 1);
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
        if(phase == 0)
        {
            if(phase1end_timer < diff)
            {
                switch(rand()%2)
                {
                case 0:
                    DoYell("Foolishly you have sought your own demise.",LANG_UNIVERSAL,NULL);
                    break;
                case 1:
                    DoYell("Teamanare shi rikk mannor rikk lok karkun",LANG_UNIVERSAL,NULL);
                    break;
                }
                phase = 1;
                phase1end_timer = 274000;
                if(pInstance)
			    {
				    pInstance->SetData(ENCOUNT_GOTHIK, 1);				    
				    HandleDoors(GO_VACCUUM_EXIT, 1);				    
			    }  

            }else phase1end_timer -=diff;

        }

        if(phase == 1)
        {
            Unit* target; 
            Creature* summon;
            if(phase1end_timer < diff)
            {
                DoYell(SAY_TELEPORT,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_TELEPORT);
                phase = 2;
                pos = 0;
                //teleport to life side
                m_creature->Relocate(teleport_spots[pos][0],teleport_spots[pos][1],teleport_spots[pos][2]);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				HandleDoors(GO_VACCUUM_COMBAT, 0);
            }else
            {   
                if (!m_creature->getVictim())
                    m_creature->AI()->EnterEvadeMode();
                phase1end_timer -=diff;
            }
              
            if(spawn_trainee < diff)
            {
                
                for(int i=0;i<=2;i++)
                {
                    summon = m_creature->SummonCreature(MOB_UNRELENTING_TRAINEE,lifeside_spawn[i][0],lifeside_spawn[i][1],lifeside_spawn[i][2],0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,15000);                                        
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    summon->getThreatManager().addThreat(target,0.0f);
                    
                }
                spawn_trainee = 20000;
            }else spawn_trainee -=diff;

            if(spawn_deathknight < diff)
            {
                summon = m_creature->SummonCreature(MOB_UNRELENTING_DEATHKNIGHT,lifeside_spawn[0][0],lifeside_spawn[0][1],lifeside_spawn[0][2],0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,15000);
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                summon->getThreatManager().addThreat(target,0.0f);
                summon = m_creature->SummonCreature(MOB_UNRELENTING_DEATHKNIGHT,lifeside_spawn[2][0],lifeside_spawn[2][1],lifeside_spawn[2][2],0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,15000);
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                summon->getThreatManager().addThreat(target,0.0f);
                spawn_deathknight = 25000;
            }else spawn_deathknight -=diff;

            if(spawn_rider < diff)
            {
                summon = m_creature->SummonCreature(MOB_UNRELENTING_RIDER,lifeside_spawn[1][0],lifeside_spawn[1][1],lifeside_spawn[1][2],0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,15000);   
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                summon->getThreatManager().addThreat(target,0.0f);
                spawn_rider = 30000;
            } else spawn_rider -=diff;
        }

        if(phase == 2)
        {
            if (m_creature->getVictim())
            {
                if(teleport_timer < diff)
                {
                    if(pos == 1)
                        pos = 0;
                    else pos = 1;
                    m_creature->Relocate(teleport_spots[pos][0],teleport_spots[pos][1],teleport_spots[pos][2]);
                    teleport_timer = 20000;
                }else 
                {   
                    if (!m_creature->getVictim())
                        m_creature->AI()->EnterEvadeMode();
                    teleport_timer  -= diff;
                }
         
                if(harvestsouls_timer < diff) 
                {          
                    Unit* target = NULL;

                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    if (target)DoCast(target,SPELL_HARVESTSOUL);
                    harvestsouls_timer = 5000;

                }else harvestsouls_timer -= diff;

                if(shadowbolt_timer < diff) 
                {          
                    Unit* target = NULL;

                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    if (target)DoCast(target,SPELL_SHADOWBOLT);
                    shadowbolt_timer = 3000;
                }else shadowbolt_timer -= diff;        
            }

        };
    }   
};

CreatureAI* GetAI_boss_gothik(Creature *_Creature)
{
    return new boss_gothikAI (_Creature);
};

struct MANGOS_DLL_DECL mob_u_traineeAI : public ScriptedAI
{
    mob_u_traineeAI(Creature *c) : ScriptedAI(c)
    {  
        Reset();
    }

    uint32 knockdown_timer;
    uint32 eagleclaw_timer;

    void Reset()
    {
        knockdown_timer = 5000;
        eagleclaw_timer = 7500;
    }

    void Aggro(Unit *who)
    {
    }

    void JustDied(Unit* Killer)
    {
        int r;
        r = rand()%4;
        Creature* temp = m_creature->SummonCreature(MOB_SPECTRAL_TRAINEE,deadside_spawn[r][0],deadside_spawn[r][1],deadside_spawn[r][2],0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,15000);
        temp->getThreatManager().addThreat(Killer,0.0f);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (knockdown_timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_KNOCKDOWN_PASSIVE);
            knockdown_timer = 5000;            
        }else knockdown_timer -= diff;

        if (eagleclaw_timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_EAGLECLAW);
            eagleclaw_timer = 5000;            
        }else eagleclaw_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_u_trainee(Creature *_Creature)
{
    return new mob_u_traineeAI (_Creature);
};


struct MANGOS_DLL_DECL mob_u_deathknightAI : public ScriptedAI
{
    mob_u_deathknightAI(Creature *c) : ScriptedAI(c)
    {  
        Reset();
    }

     uint32 charge_timer;
     uint32 shadowmark_timer;

    void Reset()
    {
        charge_timer = 7600;
        shadowmark_timer = 5000;
    }

    void Aggro(Unit *who)
    {
    }

    void JustDied(Unit* Killer)
    {
        int r;
        r = rand()%4;
        Creature* temp = m_creature->SummonCreature(MOB_SPECTRAL_DEATHKNIGHT,deadside_spawn[r][0],deadside_spawn[r][1],deadside_spawn[r][2],0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,15000);
        temp->getThreatManager().addThreat(Killer,0.0f);
        temp = m_creature->SummonCreature(MOB_SPECTRAL_HORSE,deadside_spawn[r][0],deadside_spawn[r][1],deadside_spawn[r][2],0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,15000);
        temp->getThreatManager().addThreat(Killer,0.0f);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (charge_timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target) DoCast(target,SPELL_CHARGE);
            charge_timer = 7000;            
        }else charge_timer -= diff;

        if (shadowmark_timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target) DoCast(target,SPELL_SHADOW_MARK);
            shadowmark_timer = 5000;            
        }else shadowmark_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_u_deathknight(Creature *_Creature)
{
    return new mob_u_deathknightAI (_Creature);
};



struct MANGOS_DLL_DECL mob_u_riderAI : public ScriptedAI
{
    mob_u_riderAI(Creature *c) : ScriptedAI(c)
    {  
        Reset();
    }

    bool unholy_aura;
    uint32 aura_check;
    uint32 shadowbolt_timer;

    void Reset()
    {
        unholy_aura = false;
        aura_check = 0;
        shadowbolt_timer = 15000;
    }

    void Aggro(Unit *who)
    {
    }

    void JustDied(Unit* Killer)
    {
        int r;
        r = rand()%4;
        Creature* temp = m_creature->SummonCreature(MOB_SPECTRAL_RIDER,deadside_spawn[r][0],deadside_spawn[r][1],deadside_spawn[r][2],0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,15000);
        temp->getThreatManager().addThreat(Killer,0.0f);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (aura_check < diff)
        {
            if(!(m_creature->HasAura(SPELL_UNHOLY_AURA,0)))
                DoCast(m_creature,SPELL_UNHOLY_AURA);
            aura_check = 10000;            
        }else aura_check -= diff;

        if (shadowbolt_timer < diff)
        {
            
            Unit* Temp = NULL; 
            std::list<HostilReference*>::iterator i = m_creature->getThreatManager().getThreatList().begin();
            for (; i != m_creature->getThreatManager().getThreatList().end(); ++i)
            {
                Temp = Unit::GetUnit((*m_creature),(*i)->getUnitGuid());
                if(Temp && Temp->HasAura(SPELL_SHADOW_MARK,0))
                    DoCast(Temp,SPELL_SHADOWBOLT);
            };    
            shadowbolt_timer = 10000;                    
        }else shadowbolt_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_u_rider(Creature *_Creature)
{
    return new mob_u_riderAI (_Creature);
};

void AddSC_boss_gothik()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_gothik"; // update creature_template set ScriptName = 'boss_gothik' where entry = 16060;
    newscript->GetAI = GetAI_boss_gothik;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_u_trainee";
    newscript->GetAI = GetAI_mob_u_trainee;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_u_deathknight";
    newscript->GetAI = GetAI_mob_u_deathknight;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_u_rider";
    newscript->GetAI = GetAI_mob_u_rider;
    newscript->RegisterSelf();
}