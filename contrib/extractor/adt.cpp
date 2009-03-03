#define _CRT_SECURE_NO_DEPRECATE

#ifdef WIN32
#include <windows.h>
#endif

#include "adt.h"
#include "mpq_libmpq.h"

extern uint16 *areas;
extern uint32 maxAreaId;
extern uint16 *LiqType;
// Helper
int holetab_h[4] = {0x1111, 0x2222, 0x4444, 0x8888};
int holetab_v[4] = {0x000F, 0x00F0, 0x0F00, 0xF000};

bool isHole(int holes, int i, int j)
{
    int testi = i / 2;
    int testj = j / 4;
    if(testi > 3) testi = 3;
    if(testj > 3) testj = 3;
    return (holes & holetab_h[testi] & holetab_v[testj]) != 0;
}

//
// Adt file loader class
//
ADT_file::ADT_file()
{
    data = 0;
    a_grid = 0;
}

ADT_file::~ADT_file()
{
    free();
}

void ADT_file::free()
{
    if (data)   delete [] data;
    data = 0;
    data_size = 0;
    a_grid = 0;
}

bool ADT_file::loadFile(char *filename)
{
    free();
    
    MPQFile mf(filename);

    if(mf.isEof())
    {
//        printf("No such file %s\n", filename);
        return false;
    }

    data_size = mf.getSize();

    data = new uint8 [data_size];
    if (data)
    {
        mf.read(data, data_size);
        mf.close();
        if (prepareLoadedData())
            return true;
    }
    printf("Error loading %s", filename);
    mf.close();
    free();
    return false;
}

//
// Adt file check function
//
bool ADT_file::prepareLoadedData()
{
    // Check version
    info = (adt_MVER *) data;
    if (info->fcc != 'MVER')
        return false;
    if (info->ver != ADT_FORMAT_VERSION)
        return false;

    // Check and prepare MHDR
    a_grid = (adt_MHDR *)(data+8+info->size);
    if (!a_grid->prepareLoadedData())
        return false;

    return true;
}

bool adt_MHDR::prepareLoadedData()
{
    if (fcc != 'MHDR')
        return false;

    if (size!=sizeof(adt_MHDR)-8)
        return false;

    // Check and prepare MCIN
    if (offsMCIN && !getMCIN()->prepareLoadedData())
        return false;

    // Check and prepare MH2O
    if (offsMH2O && !getMH2O()->prepareLoadedData())
        return false;

    return true;
}

bool adt_MCIN::prepareLoadedData()
{
    if (fcc != 'MCIN')
        return false;

    // Check cells data
    for (int i=0; i<ADT_CELLS_PER_GRID;i++)
        for (int j=0; j<ADT_CELLS_PER_GRID;j++)
            if (cells[i][j].offsMCNK && !getMCNK(i,j)->prepareLoadedData())
                return false;

    return true;
}

bool adt_MH2O::prepareLoadedData()
{
    if (fcc != 'MH2O')
        return false;

    // Check liquid data
//    for (int i=0; i<ADT_CELLS_PER_GRID;i++)
//        for (int j=0; j<ADT_CELLS_PER_GRID;j++)

    return true;
}

bool adt_MCNK::prepareLoadedData()
{
    if (fcc != 'MCNK')
        return false;

    // Check height map
    if (offsMCVT && !getMCVT()->prepareLoadedData())
        return false;
    // Check liquid data
    if (offsMCLQ && !getMCLQ()->prepareLoadedData())
        return false;

    return true;
}

bool adt_MCVT::prepareLoadedData()
{
    if (fcc != 'MCVT')
        return false;

    if (size != sizeof(adt_MCVT)-8)
        return false;

    return true;
}

bool adt_MCLQ::prepareLoadedData()
{
    if (fcc != 'MCLQ')
        return false;

    return true;
}

//
// Adt file convertor function and data
//

// Map file format data
#define MAP_MAGIC             'SPAM'
#define MAP_VERSION_MAGIC     '0.1v'
#define MAP_AREA_MAGIC        'AERA'
#define MAP_HEIGTH_MAGIC      'TGHM'
#define MAP_LIQUID_MAGIC      'QILM'

struct map_fileheader{
    uint32 mapMagic;
    uint32 versionMagic;
    uint32 areaMapOffset;
    uint32 areaMapSize;
    uint32 heightMapOffset;
    uint32 heightMapSize;
    uint32 liquidMapOffset;
    uint32 liquidMapSize;
};

#define MAP_AREA_NO_AREA      0x0001
struct map_areaHeader{
    uint32 fourcc;
    uint16 flags;
    uint16 gridArea;
};

#define MAP_HEIGHT_NO_HIGHT   0x0001
struct map_heightHeader{
    uint32 fourcc;
    uint32 flags;
    float  gridHeight;
};

#define MAP_LIQUID_TYPE_NO_WATER    0x00
#define MAP_LIQUID_TYPE_WATER       0x01
#define MAP_LIQUID_TYPE_LAVA        0x02
#define MAP_LIQUID_TYPE_DARK_WATER  0x04

#define MAP_LIQUID_NO_TYPE    0x0001
#define MAP_LIQUID_NO_HIGHT   0x0002
struct map_liquidHeader{
    uint32 fourcc;
    uint16 flags;
    uint16 liquidType;
    uint8  offsetX;
    uint8  offsetY;
    uint8  width;
    uint8  height;
    float  liquidLevel;
};

// Temporary data store
int16 area_flags[ADT_CELLS_PER_GRID][ADT_CELLS_PER_GRID];

float V8[ADT_GRID_SIZE][ADT_GRID_SIZE];
float V9[ADT_GRID_SIZE+1][ADT_GRID_SIZE+1];

int8  liquid_type[ADT_CELLS_PER_GRID][ADT_CELLS_PER_GRID];
bool  liquid_show[ADT_GRID_SIZE][ADT_GRID_SIZE];
float liquid_height[ADT_GRID_SIZE+1][ADT_GRID_SIZE+1];

bool ConvertADT(char *filename, char *filename2)
{
    ADT_file adt;

    if (!adt.loadFile(filename))
        return false;

    adt_MCIN *cells = adt.a_grid->getMCIN();
    if (!cells)
    {
        printf("Can't find cells in '%s'\n", filename);
        return false;
    }

    memset(liquid_show, 0, sizeof(liquid_show));
    memset(liquid_type, 0, sizeof(liquid_type));
    memset(liquid_height, 0, sizeof(liquid_height));

    // Prepare map header
    map_fileheader map;
    map.mapMagic = MAP_MAGIC;
    map.versionMagic = MAP_VERSION_MAGIC;

    // Get area flags data
    for (int i=0;i<ADT_CELLS_PER_GRID;i++)
    {
        for(int j=0;j<ADT_CELLS_PER_GRID;j++)
        {
            adt_MCNK * cell = cells->getMCNK(i,j);
            uint32 areaid = cell->areaid;
            if(areaid && areaid <= maxAreaId)
            {
                if(areas[areaid] == 0xffff)
                    printf("File: filename\nCan't find area flag for areaid %u [%d, %d].\n", filename, areaid, cell->ix, cell->iy);
                else
                    area_flags[i][j] = areas[areaid];
            }
        }
    }
    //============================================
    // Try pack area data
    //============================================
    bool fullAreaData = false;
    uint32 areaflag = area_flags[0][0];
    for (int y=0;y<ADT_CELLS_PER_GRID;y++)
    {
        for(int x=0;x<ADT_CELLS_PER_GRID;x++)
        {
            if(area_flags[y][x]!=areaflag)
                fullAreaData = true;
        }
    }

    map.areaMapOffset = sizeof(map);
    map.areaMapSize   = sizeof(map_areaHeader);

    map_areaHeader areaHeader;
    areaHeader.fourcc = MAP_AREA_MAGIC;
    areaHeader.flags = 0;
    if (fullAreaData)
    {
        areaHeader.flags = 0;
        areaHeader.gridArea = 0;
        map.areaMapSize+=sizeof(area_flags);
    }
    else
    {
        areaHeader.flags |= MAP_AREA_NO_AREA;
        areaHeader.gridArea = areaflag;
    }

    //
    // Get Height map from grid
    //
    for (int i=0;i<ADT_CELLS_PER_GRID;i++)
    {
        for(int j=0;j<ADT_CELLS_PER_GRID;j++)
        {
            adt_MCNK * cell = cells->getMCNK(i,j);
            if (!cell)
                continue;
            // Height values for triangles stored in order:
            // 1     2     3     4     5     6     7     8     9
            //    10    11    12    13    14    15    16    17
            // 18    19    20    21    22    23    24    25    26
            //    27    28    29    30    31    32    33    34
            // . . . . . . . .
            // For better get height values merge it to V9 and V8 map
            // V9 height map:
            // 1     2     3     4     5     6     7     8     9
            // 18    19    20    21    22    23    24    25    26
            // . . . . . . . .
            // V8 height map:
            //    10    11    12    13    14    15    16    17
            //    27    28    29    30    31    32    33    34
            // . . . . . . . .

            // Set map height as grid height
            for (int y=0; y <= ADT_CELL_SIZE; y++)
            {
                int cy = i*ADT_CELL_SIZE + y;
                for (int x=0; x <= ADT_CELL_SIZE; x++)
                {
                    int cx = j*ADT_CELL_SIZE + x;
                    V9[cy][cx]=cell->ypos;
                }
            }
            for (int y=0; y < ADT_CELL_SIZE; y++)
            {
                int cy = i*ADT_CELL_SIZE + y;
                for (int x=0; x < ADT_CELL_SIZE; x++)
                {
                    int cx = j*ADT_CELL_SIZE + x;
                    V8[cy][cx]=cell->ypos;
                }
            }
            // Get custom height
            adt_MCVT *v = cell->getMCVT();
            if (!v)
                continue;
            // get V9 height map
            for (int y=0; y <= ADT_CELL_SIZE; y++)
            {
                int cy = i*ADT_CELL_SIZE + y;
                for (int x=0; x <= ADT_CELL_SIZE; x++)
                {
                    int cx = j*ADT_CELL_SIZE + x;
                    V9[cy][cx]+=v->height_map[y*(ADT_CELL_SIZE*2+1)+x];
                }
            }
            // get V8 height map
            for (int y=0; y < ADT_CELL_SIZE; y++)
            {
                int cy = i*ADT_CELL_SIZE + y;
                for (int x=0; x < ADT_CELL_SIZE; x++)
                {
                    int cx = j*ADT_CELL_SIZE + x;
                    V8[cy][cx]+=v->height_map[y*(ADT_CELL_SIZE*2+1)+ADT_CELL_SIZE+1+x];
                }
            }
        }
    }
    //============================================
    // Try pack height data
    //============================================
    float maxHeight = -20000;
    float minHeight =  20000;
    for (int y=0; y<ADT_GRID_SIZE; y++)
    {
        for(int x=0;x<ADT_GRID_SIZE;x++)
        {
            float h = V8[y][x];
            if (maxHeight < h) maxHeight = h;
            if (minHeight > h) minHeight = h;
        }
    }
    for (int y=0; y<=ADT_GRID_SIZE; y++)
    {
        for(int x=0;x<=ADT_GRID_SIZE;x++)
        {
            float h = V9[y][x];
            if (maxHeight < h) maxHeight = h;
            if (minHeight > h) minHeight = h;
        }
    }
    map.heightMapOffset = map.areaMapOffset + map.areaMapSize;
    map.heightMapSize = sizeof(map_heightHeader);

    map_heightHeader heightHeader;
    heightHeader.fourcc = MAP_HEIGTH_MAGIC;
    heightHeader.flags = 0;
    heightHeader.gridHeight = minHeight;

    if (maxHeight!=minHeight)
    {
        heightHeader.flags = 0;
        map.heightMapSize+=sizeof(V9) + sizeof(V8);
    }
    else
    {
        heightHeader.flags |=MAP_HEIGHT_NO_HIGHT;
    }


    // Get liquid map for grid (in WOTLK used MH2O chunk)
    adt_MH2O * h2o = adt.a_grid->getMH2O();
    if (h2o)
    {
        for (int i=0;i<ADT_CELLS_PER_GRID;i++)
        {
            for(int j=0;j<ADT_CELLS_PER_GRID;j++)
            {
                adt_liquid_header *h = h2o->getLiquidData(i,j);
                if (!h)
                    continue;

                int count = 0;
                uint64 show = h2o->getLiquidShowMap(h);
                for (int y=0; y < h->height;y++)
                {
                    int cy = i*ADT_CELL_SIZE + y + h->yOffset;
                    for (int x=0; x < h->width; x++)
                    {
                        int cx = j*ADT_CELL_SIZE + x + h->xOffset;
                        if (show & 1)
                        {
                            liquid_show[cy][cx] = true;
                            ++count;
                        }
                        show>>=1;
                    }
                }

                uint32 type = LiqType[h->liquidType];
                if(type == 0xffff)
                    printf("\nCan't find Liquid type for map %s\nchunk %d,%d\n", filename, i, j);
                else if(type == LIQUID_TYPE_WATER || type == LIQUID_TYPE_OCEAN)
                    liquid_type[i][j] |= MAP_LIQUID_TYPE_WATER;         // water/ocean
                else if(type == LIQUID_TYPE_MAGMA || type == LIQUID_TYPE_SLIME)
                    liquid_type[i][j] |= MAP_LIQUID_TYPE_LAVA;          // magma/slime
                
                if (!count && liquid_type[i][j])
                    printf("Wrong liquid detect in MH2O chunk");

                float *height = h2o->getLiquidHeightMap(h);
                int pos = 0;
                for (int y=0; y<=h->height;y++)
                {
                    int cy = i*ADT_CELL_SIZE + y + h->yOffset;
                    for (int x=0; x<= h->width; x++)
                    {
                        int cx = j*ADT_CELL_SIZE + x + h->xOffset;
                        if (height)
                            liquid_height[cy][cx] = height[pos];
                        else
                            liquid_height[cy][cx] = h->heightLevel1;
                        pos++;
                    }
                }
            }
        }
    }
    else
    {
        for (int i=0;i<ADT_CELLS_PER_GRID;i++)
        {
            for(int j=0;j<ADT_CELLS_PER_GRID;j++)
            {
                adt_MCNK *cell = cells->getMCNK(i, j);
                if (!cell)
                    continue;

                adt_MCLQ *liquid = cell->getMCLQ();
                int count = 0;
                if (liquid && cell->sizeMCLQ > 8)
                {
                    for (int y=0; y < ADT_CELL_SIZE; y++)
                    {
                        int cy = i*ADT_CELL_SIZE + y;
                        for (int x=0; x < ADT_CELL_SIZE; x++)
                        {
                            int cx = j*ADT_CELL_SIZE + x;
                            if (liquid->flags[y][x] != 0x0F)
                            {
                                liquid_show[cy][cx] = true;
                                ++count;
                            }
                        }
                    }

                    uint32 c_flag = cell->flags;
                    if(c_flag & 4 || c_flag & 8)
                        liquid_type[i][j] |= MAP_LIQUID_TYPE_WATER;            // water
                    if(c_flag & 16)
                        liquid_type[i][j] |= MAP_LIQUID_TYPE_LAVA;             // magma/slime

                    if (!count && liquid_type[i][j])
                        printf("Wrong liquid detect in MCLQ chunk");

                    for (int y=0; y <= ADT_CELL_SIZE; y++)
                    {
                        int cy = i*ADT_CELL_SIZE + y;
                        for (int x=0; x<= ADT_CELL_SIZE; x++)
                        {
                            int cx = j*ADT_CELL_SIZE + x;
                            liquid_height[cy][cx] = liquid->liquid[y][x].height;
                        }
                    }
                }
            }
        }
    }

    //============================================
    // Pack liquid data
    //============================================
    uint8 type = liquid_type[0][0];
    bool fullType = false;
    for (int y=0;y<ADT_CELLS_PER_GRID;y++)
    {
        for(int x=0;x<ADT_CELLS_PER_GRID;x++)
        {
            if (liquid_type[y][x] !=type)
            {
                fullType = true;
                y = ADT_CELLS_PER_GRID;
                break;
            }
        }
    }

    map_liquidHeader liquidHeader;

    // no water data (if all grid have 0 liquid type)
    if (type && !fullType)
    {
        // No liquid data
        map.liquidMapOffset = 0;
        map.liquidMapSize   = 0;
    }
    else
    {
        int minX = 255, minY = 255;
        int maxX = 0, maxY = 0;
        maxHeight = -20000;
        minHeight = 20000;
        for (int y=0; y<ADT_GRID_SIZE; y++)
        {
            for(int x=0; x<ADT_GRID_SIZE; x++)
            {
                if (liquid_show[y][x])
                {
                    if (minX > x) minX = x;
                    if (maxX < x) maxX = x;
                    if (minY > y) minY = y;
                    if (maxY < y) maxY = y;
                    float h = liquid_height[y][x];
                    if (maxHeight < h) maxHeight = h;
                    if (minHeight > h) minHeight = h;
                }
            }
        }

        map.liquidMapOffset = map.heightMapOffset + map.heightMapSize;
        map.liquidMapSize = sizeof(map_liquidHeader);
        liquidHeader.fourcc = MAP_LIQUID_MAGIC;
        liquidHeader.flags = 0;
        liquidHeader.offsetX = minX;
        liquidHeader.offsetY = minY;
        liquidHeader.width   = maxX - minX + 1;
        liquidHeader.height  = maxY - minY + 1;
        liquidHeader.liquidLevel = minHeight;

        if (fullType)
        {
            liquidHeader.liquidType = 0;
            map.liquidMapSize+=sizeof(liquid_type);
        }
        else
        {
            liquidHeader.flags|=MAP_LIQUID_NO_TYPE;
            liquidHeader.liquidType = type;
        }

        if (maxHeight!=minHeight)
        {
            map.liquidMapSize+=sizeof(float)*(liquidHeader.width+1)*(liquidHeader.height+1);
        }
        else
        {
            liquidHeader.flags|=MAP_LIQUID_NO_HIGHT;
        }
    }

    // Ok all data prepared - store it
    FILE *output=fopen(filename2, "wb");
    if(!output)
    {
        printf("Can't create the output file '%s'\n", filename2);
        return false;
    }
    fwrite(&map, sizeof(map), 1, output);
    // Store area data
    fwrite(&areaHeader, sizeof(areaHeader), 1, output);
    if (!(areaHeader.flags&MAP_AREA_NO_AREA))
        fwrite(area_flags, sizeof(area_flags), 1, output);

    // Store height data
    fwrite(&heightHeader, sizeof(heightHeader), 1, output);
    if (!(heightHeader.flags&MAP_HEIGHT_NO_HIGHT))
    {
        fwrite(V9, sizeof(V9), 1, output);
        fwrite(V8, sizeof(V8), 1, output);
    }

    // Store liquid data if need
    if (map.liquidMapOffset)
    {
        fwrite(&liquidHeader, sizeof(liquidHeader), 1, output);
        if (!(liquidHeader.flags&MAP_LIQUID_NO_TYPE))
            fwrite(liquid_type, sizeof(liquid_type), 1, output);
        if (!(liquidHeader.flags&MAP_LIQUID_NO_HIGHT))
        {
            for (int y=0; y<=liquidHeader.height;y++)
                fwrite(&liquid_height[y+liquidHeader.offsetY][liquidHeader.offsetX], sizeof(float), liquidHeader.width+1, output);
        }
    }
    fclose(output);

    return true;
}
