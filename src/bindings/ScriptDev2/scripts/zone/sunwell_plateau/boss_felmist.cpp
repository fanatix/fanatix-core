/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Felmist
SD%Complete: 0
SDComment: Once again, no chance for SD2 to see this ever...
EndScriptData */

#include "precompiled.h"
#include "def_sunwell_plateau.h"

#define YELL_AGGRO                     -1930000

#define YELL_SPALTEN			    -1930001
#define YELL_KORROSION		    -1930002
#define YELL_NOXIOUSFUMES		    -1930003

#define YELL_SLAY1			    -1930004
#define YELL_SLAY2			    -1930005
#define YELL_SLAY3			    -1930006

#define YELL_DEATH			    -1930007


#define SPELL_KORROSION	            45866
#define SPELL_NOXIOUSFUMES	            47002
#define SPELL_SPALTEN	            19983

struct MANGOS_DLL_DECL boss_felmistAI : public ScriptedAI
{
    boss_felmistAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 NoxiousFumesTimer;
    uint32 KorrosionTimer;
    uint32 SpaltenTimer;

    bool CastedNoxiousFumes;  

    void Reset()
    {
    	NoxiousFumesTimer	= 20000;
    	KorrosionTimer	= 40000;
    	SpaltenTimer		= 28000;  

	CastedNoxiousFumes = false;
    }

    void Aggro(Unit *who)
    {
        DoScriptText(YELL_AGGRO, m_creature);
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(YELL_SLAY1, m_creature); break;
            case 1: DoScriptText(YELL_SLAY2, m_creature); break;
            case 2: DoScriptText(YELL_SLAY3, m_creature); break;
        }
        switch(rand()%2)
        {
            case 0: DoPlaySoundToSet(m_creature, 12480); break;
            case 1: DoPlaySoundToSet(m_creature, 12481); break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(YELL_DEATH, m_creature);
        
	 GameObject* Gate = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_FELMIST_GATE));
        Gate->SetGoState(0);
	 
	 DoPlaySoundToSet(m_creature, 12483);

	 if(pInstance)
        	pInstance->SetData(DATA_FELMYST_EVENT, DONE);

    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (NoxiousFumesTimer < diff && !CastedNoxiousFumes)
        {
	     DoScriptText(YELL_NOXIOUSFUMES, m_creature);
            DoCast(m_creature, SPELL_NOXIOUSFUMES);
            NoxiousFumesTimer = 0;
	     CastedNoxiousFumes = true;
	     DoPlaySoundToSet(m_creature, 12478);
        }else NoxiousFumesTimer -= diff;

        if (KorrosionTimer < diff)
        {
	     DoScriptText(YELL_KORROSION, m_creature);
            DoCast(m_creature->getVictim(), SPELL_KORROSION);
            KorrosionTimer = 40000;
        }else KorrosionTimer -= diff;

        if (SpaltenTimer < diff)
        {
	     DoScriptText(YELL_SPALTEN, m_creature);
            DoCast(m_creature->getVictim(), SPELL_SPALTEN);
            SpaltenTimer = 28000;
        }else SpaltenTimer -= diff;


        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_felmist(Creature *_Creature)
{
    return new boss_felmistAI (_Creature);
}

void AddSC_boss_felmist()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_felmist";
    newscript->GetAI = &GetAI_boss_felmist;
    newscript->RegisterSelf();
}
