#include "Enemy.h"
#include <vector>

#include "LGVector2D.h"
#include "Texture.h"
#include "Bullet.h"
#include "Player.h"

Enemy::Enemy(Texture * texture, LGVector2D pos, SDL_Rect rect)
    :  GameObject(texture, pos, rect)
{
	this->speed = 1;
	this->points = 10;
}

Enemy::~Enemy(void)
{
}

Enemy * Enemy::newEnemy(Texture *texture) {
	SDL_Rect rect = { 241, 345 , Enemy::WIDTH, Enemy::HEIGHT };
	return new Enemy(texture, LGVector2D(), rect);
}

SDL_Rect Enemy::getHitbox() {
	SDL_Rect rect = { (int) this->pos.x, (int) this->pos.y, Enemy::WIDTH, Enemy::HEIGHT };
	return rect;
} 

void Enemy::init(std::vector<std::vector<Enemy*>> & invaders){
	extern Texture gSpriteSheetTexture;
	std::vector<Enemy*> enemies = std::vector<Enemy*>();
	int x = 0;
	int y = 60;
	int MAX_ENEMY_IN_ROW = 10;
	int MAX_ENEMY_PER_COL = 5;
	int row, col;
	for (row = 1; row <=MAX_ENEMY_PER_COL; row++){
		for (col = 1; col <= MAX_ENEMY_IN_ROW; col++){
			Enemy * enemy = Enemy::newEnemy(&gSpriteSheetTexture);
			enemy->setPos(LGVector2D(x, y));
			enemies.push_back(enemy);
			x += Enemy::WIDTH; //location for the next invader
		}
		x = 0; //reset line
		y += Enemy::HEIGHT;
		invaders.push_back(enemies);
		enemies.clear();
	}
}
//Set invader movment speed
void Enemy::set_speed(std::vector<std::vector<Enemy*>> & invaders) {
	
	int speed = 1;

	// Changes the speed of the invaders every minute
	if ( SDL_GetTicks() > 1000 * 60 && SDL_GetTicks() < 2000 * 60 ){
		speed = 2;
	} else if ( SDL_GetTicks() > 2000 * 60 && SDL_GetTicks() < 3000 * 60 ){
		speed = 4;
	} else if ( SDL_GetTicks() > 3000 * 60 && SDL_GetTicks() < 4000 * 60 ){
		speed = 8;
	} else if ( SDL_GetTicks() > 4000 * 60 ) {
		speed = 16;
	}
	// Invaders points are based on the invaderes speed
	if (speed > 1 ){
		std::vector<std::vector<Enemy*>>::iterator row;
		std::vector<Enemy*>::iterator col;
		for (row = invaders.begin(); row != invaders.end(); ++row) {
			for (col = row->begin(); col != row->end(); ++col) {
				Enemy * enemy = *col;
				enemy->speed = speed;
				enemy->points = 10 * speed;
			}
		}
	}
}

//Move invaders down one space once the reach the edge
void Enemy::move_down(std::vector<std::vector<Enemy*>> & invaders) {

	std::vector<std::vector<Enemy*>>::iterator row;
	std::vector<Enemy*>::iterator col;
	for (row = invaders.begin(); row != invaders.end(); ++row) {
		for (col = row->begin(); col != row->end(); ++col) {
			Enemy * enemy = *col;
			enemy->move(LGVector2D(0, 3));
		}
	}
}

//Look for collisions based on player bullet and invader rectangles
void Enemy::hit_collision(std::vector<Bullet*> & bullets, std::vector<std::vector<Enemy*>> & invaders, Player * player) {
	extern int checkCollision(SDL_Rect a, SDL_Rect b);
	extern Mix_Chunk *gInvaderHit;

	std::vector<std::vector<Enemy*>>::iterator row;
	std::vector<Enemy*>::iterator col;
	for (row = invaders.begin(); row != invaders.end(); ++row) {
		for (col = row->begin(); col != row->end(); ++col) {
			Enemy * enemy = *col;
			for (std::vector<Bullet*>::iterator it = bullets.begin(); it != bullets.end();) {
                Bullet * bullet = *it;
                if (bullet->getType() == Bullet::Player &&
                    checkCollision(bullet->getHitbox(), enemy->getHitbox()))
                {
					Mix_PlayChannel( -1, gInvaderHit, 0 );

                    player->setScore(enemy->getPoints());

                    delete enemy;
                    col = row->erase(col);
                    
                    delete bullet;
                    it = bullets.erase(it);
                } else {
                    ++it;
                }
            }
		}
	}
}

//Look for collisions based on invader and player rectangles
bool Enemy::player_collision(std::vector<std::vector<Enemy*>> & invaders, Player * player) {
	extern int checkCollision(SDL_Rect a, SDL_Rect b);

	std::vector<std::vector<Enemy*>>::iterator row;
	std::vector<Enemy*>::iterator col;
	for (row = invaders.begin(); row != invaders.end(); ++row) {
		for (col = row->begin(); col != row->end(); ++col) {
			Enemy * enemy = *col;
			if (checkCollision(player->getHitbox(), enemy->getHitbox())){
				return true;
			}
		}
	}
	return false;
}

//Determine when invaders should shoot
void Enemy::shoot(std::vector<Bullet*> & bullets, std::vector<std::vector<Enemy*>> & invaders, Player * player) {
    extern Texture gSpriteSheetTexture;
	extern Mix_Chunk *gInvaderBullet;
    const int MAX_BULLETS = 3;

	std::vector<std::vector<Enemy*>>::iterator row;
	std::vector<Enemy*>::iterator col;
	for (row = invaders.begin(); row != invaders.end(); ++row) {
		for (col = row->begin(); col != row->end(); ++col) {
			Enemy * enemy = *col;
			//player
			int mid_point = (player->getHitbox().x) + (player->getHitbox().w / 2);
				
			//enemy
			int start = enemy->getHitbox().x;
			int end = enemy->getHitbox().x + enemy->getHitbox().w;
                
            //num of enemy bullets
            int num = Bullet::find_all_bullets_of_type(bullets, Bullet::Enemy);
			// checks if the player is in the enemy hit zone
			if (mid_point > start && mid_point < end) {
                if (num < MAX_BULLETS && (rand() % 30) == 1) {

                    int x = (start + (enemy->WIDTH / 2)) - enemy->WIDTH / 2 ;
                    int y = enemy->getHitbox().y + enemy->WIDTH;
                        
                    Bullet * bullet = Bullet::newInvaderBullet(&gSpriteSheetTexture);
                    bullet->setPos(LGVector2D(x, y));
                    bullets.push_back(bullet);
                        
                    Mix_PlayChannel( -1, gInvaderBullet, 0 );
                }
			}	
		}
	}
}