DROP PROCEDURE IF EXISTS add_migration;
delimiter ??
CREATE PROCEDURE `add_migration`()
BEGIN
DECLARE v INT DEFAULT 1;
SET v = (SELECT COUNT(*) FROM `migrations` WHERE `id`='20190622045822');
IF v=0 THEN
INSERT INTO `migrations` VALUES ('20190622045822');
-- Add your query below.

INSERT INTO `world`.`mangos_string` (`entry`, `content_default`) VALUES ('50000', '[Realm First] %s');

-- End of migration.
END IF;
END??
delimiter ; 
CALL add_migration();
DROP PROCEDURE IF EXISTS add_migration;
