/* Script Data Start
SDName: Boss magus_telestra
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = '' where entry = '';
*** SQL END ***/
#include "precompiled.h"


//Spells

//phase 1
#define SPELL_ICE_NOVA          47772
#define SPELL_ICE_NOVA          56935
#define SPELL_FIREBOMB          47773
#define SPELL_FIREBOMB          56934
#define SPELL_GAVITY_WELL       47756

//Phase 2      -->50% HP (3 clones, Frost, Fire, Arcane)
//Frost Magus (npc 26930)
#define SPELL_BLIZZARD_N        47727
#define SPELL_ICE_BARB_N        47729
//heroic
#define SPELL_BLIZZARD_H        56936
#define SPELL_ICE_BARB_H        56937

//Fire Magus (npc 26928)
#define SPELL_FIRE_BLAST        47721
#define SPELL_SCORCH            47723
//heroic
#define SPELL_FIRE_BLAST_H      56939
#define SPELL_SCORCH_H          56938

//Arcane Magus (npc 26929)
#define SPELL_CRITTER           47731
#define SPELL_TIME_STOP         47736

//Yell
#define SAY_AGGRO             -1999780   
#define SAY_KILL              -1999779      
#define SAY_DEATH             -1999778
#define SAY_MERGE             -1999777
#define SAY_SPLIT_1           -1999776
#define SAY_SPLIT_2           -1999775


struct MANGOS_DLL_DECL boss_magus_telestraAI : public ScriptedAI
{
    boss_magus_telestraAI(Creature *c) : ScriptedAI(c) { Reset(); }

    void Reset() {}
    void Aggro(Unit* who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }
    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who) {}
    void UpdateAI(const uint32 diff) 
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
                
        DoMeleeAttackIfReady();    
    }
    void JustDied(Unit* killer)  {}
    void KilledUnit(Unit *victim)
    {
        if(victim == m_creature)
            return;
        DoScriptText(SAY_KILL, m_creature);
    }
};

CreatureAI* GetAI_boss_magus_telestra(Creature *_Creature)
{
    return new boss_magus_telestraAI (_Creature);
}

void AddSC_boss_magus_telestra()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_magus_telestra";
    newscript->GetAI = GetAI_boss_magus_telestra;
    newscript->RegisterSelf();
}
