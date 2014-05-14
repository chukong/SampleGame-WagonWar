//
//  GameScene.h
//  TankMultiplayer
//
//  Created by WuHao on 14-5-13.
//
//

#ifndef __TankMultiplayer__GameScene__
#define __TankMultiplayer__GameScene__

#include "cocos2d.h"
#include "level.h"

class GameScene : public cocos2d::ParallaxNode
{
public:
    static cocos2d::Scene* createScene();
    static GameScene* create();
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    
    
    
    void update(float dt);
    
    CC_SYNTHESIZE(Level*, _level, Level);
};

#endif /* defined(__TankMultiplayer__GameScene__) */
