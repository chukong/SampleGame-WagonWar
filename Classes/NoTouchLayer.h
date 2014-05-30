//
//  NoTouchLayer.h
//  TankMultiplayer
//
//  Created by Jacky on 14-5-30.
//
//

#ifndef __TankMultiplayer__NoTouchLayer__
#define __TankMultiplayer__NoTouchLayer__

#include "cocos2d.h"

class NoTouchLayer : public cocos2d::LayerColor
{
    
public:
    
    CREATE_FUNC(NoTouchLayer);
    
    virtual bool init();
    
    void setError(std::string err);
    
    cocos2d::Label* label;
    
    void remove(float dt);
    
};

#endif /* defined(__TankMultiplayer__NoTouchLayer__) */
