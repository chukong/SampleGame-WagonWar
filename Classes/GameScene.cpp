//
//  GameScene.cpp
//  TankMultiplayer
//
//  Created by WuHao on 14-5-13.
//
//

#include "GameScene.h"
#include "Helper.h"
#include "GameUI.h"
#include "time.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

USING_NS_CC;

Scene* GameScene::createScene()
{

    // 'scene' is an autorelease object
    auto scene = Scene::create();
    auto uiLayer = GameUI::create();
    scene->addChild(uiLayer, 2);
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    
    
    
    // return the scene
    return scene;
}

bool GameScene::init()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto offset = Point(visibleSize/2);
    
    //load background
    auto background = Sprite::create("bluryBack.png");
    this->addChild(background, 1, Point(0.5, 0.5), offset);
    
    //load map
    auto lvl = Level::create("map.png");
    this->addChild(lvl, 2, Point(1, 1), offset);
    this->setLevel(lvl);
    

    //layer for players
    auto playerLayer = Layer::create();
    this->addChild(playerLayer, 3, Point(1, 1), offset);
    playerLayer->setContentSize(lvl->getRT()->getContentSize());
    this->setPlayerLayer(playerLayer);
    playerLayer->setAnchorPoint(Point::ANCHOR_MIDDLE);
    playerLayer->ignoreAnchorPointForPosition(false);
    
    
    
    //layer for bullets
    auto bulletLayer =Layer::create();
    bulletLayer->setContentSize(lvl->getRT()->getContentSize());
    this->setBulletLayer(bulletLayer);
    bulletLayer->setAnchorPoint(Point::ANCHOR_MIDDLE);
    bulletLayer->ignoreAnchorPointForPosition(false);
    this->addChild(bulletLayer, 4, Point(1, 1), offset);
    
    //layer for effects

    
    srand((unsigned)time(NULL));
    srand(rand());
    srand(rand());
    //default wind
    this->setWind(Point(CCRANDOM_MINUS1_1()*0.025,CCRANDOM_MINUS1_1()*0.025));
    log("Wind is : %f, %f", getWind().x, getWind().y);
    //default gravity
    this->setGravity(Point(0,-0.1));
    this->scheduleUpdate();
    
    //init explosion masks
    this->initExplosionMasks();
    
    
    //init tests
    this->initTests();
    
    //init listeners
    initListeners();
    
    
    //build my turn json
    buildMyTurn();
    return true;
}
void GameScene::initListeners()
{
    //register touches
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //event to move left right
    auto moveListener = EventListenerCustom::create("go left", CC_CALLBACK_0(GameScene::movePlayer, this, -0.3));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(moveListener, this);
    auto moveListener2 = EventListenerCustom::create("go right", CC_CALLBACK_0(GameScene::movePlayer, this, 0.3));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(moveListener2, this);
    auto stopListener = EventListenerCustom::create("stop", CC_CALLBACK_0(GameScene::movePlayer, this, 0));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(stopListener, this);
    auto windListener = EventListenerCustom::create("randomWind", CC_CALLBACK_0(GameScene::randomWind, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(windListener, this);
    auto startShootListener = EventListenerCustom::create("start shoot", CC_CALLBACK_0(GameScene::startShoot, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(startShootListener, this);
    auto endShootListener = EventListenerCustom::create("end shoot", CC_CALLBACK_0(GameScene::endShoot, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(endShootListener, this);
    
}
void GameScene::startShoot()
{
    gettimeofday(&_now, nullptr);
    if(!_playback)
    {
        rapidjson::Document::AllocatorType& allocator = _myturn.GetAllocator();
        rapidjson::Value value(rapidjson::kObjectType);
        value.AddMember("tick",_tick,allocator);
        value.AddMember("action", "start shoot", allocator);
        _myturn["actions"].PushBack(value, allocator);
    }
}
void GameScene::endShoot()
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    long seconds = now.tv_sec-_now.tv_sec;
    int usec = now.tv_usec - _now.tv_usec;
    float timed = seconds + float(usec)/1000000;
    timed = (timed>3)? 3 : timed;
    log("diff is %f", timed);
    auto p = getCurrentPlayer();
    auto offset = getMovableSize();
    auto gunlocation = p->gunPoint->getNodeToWorldAffineTransform();
    auto b = addBullet(defaultB, Point(gunlocation.tx, gunlocation.ty)+Point(offset/2)-getPosition(), Point(timed*2,timed*2));
    _following = dynamic_cast<Node*>(b);
    if(_playback)
    {
        //this is the last action
        _waitToClear = true;
    }
    else
    {
        rapidjson::Document::AllocatorType& allocator = _myturn.GetAllocator();
        rapidjson::Value value(rapidjson::kObjectType);
        value.AddMember("tick",_tick,allocator);
        value.AddMember("action", "stop shoot", allocator);
        _myturn["actions"].PushBack(value, allocator);
    }
}
void GameScene::randomWind()
{
    setWind(Point(CCRANDOM_MINUS1_1()*0.035,CCRANDOM_MINUS1_1()*0.035));
    _eventDispatcher->dispatchCustomEvent("wind", &_wind);
}
void GameScene::onEnter()
{
    ParallaxNode::onEnter();
    _eventDispatcher->dispatchCustomEvent("wind", &_wind);
    
    //here is some json
    std::string data = "{\"wind\":{\"x\":-0.01, \"y\":-0.01},\"explosions\":[{\"x\":722.227,\"y\":479.584},{\"x\":741.922,\"y\":435.848},{\"x\":648.444,\"y\":500.432}],\"actions\":[{\"tick\":30,\"action\":\"go right\"},{\"tick\":200,\"action\":\"stop\"},{\"tick\":300,\"action\":\"start shoot\"},{\"tick\":310,\"action\":\"end shoot\"}]}";
    //std::string data = "{\"explosions\":[{\"x\":722.227,\"y\":479.584},{\"x\":741.922,\"y\":435.848},{\"x\":648.444,\"y\":500.432},{\"x\":755.788,\"y\":394.456}],\"actions\":[{\"tick\":405,\"action\":\"go left\"},{\"tick\":439,\"action\":\"stop\"},{\"tick\":471,\"action\":\"go right\"},{\"tick\":488,\"action\":\"stop\"},{\"tick\":505,\"action\":\"go left\"},{\"tick\":541,\"action\":\"stop\"},{\"tick\":563,\"action\":\"go right\"},{\"tick\":567,\"action\":\"stop\"}]}";
    //log("is array? %d", doc["actions"].IsArray());

    
    playback(data);
}
void GameScene::movePlayer(float x)
{
    if(!_playback)
    {
        //save to my turn
        rapidjson::Document::AllocatorType& allocator = _myturn.GetAllocator();
        rapidjson::Value value(rapidjson::kObjectType);
        value.AddMember("tick",_tick,allocator);
        if(x>0)
        {
            value.AddMember("action", "go right", allocator);
        }
        else if(x < 0)
        {
            value.AddMember("action", "go left", allocator);
        }
        else
        {
           value.AddMember("action", "stop", allocator);
        }
        _myturn["actions"].PushBack(value, allocator);
        printMyTurn();
    }
    //TODO: replace with proper get current player
    TestNode* p = getCurrentPlayer();
    p->moveDelta.x = x;
    p->needFix = true;
}

void GameScene::initTests()
{
    auto p = TestNode::create();
    p->setPosition(500,800);
    p->setLastPos(Point(500,800));
    getPlayerLayer()->addChild(p);
    
    
    auto p2 = TestNode::create();
    p2->setPosition(800,800);
    p2->setLastPos(Point(800,800));
    getPlayerLayer()->addChild(p2);

}
void GameScene::initExplosionMasks()
{
    _ex = Sprite::create("expMask.png");
    
    _ex->setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
    _burn = Sprite::create("expMask2.png");
    _burn->setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));

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
    
    _burn->setScaleX(1.7*1.05);
    _burn->setScaleY(1.4*1.05);
    //_ex->addChild(_burn);
    //_burn->setPosition(Point(_ex->getContentSize()/2));
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
    setPosition(getActualPos(touch));
}
Size GameScene::getMovableSize()
{
    if(!&_movableSize || _movableSize.equals(Size::ZERO) )
    {
        auto visibleSize = Director::getInstance()->getWinSize();
        _movableSize =_level->getRT()->getContentSize()-visibleSize;
    }
    return _movableSize;
}
Point GameScene::getActualPos(cocos2d::Touch * touch)
{
    const int base_distance = 200;
    const float base_ratio = 0.3f;
    
    auto visibleSize = Director::getInstance()->getWinSize();
    Point pos(getPosition()+touch->getDelta());
    Point min_inclusive = Point(-(_level->getRT()->getContentSize().width-visibleSize.width)/2,-(_level->getRT()->getContentSize().height-visibleSize.height)/2);
    Point max_inclusive = Point((_level->getRT()->getContentSize().width-visibleSize.width)/2,(_level->getRT()->getContentSize().height-visibleSize.height)/2);
    Point actual_point = pos.getClampPoint(min_inclusive, max_inclusive);
    if(actual_point.x>0 && max_inclusive.x-actual_point.x<base_distance && max_inclusive.x-actual_point.x>0)
    {
        actual_point.x = getPosition().x+ (base_ratio+(max_inclusive.x-actual_point.x)/(100/(1-base_ratio)))*touch->getDelta().x;
    }
    else if(actual_point.x<0 && actual_point.x-min_inclusive.x<base_distance && actual_point.x-min_inclusive.x>0)
    {
        actual_point.x = getPosition().x+ (base_ratio+(actual_point.x-min_inclusive.x)/(100/(1-base_ratio)))*touch->getDelta().x;
    }
    
    if(actual_point.y>0 && max_inclusive.y-actual_point.y<base_distance && max_inclusive.y-actual_point.y>0)
    {
        actual_point.y = getPosition().y+ (base_ratio+(max_inclusive.y-actual_point.y)/(100/(1-base_ratio)))*touch->getDelta().y;
    }
    else if(actual_point.y<0 && actual_point.y-min_inclusive.y<base_distance && actual_point.y-min_inclusive.y>0)
    {
        actual_point.y = getPosition().y+ (base_ratio+(actual_point.y-min_inclusive.y)/(100/(1-base_ratio)))*touch->getDelta().y;
    }
    
    //log("WTF...%f,%f",actual_point.x,actual_point.y);
    return actual_point;
}

void GameScene::onTouchEnded(Touch* touch, Event* event)
{
}
Bullet* GameScene::addBullet(BulletTypes type, cocos2d::Point pos, cocos2d::Point vector)
{
    auto b = Bullet::create(type, pos, vector);
    _bulletLayer->addChild(b);
    return b;
}
TestNode* GameScene::getCurrentPlayer()
{
    auto player = _PlayerLayer->getChildren().front();
    return dynamic_cast<TestNode*>(player);
}
void GameScene::buildMyTurn()
{
    _myturn.SetObject();
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Document::AllocatorType& allocator = _myturn.GetAllocator();
    _myturn.AddMember("explosions", array, allocator);
    
    rapidjson::Value actions(rapidjson::kArrayType);
    _myturn.AddMember("actions", actions, allocator);

    
    printMyTurn();
}
void GameScene::printMyTurn()
{
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    _myturn.Accept(writer);
    log("--\n%s\n--\n", strbuf.GetString());
}
void GameScene::explode(Bullet *bullet)
{
    _following = nullptr;
    auto pos = bullet->getPosition();
    
    if(!_playback)
    {
        rapidjson::Value object(rapidjson::kObjectType);
        rapidjson::Document::AllocatorType& allocator = _myturn.GetAllocator();
        object.AddMember("x", pos.x, allocator);
        object.AddMember("y", pos.y, allocator);
        _myturn["explosions"].PushBack(object, allocator);
        
        rapidjson::StringBuffer strbuf;
        rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
        _myturn.Accept(writer);
        log("--\n%s\n--\n", strbuf.GetString());
    }

    
    
    _ex->setPosition(pos);
    //TODO: set _ex size according to bullet config
    _ex->ManualDraw();
    _burn->setPosition(pos);
    _burn->ManualDraw();
    float exRad =bullet->getConfig()->expRadius;
    bullet->runAction(RemoveSelf::create());
    
    //check to see if player got caught in the blast
    for(Node* player : _PlayerLayer->getChildren())
    {
        TestNode *p = dynamic_cast<TestNode*>(player);
        Point ppos(p->getPosition());
        float dist = ppos.getDistance(pos);
        if(dist< exRad + p->radius)
        {
            p->airborn = true;
            //TODO: bullet might have push force
            //get angle from player to bullet

            float rad = (ppos-pos).getAngle();
            float pushForce = (exRad - dist)*0.05;
            Point mid(ppos.x-pushForce*cosf(rad), ppos.y-pushForce*sinf(rad));
            log("b pos %f, %f | m pos %f, %f", (pos-ppos).x, (pos-ppos).y, (mid-ppos).x, (mid-ppos).y);
            p->setLastPos(mid);
            
            //TODO: player should take damage
        }
    }
}
void GameScene::playback(std::string json)
{
    _replay.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
    _tick = 0;
    _playback = true;
    //TODO: need to disable UI layer touch
    _eventDispatcher->dispatchCustomEvent("touch off");
    
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    _replay.Accept(writer);
    
    //copy all explosions to my turn
    if(_replay["explosions"].Size())
    {
        //recreate those explosions
        _level->getRT()->onBegin();
        for(int i = 0; i < _replay["explosions"].Size(); i++)
        {
            auto &jsonPos =_replay["explosions"][i];
            Point pos(jsonPos["x"].GetDouble(), jsonPos["y"].GetDouble());
            _ex->setPosition(pos);
            //TODO: set _ex size according to explosion size
            _ex->ManualDraw();
            _burn->setPosition(pos);
            _burn->ManualDraw();
            
        }
        _level->getRT()->onEnd();
        _myturn["explosions"] = _replay["explosions"];
    }
    //set wind according to data
    if(_replay["wind"].IsObject())
    {
        auto &jsonWind = _replay["wind"];
        Point wind(jsonWind["x"].GetDouble(), jsonWind["y"].GetDouble());
        setWind(wind);
    }
    
    
    log("--\n%s\n--\n", strbuf.GetString());
}

void GameScene::update(float dt)
{
    bool everythingSleep = true;
    
	if(_playback)
    {
        rapidjson::Value &array = _replay["actions"];
        if(array.IsArray())
        {
            for(int i=0; i < array.Size(); i++)
            {
                if(_tick == array[i]["tick"].GetInt())
                {
                    //execute that action
                    _eventDispatcher->dispatchCustomEvent(array[i]["action"].GetString());
                }
            }
        }
    }
    //if we have a following target, then follow it
    if(_following)
    {
        auto fp = _following->getPosition();
        auto tp =-fp + Point(_PlayerLayer->getContentSize()/2);
        auto cp = getPosition();
        setPosition(cp+(tp-cp)*0.02);
    }
    
    
    
    //hack alert::switch GL target to the map
    //kmGLPushMatrix();
    _level->getRT()->onBegin();
    //now the target is the render texture, we can begin reading the bullet and player pixels
    
    auto aabb2 = _bulletLayer->getBoundingBox();
    //Point offset(aabb2.origin+getPosition());
    aabb2.origin = Point::ZERO;
    for(Node* bullet : _bulletLayer->getChildren())
    {
        everythingSleep = false;
        Bullet *b = dynamic_cast<Bullet*>(bullet);
        auto pos = b->getPosition();
        bool coll = false;
        int bulletRadius =b->getConfig()->radius;
        
        //check bounding box to see if the bullet is in the world
        auto aabb1 = b->getBoundingBox();
        if(aabb2.intersectsRect(aabb1))
        {
            //move this
            b->setPosition(pos+(pos-b->getLastPos())+getGravity()+getWind());
            b->setLastPos(pos);
            
            
            //check if we are colliding with a player
            for(Node *player : _PlayerLayer->getChildren())
            {
                TestNode* p = dynamic_cast<TestNode*>(player);
                if(b->getPosition().getDistance(p->getPosition()) < bulletRadius + p->radius)
                {
                    explode(b);
                    coll = true;
                    log("collide with player");
                    break;
                }
            }
            //if we didnt collide with player, then we have to check for the terrain
            if(!coll)
            {
                Point pos(b->getPosition());
                //init a buffer size big enough to hold a square that can contain a circle with specified radius
                int bufferSize = pow(bulletRadius*2, 2);
                Color4B *buffer = (Color4B*)malloc(sizeof(Color4B)*bufferSize);
                glReadPixels(pos.x-bulletRadius, pos.y-bulletRadius, bulletRadius*2, bulletRadius*2, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
                for(int i = 0; i < bufferSize; i++)
                {
                    //for accuracy, we only want the pixels in the circle
                    if(buffer[i].a>0 && Helper::isInCircle(i, bulletRadius))
                    {
                        explode(b);
                        coll = true;
                        break;
                    }
                }
                free(buffer);
            }
            //debug set color
            if(coll)
                b->setColor(Color3B::BLUE);
            else
                b->setColor(Color3B::YELLOW);
        }
        //if its not contained in the map
        else
        {
            b->runAction(RemoveSelf::create());
            if(_following == b)
            {
                _following = nullptr;
            }
        }
    }
    
    
    //Finished looping bullet, now lets loop player
    auto playerNodes = _PlayerLayer->getChildren();
    if(playerNodes.size())
    {
        for(Node* player : _PlayerLayer->getChildren())
        {
            TestNode* p = dynamic_cast<TestNode*>(player);
            if(p->airborn || p->needFix || p->moveDelta.x)
            {
                everythingSleep = false;
                //move this
                auto pos2 = p->getPosition();
                auto pos = pos2+(pos2-p->getLastPos())+getGravity()+getWind();
                p->setPosition(pos);
                p->setLastPos(pos2);
                
                int radius = p->radius;
                int bufferSize = pow(radius*2, 2);
                Color4B *buffer = (Color4B*)malloc(sizeof(Color4B)*bufferSize);
                glReadPixels(pos.x-radius, pos.y-radius, radius*2, radius*2, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
                float angleTotal =0;
                int angleCount = 0;
                for(int i = 0; i < bufferSize; i++)
                {
                    if(buffer[i].a> 0 && Helper::isInCircle(i, radius))
                    {
                        //TODO: need to fix position, so does not clip with terrain, and get angle
                        float an = Helper::getAngleFromIndex(i, radius);
                        if(an != -999)
                        {
                            angleTotal+=an;
                            angleCount++;
                            //log("rad: %f, %i", an, i);
                        }
                        
                        p->setLastPos(p->getPosition());
                        //break;
                    }
                }
                free(buffer);
                
                //check how many collision points
                if(angleCount>1)
                {
                    p->airborn = false;
                    //set angle to average
                    float deg =CC_RADIANS_TO_DEGREES(angleTotal/angleCount);
                    if(abs(deg) > 80)//TODO: each vehicle has a climbing angle limit
                    {
                        p->setRotation((deg>0)? 80:-80);
                        p->airborn = true;
                    }
                    else
                    {
                        p->setRotation(deg);
                    }
                    if(angleCount > 8)
                    {
                        //we are colliding with too many pixels
                        
                        float pushForce = 0.4;
                        Point mid(pos.x-pushForce*sinf(angleTotal/angleCount), pos.y-pushForce*cosf(angleTotal/angleCount));
                        p->setLastPos(mid);
                        p->needFix = true;
                    }
                    else
                    {
                        p->needFix = false;
                    }
                }
                
            }
            if(p->moveDelta.x)
            {
                p->setPosition(p->getPosition()+p->moveDelta);
                p->setLastPos(p->getLastPos()+p->moveDelta);
                if(p->moveDelta.x>0)
                {
                    //TODO: replace with proper flip code
                    p->setScaleX(1);
                }
                else{
                    p->setScaleX(-1);
                }
            }
            //kmGLPopMatrix();
        }
    }
    _level->getRT()->onEnd();
    _tick++;
    
    if(_waitToClear && everythingSleep)
    {
        _eventDispatcher->dispatchCustomEvent("touch on");
        _waitToClear = false;
        _playback = false;
        _tick = 0;
        log("play back finished");
    }
}