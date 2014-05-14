//
//  GameScene.cpp
//  TankMultiplayer
//
//  Created by WuHao on 14-5-13.
//
//

#include "GameScene.h"
#include "Helper.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

GameScene* GameScene::create()
{
    auto ret = new GameScene;
    Size winSize = Director::getInstance()->getVisibleSize();
    
    
    //load map
    auto lvl = Level::create("map.png");
    ret->addChild(lvl,1, Point::ANCHOR_TOP_RIGHT, Point(winSize/2));
    ret->setLevel(lvl);
    lvl->addChild(DepthOn::create(),-1);
    ret->setLevel(lvl);
    
    
    //load background
    auto back = Sprite::create("bluryBack.png");
    ret->addChild(back, 3, Point(0.5,0.5), Point(winSize/2));
    back->addChild(DepthOff::create(), 1);
    
    

    //register touches
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, ret);
    //listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, ret);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, ret);
    ret->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, ret);
    
    ret->autorelease();
    //ret->setPosition(500,500);
    return ret;
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}
void GameScene::onTouchMoved(Touch* touch, Event* event)
{
    setPosition(getPosition()+touch->getDelta());
}

void GameScene::update(float dt)
{
    
}