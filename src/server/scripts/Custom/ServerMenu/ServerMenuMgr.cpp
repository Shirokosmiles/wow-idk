#include "ServerMenuMgr.h"
#include "Player.h"
#include "Chat.h"
#include "Translate.h"

using namespace Acore::ChatCommands;

void sServerMenu::OpenBankSlot(Player* player) {
    player->PlayerTalkClass->SendCloseGossip();
	player->GetSession()->SendShowBank(player->GetGUID());
}

void sServerMenu::OpenMailSlot(Player* player) {
    player->PlayerTalkClass->SendCloseGossip();
	player->GetSession()->SendShowMailBox(player->GetGUID());
}

std::string sServerMenu::ConfirmChangeRFN(Player* player, uint32 cost) {
    std::ostringstream ss;
    if(player->GetSession()->GetSessionDbLocaleIndex() == LOCALE_ruRU)
        ss << "\nНужно заплатить: " << cost << " бонусов. Продолжим ?";
    else
        ss << "\nNeed to pay: " << cost << " bonuses. Continue ?";
    return ss.str().c_str();
}

void sServerMenu::ChangeRFN(Player* player, int i) {
    
    uint32 bonuses = player->GetSession()->GetBonuses();

    switch (i) {
        case 0:
            if (bonuses >= sServerMenuMgr->getFactionCost()) {
                player->GetSession()->SetBonuses(uint32(bonuses - sServerMenuMgr->getFactionCost()));
                player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
                ChatHandler(player->GetSession()).PSendSysMessage(GetText(player, "Сделайте релог для применение действий.", "Relog to apply actions."));
            } else {
                ChatHandler(player->GetSession()).PSendSysMessage(GetText(player,RU_NO_BONUS_HAVE, EN_NO_BONUS_HAVE), sServerMenuMgr->getFactionCost());
            }
            break;
        case 1:
            if (bonuses >= sServerMenuMgr->getRaceCost()) {
                player->GetSession()->SetBonuses(uint32(bonuses - sServerMenuMgr->getRaceCost()));
                player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
                ChatHandler(player->GetSession()).PSendSysMessage(GetText(player, "Сделайте релог для применение действий.", "Relog to apply actions."));
            } else {
                ChatHandler(player->GetSession()).PSendSysMessage(GetText(player,RU_NO_BONUS_HAVE, EN_NO_BONUS_HAVE), sServerMenuMgr->getRaceCost());
            }
            break;
        case 2:
            if (bonuses >= sServerMenuMgr->getNickCost()) {
                player->GetSession()->SetBonuses(uint32(bonuses - sServerMenuMgr->getNickCost()));
                player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
                ChatHandler(player->GetSession()).PSendSysMessage(GetText(player, "Сделайте релог для применение действий.", "Relog to apply actions."));
            } else {
                ChatHandler(player->GetSession()).PSendSysMessage(GetText(player,RU_NO_BONUS_HAVE, EN_NO_BONUS_HAVE), sServerMenuMgr->getNickCost());
            }
            break;
        default: break;
    }
    player->PlayerTalkClass->SendCloseGossip();
}

void sServerMenu::CharControlMenu(Player* player) {
    ClearGossipMenuFor(player);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_CHAR_CONTROL_1, EN_CHAR_CONTROL_1), GOSSIP_SENDER_MAIN + 1, 0);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_CHAR_CONTROL_2, EN_CHAR_CONTROL_2), GOSSIP_SENDER_MAIN + 1, 1);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_CHAR_CONTROL_3, EN_CHAR_CONTROL_3), GOSSIP_SENDER_MAIN + 1, 5);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_CHAR_CONTROL_7, EN_CHAR_CONTROL_7), GOSSIP_SENDER_MAIN + 1, 6);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_CHAR_CONTROL_4, EN_CHAR_CONTROL_4), GOSSIP_SENDER_MAIN + 1, 2, ConfirmChangeRFN(player, sServerMenuMgr->getFactionCost()), 0, false);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_CHAR_CONTROL_5, EN_CHAR_CONTROL_5), GOSSIP_SENDER_MAIN + 1, 3, ConfirmChangeRFN(player, sServerMenuMgr->getRaceCost()), 0, false);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_CHAR_CONTROL_6, EN_CHAR_CONTROL_6), GOSSIP_SENDER_MAIN + 1, 4, ConfirmChangeRFN(player, sServerMenuMgr->getNickCost()), 0, false);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_back, EN_back), GOSSIP_SENDER_MAIN, 0);
    player->PlayerTalkClass->SendGossipMenu(sServerMenuMgr->HeadMenu(player, 1), player->GetGUID());
}

std::string sServerMenu::HeadMenu(Player* player, uint8 MenuId) {
    std::ostringstream ss;
    switch (MenuId) {
        case 0: {
            if(player->GetSession()->GetSessionDbLocaleIndex() == LOCALE_ruRU) {
                ss << "Приветствую вас, " << player->GetName() << "\n\n";
                ss << "На вашем аккаунте " << player->GetSession()->GetBonuses() << " бонусов.\n";
                ss << "Пополнить счёт: wow-idk.ru\n\n";
            }
            else {
                ss << "Greetings, " << player->GetName() << "\n\n";
                ss << "You have " << player->GetSession()->GetBonuses() << "bonuses in your account";
                ss << "Top up account wow-idk.ru\n\n";
            }
        } break;
        case 1: {
            if(player->GetSession()->GetSessionDbLocaleIndex() == LOCALE_ruRU) {
                ss << "Приветствую вас, " << player->GetName() << "\n\n";
                ss << "В данном разделе будет добавлятся уникальные разработки как и стандарт по нужде игрокам.\n";
            } else {
                ss << "Greetings, " << player->GetName() << "\n\n";
                ss << "In this section, unique developments will be added as well as a standard according to the needs of the players.\n";
            }
        } break;

        case 2: {
            if(player->GetSession()->GetSessionDbLocaleIndex() == LOCALE_ruRU) {
                ss << "Приветствую вас, " << player->GetName() << "\n\n";
                ss << "У вас " << player->GetHonorPoints() << " очков чести.\n\n";
                ss << "В данном разделе вы можете передать любому игроку на этом проекте часть или весь ваш хонор.\n";
                ss << "Для этого вам нужно выбрать количество которое хотите передать и указать ник игрока.\n";
            } else {
                ss << "Greetings, " << player->GetName() << "\n\n";
                ss << "У вас " << player->GetHonorPoints() << " очков чести.\n\n";
                ss << "В данном разделе вы можете передать любому игроку на этом проекте часть или весь ваш хонор.\n";
                ss << "Для этого вам нужно выбрать количество которое хотите передать и указать ник игрока.\n";
            }
        } break;

        default: break;
    }
    return ss.str().c_str();
}

void sServerMenu::GossipHelloMenu(Player* player) {
    ClearGossipMenuFor(player);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_HOME_MENU_1, EN_HOME_MENU_1), GOSSIP_SENDER_MAIN, 1);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_HOME_MENU_2, EN_HOME_MENU_2), GOSSIP_SENDER_MAIN, 2);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_HOME_MENU_3, EN_HOME_MENU_3), GOSSIP_SENDER_MAIN, 3);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_HOME_MENU_4, EN_HOME_MENU_4), GOSSIP_SENDER_MAIN, 4);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_HOME_MENU_5, EN_HOME_MENU_5), GOSSIP_SENDER_MAIN, 5);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_HOME_MENU_6, EN_HOME_MENU_6), GOSSIP_SENDER_MAIN, 6);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_HOME_MENU_7, EN_HOME_MENU_7), GOSSIP_SENDER_MAIN, 7);
    player->PlayerTalkClass->GetGossipMenu().SetMenuId(UNIQUE_MENU_ID);
    player->PlayerTalkClass->SendGossipMenu(sServerMenuMgr->HeadMenu(player, 0), player->GetGUID());
}

void sServerMenu::RankInfo(Player* player) {
    player->PlayerTalkClass->ClearMenus();
    std::string name = player->GetName();
    std::ostringstream femb;

    if (player->GetSession()->GetSessionDbLocaleIndex() == LOCALE_ruRU)
    {
        femb << "Уважаемый|cff065961 " << name << "|r\n\n"
                << "Ваш текуший ранг: |cff065961" << player->GetRankByExp() << "|r\n"
                << "У вас: |cff065961" << player->GetRankPoints() << "|r опыта\n"
                << "До следущего ранга: |cff065961" << player->PointsUntilNextRank() << "|r опыта\n\n"
                << "Ранги это вот этот значок |TInterface\\icons\\Ability_warrior_rampage:14:14:0:-1|t который отображается в дебафах.\n\n"
                << "Поднимать ранг вы можете за опыт который получите за:\n"
                << "   * Победу на арене\n   * Победу на поле боя\n   * Убийство игроков\n   * Награда за квесты\n   * Ивенты\n"
            //  << "У |cff065961VIP аккаунтов|r рейтинг на опыт в 2 раза больше.\n\n"
                << "Что дает ранг ?\n"
                << "На каждом ранге есть свои бонусы, чем выше ранг тем больше бонусов.\n"
                << "На каждом ранге у вас открываются секретный продавец в котором могут быть полезные вещи такие как: вещи на А9-T11, трансмогрификацию, маунты, итд...";
    }
    else
    {
        femb << "Respected|cff065961 " << name << "|r\n\n"
                << "Your current rank: |cff065961" << player->GetRankByExp() << "|r\n"
                << "You have |cff065961" << player->GetRankPoints() << "|r experience\n"
                << "To next rank: |cff065961" << player->PointsUntilNextRank() << "|r experience\n\n"
                << "Ranks this is this icon |TInterface\\icons\\Ability_warrior_rampage:14:14:0:-1|t which is displayed in debuffs.\n\n"
                << "You can raise the rank for the experience that you get for:\n"
                << "   * Victory in the arena\n   * Victory on the battlefield\n   * Kill the players\n   * Completing quests\n   * Events\n"
            //  << "At |cff065961VIP accounts|r experience rating is 2 times higher.\n\n"
                << "What gives the rank?\n"
                << "Each rank has its own bonuses, the higher the rank the more bonuses.\n"
                << "At each rank you will have a secret prodigy in which there may be useful things such as: А9-T11, things for transmogrification, mounts, etc.";
    }

    AddGossipItemFor(player, GOSSIP_ICON_BATTLE, GetText(player, "Обновить", "Refresh"), GOSSIP_SENDER_MAIN, 2);
    AddGossipItemFor(player, GOSSIP_ICON_BATTLE, GetText(player, "Назад", "Back"), GOSSIP_SENDER_MAIN, 0);
    player->PlayerTalkClass->SendGossipMenu(femb.str().c_str(), player->GetGUID());
}

void sServerMenu::CommingSoon(Player* player) {
    ChatHandler(player->GetSession()).PSendSysMessage(GetText(player,"Система в разработке, некоторые разделы еще не готовы.", "Система в разработке, некоторые разделы еще не готовы."));
    player->PlayerTalkClass->SendCloseGossip(); 
}

bool sServerMenu::CanOpenMenu(Player* player) {
    if (player->IsInCombat() || player->IsInFlight() || player->IsDeathMatch() || player->GetMap()->IsBattlegroundOrArena()
        || player->HasStealthAura() || player->isDead() || (player->getClass() == CLASS_DEATH_KNIGHT && player->GetMapId() == 609 && !player->IsGameMaster() && !player->HasSpell(50977))) {
        ChatHandler(player->GetSession()).PSendSysMessage(GetText(player, "Сейчас это невозможно.", "Now it is impossible"));
        return true;
    }
    return false;
}

void sServerMenu::OpenTradeHonor(Player* player) {
    ClearGossipMenuFor(player);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_HONOR_TRADE_1000, EN_HONOR_TRADE_1000), GOSSIP_SENDER_MAIN, 1000, "После подтверждение введите ник игрока", 0, true);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_HONOR_TRADE_5000, EN_HONOR_TRADE_5000), GOSSIP_SENDER_MAIN, 5000, "После подтверждение введите ник игрока", 0, true);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_HONOR_TRADE_10000, EN_HONOR_TRADE_10000), GOSSIP_SENDER_MAIN, 10000, "После подтверждение введите ник игрока", 0, true);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_HONOR_TRADE_25000, EN_HONOR_TRADE_25000), GOSSIP_SENDER_MAIN, 25000, "После подтверждение введите ник игрока", 0, true);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_HONOR_TRADE_50000, EN_HONOR_TRADE_50000), GOSSIP_SENDER_MAIN, 50000, "После подтверждение введите ник игрока", 0, true);
    AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, GetText(player, RU_back, EN_back), GOSSIP_SENDER_MAIN, 5);
    player->PlayerTalkClass->SendGossipMenu(sServerMenuMgr->HeadMenu(player, 2), player->GetGUID());
}

void sServerMenu::TradeHonorAccept(Player* player, uint32 honor, char const* name) {
    
    if (!player || !honor ||!name)
        return;

    ObjectGuid TargetGUID;
    Player* target = ObjectAccessor::FindPlayerByName(name, false);

    // если игрок не найден
    if (!target) {
        TargetGUID = sCharacterCache->GetCharacterGuidByName(name);
        if (!TargetGUID) {
            ChatHandler(player->GetSession()).PSendSysMessage(LANG_PLAYER_NOT_FOUND);
            return sServerMenuMgr->OpenTradeHonor(player);
        }
    } else {
        TargetGUID = target->GetGUID();
    }

    // самому себе нельзя
    if (TargetGUID == player->GetGUID()) {
        ChatHandler(player->GetSession()).PSendSysMessage(GetText(player, "Вы не можете отправить самому себе!", "You cannot send to yourself!"));
        return sServerMenuMgr->OpenTradeHonor(player);      
    }

    // если у сендера недостаточно хонора
    if (player->GetHonorPoints() < honor) {
        ChatHandler(player->GetSession()).PSendSysMessage(GetText(player, "У вас недостаточно очков чести!", "You don't have enough honor points!"));
        return sServerMenuMgr->OpenTradeHonor(player);
    }
    
    sServerMenuMgr->SendHonorToPlayer(player, TargetGUID, honor, target ? true : false, target ? target : nullptr, name); 
}

void sServerMenu::SendHonorToPlayer(Player* sender, ObjectGuid receiver, uint32 amount, bool online, Player* Preceiver, char const* name) {
    
    if (!sender || !receiver || !amount)
        return;    

    // снимает у сендера очки чести 
    sender->ModifyHonorPoints(-uint32(amount));

    if (online) {
        // выдает получателю
        Preceiver->ModifyHonorPoints(amount);
        // кидаем нотификацию получателю
        ChatHandler(Preceiver->GetSession()).PSendSysMessage(GetText(Preceiver, RU_HONOR_TRADE_OK_RECEIVER, EN_HONOR_TRADE_OK_RECEIVER), amount, sender->GetName());
    } else {
        // если игрок оффлайн то обновляем в базе
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_HONOR_BY_GUID);
        stmt->SetData(0, amount);
        stmt->SetData(1, receiver.GetCounter());
        CharacterDatabase.Execute(stmt);
    }

    // кидаем нотификацию отправителю
    ChatHandler(sender->GetSession()).PSendSysMessage(GetText(sender, RU_HONOR_TRADE_OK_SENDER, EN_HONOR_TRADE_OK_SENDER), Preceiver ? Preceiver->GetName() : name, amount);
    return sServerMenuMgr->OpenTradeHonor(sender);
}