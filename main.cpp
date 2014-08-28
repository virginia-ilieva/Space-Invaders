/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

#include "Texture.h"
#include "Bullet.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 680;

const int P_BULLETS = 1;
const int E_BULLETS = 3;

enum direction_t {left, right, stationary};
enum state_t {start, game, game_over};
state_t game_state;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;    
//Globally used font
TTF_Font *gFont = NULL;
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The sound effects that will be used
Mix_Chunk *gShipBullet = NULL;
Mix_Chunk *gShipHit = NULL;
Mix_Chunk *gInvaderBullet = NULL;
Mix_Chunk *gInvaderHit = NULL;

//Scene sprites
Texture gSpriteSheetTexture;
// Background
Texture gBackgroundTexture;
//Text texture
Texture gTitleTexture;
Texture gOtherTextTexture;
Texture gGameInfo;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

bool init()
{
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return false;
	} 

	//Set texture filtering to linear
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
		printf( "Warning: Linear texture filtering not enabled!" );
		return false;
	}

	//Create window
	gWindow = SDL_CreateWindow( "Space Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if( gWindow == NULL ) {
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}

	//Create renderer for window
	gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
	if( gRenderer == NULL ) {
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	} 

	//Initialize renderer color
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		return false;
	} 

	//Initialize SDL_ttf
	if( TTF_Init() == -1 ) {
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		return false;
	} 

	//Get window surface
	gScreenSurface = SDL_GetWindowSurface( gWindow );
	if ( SDL_Init( SDL_INIT_AUDIO ) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	} 

	//Initialize SDL
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 < 0 )){
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		return false;
	} 			

	//Initialize SDL_mixer
	if (!loadMedia() ){
		printf( "Failed to load media!\n" );
		return false;
	}

	return true;
}

//Detect any collision between any two  rectangles
int checkCollision(SDL_Rect a, SDL_Rect b) {

	//The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
    if( bottomA <= topB ){
        return false;
    }

    if( topA >= bottomB ){
        return false;
    }

    if( rightA <= leftB ){
        return false;
    }

    if( leftA >= rightB ){
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

//Load sprite sheet texture and sound FX
bool loadMedia(){
	
	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( "img/space-invaders.png", gRenderer ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		return false;
	}

	// Load background
	if( !gBackgroundTexture.loadFromFile( "img/background.png", gRenderer ) )
	{
		printf( "Failed to load backgrund!\n" );
		return false;
	}
   
	//Load sound effects
    gShipBullet = Mix_LoadWAV( "soundFX/ShipBullet.wav" );
    if( gShipBullet == NULL )
    {
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }
    
    gShipHit= Mix_LoadWAV( "soundFX/ShipHit.wav" );
    if( gShipHit == NULL )
    {
        printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }

    gInvaderBullet = Mix_LoadWAV( "soundFX/InvaderBullet.wav" );
    if( gInvaderBullet == NULL )
    {
        printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }

    gInvaderHit = Mix_LoadWAV( "soundFX/InvaderHit.wav" );
    if( gInvaderHit == NULL )
    {
        printf( "Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }

	return true;
} 

// *** Player Class ***

const int P_WIDTH = 70;
const int P_HEIGHT = 35;

struct player_t {
	SDL_Rect hitbox;
	int lives;
	int score;
};

struct player_t player;

//Initialize the player starting position and dimensions
void init_player() {

	player.hitbox.x = (SCREEN_WIDTH / 2) - (P_WIDTH / 2);
	player.hitbox.y = SCREEN_HEIGHT - (P_HEIGHT + 10);
	player.hitbox.w = P_WIDTH;
	player.hitbox.h = P_HEIGHT;
	player.lives = 3;
}

//Draw the player
void draw_player(SDL_Rect playerLoc) {

	SDL_Rect src;

	src.x = 10;
	src.y = 480;
	src.w = P_WIDTH;
	src.h = P_HEIGHT;
	gSpriteSheetTexture.render( playerLoc.x, playerLoc.y, gRenderer, &src );

}

//Move player left or right
void move_player(enum direction_t direction) {

	if (direction == left) {
		if (player.hitbox.x > 0) {
			player.hitbox.x -= 5;
		}
	} else if (direction == right) {
		if (player.hitbox.x + player.hitbox.w < SCREEN_WIDTH){
			player.hitbox.x += 5;
		}
	}
}

//Shoot bullet/s from player
void player_shoot(Bullet bullets[]) {

	int i;

	for (i = 0; i < P_BULLETS; i++) {
		if (bullets[i].getBulletAlive() == 0) {
			SDL_Rect hitbox = bullets[i].getHitbox();
			int x = (player.hitbox.x + (P_WIDTH / 2)) - bullets[i].getWidth() / 2;
			int y = player.hitbox.y - (hitbox.h);
			bullets[i].setHitbox(x, y);
			bullets[i].setBulletAlive(1);
			Mix_PlayChannel( -1, gShipBullet, 0 );
			break;
		}
	}
}

//Look for collisions based on enemy bullet and player rectangles
void player_hit_collision(Bullet e_bullets[]) {

	int i;
	bool collision;

	for(i = 0; i < E_BULLETS; i++) {	
		if (e_bullets[i].getBulletAlive() == 1) {
			collision = checkCollision(e_bullets[i].getHitbox(), player.hitbox);
			if (collision) {				
				if (player.lives >= 0) {	
					e_bullets[i].setBulletAlive(0);
					player.lives--;
					Mix_PlayChannel( -1, gShipHit, 0 );
				}
			}
		}
	}
}

// *** END Player Class ***

// *** Enemy Class ***

const int E_WIDTH = 60;
const int E_HEIGHT = 65;

struct enemy_t {

	SDL_Rect hitbox;
	unsigned int alive;

};

// *** END Enemy Class ***

// *** Invaders Class ***

struct invaders_t {

	struct enemy_t enemy[5][10];
	enum direction_t direction;
	unsigned int killed;
	int speed;
	unsigned int points;
};

struct invaders_t invaders;

//Initialize the enemies starting positions, direction, speed and colour
void init_invaders() {

	invaders.direction = right;
	invaders.speed = 1;
	invaders.killed = 0;

	int i,j;
	int x = 0;
	int y = 60;

	for (i = 0; i < 5; i++) {

		for (j = 0; j < 10; j++) {

			invaders.enemy[i][j].alive = 1;
			invaders.enemy[i][j].hitbox.x = x;
			invaders.enemy[i][j].hitbox.y = y;
			invaders.enemy[i][j].hitbox.w = E_WIDTH;
			invaders.enemy[i][j].hitbox.h = E_HEIGHT;

			x += E_WIDTH; //location for the next invader

			invaders.points = 10;
		}

		x = 0; //reset line
		y += E_HEIGHT;
	}
}

//Draw the invaders if there alive
void draw_invaders() {

	SDL_Rect src, dest;
	int i,j;
	
	src.x = 241;
	src.y = 345;
	src.w = E_WIDTH;
	src.h = E_HEIGHT;

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 10; j++) {
			if (invaders.enemy[i][j].alive == 1) {				
				dest.x = invaders.enemy[i][j].hitbox.x;
				dest.y = invaders.enemy[i][j].hitbox.y;
				dest.w = invaders.enemy[i][j].hitbox.w;
				dest.h = invaders.enemy[i][j].hitbox.h;
				
				gSpriteSheetTexture.render( dest.x, dest.y, gRenderer, &src );
			}
		}
	}
}

//Set invader movment speed
void set_invaders_speed() {
	
	// Changes the speed of the invaders every minute
	if ( SDL_GetTicks() > 1000 * 60 && SDL_GetTicks() < 2000 * 60 ){
		invaders.speed = 2;
	} else if ( SDL_GetTicks() > 2000 * 60 && SDL_GetTicks() < 3000 * 60 ){
		invaders.speed = 4;
	} else if ( SDL_GetTicks() > 3000 * 60 && SDL_GetTicks() < 4000 * 60 ){
		invaders.speed = 8;
	} else if ( SDL_GetTicks() > 4000 * 60 ) {
		invaders.speed = 16;
	}
	// Invaders points are based on the invaderes speed
	if (invaders.speed > 1 ){
		invaders.points = 10 * invaders.speed;
	}
}

//Move invaders down one space once the reach the edge
void move_invaders_down() {

	int i,j;

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 10; j++) {
			invaders.enemy[i][j].hitbox.y += 15;
		}
	}
}

//Move invaders based on there current direction
int move_invaders(int speed) {

	set_invaders_speed();

	int i,j;

	switch (invaders.direction) {
		case left:
			for (i = 0; i < 10; i++) {
				for (j = 0; j < 5; j++) {
					if (invaders.enemy[j][i].alive == 1) {
						if (invaders.enemy[j][i].hitbox.x <= 0) {
							invaders.direction = right;
							move_invaders_down();
							return 0;
						}
						//move invader speed number of pixels
						invaders.enemy[j][i].hitbox.x -= invaders.speed;
					}
				}
			}
			break;

		case right:
			for (i = 9; i >= 0; i--) {
				for (j = 0; j < 5; j++) {
					if (invaders.enemy[j][i].alive == 1) {
						if (invaders.enemy[j][i].hitbox.x + E_WIDTH >= SCREEN_WIDTH) {
							invaders.direction = left;
							move_invaders_down();
							return 0;
						}
						invaders.enemy[j][i].hitbox.x += invaders.speed;
					}
				}
			}
			break;

		default:
		break;
	}
	return 0;
}

// Gets the number of alive invaders
int get_alive_invaders(){

	int i,j;
	int alive = 0;
	
	for (i = 0; i < 5; i++) {		
		for (j = 0; j < 10; j++) {			
			if (invaders.enemy[i][j].alive == 1) {
				alive ++;						
			}
		}
	}
	return alive;
}

//Look for collisions based on player bullet and invader rectangles
void enemy_hit_collision(Bullet bullets[]) {

	int i,j,k;
	bool collision;
	
	for (i = 0; i < 5; i++) {		
		for (j = 0; j < 10; j++) {			
			if (invaders.enemy[i][j].alive == 1) {			
				for (k = 0; k < P_BULLETS; k++) {			
					if (bullets[k].getBulletAlive() == 1) {						
						collision = checkCollision(bullets[k].getHitbox(), invaders.enemy[i][j].hitbox);				
						if (collision) {				
							invaders.enemy[i][j].alive = 0;
							bullets[k].setBulletAlive(0);
							bullets[k].setHitbox(0, 0);
							invaders.killed++;
							Mix_PlayChannel( -1, gInvaderHit, 0 );
							player.score += invaders.points;
						}
					}
				}
			}
		}
	}
}

//Look for collisions based on invader and player rectangles
bool enemy_player_collision() {

	int i,j;
	bool collision;

	for(i = 0; i < 5; i++) {
		for(j = 0; j < 10; j++) {		
			if (invaders.enemy[i][j].alive == 1) {					
				collision = checkCollision(player.hitbox, invaders.enemy[i][j].hitbox);
				if (collision) {
					return true;
				}
			}
		}
	}
	return false;
}

//Determine when invaders should shoot
void enemy_shoot(Bullet e_bullets[]) {

	int i, j, k;

	for (i = 0; i < 10; i++) {		
		for (j = 4; j >= 0; j--) {			
			if (invaders.enemy[j][i].alive == 1) {				
				//player
				int mid_point = player.hitbox.x + (player.hitbox.w / 2);
				
				//enemy
				int start = invaders.enemy[j][i].hitbox.x;
				int end = invaders.enemy[j][i].hitbox.x + invaders.enemy[j][i].hitbox.w;

				// checks if the player is in the enemy hit zone
				if (mid_point > start && mid_point < end) {

					//fire bullet if available
					for (k = 0; k < E_BULLETS; k++) {			
						if (e_bullets[k].getBulletAlive() == 0) {				
							int r = rand() % 30;

							if (r == 1) {
								SDL_Rect hitbox = e_bullets[k].getHitbox();
								int x = (start + (E_WIDTH / 2)) - E_WIDTH / 2 ;
								int y = invaders.enemy[j][i].hitbox.y + E_WIDTH;
								e_bullets[k].setHitbox(x, y);
								e_bullets[k].setBulletAlive(1);					
								Mix_PlayChannel( -1, gInvaderBullet, 0 );
							}
							break;
						}
					}
				}				 
				break;
			}
		}
	}
}

// *** END Invaders Class ***

//Determine for game over event
void game_over_ai() {
	
	if (player.lives == 0 || enemy_player_collision() || get_alive_invaders() == 0 ) {		
		game_state = game_over;
	} 
}

void close()
{
	//Free loaded images
	gSpriteSheetTexture.free();
	gTitleTexture.free();
	gOtherTextTexture.free();
	gGameInfo.free();

	//Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;

	//Free the sound effects
    Mix_FreeChunk( gShipBullet );
    Mix_FreeChunk( gShipHit );
    Mix_FreeChunk( gInvaderBullet );
    Mix_FreeChunk( gInvaderHit );
    gShipBullet = NULL;
    gShipHit = NULL;
    gInvaderBullet = NULL;
    gInvaderHit = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if ( !init() ) {
		printf( "Failed to initialize!\n" );
	} else {
		//Initiate objects
		init_player();
		init_invaders();
 		Bullet bullets[P_BULLETS];
		Bullet e_bullets[E_BULLETS];

		game_state = start;		
		SDL_Color textColor = { 255, 255, 255 };

		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		//While application is running
		while ( !quit ) {
			// Set the render draw color 
			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0); 
			SDL_RenderClear(gRenderer);
			Uint32 next_game_tick = SDL_GetTicks();
			int sleep = 0;

			//Handle events on queue
			while ( SDL_PollEvent( &e ) != 0 ) {
				//User requests quit
				if ( e.type == SDL_QUIT ) {
					quit = true;
				} else {				
					switch (e.type) {
						// Key press event
						case SDL_KEYDOWN:
							switch (e.key.keysym.sym) {
								// Left
								case SDLK_LEFT:
									move_player(left);
									break;
								// Right
								case SDLK_RIGHT:
									move_player(right);
									break;
								// Space
								case SDLK_SPACE:	
									if (game_state == start) {
										game_state = game;
									} else if (game_state == game){
										player_shoot(bullets);
									} else if (game_state == game_over) {										
										SDL_RenderClear(gRenderer);
										init_player();
										init_invaders();
										game_state = game;
									}
									break;
							}							
						break;
					}
				}							
			}
			if (game_state == start) {

				//Apply the home screen background image
				gBackgroundTexture.render(0, 0, gRenderer);

				//Render text
				gTitleTexture.loadFromRenderedText( "Space Invaders", textColor, "fonts/SHOWG.TTF", 48 , gRenderer, gFont);
				gOtherTextTexture.loadFromRenderedText("Press SPACE to continue", textColor, "fonts/ANTQUAB.TTF", 32, gRenderer, gFont);
                gTitleTexture.render( ( SCREEN_WIDTH - gTitleTexture.getWidth() ) / 2, ( SCREEN_HEIGHT / 5 - gTitleTexture.getHeight() ), gRenderer);
				gOtherTextTexture.render( ( SCREEN_WIDTH - gOtherTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT * 0.8), gRenderer);

			} else if (game_state == game) {

				// Draw objects		
				draw_player(player.hitbox);		
				draw_invaders();						
				Bullet::draw_bullets(bullets, P_BULLETS, "player");	
				Bullet::draw_bullets(e_bullets, E_BULLETS, "invader");	

				// Checks for collision
				enemy_hit_collision(bullets);
				player_hit_collision(e_bullets);
				enemy_player_collision();

				// Move objects
				move_invaders(invaders.speed);
				Bullet::move_bullets(bullets, P_BULLETS, -5, SCREEN_HEIGHT);	
				Bullet::move_bullets(e_bullets, E_BULLETS, 5, SCREEN_HEIGHT);

				// Invaderes shoot
				enemy_shoot(e_bullets);
				
				// Render text
				std::stringstream gameInfo;
				gameInfo.str("");
				gameInfo << "Lives: " << player.lives - 1 << " Score: " << player.score;
 				gGameInfo.loadFromRenderedText( gameInfo.str(), textColor, "fonts/SHOWG.TTF", 20, gRenderer, gFont);
                gGameInfo.render(( SCREEN_WIDTH - ( gGameInfo.getWidth()  + 30 )), 30, gRenderer);
				// Check for game oer conditions
				game_over_ai();
						 
				next_game_tick += 1000 / 30;
				sleep = next_game_tick - SDL_GetTicks();

				if ( sleep >= 0 ) {

            		SDL_Delay(sleep);
        		}
			}

			else if (game_state == game_over){

				//Apply the home screen background image
				gBackgroundTexture.render(0, 0, gRenderer);

				//Render text
				gTitleTexture.loadFromRenderedText( "Game Over", textColor, "fonts/SHOWG.TTF", 48 , gRenderer, gFont);
                gTitleTexture.render( ( SCREEN_WIDTH - gTitleTexture.getWidth() ) / 2, ( SCREEN_HEIGHT / 5 - gTitleTexture.getHeight() ), gRenderer);
				std::stringstream gameInfo;
				gameInfo.str("");
				gameInfo << "Score: " << player.score;
 				gGameInfo.loadFromRenderedText( gameInfo.str(), textColor, "fonts/SHOWG.TTF", 20, gRenderer, gFont);
                gGameInfo.render(( SCREEN_WIDTH - gGameInfo.getWidth()) / 2, ( SCREEN_HEIGHT / 5 + 10), gRenderer);
				gOtherTextTexture.loadFromRenderedText("Press SPACE to play again", textColor, "fonts/ANTQUAB.TTF", 32, gRenderer, gFont);
				gOtherTextTexture.render( ( SCREEN_WIDTH - gOtherTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT * 0.8), gRenderer);
			}
			//Update screen
			SDL_RenderPresent( gRenderer );
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}