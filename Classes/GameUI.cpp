//
//  GameUI.cpp
//  TankMultiplayer
//
//  Created by WuHao on 14-5-20.
//
//

#include "GameUI.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

bool GameUI::init()
{
    if(!Layer::init())
        return false;
    auto director = Director::getInstance();
    auto vsize = director->getVisibleSize();
    auto vorigin = director->getVisibleOrigin();
    
    _mytouchListener = EventListenerTouchOneByOne::create();
    _mytouchListener->onTouchBegan = CC_CALLBACK_2(GameUI::onTouchBegan, this);
    _mytouchListener->onTouchEnded = CC_CALLBACK_2(GameUI::onTouchEnded, this);
    _mytouchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mytouchListener, this);
    
    auto wind = WindIndicator::create();
    addChild(wind);
    wind->setPosition(vsize.width/2+vorigin.x, vsize.height- vorigin.y - 50);
    
    _power = PowerIndicator::create();
    _power->setPosition(vsize.width/2+vorigin.x, 0);
    _power->setAnchorPoint(Point(0.5f, 0.0f));
    //_power->setVisible(false);
    //_power->runAction(MoveTo::create(0.5, Point(vsize.width/2+vorigin.x, 0)));
    addChild(_power);
    
    _controlBoard = ControlBoard::create();
    _controlBoard->setPosition(vsize.width/2+vorigin.x, -120);
    _controlBoard->setAnchorPoint(Point(0.5f, 0.0f));
    _controlBoard->setVisible(false);
    addChild(_controlBoard);
    
    _playback = PlayBackIndictaor::create();
    _playback->setPosition(vsize.width/2+vorigin.x, 30);
    addChild(_playback);

    auto touchOffListener = EventListenerCustom::create("touch off", CC_CALLBACK_0(GameUI::_toggleTouchEnable, this, false));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchOffListener, this);
    
    auto touchOnListener = EventListenerCustom::create("touch on", CC_CALLBACK_0(GameUI::_toggleTouchEnable, this, true));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchOnListener, this);
    
    auto enemyTurnListener = EventListenerCustom::create("enemy's turn", CC_CALLBACK_0(GameUI::switchTurn, this, false));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(enemyTurnListener, this);
    
    auto myTurnListener = EventListenerCustom::create("my turn", CC_CALLBACK_0(GameUI::switchTurn, this, true));
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myTurnListener, this);
    
    return true;
}

void GameUI::_toggleTouchEnable(bool onoff)
{
    _mytouchListener->setEnabled(onoff);
}

void GameUI::switchTurn(bool isMyTurn){
    auto vsize = Director::getInstance()->getVisibleSize();
    auto vorigin = Director::getInstance()->getVisibleOrigin();
    
    if(!isMyTurn){
        
        auto turnSprite1 = Sprite::create("enemy_turn_1.png");
        auto turnSprite2 = Sprite::create("enemy_turn_2.png");
        turnSprite1->setPosition(Point(vorigin.x - 120, vsize.height/2 + vorigin.y + 70));
        turnSprite2->setPosition(Point(vorigin.x +vsize.width + 120, vsize.height/2 + vorigin.y - 30));
        turnSprite1->runAction(Sequence::create(
                                                MoveTo::create(0.3f, Point(vorigin.x + vsize.width/2 - 50,vsize.height/2 + vorigin.y + 70)),
                                                MoveBy::create(0.1f, Point(-90,0)),
                                                MoveBy::create(0.1f, Point(20,0)),
                                                DelayTime::create(1),
                                                MoveTo::create(0.1f, Point(vorigin.x - 120, vsize.height/2 + vorigin.y + 70)),
                                                RemoveSelf::create(),
                                                NULL));
        turnSprite2->runAction(Sequence::create(
                                                MoveTo::create(0.3f, Point(vorigin.x + vsize.width/2 + 50,vsize.height/2 + vorigin.y - 30)),
                                                MoveBy::create(0.1f, Point(90,0)),
                                                MoveBy::create(0.1f, Point(-20,0)),
                                                DelayTime::create(1),
                                                MoveTo::create(0.1f, Point(vorigin.x +vsize.width + 120, vsize.height/2 + vorigin.y - 30)),
                                                RemoveSelf::create(),
                                                NULL));
        
        this->addChild(turnSprite1);
        this->addChild(turnSprite2);
        
        _controlBoard->runAction(EaseBackIn::create(MoveTo::create(0.5, Point(vsize.width/2+vorigin.x, -120))));
        _controlBoard->setVisible(false);
        
    } else {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("yourturn.mp3");
        _playback->setVisible(false);
        _playback->unscheduleUpdate();
        _controlBoard->setVisible(true);
        _controlBoard->runAction(Sequence::create(DelayTime::create(1),EaseBackIn::create(MoveTo::create(0.5, Point(vsize.width/2+vorigin.x, 0))), NULL));
        auto turnSprite1 = Sprite::create("your_turn_1.png");
        auto turnSprite2 = Sprite::create("your_turn_2.png");
        turnSprite1->setPosition(Point(vorigin.x - 120, vsize.height/2 + vorigin.y));
        turnSprite2->setPosition(Point(vorigin.x +vsize.width + 120, vsize.height/2 + vorigin.y));
        turnSprite1->runAction(Sequence::create(DelayTime::create(0.8),
                                                MoveTo::create(0.3f, Point(vorigin.x + vsize.width/2 - 100,vsize.height/2 + vorigin.y)),
                                                MoveBy::create(0.1f, Point(-90,0)),
                                                MoveBy::create(0.1f, Point(20,0)),
                                                DelayTime::create(1),
                                                MoveTo::create(0.1f, Point(vorigin.x - 120, vsize.height/2 + vorigin.y)),
                                                RemoveSelf::create(),
                                                NULL));
        turnSprite2->runAction(Sequence::create(DelayTime::create(0.8),
                                                MoveTo::create(0.3f, Point(vorigin.x + vsize.width/2 + 100,vsize.height/2 + vorigin.y)),
                                                MoveBy::create(0.1f, Point(90,0)),
                                                MoveBy::create(0.1f, Point(-20,0)),
                                                DelayTime::create(1),
                                                MoveTo::create(0.1f, Point(vorigin.x +vsize.width + 120, vsize.height/2 + vorigin.y)),
                                                RemoveSelf::create(),
                                                NULL));
        this->addChild(turnSprite1);
        this->addChild(turnSprite2);
        
    }
}

bool GameUI::onTouchBegan(Touch *touch, Event *event)
{

//    if(touch->getLocation().y > 600)
//    {
//        _eventDispatcher->dispatchCustomEvent("randomWind");
//        return true;
//    }

    return false;
}

void GameUI::onTouchEnded(Touch *touch, Event *event)
{
    
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
    
    _powerbar = Sprite::create("powerbar.png");
    _powerbar->setAnchorPoint(Point(0.5f, 0.30713f));
    _powerbar->setPosition(0,191);
    _powerbar->setVisible(false);
    addChild(_powerbar);
    
    _innerpower = Sprite::create("innerpower.png");
    //innerpower->setAnchorPoint(Point(0.5f, 0.34854f));
    //innerpower->setScale(0.1f);
    _innerpower->setPosition(0,191);
    _innerpower->setVisible(false);
    addChild(_innerpower);
    
    auto increasePowerListener = EventListenerCustom::create("increasePower",CC_CALLBACK_0(PowerIndicator::increasePower, this));
    auto dismissPowerListener = EventListenerCustom::create("dismissPower",CC_CALLBACK_0(PowerIndicator::dismissPower, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(increasePowerListener, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(dismissPowerListener, this);

    this->scheduleUpdate();
    
    return true;
}

void PowerIndicator::increasePower(){
    _powerFlag = true;
    _powerbar->setVisible(true);
    _innerpower->setVisible(true);
    _innerpower->setScale(0);
    _innerpower->runAction(FadeIn::create(0.1));
    _powerbar->runAction(FadeIn::create(0.1));
    //CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("firebuttondown.mp3");
    _increaseEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("power.mp3");
    _tickPre = _tick;
}

void PowerIndicator::dismissPower(){
    _powerFlag = false;
    _innerpower->runAction(FadeOut::create(0.4));
    _powerbar->runAction(FadeOut::create(0.4));
    //CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("firebuttonup.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(_increaseEffect);
}

void PowerIndicator::update(float delta){
    
    _tick++;
    
    if(_powerFlag){
        int tick = _tick - _tickPre;
        tick = tick>180?180:tick;
        _innerpower->setScale(tick/180.0f);
    }
}

bool PlayBackIndictaor::init(){
    
    auto playBackBar = Sprite::create("playbackbar.png");
    playBackBar->setPosition(Point(0,60));
    addChild(playBackBar);
    
    auto innerBarSprite = Sprite::create("playbackinnerbar.png");
    _playBackInnerBar = ProgressTimer::create(innerBarSprite);
    _playBackInnerBar->setPosition(Point(3 ,60));
    _playBackInnerBar->setScaleX(700.0f/417.0f);
    _playBackInnerBar->setPercentage(0);
    _playBackInnerBar->setType(ProgressTimer::Type::BAR);
    _playBackInnerBar->setMidpoint(Point(0.0f,0.0f));
    _playBackInnerBar->setBarChangeRate(Point(1, 0));
    addChild(_playBackInnerBar);
    
    auto playTriangle = Sprite::create("playtriangle.png");
    playTriangle->setPosition(-335, 105);
    playTriangle->runAction(RepeatForever::create(Blink::create(1,1)));
    addChild(playTriangle);
    
    TTFConfig turnTTFConfig;
    turnTTFConfig.outlineSize = 3;
    turnTTFConfig.fontSize = 25;
    turnTTFConfig.fontFilePath = "britanic bold.ttf";
    _turnInfoLabel = Label::createWithTTF(turnTTFConfig, "PLAYBACK ENEMY‘S TURN", TextHAlignment::CENTER, 500);
    _turnInfoLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    _turnInfoLabel->setPosition(-310, 105);
    _turnInfoLabel->setSpacing(-5);
    _turnInfoLabel->enableOutline(Color4B::BLACK);
    addChild(_turnInfoLabel);
    
    TTFConfig timeTTFConfig;
    timeTTFConfig.outlineSize = 3;
    timeTTFConfig.fontSize = 25;
    timeTTFConfig.fontFilePath = "britanic bold.ttf";
    _timeLabel = Label::createWithTTF(timeTTFConfig, "0'13:200", TextHAlignment::CENTER, 150);
    _timeLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
    _timeLabel->setPosition(350, 105);
    _timeLabel->setSpacing(-5);
    _timeLabel->enableOutline(Color4B::BLACK);
    addChild(_timeLabel);
    
    auto tickSumListener = EventListenerCustom::create("tickSum", [=](EventCustom* event){
        int tickSum = (intptr_t)(event->getUserData());
        _playBackTickSum = tickSum;
    });
    
    auto nameSumListener = EventListenerCustom::create("enemy", [=](EventCustom* event){
        char* enemy = static_cast<char*>(event->getUserData());
        std::string tmp(enemy);
        _turnInfoLabel->setString(std::string("PLAYBACK   " + tmp + "‘S   TURN"));
    });

    _eventDispatcher->addEventListenerWithSceneGraphPriority(tickSumListener, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(nameSumListener, this);
    
    this->scheduleUpdate();
    
    return true;
}

void PlayBackIndictaor::update(float delta){
    int tickSum = _playBackTickSum;
    int tmpTick = (tickSum - _tick)<0?0:tickSum - _tick;
    float timeSum = tmpTick * 0.016;
    int min = timeSum / 60;
    int second = timeSum;
    int msecond = (timeSum - (int)timeSum) * 1000;
    char time [15];
    sprintf(time, "%d ' %d : %d",min,second,msecond);
    //log("time %s",time);
    _tick ++;
    _playBackInnerBar->setPercentage(_tick/(float)tickSum * 100);
    _timeLabel->setString(std::string(time));
    //log("percentage....%d",_playBackInnerBar->getPercent());
}

bool ControlBoard::init(){
    
    _left = Sprite::create("button_normal.png");
    //left->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    _left->setPosition(Point(- 170,60));
    addChild(_left,2);
    
    _right = Sprite::create("button_normal.png");
    _right->setScaleX(-1);
    _right->setPosition(Point(170,60));
    addChild(_right,2);
    
    auto board = Sprite::create("board.png");
    board->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
    //board->setPosition(vsize.width/2+vorigin.x,0);
    addChild(board);
    
    _fire = Sprite::create("kaboom_normal.png");
    _fire->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
    //fire->setScale(2.0f);
    addChild(_fire);
    //fire->setPosition(Point(vsize.width/2, 0));
    
    auto _mytouchListener = EventListenerTouchOneByOne::create();
    _mytouchListener->onTouchBegan = CC_CALLBACK_2(ControlBoard::onTouchBegan, this);
    _mytouchListener->onTouchEnded = CC_CALLBACK_2(ControlBoard::onTouchEnded, this);
    _mytouchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mytouchListener, this);
    
    return true;
}

bool ControlBoard::onTouchBegan(Touch *touch, Event *event)
{
    log("powertouchbegan");
    log("touch %f, %f",touch->getLocation().x,touch->getLocation().y);
    _eventDispatcher->dispatchCustomEvent("angle check",touch);
    if(_left->getBoundingBox().containsPoint(this->convertTouchToNodeSpace(touch)))
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("movebuttondown.mp3");
        _eventDispatcher->dispatchCustomEvent("go left");
        _left->setTexture("button_pressed.png");
        _leftFlag=true;
        return true;
    }
    if(_right->getBoundingBox().containsPoint(this->convertTouchToNodeSpace(touch)))
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("movebuttondown.mp3");
        _eventDispatcher->dispatchCustomEvent("go right");
        _right->setTexture("button_pressed.png");
        _rightFlag=true;
        return true;
    }
    if(_fire->getBoundingBox().containsPoint(this->convertTouchToNodeSpace(touch)))
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("firebuttondown.mp3");
        _fire->setTexture("kaboom_pressed.png");
        _eventDispatcher->dispatchCustomEvent("start shoot");
        _eventDispatcher->dispatchCustomEvent("increasePower");
        _startShootFlag = true;
        return true;
    }
    return false;
}

void ControlBoard::onTouchEnded(Touch *touch, Event *event)
{
    if(_leftFlag)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("movebuttonup.mp3");
        _left->setTexture("button_normal.png");
        _eventDispatcher->dispatchCustomEvent("stop");
    }
    else if(_rightFlag)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("movebuttonup.mp3");
        _right->setTexture("button_normal.png");
        _eventDispatcher->dispatchCustomEvent("stop");
    }
    else if(_startShootFlag)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("firebuttonup.mp3");
        _fire->setTexture("kaboom_normal.png");
        _eventDispatcher->dispatchCustomEvent("end shoot");
        _eventDispatcher->dispatchCustomEvent("dismissPower");
    }
    
    _startShootFlag = false;
    _leftFlag = false;
    _rightFlag = false;
    
}
