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
SDName: Westfall
SD%Complete: 90
SDComment: Quest support: 155, 1651
SDCategory: Westfall
EndScriptData */

/* ContentData
npc_daphne_stilwell
npc_defias_traitor
EndContentData */

#include "precompiled.h"
#include "../../npc/npc_escortAI.h"

/*######
## npc_daphne_stilwell
######*/

enum
{
    SAY_DS_START        = -1000293,
    SAY_DS_DOWN_1       = -1000294,
    SAY_DS_DOWN_2       = -1000295,
    SAY_DS_DOWN_3       = -1000296,
    SAY_DS_PROLOGUE     = -1000297,

    SPELL_SHOOT         = 6660,
    QUEST_TOME_VALOR    = 1651,
    NPC_DEFIAS_RAIDER   = 6180,
    EQUIP_ID_RIFLE      = 2511
};

struct MANGOS_DLL_DECL npc_daphne_stilwellAI : public npc_escortAI
{
    npc_daphne_stilwellAI(Creature *c) : npc_escortAI(c) { Reset(); }

    uint32 uiWPHolder;
    uint32 uiShootTimer;

    void Reset()
    {
        if (IsBeingEscorted)
        {
            switch(uiWPHolder)
            {
                case 7: DoScriptText(SAY_DS_DOWN_1, m_creature); break;
                case 8: DoScriptText(SAY_DS_DOWN_2, m_creature); break;
                case 9: DoScriptText(SAY_DS_DOWN_3, m_creature); break;
            }
        }
        else
            uiWPHolder = 0;

        uiShootTimer = 0;
    }

    void WaypointReached(uint32 uiPoint)
    {
        Unit* pUnit = Unit::GetUnit(*m_creature, PlayerGUID);

        if (!pUnit || pUnit->GetTypeId() != TYPEID_PLAYER)
            return;

        uiWPHolder = uiPoint;

        switch(uiPoint)
        {
            case 4:
                SetEquipmentSlots(false, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE, EQUIP_ID_RIFLE);
                SetSheathState(SHEATH_STATE_RANGED);
                m_creature->HandleEmoteCommand(EMOTE_STATE_USESTANDING_NOSHEATHE);
                break;
            case 6:
                SetCombatMovement(false);
                break;
            case 7:
                m_creature->SummonCreature(NPC_DEFIAS_RAIDER, -11450.836, 1569.755, 54.267, 4.230, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                m_creature->SummonCreature(NPC_DEFIAS_RAIDER, -11449.697, 1569.124, 54.421, 4.206, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                m_creature->SummonCreature(NPC_DEFIAS_RAIDER, -11448.237, 1568.307, 54.620, 4.206, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                break;
            case 8:
                m_creature->SummonCreature(NPC_DEFIAS_RAIDER, -11450.836, 1569.755, 54.267, 4.230, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                m_creature->SummonCreature(NPC_DEFIAS_RAIDER, -11449.697, 1569.124, 54.421, 4.206, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                m_creature->SummonCreature(NPC_DEFIAS_RAIDER, -11448.237, 1568.307, 54.620, 4.206, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                m_creature->SummonCreature(NPC_DEFIAS_RAIDER, -11448.037, 1570.213, 54.961, 4.283, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                break;
            case 9:
                m_creature->SummonCreature(NPC_DEFIAS_RAIDER, -11450.836, 1569.755, 54.267, 4.230, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                m_creature->SummonCreature(NPC_DEFIAS_RAIDER, -11449.697, 1569.124, 54.421, 4.206, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                m_creature->SummonCreature(NPC_DEFIAS_RAIDER, -11448.237, 1568.307, 54.620, 4.206, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                m_creature->SummonCreature(NPC_DEFIAS_RAIDER, -11448.037, 1570.213, 54.961, 4.283, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                m_creature->SummonCreature(NPC_DEFIAS_RAIDER, -11449.018, 1570.738, 54.828, 4.220, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                break;
            case 10:
                SetRun(false);
                break;
            case 11:
                DoScriptText(SAY_DS_PROLOGUE, m_creature);
                break;
            case 13:
                SetSheathState(SHEATH_STATE_UNARMED);
                m_creature->HandleEmoteCommand(EMOTE_STATE_USESTANDING_NOSHEATHE);
                SetEquipmentSlots(true);
                break;
            case 17:
                ((Player*)pUnit)->GroupEventHappens(QUEST_TOME_VALOR, m_creature);
                break;
        }
    }

    void Aggro(Unit* who) { }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->AI()->AttackStart(m_creature);
    }

    void JustDied(Unit* killer)
    {
        if (Unit* pUnit = Unit::GetUnit(*m_creature, PlayerGUID))
        {
            if (pUnit->GetTypeId() == TYPEID_PLAYER)
            {
                if (((Player*)pUnit)->GetQuestStatus(QUEST_TOME_VALOR) == QUEST_STATUS_INCOMPLETE)
                    ((Player*)pUnit)->FailQuest(QUEST_TOME_VALOR);
            }
        }
    }

    void Update(const uint32 diff)
    {
        npc_escortAI::UpdateAI(diff);

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (uiShootTimer < diff)
        {
            if (m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
                SetCombatMovement(true);
            else
                SetCombatMovement(false);

            uiShootTimer = 1500;

            DoCast(m_creature->getVictim(), SPELL_SHOOT);
        }else uiShootTimer -= diff;
    }
};

bool QuestAccept_npc_daphne_stilwell(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_TOME_VALOR)
    {
        DoScriptText(SAY_DS_START, pCreature);
        ((npc_escortAI*)(pCreature->AI()))->Start(true, true, true, pPlayer->GetGUID());
    }

    return true;
}

CreatureAI* GetAI_npc_daphne_stilwell(Creature* pCreature)
{
    npc_daphne_stilwellAI* thisAI = new npc_daphne_stilwellAI(pCreature);

    thisAI->FillPointMovementListForCreature();

    return (CreatureAI*)thisAI;
}

/*######
## npc_defias_traitor
######*/

#define SAY_START                   -1000101
#define SAY_PROGRESS                -1000102
#define SAY_END                     -1000103
#define SAY_AGGRO_1                 -1000104
#define SAY_AGGRO_2                 -1000105

#define QUEST_DEFIAS_BROTHERHOOD    155

struct MANGOS_DLL_DECL npc_defias_traitorAI : public npc_escortAI
{
    npc_defias_traitorAI(Creature *c) : npc_escortAI(c) { Reset(); }

    void WaypointReached(uint32 i)
    {
        Unit* player = Unit::GetUnit((*m_creature), PlayerGUID);

        if (!player || player->GetTypeId() != TYPEID_PLAYER)
            return;

        switch (i)
        {
            case 35:
                SetRun(false);
                break;
            case 36:
                DoScriptText(SAY_PROGRESS, m_creature, player);
                break;
            case 44:
                DoScriptText(SAY_END, m_creature, player);
                ((Player*)player)->GroupEventHappens(QUEST_DEFIAS_BROTHERHOOD,m_creature);
                break;
        }
    }

    void Aggro(Unit* who)
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_AGGRO_1, m_creature, who); break;
            case 1: DoScriptText(SAY_AGGRO_2, m_creature, who); break;
        }
    }

    void Reset() { }

    void JustDied(Unit* killer)
    {
        if (PlayerGUID)
        {
            if (Unit* player = Unit::GetUnit((*m_creature), PlayerGUID))
                ((Player*)player)->FailQuest(QUEST_DEFIAS_BROTHERHOOD);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        npc_escortAI::UpdateAI(diff);
    }
};

bool QuestAccept_npc_defias_traitor(Player* player, Creature* creature, Quest const* quest)
{
    if (quest->GetQuestId() == QUEST_DEFIAS_BROTHERHOOD)
    {
        ((npc_escortAI*)(creature->AI()))->Start(true, true, true, player->GetGUID());
        DoScriptText(SAY_START, creature, player);
    }

    return true;
}

CreatureAI* GetAI_npc_defias_traitor(Creature* pCreature)
{
    npc_defias_traitorAI* thisAI = new npc_defias_traitorAI(pCreature);

    thisAI->FillPointMovementListForCreature();

    return (CreatureAI*)thisAI;
}

void AddSC_westfall()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_daphne_stilwell";
    newscript->GetAI = &GetAI_npc_daphne_stilwell;
    newscript->pQuestAccept = &QuestAccept_npc_daphne_stilwell;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_defias_traitor";
    newscript->GetAI = &GetAI_npc_defias_traitor;
    newscript->pQuestAccept = &QuestAccept_npc_defias_traitor;
    newscript->RegisterSelf();
}
