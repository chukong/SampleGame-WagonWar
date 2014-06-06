//
//  bullet.cpp
//  TankMultiplayer
//
//  Created by WuHao on 14-5-8.
//
//

#include "bullet.h"
USING_NS_CC;

//void Bullet::draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
//{
//    
//    _customCommand.init(_globalZOrder);
//    
//    _customCommand.func = CC_CALLBACK_0(Bullet::drawFinished, this);
//    Director::getInstance()->getRenderer()->addCommand(&_customCommand);
//    Sprite::draw(renderer, transform, transformUpdated);
//}

BulletConfig defaultBullet={
    "circle.png",
    100,
    3,
    90
};
BulletConfig tankBullet={
    "tankbullet.png",
    150,
    3,
    50
};
BulletConfig mechBullet={
    "mechbullet.png",
    175,
    3,
    45
};
BulletConfig horseyBullet={
    "cnmbullet.png",
    135,
    3,
    60
};
BulletConfig rockBullet={
    "rockbullet1.png",
    125,
    3,
    110
};

void Bullet::drawFinished()
{
    //_world->checkCollision(this);
}


Bullet* Bullet::create(BulletTypes type, cocos2d::Point pos, cocos2d::Point vector)
{
    Bullet *b = new Bullet();
    switch(type)
    {
        case mechB:
            b->setConfig(mechBullet);
            break;
        case horseyB:
            b->setConfig(horseyBullet);
            break;
        case rockB:
            b->setConfig(rockBullet);
            break;
        default:
            b->setConfig(tankBullet);
            break;
    }
    if(b->initWithFile(b->getConfig().filename))
    {
        b->autorelease();
        b->setPosition(pos);
        b->setLastPos(pos-vector);
        return b;
    }
    CC_SAFE_DELETE(b);
    return nullptr;
}
bool Bullet::willExplode(){
    return _willExplode;
}
void Bullet::explode()
{
    _willExplode = true;
}