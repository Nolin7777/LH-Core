DROP PROCEDURE IF EXISTS add_migration;
delimiter ??
CREATE PROCEDURE `add_migration`()
BEGIN
DECLARE v INT DEFAULT 1;
SET v = (SELECT COUNT(*) FROM `migrations` WHERE `id`='20190824032138');
IF v=0 THEN
INSERT INTO `migrations` VALUES ('20190824032138');
-- Add your query below.
INSERT INTO `creature_template` (`entry`, `patch`, `KillCredit1`, `KillCredit2`, `modelid_1`, `modelid_2`, `modelid_3`, `modelid_4`, `name`, `subname`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spells_template`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `Civilian`, `RacialLeader`, `RegenHealth`, `equipment_id`, `trainer_id`, `vendor_id`, `MechanicImmuneMask`, `SchoolImmuneMask`, `flags_extra`, `ScriptName`) VALUES ('1000002', '10', '0', '0', '5438', '0', '0', '0', 'Brot Wizzlebang', 'Devilsaur Vendor', '10005', '60', '60', '25000', '25000', '0', '0', '0', '495', '495', '7', '1', '2', '1', '0', '1', '1', '0', '0', '1', '1500', '1500', '0', '0', '0', '0', '0', '0', '0', '0', '1', '1', '0', '9', '8', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');


INSERT INTO `gossip_menu` (`entry`, `text_id`, `condition_id`) VALUES ('10005', '100005', '0');


INSERT INTO `broadcast_text` (`ID`, `MaleText`, `Sound`, `Type`, `Language`, `EmoteId0`, `EmoteId1`, `EmoteId2`, `EmoteDelay0`, `EmoteDelay1`, `EmoteDelay2`) VALUES ('100005', "With profiteers and opportunists in control of Un'goro, we stand no chance against the evils that face us. I took my concerns to The Council but they didn't support my radical vision for synthesized Devilsaur skins, available to all, free of charge. I have strong suspicions that some in The Council may have benefitted from the iron grip held on Un'goro. Not dissuaded by my rebuffal, I assassinated the keystone of council policy on Un'goro and retreated to my secret labatory. I've spent the last several years toiling away, day and night, in search of a way to realise my goal of a Devilsaur skin utopia. We must have equal redistribution of skins! Unfortunately, we appear to be rather out of time, so I now present my research as-is, warts and all. You may need to do a little legwork of your own... batteries not included, if you will.", '0', '0', '0', '0', '0', '0', '0', '0', '0');



INSERT INTO `npc_text` (`ID`, `BroadcastTextID0`, `Probability0`, `BroadcastTextID1`, `Probability1`, `BroadcastTextID2`, `Probability2`, `BroadcastTextID3`, `Probability3`, `BroadcastTextID4`, `Probability4`, `BroadcastTextID5`, `Probability5`, `BroadcastTextID6`, `Probability6`, `BroadcastTextID7`, `Probability7`) VALUES ('100005', '100005', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');


INSERT INTO `item_template` (`entry`, `patch`, `class`, `subclass`, `name`, `displayid`, `Quality`, `Flags`, `BuyCount`, `BuyPrice`, `SellPrice`, `InventoryType`, `AllowableClass`, `AllowableRace`, `ItemLevel`, `RequiredLevel`, `RequiredSkill`, `RequiredSkillRank`, `requiredspell`, `requiredhonorrank`, `RequiredCityRank`, `RequiredReputationFaction`, `RequiredReputationRank`, `maxcount`, `stackable`, `ContainerSlots`, `stat_type1`, `stat_value1`, `stat_type2`, `stat_value2`, `stat_type3`, `stat_value3`, `stat_type4`, `stat_value4`, `stat_type5`, `stat_value5`, `stat_type6`, `stat_value6`, `stat_type7`, `stat_value7`, `stat_type8`, `stat_value8`, `stat_type9`, `stat_value9`, `stat_type10`, `stat_value10`, `dmg_min1`, `dmg_max1`, `dmg_type1`, `dmg_min2`, `dmg_max2`, `dmg_type2`, `dmg_min3`, `dmg_max3`, `dmg_type3`, `dmg_min4`, `dmg_max4`, `dmg_type4`, `dmg_min5`, `dmg_max5`, `dmg_type5`, `armor`, `holy_res`, `fire_res`, `nature_res`, `frost_res`, `shadow_res`, `arcane_res`, `delay`, `ammo_type`, `RangedModRange`, `spellid_1`, `spelltrigger_1`, `spellcharges_1`, `spellppmRate_1`, `spellcooldown_1`, `spellcategory_1`, `spellcategorycooldown_1`, `spellid_2`, `spelltrigger_2`, `spellcharges_2`, `spellppmRate_2`, `spellcooldown_2`, `spellcategory_2`, `spellcategorycooldown_2`, `spellid_3`, `spelltrigger_3`, `spellcharges_3`, `spellppmRate_3`, `spellcooldown_3`, `spellcategory_3`, `spellcategorycooldown_3`, `spellid_4`, `spelltrigger_4`, `spellcharges_4`, `spellppmRate_4`, `spellcooldown_4`, `spellcategory_4`, `spellcategorycooldown_4`, `spellid_5`, `spelltrigger_5`, `spellcharges_5`, `spellppmRate_5`, `spellcooldown_5`, `spellcategory_5`, `spellcategorycooldown_5`, `bonding`, `description`, `PageText`, `LanguageID`, `PageMaterial`, `startquest`, `lockid`, `Material`, `sheath`, `RandomProperty`, `block`, `itemset`, `MaxDurability`, `area`, `Map`, `BagFamily`, `DisenchantID`, `FoodType`, `minMoneyLoot`, `maxMoneyLoot`, `Duration`, `ExtraFlags`, `OtherTeamEntry`) VALUES ('70000', '10', '0', '0', 'Transdimensional Devilsaur Whistle', '17494', '6', '0', '1', '100000', '0', '0', '-1', '-1', '60', '1', '0', '0', '0', '0', '0', '0', '0', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'We are not alone in the universe. Wizzlebang\'s research revealed the presence of infinite dimensions, known as \"layers\". Why synthesise skin when you can harness Gnomish technology to teleport it to your own dimension?', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');


INSERT INTO `npc_vendor` (`entry`, `item`, `maxcount`, `incrtime`, `itemflags`) VALUES ('1000002', '70000', '0', '0', '0');


UPDATE `spell_template` SET `effectMiscValue1` = '6500' WHERE (`ID` = '29225');


UPDATE `item_template` SET `spellid_1` = '29225', `spellcharges_1` = '-1' WHERE (`entry` = '70000') and (`patch` = '10');


UPDATE `item_template` SET `spellcooldown_1` = '10000' WHERE (`entry` = '70000') and (`patch` = '10');

-- End of migration.
END IF;
END??
delimiter ; 
CALL add_migration();
DROP PROCEDURE IF EXISTS add_migration;
