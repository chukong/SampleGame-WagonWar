//
//  GameUI.h
//  TankMultiplayer
//
//  Created by WuHao on 14-5-20.
//
//

#ifndef __TankMultiplayer__GameUI__
#define __TankMultiplayer__GameUI__

#include "cocos2d.h"

class GameUI : public cocos2d::Layer
{
public:
    CREATE_FUNC(GameUI);
    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
    cocos2d::Sprite* left;
    cocos2d::Sprite* right;
protected:
    bool _left;
    bool _right;
    GameUI():_left(false), _right(false){};
};

class WindIndicator : public cocos2d::Node
{
public:
    CREATE_FUNC(WindIndicator);
    virtual bool init();
    
    void setWind(cocos2d::Point pos);
protected:
    cocos2d::LabelAtlas *_label;
    cocos2d::Sprite *_arrow;
};

#endif /* defined(__TankMultiplayer__GameUI__) */
