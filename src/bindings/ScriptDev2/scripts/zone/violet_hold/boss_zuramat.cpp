/* Script Data Start
SDName: Boss zuramat
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
#define SPELL_SHROUD_OF_DARKNESS                       54524
#define SPELL_SUMMON_VOID_SENTRY                       54524
#define SPELL_VOID_SHIFT                               54524

#define NPC_VOID_SENTRY                                29364

//Yells
#define SAY_AGGRO                                   -1999585
#define SAY_SLAY_1                                  -1999584
#define SAY_SLAY_2                                  -1999583
#define SAY_SLAY_3                                  -1999582
#define SAY_DEATH                                   -1999581
#define SAY_SPAWN                                   -1999580
#define SAY_SHIELD                                  -1999579
#define SAY_WHISPER                                 -1999578

struct MANGOS_DLL_DECL boss_zuramatAI : public ScriptedAI
{
    boss_zuramatAI(Creature *c) : ScriptedAI(c) { Reset(); }

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

CreatureAI* GetAI_boss_zuramat(Creature *_Creature)
{
    return new boss_zuramatAI (_Creature);
}

void AddSC_boss_zuramat()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_zuramat";
    newscript->GetAI = GetAI_boss_zuramat;
    newscript->RegisterSelf();
}
