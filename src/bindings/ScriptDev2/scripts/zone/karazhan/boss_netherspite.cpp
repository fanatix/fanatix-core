 
/* ScriptData
SDName: boss_netherspite
SD%Complete: 100%
SDComment: finished
SDCategory: Karazhan
EndScriptData */
 
/*
Important for an working script!
UPDATE `creature_template` SET `ScriptName`='boss_netherspite' WHERE `entry`='15689';
UPDATE `creature_template` SET `minlevel` = 70, `maxlevel` = 70, `flags`= 33554496, `faction_A` = 16, `faction_H` = 16, `ScriptName` = 'mob_void_zone'  WHERE `entry` = 16697;
UPDATE `creature_template` SET `faction_A` = 35, `faction_H` = 35, `flags` = 33554496 WHERE `entry` = 17367;
UPDATE `creature_template` SET `faction_A` = 35, `faction_H` = 35, `flags` = 33554496 WHERE `entry` = 17368;
UPDATE `creature_template` SET `faction_A` = 35, `faction_H` = 35, `flags` = 33554496 WHERE `entry` = 17369;
*/
 
#include "precompiled.h"
#include "def_karazhan.h"
 
#define SPELL_NETHERBURN 30523
// #define SPELL_VOIDZONE 37063 Not needed in this script, used SummonCreature() instead of the spell
#define SPELL_NETHERBREATH 38523
#define SPELL_EMPOWERMENT 38549
#define SPELL_ENRAGE 38688
#define SPELL_VOIDEFFECT 46264 //tick time set to 3 sec
#define SPELL_BANISH_VISUAL 39833
 
#define SPELL_PERSEVERENCE_NS 30466
#define SPELL_PERSEVERENCE_PLR 30421
#define SPELL_SERENITY_NS 30467
#define SPELL_SERENITY_PLR 30422
#define SPELL_DOMINANCE_NS 30468
#define SPELL_DOMINANCE_PLR 30423
#define SPELL_BEAM_DOM 30463 //30402
#define SPELL_BEAM_SER 30464 //30401
#define SPELL_BEAM_PER 30465 //30400
 
#define BLUE_PORTAL 30491
#define GREEN_PORTAL 30490
#define RED_PORTAL 30487
#define SPELL_EXHAUSTION_DOM 38639
#define SPELL_EXHAUSTION_SER 38638
#define SPELL_EXHAUSTION_PER 38637
 
#define BOSS_NETHERSPITE 15363
#define CREATURE_BLUEPORTAL 17368
#define CREATURE_GREENPORTAL 17367
#define CREATURE_REDPORTAL 17369
#define CREATURE_VOID_ZONE 16697
 
#define EMOTE_PHASE_PORTAL          -1532089
#define EMOTE_PHASE_BANISH          -1532090
 
#define SAY_PORTAL "cries out in withdrawal, opening gates to the warp."
#define SAY_BANISH "goes into a nether-fed rage!"
 
//Portal Fixed Positions
float BasicCoords[3][3] =
{
    {-11137.846680, -1685.607422, 278.239258}, // red
    {-11094.493164, -1591.969238, 279.949188}, // blue
    {-11195.353516, -1613.237183, 278.237258}  // green
};
 
//Portal Random Positions
float RandomedCoords[3][3] =
{
    {-11094.493164, -1591.969238, 279.949188},
    {-11195.353516, -1613.237183, 278.237258},
    {-11137.846680, -1685.607422, 278.239258}
};
 
 
struct MANGOS_DLL_DECL boss_netherspiteAI : public ScriptedAI {
    /*Unit* DOMcandidate;
    Unit* SERcandidate;
    Unit* PERcandidate;
    Unit* lastDOMcandidate;
    Unit* lastSERcandidate;
    Unit* lastPERcandidate;*/
    Unit* plr;
    Unit* ExhaustCandsDOM[10];
    Unit* ExhaustCandsSER[10];
    Unit* ExhaustCandsPER[10];
    Unit* PERMaxHealth[10];
 
    uint32 Netherburn_timer;
        uint32 Voidzone_timer;
        uint32 Netherbreath_timer;
        uint32 Empowerment_timer;
        uint32 Enrage_timer;
        uint32 PortalPhase_timer;
        uint32 BanishPhase_timer;
        uint32 Beam_initialtimer;
        uint32 Beam_periodictimer;
        uint32 ExhaustCheck_timer;
 
    bool HasEmpowered;
        bool InCombat;
        bool PortalPhase;
        bool BanishPhase;
        bool Enraged;
        bool PortalsSpawned;
        bool existsDOM;
        bool existsSER;
        bool existsPER;
        bool PERhealthMaxed;
        bool PLRonRedBeam;
 
        bool bReset; // used for the portals, after the reset, the portals spawn in the startphase
 
    float BossDist;
        float PlrDist;
        float BossAngle;
        float PlrAngle;
        float degrad10;
 
    uint64 BeamerGUID[3];
    uint64 BeamerhelpGUID[3];
    uint64 CandidatesGUID[3];
    uint64 LastCandidatesGUID[3];
    uint64 VoidZone[6];
    float ZoneCoords[2];        // used for spawning the void zones around netherspite, for getting the coords of netherspite
    int voidZones_random;       // used for spawning the void zones around netherspite, specially to spawn the void zones in a kind of circle
    int voidZones_counter;      // counts the void zones
 
        ScriptedInstance* pInstance;
 
 
    boss_netherspiteAI(Creature *c) : ScriptedAI(c) {
                pInstance = ((ScriptedInstance*)c->GetInstanceData());
                Reset();
        }
 
    void Reset() {
        LastCandidatesGUID[0] = m_creature->GetGUID();
        LastCandidatesGUID[1] = m_creature->GetGUID();
        LastCandidatesGUID[2] = m_creature->GetGUID();
        /*lastDOMcandidate = Unit::GetUnit((*m_creature), LastCandidatesGUID[0]);
        lastSERcandidate = Unit::GetUnit((*m_creature), LastCandidatesGUID[1]);
        lastPERcandidate = Unit::GetUnit((*m_creature), LastCandidatesGUID[2]);*/
        Netherburn_timer = 5000;
        Voidzone_timer = 15000;
        Netherbreath_timer = 2500;
        Empowerment_timer = 6000;
        HasEmpowered = false;
        Enrage_timer = 540000;
        Enraged = false;
        InCombat = false;
        PortalPhase = true;
        BanishPhase = false;
        PortalPhase_timer = 60000;
        BanishPhase_timer = 30000;
        PortalsSpawned = false;
        Beam_initialtimer = 9000;
        Beam_periodictimer = 1000;
        ExhaustCheck_timer = 2000;
        PLRonRedBeam = false;
        degrad10 = 0.174;
        voidZones_counter = 0;
                
                bReset = true;
 
        for(int i=0;i<10;i++) {
            ExhaustCandsDOM[i] = NULL;
            ExhaustCandsSER[i] = NULL;
            ExhaustCandsPER[i] = NULL;
            PERMaxHealth[i] = NULL;
        }
 
        m_creature->RemoveAllAuras();
        m_creature->CombatStop();
        DoResetThreat();
        DestroyPortals();
                DestroyVoidZone();
 
                GameObject* Door = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_GAMEOBJECT_MASSIVE_DOOR));
                if(Door)
                        Door->SetGoState(0); // open the door
    }
 
    void RandomizeCoords() {
        //switch the portals
                int i[3];
                i[0] = rand()%3;                        // set RED 
                i[1] = i[0]+rand()%2+1;
                if(i[1] > 2) {
                        i[1] -= 3;                              // set GREEN
                }
                i[2]=3-i[1]-i[0];                       // set BLUE 
 
                memcpy(&RandomedCoords[0], &BasicCoords[i[0]], sizeof(BasicCoords[i[0]]));
                memcpy(&RandomedCoords[1], &BasicCoords[i[1]], sizeof(BasicCoords[i[1]]));
                memcpy(&RandomedCoords[2], &BasicCoords[i[2]], sizeof(BasicCoords[i[2]]));
    }
        
    bool NetherBeam(int beamerID) {
                if(beamerID < 0 || beamerID > 2)
                        return false;
 
                Map *map = m_creature->GetMap();
        if(!map->IsDungeon())
                        return false;
 
        Unit* uBeamer = Unit::GetUnit(*m_creature, BeamerGUID[beamerID]);
                if(!uBeamer)
                        return false;
 
        Unit* uBeamerHelp = Unit::GetUnit(*m_creature, BeamerhelpGUID[beamerID]);
                if(!uBeamerHelp)
                        return false;
 
        CandidatesGUID[beamerID] = m_creature->GetGUID();
        Unit* uCandidate = Unit::GetUnit(*m_creature, CandidatesGUID[beamerID]);
                if(!uCandidate)
                        return false;
 
                int spell_exhaustion    = -1;
                Unit* uLastCandidate    = NULL;
                int creature_portal             = -1;
                int spell_plr                   = -1;
                int spell_creature              = -1;
                int spell_beam                  = -1;
 
                // switch beam specific settings/variables
                uLastCandidate = Unit::GetUnit(*m_creature, LastCandidatesGUID[beamerID]);
                switch(beamerID) {
                case 0: // blue
                        spell_exhaustion        = SPELL_EXHAUSTION_DOM;
                        //uLastCandidate                = lastDOMcandidate;
                        creature_portal         = CREATURE_BLUEPORTAL;
                        spell_plr                       = SPELL_DOMINANCE_PLR;
                        spell_creature          = SPELL_DOMINANCE_NS;
                        spell_beam                      = SPELL_BEAM_DOM;
                        break;
                case 1: // green
                        spell_exhaustion        = SPELL_EXHAUSTION_SER;
                        //uLastCandidate                = lastSERcandidate;
                        creature_portal         = CREATURE_GREENPORTAL;
                        spell_plr                       = SPELL_SERENITY_PLR;
                        spell_creature          = SPELL_SERENITY_NS;
                        spell_beam                      = SPELL_BEAM_SER;
                        break;
                case 2: // red
                        spell_exhaustion        = SPELL_EXHAUSTION_PER;
                        //uLastCandidate                = lastPERcandidate;
                        creature_portal         = CREATURE_REDPORTAL;
                        spell_plr                       = SPELL_PERSEVERENCE_PLR;
                        spell_creature          = SPELL_PERSEVERENCE_NS;
                        spell_beam                      = SPELL_BEAM_PER;
                        break;
                default:
                        return false; // oO? something got wrong...
                }
 
                // step throught each player in the list and check, if he is in the beam
        Map::PlayerList const &PlayerList = map->GetPlayers();
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i) {
            if(i->getSource() && i->getSource()->isAlive()) {
                                plr = Unit::GetUnit(*m_creature, i->getSource()->GetGUID());
                                BossDist = m_creature->GetDistance2d(uBeamer);
                                BossAngle = m_creature->GetAngle(uBeamer);
                                PlrDist = plr->GetDistance2d(uBeamer);
                                PlrAngle = plr->GetAngle(uBeamer);
 
                                if((BossAngle - degrad10) < PlrAngle    // is in range?
                                &&  PlrAngle < (BossAngle + degrad10)   // is in range?
                                &&  PlrDist < BossDist                                  // is in range?
                                && !plr->HasAura(spell_exhaustion,0)    // hasn't yet the spell?
                                &&  plr->isAlive())     {                                       // is alive?
                                        uCandidate = plr;                                       // ok, take him as candidate
                                        if(beamerID==2) // red beam
                                                PLRonRedBeam = true;
                                }
                        }
        }
 
                if(uCandidate == m_creature) {
                        if(uCandidate != uLastCandidate) {
                                if(uBeamer) {
                    uBeamer->DealDamage(uBeamerHelp, uBeamerHelp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    if(uBeamerHelp) {
                        BeamerhelpGUID[beamerID] = 0;
                        uBeamerHelp = NULL;
                        uBeamerHelp = uBeamer->SummonCreature(creature_portal,RandomedCoords[beamerID][0],RandomedCoords[beamerID][1],RandomedCoords[beamerID][2],0,TEMPSUMMON_CORPSE_DESPAWN,0);
                        BeamerhelpGUID[beamerID] = uBeamerHelp->GetGUID();
                        uBeamerHelp->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                        uBeamerHelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        uBeamerHelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }
                }
                uLastCandidate = uCandidate;
            }
 
            uBeamerHelp = Unit::GetUnit(*m_creature, BeamerhelpGUID[beamerID]);
            if(uBeamer)
                uBeamer->CastSpell(m_creature,spell_creature,true);
            if(uBeamerHelp)
                uBeamerHelp->CastSpell(uCandidate,spell_beam,true);
                } else {
            if(uCandidate != uLastCandidate) {
                                if(uBeamer) {
                    uBeamer->DealDamage(uBeamerHelp, uBeamerHelp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    if(uBeamerHelp) {
                        BeamerhelpGUID[beamerID] = 0;
                        uBeamerHelp = NULL;
                        uBeamerHelp = uBeamer->SummonCreature(creature_portal,RandomedCoords[beamerID][0],RandomedCoords[beamerID][1],RandomedCoords[beamerID][2],0,TEMPSUMMON_CORPSE_DESPAWN,0);
                        BeamerhelpGUID[beamerID] = uBeamerHelp->GetGUID();
                        uBeamerHelp->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                        uBeamerHelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        uBeamerHelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }
                }
                uLastCandidate = uCandidate;
                ExhaustHandler(uCandidate,beamerID);
            }
 
            uBeamerHelp = Unit::GetUnit(*m_creature, BeamerhelpGUID[beamerID]);
            if(uBeamer)
                uBeamer->CastSpell(uCandidate,spell_plr,true);
            if(uBeamerHelp)
                                uBeamerHelp->CastSpell(uCandidate,spell_beam,true);
        }
 
                LastCandidatesGUID[beamerID] = uLastCandidate->GetGUID();
                CandidatesGUID[beamerID] = uCandidate->GetGUID();
 
                return true;
    }
 
        /*
    void BlueNetherBeam() {
        //this is the blue beam (dominance)
        Unit* BeamerD = NULL;
        BeamerD = Unit::GetUnit(*m_creature, BeamerGUID[0]);
        Unit* BeamerDhelp = NULL;
        BeamerDhelp = Unit::GetUnit(*m_creature, BeamerhelpGUID[0]);
        CandidatesGUID[0] = m_creature->GetGUID();
        DOMcandidate = Unit::GetUnit(*m_creature, CandidatesGUID[0]);
 
                Map *map = m_creature->GetMap();
        if(!map->IsDungeon())
                        return;
 
        InstanceMap::PlayerList const &PlayerList = ((InstanceMap*)map)->GetPlayers();
        for (InstanceMap::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i) {
            if((*i) && (*i)->isAlive()) {
                                plr = Unit::GetUnit((*m_creature), (*i)->GetGUID());
                                BossDist = m_creature->GetDistance2d(BeamerD);
                                BossAngle = m_creature->GetAngle(BeamerD);
                                PlrDist = plr->GetDistance2d(BeamerD);
                                PlrAngle = plr->GetAngle(BeamerD);
 
                                if((BossAngle - degrad10) < PlrAngle    // is in range?
                                &&  PlrAngle < (BossAngle + degrad10)   // is in range?
                                &&  PlrDist < BossDist                                  // is in range?
                                && !plr->HasAura(SPELL_EXHAUSTION_DOM,0)// hasn't yet the spell?
                                &&  plr->isAlive())                                             // is alive?
                                        DOMcandidate = plr;                                     // ok, take him as candidate
                        }
        }
 
                if(DOMcandidate == m_creature) {
                        if(DOMcandidate != lastDOMcandidate) {
                                if(BeamerD) {
                    BeamerD->DealDamage(BeamerDhelp, BeamerDhelp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    if(BeamerDhelp) {
                        BeamerhelpGUID[0] = 0;
                        BeamerDhelp = NULL;
                        BeamerDhelp = BeamerD->SummonCreature(CREATURE_BLUEPORTAL,RandomedCoords[0][0],RandomedCoords[0][1],RandomedCoords[0][2],0,TEMPSUMMON_CORPSE_DESPAWN,0);
                        BeamerhelpGUID[0] = BeamerDhelp->GetGUID();
                        BeamerDhelp->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                        BeamerDhelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        BeamerDhelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }
                }
                lastDOMcandidate = DOMcandidate;
            }
 
            BeamerDhelp = Unit::GetUnit((*m_creature), BeamerhelpGUID[0]);
            if(BeamerD)
                BeamerD->CastSpell(m_creature,SPELL_DOMINANCE_NS,true);
            if(BeamerDhelp)
                BeamerDhelp->CastSpell(DOMcandidate,SPELL_BEAM_DOM,true);
                } else {
            if(DOMcandidate != lastDOMcandidate) {
                                if(BeamerD) {
                    BeamerD->DealDamage(BeamerDhelp, BeamerDhelp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    if(BeamerDhelp) {
                        BeamerhelpGUID[0] = 0;
                        BeamerDhelp = NULL;
                        BeamerDhelp = BeamerD->SummonCreature(CREATURE_BLUEPORTAL,RandomedCoords[0][0],RandomedCoords[0][1],RandomedCoords[0][2],0,TEMPSUMMON_CORPSE_DESPAWN,0);
                        BeamerhelpGUID[0] = BeamerDhelp->GetGUID();
                        BeamerDhelp->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                        BeamerDhelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        BeamerDhelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }
                }
                lastDOMcandidate = DOMcandidate;
                ExhaustHandler(DOMcandidate,0);
            }
 
            BeamerDhelp = Unit::GetUnit((*m_creature), BeamerhelpGUID[0]);
            DOMcandidate->CastSpell(DOMcandidate,SPELL_DOMINANCE_PLR,true);
            if(BeamerDhelp)
                                BeamerDhelp->CastSpell(DOMcandidate,SPELL_BEAM_DOM,true);
        }
    }
 
    void GreenNetherBeam() {
        //this is the green beam (serenity)
 
        Unit* BeamerS = NULL;
        BeamerS = Unit::GetUnit((*m_creature), BeamerGUID[1]);
        Unit* BeamerShelp = NULL;
        BeamerShelp = Unit::GetUnit((*m_creature), BeamerhelpGUID[1]);
        CandidatesGUID[1] = m_creature->GetGUID();
        SERcandidate = Unit::GetUnit((*m_creature), CandidatesGUID[1]);
                
                Map *map = m_creature->GetMap();
        if(!map->IsDungeon())
                        return;
 
        InstanceMap::PlayerList const &PlayerList = ((InstanceMap*)map)->GetPlayers();
        for (InstanceMap::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i) {
            if((*i) && (*i)->isAlive()) {
                                plr = Unit::GetUnit((*m_creature), (*i)->GetGUID());
                                BossDist = m_creature->GetDistance2d(BeamerS);
                                BossAngle = m_creature->GetAngle(BeamerS);
                                PlrDist = plr->GetDistance2d(BeamerS);
                                PlrAngle = plr->GetAngle(BeamerS);
 
                                if((BossAngle - degrad10) < PlrAngle    // is in range?
                                &&  PlrAngle < (BossAngle + degrad10)   // is in range?
                                &&  PlrDist < BossDist                                  // is in range?
                                && !plr->HasAura(SPELL_EXHAUSTION_SER,0)// hasn't yet the spell?
                                &&  plr->isAlive())                                             // is alive?
                                        SERcandidate = plr;                                     // ok, take him as candidate
            }
        }
 
        if(SERcandidate == m_creature) {
            if(SERcandidate != lastSERcandidate) {
                if(BeamerS) {
                    BeamerS->DealDamage(BeamerShelp, BeamerShelp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    if(BeamerShelp) {
                        BeamerhelpGUID[1] = 0;
                        BeamerShelp = NULL;
                        BeamerShelp = BeamerS->SummonCreature(CREATURE_GREENPORTAL,RandomedCoords[1][0],RandomedCoords[1][1],RandomedCoords[1][2],0,TEMPSUMMON_CORPSE_DESPAWN,0);
                        BeamerhelpGUID[1] = BeamerShelp->GetGUID();
                        BeamerShelp->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                        BeamerShelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        BeamerShelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }
                }
                lastSERcandidate = SERcandidate;
            }
 
            BeamerShelp = Unit::GetUnit((*m_creature), BeamerhelpGUID[1]);
            if(BeamerS)
                BeamerS->CastSpell(SERcandidate,SPELL_SERENITY_NS,true);
            if(BeamerShelp)
                BeamerShelp->CastSpell(SERcandidate,SPELL_BEAM_SER,true);
        } else {
            if(SERcandidate != lastSERcandidate) {
                if(BeamerS) {
                    BeamerS->DealDamage(BeamerShelp, BeamerShelp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    if(BeamerShelp) {
                        BeamerhelpGUID[1] = 0;
                        BeamerShelp = NULL;
                        BeamerShelp = BeamerS->SummonCreature(CREATURE_GREENPORTAL,RandomedCoords[1][0],RandomedCoords[1][1],RandomedCoords[1][2],0,TEMPSUMMON_CORPSE_DESPAWN,0);
                        BeamerhelpGUID[1] = BeamerShelp->GetGUID();
                        BeamerShelp->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                        BeamerShelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        BeamerShelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }
                }
                lastSERcandidate = SERcandidate;
                ExhaustHandler(SERcandidate,1);
            }
 
            BeamerShelp = Unit::GetUnit((*m_creature), BeamerhelpGUID[1]);
            if(BeamerS)
                BeamerS->CastSpell(SERcandidate,SPELL_SERENITY_PLR,true);
            if(BeamerShelp)
                BeamerShelp->CastSpell(SERcandidate,SPELL_BEAM_SER,true);
                SERcandidate->ModifyPower(POWER_MANA, SERcandidate->GetMaxPower(POWER_MANA));
        }
    }
 
    void RedNetherBeam() {
        //this is the red beam (perseverence)
        Unit* BeamerP = NULL;
        BeamerP = Unit::GetUnit((*m_creature), BeamerGUID[2]);
        Unit* BeamerPhelp = NULL;
        BeamerPhelp = Unit::GetUnit((*m_creature), BeamerhelpGUID[2]);
        CandidatesGUID[2] = m_creature->GetGUID();
        PERcandidate = Unit::GetUnit((*m_creature), CandidatesGUID[2]);
 
        PLRonRedBeam = false;
                
                Map *map = m_creature->GetMap();
        if(!map->IsDungeon()) return;
 
        InstanceMap::PlayerList const &PlayerList = ((InstanceMap*)map)->GetPlayers();
        for (InstanceMap::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i) {
            if((*i) && (*i)->isAlive()) {
                                plr = Unit::GetUnit((*m_creature), (*i)->GetGUID());
                                BossDist = m_creature->GetDistance2d(BeamerP);
                                BossAngle = m_creature->GetAngle(BeamerP);
                                PlrDist = plr->GetDistance2d(BeamerP);
                                PlrAngle = plr->GetAngle(BeamerP);
 
                                if((BossAngle - degrad10) < PlrAngle    // is in range?
                                &&  PlrAngle < (BossAngle + degrad10)   // is in range?
                                &&  PlrDist < BossDist                                  // is in range?
                                && !plr->HasAura(SPELL_EXHAUSTION_PER,0)// hasn't yet the spell?
                                &&  plr->isAlive())     {                                       // is alive?
                                        PERcandidate = plr;                                     // ok, take him as candidate
                                        PLRonRedBeam = true;
                                }
            }
        }
 
        if(PERcandidate == m_creature) {
            if(PERcandidate != lastPERcandidate) {
                if(BeamerP) {
                    BeamerP->DealDamage(BeamerPhelp, BeamerPhelp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    if(BeamerPhelp) {
                        BeamerhelpGUID[2] = 0;
                        BeamerPhelp = NULL;
                        BeamerPhelp = BeamerP->SummonCreature(CREATURE_REDPORTAL,RandomedCoords[2][0],RandomedCoords[2][1],RandomedCoords[2][2],0,TEMPSUMMON_CORPSE_DESPAWN,0);
                        BeamerhelpGUID[2] = BeamerPhelp->GetGUID();
                        BeamerPhelp->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                        BeamerPhelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        BeamerPhelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }
                }
                lastPERcandidate = PERcandidate;
            }
            BeamerPhelp = Unit::GetUnit((*m_creature), BeamerhelpGUID[2]);
            if(BeamerP)
                BeamerP->CastSpell(PERcandidate,SPELL_PERSEVERENCE_NS,true);
            if(BeamerPhelp)
                BeamerPhelp->CastSpell(PERcandidate,SPELL_BEAM_PER,true);
        } else {
            if(PERcandidate != lastPERcandidate) {
                if(BeamerP) {
                    BeamerP->DealDamage(BeamerPhelp, BeamerPhelp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    if(BeamerPhelp) {
                        BeamerhelpGUID[2] = 0;
                        BeamerPhelp = NULL;
                        BeamerPhelp = BeamerP->SummonCreature(CREATURE_REDPORTAL,RandomedCoords[2][0],RandomedCoords[2][1],RandomedCoords[2][2],0,TEMPSUMMON_CORPSE_DESPAWN,0);
                        BeamerhelpGUID[2] = BeamerPhelp->GetGUID();
                        BeamerPhelp->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                        BeamerPhelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        BeamerPhelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }
                }
                lastPERcandidate = PERcandidate;
                ExhaustHandler(PERcandidate,2);
                PERhealthHandler(PERcandidate);
            }
 
            BeamerPhelp = Unit::GetUnit((*m_creature), BeamerhelpGUID[2]);
            if(BeamerP)
                BeamerP->CastSpell(PERcandidate,SPELL_PERSEVERENCE_PLR,true);
            if(BeamerPhelp)
                BeamerPhelp->CastSpell(PERcandidate,SPELL_BEAM_PER,true);
                PERcandidate->ModifyHealth(PERcandidate->GetMaxHealth());
        }
    }
        */
 
    void ExhaustHandler(Unit *plr, int colour) {
        //store the beam blockers, for later debuffing them
        existsDOM = false;
        existsSER = false;
        existsPER = false;
 
        if(colour == 0) {
            for(int i=0;i<10;i++) {
                if(ExhaustCandsDOM[i] == plr) {
                    existsDOM=true;
                    break;
                }
            }
 
            if(!existsDOM) {
                for(int i=0;i<10;i++) {
                    if(ExhaustCandsDOM[i] == NULL) {
                        ExhaustCandsDOM[i] = plr;
                        break;
                    }
                }
            }
        }
 
        if(colour == 1) {
            for(int i=0;i<10;i++) {
                if(ExhaustCandsSER[i] == plr) {
                    existsSER=true;
                    break;
                }
            }
 
            if(!existsSER) {
                for(int i=0;i<10;i++) {
                    if(ExhaustCandsSER[i] == NULL) {
                        ExhaustCandsSER[i] = plr;
                        break;
                    }
                }
            }
        }
 
        if(colour == 2) {
            for(int i=0;i<10;i++) {
                if(ExhaustCandsPER[i] == plr) {
                    existsPER=true;
                    break;
                }
            }
 
            if(!existsPER) {
                for(int i=0;i<10;i++) {
                    if(ExhaustCandsPER[i] == NULL) {
                        ExhaustCandsPER[i] = plr;
                        break;
                    }
                }
            }
        }
    }
 
    void PERhealthHandler(Unit *plr) {
        //store red beam blocker to keep an eye on his health
        PERhealthMaxed = false;
 
        for(int i=0;i<10;i++) {
            if(PERMaxHealth[i] == plr) {
                PERhealthMaxed = true;
                break;
            }
        }
 
        if(!PERhealthMaxed) {
            for(int i=0;i<10;i++) {
                if(PERMaxHealth[i] == NULL) {
                    PERMaxHealth[i] = plr;
                    int32 bp0 = -1;
                    int32 bp1 = 5;
                    int32 bp2 = 31000;
                    plr->CastCustomSpell(plr, SPELL_PERSEVERENCE_PLR, &bp0, &bp1, &bp2, true);
                    PERhealthMaxed = true;
                    break;
                }
            }
        }
    }
 
    void DELExhaustCandDOM() {
        //check for possible targets of dominance exhaustion
        for(int i=0;i<10;i++) {
            if(ExhaustCandsDOM[i] != NULL) {
                if(!ExhaustCandsDOM[i]->HasAura(SPELL_DOMINANCE_PLR,0)) {
                    ExhaustCandsDOM[i]->CastSpell(ExhaustCandsDOM[i],SPELL_EXHAUSTION_DOM,true);
                    ExhaustCandsDOM[i] = NULL;
                }
            }
        }
    }
 
    void DELExhaustCandSER() {
        //check for possible targets of serenity exhaustion
        for(int i=0;i<10;i++) {
            if(ExhaustCandsSER[i] != NULL) {
                if(!ExhaustCandsSER[i]->HasAura(SPELL_SERENITY_PLR,0)) {
                    ExhaustCandsSER[i]->CastSpell(ExhaustCandsSER[i],SPELL_EXHAUSTION_SER,true);
                    ExhaustCandsSER[i] = NULL;
                }
            }
        }
    }
 
    void DELExhaustCandPER() {
        //check for possible targets of perseverence exhaustion
        for(int i=0;i<10;i++) {
            if(ExhaustCandsPER[i] != NULL) {
                if(!ExhaustCandsPER[i]->HasAura(SPELL_PERSEVERENCE_PLR,0)) {
                    for(int j=0;j<10;j++) {
                        if(PERMaxHealth[j] == ExhaustCandsPER[i]) {
                            PERMaxHealth[j] = NULL;
                            break;
                        }
                    }
                    ExhaustCandsPER[i]->CastSpell(ExhaustCandsPER[i],SPELL_EXHAUSTION_PER,true);
                    ExhaustCandsPER[i] = NULL;
                }
            }
        }
    }
 
    void DestroyVoidZone() {
        for(int i= 0; i<7;i++) {
                        if(VoidZone[i]) {
                Unit* uVoidZone = Unit::GetUnit((*m_creature), VoidZone[i]);
                if(uVoidZone)
                    uVoidZone->DealDamage(uVoidZone, uVoidZone->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                VoidZone[i] = NULL;
            }
        }
    }
 
    void Aggro(Unit *who) {
        DoZoneInCombat();
    }
/*
    void AttackStart(Unit *who) {
        if(!who)
            return;
 
        if(!PLRonRedBeam) {
            if (who->isTargetableForAttack() && who->isAlive() && who != m_creature) {
                //DoStartAttackAndMovement(who); // after rev. 698: DoStartMovement()
 
                if (!InCombat) {
                   InCombat = true;
                }
            }
                } else {
                        Unit* uTmp = Unit::GetUnit(*m_creature,CandidatesGUID[2]);
                        //if(uTmp && uTmp->IsHostileTo(m_creature))
                                //DoStartAttackAndMovement(uTmp); // after rev. 698: DoStartMovement()
        }
    }
*/
    void AttackStart(Unit *who)
    {
        if (!who)
            return;
 
        Unit* pTemp = who;
 
        if (InCombat && pTemp->GetTypeId() == TYPEID_PLAYER)
        {
            if (PLRonRedBeam)
            {
                if (Unit* uTmp = Unit::GetUnit(*m_creature,CandidatesGUID[2]))
                    pTemp = uTmp;
            }
        }
 
        if (m_creature->Attack(pTemp, true))
        {
            m_creature->AddThreat(pTemp, 0.0f);
            m_creature->SetInCombatWith(pTemp);
            pTemp->SetInCombatWith(m_creature);
 
            if (!InCombat)
            {
                InCombat = true;
                Aggro(pTemp);
            }
 
            DoStartMovement(pTemp);
        }
    }
 
    void DestroyPortals() {
        //we destroy portals during banish phase or when we die
        Unit* BeamerD = NULL;
        Unit* BeamerDhelp = NULL;
        BeamerD = Unit::GetUnit((*m_creature), BeamerGUID[0]);
        BeamerDhelp = Unit::GetUnit((*m_creature), BeamerhelpGUID[0]);
        if (BeamerD && BeamerD->isAlive()) {
            BeamerD->DealDamage(BeamerD, BeamerD->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            BeamerGUID[0] = 0;
        }
        if (BeamerDhelp && BeamerDhelp->isAlive()) {
            BeamerDhelp->DealDamage(BeamerDhelp, BeamerDhelp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            BeamerhelpGUID[0] = 0;
        }
 
        Unit* BeamerS = NULL;
        Unit* BeamerShelp = NULL;
        BeamerS = Unit::GetUnit((*m_creature), BeamerGUID[1]);
        BeamerShelp = Unit::GetUnit((*m_creature), BeamerhelpGUID[1]);
        if (BeamerS && BeamerS->isAlive()) {
            BeamerS->DealDamage(BeamerS, BeamerS->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            BeamerGUID[1] = 0;
        }
        if (BeamerShelp && BeamerShelp->isAlive()) {
            BeamerShelp->DealDamage(BeamerShelp, BeamerShelp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            BeamerhelpGUID[1] = 0;
        }
 
        Unit* BeamerP = NULL;
        Unit* BeamerPhelp = NULL;
        BeamerP = Unit::GetUnit((*m_creature), BeamerGUID[2]);
        BeamerPhelp = Unit::GetUnit((*m_creature), BeamerhelpGUID[2]);
        if (BeamerP && BeamerP->isAlive()) {
            BeamerP->DealDamage(BeamerP, BeamerP->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            BeamerGUID[2] = 0;
        }
        if (BeamerPhelp && BeamerPhelp->isAlive()) {
            BeamerPhelp->DealDamage(BeamerPhelp, BeamerPhelp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            BeamerhelpGUID[2] = 0;
        }
    }
 
    void MoveInLineOfSight(Unit *who) {
        if (!who || m_creature->getVictim())
                        return;
 
        if(!PLRonRedBeam) {
            if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who) && who->isAlive()) {
                float attackRadius = m_creature->GetAttackDistance(who);
                if (m_creature->IsWithinDistInMap(who, attackRadius)
                                 && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE
                                 && m_creature->IsWithinLOSInMap(who)) {
                    if(who->HasStealthAura())
                                                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
 
                    //m_creature->AddThreat(who,0.0f);
                    //DoStartAttackAndMovement(who); /* after rev. 698: DoStartMovement() */
                    //if (!InCombat)
                    //    InCombat = true;
 
                                        if (m_creature->Attack(who, true))
                                        {
                                                m_creature->AddThreat(who, 0.0f);
                                                m_creature->SetInCombatWith(who);
                                                who->SetInCombatWith(m_creature);
 
                                                if (!InCombat)
                                                {
                                                        InCombat = true;
                                                        Aggro(who);
                                                }
 
                                                DoStartMovement(who);
                                        }
                }
            }
                } else {
                        Unit* uTmp = Unit::GetUnit(*m_creature,CandidatesGUID[2]);
                        //if(uTmp && uTmp->IsHostileTo(m_creature))
                                //DoStartAttackAndMovement(uTmp); /* after rev. 698: DoStartMovement() */
                        if (m_creature->Attack(uTmp, true))
                        {
                                m_creature->AddThreat(uTmp, 0.0f);
                                m_creature->SetInCombatWith(uTmp);
                                uTmp->SetInCombatWith(m_creature);
 
                                if (!InCombat)
                                {
                                        InCombat = true;
                                        Aggro(uTmp);
                                }
 
                                DoStartMovement(uTmp);
                        }
        }
    }
 
    void UpdateAI(const uint32 diff) {
                
                if(!m_creature->SelectHostilTarget())
            return;
 
                GameObject* Door = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_GAMEOBJECT_MASSIVE_DOOR));
                if(Door)
                        Door->SetGoState(1); // open the door
 
        if(!m_creature->getVictim() || !m_creature->isAlive())
                        return;
 
        if(ExhaustCheck_timer < diff) {
            //periodic exhaustion debuff check
            DELExhaustCandDOM();
            DELExhaustCandSER();
            DELExhaustCandPER();
            ExhaustCheck_timer = 2000;
        }else ExhaustCheck_timer -= diff;
 
        if(Enrage_timer < diff) {
            if(!Enraged) {
                DoCast(m_creature,SPELL_ENRAGE);
                Enraged = true;
            }
        }else Enrage_timer -= diff;
 
        if(PortalPhase) {
            if(PortalPhase_timer < diff) {
                m_creature->CastSpell(m_creature, SPELL_BANISH_VISUAL,0);
                DoTextEmote(SAY_BANISH, NULL, true);
                BanishPhase = true;
                PortalPhase = false;
                PLRonRedBeam = false;
                DestroyPortals();
                                if(!bReset) {
                                        RandomizeCoords();
                                } else {
                                        memcpy(&RandomedCoords, &BasicCoords, sizeof(BasicCoords)); // copy the basic coords at start
                                        bReset = false;
                                }
                PortalPhase_timer = 60000;
                (*m_creature).GetMotionMaster()->MoveIdle();
            }else PortalPhase_timer -= diff;
 
            if(!HasEmpowered) {
                if(Empowerment_timer < diff) {
                    DoCast(m_creature,SPELL_EMPOWERMENT);
                    HasEmpowered = true;
                }else Empowerment_timer -= diff;
            }
 
            if(!PortalsSpawned) {
                Creature* BeamerD = m_creature->SummonCreature(CREATURE_BLUEPORTAL,RandomedCoords[0][0],RandomedCoords[0][1],RandomedCoords[0][2],0,TEMPSUMMON_CORPSE_DESPAWN,0);
                if(BeamerD) {
                    BeamerGUID[0] = BeamerD->GetGUID();
                    BeamerD->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                    BeamerD->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    BeamerD->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }
 
                Creature* BeamerS = m_creature->SummonCreature(CREATURE_GREENPORTAL,RandomedCoords[1][0],RandomedCoords[1][1],RandomedCoords[1][2],0,TEMPSUMMON_CORPSE_DESPAWN,0);
                if(BeamerS) {
                    BeamerGUID[1] = BeamerS->GetGUID();
                    BeamerS->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                    BeamerS->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    BeamerS->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }
 
                Creature* BeamerP = m_creature->SummonCreature(CREATURE_REDPORTAL,RandomedCoords[2][0],RandomedCoords[2][1],RandomedCoords[2][2],0,TEMPSUMMON_CORPSE_DESPAWN,0);
                if(BeamerP) {
                    BeamerGUID[2] = BeamerP->GetGUID();
                    BeamerP->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                    BeamerP->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    BeamerP->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }
 
                Creature* BeamerDhelp = m_creature->SummonCreature(CREATURE_BLUEPORTAL,RandomedCoords[0][0],RandomedCoords[0][1],RandomedCoords[0][2],0,TEMPSUMMON_CORPSE_DESPAWN,0);
                if(BeamerDhelp) {
                    BeamerhelpGUID[0] = BeamerDhelp->GetGUID();
                    BeamerDhelp->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                    BeamerDhelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    BeamerDhelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }
 
                Creature* BeamerShelp = m_creature->SummonCreature(CREATURE_GREENPORTAL,RandomedCoords[1][0],RandomedCoords[1][1],RandomedCoords[1][2],0,TEMPSUMMON_CORPSE_DESPAWN,0);
                if(BeamerShelp) {
                    BeamerhelpGUID[1] = BeamerShelp->GetGUID();
                    BeamerShelp->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                    BeamerShelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    BeamerShelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }
 
                Creature* BeamerPhelp = m_creature->SummonCreature(CREATURE_REDPORTAL,RandomedCoords[2][0],RandomedCoords[2][1],RandomedCoords[2][2],0,TEMPSUMMON_CORPSE_DESPAWN,0);
                if(BeamerPhelp) {
                    BeamerhelpGUID[2] = BeamerPhelp->GetGUID();
                    BeamerPhelp->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                    BeamerPhelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    BeamerPhelp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }
 
                BeamerD = ((Creature*)Unit::GetUnit((*m_creature), BeamerGUID[0]));
                if(BeamerD)
                    BeamerD->CastSpell(BeamerD,BLUE_PORTAL,true);
 
                BeamerS = ((Creature*)Unit::GetUnit((*m_creature), BeamerGUID[1]));
                if(BeamerS)
                    BeamerS->CastSpell(BeamerS,GREEN_PORTAL,true);
 
                BeamerP = ((Creature*)Unit::GetUnit((*m_creature), BeamerGUID[2]));
                if(BeamerP)
                    BeamerP->CastSpell(BeamerP,RED_PORTAL,true);
 
                PortalsSpawned = true;
                DoTextEmote(SAY_PORTAL, NULL, true);
            }
 
            if(Beam_initialtimer < diff) {
                if(Beam_periodictimer < diff) {
                    //apply beam buff
                    /*BlueNetherBeam();
                    GreenNetherBeam();
                    RedNetherBeam();*/
                                        NetherBeam(0); // blue
                                        NetherBeam(1); // green
                                        NetherBeam(2); // red
                    Beam_periodictimer = 1000;
                    DoScriptText(EMOTE_PHASE_PORTAL, m_creature);
                }else Beam_periodictimer -= diff;
            }else Beam_initialtimer -= diff;
 
            if(Netherburn_timer < diff) {
                DoCast(m_creature,SPELL_NETHERBURN);
                Netherburn_timer = 5000;
            }else Netherburn_timer -= diff;
 
            if(Voidzone_timer < diff && !BanishPhase) {
                //function for spawning the Void Zones around the position of Netherspite
                voidZones_random = rand()%4;
 
                                float fTmp1 = (rand()%2==0) ? -1 : 1;
                                float fTmp2 = (rand()%2==0) ? -1 : 1;
 
                                ZoneCoords[0]= m_creature->GetPositionX() + 5*fTmp1 + rand()%10;
                                ZoneCoords[1]= m_creature->GetPositionY() + 5*fTmp2 + rand()%10;
 
                /*switch(voidZones_random) {
                case 0 :
                    ZoneCoords[0]= m_creature->GetPositionX() +  5 + rand()%10;
                    ZoneCoords[1]= m_creature->GetPositionY() +  5 + rand()%10;
                    break;
 
                case 1:
                    ZoneCoords[0]= m_creature->GetPositionX() +  5 + rand()%10;
                    ZoneCoords[1]= m_creature->GetPositionY() - (5 + rand()%10);
                    break;
 
                case 2:
                    ZoneCoords[0]= m_creature->GetPositionX() - (5 + rand()%10);
                    ZoneCoords[1]= m_creature->GetPositionY() +  5 + rand()%10;
                    break;
 
                case 3:
                    ZoneCoords[0]= m_creature->GetPositionX() - (5 + rand()%10);
                    ZoneCoords[1]= m_creature->GetPositionY() - (5 + rand()%10);
                    break;
                } */
                                
                                Creature* uVoidZone = m_creature->SummonCreature(CREATURE_VOID_ZONE, ZoneCoords[0], ZoneCoords[1], m_creature->GetPositionZ() , 0 , TEMPSUMMON_CORPSE_DESPAWN, 0);
               
               if(uVoidZone)
                    VoidZone[voidZones_counter++] = uVoidZone->GetGUID();
 
               if(voidZones_counter >= 6)
                   voidZones_counter=0;
 
                           // use this, when the spell is fixed (you've to overwork this then):
                /* Unit* Zonetarget = SelectUnit(SELECT_TARGET_RANDOM,0);       // No longer necessary
                if(Zonetarget)
                    ZonetargetGUID = Zonetarget->GetGUID();
 
                    Zonetarget = Unit::GetUnit((*m_creature), ZonetargetGUID);
 
                if(Zonetarget->IsHostileTo(m_creature))
                    DoCast(Zonetarget,SPELL_VOIDZONE); */
 
                    Voidzone_timer = 15000; 
                
            }else Voidzone_timer -= diff;
        }   
 
        if(BanishPhase) {
            if(BanishPhase_timer < diff) {
                m_creature->RemoveAura(SPELL_BANISH_VISUAL,0);
                PortalPhase = true;
                BanishPhase = false;
                PortalsSpawned = false;
                PLRonRedBeam = false;
                BanishPhase_timer = 30000;
                DoResetThreat();
                (*m_creature).GetMotionMaster()->MoveChase(m_creature->getVictim());
                DoScriptText(EMOTE_PHASE_BANISH, m_creature);
            }else BanishPhase_timer -= diff;
 
            if(Netherbreath_timer < diff) {
                DoCast(SelectUnit(SELECT_TARGET_RANDOM,0), SPELL_NETHERBREATH);
                Netherbreath_timer = 2500;
            }else Netherbreath_timer -= diff;
        }
 
        if(!PLRonRedBeam) {
            DoMeleeAttackIfReady();
                } else {
                        Unit* uTmp = Unit::GetUnit(*m_creature,CandidatesGUID[2]);
                        //if(uTmp && uTmp->IsHostileTo(m_creature))
                                //DoStartAttackAndMovement(uTmp); /* after rev. 698: DoStartMovement() */
                        if (m_creature->Attack(uTmp, true))
                        {
                                m_creature->AddThreat(uTmp, 0.0f);
                                m_creature->SetInCombatWith(uTmp);
                                uTmp->SetInCombatWith(m_creature);
 
                                if (!InCombat)
                                {
                                        InCombat = true;
                                        Aggro(uTmp);
                                }
 
                                DoStartMovement(uTmp);
                        }
        }
    }
 
    void KilledUnit(Unit* Victim) {
        uint64 VictimGUID = Victim->GetGUID();
        if(Victim)
            Victim = Unit::GetUnit((*m_creature), VictimGUID);
 
        for(int i=0;i<10;i++) {
            if(Victim == ExhaustCandsDOM[i])
                ExhaustCandsDOM[i] = NULL;
            if(Victim == ExhaustCandsSER[i])
                ExhaustCandsSER[i] = NULL;
            if(Victim == ExhaustCandsPER[i]) {
                ExhaustCandsPER[i] = NULL;
                if(Victim == PERMaxHealth[i]) {
                    PERMaxHealth[i] = NULL;
                }
            }
        }
    }
 
    void JustDied(Unit* Killer) {
        for(int i=0;i<10;i++) {
            ExhaustCandsDOM[i] = NULL;
            ExhaustCandsSER[i] = NULL;
            ExhaustCandsPER[i] = NULL;
            PERMaxHealth[i] = NULL;
        }
 
        DestroyPortals();
                DestroyVoidZone();
                bReset = true;
 
                GameObject* Door = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_GAMEOBJECT_MASSIVE_DOOR));
                if(Door)
                        Door->SetGoState(0); // open the door
    }
};
 
struct MANGOS_DLL_DECL mob_void_zoneAI : public ScriptedAI
{
        mob_void_zoneAI(Creature *c) : ScriptedAI(c) { Reset(); }
 
    uint32 VoidEffect_timer;
 
    void Reset() {
        VoidEffect_timer = 3000;
    }
 
    void Aggro(Unit *who) {}
    void MoveInLineOfSight(Unit *who) {}
 
    void UpdateAI(const uint32 diff) {
        if(VoidEffect_timer < diff) {
            DoCast(m_creature,SPELL_VOIDEFFECT);
            VoidEffect_timer = 3000;
        }else VoidEffect_timer -= diff;
    }
};
 
CreatureAI* GetAI_boss_netherspite(Creature *_Creature) {
    return new boss_netherspiteAI(_Creature);
}
 
CreatureAI* GetAI_mob_void_zone(Creature *_Creature) {
    return new mob_void_zoneAI(_Creature);
}
 
void AddSC_boss_netherspite() {
    Script *newscript;
 
    newscript = new Script;
    newscript->Name="boss_netherspite";
    newscript->GetAI = &GetAI_boss_netherspite;
    newscript->RegisterSelf();
 
    newscript = new Script;
    newscript->Name="mob_void_zone";
    newscript->GetAI = &GetAI_mob_void_zone;
    newscript->RegisterSelf();
}
