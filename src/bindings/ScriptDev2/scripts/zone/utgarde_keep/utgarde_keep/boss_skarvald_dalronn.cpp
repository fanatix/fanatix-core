/* ScriptData
SDName: Boss_Skarvald_Dalronn
SD%Complete: 95
SDComment: Needs adjustments to blizzlike timers, Yell Text + Sound to DB
SDCategory: Utgarde Keep
EndScriptData */

#include "precompiled.h"
#include "sc_creature.h"
#include "def_utgarde_keep.h"

#define SAY_SKARVALD_AGGRO                          -1999680
#define SAY_DALRONN_AGGRO                           -1999685

#define SAY_SKARVALD_KILL                           -1999679
#define SAY_DALRONN_KILL                            -1999684

#define SAY_DALRONN_DAL_DIEDFIRST                   -1999683
#define SAY_SKARVALD_DAL_DIEDFIRST                  -1999676
#define SAY_SKARVALD_DAL_DIED                       -1999677

#define SAY_SKARVALD_SKA_DIEDFIRST                  -1999678
#define SAY_DALRONN_SKA_DIEDFIRST                   -1999681
#define SAY_DALRONN_SKA_DIED                        -1999682

//Spells of Skarvald and his Ghost
#define MOB_SKARVALD_THE_CONSTRUCTOR                24200
#define SPELL_CHARGE                                43651
#define SPELL_STONE_STRIKE                          48583
#define SPELL_SUMMON_SKARVALD_GHOST                 48613
#define MOB_SKARVALD_GHOST                          27390
//Spells of Dalronn and his Ghost
#define MOB_DALRONN_THE_CONTROLLER                  24201
#define SPELL_SHADOW_BOLT                           43649
#define H_SPELL_SHADOW_BOLT                         59575
#define H_SPELL_SUMMON_SKELETONS                    52611
#define SPELL_DEBILITATE                            43650
#define SPELL_SUMMON_DALRONN_GHOST                  48612
#define MOB_DALRONN_GHOST                           27389

struct MANGOS_DLL_DECL boss_skarvald_the_constructorAI : public ScriptedAI
{
    boss_skarvald_the_constructorAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        HeroicMode = m_creature->GetMap()->IsHeroic();
        Reset();
    }

    ScriptedInstance* pInstance;

    bool ghost;
    bool HeroicMode;
    uint32 Charge_Timer;
    uint32 StoneStrike_Timer;
    uint32 Response_Timer;
    uint32 Check_Timer;
    bool Dalronn_isDead;

    void Reset()
    {
        Charge_Timer = 5000;
        StoneStrike_Timer = 10000;
        Dalronn_isDead = false;
        Check_Timer = 5000;

        ghost = (m_creature->GetEntry() == MOB_SKARVALD_GHOST);
        if(!ghost)
        {
            pInstance->SetData(DATA_SKARVALD_DALRONN_EVENT,NOT_STARTED);

            Unit* dalronn = Unit::GetUnit((*m_creature),pInstance->GetData64(DATA_DALRONN));
            if(dalronn && dalronn->isDead())
                ((Creature*)dalronn)->Respawn();
        }
    }

    void Aggro(Unit *who)
    {
        if(!ghost)
        {
            DoScriptText(SAY_SKARVALD_AGGRO,m_creature);

            pInstance->SetData(DATA_SKARVALD_DALRONN_EVENT,IN_PROGRESS);
            Unit* dalronn = Unit::GetUnit((*m_creature),pInstance->GetData64(DATA_DALRONN));
            if(dalronn && dalronn->isAlive() && !dalronn->getVictim())
                dalronn->getThreatManager().addThreat(who,0.0f);
        }
    }

    void JustDied(Unit* Killer)
    {
        if(!ghost)
        {
            Unit* dalronn = Unit::GetUnit((*m_creature),pInstance->GetData64(DATA_DALRONN));
            if(dalronn)
            {
                if(dalronn->isDead())
                {
                    DoScriptText(SAY_SKARVALD_DAL_DIED,m_creature);

                    pInstance->SetData(DATA_SKARVALD_DALRONN_EVENT,DONE);
                }
                else
                {
                    DoScriptText(SAY_SKARVALD_SKA_DIEDFIRST,m_creature);

                    m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                    //DoCast(m_creature,SPELL_SUMMON_SKARVALD_GHOST,true);
                    Creature* temp = m_creature->SummonCreature(MOB_SKARVALD_GHOST,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),0,TEMPSUMMON_CORPSE_DESPAWN,5000);
                    temp->AI()->AttackStart(Killer);
                }
            }
        }
    }

    void KilledUnit(Unit *victim)
    {
        if(!ghost)
        {
            DoScriptText(SAY_SKARVALD_KILL,m_creature);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(ghost)
        {
            if(pInstance->GetData(DATA_SKARVALD_DALRONN_EVENT) != IN_PROGRESS)
            {
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
        }

       if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if(!ghost)
        {
            if(Check_Timer)
                if(Check_Timer < diff)
                {
                    Check_Timer = 5000;
                    Unit* dalronn = Unit::GetUnit((*m_creature),pInstance->GetData64(DATA_DALRONN));
                    if(dalronn && dalronn->isDead())
                    {
                        Dalronn_isDead = true;
                        Response_Timer = 2000;
                        Check_Timer = 0;
                    }
                }else Check_Timer -= diff;

            if(Response_Timer) 
                if(Dalronn_isDead)
                    if(Response_Timer < diff)
                    {
                        DoScriptText(SAY_SKARVALD_DAL_DIEDFIRST,m_creature);

                        Response_Timer = 0;
                    }else Response_Timer -= diff;
        }

        if(Charge_Timer < diff)
        {
            DoCast(SelectUnit(SELECT_TARGET_RANDOM, 1), SPELL_CHARGE);
            Charge_Timer = 5000+rand()%5000;
        }else Charge_Timer -= diff;

        if(StoneStrike_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_STONE_STRIKE);
            StoneStrike_Timer = 5000+rand()%5000;
        }else StoneStrike_Timer -= diff;
      
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_skarvald_the_constructor(Creature *_Creature)
{
    return new boss_skarvald_the_constructorAI (_Creature);
}

struct MANGOS_DLL_DECL boss_dalronn_the_controllerAI : public ScriptedAI
{
    boss_dalronn_the_controllerAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        HeroicMode = m_creature->GetMap()->IsHeroic();
        Reset();
    }

    ScriptedInstance* pInstance;

    bool ghost;
    bool HeroicMode;
    uint32 ShadowBolt_Timer;
    uint32 Debilitate_Timer;
    uint32 Summon_Timer;

    uint32 Response_Timer;
    uint32 Check_Timer;
    uint32 AggroYell_Timer;
    bool Skarvald_isDead;


    void Reset()
    {
        ShadowBolt_Timer = 1000;
        Debilitate_Timer = 5000;
        Summon_Timer = 10000;
        Check_Timer = 5000;
        Skarvald_isDead = false;
        AggroYell_Timer = 0;

        ghost = m_creature->GetEntry() == MOB_DALRONN_GHOST;
        if(!ghost)
        {
            pInstance->SetData(DATA_SKARVALD_DALRONN_EVENT,NOT_STARTED);

            Unit* skarvald = Unit::GetUnit((*m_creature),pInstance->GetData64(DATA_SKARVALD));
            if(skarvald && skarvald->isDead())
                ((Creature*)skarvald)->Respawn();
        }
    }
    void Aggro(Unit *who)
    {
        if(!ghost)
        {
            pInstance->SetData(DATA_SKARVALD_DALRONN_EVENT,IN_PROGRESS);
            Unit* skarvald = Unit::GetUnit((*m_creature),pInstance->GetData64(DATA_SKARVALD));
            if(skarvald && skarvald->isAlive() && !skarvald->getVictim())
                skarvald->getThreatManager().addThreat(who,0.0f);

            AggroYell_Timer = 5000;
        }
    }

    void JustDied(Unit* Killer)
    {
        if(!ghost)
        {
            Unit* skarvald = Unit::GetUnit((*m_creature),pInstance->GetData64(DATA_SKARVALD));
            if(skarvald)
                if(skarvald->isDead())
                {
                    DoScriptText(SAY_DALRONN_SKA_DIED,m_creature);

                    pInstance->SetData(DATA_SKARVALD_DALRONN_EVENT,DONE);
                }
                else
                {
                    DoScriptText(SAY_DALRONN_DAL_DIEDFIRST,m_creature);

                    m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                    //DoCast(m_creature,SPELL_SUMMON_DALRONN_GHOST,true);
                    Creature* temp = m_creature->SummonCreature(MOB_DALRONN_GHOST,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),0,TEMPSUMMON_CORPSE_DESPAWN,5000);
                    temp->AI()->AttackStart(Killer);
                }
        }
    }

    void KilledUnit(Unit *victim)
    {
        if(!ghost)
        {
            DoScriptText(SAY_DALRONN_KILL,m_creature);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(ghost)
        {
            if(pInstance->GetData(DATA_SKARVALD_DALRONN_EVENT) != IN_PROGRESS)
            {
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);  
            }
        }

        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(AggroYell_Timer)
            if(AggroYell_Timer < diff)
            {
                DoScriptText(SAY_DALRONN_AGGRO,m_creature);

                AggroYell_Timer = 0;
            }else AggroYell_Timer -= diff;

        if(!ghost)
        {
            if(Check_Timer)
                if(Check_Timer < diff)
                {
                    Check_Timer = 5000;
                    Unit* skarvald = Unit::GetUnit((*m_creature),pInstance->GetData64(DATA_SKARVALD));
                    if(skarvald && skarvald->isDead())
                    {
                        Skarvald_isDead = true;
                        Response_Timer = 2000;
                        Check_Timer = 0;
                    }
                }else Check_Timer -= diff;

            if(Response_Timer)
                if(Skarvald_isDead)
                    if(Response_Timer < diff)
                    {
                        DoScriptText(SAY_DALRONN_SKA_DIEDFIRST,m_creature);

                        Response_Timer = 0;
                    }else Response_Timer -= diff;
        }

        if(ShadowBolt_Timer < diff)
        {
            if (!m_creature->IsNonMeleeSpellCasted(false))
            {
                DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0),HeroicMode ? H_SPELL_SHADOW_BOLT : SPELL_SHADOW_BOLT);
                ShadowBolt_Timer = 1000;
            }
        }else ShadowBolt_Timer -= diff;

        if(Debilitate_Timer < diff)
        {
            if (!m_creature->IsNonMeleeSpellCasted(false))
             {
                DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0),SPELL_DEBILITATE);
                Debilitate_Timer = 5000+rand()%5000;
            }
        }else Debilitate_Timer -= diff;
      
        if(HeroicMode)
            if(Summon_Timer < diff)
            {
                DoCast(m_creature,H_SPELL_SUMMON_SKELETONS);
                Summon_Timer = (rand()%10000) + 20000;
            }else Summon_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_dalronn_the_controller(Creature *_Creature)
{
    return new boss_dalronn_the_controllerAI (_Creature);
}

void AddSC_boss_skarvald_dalronn()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_skarvald_the_constructor";
    newscript->GetAI = &GetAI_boss_skarvald_the_constructor;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name="boss_dalronn_the_controller";
    newscript->GetAI = &GetAI_boss_dalronn_the_controller;
    newscript->RegisterSelf();
}
