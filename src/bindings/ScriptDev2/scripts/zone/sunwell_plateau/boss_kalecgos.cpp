/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Kalecgos
SD%Complete: 0
SDComment: You don't want to see this, do you ;-)
SDCategory: Sunwell Plateau
EndScriptData */

#include "precompiled.h"
#include "WorldPacket.h"
#include "sc_creature.h"
#include "sc_instance.h"
#include "def_sunwell_plateau.h"
#include "GameObject.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"

//kalecgos dragon form
#define SAY_EVIL_AGGRO                  -1580000
#define SAY_EVIL_SPELL1                 -1580001
#define SAY_EVIL_SPELL2                 -1580002
#define SAY_EVIL_SLAY1                  -1580003
#define SAY_EVIL_SLAY2                  -1580004
#define SAY_EVIL_ENRAGE                 -1580005
//kalecgos humanoid form
#define SAY_GOOD_AGGRO                  -1580006
#define SAY_GOOD_NEAR_DEATH             -1580007
#define SAY_GOOD_NEAR_DEATH2            -1580008
#define SAY_GOOD_PLRWIN                 -1580009

#define SAY_SATH_AGGRO                  -1580010
#define SAY_SATH_DEATH                  -1580011
#define SAY_SATH_SPELL1                 -1580012
#define SAY_SATH_SPELL2                 -1580013
#define SAY_SATH_SLAY1                  -1580014
#define SAY_SATH_SLAY2                  -1580015
#define SAY_SATH_ENRAGE                 -1580016

#define SPELL_SPECTRAL_EXHAUSTION       44867
#define SPELL_TELEPORT_SPECTRAL_REALM   46019
#define SPELL_SPECTRAL_REALM            46021

#define NOTIFY_SPECTRALLY_EXHAUSTED     "Your body is too exhausted to travel to the Spectral Realm."
#define ERROR_INST_DATA                 "SD2: Instance Data not set properly for Sunwell Plateau. Kalecgos Encounter will be buggy."
#define ERROR_INST_DATA_PLR             "SD2 ERROR: Instance Data not set properly for Sunwell Plateau. Please report this to your administrator."
#define ERROR_UNABLE_TO_TELEPORT        "SD2: Unable to select target for Spectral Blast. Threatlist has too few players."
#define ERROR_MISSING_TELEPORT_GUID     "SD2: [Kalecgos] Invalid TeleportTargetGUID. Unable to teleport player."
#define ERROR_KALECGOS_NOT_FOUND        "SD2: Unable to create pointer to Kalecgos from Sathrovarr."

#define KALECGOS_ARENA_X                1704.34
#define KALECGOS_ARENA_Y                928.17
#define KALECGOS_ARENA_Z                53.08

/*** Kalecgos ****/
#define SPELL_SPECTRAL_BLAST            44866
#define SPELL_ARCANE_BUFFET             45018
#define SPELL_FROST_BREATH              44799
#define SPELL_HEROIC_STRIKE             45026
#define SPELL_REVITALIZE                45027
#define SPELL_TAIL_LASH                 45122
#define SPELL_TRANSFORM_KALEC           45027
#define SPELL_CRAZED_RAGE               44806
#define SPELL_GEZEITENSCHILD            39872

uint32 WildMagic[]= { 44978, 45001, 45002, 45004, 45006, 45010 };

/*** Sathrovarr ***/
#define SPELL_CORRUPTING_STRIKE         45029
#define SPELL_CURSE_OF_BOUNDLESS_AGONY  45032
#define SPELL_SHADOW_BOLT_VOLLEY        45031

/*** Misc ***/
#define SPELL_BANISH                    44836

#define SAY_SATH_GETS_DAMAGE            -1600001     //Immer wenn Sathrovarr Schaden bekommt
#define SAY_SATH_I_AM_HERE				-1911001	 //Sathrovarr erscheint und will Ärger anzetteln

#define SAY_KALE_NEARLY_DEAD_HEAL       -1910001     //Kalecgos ist fast tot - Heilt sich nun!
#define SAY_KALE_FINALLY_FREE           -1910002     //Kalecgos wurde endlich getötet
#define SAY_KALE_RUNAWAY_PLEASE	     -1910003	 //Kalecgos versucht den Raid zu warnen
#define SAY_KALE_HE_IS_COMING	     -1910004	 //Kalecgos warnt den Raid das Sathrovarr kommt
#define SAY_KALE_HE_IS_THERE	     -1910005	 //Kalecgos kündigt das Erscheinen von Sathrovarr an
#define SAY_KALE_FIGHT_START            -1910006     //Der Kampf beginnt, Kalecgos fragt woher der Raid kommt

void ResetSathro(ScriptedInstance* instance, Creature* Caller);

/*###################################################################
#####################################################################
########################## KALECGOS #################################
#####################################################################
#####################################################################*/
struct MANGOS_DLL_DECL boss_kalecgosAI : public ScriptedAI
{
    boss_kalecgosAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint64 TeleportTargetGUID;

    uint32 ArcaneBuffetTimer;
    uint32 FrostBreathTimer;
    uint32 WildMagicTimer;
    uint32 SpectralBlastTimer;
    uint32 SpectralTeleportTimer;
    uint32 ForceFieldTimer;
    uint32 ExitTimer;
    uint32 SathrovarrTimer;
    uint32 Warning1saying;
    uint32 Warning2saying;
    uint32 PlazaClosingTimer;

    bool LockedArena;
    bool Uncorrupted;
    bool Banished;
    bool Checked;
    bool Enraged;
    bool SathroDead;
    bool Sathrospawnd;
    bool Warning1said;
    bool Warning2said;
    bool KalecDoorsClosed;

    void Reset()
    {
        TeleportTargetGUID = 0;

        // TODO: Fix timers
        ArcaneBuffetTimer       = 8000;
        FrostBreathTimer        = 24000;
        WildMagicTimer          = 18000;
        SathrovarrTimer         = 60000;
	 Warning1saying	    = 20000;
	 Warning2saying	    = 40000;
        SpectralBlastTimer      = 30000;
        SpectralTeleportTimer   = SpectralBlastTimer + 2000;
        PlazaClosingTimer       = 2500;

        ForceFieldTimer = 20000;
        ExitTimer = 0;

        LockedArena = false;
        KalecDoorsClosed = false;
        Uncorrupted = false;
        Banished    = false;
        Checked     = false;
        Enraged     = false;
        SathroDead  = false;
        Sathrospawnd = false;
	 Warning1said = false;
	 Warning2said = false;
		
	 if (GameObject* Gate1 = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_KALECGOS_FIGHT_GATE_A)))
		Gate1->SetGoState(0);
        if (GameObject* Gate2 = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_KALECGOS_FIGHT_GATE_B)))
		Gate2->SetGoState(0);
	 
	 ResetSathro(pInstance, m_creature);
    }


    void Aggro(Unit* who)
    {
        DoScriptText(SAY_KALE_FIGHT_START, m_creature);
    }

    // Dient dazu, dass Outro zu starten.
    // In meiner Version dient es dazu, Kalecgos heilen zu lassen falls er fast stirbt
    void DamageTaken(Unit* done_by, uint32 &damage)
    {
        if (damage > m_creature->GetHealth() && done_by != m_creature)
        {
            if (!SathroDead)
            {
                damage = 0;
                Unit* Kalecgos = Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_KALECGOS_DRAGON));
                if (Kalecgos)
                {
                      DoScriptText(SAY_KALE_NEARLY_DEAD_HEAL, m_creature);
                      Kalecgos->CastSpell(Kalecgos, SPELL_GEZEITENSCHILD, true);
                }
            }
           // else
           // {
               // DoScriptText(SAY_KALE_FINALLY_FREE, m_creature);
		//  if(pInstance)
               //      pInstance->SetData(DATA_KALECGOS_EVENT, DONE);
       
               // if(GameObject* Gate = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_KALECGOS_GATE)))
	       //         Gate->SetGoState(0);
                   
	 	//if (GameObject* Gate1 = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_KALECGOS_FIGHT_GATE_A)))
		//	Gate1->SetGoState(0);
        	//if (GameObject* Gate2 = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_KALECGOS_FIGHT_GATE_B)))
		//	Gate2->SetGoState(0);
            //}
        }
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_EVIL_SLAY1, m_creature); DoPlaySoundToSet(m_creature, 12425); break;
            case 1: DoScriptText(SAY_EVIL_SLAY2, m_creature); DoPlaySoundToSet(m_creature, 12426); break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_KALE_FINALLY_FREE, m_creature);
	if(pInstance)
            pInstance->SetData(DATA_KALECGOS_EVENT, DONE);
       
        if(GameObject* Gate = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_KALECGOS_GATE)))
	    Gate->SetGoState(0);     
	if (GameObject* Gate1 = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_KALECGOS_FIGHT_GATE_A)))
	    Gate1->SetGoState(0);
        if (GameObject* Gate2 = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_KALECGOS_FIGHT_GATE_B)))
	    Gate2->SetGoState(0);
    }

    void UpdateAI(const uint32 diff)
    {
	 if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 10) && !Enraged)
        {
            Unit* Sathrovarr = Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_SATHROVARR));
            if (Sathrovarr)
                Sathrovarr->CastSpell(Sathrovarr, SPELL_CRAZED_RAGE, true);
            DoCast(m_creature, SPELL_CRAZED_RAGE, true);
            Enraged = true;
        }
        
       if (PlazaClosingTimer < diff && !KalecDoorsClosed)
        {
	 	if (GameObject* Gate1 = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_KALECGOS_FIGHT_GATE_A)))
			Gate1->SetGoState(1);
        	if (GameObject* Gate2 = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_KALECGOS_FIGHT_GATE_B)))
			Gate2->SetGoState(1);
             
             KalecDoorsClosed = true;
             
        }else PlazaClosingTimer -= diff;


        if (ArcaneBuffetTimer < diff)
        {
            if (rand()%3 == 0 && Sathrospawnd)
            {    DoScriptText(SAY_EVIL_SPELL1, m_creature);
		   DoPlaySoundToSet(m_creature, 12423);
	     }

            DoCast(m_creature->getVictim(), SPELL_ARCANE_BUFFET);
            ArcaneBuffetTimer = 20000;
        }else ArcaneBuffetTimer -= diff;

        if (FrostBreathTimer < diff)
        {
            if (rand()%2 == 0 && Sathrospawnd)
            {    DoScriptText(SAY_EVIL_SPELL2, m_creature);
		   DoPlaySoundToSet(m_creature, 12424);
	     }

            DoCast(m_creature->getVictim(), SPELL_FROST_BREATH);
            FrostBreathTimer = 25000;
        }else FrostBreathTimer -= diff;

        if (WildMagicTimer < diff)
        {
            DoCast(m_creature->getVictim(), WildMagic[rand()%6]);
            WildMagicTimer = 19000;
        }else WildMagicTimer -= diff;

        // Sathrovarr spawnen lassen
        if (SathrovarrTimer < diff && !Sathrospawnd)
        {
        	 if (Creature* Sath = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_SATHROVARR))))
        	 {
               	Sath->SetVisibility(VISIBILITY_ON);
               	Sath->setFaction(14);
        	 
		  Sathrospawnd = true;

		  DoScriptText(SAY_SATH_I_AM_HERE, Sath);

		  if(Sath) Sath->AI()->AttackStart(m_creature->getVictim());
		}		
        }else SathrovarrTimer -= diff;
		

        // Erste Warnung vor Sathrovarr aussprechen
        if ((Warning1saying < diff) && !Warning1said)
        {
	     DoScriptText(SAY_KALE_RUNAWAY_PLEASE, m_creature);
            Warning1said = true;
        }else Warning1saying -= diff;

		
        // Zweite Warnung vor Sathrovarr aussprechen
        if ((Warning2saying < diff) && !Warning2said)
        {
			DoScriptText(SAY_KALE_HE_IS_COMING, m_creature);
            Warning2said = true;
        }else Warning2saying -= diff;

	DoMeleeAttackIfReady();
    }
};

/*###################################################################
#####################################################################
######################### SATHROVARR ################################
#####################################################################
#####################################################################*/
struct MANGOS_DLL_DECL boss_sathrovarrAI : public ScriptedAI
{
    boss_sathrovarrAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 CorruptingStrikeTimer;
    uint32 CurseOfBoundlessAgonyTimer;
    uint32 ShadowBoltVolleyTimer;
    bool Banished;
    bool Enraged;

    void Reset()
    {
        // FIXME: Timers
        CorruptingStrikeTimer = 5000;
        CurseOfBoundlessAgonyTimer = 15000;
        ShadowBoltVolleyTimer = 10000;

        Banished = false;
        Enraged  = false;



	m_creature->SetVisibility(VISIBILITY_OFF);
	m_creature->setFaction(35);
    }

    void Aggro(Unit* who)
    {
	DoPlaySoundToSet(m_creature, 12451);
    }

    // Dieser Block sorgt dafür, das wenn Sathrovarr stirbt, Kalecgos befreit wird.
    void DamageTaken(Unit* done_by, uint32 &damage)
    {
        if (damage > m_creature->GetHealth())
        {

            DoScriptText(SAY_SATH_DEATH, m_creature);
	     DoPlaySoundToSet(m_creature, 12452);

            if (!pInstance)
            {
                error_log(ERROR_INST_DATA);
                return;
            }

            Creature* Kalecgos = ((Creature*)Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_KALECGOS_DRAGON)));
            if (Kalecgos)
            {
                ((boss_kalecgosAI*)Kalecgos->AI())->SathroDead = true;

            }
        }
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_SATH_SLAY1, m_creature); DoPlaySoundToSet(m_creature, 12455); break;
            case 1: DoScriptText(SAY_SATH_SLAY2, m_creature); DoPlaySoundToSet(m_creature, 12456); break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
	 if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 10) && !Enraged)
        {
            Unit* Kalecgos = Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_KALECGOS_DRAGON));
            if (Kalecgos)
                Kalecgos->CastSpell(Kalecgos, SPELL_CRAZED_RAGE, true);
            DoCast(m_creature, SPELL_CRAZED_RAGE, true);
            Enraged = true;
        }

        if (CorruptingStrikeTimer < diff)
        {
            if (rand()%2 == 0)
            {    DoScriptText(SAY_SATH_SPELL2, m_creature);
		   DoPlaySoundToSet(m_creature, 12454);
	     }


            DoCast(m_creature->getVictim(), SPELL_CORRUPTING_STRIKE);
            CorruptingStrikeTimer = 13000;
        }else CorruptingStrikeTimer -= diff;

        if (CurseOfBoundlessAgonyTimer < diff)
        {
            DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_CURSE_OF_BOUNDLESS_AGONY);
            CurseOfBoundlessAgonyTimer = 35000;
            DoCast(m_creature, SPELL_SPECTRAL_REALM, true);
        }else CurseOfBoundlessAgonyTimer -= diff;

        if (ShadowBoltVolleyTimer < diff)
        {
            if (rand()%2 == 0)
            {    DoScriptText(SAY_SATH_SPELL1, m_creature);
		   DoPlaySoundToSet(m_creature, 12453);
	     }

            DoCast(m_creature->getVictim(), SPELL_SHADOW_BOLT_VOLLEY);
            ShadowBoltVolleyTimer = 15000;
        }else ShadowBoltVolleyTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

void ResetSathro(ScriptedInstance* instance, Creature* Caller)
{
	 ScriptedInstance* pInstance;
	 pInstance = instance;
	 Creature* Sathro = ((Creature*)Unit::GetUnit(*Caller, pInstance->GetData64(DATA_SATHROVARR)));
        if (Sathro)
        {
             if (!Sathro->isAlive())
                 Sathro->Respawn();

                ((boss_sathrovarrAI*)Sathro->AI())->Reset();
                ((boss_sathrovarrAI*)Sathro->AI())->EnterEvadeMode();
                Sathro->GetMotionMaster()->MoveTargetedHome();
	 }
}

CreatureAI* GetAI_boss_kalecgos(Creature* c)
{
    return new boss_kalecgosAI(c);
}

CreatureAI* GetAI_boss_sathrovarr(Creature* c)
{
    return new boss_sathrovarrAI(c);
}


void AddSC_boss_kalecgos()
{
    Script* newscript;

    newscript = new Script;
    newscript->GetAI = &GetAI_boss_kalecgos;
    newscript->Name = "boss_kalecgos";
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->GetAI = &GetAI_boss_sathrovarr;
    newscript->Name = "boss_sathrovarr";
    newscript->RegisterSelf();
}
