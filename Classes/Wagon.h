//
//  Wagon.h
//  TankMultiplayer
//
//  Created by WuHao on 14-5-30.
//
//

#ifndef __TankMultiplayer__Wagon__
#define __TankMultiplayer__Wagon__

#include "cocos2d.h"

class Wagon : public cocos2d::Sprite
{
public:
    float maxHP;
    float maxAngle;
    cocos2d::Point dudeOffset;
    cocos2d::Point gunPointOffset;
};

class Horsey : public Wagon
{
public:
    Horsey(){maxHP=1300; maxAngle= 70; dudeOffset=cocos2d::Point(18,19); gunPointOffset=cocos2d::Point(-52,-12);};
};

class Rock : public Wagon
{
    
};

class Mecha : public Wagon
{
    
};

class Tank : public Wagon
{
    
};

#endif /* defined(__TankMultiplayer__Wagon__) */
