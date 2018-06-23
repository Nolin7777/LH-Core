-- Remove patch requirement from all event quests, objects and creatures

-- First remove patch requirements on quest reward and sources that are related to
-- game events
UPDATE item_template
SET patch = 0
WHERE entry IN (
    SELECT RewItemId1 FROM quest_template qt JOIN game_event_quest ge on qt.entry = ge.quest
    UNION
    SELECT RewItemId2 FROM quest_template qt JOIN game_event_quest ge on qt.entry = ge.quest
    UNION
    SELECT RewItemId3 FROM quest_template qt JOIN game_event_quest ge on qt.entry = ge.quest
    UNION
    SELECT RewItemId4 FROM quest_template qt JOIN game_event_quest ge on qt.entry = ge.quest
    UNION
    SELECT ReqItemId1 FROM quest_template qt JOIN game_event_quest ge on qt.entry = ge.quest
    UNION
    SELECT ReqItemId2 FROM quest_template qt JOIN game_event_quest ge on qt.entry = ge.quest
    UNION
    SELECT ReqItemId3 FROM quest_template qt JOIN game_event_quest ge on qt.entry = ge.quest
    UNION
    SELECT ReqItemId4 FROM quest_template qt JOIN game_event_quest ge on qt.entry = ge.quest);


UPDATE quest_template qt
JOIN game_event_quest ge ON qt.entry = ge.quest
SET qt.patch = 0, ge.patch = 0;
