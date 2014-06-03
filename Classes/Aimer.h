//
//  Aimer.h
//  TankMultiplayer
//
//  Created by WuHao on 14-5-30.
//
//

#ifndef __TankMultiplayer__Aimer__
#define __TankMultiplayer__Aimer__

#include "cocos2d.h"

class Aimer : public cocos2d::Node
{
public:
    CREATE_FUNC(Aimer);
    virtual bool init();
    float upperLimit;
    float lowerLimit;
    Aimer():upperLimit(-45), lowerLimit(-10){};
    void setAngle(float a);
    float getWorldAngle();

protected:
    cocos2d::Sprite* _back;
    cocos2d::Sprite* _pointer;
    cocos2d::ProgressTimer* _green;
    cocos2d::Sprite* _crosshair;
    float _angle;
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif /* defined(__TankMultiplayer__Aimer__) */
