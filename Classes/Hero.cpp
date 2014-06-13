//
//  Player.cpp
//  TankMultiplayer
//
//  Created by Jacky on 14-5-14.
//
//

#include "Hero.h"
#include <algorithm>
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "GPGSChecker.h"

USING_NS_CC;

extern GameScene* g_GameScene;

Hero* Hero::create(Side side /*= Myself*/,Body body/* = BOY*/, Wagon wagon/* = HORSEY*/, bool isfacetoright/* = true*/ ,std::string name)
{
    Hero *pRet = new Hero();
    if (pRet && pRet->init(side, body, wagon, isfacetoright,name))
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

bool Hero::init(Side side, Body body, Wagon wagon, bool isfacetoright, std::string name)
{
    _heroConfig.wagonConfig = g_wagonConfig[(int)wagon];
    _heroConfig.side = side;
    _heroConfig.body = body;
    _heroConfig.wagon = wagon;
    _heroConfig.state = IDLE;
    _heroConfig.isfacetoright = isfacetoright;
    
    _lasthp = _heroConfig.wagonConfig.hp;
    _wagonPoint = Node::create();
    //_wagonPoint->setPosition(_heroConfig.wagonConfig.offsetx, _heroConfig.wagonConfig.offsety);
    this->addChild(_wagonPoint);
    
    switch (_heroConfig.wagon) {
        case MECH:
        {
            _wagonASprite = Sprite::createWithSpriteFrameName("mechidle01.png");
            _wagonBSprite = Sprite::createWithSpriteFrameName("mechgunidle01.png");//front
            _wagonASprite->setAnchorPoint(Point(_heroConfig.wagonConfig.anchorx, _heroConfig.wagonConfig.anchory));
            _wagonBSprite->setAnchorPoint(Point(_heroConfig.wagonConfig.anchorx, _heroConfig.wagonConfig.anchory));
            _wagonPoint->addChild(_wagonASprite,1);
            _wagonPoint->addChild(_wagonBSprite,3);
        }
            break;
        case HORSEY:
        {
            _wagonASprite = Sprite::createWithSpriteFrameName("cnm_idle01.png");
            _wagonASprite->setAnchorPoint(Point(_heroConfig.wagonConfig.anchorx, _heroConfig.wagonConfig.anchory));
            _wagonPoint->addChild(_wagonASprite,1);
        }
            break;
        case ROCK:
        {
            _wagonASprite = Sprite::createWithSpriteFrameName("rockidle01.png");
            _wagonASprite->setAnchorPoint(Point(_heroConfig.wagonConfig.anchorx, _heroConfig.wagonConfig.anchory));
            _wagonPoint->addChild(_wagonASprite,3);
        }
            break;
        case TANK:
        {
            _wagonASprite = Sprite::createWithSpriteFrameName("tankidle01.png");
            _wagonASprite->setAnchorPoint(Point(_heroConfig.wagonConfig.anchorx, _heroConfig.wagonConfig.anchory));
            _wagonPoint->addChild(_wagonASprite,3);
        }
            break;
        default:
            break;
    }
    
    radius = 20;
    
//    auto drawN = DrawNode::create();
//    drawN ->drawDot(Point::ZERO, radius, Color4F(0,1,0,0.5));
//    this->addChild(drawN);
    
    gunPoint = Node::create();
    _wagonPoint->addChild(gunPoint);
    gunPoint->setPosition(_heroConfig.wagonConfig.gunx, _heroConfig.wagonConfig.guny);
    
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
        _bodySprite->setPosition(_wagonASprite->getPosition()+ Point(_heroConfig.wagonConfig.offsetx, _heroConfig.wagonConfig.offsety));
        _wagonPoint->addChild(_bodySprite,2);
    }
    else// girl
    {
        if (_heroConfig.isfacetoright) {
            //_wagonPoint->setScaleX(-1);
        }
        
        _bodySprite = Sprite::create("girl.png");
        _bodySprite->setPosition(_wagonASprite->getPosition()+ Point(_heroConfig.wagonConfig.offsetx, _heroConfig.wagonConfig.offsety));
        _wagonPoint->addChild(_bodySprite,2);
    }
    
    //hp
    _hpBarOuter = Sprite::create("hpouter.png");
    _hpBarOuter->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _hpBarOuter->setPositionY(-75);
    this->addChild(_hpBarOuter);
    
    auto hpSprite = Sprite::create("hpinner.png");
    hpSprite->setColor(Color3B(255,231, 33));
    _hpBarBack = ProgressTimer::create(hpSprite);
    _hpBarBack->setType(ProgressTimer::Type::BAR);
    _hpBarBack->setBarChangeRate(Point(1,0));
    _hpBarBack->setMidpoint(Point(0,0));
    //pt->runAction(ProgressFromTo::create(5,0,100));
    _hpBarBack->setPositionY(-75);
    this->addChild(_hpBarBack);
    
    auto hpInnerSprite = Sprite::create("hpinner.png");
    hpInnerSprite->setColor(Color3B(87, 227, 0));
    _hpInnerBar = ProgressTimer::create(hpInnerSprite);
    _hpInnerBar->setType(ProgressTimer::Type::BAR);
    _hpInnerBar->setBarChangeRate(Point(1,0));
    _hpInnerBar->setMidpoint(Point(0,0));
    _hpInnerBar->setPositionY(-75);
    this->addChild(_hpInnerBar);
    
    // name
    TTFConfig ttfConfig;
    ttfConfig.outlineSize = 5;
    ttfConfig.fontSize = 20;
    ttfConfig.fontFilePath = "arial.ttf";
    _nameLabel = Label::createWithTTF(ttfConfig, name.c_str(), TextHAlignment::CENTER, 200);
    _nameLabel->setPositionY(-50);
    _nameLabel->setSpacing(-5);
    _nameLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _nameLabel->enableOutline(Color4B::BLACK);
    if(_heroConfig.side == Myself){
        _nameLabel->setTextColor(Color4B(99, 253, 253, 255));
    } else {
        _nameLabel->setTextColor(Color4B(255, 99, 99, 255));
    }
    this->addChild(_nameLabel);
    
    // angle
    TTFConfig angleTTFConfig;
    angleTTFConfig.outlineSize = 4;
    angleTTFConfig.fontSize = 18;
    angleTTFConfig.fontFilePath = "arial.ttf";
    _angleLabel = Label::createWithTTF(angleTTFConfig, "0", TextHAlignment::CENTER, 30);
    _angleLabel->setPosition(75, -30);
    _angleLabel->setSpacing(-5);
    _angleLabel->enableOutline(Color4B::BLACK);
    _angleLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    this->addChild(_angleLabel,2);
    
    
    _sideSymbol = Sprite::create();
    if(_heroConfig.side == Myself){
        _sideSymbol->setTexture("you.png");
    } else {
        _sideSymbol->setTexture("enemy.png");
    }
    _sideSymbol->setPosition(0,180);
    _sideSymbol->runAction(RepeatForever::create(Sequence::create(EaseSineInOut::create(MoveBy::create(0.58, Point(0,25))),
                                                               EaseSineInOut::create(MoveBy::create(0.58, Point(0,-25))),
                                                               NULL)));
    this->addChild(_sideSymbol,2);
    
    _triangleSymbol = Sprite::create("triangle.png");
    _triangleSymbol->setPosition(0,140);
    _triangleSymbol->runAction(RepeatForever::create(Sequence::create(EaseSineInOut::create(MoveBy::create(0.58, Point(0,25))),
                                                               EaseSineInOut::create(MoveBy::create(0.58, Point(0,-25))),
                                                               NULL)));
    this->addChild(_triangleSymbol,2);
    
    hideAimer();
    hideTurnSymbol();
    
    this->scheduleUpdate();
    
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
            _mechMoveEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("mechmove.mp3",true);
        }
            break;
        case HORSEY:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.cnm_move)));
            _horseyMoveEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("horseymove.mp3",true);
        }
            break;
        case ROCK:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.rock_move)));
            _rockMoveEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("rockmove.mp3",true);
            
        }
            break;
        case TANK:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.tank_move)));
            _tankMoveEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tankmove.mp3",true);
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
            _mechMoveEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("mechmove.mp3",true);
        }
            break;
        case HORSEY:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.cnm_move)));
            _horseyMoveEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("horseymove.mp3",true);
        }
            break;
        case ROCK:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.rock_move)));
            _rockMoveEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("rockmove.mp3",true);
        }
            break;
        case TANK:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.tank_move)));
            _tankMoveEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tankmove.mp3",true);
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
            Animate* mech_idle = g_gameConfig.getAnimate(g_gameAnimation.mech_idle);
            mech_idle->setDuration(mech_idle->getDuration()/2);
            Animate* mechgun_idle = g_gameConfig.getAnimate(g_gameAnimation.mechgun_idle);
            mechgun_idle->setDuration(mechgun_idle->getDuration()/2);
            _wagonASprite->runAction(RepeatForever::create(mech_idle));
            _wagonBSprite->runAction(RepeatForever::create(mechgun_idle));
            //_mechShootEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("mechshoot.mp3");
        }
            break;
        case HORSEY:
        {
            _wagonASprite->stopAllActions();
            Animate* horsey_idle = g_gameConfig.getAnimate(g_gameAnimation.cnm_idle);
            horsey_idle->setDuration(horsey_idle->getDuration()/2);
            _wagonASprite->runAction(RepeatForever::create(horsey_idle));
            //_horseyShootEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("horseyshoot.mp3");
        }
            break;
        case ROCK:
        {
            _wagonASprite->stopAllActions();
            Animate* rock_idle = g_gameConfig.getAnimate(g_gameAnimation.rock_idle);
            rock_idle->setDuration(rock_idle->getDuration()/2);
            _wagonASprite->runAction(RepeatForever::create(rock_idle));
            //_rockShootEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("rockshoot.mp3");

        }
            break;
        case TANK:
        {
            _wagonASprite->stopAllActions();
            Animate* tank_idle = g_gameConfig.getAnimate(g_gameAnimation.tank_idle);
            tank_idle->setDuration(tank_idle->getDuration()/2);
            _wagonASprite->runAction(RepeatForever::create(tank_idle));
            //_tankShootEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tankshoot.mp3");
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

void Hero::endshoot(){
    switch (_heroConfig.wagon) {
        case MECH:
        {
            _wagonASprite->stopAllActions();
            _wagonBSprite->stopAllActions();
            Animate* mech_idle = g_gameConfig.getAnimate(g_gameAnimation.mech_idle);
            mech_idle->setDuration(mech_idle->getDuration()*2);
            Animate* mechgun_idle = g_gameConfig.getAnimate(g_gameAnimation.mechgun_idle);
            mechgun_idle->setDuration(mechgun_idle->getDuration()*2);
            _wagonASprite->runAction(Sequence::create(g_gameConfig.getAnimate(g_gameAnimation.mech_shoot),
                                                      CallFunc::create([=](){
                                                            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.mech_idle)));
                                                        })
                                                      ,
                                                      NULL));
            _wagonBSprite->runAction(Sequence::create(g_gameConfig.getAnimate(g_gameAnimation.mechgun_shoot),
                                                      CallFunc::create([=](){
                                                          _wagonBSprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.mechgun_idle)));
                                                      }),
                                                      NULL));
            //_mechShootEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("mechshoot.mp3");
        }
            break;
        case HORSEY:
        {
            _wagonASprite->stopAllActions();
            Animate* horsey_idle = g_gameConfig.getAnimate(g_gameAnimation.cnm_idle);
            horsey_idle->setDuration(horsey_idle->getDuration()*2);
            _wagonASprite->runAction(Sequence::create(g_gameConfig.getAnimate(g_gameAnimation.cnm_shoot),
                                                      CallFunc::create(
                                                      [&](){
                                                          _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.cnm_idle)));
                                                      }),
                                                      NULL));
            //_horseyShootEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("horseyshoot.mp3");
        }
            break;
        case ROCK:
        {
            _wagonASprite->stopAllActions();
            Animate* rock_idle = g_gameConfig.getAnimate(g_gameAnimation.rock_idle);
            rock_idle->setDuration(rock_idle->getDuration()*2);
            _wagonASprite->runAction(Sequence::create(g_gameConfig.getAnimate(g_gameAnimation.rock_shoot),
                                                      CallFunc::create([&](){
                                                          _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.rock_idle)));
                                                      }),
                                                      NULL));
            //_rockShootEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("rockshoot.mp3");
        }
            break;
        case TANK:
        {
            _wagonASprite->stopAllActions();
            Animate* tank_idle = g_gameConfig.getAnimate(g_gameAnimation.tank_idle);
            tank_idle->setDuration(tank_idle->getDuration()*2);
            _wagonASprite->runAction(Sequence::create(g_gameConfig.getAnimate(g_gameAnimation.tank_shoot),
                                                      CallFunc::create([&](){
                                                          _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.tank_idle)));
                                                      }),
                                                      NULL));
            //_tankShootEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tankshoot.mp3");
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
            CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(_mechMoveEffect);
        }
            break;
        case HORSEY:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.cnm_idle)));
            CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(_horseyMoveEffect);
        }
            break;
        case ROCK:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.rock_idle)));
            CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(_rockMoveEffect);
        }
            break;
        case TANK:
        {
            _wagonASprite->stopAllActions();
            _wagonASprite->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.tank_idle)));
            CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(_tankMoveEffect);
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
        float nowHPPercent = (float)_lasthp*100/(float)_heroConfig.wagonConfig.hp;
        log("nowHPpe %f", nowHPPercent);
        _hpInnerBar->setPercentage(nowHPPercent);
        _hpBarBack->runAction(ProgressTo::create(1,nowHPPercent));
        _nameLabel->runAction(Sequence::create(MoveBy::create(0.1, Point(0, 5)),
                                                            MoveBy::create(0.1, Point(0, -10)),
                                                            MoveBy::create(0.1, Point(0, 5)),
                                                            NULL));
        _hpInnerBar->runAction(Sequence::create(MoveBy::create(0.1, Point(0, 5)),
                                                MoveBy::create(0.1, Point(0, -10)),
                                                MoveBy::create(0.1, Point(0, 5)),
                                                NULL));
        _hpBarBack->runAction(Sequence::create(MoveBy::create(0.1, Point(0, 5)),
                                                MoveBy::create(0.1, Point(0, -10)),
                                                MoveBy::create(0.1, Point(0, 5)),
                                                NULL));

        _hpBarOuter->runAction(Sequence::create(MoveBy::create(0.1, Point(0, 5)),
                                                MoveBy::create(0.1, Point(0, -10)),
                                                MoveBy::create(0.1, Point(0, 5)),
                                                NULL));
        if (_lasthp<=5) {
            if (g_GameScene != nullptr) {
                if (g_GameScene->_playback) {
                    if(g_GameScene->getCurrentPlayer() != this)
                    {
                        GPGSChecker::getInstance()->checkInvincible();
                    }
                }
                else
                {
                    if(g_GameScene->getCurrentPlayer() == this)
                    {
                        GPGSChecker::getInstance()->checkInvincible();
                    }
                }
            }
        }

    }
    else
    {
        _lasthp = 0;
        _hpInnerBar->setPercentage(0);
        _hpBarBack->runAction(ProgressTo::create(1,_lasthp));
        if(!g_GameScene->over)
            _eventDispatcher->dispatchCustomEvent("playerdead", this);
    }
    return _lasthp;
}

void Hero::setLife(int life)
{
    _lasthp = life;
    float nowHPPercent = (float)_lasthp*100/(float)_heroConfig.wagonConfig.hp;
    if(nowHPPercent <= 20){
        _hpInnerBar->setColor(Color3B(227,96,0));
    }
    _hpBarBack->setPercentage(nowHPPercent);
    _hpInnerBar->setPercentage(nowHPPercent);
}

void Hero::updateAngle(int angle){
    char angleChars[10];
    sprintf(angleChars, "%d",angle);
    _angleLabel->setString(angleChars);
}

void Hero::update(float delta){
    int angle = abs(aim->getWorldAngle());
    if(angle >= 270){
        angle -= 360;
    } else if(angle >= 180){
        angle = -1 * (angle - 180);
    } else if(angle > 90){
        angle = 180 - angle;
    }
    
    this->updateAngle(angle);
}

void Hero::setName(std::string name){
    
    for (int i = 0; i < name.length(); i++)
        name[i] = toupper((int)name[i]);
    _nameLabel->setString(name);
}

void Hero::hideAimer(){
    aim->setVisible(false);
    _angleLabel->setVisible(false);
}

void Hero::showAimer(){
    aim->setVisible(true);
    _angleLabel->setVisible(true);
}

void Hero::hideTurnSymbol(){
    _sideSymbol->setVisible(false);
    _triangleSymbol->setVisible(false);
}

void Hero::showTurnSymbol(){
    _sideSymbol->setVisible(true);
    _triangleSymbol->setVisible(true);
}

void Hero::setSideSymbol(bool isYou)
{
    if(isYou){
        _sideSymbol->setTexture("you.png");
    } else {
        _sideSymbol->setTexture("enemy.png");
    }
}