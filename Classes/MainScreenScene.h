//
//  MainScreenScene.h
//  TankMultiplayer
//
//  Created by Jacky on 14-5-16.
//
//

#ifndef __TankMultiplayer__MainScreenScene__
#define __TankMultiplayer__MainScreenScene__

#include "cocos2d.h"

const float transSceneDelayTime = 1.0f;

class MainScreenScene: public cocos2d::Layer
{
public:
    
    static cocos2d::Scene* createScene(bool _isShowMatchInBoxUI);
    
    virtual bool init(bool _isShowMatchInBoxUI);
    
    static MainScreenScene* create(bool _isShowMatchInBoxUI);
    
    void quickmatch_callback(cocos2d::Ref* pSender);
    void invitefriend_callback(cocos2d::Ref* pSender);
    void mygames_callback(cocos2d::Ref* pSender);
    void achievements_callback(cocos2d::Ref* pSender);
    void leaderboard_callback(cocos2d::Ref* pSender);
    
    void enableUI(bool isEnable);
    
    void updateStatus(float dt);
    
    void enterWagonSelect_1();
    void enterWagonSelectWithDelay_1(float dt);
    
    void enterWagonSelect_2();
    void enterWagonSelectWithDelay_2(float dt);
    
    void enterGame();
    void enterGameWithDelay(float dt);
    
    void showConnectingPopWindow();
    void showConnectingPopWindowWithDelay(float dt);
    
    void showInviteFailedPopWindow();
    void showInviteFailedPopWindowWithDelay(float dt);
    
    void showInviteSuccessPopWindow();
    void showInviteSuccessPopWindowWithDelay(float dt);
    
    void showNotYourTurnPopWindow();
    void showNotYourTurnPopWindowWithDelay(float dt);

    void showItsCompletedPopWindow();
    void showItsCompletedPopWindowWithDelay(float dt);
    
    void show_mygames(float dt);
    
    void loadingRes();
    
private:
    
    cocos2d::MenuItemImage* quickmatch_menuitem;
    cocos2d::MenuItemImage* invitefriend_menuitem;
    cocos2d::MenuItemImage* mygames_menuitem;
    cocos2d::MenuItemImage* achievements_menuitem;
    cocos2d::MenuItemImage* leaderboard_menuitem;
    
    cocos2d::Label* sign_status;
};

#endif /* defined(__TankMultiplayer__MainScreenScene__) */
