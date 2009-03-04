/* Copyright ?2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Hex_Lord_Malacrass
SD%Complete:
SDComment: 
SDCategory: Zul'Aman
EndScriptData */

#include "precompiled.h"
#include "def_zulaman.h"

#define SAY_AGGRO               -1568024
#define SAY_ENRAGE              -1568025
#define SAY_KILL_ONE            -1568026
#define SAY_KILL_TWO            -1568027
#define SAY_SOUL_SIPHON         -1568028
#define SAY_DRAIN_POWER         -1568029
#define SAY_SPIRIT_BOLTS        -1568030
#define SAY_ADD_DIED_ONE        -1568031
#define SAY_ADD_DIED_TWO        -1568032
#define SAY_ADD_DIED_THREE      -1568033
#define SAY_DEATH_ONE           -1568034
#define SAY_DEATH_TWO           -1568035

#define SPELL_SPIRIT_BOLTS      43383
#define SPELL_SIPHON_SOUL       43501
#define SPELL_DRAIN_POWER       44131

#define WEAPON_ID               33494

#define MOB_TEMP_TRIGGER        23920

//Defines for various powers he uses after using soul drain

//Druid
#define SPELL_DR_THORNS         43420
#define SPELL_DR_LIFEBLOOM      43421
#define SPELL_DR_MOONFIRE       43545

//Hunter
#define SPELL_HU_EXPLOSIVE_TRAP 43444
#define SPELL_HU_FREEZING_TRAP  43447
#define SPELL_HU_SNAKE_TRAP     43449

//Mage
#define SPELL_MG_FIREBALL       41383
#define SPELL_MG_FROST_NOVA     43426
#define SPELL_MG_ICE_LANCE      43427
#define SPELL_MG_FROSTBOLT      43428

//Paladin
#define SPELL_PA_CONSECRATION   43429
#define SPELL_PA_AVENGING_WRATH 43430
#define SPELL_PA_HOLY_LIGHT     43451

//Priest
#define SPELL_PR_HEAL           41372
#define SPELL_PR_MIND_BLAST     41374
#define SPELL_PR_SW_DEATH       41375
#define SPELL_PR_PSYCHIC_SCREAM 43432
#define SPELL_PR_MIND_CONTROL   43550
#define SPELL_PR_PAIN_SUPP      44416

//Rogue
#define SPELL_RO_WOUND_POISON   39665
#define SPELL_RO_BLIND          43433
#define SPELL_RO_SLICE_DICE     43457

//Shaman
#define SPELL_SH_CHAIN_LIGHT    43435
#define SPELL_SH_FIRE_NOVA      43436
#define SPELL_SH_HEALING_WAVE   43548

//Warlock
#define SPELL_WL_CURSE_OF_DOOM  43439
#define SPELL_WL_RAIN_OF_FIRE   43440
#define SPELL_WL_UNSTABLE_AFFL  35183

//Warrior
#define SPELL_WR_MORTAL_STRIKE  43441
#define SPELL_WR_WHIRLWIND      43442
#define SPELL_WR_SPELL_REFLECT  43443

#define ORIENT                  1.5696
#define POS_Y                   921.2795
#define POS_Z                   33.8883

static float Pos_X[4] = {127.8827, 122.8827, 112.8827, 107.8827};

static uint32 AddEntryList[8]=
{
    //Far Left
    24240, //Alyson Antille
    24241, //Thurg
    //Left
    24242, //Slither
    24243, //Lord Raadan
    //Right
    24244, //Gazakroth
    24245, //Fenstalker
    //Far Right
    24246, //Darkheart
    24247  //Koragg
};

enum AbilityTarget
{
    ABILITY_TARGET_SELF = 0,
    ABILITY_TARGET_VICTIM = 1,
    ABILITY_TARGET_ENEMY = 2,
    ABILITY_TARGET_HEAL = 3,
    ABILITY_TARGET_BUFF = 4,
    ABILITY_TARGET_SPECIAL = 5
};

struct PlayerAbilityStruct
{
    uint32 spell;
    AbilityTarget target;
    uint32 cooldown;
};

static PlayerAbilityStruct PlayerAbility[][3] =
{
    // 1 warrior
    {{SPELL_WR_MORTAL_STRIKE, ABILITY_TARGET_VICTIM, 6000},
    {SPELL_WR_WHIRLWIND, ABILITY_TARGET_SELF, 10000},
    {SPELL_WR_SPELL_REFLECT, ABILITY_TARGET_SELF, 10000}},    
    // 2 paladin
    {{SPELL_PA_CONSECRATION, ABILITY_TARGET_SELF, 10000},
    {SPELL_PA_HOLY_LIGHT, ABILITY_TARGET_HEAL, 10000},
    {SPELL_PA_AVENGING_WRATH, ABILITY_TARGET_SELF, 10000}},
    // 3 hunter
    {{SPELL_HU_EXPLOSIVE_TRAP, ABILITY_TARGET_SELF, 10000},
    {SPELL_HU_FREEZING_TRAP, ABILITY_TARGET_SELF, 10000},
    {SPELL_HU_SNAKE_TRAP, ABILITY_TARGET_SELF, 10000}},
    // 4 rogue
    {{SPELL_RO_WOUND_POISON, ABILITY_TARGET_VICTIM, 3000},
    {SPELL_RO_BLIND, ABILITY_TARGET_ENEMY, 10000},
    {SPELL_RO_SLICE_DICE, ABILITY_TARGET_SELF, 10000}},    
    // 5 priest
    {{SPELL_PR_PAIN_SUPP, ABILITY_TARGET_HEAL, 10000},
    {SPELL_PR_HEAL, ABILITY_TARGET_HEAL, 10000},
    {SPELL_PR_PSYCHIC_SCREAM, ABILITY_TARGET_SELF, 10000}},
    // 5* shadow priest
    {{SPELL_PR_MIND_CONTROL, ABILITY_TARGET_ENEMY, 15000},
    {SPELL_PR_MIND_BLAST, ABILITY_TARGET_ENEMY, 5000},
    {SPELL_PR_SW_DEATH, ABILITY_TARGET_ENEMY, 10000}},
    // 7 shaman
    {{SPELL_SH_CHAIN_LIGHT, ABILITY_TARGET_ENEMY, 8000},
    {SPELL_SH_FIRE_NOVA, ABILITY_TARGET_SELF, 10000},
    {SPELL_SH_HEALING_WAVE, ABILITY_TARGET_HEAL, 10000}},
    // 8 mage
    {{SPELL_MG_FIREBALL, ABILITY_TARGET_ENEMY, 5000},
    {SPELL_MG_FROSTBOLT, ABILITY_TARGET_ENEMY, 5000},
    //{SPELL_MG_FROST_NOVA, ABILITY_TARGET_VICTIM, 1000},
    {SPELL_MG_ICE_LANCE, ABILITY_TARGET_SPECIAL, 2000}},
    // 9 warlock
    {{SPELL_WL_CURSE_OF_DOOM, ABILITY_TARGET_ENEMY, 10000},
    {SPELL_WL_RAIN_OF_FIRE, ABILITY_TARGET_ENEMY, 10000},
    {SPELL_WL_UNSTABLE_AFFL, ABILITY_TARGET_ENEMY, 10000}},
    // 11 druid
    {{SPELL_DR_LIFEBLOOM, ABILITY_TARGET_HEAL, 10000},
    {SPELL_DR_THORNS, ABILITY_TARGET_SELF, 10000},
    {SPELL_DR_MOONFIRE, ABILITY_TARGET_ENEMY, 8000}}
};

struct MANGOS_DLL_DECL boss_hex_lord_malacrassAI : public ScriptedAI
{
    boss_hex_lord_malacrassAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        SelectAddEntry();
        for(uint8 i = 0; i < 4; ++i)
            AddGUID[i] = 0;
        Reset();
    }

    ScriptedInstance *pInstance;

    uint64 AddGUID[4];
    uint32 AddEntry[4];

    uint64 PlayerGUID;

    uint32 SpiritBolts_Timer;
    bool bDrainPower;
    uint32 DrainPower_Timer;
    uint32 SiphonSoul_Timer;
    uint32 PlayerAbility_Timer;
    uint32 CheckAddState_Timer;

    uint32 PlayerClass;

    Unit* SoulDrainTarget;

    void Reset()
    {
        if(pInstance)
            pInstance->SetData(DATA_HEXLORDEVENT, NOT_STARTED);

        SpiritBolts_Timer = 20000;
        bDrainPower = false;
        DrainPower_Timer = 60000;
        SiphonSoul_Timer = 100000;
        PlayerAbility_Timer = 99999;
        CheckAddState_Timer = 5000;

        SpawnAdds();

        m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+0, WEAPON_ID);
        m_creature->SetByteValue(UNIT_FIELD_BYTES_2, 0, SHEATH_STATE_MELEE );
    }

    void Aggro(Unit* who)
    {
        if(pInstance)
            pInstance->SetData(DATA_HEXLORDEVENT, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);

        for(uint8 i = 0; i < 4; ++i)
        {
            Unit* Temp = Unit::GetUnit((*m_creature),AddGUID[i]);
            if(Temp && Temp->isAlive())
                ((Creature*)Temp)->AI()->AttackStart(m_creature->getVictim());
            else
            {
                EnterEvadeMode();
                break;
            }
        }
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_KILL_ONE , m_creature); break;
            case 1: DoScriptText(SAY_KILL_TWO , m_creature); break;
        }
    }

    void JustDied(Unit* victim)
    {
        if(pInstance)
            pInstance->SetData(DATA_HEXLORDEVENT, DONE);

        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_DEATH_ONE, m_creature); break;
            case 1: DoScriptText(SAY_DEATH_TWO, m_creature); break;
        }

        for(uint8 i = 0; i < 4 ; ++i)
        {
            Unit* Temp = Unit::GetUnit((*m_creature),AddGUID[i]);
            if(Temp && Temp->isAlive())
                Temp->DealDamage(Temp, Temp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void SelectAddEntry()
    {
        std::vector<uint32> AddList;
        
        for(uint8 i = 0; i < 8; ++i)
            AddList.push_back(AddEntryList[i]);
        
        uint8 i = 0;
        while(AddList.size() > 4)
        {
            AddList.erase(AddList.begin()+rand()%2+i);
            i++;
        }

        i=0;
        for(std::vector<uint32>::iterator itr = AddList.begin(); itr != AddList.end(); ++itr, ++i)
            AddEntry[i] = *itr;
    }

    void SpawnAdds()
    {
        for(uint8 i = 0; i < 4; ++i)
        {
            Creature *pCreature = ((Creature*)Unit::GetUnit((*m_creature), AddGUID[i]));
            if(!pCreature || !pCreature->isAlive())
            {
                if(pCreature) pCreature->setDeathState(DEAD);
                pCreature = m_creature->SummonCreature(AddEntry[i], Pos_X[i], POS_Y, POS_Z, ORIENT, TEMPSUMMON_DEAD_DESPAWN, 0);
                if(pCreature) AddGUID[i] = pCreature->GetGUID();
            }
            else
            {
                pCreature->AI()->EnterEvadeMode();
                pCreature->Relocate(Pos_X[i], POS_Y, POS_Z, ORIENT);
                pCreature->StopMoving();
            }
        }
    }

    void AddDied()
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_ADD_DIED_ONE, m_creature); break;
            case 1: DoScriptText(SAY_ADD_DIED_TWO, m_creature); break;
            case 2: DoScriptText(SAY_ADD_DIED_THREE, m_creature); break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() && !m_creature->getVictim())
            return;

        if(CheckAddState_Timer < diff)
        {
            for(uint8 i = 0; i < 4; ++i)
            {
                Unit* Temp = Unit::GetUnit((*m_creature),AddGUID[i]);
                if(Temp && Temp->isAlive() && !Temp->getVictim())
                    ((Creature*)Temp)->AI()->AttackStart(m_creature->getVictim());
            }
            CheckAddState_Timer = 5000;
        }else CheckAddState_Timer -= diff;
		
        if(m_creature->GetHealth() * 10 < m_creature->GetMaxHealth() * 8 && !bDrainPower)
            bDrainPower = true;
        
        if(bDrainPower)
        {
            if(DrainPower_Timer < diff)
            {
                DoScriptText(SAY_DRAIN_POWER, m_creature);
                m_creature->CastSpell(m_creature, SPELL_DRAIN_POWER, true);                
                DrainPower_Timer = 40000 + rand()%15000;    // must cast in 60 sec, or buff/debuff will disappear
            }else DrainPower_Timer -= diff;
        }

        if(SpiritBolts_Timer < diff)
        {
            if(DrainPower_Timer < 12000)    // channel 10 sec
                SpiritBolts_Timer = 13000;  // cast drain power first
            else
            {
                m_creature->CastSpell(m_creature, SPELL_SPIRIT_BOLTS, false);
                DoScriptText(SAY_SPIRIT_BOLTS, m_creature);
                SpiritBolts_Timer = 40000;
                SiphonSoul_Timer = 10000;  // ready to drain
                PlayerAbility_Timer = 99999;
            }
        }else SpiritBolts_Timer -= diff;

        if(SiphonSoul_Timer < diff)
        {
            Player* target = SelectRandomPlayer(50);
            Unit *trigger = DoSpawnCreature(MOB_TEMP_TRIGGER, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 30000);
            if (!target || !trigger) EnterEvadeMode();
            else
            {
				DoScriptText(SAY_SOUL_SIPHON , m_creature);
                trigger->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                trigger->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                trigger->CastSpell(target, SPELL_SIPHON_SOUL, true);
                trigger->GetMotionMaster()->MoveChase(m_creature);

                PlayerGUID = target->GetGUID();
                PlayerAbility_Timer = 8000 + rand()%2000;
                PlayerClass = target->getClass() - 1;
                if(PlayerClass == 10) PlayerClass = 9; // druid
                if(PlayerClass == 4 && target->HasSpell(15473)) PlayerClass = 5; // shadow priest
                SiphonSoul_Timer = 99999;   // buff lasts 30 sec
            }
        }else SiphonSoul_Timer -= diff;

        if(PlayerAbility_Timer < diff)
        {
            UseAbility();
            PlayerAbility_Timer = 8000 + rand()%2000;
        }else PlayerAbility_Timer -= diff;

        DoMeleeAttackIfReady();
    }

    void UseAbility()
    {
        uint32 random = rand()%3; 
		
		//random = (PlayerClass == 7 ? rand()%4 : rand()%3);
        Unit *target = NULL;
        switch (PlayerAbility[PlayerClass][random].target)
        {
        case ABILITY_TARGET_SELF:
            target = m_creature;
            break;
        case ABILITY_TARGET_VICTIM:
            target = m_creature->getVictim();
            break;
        case ABILITY_TARGET_HEAL:
            target = DoSelectLowestHpFriendly(50, 0);
            break;
        case ABILITY_TARGET_BUFF:
            {
                std::list<Creature*> templist = DoFindFriendlyMissingBuff(50, PlayerAbility[PlayerClass][random].spell);
                if(!templist.empty()) target = *(templist.begin());
            }
            break;
		case ABILITY_TARGET_ENEMY:
        default:
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            break;
        }
        m_creature->CastSpell(target, PlayerAbility[PlayerClass][random].spell, false);
    }

    Player* SelectRandomPlayer(float range = 0.0f, bool alive = true)
    {
        Map *map = m_creature->GetMap();
        if (!map->IsDungeon()) return NULL;

        Map::PlayerList const &PlayerList = map->GetPlayers();
        if (PlayerList.isEmpty())
            return NULL;
        
        std::list<Player*> temp;
        std::list<Player*>::iterator j;
		
        for(Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if((range == 0.0f || m_creature->IsWithinDistInMap(i->getSource(), range))
                && (!alive || i->getSource()->isAlive()))
                    temp.push_back(i->getSource());

        if (temp.size()) 
        {
            j = temp.begin();
		    advance(j, rand()%temp.size());
            return (*j);
        }
        return NULL;
    }
};

struct MANGOS_DLL_DECL boss_hexlord_addAI : public ScriptedAI
{
    ScriptedInstance* pInstance;

    boss_hexlord_addAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    void Reset() {}
    void Aggro(Unit* who) {}
    void JustDied(Unit* killer)
	{
		if(!pInstance)
            return;

        Creature* HexLord = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_HEXLORDGUID)));
        if(HexLord)
			((boss_hex_lord_malacrassAI*)HexLord->AI())->AddDied();
	}

    void UpdateAI(const uint32 diff)
    {
        if(pInstance && pInstance->GetData(DATA_HEXLORDEVENT) != IN_PROGRESS)
            EnterEvadeMode();

        DoMeleeAttackIfReady();
    }
};

#define SPELL_BLOODLUST       43578
#define SPELL_CLEAVE          15496

struct MANGOS_DLL_DECL boss_thurgAI : public boss_hexlord_addAI
{

    boss_thurgAI(Creature *c) : boss_hexlord_addAI(c) {}

    uint32 bloodlust_timer;
    uint32 cleave_timer;

    void Reset()
    {
        bloodlust_timer = 15000;
        cleave_timer = 10000;

        boss_hexlord_addAI::Reset();
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if(bloodlust_timer < diff)
        {
            std::list<Creature*> templist = DoFindFriendlyMissingBuff(50, SPELL_BLOODLUST);
            if(!templist.empty()) 
            {
                Unit* target = *(templist.begin());
                m_creature->CastSpell(target, SPELL_BLOODLUST, false);
            }
            bloodlust_timer = 12000;
        }else bloodlust_timer -= diff;

        if(cleave_timer < diff)
        {
            m_creature->CastSpell(m_creature->getVictim(),SPELL_CLEAVE, false);
            cleave_timer = 12000; //3 sec cast
        }else cleave_timer -= diff;

        boss_hexlord_addAI::UpdateAI(diff);
    }
};

#define SPELL_FLASH_HEAL     43575
#define SPELL_DISPEL_MAGIC   43577

struct MANGOS_DLL_DECL boss_alyson_antilleAI : public boss_hexlord_addAI
{
    //Holy Priest
    boss_alyson_antilleAI(Creature *c) : boss_hexlord_addAI(c) {}

    uint32 flashheal_timer;
    uint32 dispelmagic_timer;

    void Reset()
    {
        flashheal_timer = 2500;
        dispelmagic_timer = 10000;

        //AcquireGUID();

        boss_hexlord_addAI::Reset();
    }

    void AttackStart(Unit* who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack())
        {
            if(m_creature->Attack(who, false))
            {
                m_creature->GetMotionMaster()->MoveChase(who, 20);
                m_creature->AddThreat(who, 0.0f);
            }

            if (!InCombat)
            {
                Aggro(who);
                InCombat = true;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if(flashheal_timer < diff)
        {
            Unit* target = DoSelectLowestHpFriendly(99, 30000);
            if(target)
            {
                if(target->IsWithinDistInMap(m_creature, 50))
                    m_creature->CastSpell(target,SPELL_FLASH_HEAL, false);
                else
                {
                    // bugged
                    //m_creature->GetMotionMaster()->Clear();
                    //m_creature->GetMotionMaster()->MoveChase(target, 20);
                }
            }
            else
            {
                if(rand()%2)
                {
                    Unit* target = DoSelectLowestHpFriendly(50, 0);
                    m_creature->CastSpell(target, SPELL_DISPEL_MAGIC, false);
                }
                else
                    m_creature->CastSpell(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_DISPEL_MAGIC, false);
            }
            flashheal_timer = 2500;
        }else flashheal_timer -= diff;

        /*if(dispelmagic_timer < diff)
        {
        if(rand()%2)
        {
        Unit* target = SelectTarget();

        m_creature->CastSpell(target, SPELL_DISPEL_MAGIC, false);
        }
        else
        m_creature->CastSpell(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_DISPEL_MAGIC, false);

        dispelmagic_timer = 12000;
        }else dispelmagic_timer -= diff;*/

        boss_hexlord_addAI::UpdateAI(diff);
    }
};

#define SPELL_FIREBOLT        43584

struct MANGOS_DLL_DECL boss_gazakrothAI : public boss_hexlord_addAI
{
    boss_gazakrothAI(Creature *c) : boss_hexlord_addAI(c)  {}

    uint32 firebolt_timer;

    void Reset()
    {
        firebolt_timer = 2000;
        boss_hexlord_addAI::Reset();
    }

    void AttackStart(Unit* who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack())
        {
            if(m_creature->Attack(who, false))
            {
                m_creature->GetMotionMaster()->MoveChase(who, 20);
                m_creature->AddThreat(who, 0.0f);
            }

            if (!InCombat)
            {
                Aggro(who);
                InCombat = true;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if(firebolt_timer < diff)
        {
            m_creature->CastSpell(m_creature->getVictim(),SPELL_FIREBOLT, false);
            firebolt_timer = 700;
        }else firebolt_timer -= diff;

        boss_hexlord_addAI::UpdateAI(diff);
    }
};

#define SPELL_FLAME_BREATH    43582
#define SPELL_THUNDERCLAP     43583

struct MANGOS_DLL_DECL boss_lord_raadanAI : public boss_hexlord_addAI
{
    boss_lord_raadanAI(Creature *c) : boss_hexlord_addAI(c)  {}

    uint32 flamebreath_timer;
    uint32 thunderclap_timer;

    void Reset()
    {
        flamebreath_timer = 8000;
        thunderclap_timer = 13000;

        boss_hexlord_addAI::Reset();

    }
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if (thunderclap_timer < diff)
        { 
            m_creature->CastSpell(m_creature->getVictim(),SPELL_THUNDERCLAP, false);
            thunderclap_timer = 12000;
        }else thunderclap_timer -= diff;

        if (flamebreath_timer < diff)
        {
            m_creature->CastSpell(m_creature->getVictim(),SPELL_FLAME_BREATH, false);
            flamebreath_timer = 12000;
        }else flamebreath_timer -= diff;

        boss_hexlord_addAI::UpdateAI(diff);
    }
};

#define SPELL_PSYCHIC_WAIL   43590

struct MANGOS_DLL_DECL boss_darkheartAI : public boss_hexlord_addAI
{
    boss_darkheartAI(Creature *c) : boss_hexlord_addAI(c)  {}

    uint32 psychicwail_timer;

    void Reset()
    {
        psychicwail_timer = 8000;

        boss_hexlord_addAI::Reset();

    }
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if (psychicwail_timer < diff)
        { 
            m_creature->CastSpell(m_creature->getVictim(),SPELL_PSYCHIC_WAIL, false);
            psychicwail_timer = 12000;
        }else psychicwail_timer -= diff;

        boss_hexlord_addAI::UpdateAI(diff);
    }
};

#define SPELL_VENOM_SPIT    43579

struct MANGOS_DLL_DECL boss_slitherAI : public boss_hexlord_addAI
{
    boss_slitherAI(Creature *c) : boss_hexlord_addAI(c) {}

    uint32 venomspit_timer;


    void Reset()
    {
        venomspit_timer = 5000;
        boss_hexlord_addAI::Reset();
    }

    void AttackStart(Unit* who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack())
        {
            if(m_creature->Attack(who, false))
            {
                m_creature->GetMotionMaster()->MoveChase(who, 20);
                m_creature->AddThreat(who, 0.0f);
            }

            if (!InCombat)
            {
                Aggro(who);
                InCombat = true;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if (venomspit_timer < diff)
        { 
            Unit* victim = SelectUnit(SELECT_TARGET_RANDOM, 0);
            m_creature->CastSpell(victim,SPELL_VENOM_SPIT, false);
            venomspit_timer = 2500;
        }else venomspit_timer -= diff;

        boss_hexlord_addAI::UpdateAI(diff);
    }
};

//Fenstalker
#define SPELL_VOLATILE_INFECTION 43586

struct MANGOS_DLL_DECL boss_fenstalkerAI : public boss_hexlord_addAI
{
    boss_fenstalkerAI(Creature *c) : boss_hexlord_addAI(c) {}

    uint32 volatileinf_timer;


    void Reset()
    {
        volatileinf_timer = 15000;
        boss_hexlord_addAI::Reset();

    }
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if (volatileinf_timer < diff)
        { 
            // core bug
            m_creature->getVictim()->CastSpell(m_creature->getVictim(),SPELL_VOLATILE_INFECTION, false);
            volatileinf_timer = 12000;
        }else volatileinf_timer -= diff;

        boss_hexlord_addAI::UpdateAI(diff);
    }
};

//Koragg
#define SPELL_COLD_STARE      43593
#define SPELL_MIGHTY_BLOW     43592


struct MANGOS_DLL_DECL boss_koraggAI : public boss_hexlord_addAI
{
    boss_koraggAI(Creature *c) : boss_hexlord_addAI(c) {}

    uint32 coldstare_timer;
    uint32 mightyblow_timer;


    void Reset()
    {
        coldstare_timer = 15000;
        mightyblow_timer = 10000;
        boss_hexlord_addAI::Reset();

    }
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if (mightyblow_timer < diff)
        { 
            m_creature->CastSpell(m_creature->getVictim(),SPELL_MIGHTY_BLOW, false);
            mightyblow_timer = 12000;
        }
        if (coldstare_timer < diff)
        {
            Unit* victim = SelectUnit(SELECT_TARGET_RANDOM, 0);
            m_creature->CastSpell(victim,SPELL_COLD_STARE, false);
            coldstare_timer = 12000;
        }

        boss_hexlord_addAI::UpdateAI(diff);
    }
};

CreatureAI* GetAI_boss_hex_lord_malacrass(Creature *_Creature)
{
    return new boss_hex_lord_malacrassAI (_Creature);
}

CreatureAI* GetAI_boss_thurg(Creature *_Creature)
{
    return new boss_thurgAI (_Creature);
}

CreatureAI* GetAI_boss_alyson_antille(Creature *_Creature)
{
    return new boss_alyson_antilleAI (_Creature);
}

CreatureAI* GetAI_boss_gazakroth(Creature *_Creature)
{
    return new boss_gazakrothAI (_Creature);
}

CreatureAI* GetAI_boss_lord_raadan(Creature *_Creature)
{
    return new boss_lord_raadanAI (_Creature);
}

CreatureAI* GetAI_boss_darkheart(Creature *_Creature)
{
    return new boss_darkheartAI (_Creature);
}

CreatureAI* GetAI_boss_slither(Creature *_Creature)
{
    return new boss_slitherAI (_Creature);
}

CreatureAI* GetAI_boss_fenstalker(Creature *_Creature)
{
    return new boss_fenstalkerAI (_Creature);
}

CreatureAI* GetAI_boss_koragg(Creature *_Creature)
{
    return new boss_koraggAI (_Creature);
}
void AddSC_boss_hex_lord_malacrass()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_hexlord_malacrass";
    newscript->GetAI = &GetAI_boss_hex_lord_malacrass;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_thurg";
    newscript->GetAI = &GetAI_boss_thurg;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_gazakroth";
    newscript->GetAI = &GetAI_boss_gazakroth;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_lord_raadan";
    newscript->GetAI = &GetAI_boss_lord_raadan;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_darkheart";
    newscript->GetAI = &GetAI_boss_darkheart;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_slither";
    newscript->GetAI = &GetAI_boss_slither;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_fenstalker";
    newscript->GetAI = &GetAI_boss_fenstalker;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_koragg";
    newscript->GetAI = &GetAI_boss_koragg;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_alyson_antille";
    newscript->GetAI = &GetAI_boss_alyson_antille;
    newscript->RegisterSelf();
}
