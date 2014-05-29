//
//  WagonSelect.h
//  TankMultiplayer
//
//  Created by Jacky on 14-5-29.
//
//

#ifndef __TankMultiplayer__WagonSelect__
#define __TankMultiplayer__WagonSelect__

#include "cocos2d.h"

class WagonSelect : public cocos2d::Layer
{
public:
    
    CREATE_FUNC(WagonSelect);
  
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void createUI();
};

#endif /* defined(__TankMultiplayer__WagonSelect__) */
