#pragma once

#define ADT_FORMAT_VERSION    18

#define ADT_CELLS_PER_GRID    16

struct adt_MVER
{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;
    uint32 ver;
};

class adt_MCVT
{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;
public:
    float height_map[9*9+8*8];

    bool  prepareLoadedData();
};

class adt_MCLQ
{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;
public:
    float height1;
    float height2;
    struct liquid_data{
        uint32 light;
        float  height;
    } liquid[9][9];
    
    // 1<<0 - ochen
    // 1<<1 - slime
    // 1<<2 - water
    // 1<<6 - all water
    // 1<<7 - dark water
    // == 0x0F - not show liquid
    uint8 flags[8][8];
    uint8 data[84];
    bool  prepareLoadedData();
};

class adt_MCNK
{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;
public:
    uint32 flags;
    uint32 ix;
    uint32 iy;
    uint32 nLayers;
    uint32 nDoodadRefs;
    uint32 offsMCVT;        // height map
    uint32 offsMCNR;        // Normal vectors for each vertex
    uint32 offsMCLY;        // Texture layer definitions
    uint32 offsMCRF;        // A list of indices into the parent file's MDDF chunk
    uint32 offsMCAL;        // Alpha maps for additional texture layers
    uint32 sizeMCAL;
    uint32 offsMCSH;        // Shadow map for static shadows on the terrain
    uint32 sizeMCSH;
    uint32 areaid;
    uint32 nMapObjRefs;
    uint32 holes;
    uint16 s[2];
    uint32 data1;
    uint32 data2;
    uint32 data3;
    uint32 predTex;
    uint32 nEffectDoodad;
    uint32 offsMCSE;
    uint32 nSndEmitters;
    uint32 offsMCLQ;         // Liqid level (used in some mapsin outland? )
    uint32 sizeMCLQ;         //
    float  zpos;
    float  xpos;
    float  ypos;
    uint32 offsMCCV;         // offsColorValues in WotLK
    uint32 props;
    uint32 effectId;

    bool   prepareLoadedData();
    adt_MCVT *getMCVT()
    {
        if (offsMCVT)
            return (adt_MCVT *)((uint8 *)this + offsMCVT);
        return 0;
    }
    adt_MCLQ *getMCLQ()
    {
        if (offsMCLQ)
            return (adt_MCLQ *)((uint8 *)this + offsMCLQ);
        return 0;
    }
};

class adt_MCIN
{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;
public:
    struct adt_CELLS{
        uint32 offsMCNK;
        uint32 size;
        uint32 flags;
        uint32 asyncId;
    } cells[ADT_CELLS_PER_GRID][ADT_CELLS_PER_GRID];

    bool   prepareLoadedData();
    // offset from begin file (used this-84)
    adt_MCNK *getMCNK(int x, int y) { return (adt_MCNK *)((uint8 *)this + cells[x][y].offsMCNK - 84); }
};

#define ADT_LIQUID_HEADER_FULL_LIGHT   0x01
#define ADT_LIQUID_HEADER_NO_HIGHT     0x02

struct adt_liquid_header{
    uint16 liquidType;             // Index from LiquidType.dbc
    uint16 formatFlags;
    float  heightLevel1;
    float  heightLevel2;
    uint8  xOffset;
    uint8  yOffset;
    uint8  width;
    uint8  height;
    uint32 offsData2a;
    uint32 offsData2b;
};

class adt_MH2O
{
public:
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;

    struct adt_LIQUID{
    uint32 offsData1;
    uint32 used;
    uint32 offsData2;
    bool dataPresent() { return (offsData2!=0 || offsData2!=0);}
    } liquid[ADT_CELLS_PER_GRID][ADT_CELLS_PER_GRID];

    bool   prepareLoadedData();

    adt_liquid_header *getLiquidData(int x, int y)
    {
        if (liquid[x][y].used && liquid[x][y].offsData1)
            return (adt_liquid_header *)((uint8*)this + 8 + liquid[x][y].offsData1);
        return 0;
    }

    float *getLiquidHeightMap(adt_liquid_header *h) 
    {
        if (h->formatFlags & ADT_LIQUID_HEADER_NO_HIGHT)
            return 0;
        if (h->offsData2b)
            return (float *)((uint8*)this + 8 + h->offsData2b);
        return 0;
    }

    uint8 *getLiquidLightMap(adt_liquid_header *h) 
    {
        if (h->formatFlags&ADT_LIQUID_HEADER_FULL_LIGHT)
            return 0;
        if (h->offsData2b)
        {
            if (h->formatFlags & ADT_LIQUID_HEADER_NO_HIGHT)
                return (uint8 *)((uint8*)this + 8 + h->offsData2b);
            return (uint8 *)((uint8*)this + 8 + h->offsData2b + (h->width+1)*(h->height+1)*4);
        }
        return 0;
    }

    uint32 *getLiquidFullLightMap(adt_liquid_header *h) 
    {
        if (!(h->formatFlags&ADT_LIQUID_HEADER_FULL_LIGHT))
            return 0;
        if (h->offsData2b)
        {
            if (h->formatFlags & ADT_LIQUID_HEADER_NO_HIGHT)
                return (uint32 *)((uint8*)this + 8 + h->offsData2b);
            return (uint32 *)((uint8*)this + 8 + h->offsData2b + (h->width+1)*(h->height+1)*4);
        }
        return 0;
    }

    uint64 getLiquidShowMap(adt_liquid_header *h)   
    {
        if (h->offsData2a)
            return *((uint64 *)((uint8*)this + 8 + h->offsData2a));
        else
            return 0xFFFFFFFFFFFFFFFFLL;
    }

};

class adt_MHDR
{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;

    uint32 pad;
    uint32 offsMCIN;           // MCIN
    uint32 offsTex;	           // MTEX
    uint32 offsModels;	       // MMDX	
    uint32 offsModelsIds;	   // MMID	
    uint32 offsMapObejcts;	   // MWMO	
    uint32 offsMapObejctsIds;  // MWID		
    uint32 offsDoodsDef;       // MDDF	
    uint32 offsObjectsDef;     // MODF
    uint32 offsMFBO;           // MFBO
    uint32 offsMH2O;           // MH2O
    uint32 data1;
    uint32 data2;
    uint32 data3;
    uint32 data4;
    uint32 data5;
public:
    bool prepareLoadedData();
    adt_MCIN *getMCIN(){ return (adt_MCIN *)((uint8 *)&pad+offsMCIN);}
    adt_MH2O *getMH2O(){ return offsMH2O ? (adt_MH2O *)((uint8 *)&pad+offsMH2O) : NULL;}

};

class ADT_file{
    uint8  *data;
    uint32  data_size;
    bool   prepareLoadedData();
public:
    ADT_file(void);
    ~ADT_file();
    adt_MVER *info;
    adt_MHDR *a_grid;
    bool loadFile(char *filename);
    void free();
};
