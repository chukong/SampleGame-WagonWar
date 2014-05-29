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
#include "Configuration.h"

USING_NS_CC;

#define ACH_ACHIEVEMENT_01 "CgkIs7qm9rYbEAIQAQ"
#define ACH_ACHIEVEMENT_02 "CgkIs7qm9rYbEAIQAg"
#define LEAD_LEADERBOARD "CgkIs7qm9rYbEAIQAw"

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
    
    g_gameConfig.match_data.push_back(100);
    
    auto taketurn_label = Label::createWithSystemFont("TakeTurn", "Arial", 20);
    auto taketurn_menuitem = MenuItemLabel::create(taketurn_label,
                                                       [&](Ref* ref){
                                                           if (GPGSManager::IsSignedIn()) {
                                                               log("Take Turn");
                                                               GPGSManager::TakeTurn(false, false);
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
    
    //Show LeaderBoards
    auto leaderboard_label = Label::createWithSystemFont("LeaderBoard", "Arial", 20);
    auto leaderboard_menuitem = MenuItemLabel::create(leaderboard_label,
                                                      [](Ref* ref){
                                                          if (GPGSManager::IsSignedIn()) {
                                                              log("Show LeaderBoard");
                                                              GPGSManager::ShowLeaderboard(LEAD_LEADERBOARD);
                                                          }
                                                          else{
                                                              log("Failed to show LeaderBoard, Not Signed in");
                                                          }
                                                      });
    leaderboard_menuitem->setPosition(Point(visibleSize.width/2-100, visibleSize.height/2-100));
    
    //Show Achievements
    auto achievement_label = Label::createWithSystemFont("Achievement", "Arial", 20);
    auto achievement_menuitem = MenuItemLabel::create(achievement_label,
                                                      [](Ref* ref){
                                                          if (GPGSManager::IsSignedIn()) {
                                                              log("Show Achievements");
                                                              GPGSManager::ShowAchievements();
                                                          }
                                                          else{
                                                              log("Failed to show Achievements, Not Signed in");
                                                          }
                                                      });
    achievement_menuitem->setPosition(Point(visibleSize.width/2+100, visibleSize.height/2-100));
    
    auto menu = Menu::create(taketurn_menuitem, return_menuitem, leaderboard_menuitem, achievement_menuitem, nullptr);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    log("TestTBMP completed.");
    
    return true;
}