#include "Bullet.h"
#include <string>
#include <vector>

#include "LGVector2D.h"
#include "Texture.h"

Bullet::Bullet(Texture * texture, LGVector2D pos, SDL_Rect rect, Bullet::Type type)
    :  GameObject(texture, pos, rect)
{
    this->type = type;
}

Bullet::~Bullet(void)
{
}

Bullet * Bullet::newInvaderBullet(Texture *texture) {
	SDL_Rect rect = { 540, 365 , Bullet::WIDTH, Bullet::HEIGHT };
	return new Bullet(texture, LGVector2D(), rect , Bullet::Enemy);
}

Bullet * Bullet::newPlayerBullet(Texture *texture) {
	SDL_Rect rect = { 32, 140 , Bullet::WIDTH, Bullet::HEIGHT };
    return new Bullet(texture, LGVector2D(), rect, Bullet::Player);
}

SDL_Rect Bullet::getHitbox() {
	SDL_Rect rect = { (int) this->pos.x, (int) this->pos.y, Bullet::WIDTH, Bullet::HEIGHT };
	return rect;
} 

//bullets util function
int Bullet::find_all_bullets_of_type(std::vector<Bullet*> & bullets, Bullet::Type type) {
    int result = 0;
    for (std::vector<Bullet*>::iterator i = bullets.begin(); i != bullets.end(); ++i )
    {
        Bullet * bullet = *i;
        if (bullet->getType() == type) {
            result++;
        }
    }
    return result;
}
