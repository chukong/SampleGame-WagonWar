//
//  Helper.h
//  TankMultiplayer
//
//  Created by WuHao on 14-5-12.
//
//

#ifndef __TankMultiplayer__Helper__
#define __TankMultiplayer__Helper__

#include "cocos2d.h"
#include "Level.h"
#include "Hero.h"

class Helper
{
public:
    static void removeAfter(cocos2d::Node* node, float seconds);
    static bool isInCircle(int index, int radius);
    static float getAngleFromIndex(int index, int radius);
    static cocos2d::Point getPointFromIndex(int index, int radius);
};

class DepthOn : public cocos2d::Node
{
public:
    void onDraw();
    virtual void visit(cocos2d::Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated);
    CREATE_FUNC(DepthOn);
protected:
    cocos2d::CustomCommand _customCommand;
};

class DepthOff : public cocos2d::Node
{
public:
    void onDraw();
    virtual void visit(cocos2d::Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated);
    CREATE_FUNC(DepthOff);
protected:
    cocos2d::CustomCommand _customCommand;
};

class CollisionCheckNode : public cocos2d::Node
{
public:
    void checkCollision();
    virtual void visit(cocos2d::Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated);
    CollisionCheckNode* create(cocos2d::ParallaxNode* game, cocos2d::Layer* player, cocos2d::Layer* bullet, cocos2d::RenderTexture* terrain);
    CC_SYNTHESIZE(cocos2d::RenderTexture*, _level, Level);
    CC_SYNTHESIZE(cocos2d::Layer*, _bullets, BulletLayer);
    CC_SYNTHESIZE(cocos2d::Layer*, _players, PlayerLayer);
    CC_SYNTHESIZE(cocos2d::ParallaxNode*, _gameLayer, GameLayer);
    CC_SYNTHESIZE(cocos2d::RenderTexture*, _rt, RT);
protected:
    cocos2d::CustomCommand _customCommand;
};



#endif /* defined(__TankMultiplayer__Helper__) */
