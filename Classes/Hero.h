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
#include "ui/CocosGUI.h"
#include "Aimer.h"

enum Side
{
    Myself,
    Other
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
    bool isfacetoright;
}HeroConfig;

class Hero : public cocos2d::Node
{
public:
    
    static Hero* create(Side side = Myself, Body body = BOY, Wagon wagon = HORSEY, bool isfacetoright = true);
    virtual bool init(Side side, Body body, Wagon wagon, bool isfacetoright);
    
    Hero():airborn(true), needFix(false), moveDelta(0,0.02){};
    
    void moveleft();
    void moveright();
    
    void flipLeft();
    void flipRight();
    void startshoot();
    void hit();
    void stop();
    
    void hurt(int t_hurt);
    
    bool airborn;
    bool needFix;
    int radius;
    CC_SYNTHESIZE(cocos2d::Point, _LastPos, LastPos);
    Node* gunPoint;
    Node* bodyPoint;
    cocos2d::Point moveDelta;
    
    
    //by Jacky
    HeroConfig _heroConfig;
    int _lasthp;

    cocos2d::Node* _wagonPoint;
    cocos2d::Sprite* _bodySprite;
    cocos2d::Sprite* _wagonASprite;
    cocos2d::Sprite* _wagonBSprite;
    
    cocos2d::ui::LoadingBar* hpBar;

    Aimer* aim;
};
