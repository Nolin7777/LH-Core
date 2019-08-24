DROP PROCEDURE IF EXISTS add_migration;
delimiter ??
CREATE PROCEDURE `add_migration`()
BEGIN
DECLARE v INT DEFAULT 1;
SET v = (SELECT COUNT(*) FROM `migrations` WHERE `id`='20190630154418');
IF v=0 THEN
INSERT INTO `migrations` VALUES ('20190630154418');
-- Add your query below.

CREATE TABLE `vpn_cache` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `ip` INT UNSIGNED NOT NULL,
  `is_vpn` TINYINT NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `ip_UNIQUE` (`ip` ASC) VISIBLE);

ALTER TABLE `account` ADD COLUMN `vpn` INT UNSIGNED NOT NULL DEFAULT 0 AFTER `invite_id`;

-- End of migration.
END IF;
END??
delimiter ; 
CALL add_migration();
DROP PROCEDURE IF EXISTS add_migration;