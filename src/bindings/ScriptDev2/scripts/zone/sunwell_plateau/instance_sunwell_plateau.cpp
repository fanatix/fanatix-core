/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

/* ScriptData
SDName: Instance_Sunwell_Plateau
SD%Complete: 70%
SDComment: Apply SQL below
SDCategory: Sunwell_Plateau
EndScriptData */

/* SQL
UPDATE `creature_template` SET `ScriptName`='boss_kalecgos' WHERE `entry`=24850;
UPDATE `creature_template` SET `ScriptName`='boss_kalecgos_humanoid' WHERE `entry`=24891;
UPDATE `creature_template` SET `ScriptName`='boss_sathrovarr' WHERE `entry`=24892;
UPDATE `gameobject_template` SET `ScriptName`='go_spectral_rift' WHERE `entry`=187055;
UPDATE `creature_template` SET `ScriptName`='boss_brutallus' WHERE `entry`=24882;
UPDATE `creature_template` SET `ScriptName`='boss_felmyst' WHERE `entry`=25038;
UPDATE `creature_template` SET `ScriptName`='mob_felmyst_vapor' WHERE `entry`=25265;
UPDATE `creature_template` SET `ScriptName`='mob_felmyst_trail' WHERE `entry`=25267;
UPDATE `creature_template` SET `ScriptName`='boss_sacrolash' WHERE `entry`=25165;
UPDATE `creature_template` SET `ScriptName`='boss_alythess' WHERE `entry`=25166;
UPDATE `creature_template` SET `ScriptName`='mob_shadow_image' WHERE `entry`=25214;
UPDATE `creature_template` SET `ScriptName`='boss_muru' WHERE `entry`=25741;
UPDATE `creature_template` SET `ScriptName`='boss_entropius' WHERE `entry`=25840;
UPDATE `creature_template` SET `ScriptName`='dark_fiend' WHERE `entry`=25744;
UPDATE `creature_template` SET `ScriptName` = 'boss_kiljaeden', `minmana` = 1693500, `maxmana` = 1693500 WHERE `entry` = '25315';
UPDATE `creature_template` SET `ScriptName` = 'mob_kiljaeden_controller' WHERE `entry` = '25608';
UPDATE `creature_template` SET `ScriptName` = 'mob_hand_of_the_deceiver' WHERE `entry` = '25588';
UPDATE `creature_template` SET `ScriptName` = 'mob_felfire_portal' WHERE `entry` = '25603';
UPDATE `creature_template` SET `ScriptName` = 'mob_volatile_felfire_fiend' WHERE `entry` = '25598';
UPDATE `creature_template` SET `ScriptName`='mob_shield_orb',`minlevel` = 70, `maxlevel` = 70, `minhealth` = 13986, `maxhealth` = 13986, `faction_A`=14, `faction_H`=14 WHERE entry =25502;
UPDATE `instance_template` SET `script`='instance_sunwell_plateau' WHERE `map`=580;
UPDATE `creature_template` SET `minlevel` = 70, `maxlevel` = 70, `minmana` = 10000, `maxmana` = 10000, `InhabitType`=4 WHERE entry =26046;
DELETE FROM `creature` WHERE map = 580 AND `id` in (25038, 26046);
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES 
(25038, 580, 1, 0, 0, 1459.35, 636.81, 19.9428, 4.93474, 25, 0, 0, 6100000, 4164, 0, 0),
(26046, 580, 1, 0, 0, 1698.45, 628.03, 28.1989, 0.331613, 25, 0, 0, 1, 1, 0, 0);
UPDATE gameobject SET `state`='1' WHERE `map`=580 AND `id` IN (188075,187766,187764,187990,188118,187765);
*/


#include "precompiled.h"
#include "def_sunwell_plateau.h"

#define ENCOUNTERS 8

#define SPELL_SPECTRAL_REALM        46021
#define SPELL_TELEPORT_NORMAL_REALM 46020
#define SPELL_SPECTRAL_EXHAUSTION   44867

/* Sunwell Plateau:
0 - Kalecgos and Sathrovarr
1 - Brutallus
2 - Felmyst
3 - Eredar Twins - Alythess
4 - Eredar Twins - Sacrolash
5 - M'uru
6 - Entropius
7 - Kil'Jaeden
*/

struct MANGOS_DLL_DECL instance_sunwell_plateau : public ScriptedInstance
{
    instance_sunwell_plateau(Map *map) : ScriptedInstance(map) {Initialize();};

    uint32 Encounters[ENCOUNTERS];

    /** Creatures **/
    uint64 Kalecgos_Dragon;
    uint64 Kalecgos_Human;
    uint64 Sathrovarr;
    uint64 Brutallus;
    uint64 Felmyst;
    uint64 Alythess;
    uint64 Sacrolash;
    uint64 Muru;
    uint64 KilJaeden;
    uint64 KilJaedenController;
    uint64 Anveena;

    /** GameObjects **/
    uint64 ForceField;                                      // Kalecgos Encounter
    uint64 DoorFireBarrier;                                 // Brutallus Encounter
    uint64 DoorTheFirstGate;                                // Felmyst Encounter
    uint64 DoorTheSecondGate;                               // Alythess Encounter
    uint64 DoorRaid_Gate_07;                                // Sacrolash Encounter
    uint64 DoorRaid_Gate_08;                                // Muru Encounter
    uint64 DoorTheThirdGate;                                // Entropius Encounter

    /*** Misc ***/
    uint32 SpectralRealmTimer;
    std::vector<uint64> SpectralRealmList;
    std::string str_data;

    void Initialize()
    {
        /*** Creatures ***/
        Kalecgos_Dragon         = 0;
        Kalecgos_Human          = 0;
        Sathrovarr              = 0;
        Brutallus               = 0;
        Felmyst                 = 0;
        Alythess                = 0;
        Sacrolash               = 0;
        Muru                    = 0;
        KilJaeden               = 0;
        KilJaedenController     = 0;
        Anveena                 = 0;

        /*** GameObjects ***/
        ForceField              = 0;
        DoorFireBarrier         = 0;
        DoorTheFirstGate        = 0;
        DoorTheSecondGate       = 0;
        DoorRaid_Gate_07        = 0;
        DoorRaid_Gate_08        = 0;
        DoorTheThirdGate        = 0;

        /*** Encounters ***/
        for(uint8 i = 0; i < ENCOUNTERS; ++i)
            Encounters[i] = NOT_STARTED;

        /*** Misc ***/
        SpectralRealmTimer = 5000;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < ENCOUNTERS; ++i)
            if(Encounters[i] == IN_PROGRESS)
                return true;

        return false;
    }

    void OnCreatureCreate(Creature* creature, uint32 entry)
    {
        switch(creature->GetEntry())
        {
            case 24850: Kalecgos_Dragon     = creature->GetGUID(); break;
            case 24891: Kalecgos_Human      = creature->GetGUID(); break;
            case 24892: Sathrovarr          = creature->GetGUID(); break;
            case 24882: Brutallus           = creature->GetGUID(); break;
            case 25038: Felmyst             = creature->GetGUID(); break;
            case 25166: Alythess            = creature->GetGUID(); break;
            case 25165: Sacrolash           = creature->GetGUID(); break;
            case 25741: Muru                = creature->GetGUID(); break;
            case 25315: KilJaeden           = creature->GetGUID(); break;
            case 25608: KilJaedenController = creature->GetGUID(); break;
            case 26046: Anveena             = creature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject* gobj)
    {
        switch(gobj->GetEntry())
        {
            case 188421: ForceField          = gobj->GetGUID(); break;
            case 188075: DoorFireBarrier     = gobj->GetGUID(); break;
            case 187766: DoorTheFirstGate    = gobj->GetGUID(); break;
            case 187764: DoorTheSecondGate   = gobj->GetGUID(); break;
            case 187990: DoorRaid_Gate_07    = gobj->GetGUID(); break;
            case 188118: DoorRaid_Gate_08    = gobj->GetGUID(); break;
            case 187765: DoorTheThirdGate    = gobj->GetGUID(); break;
        }
    }

    void OpenDoor(uint64 DoorGUID, bool open)
    {
    	Player *player = GetPlayerInMap();

        if (!player)
            return;

        if (GameObject *Door = GameObject::GetGameObject(*player, DoorGUID))
            Door->SetGoState(open ? 0 : 1);
    }

    uint32 GetData(uint32 id)
    {
        switch(id)
        {
            case DATA_KALECGOS_EVENT:     return Encounters[0]; break;
            case DATA_BRUTALLUS_EVENT:    return Encounters[1]; break;
            case DATA_FELMYST_EVENT:      return Encounters[2]; break;
            case DATA_ALYTHESS_EVENT:     return Encounters[3]; break;
            case DATA_SACROLASH_EVENT:    return Encounters[4]; break;
            case DATA_MURU_EVENT:         return Encounters[5]; break;
            case DATA_ENTROPIUS_EVENT:    return Encounters[6]; break;
            case DATA_KILJAEDEN_EVENT:    return Encounters[7]; break;
        }

        return 0;
    }

    uint64 GetData64(uint32 id)
    {
        switch(id)
        {
            case DATA_KALECGOS_DRAGON:      return Kalecgos_Dragon;     break;
            case DATA_KALECGOS_HUMAN:       return Kalecgos_Human;      break;
            case DATA_SATHROVARR:           return Sathrovarr;          break;
            case DATA_BRUTALLUS:            return Brutallus;           break;
            case DATA_FELMYST:              return Felmyst;             break;
            case DATA_ALYTHESS:             return Alythess;            break;
            case DATA_SACROLASH:            return Sacrolash;           break;
            case DATA_MURU:                 return Muru;                break;
            case DATA_KILJAEDEN:            return KilJaeden;           break;
            case DATA_KILJAEDEN_CONTROLLER: return KilJaedenController; break;
            case DATA_ANVEENA:              return Anveena;             break;

            case DATA_RANDOM_SPECTRAL_PLAYER:
                return *(SpectralRealmList.begin() + rand()%SpectralRealmList.size());
                break;
        }
        return 0;
    }

    void SetData(uint32 id, uint32 data)
    {
        switch(id)
        {
            case DATA_KALECGOS_EVENT:      Encounters[0] = data; break;
            case DATA_BRUTALLUS_EVENT:
                Encounters[1] = data;
                OpenDoor(DoorFireBarrier, data == DONE);
                break;
            case DATA_FELMYST_EVENT:
                Encounters[2] = data;
                OpenDoor(DoorTheFirstGate, data == DONE);
                break;
            case DATA_ALYTHESS_EVENT:
                Encounters[3] = data;
                OpenDoor(DoorTheSecondGate, data == DONE);
                break;
            case DATA_SACROLASH_EVENT:
                Encounters[4] = data;
                OpenDoor(DoorRaid_Gate_07, data == DONE);
                break;
            case DATA_MURU_EVENT:
                Encounters[5] = data;
                OpenDoor(DoorRaid_Gate_08, data == DONE);
                break;
            case DATA_ENTROPIUS_EVENT:
                Encounters[6] = data;
                OpenDoor(DoorTheThirdGate, data == DONE);
                break;
            case DATA_KILJAEDEN_EVENT:     Encounters[7] = data; break;

            case DATA_SET_SPECTRAL_CHECK:  SpectralRealmTimer = data; break;
            case DATA_INST_EJECT_PLAYERS:  EjectPlayers(); break;
        }
        if (data == DONE)
        {
            SaveToDB();
        }
    }

    void SetData64(uint32 id, uint64 guid)
    {
        switch(id)
        {
            case DATA_PLAYER_SPECTRAL_REALM:
                SpectralRealmList.push_back(guid);
                break;
        }
    }

    Player* GetPlayerInMap()
    {
        Map::PlayerList const& players = instance->GetPlayers();

        if (!players.isEmpty())
        {
            for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* plr = itr->getSource())
                    return plr;
            }
        }

        return NULL;
    }

    void EjectPlayer(Player* plr)
    {
        debug_log("SD2: INST: Ejecting Player %s from Spectral Realm", plr->GetName());
        // Remove player from Sathrovarr's threat list
        Creature* Sath = ((Creature*)Unit::GetUnit(*plr, Sathrovarr));
        if (Sath && Sath->isAlive())
        {
            HostilReference* ref = Sath->getThreatManager().getOnlineContainer().getReferenceByTarget(plr);
            if (ref)
            {
                ref->removeReference();
                debug_log("SD2: INST: Deleting %s from Sathrovarr's threatlist", plr->GetName());
            }
        }

        // Put player back in Kalecgos(Dragon)'s threat list
        Creature* Kalecgos = ((Creature*)Unit::GetUnit(*plr, Kalecgos_Dragon));
        if (Kalecgos && Kalecgos->isAlive())
        {
            debug_log("SD2: INST: Putting %s in Kalecgos' threatlist", plr->GetName());
            Kalecgos->AddThreat(plr, 1.0f);
        }

        plr->CastSpell(plr, SPELL_TELEPORT_NORMAL_REALM, true);
        plr->CastSpell(plr, SPELL_SPECTRAL_EXHAUSTION, true);
    }

    void EjectPlayers()
    {
        if (SpectralRealmList.empty())
            return;

        Map::PlayerList const& players = instance->GetPlayers();
        for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            Player* plr = itr->getSource();
            if (!plr)
                continue;

            if (std::find(SpectralRealmList.begin(),SpectralRealmList.end(),plr->GetGUID())!=SpectralRealmList.end() &&
                !plr->HasAura(SPELL_SPECTRAL_REALM, 0))
            {
                EjectPlayer(plr);
            }
        }

        SpectralRealmList.clear();
    }

    void Update(uint32 diff)
    {
        // Only check for Spectral Realm if Kalecgos Encounter is running
        if(Encounters[0] == IN_PROGRESS)
            if(SpectralRealmTimer < diff)
        {
            EjectPlayers();
            SpectralRealmTimer = 5000;
        }else SpectralRealmTimer -= diff;
    }

   const char* Save()
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << Encounters[0] << " " << Encounters[1] << " "
            << Encounters[2] << " " << Encounters[3] << " "
            << Encounters[4] << " " << Encounters[5] << " "
            << Encounters[6] << " " << Encounters[7];

        str_data = saveStream.str();

        OUT_SAVE_INST_DATA_COMPLETE;
        return str_data.c_str();
    }

    void Load(const char* in)
    {
        if (!in)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(in);

        std::istringstream loadStream(in);
        loadStream >> Encounters[0] >> Encounters[1] >> Encounters[2] >> Encounters[3] >> Encounters[4] >> Encounters[5] >> Encounters[6] >> Encounters[7];

        for(uint8 i = 0; i < ENCOUNTERS; ++i)
        {
            if (Encounters[i] == IN_PROGRESS)
                Encounters[i] = NOT_STARTED;

            if (i == 1)
                OpenDoor(DoorFireBarrier, Encounters[i] == DONE);
            if (i == 2)
                OpenDoor(DoorTheFirstGate, Encounters[i] == DONE);
            if (i == 3)
                OpenDoor(DoorTheSecondGate, Encounters[i] == DONE);
            if (i == 4)
                OpenDoor(DoorRaid_Gate_07, Encounters[i] == DONE);
            if (i == 5)
                OpenDoor(DoorRaid_Gate_08, Encounters[i] == DONE);
            if (i == 6)
                OpenDoor(DoorTheThirdGate, Encounters[i] == DONE);
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }

};

InstanceData* GetInstanceData_instance_sunwell_plateau(Map* map)
{
    return new instance_sunwell_plateau(map);
}

void AddSC_instance_sunwell_plateau()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_sunwell_plateau";
    newscript->GetInstanceData = &GetInstanceData_instance_sunwell_plateau;
    newscript->RegisterSelf();
}
