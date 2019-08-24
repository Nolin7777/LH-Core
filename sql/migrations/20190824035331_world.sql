DROP PROCEDURE IF EXISTS add_migration;
delimiter ??
CREATE PROCEDURE `add_migration`()
BEGIN
DECLARE v INT DEFAULT 1;
SET v = (SELECT COUNT(*) FROM `migrations` WHERE `id`='20190824035331');
IF v=0 THEN
INSERT INTO `migrations` VALUES ('20190824035331');
-- Add your query below.

UPDATE `map_template` SET `MapType` = '0' WHERE (`Entry` = '169');

-- End of migration.
END IF;
END??
delimiter ; 
CALL add_migration();
DROP PROCEDURE IF EXISTS add_migration;
