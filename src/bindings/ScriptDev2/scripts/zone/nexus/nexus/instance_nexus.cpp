#include "precompiled.h"
#include "def_nexus.h"

struct MANGOS_DLL_DECL instance_nexus : public ScriptedInstance
{
    instance_nexus(Map *Map) : ScriptedInstance(Map) {Initialize();};
};

InstanceData* GetInstanceData_instance_nexus(Map* map)
{
    return new instance_nexus(map);
}

void AddSC_instance_nexus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_nexus";
    newscript->GetInstanceData = GetInstanceData_instance_nexus;
    newscript->RegisterSelf();
}
