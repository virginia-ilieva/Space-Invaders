#include "Player.h"

#include "LGVector2D.h"
#include "Texture.h"
#include "Bullet.h"


Player::Player(Texture * texture, LGVector2D pos, SDL_Rect rect)
    :  GameObject(texture, pos, rect)
{
    this->lives = 3;
	this->score = 0;
}


Player::~Player(void)
{
}

extern const int SCREEN_WIDTH = 1000;
extern const int SCREEN_HEIGHT = 680;

Player * Player::newPlayer(Texture *texture) { 
	SDL_Rect rect = { 10, 480 , Player::WIDTH, Player::HEIGHT };
	return new Player(texture, LGVector2D((SCREEN_WIDTH / 2) - (WIDTH / 2), SCREEN_HEIGHT - (HEIGHT + 10)), rect);
}

SDL_Rect Player::getHitbox() {
	SDL_Rect rect = { (int) this->pos.x, (int) this->pos.y, Player::WIDTH, Player::HEIGHT };
	return rect;
} 

//Shoot bullet/s from player
void Player::shoot(std::vector<Bullet*> & bullets) {
	extern Texture gSpriteSheetTexture;
	extern Mix_Chunk *gShipBullet;

    static const int MAX_BULLETS = 1;
    
    int num = Bullet::find_all_bullets_of_type(bullets, Bullet::Player);

    if (num < MAX_BULLETS) {
        Bullet * bullet = Bullet::newPlayerBullet(&gSpriteSheetTexture);
        int x = (this->getHitbox().x + (WIDTH / 2)) - Bullet::WIDTH / 2;
        int y = this->getHitbox().y - (Bullet::HEIGHT);
        bullet->setPos(LGVector2D(x, y));
        bullets.push_back(bullet);
        Mix_PlayChannel( -1, gShipBullet, 0 );
    }
}

//Look for collisions based on enemy bullet and player rectangles
void Player::hit_collision(std::vector<Bullet*> & bullets) {
    extern Mix_Chunk *gShipHit;
	extern int checkCollision(SDL_Rect a, SDL_Rect b);

    for (std::vector<Bullet*>::iterator i = bullets.begin(); i != bullets.end(); )
    {
        Bullet * bullet = *i;
        if (bullet->getType() == Bullet::Enemy &&
            checkCollision(bullet->getHitbox(), this->getHitbox()) && this->lives >= 0)
        {
            delete bullet;
            i = bullets.erase(i);
            this->lives--;
            Mix_PlayChannel( -1, gShipHit, 0 );
            break;
        } else {
            ++i;
        }
    }
}

