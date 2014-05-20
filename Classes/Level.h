//
//  Level.h
//  TankMultiplayer
//
//  Created by WuHao on 14-5-13.
//
//

#ifndef __TankMultiplayer__Level__
#define __TankMultiplayer__Level__

#include "cocos2d.h"

class Level :public cocos2d::Node
{
public:
    void createHole(cocos2d::Point pos, float size);
    static Level* create(const std::string &filename);
    //CC_SYNTHESIZE(cocos2d::Sprite*, _defaultSprite, DefaultSprite);
    //cocos2d::Size getContentSize();
    CC_SYNTHESIZE(cocos2d::RenderTexture*, _rt, RT);
protected:
    //void checkCollisions();
    //std::map<cocos2d::Node*, > m_map;
};

#endif /* defined(__TankMultiplayer__Level__) */
