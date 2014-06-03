//
//  Hero.h
//  TankMultiplayer
//
//  Created by Jacky on 14-5-14.
//
//

#pragma once

#include "cocos2d.h"
#include "Configuration.h"

enum Side
{
    Myself,
    other
};

enum Body
{
    BOY,
    GIRL
};

enum Wagon
{
    HORSEY = 0,
    TANK,
    ROCK,
    MECH
};

enum State
{
    IDLE,
    MOVE,
    ATTCK,
    HURT,
    DEAD
};

typedef struct heroConfig
{
    Side side;
    Body body;
    Wagon wagon;
    State state;
    WagonConfig wagonConfig;
}HeroConfig;

class Hero : public cocos2d::Node
{
public:
    
    static Hero* create(Side side = Myself, Body body = BOY, Wagon wagon = HORSEY);
    virtual bool init(Side side, Body body, Wagon wagon);
    
    Hero():airborn(true), needFix(false), moveDelta(0,0.02){};
    
    bool airborn;
    bool needFix;
    int radius;
    CC_SYNTHESIZE(cocos2d::Point, _LastPos, LastPos);
    Node* gunPoint;
    Node* bodyPoint;
    cocos2d::Point moveDelta;
    
    HeroConfig _heroConfig;
    
    cocos2d::Node* _wagonPoint;
    
    cocos2d::Sprite* _bodySprite;
    cocos2d::Sprite* _carASprite;
    cocos2d::Sprite* _carBSprite;
    
};
