/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ZULAMAN_H
#define DEF_ZULAMAN_H

#define ENCOUNTERS     7
#define RAND_VENDOR    2

enum InstanceZA
{
    SAY_INST_RELEASE        = -1568067,
    SAY_INST_BEGIN          = -1568068,
    SAY_INST_PROGRESS_1     = -1568069,
    SAY_INST_PROGRESS_2     = -1568070,
    SAY_INST_PROGRESS_3     = -1568071,
    SAY_INST_WARN_1         = -1568072,
    SAY_INST_WARN_2         = -1568073,
    SAY_INST_WARN_3         = -1568074,
    SAY_INST_WARN_4         = -1568075,
    SAY_INST_SACRIF1        = -1568076,
    SAY_INST_SACRIF2        = -1568077,
    SAY_INST_COMPLETE       = -1568078,

    WORLD_STATE_ID          = 3104,
    WORLD_STATE_COUNTER     = 3106,

    TYPE_EVENT_RUN          = 1,
    TYPE_AKILZON            = 2,
    TYPE_NALORAKK           = 3,
    TYPE_JANALAI            = 4,
    TYPE_HALAZZI            = 5,
    TYPE_MALACRASS          = 6,
    TYPE_ZULJIN             = 7,

    TYPE_RAND_VENDOR_1      = 8,
    TYPE_RAND_VENDOR_2      = 9,

    DATA_AKILZON            = 10,
    DATA_NALORAKK           = 11,
    DATA_JANALAI            = 12,
    DATA_HALAZZI            = 13,
    DATA_MALACRASS          = 14,
    DATA_ZULJIN             = 15,
    DATA_HARRISON           = 16,

    DATA_J_EGGSLEFT         = 17,
    DATA_J_EGGSRIGHT        = 18,
    DATA_J_HATCHLEFT        = 19,
    DATA_J_HATCHRIGHT       = 20,

    DATA_GO_GONG            = 21,
    DATA_GO_MALACRASS_GATE  = 22,
    DATA_GO_ENTRANCE        = 23,
    DATA_HEXLORDEVENT       = 24,
    DATA_HEXLORDGUID        = 25
};

#endif
