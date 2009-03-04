#ifndef WMO_H
#define WMO_H

#include "loadlib.h"


// WMO group files
class WMO_group_file : public FileLoader{
public:
    bool   prepareLoadedData();

    WMO_group_file();
    ~WMO_group_file();
    void free();
};

// WMO root file
class wmo_MOHD{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
public:
    uint32 size;

    uint32 nTextures;
    uint32 nGroups;
    uint32 nPortals;
    uint32 nLights;
    uint32 nModels;
    uint32 nDoodads;
    uint32 nSets;
    uint32 colorABGR;
    uint32 wmoID;
    float  bb1[3];
    float  bb2[3];
    uint32 nullish;

    bool  prepareLoadedData();
};

class WMO_root_file : public FileLoader{
public:
    bool   prepareLoadedData();
    wmo_MOHD *header;

    WMO_root_file();
    ~WMO_root_file();
    void free();
};
#endif