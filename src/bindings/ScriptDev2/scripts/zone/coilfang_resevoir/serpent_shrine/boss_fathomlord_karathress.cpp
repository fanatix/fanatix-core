/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Boss_Fathomlord_Karathress
SD%Complete: 50
SDComment: Missing Multishot, pet, Totems, Windfury, Whirlwind
SDCategory: Coilfang Resevoir, Serpent Shrine Cavern
EndScriptData */

#include "precompiled.h"
#include "def_serpent_shrine.h"

#define SAY_AGGRO                   -1548021
#define SAY_GAIN_BLESSING           -1548022
#define SAY_GAIN_ABILITY1           -1548023
#define SAY_GAIN_ABILITY2           -1548024
#define SAY_GAIN_ABILITY3           -1548025
#define SAY_SLAY1                   -1548026
#define SAY_SLAY2                   -1548027
#define SAY_SLAY3                   -1548028
#define SAY_DEATH                   -1548029

//Karathress spells
#define SPELL_CATACLYSMIC_BOLT      38441
#define SPELL_POWER_OF_SHARKKIS     38455
#define SPELL_POWER_OF_TIDALVESS    38452
#define SPELL_POWER_OF_CARIBDIS     38451
#define SPELL_ENRAGE                24318
#define SPELL_SEAR_NOVA             38445
#define SPELL_BLESSING_OF_THE_TIDES 38449
//Sharkkis spells
#define SPELL_LEECHING_THROW        29436
#define SPELL_THE_BEAST_WITHIN      38373
#define SPELL_SUMMON_FATHOM_LURKER  38433
#define SPELL_SUMMON_FATHOM_SPOREBAT 38431
//Tidalvess spells
#define SPELL_FROST_SHOCK           38234
//Caribdis Spells
#define SPELL_WATER_BOLT_VOLLEY     38335
#define SPELL_TIDAL_SURGE           38353
#define SPELL_HEAL                  38330

//entry and position for Seer Olum
#define SEER_OLUM                   22820
#define OLUM_X                      446.78f
#define OLUM_Y                      -542.76f
#define OLUM_Z                      -7.54773f
#define OLUM_O                      0.401581f

//Fathom-Lord Karathress AI
struct MANGOS_DLL_DECL boss_fathomlord_karathressAI : public ScriptedAI
{
    boss_fathomlord_karathressAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Advisors[0] = 0;
        Advisors[1] = 0;
        Advisors[2] = 0;
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 CataclysmicBolt_Timer;
    uint32 Enrage_Timer;
    bool bMobsChecked;

    uint64 Advisors[3];

    void Reset()
    {
        CataclysmicBolt_Timer = 10000;
        Enrage_Timer = 600000;                              //10 minutes
        bMobsChecked = false;

        for(uint8 i = 0; i < 3; ++i)
        {
            if (Advisors[i])
            {
                if (Creature *Advisor = (Creature*)Unit::GetUnit(*m_creature, Advisors[i]))
                {
                    if (Advisor->isAlive())
                    {
                        Advisor->DealDamage(Advisor, Advisor->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        Advisor->RemoveCorpse();
                        Advisor->Respawn();
                    }
                    Advisor->AI()->EnterEvadeMode();
                }
            }
        }

        if (pInstance)
            pInstance->SetData(DATA_KARATHRESSEVENT, NOT_STARTED);
    }

    void EventSharkkisDeath()
    {
        DoScriptText(SAY_GAIN_ABILITY1, m_creature);
        DoCast(m_creature, SPELL_POWER_OF_SHARKKIS);
    }

    void EventTidalvessDeath()
    {
        DoScriptText(SAY_GAIN_ABILITY2, m_creature);
        DoCast(m_creature, SPELL_POWER_OF_TIDALVESS);
    }

    void EventCaribdisDeath()
    {
        DoScriptText(SAY_GAIN_ABILITY3, m_creature);
        DoCast(m_creature, SPELL_POWER_OF_CARIBDIS);
    }

    void GetAdvisors()
    {
        if (!pInstance)
            return;

        Advisors[0] = pInstance->GetData64(DATA_SHARKKIS);
        Advisors[1] = pInstance->GetData64(DATA_TIDALVESS);
        Advisors[2] = pInstance->GetData64(DATA_CARIBDIS);
    }

    void StartEvent(Unit *who)
    {
        if (!pInstance)
            return;

        GetAdvisors();

        DoScriptText(SAY_AGGRO, m_creature);

        pInstance->SetData64(DATA_KARATHRESSEVENT_STARTER, who->GetGUID());
        pInstance->SetData(DATA_KARATHRESSEVENT, IN_PROGRESS);
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY3, m_creature); break;
        }
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (pInstance)
            pInstance->SetData(DATA_FATHOMLORDKARATHRESSEVENT, NOT_STARTED);

        //support for quest 10944
        m_creature->SummonCreature(SEER_OLUM, OLUM_X, OLUM_Y, OLUM_Z, OLUM_O, TEMPSUMMON_TIMED_DESPAWN, 3600000);
    }

    void Aggro(Unit *who)
    {
        StartEvent(who);
    }

    void UpdateAI(const uint32 diff)
    {
        //Only if not incombat check if the event is started
        if (!InCombat && pInstance && pInstance->GetData(DATA_KARATHRESSEVENT))
        {
            Unit* target = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_KARATHRESSEVENT_STARTER));

            if (target)
            {
                AttackStart(target);
                GetAdvisors();
            }
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //someone evaded!
        if (pInstance && !pInstance->GetData(DATA_KARATHRESSEVENT))
            EnterEvadeMode();

        //CataclysmicBolt_Timer
        if (CataclysmicBolt_Timer < diff)
        {
            //select a random unit other than the main tank
            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 1);

            //if there aren't other units, cast on the tank
            if (!target)
                target = m_creature->getVictim();

            int32 dmg = target->GetMaxHealth() / 2;
            m_creature->CastCustomSpell(target, SPELL_CATACLYSMIC_BOLT, &dmg, NULL, NULL, false, NULL, NULL, m_creature->GetGUID());

            CataclysmicBolt_Timer = 10000;
        }else CataclysmicBolt_Timer -= diff;

        //hp under 75%
        if (!bMobsChecked && (m_creature->GetHealth()/m_creature->GetMaxHealth())*100 <= 75)
        {
            for(uint8 i = 0; i < 3; ++i)
            {
                if (Unit *Advisor = Unit::GetUnit(*m_creature, Advisors[i]))
                {
                    //stack max three times (one for each alive)
                    if (Advisor->isAlive())
                    {
                        Advisor->InterruptNonMeleeSpells(false);
                        Advisor->CastSpell(m_creature, SPELL_BLESSING_OF_THE_TIDES, false);
                    }
                }
            }

            //yell if we now have the aura
            if (m_creature->HasAura(SPELL_BLESSING_OF_THE_TIDES))
                DoScriptText(SAY_GAIN_BLESSING, m_creature);

            bMobsChecked = true;
        }

        //Enrage_Timer
        if (Enrage_Timer < diff)
        {
            DoCast(m_creature, SPELL_ENRAGE);
            Enrage_Timer = 90000;
        }else Enrage_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Fathom-Guard Sharkkis AI
struct MANGOS_DLL_DECL boss_fathomguard_sharkkisAI : public ScriptedAI
{
    boss_fathomguard_sharkkisAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 LeechingThrow_Timer;
    uint32 TheBeastWithin_Timer;

    void Reset()
    {
        LeechingThrow_Timer = 20000;
        TheBeastWithin_Timer = 30000;

        if (pInstance)
            pInstance->SetData(DATA_KARATHRESSEVENT, NOT_STARTED);
    }

    void JustDied(Unit *victim)
    {
        if (pInstance)
        {
            Creature *Karathress = NULL;
            Karathress = (Creature*)(Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_KARATHRESS)));

            if (Karathress)
                ((boss_fathomlord_karathressAI*)Karathress->AI())->EventSharkkisDeath();
        }
    }

    void Aggro(Unit *who)
    {
        if (pInstance)
        {
            pInstance->SetData64(DATA_KARATHRESSEVENT_STARTER, who->GetGUID());
            pInstance->SetData(DATA_KARATHRESSEVENT, IN_PROGRESS);
        }

        // spawn one of the pets on aggro
        DoCast(m_creature, urand(0,1) ? SPELL_SUMMON_FATHOM_LURKER : SPELL_SUMMON_FATHOM_SPOREBAT);
    }

    void UpdateAI(const uint32 diff)
    {
        //Only if not incombat check if the event is started
        if (!InCombat && pInstance && pInstance->GetData(DATA_KARATHRESSEVENT))
        {
            Unit* target = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_KARATHRESSEVENT_STARTER));

            if (target)
            {
                AttackStart(target);
            }
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //someone evaded!
        if (pInstance && !pInstance->GetData(DATA_KARATHRESSEVENT))
            EnterEvadeMode();

        //LeechingThrow_Timer
        if (LeechingThrow_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_LEECHING_THROW);
            LeechingThrow_Timer = 20000;
        }else LeechingThrow_Timer -= diff;

        //TheBeastWithin_Timer
        if (TheBeastWithin_Timer < diff)
        {
            DoCast(m_creature, SPELL_THE_BEAST_WITHIN);
            TheBeastWithin_Timer = 30000;
        }else TheBeastWithin_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Fathom-Guard Tidalvess AI
struct MANGOS_DLL_DECL boss_fathomguard_tidalvessAI : public ScriptedAI
{
    boss_fathomguard_tidalvessAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 FrostShock_Timer;

    void Reset()
    {
        FrostShock_Timer = 25000;

        if (pInstance)
            pInstance->SetData(DATA_KARATHRESSEVENT, NOT_STARTED);
    }

    void JustDied(Unit *victim)
    {
        if (pInstance)
        {
            Creature *Karathress = NULL;
            Karathress = (Creature*)(Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_KARATHRESS)));

            if (Karathress)
                ((boss_fathomlord_karathressAI*)Karathress->AI())->EventTidalvessDeath();
        }
    }

    void Aggro(Unit *who)
    {
        if (pInstance)
        {
            pInstance->SetData64(DATA_KARATHRESSEVENT_STARTER, who->GetGUID());
            pInstance->SetData(DATA_KARATHRESSEVENT, IN_PROGRESS);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Only if not incombat check if the event is started
        if (!InCombat && pInstance && pInstance->GetData(DATA_KARATHRESSEVENT))
        {
            Unit* target = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_KARATHRESSEVENT_STARTER));

            if (target)
            {
                AttackStart(target);
            }
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //someone evaded!
        if (pInstance && !pInstance->GetData(DATA_KARATHRESSEVENT))
            EnterEvadeMode();

        //FrostShock_Timer
        if (FrostShock_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_FROST_SHOCK);
            FrostShock_Timer = 25000+rand()%5000;
        }else FrostShock_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Fathom-Guard Caribdis AI
struct MANGOS_DLL_DECL boss_fathomguard_caribdisAI : public ScriptedAI
{
    boss_fathomguard_caribdisAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 WaterBoltVolley_Timer;
    uint32 TidalSurge_Timer;
    uint32 Heal_Timer;

    void Reset()
    {
        WaterBoltVolley_Timer = 35000;
        TidalSurge_Timer = 15000+rand()%5000;
        Heal_Timer = 55000;

        if (pInstance)
            pInstance->SetData(DATA_KARATHRESSEVENT, NOT_STARTED);
    }

    void JustDied(Unit *victim)
    {
        if (pInstance)
        {
            Creature *Karathress = NULL;
            Karathress = (Creature*)(Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_KARATHRESS)));

            if (Karathress)
                ((boss_fathomlord_karathressAI*)Karathress->AI())->EventCaribdisDeath();
        }
    }

    void Aggro(Unit *who)
    {
        if (pInstance)
        {
            pInstance->SetData64(DATA_KARATHRESSEVENT_STARTER, who->GetGUID());
            pInstance->SetData(DATA_KARATHRESSEVENT, IN_PROGRESS);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Only if not incombat check if the event is started
        if (!InCombat && pInstance && pInstance->GetData(DATA_KARATHRESSEVENT))
        {
            Unit* target = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_KARATHRESSEVENT_STARTER));

            if (target)
            {
                AttackStart(target);
            }
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //someone evaded!
        if (pInstance && !pInstance->GetData(DATA_KARATHRESSEVENT))
            EnterEvadeMode();

        //WaterBoltVolley_Timer
        if (WaterBoltVolley_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_WATER_BOLT_VOLLEY);
            WaterBoltVolley_Timer = 30000;
        }else WaterBoltVolley_Timer -= diff;

        //TidalSurge_Timer
        if (TidalSurge_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_TIDAL_SURGE);
            TidalSurge_Timer = 15000+rand()%5000;
        }else TidalSurge_Timer -= diff;

        //Heal_Timer
        if (Heal_Timer < diff)
        {
            // It can be cast on any of the mobs
            Unit *pUnit = NULL;

            if (pInstance)
            {
                switch(rand()%4)
                {
                    case 0:
                        pUnit = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_KARATHRESS));
                        break;
                    case 1:
                        pUnit = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_SHARKKIS));
                        break;
                    case 2:
                        pUnit = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_TIDALVESS));
                        break;
                    case 3:
                        pUnit = m_creature;
                        break;
                }
            }else pUnit = m_creature;

            if (pUnit && pUnit->isAlive())
                DoCast(pUnit, SPELL_HEAL);

            Heal_Timer = 60000;
        }else Heal_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_fathomlord_karathress(Creature *_Creature)
{
    return new boss_fathomlord_karathressAI (_Creature);
}

CreatureAI* GetAI_boss_fathomguard_sharkkis(Creature *_Creature)
{
    return new boss_fathomguard_sharkkisAI (_Creature);
}

CreatureAI* GetAI_boss_fathomguard_tidalvess(Creature *_Creature)
{
    return new boss_fathomguard_tidalvessAI (_Creature);
}

CreatureAI* GetAI_boss_fathomguard_caribdis(Creature *_Creature)
{
    return new boss_fathomguard_caribdisAI (_Creature);
}

void AddSC_boss_fathomlord_karathress()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_fathomlord_karathress";
    newscript->GetAI = &GetAI_boss_fathomlord_karathress;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_fathomguard_sharkkis";
    newscript->GetAI = &GetAI_boss_fathomguard_sharkkis;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_fathomguard_tidalvess";
    newscript->GetAI = &GetAI_boss_fathomguard_tidalvess;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_fathomguard_caribdis";
    newscript->GetAI = &GetAI_boss_fathomguard_caribdis;
    newscript->RegisterSelf();
}
