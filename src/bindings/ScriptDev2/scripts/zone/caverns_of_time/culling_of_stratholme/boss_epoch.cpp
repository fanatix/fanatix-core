/* Script Data Start
SDName: Boss epoch
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
#define SPELL_CURSE_OF_EXERTION                   52772
#define SPELL_TIME_WARP                           52766 //Time slows down, reducing attack, casting and movement speed by 70% for 6 sec.
#define SPELL_TIME_STOP                           58848 //Stops time in a 50 yard sphere for 2 sec.
#define SPELL_WOUNDING_STRIKE_N                   52771 //Used only on the tank
#define SPELL_WOUNDING_STRIKE_H                   58830

//Say
#define SAY_INTRO                                -1999991 //"Prince Arthas Menethil, on this day, a powerful darkness has taken hold of your soul. The death you are destined to visit upon others will this day be your own."
#define SAY_AGGRO                                -1999998 //"We'll see about that, young prince."
#define SAY_TIME_WARP_1                          -1999997 //"Tick tock, tick tock..."
#define SAY_TIME_WARP_2                          -1999996 //"Not quick enough!"
#define SAY_TIME_WARP_3                          -1999995 //"Let's get this over with. "
#define SAY_SLAY_1                               -1999994 //"There is no future for you."
#define SAY_SLAY_2                               -1999993 //"This is the hour of our greatest triumph!"
#define SAY_SLAY_3                               -1999992 //"You were destined to fail. "
#define SAY_DEATH                                -1999990 //"*gurgles*"

struct MANGOS_DLL_DECL boss_epochAI : public ScriptedAI
{
    boss_epochAI(Creature *c) : ScriptedAI(c) { Reset(); }

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

CreatureAI* GetAI_boss_epoch(Creature *_Creature)
{
    return new boss_epochAI (_Creature);
}

void AddSC_boss_epoch()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_epoch";
    newscript->GetAI = GetAI_boss_epoch;
    newscript->RegisterSelf();
}
