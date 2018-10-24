-- Remove patch requirement from all event quests, objects and creatures


CREATE TEMPORARY TABLE seasonal_events (entry bigint);
INSERT INTO seasonal_events (entry) VALUES
    (1), -- mid summer
    (2), -- winter veil
    (7), -- lunar festival
    (8), -- love is in the air
    (9), -- noblegarden
    (10), -- children's week
    (11), -- harvest festival
    (12), -- hallow's end
    (21), -- winter veil gifts
    (28), -- noblegarden
    (34), -- new year's
    (76), -- lunar festival fireworks
    (140); -- kwee peddlefoot love is in the air
    

DELETE FROM forbidden_items
WHERE entry IN (
    SELECT RewItemId1 FROM quest_template qt JOIN game_event_quest ge on qt.entry = ge.quest JOIN seasonal_events se ON se.entry = ge.event
    UNION
    SELECT RewItemId2 FROM quest_template qt JOIN game_event_quest ge on qt.entry = ge.quest JOIN seasonal_events se ON se.entry = ge.event
    UNION
    SELECT RewItemId3 FROM quest_template qt JOIN game_event_quest ge on qt.entry = ge.quest JOIN seasonal_events se ON se.entry = ge.event
    UNION
    SELECT RewItemId4 FROM quest_template qt JOIN game_event_quest ge on qt.entry = ge.quest JOIN seasonal_events se ON se.entry = ge.event
    UNION
    SELECT ReqItemId1 FROM quest_template qt JOIN game_event_quest ge on qt.entry = ge.quest JOIN seasonal_events se ON se.entry = ge.event
    UNION
    SELECT ReqItemId2 FROM quest_template qt JOIN game_event_quest ge on qt.entry = ge.quest JOIN seasonal_events se ON se.entry = ge.event
    UNION
    SELECT ReqItemId3 FROM quest_template qt JOIN game_event_quest ge on qt.entry = ge.quest JOIN seasonal_events se ON se.entry = ge.event
    UNION
    SELECT ReqItemId4 FROM quest_template qt JOIN game_event_quest ge on qt.entry = ge.quest JOIN seasonal_events se ON se.entry = ge.event);

-- Remove quest relation patch requirements
UPDATE creature_questrelation SET patch = 0 WHERE quest IN (SELECT quest FROM game_event_quest WHERE event IN (SELECT entry FROM seasonal_events));
UPDATE creature_involvedrelation SET patch = 0 WHERE quest IN (SELECT quest FROM game_event_quest WHERE event IN (SELECT entry FROM seasonal_events));
UPDATE gameobject_questrelation SET patch = 0 WHERE quest IN (SELECT quest FROM game_event_quest WHERE event IN (SELECT entry FROM seasonal_events));
UPDATE gameobject_involvedrelation SET patch = 0 WHERE quest IN (SELECT quest FROM game_event_quest WHERE event IN (SELECT entry FROM seasonal_events));

-- Remove patch requirements on items that are contained in seasonal event objects???

-- Remove patch requirements from event creatures
UPDATE creature SET patch_min = 10, patch_max = 10 WHERE guid IN (SELECT guid FROM game_event_creature WHERE event IN (SELECT entry FROM seasonal_events));
-- Game objects
UPDATE gameobject SET patch_min = 0, patch_max = 10 WHERE guid IN (SELECT guid FROM game_event_gameobject WHERE event IN (SELECT entry FROM seasonal_events));
-- Quests
UPDATE quest_template qt
JOIN game_event_quest ge ON qt.entry = ge.quest
JOIN seasonal_events se on ge.event = se.entry
SET qt.patch = 0, ge.patch = 0;


-- Remove patch requirements on seasonal game events
UPDATE game_event SET patch_min = 0, patch_max = 10
WHERE entry IN (
    SELECT entry FROM seasonal_events;
);


DROP TABLE seasonal_events;
