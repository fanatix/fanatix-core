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
SDName: Feralas
SD%Complete: 100
SDComment: Quest support: 3520, 2767. Special vendor Gregan Brewspewer
SDCategory: Feralas
EndScriptData */

#include "precompiled.h"
#include "../../npc/npc_escortAI.h"

/*######
## npc_gregan_brewspewer
######*/

bool GossipHello_npc_gregan_brewspewer(Player *player, Creature *_Creature)
{
    if( _Creature->isQuestGiver() )
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if( _Creature->isVendor() && player->GetQuestStatus(3909) == QUEST_STATUS_INCOMPLETE )
        player->ADD_GOSSIP_ITEM(0, "Buy somethin', will ya?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    player->SEND_GOSSIP_MENU(2433,_Creature->GetGUID());
    return true;
}

bool GossipSelect_npc_gregan_brewspewer(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if( action == GOSSIP_ACTION_INFO_DEF+1 )
    {
        player->ADD_GOSSIP_ITEM(1, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
        player->SEND_GOSSIP_MENU(2434,_Creature->GetGUID());
    }
    if( action == GOSSIP_ACTION_TRADE )
        player->SEND_VENDORLIST( _Creature->GetGUID() );
    return true;
}

/*######
## npc_oox22fe
######*/

enum
{
    SAY_START               = -1000287,
    SAY_AGGRO               = -1000288,
    SAY_AGGRO2              = -1000289,
    SAY_AMBUSH              = -1000290,
    SAY_END                 = -1000292,

    NPC_YETI                = 7848,
    NPC_GORILLA             = 5260,
    NPC_WOODPAW_REAVER      = 5255,
    NPC_WOODPAW_BRUTE       = 5253,
    NPC_WOODPAW_ALPHA       = 5258,
    NPC_WOODPAW_MYSTIC      = 5254,

    QUEST_RESCUE_OOX22FE    = 2767,
    FACTION_ESCORTEE_A      = 774,
    FACTION_ESCORTEE_H      = 775
};

struct MANGOS_DLL_DECL npc_oox22feAI : public npc_escortAI
{
    npc_oox22feAI(Creature *c) : npc_escortAI(c)
    {
        normFaction = c->getFaction();
        Reset();
    }

    uint32 normFaction;

    void WaypointReached(uint32 i)
    {
        switch (i)
        {
            // First Ambush(3 Yetis)
            case 11:
                DoScriptText(SAY_AMBUSH,m_creature);
                m_creature->SummonCreature(NPC_YETI, -4841.01, 1593.91, 73.42, 3.98, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
                m_creature->SummonCreature(NPC_YETI, -4837.61, 1568.58, 78.21, 3.13, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
                m_creature->SummonCreature(NPC_YETI, -4841.89, 1569.95, 76.53, 0.68, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
                break;
            //Second Ambush(3 Gorillas)
            case 21:
                DoScriptText(SAY_AMBUSH,m_creature);
                m_creature->SummonCreature(NPC_GORILLA, -4595.81, 2005.99, 53.08, 3.74, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
                m_creature->SummonCreature(NPC_GORILLA, -4597.53, 2008.31, 52.70, 3.78, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
                m_creature->SummonCreature(NPC_GORILLA, -4599.37, 2010.59, 52.77, 3.84, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
                break;
            //Third Ambush(4 Gnolls)
            case 30:
                DoScriptText(SAY_AMBUSH,m_creature);
                m_creature->SummonCreature(NPC_WOODPAW_REAVER, -4425.14, 2075.87, 47.77, 3.77, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
                m_creature->SummonCreature(NPC_WOODPAW_BRUTE , -4426.68, 2077.98, 47.57, 3.77, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
                m_creature->SummonCreature(NPC_WOODPAW_MYSTIC, -4428.33, 2080.24, 47.43, 3.87, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
                m_creature->SummonCreature(NPC_WOODPAW_ALPHA , -4430.04, 2075.54, 46.83, 3.81, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
                break;
            case 37:
                DoScriptText(SAY_END,m_creature);
                // Award quest credit
                if (Unit* pUnit = Unit::GetUnit((*m_creature), PlayerGUID))
                {
                    if (pUnit->GetTypeId() == TYPEID_PLAYER)
                        ((Player*)pUnit)->GroupEventHappens(QUEST_RESCUE_OOX22FE,m_creature);
                }
                break;
        }
    }

    void Reset()
    {
        if (!IsBeingEscorted)
        {
            m_creature->setFaction(normFaction);
            m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
        }
    }

    void Aggro(Unit* who)
    {
        //For an small probability the npc says something when he get aggro
        switch(rand()%10)
        {
           case 0: DoScriptText(SAY_AGGRO,m_creature); break;
           case 1: DoScriptText(SAY_AGGRO2,m_creature); break;
        }
    }

    void JustSummoned(Creature* summoned)
    {
        summoned->AI()->AttackStart(m_creature);
    }

    void JustDied(Unit* killer)
    {
        if (!IsBeingEscorted)
            return;

        if (Unit* pUnit = Unit::GetUnit((*m_creature), PlayerGUID))
        {
            // If NPC dies, player fails the quest
            if (pUnit->GetTypeId() == TYPEID_PLAYER && ((Player*)pUnit)->GetQuestStatus(QUEST_RESCUE_OOX22FE) != QUEST_STATUS_COMPLETE)
                ((Player*)pUnit)->FailQuest(QUEST_RESCUE_OOX22FE);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        npc_escortAI::UpdateAI(diff);
    }
};

CreatureAI* GetAI_npc_oox22fe(Creature* pCreature)
{
    npc_oox22feAI* oox22AI = new npc_oox22feAI(pCreature);

    oox22AI->FillPointMovementListForCreature();

    return (CreatureAI*)oox22AI;
}

bool QuestAccept_npc_oox22fe(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_RESCUE_OOX22FE)
    {
        DoScriptText(SAY_START, pCreature);
        //change that the npc is not lying dead on the ground
        pCreature->SetStandState(UNIT_STAND_STATE_STAND);

        if (pPlayer->GetTeam() == ALLIANCE)
            pCreature->setFaction(FACTION_ESCORTEE_A);

        if (pPlayer->GetTeam() == HORDE)
            pCreature->setFaction(FACTION_ESCORTEE_H);

        ((npc_escortAI*)(pCreature->AI()))->Start(true, true, false, pPlayer->GetGUID());

    }
    return true;
}

/*######
## npc_screecher_spirit
######*/

bool GossipHello_npc_screecher_spirit(Player *player, Creature *_Creature)
{
    player->SEND_GOSSIP_MENU(2039,_Creature->GetGUID() );
    player->TalkedToCreature(_Creature->GetEntry(), _Creature->GetGUID());
    _Creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

    return true;
}

/*######
## AddSC
######*/

void AddSC_feralas()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_gregan_brewspewer";
    newscript->pGossipHello = &GossipHello_npc_gregan_brewspewer;
    newscript->pGossipSelect = &GossipSelect_npc_gregan_brewspewer;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_oox22fe";
    newscript->GetAI = &GetAI_npc_oox22fe;
    newscript->pQuestAccept = &QuestAccept_npc_oox22fe;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_screecher_spirit";
    newscript->pGossipHello = &GossipHello_npc_screecher_spirit;
    newscript->RegisterSelf();
}
