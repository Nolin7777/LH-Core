ALTER TABLE `npc_vendor_template`
	CHANGE COLUMN `ExtendedCost` `ExtendedCost` MEDIUMINT(8) NULL DEFAULT NULL AFTER `incrtime`,
	CHANGE COLUMN `condition_id` `condition_id` MEDIUMINT(8) NULL DEFAULT NULL AFTER `ExtendedCost`;
