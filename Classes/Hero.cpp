//
//  Player.cpp
//  TankMultiplayer
//
//  Created by Jacky on 14-5-14.
//
//

#include "Hero.h"

USING_NS_CC;

Hero* Hero::create(Side side /*= Myself*/,Body body/* = BOY*/, Wagon wagon/* = HORSEY*/)
{
    Hero *pRet = new Hero();
    if (pRet && pRet->init(side, body, wagon))
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

bool Hero::init(Side side, Body body, Wagon wagon)
{
    _heroConfig.wagonConfig = g_wagonConfig[(int)wagon];
    _heroConfig.state = IDLE;
    
    _wagonPoint = Node::create();
    
    
    if (_heroConfig.wagon == MECH)
    {
        
    }
    else
    {
        
    }
    
    auto sp = Sprite::create("testnode.png");
    
    this->addChild(sp);
    sp->setPosition(0, 40);
    radius = 20;
    
    auto drawN = DrawNode::create();
    drawN ->drawDot(Point::ZERO, radius, Color4F(0,1,0,0.5));
    this->addChild(drawN);
    
    gunPoint = Node::create();
    this->addChild(gunPoint);
    gunPoint->setPosition(43, 45);
    
    return true;
}