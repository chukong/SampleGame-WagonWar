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
    
    auto wagon_bk = Sprite::create("wagon_bk.png");
    if(g_visibleRect.visibleWidth>960)
    {
        wagon_bk->setScaleX(g_visibleRect.visibleWidth/960);
    }
    wagon_bk->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    wagon_bk->setPosition(Point(130,280));
    this->addChild(wagon_bk,1);
    
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
    wagon_bk->addChild(menu_wagon,1);
    
    //
    auto wagon1 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("cnm_idle01.png"));
    wagon1->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.cnm_idle)));
    wagon1->setAnchorPoint(Point::ANCHOR_MIDDLE);
    wagon1->setPosition(Point(120,g_visibleRect.visibleHeight/2-20));
    wagon1->setScale(1.5f);
    wagon_bk->addChild(wagon1, 2);
    
    auto wagon2 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("tankidle01.png"));
    wagon2->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.tank_idle)));
    wagon2->setAnchorPoint(Point::ANCHOR_MIDDLE);
    wagon2->setPosition(Point(310,g_visibleRect.visibleHeight/2-20));
    wagon2->setScale(1.5f);
    wagon_bk->addChild(wagon2, 3);
    
    auto wagon3 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("rockidle01.png"));
    wagon3->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.rock_idle)));
    wagon3->setAnchorPoint(Point::ANCHOR_MIDDLE);
    wagon3->setPosition(Point(500,g_visibleRect.visibleHeight/2-10));
    wagon3->setScale(1.5f);
    wagon_bk->addChild(wagon3, 3);
    
    auto wagon4_1 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mechidle01.png"));
    wagon4_1->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.mech_idle)));
    wagon4_1->setAnchorPoint(Point::ANCHOR_MIDDLE);
    wagon4_1->setPosition(Point(700,g_visibleRect.visibleHeight/2-20));
    wagon4_1->setScale(1.5f);
    wagon_bk->addChild(wagon4_1, 2);
    
    auto wagon4_2 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mechgunidle01.png"));
    wagon4_2->runAction(RepeatForever::create(g_gameConfig.getAnimate(g_gameAnimation.mechgun_idle)));
    wagon4_2->setAnchorPoint(Point::ANCHOR_MIDDLE);
    wagon4_2->setPosition(Point(700,g_visibleRect.visibleHeight/2-20));
    wagon4_2->setScale(1.5f);
    wagon_bk->addChild(wagon4_2, 3);

    //dude
    auto offsetPoint1 = Point(35.0f, 55.0f);
    _dude1 = Sprite::create("boy.png");
    _dude1->setAnchorPoint(Point(Point::ANCHOR_MIDDLE));
    _dude1->setPosition(wagon1->getPosition() + offsetPoint1);
    _dude1->setScale(1.5f);
    wagon_bk->addChild(_dude1,2);

    auto offsetPoint2 = Point(20.0f, 65.0f);
    _dude2 = Sprite::create("boy.png");
    _dude2->setAnchorPoint(Point(Point::ANCHOR_MIDDLE));
    _dude2->setPosition(wagon2->getPosition() + offsetPoint2);
    _dude2->setScale(1.5f);
    wagon_bk->addChild(_dude2,2);
    
    auto offsetPoint3 = Point(30.0f, 50.0f);
    _dude3 = Sprite::create("boy.png");
    _dude3->setAnchorPoint(Point(Point::ANCHOR_MIDDLE));
    _dude3->setPosition(wagon3->getPosition() + offsetPoint3);
    _dude3->setScale(1.5f);
    wagon_bk->addChild(_dude3,2);
    
    auto offsetPoint4 = Point(10.0f, 55.0f);
    _dude4 = Sprite::create("boy.png");
    _dude4->setAnchorPoint(Point(Point::ANCHOR_MIDDLE));
    _dude4->setPosition(wagon4_1->getPosition() + offsetPoint4);
    _dude4->setScale(1.5f);
    wagon_bk->addChild(_dude4,2);
    
    //shawdow
    auto shawdow1 = Sprite::create("shawdow.png");
    shawdow1->setAnchorPoint(Point::ANCHOR_MIDDLE);
    shawdow1->setPosition(Point(120,g_visibleRect.visibleHeight/2-120));
    wagon_bk->addChild(shawdow1,2);

    auto shawdow2 = Sprite::create("shawdow.png");
    shawdow2->setAnchorPoint(Point::ANCHOR_MIDDLE);
    shawdow2->setPosition(Point(310,g_visibleRect.visibleHeight/2-120));
    wagon_bk->addChild(shawdow2,2);

    auto shawdow3 = Sprite::create("shawdow.png");
    shawdow3->setAnchorPoint(Point::ANCHOR_MIDDLE);
    shawdow3->setPosition(Point(500,g_visibleRect.visibleHeight/2-120));
    wagon_bk->addChild(shawdow3,2);

    auto shawdow4 = Sprite::create("shawdow.png");
    shawdow4->setAnchorPoint(Point::ANCHOR_MIDDLE);
    shawdow4->setPosition(Point(700,g_visibleRect.visibleHeight/2-120));
    wagon_bk->addChild(shawdow4,2);

    //flash
    flash = Sprite::create("wagon_flash.png");
    flash->setAnchorPoint(Point::ANCHOR_MIDDLE);
    flash->setPosition(Point(120,g_visibleRect.visibleHeight/2-80));
    flash->runAction(RepeatForever::create(Blink::create(1,1)));
    wagon_bk->addChild(flash,3);
    
    auto returnMenuListener = EventListenerCustom::create("returntoMenu", CC_CALLBACK_0(WagonSelect::returntoMenu, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(returnMenuListener, this);
}

void WagonSelect::ready_callback(Ref* ref)
{
    auto notouchlayer = NoTouchLayer::create();
    notouchlayer->setTag(NOTOUCHTAG);
    Director::getInstance()->getRunningScene()->addChild(notouchlayer,100);
    
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
    player1.AddMember("wagon", _wagon, allocator);
    player1.AddMember("male", _isBoy, allocator);
    player1.AddMember("hp", 1000, allocator);
    player1.AddMember("posx", 520, allocator);
    player1.AddMember("posy", 800, allocator);
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
    player2.AddMember("wagon", _wagon, allocator);
    player2.AddMember("male", _isBoy, allocator);
    player2.AddMember("hp", 1000, allocator);
    player2.AddMember("posx", 1000, allocator);
    player2.AddMember("posy", 800, allocator);
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
    log("call...return to menu");
    scheduleOnce(schedule_selector(GameScene::entertoMenu), 1.0f);
}

void WagonSelect::entertoMenu(float dt)
{
    log("call...entertomenu");
    auto scene = MainScreenScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(scene);
}
