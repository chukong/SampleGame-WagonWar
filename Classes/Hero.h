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
    
    static Hero* create(Side side = Myself, Body body = BOY, Wagon wagon = HORSEY, bool isfacetoright = true, std::string name = "");
    virtual bool init(Side side, Body body, Wagon wagon, bool isfacetoright, std::string name);
    
    Hero():airborn(false), needFix(false), moveDelta(0,0.02){};
    
    void moveleft();
    void moveright();
    
    void flipLeft();
    void flipRight();
    void startshoot();
    void endshoot();
    void hit();
    void stop();
    
    int hurt(int t_hurt);
    
    void setLife(int life);
    
    void setSideSymbol(bool isYou);
    
    bool airborn;
    bool needFix;
    int radius;
    CC_SYNTHESIZE(cocos2d::Point, _LastPos, LastPos);
    Node* gunPoint;
    Node* bodyPoint;
    cocos2d::Point moveDelta;
    
    
    //by Jacky
    HeroConfig _heroConfig;
    int _lasthp ;

    cocos2d::Node* _wagonPoint;
    cocos2d::Sprite* _bodySprite;
    cocos2d::Sprite* _wagonASprite;
    cocos2d::Sprite* _wagonBSprite;
    
    cocos2d::Sprite* _hpBarOuter;
    cocos2d::ProgressTimer* _hpInnerBar;
    cocos2d::ProgressTimer* _hpBarBack;
    cocos2d::Label* _nameLabel;
    
    void setName(std::string name);
    
    Aimer* aim;
    void showAimer();
    void hideAimer();
    
    cocos2d::Sprite* _sideSymbol;
    cocos2d::Sprite* _triangleSymbol;
    void showTurnSymbol();
    void hideTurnSymbol();
    
    cocos2d::Label* _angleLabel;
    void updateAngle(int angle);
    
    void update(float delta);
    
protected:
    int _tankMoveEffect;
    int _tankShootEffect;
    int _horseyMoveEffect;
    int _horseyShootEffect;
    int _rockMoveEffect;
    int _rockShootEffect;
    int _mechMoveEffect;
    int _mechShootEffect;
};
