//
//  GameObject.cpp
//  SDL_Template
//
//  Created by Evgeni Petrov on 8/28/14.
//  Copyright (c) 2014 Tryad Games. All rights reserved.
//

#include "GameObject.h"
#include "Texture.h"


GameObject::GameObject(Texture * texture, LGVector2D pos, SDL_Rect clip) {
    this->texture = texture;
    this->pos = pos;
    this->clip = clip;
}


GameObject::~GameObject()
{
}


void GameObject::move(LGVector2D delta) {
    this->pos.add(delta);
}

void GameObject::draw(SDL_Renderer * renderer) {
    if (this->texture) {
        this->texture->render(this->pos.x, this->pos.y, renderer, & this->clip);
    }
}

SDL_Rect GameObject::getHitbox() {
    return SDL_Rect { (int) this->pos.x, (int) this->pos.y, this->texture->getWidth(), this->texture->getHeight() };
}