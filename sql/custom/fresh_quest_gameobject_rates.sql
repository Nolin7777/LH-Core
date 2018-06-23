-- Super quick gameobject spawn rates for quests below level 10
UPDATE gameobject go
JOIN gameobject_template got ON go.id = got.entry
JOIN gameobject_loot_template golt ON got.data1 = golt.entry
SET go.spawntimesecsmin = 3, go.spawntimesecsmax = 7
WHERE got.type = 3 AND golt.item IN (SELECT ReqItemId1 FROM quest_template WHERE MinLevel < 10 AND ReqItemId1 <> 0)

-- Relatively quick spawn rates for quests >10, <20 and dynamic w/ total population
UPDATE gameobject go
JOIN gameobject_template got ON go.id = got.entry
JOIN gameobject_loot_template golt ON got.data1 = golt.entry
SET go.spawntimesecsmin = 120, go.spawntimesecsmax = 180, go.spawnFlags = go.spawnFlags | 8
WHERE got.type = 3 AND golt.item IN (SELECT ReqItemId1 FROM quest_template WHERE MinLevel > 10 AND MinLevel < 20 AND ReqItemId1 <> 0)
