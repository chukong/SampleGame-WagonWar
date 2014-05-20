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
#include "StateManager.h"

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
    
    auto m_pUILayer = Layer::create();
    m_pUILayer->scheduleUpdate();
    addChild(m_pUILayer);
    
    auto myLayout = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("MainScreen.ExportJson"));
    m_pUILayer->addChild(myLayout);
    
    auto startGame = static_cast<Button*>(Helper::seekWidgetByName(myLayout, "start_game"));
    startGame->addTouchEventListener(this,toucheventselector(MainScreenScene::startgame_callback));
    
    auto showAchievement = static_cast<Button*>(Helper::seekWidgetByName(myLayout, "show_achievement"));
    showAchievement->addTouchEventListener(this, toucheventselector(MainScreenScene::showachievement_callback));
    
    auto showleaderboard = static_cast<Button*>(Helper::seekWidgetByName(myLayout, "show_leaderboard"));
    showleaderboard->addTouchEventListener(this, toucheventselector(MainScreenScene::showleaderboard_callback));
    
    return true;
}

void MainScreenScene::startgame_callback(Ref *pSender, TouchEventType type)
{
	switch (type)
	{
        case TouchEventType::TOUCH_EVENT_ENDED:
		{
            //Director::getInstance()->replaceScene(GameScene::createScene());
            if (StateManager::IsSignedIn()) {
                Director::getInstance()->replaceScene(GameScene::createScene());
            }
            else
            {
                StateManager::BeginUserInitiatedSignIn();
            }
		}
            break;
        default:
            break;
	}
}

void MainScreenScene::showleaderboard_callback(cocos2d::Ref *pSender, cocos2d::ui::TouchEventType type)
{
    switch (type)
	{
        case TouchEventType::TOUCH_EVENT_ENDED:
		{
            //Director::getInstance()->replaceScene(GameScene::createScene());
            if (StateManager::IsSignedIn()) {
                StateManager::ShowLeaderboard(LEAD_LEADERBOARD);
            }
            else
            {
                StateManager::BeginUserInitiatedSignIn();
            }
		}
            break;
        default:
            break;
	}
}

void MainScreenScene::showachievement_callback(cocos2d::Ref *pSender, cocos2d::ui::TouchEventType type)
{
    switch (type)
	{
        case TouchEventType::TOUCH_EVENT_ENDED:
		{
            //Director::getInstance()->replaceScene(GameScene::createScene());
            if (StateManager::IsSignedIn()) {
                StateManager::ShowAchievements();
            }
            else
            {
                StateManager::BeginUserInitiatedSignIn();
            }
		}
            break;
        default:
            break;
	}
}
