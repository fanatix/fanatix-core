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
SDName: Azuremyst_Isle
SD%Complete: 75
SDComment: Quest support: 9283, 9303, 9537, 9582, 9554(special flight path, proper model for mount missing). Injured Draenei cosmetic only
SDCategory: Azuremyst Isle
EndScriptData */

/* ContentData
npc_draenei_survivor
npc_engineer_spark_overgrind
npc_injured_draenei
npc_magwin
npc_susurrus
mob_nestlewood_owlkin
EndContentData */

#include "precompiled.h"
#include "../../npc/npc_escortAI.h"
#include <cmath>

/*######
## npc_draenei_survivor
######*/

#define SAY_HEAL1       -1000176
#define SAY_HEAL2       -1000177
#define SAY_HEAL3       -1000178
#define SAY_HEAL4       -1000179
#define SAY_HELP1       -1000180
#define SAY_HELP2       -1000181
#define SAY_HELP3       -1000182
#define SAY_HELP4       -1000183

#define SPELL_IRRIDATION    35046
#define SPELL_STUNNED       28630

struct MANGOS_DLL_DECL npc_draenei_survivorAI : public ScriptedAI
{
    npc_draenei_survivorAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint64 pCaster;

    uint32 SayThanksTimer;
    uint32 RunAwayTimer;
    uint32 SayHelpTimer;

    bool CanSayHelp;

    void Reset()
    {
        pCaster = 0;

        SayThanksTimer = 0;
        RunAwayTimer = 0;
        SayHelpTimer = 10000;

        CanSayHelp = true;

        m_creature->CastSpell(m_creature, SPELL_IRRIDATION, true);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
        m_creature->SetHealth(int(m_creature->GetMaxHealth()*.1));
        m_creature->SetStandState(UNIT_STAND_STATE_SLEEP);
    }

    void Aggro(Unit *who) {}

    void MoveInLineOfSight(Unit *who)
    {
        if (CanSayHelp && who->GetTypeId() == TYPEID_PLAYER && m_creature->IsFriendlyTo(who) && m_creature->IsWithinDistInMap(who, 25.0f))
        {
            //Random switch between 4 texts
            switch (rand()%4)
            {
                case 0: DoScriptText(SAY_HELP1, m_creature, who); break;
                case 1: DoScriptText(SAY_HELP2, m_creature, who); break;
                case 2: DoScriptText(SAY_HELP3, m_creature, who); break;
                case 3: DoScriptText(SAY_HELP4, m_creature, who); break;
            }

            SayHelpTimer = 20000;
            CanSayHelp = false;
        }
    }

    void SpellHit(Unit *Caster, const SpellEntry *Spell)
    {
        if (Spell->SpellFamilyFlags2 & 0x080000000)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
            m_creature->SetStandState(UNIT_STAND_STATE_STAND);

            m_creature->CastSpell(m_creature, SPELL_STUNNED, true);

            pCaster = Caster->GetGUID();

            SayThanksTimer = 5000;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (SayThanksTimer)
        {
            if (SayThanksTimer <= diff)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_IRRIDATION);

                if (Player *pPlayer = (Player*)Unit::GetUnit(*m_creature,pCaster))
                {
                    if (pPlayer->GetTypeId() != TYPEID_PLAYER)
                        return;

                    switch (rand()%4)
                    {
                        case 0: DoScriptText(SAY_HEAL1, m_creature, pPlayer); break;
                        case 1: DoScriptText(SAY_HEAL2, m_creature, pPlayer); break;
                        case 2: DoScriptText(SAY_HEAL3, m_creature, pPlayer); break;
                        case 3: DoScriptText(SAY_HEAL4, m_creature, pPlayer); break;
                    }

                    pPlayer->TalkedToCreature(m_creature->GetEntry(),m_creature->GetGUID());
                }

                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MovePoint(0, -4115.053711f, -13754.831055f, 73.508949f);

                RunAwayTimer = 10000;
                SayThanksTimer = 0;
            }else SayThanksTimer -= diff;

            return;
        }

        if (RunAwayTimer)
        {
            if (RunAwayTimer <= diff)
            {
                m_creature->RemoveAllAuras();
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveIdle();
                m_creature->setDeathState(JUST_DIED);
                m_creature->SetHealth(0);
                m_creature->CombatStop();
                m_creature->DeleteThreatList();
                m_creature->RemoveCorpse();
            }else RunAwayTimer -= diff;

            return;
        }

        if (SayHelpTimer < diff)
        {
            CanSayHelp = true;
            SayHelpTimer = 20000;
        }else SayHelpTimer -= diff;
    }
};

CreatureAI* GetAI_npc_draenei_survivor(Creature *_Creature)
{
    return new npc_draenei_survivorAI (_Creature);
}

/*######
## npc_engineer_spark_overgrind
######*/

enum
{
    SAY_TEXT                = -1000184,
    EMOTE_SHELL             = -1000185,
    SAY_ATTACK              = -1000186,

    AREA_COVE               = 3579,
    AREA_ISLE               = 3639,
    QUEST_GNOMERCY          = 9537,
    FACTION_HOSTILE         = 14,
    SPELL_DYNAMITE          = 7978
};

#define GOSSIP_FIGHT        "Traitor! You will be brought to justice!"

struct MANGOS_DLL_DECL npc_engineer_spark_overgrindAI : public ScriptedAI
{
    npc_engineer_spark_overgrindAI(Creature *c) : ScriptedAI(c)
    {
        uiNormFaction = c->getFaction();
        uiNpcFlags = c->GetUInt32Value(UNIT_NPC_FLAGS);
        Reset();

        if (c->GetAreaId() == AREA_COVE || c->GetAreaId() == AREA_ISLE)
            bIsTreeEvent = true;
    }

    uint32 uiNpcFlags;
    uint32 uiNormFaction;

    uint32 uiDynamiteTimer;
    uint32 uiEmoteTimer;

    bool bIsTreeEvent;

    void Reset()
    {
        m_creature->setFaction(uiNormFaction);
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, uiNpcFlags);

        uiDynamiteTimer = 8000;
        uiEmoteTimer = 120000 + rand()%30000;

        bIsTreeEvent = false;
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_ATTACK, m_creature, who);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!InCombat && !bIsTreeEvent)
        {
            if (uiEmoteTimer < diff)
            {
                DoScriptText(SAY_TEXT, m_creature);
                DoScriptText(EMOTE_SHELL, m_creature);
                uiEmoteTimer = 120000 + rand()%30000;
            }
            else uiEmoteTimer -= diff;
        }
        else if (bIsTreeEvent)
        {
            //nothing here yet
            return;
        }

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (uiDynamiteTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_DYNAMITE);
            uiDynamiteTimer = 8000;
        }
        else uiDynamiteTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_engineer_spark_overgrind(Creature* pCreature)
{
    return new npc_engineer_spark_overgrindAI(pCreature);
}

bool GossipHello_npc_engineer_spark_overgrind(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_GNOMERCY) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(0, GOSSIP_FIGHT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_engineer_spark_overgrind(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction )
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->setFaction(FACTION_HOSTILE);
        ((npc_engineer_spark_overgrindAI*)pCreature->AI())->AttackStart(pPlayer);
    }
    return true;
}

/*######
## npc_injured_draenei
######*/

struct MANGOS_DLL_DECL npc_injured_draeneiAI : public ScriptedAI
{
    npc_injured_draeneiAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
        m_creature->SetHealth(int(m_creature->GetMaxHealth()*.15));
        switch (rand()%2)
        {
            case 0: m_creature->SetStandState(UNIT_STAND_STATE_SIT); break;
            case 1: m_creature->SetStandState(UNIT_STAND_STATE_SLEEP); break;
        }
    }

    void Aggro(Unit *who) {}

    void MoveInLineOfSight(Unit *who)
    {
        return;                                             //ignore everyone around them (won't aggro anything)
    }

    void UpdateAI(const uint32 diff)
    {
        return;
    }

};
CreatureAI* GetAI_npc_injured_draenei(Creature *_Creature)
{
    return new npc_injured_draeneiAI (_Creature);
}

/*######
## npc_magwin
######*/

#define SAY_START               -1000111
#define SAY_AGGRO               -1000112
#define SAY_PROGRESS            -1000113
#define SAY_END1                -1000114
#define SAY_END2                -1000115
#define EMOTE_HUG               -1000116

#define QUEST_A_CRY_FOR_HELP    9528

struct MANGOS_DLL_DECL npc_magwinAI : public npc_escortAI
{
    npc_magwinAI(Creature *c) : npc_escortAI(c) {Reset();}

    void WaypointReached(uint32 i)
    {
        Unit* player = Unit::GetUnit((*m_creature), PlayerGUID);

        if (!player)
            return;

        switch(i)
        {
            case 0:
                DoScriptText(SAY_START, m_creature, player);
                break;
            case 17: 
                DoScriptText(SAY_PROGRESS, m_creature, player);
                break;
            case 28:
                DoScriptText(SAY_END1, m_creature, player);
                break;
            case 29:
                DoScriptText(EMOTE_HUG, m_creature, player);
                DoScriptText(SAY_END2, m_creature, player);
                if (player && player->GetTypeId() == TYPEID_PLAYER)
                    ((Player*)player)->GroupEventHappens(QUEST_A_CRY_FOR_HELP,m_creature);
                break;
        }
    }

    void Aggro(Unit* who)
    {
        DoScriptText(SAY_AGGRO, m_creature, who);
    }

    void Reset()
    {
        if (!IsBeingEscorted)
            m_creature->setFaction(80);
    }

    void JustDied(Unit* killer)
    {
        if (PlayerGUID)
        {
            Unit* player = Unit::GetUnit((*m_creature), PlayerGUID);
            if (player)
                ((Player*)player)->FailQuest(QUEST_A_CRY_FOR_HELP);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        npc_escortAI::UpdateAI(diff);
    }
};

bool QuestAccept_npc_magwin(Player* player, Creature* creature, Quest const* quest)
{
    if (quest->GetQuestId() == QUEST_A_CRY_FOR_HELP)
    {
        creature->setFaction(10);
        ((npc_escortAI*)(creature->AI()))->Start(true, true, false, player->GetGUID());
    }
    return true;
}

CreatureAI* GetAI_npc_magwinAI(Creature* pCreature)
{
    npc_magwinAI* magwinAI = new npc_magwinAI(pCreature);

    magwinAI->FillPointMovementListForCreature();

    return (CreatureAI*)magwinAI;
}

/*######
## npc_nestlewood_owlkin
######*/

enum
{
    SPELL_INOCULATE_OWLKIN  = 29528,
    ENTRY_OWLKIN            = 16518,
    ENTRY_OWLKIN_INOC       = 16534,
};

struct MANGOS_DLL_DECL npc_nestlewood_owlkinAI : public ScriptedAI
{
    npc_nestlewood_owlkinAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 DespawnTimer;

    void Reset()
    {
        DespawnTimer = 0;
        m_creature->SetVisibility(VISIBILITY_ON);
    }

    void Aggro(Unit *who) {}

    void UpdateAI(const uint32 diff)
    {
        //timer gets adjusted by the triggered aura effect
        if (DespawnTimer)
        {
            if (DespawnTimer <= diff)
            {
                //once we are able to, despawn us
                m_creature->SetVisibility(VISIBILITY_OFF);
                m_creature->setDeathState(JUST_DIED);
                m_creature->SetHealth(0);
                m_creature->CombatStop();
                m_creature->DeleteThreatList();
                m_creature->RemoveCorpse();
            }else DespawnTimer -= diff;
        }

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_nestlewood_owlkin(Creature* pCreature)
{
    return new npc_nestlewood_owlkinAI(pCreature);
}

bool EffectDummyCreature_npc_nestlewood_owlkin(Unit *pCaster, uint32 spellId, uint32 effIndex, Creature *pCreatureTarget)
{
    //always check spellid and effectindex
    if (spellId == SPELL_INOCULATE_OWLKIN && effIndex == 0)
    {
        if (pCreatureTarget->GetEntry() != ENTRY_OWLKIN)
            return true;

        pCreatureTarget->UpdateEntry(ENTRY_OWLKIN_INOC);

        //set despawn timer, since we want to remove creature after a short time
        ((npc_nestlewood_owlkinAI*)pCreatureTarget->AI())->DespawnTimer = 15000;

        //always return true when we are handling this spell and effect
        return true;
    }
    return false;
}

/*######
## npc_susurrus
######*/

bool GossipHello_npc_susurrus(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (player->HasItemCount(23843,1,true))
        player->ADD_GOSSIP_ITEM(0, "I am ready.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_susurrus(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF)
    {
        player->CLOSE_GOSSIP_MENU();
        player->CastSpell(player,32474,true);               //apparently correct spell, possible not correct place to cast, or correct caster

        std::vector<uint32> nodes;

        nodes.resize(2);
        nodes[0] = 92;                                      //from susurrus
        nodes[1] = 91;                                      //end at exodar
        player->ActivateTaxiPathTo(nodes,11686);            //TaxiPath 506. Using invisible model, possible mangos must allow 0(from dbc) for cases like this.
    }
    return true;
}

/*######
## mob_nestlewood_owlkin
######*/

#define INOCULATION_CHANNEL 29528
#define INOCULATED_OWLKIN   16534

struct MANGOS_DLL_DECL mob_nestlewood_owlkinAI : public ScriptedAI
{
    mob_nestlewood_owlkinAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 ChannelTimer;
    bool Channeled;
    bool Hitted;

    void Reset()
    {
        ChannelTimer = 0;
        Channeled = false;
        Hitted = false;
    }

    void Aggro(Unit *who){}

    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        if(!caster)
            return;

        if(caster->GetTypeId() == TYPEID_PLAYER && spell->Id == INOCULATION_CHANNEL)
        {
            ChannelTimer = 3000;
            Hitted = true;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(ChannelTimer < diff && !Channeled && Hitted)
        {
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            m_creature->RemoveCorpse();
            m_creature->SummonCreature(INOCULATED_OWLKIN, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 180000);
            Channeled = true;
        }else ChannelTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_nestlewood_owlkinAI(Creature *_Creature)
{
    return new mob_nestlewood_owlkinAI (_Creature);
}

void AddSC_azuremyst_isle()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_draenei_survivor";
    newscript->GetAI = &GetAI_npc_draenei_survivor;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_engineer_spark_overgrind";
    newscript->GetAI = &GetAI_npc_engineer_spark_overgrind;
    newscript->pGossipHello =  &GossipHello_npc_engineer_spark_overgrind;
    newscript->pGossipSelect = &GossipSelect_npc_engineer_spark_overgrind;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_injured_draenei";
    newscript->GetAI = &GetAI_npc_injured_draenei;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_magwin";
    newscript->GetAI = &GetAI_npc_magwinAI;
    newscript->pQuestAccept = &QuestAccept_npc_magwin;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_nestlewood_owlkin";
    newscript->GetAI = &GetAI_npc_nestlewood_owlkin;
    newscript->pEffectDummyCreature = &EffectDummyCreature_npc_nestlewood_owlkin;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_susurrus";
    newscript->pGossipHello =  &GossipHello_npc_susurrus;
    newscript->pGossipSelect = &GossipSelect_npc_susurrus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_nestlewood_owlkin";
    newscript->GetAI = &GetAI_mob_nestlewood_owlkinAI;
    newscript->RegisterSelf();
}
