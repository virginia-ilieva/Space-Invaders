/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#ifdef _WIN32

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#elif __APPLE__

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>

#endif

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

#include "Texture.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 680;

enum direction_t {left, right, stationary};
direction_t enemy_direction;
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
//Detect any collision between any two  rectangles
int checkCollision(SDL_Rect a, SDL_Rect b);
//Renders the game start window
void game_state_start(SDL_Color textColor);
// Hadles the main game loop 
void game_state_game(std::vector<Bullet*> & bullets, std::vector<std::vector<Enemy*>> & invaders, Player * player, SDL_Color textColor);
// Renders the game ove screen
void game_state_game_over(Player * player, SDL_Color textColor);
//Determine for game over event
void game_over_ai(std::vector<std::vector<Enemy*>> & invaders, Player * player);


//Starts up SDL and creates window
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
//Loads media
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

//Renders the game start window
void game_state_start(SDL_Color textColor){
	//Apply the home screen background image
	gBackgroundTexture.render(0, 0, gRenderer);

	//Render text
	gTitleTexture.loadFromRenderedText( "Space Invaders", textColor, "fonts/SHOWG.TTF", 48 , gRenderer, gFont);
	gOtherTextTexture.loadFromRenderedText("Press SPACE to continue", textColor, "fonts/ANTQUAB.TTF", 32, gRenderer, gFont);
    gTitleTexture.render( ( SCREEN_WIDTH - gTitleTexture.getWidth() ) / 2, ( SCREEN_HEIGHT / 5 - gTitleTexture.getHeight() ), gRenderer);
	gOtherTextTexture.render( ( SCREEN_WIDTH - gOtherTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT * 0.8), gRenderer);
}

// Hadles the main game loop 
void game_state_game(std::vector<Bullet*> & bullets, std::vector<std::vector<Enemy*>> & invaders, Player * player, SDL_Color textColor){

	std::vector<std::vector<Enemy*>>::iterator row;
	std::vector<Enemy*>::iterator col;

	// Draw objects		
	player->draw(gRenderer);

    //Drawing invaders
	for (row = invaders.begin(); row != invaders.end(); ++row) {
			for (col = row->begin(); col != row->end(); ++col) {
			Enemy * enemy = *col;
			enemy->draw(gRenderer);
			}
	}

    //drawing bullets, maybe try to combine them? can use simple enum to distinguish between type of bullets
    for (std::vector<Bullet*>::iterator i = bullets.begin(); i != bullets.end(); ++i) {
        Bullet * bullet = *i;
        bullet->draw(gRenderer);
    }

	// Checks for collision
	Enemy::hit_collision(bullets, invaders, player);
	player->hit_collision(bullets);
	//enemy_player_collision();

	// Move invaders
	//move_invaders(invaders.speed);
	Enemy::set_speed(invaders);
	switch (enemy_direction) {
		case left:
			for (row = invaders.begin(); row != invaders.end(); ++row) {
					for (col = row->begin(); col != row->end(); ++col) {
					Enemy * enemy = *col;
					if (enemy->getHitbox().x <= 0) {
						enemy_direction = right;
						Enemy::move_down(invaders);
						break;
					}
					enemy->move(LGVector2D(-enemy->getSpeed(), 0));
				}
			}
			break;

		case right:
			for (row = invaders.begin(); row != invaders.end(); ++row) {
					for (col = row->begin(); col != row->end(); ++col) {
					Enemy * enemy = *col;
					if (enemy->getHitbox().x + enemy->WIDTH >= SCREEN_WIDTH) {
						enemy_direction = left;
						Enemy::move_down(invaders);
						break;
					}
					enemy->move(LGVector2D(enemy->getSpeed(), 0));
				}
			}
			break;

		default:
		break;
	}
				
    //move bullets
    for (std::vector<Bullet*>::iterator it = bullets.begin(); it != bullets.end(); ) {
        Bullet * bullet = *it;
                    
        switch (bullet->getType()) {
            case Bullet::Player:
                bullet->move(LGVector2D(0, -5));
                break;
            case Bullet::Enemy:
                bullet->move(LGVector2D(0, 5));
                break;
            default:
                break;
        }
                    
                    
        SDL_Rect hitbox = bullet->getHitbox();
        if (hitbox.y <= 0 ||
            hitbox.y + hitbox.h >= SCREEN_HEIGHT)
        {
            delete bullet;
            it = bullets.erase(it);
        } else {
            ++it;
        }
                    
    }

	// Invaderes shoot
	Enemy::shoot(bullets, invaders, player);
				
	// Render text
	std::stringstream gameInfo;
	gameInfo.str("");
	gameInfo << "Lives: " << player->getLives() - 1 << " Score: " << player->getScore();
 	gGameInfo.loadFromRenderedText( gameInfo.str(), textColor, "fonts/SHOWG.TTF", 20, gRenderer, gFont);
    gGameInfo.render(( SCREEN_WIDTH - ( gGameInfo.getWidth()  + 30 )), 30, gRenderer);
	// Check for game oer conditions
	game_over_ai(invaders, player);
	
}

// Renders the game ove screen
void game_state_game_over(Player * player, SDL_Color textColor){
	//Apply the home screen background image
	gBackgroundTexture.render(0, 0, gRenderer);

	//Render text
	gTitleTexture.loadFromRenderedText( "Game Over", textColor, "fonts/SHOWG.TTF", 48 , gRenderer, gFont);
    gTitleTexture.render( ( SCREEN_WIDTH - gTitleTexture.getWidth() ) / 2, ( SCREEN_HEIGHT / 5 - gTitleTexture.getHeight() ), gRenderer);
	std::stringstream gameInfo;
	gameInfo.str("");
	gameInfo << "Score: " << player->getScore();
 	gGameInfo.loadFromRenderedText( gameInfo.str(), textColor, "fonts/SHOWG.TTF", 20, gRenderer, gFont);
    gGameInfo.render(( SCREEN_WIDTH - gGameInfo.getWidth()) / 2, ( SCREEN_HEIGHT / 5 + 10), gRenderer);
	gOtherTextTexture.loadFromRenderedText("Press SPACE to play again", textColor, "fonts/ANTQUAB.TTF", 32, gRenderer, gFont);
	gOtherTextTexture.render( ( SCREEN_WIDTH - gOtherTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT * 0.8), gRenderer);
}

//Determine for game over event
void game_over_ai(std::vector<std::vector<Enemy*>> & invaders, Player * player) {
	
	if (player->getLives() == 0 || Enemy::player_collision(invaders, player) || invaders.empty() ) {		
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
		//Initiate player
		Player * player = Player::newPlayer(&gSpriteSheetTexture);

		//Initiate invaders
		enemy_direction = right;
		std::vector<std::vector<Enemy*>> invaders =			std::vector<std::vector<Enemy*>>();
 		Enemy::init(invaders);

		//Initiate bullets
        std::vector<Bullet*> bullets = std::vector<Bullet*>();

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
									if (player->getHitbox().x > 0) {
										player->move(LGVector2D(-5, 0));
									}
									break;
								// Right
								case SDLK_RIGHT:
									if (player->getHitbox().x + player->WIDTH < SCREEN_WIDTH){
										player->move(LGVector2D(5, 0));
									}
									break;
								// Space
								case SDLK_SPACE:	
									if (game_state == start) {
										game_state = game;
									} else if (game_state == game){
										player->shoot(bullets);
									} else if (game_state == game_over) {										
										SDL_RenderClear(gRenderer);
										Player * player = Player::newPlayer(&gSpriteSheetTexture);										
 										Enemy::init(invaders);
										game_state = game;
									}
									break;
							}							
						break;
					}
				}							
			}
			if (game_state == start) {

				game_state_start(textColor);

			} else if (game_state == game) {

				game_state_game(bullets, invaders, player, textColor);
									 
				next_game_tick += 1000 / 30;
				sleep = next_game_tick - SDL_GetTicks();

				if ( sleep >= 0 ) {

					SDL_Delay(sleep);
				}

			}
			else if (game_state == game_over){

				game_state_game_over(player, textColor);
				
			}
			//Update screen
			SDL_RenderPresent( gRenderer );
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}