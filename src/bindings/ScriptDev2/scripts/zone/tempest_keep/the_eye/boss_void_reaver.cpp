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
SDName: Boss_Void_Reaver
SD%Complete: 90
SDComment: Should reset if raid are out of room.
SDCategory: Tempest Keep, The Eye
EndScriptData */

#include "precompiled.h"
#include "def_the_eye.h"

#define SAY_AGGRO                   -1550000
#define SAY_SLAY1                   -1550001
#define SAY_SLAY2                   -1550002
#define SAY_SLAY3                   -1550003
#define SAY_DEATH                   -1550004
#define SAY_POUNDING1               -1550005
#define SAY_POUNDING2               -1550006

#define SPELL_POUNDING              34162
#define SPELL_ARCANE_ORB_MISSILE    34172
#define SPELL_KNOCK_AWAY            25778
#define SPELL_BERSERK               26662

//Unknown function. If target not found, this will be created and used as dummy target instead?
//#define CREATURE_ORB_TARGET         19577

struct MANGOS_DLL_DECL boss_void_reaverAI : public ScriptedAI
{
    boss_void_reaverAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 Pounding_Timer;
    uint32 ArcaneOrb_Timer;
    uint32 KnockAway_Timer;
    uint32 Berserk_Timer;

    void Reset()
    {
        Pounding_Timer = 15000;
        ArcaneOrb_Timer = 18000;
        KnockAway_Timer = 30000;
        Berserk_Timer = 600000;

        m_creature->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
        m_creature->ApplySpellImmune(1, IMMUNITY_EFFECT,SPELL_EFFECT_ATTACK_ME, true);

        if (pInstance && m_creature->isAlive())
            pInstance->SetData(DATA_VOIDREAVEREVENT, NOT_STARTED);
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY3, m_creature); break;
        }
    }

    void JustDied(Unit *victim)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (pInstance)
            pInstance->SetData(DATA_VOIDREAVEREVENT, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
		DoZoneInCombat();

        if (pInstance)
            pInstance->SetData(DATA_VOIDREAVEREVENT, IN_PROGRESS);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        // Pounding
        if (Pounding_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_POUNDING);

            switch(rand()%2)
            {
                case 0: DoScriptText(SAY_POUNDING1, m_creature); break;
                case 1: DoScriptText(SAY_POUNDING2, m_creature); break;
            }

            Pounding_Timer = 15000;                         //cast time(3000) + cooldown time(12000)
        }else Pounding_Timer -= diff;

        // Arcane Orb
        if (ArcaneOrb_Timer < diff)
        {
            Unit *target = NULL;
            std::list<HostilReference *> t_list = m_creature->getThreatManager().getThreatList();
            std::vector<Unit *> target_list;
            for(std::list<HostilReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
            {
                target = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
                // exclude pets & totems
                if (target->GetTypeId() != TYPEID_PLAYER)
                continue;

                //18 yard radius minimum
                if (target && target->GetDistance2d(m_creature) > 18)
                    target_list.push_back(target);

                target = NULL;
            }

            if (target_list.size())
                target = *(target_list.begin()+rand()%target_list.size());
            else
                target = m_creature->getVictim();

            if (target)
                DoCast(target, SPELL_ARCANE_ORB_MISSILE);

            ArcaneOrb_Timer = 18000;
        }else ArcaneOrb_Timer -= diff;

        // Single Target knock back, reduces aggro
        if (KnockAway_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_KNOCK_AWAY);

            //Drop 25% aggro
            if (m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-25);

            KnockAway_Timer = 30000;
        }else KnockAway_Timer -= diff;

        //Berserk
        if (Berserk_Timer < diff)
        {
            if (m_creature->IsNonMeleeSpellCasted(false))
                m_creature->InterruptNonMeleeSpells(false);

            DoCast(m_creature,SPELL_BERSERK);
            Berserk_Timer = 600000;
        }else Berserk_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_void_reaver(Creature *_Creature)
{
    return new boss_void_reaverAI (_Creature);
}

void AddSC_boss_void_reaver()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_void_reaver";
    newscript->GetAI = &GetAI_boss_void_reaver;
    newscript->RegisterSelf();
}
