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

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

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
    
    auto myLayout = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("mainscreen.Exportjson"));
    m_pUILayer->addChild(myLayout);
    
    auto startGame = static_cast<Button*>(Helper::seekWidgetByName(myLayout, "start_game"));
    startGame->addTouchEventListener(this,toucheventselector(MainScreenScene::start_callback));
    
    return true;
}

void MainScreenScene::start_callback(Ref *pSender, TouchEventType type)
{
	switch (type)
	{
        case TouchEventType::TOUCH_EVENT_ENDED:
		{
            Director::getInstance()->replaceScene(GameScene::createScene());
		}
            break;
        default:
            break;
	}
}