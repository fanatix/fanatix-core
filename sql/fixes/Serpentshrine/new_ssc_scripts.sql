-- Underboc Colossus script and Refreshing Myst script
UPDATE creature_template SET ScriptName = 'mob_underbog_colossus' WHERE entry = '21251';

-- Leotheras the Blind script and new demon form healths
UPDATE creature_template SET ScriptName = 'boss_leotheras_the_blind' WHERE entry = '21215';
UPDATE creature_template SET ScriptName = 'boss_leotheras_the_blind_demonform' WHERE entry = '21875';
UPDATE creature_template SET ScriptName = 'mob_greyheart_spellbinder' WHERE entry = '21806';
UPDATE creature_template SET ScriptName = 'mob_inner_demon' WHERE entry = '21857';


-- Lady Vashj scripts (only new ones)
UPDATE creature_template SET ScriptName = 'mob_toxic_sporebat' WHERE entry = '22140';