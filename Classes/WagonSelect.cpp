//
//  WagonSelect.cpp
//  TankMultiplayer
//
//  Created by Jacky on 14-5-29.
//
//

#include "WagonSelect.h"
#include "Configuration.h"

USING_NS_CC;

Scene* WagonSelect::createScene()
{
    auto scene = Scene::create();
    auto layer = WagonSelect::create();
    scene->addChild(layer);
    return scene;
}

bool WagonSelect::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    createUI();
    
    return true;
}

void WagonSelect::createUI()
{
    
}
