-- Super quick gameobject spawn rates for quests below level 10
UPDATE gameobject go
JOIN gameobject_template got ON go.id = got.entry
JOIN gameobject_loot_template golt ON got.data1 = golt.entry
SET go.spawntimesecsmin = 3, go.spawntimesecsmax = 7
WHERE got.type = 3 AND golt.item IN (SELECT ReqItemId1 FROM quest_template WHERE MinLevel < 10 AND ReqItemId1 <> 0)
    AND golt.ChanceOrQuestChance < 0;

UPDATE gameobject go
JOIN gameobject_template got ON go.id = got.entry
JOIN gameobject_loot_template golt ON got.data1 = golt.entry
SET go.spawntimesecsmin = 3, go.spawntimesecsmax = 7
WHERE got.type = 3 AND golt.item IN (SELECT ReqItemId2 FROM quest_template WHERE MinLevel < 10 AND ReqItemId2 <> 0)
    AND golt.ChanceOrQuestChance < 0;

UPDATE gameobject go
JOIN gameobject_template got ON go.id = got.entry
JOIN gameobject_loot_template golt ON got.data1 = golt.entry
SET go.spawntimesecsmin = 3, go.spawntimesecsmax = 7
WHERE got.type = 3 AND golt.item IN (SELECT ReqItemId3 FROM quest_template WHERE MinLevel < 10 AND ReqItemId3 <> 0)
    AND golt.ChanceOrQuestChance < 0;

UPDATE gameobject go
JOIN gameobject_template got ON go.id = got.entry
JOIN gameobject_loot_template golt ON got.data1 = golt.entry
SET go.spawntimesecsmin = 3, go.spawntimesecsmax = 7
WHERE got.type = 3 AND golt.item IN (SELECT ReqItemId4 FROM quest_template WHERE MinLevel < 10 AND ReqItemId4 <> 0)
    AND golt.ChanceOrQuestChance < 0;


UPDATE gameobject go JOIN (SELECT count(*) as spawns, go.id as id, got.name FROM gameobject go JOIN gameobject_template got ON go.id = got.entry JOIN gameobject_loot_template golt ON got.data1 = golt.entry WHERE got.type = 3 AND golt.item IN (SELECT distinct(entry) from item_template where bonding = 4) AND go.map in (0,1) GROUP BY go.id HAVING spawns < 70) x ON x.id = go.id SET go.spawntimesecsmin = 15, go.spawntimesecsmax = 60, go.spawnFlags = go.spawnFlags | 8;

UPDATE gameobject go JOIN (SELECT count(*) as spawns, go.id as id, got.name FROM gameobject go JOIN gameobject_template got ON go.id = got.entry JOIN gameobject_loot_template golt ON got.data1 = golt.entry WHERE got.type = 3 AND golt.item IN (SELECT distinct(srcitemid) from quest_template) AND go.map in (0,1) GROUP BY go.id HAVING spawns < 70) x ON x.id = go.id SET go.spawntimesecsmin = 15, go.spawntimesecsmax = 60, go.spawnFlags = go.spawnFlags | 8;

-- Set resource respawn rates to half of the default value

-- low level ores
UPDATE `gameobject` SET `spawntimesecsmin`=300, `spawntimesecsmax` = 420 WHERE `id` IN (1610,1667,1731,1732,1733,2054,2055,3763,3764,73940,103711,103713,1731,1732,1733) and map IN (0,1);

-- mid level ores
UPDATE `gameobject` SET `spawntimesecsmin`=540, `spawntimesecsmax` = 660 WHERE `id` IN (1734,1735,2040,73941,123310,150079,176645) and map IN (0,1);

-- high level ores
UPDATE `gameobject` SET `spawntimesecs`=900 WHERE `id` IN (324,2047,123309,123848,150081,150082,165658,175404,176643,177388) and map IN (0,1);

-- low level herbs
UPDATE `gameobject` SET `spawntimesecsmin`=300, `spawntimesecsmax`= 420 WHERE `id` IN (1617,1618,1619,1620,1621,2045) and map IN (0,1);

-- mid level herbs
UPDATE `gameobject` SET `spawntimesecsmin`=540, `spawntimesecsmax` = 660 WHERE `id` IN (1622,1623,1624,1628,2041,2042,2043,2044,2046) and map IN (0,1);

-- high level herbs
UPDATE `gameobject` SET `spawntimesecsmin`=1600, `spawntimesecsmax` = 1800 WHERE `id` IN (2866,142140,142141,142142,142143,142144,142145,176583,176584,176586,176587,176588,176642) and map IN (0,1);
-- black lotus
UPDATE `gameobject` SET `spawntimesecsmin`=1600, `spawntimesecsmax` = 1900 WHERE `id` IN (176589) and map IN (0,1);
