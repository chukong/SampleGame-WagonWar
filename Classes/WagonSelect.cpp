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

USING_NS_CC;

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
    layer->setTag(WAGONSELECTTAG);
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
    auto boy_memuitem = MenuItemImage::create("boy_0.png","boy_1.png",
                                                  CC_CALLBACK_1(WagonSelect::boy_selected_callback, this));
    boy_memuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    boy_memuitem->setPosition(Point(60,g_visibleRect.visibleHeight/2+60));
    
    auto girl_memuitem = MenuItemImage::create("girl_0.png","girl_1.png",
                                                 CC_CALLBACK_1(WagonSelect::girl_selected_callback, this));
    girl_memuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    girl_memuitem->setPosition(Point(60,g_visibleRect.visibleHeight/2-60));
    
    auto menu = Menu::create(boy_memuitem, girl_memuitem, nullptr);
    menu->setPosition(Point::ZERO);
    this->addChild(menu,1);
    
    MenuItemImage* ready_menu;
    if (_turntype == FIRSR_TURN) {
        ready_menu = MenuItemImage::create("btn_ready_1.png","btn_ready_2.png",CC_CALLBACK_1(WagonSelect::ready_callback, this));
    }
    else if(_turntype == SECOND_TURN) {
        ready_menu = MenuItemImage::create("btn_start_1.png","btn_start_2.png",CC_CALLBACK_1(WagonSelect::ready_callback, this));
    }
    ready_menu->setPosition(Point(g_visibleRect.visibleWidth-120, g_visibleRect.visibleHeight-60));
    auto menu_ready = Menu::create(ready_menu, nullptr);
    menu_ready->setPosition(Point::ZERO);
    this->addChild(menu_ready,1);
    
    auto wagon1_menuitem = MenuItemImage::create("wagon_item_0.png","wagon_item_1.png",
                                                 CC_CALLBACK_1(WagonSelect::wagon1_selected_callback, this));
    wagon1_menuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    wagon1_menuitem->setPosition(Point(120,g_visibleRect.visibleHeight/2-80));
    
    auto wagon2_menuitem = MenuItemImage::create("wagon_item_0.png","wagon_item_1.png",
                                                 CC_CALLBACK_1(WagonSelect::wagon2_selected_callback, this));
    wagon2_menuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    wagon2_menuitem->setPosition(Point(310,g_visibleRect.visibleHeight/2-80));
    
    auto wagon3_menuitem = MenuItemImage::create("wagon_item_0.png","wagon_item_1.png",
                                                 CC_CALLBACK_1(WagonSelect::wagon3_selected_callback, this));
    wagon3_menuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    wagon3_menuitem->setPosition(Point(500,g_visibleRect.visibleHeight/2-80));
    
    auto wagon4_menuitem = MenuItemImage::create("wagon_item_0.png","wagon_item_1.png",
                                                 CC_CALLBACK_1(WagonSelect::wagon4_selected_callback, this));
    wagon4_menuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    wagon4_menuitem->setPosition(Point(690,g_visibleRect.visibleHeight/2-80));
    
    auto menu_wagon = Menu::create(wagon1_menuitem, wagon2_menuitem, wagon3_menuitem, wagon4_menuitem, nullptr);
    menu_wagon->setPosition(Point::ZERO);
    wagon_bk->addChild(menu_wagon,1);
}

void WagonSelect::ready_callback(Ref* ref)
{
    auto notouchlayer = NoTouchLayer::create();
    notouchlayer->setTag(NOTOUCHTAG);
    this->addChild(notouchlayer,100);
    
    if (_turntype == FIRSR_TURN) {
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
    doc.AddMember("player1", player1, allocator);
    
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
    doc.AddMember("player2", player2, allocator);
    
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    doc.Accept(writer);
    
    g_gameConfig.match_string = strbuf.GetString();
    log("setup_player2_matchdata...%s",g_gameConfig.match_string.c_str());
}

void WagonSelect::boy_selected_callback(Ref* ref)
{
    _isBoy = true;
}

void WagonSelect::girl_selected_callback(Ref* ref)
{
    _isBoy = false;
}

void WagonSelect::wagon1_selected_callback(cocos2d::Ref* ref)
{
    _wagon = 0;
}

void WagonSelect::wagon2_selected_callback(cocos2d::Ref* ref)
{
    _wagon = 1;
}

void WagonSelect::wagon3_selected_callback(cocos2d::Ref* ref)
{
    _wagon = 2;
}

void WagonSelect::wagon4_selected_callback(cocos2d::Ref* ref)
{
    _wagon = 3;
}
