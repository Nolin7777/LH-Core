-- Updates any creature spawn rates for Northdale

-- Lupos
UPDATE `creature` SET `spawntimesecsmin` = 1200, `spawntimesecsmax` = 1800 WHERE `id` = 521;

-- Devilsaurs ???
-- Normal Devilsaur (10-12mins vs 15 static)
UPDATE `creature` SET `spawntimesecsmin` = 600, `spawntimesecsmax` = 720 WHERE `id` = 6498;
-- Ironhide Devilsaur (8-9mins, 10-12mins vs 10/15 static)
UPDATE `creature` SET `spawntimesecsmin` = 600, `spawntimesecsmax` = 720 WHERE `id` = 6499 AND `guid` <> 24173;
UPDATE `creature` SET `spawntimesecsmin` = 480, `spawntimesecsmax` = 540 WHERE `id` = 6499 AND `guid` = 24173;
-- Tyrant Devilsaur
UPDATE `creature` SET `spawntimesecsmin` = 600, `spawntimesecsmax` = 720 WHERE `id` = 6500 AND `guid` <> 24174;
UPDATE `creature` SET `spawntimesecsmin` = 480, `spawntimesecsmax` = 540 WHERE `id` = 6500 AND `guid` = 24174;

-- Eris Havenfire
UPDATE `creature` SET `spawntimesecsmin` = 900, `spawntimesecsmax` = 1500 WHERE `id` = 14494;

-- Hunter demon chain
-- Simone the Inconspicuous
-- Franklin the Friendly
-- Nelson the Nice
-- Artorius the Amiable
UPDATE `creature` SET `spawntimesecsmin` = 1200, `spawntimesecsmax` = 1800 WHERE `id` IN (14527, 14529, 14536, 14531);

-- Prince Nazjak asdf
UPDATE `creature` SET `spawntimesecsmin` = 115200/3, `spawntimesecsmax` = 172800/3 WHERE `id` = 2779;
