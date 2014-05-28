//
//  TestTBMP.h
//  TankMultiplayer
//
//  Created by Jacky on 14-5-26.
//
//

#ifndef __TankMultiplayer__TestTBMP__
#define __TankMultiplayer__TestTBMP__

#include "cocos2d.h"

class TestTBMP : public cocos2d::Layer
{
public:
    
    static cocos2d::Scene* createScene(std::string gamedata);
    
    virtual bool init();
    
    CREATE_FUNC(TestTBMP);

private:
    
    static std::string m_GameData;
};

#endif /* defined(__TankMultiplayer__TestTBMP__) */
