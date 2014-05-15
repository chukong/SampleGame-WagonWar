//
//  Player.cpp
//  TankMultiplayer
//
//  Created by Jacky on 14-5-14.
//
//

#include "Player.h"

Player::Player()
{
    
}

Player::~Player()
{
    
}

Player* Player::create()
{
    Player* pRet = new Player;
    if (pRet != nullptr)
    {
        pRet->autorelease();
        return pRet;
    }
    else
        return nullptr;
}