#include "Bullet.h"
#include <string>

#include "Texture.h"

Bullet::Bullet()
{
		alive = 0;
		hitbox.x = 0;
		hitbox.y = 0;
		hitbox.w = B_WIDTH;
		hitbox.h = B_HEIGHT;
}

Bullet::~Bullet(void)
{
}

void Bullet::setHitbox( int x, int y ){
		hitbox.x = x;
		hitbox.y = y;	
}

//Draw both the enemy and the players bullets if there alive
void Bullet::draw_bullets(Bullet b[], int max, std::string type) {

	extern Texture gSpriteSheetTexture; 
	extern SDL_Renderer* gRenderer;

	SDL_Rect src;
	// Sets the location of the image on the sprite sheet based on the bullet type
	if ( type == "player"){
		src.x = 32;
		src.y = 140;
	} else if ( type == "invader") {
		src.x = 540;
		src.y = 365;
	}
	
	src.w = Bullet::getWidth();	
	src.h = Bullet::getHeight();
	
	int i;
	// Renders the bullets
	for (i = 0; i < max; i++) {
		if (b[i].getBulletAlive() == 1) {
			SDL_Rect hitbox = b[i].getHitbox();
			gSpriteSheetTexture.render(hitbox.x, hitbox.y, gRenderer, &src );
		} 
	}
}

//Move positions of both enemy and player bullets on screen
int Bullet::move_bullets(Bullet b[], int max, int speed, int SCREEN_HEIGHT) {

	int i;

	for(i = 0; i < max; i++) {
		if (b[i].alive == 1) {
			b[i].hitbox.y += speed;
			if (b[i].hitbox.y <= 0) {
				b[i].alive = 0;	
			}
			if (b[i].hitbox.y + b[i].hitbox.h >= SCREEN_HEIGHT) {
				b[i].alive = 0;	
			}
		}
	}
	return 0;
}