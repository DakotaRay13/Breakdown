//Car Controller
#include "CarSpriteController.c"

//Background Sprites
//#include "RoadMap.c"

//Sprites
#include "Car.c"

//HUD
#include "HUD_Map.c"
#include "NUMERALS.c"

//High Score
#include "saveState.c"

UINT8 GAME_SPEED = 1;
UINT8 DRIFT_DELAY = 2;
UINT8 CRNT_DELAY = 0;
BOOLEAN isAlive;

//Edges of the road
const UINT8 EDGE_LEFT = 16;
const UINT8 EDGE_RIGHT = 120;
const UINT8 EDGE_TOP = 24;
const UINT8 EDGE_BOTTOM = 112;

UINT8 score[6] = {0,0,0,0,0,0};
UINT8 cur_ScoreDelay = 0;
const UINT8 SCORE_DELAY = 15;

//Lvl Score requirements (single digits only)
const UINT8 LVL2_REQ = 1;
const UINT8 LVL3_REQ = 5;

//Perform a delay by waiting for the vblank to finish for a number of frames
void performDelay(UINT8 numLoops)
{
    UINT8 i;
    for(i = 0; i < numLoops; i++)
    {
        wait_vbl_done();
    }
}

void SetupCarsTiles()
{
    set_sprite_data(0, 8, Car);
}

//***************************************************************************//
//Collision function and variables

//Check to see if the car has collided with another object or ran off the road
BOOLEAN CheckCollision(UINT8 x)
{
    UBYTE result = 1;

    //Check X coordinate to see if car ran off the road.
    if(x - 4 >= EDGE_RIGHT || x - 12 <= EDGE_LEFT)
        result = 0;

    //check for collision with other car
    if(GetCarCollision())
    {
        result = 0;
    }

    return result;
}

//***************************************************************************//
// Player Control Functions

//Check if the player is pressing a button durring gameplay
void CheckPlayerInputGame()
{
    if(joypad() & J_LEFT)
    {
        playerCar.x -= moveSpeed;
    }

    if(joypad() & J_RIGHT)
    {
        playerCar.x += moveSpeed;
    }
    
    if((joypad() & J_A) || (joypad() & J_UP))
    {
        playerCar.y -= moveSpeed;
    }

    if((joypad() & J_B) || (joypad() & J_DOWN))
    {
        playerCar.y += moveSpeed;
    }
    
    if(!((joypad() & J_RIGHT) || (joypad() & J_LEFT)))
    {
        //Player Drifts to the left
        if(CRNT_DELAY >= DRIFT_DELAY)
        {
            playerCar.x -= driftSpeed;
            CRNT_DELAY = 0;
        }
        else CRNT_DELAY++;
    }
    

    //Car cannot go off top of screen
    if(playerCar.y < EDGE_TOP) playerCar.y = EDGE_TOP;

    //Car cannot go off bottom of screen
    if(playerCar.y > EDGE_BOTTOM) playerCar.y = EDGE_BOTTOM;

    

    MoveSprite(&playerCar);
}

//***************************************************************************//
//Score Functions

void ResetScore()
{
    int i = 0;
    while(i < 6)
    {
        score[i] = 0;
        i++;
    }
}

void IncramentScore(UINT8 index)
{
    score[index]++;

    if(score[index] >= 10)
    {
        score[index] -= 10;
        IncramentScore(index + 1);
    }
}

void UpdateScore()
{
    if(cur_ScoreDelay >= SCORE_DELAY)
    {
        cur_ScoreDelay = 0;

        if (!(score[0] == 9 && score[1] == 9 && score[2] == 9 && 
        score[3] == 9 && score[4] == 9 && score[5] == 9))
        {
            //Incrament the score
            IncramentScore(0);
            UpdateScoreDigits(score);
        }
        
    }
    else
    {
        if(playerCar.x < 80)
        {
            cur_ScoreDelay += GAME_SPEED + GAME_SPEED;
        }
        else cur_ScoreDelay += GAME_SPEED;
    }
}

//***************************************************************************//

void SaveNewHighScore()
{
    highScore[0] = score[0];
    highScore[1] = score[1];
    highScore[2] = score[2];
    highScore[3] = score[3];
    highScore[4] = score[4];
    highScore[5] = score[5];
}

void CheckForHighScore()
{
    //Start index at 5. Go down to 0.
    UINT8 x = 5;
    while(x > 0)
    {
        if(highScore[x] != 0 || score[x] != 0)
        {
            if(score[x] > highScore[x])
            {
                SaveNewHighScore();
                return;
            }
            else if (score[x] < highScore[x])
            {
                return;
            }
        }
        x -= 1;
    }

    if(highScore[x] != 0 || score[x] != 0)
    {
        if(score[x] > highScore[x])
        {
            SaveNewHighScore();
            return;
        }
        else if (score[x] < highScore[x])
        {
            return;
        }
    }
}

//Check if High Score is set
void CheckForScoreError()
{
    BOOLEAN result = TRUE;
    UINT8 x = 0;

    //Go through all the integers. if one is garbage, reset the score.
    while(x < 6)
    {
        if(highScore[x] > 0x09) result = FALSE;
        x += 1;
    }

    if(result == FALSE)
    {
        highScore[0] = 0;
        highScore[1] = 0;
        highScore[2] = 0;
        highScore[3] = 0;
        highScore[4] = 0;
        highScore[5] = 0;
    }
}

//***************************************************************************//
void Gameplay()
{
    //Where the screen scroll is right now
    UINT8 CURRENT_SCROLL = 0;
    
    //Set up Background
    //set_bkg_tiles(0, 0, 20, 32, RoadMap);

    //Set up the HUD
    //set_window_tiles(x pos, y pos, width, height, tilemap)
    set_win_tiles(0, 0, 20, 5, HUD_Map);

    //Set Level and score digits
    ResetScoreDigits();
    ResetScore();
    cur_ScoreDelay = 0;

    //Move the window layer down to show the screen above it.
    move_win(7, 120);

    //Set up player's car
    SetupPlayerChar(84, EDGE_BOTTOM);

    //Set up the other cars
    SetupOtherCars();

    //Set isAlive to true
    isAlive = TRUE;

    //Game Loop
    while(isAlive == TRUE){
        //Update the Score
        UpdateScore();
        
        // scroll_bkg(how far on X, how far on Y)
        scroll_bkg(0,-GAME_SPEED - GAME_SPEED - GAME_SPEED);
        CURRENT_SCROLL += GAME_SPEED + GAME_SPEED + GAME_SPEED;

        //Move the other cars
        MoveCars(GAME_SPEED);

        //Get the character controls
        CheckPlayerInputGame();

        //Check for collision
        isAlive = CheckCollision(playerCar.x);

        performDelay(1);
    }

    //Check for high score
    CheckForHighScore();

    //Go to results screen
    UpdateHighScoreDigits(highScore);
    scroll_bkg(0, CURRENT_SCROLL);
}

