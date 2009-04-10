
#ifndef SC_HYJAL_TRASH_AI_H
#define SC_HYJAL_TRASH_AI_H

#include "def_hyjal.h"
#include "../../../npc/npc_escortAI.h"

struct MANGOS_DLL_DECL hyjal_trashAI : public npc_escortAI
{
    hyjal_trashAI(Creature *c);

    void Reset();

    //void EnterEvadeMode();

    void Aggro(Unit *who);

    void UpdateAI(const uint32 diff);

    void JustDied(Unit* killer);

    void DamageTaken(Unit *done_by, uint32 &damage);

    public:   
        ScriptedInstance* pInstance;
        bool IsEvent;
        uint32 Delay;
        uint32 LastOverronPos;
        bool IsOverrun;
        bool SetupOverrun;
        uint32 OverrunType;
        uint8 faction;
        bool useFlyPath;

    //private:
};
#endif