DROP PROCEDURE IF EXISTS add_migration;
delimiter ??
CREATE PROCEDURE `add_migration`()
BEGIN
DECLARE v INT DEFAULT 1;
SET v = (SELECT COUNT(*) FROM `migrations` WHERE `id`='20190702193756');
IF v=0 THEN
INSERT INTO `migrations` VALUES ('20190702193756');
-- Add your query below.

UPDATE `mangos_string` SET `content_default` = '%s %s%s %s (guid: %u) Account: %s (id: %u%s) GMLevel: %u Last IP: %s%s Last login: %s Latency: %ums Client: %s 2FA: %s VPN: %s' WHERE (`entry` = '616');

-- End of migration.
END IF;
END??
delimiter ; 
CALL add_migration();
DROP PROCEDURE IF EXISTS add_migration;
