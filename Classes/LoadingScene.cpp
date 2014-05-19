//
//  LoadingScene.cpp
//  TankMultiplayer
//
//  Created by Jacky on 14-5-16.
//
//

#include "LoadingScene.h"

Scene* LoadingScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = LoadingScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool LoadingScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    auto loading_bg = Sprite::create("HelloWorld.png");
    loading_bg->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(loading_bg, 0);
    
    loadRes();
    
    return true;
}

void LoadingScene::loadRes()
{
    
}
