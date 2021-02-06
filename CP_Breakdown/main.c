//Implementing a background on Gameboy//
//Copy "E:/GB_Dev/gbdk-2.95-3-win32/gbdk/include/**" to include settings

#include <gb\gb.h>

//Start Screen
#include "START.c"

//Holds the gameplay script
#include "GAMEPLAY.c"

//Results Screen
#include "RESULTS.c"

//Background Sprite Tiles
#include "RoadTiles.c"
#include "HUD_Tiles.c"
#include "ResultsTiles.c"

#include "RoadMap.c"

//Load All background sprites
void LoadBackgroundSpriteData()
{
    set_bkg_data(0,12, RoadTiles);
    set_bkg_tiles(0, 0, 20, 32, RoadMap);
    set_win_data(12, 16, HUD_Tiles);
    set_win_data(35, 43, ResultsTiles);
}

void main()
{
    //Check save file
    CheckForScoreError();

    LoadBackgroundSpriteData();
    SetupCarsTiles();

    // Show sprites, background, window, and turn on display
    SHOW_BKG;
    SHOW_SPRITES;
    SHOW_WIN;
    DISPLAY_ON;

    //Start Screen
    Start();

    while(1)
    {
        Gameplay();
        Results();
        //set_bkg_tiles(0, 0, 20, 32, BlankMap);
    }
}