//The headers
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <iostream>
#include <ctype.h>


//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The button states in the sprite sheet
const int CLIP_STARTGAME_IDLE = 0;
const int CLIP_STARTGAME_HOVER = 1;
const int CLIP_STARTGAME_PRESSED = 2;

const int CLIP_QUITGAME_IDLE = 3;
const int CLIP_QUITGAME_HOVER = 4;
const int CLIP_QUITGAME_PRESSED = 5;


bool show_menu = false;
bool show_maze = false;
bool Border_show_maze = false;

//The surfaces
SDL_Surface *wallBlock = NULL;
SDL_Surface *wallBlockBLACK = NULL;
SDL_Surface *message = NULL;
SDL_Surface *MenuOptionsButtonsSheet = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;

//The event structure
SDL_Event event;

//The color of the font
SDL_Color textColor = { 0, 0, 0 };

//The font that's going to be used
TTF_Font *font = NULL;

//The clip regions of the sprite sheet
SDL_Rect clips[ 6 ];

//The button
class Button
{
    private:
    //The attributes of the button
    SDL_Rect box;

    //The part of the button sprite sheet that will be shown
    SDL_Rect* clip;

    public:
    //Initialize the variables
    Button( int x, int y, int w, int h );

    //Handles events and set the button's sprite region
    void handle_events();

    //Shows the button on the screen
    void show();
};

//The button quit game
class ButtonQuitGame
{
    private:
    //The attributes of the button
    SDL_Rect box;

    //The part of the button sprite sheet that will be shown
    SDL_Rect* clip;

    public:
    //Initialize the variables
    ButtonQuitGame( int x, int y, int w, int h );

    //Handles events and set the button's sprite region
    void handle_eventsQuitGame();

    //Shows the button on the screen
    void showQuitGame();
};

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );
/*
        //If the image was optimized just fine
        if( optimizedImage != NULL )
        {
            //Map the color key
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0xFF, 0xFF, 0xFF );

            //Set all pixels of color R 0xFF, G 0xFF, B 0xFF to be transparent
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }*/
    }

    //Return the optimized image
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

//Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Projekt 2 - Labiryncik", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
background = load_image( "background.png" );

if( background == NULL )
    {
        return false;
    }

wallBlockBLACK = load_image( "wallBlockBLACK.png" );

if( wallBlockBLACK == NULL )
    {
        return false;
    }

wallBlock= load_image( "wallBlock.png" );

if( wallBlock == NULL )
    {
        return false;
    }

    //Load the button sprite sheet
    MenuOptionsButtonsSheet = load_image( "MenuOptionsSheet.gif" );

    //If there was a problem in loading the button sprite sheet
    if( MenuOptionsButtonsSheet == NULL )
    {
        return false;
    }

    //Open the font
    font = TTF_OpenFont( "lazy.ttf", 28 );

    //If there was an error in loading the font
    if( font == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surface
SDL_FreeSurface( wallBlock );
SDL_FreeSurface( wallBlockBLACK );
SDL_FreeSurface( message );
    SDL_FreeSurface( MenuOptionsButtonsSheet );
SDL_FreeSurface( background );

    //Quit SDL
    SDL_Quit();
}

void set_clips()
{
    //Clip the sprite sheet
    clips[ CLIP_STARTGAME_IDLE ].x = 0;
    clips[ CLIP_STARTGAME_IDLE ].y = 0;
    clips[ CLIP_STARTGAME_IDLE ].w = 136;
    clips[ CLIP_STARTGAME_IDLE ].h = 22;

    clips[ CLIP_STARTGAME_HOVER ].x = 136;
    clips[ CLIP_STARTGAME_HOVER ].y = 0;
    clips[ CLIP_STARTGAME_HOVER ].w = 136;
    clips[ CLIP_STARTGAME_HOVER ].h = 22;

    clips[ CLIP_STARTGAME_PRESSED ].x = 272;
    clips[ CLIP_STARTGAME_PRESSED ].y = 0;
    clips[ CLIP_STARTGAME_PRESSED ].w = 136;
    clips[ CLIP_STARTGAME_PRESSED ].h = 22;
    clips[ CLIP_QUITGAME_IDLE ].x = 0;
    clips[ CLIP_QUITGAME_IDLE ].y = 22;
    clips[ CLIP_QUITGAME_IDLE ].w = 136;
    clips[ CLIP_QUITGAME_IDLE ].h = 22;

    clips[ CLIP_QUITGAME_HOVER ].x = 136;
    clips[ CLIP_QUITGAME_HOVER ].y = 22;
    clips[ CLIP_QUITGAME_HOVER ].w = 136;
    clips[ CLIP_QUITGAME_HOVER ].h = 22;

    clips[ CLIP_QUITGAME_PRESSED ].x = 272;
    clips[ CLIP_QUITGAME_PRESSED ].y = 22;
    clips[ CLIP_QUITGAME_PRESSED ].w = 136;
    clips[ CLIP_QUITGAME_PRESSED ].h = 22;

}

Button::Button( int x, int y, int w, int h )
{
    //Set the button's attributes
    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;

    //Set the default sprite
    clip = &clips[ CLIP_STARTGAME_IDLE ];
}

ButtonQuitGame::ButtonQuitGame( int x, int y, int w, int h )
{
    //Set the button's attributes
    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;

    //Set the default sprite
    clip = &clips[ CLIP_QUITGAME_IDLE ];
}

void Button::handle_events()
{
    //The mouse offsets
    int x = 0, y = 0;
    //If the mouse moved
    if( event.type == SDL_MOUSEMOTION )
    {
        //Get the mouse offsets
        x = event.motion.x;
        y = event.motion.y;

        //If the mouse is over the button
        if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
        {
            //Set the button sprite
            clip = &clips[ CLIP_STARTGAME_HOVER ];
        }
        //If not
        else
        {
            //Set the button sprite
            clip = &clips[ CLIP_STARTGAME_IDLE ];
        }
    }
    //If a mouse button was pressed
    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        //If the left mouse button was pressed
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                //Set the button sprite
                clip = &clips[ CLIP_STARTGAME_PRESSED ];
            }
        }
    }
    //If a mouse button was released
    if( event.type == SDL_MOUSEBUTTONUP )
    {
        //If the left mouse button was released
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                //Set the button sprite
                clip = &clips[ CLIP_STARTGAME_PRESSED ];
            }
        }
    }
}

void ButtonQuitGame::handle_eventsQuitGame()
{
    //The mouse offsets
    int x = 0, y = 0;
    //If the mouse moved
    if( event.type == SDL_MOUSEMOTION )
    {
        //Get the mouse offsets
        x = event.motion.x;
        y = event.motion.y;

        //If the mouse is over the button
        if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
        {
            //Set the button sprite
            clip = &clips[ CLIP_QUITGAME_HOVER ];
        }
        //If not
        else
        {
            //Set the button sprite
            clip = &clips[ CLIP_QUITGAME_IDLE ];
        }
    }
    //If a mouse button was pressed
    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        //If the left mouse button was pressed
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                //Set the button sprite
                clip = &clips[ CLIP_QUITGAME_PRESSED ];
            }
        }
    }
    //If a mouse button was released
    if( event.type == SDL_MOUSEBUTTONUP )
    {
        //If the left mouse button was released
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                //Set the button sprite
                clip = &clips[ CLIP_QUITGAME_PRESSED ];
            }
        }
    }
}

void Button::show()
{
    //Show the button
    apply_surface( box.x, box.y, MenuOptionsButtonsSheet, screen, clip );
}

void ButtonQuitGame::showQuitGame()
{
    //Show the button
    apply_surface( box.x, box.y, MenuOptionsButtonsSheet, screen, clip );
}

void Menu_show()
{
if ( show_menu == true )
    {
apply_surface( 0, 0, background, screen );
apply_surface( 0, 250, message, screen );
}
}

void Maze_show(int x, int y)
{
if ( show_maze == true )
    {
  
//apply_surface( 0, 0, background, screen );
apply_surface( x, y, wallBlock, screen );

  
}
}
void Border_Maze_show(int x, int y)
{
if ( Border_show_maze == true )
    {
  
//apply_surface( 0, 0, background, screen );
apply_surface( x, y, wallBlockBLACK, screen );

  
}
}
void Maze_print(int max_x, int max_y, int tab[100][100]){
	
int x=10;
for (int i=1; i<=max_x; i++){
x=x+11;
int y=10;
for (int j=1; j<=max_y;j++){
y=y+11;
if (tab[i][j]==1) Maze_show(x, y);
}
}
}
void Border_Maze_print(int max_x, int max_y, int tab[100][100]){
	
int x=0;
for (int i=0; i<=(max_x+1); i++){
x=x+11;
int y=0;
for (int j=0; j<=(max_y+1);j++){
y=y+11;
Border_Maze_show(x, y);
}
}
}

void Maze_generate(int max_x, int max_y, int tab[100][100]){
int i;
int j;
int x=10;
int y=10;
for (i=1; i<=max_x; i++){
for (j=1; j<=max_y;j++){
tab[i][j] = 1;
}
}
max_x=max_x-1;
max_y=max_y-1;
i = rand() % max_x +1;
j = rand() % max_y +1;
tab[i][j] = 0;
}

int main( int argc, char* args[] )
{

srand ( time(NULL) );

int max_x = 9; 
int max_y = 9; 
//int const wiersz = 30,kolumna= 30;
int tab[100][100];
Maze_generate(max_x, max_y, tab);
    //Quit flag
    bool quit = false;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

    //Clip the sprite sheet
    set_clips();

    //Make the button
    Button StartGameBtn( 254, 229, 136, 22 );
ButtonQuitGame QuitGameBtn( 259, 255, 136, 22 );
	// Labiryncik tworzenie


    //While the user hasn't quit
    while( quit == false )
    {


        //If there's events to handle
        if( SDL_PollEvent( &event ) )
        {
            //Handle button events
            StartGameBtn.handle_events();
QuitGameBtn.handle_eventsQuitGame();

//The mouse offsets
int x = 0, y = 0;
if( event.type == SDL_MOUSEBUTTONDOWN )
{
//If the left mouse button was pressed
if( event.button.button == SDL_BUTTON_LEFT )
{
//Get the mouse offsets
x = event.button.x;
y = event.button.y;

//If the mouse is over the Start Game button 
if( ( x > 254 ) && ( x < 390 ) && ( y > 229 ) && ( y < 251 ) )
{
//Set the button sprite
show_menu = true;
show_maze = true;
Border_show_maze = true;
}
if( ( x > 254 ) && ( x < 390 ) && ( y > 255 ) && ( y < 272 ) )
{
//Set the button sprite
quit = true;
}

}
    
}


            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }

        //Apply the background
        apply_surface( 0, 0, background, screen );

message = TTF_RenderText_Solid( font, "Lorem ipsum dolor", textColor );
   
//Show the button
        StartGameBtn.show();
QuitGameBtn.showQuitGame();
Menu_show();
Maze_print(max_x, max_y, tab);
Border_Maze_print(max_x, max_y, tab);
/*
int x = 10, y = 10, c=1, size=20;
Maze_show(x, y);
for (c=1; c<=size;c++){
x=x+10;
y=y;
  
Maze_show(x, y);
}
  
for (c=1; c<=size;c++){
x=x;
y=y+10;
  
Maze_show(x, y);
}

x = 10, y = 10;
for (c=1; c<=size;c++){
x=x;
y=y+10;
  
Maze_show(x, y);
}

for (c=1; c<=size;c++){
x=x+10;
y=y;
  
Maze_show(x, y);
}
int max_x=x-10;
y=y-10;
for (y; y>=10; y=y-10){

for (x=max_x; x>=10; x=x-10){
Maze_show(x, y);
}
}
*/
        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }


    }

    //Clean up
    clean_up();

    return 0;
}