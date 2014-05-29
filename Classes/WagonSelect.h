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
#include "Configuration.h"

class WagonSelect : public cocos2d::Layer
{
public:
    
    static WagonSelect* create(TurnType turntype);
  
    static cocos2d::Scene* createScene(TurnType turntype);
    
    virtual bool init(TurnType turntype);
    
    void createUI();
        
    void ready_callback(cocos2d::Ref* ref);
    
private:
    
    TurnType _turntype;
};

#endif /* defined(__TankMultiplayer__WagonSelect__) */
