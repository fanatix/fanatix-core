/* Script Data Start
SDName: Boss salramm
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_salramm' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_CURSE_OF_TWISTED_FLESH                58845
#define SPELL_EXPLODE_GHOUL_N                       52480
#define SPELL_EXPLODE_GHOUL_H                       58825
#define SPELL_SHADOW_BOLT_N                         57725
#define SPELL_SHADOW_BOLT_H                         58828
#define SPELL_STEAL_FLESH                           52708
#define SPELL_SUMMON_GHOULS                         52451

//Yell
#define SAY_AGGRO                                -1999904
#define SAY_SPAWN                                -1999903 
#define SAY_SLAY_1                               -1999902
#define SAY_SLAY_2                               -1999901     
#define SAY_SLAY_3                               -1999900        
#define SAY_DEATH                                -1999899                                
#define SAY_EXPLODE_GHOUL_1                      -1999898       
#define SAY_EXPLODE_GHOUL_2                      -1999897          
#define SAY_STEAL_FLESH_1                        -1999896      
#define SAY_STEAL_FLESH_2                        -1999895        
#define SAY_STEAL_FLESH_3                        -1999894          
#define SAY_SUMMON_GHOULS_1                      -1999893         
#define SAY_SUMMON_GHOULS_2                      -1999892                                   

struct MANGOS_DLL_DECL boss_salrammAI : public ScriptedAI
{
    boss_salrammAI(Creature *c) : ScriptedAI(c) { Reset(); }
    
    void Reset() {}
    
    void Aggro(Unit* who) 
        {DoScriptText(SAY_AGGRO, m_creature);}
    
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
    {DoScriptText(SAY_DEATH, m_creature);}
    
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

CreatureAI* GetAI_boss_salramm(Creature *_Creature)
{
    return new boss_salrammAI (_Creature);
}

void AddSC_boss_salramm()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_salramm";
    newscript->GetAI = GetAI_boss_salramm;
    newscript->RegisterSelf();
}
