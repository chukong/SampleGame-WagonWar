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
    cocos2d::Sprite* fire;
protected:
    bool _left;
    bool _right;
    bool _startShoot;
    GameUI():_left(false), _right(false),_startShoot(false){};
    cocos2d::EventListenerTouchOneByOne *_mytouchListener;
    void _toggleTouchEnable(bool onoff);
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

class PowerIndicator : public cocos2d::Node
{
public:
    CREATE_FUNC(PowerIndicator);
    virtual bool init();
    
protected:
    cocos2d::Sprite* fire;
    cocos2d::Sprite* powerbar;
    cocos2d::Sprite* innerpower;
    
    bool _powerFlag = false;
    int _tick = 0;
    int _tickPre;
    //timeval _now;
    
    void update(float delta);
    
    void increasePower();
    void dismissPower();
};

#endif /* defined(__TankMultiplayer__GameUI__) */
