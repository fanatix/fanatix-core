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
SDName: Boss_Heigan
SD%Complete: 90
SDComment: Missing Eruption
SDCategory: Naxxramas
EndScriptData */
 
#include "precompiled.h"
#include "def_naxxramas.h"
#include "Player.h"
 
#define SAY_AGGRO1          "You are mine now!"
#define SAY_AGGRO2          "I see you!"
#define SAY_AGGRO3          "You... are next!"
#define SAY_SLAY1           "Close your eyes... sleep!"
#define SAY_SLAY2           "The races of the world will perish. It is only a matter of time."
#define SAY_SLAY3           "I see endless suffering, I see torment, I see rage. I see... everything!"
#define SAY_SLAY4           "Soon... the world will tremble!"
#define SAY_SLAY5           "The end is upon you."
#define SAY_SLAY6           "Hungry worms will feast on your rotten flesh!"
 
#define SOUND_AGGRO1        8825
#define SOUND_AGGRO2        8826
#define SOUND_AGGRO3        8827
#define SOUND_SLAY1         8829
#define SOUND_SLAY2         8830
#define SOUND_SLAY3         8831
#define SOUND_SLAY4         8832
#define SOUND_SLAY5         8833
#define SOUND_SLAY6         8834
#define SOUND_DEATH         8828
 
//Spell used by floor peices to cause damage to players
#define SPELL_ERUPTION      29371
 
//Spells by boss
#define SPELL_WILT          23772
#define SPELL_FEAVER        29998
 
//Spell by eye stalks
#define SPELL_MIND_FLAY     26143
 
//Coordinates of Eye Stalks Tunnel
#define TUNNEL_X           2893.972168
#define TUNNEL_Y           -3769.962402
#define TUNNEL_Z           273.621216
#define TUNNEL_O           3.139074
 
struct MANGOS_DLL_DECL boss_heiganAI : public ScriptedAI
{
    boss_heiganAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }

    ScriptedInstance *pInstance;
 
    uint32 Wilt_Timer;
    uint32 Feaver_Timer;
    uint32 Teleport_Timer;
 
    bool InCombat;
 
    void Reset()
    {
        InCombat = false;

        Wilt_Timer = 6500;
        Feaver_Timer = 17000;
        Teleport_Timer = 60000;

        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_HEIGAN, 0);
            HandleDoors(GO_HEIGAN_ENTRY, 0);
            HandleDoors(GO_HEIGAN_EXIT, 1);
            HandleDoors(GO_HEIGAN_EXITGATE, 1);
            HandleDoors(GO_LOATHEB_DOOR, 1);
        }
    }
 
    void Aggro(Unit *who)
    {
        DoZoneInCombat();
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
            pInstance->SetData(ENCOUNT_HEIGAN, 1);
            HandleDoors(GO_HEIGAN_ENTRY, 1);
            HandleDoors(GO_HEIGAN_EXIT, 0);
        }
    }
 
    void KilledUnit(Unit* victim)
    {
 
        switch (rand()%6)
        {
        case 0:
            DoYell(SAY_SLAY1,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY1);
            break;
        case 1:
            DoYell(SAY_SLAY2,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY2);
            break;
        case 2:
            DoYell(SAY_SLAY3,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY3);
            break;
        case 3:
            DoYell(SAY_SLAY4,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY4);
            break;
        case 4:
            DoYell(SAY_SLAY5,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY5);
            break;
        case 5:
            DoYell(SAY_SLAY6,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY6);
            break;
        }
 
    }
 
    void ResetThreat()
    {
        std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
        std::list<HostilReference*>::iterator i = m_threatlist.begin();
        for(i = m_threatlist.begin(); i != m_threatlist.end(); ++i)
        {
            Unit* pUnit = NULL;
            pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
            if(pUnit)
            {
                m_creature->getThreatManager().modifyThreatPercent(pUnit, -99);
            }
        }
    }
 
    void JustDied(Unit* Killer)
    {
        DoPlaySoundToSet(m_creature,SOUND_DEATH);
        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_HEIGAN, 3);
            HandleDoors(GO_HEIGAN_ENTRY, 0);
            HandleDoors(GO_HEIGAN_EXITGATE, 0);
            HandleDoors(GO_HEIGAN_EXIT, 0);
            Unit *temp = Unit::GetUnit((*m_creature),pInstance->GetData64(GUID_NOTH));
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
 
        //Wilt_Timer
        if (Wilt_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_WILT);
 
            Wilt_Timer = 13000;
        }else Wilt_Timer -= diff;
 
        //Feaver_Timer
        if (Feaver_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FEAVER);
 
            Feaver_Timer = 21000;
        }else Feaver_Timer -= diff;
 
        //Teleport_Timer
        if (Teleport_Timer < diff)
        {
            //Teleport in the Tunnel
          for (int i = 0; i < 2; i++)
          {
              Unit *target = NULL;
              target = SelectUnit(SELECT_TARGET_RANDOM, 0);
     
                if (target && target->GetTypeId() == TYPEID_PLAYER) 
                {
                    ((Player*)target)->TeleportTo(533, TUNNEL_X, TUNNEL_Y, TUNNEL_Z, TUNNEL_O);
                }
     
                ResetThreat();
          }
 
            Teleport_Timer = 70000;
        }else Teleport_Timer -= diff;
 
        DoMeleeAttackIfReady();
    }
}; 
 
struct MANGOS_DLL_DECL mob_eye_stalksAI : public ScriptedAI
{
    mob_eye_stalksAI(Creature *c) : ScriptedAI(c) {;}
 
    uint32 MindFlay_Timer;
 
    bool InCombat;
 
    void Reset()
    {       
        MindFlay_Timer = 3500;
        InCombat = false;
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
    
    void Aggro(Unit *who)
    {
        if (!InCombat)
        {
            InCombat = true;
        }
    }
 
    void UpdateAI(const uint32 diff)
    {
 
        //MindFlay_Timer
        if (MindFlay_Timer < diff)
        {
            if(m_creature->IsWithinDistInMap(m_creature->getVictim(),50))
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_MIND_FLAY);
            }
            else
            {
                m_creature->AI()->EnterEvadeMode();
            }
 
            MindFlay_Timer = 15000;
        }else MindFlay_Timer -= diff;
 
        DoMeleeAttackIfReady();
 
    }
 
};
 
CreatureAI* GetAI_boss_heigan(Creature *_Creature)
{
    return new boss_heiganAI (_Creature);
}
CreatureAI* GetAI_mob_eye_stalks(Creature *_Creature)
{
    return new mob_eye_stalksAI (_Creature);
}
 
void AddSC_boss_heigan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_heigan";
    newscript->GetAI = GetAI_boss_heigan;
    newscript->RegisterSelf();
 
    newscript = new Script;
    newscript->Name="mob_eye_stalks";
    newscript->GetAI = GetAI_mob_eye_stalks;
    newscript->RegisterSelf();
}