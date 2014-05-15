//
//  Player.h
//  TankMultiplayer
//
//  Created by Jacky on 14-5-14.
//
//

#ifndef __TankMultiplayer__Player__
#define __TankMultiplayer__Player__

#include "cocos2d.h"

class Player : public cocos2d::Node
{
public:
    Player();
    ~Player();
    
    Player* create();
    
    void bindDude(const char* dudeFileName);
    
    void bindCar(const char* car1FileName, const char* car2FileName);
    
private:
    
    
};

#endif /* defined(__TankMultiplayer__Player__) */
