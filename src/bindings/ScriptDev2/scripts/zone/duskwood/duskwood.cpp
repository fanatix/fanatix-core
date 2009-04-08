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
SDName: Duskwood
SD%Complete: 100
SDComment: Quest support: 55
SDCategory: Duskwood
EndScriptData */

/* ContentData
npc_morbent_fel
EndContentData */

#include "precompiled.h"

/*######
## npc_morbent_fel
######*/

enum
{
	SPELL_SACRED_CLEANSING		= 8913,
	NPC_MORBENT_FEL				= 1200,
	NPC_WEAKENED_MORBENT_FEL	= 24782,
};

struct MANGOS_DLL_DECL npc_morbent_felAI : public ScriptedAI
{
	npc_morbent_felAI(Creature* c) : ScriptedAI(c)
	{
		Reset();
	}

	bool mSpawned;

	void Reset()
	{
		mSpawned = false;
		m_creature->SetVisibility(VISIBILITY_ON);
	}
	void Aggro(Unit* who) {}
	void UpdateAI(const uint32 diff)
	{
		if(mSpawned)
		{
			m_creature->SetVisibility(VISIBILITY_OFF);
			m_creature->setDeathState(JUST_DIED);
			m_creature->SetHealth(0);
			m_creature->CombatStop();
			m_creature->DeleteThreatList();
			m_creature->RemoveCorpse();
		}
		if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_npc_morbent_felAI(Creature* _Creature)
{
	return new npc_morbent_felAI(_Creature);
}

bool EffectDummyCreature_npc_morbent_fel(Unit* mCaster, uint32 mSpellID, uint32 effIndex, Creature* mCreatureTarget)
{
	if(mSpellID == SPELL_SACRED_CLEANSING)
	{
		if(mCreatureTarget->GetEntry() != NPC_MORBENT_FEL)
			return true;

		mCreatureTarget->UpdateEntry(NPC_WEAKENED_MORBENT_FEL);
		
		// Set spawned to true
		((npc_morbent_felAI*)mCreatureTarget->AI())->mSpawned = true;

		return true;
	}
	return false;
}

void AddSC_duskwood()
{
	Script* newscript;
	newscript = new Script;
	newscript->Name = "npc_morbent_fel";
	newscript->GetAI = &GetAI_npc_morbent_felAI;
	newscript->pEffectDummyCreature = &EffectDummyCreature_npc_morbent_fel;
	newscript->RegisterSelf();
}
