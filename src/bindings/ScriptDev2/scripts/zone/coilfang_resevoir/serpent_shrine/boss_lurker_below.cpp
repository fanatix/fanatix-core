#include "precompiled.h"
#include "ObjectMgr.h"
#include "def_serpent_shrine.h"


#define SPELL_SPOUT			          37433
#define SPELL_GEYSER		          37478
#define SPELL_WHIRL			          37660
#define SPELL_WATERBOLT		          37138
#define SPELL_SUBMERGE		          37550
#define SPELL_EMERGE		          20568	//Ragnaros' Emerge: It works and looks nice :-P

#define SCALDING_WATER			      37284
#define MOB_COILFANG_FRENZY		      21508

#define EMOTE_SPOUT	                  "atmet tief ein."
#define EMOTE_STOPFISHING             "Hoert auf zu angeln oder sterbt."

#define MOBID_COILFANG_GUARDIAN       21873
#define MOBID_COILFANG_AMBUSHER       21865

//Ambusher spells
#define SPELL_MULTISHOT		          37790
#define SPELL_SHOOT                   37770

//Guardian spells
#define SPELL_ARCINGSMASH	          28168 
#define SPELL_HAMSTRING		          9080

#define PI 3.1415

#define FISHING             33095

float AddPos[9][3] = 
{
    {2.855381, -459.823914, -19.182686},	//MOVE_AMBUSHER_1 X, Y, Z
    {12.4, -466.042267, -19.182686},		//MOVE_AMBUSHER_2 X, Y, Z
    {51.366653, -460.836060, -19.182686},	//MOVE_AMBUSHER_3 X, Y, Z
    {62.597980, -457.433044, -19.182686},	//MOVE_AMBUSHER_4 X, Y, Z
    {77.607452, -384.302765, -19.182686},	//MOVE_AMBUSHER_5 X, Y, Z
    {63.897900, -378.984924, -19.182686},	//MOVE_AMBUSHER_6 X, Y, Z
    {34.447250, -387.333618, -19.182686},	//MOVE_GUARDIAN_1 X, Y, Z
    {14.388216, -423.468018, -19.625271},	//MOVE_GUARDIAN_2 X, Y, Z
	{42.471519, -445.115295, -19.769423}	//MOVE_GUARDIAN_3 X, Y, Z
};

struct MANGOS_DLL_DECL boss_the_lurker_belowAI : public Scripted_NoMovementAI
{
    boss_the_lurker_belowAI(Creature *c) : Scripted_NoMovementAI(c) 
	{
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
		Reset();
	}

    ScriptedInstance* pInstance;

	uint32 SpoutTimer;
	uint32 SVTimer;
	uint32 PhaseTimer;
	uint32 GeyserTimer;
	uint32 WaterboltTimer;
	uint32 WhirlTimer;
	uint32 OrientationUpdated;
    uint32 WaterTimer;
    uint32 SpTimer;
	uint32 TimeToSpawn;

	float SpoutAngle;
	std::map<uint64,bool>guids;
	bool Spawned;
	bool Submerged;
	bool Clockwise;
    bool InCombat;
	Creature* Summoned;
    Creature* Fishy;
	Player* FirstVictim;

    void Reset()
    {
        if (pInstance)
            pInstance->SetData(DATA_THELURKERBELOWEVENT, NOT_STARTED);

		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SUBMERGED);
        m_creature->setFaction(35);
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
		WhirlTimer = rand()%5000 + 15000;
		PhaseTimer = 120000;
		SpoutTimer = 15000;
		SVTimer = 0;
		OrientationUpdated = -1;
		GeyserTimer = rand()%5000 + 15000;
		WaterboltTimer = 3000;
		Submerged = false;
		Spawned = false;
		Clockwise = true;
		SpoutAngle = 0;
		guids.clear();
		SpTimer = 0;
        WaterTimer = 1000;
        InCombat = false;
		TimeToSpawn = rand()%120; // at first up to 1 effective minute(s) until spawn 
		FirstVictim = 0;        
	}

    void Aggro(Unit *who)
    {
        DoZoneInCombat();
        InCombat = true;
    }

    void JustDied(Unit* Killer)
    {
			Map *map = m_creature->GetMap();
            if(!map->IsDungeon()) return;

			Map::PlayerList const &players = map->GetPlayers();
            if(players.getSize()==0)
                return;


            for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                Player* plr = itr->getSource();
            
			    if (!plr->GetSession() || !plr->isAlive())
				    continue;
                if (plr->HasAura(SCALDING_WATER,0))
					    plr->RemoveAurasDueToSpell(SCALDING_WATER);
            }

	   if (pInstance)
            pInstance->SetData(DATA_THELURKERBELOWEVENT, DONE);
	}

	void UpdateAI(const uint32 diff)
    {

        if (WaterTimer < diff) // every 1000ms
		{
			Map *map = m_creature->GetMap();
            if(!map->IsDungeon()) return;

            Map::PlayerList const &players = map->GetPlayers();
            if(players.getSize()==0)
                return;


            for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                Player* plr = itr->getSource();

                if (!plr) continue;

			    if (!(plr)->GetSession() || !(plr)->isAlive())
				    continue;


				if (!InCombat && plr->GetDistance(m_creature) < 2.25 && (plr->HasAura(33095, 2)))
				{
				  if (TimeToSpawn%40 == 0)
				  {
					  DoYell(EMOTE_STOPFISHING, LANG_UNIVERSAL, NULL);
				  }

				  if (FirstVictim == 0)
					    FirstVictim = plr; // Remember the first player who started fishing

				                      // the player is trying to catch fish
					TimeToSpawn--;    // Time to spawn reduced for every fishing player by 1 per second
				}                     // --> more fishermen(and-women): faster spawn

				if (!InCombat && (TimeToSpawn<=1)) // effective up to TimeToSpawn seconds of fishing
                {
                    if (pInstance)
                      pInstance->SetData(DATA_THELURKERBELOWEVENT, IN_PROGRESS);

					m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    m_creature->RemoveFlag(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SUBMERGED);
                    DoCast(m_creature, SPELL_EMERGE);
                    m_creature->setFaction(14);
                    InCombat = true;

					if (FirstVictim)
					{
  			          if (FirstVictim->GetSession() && FirstVictim->isAlive())
							m_creature->AddThreat(FirstVictim, 1.0f); // guarantee, that first fisher is first in ThreatList
					}
					
					Aggro(NULL); // Put anybody else into the fight (with 0.0 aggro)
                }

                if(plr && plr->GetDistance(m_creature ) > 5625)
                {
                    if (plr->HasAura(SCALDING_WATER,0))
					    plr->RemoveAurasDueToSpell(SCALDING_WATER);

                    if (plr->IsInWater() && !plr->isGameMaster() && !InCombat)
			        {
                        Fishy = plr->SummonCreature(MOB_COILFANG_FRENZY,plr->GetPositionX(),plr->GetPositionY(),plr->GetPositionZ(),plr->GetOrientation(),TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,15000);
                        if (Fishy)
                            Fishy->AI()->AttackStart(plr);
                    }
                }else{
			        if (plr->IsInWater())
			        {
				        if (!plr->HasAura(SCALDING_WATER,0))
					        plr->CastSpell(plr,SCALDING_WATER,true);

			        }else{

				        if (plr->HasAura(SCALDING_WATER,0))
					        plr->RemoveAurasDueToSpell(SCALDING_WATER);
			        }
                }
		    }
		    WaterTimer = 1000;
	    }else WaterTimer -= diff;

		//Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if(!Submerged)
        {
            if(SpoutTimer < diff)
            {
                //Turning while spouting ;) bit fuzzy but working
                if (OrientationUpdated == -1)
                {
                    OrientationUpdated = 0;
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_TOPAGGRO,0);
                    m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
                    //DoTextEmote(EMOTE_SPOUT,NULL);

                    SpoutTimer = 5000;

                    switch(rand()%2)
                    {
                    case 0:
                        Clockwise = true; 
                        break;
                    case 1: 
                        Clockwise = false; 
                        break;
                    }
                    guids.clear();	//clear targets
                    return;
                } else {
                    if(Clockwise)
                        SpoutAngle += PI/100;
                    else
                        SpoutAngle -= PI/100;

                    if(SpoutAngle > 2*PI)
                        SpoutAngle -= 2*PI;
                    if(SpoutAngle < 0)
                        SpoutAngle += 2*PI;

                    //So spin around ya evil fish
                    m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
                    m_creature->SetOrientation(SpoutAngle);
                    m_creature->StopMoving();

                    //Spout part
                    if(SVTimer < diff)
                    {
                        DoCast(m_creature, SPELL_SPOUT);
                        SVTimer = 850;
                    }

                    SVTimer -= diff;
                    OrientationUpdated++;
                    SpoutTimer = 100;

                    if (OrientationUpdated == 200)
                    {
                        SpoutTimer = 45000;
                        OrientationUpdated = -1;
                        WhirlTimer = 5000;
                        Unit *victim = m_creature->getVictim();
                        if(victim)
                        {
                            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, victim->GetGUID());
                        }
                    }
                }
            }else SpoutTimer -= diff;

            if(PhaseTimer >= diff)
                PhaseTimer -= diff; //need to count the phase timer while spouting too
            if(OrientationUpdated != -1)
                return;

            //Whirl directly after a Spout and at random times
            if(WhirlTimer < diff)
            {
                WhirlTimer = rand()%5000 + 15000;
                DoCast(m_creature,SPELL_WHIRL);
            }else WhirlTimer -= diff;

            if(PhaseTimer < diff)
            {
                DoCast(m_creature,SPELL_SUBMERGE);
                PhaseTimer = 60000;
                Submerged = true;
            }else PhaseTimer -= diff;

            if(GeyserTimer < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,1);
                GeyserTimer = rand()%15000 + 10000;
                if(target)
                    DoCast(target,SPELL_GEYSER);
            }else GeyserTimer -= diff;

            if(WaterboltTimer < diff)
            {
                Unit* target = NULL;
                int i = 0;
                bool meleeTarget = false;
                target = SelectUnit(SELECT_TARGET_TOPAGGRO,0);
                if (!target)
                    target = m_creature->getVictim();
                while (target)
                {
                    if( m_creature->IsWithinDistInMap(target, 6))
                    {
                        meleeTarget = true;
                        break;
                    }
                    target = SelectUnit(SELECT_TARGET_TOPAGGRO,i);
                    i++;
                }

                if(!meleeTarget)
                {
                    DoCast(m_creature->getVictim(),SPELL_WATERBOLT);
                    WaterboltTimer = 3000;
                } else DoMeleeAttackIfReady(); // WTF ? (1)

            }else WaterboltTimer -= diff;

            DoMeleeAttackIfReady(); //(1)

        } else {
            if(PhaseTimer < diff)
            {
                Submerged = false;
                m_creature->RemoveAllAuras();
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->RemoveFlag(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SUBMERGED);
                m_creature->RemoveFlag(UNIT_FIELD_BYTES_1,9);
                DoCast(m_creature, SPELL_EMERGE);
                Spawned = false;
                SpoutTimer = 5000;
                PhaseTimer = 120000;
                return;
            }
            else
            {
                /*if(!m_creature->isInCombat())
                m_creature->SetInCombat();*/
                PhaseTimer -= diff;
            }

            if(!Spawned)
            {
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                SummonAdd(MOBID_COILFANG_AMBUSHER,AddPos[0][0],AddPos[0][1],AddPos[0][2]);
                SummonAdd(MOBID_COILFANG_AMBUSHER,AddPos[1][0],AddPos[1][1],AddPos[1][2]);
                SummonAdd(MOBID_COILFANG_AMBUSHER,AddPos[2][0],AddPos[2][1],AddPos[2][2]);
                SummonAdd(MOBID_COILFANG_AMBUSHER,AddPos[3][0],AddPos[3][1],AddPos[3][2]);
                SummonAdd(MOBID_COILFANG_AMBUSHER,AddPos[4][0],AddPos[4][1],AddPos[4][2]);
                SummonAdd(MOBID_COILFANG_AMBUSHER,AddPos[5][0],AddPos[5][1],AddPos[5][2]);
                SummonAdd(MOBID_COILFANG_GUARDIAN,AddPos[6][0],AddPos[6][1],AddPos[6][2]);
                SummonAdd(MOBID_COILFANG_GUARDIAN,AddPos[7][0],AddPos[7][1],AddPos[7][2]);
                SummonAdd(MOBID_COILFANG_GUARDIAN,AddPos[8][0],AddPos[8][1],AddPos[8][2]);
                Spawned = true;
            }
        }
	}

	void SummonAdd(uint32 entry, float x, float y, float z)
	{
		Summoned = m_creature->SummonCreature(entry, x, y, z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
		Unit *target = NULL;
		target = SelectUnit(SELECT_TARGET_RANDOM, 0);
		if(target)
			Summoned->AI()->AttackStart(target);
	}
};

CreatureAI* GetAI_boss_the_lurker_below(Creature *_Creature)
{
    return new boss_the_lurker_belowAI (_Creature);
}

void AddSC_boss_the_lurker_below()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_the_lurker_below";
    newscript->GetAI = &GetAI_boss_the_lurker_below;
    newscript->RegisterSelf();
}

struct MANGOS_DLL_DECL mob_coilfang_ambusherAI : public Scripted_NoMovementAI
{
    mob_coilfang_ambusherAI(Creature *c) : Scripted_NoMovementAI(c) {Reset();}

	uint32 MultiShotTimer;
	uint32 ShootBowTimer;
    
    void Reset()
    {
		MultiShotTimer = 10000;
		ShootBowTimer = 4000;
	}

    void Aggro(Unit *who) 
    {
    }

	void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

		if(MultiShotTimer < diff)
		{
			Unit* target = NULL;
			target = SelectUnit(SELECT_TARGET_RANDOM,0);
				if(target)
					DoCast(target ,SPELL_MULTISHOT);
			MultiShotTimer = 14000;
		}else MultiShotTimer -= diff;

		if(ShootBowTimer < diff)
		{
            DoCast(m_creature->getVictim(),SPELL_SHOOT);
			ShootBowTimer = 4000;
		}else ShootBowTimer -= diff;

        DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_coilfang_ambusher(Creature *_Creature)
{
    return new mob_coilfang_ambusherAI (_Creature);
}

void AddSC_mob_coilfang_ambusher()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_coilfang_ambusher";
    newscript->GetAI = &GetAI_mob_coilfang_ambusher;
    newscript->RegisterSelf();
}

struct MANGOS_DLL_DECL mob_coilfang_guardianAI : public ScriptedAI
{
    mob_coilfang_guardianAI(Creature *c) : ScriptedAI(c) {Reset();}

	uint32 SmashTimer;
	uint32 HamstringTimer;
    
    void Reset()
    {
		SmashTimer = 10000;
		HamstringTimer = 4000;
	}

    void Aggro(Unit *who) 
    {
    }

	void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

		if(SmashTimer < diff)
		{
			Unit* target = NULL;
			target = SelectUnit(SELECT_TARGET_RANDOM,0);
				if(target)
					DoCast(target ,SPELL_ARCINGSMASH);
			SmashTimer = 14000;
		}else SmashTimer -= diff;

		if(HamstringTimer < diff)
		{
            DoCast(m_creature->getVictim(),SPELL_HAMSTRING);
			HamstringTimer = 4000;
		}else HamstringTimer -= diff;

        DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_coilfang_guardian(Creature *_Creature)
{
    return new mob_coilfang_guardianAI (_Creature);
}

void AddSC_mob_coilfang_guardian()
{
	Script *newscript;
    newscript = new Script;
    newscript->Name= "mob_coilfang_guardian";
    newscript->GetAI = &GetAI_mob_coilfang_guardian;
    newscript->RegisterSelf();
}