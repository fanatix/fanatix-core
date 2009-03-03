/* Script Data Start
SDName: Boss volazj
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_volazj' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_INSANITY                         57496 //Dummy
#define INSANITY_VISUAL                        57561
#define SPELL_MIND_FLAY_N                      57941
#define SPELL_MIND_FLAY_H                      59974 
#define SPELL_SHADOW_BOLT_VOLLEY_1             57942 
#define SPELL_SHADOW_BOLT_VOLLEY_2             59975 
#define SPELL_SHIVER_N                         57949 
#define SPELL_SHIVER_H                         59978 

//Yell
#define SAY_AGGRO                               -1999959
#define SAY_SLAY_1                              -1999958
#define SAY_SLAY_2                              -1999957
#define SAY_SLAY_3                              -1999956
#define SAY_DEATH_1                             -1999955
#define SAY_DEATH_2                             -1999954
#define SAY_PHASE                               -1999953

bool VolazjPhase1 = true,
     VolazjPhase2 = false;

struct MANGOS_DLL_DECL boss_volazjAI : public ScriptedAI
{
    boss_volazjAI(Creature *c) : ScriptedAI(c) { Reset(); }
    
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
    void JustDied(Unit* killer)  
    {
        if(VolazjPhase1)            
        DoScriptText(SAY_DEATH_1, m_creature);
        else 
        DoScriptText(SAY_DEATH_2, m_creature);
    }

    void KilledUnit(Unit *victim)
    {
        if (victim == m_creature)
            return;

        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature);break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature);break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature);break;
        }
    }
};

CreatureAI* GetAI_boss_volazj(Creature *_Creature)
{
    return new boss_volazjAI (_Creature);
}

void AddSC_boss_volazj()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_volazj";
    newscript->GetAI = GetAI_boss_volazj;
    newscript->RegisterSelf();
}
