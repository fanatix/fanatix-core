/* ScriptData
SDName: Boss_Kiljaeden
SD%Complete: 30
SDComment: Phase4, Phase5, Shadow Spike 
SDCategory: Sunwell_Plateau
EndScriptData */

#include "precompiled.h"
#include "def_sunwell_plateau.h"

/*** Speech and sounds***/
enum Speeches
{
    // These are used throughout Sunwell and Magisters(?). Players can hear this while running through the instances.
    SAY_KJ_OFFCOMBAT1   = -1580066,
    SAY_KJ_OFFCOMBAT2   = -1580067,
    SAY_KJ_OFFCOMBAT3   = -1580068,
    SAY_KJ_OFFCOMBAT4   = -1580069,
    SAY_KJ_OFFCOMBAT5   = -1580070,

    // Encounter speech and sounds
    SAY_KJ_EMERGE       = -1580071,
    SAY_KJ_SLAY1        = -1580072,
    SAY_KJ_SLAY2        = -1580073,
    SAY_KJ_REFLECTION1  = -1580074,
    SAY_KJ_REFLECTION2  = -1580075,
    SAY_KJ_DARKNESS1    = -1580076,
    SAY_KJ_DARKNESS2    = -1580077,
    SAY_KJ_DARKNESS3    = -1580078,
    SAY_KJ_PHASE3       = -1580079,
    SAY_KJ_PHASE4       = -1580080,
    SAY_KJ_PHASE5       = -1580081,
    SAY_KJ_DEATH        = -1580093,
    EMOTE_KJ_DARKNESS   = -1580094,

    /*** Kalecgos - Anveena speech at the beginning of Phase 5; Anveena's sacrifice ***/
    SAY_KALECGOS_AWAKEN     = -1580082,
    SAY_ANVEENA_IMPRISONED  = -1580083,
    SAY_KALECGOS_LETGO      = -1580084,
    SAY_ANVEENA_LOST        = -1580085,
    SAY_KALECGOS_FOCUS      = -1580086,
    SAY_ANVEENA_KALEC       = -1580087,
    SAY_KALECGOS_FATE       = -1580088,
    SAY_ANVEENA_GOODBYE     = -1580089,
    SAY_KALECGOS_GOODBYE    = -1580090,
    SAY_KALECGOS_ENCOURAGE  = -1580091,
    SAY_KALECGOS_JOIN       = -1580092,
    SAY_KALEC_ORB_READY1    = -1580095,
    SAY_KALEC_ORB_READY2    = -1580096,
    SAY_KALEC_ORB_READY3    = -1580097,
    SAY_KALEC_ORB_READY4    = -1580098
};

/*** Spells used during the encounter ***/
enum SpellIds
{
    /* Hand of the Deceiver's spells and cosmetics */
    SPELL_SHADOW_BOLT_VOLLEY = 45770, // ~30 yard range Shadow Bolt Volley for ~2k(?) damage
    SPELL_SHADOW_INFUSION = 45772, // They gain this at 20% - Immunity to Stun/Silence and makes them look angry!
    SPELL_FELFIRE_PORTAL = 46875, // Creates a portal that spawns Felfire Fiends (LIVE FOR THE SWARM!1 FOR THE OVERMIND!)
    SPELL_SHADOW_CHANNELING = 46757, // Channeling animation out of combat

    /* Volatile Felfire Fiend's spells */
    SPELL_FELFIRE_FISSION = 45779, // Felfire Fiends explode when they die or get close to target.

    /* Kil'Jaeden's spells and cosmetics */
    SPELL_TRANS = 23188, // Surprisingly, this seems to be the right spell.. (Where is it used?)
    SPELL_REBIRTH = 44200, // Emerge from the Sunwell
    SPELL_SOUL_FLAY = 45442, // 9k Shadow damage over 3 seconds. Spammed throughout all the fight.      
    SPELL_LEGION_LIGHTNING = 45664, // Chain Lightning, 4 targets, ~3k Shadow damage, 1.5k mana burn
    SPELL_FIRE_BLOOM = 45641, // Places a debuff on 5 raid members, which causes them to deal 2k Fire damage to nearby allies and selves. MIGHT NOT WORK
    
    SPELL_SINISTER_REFLECTION = 45785, // Summon shadow copies of 5 raid members that fight against KJ's enemies
    SPELL_COPY_WEAPON = 41055, // }
    SPELL_COPY_WEAPON2 = 41054, // }
    SPELL_COPY_OFFHAND = 45206, // }- Spells used in Sinister Reflection creation
    SPELL_COPY_OFFHAND_WEAPON = 45205, // }
    
    SPELL_SHADOW_SPIKE = 46680, // Bombard random raid members with Shadow Spikes (Very similar to Void Reaver orbs)
    SPELL_FLAME_DART = 45737, // Bombards the raid with flames every 3(?) seconds
    SPELL_DARKNESS_OF_A_THOUSAND_SOULS = 46605, // Begins a 8-second channeling, after which he will deal 50'000 damage to the raid
    SPELL_ARMAGEDDON = 45909, // Meteor spell
    SPELL_ARMAGEDDON_VISUAL = 45911, // Does the hellfire visual to indicate where the meteor missle lands

    /* Anveena's spells and cosmetics (Or, generally, everything that has "Anveena" in name) */
    SPELL_ANVEENA_PRISON = 46367, // She hovers locked within a bubble
    SPELL_ANVEENA_ENERGY_DRAIN = 46410, // Sunwell energy glow animation (Control mob uses this)
    SPELL_SACRIFICE_OF_ANVEENA = 46474, // This is cast on Kil'Jaeden when Anveena sacrifices herself into the Sunwell

    /*** Other Spells (used by players, etc) ***/
    SPELL_VENGEANCE_OF_THE_BLUE_FLIGHT = 45839, // Possess the blue dragon from the orb to help the raid.
    SPELL_ENTROPIUS_BODY = 46819 // Visual for Entropius at the Epilogue
};

enum CreatureIds
{
    CREATURE_ANVEENA = 26046, // Embodiment of the Sunwell
    CREATURE_KALECGOS = 25319, // Helps the raid throughout the fight
    CREATURE_KILJAEDEN = 25315, // Give it to 'em KJ!
    CREATURE_HAND_OF_THE_DECEIVER = 25588, // Adds found before KJ emerges
    CREATURE_FELFIRE_PORTAL = 25603, // Portal spawned be Hand of the Deceivers
    CREATURE_VOLATILE_FELFIRE_FIEND = 25598, // Fiends spawned by the above portal
    CREATURE_ARMAGEDDON_TARGET = 25735, // This mob casts meteor on itself.. I think
    CREATURE_SHIELD_ORB = 25502, // Shield orbs circle the room raining shadow bolts on raid
    CREATURE_THE_CORE_OF_ENTROPIUS = 26262, // Used in the ending cinematic?
    CREATURE_POWER_OF_THE_BLUE_DRAGONFLIGHT = 25653, // NPC that players possess when using the Orb of the Blue Dragonflight
    CREATURE_SPIKE_TARGET1 = 30598, //Should summon these under Shadow Spike Channel on targets place
    CREATURE_SPIKE_TARGET2 = 30614                                                                            
};

/*** GameObjects ***/
#define GAMEOBJECT_ORB_OF_THE_BLUE_DRAGONFLIGHT 188415

/*** Error messages ***/
#define ERROR_KJ_NOT_SUMMONED "TSCR ERROR: Unable to summon Kil'Jaeden for some reason"

/*** Others ***/
#define FLOOR_Z 28.050388
#define SHIELD_ORB_Z 45.000000 // TODO: Find correct height.

enum Phase
{
    PHASE_DECEIVERS = 1, // Fight 3 adds
    PHASE_NORMAL = 2, // Kil'Jaeden emerges from the sunwell
    PHASE_DARKNESS = 3, // At 85%, he gains few abilities; Kalecgos joins the fight
    PHASE_ARMAGEDDON = 4, // At 55%, he gains even more abilities
    PHASE_SACRIFICE = 5, // At 25%, Anveena sacrifices herself into the Sunwell; at this point he becomes enraged and has *significally* shorter cooldowns.
};

// Locations of the Hand of Deceiver adds
float DeceiverLocations[3][3]=
{
// X Y O
    {1682.045, 631.299, 5.936},
    {1684.099, 618.848, 0.589},
    {1694.170, 612.272, 1.416},
};

/* TODO:
Middle of the Sunwell: {1698.897217, 628.214600}
Northeast: {1713.255249, 619.132324}
2D distance ~17 */

// Locations, where Shield Orbs will spawn, TODO: Use Sunwell middle point, radius and sin/cos instead of this.
float ShieldOrbLocations[3][2]=
{ // TODO: Find correct XY's
    {1683.000, 625.000}, // First one spawns northeast of KJ
    {1683.000, 625.000}, // Second one spawns southeast
    {1683.000, 625.000}, // Third one spawns (?)
};

struct Speech
{
    int32 textid;
    uint32 creature, timer;
};
// TODO: Timers
static Speech Sacrifice[]=
{
    {SAY_KALECGOS_AWAKEN, CREATURE_KALECGOS, 5000},
    {SAY_ANVEENA_IMPRISONED, CREATURE_ANVEENA, 5000},
    {SAY_KALECGOS_LETGO, CREATURE_KALECGOS, 8000},
    {SAY_ANVEENA_LOST, CREATURE_ANVEENA, 5000},
    {SAY_KALECGOS_FOCUS, CREATURE_KALECGOS, 7000},
    {SAY_ANVEENA_KALEC, CREATURE_ANVEENA, 2000},
    {SAY_KALECGOS_FATE, CREATURE_KALECGOS, 3000},
    {SAY_ANVEENA_GOODBYE, CREATURE_ANVEENA, 6000},
    {SAY_KALECGOS_GOODBYE, CREATURE_KALECGOS, 12000},
    {SAY_KJ_PHASE5, CREATURE_KILJAEDEN, 8000},
    {SAY_KALECGOS_ENCOURAGE, CREATURE_KALECGOS, 5000}
};

class AllOrbsInGrid
{
public:
    AllOrbsInGrid() {}
    bool operator() (GameObject* go)
    {
        if(go->GetEntry() == GAMEOBJECT_ORB_OF_THE_BLUE_DRAGONFLIGHT)
            return true;
        return false;
    }
};

bool GOHello_go_orb_of_the_blue_flight(Player *plr, GameObject* go)
{
    go->SummonCreature(CREATURE_POWER_OF_THE_BLUE_DRAGONFLIGHT, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 121000);
    plr->CastSpell(plr, SPELL_VENGEANCE_OF_THE_BLUE_FLIGHT, true);
    go->SetUInt32Value(GAMEOBJECT_FACTION, 0);
    go->Refresh();
    return true;
}

//AI for Kalecgos
struct MANGOS_DLL_DECL boss_kalecgosAI : public ScriptedAI
{
    boss_kalecgosAI(Creature* c) : ScriptedAI(c)
    {
        for(uint8 i = 0; i < 4; ++i)
            Orb[i] = 0;
        FindOrbs();
        Reset();
    }
    
    uint64 Orb[4];
    uint8 OrbsEmpowered;
    uint32 EmpowerTimer;
    
    void Reset()
    {
        EmpowerTimer = 0;
    }

    void Aggro(Unit* who) {}

    void FindOrbs()
    {
        CellPair pair(MaNGOS::ComputeCellPair(m_creature->GetPositionX(), m_creature->GetPositionY()));
        Cell cell(pair);
        cell.data.Part.reserved = ALL_DISTRICT;
        cell.SetNoCreate();
        std::list<GameObject*> orbList;
        AllOrbsInGrid check;
        MaNGOS::GameObjectListSearcher<AllOrbsInGrid> searcher(m_creature, orbList, check);
        TypeContainerVisitor<MaNGOS::GameObjectListSearcher<AllOrbsInGrid>, GridTypeMapContainer> visitor(searcher);
        CellLock<GridReadGuard> cell_lock(cell, pair);
        cell_lock->Visit(cell_lock, visitor, *(m_creature->GetMap()));
        if(orbList.empty())
            return;
        uint8 i = 0;
        for(std::list<GameObject*>::iterator itr = orbList.begin(); itr != orbList.end(); ++itr, ++i)
            Orb[i] = (*itr)->GetGUID();
    }

    void EmpowerOrb()
    {
        GameObject* orb = GameObject::GetGameObject(*m_creature, Orb[OrbsEmpowered]);
        if(!orb)
            return;
        
        orb->SetUInt32Value(GAMEOBJECT_FACTION, 35);
        orb->Refresh();
        ++OrbsEmpowered;

        switch(rand()%4)
        {
        case 0: DoScriptText(SAY_KALEC_ORB_READY1, m_creature); break;
        case 1: DoScriptText(SAY_KALEC_ORB_READY2, m_creature); break;
        case 2: DoScriptText(SAY_KALEC_ORB_READY3, m_creature); break;
        case 3: DoScriptText(SAY_KALEC_ORB_READY4, m_creature); break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
    }
};
//AI for Kil'jaeden
struct MANGOS_DLL_DECL boss_kiljaedenAI : public Scripted_NoMovementAI
{
    boss_kiljaedenAI(Creature* c) : Scripted_NoMovementAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }
    
    /* GUIDs */
    uint64 KalecGUID;
    
    /* General */
    ScriptedInstance* pInstance;
    uint32 Phase;

    /* Phase 2+ spells */
    uint32 SoulFlayTimer;
    uint32 LegionLightningTimer;
    uint32 FireBloomTimer;
    uint32 SummonShieldOrbTimer;

    /* Phase 3+ spells */
    uint32 ShadowSpikeTimer;
    uint32 FlameDartTimer;
    uint32 DarknessTimer;
    uint32 DarknessTriggerTimer; //When the channel will end
    
    /* Phase 4+ spells */
    uint32 ArmageddonTimer;

    void Reset()
    {
        KalecGUID = 0;
        // TODO: Fix timers
        SoulFlayTimer = 15000;
        LegionLightningTimer = 30000;
        FireBloomTimer = 30000;
        SummonShieldOrbTimer = 45000;
        ShadowSpikeTimer = 60000;
        FlameDartTimer = 3000;
        DarknessTimer = 60000;
        DarknessTriggerTimer = 0;
        ArmageddonTimer = 10000;
        Phase = PHASE_DECEIVERS;
    }

    void JustSummoned(Creature* summoned)
    {
        summoned->setFaction(m_creature->getFaction());
        summoned->SetLevel(m_creature->getLevel());

        if(summoned->GetEntry() == CREATURE_ARMAGEDDON_TARGET)
        {
            summoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            summoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }       
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_KJ_DEATH, m_creature);

        if(pInstance)
            pInstance->SetData(DATA_KILJAEDEN_EVENT, DONE);
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%2)
        {
        case 0: DoScriptText(SAY_KJ_SLAY1, m_creature); break;
        case 1: DoScriptText(SAY_KJ_SLAY2, m_creature); break;
        }
    }

    void EnterEvadeMode()
    {
        Scripted_NoMovementAI::EnterEvadeMode();
        
        // Reset the controller
        if(pInstance)
        {
            Creature* Control = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_KILJAEDEN_CONTROLLER)));
            if(Control)
                ((Scripted_NoMovementAI*)Control->AI())->Reset();
            //Should we kill KJ too? Dunno
            m_creature->DealDamage(m_creature, m_creature->GetHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            m_creature->RemoveCorpse();
        }
    }

    void Aggro(Unit* who) 
    {
        if(pInstance)
            pInstance->SetData(DATA_KILJAEDEN_EVENT, IN_PROGRESS);

        DoZoneInCombat();
    }

    void CastSinisterReflection() // TODO: Create copies of 5 random raid members.
    {
        switch(rand()%2)
        {
        case 0: DoScriptText(SAY_KJ_REFLECTION1, m_creature); break;
        case 1: DoScriptText(SAY_KJ_REFLECTION2, m_creature); break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() || Phase < PHASE_NORMAL)
            return;
        
        if(SoulFlayTimer < diff) // 9k damage over 3 seconds on highest aggro
        {
            DoCast(m_creature->getVictim(), SPELL_SOUL_FLAY);
            SoulFlayTimer = 3500;
        }else SoulFlayTimer -= diff;
        
        if(LegionLightningTimer < diff) // Lightning on random target, jumps to 3 additional players, burns 1500 mana and deals 3k damage
        {
            m_creature->InterruptNonMeleeSpells(true);
            DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_LEGION_LIGHTNING);
            LegionLightningTimer = (Phase == PHASE_SACRIFICE) ? 18000 : 30000; // 18 seconds in PHASE_SACRIFICE
            SoulFlayTimer = 2500;
        }else LegionLightningTimer -= diff;

        if(FireBloomTimer < diff) // Places a debuff on 5 random targets; targets and all nearby allies suffer ~1.7k fire damage every 2 seconds
        {            
            m_creature->InterruptNonMeleeSpells(true);
            DoCast(m_creature, SPELL_FIRE_BLOOM);
            FireBloomTimer = (Phase == PHASE_SACRIFICE) ? 25000 : 40000; // 25 seconds in PHASE_SACRIFICE
            SoulFlayTimer = 1000;
        }else FireBloomTimer -= diff;
        
        if(SummonShieldOrbTimer < diff)
        {
            for(uint8 i = 1; i < Phase; ++i)
                m_creature->SummonCreature(CREATURE_SHIELD_ORB, ShieldOrbLocations[i-1][0], ShieldOrbLocations[i-1][1], SHIELD_ORB_Z, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 45000);
            SummonShieldOrbTimer = (Phase == PHASE_SACRIFICE) ? 30000 : 60000; // 30 seconds in PHASE_SACRIFICE
            SoulFlayTimer = 2000;
        }else SummonShieldOrbTimer -= diff;
        
        if(Phase <= PHASE_NORMAL)
        {
            if(Phase == PHASE_NORMAL && ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 85))
            {
                CastSinisterReflection();
                DoScriptText(SAY_KJ_PHASE3, m_creature);                
                Phase = PHASE_DARKNESS;
            }
            else return;
        }
        
        if(DarknessTimer < diff) // Begins to channel for 8 seconds, then deals 50'000 damage to all raid members.
        {
            DoScriptText(EMOTE_KJ_DARKNESS, m_creature);
            m_creature->InterruptNonMeleeSpells(true);
            DoCast(m_creature, SPELL_DARKNESS_OF_A_THOUSAND_SOULS);
            DarknessTimer = (Phase == PHASE_SACRIFICE) ? 20000 + rand()%15000 : 40000 + rand()%30000;
            SummonShieldOrbTimer += 8000;
            FireBloomTimer += 8000; // Don't let other spells
            LegionLightningTimer += 8000; // interrupt this cast!
            FlameDartTimer += 8000;
            ShadowSpikeTimer += 8000;
            DarknessTriggerTimer = 8000;
            
            if(Phase == PHASE_SACRIFICE)
                ArmageddonTimer += 8000; // Armageddon on the other hand, can be casted now, if Anveena has already sacrificed
            SoulFlayTimer = 9000;
        }else DarknessTimer -= diff;

        if(DarknessTriggerTimer < diff)
        {
            switch(rand()%3)
            {
            case 0: DoScriptText(SAY_KJ_DARKNESS1, m_creature); break;
            case 1: DoScriptText(SAY_KJ_DARKNESS2, m_creature); break;
            case 2: DoScriptText(SAY_KJ_DARKNESS3, m_creature); break;
            }
            DarknessTriggerTimer = 0;
        }else DarknessTriggerTimer -= diff;
        
        if(ArmageddonTimer < diff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target)
            {
                float x, y, z;
                target->GetPosition(x, y, z);
                Creature* Armageddon = m_creature->SummonCreature(CREATURE_ARMAGEDDON_TARGET, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN, 5000);
                if(Armageddon)
                {                    
                    DoCast(Armageddon, SPELL_ARMAGEDDON, true);
                    Armageddon->CastSpell(Armageddon, SPELL_ARMAGEDDON_VISUAL, true);
                }
            }
            ArmageddonTimer = 2000; // No, I'm not kidding
        }else ArmageddonTimer -= diff;
        
        if(ShadowSpikeTimer < diff)
        {
            DoCast(m_creature, SPELL_SHADOW_SPIKE);
            ShadowSpikeTimer = 90000 + rand()%30000; // Not too often, amirite? It's 28 second casting timer...
        }else ShadowSpikeTimer -= diff;
        
        if(Phase <= PHASE_DARKNESS)
        {
            if(Phase == PHASE_DARKNESS && ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 55))
            {
                DoScriptText(SAY_KJ_PHASE4, m_creature);
                Phase = PHASE_ARMAGEDDON;
            }
            else return;
        }
        
        // TODO: Phase 4 specific spells
        /* if(Phase <= PHASE_ARMAGEDDON)
        {
        if(Phase == PHASE_ARMAGEDDON && ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 25))
        {
        Phase = PHASE_SACRIFICE;
        // He shouldn't cast spells for ~10 seconds after Anveena's sacrifice. This will be done within Anveena's script
        }
        else return;
        } */
}
};
//AI for Kil'jaeden Event Controller
struct MANGOS_DLL_DECL mob_kiljaeden_controllerAI : public Scripted_NoMovementAI
{
    mob_kiljaeden_controllerAI(Creature* c) : Scripted_NoMovementAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }
    
    ScriptedInstance* pInstance;

    uint64 DeceiverGUID[3];
    uint64 AnveenaGUID;

    uint16 DeceiverDeathCount;

    uint32 Phase;

    bool SummonedDeceivers;
    
    void Reset()
    {
        for(uint8 i = 0; i < 3; ++i)
        {
            if(DeceiverGUID[i])
            {
                Creature* Deceiver = ((Creature*)Unit::GetUnit(*m_creature, DeceiverGUID[i]));
                if(Deceiver)
                {
                    Deceiver->DealDamage(Deceiver, Deceiver->GetHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    Deceiver->RemoveCorpse();
                }
                DeceiverGUID[i] = 0;
            }
        }
        
        if(AnveenaGUID)
        {
            Creature* Anveena = ((Creature*)Unit::GetUnit(*m_creature, AnveenaGUID));
            if(Anveena)
            {
                Anveena->DealDamage(Anveena, Anveena->GetHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                Anveena->RemoveCorpse();
            }
        }
        
        Phase = PHASE_DECEIVERS;
        DeceiverDeathCount = 0;
        SummonedDeceivers = false;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }
    
    void MoveInLineOfSight(Unit* who)
    {
        if(Phase == PHASE_DECEIVERS)
            return;

        Scripted_NoMovementAI::MoveInLineOfSight(who);
    }
    
    void AttackStart(Unit* who)
    {
        if(Phase == PHASE_DECEIVERS)
            return;

        Scripted_NoMovementAI::AttackStart(who);
    }

    void Aggro(Unit* who) {}

    void UpdateAI(const uint32 diff)
    {
        if(!SummonedDeceivers)
        {
            for(uint8 i = 0; i < 3; ++i)
            {
                Creature* Deceiver = m_creature->SummonCreature(CREATURE_HAND_OF_THE_DECEIVER, DeceiverLocations[i][0], DeceiverLocations[i][1], FLOOR_Z, DeceiverLocations[i][2], TEMPSUMMON_DEAD_DESPAWN, 0);
                if(Deceiver)
                {
                    Deceiver->CastSpell(Deceiver, SPELL_SHADOW_CHANNELING, false);
                    DeceiverGUID[i] = Deceiver->GetGUID();
                }
            }
            Creature* Anveena = DoSpawnCreature(CREATURE_ANVEENA, 0, 0, 20, 0, TEMPSUMMON_DEAD_DESPAWN, 0);
            if(Anveena)
            {
                Anveena->AddUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
                Anveena->CastSpell(Anveena, SPELL_ANVEENA_PRISON, false);
                DoCast(m_creature, SPELL_ANVEENA_ENERGY_DRAIN);
                AnveenaGUID = Anveena->GetGUID();
            }
            SummonedDeceivers = true;
        }
        if((DeceiverDeathCount > 2) && Phase == PHASE_DECEIVERS)
        {
            m_creature->RemoveAurasDueToSpell(SPELL_ANVEENA_ENERGY_DRAIN) ;
            Phase = PHASE_NORMAL;
            Creature* KJ = DoSpawnCreature(CREATURE_KILJAEDEN, 0, 0,0, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 45000);
            if(KJ)
            {
                KJ->CastSpell(KJ, SPELL_REBIRTH, false);
                ((boss_kiljaedenAI*)KJ->AI())->Phase = PHASE_NORMAL;
                KJ->AddThreat(m_creature->getVictim(), 1.0f);
                DoScriptText(SAY_KJ_EMERGE, KJ);                
            }
            else error_log(ERROR_KJ_NOT_SUMMONED);
        }
    }
};
//AI for Hand of the Deceiver
struct MANGOS_DLL_DECL mob_hand_of_the_deceiverAI : public ScriptedAI
{
    mob_hand_of_the_deceiverAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }
    ScriptedInstance* pInstance;
    
    uint32 ShadowBoltVolleyTimer;
    uint32 FelfirePortalTimer;
    
    void Reset()
    {
        // TODO: Timers!
        ShadowBoltVolleyTimer = 8000 + rand()%6000; // So they don't all cast it in the same moment.
        FelfirePortalTimer = 20000;
    }

    void JustSummoned(Creature* summoned)
    {
        summoned->setFaction(m_creature->getFaction());
        summoned->SetLevel(m_creature->getLevel());    
    }

    void Aggro(Unit* who)
    {
        if(pInstance)
        {
            Creature* Control = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_KILJAEDEN_CONTROLLER)));
            if(Control)
                Control->AddThreat(who, 1.0f);
        }
        m_creature->InterruptNonMeleeSpells(true);
    }
    void JustDied(Unit* killer)
    {
        if(!pInstance)
            return;
        
        Creature* Control = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_KILJAEDEN_CONTROLLER)));
        if(Control)
            ((mob_kiljaeden_controllerAI*)Control->AI())->DeceiverDeathCount++;
    }
    
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        // Gain Shadow Infusion at 20% health
        if(((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 20) && !m_creature->HasAura(SPELL_SHADOW_INFUSION, 0))
            DoCast(m_creature, SPELL_SHADOW_INFUSION, true);

        // Shadow Bolt Volley - Shoots Shadow Bolts at all enemies within 30 yards, for ~2k Shadow damage.
        if(ShadowBoltVolleyTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SHADOW_BOLT_VOLLEY);
            ShadowBoltVolleyTimer = 12000;
        }else ShadowBoltVolleyTimer -= diff;

        // Felfire Portal - Creatres a portal, that spawns Volatile Felfire Fiends, which do suicide bombing.
        if(FelfirePortalTimer < diff)
        {
            Creature* Portal = DoSpawnCreature(CREATURE_FELFIRE_PORTAL, 0, 0,0, 0, TEMPSUMMON_TIMED_DESPAWN, 20000);
            if(Portal)
            {
                std::list<HostilReference*>::iterator itr;
                for(itr = m_creature->getThreatManager().getThreatList().begin(); itr != m_creature->getThreatManager().getThreatList().end(); ++itr)
                {
                    Unit* pUnit = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
                    if(pUnit)
                        Portal->AddThreat(pUnit, 1.0f);
                }
            }
            FelfirePortalTimer = 20000;
        }else FelfirePortalTimer -= diff;

        DoMeleeAttackIfReady();
    }
};
//AI for Felfire Portal
struct MANGOS_DLL_DECL mob_felfire_portalAI : public Scripted_NoMovementAI
{
    mob_felfire_portalAI(Creature* c) : Scripted_NoMovementAI(c) {Reset();}
    
    uint32 SpawnFiendTimer;
    
    // TODO: Timers
    void Reset() 
    { 
        SpawnFiendTimer = 5000; 
         m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
         m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void Aggro(Unit* who) {}

    void JustSummoned(Creature* summoned)
    {
        summoned->setFaction(m_creature->getFaction());
        summoned->SetLevel(m_creature->getLevel());    
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim()) 
            return;
        
        if(SpawnFiendTimer < diff)
        {
            Creature* Fiend = DoSpawnCreature(CREATURE_VOLATILE_FELFIRE_FIEND, 0, 0, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 20000);
            if(Fiend)
                Fiend->AddThreat(SelectUnit(SELECT_TARGET_RANDOM,0), 100000.0f);
            SpawnFiendTimer = 4000 + rand()%4000;
        }else SpawnFiendTimer -= diff;
    }
};
//AI for Felfire Fiend
struct MANGOS_DLL_DECL mob_volatile_felfire_fiendAI : public ScriptedAI
{
    mob_volatile_felfire_fiendAI(Creature* c) : ScriptedAI(c)
    {
        Reset();
    }
    
    uint32 ExplodeTimer;
    
    bool LockedTarget;

    void Reset()
    {
        ExplodeTimer = 2000;
        LockedTarget = false;
    }

    void Aggro(Unit* who) {}

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if(damage > m_creature->GetHealth())
            DoCast(m_creature, SPELL_FELFIRE_FISSION, true);
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim()) 
            return;

        if(!LockedTarget)
        {
            m_creature->AddThreat(m_creature->getVictim(), 10000000.0f);
            LockedTarget = true;
        }
        
        if(ExplodeTimer) // Just so it doesn't explode as soon as it spawn
        {
            if(ExplodeTimer < diff) 
                ExplodeTimer = 0;
            else ExplodeTimer -= diff;
        }
        else if(m_creature->IsWithinDistInMap(m_creature->getVictim(), 3)) // Explode if it's close enough to it's target
        {
            DoCast(m_creature->getVictim(), SPELL_FELFIRE_FISSION);
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }
};

CreatureAI* GetAI_boss_kiljaeden(Creature *_Creature)
{
    return new boss_kiljaedenAI (_Creature);
}

CreatureAI* GetAI_mob_kiljaeden_controller(Creature *_Creature)
{
    return new mob_kiljaeden_controllerAI (_Creature);
}

CreatureAI* GetAI_mob_hand_of_the_deceiver(Creature *_Creature)
{
    return new mob_hand_of_the_deceiverAI (_Creature);
}

CreatureAI* GetAI_mob_felfire_portal(Creature *_Creature)
{
    return new mob_felfire_portalAI (_Creature);
}

CreatureAI* GetAI_mob_volatile_felfire_fiend(Creature *_Creature)
{
    return new mob_volatile_felfire_fiendAI (_Creature);
}

void AddSC_boss_kiljaeden()
{
    Script* newscript;

    newscript = new Script;
    newscript->pGOHello = &GOHello_go_orb_of_the_blue_flight;
    newscript->Name = "go_orb_of_the_blue_flight";
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->GetAI = &GetAI_boss_kiljaeden;
    newscript->Name = "boss_kiljaeden";
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->GetAI = &GetAI_mob_kiljaeden_controller;
    newscript->Name = "mob_kiljaeden_controller";
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->GetAI = &GetAI_mob_hand_of_the_deceiver;
    newscript->Name = "mob_hand_of_the_deceiver";
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->GetAI = &GetAI_mob_felfire_portal;
    newscript->Name = "mob_felfire_portal";
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->GetAI = &GetAI_mob_volatile_felfire_fiend;
    newscript->Name = "mob_volatile_felfire_fiend";
    newscript->RegisterSelf();
};
