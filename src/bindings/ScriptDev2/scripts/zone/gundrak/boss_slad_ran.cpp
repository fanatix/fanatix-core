/* Script Data Start
SDName: Boss slad_ran
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_slad_ran' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_POISON_NOVA                           55081
#define SPELL_POISON_NOVA                           59842
//--
#define SPELL_POWERFUL_BITE                         48287
#define SPELL_POWERFUL_BITE                         59840
//--
#define SPELL_VENOM_BOLT                            54970
#define SPELL_VENOM_BOLT                            59839
//At 30% HPStart summoning small serpents

//Yell
#define SAY_AGGRO                                 -1999826
#define SAY_SLAY_1                                -1999826
#define SAY_SLAY_2                                -1999826
#define SAY_SLAY_3                                -1999826
#define SAY_DEATH                                 -1999826
#define SAY_SUMMON_SNAKES                         -1999826    
#define SAY_SUMMON_CONSTRICTORS                   -1999826       

struct MANGOS_DLL_DECL boss_slad_ranAI : public ScriptedAI
{
    boss_slad_ranAI(Creature *c) : ScriptedAI(c) { Reset(); }

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
        DoScriptText(SAY_DEATH, m_creature);
    }
    void KilledUnit(Unit *victim)
    {
        if (victim == m_creature)
            return;

        switch(rand()%3)
        {
            case 0:DoScriptText(SAY_SLAY_1, m_creature);break;
            case 1:DoScriptText(SAY_SLAY_2, m_creature);break;
            case 2:DoScriptText(SAY_SLAY_3, m_creature);break;
        }
    }
};

CreatureAI* GetAI_boss_slad_ran(Creature *_Creature)
{
    return new boss_slad_ranAI (_Creature);
}

void AddSC_boss_slad_ran()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_slad_ran";
    newscript->GetAI = GetAI_boss_slad_ran;
    newscript->RegisterSelf();
}
