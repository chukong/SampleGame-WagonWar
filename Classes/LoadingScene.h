//
//  LoadingScene.h
//  TankMultiplayer
//
//  Created by Jacky on 14-5-16.
//
//

#ifndef __TankMultiplayer__LoadingScene__
#define __TankMultiplayer__LoadingScene__

#include "cocos2d.h"

USING_NS_CC;

class LoadingScene : public Layer
{
public:
    
    static Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(LoadingScene);
    
    void loadRes();
};

#endif /* defined(__TankMultiplayer__LoadingScene__) */
