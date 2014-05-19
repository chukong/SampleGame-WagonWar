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
#include "cocostudio/CocoStudio.h"

class MainScreenScene: public cocos2d::Layer
{
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(MainScreenScene);
    
    void start_callback(cocos2d::Ref *pSender, cocos2d::ui::TouchEventType type);
};

#endif /* defined(__TankMultiplayer__MainScreenScene__) */
