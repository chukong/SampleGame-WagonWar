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
    
    static Aimer* create(bool isRight, float upper, float lower);

    virtual bool init(bool isRight, float upper, float lower);
    float upperLimit;
    float lowerLimit;
    Aimer():upperLimit(-45), lowerLimit(-10){};
    void runRotation(float duration,int endAngle);
    void hideCrossHair();
    void showCrossHair();
    void setAngle(float a);
    float getWorldAngle();
    bool reversed;
protected:
    cocos2d::Sprite* _back;
    cocos2d::Sprite* _pointer;
    cocos2d::ProgressTimer* _green;
    cocos2d::Sprite* _crosshair;
    bool _isRight;
    float _angle;
    float _backAnglePre;
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif /* defined(__TankMultiplayer__Aimer__) */
