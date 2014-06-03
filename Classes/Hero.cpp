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
    
    _wagonPoint = Node::create();
    _wagonPoint->setPosition(_heroConfig.wagonConfig.offsetx, _heroConfig.wagonConfig.offsety);
    log("_wagonPoint.x ==%f,  _wagonPoint.y ==%f",_wagonPoint->getPositionX(),_wagonPoint->getPositionY());
    this->addChild(_wagonPoint);
    
    if (_heroConfig.body == BOY)
    {
        if (_heroConfig.isfacetoright) {
            _wagonPoint->setScaleX(-1);
        }
    }
    else
    {
        if (_heroConfig.isfacetoright) {
            _wagonPoint->setScaleX(-1);
        }
    }
    
    switch (_heroConfig.wagon) {
        case MECH:
        {
            _wagonASprite = Sprite::createWithSpriteFrameName("mechidle01.png");
            _wagonBSprite = Sprite::createWithSpriteFrameName("mechgunidle01.png");//front
            _wagonPoint->addChild(_wagonASprite);
            _wagonPoint->addChild(_wagonBSprite);
        }
            break;
        case HORSEY:
        {
            _wagonASprite = Sprite::createWithSpriteFrameName("cnm_idle01.png");
            _wagonPoint->addChild(_wagonASprite);
        }
            break;
        case ROCK:
        {
            _wagonASprite = Sprite::createWithSpriteFrameName("rockidle01.png");
            _wagonPoint->addChild(_wagonASprite);
        }
            break;
        case TANK:
        {
            _wagonASprite = Sprite::createWithSpriteFrameName("tankidle01.png");
            _wagonPoint->addChild(_wagonASprite);
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
    this->addChild(gunPoint);
    gunPoint->setPosition(43, 45);
    
    return true;
}

void Hero::moveleft()
{

    if(!_heroConfig.isfacetoright )
    {
        _wagonPoint->setScaleX(-1);
    }
    
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
    if(!_heroConfig.isfacetoright )
    {
        _wagonPoint->setScaleX(-1);
    }
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