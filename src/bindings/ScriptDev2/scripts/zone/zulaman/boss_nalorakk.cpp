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
SDName: Boss_Nalorakk
SD%Complete: 80
SDComment: Todo: Trash Waves
SDCategory: Zul'Aman
EndScriptData */

#include "precompiled.h"
#include "def_zulaman.h"

#define SAY_WAVE1_AGGRO         -1568010
#define SAY_WAVE2_STAIR1        -1568011
#define SAY_WAVE3_STAIR2        -1568012
#define SAY_WAVE4_PLATFORM      -1568013

#define SAY_EVENT1_SACRIFICE    -1568014
#define SAY_EVENT2_SACRIFICE    -1568015

#define SAY_AGGRO               -1568016
#define SAY_SURGE               -1568017
#define SAY_TOBEAR              -1568018
#define SAY_TOTROLL             -1568019
#define SAY_BERSERK             -1568020
#define SAY_SLAY1               -1568021
#define SAY_SLAY2               -1568022
#define SAY_DEATH               -1568023

#define SPELL_BERSERK           45078

//Defines for Troll form
#define SPELL_BRUTALSWIPE       42384
#define SPELL_MANGLE            42389
#define SPELL_MANGLEEFFECT      44955
#define SPELL_SURGE             42402
#define SPELL_BEARFORM          42377

//Defines for Bear form
#define SPELL_LACERATINGSLASH   42395
#define SPELL_RENDFLESH         42397
#define SPELL_DEAFENINGROAR     42398

#define WEAPON_ID               33094

struct MANGOS_DLL_DECL boss_nalorakkAI : public ScriptedAI
{
    boss_nalorakkAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 BrutalSwipe_Timer;
    uint32 Mangle_Timer;
    uint32 Surge_Timer;

    uint32 LaceratingSlash_Timer;
    uint32 RendFlesh_Timer;
    uint32 DeafeningRoar_Timer;

    uint32 ShapeShift_Timer;
    uint32 Berserk_Timer;

    uint64 ChargeTargetGUID;
    bool isCharging;

    bool inBearForm;

    void Reset()
    {
        if(pInstance)
            pInstance->SetData(DATA_NALORAKKEVENT, NOT_STARTED);

        Surge_Timer = 15000 + rand()%5000;
        BrutalSwipe_Timer = 7000 + rand()%5000;
        Mangle_Timer = 10000 + rand()%5000;
        ShapeShift_Timer = 45000 + rand()%5000;
        Berserk_Timer = 600000;

        ChargeTargetGUID = 0;
        isCharging = false;

        inBearForm = false;
        m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+1, WEAPON_ID);
    }

    void Aggro(Unit *who)
    {
        if(pInstance)
            pInstance->SetData(DATA_NALORAKKEVENT, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
        DoZoneInCombat();
    }

    void JustDied(Unit* Killer)    
    {	
        if(pInstance)
            pInstance->SetData(DATA_NALORAKKEVENT, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit* victim)    
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
        }
    }

    void MovementInform(uint32, uint32)
    {
        if(ChargeTargetGUID)
        {
            if(Unit* target = Unit::GetUnit(*m_creature, ChargeTargetGUID))
                m_creature->CastSpell(target, SPELL_SURGE, true);
            ChargeTargetGUID = 0;
        }
    }

    Player* SelectRandomPlayer(float range = 0.0f, bool alive = true)
    {
        Map *map = m_creature->GetMap();
        if (!map->IsDungeon()) return NULL;

        Map::PlayerList const &PlayerList = map->GetPlayers();
        if (PlayerList.isEmpty())
            return NULL;
        
        std::list<Player*> temp;
        std::list<Player*>::iterator j;
		
        for(Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
			if((range == 0.0f || m_creature->IsWithinDistInMap(i->getSource(), range))
				&& (!alive || i->getSource()->isTargetableForAttack()))
				temp.push_back(i->getSource());

		if (temp.size()) {
			j = temp.begin();
		    advance(j, rand()%temp.size());
		    return (*j);
		}
        return NULL;

    }

    void UpdateAI(const uint32 diff)
    {
        if(isCharging)
        {
            if(!ChargeTargetGUID)
            {
                m_creature->SetSpeed(MOVE_RUN, 1.2f);
                m_creature->GetMotionMaster()->Clear();
                if(m_creature->getVictim())
                {
                    m_creature->Attack(m_creature->getVictim(), true);
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                }
                isCharging = false;
            }
            return;
        }

        if(!m_creature->SelectHostilTarget() && !m_creature->getVictim())
            return;

        if(Berserk_Timer < diff)
        {
            DoScriptText(SAY_BERSERK, m_creature);
            DoCast(m_creature, SPELL_BERSERK, true);
            Berserk_Timer = 600000;
        }else Berserk_Timer -= diff;

        if(ShapeShift_Timer < diff)
        {
            if(inBearForm)
            {
                DoScriptText(SAY_TOTROLL, m_creature);

                m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+1, WEAPON_ID);
                m_creature->RemoveAurasDueToSpell(SPELL_BEARFORM);
                Surge_Timer = 15000 + rand()%5000;
                BrutalSwipe_Timer = 7000 + rand()%5000;
                Mangle_Timer = 10000 + rand()%5000;
                ShapeShift_Timer = 45000 + rand()%5000;
                inBearForm = false;
            }
            else
            {
                DoScriptText(SAY_TOBEAR, m_creature);
                m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+1, 0);
                DoCast(m_creature, SPELL_BEARFORM, true);
                LaceratingSlash_Timer = 2000; // dur 18s
                RendFlesh_Timer = 3000;  // dur 5s
                DeafeningRoar_Timer = 5000 + rand()%5000;  // dur 2s
                ShapeShift_Timer = 20000 + rand()%5000; // dur 30s
                inBearForm = true;
            }
        }else ShapeShift_Timer -= diff;

        if(!inBearForm)
        {
            if(BrutalSwipe_Timer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_BRUTALSWIPE);
                BrutalSwipe_Timer = 7000 + rand()%5000;
            }else BrutalSwipe_Timer -= diff;

            if(Mangle_Timer < diff)
            {
                if(!m_creature->getVictim()->HasAura(SPELL_MANGLEEFFECT, 0))
                {
                    DoCast(m_creature->getVictim(), SPELL_MANGLE);
                    Mangle_Timer = 1000;
                }
                else Mangle_Timer = 10000 + rand()%5000;
            }else Mangle_Timer -= diff;

            if(Surge_Timer < diff)
            {
                DoScriptText(SAY_SURGE, m_creature);

                Unit *target = SelectRandomPlayer(45);
                if(!target) target = m_creature->getVictim();
                isCharging = true;
                ChargeTargetGUID = target->GetGUID();

                float x, y, z;
                target->GetContactPoint(m_creature, x, y, z);
                m_creature->SetSpeed(MOVE_RUN, 5.0f);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MovePoint(0, x, y, z);

                Surge_Timer = 15000 + rand()%5000;
                return;
            }else Surge_Timer -= diff;
        }
        else
        {
            if(LaceratingSlash_Timer < diff)
            {
                if(!m_creature->getVictim()->HasAura(SPELL_MANGLEEFFECT, 0))
                    DoCast(m_creature->getVictim(), SPELL_LACERATINGSLASH);
                else
                {
                    int32 bp0 = 3470;
                    m_creature->CastCustomSpell(m_creature->getVictim(), SPELL_LACERATINGSLASH, &bp0, NULL, NULL, false);
                }
                LaceratingSlash_Timer = 18000 + rand()%5000;
            }else LaceratingSlash_Timer -= diff;

            if(RendFlesh_Timer < diff)
            {
                if(!m_creature->getVictim()->HasAura(SPELL_MANGLEEFFECT, 0))
                    DoCast(m_creature->getVictim(), SPELL_RENDFLESH);
                else
                {
                    int32 bp1 = 4670;
                    m_creature->CastCustomSpell(m_creature->getVictim(), SPELL_RENDFLESH, NULL, &bp1, NULL, false);
                }
                RendFlesh_Timer = 5000 + rand()%5000;
            }else RendFlesh_Timer -= diff;

            if(DeafeningRoar_Timer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_DEAFENINGROAR);
                DeafeningRoar_Timer = 15000 + rand()%5000;
            }else DeafeningRoar_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_nalorakk(Creature *_Creature)
{
    return new boss_nalorakkAI (_Creature);
}

void AddSC_boss_nalorakk()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_nalorakk";
    newscript->GetAI = &GetAI_boss_nalorakk;
    newscript->RegisterSelf();
}
