#include "Bullet.h"
#include <string>

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
    return new Bullet(texture, LGVector2D(), SDL_Rect { 540, 365 , Bullet::WIDTH, Bullet::HEIGHT }, Bullet::Enemy);
}

Bullet * Bullet::newPlayerBullet(Texture *texture) {
    return new Bullet(texture, LGVector2D(), SDL_Rect { 32, 140 , Bullet::WIDTH, Bullet::HEIGHT }, Bullet::Player);
}

SDL_Rect Bullet::getHitbox() {
    return SDL_Rect { (int) this->pos.x, (int) this->pos.y, Bullet::WIDTH, Bullet::HEIGHT  };
}
