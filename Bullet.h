#ifdef _WIN32

#include <SDL.h>

#elif __APPLE__

#include <SDL2/SDL.h>

#endif

#include <string>
#include <vector>

#include "GameObject.h"

#ifndef _BULLET_H_
#define _BULLET_H_



class Bullet : public GameObject
{
public:
    enum Type { Player = 0, Enemy = 1};

    static const int WIDTH  = 25;
    static const int HEIGHT = 40;


    Bullet::Type getType() { return this->type; }


    Bullet();
    Bullet(Texture * texture, LGVector2D pos, SDL_Rect rect, Bullet::Type type);
    ~Bullet(void);
    
    virtual SDL_Rect getHitbox();

	static int Bullet::find_all_bullets_of_type(std::vector<Bullet*> & bullets, Bullet::Type type);

    static Bullet * newInvaderBullet(Texture * texture);
    static Bullet * newPlayerBullet(Texture * texture);
private:
    Bullet::Type type;
};

#endif /* _BULLET_H_ */
