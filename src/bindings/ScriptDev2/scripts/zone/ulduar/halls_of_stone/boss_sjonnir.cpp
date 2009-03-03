/* Script Data Start
SDName: Boss sjonnir
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_sjonnir' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_LIGHTING_RING                        50840 //Periodic Trigger (interval 2s) spell = 50841
#define SPELL_LIGHTING_RING                        51849 //Periodic Trigger (interval 2s) spell = 50841
#define SPELL_LIGHTING_RING                        59848 //Periodic Trigger (interval 2s) spell = 59849
#define SPELL_LIGHTING_RING                        59861 //Periodic Trigger (interval 2s) spell = 59849
#define SPELL_STATIC_CHARGE                        50834 //Periodic Trigger 2s interval, spell =50835
#define SPELL_STATIC_CHARGE                        59846 //Periodic Trigger 2s interval, spell =50847
#define SPELL_CHAIN_LIGHTING                       50830
#define SPELL_CHAIN_LIGHTING                       59844
#define SPELL_LIGHTING_SHIELD                      50831
#define SPELL_LIGHTING_SHIELD                      59845
#define SPELL_FRENZY                               28747

//Yell
#define SAY_AGGRO                               -1999690
#define SAY_SLAY_1                              -1999690
#define SAY_SLAY_2                              -1999690
#define SAY_SLAY_3                              -1999690
#define SAY_DEATH                               -1999690

struct MANGOS_DLL_DECL boss_sjonnirAI : public ScriptedAI
{
    boss_sjonnirAI(Creature *c) : ScriptedAI(c) { Reset(); }

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
        if(victim == m_creature)
            return;
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature);break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature);break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature);break;
        }
    }
};

CreatureAI* GetAI_boss_sjonnir(Creature *_Creature)
{
    return new boss_sjonnirAI (_Creature);
}

void AddSC_boss_sjonnir()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_sjonnir";
    newscript->GetAI = GetAI_boss_sjonnir;
    newscript->RegisterSelf();
}
