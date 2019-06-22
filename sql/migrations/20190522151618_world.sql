DROP PROCEDURE IF EXISTS add_migration;
delimiter ??
CREATE PROCEDURE `add_migration`()
BEGIN
DECLARE v INT DEFAULT 1;
SET v = (SELECT COUNT(*) FROM `migrations` WHERE `id`='20190522151618');
IF v=0 THEN
INSERT INTO `migrations` VALUES ('20190522151618');
-- Add your query below.

-- https://web.archive.org/web/20061126083847/http://wow.allakhazam.com:80/db/item.html?witem=22819
UPDATE `item_template` SET `armor`=3425, `block`=68 WHERE `entry`=22819 AND `patch`=9;

-- End of migration.
END IF;
END??
delimiter ; 
CALL add_migration();
DROP PROCEDURE IF EXISTS add_migration;
