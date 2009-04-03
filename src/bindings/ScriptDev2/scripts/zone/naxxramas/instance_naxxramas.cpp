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

#include "precompiled.h"
#include "def_naxxramas.h"

#define ENCOUNTERS     15

/* Naxxramas encounters:
Spiderwing
ENCOUNT_ANUBREKHAN - Anub'Rekhan
ENCOUNT_FAERLINA - Grand Widow Faerlina
ENCOUNT_MAEXXNA - Maexxna
*/

struct MANGOS_DLL_DECL instance_naxxramas : public ScriptedInstance
{
    instance_naxxramas(Map *Map) : ScriptedInstance(Map) {Initialize();};

    uint64 go_anubrekhan_door;
    uint64 go_anubrekhan_gate;
    uint64 go_faerlina_door;
    uint64 go_faerlina_web;
    uint64 go_maexxna_outerweb;
    uint64 go_maexxna_innerweb;
    uint64 go_vaccuum_enter;
    uint64 go_vaccuum_exit;
    uint64 go_vaccuum_combat;
    uint64 go_deathknight_door;
    uint64 go_fourhorsemen_chest;
    uint64 go_patchwork_exit;
    uint64 go_gluth_exit;
    uint64 go_thaddius_door;
    uint64 go_noth_entry;
    uint64 go_noth_exit;
	uint64 go_heigan_exitgate;
    uint64 go_heigan_entry;
    uint64 go_heigan_exit;
    uint64 go_loatheb_door;
    uint64 go_frostwyrm_waterfall;
    uint64 go_kelthuzad_door;

	uint64 guid_anubrekhan;
	uint64 guid_faerlina;
	uint64 guid_maexxna;
    uint64 guid_razuvious;
    uint64 guid_gothik;
    uint64 guid_korthazz;
    uint64 guid_blaumeux;
    uint64 guid_mograine;
    uint64 guid_zeliek;
    uint64 guid_patchwerk;
    uint64 guid_grobbulus;
    uint64 guid_gluth;
    uint64 guid_thaddius;
    uint64 guid_feugen;
    uint64 guid_stalagg;
    uint64 guid_noth;
    uint64 guid_heigan;
    uint64 guid_loatheb;
    uint64 guid_sapphiron;
    uint64 guid_kelthuzad;

	uint32 Encounters[ENCOUNTERS];

    void Initialize()
    {
		for(uint8 i = 0; i < ENCOUNTERS; i++)
			Encounters[i] = NOT_STARTED;

        go_anubrekhan_door = 0;
        go_anubrekhan_gate = 0;
        go_faerlina_door = 0;
        go_faerlina_web = 0;
        go_maexxna_outerweb = 0;
        go_maexxna_innerweb = 0;
        go_vaccuum_enter = 0;
        go_vaccuum_exit = 0;
        go_vaccuum_combat = 0;
        go_deathknight_door = 0;
        go_fourhorsemen_chest = 0;
        go_patchwork_exit = 0;
        go_gluth_exit = 0;
        go_thaddius_door = 0;
        go_noth_entry = 0;
        go_noth_exit = 0;
        go_heigan_entry = 0;
        go_heigan_exit = 0;
	    go_heigan_exitgate = 0;
        go_loatheb_door = 0;
        go_frostwyrm_waterfall = 0;
        go_kelthuzad_door = 0;

		guid_anubrekhan = 0;
		guid_faerlina = 0;
		guid_maexxna = 0;   
        guid_razuvious = 0;
        guid_gothik = 0;
        guid_korthazz = 0;
        guid_blaumeux = 0;
        guid_mograine = 0;
        guid_zeliek = 0;
        guid_patchwerk = 0;
        guid_grobbulus = 0;
        guid_gluth = 0;
        guid_thaddius = 0;
        guid_feugen = 0;
        guid_stalagg = 0;
        guid_noth = 0;
        guid_heigan = 0;
        guid_loatheb = 0;
        guid_sapphiron = 0;
        guid_kelthuzad = 0;

    }

    bool IsEncounterInProgress() const 
    {
		for(uint8 i = 0; i < ENCOUNTERS; i++)
            if(Encounters[i] == IN_PROGRESS) return true;

        return false;
    }


    void OnCreatureCreate(Creature *creature, uint32 creature_entry)
    {
        switch(creature_entry)
        {
            //Spiderwing ------------------------------------
            case 15956:
                guid_anubrekhan = creature->GetGUID();
                if(creature->isDead())
                    SetData(ENCOUNT_ANUBREKHAN, DONE);
                break;
            case 15953: 
                guid_faerlina = creature->GetGUID();
                if(creature->isDead())
                    SetData(ENCOUNT_FAERLINA, DONE);
                break;
            case 15952: 
                guid_maexxna = creature->GetGUID();
                if(creature->isDead())
                    SetData(ENCOUNT_MAEXXNA, DONE);
                break;
            //Spiderwing ------------------------------------
            //Deathknight Wing ------------------------------
            case 16061: 
                guid_razuvious = creature->GetGUID();
                if(creature->isDead())
                    SetData(ENCOUNT_RAZUVIOUS, DONE);
                break;
            case 16060: 
                guid_gothik = creature->GetGUID();
                if(creature->isDead())
                    SetData(ENCOUNT_GOTHIK, DONE);
                break;
            case 16064: 
                guid_korthazz = creature->GetGUID();
                SetData(ENCOUNT_FOURHORSEMAN, 0);
                break;
            case 16065: 
                guid_blaumeux = creature->GetGUID();
                SetData(ENCOUNT_FOURHORSEMAN, 0);
                break;
            case 16062: 
                guid_mograine = creature->GetGUID();
                SetData(ENCOUNT_FOURHORSEMAN, 0);
                break;
            case 16063: 
                guid_zeliek = creature->GetGUID();
                SetData(ENCOUNT_FOURHORSEMAN, 0);
                break;
            //Deathknight Wing ------------------------------
            //Abomination Wing ------------------------------
            case 16028: 
                guid_patchwerk = creature->GetGUID();
                if(creature->isDead())
                    SetData(ENCOUNT_PATCHWERK, DONE);
                break;
            case 15931: 
                guid_grobbulus = creature->GetGUID();
                if(creature->isDead())
                    SetData(ENCOUNT_GROBBULUS, DONE);
                break;
            case 15932: 
                guid_gluth = creature->GetGUID();
                if(creature->isDead())
                    SetData(ENCOUNT_GLUTH, DONE);
                break;
            case 15928: 
                guid_thaddius = creature->GetGUID();
                if(creature->isDead())
                    SetData(ENCOUNT_THADDIUS, DONE);
                break;
            case 15930: 
                guid_feugen = creature->GetGUID();
                break;
            case 15929: 
                guid_stalagg = creature->GetGUID();
                break;
            //Abomination Wing ------------------------------
            //Plague Wing -----------------------------------
            case 15954: 
                guid_noth = creature->GetGUID();
                if(creature->isDead())
                    SetData(ENCOUNT_NOTH, DONE);
                break;
            case 15936: 
                guid_heigan = creature->GetGUID();
                if(creature->isDead())
                    SetData(ENCOUNT_HEIGAN, DONE);
                break;
            case 16011: 
                guid_loatheb = creature->GetGUID();
                if(creature->isDead())
                    SetData(ENCOUNT_LOATHEB, DONE);
                break;
            //Plague Wing -----------------------------------
            //Frostwyrm Lair --------------------------------
            case 15989: 
                guid_sapphiron = creature->GetGUID();
                if(creature->isDead())
                    SetData(ENCOUNT_SAPPHIRON, DONE);
                break;
            case 15990: 
                guid_kelthuzad = creature->GetGUID();
                if(creature->isDead())
                    SetData(ENCOUNT_KELTHUZAD, DONE);
                break;
            //Frostwyrm Lair --------------------------------
        }
    }

    void OnObjectCreate(GameObject *go)
    {
        switch(go->GetEntry())
        {
			//Spiderwing ------------------------------------
            case 181126:
                go_anubrekhan_door = go->GetGUID();
                break;
            case 181195:
                go_anubrekhan_gate = go->GetGUID();
                break;
            case 181167:
                go_faerlina_door = go->GetGUID();
                break;
            case 181235:
                go_faerlina_web = go->GetGUID();
                break;
            case 181209:
                go_maexxna_outerweb = go->GetGUID();
                break;
            case 181197:
                go_maexxna_innerweb = go->GetGUID();
            break;
			//Spiderwing ------------------------------------
            //Deathknight Wing ------------------------------
            case 181124:
                go_vaccuum_enter = go->GetGUID();
            break;
            case 181125:
                go_vaccuum_exit = go->GetGUID();
            break;
            case 181170:
                go_vaccuum_combat = go->GetGUID();
            break;
            case 181119:
                go_deathknight_door = go->GetGUID();
            break;
            case 181366:
                go_fourhorsemen_chest = go->GetGUID();
            break;
            //Deathknight Wing ------------------------------
            //Abomination Wing ------------------------------
            case 181123:
                go_patchwork_exit = go->GetGUID();
            break;
            case 181120:
                go_gluth_exit = go->GetGUID();
            break;
            case 181121:
                go_thaddius_door = go->GetGUID();
            break;
            //Abomination Wing ------------------------------
            //Plague Wing -----------------------------------
            case 181200:
                go_noth_entry = go->GetGUID();
            break;
            case 181201:
                go_noth_exit = go->GetGUID();
            break;
            case 181202:
                go_heigan_entry = go->GetGUID();
            break;
            case 181203:
                go_heigan_exit = go->GetGUID();
            break;
			case 181496:
                go_heigan_exitgate = go->GetGUID();
            break;
            case 181241:
                go_loatheb_door = go->GetGUID();
            break;		
            //Plague Wing -----------------------------------
            //Frostwyrm Lair --------------------------------
            case 181225:
                go_frostwyrm_waterfall = go->GetGUID();
            break;
            case 181228:
                go_kelthuzad_door = go->GetGUID();
            break;
            //Frostwyrm Lair --------------------------------
        }
    }


    uint64 GetData64(uint32 identifier)
    {
        switch (identifier)
        {
        //Spiderwing -------------------------------------
        case GUID_ANUBREKHAN:
            return guid_anubrekhan;
            break;
        case GUID_FAERLINA:
            return guid_faerlina;
            break;
        case GUID_MAEXXNA:
            return guid_maexxna;
            break;
        //Spiderwing -------------------------------------
        //Abomination Wing ------------------------------
		case GUID_PATCHWERK:
            return guid_patchwerk;
            break;
        case GUID_GROBBULUS:
            return guid_grobbulus;
            break;
        case GUID_GLUTH:
            return guid_gluth;
            break;
		case GUID_THADDIUS:
            return guid_thaddius;
            break;
		case GUID_FEUGEN:
            return guid_feugen;
            break;
		case GUID_STALAGG:
            return guid_stalagg;
            break;
        //Abomination Wing ------------------------------
        //Deathknight Wing ------------------------------
		case GUID_RAZUVIOUS:
            return guid_razuvious;
            break;
        case GUID_GOTHIK:
            return guid_gothik;
            break;
        case GUID_KORTHAZZ:
            return guid_korthazz;
            break;
		case GUID_BLAUMEUX:
            return guid_blaumeux;
            break;
		case GUID_MOGRAINE:
            return guid_mograine;
            break;
        case GUID_ZELIEK:
            return guid_zeliek;
            break;
        //Deathknight Wing ------------------------------
	    //Plague Wing ------------------------------------
		case GUID_NOTH:
            return guid_noth;
            break;
        case GUID_HEIGAN:
            return guid_heigan;
            break;
        case GUID_LOATHEB:
            return guid_loatheb;
            break;
		//Plague Wing ------------------------------------
        //Frostwyrm Lair --------------------------------
        case GUID_SAPPHIRON:
            return guid_sapphiron;
            break;
        case GUID_KELTHUZAD:
            return guid_kelthuzad;
            break;
        //Frostwyrm Lair --------------------------------
        //Doors -------------------------------------------
        case GO_ANUBREKHAN_DOOR:
            return go_anubrekhan_door;
            break;
        case GO_ANUBREKHAN_GATE:  
            return go_anubrekhan_gate;
            break;
        case GO_FAERLINA_DOOR:  
            return go_faerlina_door;
            break;
        case GO_FAERLINA_WEB:   
            return  go_faerlina_web;
            break;
        case GO_MAEXXNA_OUTERWEB:
            return go_maexxna_outerweb;
            break;
        case GO_MAEXXNA_INNERWEB:
            return go_maexxna_innerweb;
            break;
        case GO_VACCUUM_ENTER:
            return go_vaccuum_enter;
            break;
        case GO_VACCUUM_EXIT:
            return go_vaccuum_exit;
            break;
        case GO_VACCUUM_COMBAT:
            return go_vaccuum_combat;
            break;
        case GO_DEATHKNIGHT_DOOR:
            return go_deathknight_door;
            break;
        case GO_FOURHORSEMEN_CHEST:
            return go_fourhorsemen_chest;
            break;
        case GO_PATCHWORK_EXIT:
            return go_patchwork_exit;
            break;
        case GO_GLUTH_EXIT:
            return go_gluth_exit;
            break;
        case GO_THADDIUS_DOOR:
            return go_thaddius_door;
            break;            
        case GO_NOTH_ENTRY:
            return go_noth_entry;
            break;
        case GO_NOTH_EXIT:
            return go_noth_exit;
            break;  
        case GO_HEIGAN_ENTRY:
            return go_heigan_entry;
            break;
        case GO_HEIGAN_EXIT:
            return go_heigan_exit;
            break;  
        case GO_HEIGAN_EXITGATE:
            return go_heigan_exitgate;
            break;
        case GO_LOATHEB_DOOR:
            return go_loatheb_door;
            break;  
        case GO_FROSTWYRM_WATERFALL:
            return go_frostwyrm_waterfall;
            break; 
        case GO_KELTHUZAD_DOOR:
            return go_kelthuzad_door;
            break; 
        }
        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        switch (type)
        {
			//Spiderwing ------------------------------------
        case ENCOUNT_ANUBREKHAN:
            Encounters[ENCOUNT_ANUBREKHAN] = data;
            switch (data)
            {
            case NOT_STARTED:
                //CloseDoor(go_anubrekhan_gate);
                //OpenDoor(go_anubrekhan_door);
                //CloseDoor(go_maexxna_outerweb);
                break;
            case IN_PROGRESS:
                //CloseDoor(go_anubrekhan_door);
                break;
            case DONE:
                //OpenDoor(go_anubrekhan_gate);
                //OpenDoor(go_anubrekhan_door);
                if(Encounters[ENCOUNT_FAERLINA] == DONE)
                {
                    //OpenDoor(go_maexxna_outerweb);
                }
                break;
            }
            break;
        case ENCOUNT_FAERLINA:
            Encounters[ENCOUNT_FAERLINA] = data;
            switch (data)
            {
            case NOT_STARTED:
                //CloseDoor(go_faerlina_door);
                //OpenDoor(go_faerlina_web);
                //CloseDoor(go_maexxna_outerweb);
                break;
            case IN_PROGRESS:
                //CloseDoor(go_faerlina_web);
                break;
            case DONE:
                //OpenDoor(go_faerlina_door);
                //OpenDoor(go_faerlina_web);
                if(Encounters[ENCOUNT_ANUBREKHAN] == DONE)
                {
                    //OpenDoor(go_maexxna_outerweb);
                }
                break;
            }
            break;
        case ENCOUNT_MAEXXNA:
            Encounters[ENCOUNT_MAEXXNA] = data;
            switch (data)
            {
            case NOT_STARTED:
                //OpenDoor(go_maexxna_innerweb);
                break;
            case IN_PROGRESS:
                //CloseDoor(go_maexxna_innerweb);
                break;
            case DONE:
                //OpenDoor(go_maexxna_innerweb);
                break;
            }
            break;
			//Spiderwing ------------------------------------
			//Abomination Wing ------------------------------
		case ENCOUNT_PATCHWERK:
            Encounters[ENCOUNT_PATCHWERK] = data;
            switch (data)
            {
            case NOT_STARTED:
                //CloseDoor(go_patchwork_exit);
                //CloseDoor(go_thaddius_door);
                //CloseDoor(go_gluth_exit);
                break;
            case IN_PROGRESS:
                //CloseDoor(go_patchwork_exit);
                break;
            case DONE:
                //OpenDoor(go_patchwork_exit);
                if (Encounters[ENCOUNT_GLUTH] == DONE && Encounters[ENCOUNT_GROBBULUS] == DONE)
                {
                    //OpenDoor(go_thaddius_door);
                    //OpenDoor(go_gluth_exit);
                }
                break;
            }
            break;
		case ENCOUNT_GROBBULUS:
            Encounters[ENCOUNT_GROBBULUS] = data;
            switch (data)
            {
            case NOT_STARTED:
                //CloseDoor(go_thaddius_door);
                //CloseDoor(go_gluth_exit);
                break;
            case IN_PROGRESS:
                //CloseDoor(go_patchwork_exit);
                break;
            case DONE:
                //OpenDoor(go_patchwork_exit);
                if (Encounters[ENCOUNT_GLUTH] == DONE && Encounters[ENCOUNT_PATCHWERK] == DONE)
                {
                    //OpenDoor(go_thaddius_door);
                    //OpenDoor(go_gluth_exit);
                }
                break;
            }
            break;
		case ENCOUNT_GLUTH:
            Encounters[ENCOUNT_GLUTH] = data;
            switch (data)
            {
            case NOT_STARTED:
                //CloseDoor(go_gluth_exit);
                //CloseDoor(go_thaddius_door);
                break;
            case IN_PROGRESS:
                //CloseDoor(go_gluth_exit);
                break;
            case DONE:
                if (Encounters[ENCOUNT_GROBBULUS] == DONE && Encounters[ENCOUNT_PATCHWERK] == DONE)
                {
                    //OpenDoor(go_thaddius_door);
                    //OpenDoor(go_gluth_exit);
                }
                break;
            }
            break;
		case ENCOUNT_THADDIUS:
            Encounters[ENCOUNT_THADDIUS] = data;
            switch (data)
            {
            case NOT_STARTED:
                break;
            case IN_PROGRESS:
                break;
            case DONE:
                break;
            }
            break;
			//Abomination Wing ------------------------------
			//Deathknight Wing ------------------------------
		case ENCOUNT_RAZUVIOUS:
            Encounters[ENCOUNT_RAZUVIOUS] = data;
            switch (data)
            {
            case NOT_STARTED:
                //CloseDoor(go_vaccuum_enter);
                //CloseDoor(go_vaccuum_exit);
                //CloseDoor(go_deathknight_door);
                break;
            case IN_PROGRESS:
                //CloseDoor(go_vaccuum_enter);
                //CloseDoor(go_vaccuum_exit);
                //CloseDoor(go_deathknight_door);
                break;
            case DONE:
                //OpenDoor(go_vaccuum_enter);
                if (Encounters[ENCOUNT_GOTHIK] == DONE)
                {
                    //OpenDoor(go_deathknight_door);
                }
                break;
            }
            break;
		case ENCOUNT_GOTHIK:
            Encounters[ENCOUNT_GOTHIK] = data;
            switch (data)
            {
            case NOT_STARTED:
                //CloseDoor(go_vaccuum_exit);
                //OpenDoor(go_vaccuum_combat);
                //CloseDoor(go_deathknight_door);
                break;
            case IN_PROGRESS:
                //CloseDoor(go_vaccuum_enter);
                //CloseDoor(go_vaccuum_exit);
                //CloseDoor(go_deathknight_door);
                break;
            case DONE:
                //OpenDoor(go_vaccuum_enter);
                //OpenDoor(go_vaccuum_exit);
                if (Encounters[ENCOUNT_RAZUVIOUS] == DONE)
                {
                    //OpenDoor(go_deathknight_door);
                }
                break;
            }
            break;
        //case GOTHIK_OPEN_COMBAT_DOOR: 
            //OpenDoor(go_vaccuum_combat);
            //break;
        //case GOTHIK_CLOSE_COMBAT_DOOR: 
            //CloseDoor(go_vaccuum_combat);
            //break;
		case ENCOUNT_FOURHORSEMAN:
            Encounters[ENCOUNT_FOURHORSEMAN] = data;
            switch (data)
            {
            case NOT_STARTED:
                //OpenDoor(go_deathknight_door);
                //go_fourhorsemen_chest->SetUInt32Value(GAMEOBJECT_FLAGS,4);
                //go_fourhorsemen_chest->SetUInt32Value(GAMEOBJECT_FACTION,21);
                break;
            case IN_PROGRESS:
                //CloseDoor(go_deathknight_door);
                break;
            case DONE:
                //OpenDoor(go_deathknight_door);
                //go_fourhorsemen_chest->SetUInt32Value(GAMEOBJECT_FLAGS,0);
                //go_fourhorsemen_chest->SetUInt32Value(GAMEOBJECT_FACTION,35);
                break;
            }
            break;
			//Deathknight Wing ------------------------------
			//Plague Wing -----------------------------------  
		case ENCOUNT_NOTH:
            Encounters[ENCOUNT_NOTH] = data;
            switch (data)
            {
            case NOT_STARTED:
                //OpenDoor(go_noth_entry);
                //CloseDoor(go_noth_exit);
                //CloseDoor(go_loatheb_door);
                break;
            case IN_PROGRESS:
                //CloseDoor(go_noth_entry);
                break;
            case DONE:
                //OpenDoor(go_noth_entry);
                //OpenDoor(go_noth_exit);
				//OpenDoor(go_heigan_entry);
                if(Encounters[ENCOUNT_HEIGAN] == DONE)
                    //OpenDoor(go_loatheb_door);
                break;
            }
            break;
		case ENCOUNT_HEIGAN:
            Encounters[ENCOUNT_HEIGAN] = data;
            switch (data)
            {
            case NOT_STARTED:
                //CloseDoor(go_heigan_exitgate);
                //OpenDoor(go_heigan_exit);
                //CloseDoor(go_loatheb_door);
                break;
            case IN_PROGRESS:
                //CloseDoor(go_heigan_entry);
                //CloseDoor(go_heigan_exitgate);
                break;
            case DONE:
                //OpenDoor(go_heigan_entry);
				//OpenDoor(go_heigan_exit);
                //OpenDoor(go_heigan_exitgate);
                if(Encounters[ENCOUNT_NOTH] == DONE)
                    //OpenDoor(go_loatheb_door);
                break;
            }
            break;
		case ENCOUNT_LOATHEB:
            Encounters[ENCOUNT_LOATHEB] = data;
            switch (data)
            {
            case NOT_STARTED:
                //OpenDoor(go_loatheb_door);
                break;
            case IN_PROGRESS:
                //CloseDoor(go_loatheb_door);
                break;
            case DONE:
                //OpenDoor(go_loatheb_door);
                break;
            }
            break;
			//Plague Wing -----------------------------------
			//Frostwyrm Lair --------------------------------
		case ENCOUNT_SAPPHIRON:
            Encounters[ENCOUNT_SAPPHIRON] = data;
            switch (data)
            {
            case NOT_STARTED:
                //CloseDoor(go_frostwyrm_waterfall);
                break;
            case IN_PROGRESS:
                break;
            case DONE:
                //OpenDoor(go_frostwyrm_waterfall);
                //OpenDoor(go_kelthuzad_door);
                break;
            }
            break;
		case ENCOUNT_KELTHUZAD:
            Encounters[ENCOUNT_KELTHUZAD] = data;
            switch (data)
            {
            case NOT_STARTED:
                //OpenDoor(go_kelthuzad_door);
                break;
            case IN_PROGRESS:
                //CloseDoor(go_kelthuzad_door);
                break;
            case DONE:
                //OpenDoor(go_kelthuzad_door);
                break;
            }
            break;
			//Frostwyrm Lair --------------------------------
        }
   
    }

    uint32 GetData(uint32 type)
    {
        return 0;
    }
};

InstanceData* GetInstanceData_naxxramas(Map* map)
{
    return new instance_naxxramas(map);
}

void AddSC_instance_naxxramas()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_naxxramas";
    newscript->GetInstanceData = GetInstanceData_naxxramas;
    newscript->RegisterSelf();
}
