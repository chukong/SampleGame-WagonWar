//
//  NoTouchLayer.cpp
//  TankMultiplayer
//
//  Created by Jacky on 14-5-30.
//
//

#include "NoTouchLayer.h"

USING_NS_CC;

bool NoTouchLayer::init()
{
    if(!Layer::create())
    {
        return false;
    }
    
    initWithColor(Color4B(100,100,100,20));
    
    //register touches
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch* touch, Event* event){return true;};
    listener->onTouchEnded = [](Touch* touch, Event* event){};
    listener->onTouchMoved = [](Touch* touch, Event* event){};
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}