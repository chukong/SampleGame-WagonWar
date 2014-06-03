//
//  Aimer.cpp
//  TankMultiplayer
//
//  Created by WuHao on 14-5-30.
//
//

#include "Aimer.h"
USING_NS_CC;

bool Aimer::init()
{
    _back = Sprite::create("aimerback.png");
    addChild(_back);
    
    //const Point mid(getContentSize()/2);
    _pointer = Sprite::create("pointer.png");
    _pointer->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    //_pointer->setPosition(mid);
    _pointer->setScaleY(0.5);
    _pointer->setOpacity(200);
    
    _green = ProgressTimer::create(Sprite::create("aimergreen.png"));
    
    //_green->setPosition(mid);
    _green->setType(ProgressTimer::Type::RADIAL);
    _green->setOpacity(150);
    
    
    addChild(_green);
    addChild(_pointer);
    
    
    
    //find out the percentage
    _green->setPercentage((lowerLimit-upperLimit)/3.6);
    _green->setRotation(-upperLimit);
    _pointer->setRotation((upperLimit+lowerLimit)/2);
    _back->setRotation((upperLimit+lowerLimit)/2);
    return true;

}