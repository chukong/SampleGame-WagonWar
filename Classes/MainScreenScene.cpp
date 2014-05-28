//
//  MainScreenScene.cpp
//  TankMultiplayer
//
//  Created by Jacky on 14-5-16.
//
//

#include "MainScreenScene.h"
#include "ui/CocosGUI.h"
#include "GameScene.h"
#include "GPGSManager.h"
#include <random>
#include "TestTBMP.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

#define ACH_ACHIEVEMENT_01 "CgkIs7qm9rYbEAIQAQ"
#define ACH_ACHIEVEMENT_02 "CgkIs7qm9rYbEAIQAg"
#define LEAD_LEADERBOARD "CgkIs7qm9rYbEAIQAw"

Scene* MainScreenScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = MainScreenScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool MainScreenScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
//    auto m_pUILayer = Layer::create();
//    m_pUILayer->scheduleUpdate();
//    addChild(m_pUILayer);
//    
//    auto myLayout = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("MainScreen.ExportJson"));
//    m_pUILayer->addChild(myLayout);
//    
//    auto startGame = static_cast<Button*>(ui::Helper::seekWidgetByName(myLayout, "start_game"));
//    startGame->addTouchEventListener(this,toucheventselector(MainScreenScene::startgame_callback));
//    
//    auto showAchievement = static_cast<Button*>(ui::Helper::seekWidgetByName(myLayout, "show_achievement"));
//    showAchievement->addTouchEventListener(this, toucheventselector(MainScreenScene::showachievement_callback));
//    
//    auto showleaderboard = static_cast<Button*>(ui::Helper::seekWidgetByName(myLayout, "show_leaderboard"));
//    showleaderboard->addTouchEventListener(this, toucheventselector(MainScreenScene::showleaderboard_callback));
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //SignIn and Sign Out.
    std::string strSign = "Sign In";
    if (GPGSManager::IsSignedIn()) {
        strSign = "Sign Out";
    }
    auto signin_label = Label::createWithSystemFont(strSign, "Arial", 20);
    auto signin_menuitem = MenuItemLabel::create(signin_label,
                                                 [signin_label](Ref* ref){
                                                     if (GPGSManager::IsSignedIn()) {
                                                         log("The Status of now is SignedIn");
                                                         signin_label->setString("Sign Out");
                                                         GPGSManager::SignOut();
                                                     } else {
                                                         log("The Status of now is not SignedIn");
                                                         GPGSManager::BeginUserInitiatedSignIn();
                                                     }
                                                     
                                                 });
    signin_menuitem->setPosition(Point(visibleSize.width/2-100, visibleSize.height/2+50));
    
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
    leaderboard_menuitem->setPosition(Point(visibleSize.width/2+100, visibleSize.height/2+50));
    
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
    achievement_menuitem->setPosition(Point(visibleSize.width/2-100, visibleSize.height/2));
    
    //Submit Score
    std::uniform_int_distribution<unsigned> u(0,10000);
    std::default_random_engine e;
    e.seed((unsigned)time(NULL));
    auto rand_score =  u(e);
    auto submitscore_label = Label::createWithSystemFont("SubmitScore", "Arial", 20);
    auto submitscore_menuitem = MenuItemLabel::create(submitscore_label,
                                                      [rand_score](Ref* ref){
                                                          if (GPGSManager::IsSignedIn()) {
                                                              log("Submit Score %d", rand_score);
                                                              GPGSManager::SubmitHighScore(LEAD_LEADERBOARD, rand_score);
                                                          }
                                                          else{
                                                              log("Failed to submit score, Not Signed in");
                                                          }
                                                      });
    submitscore_menuitem->setPosition(Point(visibleSize.width/2+100, visibleSize.height/2));
    
    //Unlock Achievement
    auto unlockachievement_label = Label::createWithSystemFont("UnlockAchievement", "Arial", 20);
    auto unlockachievement_menuitem = MenuItemLabel::create(unlockachievement_label,
                                                            [](Ref* ref){
                                                                if (GPGSManager::IsSignedIn()) {
                                                                    log("Unlock Achievement %s", ACH_ACHIEVEMENT_01);
                                                                    GPGSManager::UnlockAchievement(ACH_ACHIEVEMENT_01);
                                                                }
                                                                else{
                                                                    log("Failed to unlock Achievements, Not Signed in");
                                                                }
                                                            });
    unlockachievement_menuitem->setPosition(Point(visibleSize.width/2-100, visibleSize.height/2-50));
    
    //Increase Achievement
    auto increaseachievement_label = Label::createWithSystemFont("IncreaseAchievement", "Arial", 20);
    auto increaseachievement_menuitem = MenuItemLabel::create(increaseachievement_label,
                                                              [](Ref* ref){
                                                                  if (GPGSManager::IsSignedIn()) {
                                                                      log("Increase Achievement %s", ACH_ACHIEVEMENT_02);
                                                                   GPGSManager::IncrementAchievement(ACH_ACHIEVEMENT_02);
                                                                  }
                                                              });
    increaseachievement_menuitem->setPosition(Point(visibleSize.width/2+100, visibleSize.height/2-50));
    
    auto quickmatch_label = Label::createWithSystemFont("QuickMatch", "Arial", 20);
    auto quickmatch_menuitem = MenuItemLabel::create(quickmatch_label,
                                                              [](Ref* ref){
                                                                  if (GPGSManager::IsSignedIn()) {
                                                                      log("Quick Match");
                                                                      GPGSManager::QuickMatch();
                                                                  }
                                                              });
    quickmatch_menuitem->setPosition(Point(visibleSize.width/2-100, visibleSize.height/2-100));

    
    auto invitefriend_label = Label::createWithSystemFont("InviteFriend", "Arial", 20);
    auto invitefriend_menuitem = MenuItemLabel::create(invitefriend_label,
                                                     [](Ref* ref){
                                                         if (GPGSManager::IsSignedIn()) {
                                                             log("InviteFriend");
                                                             GPGSManager::InviteFriend();
                                                         }
                                                     });
    invitefriend_menuitem->setPosition(Point(visibleSize.width/2+100, visibleSize.height/2-100));
    
    auto showmatchinbox_label = Label::createWithSystemFont("ShowMatchInBox", "Arial", 20);
    auto showmatchinbox_menuitem = MenuItemLabel::create(showmatchinbox_label,
                                                       [](Ref* ref){
                                                           if (GPGSManager::IsSignedIn()) {
                                                               log("ShowMatchInBox");
                                                               GPGSManager::ShowMatchInbox();
                                                           }
                                                       });
    showmatchinbox_menuitem->setPosition(Point(visibleSize.width/2-100, visibleSize.height/2-150));
    
    auto menu = Menu::create(signin_menuitem, leaderboard_menuitem, achievement_menuitem,
                             submitscore_menuitem, unlockachievement_menuitem, increaseachievement_menuitem,
                             quickmatch_menuitem, invitefriend_menuitem, showmatchinbox_menuitem, nullptr);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    auto listener = EventListenerCustom::create("replacexxx", CC_CALLBACK_0(MainScreenScene::startxxxgame, this, 0));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    
    return true;
}

void MainScreenScene::startxxxgame(int i)
{
    
    scheduleOnce(schedule_selector(MainScreenScene::starxxxxxgame), 3.0);
}

void MainScreenScene::starxxxxxgame(float dt)
{
    log("Replace with TestTMBP Scene...before");
    auto scene = TestTBMP::createScene("123456");
    cocos2d::Director::getInstance()->replaceScene(scene);
    log("Replace with TestTMBP Scene...after");
}

void MainScreenScene::startgame_callback(Ref *pSender, TouchEventType type)
{
	switch (type)
	{
        case TouchEventType::TOUCH_EVENT_ENDED:
		{
            //Director::getInstance()->replaceScene(GameScene::createScene());
            if (GPGSManager::IsSignedIn()) {
                Director::getInstance()->replaceScene(GameScene::createScene());
            }
            else
            {
                GPGSManager::BeginUserInitiatedSignIn();
            }
		}
            break;
        default:
            break;
	}
}

void MainScreenScene::showleaderboard_callback(cocos2d::Ref *pSender, cocos2d::ui::TouchEventType type)
{
//    switch (type)
//	{
//        case TouchEventType::TOUCH_EVENT_ENDED:
//		{
//            //Director::getInstance()->replaceScene(GameScene::createScene());
//            if (GPGSManager::IsSignedIn()) {
//                GPGSManager::ShowLeaderboard(LEAD_LEADERBOARD);
//            }
//            else
//            {
//                GPGSManager::BeginUserInitiatedSignIn();
//            }
//		}
//            break;
//        default:
//            break;
//	}
    
    if(GPGSManager::IsSignedIn())
    {
        GPGSManager::QuickMatch();
    }
    else
    {
        GPGSManager::BeginUserInitiatedSignIn();
    }
}

void MainScreenScene::showachievement_callback(cocos2d::Ref *pSender, cocos2d::ui::TouchEventType type)
{
//    switch (type)
//	{
//        case TouchEventType::TOUCH_EVENT_ENDED:
//		{
//            //Director::getInstance()->replaceScene(GameScene::createScene());
//            if (GPGSManager::IsSignedIn()) {
//                GPGSManager::ShowAchievements();
//            }
//            else
//            {
//                GPGSManager::BeginUserInitiatedSignIn();
//            }
//		}
//            break;
//        default:
//            break;
//	}
    
    if (GPGSManager::IsSignedIn()) {
        GPGSManager::ShowMatchInbox();
    }
    else
    {
        GPGSManager::BeginUserInitiatedSignIn();
    }
}
