ALTER TABLE `npc_vendor`
	ADD COLUMN `ExtendedCost` MEDIUMINT(8) NULL AFTER `incrtime`,
	ADD COLUMN `condition_id` MEDIUMINT(8) NULL AFTER `ExtendedCost`;

ALTER TABLE `npc_vendor`
	ALTER `ExtendedCost` DROP DEFAULT,
	ALTER `condition_id` DROP DEFAULT;
ALTER TABLE `npc_vendor`
	CHANGE COLUMN `ExtendedCost` `ExtendedCost` MEDIUMINT(8) UNSIGNED NOT NULL AFTER `incrtime`,
	CHANGE COLUMN `condition_id` `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL AFTER `ExtendedCost`;


ALTER TABLE `npc_vendor_template`
	ADD COLUMN `ExtendedCost` MEDIUMINT(8) NULL AFTER `incrtime`,
	ADD COLUMN `condition_id` MEDIUMINT(8) NULL AFTER `ExtendedCost`;

ALTER TABLE `npc_vendor_template`
	ALTER `ExtendedCost` DROP DEFAULT,
	ALTER `condition_id` DROP DEFAULT;

ALTER TABLE `npc_vendor_template`
	CHANGE COLUMN `ExtendedCost` `ExtendedCost` MEDIUMINT(8) UNSIGNED NOT NULL AFTER `incrtime`,
	CHANGE COLUMN `condition_id` `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL AFTER `ExtendedCost`;
