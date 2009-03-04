-- Vazruden script update already exist event_type=6
UPDATE eventai_scripts SET action3_type=34, action3_param1=1, action3_param2=3, comment='Vazrudens - Yell on Death Set instance data' WHERE id=1753702;
-- Nazan add event_type 6 to curent script
insert into `eventai_scripts` (`id`, `creature_id`, `event_type`, `event_inverse_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action1_type`, `action1_param1`, `action1_param2`, `action1_param3`, `action2_type`, `action2_param1`, `action2_param2`, `action2_param3`, `action3_type`, `action3_param1`, `action3_param2`, `action3_param3`, `comment`) values('1753610','17536','6','0','100','6','0','0','0','0','34','2','3','0','0','0','0','0','0','0','0','0','Nazan - Set instance data');
