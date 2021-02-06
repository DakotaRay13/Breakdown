#include <gb/gb.h>
#include "ResultsScreen.c"

void Results(){
    UINT8 r = 1;
    
    HIDE_SPRITES;

    //Load Results Screen
    set_win_tiles(0, 0, 20, 18, ResultsScreen);
    move_win(8, 0);

    while(r == 1)
    {
        if(joypad() & J_START)
        {
            r = 0;
        }

        wait_vbl_done();
    }

    SHOW_SPRITES;
}