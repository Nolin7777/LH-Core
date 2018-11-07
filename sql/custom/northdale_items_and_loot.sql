-- Sets any custom item, quest or loot availability

-- --------------------------------
-- Mage Conjure
-- --------------------------------
UPDATE `item_template` SET `patch` = 1 WHERE `entry` = 22897;
DELETE FROM `forbidden_items` WHERE `entry` = 22897;

-- --------------------------------
-- Patch 1.6 related additions
-- --------------------------------
-- Transmute: Elemental Fire
UPDATE `item_template` SET `patch` = 4 WHERE `entry` = 20761;
UPDATE `forbidden_items` SET `patch` = 3 WHERE `entry` = 20761;
