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
SDName: Boss_Razuvious
SD%Complete: 50
SDComment: Missing adds and event is impossible without Mind Control
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "def_naxxramas.h"

#define SAY_AGGRO1    "Hah hah, I'm just getting warmed up!"
#define SAY_AGGRO2    "Stand and fight!"
#define SAY_AGGRO3    "Show me what you've got!"
#define SAY_SLAY1     "You should've stayed home!"
#define SAY_SLAY2     ""
#define SAY_COMMND1   "Do as I taught you!"
#define SAY_COMMND2   "Show them no mercy!"
#define SAY_COMMND3   "You disappoint me, students!"
#define SAY_COMMND4   "The time for practice is over! Show me what you've learned!"
#define SAY_COMMND5   "Sweep the leg! Do you have a problem with that?"
#define SAY_DEATH     "An honorable... death..."

#define SOUND_AGGRO1    8852
#define SOUND_AGGRO2    8853
#define SOUND_AGGRO3    8854
#define SOUND_SLAY1     8861
#define SOUND_SLAY2     8863
#define SOUND_COMMND1   8855
#define SOUND_COMMND2   8856
#define SOUND_COMMND3   8858
#define SOUND_COMMND4   8859
#define SOUND_COMMND5   8861
#define SOUND_DEATH     8860
#define SOUND_AGGROMIX  8847

#define SPELL_UNBALANCINGSTRIKE     26613
#define SPELL_DISRUPTINGSHOUT       29107

struct MANGOS_DLL_DECL boss_razuviousAI : public ScriptedAI
{
    boss_razuviousAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 UnbalancingStrike_Timer;
    uint32 DisruptingShout_Timer;
    uint32 CommandSound_Timer;
    bool InCombat;

    void Reset()
    {
        UnbalancingStrike_Timer = 30000;    //30 seconds
        DisruptingShout_Timer = 25000;      //25 seconds
        CommandSound_Timer = 40000;         //40 seconds
        InCombat = false;
		if(pInstance)
		{
			pInstance->SetData(ENCOUNT_RAZUVIOUS, 1);
			HandleDoors(GO_VACCUUM_ENTER, 1);	
			Unit *temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_GOTHIK));
			if(temp && temp->isDead())
				HandleDoors(GO_DEATHKNIGHT_DOOR, 0);			
		}

    }

    void KilledUnit(Unit* Victim)
    {
        if (rand()%3)
            return;

        switch (rand()%2)
        {
        case 0:
            DoPlaySoundToSet(m_creature, SOUND_SLAY1);
            DoYell(SAY_SLAY1,LANG_UNIVERSAL,NULL);
            break;

        case 1:
            DoPlaySoundToSet(m_creature, SOUND_SLAY2);
            DoYell(SAY_SLAY2,LANG_UNIVERSAL,NULL);
            break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoPlaySoundToSet(m_creature, SOUND_DEATH);
        DoYell(SAY_DEATH,LANG_UNIVERSAL,NULL);
		if(pInstance)
		{
			pInstance->SetData(ENCOUNT_RAZUVIOUS, 2); 
			HandleDoors(GO_VACCUUM_ENTER, 0);	
			Unit *temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_GOTHIK));
			if(temp && temp->isDead())
				HandleDoors(GO_DEATHKNIGHT_DOOR, 0);				
		}
	}

    void Aggro(Unit *who)
    {

        DoPlaySoundToSet(m_creature, SOUND_AGGRO1);
        DoYell(SAY_COMMND1,LANG_UNIVERSAL,NULL);        
		
		if(pInstance)
		{
			pInstance->SetData(ENCOUNT_RAZUVIOUS, 1);
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

        //UnbalancingStrike_Timer
        if (UnbalancingStrike_Timer < diff)
        {
            //Cast Unbalancing strike
            DoCast(m_creature->getVictim(),SPELL_UNBALANCINGSTRIKE);

            //30 seconds until we should cast this agian
            UnbalancingStrike_Timer = 30000;
        }else UnbalancingStrike_Timer -= diff;

        //DisruptingShout_Timer
        if (DisruptingShout_Timer < diff)
        {
            //Cast
            DoCast(m_creature, SPELL_DISRUPTINGSHOUT, true);

            //25 seconds until we should cast this agian
            DisruptingShout_Timer = 25000;
        }else DisruptingShout_Timer -= diff;

  

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_razuvious(Creature *_Creature)
{
    return new boss_razuviousAI (_Creature);
}

void AddSC_boss_razuvious()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_razuvious";
    newscript->GetAI = GetAI_boss_razuvious;
    newscript->RegisterSelf();
}
