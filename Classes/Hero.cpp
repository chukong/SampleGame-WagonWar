//
//  Player.cpp
//  TankMultiplayer
//
//  Created by Jacky on 14-5-14.
//
//

#include "Hero.h"

USING_NS_CC;

Hero* Hero::create(Side side /*= Myself*/,Body body/* = BOY*/, Wagon wagon/* = HORSEY*/, bool isfacetoright/* = true*/)
{
    Hero *pRet = new Hero();
    if (pRet && pRet->init(side, body, wagon, isfacetoright))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool Hero::init(Side side, Body body, Wagon wagon, bool isfacetoright)
{
    _heroConfig.wagonConfig = g_wagonConfig[(int)wagon];
    _heroConfig.side = side;
    _heroConfig.body = body;
    _heroConfig.wagon = wagon;
    _heroConfig.state = IDLE;
    _heroConfig.isfacetoright = isfacetoright;
    
    _lasthp = _heroConfig.wagonConfig.hp;
    _wagonPoint = Node::create();
    _wagonPoint->setPosition(_heroConfig.wagonConfig.offsetx, _heroConfig.wagonConfig.offsety);
    this->addChild(_wagonPoint);
    
    switch (_heroConfig.wagon) {
        case MECH:
        {
            _wagonASprite = Sprite::createWithSpriteFrameName("mechidle01.png");
            _wagonBSprite = Sprite::createWithSpriteFrameName("mechgunidle01.png");//front
            _wagonPoint->addChild(_wagonASprite,1);
            _wagonPoint->addChild(_wagonBSprite,3);
        }
            break;
        case HORSEY:
        {
            _wagonASprite = Sprite::createWithSpriteFrameName("cnm_idle01.png");
            _wagonPoint->addChild(_wagonASprite,1);
        }
            break;
        case ROCK:
        {
            _wagonASprite = Sprite::createWithSpriteFrameName("rockidle01.png");
            _wagonPoint->addChild(_wagonASprite,3);
        }
            break;
        case TANK:
        {
            _wagonASprite = Sprite::createWithSpriteFrameName("tankidle01.png");
            _wagonPoint->addChild(_wagonASprite,3);
        }
            break;
        default:
            break;
    }
    

    
    radius = 20;
    
    auto drawN = DrawNode::create();
    drawN ->drawDot(Point::ZERO, radius, Color4F(0,1,0,0.5));
    this->addChild(drawN);
    
    gunPoint = Node::create();
    _wagonPoint->addChild(gunPoint);
    gunPoint->setPosition(-43, 45);
    
    aim = Aimer::create(isfacetoright, _heroConfig.wagonConfig.upperlimit,_heroConfig.wagonConfig.lowerlimit);
    aim->lowerLimit = _heroConfig.wagonConfig.lowerlimit;
    aim->upperLimit = _heroConfig.wagonConfig.upperlimit;
    gunPoint->addChild(aim);
    aim->setScaleX(-1);
    
    if (_heroConfig.body == BOY)
    {
        if (_heroConfig.isfacetoright) {
            flipRight();
        }
        else{
            flipLeft();
        }
        
        _bodySprite = Sprite::create("boy.png");
        _bodySprite->setPosition(_wagonASprite->getPosition()+ Point(30,30));
        _wagonPoint->addChild(_bodySprite,2);
    }
    else// girl
    {
        if (_heroConfig.isfacetoright) {
            //_wagonPoint->setScaleX(-1);
        }
        
        _bodySprite = Sprite::create("girl.png");
        _bodySprite->setPosition(_wagonASprite->getPosition()+ Point(30,30));
        _wagonPoint->addChild(_bodySprite,2);
    }
    
    if (_heroConfig.side == Myself) {
        hpBar = ui::LoadingBar::create("life_me.png");
    }
    else
    {
        hpBar = ui::LoadingBar::create("life_other.png");
    }
    hpBar->setScale(0.5f);
    hpBar->setPercent((float)_lasthp*100/(float)_heroConfig.wagonConfig.hp);
    hpBar->setPositionY(-50);
    hpBar->setAnchorPoint(Point::ANCHOR_MIDDLE);
    this->addChild(hpBar);
    
    return true;
}

void Hero::flipLeft()
{
        _wagonPoint->setScaleX(1);
        aim->reversed = false;
        _heroConfig.isfacetoright = false;
}
void Hero::flipRight()
{
    _wagonPoint->setScaleX(-1);
    aim->reversed = true;
    _heroConfig.isfacetoright = true;
}

void Hero::moveleft()
{

    flipLeft();

    
    switch (_heroConfig.wagon) {
        case MECH:
        {
            _wagonASprite->stopAllActions();
            _wagonBSprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.mech_move)));
            _wagonBSprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.mechgun_move)));
        }
            break;
        case HORSEY:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.cnm_move)));
        }
            break;
        case ROCK:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.rock_move)));
            
        }
            break;
        case TANK:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.tank_move)));
        }
            break;
        default:
            break;
    }
}
void Hero::moveright()
{
    flipRight();
    switch (_heroConfig.wagon) {
        case MECH:
        {
            _wagonASprite->stopAllActions();
            _wagonBSprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.mech_move)));
            _wagonBSprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.mechgun_move)));
        }
            break;
        case HORSEY:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.cnm_move)));
        }
            break;
        case ROCK:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.rock_move)));
            
        }
            break;
        case TANK:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.tank_move)));
        }
            break;
        default:
            break;
    }
}
void Hero::startshoot()
{
    switch (_heroConfig.wagon) {
        case MECH:
        {
            _wagonASprite->stopAllActions();
            _wagonBSprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.mech_shoot)));
            _wagonBSprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.mechgun_shoot)));
        }
            break;
        case HORSEY:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.cnm_shoot)));
        }
            break;
        case ROCK:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.rock_shoot)));

        }
            break;
        case TANK:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.tank_shoot)));
        }
            break;
        default:
            break;
    }
}
void Hero::hit()
{
    switch (_heroConfig.wagon) {
        case MECH:
        {
            _wagonASprite->stopAllActions();
            _wagonBSprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.mech_hit)));
            _wagonBSprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.mechgun_hit)));
        }
            break;
        case HORSEY:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.cnm_hit)));
        }
            break;
        case ROCK:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.rock_hit)));
            
        }
            break;
        case TANK:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.tank_hit)));
        }
            break;
        default:
            break;
    }
}
void Hero::stop()
{
    switch (_heroConfig.wagon) {
        case MECH:
        {
            _wagonASprite->stopAllActions();
            _wagonBSprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.mech_idle)));
            _wagonBSprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.mechgun_idle)));
        }
            break;
        case HORSEY:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.cnm_idle)));
        }
            break;
        case ROCK:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.rock_idle)));
            
        }
            break;
        case TANK:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.tank_idle)));
        }
            break;
        default:
            break;
    }
}

int Hero::hurt(int t_hurt)
{
    _lasthp = _lasthp - t_hurt;
    if (_lasthp > 0)
    {
        log("%f",(float)_lasthp/(float)_heroConfig.wagonConfig.hp);
        hpBar->setPercent((float)_lasthp*100/(float)_heroConfig.wagonConfig.hp);
    }
    else
    {
        _lasthp = 0;
        hpBar->setPercent(0);
        _eventDispatcher->dispatchCustomEvent("playerdead", this);
    }
    return _lasthp;
}

void Hero::setLife(int life)
{
    _lasthp = life;
    log("..........%f",(float)_lasthp*100/(float)_heroConfig.wagonConfig.hp);
    hpBar->setPercent((float)_lasthp*100/(float)_heroConfig.wagonConfig.hp);
}