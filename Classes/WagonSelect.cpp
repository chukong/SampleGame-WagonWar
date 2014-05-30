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
    bk->setScale(0.5f);
    bk->setAnchorPoint(Point::ANCHOR_MIDDLE);
    bk->setPosition(g_visibleRect.center);
    this->addChild(bk);
    
    //boy or girl
    auto boy_memuitem = MenuItemImage::create("boy_0.png","boy_1.png",
                                                  CC_CALLBACK_1(WagonSelect::boy_selected_callback, this));
    boy_memuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    boy_memuitem->setPosition(Point(30,g_visibleRect.visibleHeight/2+130));
    
    auto girl_memuitem = MenuItemImage::create("girl_0.png","girl_1.png",
                                                 CC_CALLBACK_1(WagonSelect::girl_selected_callback, this));
    girl_memuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    girl_memuitem->setPosition(Point(30,g_visibleRect.visibleHeight/2-130));
    
    auto menu = Menu::create(boy_memuitem, girl_memuitem, nullptr);
    menu->setPosition(Point::ZERO);
    bk->addChild(menu,1);
    
    MenuItemImage* ready_menu;
    if (_turntype == FIRSR_TURN) {
        ready_menu = MenuItemImage::create("btn_ready_1.png","btn_ready_1.png",CC_CALLBACK_1(WagonSelect::ready_callback, this));
    }
    else if(_turntype == SECOND_TURN) {
        ready_menu = MenuItemImage::create("btn_start_1.png","btn_start_1.png",CC_CALLBACK_1(WagonSelect::ready_callback, this));
    }
    ready_menu->setPosition(Point(g_visibleRect.visibleWidth-30, g_visibleRect.visibleHeight-30));
    auto menu_ready = Menu::create(ready_menu, nullptr);
    menu_ready->setPosition(Point::ZERO);
    bk->addChild(menu_ready);
    
    auto wagon_bk = Sprite::create("wagon_bk.png");
    wagon_bk->setAnchorPoint(Point::ANCHOR_MIDDLE);
    wagon_bk->setPosition(Point(530,270));
    bk->addChild(wagon_bk);
    
//    auto wagon1_menuitem = MenuItemImage::create("girl_0.png","girl_1.png",
//                                                 CC_CALLBACK_1(WagonSelect::girl_selected_callback, this));
//    girl_memuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
//    girl_memuitem->setPosition(Point(-200,g_visibleRect.visibleHeight/2-130));
}

void WagonSelect::ready_callback(Ref* ref)
{
    if (_turntype == FIRSR_TURN) {
        setup_player1_mactchdata();
        GPGSManager::TakeTurn(false, false);
    }
    else if(_turntype == SECOND_TURN){
        setup_player2_mactchdata();
        Director::getInstance()->replaceScene(GameScene::createScene());
    }
}

void WagonSelect::setup_player1_mactchdata(){
    //todo:
    g_gameConfig.match_string;
}

void WagonSelect::setup_player2_mactchdata(){
    
}

void WagonSelect::boy_selected_callback(Ref* ref)
{
    isBoy = true;
}

void WagonSelect::girl_selected_callback(Ref* ref)
{
    isBoy = false;
}
