/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#include "precompiled.h"
#include "../../shared/Config/Config.h"
#include "config.h"
#include "Database/DatabaseEnv.h"
#include "DBCStores.h"
#include "ObjectMgr.h"
#include "ProgressBar.h"

int num_sc_scripts;
Script *m_scripts[MAX_SCRIPTS];

DatabaseType SD2Database;
Config SD2Config;

// String text additional data, used in TextMap
struct StringTextData
{
    uint32 SoundId;
    uint8  Type;
    uint32 Language;
    uint32 Emote;
};

#define TEXT_SOURCE_RANGE   -1000000                        //the amount of entries each text source has available

// Text Maps
UNORDERED_MAP<int32, StringTextData> TextMap;

// Waypoint map (escorts)
UNORDERED_MAP<uint32, std::vector<PointMovement> > PointMovementMap;

void FillSpellSummary();

// -- Scripts to be added --

// -- Areatrigger --
extern void AddSC_areatrigger_scripts();

// -- Boss --
extern void AddSC_boss_emeriss();
extern void AddSC_boss_taerar();
extern void AddSC_boss_ysondre();

// -- Creature --
extern void AddSC_generic_creature();

// -- Custom --
extern void AddSC_npc_acherus_taxi();
extern void AddSC_npc_wyrmresttempel_taxi();
extern void AddSC_telefratz();
extern void AddSC_npc_paymaster();
extern void AddSC_npc_sickness();
//extern void AddSC_paradox();

// -- Examples --
extern void AddSC_example_creature();
extern void AddSC_example_escort();
extern void AddSC_example_gossip_codebox();
extern void AddSC_example_misc();

// -- GO --
extern void AddSC_go_scripts();

// -- Guard --
extern void AddSC_guards();

// -- Honor --

// -- Item --
extern void AddSC_item_scripts();

// -- NPC --
extern void AddSC_npc_professions();
extern void AddSC_npcs_special();

// -- Servers --

//--------------------
//------ ZONE --------

//Alterac Mountains
extern void AddSC_alterac_mountains();

//Arathi Highlands
extern void AddSC_arathi_highlands();

//Ashenvale Forest
extern void AddSC_ashenvale();

//Aunchindoun
//--Auchenai Crypts
extern void AddSC_boss_exarch_maladaar();
extern void AddSC_boss_shirrak_the_dead_watcher();

//--Mana Tombs
extern void AddSC_boss_nexusprince_shaffar();
extern void AddSC_boss_pandemonius();

//--Sekketh Halls
extern void AddSC_boss_darkweaver_syth();
extern void AddSC_boss_talon_king_ikiss();
extern void AddSC_instance_sethekk_halls();

//--Shadow Labyrinth
extern void AddSC_boss_Terrordar_der_Peiniger();
extern void AddSC_boss_blackheart_the_inciter();
extern void AddSC_boss_grandmaster_vorpil();
extern void AddSC_boss_murmur();
extern void AddSC_instance_shadow_labyrinth();

//Azshara
extern void AddSC_boss_azuregos();
extern void AddSC_azshara();

//Azuremyst Isle
extern void AddSC_azuremyst_isle();

//Badlands
//Barrens
extern void AddSC_the_barrens();

//Black Temple
extern void AddSC_black_temple();
extern void AddSC_boss_illidan();
extern void AddSC_boss_shade_of_akama();
extern void AddSC_boss_supremus();
extern void AddSC_boss_gurtogg_bloodboil();
extern void AddSC_boss_mother_shahraz();
extern void AddSC_boss_reliquary_of_souls();
extern void AddSC_boss_teron_gorefiend();
extern void AddSC_boss_najentus();
extern void AddSC_boss_illidari_council();
extern void AddSC_instance_black_temple();

//Blackfathom Depths
//Blackrock Depths
extern void AddSC_blackrock_depths();
extern void AddSC_boss_ambassador_flamelash();
extern void AddSC_boss_anubshiah();
extern void AddSC_boss_draganthaurissan();
extern void AddSC_boss_general_angerforge();
extern void AddSC_boss_gorosh_the_dervish();
extern void AddSC_boss_grizzle();
extern void AddSC_boss_high_interrogator_gerstahn();
extern void AddSC_boss_magmus();
extern void AddSC_boss_moira_bronzebeard();
extern void AddSC_boss_tomb_of_seven();
extern void AddSC_instance_blackrock_depths();

//Blackrock Spire
extern void AddSC_boss_drakkisath();
extern void AddSC_boss_halycon();
extern void AddSC_boss_highlordomokk();
extern void AddSC_boss_mothersmolderweb();
extern void AddSC_boss_overlordwyrmthalak();
extern void AddSC_boss_shadowvosh();
extern void AddSC_boss_thebeast();
extern void AddSC_boss_warmastervoone();
extern void AddSC_boss_quatermasterzigris();
extern void AddSC_boss_pyroguard_emberseer();
extern void AddSC_boss_gyth();
extern void AddSC_boss_rend_blackhand();

//Blackwing lair
extern void AddSC_boss_razorgore();
extern void AddSC_boss_vael();
extern void AddSC_boss_broodlord();
extern void AddSC_boss_firemaw();
extern void AddSC_boss_ebonroc();
extern void AddSC_boss_flamegor();
extern void AddSC_boss_chromaggus();
extern void AddSC_boss_nefarian();
extern void AddSC_boss_victor_nefarius();

//Blade's Edge Mountains
extern void AddSC_blades_edge_mountains();

//Blasted lands
extern void AddSC_boss_kruul();
extern void AddSC_blasted_lands();

//Bloodmyst Isle
extern void AddSC_bloodmyst_isle();

//Borean Tundra
extern void AddSC_borean_tundra();

//Burning steppes
extern void AddSC_burning_steppes();

//Caverns of Time
//--Battle for Mt. Hyjal
extern void AddSC_hyjal();
extern void AddSC_boss_anetheron();
extern void AddSC_boss_archimonde();
extern void AddSC_boss_azgalor();
extern void AddSC_boss_kazrogal();
extern void AddSC_boss_rage_winterchill();
extern void AddSC_instance_mount_hyjal();

//--Culling of Stratholme
extern void AddSC_boss_epoch();
extern void AddSC_boss_mal_ganis();
extern void AddSC_boss_meathook();
extern void AddSC_boss_salramm();
extern void AddSC_instance_culling_of_stratholme();

//--Old Hillsbrad
extern void AddSC_boss_captain_skarloc();
extern void AddSC_boss_epoch_hunter();
extern void AddSC_boss_lieutenant_drake();
extern void AddSC_instance_old_hillsbrad();
extern void AddSC_old_hillsbrad();

//--The Dark Portal
extern void AddSC_boss_aeonus();
extern void AddSC_boss_chrono_lord_deja();
extern void AddSC_boss_temporus();
extern void AddSC_dark_portal();
extern void AddSC_instance_dark_portal();

//Coilfang Resevoir
//--Serpent Shrine Cavern
extern void AddSC_boss_fathomlord_karathress();
extern void AddSC_boss_hydross_the_unstable();
extern void AddSC_boss_lady_vashj();
extern void AddSC_boss_leotheras_the_blind();
extern void AddSC_boss_morogrim_tidewalker();
extern void AddSC_instance_serpentshrine_cavern();
extern void AddSC_mob_underbog_colossus();
extern void AddSC_boss_the_lurker_below();

//--Slave Pens

//--Steam Vault
extern void AddSC_boss_hydromancer_thespia();
extern void AddSC_boss_mekgineer_steamrigger();
extern void AddSC_boss_warlord_kalithresh();
extern void AddSC_instance_steam_vault();

//--Underbog
extern void AddSC_boss_hungarfen();

//Darkshore
//Darnassus
//Deadmines
extern void AddSC_deadmines();
extern void AddSC_instance_deadmines();

//Deadwind pass
//Desolace
//Dire Maul
//Dragonblight
extern void AddSC_dragonblight();

//Dun Morogh
extern void AddSC_dun_morogh();

//Durotar
//Duskwood
extern void AddSC_duskwood();
//Dustwallow marsh
extern void AddSC_dustwallow_marsh();

//Eversong Woods
extern void AddSC_eversong_woods();

//Exodar
//Eastern Plaguelands
extern void AddSC_eastern_plaguelands();

//Elwynn Forest
extern void AddSC_elwynn_forest();

//Felwood
extern void AddSC_felwood();

//Feralas
extern void AddSC_feralas();

//Ghostlands
extern void AddSC_ghostlands();

//Gnomeregan
//Gruul's Lair
extern void AddSC_boss_gruul();
extern void AddSC_boss_high_king_maulgar();
extern void AddSC_instance_gruuls_lair();

//Hellfire Citadel
//--Blood Furnace
extern void AddSC_boss_broggok();
extern void AddSC_boss_the_maker();

//--Magtheridon's Lair
extern void AddSC_boss_magtheridon();
extern void AddSC_instance_magtheridons_lair();

//--Shattered Halls
extern void AddSC_boss_grand_warlock_nethekurse();
extern void AddSC_boss_warbringer_omrogg();
extern void AddSC_boss_warchief_kargath_bladefist();
extern void AddSC_instance_shattered_halls();

//--Ramparts
extern void AddSC_boss_watchkeeper_gargolmar();
extern void AddSC_boss_omor_the_unscarred();
extern void AddSC_instance_ramparts();

//Hellfire Peninsula
extern void AddSC_boss_doomlordkazzak();
extern void AddSC_hellfire_peninsula();

//Hillsbrad Foothills
//Hinterlands
//Ironforge
extern void AddSC_ironforge();

//Isle of Quel'Danas
extern void AddSC_isle_of_queldanas();

//Karazhan
extern void AddSC_boss_attumen();
extern void AddSC_boss_curator();
extern void AddSC_boss_maiden_of_virtue();
extern void AddSC_boss_shade_of_aran();
extern void AddSC_boss_malchezaar();
extern void AddSC_boss_terestian_illhoof();
extern void AddSC_boss_nightbane();
extern void AddSC_boss_netherspite();
extern void AddSC_netherspite_infernal();
extern void AddSC_boss_moroes();
extern void AddSC_bosses_opera();
extern void AddSC_instance_karazhan();
extern void AddSC_karazhan();

//Loch Modan
extern void AddSC_loch_modan();

//Lower Blackrock Spire

// Magister's Terrace
extern void AddSC_boss_felblood_kaelthas();
extern void AddSC_boss_selin_fireheart();
extern void AddSC_boss_vexallus();
extern void AddSC_boss_priestess_delrissa();
extern void AddSC_instance_magisters_terrace();

//Maraudon
extern void AddSC_boss_celebras_the_cursed();
extern void AddSC_boss_landslide();
extern void AddSC_boss_noxxion();
extern void AddSC_boss_ptheradras();

//Molten core
extern void AddSC_boss_lucifron();
extern void AddSC_boss_magmadar();
extern void AddSC_boss_gehennas();
extern void AddSC_boss_garr();
extern void AddSC_boss_baron_geddon();
extern void AddSC_boss_shazzrah();
extern void AddSC_boss_golemagg();
extern void AddSC_boss_sulfuron();
extern void AddSC_boss_majordomo();
extern void AddSC_boss_ragnaros();
extern void AddSC_instance_molten_core();
extern void AddSC_molten_core();

//Moonglade
extern void AddSC_moonglade();

//Mulgore
extern void AddSC_mulgore();

//Nagrand
extern void AddSC_nagrand();

//Naxxramas
extern void AddSC_boss_anubrekhan();
extern void AddSC_boss_faerlina();
extern void AddSC_boss_fourhorsemen();
extern void AddSC_boss_gluth();
extern void AddSC_boss_gothik();
extern void AddSC_boss_grobbulus();
extern void AddSC_boss_heigan();
extern void AddSC_boss_kelthuzad();
extern void AddSC_boss_loatheb();
extern void AddSC_boss_maexxna();
extern void AddSC_boss_noth();
extern void AddSC_boss_patchwerk();
extern void AddSC_boss_razuvious();
extern void AddSC_boss_sapphiron();
extern void AddSC_boss_thaddius();
extern void AddSC_instance_naxxramas();
extern void AddSC_npc_naxxramas_controler();

//Netherstorm
extern void AddSC_netherstorm();

//Onyxia's Lair
extern void AddSC_boss_onyxia();

//Orgrimmar
extern void AddSC_orgrimmar();

//Ragefire Chasm
//Razorfen Downs
extern void AddSC_razorfen_downs();
extern void AddSC_boss_amnennar_the_coldbringer();

//Razorfen Kraul
extern void AddSC_razorfen_kraul();

//Redridge Mountains
//Ruins of Ahn'Qiraj
//Scarlet Monastery
extern void AddSC_boss_arcanist_doan();
extern void AddSC_boss_azshir_the_sleepless();
extern void AddSC_boss_bloodmage_thalnos();
extern void AddSC_boss_headless_horseman();
extern void AddSC_boss_herod();
extern void AddSC_boss_high_inquisitor_fairbanks();
extern void AddSC_boss_houndmaster_loksey();
extern void AddSC_boss_interrogator_vishas();
extern void AddSC_boss_mograine_and_whitemane();
extern void AddSC_boss_scorn();
extern void AddSC_instance_scarlet_monastery();

//Scholomance
extern void AddSC_boss_darkmaster_gandling();
extern void AddSC_boss_death_knight_darkreaver();
extern void AddSC_boss_theolenkrastinov();
extern void AddSC_boss_illuciabarov();
extern void AddSC_boss_instructormalicia();
extern void AddSC_boss_jandicebarov();
extern void AddSC_boss_kormok();
extern void AddSC_boss_lordalexeibarov();
extern void AddSC_boss_lorekeeperpolkelt();
extern void AddSC_boss_rasfrost();
extern void AddSC_boss_theravenian();
extern void AddSC_boss_vectus();
extern void AddSC_instance_scholomance();

//Searing gorge
extern void AddSC_searing_gorge();

//Shadowfang keep
extern void AddSC_shadowfang_keep();
extern void AddSC_instance_shadowfang_keep();

//Shadowmoon Valley
extern void AddSC_boss_doomwalker();
extern void AddSC_shadowmoon_valley();

//Shattrath
extern void AddSC_shattrath_city();

//Silithus
extern void AddSC_silithus();

//Silvermoon
extern void AddSC_silvermoon_city();

//Silverpine forest
extern void AddSC_silverpine_forest();

//Stockade
//Stonetalon mountains
extern void AddSC_stonetalon_mountains();

//Stormwind City
extern void AddSC_stormwind_city();

//Stranglethorn Vale
extern void AddSC_stranglethorn_vale();

//Stratholme
extern void AddSC_boss_magistrate_barthilas();
extern void AddSC_boss_maleki_the_pallid();
extern void AddSC_boss_nerubenkan();
extern void AddSC_boss_cannon_master_willey();
extern void AddSC_boss_baroness_anastari();
extern void AddSC_boss_ramstein_the_gorger();
extern void AddSC_boss_timmy_the_cruel();
extern void AddSC_boss_postmaster_malown();
extern void AddSC_boss_baron_rivendare();
extern void AddSC_boss_dathrohan_balnazzar();
extern void AddSC_boss_order_of_silver_hand();
extern void AddSC_instance_stratholme();
extern void AddSC_stratholme();

//Sunken Temple
//Sunwell Plateau
extern void AddSC_instance_sunwell_plateau();
extern void AddSC_boss_kalecgos();
extern void AddSC_boss_brutallus();
extern void AddSC_boss_felmyst();
extern void AddSC_boss_eredar_twins();
extern void AddSC_boss_muru_entropius();
//extern void AddSC_boss_kiljaeden();
//Swamp of Sorrows
//Tanaris
extern void AddSC_tanaris();

//Teldrassil
extern void AddSC_teldrassil();

//Tempest Keep
//--Arcatraz
extern void AddSC_arcatraz();
extern void AddSC_boss_harbinger_skyriss();
extern void AddSC_instance_arcatraz();

//--Botanica
extern void AddSC_boss_high_botanist_freywinn();
extern void AddSC_boss_laj();
extern void AddSC_boss_warp_splinter();

//--The Eye
extern void AddSC_boss_kaelthas();
extern void AddSC_boss_void_reaver();
extern void AddSC_boss_high_astromancer_solarian();
extern void AddSC_boss_alar();
extern void AddSC_instance_the_eye();
extern void AddSC_the_eye();

//--The Mechanar
extern void AddSC_boss_gatewatcher_iron_hand();
extern void AddSC_boss_nethermancer_sepethrea();
extern void AddSC_boss_pathaleon_the_calculator();
extern void AddSC_instance_mechanar();

//Temple of ahn'qiraj
extern void AddSC_boss_cthun();
extern void AddSC_boss_fankriss();
extern void AddSC_boss_huhuran();
extern void AddSC_bug_trio();
extern void AddSC_boss_sartura();
extern void AddSC_boss_skeram();
extern void AddSC_boss_twinemperors();
extern void AddSC_mob_anubisath_sentinel();
extern void AddSC_instance_temple_of_ahnqiraj();

//Terokkar Forest
extern void AddSC_terokkar_forest();

//Thousand Needles
//Thunder Bluff
extern void AddSC_thunder_bluff();

//Tirisfal Glades
extern void AddSC_tirisfal_glades();

//Uldaman
extern void AddSC_boss_ironaya();
extern void AddSC_uldaman();

//Undercity
extern void AddSC_undercity();

//Un'Goro Crater
//Upper blackrock spire
//Wailing caverns

//Western plaguelands
extern void AddSC_western_plaguelands();

//Westfall
extern void AddSC_westfall();

//Wetlands
//Winterspring
extern void AddSC_winterspring();

//Zangarmarsh
extern void AddSC_zangarmarsh();

//Zul'Farrak
extern void AddSC_zulfarrak();

//Zul'Gurub
extern void AddSC_boss_arlokk();
extern void AddSC_boss_gahzranka();
extern void AddSC_boss_grilek();
extern void AddSC_boss_hakkar();
extern void AddSC_boss_hazzarah();
extern void AddSC_boss_jeklik();
extern void AddSC_boss_jindo();
extern void AddSC_boss_mandokir();
extern void AddSC_boss_marli();
extern void AddSC_boss_ouro();
extern void AddSC_boss_renataki();
extern void AddSC_boss_thekal();
extern void AddSC_boss_venoxis();
extern void AddSC_boss_wushoolay();
extern void AddSC_instance_zulgurub();

//Zul'Aman
extern void AddSC_instance_zulaman();
extern void AddSC_zulaman();
extern void AddSC_boss_nalorakk();
extern void AddSC_boss_akilzon();
extern void AddSC_boss_janalai();
extern void AddSC_boss_halazzi();
extern void AddSC_boss_hex_lord_malacrass();
extern void AddSC_boss_zuljin();
extern void AddSC_zulaman();

//Utgarde: Utgarde Keep
extern void AddSC_boss_ingvar_the_plunderer();
extern void AddSC_boss_keleseth();
extern void AddSC_boss_skarvald_dalronn();
extern void AddSC_instance_utgarde_keep();

//Utgarde: Utgarde Pinnacle
extern void AddSC_instance_pinnacle();
extern void AddSC_boss_skadi();
extern void AddSC_boss_ymiron();
extern void AddSC_boss_svala();
extern void AddSC_boss_palehoof();

//Nexus: The Nexus
extern void AddSC_instance_nexus();
extern void AddSC_boss_magus_telestra();
extern void AddSC_boss_anomalus();
extern void AddSC_boss_ormorok();
extern void AddSC_boss_keristrasza();
//Nexus: The Oculus
extern void AddSC_instance_oculus();
extern void AddSC_boss_drakos();
extern void AddSC_boss_urom();
extern void AddSC_boss_varos();
extern void AddSC_boss_eregos();
//Azjol'Nerub: Azjol'Nerub
extern void AddSC_instance_azjol_nerub();
extern void AddSC_boss_krik_thir();
extern void AddSC_boss_hadronox();
extern void AddSC_boss_anub_arak();
//Azjol'Nerub: Ahn'kahet
extern void AddSC_instance_ahn_kahet();
extern void AddSC_boss_taldaram();
extern void AddSC_boss_nadox();
extern void AddSC_boss_jedoga();
extern void AddSC_boss_volazj();
// Drak Tharon
extern void AddSC_instance_drak_tharon();
extern void AddSC_boss_trollgore();
extern void AddSC_boss_novos();
extern void AddSC_boss_dred();
extern void AddSC_boss_tharon_ja();
//The Violet Hold
extern void AddSC_instance_violet_hold();
extern void AddSC_boss_cyanigosa();
extern void AddSC_boss_erekem();
extern void AddSC_boss_ichoron();
extern void AddSC_boss_lavanthor();
extern void AddSC_boss_moragg();
extern void AddSC_boss_xevozz();
extern void AddSC_boss_zuramat();
//Gundrak
extern void AddSC_instance_gundrak();
extern void AddSC_boss_slad_ran();
extern void AddSC_boss_moorabi();
extern void AddSC_boss_drakkari_colossus();
extern void AddSC_boss_gal_darah();
extern void AddSC_boss_eck();
//Ulduar: Halls of stone
extern void AddSC_instance_halls_of_stone();
extern void AddSC_boss_maiden_of_grief();
extern void AddSC_boss_krystallus();
extern void AddSC_boss_sjonnir();
//Ulduar: Halls of Lightning
extern void AddSC_instance_halls_of_lightning();
extern void AddSC_boss_bjarngrim();
extern void AddSC_boss_ionar();
extern void AddSC_boss_volkhan();
extern void AddSC_boss_loken();
//Caverns of Time: The Culling of Stratholme
extern void AddSC_instance_culling_of_stratholme();
extern void AddSC_boss_meathook();
extern void AddSC_boss_epoch();
extern void AddSC_boss_mal_ganis();
extern void AddSC_boss_salramm();
//Nexus: The Eye of Eternity
extern void AddSC_instance_eye_of_eternity();
extern void AddSC_boss_malygos();
//Obsidian Sanctum
extern void AddSC_instance_obsidian_sanctum();
extern void AddSC_boss_sartharion();
//Vault of Archavon
extern void AddSC_instance_archavon();
extern void AddSC_boss_archavon();
// -------------------
void LoadDatabase()
{
    //Get db string from file
    char const* sd2dbstring = NULL;

    if (!SD2Config.GetString("ScriptDev2DatabaseInfo", &sd2dbstring))
    {
        error_log("SD2: Missing Scriptdev2 database info from configuration file. Load database aborted.");
        return;
    }

    //Initialize connection to DB
    if (sd2dbstring && SD2Database.Initialize(sd2dbstring))
        outstring_log("SD2: ScriptDev2 database: %s",sd2dbstring);
    else
    {
        error_log("SD2: Unable to connect to Database. Load database aborted.");
        return;
    }

    //***Preform all DB queries here***
    QueryResult *result;

    //Get Version information
    result = SD2Database.PQuery("SELECT version FROM sd2_db_version");

    if (result)
    {
        Field *fields = result->Fetch();
        outstring_log("SD2: Database version is: %s", fields[0].GetString());
        outstring_log("");
        delete result;

    }else
    {
        error_log("SD2: Missing `sd2_db_version` information.");
        outstring_log("");
    }

    // Load Script Text
    outstring_log("SD2: Loading Script Texts...");
    LoadMangosStrings(SD2Database,"script_texts",TEXT_SOURCE_RANGE,1+(TEXT_SOURCE_RANGE*2));

    // Gather Additional data from Script Texts
    result = SD2Database.PQuery("SELECT entry, sound, type, language, emote FROM script_texts");

    outstring_log("SD2: Loading Script Texts additional data...");
    if (result)
    {
        barGoLink bar(result->GetRowCount());
        uint32 count = 0;

        do
        {
            bar.step();
            Field* fields = result->Fetch();
            StringTextData temp;

            int32 i             = fields[0].GetInt32();
            temp.SoundId        = fields[1].GetInt32();
            temp.Type           = fields[2].GetInt32();
            temp.Language       = fields[3].GetInt32();
            temp.Emote          = fields[4].GetInt32();

            if (i >= 0)
            {
                error_db_log("SD2: Entry %i in table `script_texts` is not a negative value.",i);
                continue;
            }

            if (i > TEXT_SOURCE_RANGE || i <= TEXT_SOURCE_RANGE*2)
            {
                error_db_log("SD2: Entry %i in table `script_texts` is out of accepted entry range for table.",i);
                continue;
            }

            if (temp.SoundId)
            {
                if (!GetSoundEntriesStore()->LookupEntry(temp.SoundId))
                    error_db_log("SD2: Entry %i in table `script_texts` has soundId %u but sound does not exist.",i,temp.SoundId);
            }

            if (!GetLanguageDescByID(temp.Language))
                error_db_log("SD2: Entry %i in table `script_texts` using Language %u but Language does not exist.",i,temp.Language);

            if (temp.Type > CHAT_TYPE_ZONE_YELL)
                error_db_log("SD2: Entry %i in table `script_texts` has Type %u but this Chat Type does not exist.",i,temp.Type);

            TextMap[i] = temp;
            ++count;
        } while (result->NextRow());

        delete result;

        outstring_log("");
        outstring_log(">> Loaded %u additional Script Texts data.", count);
    }else
    {
        barGoLink bar(1);
        bar.step();
        outstring_log("");
        outstring_log(">> Loaded 0 additional Script Texts data. DB table `script_texts` is empty.");
    }

    // Load Custom Text
    outstring_log("SD2: Loading Custom Texts...");
    LoadMangosStrings(SD2Database,"custom_texts",TEXT_SOURCE_RANGE*2,1+(TEXT_SOURCE_RANGE*3));

    // Gather Additional data from Custom Texts
    result = SD2Database.PQuery("SELECT entry, sound, type, language, emote FROM custom_texts");

    outstring_log("SD2: Loading Custom Texts additional data...");
    if (result)
    {
        barGoLink bar(result->GetRowCount());
        uint32 count = 0;

        do
        {
            bar.step();
            Field* fields = result->Fetch();
            StringTextData temp;

            int32 i             = fields[0].GetInt32();
            temp.SoundId        = fields[1].GetInt32();
            temp.Type           = fields[2].GetInt32();
            temp.Language       = fields[3].GetInt32();
            temp.Emote          = fields[4].GetInt32();

            if (i >= 0)
            {
                error_db_log("SD2: Entry %i in table `custom_texts` is not a negative value.",i);
                continue;
            }

            if (i > TEXT_SOURCE_RANGE*2 || i <= TEXT_SOURCE_RANGE*3)
            {
                error_db_log("SD2: Entry %i in table `custom_texts` is out of accepted entry range for table.",i);
                continue;
            }

            if (temp.SoundId)
            {
                if (!GetSoundEntriesStore()->LookupEntry(temp.SoundId))
                    error_db_log("SD2: Entry %i in table `custom_texts` has soundId %u but sound does not exist.",i,temp.SoundId);
            }

            if (!GetLanguageDescByID(temp.Language))
                error_db_log("SD2: Entry %i in table `custom_texts` using Language %u but Language does not exist.",i,temp.Language);

            if (temp.Type > CHAT_TYPE_ZONE_YELL)
                error_db_log("SD2: Entry %i in table `custom_texts` has Type %u but this Chat Type does not exist.",i,temp.Type);

            TextMap[i] = temp;
            ++count;
        } while (result->NextRow());

        delete result;

        outstring_log("");
        outstring_log(">> Loaded %u additional Custom Texts data.", count);
    }else
    {
        barGoLink bar(1);
        bar.step();
        outstring_log("");
        outstring_log(">> Loaded 0 additional Custom Texts data. DB table `custom_texts` is empty.");
    }

    // Drop Existing Waypoint list
    PointMovementMap.clear();
    uint64 uiCreatureCount = 0;

    // Load Waypoints
    result = SD2Database.PQuery("SELECT COUNT(entry) FROM script_waypoint GROUP BY entry");
    if (result)
    {
        uiCreatureCount = result->GetRowCount();
        delete result;
    }

    outstring_log("SD2: Loading Script Waypoints for %u creature(s)...", uiCreatureCount);

    result = SD2Database.PQuery("SELECT entry, pointid, location_x, location_y, location_z, waittime FROM script_waypoint ORDER BY pointid");

    if (result)
    {
        barGoLink bar(result->GetRowCount());
        uint32 uiNodeCount = 0;

        do
        {
            bar.step();
            Field* pFields = result->Fetch();
            PointMovement pTemp;

            pTemp.m_uiCreatureEntry  = pFields[0].GetUInt32();
            uint32 uiCreatureEntry   = pTemp.m_uiCreatureEntry;
            pTemp.m_uiPointId        = pFields[1].GetUInt32();
            pTemp.m_fX               = pFields[2].GetFloat();
            pTemp.m_fY               = pFields[3].GetFloat();
            pTemp.m_fZ               = pFields[4].GetFloat();
            pTemp.m_uiWaitTime       = pFields[5].GetUInt32();

            CreatureInfo const* pCInfo = GetCreatureTemplateStore(pTemp.m_uiCreatureEntry);
            if (!pCInfo)
            {
                error_db_log("SD2: DB table script_waypoint has waypoint for non-existant creature entry %u", pTemp.m_uiCreatureEntry);
                continue;
            }

            if (!pCInfo->ScriptID)
                error_db_log("SD2: DB table script_waypoint has waypoint for creature entry %u, but creature does not have ScriptName defined and then useless.", pTemp.m_uiCreatureEntry);

            PointMovementMap[uiCreatureEntry].push_back(pTemp);
            ++uiNodeCount;
        } while (result->NextRow());

        delete result;

        outstring_log("");
        outstring_log(">> Loaded %u Script Waypoint nodes.", uiNodeCount);
    }
    else
    {
        barGoLink bar(1);
        bar.step();
        outstring_log("");
        outstring_log(">> Loaded 0 Script Waypoints. DB table `script_waypoint` is empty.");
    }

    //Free database thread and resources
    SD2Database.HaltDelayThread();

}

struct TSpellSummary {
    uint8 Targets;                                          // set of enum SelectTarget
    uint8 Effects;                                          // set of enum SelectEffect
}extern *SpellSummary;

MANGOS_DLL_EXPORT
void ScriptsFree()
{
    // Free Spell Summary
    delete []SpellSummary;

    // Free resources before library unload
    for(int i=0;i<MAX_SCRIPTS;i++)
        delete m_scripts[i];

    num_sc_scripts = 0;
}

MANGOS_DLL_EXPORT
void ScriptsInit()
{
    bool CanLoadDB = true;

    //ScriptDev2 startup
    outstring_log("");
    outstring_log(" MMM  MMM    MM");
    outstring_log("M  MM M  M  M  M");
    outstring_log("MM    M   M   M");
    outstring_log(" MMM  M   M  M");
    outstring_log("   MM M   M MMMM");
    outstring_log("MM  M M  M ");
    outstring_log(" MMM  MMM  http://www.scriptdev2.com");
    outstring_log("");
	outstring_log("::CORE::VISIT::WWW.FANATIX.BIZ:::");
	outstring_log("#################################");
	outstring_log("#  Compiled from Fanatix Team   #");
	outstring_log("#################################");

    outstring_log("ScriptDev2 initializing: %s", _FULLVERSION);
    outstring_log("");

    //Get configuration file
    if (!SD2Config.SetSource(_SCRIPTDEV2_CONFIG))
    {
        CanLoadDB = false;
        error_log("SD2: Unable to open configuration file. Database will be unaccessible. Configuration values will use default.");
    }
    else outstring_log("SD2: Using configuration file %s",_SCRIPTDEV2_CONFIG);

    //Check config file version
    if (SD2Config.GetIntDefault("ConfVersion", 0) != SD2_CONF_VERSION)
        error_log("SD2: Configuration file version doesn't match expected version. Some config variables may be wrong or missing.");

    outstring_log("");

    //Load database (must be called after SD2Config.SetSource). In case it failed, no need to even try load.
    if (CanLoadDB)
        LoadDatabase();

    outstring_log("SD2: Loading C++ scripts");
    barGoLink bar(1);
    bar.step();
    outstring_log("");

    for(int i=0;i<MAX_SCRIPTS;i++)
        m_scripts[i]=NULL;

    FillSpellSummary();

    // -- Scripts to be added --

    // -- Areatrigger --
    AddSC_areatrigger_scripts();

    // -- Boss --
    AddSC_boss_emeriss();
    AddSC_boss_taerar();
    AddSC_boss_ysondre();

    // -- Creature --
    AddSC_generic_creature();

    // -- Custom --
    AddSC_npc_acherus_taxi();
    AddSC_npc_wyrmresttempel_taxi();
    AddSC_telefratz();
	AddSC_npc_paymaster();
	AddSC_npc_sickness();

    // -- Examples --
    AddSC_example_creature();
    AddSC_example_escort();
    AddSC_example_gossip_codebox();
    AddSC_example_misc();

    // -- GO --
    AddSC_go_scripts();

    // -- Guard --
    AddSC_guards();

    // -- Honor --

    // -- Item --
    AddSC_item_scripts();

    // -- NPC --
    AddSC_npc_professions();
    AddSC_npcs_special();

    // -- Servers --

    //--------------------
    //------ ZONE --------

    //Alterac Mountains
    AddSC_alterac_mountains();

    //Arathi Highlands
    AddSC_arathi_highlands();

    //Ashenvale Forest
    AddSC_ashenvale();

    //Aunchindoun
    //--Auchenai Crypts
    AddSC_boss_exarch_maladaar();
    AddSC_boss_shirrak_the_dead_watcher();

    //--Mana Tombs
    AddSC_boss_nexusprince_shaffar();
    AddSC_boss_pandemonius();

    //--Sekketh Halls
    AddSC_boss_darkweaver_syth();
    AddSC_boss_talon_king_ikiss();
    AddSC_instance_sethekk_halls();

    //--Shadow Labyrinth
    AddSC_boss_Terrordar_der_Peiniger();
    AddSC_boss_blackheart_the_inciter();
    AddSC_boss_grandmaster_vorpil();
    AddSC_boss_murmur();
    AddSC_instance_shadow_labyrinth();

    //Azshara
    AddSC_boss_azuregos();
    AddSC_azshara();

    //Azuremyst Isle
    AddSC_azuremyst_isle();

    //Badlands
    //Barrens
    AddSC_the_barrens();

    //Black Temple
    AddSC_black_temple();
    AddSC_boss_illidan();
    AddSC_boss_shade_of_akama();
    AddSC_boss_supremus();
    AddSC_boss_gurtogg_bloodboil();
    AddSC_boss_mother_shahraz();
    AddSC_boss_reliquary_of_souls();
    AddSC_boss_teron_gorefiend();
    AddSC_boss_najentus();
    AddSC_boss_illidari_council();
    AddSC_instance_black_temple();

    //Blackfathom Depths
    //Blackrock Depths
    AddSC_blackrock_depths();
    AddSC_boss_ambassador_flamelash();
    AddSC_boss_anubshiah();
    AddSC_boss_draganthaurissan();
    AddSC_boss_general_angerforge();
    AddSC_boss_gorosh_the_dervish();
    AddSC_boss_grizzle();
    AddSC_boss_high_interrogator_gerstahn();
    AddSC_boss_magmus();
    AddSC_boss_moira_bronzebeard();
    AddSC_boss_tomb_of_seven();
    AddSC_instance_blackrock_depths();

    //Blackrock Spire
    AddSC_boss_drakkisath();
    AddSC_boss_halycon();
    AddSC_boss_highlordomokk();
    AddSC_boss_mothersmolderweb();
    AddSC_boss_overlordwyrmthalak();
    AddSC_boss_shadowvosh();
    AddSC_boss_thebeast();
    AddSC_boss_warmastervoone();
    AddSC_boss_quatermasterzigris();
    AddSC_boss_pyroguard_emberseer();
    AddSC_boss_gyth();
    AddSC_boss_rend_blackhand();

    //Blackwing lair
    AddSC_boss_razorgore();
    AddSC_boss_vael();
    AddSC_boss_broodlord();
    AddSC_boss_firemaw();
    AddSC_boss_ebonroc();
    AddSC_boss_flamegor();
    AddSC_boss_chromaggus();
    AddSC_boss_nefarian();
    AddSC_boss_victor_nefarius();

    //Blade's Edge Mountains
    AddSC_blades_edge_mountains();

    //Blasted lands
    AddSC_boss_kruul();
    AddSC_blasted_lands();

    //Bloodmyst Isle
    AddSC_bloodmyst_isle();

    //Borean Tundra
    AddSC_borean_tundra();

    //Burning steppes
    AddSC_burning_steppes();

    //Caverns of Time
    //--Battle for Mt. Hyjal
    AddSC_hyjal();
    AddSC_boss_anetheron();
    AddSC_boss_archimonde();
    AddSC_boss_azgalor();
    AddSC_boss_kazrogal();
    AddSC_boss_rage_winterchill();
    AddSC_instance_mount_hyjal();
	  
    //--Culling of Stratholme
    AddSC_boss_epoch();
    AddSC_boss_mal_ganis();
    AddSC_boss_meathook();
    AddSC_boss_salramm();
    AddSC_instance_culling_of_stratholme();

    //--Old Hillsbrad
    AddSC_boss_captain_skarloc();
    AddSC_boss_epoch_hunter();
    AddSC_boss_lieutenant_drake();
    AddSC_instance_old_hillsbrad();
    AddSC_old_hillsbrad();

    //--The Dark Portal
    AddSC_boss_aeonus();
    AddSC_boss_chrono_lord_deja();
    AddSC_boss_temporus();
    AddSC_dark_portal();
    AddSC_instance_dark_portal();

    //Coilfang Resevoir
    //--Serpent Shrine Cavern
    AddSC_boss_fathomlord_karathress();
    AddSC_boss_hydross_the_unstable();
    AddSC_boss_lady_vashj();
    AddSC_boss_leotheras_the_blind();
    AddSC_boss_morogrim_tidewalker();
    AddSC_instance_serpentshrine_cavern();
    AddSC_mob_underbog_colossus();
    AddSC_boss_the_lurker_below();


    //--Slave Pens

    //--Steam Vault
    AddSC_mob_underbog_colossus();
    AddSC_boss_hydromancer_thespia();
    AddSC_boss_mekgineer_steamrigger();
    AddSC_boss_warlord_kalithresh();
    AddSC_instance_steam_vault();

    //--Underbog
    AddSC_boss_hungarfen();

    //Darkshore
    //Darnassus
    //Deadmines
    AddSC_deadmines();
    AddSC_instance_deadmines();

    //Deadwind pass
    //Desolace
    //Dire Maul
    //Dragonblight
    AddSC_dragonblight();

    //Dun Morogh
    AddSC_dun_morogh();

    //Durotar
    //Duskwood
    AddSC_duskwood();

    //Dustwallow marsh
    AddSC_dustwallow_marsh();

    //Eversong Woods
    AddSC_eversong_woods();

    //Exodar
    //Eastern Plaguelands
    AddSC_eastern_plaguelands();

    //Elwynn Forest
    AddSC_elwynn_forest();

    //Felwood
    AddSC_felwood();

    //Feralas
    AddSC_feralas();

    //Ghostlands
    AddSC_ghostlands();

    //Gnomeregan
    //Gruul's Lair
    AddSC_boss_gruul();
    AddSC_boss_high_king_maulgar();
    AddSC_instance_gruuls_lair();

    //Hellfire Citadel
    //--Blood Furnace
    AddSC_boss_broggok();
    AddSC_boss_the_maker();

    //--Magtheridon's Lair
    AddSC_boss_magtheridon();
    AddSC_instance_magtheridons_lair();

    //--Shattered Halls
    AddSC_boss_grand_warlock_nethekurse();
    AddSC_boss_warbringer_omrogg();
    AddSC_boss_warchief_kargath_bladefist();
    AddSC_instance_shattered_halls();

    //--Ramparts
    AddSC_boss_watchkeeper_gargolmar();
    AddSC_boss_omor_the_unscarred();
    AddSC_instance_ramparts();

    //Hellfire Peninsula
    AddSC_boss_doomlordkazzak();
    AddSC_hellfire_peninsula();

    //Hillsbrad Foothills
    //Hinterlands
    //Ironforge
    AddSC_ironforge();

    //Isle of Quel'Danas
    AddSC_isle_of_queldanas();

    //Karazhan
    AddSC_boss_attumen();
    AddSC_boss_curator();
    AddSC_boss_maiden_of_virtue();
    AddSC_boss_shade_of_aran();
    AddSC_boss_malchezaar();
    AddSC_boss_terestian_illhoof();
    AddSC_boss_nightbane();
    AddSC_boss_netherspite();
    AddSC_netherspite_infernal();
    AddSC_boss_moroes();
    AddSC_bosses_opera();
    AddSC_instance_karazhan();
    AddSC_karazhan();

    //Loch Modan
    AddSC_loch_modan();

    //Lower Blackrock Spire

    // Magister's Terrace
    AddSC_boss_felblood_kaelthas();
    AddSC_boss_selin_fireheart();
    AddSC_boss_vexallus();
    AddSC_boss_priestess_delrissa();
    AddSC_instance_magisters_terrace();

    //Maraudon
    AddSC_boss_celebras_the_cursed();
    AddSC_boss_landslide();
    AddSC_boss_noxxion();
    AddSC_boss_ptheradras();

    //Molten core
    AddSC_boss_lucifron();
    AddSC_boss_magmadar();
    AddSC_boss_gehennas();
    AddSC_boss_garr();
    AddSC_boss_baron_geddon();
    AddSC_boss_shazzrah();
    AddSC_boss_golemagg();
    AddSC_boss_sulfuron();
    AddSC_boss_majordomo();
    AddSC_boss_ragnaros();
    AddSC_instance_molten_core();
    AddSC_molten_core();

    //Moonglade
    AddSC_moonglade();

    //Mulgore
    AddSC_mulgore();

    //Nagrand
    AddSC_nagrand();

    //Naxxramas
    AddSC_boss_anubrekhan();
    AddSC_boss_faerlina();
    AddSC_boss_fourhorsemen();
    AddSC_boss_gluth();
    AddSC_boss_gothik();
    AddSC_boss_grobbulus();
    AddSC_boss_heigan();
    AddSC_boss_kelthuzad();
    AddSC_boss_loatheb();
    AddSC_boss_maexxna();
    AddSC_boss_noth();
    AddSC_boss_patchwerk();
    AddSC_boss_razuvious();
    AddSC_boss_sapphiron();
    AddSC_boss_thaddius();
    AddSC_instance_naxxramas();
    AddSC_npc_naxxramas_controler();
    //Netherstorm
    AddSC_netherstorm();

    //Onyxia's Lair
    AddSC_boss_onyxia();

    //Orgrimmar
    AddSC_orgrimmar();

    //Ragefire Chasm

    //Razorfen Kraul
	AddSC_razorfen_kraul();

    //Razorfen Downs
    AddSC_boss_amnennar_the_coldbringer();
    AddSC_razorfen_downs();

    //Redridge Mountains
    //Ruins of Ahn'Qiraj
    //Scarlet Monastery
    AddSC_boss_arcanist_doan();
    AddSC_boss_azshir_the_sleepless();
    AddSC_boss_bloodmage_thalnos();
    AddSC_boss_headless_horseman();
    AddSC_boss_herod();
    AddSC_boss_high_inquisitor_fairbanks();
    AddSC_boss_houndmaster_loksey();
    AddSC_boss_interrogator_vishas();
    AddSC_boss_mograine_and_whitemane();
    AddSC_boss_scorn();
    AddSC_instance_scarlet_monastery();

    //Scholomance
    AddSC_boss_darkmaster_gandling();
    AddSC_boss_death_knight_darkreaver();
    AddSC_boss_theolenkrastinov();
    AddSC_boss_illuciabarov();
    AddSC_boss_instructormalicia();
    AddSC_boss_jandicebarov();
    AddSC_boss_kormok();
    AddSC_boss_lordalexeibarov();
    AddSC_boss_lorekeeperpolkelt();
    AddSC_boss_rasfrost();
    AddSC_boss_theravenian();
    AddSC_boss_vectus();
    AddSC_instance_scholomance();

    //Searing gorge
    AddSC_searing_gorge();

    //Shadowfang keep
    AddSC_shadowfang_keep();
    AddSC_instance_shadowfang_keep();

    //Shadowmoon Valley
    AddSC_boss_doomwalker();
    AddSC_shadowmoon_valley();

    //Shattrath
    AddSC_shattrath_city();

    //Silithus
    AddSC_silithus();

    //Silvermoon
    AddSC_silvermoon_city();

    //Silverpine forest
    AddSC_silverpine_forest();

    //Stockade
    //Stonetalon mountains
    AddSC_stonetalon_mountains();

    //Stormwind City
    AddSC_stormwind_city();

    //Stranglethorn Vale
    AddSC_stranglethorn_vale();

    //Stratholme
    AddSC_boss_magistrate_barthilas();
    AddSC_boss_maleki_the_pallid();
    AddSC_boss_nerubenkan();
    AddSC_boss_cannon_master_willey();
    AddSC_boss_baroness_anastari();
    AddSC_boss_ramstein_the_gorger();
    AddSC_boss_timmy_the_cruel();
    AddSC_boss_postmaster_malown();
    AddSC_boss_baron_rivendare();
    AddSC_boss_dathrohan_balnazzar();
    AddSC_boss_order_of_silver_hand();
    AddSC_instance_stratholme();
    AddSC_stratholme();

    //Sunken Temple
	//Sunwell Plateau
    AddSC_instance_sunwell_plateau();
    AddSC_boss_kalecgos();
    AddSC_boss_brutallus();
    AddSC_boss_felmyst();
    AddSC_boss_eredar_twins();
    AddSC_boss_muru_entropius();
    //AddSC_boss_kiljaeden();

    //Tanaris
    AddSC_tanaris();

    //Teldrassil
    AddSC_teldrassil();

    //Tempest Keep
    //--Arcatraz
    AddSC_arcatraz();
    AddSC_boss_harbinger_skyriss();
    AddSC_instance_arcatraz();

    //--Botanica
    AddSC_boss_high_botanist_freywinn();
    AddSC_boss_laj();
    AddSC_boss_warp_splinter();

    //--The Eye
    AddSC_boss_kaelthas();
    AddSC_boss_void_reaver();
    AddSC_boss_high_astromancer_solarian();
	AddSC_boss_alar();
    AddSC_instance_the_eye();
    AddSC_the_eye();

    //--The Mechanar
    AddSC_boss_gatewatcher_iron_hand();
    AddSC_boss_nethermancer_sepethrea();
    AddSC_boss_pathaleon_the_calculator();
    AddSC_instance_mechanar();

    //Temple of ahn'qiraj
    AddSC_boss_cthun();
    AddSC_boss_fankriss();
    AddSC_boss_huhuran();
    AddSC_bug_trio();
    AddSC_boss_sartura();
    AddSC_boss_skeram();
    AddSC_boss_twinemperors();
    AddSC_mob_anubisath_sentinel();
    AddSC_instance_temple_of_ahnqiraj();

    //Terokkar Forest
    AddSC_terokkar_forest();

    //Thousand Needles
    //Thunder Bluff
    AddSC_thunder_bluff();

    //Tirisfal Glades
    AddSC_tirisfal_glades();

    //Uldaman
    AddSC_boss_ironaya();
    AddSC_uldaman();

    //Undercity
    AddSC_undercity();

    //Un'Goro Crater
    //Upper blackrock spire
    //Wailing caverns

    //Western plaguelands
    AddSC_western_plaguelands();

    //Westfall
    AddSC_westfall();

    //Wetlands
    //Winterspring
    AddSC_winterspring();

    //Zangarmarsh
    AddSC_zangarmarsh();

    //Zul'Farrak
    AddSC_zulfarrak();

    //Zul'Gurub
    AddSC_boss_arlokk();
    AddSC_boss_gahzranka();
    AddSC_boss_grilek();
    AddSC_boss_hakkar();
    AddSC_boss_hazzarah();
    AddSC_boss_jeklik();
    AddSC_boss_jindo();
    AddSC_boss_mandokir();
    AddSC_boss_marli();
    AddSC_boss_ouro();
    AddSC_boss_renataki();
    AddSC_boss_thekal();
    AddSC_boss_venoxis();
    AddSC_boss_wushoolay();
    AddSC_instance_zulgurub();

    //Zul'Aman
    AddSC_instance_zulaman();
    AddSC_zulaman();
    AddSC_boss_nalorakk();
    AddSC_boss_akilzon();
    AddSC_boss_janalai();
    AddSC_boss_halazzi();
    AddSC_boss_hex_lord_malacrass();
    AddSC_boss_zuljin();
    AddSC_zulaman();

   //Utgarde: Utgarde Keep
   AddSC_boss_ingvar_the_plunderer();
   AddSC_boss_keleseth();
   AddSC_boss_skarvald_dalronn();
   AddSC_instance_utgarde_keep();

   //Utgarde: Utgarde Pinnacle
   AddSC_instance_pinnacle();
   AddSC_boss_skadi();
   AddSC_boss_ymiron();
   AddSC_boss_svala();
   AddSC_boss_palehoof();

   //Nexus: The Nexus
   AddSC_instance_nexus();
   AddSC_boss_magus_telestra();
   AddSC_boss_anomalus();
   AddSC_boss_ormorok();
   AddSC_boss_keristrasza();
   //Nexus: The Oculus
   AddSC_instance_oculus();
   AddSC_boss_drakos();
   AddSC_boss_urom();
   AddSC_boss_varos();
   AddSC_boss_eregos();
   //Azjol'Nerub: Azjol'Nerub
   AddSC_instance_azjol_nerub();
   AddSC_boss_krik_thir();
   AddSC_boss_hadronox();
   AddSC_boss_anub_arak();
   //Azjol'Nerub: Ahn'kahet
   AddSC_instance_ahn_kahet();
   AddSC_boss_taldaram();
   AddSC_boss_nadox();
   AddSC_boss_jedoga();
   AddSC_boss_volazj();
   // Drak Tharon
   AddSC_instance_drak_tharon();
   AddSC_boss_trollgore();
   AddSC_boss_novos();
   AddSC_boss_dred();
   AddSC_boss_tharon_ja();
   //The Violet Hold
   AddSC_instance_violet_hold();
   AddSC_boss_cyanigosa();
   AddSC_boss_erekem();
   AddSC_boss_ichoron();
   AddSC_boss_lavanthor();
   AddSC_boss_moragg();
   AddSC_boss_xevozz();
   AddSC_boss_zuramat();
   //Gundrak
   AddSC_instance_gundrak();
   AddSC_boss_slad_ran();
   AddSC_boss_moorabi();
   AddSC_boss_drakkari_colossus();
   AddSC_boss_gal_darah();
   AddSC_boss_eck();
   //Ulduar: Halls of stone
   AddSC_instance_halls_of_stone();
   AddSC_boss_maiden_of_grief();
   AddSC_boss_krystallus();
   AddSC_boss_sjonnir();
   //Ulduar: Halls of Lightning
   AddSC_instance_halls_of_lightning();
   AddSC_boss_bjarngrim();
   AddSC_boss_ionar();
   AddSC_boss_volkhan();
   AddSC_boss_loken();
   //Caverns of Time: The Culling of Stratholme
   AddSC_instance_culling_of_stratholme();
   AddSC_boss_meathook();
   AddSC_boss_epoch();
   AddSC_boss_mal_ganis();
   AddSC_boss_salramm();
   //Nexus: The Eye of Eternity
   AddSC_instance_eye_of_eternity();
   AddSC_boss_malygos();
   //Obsidian Sanctum
   AddSC_instance_obsidian_sanctum();
   AddSC_boss_sartharion();
   //Vault of Archavon
   AddSC_instance_archavon();
   AddSC_boss_archavon();
   /*Naxxramas (new)
   AddSC_instance_naxx();
   AddSC_boss_anub_rekhan();
   AddSC_boss_faerlina();
   AddSC_boss_maexxna();
   AddSC_boss_patchwerk();
   AddSC_boss_grobbulus();
   AddSC_boss_gluth();
   AddSC_boss_thaddius();
   AddSC_boss_razuvious();
   AddSC_boss_gothik();
   AddSC_boss_four_horsemen();
   AddSC_boss_noth();
   AddSC_boss_heigan();
   AddSC_boss_loatheb();
   AddSC_boss_sapphiron();
   AddSC_boss_kel_thuzad(); */

    // -------------------

    outstring_log(">> Loaded %i C++ Scripts.", num_sc_scripts);
}

//*********************************
//*** Functions used globally ***

void DoScriptText(int32 textEntry, WorldObject* pSource, Unit* target)
{
    if (!pSource)
    {
        error_log("SD2: DoScriptText entry %i, invalid Source pointer.",textEntry);
        return;
    }

    if (textEntry >= 0)
    {
        error_log("SD2: DoScriptText with source entry %u (TypeId=%u, guid=%u) attempts to process text entry %i, but text entry must be negative.",pSource->GetEntry(),pSource->GetTypeId(),pSource->GetGUIDLow(),textEntry);
        return;
    }

    UNORDERED_MAP<int32, StringTextData>::iterator i = TextMap.find(textEntry);

    if (i == TextMap.end())
    {
        error_log("SD2: DoScriptText with source entry %u (TypeId=%u, guid=%u) could not find text entry %i.",pSource->GetEntry(),pSource->GetTypeId(),pSource->GetGUIDLow(),textEntry);
        return;
    }

    debug_log("SD2: DoScriptText: text entry=%i, Sound=%u, Type=%u, Language=%u, Emote=%u",textEntry,(*i).second.SoundId,(*i).second.Type,(*i).second.Language,(*i).second.Emote);

    if((*i).second.SoundId)
    {
        if (GetSoundEntriesStore()->LookupEntry((*i).second.SoundId))
        {
            WorldPacket data(4);
            data.SetOpcode(SMSG_PLAY_SOUND);
            data << uint32((*i).second.SoundId);
            pSource->SendMessageToSet(&data,false);
        }
        else
            error_log("SD2: DoScriptText entry %i tried to process invalid sound id %u.",textEntry,(*i).second.SoundId);
    }

    if((*i).second.Emote)
    {
        if (pSource->GetTypeId() == TYPEID_UNIT || pSource->GetTypeId() == TYPEID_PLAYER)
        {
            ((Unit*)pSource)->HandleEmoteCommand((*i).second.Emote);
        }
        else
            error_log("SD2: DoScriptText entry %i tried to process emote for invalid TypeId (%u).",textEntry,pSource->GetTypeId());
    }

    switch((*i).second.Type)
    {
        case CHAT_TYPE_SAY:
            pSource->MonsterSay(textEntry, (*i).second.Language, target ? target->GetGUID() : 0);
            break;
        case CHAT_TYPE_YELL:
            pSource->MonsterYell(textEntry, (*i).second.Language, target ? target->GetGUID() : 0);
            break;
        case CHAT_TYPE_TEXT_EMOTE:
            pSource->MonsterTextEmote(textEntry, target ? target->GetGUID() : 0);
            break;
        case CHAT_TYPE_BOSS_EMOTE:
            pSource->MonsterTextEmote(textEntry, target ? target->GetGUID() : 0, true);
            break;
        case CHAT_TYPE_WHISPER:
            {
                if (target && target->GetTypeId() == TYPEID_PLAYER)
                    pSource->MonsterWhisper(textEntry, target->GetGUID());
                else error_log("SD2: DoScriptText entry %i cannot whisper without target unit (TYPEID_PLAYER).", textEntry);
            }break;
        case CHAT_TYPE_BOSS_WHISPER:
            {
                if (target && target->GetTypeId() == TYPEID_PLAYER)
                    pSource->MonsterWhisper(textEntry, target->GetGUID(), true);
                else error_log("SD2: DoScriptText entry %i cannot whisper without target unit (TYPEID_PLAYER).", textEntry);
            }break;
        case CHAT_TYPE_ZONE_YELL:
            pSource->MonsterYellToZone(textEntry, (*i).second.Language, target ? target->GetGUID() : 0);
            break;
    }
}

//*********************************
//*** Functions used internally ***

void Script::RegisterSelf()
{
    int id = GetScriptId(Name.c_str());
    if (id != 0)
    {
        m_scripts[id] = this;
        ++num_sc_scripts;
    }
    else
    {
        debug_log("SD2: RegisterSelf, but script named %s does not have ScriptName assigned in database.",(this)->Name.c_str());
        delete this;
    }
}

//********************************
//*** Functions to be Exported ***

MANGOS_DLL_EXPORT
char const* ScriptsVersion()
{
    //TODO: add version text from sd2_db_version
    return _FULLVERSION;
}

MANGOS_DLL_EXPORT
bool GossipHello ( Player * player, Creature *_Creature )
{
    Script *tmpscript = m_scripts[_Creature->GetScriptId()];
    if (!tmpscript || !tmpscript->pGossipHello) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGossipHello(player,_Creature);
}

MANGOS_DLL_EXPORT
bool GossipSelect( Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    debug_log("SD2: Gossip selection, sender: %d, action: %d",sender, action);

    Script *tmpscript = m_scripts[_Creature->GetScriptId()];
    if (!tmpscript || !tmpscript->pGossipSelect) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGossipSelect(player,_Creature,sender,action);
}

MANGOS_DLL_EXPORT
bool GossipSelectWithCode( Player *player, Creature *_Creature, uint32 sender, uint32 action, const char* sCode )
{
    debug_log("SD2: Gossip selection with code, sender: %d, action: %d",sender, action);

    Script *tmpscript = m_scripts[_Creature->GetScriptId()];
    if (!tmpscript || !tmpscript->pGossipSelectWithCode) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGossipSelectWithCode(player,_Creature,sender,action,sCode);
}

MANGOS_DLL_EXPORT
bool QuestAccept( Player *player, Creature *_Creature, Quest const *_Quest )
{
    Script *tmpscript = m_scripts[_Creature->GetScriptId()];
    if (!tmpscript || !tmpscript->pQuestAccept) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestAccept(player,_Creature,_Quest);
}

MANGOS_DLL_EXPORT
bool QuestSelect( Player *player, Creature *_Creature, Quest const *_Quest )
{
    Script *tmpscript = m_scripts[_Creature->GetScriptId()];
    if (!tmpscript || !tmpscript->pQuestSelect) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestSelect(player,_Creature,_Quest);
}

MANGOS_DLL_EXPORT
bool QuestComplete( Player *player, Creature *_Creature, Quest const *_Quest )
{
    Script *tmpscript = m_scripts[_Creature->GetScriptId()];
    if (!tmpscript || !tmpscript->pQuestComplete) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestComplete(player,_Creature,_Quest);
}

MANGOS_DLL_EXPORT
bool ChooseReward( Player *player, Creature *_Creature, Quest const *_Quest, uint32 opt )
{
    Script *tmpscript = m_scripts[_Creature->GetScriptId()];
    if (!tmpscript || !tmpscript->pChooseReward) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pChooseReward(player,_Creature,_Quest,opt);
}

MANGOS_DLL_EXPORT
uint32 NPCDialogStatus( Player *player, Creature *_Creature )
{
    Script *tmpscript = m_scripts[_Creature->GetScriptId()];
    if (!tmpscript || !tmpscript->pNPCDialogStatus) return 100;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pNPCDialogStatus(player,_Creature);
}

MANGOS_DLL_EXPORT
uint32 GODialogStatus( Player *player, GameObject *_GO )
{
    Script *tmpscript = m_scripts[_GO->GetGOInfo()->ScriptId];
    if (!tmpscript || !tmpscript->pGODialogStatus) return 100;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGODialogStatus(player,_GO);
}

MANGOS_DLL_EXPORT
bool ItemHello( Player *player, Item *_Item, Quest const *_Quest )
{
    Script *tmpscript = m_scripts[_Item->GetProto()->ScriptId];
    if (!tmpscript || !tmpscript->pItemHello) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pItemHello(player,_Item,_Quest);
}

MANGOS_DLL_EXPORT
bool ItemQuestAccept( Player *player, Item *_Item, Quest const *_Quest )
{
    Script *tmpscript = m_scripts[_Item->GetProto()->ScriptId];
    if (!tmpscript || !tmpscript->pItemQuestAccept) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pItemQuestAccept(player,_Item,_Quest);
}

MANGOS_DLL_EXPORT
bool GOHello( Player *player, GameObject *_GO )
{
    Script *tmpscript = m_scripts[_GO->GetGOInfo()->ScriptId];
    if (!tmpscript || !tmpscript->pGOHello) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOHello(player,_GO);
}

MANGOS_DLL_EXPORT
bool GOQuestAccept( Player *player, GameObject *_GO, Quest const *_Quest )
{
    Script *tmpscript = m_scripts[_GO->GetGOInfo()->ScriptId];
    if (!tmpscript || !tmpscript->pGOQuestAccept) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOQuestAccept(player,_GO,_Quest);
}

MANGOS_DLL_EXPORT
bool GOChooseReward( Player *player, GameObject *_GO, Quest const *_Quest, uint32 opt )
{
    Script *tmpscript = m_scripts[_GO->GetGOInfo()->ScriptId];
    if (!tmpscript || !tmpscript->pGOChooseReward) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOChooseReward(player,_GO,_Quest,opt);
}

MANGOS_DLL_EXPORT
bool AreaTrigger( Player *player, AreaTriggerEntry * atEntry)
{
    Script *tmpscript = m_scripts[GetAreaTriggerScriptId(atEntry->id)];
    if (!tmpscript || !tmpscript->pAreaTrigger) return false;

    return tmpscript->pAreaTrigger(player, atEntry);
}

MANGOS_DLL_EXPORT
CreatureAI* GetAI(Creature *_Creature)
{
    Script *tmpscript = m_scripts[_Creature->GetScriptId()];
    if (!tmpscript || !tmpscript->GetAI) return NULL;

    return tmpscript->GetAI(_Creature);
}

MANGOS_DLL_EXPORT
bool ItemUse( Player *player, Item* _Item, SpellCastTargets const& targets)
{
    Script *tmpscript = m_scripts[_Item->GetProto()->ScriptId];
    if (!tmpscript || !tmpscript->pItemUse) return false;

    return tmpscript->pItemUse(player,_Item,targets);
}

MANGOS_DLL_EXPORT
bool ReceiveEmote( Player *player, Creature *_Creature, uint32 emote )
{
    Script *tmpscript = m_scripts[_Creature->GetScriptId()];
    if (!tmpscript || !tmpscript->pReceiveEmote) return false;

    return tmpscript->pReceiveEmote(player, _Creature, emote);
}

MANGOS_DLL_EXPORT
bool EffectDummyCreature(Unit *pCaster, uint32 spellId, uint32 effIndex, Creature *pCreatureTarget)
{
    Script *tmpscript = m_scripts[pCreatureTarget->GetScriptId()];

    if (!tmpscript || !tmpscript->pEffectDummyCreature) return false;

    return tmpscript->pEffectDummyCreature(pCaster, spellId, effIndex, pCreatureTarget);
}

MANGOS_DLL_EXPORT
bool EffectDummyGameObj(Unit *pCaster, uint32 spellId, uint32 effIndex, GameObject *pGameObjTarget)
{
    Script *tmpscript = m_scripts[pGameObjTarget->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pEffectDummyGameObj) return false;

    return tmpscript->pEffectDummyGameObj(pCaster, spellId, effIndex, pGameObjTarget);
}

MANGOS_DLL_EXPORT
bool EffectDummyItem(Unit *pCaster, uint32 spellId, uint32 effIndex, Item *pItemTarget)
{
    Script *tmpscript = m_scripts[pItemTarget->GetProto()->ScriptId];

    if (!tmpscript || !tmpscript->pEffectDummyItem) return false;

    return tmpscript->pEffectDummyItem(pCaster, spellId, effIndex, pItemTarget);
}

MANGOS_DLL_EXPORT
InstanceData* CreateInstanceData(Map *map)
{
    if (!map->IsDungeon()) return NULL;

    Script *tmpscript = m_scripts[((InstanceMap*)map)->GetScriptId()];
    if (!tmpscript || !tmpscript->GetInstanceData) return NULL;

    return tmpscript->GetInstanceData(map);
}
