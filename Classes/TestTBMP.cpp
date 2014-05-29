//
//  TestTBMP.cpp
//  TankMultiplayer
//
//  Created by Jacky on 14-5-26.
//
//

#include "TestTBMP.h"
#include "GPGSManager.h"
#include "MainScreenScene.h"

USING_NS_CC;

std::string TestTBMP::m_GameData = "";

Scene* TestTBMP::createScene(std::string gamedata)
{
    m_GameData = gamedata;
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TestTBMP::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool TestTBMP::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

    
    log("the recv data is %s",m_GameData.c_str());
    
    auto bkgnd = Sprite::create("HelloWorld.png");
    bkgnd->setAnchorPoint(Point::ANCHOR_MIDDLE);
    bkgnd->setPosition(visibleSize.width/2, visibleSize.height/2);
    addChild(bkgnd);
    
    std::vector<uint8_t> match_data;
    match_data.push_back(100);
    
    auto taketurn_label = Label::createWithSystemFont("TakeTurn", "Arial", 20);
    auto taketurn_menuitem = MenuItemLabel::create(taketurn_label,
                                                       [&](Ref* ref){
                                                           if (GPGSManager::IsSignedIn()) {
                                                               log("Take Turn");
                                                               GPGSManager::TakeTurn(false, false, match_data);
                                                           }
                                                       });
    taketurn_menuitem->setPosition(Point(visibleSize.width/2-100, visibleSize.height/2));
    
    auto return_label = Label::createWithSystemFont("ShowMatchInBox", "Arial", 20);
    auto return_menuitem = MenuItemLabel::create(return_label,
                                                         [](Ref* ref){
                                                             if (GPGSManager::IsSignedIn()) {
                                                                 log("Return");
                                                                 cocos2d::Director::getInstance()->replaceScene(MainScreenScene::createScene());
                                                             }
                                                         });
    return_menuitem->setPosition(Point(visibleSize.width/2+100, visibleSize.height/2));
    
    auto menu = Menu::create(taketurn_menuitem, return_menuitem, nullptr);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    log("TestTBMP completed.");
    
    return true;
}