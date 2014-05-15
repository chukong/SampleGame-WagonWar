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
    auto offset = Point(winSize/2);
    ret->offset = offset;
    
    //load map
    auto lvl = Level::create("map.png");
    ret->addChild(lvl,1, Point::ANCHOR_TOP_RIGHT, offset);
    ret->setLevel(lvl);
    
    //check collision before background is drawn
    auto collChecker = CollisionCheckNode::create();
    collChecker->setLevel(lvl);
    collChecker->setGameLayer(ret);
    ret->addChild(collChecker, 2, Point::ANCHOR_TOP_RIGHT, offset);

    
    
    //load background
    auto back = Sprite::create("bluryBack.png");
    ret->addChild(back, 3, Point(0.5,0.5), offset);
    BlendFunc background={
        GL_ONE_MINUS_DST_ALPHA,
        GL_ONE
    };
    back->setBlendFunc(background);

    
    
    //layer for players
    
    //layer for bullets
    auto bulletLayer =Layer::create();
    //auto bulletLayer =LayerColor::create(Color4B(100,0,0,100));
    bulletLayer->setContentSize(lvl->getRT()->getContentSize());
    //log("contentsize %f, %f", lvl->getRT()->getContentSize().width, lvl->getRT()->getContentSize().height);
    ret->setBulletLayer(bulletLayer);
    collChecker->setBulletLayer(bulletLayer);
    bulletLayer->setAnchorPoint(Point::ANCHOR_MIDDLE);
    bulletLayer->ignoreAnchorPointForPosition(false);
    ret->addChild(bulletLayer, 4, Point::ANCHOR_TOP_RIGHT, offset);
    
    
    //layer for effects
    
    

    //register touches
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, ret);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, ret);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, ret);
    ret->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, ret);
    
    ret->autorelease();


    //default wind
    ret->setWind(Point::ZERO);
    //default gravity
    ret->setGravity(Point(0,-0.1));
    ret->scheduleUpdate();
    
    
    //init explosion masks
    ret->initExplosionMasks();
    return ret;
}
void GameScene::initExplosionMasks()
{
    _ex = Sprite::create("expMask.png");
    _burn = Sprite::create("expMask2.png");
    BlendFunc cut;
    cut ={
        GL_ZERO,
        GL_ONE_MINUS_SRC_ALPHA
    };
    
    BlendFunc keepAlpha;
    keepAlpha={
        GL_DST_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    };
    _ex->setBlendFunc(cut);
    
    _burn->setBlendFunc(keepAlpha);
    _ex->retain();
    _burn->retain();
    
    _burn->setScale(1.05);
    _ex->addChild(_burn);
    _burn->setPosition(Point(_ex->getContentSize()/2));
    _ex->setScaleX(1.4);
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
        Point test(getBulletLayer()->convertToNodeSpace(touch->getLocation()));
        addBullet(defaultB, test, Point(2,5));

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
    _level->getRT()->begin();
        for(Node* bullet : _bulletLayer->getChildren())
        {
            Bullet *b = dynamic_cast<Bullet*>(bullet);
            auto pos = b->getPosition();
            if(b->willExplode())
            {
                //log("%f, %f", b->getPosition().x, b->getPosition().y);
                _ex->setPosition(b->getPosition());
                _ex->visit();
                b->runAction(RemoveSelf::create());
            }
            else
            {
                b->setPosition(pos+(pos-b->getLastPos())+getGravity()+getWind());
                b->setLastPos(pos);
            }
        }
    _level->getRT()->end();
}