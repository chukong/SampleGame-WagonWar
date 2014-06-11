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
    
    void returntoMenu();
    void entertoMenu(float dt);
    
    void showConnectingPopWindow();

    void showConnectingPopWindowWithDelay(float dt);

private:
    
    WagonSelect():_isBoy(true), _wagon(0){};
    
    TurnType _turntype;
    
    cocos2d::MenuItemImage* _boy_memuitem;
    cocos2d::MenuItemImage* _girl_memuitem;
    
    cocos2d::MenuItemImage* _wagon1_menuitem;
    cocos2d::MenuItemImage* _wagon2_menuitem;
    cocos2d::MenuItemImage* _wagon3_menuitem;
    cocos2d::MenuItemImage* _wagon4_menuitem;
    
    cocos2d::Sprite* _dude1;
    cocos2d::Sprite* _dude2;
    cocos2d::Sprite* _dude3;
    cocos2d::Sprite* _dude4;
    
    cocos2d::Sprite* _wagon_bk;
    
    cocos2d::Sprite* flash;
    
    int _selectWagonIndex = 0;
    
    bool _isBoy;
    int _wagon;
    
    void initWagonParams();
    
};

#endif /* defined(__TankMultiplayer__WagonSelect__) */
