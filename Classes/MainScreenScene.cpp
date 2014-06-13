//
//  MainScreenScene.cpp
//  TankMultiplayer
//
//  Created by Jacky on 14-5-16.
//
//

#include "MainScreenScene.h"
#include "GameScene.h"
#include "GPGSManager.h"
#include "VisibleRect.h"
#include "Configuration.h"
#include "GameScene.h"
#include "Configuration.h"
#include "WagonSelect.h"
#include "NoTouchLayer.h"
#include "SimpleAudioEngine.h"
#include "PopWindow.h"

#define LEAD_VICTORIES "CgkIt8qQwKsFEAIQCQ"


USING_NS_CC;

Scene* MainScreenScene::createScene(bool _isShowMatchInBoxUI)
{
    auto scene = Scene::create();
    
    auto layer = MainScreenScene::create(_isShowMatchInBoxUI);
    
    scene->addChild(layer);
    
    return scene;
}

MainScreenScene* MainScreenScene::create(bool _isShowMatchInBoxUI)
{
    MainScreenScene *pRet = new MainScreenScene();
    if (pRet && pRet->init(_isShowMatchInBoxUI))
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

bool MainScreenScene::init(bool _isShowMatchInBoxUI)
{
    if ( !Layer::init() )
    {
        return false;
    }
    loadingRes();
    
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.3);
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Stepping Stones m.mp3");

    auto mainscreen_bk = Sprite::create("mainscreen_bk.png");
    if(g_visibleRect.visibleWidth>960)
    {
        mainscreen_bk->setScale(g_visibleRect.visibleWidth/960);
    }
    mainscreen_bk->setAnchorPoint(Point::ANCHOR_MIDDLE);
    mainscreen_bk->setPosition(g_visibleRect.center);
    this->addChild(mainscreen_bk);
    
    auto mainscreen_logo = Sprite::create("mainscreen_logo.png");
    mainscreen_logo->setAnchorPoint(Point::ANCHOR_MIDDLE);
    mainscreen_logo->setPosition(Point(g_visibleRect.center.x-150,g_visibleRect.center.y+130));
    mainscreen_logo->setRotation(-10);
    this->addChild(mainscreen_logo,2);
    
    mainscreen_logo->runAction(RepeatForever::create(Sequence::create(MoveBy::create(1.0f, Point(0,-20)),MoveBy::create(1.0f, Point(0,20)),nullptr)));
    
    quickmatch_menuitem = MenuItemImage::create("btn_quickmatch_0.png",
                                                     "btn_quickmatch_1.png",
                                                     CC_CALLBACK_1(MainScreenScene::quickmatch_callback, this));
    quickmatch_menuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    quickmatch_menuitem->setPosition(Point(g_visibleRect.visibleWidth-350,g_visibleRect.visibleHeight-95));
    
    invitefriend_menuitem = MenuItemImage::create("btn_invitefriend_0.png",
                                                     "btn_invitefriend_1.png",
                                                     CC_CALLBACK_1(MainScreenScene::invitefriend_callback, this));
    invitefriend_menuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    invitefriend_menuitem->setPosition(Point(g_visibleRect.visibleWidth-350,g_visibleRect.visibleHeight-245));
    
    mygames_menuitem = MenuItemImage::create("btn_mygames_0.png",
                                                     "btn_mygames_1.png",
                                                     CC_CALLBACK_1(MainScreenScene::mygames_callback, this));
    mygames_menuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    mygames_menuitem->setPosition(Point(g_visibleRect.visibleWidth-350,g_visibleRect.visibleHeight-395));
    
    achievements_menuitem = MenuItemImage::create("btn_achievements_0.png",
                                                     "btn_achievements_1.png",
                                                     CC_CALLBACK_1(MainScreenScene::achievements_callback, this));
    achievements_menuitem->setScale(0.8f);
    achievements_menuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    achievements_menuitem->setPosition(Point(g_visibleRect.visibleWidth-420,g_visibleRect.visibleHeight-520));
    
    leaderboard_menuitem = MenuItemImage::create("btn_leaderboard_0.png",
                                                     "btn_leaderboard_1.png",
                                                     CC_CALLBACK_1(MainScreenScene::leaderboard_callback, this));
    
    leaderboard_menuitem->setScale(0.8f);
    leaderboard_menuitem->setAnchorPoint(Point::ANCHOR_MIDDLE);
    leaderboard_menuitem->setPosition(Point(g_visibleRect.visibleWidth-280,g_visibleRect.visibleHeight-520));
    
    auto menu = Menu::create(quickmatch_menuitem, invitefriend_menuitem, mygames_menuitem, achievements_menuitem, leaderboard_menuitem, nullptr);
    menu->setPosition(Point(150,-30));
    this->addChild(menu,1);
    
    sign_status = Label::create("Status:Unknown.", GameConfig::defaultFontName, 20);
    sign_status->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    sign_status->setPosition(Point(20,20));
    this->addChild(sign_status,1);
    this->schedule(schedule_selector(MainScreenScene::updateStatus), 0.1f, kRepeatForever, 2.0f);
    
    auto listener1 = EventListenerCustom::create("enterWagonSelect_1", CC_CALLBACK_0(MainScreenScene::enterWagonSelect_1, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

    auto listener2 = EventListenerCustom::create("enterWagonSelect_2", CC_CALLBACK_0(MainScreenScene::enterWagonSelect_2, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
    
    auto listener3 = EventListenerCustom::create("enterGame", CC_CALLBACK_0(MainScreenScene::enterGame, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener3, this);
    
    auto listener4 = EventListenerCustom::create("showInviteFailedPopWindow", CC_CALLBACK_0(MainScreenScene::showInviteFailedPopWindow, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener4, this);
    
    auto listener5 = EventListenerCustom::create("showNotYourTurnPopWindow", CC_CALLBACK_0(MainScreenScene::showNotYourTurnPopWindow, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener5, this);
    
    auto listener6 = EventListenerCustom::create("showConnectingPopWindow", CC_CALLBACK_0(MainScreenScene::showConnectingPopWindow, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener6, this);
    
    auto listener7 = EventListenerCustom::create("showInviteSuccessPopWindow", CC_CALLBACK_0(MainScreenScene::showInviteSuccessPopWindow, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener7, this);
    
    auto listener8 = EventListenerCustom::create("showItsCompletedPopWindow", CC_CALLBACK_0(MainScreenScene::showItsCompletedPopWindow, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener8, this);
    
    log("_isShowMatchInBoxUI == %d",_isShowMatchInBoxUI);
    if(_isShowMatchInBoxUI)
        this->scheduleOnce(schedule_selector(MainScreenScene::show_mygames), 0.5f);
    
    return true;
}

void MainScreenScene::loadingRes()
{
    Director::getInstance()->getTextureCache()->addImage("pop_btn.png");
    Director::getInstance()->getTextureCache()->addImage("pop_bk.png");
    Director::getInstance()->getTextureCache()->addImage("android.png");

}

void MainScreenScene::quickmatch_callback(cocos2d::Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("menuclick.mp3");
    log("QuickMatch");
    if (GPGSManager::IsSignedIn()) {
        GPGSManager::QuickMatch();
    }
    else{
        GPGSManager::BeginUserInitiatedSignIn();
    }
}
void MainScreenScene::invitefriend_callback(cocos2d::Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("menuclick.mp3");
    log("InviteFriend");
    if (GPGSManager::IsSignedIn()) {
        GPGSManager::InviteFriend();
    }
    else{
        GPGSManager::BeginUserInitiatedSignIn();
    }
}
void MainScreenScene::mygames_callback(cocos2d::Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("menuclick.mp3");
    log("MyGames");
    if (GPGSManager::IsSignedIn()) {
        GPGSManager::ShowMatchInbox();
    }
    else{
        GPGSManager::BeginUserInitiatedSignIn();
    }
}
void MainScreenScene::achievements_callback(cocos2d::Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("menuclick.mp3");
    log("Achievements");
    if (GPGSManager::IsSignedIn()) {
        GPGSManager::ShowAchievements();
    }
    else{
        GPGSManager::BeginUserInitiatedSignIn();
    }
}
void MainScreenScene::leaderboard_callback(cocos2d::Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("menuclick.mp3");
    log("LeaderBoard");
    if (GPGSManager::IsSignedIn()) {
        GPGSManager::ShowLeaderboard(LEAD_VICTORIES);
    }
    else{
        GPGSManager::BeginUserInitiatedSignIn();
    }
}

void MainScreenScene::enableUI(bool isEnable)
{
    quickmatch_menuitem->setEnabled(isEnable);
    invitefriend_menuitem->setEnabled(isEnable);
    mygames_menuitem->setEnabled(isEnable);
    achievements_menuitem->setEnabled(isEnable);
    leaderboard_menuitem->setEnabled(isEnable);
}

void MainScreenScene::updateStatus(float dt)
{
    if (GPGSManager::IsSignedIn()) {
        sign_status->setString("Status:Sign in.");
    }
    else
    {
        sign_status->setString("Status:Sign out.");
    }
}

void MainScreenScene::enterWagonSelect_1()
{
    
    scheduleOnce(schedule_selector(MainScreenScene::enterWagonSelectWithDelay_1), transSceneDelayTime);
}

void MainScreenScene::enterWagonSelectWithDelay_1(float dt)
{
    log("recv data is ===>%s", g_gameConfig.match_string.c_str());
    Director::getInstance()->getRunningScene()->removeChildByTag(NOTOUCHTAG);
    auto scene = WagonSelect::createScene(FIRST_TURN);
    cocos2d::Director::getInstance()->replaceScene(scene);
}

void MainScreenScene::enterWagonSelect_2()
{
    scheduleOnce(schedule_selector(MainScreenScene::enterWagonSelectWithDelay_2), transSceneDelayTime);
}

void MainScreenScene::enterWagonSelectWithDelay_2(float dt)
{
    log("recv data is ===>%s", g_gameConfig.match_string.c_str());
    Director::getInstance()->getRunningScene()->removeChildByTag(NOTOUCHTAG);
    auto scene = WagonSelect::createScene(SECOND_TURN);
    cocos2d::Director::getInstance()->replaceScene(scene);
}

void MainScreenScene::enterGame()
{
    log("I had enterd the game....");
    scheduleOnce(schedule_selector(MainScreenScene::enterGameWithDelay), transSceneDelayTime);
}
void MainScreenScene::enterGameWithDelay(float dt)
{
    log("recv data is ===>%s", g_gameConfig.match_string.c_str());
    //this->removeChildByTag(NOTOUCHTAG);
    auto scene = GameScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(scene);
}

void MainScreenScene::show_mygames(float dt)
{
    log("111111111111");
    if (GPGSManager::IsSignedIn()) {
        log("222222222222222");
        GPGSManager::ShowMatchInbox();
    }
    else{
        GPGSManager::BeginUserInitiatedSignIn();
    }
}

void MainScreenScene::showConnectingPopWindow()
{
    scheduleOnce(schedule_selector(MainScreenScene::showConnectingPopWindowWithDelay), transSceneDelayTime/2);
}
void MainScreenScene::showConnectingPopWindowWithDelay(float dt)
{
    auto popwindow = PopWindow::create();
    addChild(popwindow,100);
}

//change popwindow status
void MainScreenScene::showInviteFailedPopWindow()
{
    scheduleOnce(schedule_selector(MainScreenScene::showInviteFailedPopWindowWithDelay), transSceneDelayTime/2);
}

void MainScreenScene::showInviteFailedPopWindowWithDelay(float dt)
{
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("InviteFailed");
}

void MainScreenScene::showInviteSuccessPopWindow()
{
    scheduleOnce(schedule_selector(MainScreenScene::showInviteSuccessPopWindowWithDelay), transSceneDelayTime/2);
}
void MainScreenScene::showInviteSuccessPopWindowWithDelay(float dt)
{
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("InviteSuccess");
}

void MainScreenScene::showNotYourTurnPopWindow()
{
    scheduleOnce(schedule_selector(MainScreenScene::showNotYourTurnPopWindowWithDelay), transSceneDelayTime/2);
}

void MainScreenScene::showNotYourTurnPopWindowWithDelay(float dt)
{
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("ItsNotYourTurn");
}

void MainScreenScene::showItsCompletedPopWindow()
{
    scheduleOnce(schedule_selector(MainScreenScene::showItsCompletedPopWindowWithDelay), transSceneDelayTime/2);
}
void MainScreenScene::showItsCompletedPopWindowWithDelay(float dt)
{
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("ItsCompleted");
}
