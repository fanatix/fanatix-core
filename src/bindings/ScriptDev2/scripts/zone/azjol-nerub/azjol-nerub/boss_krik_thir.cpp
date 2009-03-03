/* Script Data Start
SDName: Boss krik_thir
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_krik_thir' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_SUMMON_SKITTERING_SWARMER               52438 //maybe 52439 //summons NPC 28735
#define SPELL_MIND_FLAY                               52586
#define SPELL_FRENZY                                  53361
#define SPELL_COURSE_OF_FATIGUE                       52592

#define SKITTERING_SWARMER                            28735

//Yell
#define SAY_AGGRO                                    -1999940
#define SAY_SLAY_1                                   -1999939 
#define SAY_SLAY_2                                   -1999938  
#define SAY_SLAY_3                                   -1999937 
#define SAY_DEATH                                    -1999936  
#define SAY_SEND_GROUP_1                             -1999935     
#define SAY_SEND_GROUP_2                             -1999934       
#define SAY_SEND_GROUP_3                             -1999933       
#define SAY_SWARM_1                                  -1999932
#define SAY_SWARM_2                                  -1999931   
#define SAY_PREFIGHT_1                               -1999930   
#define SAY_PREFIGHT_2                               -1999929     
#define SAY_PREFIGHT_3                               -1999928    

struct MANGOS_DLL_DECL boss_krik_thirAI : public ScriptedAI
{
    boss_krik_thirAI(Creature *c) : ScriptedAI(c) { Reset(); }

    void Reset() {}
    
    void Aggro(Unit* who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void UpdateAI(const uint32 diff) 
    {
                //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
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
            case 0: DoScriptText(SAY_SLAY_1, m_creature);break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature);break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature);break;
        }
    }
};

CreatureAI* GetAI_boss_krik_thir(Creature *_Creature)
{
    return new boss_krik_thirAI (_Creature);
}

void AddSC_boss_krik_thir()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_krik_thir";
    newscript->GetAI = GetAI_boss_krik_thir;
    newscript->RegisterSelf();

}
