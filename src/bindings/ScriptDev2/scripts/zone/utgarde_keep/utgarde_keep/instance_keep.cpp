#include "precompiled.h"
#include "def_keep.h"

struct MANGOS_DLL_DECL instance_keep : public ScriptedInstance
{
    instance_keep(Map *Map) : ScriptedInstance(Map) {Initialize();};
};

InstanceData* GetInstanceData_instance_keep(Map* map)
{
    return new instance_keep(map);
}

void AddSC_instance_keep()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_keep";
    newscript->GetInstanceData = GetInstanceData_instance_keep;
    newscript->RegisterSelf();
}
