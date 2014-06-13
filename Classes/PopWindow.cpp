//
//  PopWindow.cpp
//  TankMultiplayer
//
//  Created by Jacky on 14-6-9.
//
//

#include "PopWindow.h"
#include "VisibleRect.h"
#include "MainScreenScene.h"

USING_NS_CC;

GOTO PopWindow::_goto = GOTO::NONE;

PopWindow* PopWindow::create()
{
    PopWindow *pRet = new PopWindow();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool PopWindow::init()
{
    if ( LayerColor::initWithColor(Color4B(0,0,0,0)) )
    {
        _goto = GOTO::NONE;
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [](Touch* touch, Event* event){
            return true;
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        _bk = Sprite::create("pop_bk.png");
        _bk->setScale(0.1f);
        _bk->setPosition(g_visibleRect.center);
        _bk->setAnchorPoint(Point::ANCHOR_MIDDLE);
        _bk->runAction(EaseBackOut::create(ScaleTo::create(0.1f, 1.0f)));
        this->addChild(_bk);
        
        TTFConfig turnTTFConfig;
        turnTTFConfig.outlineSize = 3;
        turnTTFConfig.fontSize = 30;
        turnTTFConfig.fontFilePath = "fonts/britanic bold.ttf";
        _contentLabel = Label::createWithTTF(turnTTFConfig, "Connecting...", TextHAlignment::CENTER, 400);
        _contentLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
        _contentLabel->setPosition(_bk->getContentSize().width/2+20,_bk->getContentSize().height/2);
        _contentLabel->setSpacing(-5);
        _contentLabel->enableOutline(Color4B::BLACK);
        _bk->addChild(_contentLabel);

        _loadingSprite = Sprite::create("android.png");
        _loadingSprite->setScale(0.7f);
        _loadingSprite->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        _loadingSprite->setPosition(_contentLabel->getPosition()+Point(20,0));
        _loadingSprite->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5f, Point(0,20)),MoveBy::create(0.5f, Point(0,-20)), nullptr)));
        _bk->addChild(_loadingSprite);
        
        auto btn_menuitem = MenuItemImage::create("pop_btn.png","pop_btn.png",CC_CALLBACK_1(PopWindow::menuCallback, this));
        btn_menuitem->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
        btn_menuitem->setPosition(_bk->getContentSize().width/2,100);
        
        _btn_menu = Menu::create(btn_menuitem,nullptr);
        _btn_menu->setPosition(Point::ZERO);
        _btn_menu->setVisible(false);
        _bk->addChild(_btn_menu);
        
        TTFConfig t_turnTTFConfig;
        t_turnTTFConfig.outlineSize = 3;
        t_turnTTFConfig.fontSize = 20;
        t_turnTTFConfig.fontFilePath = "fonts/britanic bold.ttf";
        
        _btn_label = Label::createWithTTF(t_turnTTFConfig, "OK", TextHAlignment::CENTER, 400);
        _btn_label->setAnchorPoint(Point::ANCHOR_MIDDLE);
        _btn_label->setPosition(btn_menuitem->getContentSize().width/2, btn_menuitem->getContentSize().height/2);
        _btn_label->setSpacing(-5);
        _btn_label->enableOutline(Color4B::BLACK);
        btn_menuitem->addChild(_btn_label);
        
    }
    return true;
}

void PopWindow::onEnter()
{
    LayerColor::onEnter();
    
    auto Q1SuccessListener = EventListenerCustom::create("Q1Success", CC_CALLBACK_0(PopWindow::Q1Success, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(Q1SuccessListener, this);
    
    auto Q2SuccessListener = EventListenerCustom::create("Q2Success", CC_CALLBACK_0(PopWindow::Q2Success, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(Q2SuccessListener, this);

    auto QFailedListener = EventListenerCustom::create("QFailed", CC_CALLBACK_0(PopWindow::QFailed, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(QFailedListener, this);
    
    auto InviteSuccessListener = EventListenerCustom::create("InviteSuccess", CC_CALLBACK_0(PopWindow::InviteSuccess, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(InviteSuccessListener, this);
    
    auto InviteFailedListener = EventListenerCustom::create("InviteFailed", CC_CALLBACK_0(PopWindow::InviteFailed, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(InviteFailedListener, this);
    
    auto ReadySuccessListener = EventListenerCustom::create("ReadySuccess", CC_CALLBACK_0(PopWindow::ReadySuccess, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(ReadySuccessListener, this);
    
    auto TakeTurnSuccessListener = EventListenerCustom::create("TakeTurnSuccess", CC_CALLBACK_0(PopWindow::TakeTurnSuccess, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(TakeTurnSuccessListener, this);
    
    auto ItsNotYourTurnListener = EventListenerCustom::create("ItsNotYourTurn", CC_CALLBACK_0(PopWindow::ItsNotYourTurn, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(ItsNotYourTurnListener, this);
    
    auto ItsCompletedListener = EventListenerCustom::create("ItsCompleted", CC_CALLBACK_0(PopWindow::ItsCompleted, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(ItsCompletedListener, this);
}

void PopWindow::onExit()
{
    _eventDispatcher->removeCustomEventListeners("Q1Success");
    _eventDispatcher->removeCustomEventListeners("Q1Success");
    _eventDispatcher->removeCustomEventListeners("QFailed");
    LayerColor::onExit();
}

void PopWindow::Q1Success()
{
    _contentLabel->setString("Success...");
    _contentLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _contentLabel->setPosition(_bk->getContentSize().width/2,_bk->getContentSize().height/2);
    _loadingSprite->removeFromParent();
    _eventDispatcher->dispatchCustomEvent("enterWagonSelect_1");
}

void PopWindow::Q2Success()
{
    _contentLabel->setString("Success...");
    _contentLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _contentLabel->setPosition(_bk->getContentSize().width/2,_bk->getContentSize().height/2);
    _loadingSprite->removeFromParent();
    _eventDispatcher->dispatchCustomEvent("enterWagonSelect_2");
}

void PopWindow::QFailed()
{
    _contentLabel->setString("Created Failed...");
    _contentLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _contentLabel->setPosition(_bk->getContentSize().width/2,_bk->getContentSize().height/2+30);
    _loadingSprite->removeFromParent();
    
    _goto = GOTO::CLOSE;
    _btn_menu->setVisible(true);
}

void PopWindow::InviteSuccess()
{
    _contentLabel->setString("Success...");
    _contentLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _contentLabel->setPosition(_bk->getContentSize().width/2,_bk->getContentSize().height/2);
    _loadingSprite->removeFromParent();
    _eventDispatcher->dispatchCustomEvent("enterWagonSelect_1");
}

void PopWindow::InviteFailed()
{
    _contentLabel->setString("Invite Failed...");
    _contentLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _contentLabel->setPosition(_bk->getContentSize().width/2,_bk->getContentSize().height/2+30);
    _loadingSprite->removeFromParent();
    
    _goto = GOTO::CLOSE;
    _btn_menu->setVisible(true);
}

void PopWindow::ReadySuccess()
{
    _contentLabel->setString("Success...");
    _contentLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _contentLabel->setPosition(_bk->getContentSize().width/2,_bk->getContentSize().height/2+30);
    _loadingSprite->removeFromParent();
    
    _goto = GOTO::MAINMENU;
    _btn_label->setString("Return");
    _btn_menu->setVisible(true);
}

void PopWindow::TakeTurnSuccess()
{
    _eventDispatcher->dispatchCustomEvent("showReturnBtn");
    _contentLabel->setString("Success...");
    _contentLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _contentLabel->setPosition(_bk->getContentSize().width/2,_bk->getContentSize().height/2+30);
    _loadingSprite->removeFromParent();
    
//    _goto = GOTO::CLOSE;
//    _btn_label->setString("OK");
//    _btn_menu->setVisible(true);
    
    runAction(Sequence::create(DelayTime::create(2.0f), RemoveSelf::create(), nullptr));
}

void PopWindow::ItsNotYourTurn()
{
    _contentLabel->setString("It's not your turn...");
    _contentLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _contentLabel->setPosition(_bk->getContentSize().width/2,_bk->getContentSize().height/2+30);
    _loadingSprite->removeFromParent();
    
    _goto = GOTO::CLOSE;
    _btn_menu->setVisible(true);
}

void PopWindow::ItsCompleted()
{
    log("xxxxxxxxxxxx  is completed");
    _contentLabel->setString("It's completed...");
    _contentLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _contentLabel->setPosition(_bk->getContentSize().width/2,_bk->getContentSize().height/2+30);
    _loadingSprite->removeFromParent();
    
    _goto = GOTO::CLOSE;
    _btn_menu->setVisible(true);
    
    log("_btn_menu is visiable ....");
}

void PopWindow::menuCallback(Ref* ref)
{
    switch (_goto) {
        case GOTO::NONE:
            break;
        case GOTO::CLOSE:
            removeFromParent();
            break;
        case GOTO::MAINMENU:
            Director::getInstance()->replaceScene(MainScreenScene::createScene(true));
            break;
        default:
            break;
    }
}