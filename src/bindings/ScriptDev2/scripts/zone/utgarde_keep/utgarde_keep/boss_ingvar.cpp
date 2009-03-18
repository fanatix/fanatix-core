/* Script Data Start
SDName: Boss ingvar
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_ingvar' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//phase 1 Human
#define SPELL_STAGGERING_ROAR                     42708
#define SPELL_DREADFUL_ROAR                       42729
#define SPELL_SMASH                               42669
#define SPELL_ENRAGE                              42705
#define SPELL_CLEAVE                              42724
//whed killed in phase1 is resurected by Annhylde the Caller and becomes Undead
//only in phase 2 
#define NPC_ANNHYLDE_THE_CALLER                   24068
#define SPELL_WOE_STRIKE                          42730
#define SPELL_DARK_SMASH                          42723
#define SPELL_SHADOW_AXE                          42748

//Yell
#define SAY_AGGRO                              -1999670
#define SAY_KILL                               -1999669
#define SAY_DEATH                              -1999668
#define SAY_ATTACK_1                           -1999667
#define SAY_ATTACK_2                           -1999666
#define SAY_ATTACK_3                           -1999665
#define SAY_ATTACK_4                           -1999664
#define SAY_PHASE2_AGGRO                       -1999663
#define SAY_PHASE2_KILL                        -1999662
#define SAY_PHASE2_DEATH                       -1999661
#define SAY_ANNHYLDE_THE_CALLER_RESURECTION    -1999524

bool IngvarPhase1 = true,
     IngvarPhase2 = false;

struct MANGOS_DLL_DECL boss_ingvarAI : public ScriptedAI
{
    boss_ingvarAI(Creature *c) : ScriptedAI(c) { Reset(); }

    void Reset() {}
    void Aggro(Unit* who) 
    {
        if(IngvarPhase1)
            DoScriptText(SAY_AGGRO, m_creature);
        if(IngvarPhase2)
            DoScriptText(SAY_PHASE2_AGGRO, m_creature);
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
    void JustDied(Unit* killer)  
    {
        if(IngvarPhase1)
            DoScriptText(SAY_DEATH, m_creature);
        if(IngvarPhase2)
            DoScriptText(SAY_PHASE2_DEATH, m_creature);
    }
    void KilledUnit(Unit *victim)
    {
        if(victim == m_creature)
            return;
        if(IngvarPhase1)
            DoScriptText(SAY_KILL, m_creature);
        if(IngvarPhase2)
            DoScriptText(SAY_PHASE2_KILL, m_creature);
    }
};

CreatureAI* GetAI_boss_ingvar(Creature *_Creature)
{
    return new boss_ingvarAI (_Creature);
}

void AddSC_boss_ingvar()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_ingvar";
    newscript->GetAI = GetAI_boss_ingvar;
    newscript->RegisterSelf();
}
