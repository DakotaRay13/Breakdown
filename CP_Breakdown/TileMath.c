#include <stdio.h>

UINT16 GetTileIndex(UINT16 mapWidth, UINT16 index_Y, UINT16 index_X)
{
    return  mapWidth * index_Y + index_X;
}