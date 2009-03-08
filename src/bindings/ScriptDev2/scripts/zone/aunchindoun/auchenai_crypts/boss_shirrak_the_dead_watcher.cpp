/* Copyright © 2006 - 2008 ScriptDev2 
<https://scriptdev2.svn.sourceforge.net/> * This program is free software; you 
can redistribute it and/or modify * it under the terms of the GNU General Public 
License as published by * the Free Software Foundation; either version 2 of the 
License, or * (at your option) any later version. * * This program is 
distributed in the hope that it will be useful, * but WITHOUT ANY WARRANTY; 
without even the implied warranty of * MERCHANTABILITY or FITNESS FOR A 
PARTICULAR PURPOSE. See the * GNU General Public License for more details. * * 
You should have received a copy of the GNU General Public License * along with 
this program; if not, write to the Free Software * Foundation, Inc., 59 Temple 
Place, Suite 330, Boston, MA 02111-1307 USA */

/* ScriptData
SDName: Boss_Shirrak_the_Dead_Watcher
SD%Complete: 100
SDComment: 
SDCategory: Auchindoun, Crypts
EndScriptData */

#include "precompiled.h"
#include "sc_creature.h"

#define SPELL_INHIBIT_MAGIC 32264
#define SPELL_ATTRACT_MAGIC 32265
#define SPELL_CARNIVOROUS_BITE 41092
#define SPELL_FOCUS_FIRE 32302

#define MESSAGE_FOCUS_FIRE "focuses his energy on $N"

#define MOB_FOCUS_FIRE_DUMMY 18374

/* Shirrak the Dead Watcher AI */

struct MANGOS_DLL_DECL boss_shirrak_the_dead_watcherAI : public ScriptedAI
{
boss_shirrak_the_dead_watcherAI(Creature *c) : ScriptedAI (c) {Reset();} 

uint32 Inhibit_Magic_Timer;
uint32 Attract_Magic_Timer;
uint32 Carnivorous_Bite_Timer;
uint32 Focus_Fire_Timer;
uint32 Intern_Focus_Fire_Timer;
uint32 Intern_Focus_Fire_Counter;
Unit *target;

void Reset()
{ 
Inhibit_Magic_Timer = 3000;
Attract_Magic_Timer = 30000;
Carnivorous_Bite_Timer = 6000;
Focus_Fire_Timer = 20000;
Intern_Focus_Fire_Timer = 0;
Intern_Focus_Fire_Counter = 0;
target = NULL;
}

void Aggro(Unit*){}

void UpdateAI(const uint32 diff)
{
// Cast Inhibit Magic even if out of combat
if(Inhibit_Magic_Timer < diff)
{
DoCast(m_creature, SPELL_INHIBIT_MAGIC);
Inhibit_Magic_Timer = 3000;
}else Inhibit_Magic_Timer -= diff;

if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
return;

// Focus Fire
if(Focus_Fire_Timer < diff)
{ 
if(Intern_Focus_Fire_Timer < 400 && Intern_Focus_Fire_Counter == 0)
{
target = SelectUnit(SELECT_TARGET_RANDOM, 0);

if(target)
{
DoTextEmote(MESSAGE_FOCUS_FIRE, target);
Creature* FocusFireDummy = m_creature->SummonCreature(MOB_FOCUS_FIRE_DUMMY, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0.0,TEMPSUMMON_TIMED_DESPAWN, 4000);
Intern_Focus_Fire_Counter++;
}else
Intern_Focus_Fire_Counter = 4;
}
else if(Intern_Focus_Fire_Timer > 400 && Intern_Focus_Fire_Timer < 800 && Intern_Focus_Fire_Counter == 1)
{
if(target)
{
Creature* FocusFireDummy = m_creature->SummonCreature(MOB_FOCUS_FIRE_DUMMY, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0.0,TEMPSUMMON_TIMED_DESPAWN, 4000);
Intern_Focus_Fire_Counter++;
}else
Intern_Focus_Fire_Counter = 4;
}
else if(Intern_Focus_Fire_Timer > 800 && Intern_Focus_Fire_Timer < 1200 && Intern_Focus_Fire_Counter == 2)
{
if(target)
{
Creature* FocusFireDummy = m_creature->SummonCreature(MOB_FOCUS_FIRE_DUMMY, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0.0,TEMPSUMMON_TIMED_DESPAWN, 4000);
Intern_Focus_Fire_Counter++;
}else
Intern_Focus_Fire_Counter = 4;
}
else if(Intern_Focus_Fire_Timer > 1200 && Intern_Focus_Fire_Counter == 3)
{
if(target)
{
Creature* FocusFireDummy = m_creature->SummonCreature(MOB_FOCUS_FIRE_DUMMY, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0.0,TEMPSUMMON_TIMED_DESPAWN, 4000);
Intern_Focus_Fire_Counter++;
}else
Intern_Focus_Fire_Counter = 4;
}else if(Intern_Focus_Fire_Counter == 4)
{
Focus_Fire_Timer = 20000;
Intern_Focus_Fire_Timer = 0;
Intern_Focus_Fire_Counter = 0;
}else
Intern_Focus_Fire_Timer += diff;
}else Focus_Fire_Timer -= diff;

// Attract Magic
if(Attract_Magic_Timer < diff)
{
DoCast(m_creature, SPELL_ATTRACT_MAGIC);
Attract_Magic_Timer = 30000;
}else Attract_Magic_Timer -= diff;

// Carnivorous Bite
if(Carnivorous_Bite_Timer < diff)
{
DoCast(m_creature->getVictim(), SPELL_CARNIVOROUS_BITE);
Carnivorous_Bite_Timer = 6000;
}else Carnivorous_Bite_Timer -= diff;

DoMeleeAttackIfReady();
}
};

/* Shirrak the Dead Watcher AI End */

/* Focus Fire Dummy AI */

struct MANGOS_DLL_DECL mob_focusfireAI : public Scripted_NoMovementAI
{
mob_focusfireAI(Creature *c) : Scripted_NoMovementAI (c) {Reset();}

uint32 Focus_Fire_Effect_Timer;
bool Is_Casted;

void Reset()
{
Focus_Fire_Effect_Timer = 3000;
Is_Casted = false;
}

void Aggro(Unit*){return;}

void MoveInLineOfSight(Unit* who){return;}

void DamageTaken(Unit *done_by, uint32 &damage) { damage = 0; }

void UpdateAI(const uint32 diff)
{
if(Focus_Fire_Effect_Timer < diff && !Is_Casted)
{
DoCast(m_creature, SPELL_FOCUS_FIRE);
Is_Casted = true;
}else Focus_Fire_Effect_Timer -= diff;
}
};

/* Focus Fire Dummy AI END */

CreatureAI* GetAI_boss_shirrak_the_dead_watcherAI(Creature *_Creature)
{
return new boss_shirrak_the_dead_watcherAI (_Creature);
}

void AddSC_boss_shirrak_the_dead_watcher()
{
Script *newscript;
newscript = new Script;
newscript->Name="boss_shirrak_the_dead_watcher";
newscript->GetAI = &GetAI_boss_shirrak_the_dead_watcherAI;
newscript->RegisterSelf();
}

CreatureAI* GetAI_mob_focusfireAI(Creature *_Creature)
{
return new mob_focusfireAI (_Creature);
}

void AddSC_mob_focusfire()
{
Script *newscript;
newscript = new Script;
newscript->Name="mob_focusfire";
newscript->GetAI = &GetAI_mob_focusfireAI;
newscript->RegisterSelf();
}