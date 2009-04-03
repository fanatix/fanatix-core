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
SDName: Boss_Thane_Korthazz
SD%Complete: 100
SDComment: 
EndScriptData */

#include "precompiled.h"
#include "def_naxxramas.h"

//All horsemen
#define SPELL_SHIELDWALL           29061
#define SPELL_BESERK               26662

// thane korthazz
#define SPELL_MARK_OF_KORTHAZZ     28832
#define SPELL_METEOR               26558 // m_creature->getVictim() auto-area spell but with a core problem

#define SAY_KORTHAZZ_AGGRO                  "Come out and fight, ye wee ninny!"
#define SAY_KORTHAZZ_TAUNT1                 "To arms, ye roustabouts! We've got company!"
#define SAY_KORTHAZZ_TAUNT2                 "I heard about enough of yer sniveling. Shut yer fly trap 'afore I shut it for ye!"
#define SAY_KORTHAZZ_TAUNT3                 "I'm gonna enjoy killin' these slack-jawed daffodils!"
#define SAY_KORTHAZZ_SLAY                   "Next time, bring more friends!"
#define SAY_KORTHAZZ_SPECIAl                "I like my meat extra crispy!"
#define SAY_KORTHAZZ_DEATH                  "What a bloody waste this is!"

#define SOUND_KORTHAZZ_AGGRO                8899
#define SOUND_KORTHAZZ_TAUNT1               8903
#define SOUND_KORTHAZZ_TAUNT2               8904
#define SOUND_KORTHAZZ_TAUNT3               8905
#define SOUND_KORTHAZZ_SLAY                 8901
#define SOUND_KORTHAZZ_SPECIAL              8902
#define SOUND_KORTHAZZ_DEATH                8900

#define SPIRIT_OF_KORTHAZZ                  16778

// sir zeliek
#define SPELL_MARK_OF_ZELIEK                28835
#define SPELL_HOLY_WRATH                    28883

#define SAY_ZELIEK_AGGRO                    "Flee, before it's too late!"
#define SAY_ZELIEK_TAUNT1                   "Invaders, cease this foolish venture at once! Turn away while you still can!"
#define SAY_ZELIEK_TAUNT2                   "Perhaps they will come to their senses, and run away as fast as they can!"
#define SAY_ZELIEK_TAUNT3                   "Do not continue! Turn back while there's still time!"
#define SAY_ZELIEK_SPECIAL                  "I- I have no choice but to obey!"
#define SAY_ZELIEK_SLAY                     "Forgive me!"
#define SAY_ZELIEK_DEATH                    "It is... as it should be."

#define SOUND_ZELIEK_AGGRO                  8913
#define SOUND_ZELIEK_TAUNT1                 8917
#define SOUND_ZELIEK_TAUNT2                 8918
#define SOUND_ZELIEK_TAUNT3                 8919
#define SOUND_ZELIEK_SPECIAl                8916
#define SOUND_ZELIEK_SLAY                   8915
#define SOUND_ZELIEK_DEATH                  8914

// lady blaumeux
#define SPELL_MARK_OF_BLAUMEUX              28833
#define SPELL_VOIDZONE                      28863

#define SAY_BLAUMEUX_AGGRO                  "Defend youself!"
#define SAY_BLAUMEUX_TAUNT1                 "Come, Zeliek, do not drive them out. Not before we've had our fun."
#define SAY_BLAUMEUX_TAUNT2                 "I do hope they stay alive long enough for me to... introduce myself."
#define SAY_BLAUMEUX_TAUNT3                 "The first kill goes to me! Anyone care to wager?"
#define SAY_BLAUMEUX_SPECIAL                "Your life is mine!"
#define SAY_BLAUMEUX_SLAY                   "Who's next?"
#define SAY_BLAUMEUX_DEATH                  "Tou... che!"

#define SOUND_BLAUMEUX_AGGRO                8892
#define SOUND_BLAUMEUX_TAUNT1               8896
#define SOUND_BLAUMEUX_TAUNT2               8897
#define SOUND_BLAUMEUX_TAUNT3               8898
#define SOUND_BLAUMEUX_SPECIAL              8895
#define SOUND_BLAUMEUX_SLAY                 8894
#define SOUND_BLAUMEUX_DEATH                8893

#define SPIRIT_OF_BLAUMEUX                  16776

// highlord mograine
#define SPELL_MARK_OF_MOGRAINE              28834
#define SPELL_RIGHTEOUS_FIRE                28882 // Applied as a 25% chance on melee hit to proc. m_creature->GetVictim()

#define SAY_MOGRAINE_TAUNT1                 "Enough prattling. Let them come! We shall grind their bones to dust."
#define SAY_MOGRAINE_TAUNT2                 "Conserve your anger! Harness your rage! You will all have outlets for your frustration soon enough."
#define SAY_MOGRAINE_TAUNT3                 "Life is meaningless. It is in death that we are truly tested."
#define SAY_MOGRAINE_AGGRO1                 "You seek death?"
#define SAY_MOGRAINE_AGGRO2                 "None shall pass!"
#define SAY_MOGRAINE_AGGRO3                 "Be still!"
#define SAY_MOGRAINE_SLAY1                  "You will find no peace in death."
#define SAY_MOGRAINE_SLAY2                  "The master's will is done."
#define SAY_MOGRAINE_SPECIAL                "Bow to the might of the Highlord!"
#define SAY_MOGRAINE_DEATH                  "I... am... released! Perhaps it's not too late to - noo! I need... more time..."

#define SOUND_MOGRAINE_TAUNT1               8842
#define SOUND_MOGRAINE_TAUNT2               8843
#define SOUND_MOGRAINE_TAUNT3               8844
#define SOUND_MOGRAINE_AGGRO1               8835
#define SOUND_MOGRAINE_AGGRO2               8836
#define SOUND_MOGRAINE_AGGRO3               8837
#define SOUND_MOGRAINE_SLAY1                8839
#define SOUND_MOGRAINE_SLAY2                8840
#define SOUND_MOGRAINE_SPECIAL              8841
#define SOUND_MOGRAINE_DEATH                8838

#define SPIRIT_OF_MOGRAINE                  16775

struct MANGOS_DLL_DECL boss_thane_korthazzAI : public ScriptedAI
{
    boss_thane_korthazzAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }
    ScriptedInstance *pInstance;

    uint32 Mark_Timer;
    uint32 Meteor_Timer;
    bool ShieldWall1;
    bool ShieldWall2;
    bool InCombat;

    void Reset()
    {       
        Mark_Timer = 20000; // First Horsemen Mark is applied at 20 sec.
        Meteor_Timer = 30000; // wrong
        ShieldWall1 = true;
        ShieldWall2 = true;
        InCombat = false;

        if (pInstance)
        {
            bool progress;
            progress = false;

            Unit *temp;
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_BLAUMEUX));
            if(temp && temp->isAlive() && temp->getVictim())
            {
                progress = true;
                m_creature->getThreatManager().addThreat(temp->getVictim(),0.0f);
            }
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_MOGRAINE));
            if(temp && temp->isAlive() && temp->getVictim())
            {
                progress = true;
                m_creature->getThreatManager().addThreat(temp->getVictim(),0.0f);
            }
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_ZELIEK));
            if(temp && temp->isAlive() && temp->getVictim())
            {
                progress = true;
                m_creature->getThreatManager().addThreat(temp->getVictim(),0.0f);
            }
            if(!progress)
            {
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_BLAUMEUX));
                if(temp && temp->isDead())
                {
                    temp->setDeathState(ALIVE);
                    ((Creature*)temp)->AI()->EnterEvadeMode();
                }
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_MOGRAINE));
                if(temp && temp->isDead())
                {
                    temp->setDeathState(ALIVE);
                    ((Creature*)temp)->AI()->EnterEvadeMode();
                }
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_ZELIEK));
                if(temp && temp->isDead())
                {
                    temp->setDeathState(ALIVE);
                    ((Creature*)temp)->AI()->EnterEvadeMode();
                }
                HandleDoors(GO_DEATHKNIGHT_DOOR, 0);
            }

            pInstance->SetData(ENCOUNT_FOURHORSEMAN, 0);            
        }

    }

    void Aggro(Unit *who)
    {
        if(!InCombat)
        {
            DoZoneInCombat();
            DoYell(SAY_KORTHAZZ_AGGRO,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_KORTHAZZ_AGGRO);
            if (pInstance)
            {
                pInstance->SetData(ENCOUNT_FOURHORSEMAN, 1);
                HandleDoors(GO_DEATHKNIGHT_DOOR, 1);
                Unit *temp;
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_BLAUMEUX));
                temp->getThreatManager().addThreat(who,0.0f);
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_MOGRAINE));
                temp->getThreatManager().addThreat(who,0.0f);
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_ZELIEK));
                temp->getThreatManager().addThreat(who,0.0f);
            }
        }
        InCombat = true;

    }

    void KilledUnit()
    {
        DoYell(SAY_KORTHAZZ_SLAY,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_KORTHAZZ_SLAY);
    }

    void JustDied(Unit* Killer)
    {
        m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE); 
        DoYell(SAY_KORTHAZZ_DEATH,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature, SOUND_KORTHAZZ_DEATH);

        if (pInstance)
        {
            bool alldeath;
            alldeath = true;
            
            Unit *temp;
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_BLAUMEUX));
            alldeath = temp->isDead();
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_MOGRAINE));
            if(alldeath)
                alldeath = temp->isDead();
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_ZELIEK));
            if(alldeath)
                alldeath = temp->isDead();
            if(alldeath)
            {
                GameObject *chest;
                chest = GameObject::GetGameObject((*m_creature),pInstance->GetData64(GO_FOURHORSEMEN_CHEST));
                chest->SetUInt32Value(GAMEOBJECT_FACTION,35);  
                HandleDoors(GO_DEATHKNIGHT_DOOR, 0);
                pInstance->SetData(ENCOUNT_FOURHORSEMAN, 2);
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

        // Mark of Korthazz
        if(Mark_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_MARK_OF_KORTHAZZ);
            Mark_Timer = 12000;
        }else Mark_Timer -= diff;

        // Shield Wall - All 4 horsemen will shield wall at 50% hp and 20% hp for 20 seconds 
        if(ShieldWall1 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
        {
            if(ShieldWall1)
            {
                DoCast(m_creature,SPELL_SHIELDWALL);
                ShieldWall1 = false;
            }
        }
        if(ShieldWall2 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 20)
        {
            if(ShieldWall2)
            {
                DoCast(m_creature,SPELL_SHIELDWALL);
                ShieldWall2 = false;
            }
        }

        // Meteor
        if(Meteor_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_METEOR);
            Meteor_Timer = 20000; // wrong
        }else Meteor_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_thane_korthazz(Creature *_Creature)
{
    return new boss_thane_korthazzAI (_Creature);
}


struct MANGOS_DLL_DECL boss_sir_zeliekAI : public ScriptedAI
{
    boss_sir_zeliekAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 Mark_Timer;
    uint32 HolyWrath_Timer;
    bool ShieldWall1;
    bool ShieldWall2;
    bool InCombat;

    void Reset()
    {       
        Mark_Timer = 20000; // First Horsemen Mark is applied at 20 sec.
        HolyWrath_Timer = 12000; // right
        ShieldWall1 = true;
        ShieldWall2 = true;
        InCombat = false;

        if (pInstance)
        {
            bool progress;
            progress = false;

            Unit *temp;
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_BLAUMEUX));
            if(temp && temp->isAlive() && temp->getVictim())
            {
                progress = true;
                m_creature->getThreatManager().addThreat(temp->getVictim(),0.0f);
            }
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_MOGRAINE));
            if(temp && temp->isAlive() && temp->getVictim())
            {
                progress = true;
                m_creature->getThreatManager().addThreat(temp->getVictim(),0.0f);
            }
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_KORTHAZZ));
            if(temp && temp->isAlive() && temp->getVictim())
            {
                progress = true;
                m_creature->getThreatManager().addThreat(temp->getVictim(),0.0f);
            }
            if(!progress)
            {
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_BLAUMEUX));
                if(temp && temp->isDead())
                {
                    temp->setDeathState(ALIVE);
                    ((Creature*)temp)->AI()->EnterEvadeMode();
                }
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_MOGRAINE));
                if(temp && temp->isDead())
                {
                    temp->setDeathState(ALIVE);
                    ((Creature*)temp)->AI()->EnterEvadeMode();
                }
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_KORTHAZZ));
                if(temp && temp->isDead())
                {
                    temp->setDeathState(ALIVE);
                    ((Creature*)temp)->AI()->EnterEvadeMode();
                }
                HandleDoors(GO_DEATHKNIGHT_DOOR, 0);
            }

            pInstance->SetData(ENCOUNT_FOURHORSEMAN, 0);            
        }
    }

    void Aggro(Unit *who)
    {
        if(!InCombat)
        {
            DoYell(SAY_ZELIEK_AGGRO,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_ZELIEK_AGGRO);

            if (pInstance)
            {
                pInstance->SetData(ENCOUNT_FOURHORSEMAN, 1);
                HandleDoors(GO_DEATHKNIGHT_DOOR, 1);
                Unit *temp;
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_KORTHAZZ));
                temp->getThreatManager().addThreat(who,0.0f);
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_BLAUMEUX));
                temp->getThreatManager().addThreat(who,0.0f);
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_MOGRAINE));
                temp->getThreatManager().addThreat(who,0.0f);
            }
            InCombat = true;
        }
    }

    void KilledUnit()
    {
        DoYell(SAY_ZELIEK_SLAY,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_ZELIEK_SLAY);
    }

    void JustDied(Unit* Killer)
    {
        m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE); 
        DoYell(SAY_ZELIEK_DEATH,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature, SOUND_ZELIEK_DEATH);

        if (pInstance)
        {
            bool alldeath;
            alldeath = true;
            
            Unit *temp;
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_BLAUMEUX));
            alldeath = temp->isDead();
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_MOGRAINE));
            if(alldeath)
                alldeath = temp->isDead();
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_KORTHAZZ));
            if(alldeath)
                alldeath = temp->isDead();
            if(alldeath)
            {
                GameObject *chest;
                chest = GameObject::GetGameObject((*m_creature),pInstance->GetData64(GO_FOURHORSEMEN_CHEST));
                chest->SetUInt32Value(GAMEOBJECT_FACTION,35); 
                HandleDoors(GO_DEATHKNIGHT_DOOR, 0);
                pInstance->SetData(ENCOUNT_FOURHORSEMAN, 2);
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

        // Mark of Zeliek
        if(Mark_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_MARK_OF_ZELIEK);
            Mark_Timer = 12000;
        }else Mark_Timer -= diff;

        // Shield Wall - All 4 horsemen will shield wall at 50% hp and 20% hp for 20 seconds 
        if(ShieldWall1 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
        {
            if(ShieldWall1)
            {
                DoCast(m_creature,SPELL_SHIELDWALL);
                ShieldWall1 = false;
            }
        }
        if(ShieldWall2 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 20)
        {
            if(ShieldWall2)
            {
                DoCast(m_creature,SPELL_SHIELDWALL);
                ShieldWall2 = false;
            }
        }

        // Holy Wrath
        if(HolyWrath_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_HOLY_WRATH);
            HolyWrath_Timer = 12000;
        }else HolyWrath_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_sir_zeliek(Creature *_Creature)
{
    return new boss_sir_zeliekAI (_Creature);
}



struct MANGOS_DLL_DECL boss_lady_blaumeuxAI : public ScriptedAI
{
    boss_lady_blaumeuxAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 Mark_Timer;
    uint32 VoidZone_Timer;
    bool ShieldWall1;
    bool ShieldWall2;
    bool InCombat;

    void Reset()
    {       
        Mark_Timer = 20000; // First Horsemen Mark is applied at 20 sec.
        VoidZone_Timer = 12000; // right
        ShieldWall1 = true;
        ShieldWall2 = true;
        InCombat = false;

        if (pInstance)
        {
            bool progress;
            progress = false;
            
            Unit *temp;
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_ZELIEK));
            if(temp && temp->isAlive() && temp->getVictim())
            {
                progress = true;
                m_creature->getThreatManager().addThreat(temp->getVictim(),0.0f);
            }
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_MOGRAINE));
            if(temp && temp->isAlive() && temp->getVictim())
            {
                progress = true;
                m_creature->getThreatManager().addThreat(temp->getVictim(),0.0f);
            }
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_KORTHAZZ));
            if(temp && temp->isAlive() && temp->getVictim())
            {
                progress = true;
                m_creature->getThreatManager().addThreat(temp->getVictim(),0.0f);
            }

            if(!progress)
            {
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_ZELIEK));
                if(temp && temp->isDead())
                {
                    temp->setDeathState(ALIVE);
                    ((Creature*)temp)->AI()->EnterEvadeMode();
                }
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_MOGRAINE));
                if(temp && temp->isDead())
                {
                    temp->setDeathState(ALIVE);
                    ((Creature*)temp)->AI()->EnterEvadeMode();
                }
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_KORTHAZZ));
                if(temp && temp->isDead())
                {
                    temp->setDeathState(ALIVE);
                    ((Creature*)temp)->AI()->EnterEvadeMode();
                }
                HandleDoors(GO_DEATHKNIGHT_DOOR, 0);
            }

            pInstance->SetData(ENCOUNT_FOURHORSEMAN, 0);            
        }
    }

    void Aggro(Unit *who)
    {
        if(!InCombat)
        {
            DoYell(SAY_BLAUMEUX_AGGRO,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_BLAUMEUX_AGGRO);

            if (pInstance)
            {
                pInstance->SetData(ENCOUNT_FOURHORSEMAN, 1);
                HandleDoors(GO_DEATHKNIGHT_DOOR, 1);
                Unit *temp;
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_KORTHAZZ));
                temp->getThreatManager().addThreat(who,0.0f);
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_MOGRAINE));
                temp->getThreatManager().addThreat(who,0.0f);
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_ZELIEK));
                temp->getThreatManager().addThreat(who,0.0f);
            }
        }
        InCombat = true;
    }

    void KilledUnit()
    {
        DoYell(SAY_BLAUMEUX_SLAY,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_BLAUMEUX_SLAY);
    }

    void JustDied(Unit* Killer)
    {
        m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE); 
        DoYell(SAY_BLAUMEUX_DEATH,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_BLAUMEUX_DEATH);

        if (pInstance)
        {
            bool alldeath;
            alldeath = true;
            
            Unit *temp;
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_KORTHAZZ));
            alldeath = temp->isDead();
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_MOGRAINE));
            if(alldeath)
                alldeath = temp->isDead();
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_ZELIEK));
            if(alldeath)
                alldeath = temp->isDead();
            if(alldeath)
            {
                GameObject *chest;
                chest = GameObject::GetGameObject((*m_creature),pInstance->GetData64(GO_FOURHORSEMEN_CHEST));
                chest->SetUInt32Value(GAMEOBJECT_FACTION,35);  
                HandleDoors(GO_DEATHKNIGHT_DOOR, 0);
                pInstance->SetData(ENCOUNT_FOURHORSEMAN, 2);
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

        // Mark of Blaumeux
        if(Mark_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_MARK_OF_BLAUMEUX);
            Mark_Timer = 12000;
        }else Mark_Timer -= diff;

        // Shield Wall - All 4 horsemen will shield wall at 50% hp and 20% hp for 20 seconds 
        if(ShieldWall1 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
        {
            if(ShieldWall1)
            {
                DoCast(m_creature,SPELL_SHIELDWALL);
                ShieldWall1 = false;
            }
        }
        if(ShieldWall2 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 20)
        {
            if(ShieldWall2)
            {
                DoCast(m_creature,SPELL_SHIELDWALL);
                ShieldWall2 = false;
            }
        }

        // Void Zone
        if(VoidZone_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_VOIDZONE);
            VoidZone_Timer = 12000; // right
        }else VoidZone_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_lady_blaumeux(Creature *_Creature)
{
    return new boss_lady_blaumeuxAI (_Creature);
}



struct MANGOS_DLL_DECL boss_highlord_mograineAI : public ScriptedAI
{
    boss_highlord_mograineAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 Mark_Timer;
    uint32 RighteousFire_Timer;
    bool ShieldWall1;
    bool ShieldWall2;
    bool InCombat;

    void Reset()
    {       
        Mark_Timer = 20000; // First Horsemen Mark is applied at 20 sec.
        RighteousFire_Timer = 2000; // applied approx 1 out of 4 attacks
        ShieldWall1 = true;
        ShieldWall2 = true;
        InCombat = false;

        if (pInstance)
        {
            bool progress;
            progress = false;
            
            Unit *temp;
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_ZELIEK));
            if(temp && temp->isAlive() && temp->getVictim())
            {
                progress = true;
                m_creature->getThreatManager().addThreat(temp->getVictim(),0.0f);
            }
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_BLAUMEUX));
            if(temp && temp->isAlive() && temp->getVictim())
            {
                progress = true;
                m_creature->getThreatManager().addThreat(temp->getVictim(),0.0f);
            }
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_KORTHAZZ));
            if(temp && temp->isAlive() && temp->getVictim())
            {
                progress = true;
                m_creature->getThreatManager().addThreat(temp->getVictim(),0.0f);
            }

            if(!progress)
            {
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_ZELIEK));
                if(temp && temp->isDead())
                {
                    temp->setDeathState(ALIVE);
                    ((Creature*)temp)->AI()->EnterEvadeMode();
                }
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_BLAUMEUX));
                if(temp && temp->isDead())
                {
                    temp->setDeathState(ALIVE);
                    ((Creature*)temp)->AI()->EnterEvadeMode();
                }
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_KORTHAZZ));
                if(temp && temp->isDead())
                {
                    temp->setDeathState(ALIVE);
                    ((Creature*)temp)->AI()->EnterEvadeMode();
                }
                HandleDoors(GO_DEATHKNIGHT_DOOR, 0);
            }

            pInstance->SetData(ENCOUNT_FOURHORSEMAN, 0);            
        }
    }

    void Aggro(Unit *who)
    {
        if(!InCombat)
        {
            switch(rand()%3)
            {
            case 0:
                DoYell(SAY_MOGRAINE_AGGRO1,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_MOGRAINE_AGGRO1);
                break;

            case 1:
                DoYell(SAY_MOGRAINE_AGGRO2,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_MOGRAINE_AGGRO2);
                break;

            case 2:
                DoYell(SAY_MOGRAINE_AGGRO3,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_MOGRAINE_AGGRO3);
                break;
            }

            if (pInstance)
            {
                pInstance->SetData(ENCOUNT_FOURHORSEMAN, 1);
                HandleDoors(GO_DEATHKNIGHT_DOOR, 1);
                Unit *temp;
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_KORTHAZZ));
                temp->getThreatManager().addThreat(who,0.0f);
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_BLAUMEUX));
                temp->getThreatManager().addThreat(who,0.0f);
                temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_ZELIEK));
                temp->getThreatManager().addThreat(who,0.0f);
            }
        }
        InCombat = true;
    }

    void KilledUnit()
    {
        switch(rand()%2)
        {
        case 0:
            DoYell(SAY_MOGRAINE_SLAY1,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_MOGRAINE_SLAY1);
            break;

        case 1:
            DoYell(SAY_MOGRAINE_SLAY2,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_MOGRAINE_SLAY2);
            break;
        }
    }

    void JustDied(Unit* Killer)
    {
        m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
        DoYell(SAY_MOGRAINE_DEATH,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature, SOUND_MOGRAINE_DEATH);

        if (pInstance)
        {
            bool alldeath;
            alldeath = true;
            
            Unit *temp;
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_BLAUMEUX));
            alldeath = temp->isDead();
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_KORTHAZZ));
            if(alldeath)
                alldeath = temp->isDead();
            temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_ZELIEK));
            if(alldeath)
                alldeath = temp->isDead();
            if(alldeath)
            {
                GameObject *chest;
                chest = GameObject::GetGameObject((*m_creature),pInstance->GetData64(GO_FOURHORSEMEN_CHEST));
                chest->SetUInt32Value(GAMEOBJECT_FACTION,35); 
                HandleDoors(GO_DEATHKNIGHT_DOOR, 0);
                pInstance->SetData(ENCOUNT_FOURHORSEMAN, 2);
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

        // Mark of Mograine
        if(Mark_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_MARK_OF_MOGRAINE);
            Mark_Timer = 12000;
        }else Mark_Timer -= diff;

        // Shield Wall - All 4 horsemen will shield wall at 50% hp and 20% hp for 20 seconds 
        if(ShieldWall1 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
        {
            if(ShieldWall1)
            {
                DoCast(m_creature,SPELL_SHIELDWALL);
                ShieldWall1 = false;
            }
        }
        if(ShieldWall2 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 20)
        {
            if(ShieldWall2)
            {
                DoCast(m_creature,SPELL_SHIELDWALL);
                ShieldWall2 = false;
            }
        }

        // Righteous Fire
        if(RighteousFire_Timer < diff)
        {
            if(rand()%4 == 1) // 1/4
            {
                DoCast(m_creature->getVictim(),SPELL_RIGHTEOUS_FIRE);
            }
            RighteousFire_Timer = 2000; // right
        }else RighteousFire_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_highlord_mograine(Creature *_Creature)
{
    return new boss_highlord_mograineAI (_Creature);
}

void AddSC_boss_fourhorsemen()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_thane_korthazz";
    newscript->GetAI = GetAI_boss_thane_korthazz;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name="boss_sir_zeliek";
    newscript->GetAI = GetAI_boss_sir_zeliek;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name="boss_lady_blaumeux";
    newscript->GetAI = GetAI_boss_lady_blaumeux;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name="boss_highlord_mograine";
    newscript->GetAI = GetAI_boss_highlord_mograine;
    newscript->RegisterSelf();
}
