-- --------------------------------
-- MC Teleport avail in 1.2 (no run through BRD)
-- --------------------------------
UPDATE `areatrigger_teleport` SET `patch` = 0 WHERE `id` IN (3528, 3529);
UPDATE 
