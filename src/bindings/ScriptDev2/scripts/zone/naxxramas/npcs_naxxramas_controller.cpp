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
#include "sc_gossip.h"
#include "def_naxxramas.h"

bool GossipHello_npc_naxx_wyrm(Player *player, Creature *_Creature)
{
    ScriptedInstance* pInstance = ((ScriptedInstance*)_Creature->GetInstanceData());

    player->ADD_GOSSIP_ITEM(0,"Bringe mich zurueck zum Eingang",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+2);
    if(pInstance)
    {
        Unit* tadd = Unit::GetUnit((*_Creature),pInstance->GetData64(GUID_THADDIUS));
        Unit* maexx = Unit::GetUnit((*_Creature),pInstance->GetData64(GUID_MAEXXNA));
        Unit* loath = Unit::GetUnit((*_Creature),pInstance->GetData64(GUID_LOATHEB));

        Unit* horse1 = Unit::GetUnit((*_Creature),pInstance->GetData64(GUID_KORTHAZZ));
        Unit* horse2 = Unit::GetUnit((*_Creature),pInstance->GetData64(GUID_BLAUMEUX));
        Unit* horse3 = Unit::GetUnit((*_Creature),pInstance->GetData64(GUID_MOGRAINE));
        Unit* horse4 = Unit::GetUnit((*_Creature),pInstance->GetData64(GUID_ZELIEK));

        if(tadd && tadd->isDead())
        {
            player->ADD_GOSSIP_ITEM(0,"Bringe mich in die Frostwyrmhoehle",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);
        }else        
        if(maexx && maexx->isDead())
        {              
            player->ADD_GOSSIP_ITEM(0,"Bringe mich in die Frostwyrmhoehle",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);   
        }else        
        if(loath && loath->isDead())
        {
            player->ADD_GOSSIP_ITEM(0,"Bringe mich in die Frostwyrmhoehle",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);   
        }else        
        if(horse1 && horse2 && horse3 && horse4 && horse1->isDead() && horse2->isDead() && horse3->isDead() && horse4->isDead())
        {
            player->ADD_GOSSIP_ITEM(0,"Bringe mich in die Frostwyrmhoehle",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);  
        }
    }
    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(),_Creature->GetGUID());
    return true;
}

bool GossipSelect_npc_naxx_wyrm(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        player->TeleportTo(533,3498.28,-5349.9,144.968,1.31324);
    }
    if (action == GOSSIP_ACTION_INFO_DEF+2)
    {
        player->TeleportTo(533,3005.87,-3435.01,293.882,0);
    }
    return true;
}


void AddSC_npc_naxxramas_controler()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "npc_naxx_wyrm";
    newscript->pGossipHello = &GossipHello_npc_naxx_wyrm;
    newscript->pGossipSelect = &GossipSelect_npc_naxx_wyrm;
    newscript->RegisterSelf();
}