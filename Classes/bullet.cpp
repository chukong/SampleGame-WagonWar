//
//  bullet.cpp
//  TankMultiplayer
//
//  Created by WuHao on 14-5-8.
//
//

#include "bullet.h"
USING_NS_CC;

void Bullet::draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
{
    
    _customCommand.init(_globalZOrder);
    
    _customCommand.func = CC_CALLBACK_0(Bullet::drawFinished, this);
    Director::getInstance()->getRenderer()->addCommand(&_customCommand);
    Sprite::draw(renderer, transform, transformUpdated);
}

void Bullet::drawFinished()
{
    //_world->checkCollision(this);
}

Bullet* Bullet::create(HelloWorld *world)
{
    Bullet *b = new Bullet();
    if(b->initWithFile("circle.png"))
    {
        b->autorelease();
        b->setWorld(world);
        return b;
    }
    CC_SAFE_DELETE(b);
    return nullptr;
}