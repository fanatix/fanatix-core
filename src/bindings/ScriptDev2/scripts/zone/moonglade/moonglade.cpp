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

/* ScriptData
SDName: Moonglade
SD%Complete: 100
SDComment: Quest support: 30, 272, 5929, 5930, 10965. Special Flight Paths for Druid class.
SDCategory: Moonglade
EndScriptData */

/* ContentData
npc_bunthen_plainswind
npc_clintar_dw_spirit
npc_great_bear_spirit
npc_silva_filnaveth
EndContentData */

#include "precompiled.h"
#include "../../npc/npc_escortAI.h"
#include "ObjectMgr.h"

/*######
## npc_bunthen_plainswind
######*/

bool GossipHello_npc_bunthen_plainswind(Player *player, Creature *_Creature)
{
    if(player->getClass() != CLASS_DRUID)
        player->SEND_GOSSIP_MENU(4916,_Creature->GetGUID());
    else if(player->GetTeam() != HORDE)
    {
        if(player->GetQuestStatus(272) == QUEST_STATUS_INCOMPLETE)
            player->ADD_GOSSIP_ITEM( 0, "Do you know where I can find Half Pendant of Aquatic Endurance?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

        player->SEND_GOSSIP_MENU(4917,_Creature->GetGUID());
    }
    else if(player->getClass() == CLASS_DRUID && player->GetTeam() == HORDE)
    {
        player->ADD_GOSSIP_ITEM( 0, "I'd like to fly to Thunder Bluff.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        if(player->GetQuestStatus(30) == QUEST_STATUS_INCOMPLETE)
            player->ADD_GOSSIP_ITEM( 0, "Do you know where I can find Half Pendant of Aquatic Endurance?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

        player->SEND_GOSSIP_MENU(4918,_Creature->GetGUID());
    }
    return true;
}

bool GossipSelect_npc_bunthen_plainswind(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch(action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
        {
            player->CLOSE_GOSSIP_MENU();
            if (player->getClass() == CLASS_DRUID && player->GetTeam() == HORDE)
            {
                std::vector<uint32> nodes;

                nodes.resize(2);
                nodes[0] = 63;                              // Nighthaven, Moonglade
                nodes[1] = 22;                              // Thunder Bluff, Mulgore
                player->ActivateTaxiPathTo(nodes);
            }
            break;
        }
        case GOSSIP_ACTION_INFO_DEF + 2:
            player->SEND_GOSSIP_MENU(5373,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            player->SEND_GOSSIP_MENU(5376,_Creature->GetGUID());
            break;
    }
    return true;
}

/*####
# npc_clintar_dw_spirit
####*/

enum
{
    SAY_START               = -1000280,
    SAY_AGGRO_1             = -1000281,
    SAY_AGGRO_2             = -1000282,
    SAY_RELIC1              = -1000283,
    SAY_RELIC2              = -1000284,
    SAY_RELIC3              = -1000285,
    SAY_END                 = -1000286,

    QUEST_MERE_DREAM        = 10965,
    SPELL_EMERALD_DREAM     = 39601,
    NPC_CLINTAR_DW_SPIRIT   = 22916,
    NPC_CLINTAR_SPIRIT      = 22901,
    NPC_ASPECT_OF_RAVEN     = 22915,
};

struct MANGOS_DLL_DECL npc_clintar_dw_spiritAI : public npc_escortAI
{
    npc_clintar_dw_spiritAI(Creature *c) : npc_escortAI(c) { Reset(); }

    void WaypointReached(uint32 i)
    {
        Unit* pUnit = Unit::GetUnit(*m_creature, PlayerGUID);

        if (!pUnit || pUnit->GetTypeId() != TYPEID_PLAYER)
            return;

        //visual details here probably need refinement
        switch(i)
        {
            case 0:
                DoScriptText(SAY_START, m_creature, pUnit);
                break;
            case 13:
                m_creature->HandleEmoteCommand(EMOTE_STATE_USESTANDING_NOSHEATHE);
                break;
            case 14:
                DoScriptText(SAY_RELIC1, m_creature, pUnit);
                break;
            case 26:
                m_creature->HandleEmoteCommand(EMOTE_STATE_USESTANDING_NOSHEATHE);
                break;
            case 27:
                DoScriptText(SAY_RELIC2, m_creature, pUnit);
                break;
            case 31:
                m_creature->SummonCreature(NPC_ASPECT_OF_RAVEN, 7465.321, -3088.515, 429.006, 5.550, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                break;
            case 35:
                m_creature->HandleEmoteCommand(EMOTE_STATE_USESTANDING_NOSHEATHE);
                break;
            case 36:
                DoScriptText(SAY_RELIC3, m_creature, pUnit);
                break;
            case 49:
                DoScriptText(SAY_END, m_creature, pUnit);
                ((Player*)pUnit)->TalkedToCreature(m_creature->GetEntry(),m_creature->GetGUID());
                break;
        }
    }

    void Aggro(Unit* who)
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_AGGRO_1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO_2, m_creature); break;
        }
    }

    void Reset()
    {
        if (IsBeingEscorted)
            return;

        //m_creature are expected to always be spawned, but not visible for player
        //spell casted from quest_template.SrcSpell require this to be this way
        //we handle the triggered spell to get a "hook" to our guy so he can be escorted on quest accept

        if (CreatureInfo const* pTemp = GetCreatureTemplateStore(m_creature->GetEntry()))
            m_creature->SetDisplayId(pTemp->DisplayID_H);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetVisibility(VISIBILITY_OFF);
    }

    //called only from EffectDummy
    void DoStart(uint64 uiPlayerGuid)
    {
        //not the best way, maybe check in DummyEffect if this creature are "free" and not in escort.
        if (IsBeingEscorted)
            return;

        m_creature->SetVisibility(VISIBILITY_ON);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        Start(true,true,false,uiPlayerGuid);
    }

    void JustSummoned(Creature* summoned)
    {
        summoned->AI()->AttackStart(m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        npc_escortAI::UpdateAI(diff);
    }
};

CreatureAI* GetAI_npc_clintar_dw_spirit(Creature* pCreature)
{
    npc_clintar_dw_spiritAI* tempAI = new npc_clintar_dw_spiritAI(pCreature);

    tempAI->FillPointMovementListForCreature();

    return (CreatureAI*)tempAI;
}

//we expect this spell to be triggered from spell casted at questAccept
bool EffectDummyCreature_npc_clintar_dw_spirit(Unit *pCaster, uint32 spellId, uint32 effIndex, Creature *pCreatureTarget)
{
    //always check spellid and effectindex
    if (spellId == SPELL_EMERALD_DREAM && effIndex == 0)
    {
        if (pCaster->GetTypeId() != TYPEID_PLAYER || pCaster->HasAura(SPELL_EMERALD_DREAM))
            return true;

        if (pCreatureTarget->GetEntry() != NPC_CLINTAR_DW_SPIRIT)
            return true;

        if (CreatureInfo const* pTemp = GetCreatureTemplateStore(NPC_CLINTAR_SPIRIT))
            pCreatureTarget->SetDisplayId(pTemp->DisplayID_H);
        else
            return true;

        //done here, escort can start
        ((npc_clintar_dw_spiritAI*)pCreatureTarget->AI())->DoStart(pCaster->GetGUID());

        //always return true when we are handling this spell and effect
        return true;
    }
    return true;
}

/*######
## npc_great_bear_spirit
######*/

#define GOSSIP_BEAR1 "What do you represent, spirit?"
#define GOSSIP_BEAR2 "I seek to understand the importance of strength of the body."
#define GOSSIP_BEAR3 "I seek to understand the importance of strength of the heart."
#define GOSSIP_BEAR4 "I have heard your words, Great Bear Spirit, and I understand. I now seek your blessings to fully learn the way of the Claw."

bool GossipHello_npc_great_bear_spirit(Player *player, Creature *_Creature)
{
    //ally or horde quest
    if (player->GetQuestStatus(5929) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(5930) == QUEST_STATUS_INCOMPLETE)
    {
        player->ADD_GOSSIP_ITEM( 0, GOSSIP_BEAR1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        player->SEND_GOSSIP_MENU(4719, _Creature->GetGUID());
    }
    else
        player->SEND_GOSSIP_MENU(4718, _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_great_bear_spirit(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_BEAR2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(4721, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 1:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_BEAR3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(4733, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            player->ADD_GOSSIP_ITEM( 0, GOSSIP_BEAR4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(4734, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            player->SEND_GOSSIP_MENU(4735, _Creature->GetGUID());
            if (player->GetQuestStatus(5929)==QUEST_STATUS_INCOMPLETE)
                player->AreaExploredOrEventHappens(5929);
            if (player->GetQuestStatus(5930)==QUEST_STATUS_INCOMPLETE)
                player->AreaExploredOrEventHappens(5930);
            break;
    }
    return true;
}

/*######
## npc_silva_filnaveth
######*/

bool GossipHello_npc_silva_filnaveth(Player *player, Creature *_Creature)
{
    if(player->getClass() != CLASS_DRUID)
        player->SEND_GOSSIP_MENU(4913,_Creature->GetGUID());
    else if(player->GetTeam() != ALLIANCE)
    {
        if(player->GetQuestStatus(30) == QUEST_STATUS_INCOMPLETE)
            player->ADD_GOSSIP_ITEM( 0, "Do you know where I can find Half Pendant of Aquatic Agility?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

        player->SEND_GOSSIP_MENU(4915,_Creature->GetGUID());
    }
    else if(player->getClass() == CLASS_DRUID && player->GetTeam() == ALLIANCE)
    {
        player->ADD_GOSSIP_ITEM( 0, "I'd like to fly to Rut'theran Village.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        if(player->GetQuestStatus(272) == QUEST_STATUS_INCOMPLETE)
            player->ADD_GOSSIP_ITEM( 0, "Do you know where I can find Half Pendant of Aquatic Agility?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

        player->SEND_GOSSIP_MENU(4914,_Creature->GetGUID());
    }
    return true;
}

bool GossipSelect_npc_silva_filnaveth(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch(action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
        {
            player->CLOSE_GOSSIP_MENU();
            if (player->getClass() == CLASS_DRUID && player->GetTeam() == ALLIANCE)
            {
                std::vector<uint32> nodes;

                nodes.resize(2);
                nodes[0] = 62;                              // Nighthaven, Moonglade
                nodes[1] = 27;                              // Rut'theran Village, Teldrassil
                player->ActivateTaxiPathTo(nodes);
            }
            break;
        }
        case GOSSIP_ACTION_INFO_DEF + 2:
            player->SEND_GOSSIP_MENU(5374,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            player->SEND_GOSSIP_MENU(5375,_Creature->GetGUID());
            break;
    }
    return true;
}

/*######
##
######*/

void AddSC_moonglade()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_bunthen_plainswind";
    newscript->pGossipHello =  &GossipHello_npc_bunthen_plainswind;
    newscript->pGossipSelect = &GossipSelect_npc_bunthen_plainswind;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_clintar_dw_spirit";
    newscript->GetAI = &GetAI_npc_clintar_dw_spirit;
    newscript->pEffectDummyCreature = &EffectDummyCreature_npc_clintar_dw_spirit;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_great_bear_spirit";
    newscript->pGossipHello =  &GossipHello_npc_great_bear_spirit;
    newscript->pGossipSelect = &GossipSelect_npc_great_bear_spirit;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_silva_filnaveth";
    newscript->pGossipHello =  &GossipHello_npc_silva_filnaveth;
    newscript->pGossipSelect = &GossipSelect_npc_silva_filnaveth;
    newscript->RegisterSelf();
}
