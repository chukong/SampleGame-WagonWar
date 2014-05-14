//
//  GameScene.cpp
//  TankMultiplayer
//
//  Created by WuHao on 14-5-13.
//
//

#include "GameScene.h"

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

    //load map
    auto lvl = Level::create("map.png");
    ret->addChild(lvl,1, Point::ANCHOR_TOP_RIGHT, Point::ZERO);
    ret->setLevel(lvl);
    
    //load background
    auto back = Sprite::create("bluryBack.png");
    ret->addChild(back, 3, Point(0.5,0.5), Point::ZERO);
    
    ret->autorelease();
    return ret;
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}
void GameScene::onTouchMoved(Touch* touch, Event* event)
{
    
}

void GameScene::update(float dt)
{
    
}