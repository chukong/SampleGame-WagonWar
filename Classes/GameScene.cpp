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

bool GameScene::init()
{
    if( !ParallaxNode::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto offset = Point(visibleSize/2);
    
    //load map
    auto lvl = Level::create("map.png");
    this->addChild(lvl, 1, Point(1, 1), offset);
    this->setLevel(lvl);
    
    //check collision before background is drawn
    auto collChecker = CollisionCheckNode::create();
    collChecker->setLevel(lvl);
    collChecker->setGameLayer(this);
    this->addChild(collChecker, 2, Point(1, 1), Point::ZERO);
    
    //load background
    auto background = Sprite::create("bluryBack.png");
    this->addChild(background, 3, Point(0.5, 0.5), offset);
    BlendFunc background_blendfunc={
        GL_ONE_MINUS_DST_ALPHA,
        GL_ONE
    };
    background->setBlendFunc(background_blendfunc);

    //layer for players
    
    //layer for bullets
    auto bulletLayer =Layer::create();
    this->setBulletLayer(bulletLayer);
    collChecker->setBulletLayer(bulletLayer);
    this->addChild(bulletLayer, 4, Point::ANCHOR_TOP_RIGHT, Point::ZERO);
    
    
    //layer for effects
    
    //register touches
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //default wind
    this->setWind(Point::ZERO);
    //default gravity
    this->setGravity(Point(0,-0.1));
    this->scheduleUpdate();
    return true;
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
    _click = true;
    return true;
}
void GameScene::onTouchMoved(Touch* touch, Event* event)
{
    _click = false;
    setPosition(getPosition()+touch->getDelta());
}
void GameScene::onTouchEnded(Touch* touch, Event* event)
{
    if(_click)
    {
        addBullet(defaultB, touch->getLocation()-getPosition(), Point(2,5));

    }
}

Bullet* GameScene::addBullet(BulletTypes type, cocos2d::Point pos, cocos2d::Point vector)
{
    auto b = Bullet::create(type, pos, vector);
    _bulletLayer->addChild(b);
    return b;
}

void GameScene::update(float dt)
{
        for(Node* bullet : _bulletLayer->getChildren())
        {
            Bullet *b = dynamic_cast<Bullet*>(bullet);
            auto pos = b->getPosition();
            b->setPosition(pos+(pos-b->getLastPos())+getGravity()+getWind());
            b->setLastPos(pos);
        }
}