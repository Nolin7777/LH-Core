-- Sets the PVP timeline for Northdale.
-- Updates BG level requirements and patches, portal requirements,
-- release gear, Battlemaster access
-- See other PVP-related patches for updating later gear.

-- --------------------------------
-- Warsong Gulch available in 1.2
-- --------------------------------
UPDATE `battleground_template` SET `MinLvl` = 10, `MaxLvl` = 60 WHERE `id` = 2;
UPDATE `battleground_template` SET `AllianceWinSpell` = 23661, `HordeWinSpell` = 23702 WHERE `id` = 2 AND `patch` = 0;

-- Update quest relation first
UPDATE `creature_questrelation` SET `patch` = 0 WHERE `quest` IN (
        SELECT `entry` FROM `quest_template` WHERE `ReqItemId1` IN (19213, 19322) AND `patch` IN (3, 5)
    );

UPDATE `creature_involvedrelation` SET `patch` = 0 WHERE `quest` IN (
        SELECT `entry` FROM `quest_template` WHERE `ReqItemId1` IN (19213, 19322) AND `patch` IN (3, 5)
    );

-- Now the template
UPDATE `quest_template` SET `patch` = 0 WHERE `ReqItemId1` IN (19213, 19322) AND `patch` IN (3, 5);

-- --------------------------------
-- Arathi Basin available in 1.2
-- --------------------------------
UPDATE `battleground_template` SET `MinLvl` = 20, `MaxLvl` = 60 WHERE `id` = 3;
UPDATE `battleground_template` SET `AllianceWinSpell` = 24017, `HordeWinSpell` = 24017 WHERE `id` = 3 AND `patch` = 0;

-- Quests
UPDATE `creature_questrelation` SET `patch` = 0 WHERE `quest` IN (
        SELECT `entry` FROM `quest_template` WHERE `ReqItemId1` = 19725 AND `patch` = 5
    );

UPDATE `creature_involvedrelation` SET `patch` = 0 WHERE `quest` IN (
        SELECT `entry` FROM `quest_template` WHERE `ReqItemId1` = 19725 AND `patch` = 5
    );

-- Now the template
UPDATE `quest_template` SET `patch` = 0 WHERE `ReqItemId1` = 19725 AND `patch` = 5;

-- --------------------------------
-- Battlemasters in 1.3
-- --------------------------------
UPDATE `creature` SET `patch_min` = 1 WHERE `id` IN (SELECT `entry` FROM `battlemaster_entry`);

-- --------------------------------
-- Call to Arms - No AV CTA until 1.5
-- --------------------------------
UPDATE `game_event` SET `patch_min` = 0 WHERE `entry` IN (19, 20);
UPDATE `game_event` SET `patch_min` = 3 WHERE `entry` = 18;

-- --------------------------------
-- Level 58 gear & initial epic pieces
-- --------------------------------
-- Level 58 armour
UPDATE `item_template`
    SET `patch` = 0
    WHERE `itemset` IN (382, 347, 301, 281, 341, 345, 342, 362,
                        381, 346, 401, 348, 282, 344, 343, 361)
        AND `patch` = 2;

DELETE FROM `forbidden_items`
    WHERE `patch` = 1 
        AND `entry` IN (SELECT DISTINCT(`entry`) FROM `item_template` WHERE `itemset` IN
                           (382, 347, 301, 281, 341, 345, 342, 362,
                            381, 346, 401, 348, 282, 344, 343, 361));

-- Epic armour
UPDATE `item_template`
    SET `patch` = 0
    WHERE `itemset` IN (402, 384, 395, 389, 388, 397, 392, 394,
                        383, 386, 396, 390, 387, 398, 391, 393)
        AND `patch` = 2;

DELETE FROM `forbidden_items`
    WHERE `patch` = 1 
        AND `entry` IN (SELECT DISTINCT(`entry`) FROM `item_template` WHERE `itemset` IN
                           (402, 384, 395, 389, 388, 397, 392, 394,
                            383, 386, 396, 390, 387, 398, 391, 393));

-- Epic weapons
UPDATE `item_template`
    SET `patch` = 0
    WHERE `entry` IN (12584, 16345, 18825, 18826, 18827, 18828,
                      18830, 18831, 18833, 18835, 18836, 18837,
                      18838, 18840, 18843, 18844, 18847, 18848,
                      18855, 18860, 18865, 18866, 18867, 18868,
                      18869, 18871, 18873, 18874, 18876, 18877)
        AND `patch` = 2;

DELETE FROM `forbidden_items`
    WHERE `patch` = 1 
        AND `entry` IN (12584, 16345, 18825, 18826, 18827, 18828,
                        18830, 18831, 18833, 18835, 18836, 18837,
                        18838, 18840, 18843, 18844, 18847, 18848,
                        18855, 18860, 18865, 18866, 18867, 18868,
                        18869, 18871, 18873, 18874, 18876, 18877);

-- Mounts
DELETE FROM `forbidden_items` WHERE `patch` = 1
    AND `entry` IN (SELECT DISTINCT(`entry`) FROM `item_template` WHERE `requiredhonorrank` = 15);
UPDATE `item_template` SET `patch` = 0 WHERE `requiredhonorrank` = 15 AND `patch` = 2;

-- --------------------------------
-- PVP Halls & Accessories
-- --------------------------------
-- Door/portal
UPDATE `gameobject` SET `state` = 0 WHERE `id` IN (176576, 176562);
UPDATE `areatrigger_teleport` SET `patch` = 0 WHERE `id` IN (2532, 2527);

-- Accessories
DELETE FROM `forbidden_items` WHERE `patch` = 1
    AND `entry` IN (SELECT `entry` FROM `item_template`
                    WHERE `patch` = 2 AND `requiredhonorrank` BETWEEN 1 AND 10);

UPDATE `item_template` SET `patch` = 0 WHERE `requiredhonorrank` BETWEEN 1 AND 10 AND `patch` = 2;

-- NPCs
UPDATE `creature` SET `patch_min` = 0 WHERE `map` IN (449, 450);
