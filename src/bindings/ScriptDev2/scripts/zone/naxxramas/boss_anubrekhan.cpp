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
SDName: Boss_Anubrekhan
SD%Complete: 100
SDComment: 
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "def_naxxramas.h"

#define SAY_AGGRO1  "Just a little taste..."
#define SAY_AGGRO2  "There is no way out."
#define SAY_AGGRO3  "Yes, Run! It makes the blood pump faster!"
#define SAY_GREET   "Ahh... welcome to my parlor"
#define SAY_TAUNT1  "I hear little hearts beating. Yesss... beating faster now. Soon the beating will stop."
#define SAY_TAUNT2  "Where to go? What to do? So many choices that all end in pain, end in death."
#define SAY_TAUNT3  "Which one shall I eat first? So difficult to choose... the all smell so delicious."
#define SAY_TAUNT4  "Closer now... tasty morsels. I've been too long without food. Without blood to drink."
#define SAY_SLAY    "Shh... it will all be over soon."

#define SOUND_AGGRO1  8785
#define SOUND_AGGRO2  8786
#define SOUND_AGGRO3  8787
#define SOUND_GREET   8788
#define SOUND_TAUNT1  8790
#define SOUND_TAUNT2  8791
#define SOUND_TAUNT3  8792
#define SOUND_TAUNT4  8793
#define SOUND_SLAY    8789

#define SPELL_IMPALE        28783   
#define SPELL_LOCUSTSWARM   28785   //This is a self buff that triggers the dmg debuff
#define MOB_CRYPT_GARD      16573

#define SPELL_SELF_SPAWN_5  29105   //This spawns 5 corpse scarabs ontop of us (most likely the player casts this on death)
#define SPELL_SELF_SPAWN_10 28864   //This is used by the crypt guards when they die

struct MANGOS_DLL_DECL boss_anubrekhanAI : public ScriptedAI
{
    boss_anubrekhanAI(Creature *c) : ScriptedAI(c) 
	{
		pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
		Reset();
		HasTaunted = false;
	}

	ScriptedInstance *pInstance;

    uint32 Impale_Timer;
    uint32 LocustSwarm_Timer;
    uint32 Summon_Timer;
    bool swarm;
    bool InCombat;
    bool HasTaunted;

    void Reset()
    {
        Impale_Timer = 15000;                       //15 seconds
        LocustSwarm_Timer = 90000; 
        Summon_Timer = LocustSwarm_Timer + 45000;   //45 seconds after initial locust swarm
        InCombat = false;
        swarm = false;

        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_ANUBREKHAN, 0);
            HandleDoors(GO_ANUBREKHAN_DOOR, 0);
            HandleDoors(GO_ANUBREKHAN_GATE, 1);
            HandleDoors(GO_MAEXXNA_OUTERWEB, 1);
        }
    }

    void JustDied(Unit*)
    {
        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_ANUBREKHAN, 2);
            HandleDoors(GO_ANUBREKHAN_DOOR, 0);
            HandleDoors(GO_ANUBREKHAN_GATE, 0);
            Unit *temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_FAERLINA));
            if (temp && temp->isDead())
                HandleDoors(GO_MAEXXNA_OUTERWEB, 0);
        }
    }

    void KilledUnit(Unit* Victim)
    {
        //Force the player to spawn corpse scarabs via spell
        //Else its 
        if (Victim->GetTypeId() == TYPEID_PLAYER)
        {
            Victim->CastSpell(Victim, SPELL_SELF_SPAWN_5, true);
        }       

        if (rand()%2)
            return;

        DoYell(SAY_SLAY, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_SLAY);
    }

    void Aggro(Unit *who)
    {
        DoZoneInCombat();
        switch(rand()%3)
        {
        case 0:
            DoYell(SAY_AGGRO1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_AGGRO1);
            break;

        case 1:
            DoYell(SAY_AGGRO2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_AGGRO2);
        break;

        case 2:
            DoYell(SAY_AGGRO3, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_AGGRO3);
            break;
        }

        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_ANUBREKHAN, 1);
            HandleDoors(GO_ANUBREKHAN_DOOR, 1);
        }
        
     }

    //Extended Visbility range for taunts
    bool IsVisible(Unit *who) const
    {
        if (!who)
            return false;

        return m_creature->IsWithinDistInMap(who, 100.0f) && who->isVisibleForOrDetect(m_creature,true);
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                AttackStart(who);
                Aggro(who);
            }
            else if (!HasTaunted && m_creature->IsWithinDistInMap(who, 60.0f))
            {
                switch(rand()%5)
                {
                case 0:
                    DoYell(SAY_TAUNT1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_TAUNT1);
                    break;

                case 1:
                    DoYell(SAY_TAUNT2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_TAUNT2);
                    break;

                case 2:
                    DoYell(SAY_TAUNT3, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_TAUNT3);
                    break;

                case 3:
                    DoYell(SAY_TAUNT4, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_TAUNT4);
                    break;

                case 4:
                    DoYell(SAY_GREET, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_GREET);
                    break;
                }
                HasTaunted = true;
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
        
        if(!swarm)
        {
            //Impale_Timer
            if (Impale_Timer < diff)
            {
                //Cast Impale on a random target            
                Unit* target = NULL;

                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target) DoCast(target,SPELL_IMPALE,true);

                //15 seconds until we should cast this agian
                //Impale_Timer = 15000;
                Impale_Timer = 35000;
            }else Impale_Timer -= diff;   
       
            //LocustSwarm_Timer
            if (LocustSwarm_Timer < diff)
            {
                //Cast Locust Swarm buff on ourselves
                DoCast(m_creature, SPELL_LOCUSTSWARM);
                swarm = true;
                Creature* temp = DoSpawnCreature(16573,0,0,0,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,120000);
                temp->AddThreat(m_creature->getVictim(),0.0f);
                temp = DoSpawnCreature(16573,0,0,0,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,120000);
                temp->AddThreat(m_creature->getVictim(),0.0f);
                //90 seconds until we should cast this agian
                //LocustSwarm_Timer = 90000;
                LocustSwarm_Timer = 20000;
            }else LocustSwarm_Timer -= diff;
        }
        else
        {
            if (LocustSwarm_Timer < diff)
            {            
                swarm = false;
                LocustSwarm_Timer = 65000;
            }else LocustSwarm_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_anubrekhan(Creature *_Creature)
{
    return new boss_anubrekhanAI (_Creature);
};

#define SPELL_CG_ACID_SPIT  28969
#define SPELL_CG_GLEVE      40504
#define SPELL_CG_WEB        28991

struct MANGOS_DLL_DECL mob_crypt_guardAI : public ScriptedAI
{
    mob_crypt_guardAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }

    ScriptedInstance *pInstance;

    bool InCombat;
        
    uint32 spit_timer;
    uint32 gleve_timer;
    uint32 web_timer;

    void Reset()
    {       
        InCombat = false;

        spit_timer = 20000;
        gleve_timer = 10000;
        web_timer = 1000;

    }

     void JustDied(Unit* Killer)
    {
        if(pInstance)
        {
            Unit *temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_ANUBREKHAN));
            if(temp && m_creature->IsWithinDistInMap(temp,30))
                DoCast(m_creature,SPELL_SELF_SPAWN_10,true);
            if(!temp)
                DoCast(m_creature,SPELL_SELF_SPAWN_10,true);
        }
    }

    void Aggro(Unit *who){}

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(spit_timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target) DoCast(target,SPELL_CG_ACID_SPIT);
            spit_timer = 20000;
        }else spit_timer -= diff;

        if(gleve_timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_CG_GLEVE);
            gleve_timer = 20000;
        }else gleve_timer -= diff;

        if(web_timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target) DoCast(m_creature,SPELL_CG_WEB);
            web_timer = 20000;
        }else web_timer -= diff;

 

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_mob_crypt_guard(Creature *_Creature)
{
    return new mob_crypt_guardAI (_Creature);
}




void AddSC_boss_anubrekhan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_anubrekhan";
    newscript->GetAI = &GetAI_boss_anubrekhan;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_crypt_guard";
    newscript->GetAI = &GetAI_mob_crypt_guard;
    newscript->RegisterSelf();
}
