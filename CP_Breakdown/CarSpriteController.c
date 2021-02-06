//Structures and functions of the Game Characters

#include <gb\gb.h>
#include <rand.h>

//Generical character structure
typedef struct CarSprite {
    UBYTE spritids[4]; // All characters use 4 sprites
    UINT8 x;
    UINT8 y;
    UINT8 width;
    UINT8 height;
} CarSprite;

CarSprite playerCar;
CarSprite OtherCars[6];

const UINT8 moveSpeed = 1;
UINT8 driftSpeed = 1;
const UBYTE spritesize = 8;

const UINT8 COLLISION_OFFSET = 4;
const UINT8 LANES[6] = {37, 52, 68, 84, 100, 115};

//const UBYTE START_MASK = 0xA0;
const UINT8 END_POINT[2] = {144, 160};

//RNG didn't work. Using manual RNG.
const UINT8 CAR_POSITIONS[25] = {224, 200, 232, 216, 160, 220, 172, 208, 204, 244, 180, 184, 164, 240, 236, 168, 254, 196, 252, 248, 228, 212, 192, 188, 176};
UINT8 cp_index = 0;

//Place the character in a certain position
void MoveSprite(CarSprite* carSprite)
{
    move_sprite(carSprite->spritids[0], carSprite->x, carSprite->y);
    move_sprite(carSprite->spritids[1], carSprite->x, carSprite->y + spritesize);
    move_sprite(carSprite->spritids[2], carSprite->x + spritesize, carSprite->y);
    move_sprite(carSprite->spritids[3], carSprite->x + spritesize, carSprite->y + spritesize);
}

//Set up the player character's sprite and location
void SetupPlayerChar(UINT8 x, UINT8 y)
{
    playerCar.x = x;
    playerCar.y = y;
    playerCar.width = 16;
    playerCar.height = 16;

    //Load sprite tiles for the player
    set_sprite_tile(0, 0);
    playerCar.spritids[0] = 0;

    set_sprite_tile(1, 1);
    playerCar.spritids[1] = 1;

    set_sprite_tile(2, 2);
    playerCar.spritids[2] = 2;

    set_sprite_tile(3, 3);
    playerCar.spritids[3] = 3;

    MoveSprite(&playerCar, playerCar.x, playerCar.y);
}

//Generate the cars that will be moving down the screen
UINT8 GenerateCarPosition()
{
    //UINT8 result = bitset<8>(rand()|START_MASK) ;
    //result |= START_MASK;
    //return (rand()|START_MASK);

    UINT8 result = CAR_POSITIONS[cp_index];
    cp_index += 1;

    if(cp_index >= 25) cp_index = 0;

    return result;
}

// Set up other 6 cars on the road
void SetupOtherCars()
{
    UINT8 i = 0;
    UINT8 currentSprite = 6;

    //Set up Left Lane Cars
    while(i < 3)
    {
        OtherCars[i].x = LANES[i];
        OtherCars[i].y = GenerateCarPosition();
        OtherCars[i].width = 16;
        OtherCars[i].height = 16;

        set_sprite_tile(currentSprite, 4);
        OtherCars[i].spritids[0] = currentSprite;
        set_sprite_prop(currentSprite, S_PRIORITY);
        currentSprite++;

        set_sprite_tile(currentSprite, 5);
        OtherCars[i].spritids[1] = currentSprite;
        set_sprite_prop(currentSprite, S_PRIORITY);
        currentSprite++;

        set_sprite_tile(currentSprite, 6);
        OtherCars[i].spritids[2] = currentSprite;
        set_sprite_prop(currentSprite, S_PRIORITY);
        currentSprite++;

        set_sprite_tile(currentSprite, 7);
        OtherCars[i].spritids[3] = currentSprite;
        set_sprite_prop(currentSprite, S_PRIORITY);
        currentSprite++;

        MoveSprite(&OtherCars[i], OtherCars[i].x, OtherCars[i].y);
        i++;
    }

    //Set up Right Lane Cars
    while(i < 6)
    {
        OtherCars[i].x = LANES[i];
        OtherCars[i].y = GenerateCarPosition();
        OtherCars[i].width = 16;
        OtherCars[i].height = 16;

        set_sprite_tile(currentSprite, 0);
        OtherCars[i].spritids[0] = currentSprite;
        set_sprite_prop(currentSprite, S_PRIORITY);
        currentSprite++;

        set_sprite_tile(currentSprite, 1);
        OtherCars[i].spritids[1] = currentSprite;
        set_sprite_prop(currentSprite, S_PRIORITY);
        currentSprite++;

        set_sprite_tile(currentSprite, 2);
        OtherCars[i].spritids[2] = currentSprite;
        set_sprite_prop(currentSprite, S_PRIORITY);
        currentSprite++;

        set_sprite_tile(currentSprite, 3);
        OtherCars[i].spritids[3] = currentSprite;
        set_sprite_prop(currentSprite, S_PRIORITY);
        currentSprite++;

        MoveSprite(&OtherCars[i], OtherCars[i].x, OtherCars[i].y);
        i++;
    }
}

//If the player collides with another car, return true. Else, return false.
BOOLEAN GetCarCollision()
{
    UINT8 i = 0;
    while(i < 6)
    {
        // EXAMPLE COLLISION DETECTION
        // if (rect1.x < rect2.x + rect2.width &&
        //     rect1.x + rect1.width > rect2.x &&
        //     rect1.y < rect2.y + rect2.height &&
        //     rect1.y + rect1.height > rect2.y)
        //         // collision detected!
        
        if(playerCar.x - COLLISION_OFFSET > OtherCars[i].x - OtherCars[i].width + COLLISION_OFFSET &&
            playerCar.x - playerCar.width + COLLISION_OFFSET < OtherCars[i].x - COLLISION_OFFSET &&
            playerCar.y > OtherCars[i].y - OtherCars[i].height &&
            playerCar.y - playerCar.height < OtherCars[i].y)
        {
            return TRUE;
        }

        i++;
    }
    
    return FALSE;
}

//Move the cars down the screen
void MoveCars(UINT8 Speed)
{
    //Move Left cars twice as fast as Right Cars
    INT8 i = 0;
    
    //Left Lane
    while(i < 3)
    {
        //If in threshold, generate new position. Else, move car.
        if(OtherCars[i].y > END_POINT[0] && OtherCars[i].y < END_POINT[1])
        {
            OtherCars[i].y = GenerateCarPosition();
        }
        else OtherCars[i].y += Speed + Speed;

        MoveSprite(&OtherCars[i], OtherCars[i].x, OtherCars[i].y);
        i++;
    }

    //Right Lane
    while(i < 6)
    {
        //If in threshold, generate new position. Else, move car.
        if(OtherCars[i].y > END_POINT[0] && OtherCars[i].y < END_POINT[1])
        {
            OtherCars[i].y = GenerateCarPosition();
        }
        else OtherCars[i].y += Speed;

        MoveSprite(&OtherCars[i], OtherCars[i].x, OtherCars[i].y);
        i++;
    }
}