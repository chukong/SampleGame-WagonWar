//
//  NoTouchLayer.cpp
//  TankMultiplayer
//
//  Created by Jacky on 14-5-30.
//
//

#include "NoTouchLayer.h"
#include "VisibleRect.h"

USING_NS_CC;

bool NoTouchLayer::init()
{
    if(!Layer::create())
    {
        return false;
    }
    
    initWithColor(Color4B(100,100,100,200));
    
    //register touches
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch* touch, Event* event){return true;};
    listener->onTouchEnded = [](Touch* touch, Event* event){};
    listener->onTouchMoved = [](Touch* touch, Event* event){};
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    label = Label::create("Connecting to Google Service...", "Arial", 30);
    label->setAnchorPoint(Point::ANCHOR_MIDDLE);
    label->setPosition(g_visibleRect.center);
    this->addChild(label);
    
    return true;
}

void NoTouchLayer::setError(std::string err)
{
    label->setString(err);
    
    this->scheduleOnce(schedule_selector(NoTouchLayer::remove), 2.0f);
}

void NoTouchLayer::remove(float dt)
{
    removeFromParentAndCleanup(true);
}