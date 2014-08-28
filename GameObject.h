//
//  GameObject.h
//  SDL_Template
//
//  Created by Evgeni Petrov on 8/28/14.
//  Copyright (c) 2014 Tryad Games. All rights reserved.
//

#ifndef __SDL_Template__GameObject__
#define __SDL_Template__GameObject__

#ifdef _WIN32

#include <SDL.h>

#elif __APPLE__

#include <SDL2/SDL.h>

#endif


#include "LGVector2D.h"

class Texture;


class GameObject {
public:
    GameObject();
    GameObject(Texture * texture, LGVector2D pos, SDL_Rect clip);
    ~GameObject();
    
    virtual SDL_Rect getHitbox();
    LGVector2D getPos() { return this->pos; }
    void setPos(LGVector2D newPos) { this->pos = newPos; }
    void move(LGVector2D delta);

    void printPos( bool newLine) { this->pos.print(newLine); }
    
    void draw(SDL_Renderer * renderer);
protected:
    Texture * texture;
    SDL_Rect clip;
    LGVector2D pos;
};

#endif /* defined(__SDL_Template__GameObject__) */
