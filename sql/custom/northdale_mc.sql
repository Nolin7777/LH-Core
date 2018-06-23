-- --------------------------------
-- MC Teleport avail in 1.2 (no run through BRD)
-- Also core does not support resing at BRD if you die in MC
-- --------------------------------
UPDATE `areatrigger_teleport` SET `patch` = 0 WHERE `id` IN (3528, 3529);

UPDATE `quest_template` SET `patch` = 0 WHERE `entry` IN (7487, 7848);
UPDATE `creature_questrelation` SET `patch` = 0 WHERE `quest` IN (7487, 7848);
UPDATE `creature_involvedrelation` SET `patch` = 0 WHERE `quest` IN (7487, 7848);

-- Core Fragment object
UPDATE `gameobject` SET `patch_min` = 0 WHERE `id` = 179553;
