/* Script Data Start
SDName: Boss meathook
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_meathook' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spell
#define SPELL_CONSTRICTING_CHAINS_N                  52696 //Encases the targets in chains, dealing 1800 Physical damage every 1 sec. and stunning the target for 5 sec.
#define SPELL_CONSTRICTING_CHAINS_H                  58823 
#define SPELL_DISEASE_EXPULSION_N                    52666 //Meathook belches out a cloud of disease, dealing 1710 to 1890 Nature damage and interrupting the spell casting of nearby enemy targets for 4 sec.
#define SPELL_DISEASE_EXPULSION_H                    58824
#define SPELL_FRENZY                                 58841 //Increases the caster's Physical damage by 10% for 30 sec.

//Yell
#define SAY_AGGRO                                 -1999910
#define SAY_SLAY_1                                -1999909
#define SAY_SLAY_2                                -1999908
#define SAY_SLAY_3                                -1999907
#define SAY_SPAWN                                 -1999906
#define SAY_DEATH                                 -1999905

struct MANGOS_DLL_DECL boss_meathookAI : public ScriptedAI
{
    boss_meathookAI(Creature *c) : ScriptedAI(c) { Reset(); }

    uint32 Chain_Timer;
    uint32 Disease_Timer;
    uint32 Frenzy_Timer;

    void Reset() 
    {
        Chain_Timer =   12000 + rand()%5000;   //seen on video 13, 17, 15, 12, 16
        Disease_Timer =  2000 + rand()%1000;  //approx 3s
        Frenzy_Timer =  20000 + rand()%10000; //made it up
    }
    
    void Aggro(Unit* who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }
    
    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who, const uint32 diff) {}
    
    void UpdateAI(const uint32 diff) 
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
    
        if(Disease_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_DISEASE_EXPULSION_N);
            Disease_Timer = 1500 + rand()%2500;
        }else Disease_Timer -= diff;
        
        if(Frenzy_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_FRENZY);
            Frenzy_Timer = 20000 + rand()%10000;
        }else Frenzy_Timer -= diff;
        
        if(Chain_Timer < diff)
        {
            switch(rand()%2) //Idk which one is it.
            {
            case 0: 
                DoCast(SelectUnit(SELECT_TARGET_RANDOM, 1), SPELL_CONSTRICTING_CHAINS_N); //anyone but the tank
                break;
            case 1: 
                DoCast(SelectUnit(SELECT_TARGET_BOTTOMAGGRO, 0), SPELL_CONSTRICTING_CHAINS_N);
                break;
                Chain_Timer = 2000 + rand()%1000;
            }        
        }else Chain_Timer -= diff;
        
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

CreatureAI* GetAI_boss_meathook(Creature *_Creature)
{
    return new boss_meathookAI (_Creature);
}

void AddSC_boss_meathook()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_meathook";
    newscript->GetAI = GetAI_boss_meathook;
    newscript->RegisterSelf();
}
