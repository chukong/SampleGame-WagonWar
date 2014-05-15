#include "HelloWorldScene.h"
#include "bullet.h"
#include "Helper.h"
#include "Level.h"
//#include "PixelMap.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    auto winSize = Director::getInstance()->getWinSize();
    
    
//    Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGB5A1);
//    auto sp = Sprite::create("map.png");
//    sp->retain();
//    
//    map = RenderTexture::create(sp->getContentSize().width,sp->getContentSize().height, Texture2D::PixelFormat::RGB5A1);
//    //map->setSprite(sp);
//    
//    map->setPosition(Point(sp->getContentSize())/2);

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    scheduleUpdate();
    
    
    Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA8888);
    auto b = Sprite::create("bluryBack.png");
    
    auto winsize = Director::getInstance()->getWinSize();
    b->setPosition(Point(winsize/2));
    
    //s->setGlobalZOrder(1);
    
    
    auto p = ShaderCache::getInstance()->getProgram("ShaderPositionTextureColorAlphaTest_NoMV");
    //map->getSprite()->setShaderProgram(p);
    
    
    //circ = Bullet::create(this);
        circ->setPosition(Point(600,400));
    //Helper::removeAfter(circ, 3);
    
    
    //circ->setGlobalZOrder(1.5);
    b->setGlobalZOrder(2);
    //map->setGlobalZOrder(1);
    
    //addChild(map);
    addChild(circ);
    addChild(b);


    
    //test render texture
//    map->begin();
//    sp->setPosition(Point(sp->getContentSize()/2));
//    sp->visit();
//        //ex->visit();
//    map->end();
    //addChild(ex);

    
    
    auto l = Level::create("map.png");
    addChild(l);
    
    
        glEnable(GL_DEPTH_TEST);
    return true;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    circ->setPosition(touch->getLocation());
    return true;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *event)
{
    circ->setPosition(touch->getLocation());
}

void HelloWorld::update(float dt)
{
//    // check if dn bounding box collide with rt bounding box
//    auto aabb1 = circ->getBoundingBox();
//    auto aabb2 = map->getBoundingBox();
//    //log("%f, %f, %f, %f", aabb2.origin.x, aabb2.origin.y, aabb2.size.width, aabb2.size.height);
//    
//    if(aabb1.intersectsRect(aabb2))
//    {
//        //ok we have bounding box collision, but what about pixel collision
//        //get
//        
//        circ->setColor(Color3B::YELLOW);
//    }
//    else{
//        circ->setColor(Color3B::WHITE);
//    }
    if(expLocations != Point::ZERO)
        makeExplosion();
    
}
void HelloWorld::makeExplosion()
{
    map->begin();
    auto ex = Sprite::create("expMask.png");
    auto burn = Sprite::create("expMask2.png");
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
    ex->setBlendFunc(cut);
    
    burn->setBlendFunc(keepAlpha);
    //ex->retain();
    ex->setPosition(expLocations - map->getPosition() + map->getSprite()->getAnchorPointInPoints());
        burn->setPosition(Point(ex->getContentSize()/2));
    burn->setScale(1.05);
    ex->addChild(burn);
    ex->setScaleX(1.3);
    burn->visit();
        ex->visit();
    

    map->end();
    expLocations = Point::ZERO;
    //Director::getInstance()->getRenderer()->render();
}
void HelloWorld::checkCollision(Node *b)
{
    // check if dn bounding box collide with rt bounding box
    auto aabb1 = b->getBoundingBox();
    auto aabb2 = map->getBoundingBox();
    //log("%f, %f, %f, %f", aabb2.origin.x, aabb2.origin.y, aabb2.size.width, aabb2.size.height);
    
    if(aabb1.intersectsRect(aabb2))
    {
        //ok we have bounding box collision, but what about pixel collision
        //get pixels
        Color4B *buffer = (Color4B*)malloc(sizeof(Color4B)*400);
        Point pos = b->getPosition();
        glReadPixels(pos.x-10, pos.y-10, 20, 20, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        bool coll = false;
        for(int i = 0; i < 400; i++)
        {
            if(buffer[i].r < 1 || buffer[i].b < 1 || buffer[i].g > 0)
            {
                //log("oh %i, %i, %i. %i", buffer[0].r, buffer[0].g, buffer[0].b, buffer[0].a);
                expLocations = pos;
                coll = true;
                break;
            }
        }

        free(buffer);
        if(coll)
            circ->setColor(Color3B::BLUE);
        else
        circ->setColor(Color3B::YELLOW);
    }
    else{
        circ->setColor(Color3B::WHITE);
    }
}