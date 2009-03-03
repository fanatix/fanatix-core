/* Script Data Start
SDName: Boss skarvald
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_skarvald' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_CHARGE                                 43651
#define SPELL_ENRAGE                                 48193
#define SPELL_STONE_STRIKE                           48583

//Yells
#define SAY_AGGRO                                 -1999680
#define SAY_KILL                                  -1999680
#define SAY_DEATH_1                               -1999680
#define SAY_DEATH_2                               -1999680
#define SAY_RAISES                                -1999680

struct MANGOS_DLL_DECL boss_skarvaldAI : public ScriptedAI
{
    boss_skarvaldAI(Creature *c) : ScriptedAI(c) { Reset(); }

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
    void JustDied(Unit* killer) {}
    void KilledUnit(Unit *victim)
    {
        if(victim == m_creature)
            return;
        DoScriptText(SAY_KILL, m_creature);
    }
};

CreatureAI* GetAI_boss_skarvald(Creature *_Creature)
{
    return new boss_skarvaldAI (_Creature);
}

void AddSC_boss_skarvald()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_skarvald";
    newscript->GetAI = GetAI_boss_skarvald;
    newscript->RegisterSelf();
}
