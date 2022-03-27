#include "DBCStores.h"
#include "Define.h"
#include "GossipDef.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "Language.h"
#include "NpcProfessionMgr.h"
#include "Translate.h"

#define GetText(a, b, c)    a->GetSession()->GetSessionDbLocaleIndex() == LOCALE_ruRU ? b : c

uint8 sProfession::CountPlayerCanLearn(const Player *player) {
    return 2;
}

void sProfession::CompleteQuest(Player* player)
{
    /* custom quest */
    if(player->GetQuestStatus(26035) == QUEST_STATUS_INCOMPLETE)
        player->KilledMonsterCredit(26035, 0);
    else if (player->GetQuestStatus(26036) == QUEST_STATUS_INCOMPLETE)
        player->KilledMonsterCredit(26036, 0);
}

bool sProfession::PlayerAlreadyHasMaxProfessions(const Player *player)
{
    uint8 skillCount = 0;
    for (const auto& i: _PROF)
        if(player->HasSkill(i) && !IsSecondarySkill(i))
            skillCount++;
    return (CountPlayerCanLearn(player) > skillCount) ? false : true;
}

bool sProfession::LearnAllRecipesInProfession(Player *player, uint32 skill)
{
    switch (skill)
    {
        case SKILL_ALCHEMY:
        {
            for (const auto& i: _SKILL_ALCHEMY)
                if (!player->HasSpell(i))
                    player->learnSpell(i);
            break;
        }
        case SKILL_BLACKSMITHING:
        {
            for (const auto& i: _SKILL_BLACKSMITHING)
                if (!player->HasSpell(i))
                    player->learnSpell(i);
            break;
        }
        case SKILL_ENCHANTING:
        {
            for (const auto& i: _SKILL_ENCHANTING)
                if (!player->HasSpell(i))
                    player->learnSpell(i);
            break;
        }
        case SKILL_ENGINEERING:
        {
            for (const auto& i: _SKILL_ENGINEERING)
                if (!player->HasSpell(i))
                    player->learnSpell(i);
            break;
        }
        case SKILL_INSCRIPTION:
        {
            for (const auto& i: _SKILL_INSCRIPTION)
                if (!player->HasSpell(i))
                    player->learnSpell(i);
            break;
        }
        case SKILL_JEWELCRAFTING:
        {
            for (const auto& i: _SKILL_JEWELCRAFTING)
                if (!player->HasSpell(i))
                    player->learnSpell(i);
            break;
        }
        case SKILL_LEATHERWORKING:
        {
            for (const auto& i: _SKILL_LEATHERWORKING)
                if (!player->HasSpell(i))
                    player->learnSpell(i);
            break;
        }
        case SKILL_TAILORING:
        {
            for (const auto& i: _SKILL_TAILORING)
                if (!player->HasSpell(i))
                    player->learnSpell(i);
            break;
        }
        case SKILL_COOKING:
        {
            for (const auto& i: _SKILL_COOKING)
                if (!player->HasSpell(i))
                    player->learnSpell(i);
            break;
        }
        case SKILL_FIRST_AID:
        {
            for (const auto& i: _SKILL_FIRST_AID)
                if (!player->HasSpell(i))
                    player->learnSpell(i);
            break;
        }
        case SKILL_FISHING:
        {
            for (const auto& i: _SKILL_FISHING)
                if (!player->HasSpell(i))
                    player->learnSpell(i);
            break;
        }
        case SKILL_MINING:
        {
            for (const auto& i: _SKILL_MINING)
                if (!player->HasSpell(i))
                    player->learnSpell(i);
            break;
        }
        case SKILL_SKINNING:
        {
            for (const auto& i: _SKILL_SKINNING)
                if (!player->HasSpell(i))
                    player->learnSpell(i);
            break;
        }
        case SKILL_HERBALISM:
        {
            for (const auto& i: _SKILL_HERBALISM)
                if (!player->HasSpell(i))
                    player->learnSpell(i);
            player->removeSpell(55503, SPEC_MASK_ALL, false);
            break;
        }
        default:
            break;
    }

    if (SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(skill))
    {
        player->SetSkill(SkillInfo->id, player->GetSkillStep(SkillInfo->id), 450, 450);
        uint32 ClassMask = player->getClassMask();

        for (uint32 i = 0; i < sSkillLineAbilityStore.GetNumRows(); ++i)
        {
            if (SkillLineAbilityEntry const *SkillLine = sSkillLineAbilityStore.LookupEntry(i))
            {
                if (SkillLine->skillId != SkillInfo->id)
                    continue;

                if (SkillLine->forward_spellid)
                    continue;

                if (SkillLine->racemask != 0)
                    continue;

                if (SkillLine->classmask && (SkillLine->classmask & ClassMask) == 0)
                    continue;

                SpellInfo const *SpellInfo2 = sSpellMgr->GetSpellInfo(SkillLine->spellId);

                if (!SpellInfo2 || !SpellMgr::IsSpellValid(SpellInfo2))
                    continue;

                player->learnSpell(SkillLine->spellId);
            }
        }
    }
    return true;
}

bool sProfession::IsSecondarySkill(uint32 skill) const {
    return skill == SKILL_COOKING || skill == SKILL_FIRST_AID || skill == SKILL_FISHING;
}

void sProfession::CompleteLearnProfession(Player *player, uint32 skill) {
    if (PlayerAlreadyHasMaxProfessions(player) && !IsSecondarySkill(skill)) {
        ChatHandler(player->GetSession()).PSendSysMessage(GetText(player,RU_player_errors_2,EN_player_errors_2));
        player->GetSession()->SendAreaTriggerMessage(GetText(player,RU_player_errors_2,EN_player_errors_2));
    }
    else {
        LearnAllRecipesInProfession(player, skill);
        if(!IsSecondarySkill(skill))
            CompleteQuest(player);
    }
    player->PlayerTalkClass->SendCloseGossip();
}

void sProfession::MainMenu(Player *player) {
    player->PlayerTalkClass->ClearMenus();
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_proff_text_17,EN_player_proff_text_17), GOSSIP_SENDER_MAIN, 1); // second
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_proff_text_18,EN_player_proff_text_18), GOSSIP_SENDER_MAIN, 2); // primary
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_back,EN_back), GOSSIP_SENDER_MAIN, 3); //  на главное меню
    player->PlayerTalkClass->GetGossipMenu().SetMenuId(GossipHelloMenu + 2);
    player->PlayerTalkClass->SendGossipMenu(HeadMenu(player), player->GetGUID());
}

void sProfession::PrimaryMenu(Player *player) {
    player->PlayerTalkClass->ClearMenus();
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_proff_text_20,EN_player_proff_text_20), GOSSIP_SENDER_MAIN + 1, SKILL_ALCHEMY);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_proff_text_21,EN_player_proff_text_21), GOSSIP_SENDER_MAIN + 1, SKILL_BLACKSMITHING);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_proff_text_22,EN_player_proff_text_22), GOSSIP_SENDER_MAIN + 1, SKILL_LEATHERWORKING);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_proff_text_23,EN_player_proff_text_23), GOSSIP_SENDER_MAIN + 1, SKILL_TAILORING);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_proff_text_24,EN_player_proff_text_24), GOSSIP_SENDER_MAIN + 1, SKILL_ENGINEERING);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_proff_text_25,EN_player_proff_text_25), GOSSIP_SENDER_MAIN + 1, SKILL_ENCHANTING);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_proff_text_26,EN_player_proff_text_26), GOSSIP_SENDER_MAIN + 1, SKILL_JEWELCRAFTING);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_proff_text_27,EN_player_proff_text_27), GOSSIP_SENDER_MAIN + 1, SKILL_INSCRIPTION);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_proff_text_28,EN_player_proff_text_28), GOSSIP_SENDER_MAIN + 1, SKILL_HERBALISM);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_proff_text_29,EN_player_proff_text_29), GOSSIP_SENDER_MAIN + 1, SKILL_SKINNING);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_proff_text_30,EN_player_proff_text_30), GOSSIP_SENDER_MAIN + 1, SKILL_MINING);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_back,EN_back), GOSSIP_SENDER_MAIN, 0);
    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
}

void sProfession::SecondMenu(Player *player) {
    player->PlayerTalkClass->ClearMenus();
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_proff_text_31,EN_player_proff_text_31), GOSSIP_SENDER_MAIN + 1, SKILL_FIRST_AID);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_proff_text_32,EN_player_proff_text_32), GOSSIP_SENDER_MAIN + 1, SKILL_FISHING);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_proff_text_33,EN_player_proff_text_33), GOSSIP_SENDER_MAIN + 1, SKILL_COOKING);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_back,EN_back), GOSSIP_SENDER_MAIN, 0);
    player->PlayerTalkClass->SendGossipMenu(HeadMenu(player), player->GetGUID());
}

void sProfession::ReagentsMenu(Player *player, Creature* creature) {
    player->PlayerTalkClass->ClearMenus();
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_reagents_1,EN_player_reagents_1), GOSSIP_SENDER_MAIN, 44001);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_reagents_2,EN_player_reagents_2), GOSSIP_SENDER_MAIN, 44002);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_reagents_3,EN_player_reagents_3), GOSSIP_SENDER_MAIN, 44003);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_reagents_4,EN_player_reagents_4), GOSSIP_SENDER_MAIN, 44004);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_reagents_5,EN_player_reagents_5), GOSSIP_SENDER_MAIN, 44005);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_reagents_6,EN_player_reagents_6), GOSSIP_SENDER_MAIN, 44006);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_reagents_7,EN_player_reagents_7), GOSSIP_SENDER_MAIN, 44007);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_reagents_8,EN_player_reagents_8), GOSSIP_SENDER_MAIN, 44008);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_reagents_9,EN_player_reagents_9), GOSSIP_SENDER_MAIN, 44009);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_reagents_10,EN_player_reagents_10), GOSSIP_SENDER_MAIN, 44010);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetText(player,RU_player_reagents_11,EN_player_reagents_11), GOSSIP_SENDER_MAIN, 44011);
    player->PlayerTalkClass->SendGossipMenu(HeadMenuNPC(player), creature->GetGUID());
}

std::string sProfession::HeadMenu(Player* player) {
    std::stringstream ss;
    if(player->GetSession()->GetSessionDbLocaleIndex() == LOCALE_ruRU)
        ss << RU_player_proff_text_1;
    else
        ss << EN_player_proff_text_1;
    return ss.str();
}

std::string sProfession::HeadMenuNPC(Player* player) {
    std::stringstream ss;
    if(player->GetSession()->GetSessionDbLocaleIndex() == LOCALE_ruRU)
        ss << RU_player_proff_text_2;
    else
        ss << EN_player_proff_text_2;
    return ss.str();
}
