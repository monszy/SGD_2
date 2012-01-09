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
#include <math.h>
#include <vector>
#include <deque>
#include <stack>


using namespace std;



//Screen attributes
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

//The button states in the sprite sheet
const int CLIP_STARTGAME_IDLE = 0;
const int CLIP_STARTGAME_HOVER = 1;
const int CLIP_STARTGAME_PRESSED = 2;

const int CLIP_QUITGAME_IDLE = 3;
const int CLIP_QUITGAME_HOVER = 4;
const int CLIP_QUITGAME_PRESSED = 5;


//The frame rate
const int FRAMES_PER_SECOND = 30;

//The dimensions of the dot
const int DOT_WIDTH = 10;
const int DOT_HEIGHT = 10;


bool resetBackground = false;
bool show_maze = false;
bool Border_show_maze = false;
bool var_arrows_show = false;
bool var_show_dot = false;

//The surfaces
SDL_Surface *dot = NULL;
SDL_Surface *yAxis = NULL;
SDL_Surface *xAxis = NULL;
SDL_Surface *pickSize = NULL;
SDL_Surface *upArrow = NULL;
SDL_Surface *downArrow = NULL;
SDL_Surface *wallBlock = NULL;
SDL_Surface *wallBlockBLACK = NULL;
SDL_Surface *message = NULL;
SDL_Surface *MenuOptionsButtonsSheet = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;
SDL_Rect wall;
SDL_Rect walls[5000];
//The event structure
SDL_Event event;

//The color of the font
SDL_Color textColor = { 0, 0, 0 };

//The font that's going to be used
TTF_Font *font = NULL;

//The clip regions of the sprite sheet
SDL_Rect clips[ 6 ];


//The dot that will move around on the screen
class Dot
{
    private:
    //The X and Y offsets of the dot
    SDL_Rect Mydot;

    //The velocity of the dot
    int xVel, yVel;

    public:
    //Initializes the variables
    Dot();

    //Takes key presses and adjusts the dot's velocity
    void handle_input();

    //Moves the dot
    void move(int max_x, int max_y);

    //Shows the dot on the screen
    void show();
};

//The timer
class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};

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

    //Load the dot image
    dot = load_image( "dot.png" );

    //If there was a problem in loading the dot
   if( dot == NULL )
   {
       return false;
    }

	background = load_image( "background2.png" );

	if( background == NULL )
		{
			return false;
		}

	pickSize = load_image( "pickSize.gif" );

	if( pickSize == NULL )
		{
			return false;
		}

	xAxis = load_image( "xAxis.gif" );

	if( xAxis == NULL )
		{
			return false;
		}

	yAxis = load_image( "yAxis.gif" );

	if( yAxis == NULL )
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

	upArrow= load_image( "upArrow2.gif" );

	if( upArrow == NULL )
		{
			return false;
		}

	downArrow= load_image( "downArrow2.gif" );

	if( downArrow == NULL )
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
bool check_collision( SDL_Rect A, SDL_Rect B ) { 
	//The sides of the rectangles 
	int leftA, leftB; 
	int rightA, rightB; 
	int topA, topB; 
	int bottomA, bottomB; 
	//Calculate the sides of rect A 
	leftA = A.x; 
	rightA = A.x + A.w; 
	topA = A.y; 
	bottomA = A.y+ A.h; 
	//Calculate the sides of rect B 
	leftB = B.x; 
	rightB = B.x + B.w;
	topB = B.y; 
	bottomB = B.y + B.h;
	//If any of the sides from A are outside of B 
	if( bottomA <= topB ) { return false; } 
	if( topA >= bottomB ) { return false; } 
	if( rightA <= leftB ) { return false; } 
	if( leftA >= rightB ) { return false; } 
	//If none of the sides from A are outside B 
	return true; 
}

Dot::Dot()
{
    //Initialize the offsets
    Mydot.x = 21;
    Mydot.y = 21;
	Mydot.w = DOT_WIDTH;
	Mydot.h = DOT_HEIGHT;
    //Initialize the velocity
    xVel = 0;
    yVel = 0;
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

void Dot::handle_input()
{
    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: (yVel -= DOT_HEIGHT+1) / 2;break;
            case SDLK_DOWN: (yVel += DOT_HEIGHT+1) / 2;  break;
            case SDLK_LEFT: (xVel -= DOT_WIDTH+1) / 2;   break;
            case SDLK_RIGHT: (xVel += DOT_WIDTH+1) / 2;  break;
				
        }
		Sleep(100);
    }
    //If a key was released 
    else if( event.type == SDL_KEYUP )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: (yVel += DOT_HEIGHT+1) / 2; break;
            case SDLK_DOWN: (yVel -= DOT_HEIGHT+1) / 2; break;
            case SDLK_LEFT: (xVel += DOT_WIDTH+1) / 2; break;
            case SDLK_RIGHT: (xVel -= DOT_WIDTH+1) / 2; break;
        }
		Sleep(100);
    }
}

void Dot::move(int max_x, int max_y)
{
    //Move the dot left or right
    Mydot.x += xVel;

    //If the dot went too far to the left or right
	for (int i=0; i<5000;i++){
    if( ( Mydot.x < 20 ) || ( Mydot.x + DOT_WIDTH > (max_x*11)+21 ) || check_collision(Mydot,walls[i]) )
    {
        //move back
        Mydot.x -= xVel;
    }
	}
    //Move the dot up or down
    Mydot.y += yVel;
	for (int i=0; i<5000;i++){
    //If the dot went too far up or down
    if( ( Mydot.y < 20 ) || ( Mydot.y + DOT_HEIGHT > (max_y*11)+21 ) || check_collision(Mydot,walls[i]) )
    {
        //move back
        Mydot.y -= yVel;
    }
	}
}
void ApplySurface(SDL_Rect &Rect, SDL_Surface* Source, SDL_Surface* Destination)
{  
    SDL_BlitSurface(Source, NULL, Destination, &Rect);
    return;
}

void Dot::show()
{
	if ( var_show_dot == true )
    {
		//Show the dot
		ApplySurface(Mydot,dot,screen);
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

Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

void resetBackground_show()
{

	if ( resetBackground == true )
    {
		apply_surface( 0, 0, background, screen );
		//apply_surface( 190, 0, message, screen );
	}

}

void Maze_show(int x, int y)
{

	if ( show_maze == true )
    {
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

void Maze_print(int max_x, int max_y, int tab[100][100])
{

	int x=10;

	for (int i=1; i<=max_x; i++)
	{
		x=x+11;
		int y=10;

		for (int j=1; j<=max_y;j++)
		{
			y=y+11;
			if (tab[i][j]==1) 
				Maze_show(x, y);
		}
	}

}

void Border_Maze_print(int max_x, int max_y, int tab[100][100])
{

	int y=10;
	int x=10;

	Border_Maze_show(x, y);

	for (int i=0; i<=(max_x); i++)
	{
		x=x+11;
		Border_Maze_show(x, y);
	}

	for (int j=0; j<=(max_y);j++)
	{
		y=y+11;
		Border_Maze_show(x, y);
	}

	y=10; x=10;

	for (int j=0; j<=(max_y);j++)
	{
		y=y+11;
		Border_Maze_show(x, y);
	}

	for (int i=0; i<=(max_x); i++)
	{
		x=x+11;
		Border_Maze_show(x, y);
	}

}
	
void Maze_generate(int max_x, int max_y, int tab[100][100]){
	
int round = 1;
int i;
int j;
int w=0;
int g=40;
int x=10;
int y=10;
int strona;
int call=((max_x+4)*(max_y+4));
stack<int> back_x;
stack<int> back_y;
while (!back_x.empty())
  {
	  back_x.pop();
}

while (!back_y.empty())
{
   back_y.pop();
}

//clear walls
for (w; w<=500;w++){
	walls[w].x = 0;
	walls[w].y = 0;
	walls[w].h = 0;
	walls[w].w = 0;

}
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


back_x.push(i), back_y.push(j);
for (call; call >0; call--){
	
strona = rand() % 4 +1;

if ((i == 1 || i == 2 ||tab[i-1][j] == 0||tab[i-2][j] == 0) && 
 (i == (max_x+1) || i == max_x ||tab[i+1][j] == 0||tab[i+2][j] == 0)&& 
 (j == 1 || j == 2 || tab[i][j-1] == 0||tab[i][j-2] == 0 )&& 
 (j == (max_y+1) || j == max_y || tab[i][j+1] == 0||tab[i][j+2] == 0)){
if ((back_x.size() !=0 ) || (back_y.size() != 0)){
i=back_x.top(); j=back_y.top();
back_x.pop();back_y.pop();
}
}
else 
if ( strona==1 && (i>1 && tab[i-1][j] == 1 && tab[i-2][j] == 1 ))   {i=i-1; tab[i][j] = 0; i=i-1; tab[i][j] = 0; back_x.push(i); back_y.push(j); call--; } else          // left
if ( strona==2 && (i<max_x+1 && tab[i+1][j] == 1 && tab[i+2][j] == 1)) {i=i+1; tab[i][j] = 0; i=i+1; tab[i][j] = 0; back_x.push(i); back_y.push(j); call--; } else     // right
if ( strona==3 && (j>1 && tab[i][j-1] == 1 && tab[i][j-2] == 1))  {j=j-1; tab[i][j] = 0; j=j-1; tab[i][j] = 0;back_x.push(i); back_y.push(j);call--; } else        // up
if ( strona==4 && (j<max_y+1 &&tab[i][j+1] == 1 && tab[i][j+2] == 1)) { j=j+1; tab[i][j] = 0; j=j+1; tab[i][j] = 0; back_x.push(i); back_y.push(j); call--; } ;   // down
}
//cykle
while(tab[i][j]==0 || i > max_x || j > max_y || (((tab[i-1][j] == 0) && (tab[i+1][j] == 0)) && ((tab[i][j-1] == 1) && (tab[i][j+1] == 1)))|| ((tab[i-1][j] == 0)&&(tab[i+1][j] == 1)) || ((tab[i+1][j] == 0)&&(tab[i-1][j] == 1))|| ((tab[i][j-1] == 0)&&(tab[i][j+1] == 1)) || ((tab[i][j+1] == 0)&&(tab[i][j-1] == 1))   ){
i = rand() % max_x +2;
j = rand() % max_y +2;
}
tab[i][j] = 0;
w=0;
	for (i=1; i<=max_x+1; i++){
		
	for (j=1; j<=max_y+1;j++){
		
	if(tab[i][j] == 1){
	w++;
	walls[w].x = 11+(11*i);
	walls[w].y = 11+(11*j);
	walls[w].h = 10;
	walls[w].w = 10;
	}
	}
	}
}



void arrows_show()
{

	if ( var_arrows_show == true )
    {
		apply_surface( 700, 5, pickSize, screen );
		//horizontal size arrows
		apply_surface( 700, 30, downArrow, screen );
		apply_surface( 778, 30, upArrow, screen );
		//vertical size arrows
		apply_surface( 700, 60, downArrow, screen );
		apply_surface( 778, 60, upArrow, screen );

		apply_surface( 723, 33, xAxis, screen );
		apply_surface( 723, 63, yAxis, screen );

	}
}



int main( int argc, char* args[] )
{

	srand ( time(NULL) );
	
	//rozmiar labiryntu
	int max_x = 15;
	int max_y = 10; 

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

    //The dot that will be used
    Dot myDot;

    //The frame rate regulator
    Timer fps;

    //Make the button
    Button StartGameBtn( 345, 229, 136, 22 );
	ButtonQuitGame QuitGameBtn( 350, 255, 136, 22 );


    //While the user hasn't quit
    while( quit == false )
    {

        //Start the frame timer
        fps.start();

        //If there's events to handle
        if( SDL_PollEvent( &event ) )
        {

            //Handle events for the dot
            myDot.handle_input();

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
					if( ( x > 345 ) && ( x < 481 ) && ( y > 229 ) && ( y < 251 ) )
					{
						//Set the button sprite
						resetBackground = true;
						var_arrows_show = true;
						var_show_dot = true;
					}

					//If the mouse is over the Quit Game button
					if( ( x > 350 ) && ( x < 486 ) && ( y > 255 ) && ( y < 277 ) )
					{
						//Set the button sprite
						quit = true;
					}

					if (var_arrows_show == true)
					{
							resetBackground = true;
							show_maze = true;
							Border_show_maze = true;
						
						//Horizontal Pickers
						if( ( x > 700 ) && ( x < 720 ) && ( y > 30 ) && ( y < 55 ) )
						{							
							max_x = max_x - 1;					

							if( max_x == 3 )
								max_x++;

							Maze_generate(max_x, max_y, tab);
						}
						if( ( x > 778 ) && ( x < 798 ) && ( y > 30 ) && ( y < 55 ) )
						{
							
							max_x = max_x + 1;

							if( max_x == 58 )
								max_x--;

							Maze_generate(max_x, max_y, tab);
						}
						//Vertical Pickers
						if( ( x > 700 ) && ( x < 720 ) && ( y > 60 ) && ( y < 85 ) )
						{
							max_y = max_y - 1;

							if( max_y == 3 )
								max_y++;

							Maze_generate(max_x, max_y, tab);
						}
						if( ( x > 778 ) && ( x < 798 ) && ( y > 60 ) && ( y < 85 ) )
						{
							max_y = max_y + 1;

							if( max_y == 52 )
								max_y--;

							Maze_generate(max_x, max_y, tab);
						}
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

		message = TTF_RenderText_Solid( font, "Lorem ipsum dolor" , textColor );
 


		//Show the button
        StartGameBtn.show();
		QuitGameBtn.showQuitGame();

		resetBackground_show();
	    //Show the wall 
		for (int i =0; i<(max_x*max_y); i++)
		SDL_FillRect( screen, &walls[i], SDL_MapRGB( screen->format, 0x77, 0x77, 0x77 ) );
        //Move the dot
        myDot.move(max_x, max_y);
        //Show the dot on the screen
        myDot.show();

		arrows_show();
		
		//Maze_print(max_x, max_y, tab);
		Border_Maze_print(max_x, max_y, tab);


        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }

        //Cap the frame rate
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }


    }

    //Clean up
    clean_up();

    return 0;
}