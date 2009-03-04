INSERT INTO `script_texts` VALUES ('-1000335', 'Let\'s get to the others, and keep an eye open for those wolves cutside...', null, null, 'Lass uns zu den anderen gehen . Achtet auf die Wölfe , die draußen umherstreifen...', null, null, null, null, null, '0', '0', '0', '0', 'npc_deathstalker_erland \n- SAY_QUESTACCEPT');
INSERT INTO `script_texts` VALUES ('-1000336', 'Be careful, $N. Those wolves like to hide among the trees.', null, null, 'Sei vorsichtig, $N. Die Wölfe lauern gerne im Dickicht.', null, null, null, null, null, '0', '0', '0', '0', 'npc_deathstalker_erland - SAY_START');
INSERT INTO `script_texts` VALUES ('-1000337', 'A $C attacks!', null, null, 'Ein $C greift uns an!', null, null, null, null, null, '0', '0', '0', '0', 'npc_deathstalker_erland - SAY_AGGRO_1');
INSERT INTO `script_texts` VALUES ('-1000338', 'Beware! I am under attack!', null, null, 'Los, machen wir ihn platt.', null, null, null, null, null, '0', '0', '0', '0', 'npc_deathstalker_erland - SAY_AGGRO_2');
INSERT INTO `script_texts` VALUES ('-1000339', 'We\'re almost there!', null, null, 'Wir sind bald da!', null, null, null, null, null, '0', '0', '0', '0', 'npc_deathstalker_erland - SAY_LAST');
INSERT INTO `script_texts` VALUES ('-1000340', 'We made it! Thanks, $N. I couldn\'t have gotten without you.', null, null, 'Wir haben es geschafft . Ich danke euch, $N! Ohne Euch hätte ich das niemals geschafft', null, null, null, null, null, '0', '0', '0', '0', 'npc_deathstalker_erland - \nSAY_THANKS');
INSERT INTO `script_texts` VALUES ('-1000341', 'It\'s good to see you again, Erland. What is your report?', null, null, 'Schön dich wiederzusehen, Erland! Was gibt es Neues zu berichten?', null, null, null, null, null, '0', '0', '0', '0', 'npc_deathstalker_erland - SAY_RANE');
INSERT INTO `script_texts` VALUES ('-1000342', 'Masses of wolves are to the east, and whoever lived at Malden\'s Orchard is gone.', null, null, 'Massen von Wölfen befinden sich östlich von hier! Wer immer dort gelebt hat...ist nicht mehr...', null, null, null, null, null, '0', '0', '0', '0', 'npc_deathstalker_erland - SAY_ANSWER');
INSERT INTO `script_texts` VALUES ('-1000343', 'If I am excused, then I\'d like to check on Quinn...', null, null, 'Wenn du mich kurz entschuldigen würdest! Ich werde eben mal nach Quinn sehen!', null, null, null, null, null, '0', '0', '0', '0', 'npc_deathstalker_erland - SAY_MOVE_QUINN');
INSERT INTO `script_texts` VALUES ('-1000344', 'Hello, Quinn. How are you faring?', null, null, 'Hallo, Quinn. Wie ist das werte Befinden?', null, null, null, null, null, '0', '0', '0', '0', 'npc_deathstalker_erland - SAY_GREETINGS');
INSERT INTO `script_texts` VALUES ('-1000345', 'I\'ve been better. Ivar the Foul got the better of me...', null, null, 'Es geht mir schon besser. Ivan der Faule hat mich besiegt.', null, null, null, null, null, '0', '0', '0', '0', 'npc_deathstalker_erland - SAY_QUINN');
INSERT INTO `script_texts` VALUES ('-1000346', 'Try to take better care of yourself, Quinn. You were lucky this time.', null, null, 'Pass das nächste Mal besser auf dich auf, Quinn! Vielleicht hast du dann mehr Glück.', null, null, null, null, null, '0', '0', '0', '0', 'npc_deathstalker_erland - \nSAY_ON_BYE');

DELETE FROM script_texts where entry BETWEEN -1000292 and -1000286;
INSERT INTO script_texts (entry, content_default, type, emote, comment) VALUES
(-1000286, 'A shadowy, sinister presence has invader the Emerald Dream. Its power is poised to spill over into our world, $N. We must oppose it! That\'s why I cannot accompany you in person.', 0, 0, 'clintar_spirit'),
(-1000287, 'The Emerald Dream will never be yours!', 0, 0, 'clintar_spirit'),
(-1000288, 'Begone from this place!', 0, 0, 'clintar_spirit'),
(-1000289, 'That\'s the first relic, but there are still two more. Follow me, $N.', 0, 0, 'clintar_spirit'),
(-1000290, 'I\'ve recovered the second relic. Take a moment to rest, and then we\'ll continue to the last reliquary.', 0, 0, 'clintar_spirit'),
(-1000291, 'We have all three of the relics, but my energy is rapidly fading. We must make our way back to Dreamwarden Lurosa! He will let you know what to do next.', 0, 0, 'clintar_spirit'),
(-1000292, 'Lurosa, I am entrusting the Relics of Aviana to $N, who will take them to Morthis Whisperwing. I must return completely to the Emerald Dream now. Do not let $N fail!', 0, 0, 'clintar_spirit');

DELETE FROM script_texts WHERE entry BETWEEN -1000279 and -1000272;
INSERT INTO script_texts (entry, content_default, type, comment) VALUES
(-1000272, 'Alright, alright I think I can figure out how to operate this thing...', 0, 'shredder - SAY_PROGRESS_1'),
(-1000273, 'Arrrgh! This isn\'t right!', 0, 'shredder - SAY_PROGRESS_2'),
(-1000274, 'Okay, I think I\'ve got it, now. Follow me, $N!', 0, 'shredder - SAY_PROGRESS_3'),
(-1000275, 'There\'s the stolen shredder! Stop it or Lugwizzle will have our hides!', 1, 'shredder - SAY_MERCENARY_4'),
(-1000276, 'Looks like we\'re out of woods, eh? Wonder what this does...', 0, 'shredder - SAY_PROGRESS_5'),
(-1000277, 'Come on, don\'t break down on me now!', 0, 'shredder - SAY_PROGRESS_6'),
(-1000278, 'That was a close one! Well, let\'s get going, it\'s still a ways to Ratchet!', 0, 'shredder - SAY_PROGRESS_7'),
(-1000279, 'Hmm... I don\'t think this blinking red light is a good thing...', 0, 'shredder - SAY_PROGRESS_8');


DELETE FROM script_texts WHERE entry BETWEEN -1000244 and -1000235;
INSERT INTO script_texts (entry, content_default, type, emote, comment) VALUES
(-1000235, 'Ok, $N. Follow me to the cave where I\'ll attempt to harness the power of the rune stone into these goggles.', 0, 1, 'phizzlethorpe - SAY_PROGRESS_1'),
(-1000236, 'I discovered this cave on our first day here. I believe the energy in the stone can be used to our advantage.', 0, 1, 'phizzlethorpe - SAY_PROGRESS_2'),
(-1000237, 'I\'ll begin drawing energy from the stone. Your job, $N, is to defend me. This place is cursed... trust me.', 0, 1, 'phizzlethorpe - SAY_PROGRESS_3'),
(-1000238, 'begins tinkering with the goggles before the stone.', 2, 0, 'phizzlethorpe - EMOTE_PROGRESS_4'),
(-1000239, 'Help!!! Get these things off me so I can get my work done!', 0, 0, 'SAY_AGGRO'),
(-1000240, 'Almost done! Just a little longer!', 0, 1, 'phizzlethorpe - SAY_PROGRESS_5'),
(-1000241, 'I\'ve done it! I have harnessed the power of the stone into the goggles! Let\'s get out of here!', 0, 1, 'phizzlethorpe - SAY_PROGRESS_6'),
(-1000242, 'Phew! Glad to be back from that creepy cave.', 0, 1, 'phizzlethorpe - SAY_PROGRESS_7'),
(-1000243, 'hands one glowing goggles over to Doctor Draxlegauge.', 2, 0, 'phizzlethorpe - EMOTE_PROGRESS_8'),
(-1000244, 'Doctor Draxlegauge will give you further instructions, $N. Many thanks for your help!', 0, 1, 'phizzlethorpe - SAY_PROGRESS_9');

-- Script Texts
DELETE FROM `script_texts` WHERE `entry` IN (-1000306,-1000307,-1000308,-1000309,-1000310,-1000311,-1000312,-1000313,-1000314,-1000315,-1000316,-1000317,-1000318,-1000319,-1000320,-1000321,-1000322,-1000323,-1000324,-1000325,-1000326,-1000327);
INSERT INTO `script_texts` (`entry`,`content_default`,`sound`,`type`,`language`,`emote`,`comment`) VALUES
('-1000306','Follow me\, stranger. This won\'t take long.','0','0','0','0','kservant SAY1'),
('-1000307','Shattrath was once the draenei capital of this world. Its name means \"dwelling of light.\"','0','4','0','0','kservant WHISP1'),
('-1000308','When the Burning Legion turned the orcs against the draenei\, the fiercest battle was fought here. The draenei fought tooth and nail\, but in the end the city fell.','0','4','0','0','kservant WHISP2'),
('-1000309','The city was left in ruins and darkness... until the Sha\'tar arrived.','0','4','0','0','kservant WHISP3'),
('-1000310','Let us go into the Lower City. I will warn you that as one of the only safe havens in Outland\, Shattrath has attracted droves of refugees from all wars\, current and past.','0','4','0','0','kservant WHISP4'),
('-1000311','The Sha\'tar\, or \"born from light\" are the naaru that came to Outland to fight the demons of the Burning Legion.','0','4','0','0','kservant WHISP5'),
('-1000312','They were drawn to the ruins of Shattrath City where a small remnant of the draenei priesthood conducted its rites inside a ruined temple on this very spot.','0','4','0','0','kservant WHISP6'),
('-1000313','The priesthood\, known as the Aldor\, quickly regained its strength as word spread that the naaru had returned and reconstruction soon began. The ruined temple is now used as an infirmary for injured refugees.','0','4','0','0','kservant WHISP7'),
('-1000314','It wouldn\'t be long\, however\, before the city came under attack once again. This time\, the attack came from Illidan\'s armies. A large regiment of blood elves had been sent by Illidan\'s ally, Kael\'thas Sunstrider\, to lay waste to the city.','0','4','0','0','kservant WHISP8'),
('-1000315','As the regiment of blood elves crossed this very bridge\, the Aldor\'s exarchs and vindicators lined up to defend the Terrace of Light. But then the unexpected happened.','0','4','0','0','kservant WHISP9'),
('-1000316','The blood elves laid down their weapons in front of the city\'s defenders; their leader\, a blood elf elder known as Voren\'thal\, stormed into the Terrace of Light and demanded to speak to A\'dal.','0','4','0','0','kservant WHISP10'),
('-1000317','As the naaru approached him\, Voren\'thal kneeled before him and uttered the following words: \"I\'ve seen you in a vision\, naaru. My race\'s only hope for survival lies with you. My followers and I are here to serve you.\"','0','4','0','0','kservant WHISP11'),
('-1000318','The defection of Voren\'thal and his followers was the largest loss ever incurred by Kael\'s forces. And these weren\'t just any blood elves. Many of the best and brightest amongst Kael\'s scholars and magisters had been swayed by Voren\'thal\'s influence.','0','4','0','0','kservant WHISP12'),
('-1000319','The naaru accepted the defectors\, who would become known as the Scryers; their dwelling lies in the platform above. Only those initiated with the Scryers are allowed there.','0','4','0','0','kservant WHISP13'),
('-1000320','The Aldor are followers of the Light and forgiveness and redemption are values they understand. However\, they found hard to forget the deeds of the blood elves while under Kael\'s command.','0','4','0','0','kservant WHISP14'),
('-1000321','Many of the priesthood had been slain by the same magisters who now vowed to serve the naaru. They were not happy to share the city with their former enemies.','0','4','0','0','kservant WHISP15'),
('-1000322','The Aldor\'s most holy temple and its surrounding dwellings lie on the terrace above. As a holy site\, only the initiated are welcome inside.','0','4','0','0','kservant WHISP16'),
('-1000323','The attacks against Shattrath continued\, but the city did not fall\, as you can see. On the contrary\, the naaru known as Xi\'ri led a successful incursion into Shadowmoon Valley - Illidan\'s doorstep.','0','4','0','0','kservant WHISP17'),
('-1000324','There he continues to wage war on Illidan with the assistance of the Aldor and the Scryers. The two factions have not given up on their old feuds\, though.','0','4','0','0','kservant WHISP18'),
('-1000325','Such is their animosity that they vie for the honor of being sent to assist the naaru there. Each day\, that decision is made here by A\'dal. The armies gather here to receive A\'dal\'s blessing before heading to Shadowmoon.','0','4','0','0','kservant WHISP19'),
('-1000326','Khadgar should be ready to see you again. Just remember that to serve the Sha\'tar you will most likely have to ally with the Aldor or the Scryers. And seeking the favor of one group will cause the others\' dislike.','0','4','0','0','kservant WHISP20'),
('-1000327','Good luck stranger\, and welcome to Shattrath City.','0','4','0','0','kservant WHISP21');


