//
//  WagonSelect.cpp
//  TankMultiplayer
//
//  Created by Jacky on 14-5-29.
//
//

#include "WagonSelect.h"
#include "GPGSManager.h"
#include "GameScene.h"
#include "VisibleRect.h"
#include "NoTouchLayer.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "MainScreenScene.h"
#include "SimpleAudioEngine.h"
#include "PopWindow.h"

USING_NS_CC;

extern GameConfig g_gameConfig;

WagonSelect* WagonSelect::create(TurnType turntype)
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
    auto bk =  Sprite::create("wagonselect_bk.png");
    if(g_visibleRect.visibleWidth>960)
    {
        bk->setScaleX(g_visibleRect.visibleWidth/960);
    }
    bk->setAnchorPoint(Point::ANCHOR_MIDDLE);
    bk->setPosition(g_visibleRect.center);
    this->addChild(bk);
    
    _wagon_bk = Sprite::create("wagon_bk.png");
    if(g_visibleRect.visibleWidth>960)
    {
        _wagon_bk->setScaleX(g_visibleRect.visibleWidth/960);
    }
    _wagon_bk->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    _wagon_bk->setPosition(Point(130,280));
    this->addChild(_wagon_bk,1);
    
    //boy or girl
    _boy_memuitem = MenuItemImage::create("boy_0.png","boy_1.png",
                                                  CC_CALLBACK_1(WagonSelect::boy_selected_callback, this));
    _boy_memuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _boy_memuitem->setPosition(Point(60,g_visibleRect.visibleHeight/2+60));
    _boy_memuitem->selected();
    
    _girl_memuitem = MenuItemImage::create("girl_0.png","girl_1.png",
                                                 CC_CALLBACK_1(WagonSelect::girl_selected_callback, this));
    _girl_memuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _girl_memuitem->setPosition(Point(60,g_visibleRect.visibleHeight/2-60));
    
    auto menu = Menu::create(_boy_memuitem, _girl_memuitem, nullptr);
    menu->setPosition(Point::ZERO);
    this->addChild(menu,1);
    
    //
    MenuItemImage* ready_menu;
    if (_turntype == FIRST_TURN) {
        ready_menu = MenuItemImage::create("btn_ready_1.png","btn_ready_2.png",CC_CALLBACK_1(WagonSelect::ready_callback, this));
    }
    else if(_turntype == SECOND_TURN) {
        ready_menu = MenuItemImage::create("btn_start_1.png","btn_start_2.png",CC_CALLBACK_1(WagonSelect::ready_callback, this));
    }
    ready_menu->setPosition(Point(g_visibleRect.visibleWidth-120, g_visibleRect.visibleHeight-60));
    auto menu_ready = Menu::create(ready_menu, nullptr);
    menu_ready->setPosition(Point::ZERO);
    this->addChild(menu_ready,1);
    
    //
    _wagon1_menuitem = MenuItemImage::create("wagon_item_0.png","wagon_item_1.png",
                                                 CC_CALLBACK_1(WagonSelect::wagon1_selected_callback, this));
    _wagon1_menuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _wagon1_menuitem->setPosition(Point(120,g_visibleRect.visibleHeight/2-80));
    _wagon1_menuitem->selected();
    
    _wagon2_menuitem = MenuItemImage::create("wagon_item_0.png","wagon_item_1.png",
                                                 CC_CALLBACK_1(WagonSelect::wagon2_selected_callback, this));
    _wagon2_menuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _wagon2_menuitem->setPosition(Point(310,g_visibleRect.visibleHeight/2-80));
    
    _wagon3_menuitem = MenuItemImage::create("wagon_item_0.png","wagon_item_1.png",
                                                 CC_CALLBACK_1(WagonSelect::wagon3_selected_callback, this));
    _wagon3_menuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _wagon3_menuitem->setPosition(Point(500,g_visibleRect.visibleHeight/2-80));
    
    _wagon4_menuitem = MenuItemImage::create("wagon_item_0.png","wagon_item_1.png",
                                                 CC_CALLBACK_1(WagonSelect::wagon4_selected_callback, this));
    _wagon4_menuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _wagon4_menuitem->setPosition(Point(690,g_visibleRect.visibleHeight/2-80));
    
    auto menu_wagon = Menu::create(_wagon1_menuitem, _wagon2_menuitem, _wagon3_menuitem, _wagon4_menuitem, nullptr);
    menu_wagon->setPosition(Point::ZERO);
    _wagon_bk->addChild(menu_wagon,1);
    
    //
    auto wagon1 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("cnm_idle01.png"));
    wagon1->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.cnm_idle)));
    wagon1->setAnchorPoint(Point::ANCHOR_MIDDLE);
    wagon1->setPosition(Point(120,g_visibleRect.visibleHeight/2-10));
    wagon1->setScale(1.3f);
    _wagon_bk->addChild(wagon1, 2);
    
    auto wagon2 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("tankidle01.png"));
    wagon2->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.tank_idle)));
    wagon2->setAnchorPoint(Point::ANCHOR_MIDDLE);
    wagon2->setPosition(Point(310,g_visibleRect.visibleHeight/2-15));
    wagon2->setScale(1.3f);
    _wagon_bk->addChild(wagon2, 3);
    
    auto wagon3 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("rockidle01.png"));
    wagon3->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.rock_idle)));
    wagon3->setAnchorPoint(Point::ANCHOR_MIDDLE);
    wagon3->setPosition(Point(500,g_visibleRect.visibleHeight/2));
    wagon3->setScale(1.3f);
    _wagon_bk->addChild(wagon3, 3);
    
    auto wagon4_1 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mechidle01.png"));
    wagon4_1->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.mech_idle)));
    wagon4_1->setAnchorPoint(Point::ANCHOR_MIDDLE);
    wagon4_1->setPosition(Point(700,g_visibleRect.visibleHeight/2-5));
    wagon4_1->setScale(1.3f);
    _wagon_bk->addChild(wagon4_1, 2);
    
    auto wagon4_2 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mechgunidle01.png"));
    wagon4_2->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.mechgun_idle)));
    wagon4_2->setAnchorPoint(Point::ANCHOR_MIDDLE);
    wagon4_2->setPosition(Point(700,g_visibleRect.visibleHeight/2-20));
    wagon4_2->setScale(1.3f);
    _wagon_bk->addChild(wagon4_2, 3);

    //dude
    auto offsetPoint1 = Point(30.0f, 55.0f);
    _dude1 = Sprite::create("boy.png");
    _dude1->setAnchorPoint(Point(Point::ANCHOR_MIDDLE));
    _dude1->setPosition(wagon1->getPosition() + offsetPoint1);
    _dude1->setScale(1.3f);
    _wagon_bk->addChild(_dude1,2);

    auto offsetPoint2 = Point(20.0f, 65.0f);
    _dude2 = Sprite::create("boy.png");
    _dude2->setAnchorPoint(Point(Point::ANCHOR_MIDDLE));
    _dude2->setPosition(wagon2->getPosition() + offsetPoint2);
    _dude2->setScale(1.3f);
    _wagon_bk->addChild(_dude2,2);
    
    auto offsetPoint3 = Point(30.0f, 50.0f);
    _dude3 = Sprite::create("boy.png");
    _dude3->setAnchorPoint(Point(Point::ANCHOR_MIDDLE));
    _dude3->setPosition(wagon3->getPosition() + offsetPoint3);
    _dude3->setScale(1.3f);
    _wagon_bk->addChild(_dude3,2);
    
    auto offsetPoint4 = Point(10.0f, 55.0f);
    _dude4 = Sprite::create("boy.png");
    _dude4->setAnchorPoint(Point(Point::ANCHOR_MIDDLE));
    _dude4->setPosition(wagon4_1->getPosition() + offsetPoint4);
    _dude4->setScale(1.3f);
    _wagon_bk->addChild(_dude4,2);
    
    //shawdow
    auto shawdow1 = Sprite::create("shawdow.png");
    shawdow1->setAnchorPoint(Point::ANCHOR_MIDDLE);
    shawdow1->setPosition(Point(120,g_visibleRect.visibleHeight/2-70));
    shawdow1->setOpacity(200);
    _wagon_bk->addChild(shawdow1,2);

    auto shawdow2 = Sprite::create("shawdow.png");
    shawdow2->setAnchorPoint(Point::ANCHOR_MIDDLE);
    shawdow2->setPosition(Point(310,g_visibleRect.visibleHeight/2-70));
    shawdow2->setOpacity(200);
    _wagon_bk->addChild(shawdow2,2);

    auto shawdow3 = Sprite::create("shawdow.png");
    shawdow3->setAnchorPoint(Point::ANCHOR_MIDDLE);
    shawdow3->setPosition(Point(500,g_visibleRect.visibleHeight/2-70));
    shawdow3->setOpacity(200);
    _wagon_bk->addChild(shawdow3,2);

    auto shawdow4 = Sprite::create("shawdow.png");
    shawdow4->setAnchorPoint(Point::ANCHOR_MIDDLE);
    shawdow4->setPosition(Point(690,g_visibleRect.visibleHeight/2-70));
    shawdow4->setOpacity(200);
    _wagon_bk->addChild(shawdow4,2);

    // params
    initWagonParams();
    
    // name
    TTFConfig horseyTTFConfig;
    horseyTTFConfig.outlineSize = 3;
    horseyTTFConfig.fontSize = 30;
    horseyTTFConfig.fontFilePath = "fonts/britanic bold.ttf";
    auto horseyLabel = Label::createWithTTF(horseyTTFConfig, "HORSEY", TextHAlignment::CENTER, 20);
    horseyLabel->setPosition(120,g_visibleRect.visibleHeight/2-110);
    horseyLabel->setSpacing(-5);
    horseyLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    horseyLabel->enableOutline(Color4B::BLACK);
    _wagon_bk->addChild(horseyLabel,2);
    
    TTFConfig tankTTFConfig;
    tankTTFConfig.outlineSize = 3;
    tankTTFConfig.fontSize = 30;
    tankTTFConfig.fontFilePath = "fonts/britanic bold.ttf";
    auto tankLabel = Label::createWithTTF(horseyTTFConfig, "TANK", TextHAlignment::CENTER, 20);
    tankLabel->setPosition(310,g_visibleRect.visibleHeight/2-110);
    tankLabel->setSpacing(-5);
    tankLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    tankLabel->enableOutline(Color4B::BLACK);
    _wagon_bk->addChild(tankLabel,2);
    
    TTFConfig rockTTFConfig;
    rockTTFConfig.outlineSize = 3;
    rockTTFConfig.fontSize = 30;
    rockTTFConfig.fontFilePath = "fonts/britanic bold.ttf";
    auto rockLabel = Label::createWithTTF(horseyTTFConfig, "ROCK", TextHAlignment::CENTER, 20);
    rockLabel->setPosition(500,g_visibleRect.visibleHeight/2-110);
    rockLabel->setSpacing(-5);
    rockLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    rockLabel->enableOutline(Color4B::BLACK);
    _wagon_bk->addChild(rockLabel,2);
    
    TTFConfig mechTTFConfig;
    mechTTFConfig.outlineSize = 3;
    mechTTFConfig.fontSize = 30;
    mechTTFConfig.fontFilePath = "fonts/britanic bold.ttf";
    auto mechLabel = Label::createWithTTF(horseyTTFConfig, "MECH", TextHAlignment::CENTER, 20);
    mechLabel->setPosition(690,g_visibleRect.visibleHeight/2-110);
    mechLabel->setSpacing(-5);
    mechLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    mechLabel->enableOutline(Color4B::BLACK);
    _wagon_bk->addChild(mechLabel,2);
    
    // flash
    flash = Sprite::create("wagon_flash.png");
    flash->setAnchorPoint(Point::ANCHOR_MIDDLE);
    flash->setPosition(Point(120,g_visibleRect.visibleHeight/2-80));
    flash->runAction(RepeatForever::create(Blink::create(1,1)));
    _wagon_bk->addChild(flash,3);
    
    auto returnMenuListener = EventListenerCustom::create("returntoMenu", CC_CALLBACK_0(WagonSelect::returntoMenu, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(returnMenuListener, this);
    
    auto wagonselectshowpopwindowlistener = EventListenerCustom::create("wagonselectshowpopwindowlistener", CC_CALLBACK_0(WagonSelect::showConnectingPopWindow, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(wagonselectshowpopwindowlistener, this);
}

void WagonSelect::ready_callback(Ref* ref)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("readystart.mp3");
//    auto notouchlayer = NoTouchLayer::create();
//    notouchlayer->setTag(NOTOUCHTAG);
//    Director::getInstance()->getRunningScene()->addChild(notouchlayer,100);
    
//    auto popwindow = PopWindow::create();
//    this->addChild(popwindow,100);
    
    if (_turntype == FIRST_TURN) {
        setup_player1_mactchdata();
        GPGSManager::TakeTurn(false, false);
    }
    else if(_turntype == SECOND_TURN){
        setup_player2_mactchdata();
        Director::getInstance()->replaceScene(GameScene::createScene());
    }
}

void WagonSelect::setup_player1_mactchdata()
{
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    
    doc.AddMember("turn", 1, allocator);
    
    rapidjson::Value player1(rapidjson::kObjectType);
    player1.AddMember("name", g_gameConfig.player1Name.c_str(), allocator);
    player1.AddMember("wagon", _wagon, allocator);
    player1.AddMember("male", _isBoy, allocator);
    player1.AddMember("hp", g_wagonConfig[_wagon].hp, allocator);
    player1.AddMember("posx", 229, allocator);
    player1.AddMember("posy", 513, allocator);
    player1.AddMember("rot", 0, allocator);
    player1.AddMember("shootangle", "", allocator);
    player1.AddMember("facing", "right", allocator);
    doc.AddMember("player1", player1, allocator);
    
    srand((unsigned)time(NULL));
    srand(rand());
    doc.AddMember("windx", CCRANDOM_MINUS1_1()*0.025, allocator);
    srand(rand());
    doc.AddMember("windy", CCRANDOM_MINUS1_1()*0.025, allocator);
    rapidjson::Value array(rapidjson::kArrayType);
    doc.AddMember("explosions", array, allocator);
    rapidjson::Value actions(rapidjson::kArrayType);
    doc.AddMember("actions", actions, allocator);

    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    doc.Accept(writer);
    
    g_gameConfig.match_string = strbuf.GetString();

}

void WagonSelect::setup_player2_mactchdata()
{
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(g_gameConfig.match_string.c_str());
    
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value player2(rapidjson::kObjectType);
    player2.AddMember("name", g_gameConfig.player2Name.c_str(), allocator);
    player2.AddMember("wagon", _wagon, allocator);
    player2.AddMember("male", _isBoy, allocator);
    player2.AddMember("hp", g_wagonConfig[_wagon].hp, allocator);
    player2.AddMember("posx", 1443, allocator);
    player2.AddMember("posy", 509, allocator);
    player2.AddMember("rot", 0, allocator);
    player2.AddMember("shootangle", "", allocator);
    player2.AddMember("facing", "left", allocator);
    doc.AddMember("player2", player2, allocator);
    
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    doc.Accept(writer);
    
    g_gameConfig.match_string = strbuf.GetString();
    log("setup_player2_matchdata...%s",g_gameConfig.match_string.c_str());
}

void WagonSelect::boy_selected_callback(Ref* ref)
{
    if(!_isBoy){
        _dude1->setTexture("boy.png");
        _dude2->setTexture("boy.png");
        _dude3->setTexture("boy.png");
        _dude4->setTexture("boy.png");
    }
    
    _boy_memuitem->selected();
    _girl_memuitem->unselected();
    
    _isBoy = true;
}

void WagonSelect::girl_selected_callback(Ref* ref)
{
    if(_isBoy){
        _dude1->setTexture("girl.png");
        _dude2->setTexture("girl.png");
        _dude3->setTexture("girl.png");
        _dude4->setTexture("girl.png");
    }
    
    _boy_memuitem->unselected();
    _girl_memuitem->selected();
    
    _isBoy = false;
}

void WagonSelect::wagon1_selected_callback(cocos2d::Ref* ref)
{
    _wagon1_menuitem->selected();
    _wagon2_menuitem->unselected();
    _wagon3_menuitem->unselected();
    _wagon4_menuitem->unselected();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("selectwagon.mp3");
    flash->setPosition(_wagon1_menuitem->getPosition());
    _wagon = 0;
}

void WagonSelect::wagon2_selected_callback(cocos2d::Ref* ref)
{
    _wagon1_menuitem->unselected();
    _wagon2_menuitem->selected();
    _wagon3_menuitem->unselected();
    _wagon4_menuitem->unselected();
    
    flash->setPosition(_wagon2_menuitem->getPosition());
    _wagon = 1;
}

void WagonSelect::wagon3_selected_callback(cocos2d::Ref* ref)
{
    _wagon1_menuitem->unselected();
    _wagon2_menuitem->unselected();
    _wagon3_menuitem->selected();
    _wagon4_menuitem->unselected();
    
    flash->setPosition(_wagon3_menuitem->getPosition());
    _wagon = 2;
}

void WagonSelect::wagon4_selected_callback(cocos2d::Ref* ref)
{
    _wagon1_menuitem->unselected();
    _wagon2_menuitem->unselected();
    _wagon3_menuitem->unselected();
    _wagon4_menuitem->selected();
    
    flash->setPosition(_wagon4_menuitem->getPosition());
    _wagon = 3;
}

void WagonSelect::returntoMenu()
{
    scheduleOnce(schedule_selector(WagonSelect::entertoMenu), 1.0f);
}

void WagonSelect::entertoMenu(float dt)
{
    auto scene = MainScreenScene::createScene(true);
    cocos2d::Director::getInstance()->replaceScene(scene);
}

void WagonSelect::initWagonParams(){
    
    auto attack1 = Sprite::create("attack.png");
    auto attack2 = Sprite::create("attack.png");
    auto attack3 = Sprite::create("attack.png");
    auto attack4 = Sprite::create("attack.png");
    
    attack1->setPosition(70, g_visibleRect.visibleHeight/2-185);
    _wagon_bk->addChild(attack1,2);
    attack2->setPosition(260, g_visibleRect.visibleHeight/2-185);
    _wagon_bk->addChild(attack2,2);
    attack3->setPosition(450, g_visibleRect.visibleHeight/2-185);
    _wagon_bk->addChild(attack3,2);
    attack4->setPosition(640, g_visibleRect.visibleHeight/2-185);
    _wagon_bk->addChild(attack4,2);
    
    auto attackBar1 = ui::LoadingBar::create("attackBar.png");
    auto attackBar2 = ui::LoadingBar::create("attackBar.png");
    auto attackBar3 = ui::LoadingBar::create("attackBar.png");
    auto attackBar4 = ui::LoadingBar::create("attackBar.png");
    
    attackBar1->setPosition(Point(135, g_visibleRect.visibleHeight/2-185));
    attackBar1->setPercent(100);
    _wagon_bk->addChild(attackBar1,4);
    attackBar2->setPosition(Point(325, g_visibleRect.visibleHeight/2-185));
    attackBar2->setPercent(100);
    _wagon_bk->addChild(attackBar2,4);
    attackBar3->setPosition(Point(515, g_visibleRect.visibleHeight/2-185));
    attackBar3->setPercent(100);
    _wagon_bk->addChild(attackBar3,4);
    attackBar4->setPosition(Point(705, g_visibleRect.visibleHeight/2-185));
    attackBar4->setPercent(100);
    _wagon_bk->addChild(attackBar4,4);
    
    auto explosion1 = Sprite::create("explosion.png");
    auto explosion2 = Sprite::create("explosion.png");
    auto explosion3 = Sprite::create("explosion.png");
    auto explosion4 = Sprite::create("explosion.png");
    
    explosion1->setPosition(70, g_visibleRect.visibleHeight/2-255);
    _wagon_bk->addChild(explosion1,2);
    explosion2->setPosition(260, g_visibleRect.visibleHeight/2-255);
    _wagon_bk->addChild(explosion2,2);
    explosion3->setPosition(450, g_visibleRect.visibleHeight/2-255);
    _wagon_bk->addChild(explosion3,2);
    explosion4->setPosition(640, g_visibleRect.visibleHeight/2-255);
    _wagon_bk->addChild(explosion4,2);

    
    auto explosionBar1 = ui::LoadingBar::create("explosionBar.png");
    auto explosionBar2 = ui::LoadingBar::create("explosionBar.png");
    auto explosionBar3 = ui::LoadingBar::create("explosionBar.png");
    auto explosionBar4 = ui::LoadingBar::create("explosionBar.png");
    
    explosionBar1->setPosition(Point(135, g_visibleRect.visibleHeight/2-255));
    explosionBar1->setPercent(100);
    _wagon_bk->addChild(explosionBar1,4);
    explosionBar2->setPosition(Point(325, g_visibleRect.visibleHeight/2-255));
    explosionBar2->setPercent(100);
    _wagon_bk->addChild(explosionBar2,4);
    explosionBar3->setPosition(Point(515, g_visibleRect.visibleHeight/2-255));
    explosionBar3->setPercent(100);
    _wagon_bk->addChild(explosionBar3,4);
    explosionBar4->setPosition(Point(705, g_visibleRect.visibleHeight/2-255));
    explosionBar4->setPercent(100);
    _wagon_bk->addChild(explosionBar4,4);
    
    auto hp1 = Sprite::create("hp.png");
    auto hp2 = Sprite::create("hp.png");
    auto hp3 = Sprite::create("hp.png");
    auto hp4 = Sprite::create("hp.png");
    
    hp1->setPosition(70, g_visibleRect.visibleHeight/2-150);
    _wagon_bk->addChild(hp1,2);
    hp2->setPosition(260, g_visibleRect.visibleHeight/2-150);
    _wagon_bk->addChild(hp2,2);
    hp3->setPosition(450, g_visibleRect.visibleHeight/2-150);
    _wagon_bk->addChild(hp3,2);
    hp4->setPosition(640, g_visibleRect.visibleHeight/2-150);
    _wagon_bk->addChild(hp4,2);
    
    auto hpBar1 = ui::LoadingBar::create("hpBar.png");
    auto hpBar2 = ui::LoadingBar::create("hpBar.png");
    auto hpBar3 = ui::LoadingBar::create("hpBar.png");
    auto hpBar4 = ui::LoadingBar::create("hpBar.png");
    
    hpBar1->setPosition(Point(135, g_visibleRect.visibleHeight/2-150));
    hpBar1->setPercent(100);
    _wagon_bk->addChild(hpBar1,4);
    hpBar2->setPosition(Point(325, g_visibleRect.visibleHeight/2-150));
    hpBar2->setPercent(100);
    _wagon_bk->addChild(hpBar2,4);
    hpBar3->setPosition(Point(515, g_visibleRect.visibleHeight/2-150));
    hpBar3->setPercent(100);
    _wagon_bk->addChild(hpBar3,4);
    hpBar4->setPosition(Point(705, g_visibleRect.visibleHeight/2-150));
    hpBar4->setPercent(100);
    _wagon_bk->addChild(hpBar4,4);
    
    auto speed1 = Sprite::create("speed.png");
    auto speed2 = Sprite::create("speed.png");
    auto speed3 = Sprite::create("speed.png");
    auto speed4 = Sprite::create("speed.png");
    
    speed1->setPosition(70, g_visibleRect.visibleHeight/2-220);
    _wagon_bk->addChild(speed1,2);
    speed2->setPosition(260, g_visibleRect.visibleHeight/2-220);
    _wagon_bk->addChild(speed2,2);
    speed3->setPosition(450, g_visibleRect.visibleHeight/2-220);
    _wagon_bk->addChild(speed3,2);
    speed4->setPosition(640, g_visibleRect.visibleHeight/2-220);
    _wagon_bk->addChild(speed4,2);
    
    auto speedBar1 = ui::LoadingBar::create("speedBar.png");
    auto speedBar2 = ui::LoadingBar::create("speedBar.png");
    auto speedBar3 = ui::LoadingBar::create("speedBar.png");
    auto speedBar4 = ui::LoadingBar::create("speedBar.png");
    
    speedBar1->setPosition(Point(135, g_visibleRect.visibleHeight/2-220));
    speedBar1->setPercent(100);
    _wagon_bk->addChild(speedBar1,4);
    speedBar2->setPosition(Point(325, g_visibleRect.visibleHeight/2-220));
    speedBar2->setPercent(100);
    _wagon_bk->addChild(speedBar2,4);
    speedBar3->setPosition(Point(515, g_visibleRect.visibleHeight/2-220));
    speedBar3->setPercent(100);
    _wagon_bk->addChild(speedBar3,4);
    speedBar4->setPosition(Point(705, g_visibleRect.visibleHeight/2-220));
    speedBar4->setPercent(100);
    _wagon_bk->addChild(speedBar4,4);
}

void WagonSelect::showConnectingPopWindow()
{
    scheduleOnce(schedule_selector(WagonSelect::showConnectingPopWindowWithDelay), transSceneDelayTime/2);
}
void WagonSelect::showConnectingPopWindowWithDelay(float dt)
{
    auto popwindow = PopWindow::create();
    Director::getInstance()->getRunningScene()->addChild(popwindow,100);
}
