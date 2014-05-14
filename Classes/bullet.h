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

class Bullet : public cocos2d::Sprite
{
public:
    CC_SYNTHESIZE(HelloWorld*, _world, World);
    static Bullet* create(HelloWorld* world);
protected:
    virtual void draw(cocos2d::Renderer *renderer, const kmMat4 &transform, bool transformUpdated);
    virtual void drawFinished();
    cocos2d::CustomCommand _customCommand;
};


#endif /* defined(__TankMultiplayer__bullet__) */
