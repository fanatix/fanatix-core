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
SDName: Boss_Thaddius
SD%Complete: 0
SDComment: Merge Feugen & Stalagg with this script
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "def_naxxramas.h"

//Thaddus
//8873 Lamnt01 - Pleeease!
//8874 Lamnt02 - Stop, make it stop!
//8875 Lamnt03 - Help me! Save me!
//8876 Lamnt04 - Please, nooo!
//8872 greet - You are too late... I... must... OBEY!
//8867 aggro1 - KILL!
//8868 aggro2 - EAT YOUR BONES!
//8869 aggro3 - BREAK YOU!
//8871 elect - Now YOU feel pain!
//8877 slay - You die now!
//8870 die - Thank... you...
#define SAY_THADDIUS_RANDOM_1           "Pleeease!"
#define SOUND_THADDIUS_RANDOM_1         8873
#define SAY_THADDIUS_RANDOM_2           "Stop, make it stop!"
#define SOUND_THADDIUS_RANDOM_2         8874
#define SAY_THADDIUS_RANDOM_3           "Help me! Save me!"
#define SOUND_THADDIUS_RANDOM_3         8875
#define SAY_THADDIUS_RANDOM_4           "Please, nooo!"
#define SOUND_THADDIUS_RANDOM_4         8876

#define SAY_THADDIUS_GREET              "You are too late... I... must... OBEY!"
#define SOUND_THADDIUS_GREET            8872
#define SAY_THADDIUS_AGGRO_1            "KILL!"
#define SOUND_THADDIUS_AGGRO_1          8867
#define SAY_THADDIUS_AGGRO_2            "EAT YOUR BONES!"
#define SOUND_THADDIUS_AGGRO_2          8868
#define SAY_THADDIUS_AGGRO_3            "BREAK YOU!"
#define SOUND_THADDIUS_AGGRO_3          8869
#define SAY_THADDIUS_ELECT              "Now YOU feel pain!"
#define SOUND_THADDIUS_ELECT            8871
#define SAY_THADDIUS_SLAY               "You die now!"
#define SOUND_THADDIUS_SLAY             8877
#define SAY_THADDIUS_DIE                "Thank... you.."
#define SOUND_THADDIUS_DIE              8870


#define SPELL_BALL_LIGHTNING                28299

#define SPELL_CHARGE_POSITIVE_DMGBUFF       29659
#define SPELL_CHARGE_POSITIVE_NEARDMG       28059

#define SPELL_CHARGE_NEGATIVE_DMGBUFF       29660
#define SPELL_CHARGE_NEGATIVE_NEARDMG       28084

#define SPELL_CHAIN_LIGHTNING               28900

#define SPELL_BESERK                        26662



struct MANGOS_DLL_DECL boss_thaddiusAI : public ScriptedAI
{
    boss_thaddiusAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        adds_death = false;
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 enrage_timer;
    uint32 random_timer;
    uint32 ball_timer;
    uint32 chainlightning_timer;
    uint32 polarity_timer;

    bool adds_death;

    bool start;
    bool InCombat;

    void Reset()
    { 
        InCombat = false;
        start = false;
        enrage_timer = 300000;
        random_timer = 20000;
        polarity_timer = 40000;

        ball_timer = 10000;
        chainlightning_timer = 20000;

        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_THADDIUS, 0);

            Unit* temp1 = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_FEUGEN));
            Unit* temp2 = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_STALAGG));
            if(temp1 && temp2)
                start = temp1->isDead() && temp2->isDead();
            if(start)
            {
                HandleDoors(GO_THADDIUS_DOOR, 0);
            }
            else 
            {
                HandleDoors(GO_THADDIUS_DOOR, 1);
            }
        }
        
        if(!adds_death)
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->setFaction(35);
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
                AttackStart(who);
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
                    AttackStart(who);
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

    void KilledUnit(Unit*)
    {
        DoYell(SAY_THADDIUS_SLAY, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_THADDIUS_SLAY);
    }

    void JustDied(Unit* Killer)
    {
        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_THADDIUS, 2);
            HandleDoors(GO_THADDIUS_DOOR, 0);
        }

        DoYell(SAY_THADDIUS_DIE, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_THADDIUS_DIE);
    }

    void Aggro(Unit *who)
    {
        if (!InCombat)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            DoYell(SAY_THADDIUS_GREET, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_THADDIUS_GREET);
            InCombat = true;
            m_creature->setFaction(21);
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
        if (!adds_death)
        {
            if(!start)
            {
                Unit* temp1 = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_FEUGEN));
                Unit* temp2 = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_STALAGG));
                if(temp1 && temp2)
                    start = temp1->isDead() && temp2->isDead();
                else start = true;
            }else
            {
                //DoZoneInCombat();
                adds_death = true;
            }

            if(random_timer < diff)
            {
                switch (rand()%4)
                {
                case 0:
                    DoYell(SAY_THADDIUS_RANDOM_1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_THADDIUS_RANDOM_1);
                    break;
                case 1:
                    DoYell(SAY_THADDIUS_RANDOM_2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_THADDIUS_RANDOM_2);
                    break;
                case 2:
                    DoYell(SAY_THADDIUS_RANDOM_3, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_THADDIUS_RANDOM_3);
                    break;
                case 3: 
                    DoYell(SAY_THADDIUS_RANDOM_4, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_THADDIUS_RANDOM_4);
                    break;

                }
                random_timer = 20000;
            }else random_timer -=diff;
        }
        else
        {
            //Return since we have no target
            if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
                return;

            if(ball_timer < diff)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
                DoCast(target,SPELL_BALL_LIGHTNING);
                ball_timer = 20000;
            }else ball_timer -= diff;

            if(chainlightning_timer < diff)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
                DoCast(target,SPELL_CHAIN_LIGHTNING);
                chainlightning_timer = 35000;
            }else chainlightning_timer -= diff;

            if(polarity_timer < diff)
            {
                // workaround
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
                switch (rand()%2)
                {
                case 0:
                        target->CastSpell(target,SPELL_CHARGE_NEGATIVE_NEARDMG,true);
                    break;
                case 1:
                        target->CastSpell(target,SPELL_CHARGE_POSITIVE_NEARDMG,true);
                    break;
                }                
                 polarity_timer = 15000;
            }else polarity_timer -= diff;

            if(random_timer < diff)
            {
                if(rand()%3 == 1)
                    switch (rand()%3)
                    {
                    case 0:
                        DoYell(SAY_THADDIUS_AGGRO_1, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature, SOUND_THADDIUS_AGGRO_1);
                        break;
                    case 1:
                        DoYell(SAY_THADDIUS_AGGRO_2, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature, SOUND_THADDIUS_AGGRO_2);
                        break;
                    case 2:
                        DoYell(SAY_THADDIUS_AGGRO_3, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature, SOUND_THADDIUS_AGGRO_3);
                        break;
                    }
                random_timer = 20000;
            }else random_timer -=diff;

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_boss_thaddius(Creature *_Creature)
{
    return new boss_thaddiusAI (_Creature);
}

//Feugen
#define SAY_FEUGEN_AGGRO       "Feed you to master!"
#define SOUND_FEUGEN_AGGRO     8802
#define SAY_FEUGEN_SLAY        "Feugen make master happy!"
#define SOUND_FEUGEN_SLAY      8804
#define SAY_FEUGEN_DIE         "No... more... Feugen..."
#define SOUND_FEUGEN_DIE       8803

#define SPELL_WARSTOMP          28125
#define SPELL_MANABURN          28135
//#define SPELL_CHAIN_LIGHTNING 28900

struct MANGOS_DLL_DECL mob_feugenAI : public ScriptedAI
{
    mob_feugenAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }

    ScriptedInstance *pInstance;

    bool InCombat;
        
    uint32 manaburn_timer;
    uint32 warstomp_timer;
    uint32 chain_timer;

    uint32 initx,inity,initz;

    void Reset()
    {       
        InCombat = false;

        manaburn_timer = 20000;
        warstomp_timer = 10000;
        chain_timer = 1000;

        initx = 0;
        inity = 0;
        initz = 0;

        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_THADDIUS, 0);
            HandleDoors(GO_THADDIUS_DOOR, 0);
        }

        Unit* Temp =  Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_STALAGG));
            if (Temp && Temp->getVictim())
                m_creature->getThreatManager().addThreat(Temp->getVictim(),0.0f);
    }

    void KilledUnit(Unit*)
    {
        DoYell(SAY_FEUGEN_SLAY, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_FEUGEN_SLAY);
    }

    void JustDied(Unit* Killer)
    {
        DoYell(SAY_FEUGEN_DIE, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_FEUGEN_DIE);
        Unit* Temp =  Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_STALAGG));
        Unit* Thadd = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_THADDIUS));
        if (Temp && Thadd && Temp->isDead())
            Thadd->getThreatManager().addThreat(Killer,0.0f);

    }

    bool innerhalb(uint32 x, uint32 y, uint32 z,uint32 abstand)
    {
        bool temp;
        temp = true;
        if(m_creature->GetPositionX() > x+abstand || m_creature->GetPositionX() < x-abstand)
           temp = false;
        if(m_creature->GetPositionY() > y+abstand || m_creature->GetPositionY() < y-abstand)
           temp = false;
        if(m_creature->GetPositionZ() > z+abstand || m_creature->GetPositionZ() < z-abstand)
           temp = false;
        return temp;
    }


    void Aggro(Unit *who)
    {
        initx = m_creature->GetPositionX();
        inity = m_creature->GetPositionY();
        initz = m_creature->GetPositionZ();

        if (!InCombat)
        {
            DoZoneInCombat();
            if(pInstance)
            {
                pInstance->SetData(ENCOUNT_THADDIUS, 1);
                HandleDoors(GO_THADDIUS_DOOR, 1);
            }
            InCombat = true;
            DoYell(SAY_FEUGEN_AGGRO, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_FEUGEN_AGGRO);

            Unit* Temp =  Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_STALAGG));
            if (Temp)
                Temp->getThreatManager().addThreat(who,0.0f);
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

        if(!innerhalb(initx, inity, initz,10))
            if(chain_timer < diff)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
                DoCast(target,SPELL_CHAIN_LIGHTNING,true);
                chain_timer = 1000;
            }else chain_timer -= diff;

        if(warstomp_timer < diff)
        {
            DoCast(m_creature,SPELL_WARSTOMP);
            warstomp_timer = 20000;
        }else warstomp_timer -= diff;

        if(manaburn_timer < diff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
            DoCast(target,SPELL_MANABURN);
            manaburn_timer = 20000;
        }else manaburn_timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_mob_feugen(Creature *_Creature)
{
    return new mob_feugenAI (_Creature);
}

//Stalagg
#define SAY_STALAGG_AGGRO       "Stalagg crush you!"
#define SOUND_STALAGG_AGGRO     8864
#define SAY_STALAGG_SLAY        "Stalagg Kill!"
#define SOUND_STALAGG_SLAY      8866
#define SAY_STALAGG_DIE         "Master save me..."
#define SOUND_STALAGG_DIE       8865

#define SPELL_WARSTOMP          28125
#define SPELL_POWERSURGE        28134
//#define SPELL_CHAIN_LIGHTNING 28900

struct MANGOS_DLL_DECL mob_stalaggAI : public ScriptedAI
{
    mob_stalaggAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }

    ScriptedInstance *pInstance;

    bool InCombat;

    uint32 warstomp_timer;
    uint32 powersurge_timer;
    uint32 chain_timer;

    uint32 initx,inity,initz;

    void Reset()
    {  
        InCombat = false;
        warstomp_timer = 10000;
        powersurge_timer = 20000;
        chain_timer = 1000;

        initx = 0;
        inity = 0;
        initz = 0;

        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_THADDIUS, 0);
            HandleDoors(GO_THADDIUS_DOOR, 0);
        }

        Unit* Temp =  Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_FEUGEN));
            if (Temp && Temp->getVictim())
                m_creature->getThreatManager().addThreat(Temp->getVictim(),0.0f);
    }

    bool innerhalb(uint32 x, uint32 y, uint32 z,uint32 abstand)
    {
        bool temp;
        temp = true;
        if(m_creature->GetPositionX() > x+abstand || m_creature->GetPositionX() < x-abstand)
           temp = false;
        if(m_creature->GetPositionY() > y+abstand || m_creature->GetPositionY() < y-abstand)
           temp = false;
        if(m_creature->GetPositionZ() > z+abstand || m_creature->GetPositionZ() < z-abstand)
           temp = false;
        return temp;
    }


    void KilledUnit(Unit*)
    {
        DoYell(SAY_STALAGG_SLAY, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_STALAGG_SLAY);
    }

    void JustDied(Unit* Killer)
    {
        DoYell(SAY_STALAGG_DIE, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_STALAGG_DIE);

        Unit* Temp =  Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_FEUGEN));
        Unit* Thadd = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_THADDIUS));
        if (Temp && Thadd && Temp->isDead())
            Thadd->getThreatManager().addThreat(Killer,0.0f);
    }

    void Aggro(Unit *who)
    {
      
        if (!InCombat)
        {
            DoZoneInCombat();
            if(pInstance)
            {
                pInstance->SetData(ENCOUNT_THADDIUS, 1);
                HandleDoors(GO_THADDIUS_DOOR, 1);
            }
            InCombat = true;
            DoYell(SAY_STALAGG_AGGRO, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_STALAGG_AGGRO);

            Unit* Temp =  Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_FEUGEN));
            if (Temp)
                Temp->getThreatManager().addThreat(who,0.0f);
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

        if(!innerhalb(initx, inity, initz,10))
            if(chain_timer < diff)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
                DoCast(target,SPELL_CHAIN_LIGHTNING,true);
                chain_timer = 1000;
            }else chain_timer -= diff;

        if(warstomp_timer < diff)
        {
            DoCast(m_creature,SPELL_WARSTOMP);
            warstomp_timer = 20000;
        }else warstomp_timer -= diff;

        if(powersurge_timer < diff)
        {
            DoCast(m_creature,SPELL_POWERSURGE);
            powersurge_timer = 20000;
        }else powersurge_timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_mob_stalagg(Creature *_Creature)
{
    return new mob_stalaggAI (_Creature);
}

void AddSC_boss_thaddius()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_thaddius";
    newscript->GetAI = GetAI_boss_thaddius;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_feugen";
    newscript->GetAI = GetAI_mob_feugen;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_stalagg";
    newscript->GetAI = GetAI_mob_stalagg;
    newscript->RegisterSelf();
}
