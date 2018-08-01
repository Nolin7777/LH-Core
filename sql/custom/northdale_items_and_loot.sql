-- Sets any custom item, quest or loot availability

-- --------------------------------
-- Mage Conjure
-- --------------------------------
UPDATE `item_template` SET `patch` = 1 WHERE `entry` = 22897;
DELETE FROM `forbidden_items` WHERE `entry` = 22897;
