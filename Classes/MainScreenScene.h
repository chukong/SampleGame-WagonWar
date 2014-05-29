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


class MainScreenScene: public cocos2d::Layer
{
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(MainScreenScene);
    
    void quickmatch_callback(cocos2d::Ref* pSender);
    void invitefriend_callback(cocos2d::Ref* pSender);
    void mygames_callback(cocos2d::Ref* pSender);
    void achievements_callback(cocos2d::Ref* pSender);
    void leaderboard_callback(cocos2d::Ref* pSender);
    
    void enableUI(bool isEnable);
    
    void updateStatus(float dt);
    
    void entergame();

    void starxxxxxgame(float dt);
    
private:
    
    cocos2d::MenuItemImage* quickmatch_menuitem;
    cocos2d::MenuItemImage* invitefriend_menuitem;
    cocos2d::MenuItemImage* mygames_menuitem;
    cocos2d::MenuItemImage* achievements_menuitem;
    cocos2d::MenuItemImage* leaderboard_menuitem;
    
    cocos2d::Label* sign_status;
};

#endif /* defined(__TankMultiplayer__MainScreenScene__) */
