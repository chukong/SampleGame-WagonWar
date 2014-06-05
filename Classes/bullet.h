//
//  bullet.h
//  TankMultiplayer
//
//  Created by WuHao on 14-5-8.
//
//

#ifndef __TankMultiplayer__bullet__
#define __TankMultiplayer__bullet__

#include "cocos2d.h"
#include "HelloWorldScene.h"

enum BulletTypes{
    defaultB
};

struct BulletConfig{
    std::string filename;
    int damage;
    int radius;
    int expRadius;
};

const BulletConfig defaultBullet={
    "circle.png",
    100,
    3,
    90
};

const BulletConfig horseyBullet =
{
    "circle.png",
    100,
    3,
    90
};

class Bullet : public cocos2d::Sprite
{
public:
    CC_SYNTHESIZE(HelloWorld*, _world, World);
    static Bullet* create(BulletTypes type, cocos2d::Point pos, cocos2d::Point vector);
    CC_SYNTHESIZE(BulletConfig*, _config, Config);
    CC_SYNTHESIZE(cocos2d::Point, _LastPos, LastPos);
    void explode();
    static BulletConfig defaultBullet;
    bool willExplode();
protected:
    Bullet():_willExplode(false){};
    //virtual void draw(cocos2d::Renderer *renderer, const kmMat4 &transform, bool transformUpdated);
    virtual void drawFinished();
    cocos2d::CustomCommand _customCommand;
    bool _willExplode;
};


#endif /* defined(__TankMultiplayer__bullet__) */
