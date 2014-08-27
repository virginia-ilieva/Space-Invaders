#pragma once
#include <SDL.h>
#include <string>

#include "Texture.h"

class Bullet
{
	private:
		static const int B_WIDTH  = 25;
		static const int B_HEIGHT = 40;
		
		unsigned int alive;
		SDL_Rect hitbox;

	public:
		Bullet();
		~Bullet(void);

		static void Bullet::draw_bullets(Bullet b[], int max, std::string type, Texture texture, SDL_Renderer* renderer);
		static int Bullet::move_bullets(Bullet b[], int max, int speed, int SCREEN_HEIGHT);

		static int Bullet::getWidth() { return B_WIDTH; }
		static int Bullet::getHeight() { return B_HEIGHT; }

		const int Bullet::getBulletAlive() { return alive; }
		void Bullet::setBulletAlive( int state ) { alive == state; } 

		const SDL_Rect Bullet::getHitbox() { return hitbox; }
		void Bullet::setHitbox( int x, int y );
};

