game_event_condition : contains conditions to meet for the specified game event to be completed. Also contains the world state fields used for reporting the given conditions progress and/or max required value.

game_event_quest_condition : contains quest -> event, condition mappings. Also contains how much a quest adds to a condition.

game_event_gameobject_quest : like game_event_creature_quest, but for gameobject questgivers

game_event_npcflag : npcflags to be added together when the specified event is active for the creature with the given guid

game_event_prerequisite : events that must have been completed to start the given event

game_event_npc_gossip : gossipid for the given npc when the specified event is active