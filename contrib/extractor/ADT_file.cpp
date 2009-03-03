#include "StdAfx.h"
#include "ADT_file.h"
#include <stdio.h>

ADT_file::ADT_file(void)
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
    FILE *in = fopen(filename, "rb");
    if (in == 0)
        return false;
    fseek(in, 0, SEEK_END);
    data_size = ftell(in);
    fseek(in, 0, SEEK_SET);
    data = new uint8 [data_size];
    if (data)
    {
        fread(data, data_size, 1, in);
        fclose(in);
        if (prepareLoadedData())
            return true;
    }
    printf("Error loading %s", filename);
    fclose(in);
    free();
    return false;
}

bool ADT_file::prepareLoadedData()
{
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

    int f = 0;
    // Check liquid data
    for (int i=0; i<ADT_CELLS_PER_GRID;i++)
    {
        for (int j=0; j<ADT_CELLS_PER_GRID;j++)
        {
            if (liquid[i][j].dataPresent())
            {
                adt_liquid_header * head = getLiquidData(i,j);
            }
        }
    }

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