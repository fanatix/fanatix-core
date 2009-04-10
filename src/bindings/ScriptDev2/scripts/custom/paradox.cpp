#include "precompiled.h"
#include <string>
#include <sstream>
#include "Database/DatabaseEnv.h"
#include "../../../../shared/Database/DatabaseEnv.h"
#include "../../../../game/GossipDef.h"
#include "../../../../game/Player.h"
DatabaseType CharacterDatabase;
// All in One Script Eigentum von http://www.wow-paradox.de

bool GossipHello_paradox(Player *player, Creature *_Creature)
{
	player->ADD_GOSSIP_ITEM( 5, "Staedte Reisen", GOSSIP_SENDER_MAIN, 1300);
	player->ADD_GOSSIP_ITEM( 5, "Arena", GOSSIP_SENDER_MAIN, 1400);
	player->ADD_GOSSIP_ITEM( 5, "Gilde", GOSSIP_SENDER_MAIN, 1500);
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
	return true;
}


void SendDefaultMenu_paradox(Player *player, Creature *_Creature, uint32 action )

// Teleport
{
        /*if(!player->getAttackers().empty()) 
	{
		_Creature->MonsterSay("Du befindest dich im Kampf!", LANG_COMMON, NULL);
		return;
	}*///getAttackers gibt es nicht mehr ...

	if( player->getLevel() < 8  ) //sollte noch funktionieren
	{
		_Creature->MonsterSay("Du benoetigst 8+", LANG_COMMON, NULL);
		return;
	}
	if(action>1300 && action < 1399)
	{
		std::string q2("SELECT `map`, `x`, `y`, `z`, `r` FROM `teleguy` WHERE `gossipNR` = ");
		std::stringstream ac_str;
		ac_str << action;
		q2 += ac_str.str();
		QueryResult *result__ = CharacterDatabase.Query( q2.c_str() );
		if(result__)
		{
			Field *fieldss = result__->Fetch();
//_player->TeleportTo(mapid, x, y, z, ort);ort??? vielleicht ORienTation
			player->TeleportTo(fieldss[0].GetUInt32(), fieldss[1].GetFloat(), fieldss[2].GetFloat(), fieldss[3].GetFloat(), fieldss[4].GetFloat());
		} else
		{
			_Creature->MonsterSay("Fehler: teleguy:X001 -> bitte melde es einem GM bzw Admin", LANG_COMMON, NULL);
		}
	}
	if(action==1300)
	{	
			
			if ( player->GetTeam() == ALLIANCE )
			{
				std::string q1("SELECT `name`, `gossipNR`, `lvl` from `teleguy` where `fraktion` = 'a' or `fraktion` = 'b'");
				QueryResult *result_ = CharacterDatabase.Query( q1.c_str() );
				if(result_)
				{
					do
					{
						Field *fields = result_->Fetch();
						if(player->getLevel() >= fields[2].GetUInt32())
						{
							//sLog.outString(fields[0].GetString());
							player->ADD_GOSSIP_ITEM( 5, fields[0].GetString(), GOSSIP_SENDER_MAIN, fields[1].GetUInt32());
						}
					} while(result_->NextRow());
				}


			}  else {
				std::string q1("SELECT `name`, `gossipNR`, `lvl` from `teleguy` where `fraktion` = 'h' or `fraktion` = 'b'");
				QueryResult *result_ = CharacterDatabase.Query( q1.c_str() );
				if(result_)
				{
					do
					{
						Field *fields = result_->Fetch();
						if(player->getLevel() >= fields[2].GetUInt32())
						{
							player->ADD_GOSSIP_ITEM( 5, fields[0].GetString(), GOSSIP_SENDER_MAIN, fields[1].GetUInt32());
						}
					} while(result_->NextRow());
				}
			}
			player->ADD_GOSSIP_ITEM( 5, "Ende", GOSSIP_SENDER_MAIN, 1600);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
	}


// Event Port
	if(action>1400 && action<1500)
	{
		std::string q2("SELECT `map`, `x`, `y`, `z`, `r` FROM `eventguy` WHERE `gossipNR` = ");
		std::stringstream ac_str;
		ac_str << action;
		q2 += ac_str.str();
		QueryResult *result__ = CharacterDatabase.Query( q2.c_str() );
		if(result__)
		{
			Field *fieldss = result__->Fetch();
			player->TeleportTo(fieldss[0].GetUInt32(), fieldss[1].GetFloat(), fieldss[2].GetFloat(), fieldss[3].GetFloat(), fieldss[4].GetFloat());
		} else
		{
			_Creature->MonsterSay("Fehler: teleguy:X001 -> bitte melde es einem GM bzw Admin", LANG_COMMON, NULL);
		}
	}
	if(action==1400)
	{	
			
			if ( player->GetTeam() == ALLIANCE )
			{
				std::string q1("SELECT `name`, `gossipNR`, `lvl` from `eventguy` where `fraktion` = 'a' or `fraktion` = 'b'");
				QueryResult *result_ = CharacterDatabase.Query( q1.c_str() );
				if(result_)
				{
					do
					{
						Field *fields = result_->Fetch();
						if(player->getLevel() >= fields[2].GetUInt32())
						{
							//sLog.outString(fields[0].GetString());
							player->ADD_GOSSIP_ITEM( 5, fields[0].GetString(), GOSSIP_SENDER_MAIN, fields[1].GetUInt32());
						}
					} while(result_->NextRow());
				}


			}  else {
				std::string q1("SELECT `name`, `gossipNR`, `lvl` from `eventguy` where `fraktion` = 'h' or `fraktion` = 'b'");
				QueryResult *result_ = CharacterDatabase.Query( q1.c_str() );
				if(result_)
				{
					do
					{
						Field *fields = result_->Fetch();
						if(player->getLevel() >= fields[2].GetUInt32())
						{
							player->ADD_GOSSIP_ITEM( 5, fields[0].GetString(), GOSSIP_SENDER_MAIN, fields[1].GetUInt32());
						}
					} while(result_->NextRow());
				}
			}
			player->ADD_GOSSIP_ITEM( 5, "Ende", GOSSIP_SENDER_MAIN, 1600);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
	}


// Gildenhaus
	if(action== 1500)
	{
		player->ADD_GOSSIP_ITEM( 5, "Zum Gildenhaus", GOSSIP_SENDER_MAIN, 1501);
		player->ADD_GOSSIP_ITEM( 5, "Ende", GOSSIP_SENDER_MAIN, 1600);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
	}
	if(action== 1501)
	{
		//Test
		//Query vorbereiten
		std::string q1("SELECT `guid` FROM `character` where `name` = '");
		q1 += player->GetName();
		q1 += "'";
		//ID des Players
		//sLog.outString(q1.c_str());
		QueryResult *result_ = CharacterDatabase.Query( q1.c_str() );
    		if(result_)
    		{
        		do
        		{
        			Field *fields = result_->Fetch();
				//Query
				std::string q2("SELECT `guildid` FROM `guild_member` where `guid` = '");
				q2 += fields[0].GetString();
				q2 += "'";
				//GildenID des Players
				QueryResult *result__ = CharacterDatabase.Query( q2.c_str() );
				if(result__)
				{
				Field *fieldss = result__->Fetch();
				if(fieldss[0].GetString()!=""){
					std::string q3("SELECT `map`,`x`,`y`,`z`,`r` FROM `guildmeister` where `id` = '");
					q3 += fieldss[0].GetString();
					q3 += "'";

					QueryResult *result___ = CharacterDatabase.Query( q3.c_str() );
					if(result___)
					{
					Field *fieldsss = result___->Fetch();
	 				player->TeleportTo(fieldsss[0].GetUInt32(), fieldsss[1].GetFloat(), fieldsss[2].GetFloat(), fieldsss[3].GetFloat(), fieldsss[4].GetFloat());
					} else {
						std::string ngh(player->GetName());
						ngh += " Deine Gilde hat noch kein Gildenhaus!";
						_Creature->MonsterSay(ngh.c_str(),LANG_COMMON,NULL);	
					}

				} 
				} else { 
					std::string ng(player->GetName());
					ng += " Du bist in keiner Gilde!";
					_Creature->MonsterSay(ng.c_str(),LANG_COMMON,NULL);	
				}
        		} while( result_->NextRow() );
        		delete result_;
    		}
	}
}
bool GossipSelect_paradox(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
// Main menu
	if (sender == GOSSIP_SENDER_MAIN)
		SendDefaultMenu_paradox(player, _Creature, action   );
		return true;
}
void AddSC_paradox()
{
    Script *newscript;
 
newscript = new Script;
newscript->Name="paradox";
newscript->pGossipHello = &GossipHello_paradox;
newscript->pGossipSelect = &GossipSelect_paradox;
newscript->pItemHello = NULL;
newscript->pGOHello = NULL;
newscript->pAreaTrigger = NULL;
newscript->pItemQuestAccept = NULL;
newscript->pGOQuestAccept = NULL;
newscript->pGOChooseReward = NULL;
newscript->RegisterSelf();
}
