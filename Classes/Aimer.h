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
protected:
    cocos2d::Sprite* _back;
    cocos2d::Sprite* _pointer;
    cocos2d::ProgressTimer* _green;
};

#endif /* defined(__TankMultiplayer__Aimer__) */
