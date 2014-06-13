//
//  PopWindow.h
//  TankMultiplayer
//
//  Created by Jacky on 14-6-9.
//
//

#ifndef __TankMultiplayer__PopWindow__
#define __TankMultiplayer__PopWindow__

#include "cocos2d.h"

enum class GOTO
{
    NONE,
    CLOSE,
    MAINMENU,
};

class PopWindow : public cocos2d::LayerColor
{
public:
    
    static PopWindow* create();

    bool init();
    
    virtual void onEnter();
    
    virtual void onExit();

    CC_SYNTHESIZE(std::string, _contenttext, ContentText);
    
    cocos2d::Label* _contentLabel;
    
    CC_SYNTHESIZE(std::string, _btntext, BtnText);
    
    cocos2d::Sprite* _bk;
    
    cocos2d::Sprite* _loadingSprite;
    
    cocos2d::Menu* _btn_menu;
        
    cocos2d::Label* _btn_label;
    
    void menuCallback(Ref* ref);
    
    static GOTO _goto;
    
private:

    void Q1Success();
    void Q2Success();
    void QFailed();
    void InviteSuccess();
    void InviteFailed();
    void ReadySuccess();
    void TakeTurnSuccess();
    void ItsNotYourTurn();
    void ItsCompleted();

};

#endif /* defined(__TankMultiplayer__PopWindow__) */
