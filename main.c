/*
 * Avian Simulator
 * By: Cody
*/
#include <gb/gb.h>
#include "sprites.c"
#include "BackgroundTilleSet.c"
#include "BackgroundTileMap.c"
#include "GameCharacter.c"
#include <stdio.h>

GameCharacter bird;
GameCharacter glass;
GameCharacter bread;
UBYTE spritesize = 8;
uint8_t score = 0;
BOOLEAN gameOver = 0;
uint8_t breads = 0;
uint8_t energy = 10;

UBYTE CheckCollisions(GameCharacter* one, GameCharacter* two)
{
    return (one->x >= two->x && one->x <= two->x + two->width) && (one->y >= two->y && one->y <= two->y + two->height) || (two->x >= one->x && two->x <= one->x + one->width) && (two->y >= one->y && two->y <= one->y + one->height);
}

void MoveGameCharacter(GameCharacter* character, uint8_t x, uint8_t y)
{
    move_sprite(character->spritids[0], x, y);
    move_sprite(character->spritids[1], x + spritesize, y);
    move_sprite(character->spritids[2], x, y + spritesize);
    move_sprite(character->spritids[3], x + spritesize, y + spritesize);
}

void GlassMove(GameCharacter* character, uint8_t x, uint8_t y)
{
    move_sprite(character->spritids[0], x, y);
    move_sprite(character->spritids[1], x, y + spritesize);
}

void SetupBird()
{
    bird.x = 20;
    bird.y = 80;
    bird.width = 16;
    bird.height = 16;

    set_sprite_tile(0,0);
    bird.spritids[0] = 0;
    set_sprite_tile(1,1);
    bird.spritids[1] = 1;
    set_sprite_tile(2,2);
    bird.spritids[2] = 2;
    set_sprite_tile(3,3);
    bird.spritids[3] = 3;

    MoveGameCharacter(&bird, bird.x, bird.y);
}

void SetupGlass()
{
    glass.x = 180;
    glass.y = bird.y;
    glass.width = 8;
    glass.height = 16;

    set_sprite_tile(4,4);
    glass.spritids[0] = 4;
    set_sprite_tile(5,4);
    glass.spritids[1] = 5;
    /*set_sprite_prop(5,S_FLIPX);*/
    set_sprite_prop(5,S_FLIPY);

    GlassMove(&glass, glass.x, glass.y);
}

void SetupBread()
{
    bread.x = 180;
    bread.y = 0;
    bread.width = 8;
    bread.height = 8;

    set_sprite_tile(7,5);
    bread.spritids[0] = 67;
    move_sprite(bread.spritids[0], bread.x, bread.y);
}

void JoypadInput()
{
    if (gameOver == 0)
    {
        if (joypad() & J_UP){
            bird.y -= 2;
            MoveGameCharacter(&bird, bird.x, bird.y);
        }
        if (joypad() & J_DOWN){
            bird.y += 2;
            MoveGameCharacter(&bird, bird.x, bird.y);
        }
    }
    else
    {   
        bird.y += 2;
        MoveGameCharacter(&bird, bird.x, bird.y);
    }
}

void main() 
{
    SPRITES_8x8;
    set_sprite_data(0,7, sprites);
    SetupBird();
    SetupGlass();
    set_bkg_data(0,6, BackgroundTileSet);
    set_bkg_tiles(0,0, BackgroundTileMapWidth, BackgroundTileMapHeight, BackgroundTileMap);

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;

    while (1)
    {
        scroll_bkg(1,0);
        JoypadInput();

        glass.x -= 2;
    
        if (glass.x <= 0)
        {
            score += 1;
            glass.x = 180;
            glass.y = bird.y;
        }
        if (gameOver == 0){
        GlassMove(&glass, glass.x, glass.y);
        }

        bread.x -= 1;

        if (bread.x <= 0)
        {
            bread.x = 180;
            bread.y = bird.y + 8;
        }

        move_sprite(bread.spritids[0], bread.x, bread.y);

        if (CheckCollisions(&bird, &glass))
        {
            bird.y += 2;
            gameOver = 1;
        }

        if (CheckCollisions(&bird, &bread))
        {
            breads += 1;
        }

        /* TODO: Bread that gives you more energy */
        /*energy -= 1;*/
        wait_vbl_done();
    }
}