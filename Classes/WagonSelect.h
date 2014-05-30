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
    
    void boy_selected_callback(cocos2d::Ref* ref);
    
    void girl_selected_callback(cocos2d::Ref* ref);
    
    void wagon1_selected_callback(cocos2d::Ref* ref);
    
    void wagon2_selected_callback(cocos2d::Ref* ref);
    
    void wagon3_selected_callback(cocos2d::Ref* ref);
    
    void wagon4_selected_callback(cocos2d::Ref* ref);
        
    void ready_callback(cocos2d::Ref* ref);
    
    void setup_player1_mactchdata();
    
    void setup_player2_mactchdata();

private:
    
    WagonSelect():_isBoy(true), _wagon(0){};
    
    TurnType _turntype;
    
    bool _isBoy;
    int _wagon;
};

#endif /* defined(__TankMultiplayer__WagonSelect__) */
