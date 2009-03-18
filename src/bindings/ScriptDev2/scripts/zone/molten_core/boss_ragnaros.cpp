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
SDName: Boss_Ragnaros
SD%Complete: 90
SDComment: ready for testing
SDCategory: Molten Core
EndScriptData */

#include "precompiled.h"

#define SAY_REINFORCEMENTS1         -1409013
#define SAY_REINFORCEMENTS2         -1409014
#define SAY_HAND                    -1409015
#define SAY_WRATH                   -1409016
#define SAY_KILL                    -1409017
#define SAY_MAGMABURST              -1409018

#define SPELL_HANDOFRAGNAROS        19780
#define SPELL_WRATHOFRAGNAROS       20566
#define SPELL_LAVABURST             21158

#define SPELL_MAGMABURST            20565       //Ranged attack

#define SPELL_SONSOFFLAME_DUMMY     21108       //Server side effect
#define SPELL_RAGSUBMERGE           21107       //Stealth aura
#define SPELL_RAGSUBMERGEVISUAL		20567
#define SPELL_RAGEMERGE             20568
#define SPELL_MELTWEAPON            21388
#define SPELL_ELEMENTALFIRE         20564
#define SPELL_ERRUPTION             17731

#define SAY_ARRIVAL1_RAG    -1409009
#define SAY_ARRIVAL3_RAG    -1409011
#define SAY_ARRIVAL5_RAG    -1409012
//#define SAY_ARRIVAL_1       "ZU FRÜH! IHR HABT MICH ZU FRÜH ERWECKT, EXEKUTOR! WAS HAT DIESE STÖRUNG ZU BEDEUTEN?"
//#define SAY_ARRIVAL_3       "NARR! DU LÄSST ES ZU DAS SICH DIESE INSEKTEN IM GEHEILIGTEN KERN HERUMTREIBEN, UND NUN FÜHRST DU SIE AUCH NOCH IN MEINE HEIMSTÄTTE? DU ENTTÄUSCHT MICH, EXECUTUS! IN DER TAT WERDE ICH GERECHTIGKRILT WALTEN LASSEN!"
//#define SAY_ARRIVAL_5       "NUN ZU EUCH, INSEKTEN. MUTIG HABT IHR NACH DER MACHT VON RAGNAROS GESUCHT. NUN SOLLT IHR SIE AUS ERSTER HAND ERFAHREN."

#define SOUND_ARRIVAL_1         8043
#define SOUND_ARRIVAL_3         8044
#define SOUND_ARRIVAL_5         8045
#define SOUND_REINFORCEMENTS1   8049
#define SOUND_REINFORCEMENTS2   8050
#define SOUND_HAND              8046
#define SOUND_WRATH             8047
#define SOUND_KILL              8051
#define SOUND_MAGMABURST        8048

#define ADD_1X 848.740356
#define ADD_1Y -816.103455
#define ADD_1Z -229.74327
#define ADD_1O 2.615287

#define ADD_2X 852.560791
#define ADD_2Y -849.861511
#define ADD_2Z -228.560974
#define ADD_2O 2.836073  

#define ADD_3X 808.710632
#define ADD_3Y -852.845764
#define ADD_3Z -227.914963
#define ADD_3O 0.964207

#define ADD_4X 786.597107
#define ADD_4Y -821.132874
#define ADD_4Z -226.350128
#define ADD_4O 0.949377

#define ADD_5X 796.219116
#define ADD_5Y -800.948059
#define ADD_5Z -226.010361
#define ADD_5O 0.560603

#define ADD_6X 821.602539
#define ADD_6Y -782.744109
#define ADD_6Z -226.023575
#define ADD_6O 6.157440

#define ADD_7X 844.924744
#define ADD_7Y -769.453735
#define ADD_7Z -225.521698
#define ADD_7O 4.4539958

#define ADD_8X 839.823364
#define ADD_8Y -810.869385
#define ADD_8Z -229.683182
#define ADD_8O 4.693108

struct MANGOS_DLL_DECL boss_ragnarosAI : public Scripted_NoMovementAI
{
    boss_ragnarosAI(Creature *c) : Scripted_NoMovementAI(c) {Reset();BeginEvent();}

    uint32 WrathOfRagnaros_Timer;
    uint32 HandOfRagnaros_Timer;
    uint32 LavaBurst_Timer;
    uint32 MagmaBurst_Timer;
    uint32 ElementalFire_Timer;
    uint32 Erruption_Timer;
    uint32 Submerge_Timer;
    uint32 Attack_Timer;
    Creature *Summoned;
    bool HasYelledMagmaBurst;
    bool HasSubmergedOnce;
    bool WasBanished; 
    bool HasAura;

	bool Speech1, Speech2, AlterFaction;
	uint32 Speech1_Timer, Speech2_Timer, AlterFaction_Timer;

    void Reset()
    {
        WrathOfRagnaros_Timer = 30000;
        HandOfRagnaros_Timer = 25000;
        LavaBurst_Timer = 10000;
        MagmaBurst_Timer = 2000;
        Erruption_Timer = 15000;
        ElementalFire_Timer = 3000;
        Submerge_Timer = 180000;
        Attack_Timer = 90000;
        HasYelledMagmaBurst = false;
        HasSubmergedOnce = false;
        WasBanished = false; 

        m_creature->CastSpell(m_creature,SPELL_MELTWEAPON,true);
        HasAura = true;

		Speech1 = Speech2 = AlterFaction = false;
		Speech1_Timer=9000;
		Speech2_Timer=25000;
		AlterFaction_Timer = 37000;
    }

	void BeginEvent()
    {
		Speech1=true;
	}

    void KilledUnit(Unit* victim)
    {
        if (rand()%5)
            return;

        DoScriptText(SAY_KILL, m_creature);
    }

    void Aggro(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
		if(Speech1){
			if(Speech1_Timer <diff){
				DoScriptText(SAY_ARRIVAL1_RAG, m_creature);
				//m_creature->Yell(SAY_ARRIVAL_1, LANG_UNIVERSAL, 0);
				DoPlaySoundToSet(m_creature,SOUND_ARRIVAL_1);
				Speech2=true;
				Speech1=false;
			}else Speech1_Timer -= diff;
		}

		if(Speech2){
			if(Speech2_Timer<diff){
				DoScriptText(SAY_ARRIVAL3_RAG, m_creature);
				//m_creature->Yell(SAY_ARRIVAL_3, LANG_UNIVERSAL, 0);
				DoPlaySoundToSet(m_creature,SOUND_ARRIVAL_3);

				Speech2=false;
				AlterFaction = true;
			}else Speech2_Timer -=diff;
		}

		if(AlterFaction){
			if(AlterFaction_Timer < diff){
				AlterFaction=false;
				DoScriptText(SAY_ARRIVAL5_RAG, m_creature);
				//m_creature->Yell(SAY_ARRIVAL_5, LANG_UNIVERSAL, 0);
				DoPlaySoundToSet(m_creature,SOUND_ARRIVAL_5);
				m_creature->setFaction(91);
			}else AlterFaction_Timer -= diff;
		}

        if (WasBanished && Attack_Timer < diff)
        {
            //Become unbanished again 
            m_creature->setFaction(14);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            DoCast(m_creature,SPELL_RAGEMERGE);
            WasBanished = false;
        } else if (WasBanished)
        {
            Attack_Timer -= diff;
            //Do nothing while banished
            return;
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //WrathOfRagnaros_Timer
        if (WrathOfRagnaros_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_WRATHOFRAGNAROS);

            if (rand()%2 == 0)
            {
                //Say our dialog
                DoScriptText(SAY_WRATH, m_creature);
                DoPlaySoundToSet(m_creature,SOUND_WRATH);
            }

            //60 seconds until we should cast this agian
            WrathOfRagnaros_Timer = 30000;
        }else WrathOfRagnaros_Timer -= diff;

        //HandOfRagnaros_Timer
        if (HandOfRagnaros_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_HANDOFRAGNAROS);

            if (rand()%2==0)
            {
                //Say our dialog
                DoScriptText(SAY_HAND, m_creature);
                DoPlaySoundToSet(m_creature,SOUND_HAND);
            }

            //60 seconds until we should cast this agian
            HandOfRagnaros_Timer = 25000;
        }else HandOfRagnaros_Timer -= diff;

        //LavaBurst_Timer
        if (LavaBurst_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_LAVABURST);

            //10 seconds until we should cast this agian
            LavaBurst_Timer = 10000;
        }else LavaBurst_Timer -= diff;

        //Erruption_Timer
        if (LavaBurst_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_ERRUPTION);

            //10 seconds until we should cast this agian
            Erruption_Timer = 20000 + rand()%25000;
        }else Erruption_Timer -= diff;

        //ElementalFire_Timer
        if (ElementalFire_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_ELEMENTALFIRE);

            //10 seconds until we should cast this agian
            ElementalFire_Timer = 10000 + rand()%4000;
        }else ElementalFire_Timer -= diff;

        //Submerge_Timer
        if (!WasBanished && Submerge_Timer < diff)
        {
            //Creature spawning and ragnaros becomming unattackable
            //is not very well supported in the core
            //so added normaly spawning and banish workaround and attack again after 90 secs.

            m_creature->InterruptNonMeleeSpells(false);
            //Root self
            DoCast(m_creature,23973);
            m_creature->setFaction(35);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			m_creature->CastSpell(m_creature,SPELL_RAGSUBMERGEVISUAL,true);
            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_SUBMERGE);

            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);



            if (!HasSubmergedOnce)
            {
                DoScriptText(SAY_REINFORCEMENTS1, m_creature);

                // summon 4 elementals
                Unit* target = NULL;
                for(int i = 0; i < 7;i++)
                {
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    Summoned = m_creature->SummonCreature(12143,target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,900000);
                    if (Summoned)
                        ((CreatureAI*)Summoned->AI())->AttackStart(target);
                }

                HasSubmergedOnce = true;
                WasBanished = true;
                DoCast(m_creature,SPELL_RAGSUBMERGE);
                Attack_Timer = 90000;

            }else
            {
                //Say our dialog
                DoScriptText(SAY_REINFORCEMENTS2, m_creature);
                DoPlaySoundToSet(m_creature,SOUND_REINFORCEMENTS2);

                Unit* target = NULL;
                for(int i = 0; i < 9;i++)
                {
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    Summoned = m_creature->SummonCreature(12143,target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,900000);
                    if (Summoned)
                        ((CreatureAI*)Summoned->AI())->AttackStart(target);
                }

                WasBanished = true;
                DoCast(m_creature,SPELL_RAGSUBMERGE);
                Attack_Timer = 90000;
            }

            //3 minutes until we should cast this agian
            Submerge_Timer = 180000;
        }else Submerge_Timer -= diff;

        //If we are within range melee the target
        if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
        {
            //Make sure our attack is ready and we arn't currently casting
            if( m_creature->isAttackReady() && !m_creature->IsNonMeleeSpellCasted(false))
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
        }else
        {
            //MagmaBurst_Timer
            if (MagmaBurst_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_MAGMABURST);

                if (!HasYelledMagmaBurst)
                {
                    DoScriptText(SAY_MAGMABURST, m_creature);
                    HasYelledMagmaBurst = true;
                }

                //3 seconds until we should cast this agian
                MagmaBurst_Timer = 2500;
            }else MagmaBurst_Timer -= diff;
        }
    }
}; 
CreatureAI* GetAI_boss_ragnaros(Creature *_Creature)
{
    return new boss_ragnarosAI (_Creature);
}


void AddSC_boss_ragnaros()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_ragnaros";
    newscript->GetAI = &GetAI_boss_ragnaros;
    newscript->RegisterSelf();
}
