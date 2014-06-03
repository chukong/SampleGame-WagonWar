//
//  GameUI.cpp
//  TankMultiplayer
//
//  Created by WuHao on 14-5-20.
//
//

#include "GameUI.h"
USING_NS_CC;


bool GameUI::init()
{
    if(!Layer::init())
        return false;
    auto director = Director::getInstance();
    auto vsize = director->getVisibleSize();
    auto vorigin = director->getVisibleOrigin();
    
    
    left = Sprite::create("b1.png");
    //left->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    left->setPosition(Point(left->getContentSize()/2)+Point(30,30));
    addChild(left);
    
    right = Sprite::create("f1.png");
    auto rightsize = right->getContentSize();
    right->setPosition(Point(vsize.width-rightsize.width, 30+rightsize.height));
    addChild(right);
    
    fire = Sprite::create("CloseSelected.png");
    fire->setScale(2.0f);
    addChild(fire);
    fire->setPosition(Point(vsize.width/2, 30));
    
    
    _mytouchListener = EventListenerTouchOneByOne::create();
    _mytouchListener->onTouchBegan = CC_CALLBACK_2(GameUI::onTouchBegan, this);
    _mytouchListener->onTouchEnded = CC_CALLBACK_2(GameUI::onTouchEnded, this);
    _mytouchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mytouchListener, this);
    
    auto wind = WindIndicator::create();
    addChild(wind);
    wind->setPosition(vsize.width/2+vorigin.x, vsize.height- vorigin.y - 50);
    
    auto power = PowerIndicator::create();
    //power->setAnchorPoint(Point(0.5f, 0.0f));
    addChild(power);
    power->setPosition(vsize.width/2+vorigin.x, 191);
    
    auto touchOffListener = EventListenerCustom::create("touch off", CC_CALLBACK_0(GameUI::_toggleTouchEnable, this, false));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchOffListener, this);
    
    auto touchOnListener = EventListenerCustom::create("touch on", CC_CALLBACK_0(GameUI::_toggleTouchEnable, this, true));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchOnListener, this);
    return true;
}

void GameUI::_toggleTouchEnable(bool onoff)
{
    _mytouchListener->setEnabled(onoff);
}
bool GameUI::onTouchBegan(Touch *touch, Event *event)
{
    if(left->getBoundingBox().containsPoint(touch->getLocation()))
    {
        _eventDispatcher->dispatchCustomEvent("go left");
        left->runAction(ScaleTo::create(0.1, 1.4));
        _left=true;
        return true;
    }
    if(right->getBoundingBox().containsPoint(touch->getLocation()))
    {
        _eventDispatcher->dispatchCustomEvent("go right");
        right->runAction(ScaleTo::create(0.1, 1.4));
        _right=true;
        return true;
    }
    if(touch->getLocation().y > 600)
    {
        _eventDispatcher->dispatchCustomEvent("randomWind");
        return true;
    }
    if(fire->getBoundingBox().containsPoint(touch->getLocation()))
    {
        _eventDispatcher->dispatchCustomEvent("start shoot");
        _eventDispatcher->dispatchCustomEvent("increasePower");
        _startShoot = true;
        return true;
    }
    return false;
}

void GameUI::onTouchEnded(Touch *touch, Event *event)
{
    if(_left)
    {
        left->runAction(EaseElasticOut::create(ScaleTo::create(0.7, 1)));
        _eventDispatcher->dispatchCustomEvent("stop");
    }
    else if(_right)
    {
        right->runAction(EaseElasticOut::create(ScaleTo::create(0.7, 1)));
        _eventDispatcher->dispatchCustomEvent("stop");
    }
    else if(_startShoot)
    {
        _eventDispatcher->dispatchCustomEvent("end shoot");
        _eventDispatcher->dispatchCustomEvent("dismissPower");
    }
    
    _startShoot = false;
    _left = false;
    _right = false;
    
}



bool WindIndicator::init()
{
    auto dial = Sprite::create("windDial.png");
    addChild(dial);
    
    _label = LabelAtlas::create("0", "number1.png", 20, 26, '.');
    addChild(_label);
    _label->setAnchorPoint(Point::ANCHOR_MIDDLE);
    
    _arrow = Sprite::create("windArrow.png");
    addChild(_arrow);
    _arrow->setAnchorPoint(Point(0.5, -27/22.0));
    //_arrow->runAction(RepeatForever::create(RotateBy::create(2,360)));
    
    auto listener = EventListenerCustom::create("wind",[&](EventCustom* event){
        this->setWind(*(static_cast<Point*>(event->getUserData())));
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    
    return true;
}

void WindIndicator::setWind(Point pos)
{
    _arrow->runAction(EaseElasticOut::create(RotateTo::create(1, 90-CC_RADIANS_TO_DEGREES(pos.getAngle()))));
    log("dist %d", int(pos.getDistance(Point::ZERO)*1000));
    _label->setString(Value(int(pos.getDistance(Point::ZERO)*1000)).asString());
    //pos.getAngle();
}


bool PowerIndicator::init(){
    powerbar = Sprite::create("powerbar.png");
    powerbar->setAnchorPoint(Point(0.5f, 0.30713f));
    powerbar->setVisible(false);
    addChild(powerbar);
    
    innerpower = Sprite::create("innerpower.png");
    //innerpower->setAnchorPoint(Point(0.5f, 0.34854f));
    //innerpower->setScale(0.1f);
    innerpower->setVisible(false);
    addChild(innerpower);
    
    auto increasePowerListener = EventListenerCustom::create("increasePower",CC_CALLBACK_0(PowerIndicator::increasePower, this));
    auto dismissPowerListener = EventListenerCustom::create("dismissPower",CC_CALLBACK_0(PowerIndicator::dismissPower, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(increasePowerListener, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(dismissPowerListener, this);
    
    this->scheduleUpdate();
    
    return true;
}

void PowerIndicator::increasePower(){
    _powerFlag = true;
    powerbar->setVisible(true);
    innerpower->setVisible(true);
    innerpower->runAction(FadeIn::create(0.1));
    powerbar->runAction(FadeIn::create(0.1));

    _tickPre = _tick;
}

void PowerIndicator::dismissPower(){
    _powerFlag = false;
    innerpower->runAction(FadeOut::create(0.4));
    powerbar->runAction(FadeOut::create(0.4));
}

void PowerIndicator::update(float delta){
    
    _tick++;
    
    if(_powerFlag){
        int tick = _tick - _tickPre;
        innerpower->setScale(tick/180.0f);
    }
}