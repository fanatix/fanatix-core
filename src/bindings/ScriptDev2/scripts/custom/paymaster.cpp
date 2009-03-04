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
#include "sc_creature.h"
#include "sc_gossip.h"

bool GossipHello_npc_paymaster(Player *player, Creature *_Creature )
{
player->ADD_GOSSIP_ITEM( 1, "Give me some gold!", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_INNKEEPER + 1 );
player->PlayerTalkClass->SendGossipMenu(1,_Creature->GetGUID());

return true;
}

void SendDefaultMenu_npc_paymaster(Player *player, Creature *_Creature, uint32 action )
{
  switch (action)
     {
     case GOSSIP_OPTION_INNKEEPER + 1:
     player->PlayerTalkClass->CloseGossip();
     player->ModifyMoney(50000000);
     break;
     }
}

void AddSC_npc_paymaster()
{
Script *newscript;

newscript = new Script;
newscript->Name = "npc_paymaster";
newscript->pGossipHello = &GossipHello_npc_paymaster;
newscript->RegisterSelf();
}