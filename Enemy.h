#ifdef _WIN32

#include <SDL.h>
#include <SDL_mixer.h>

#elif __APPLE__

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>

#endif

#include <vector>

#include "GameObject.h"
#include "Bullet.h"
#include "Player.h"

#ifndef _ENEMY_H_
#define _ENEMY_H_

class Enemy : public GameObject
{
public:
	static const int WIDTH  = 60;
    static const int HEIGHT = 65;

	int getSpeed() { return this->speed; }
	int getPoints() { return this->points; }

	Enemy();
	Enemy(Texture * texture, LGVector2D pos, SDL_Rect rect);
	~Enemy(void);

	virtual SDL_Rect getHitbox();

	static void Enemy::init(std::vector<std::vector<Enemy*>> & invaders);
	static void Enemy::set_speed(std::vector<std::vector<Enemy*>> & invaders);
	static void Enemy::move_down(std::vector<std::vector<Enemy*>> & invaders);
	static void Enemy::hit_collision(std::vector<Bullet*> & bullets, std::vector<std::vector<Enemy*>> & invaders, Player * player);
	static bool Enemy::player_collision(std::vector<std::vector<Enemy*>> & invaders, Player * player);
	static void Enemy::shoot(std::vector<Bullet*> & bullets, std::vector<std::vector<Enemy*>> & invaders, Player * player);

	static Enemy * newEnemy(Texture * texture);

private:
	int speed;
	unsigned int points;
};

#endif /* _ENEMY_H_ */