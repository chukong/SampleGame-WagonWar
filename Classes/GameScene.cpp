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
    auto playerLayer = Layer::create();
    this->addChild(playerLayer, 3, Point(1, 1), offset);
    playerLayer->setContentSize(lvl->getRT()->getContentSize());
    this->setPlayerLayer(playerLayer);
    collChecker->setPlayerLayer(playerLayer);
    playerLayer->setAnchorPoint(Point::ANCHOR_MIDDLE);
    playerLayer->ignoreAnchorPointForPosition(false);
    
    
    
    //layer for bullets
    auto bulletLayer =Layer::create();
    bulletLayer->setContentSize(lvl->getRT()->getContentSize());
    this->setBulletLayer(bulletLayer);
    collChecker->setBulletLayer(bulletLayer);
    bulletLayer->setAnchorPoint(Point::ANCHOR_MIDDLE);
    bulletLayer->ignoreAnchorPointForPosition(false);
    this->addChild(bulletLayer, 4, Point(1, 1), offset);
    
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
    
    //init explosion masks
    this->initExplosionMasks();
    
    
    //init tests
    this->initTests();
    return true;
}
void GameScene::initTests()
{
    auto p = TestNode::create();
    p->setPosition(500,800);
    p->setLastPos(Point(500,800));
    getPlayerLayer()->addChild(p);
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
    _ex->setScaleX(1.7);
    _ex->setScaleY(1.4);
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
                _ex->setPosition(pos);
                _ex->visit();
                b->runAction(RemoveSelf::create());
                //check to see if any player got caught in the blast
                for(Node* player : _PlayerLayer->getChildren())
                {
                    TestNode *p = dynamic_cast<TestNode*>(player);
                    if(!p->airborn && p->getPosition().getDistance(pos)< b->getConfig()->expRadius)
                    {
                        p->airborn = true;
                    }
                }
            }
            else
            {
                b->setPosition(pos+(pos-b->getLastPos())+getGravity()+getWind());
                b->setLastPos(pos);
            }
        }
    _level->getRT()->end();
    for(Node* player : _PlayerLayer->getChildren())
    {
        TestNode *p = dynamic_cast<TestNode*>(player);
        if(p->airborn)
        {
            auto pos = p->getPosition();
            p->setPosition(pos+(pos-p->getLastPos())+getGravity()+getWind());
            p->setLastPos(pos);
        }
    }
}