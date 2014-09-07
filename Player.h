#ifdef _WIN32

#include <SDL.h>
#include <SDL_mixer.h>

#elif __APPLE__

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>

#endif

#include "GameObject.h"
#include "Bullet.h"

#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player : public GameObject
{
public:
	static const int WIDTH = 70;
	static const int HEIGHT = 35;

	int getLives() { return this->lives; }
	int getScore() { return this->score; }
	void setScore(int points) { this->score + points; }

	Player();
	Player(Texture * texture, LGVector2D pos, SDL_Rect rect);
	~Player(void);
	
	virtual SDL_Rect getHitbox();
	void shoot(std::vector<Bullet*> & bullets);
	void hit_collision(std::vector<Bullet*> & bullets);

	static Player * newPlayer(Texture * texture);

private:
	int lives;
	int score;
};

#endif /* _PLAYER_H_ */