/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Item_Scripts
SD%Complete: 100
SDComment: Items for a range of different items. See content below (in script)
SDCategory: Items
EndScriptData */

/* ContentData
item_nether_wraith_beacon(i31742)   Summons creatures for quest Becoming a Spellfire Tailor (q10832)
item_flying_machine(i34060, i34061)  Engineering crafted flying machines
item_gor_dreks_ointment(i30175)     Protecting Our Own(q10488)
item_only_for_flight                Items which should only useable while flying
EndContentData */

#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Spell.h"
#include "Translate.h"
#include "WorldSession.h"
#include "../Custom/ServerMenu/ServerMenuMgr.h"

#define GetText(a, b, c)    a->GetSession()->GetSessionDbLocaleIndex() == LOCALE_ruRU ? b : c

/*#####
# item_only_for_flight
#####*/

enum OnlyForFlight
{
    SPELL_ARCANE_CHARGES    = 45072
};

class item_only_for_flight : public ItemScript
{
public:
    item_only_for_flight() : ItemScript("item_only_for_flight") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        uint32 itemId = item->GetEntry();
        bool disabled = false;

        //for special scripts
        switch (itemId)
        {
            case 24538:
                if (player->GetAreaId() != 3628)
                    disabled = true;
                break;
            case 34489:
                if (player->GetZoneId() != 4080)
                    disabled = true;
                break;
            case 34475:
                if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_ARCANE_CHARGES))
                    Spell::SendCastResult(player, spellInfo, 1, SPELL_FAILED_NOT_ON_GROUND);
                break;
        }

        // allow use in flight only
        if (player->IsInFlight() && !disabled)
            return false;

        // error
        player->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, item, nullptr);
        return true;
    }
};

/*#####
# item_nether_wraith_beacon
#####*/

class item_nether_wraith_beacon : public ItemScript
{
public:
    item_nether_wraith_beacon() : ItemScript("item_nether_wraith_beacon") { }

    bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& /*targets*/) override
    {
        if (player->GetQuestStatus(10832) == QUEST_STATUS_INCOMPLETE)
        {
            if (Creature* nether = player->SummonCreature(22408, player->GetPositionX(), player->GetPositionY() + 20, player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 180000))
                nether->AI()->AttackStart(player);

            if (Creature* nether = player->SummonCreature(22408, player->GetPositionX(), player->GetPositionY() - 20, player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 180000))
                nether->AI()->AttackStart(player);
        }
        return false;
    }
};

/*#####
# item_gor_dreks_ointment
#####*/

class item_gor_dreks_ointment : public ItemScript
{
public:
    item_gor_dreks_ointment() : ItemScript("item_gor_dreks_ointment") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& targets) override
    {
        if (targets.GetUnitTarget() && targets.GetUnitTarget()->GetTypeId() == TYPEID_UNIT &&
                targets.GetUnitTarget()->GetEntry() == 20748 && !targets.GetUnitTarget()->HasAura(32578))
            return false;

        player->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, item, nullptr);
        return true;
    }
};

/*#####
# item_incendiary_explosives
#####*/

class item_incendiary_explosives : public ItemScript
{
public:
    item_incendiary_explosives() : ItemScript("item_incendiary_explosives") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        if (player->FindNearestCreature(26248, 15) || player->FindNearestCreature(26249, 15))
            return false;
        else
        {
            player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, nullptr);
            return true;
        }
    }
};

/*#####
# item_mysterious_egg
#####*/

class item_mysterious_egg : public ItemScript
{
public:
    item_mysterious_egg() : ItemScript("item_mysterious_egg") { }

    bool OnExpire(Player* player, ItemTemplate const* /*pItemProto*/) override
    {
        ItemPosCountVec dest;
        uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 39883, 1); // Cracked Egg
        if (msg == EQUIP_ERR_OK)
            player->StoreNewItem(dest, 39883, true);

        return true;
    }
};

/*#####
# item_disgusting_jar
#####*/

class item_disgusting_jar : public ItemScript
{
public:
    item_disgusting_jar() : ItemScript("item_disgusting_jar") { }

    bool OnExpire(Player* player, ItemTemplate const* /*pItemProto*/) override
    {
        ItemPosCountVec dest;
        uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 44718, 1); // Ripe Disgusting Jar
        if (msg == EQUIP_ERR_OK)
            player->StoreNewItem(dest, 44718, true);

        return true;
    }
};

/*#####
# item_petrov_cluster_bombs
#####*/

enum PetrovClusterBombs
{
    SPELL_PETROV_BOMB           = 42406,
    AREA_ID_SHATTERED_STRAITS   = 4064,
    ZONE_ID_HOWLING             = 495
};

class item_petrov_cluster_bombs : public ItemScript
{
public:
    item_petrov_cluster_bombs() : ItemScript("item_petrov_cluster_bombs") { }

    bool OnUse(Player* player, Item* item, const SpellCastTargets& /*targets*/) override
    {
        if (player->GetZoneId() != ZONE_ID_HOWLING)
            return false;

        if (!player->GetTransport() || player->GetAreaId() != AREA_ID_SHATTERED_STRAITS)
        {
            player->SendEquipError(EQUIP_ERR_NONE, item, nullptr);

            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_PETROV_BOMB))
                Spell::SendCastResult(player, spellInfo, 1, SPELL_FAILED_NOT_HERE);

            return true;
        }

        return false;
    }
};

enum CapturedFrog
{
    QUEST_THE_PERFECT_SPIES      = 25444,
    NPC_VANIRAS_SENTRY_TOTEM     = 40187
};

class item_captured_frog : public ItemScript
{
public:
    item_captured_frog() : ItemScript("item_captured_frog") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        if (player->GetQuestStatus(QUEST_THE_PERFECT_SPIES) == QUEST_STATUS_INCOMPLETE)
        {
            if (player->FindNearestCreature(NPC_VANIRAS_SENTRY_TOTEM, 10.0f))
                return false;
            else
                player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, nullptr);
        }
        else
            player->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, item, nullptr);
        return true;
    }
};

// Only used currently for
// 19169: Nightfall
class item_generic_limit_chance_above_60 : public ItemScript
{
public:
    item_generic_limit_chance_above_60() : ItemScript("item_generic_limit_chance_above_60") { }

    bool OnCastItemCombatSpell(Player* /*player*/, Unit* victim, SpellInfo const* /*spellInfo*/, Item* /*item*/) override
    {
        // spell proc chance gets severely reduced on victims > 60 (formula unknown)
        if (victim->getLevel() > 60)
        {
            // gives ~0.1% proc chance at lvl 70
            float const lvlPenaltyFactor = 9.93f;
            float const failureChance = (victim->getLevel() - 60) * lvlPenaltyFactor;

            // base ppm chance was already rolled, only roll success chance
            return !roll_chance_f(failureChance);
        }

        return true;
    }
};

class RandomMorphItem : public ItemScript
{
public:
    RandomMorphItem() : ItemScript("RandomMorphItem") { }

    static constexpr uint32 tab[3] = {29001, 29002, 29003};

    bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& /*targets*/) override
    {
        player->CastSpell(player, urand(tab[0], tab[2]), true);
        return false;
    }
};

class BonusGetOnAccountItem : public ItemScript
{
public:
    BonusGetOnAccountItem() : ItemScript("BonusGetOnAccountItem") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        if (!player || !item)
            return true;

        if (!player->GetSession())
            return true;    
            
        /* ид предмета */
        uint32 entry = item->GetEntry();
        /* выводим количество предметов */
        uint32 count = player->GetItemCount(entry, true);
        /* выводим бонусы игрока */
        uint32 bonuses = player->GetSession()->GetBonuses();
        /* удаляем предметы */
        player->DestroyItemCount(entry, count, true);
        /* выдаем бонусы */
        player->GetSession()->SetBonuses(bonuses + count);
        /* анонс игроку */
        ChatHandler(player->GetSession()).PSendSysMessage(GetText(player, RU_GET_BONUS_USE_ITEM, EN_GET_BONUS_USE_ITEM), count);
        return true;
    }
};

class ItemUse_Glory_Exp : public ItemScript
{
public:
    ItemUse_Glory_Exp() : ItemScript("ItemUse_Glory_Exp") { }

    bool OnUse(Player* pPlayer, Item* pItem, const SpellCastTargets& /*pTargets*/) override
    {
        if (!pPlayer || !pItem)
            return true;

        if (pPlayer->GetAuraCount(71201) >= 50)
            return true;            

        uint32 entry = pItem->GetEntry();
        uint32 rate = 0;

        if (pPlayer->GetAuraCount(71201) < 50) {
            switch (entry) {
                case 1042:
                    rate = 50;
                    break;
                case 1043:
                    rate = 100;
                    break;
                case 1044:
                    rate = 250;
                    break;
                case 35778:
                    rate = 1000;
                    break;
                case 842:
                    rate = 5000;
                    break;
                default:
                    break;
            }
        }

        if (rate != 0)
        {
            pPlayer->DestroyItemCount(entry,1,true);
            pPlayer->RewardRankPoints(rate, 4);
        }
        else
            ChatHandler(pPlayer->GetSession()).PSendSysMessage(GetText(pPlayer,"Вам нужен более высокий ранг для использование данного предмета.","You need a higher rank to use this item."));
        return true;
    }
};

class PremiumItemGetter : public ItemScript
{
public:
    PremiumItemGetter() : ItemScript("PremiumItemGetter") { }

    bool OnUse(Player* pPlayer, Item* pItem, const SpellCastTargets& /*pTargets*/) override
    {
        if (!pPlayer || !pItem)
            return true;

        if (pPlayer->GetSession()->IsPremium()) {
            ChatHandler(pPlayer->GetSession()).PSendSysMessage(GetText(pPlayer, 
            "|TInterface\\GossipFrame\\Battlemastergossipicon:15:15:|t |cffff9933[Премиум Аккаунт]: Дождитеcь окончание премиума перед использованием.", 
            "|TInterface\\GossipFrame\\Battlemastergossipicon:15:15:|t |cffff9933[Premium Account]: Wait until the end of the premium before using"));
            return true;
        }    

        uint32 entry = pItem->GetEntry();
        uint32 days = 1;

        switch (entry) {
            // 1 день
            case 34631: days = 1; break;
            // 7 дней    
            case 34632: days = 7; break;
            // 31 дня
            case 34633: days = 31; break;
            default: break;
        }

        pPlayer->DestroyItemCount(entry, 1, true);
        sServerMenuMgr->GetVipStatus(pPlayer, days);
        return true;
    }
};

class EventItemReward : public ItemScript
{
public:
    EventItemReward() : ItemScript("EventItemReward") { }

    bool OnUse(Player* pPlayer, Item* pItem, const SpellCastTargets& /*pTargets*/) override
    {
        if (!pPlayer || !pItem)
            return true;

        uint32 entry = pItem->GetEntry();
        
        if (!entry)
            return true;

        uint32 prize = 3;
        bool bigEvent = false;

        // назначаем ID приза за ивент
        sServerMenuMgr->SetItemRewardID(entry);            

        switch (entry) {
            case 34634: bigEvent = true; prize = 1; break;
            case 34635: bigEvent = true; prize = 2; break;
            case 34636: bigEvent = true; prize = 3; break;
            case 20486: /* авто ивент осада столиц */
            case 34628: bigEvent = false; prize = 1; break;
            case 34629: bigEvent = false; prize = 2; break;
            case 34630: bigEvent = false; prize = 3; break;
            default: break;
        }

        sServerMenuMgr->GossipMenuEventReward(pPlayer, entry, bigEvent, prize);
        return true;
    }
};

void AddSC_item_scripts()
{
    new item_only_for_flight();
    new item_nether_wraith_beacon();
    new item_gor_dreks_ointment();
    new item_incendiary_explosives();
    new item_mysterious_egg();
    new item_disgusting_jar();
    new item_petrov_cluster_bombs();
    new item_captured_frog();
    new item_generic_limit_chance_above_60();
    new BonusGetOnAccountItem();
    new RandomMorphItem();
    new ItemUse_Glory_Exp();
    new PremiumItemGetter();
    new EventItemReward();
}
