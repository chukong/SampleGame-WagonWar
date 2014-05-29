//
//  WagonSelect.cpp
//  TankMultiplayer
//
//  Created by Jacky on 14-5-29.
//
//

#include "WagonSelect.h"
#include "GPGSManager.h"

USING_NS_CC;

static WagonSelect* create(TurnType turntype)
{
    WagonSelect *pRet = new WagonSelect();
    if (pRet && pRet->init(turntype))
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

Scene* WagonSelect::createScene(TurnType turntype)
{
    auto scene = Scene::create();
    auto layer = WagonSelect::create(turntype);
    scene->addChild(layer);
    return scene;
}

bool WagonSelect::init(TurnType turntype)
{
    if(!Layer::init())
    {
        return false;
    }
    
    _turntype = turntype;
    
    createUI();
    
    return true;
}

void WagonSelect::createUI()
{
    
}

void WagonSelect::ready_callback(cocos2d::Ref* ref)
{
    if (_turntype == FIRSR_TURN) {
        GPGSManager::QuickMatch();
    }
    else if(_turntype == SECOND_TURN){
        ;
    }
}

