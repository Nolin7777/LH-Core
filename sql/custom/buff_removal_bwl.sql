-- Aura removal on entering naxxramas, disabled by default
DELETE FROM `instance_buff_removal` where mapId = 469;
insert into `instance_buff_removal` (mapId, auraId, enabled, flags, comment) 
VALUES
(469, 22888, 0, 0, "Rallying Cry"),
(469, 16609, 0, 0, "Warchief's blessing"),
(469, 24425, 0, 0, "Spirit of Zandalar"),
(469, 26393, 0, 0, "Elune's Blessing"),
(469, 15366, 0, 0, "Songflower Serenade"),
(469, 22818, 0, 0, "Mol'dar's Moxie (15% stam)"),
(469, 22820, 0, 0, "Slip'kik's Savvy (3% spellcrit)"),
(469, 22817, 0, 0, "Fengus' Ferocity (200 AP)"),
(469, 20707, 0, 0, "Soulstone"),
(469, 20765, 0, 0, "Soulstone"),
(469, 20764, 0, 0, "Soulstone"),
(469, 20762, 0, 0, "Soulstone"),
(469, 20763, 0, 0, "Soulstone"),
(469, 28681, 0, 0, "Soul Revival (Scourge Invasion Buff)"),

(469, 23735, 0, 0, "Sayge's Dark Fortune of Strength"),
(469, 23736, 0, 0, "Sayge's Dark Fortune of Agility"),
(469, 23737, 0, 0, "Sayge's Dark Fortune of Stamina"),
(469, 23738, 0, 0, "Sayge's Dark Fortune of Spirit"),
(469, 23766, 0, 0, "Sayge's Dark Fortune of Intelligence"),
(469, 23767, 0, 0, "Sayge's Dark Fortune of Armor"),
(469, 23768, 0, 0, "Sayge's Dark Fortune of Damage"),
(469, 23769, 0, 0, "Sayge's Dark Fortune of Resistance"),

(469, 1038,  0, 2, "Blessing of Salvation"),
(469, 25895, 0, 2, "Greater Blessing of Salvation"),

(469, 19740, 0, 2, "Blessing of Might (R1)"),
(469, 19834, 0, 2, "Blessing of Might (R2)"),
(469, 19835, 0, 2, "Blessing of Might (R3)"),
(469, 19836, 0, 2, "Blessing of Might (R4)"),
(469, 19837, 0, 2, "Blessing of Might (R5)"),
(469, 19838, 0, 2, "Blessing of Might (R6)"),
(469, 25291, 0, 2, "Blessing of Might (R7)"),
(469, 25782, 0, 2, "Greater Blessing of Might (R1)"),
(469, 25916, 0, 2, "Greater Blessing of Might (R2)"),

(469, 20911, 0, 2, "Blessing of Sanctuary (R1)"),
(469, 20912, 0, 2, "Blessing of Sanctuary (R2)"),
(469, 20913, 0, 2, "Blessing of Sanctuary (R3)"),
(469, 20914, 0, 2, "Blessing of Sanctuary (R4)"),
(469, 25899, 0, 2, "Greater Blessing of Sanctuary (R1)"),

(469, 19742, 0, 2, "Blessing of Wisdom (R1)"),
(469, 19850, 0, 2, "Blessing of Wisdom (R2)"),
(469, 19852, 0, 2, "Blessing of Wisdom (R3)"),
(469, 19853, 0, 2, "Blessing of Wisdom (R4)"),
(469, 19854, 0, 2, "Blessing of Wisdom (R5)"),
(469, 25290, 0, 2, "Blessing of Wisdom (R6)"),
(469, 25894, 0, 2, "Greater Blessing of Wisdom (R1)"),
(469, 25918, 0, 2, "Greater Blessing of Wisdom (R1)"),

(469, 20217, 0, 2, "Blessing of Kings (R1)"),
(469, 25898, 0, 2, "Greater Blessing of Kings (R1)"),

(469, 15123, 0, 0, 'Resist Fire (UBRS)');
