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
#include "Configuration.h"
#include "GPGSManager.h"
#include "Aimer.h"
#include "NoTouchLayer.h"
#include "MainScreenScene.h"
#include "SimpleAudioEngine.h"
#include "VisibleRect.h"
#include "PopWindow.h"
#include "GPGSChecker.h"

USING_NS_CC;

GameScene* g_GameScene;

Scene* GameScene::createScene()
{
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    auto uiLayer = GameUI::create();
    uiLayer->setTag(76);
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
    g_GameScene = this;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto offset = Point(visibleSize/2);
    
    //load background
    auto background = Sprite::create("bluryBack.png");
    this->addChild(background, 1, Point(0.5, 0.3), offset+Point(0, 50));
    
    //load map
    auto lvl = Level::create("map2.png");
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
    auto effectLayer =Layer::create();
    effectLayer->setContentSize(lvl->getRT()->getContentSize());
    this->setEffectLayer(effectLayer);
    effectLayer->setAnchorPoint(Point::ANCHOR_MIDDLE);
    effectLayer->ignoreAnchorPointForPosition(false);
    this->addChild(effectLayer, 5, Point(1, 1), offset);
    
    
    
    //default gravity
    this->setGravity(Point(0,-0.1));
    this->scheduleUpdate();
    
    //init explosion masks
    this->initExplosionMasks();
    
    //init listeners
    initListeners();
    
    _isWentOut = false;
    
    
    auto menuitem = MenuItemImage::create("return_0.png", "return_1.png", [](Ref* sender)
                                          {
                                              auto scene = MainScreenScene::createScene(true);
                                              Director::getInstance()->replaceScene(scene);
                                          } );
    
    menuitem->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
    menuitem->setPosition(Point(20, g_visibleRect.visibleHeight-20));
    
    returnMenu = Menu::create(menuitem, nullptr);
    returnMenu->setAnchorPoint(Point::ANCHOR_MIDDLE);
    returnMenu->setPosition(Point::ZERO);
    this->addChild(returnMenu, 100, Point(0, 0),Point(0, 0));
    returnMenu->setVisible(false);
    
    return true;
}
void GameScene::initListeners()
{
    //register touches
    listenertouch = EventListenerTouchOneByOne::create();
    listenertouch->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listenertouch->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    listenertouch->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenertouch, this);
    
    //event to move left right
    auto moveListener = EventListenerCustom::create("go left", CC_CALLBACK_0(GameScene::movePlayer, this, -1));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(moveListener, this);
    auto moveListener2 = EventListenerCustom::create("go right", CC_CALLBACK_0(GameScene::movePlayer, this, 1));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(moveListener2, this);
    auto stopListener = EventListenerCustom::create("stop", CC_CALLBACK_0(GameScene::movePlayer, this, 0));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(stopListener, this);
//    auto windListener = EventListenerCustom::create("randomWind", CC_CALLBACK_0(GameScene::randomWind, this));
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(windListener, this);
    auto startShootListener = EventListenerCustom::create("start shoot", CC_CALLBACK_0(GameScene::startShoot, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(startShootListener, this);
    auto endShootListener = EventListenerCustom::create("end shoot", CC_CALLBACK_0(GameScene::endShoot, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(endShootListener, this);
    
    auto aimTouchListener = EventListenerCustom::create("start angle", std::bind(&GameScene::startAngle,this, std::placeholders::_1));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(aimTouchListener, this);
    
    auto aimEndListener = EventListenerCustom::create("end angle", std::bind(&GameScene::endAngle, this,std::placeholders::_1));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(aimEndListener, this);
    
    auto playerDeadListener = EventListenerCustom::create("playerdead", CC_CALLBACK_1(GameScene::playerdead, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(playerDeadListener, this);
    
//    auto returnMenuListener = EventListenerCustom::create("returntoMenu", CC_CALLBACK_0(GameScene::returntoMenu, this));
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(returnMenuListener, this);
    
    auto gameshowpopwindowlistener = EventListenerCustom::create("gameshowpopwindowlistener", CC_CALLBACK_0(GameScene::showConnectingPopWindow, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(gameshowpopwindowlistener, this);
    
    auto showReturnBtnlistener = EventListenerCustom::create("showReturnBtn", CC_CALLBACK_0(GameScene::showReturnBtn, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(showReturnBtnlistener, this);
}

void GameScene::startAngle(EventCustom* event){
    
    auto p = getCurrentPlayer();
    
    if(_playback){
        
        int array[2];
        array[0] = *(int*)event->getUserData();
        array[1] = *((int*)(event->getUserData())+1);
        log(" aaarray %d", array[0]);
        log("aaarray %d",array[1]);
        if(p->getTag() == TAG_MYSELF)//player 1
        {
            p->aim->runRotation(array[0] * 0.016f, array[1]);
        }
        else{
            p->aim->runRotation(array[0] * 0.016f, array[1]);
        }
        
    } else {
        
        rapidjson::Document::AllocatorType& allocator = _myturn.GetAllocator();
        rapidjson::Value value(rapidjson::kObjectType);
        value.AddMember("tick",_tick,allocator);
        value.AddMember("action", "start angle", allocator);
        int x = (intptr_t)event->getUserData();
        value.AddMember("value", (int)(x),allocator);
        _myturn["actions"].PushBack(value, allocator);
        printMyTurn();
    }
    
}

void GameScene::endAngle(EventCustom* event){
    if(_playback){
        
//        if(p->getTag() == TAG_MYSELF)//player 1
//        {
//            _replay["actions"]["shootangle"].SetDouble(angle);
//            //p->aim->setAngle(angle-p->_wagonPoint->getRotation());
//        }
//        else{
//            _replay["player2"]["shootangle"].SetDouble(angle);
//            //p->aim->setAngle(angle-p->_wagonPoint->getRotation());
//        }
        
    } else {
        rapidjson::Document::AllocatorType& allocator = _myturn.GetAllocator();
        rapidjson::Value value(rapidjson::kObjectType);
        value.AddMember("tick",_tick,allocator);
        value.AddMember("action", "end angle", allocator);
        int x = (intptr_t)event->getUserData();
        value.AddMember("value", x, allocator);
        _myturn["actions"].PushBack(value, allocator);
        printMyTurn();
    }
}

void GameScene::startShoot()
{
    _tickPre = _tick;
    getCurrentPlayer()->startshoot();
    if(!_playback)
    {
        rapidjson::Document::AllocatorType& allocator = _myturn.GetAllocator();
        rapidjson::Value value(rapidjson::kObjectType);
        value.AddMember("tick",_tick,allocator);
        value.AddMember("action", "start shoot", allocator);
        _myturn["actions"].PushBack(value, allocator);
        
        if(!_isFirstAction)
        {
            _isFirstAction = true;
            
            Hero* myself = dynamic_cast<Hero*>(_PlayerLayer->getChildByTag(TAG_MYSELF));
            Hero* other = dynamic_cast<Hero*>(_PlayerLayer->getChildByTag(TAG_OTHER));
            _myturn["player1"]["posx"].SetDouble(myself->getPositionX());
            _myturn["player1"]["posy"].SetDouble(myself->getPositionY());
            _myturn["player2"]["posx"].SetDouble(other->getPositionX());
            _myturn["player2"]["posy"].SetDouble(other->getPositionY());
            _myturn["player1"]["rot"].SetDouble(myself->_wagonPoint->getRotation());
            _myturn["player2"]["rot"].SetDouble(other->_wagonPoint->getRotation());
        }
    }
    else
    {
        _eventDispatcher->dispatchCustomEvent("increasePower");
        log("playback shoot start");
    }
}
void GameScene::endShoot()
{
    listenertouch->setEnabled(false);
    int tick = _tick - _tickPre;
    tick = tick>180?180:tick;
    //log("tick %d", tick);
    auto p = getCurrentPlayer();
    auto offset = getMovableSize();
    auto gunlocation = p->gunPoint->getNodeToWorldAffineTransform();
    float angle;
    
    getCurrentPlayer()->endshoot();
    //getCurrentPlayer()->stop();
    if(_playback)
    {
        log("playback shoot end");
        if(p->getTag() == TAG_MYSELF)//player 1
        {
            angle = _replay["player1"]["shootangle"].GetDouble();
            p->aim->setAngle(angle-p->_wagonPoint->getRotation());
        }
        else{
            angle = _replay["player2"]["shootangle"].GetDouble();
            p->aim->setAngle(angle-p->_wagonPoint->getRotation());
        }
        _eventDispatcher->dispatchCustomEvent("dismissPower");
//        _waitToClear = true;
    }
    else
    {
        rapidjson::Document::AllocatorType& allocator = _myturn.GetAllocator();
        rapidjson::Value value(rapidjson::kObjectType);
        value.AddMember("tick",_tick,allocator);
        value.AddMember("action", "end shoot", allocator);
        angle = p->aim->getWorldAngle();
        if(p->getTag() == TAG_MYSELF)//player 1
        {
            _myturn["player1"]["shootangle"].SetDouble(angle);
            
            //p->aim->setAngle(angle-p->_wagonPoint->getRotation());
        }
        else{
            _myturn["player2"]["shootangle"].SetDouble(angle);
            //p->aim->setAngle(angle-p->_wagonPoint->getRotation());
        }
        
        _myturn["actions"].PushBack(value, allocator);
        _eventDispatcher->dispatchCustomEvent("touch off");
        
        ((GameUI*)(getParent()->getChildByTag(76)))->_controlBoard->runAction(EaseBackIn::create(MoveTo::create(0.5, Point(g_visibleRect.visibleWidth/2 + g_visibleRect.visibleOriginX, -120))));
        getCurrentPlayer()->hideAimer();
        
//        _waitToClear = true;
        
    }
    //log("angle %f", angle);
    switch(getCurrentPlayer()->_heroConfig.wagon)
    {
        case Wagon::TANK:
        {
            auto fire = [=](){
                auto b = addBullet(tankB, Point(gunlocation.tx, gunlocation.ty)+Point(offset/2)-getPosition(), Point(tick/60.0f*8*cosf(CC_DEGREES_TO_RADIANS(-angle)), tick/60.0f*8*sinf(CC_DEGREES_TO_RADIANS(-angle))));
                _following = dynamic_cast<Node*>(b);
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tankshoot.mp3");
            };
            runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create(fire), nullptr));
            break;
        }
        case Wagon::ROCK:
        {
            incre = -1.5;
            auto fire = [=](){
                float powpercent = 20.0;
                float angMultiplier = 3.5;
                float powx = tick/60.0f*8*cosf(CC_DEGREES_TO_RADIANS(-angle+incre*angMultiplier)) * (powpercent- abs(incre))/powpercent;
                float powy = tick/60.0f*8*sinf(CC_DEGREES_TO_RADIANS(-angle+incre*angMultiplier)) * (powpercent- abs(incre))/powpercent;
                addBullet(rockB, Point(gunlocation.tx, gunlocation.ty)+Point(offset/2)-getPosition(), Point(powx, powy));
                incre++;
                powx = tick/60.0f*8*cosf(CC_DEGREES_TO_RADIANS(-angle+incre*angMultiplier)) * (powpercent- abs(incre))/powpercent;
                powy = tick/60.0f*8*sinf(CC_DEGREES_TO_RADIANS(-angle+incre*angMultiplier)) * (powpercent- abs(incre))/powpercent;
                addBullet(rockB, Point(gunlocation.tx, gunlocation.ty)+Point(offset/2)-getPosition(), Point(powx, powy));
                incre++;
                powx = tick/60.0f*8*cosf(CC_DEGREES_TO_RADIANS(-angle+incre*angMultiplier)) * (powpercent- abs(incre))/powpercent;
                powy = tick/60.0f*8*sinf(CC_DEGREES_TO_RADIANS(-angle+incre*angMultiplier)) * (powpercent- abs(incre))/powpercent;
                addBullet(rockB, Point(gunlocation.tx, gunlocation.ty)+Point(offset/2)-getPosition(), Point(powx, powy));
                incre++;
                powx = tick/60.0f*8*cosf(CC_DEGREES_TO_RADIANS(-angle+incre*angMultiplier)) * (powpercent- abs(incre))/powpercent;
                powy = tick/60.0f*8*sinf(CC_DEGREES_TO_RADIANS(-angle+incre*angMultiplier)) * (powpercent- abs(incre))/powpercent;
                auto b = addBullet(rockB, Point(gunlocation.tx, gunlocation.ty)+Point(offset/2)-getPosition(), Point(powx, powy));
                _following = dynamic_cast<Node*>(b);
                incre++;
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("rockshoot.mp3");
            };
            
            runAction(Sequence::create(DelayTime::create(1.2), CallFunc::create(fire), nullptr));

            break;
        }
        case Wagon::HORSEY:
        {
            auto fire = [=](){
                auto b = addBullet(horseyB, Point(gunlocation.tx, gunlocation.ty)+Point(offset/2)-getPosition(), Point(tick/60.0f*8*cosf(CC_DEGREES_TO_RADIANS(-angle)), tick/60.0f*8*sinf(CC_DEGREES_TO_RADIANS(-angle))));
                _following = dynamic_cast<Node*>(b);
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("horseyshoot.mp3");
            };
            runAction(Sequence::create(DelayTime::create(1.2), CallFunc::create(fire), nullptr));
            break;
        }
        case Wagon::MECH:
        {
            auto fire = [=](){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("mechshoot.mp3");
                auto b = addBullet(mechB, Point(gunlocation.tx, gunlocation.ty)+Point(offset/2)-getPosition(), Point(tick/60.0f*8*cosf(CC_DEGREES_TO_RADIANS(-angle)), tick/60.0f*8*sinf(CC_DEGREES_TO_RADIANS(-angle))));
                _following = dynamic_cast<Node*>(b);
            };
            runAction(Sequence::create(DelayTime::create(1.2), CallFunc::create(fire), nullptr));
            break;
        }
        default:
        {
            auto b = addBullet(defaultB, Point(gunlocation.tx, gunlocation.ty)+Point(offset/2)-getPosition(), Point(tick/60.0f*8*cosf(CC_DEGREES_TO_RADIANS(-angle)), tick/60.0f*8*sinf(CC_DEGREES_TO_RADIANS(-angle))));
            _following = dynamic_cast<Node*>(b);
            break;
        }
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
    std::string player2turn2 = "{\"turn\":1,\"player1\":{\"shootangle\":\"\",\"wagon\":0,\"male\":true,\"hp\":1000,\"posx\":520,\"posy\":800,\"facing\":\"right\"},\"player2\":{\"shootangle\":\"\",\"wagon\":1,\"male\":false,\"hp\":1000,\"posx\":1000,\"posy\":800,\"facing\":\"left\"},\"actions\":[],\"explosions\":[],\"windx\":0.01,\"windy\":0.01}";
    std::string player1turn3 = "{\"turn\":2,\"player1\":{\"shootangle\":\"\",\"wagon\":0,\"male\":true,\"hp\":1000,\"posx\":520,\"posy\":800,\"facing\":\"right\"},\"player2\":{\"shootangle\":-179.172,\"wagon\":1,\"male\":false,\"hp\":1000,\"posx\":1000,\"posy\":800,\"facing\":\"left\"},\"actions\":[{\"tick\":139,\"action\":\"go left\"},{\"tick\":154,\"action\":\"stop\"},{\"tick\":172,\"action\":\"go right\"},{\"tick\":489,\"action\":\"stop\"},{\"tick\":511,\"action\":\"go left\"},{\"tick\":513,\"action\":\"stop\"},{\"tick\":590,\"action\":\"start shoot\"},{\"tick\":609,\"action\":\"end shoot\"}],\"explosions\":[],\"windx\":0.01,\"windy\":0.01}";
    std::string player2turn4 = "{\"turn\":3,\"player1\":{\"shootangle\":-45,\"wagon\":0,\"male\":true,\"hp\":1000,\"posx\":546.472,\"posy\":573.07,\"facing\":\"right\"},\"player2\":{\"shootangle\":-179.172,\"wagon\":1,\"male\":false,\"hp\":1000,\"posx\":1084.18,\"posy\":592.764,\"facing\":\"left\"},\"actions\":[{\"tick\":270,\"action\":\"go right\"},{\"tick\":637,\"action\":\"stop\"},{\"tick\":670,\"action\":\"start shoot\"},{\"tick\":696,\"action\":\"end shoot\"}],\"explosions\":[{\"x\":676.935,\"y\":485.313}],\"windx\":0.01,\"windy\":0.01}";
    std::string player1turn5 = "{\"turn\":20,\"player1\":{\"name\":\"Hao Wu\",\"rot\":15,\"wagon\":3,\"male\":true,\"hp\":580,\"posx\":229,\"posy\":513,\"shootangle\":-31.1497,\"facing\":\"right\"},\"windx\":-0.00829815,\"windy\":-0.00761271,\"explosions\":[{\"x\":560.848,\"y\":545.339},{\"x\":1605.65,\"y\":647.186},{\"x\":469.664,\"y\":565.777},{\"x\":883.879,\"y\":482.913},{\"x\":504.41,\"y\":533.904},{\"x\":442.529,\"y\":525.837},{\"x\":1079.57,\"y\":572.658},{\"x\":1151.67,\"y\":580.816},{\"x\":620.525,\"y\":515.031},{\"x\":938.869,\"y\":478.232},{\"x\":348.725,\"y\":547.389},{\"x\":479.785,\"y\":308.971},{\"x\":554.495,\"y\":542.633},{\"x\":540.459,\"y\":522.691},{\"x\":182.593,\"y\":225.413},{\"x\":526.409,\"y\":351.889}],\"actions\":[{\"tick\":200,\"action\":\"go right\"},{\"tick\":304,\"action\":\"stop\"},{\"tick\":330,\"action\":\"go left\"},{\"tick\":335,\"action\":\"stop\"},{\"tick\":467,\"action\":\"start shoot\"},{\"tick\":468,\"action\":\"end shoot\"}],\"player2\":{\"name\":\"Chenhui Lin\",\"rot\":-15,\"wagon\":2,\"male\":false,\"hp\":250,\"posx\":1050,\"posy\":354.995,\"shootangle\":-176.795,\"facing\":\"left\"}}";
    
    std::string playerTurn6 = "{\"turn\":3,\"player1\":{\"name\":\"Hao Wu\",\"wagon\":2,\"male\":true,\"hp\":300,\"posx\":575.099,\"posy\":559.174,\"shootangle\":-2.08812,\"facing\":\"right\",\"rot\":0},\"windx\":0.0212713,\"windy\":0.00225526,\"explosions\":[{\"x\":1003.26,\"y\":536.647,\"size\":64}],\"actions\":[{\"tick\":174,\"action\":\"start angle\",\"value\":4},{\"tick\":220,\"action\":\"end angle\",\"value\":-2},{\"tick\":409,\"action\":\"start shoot\"},{\"tick\":481,\"action\":\"end shoot\"}],\"player2\":{\"name\":\"Chenhui Lin\",\"wagon\":0,\"male\":false,\"hp\":259,\"posx\":1047.09,\"posy\":583.947,\"shootangle\":-81.2836,\"facing\":\"left\",\"rot\":0}}";
    
    std::string playerTurn7 = "{\"turn\":2,\"player1\":{\"name\":\"Chenhui Lin\",\"wagon\":3,\"male\":true,\"hp\":300,\"posx\":229,\"posy\":513,\"rot\":0,\"shootangle\":\"\",\"facing\":\"right\"},\"windx\":-0.008853,\"windy\":0.00858232,\"explosions\":[],\"actions\":[{\"tick\":147,\"action\":\"start shoot\"},{\"tick\":202,\"action\":\"end shoot\"}],\"player2\":{\"name\":\"Hao Wu\",\"wagon\":3,\"male\":true,\"hp\":300,\"posx\":1443,\"posy\":509,\"rot\":0,\"shootangle\":231.237,\"facing\":\"left\"}}";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    playback(g_gameConfig.match_string);
#else
    playback(playerTurn7);
#endif
    buildMyTurn();
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
    Hero* p = getCurrentPlayer();
    
    p->moveDelta.x = x;
    p->needFix = true;
    if (x>0)
    {
        p->moveright();
    }
    else if(x<0)
    {
        p->moveleft();
    }
    else
    {
        p->stop();
    }
    
    log("I am in %d",_isFirstAction);
    if(!_isFirstAction && !_playback)
    {
        log("I am in ....");
        _isFirstAction = true;
        Hero* myself = dynamic_cast<Hero*>(_PlayerLayer->getChildByTag(TAG_MYSELF));
        Hero* other = dynamic_cast<Hero*>(_PlayerLayer->getChildByTag(TAG_OTHER));
        _myturn["player1"]["posx"].SetDouble(myself->getPositionX());
        _myturn["player1"]["posy"].SetDouble(myself->getPositionY());
        _myturn["player2"]["posx"].SetDouble(other->getPositionX());
        _myturn["player2"]["posy"].SetDouble(other->getPositionY());
        _myturn["player1"]["rot"].SetDouble(myself->_wagonPoint->getRotation());
        _myturn["player2"]["rot"].SetDouble(other->_wagonPoint->getRotation());
    }
}

void GameScene::initPlayers()
{

    Body boyorgirl;
    if (_replay["player1"]["male"].GetBool())
        boyorgirl = BOY;
    else
        boyorgirl = GIRL;
    p1 = Hero::create(Myself,boyorgirl,(Wagon)_replay["player1"]["wagon"].GetInt(),false);
    p1->setName("player1");
    p1->setTag(TAG_MYSELF);
    p1->stop();
    getPlayerLayer()->addChild(p1);
    

    if (_replay["player2"]["male"].GetBool())
        boyorgirl = BOY;
    else
        boyorgirl = GIRL;
    p2 = Hero::create(Other,boyorgirl,(Wagon)_replay["player2"]["wagon"].GetInt(),false);
    p2->setName("player2");
    p2->setTag(TAG_OTHER);
    
    p2->stop();
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
    _following = nullptr;
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
    listenertouch->setEnabled(true);
            _waitToClear = true;
    auto b = Bullet::create(type, pos, vector);
    _bulletLayer->addChild(b);
    return b;
}
Hero* GameScene::getCurrentPlayer()
{
    auto turn = _replay["turn"].GetInt();
    if(turn%2)
    {
        auto player = _PlayerLayer->getChildByTag(!_playback?TAG_OTHER:TAG_MYSELF);
        return dynamic_cast<Hero*>(player);
    }
    else
    {
        auto player = _PlayerLayer->getChildByTag(_playback?TAG_OTHER:TAG_MYSELF);
        return dynamic_cast<Hero*>(player);
    }
}
void GameScene::buildMyTurn()
{
    _myturn.Parse<rapidjson::kParseDefaultFlags>(tempjson.c_str());
    printMyTurn();
}
void GameScene::printMyTurn()
{
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    _myturn.Accept(writer);
    log("--\n%s\n--\n", strbuf.GetString());
}
void GameScene::explode(Bullet *bullet, Hero* hero)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explosion.mp3");
    auto pos = bullet->getPosition();
    
    //spawn dirts
    auto dirt1 = ParticleSystemQuad::create("DirtExplosion1.plist");
    dirt1->setPositionType(ParticleSystem::PositionType::GROUPED);
    _effectLayer->addChild(dirt1);
    dirt1->setPosition(pos);
    dirt1->setTotalParticles(10);
    dirt1->setEmissionRate(99999999);
    dirt1->setAutoRemoveOnFinish(true);
    auto dirt2 = ParticleSystemQuad::create("DirtExplosion2.plist");
    dirt2->setPositionType(ParticleSystem::PositionType::GROUPED);
    _effectLayer->addChild(dirt2);
    dirt2->setPosition(pos);
    dirt2->setTotalParticles(7);
    dirt2->setEmissionRate(99999999);
    dirt2->setAutoRemoveOnFinish(true);
    auto dirt3 = ParticleSystemQuad::create("DirtExplosion3.plist");
    dirt3->setPositionType(ParticleSystem::PositionType::GROUPED);
    _effectLayer->addChild(dirt3);
    dirt3->setPosition(pos);
    dirt3->setTotalParticles(5);
    dirt3->setEmissionRate(99999999);
    dirt3->setAutoRemoveOnFinish(true);
    
    
    if(_playback)
    {
        rapidjson::Value object(rapidjson::kObjectType);
        rapidjson::Document::AllocatorType& allocator = _myturn.GetAllocator();
        object.AddMember("x", pos.x, allocator);
        object.AddMember("y", pos.y, allocator);
        object.AddMember("size", bullet->getConfig().expRadius, allocator);
        _myturn["explosions"].PushBack(object, allocator);
        
        rapidjson::StringBuffer strbuf;
        rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
        _myturn.Accept(writer);
        log("--\n%s\n--\n", strbuf.GetString());
    }
    
    _ex->setPosition(pos);
    _burn->setPosition(pos);

    _ex->setScaleX((float)bullet->getConfig().expRadius/64);
    _ex->setScaleY(_ex->getScaleX()*0.7);
    _burn->setScaleX(_ex->getScaleX()*1.05);
    _burn->setScaleY(_ex->getScaleY()*1.05);
    
    _ex->ManualDraw();
    _burn->ManualDraw();
    
    float exRad =bullet->getConfig().expRadius;
    float damage = bullet->getConfig().damage;
    bullet->charges--;
    if(bullet->charges <=0)
    {
            _following = nullptr;
        bullet->runAction(RemoveSelf::create());
    }
    if(bullet->bounces)
    {
        bullet->setLastPos(Point((bullet->getPosition().x- bullet->getLastPos().x)*0.3 + bullet->getPosition().x, (bullet->getPosition().y- bullet->getLastPos().y) +bullet->getPosition().y));
    }
    bool is_player1_hurt = false;
    bool is_player2_hurt = false;
    
    //check to see if player got caught in the blast
    for(Node* player : _PlayerLayer->getChildren())
    {
        Hero *p = dynamic_cast<Hero*>(player);
        
        Point ppos(p->getPosition());
        float dist = ppos.getDistance(pos);
        if(dist< exRad + p->radius)
        {
            if(p == hero)
            {
                {
                    if(_playback)
                    {
                        if(p->getTag() == TAG_MYSELF)
                        {
                            _myturn["player1"]["hp"].SetInt(p->hurt(damage));
                        }
                        else
                        {
                            _myturn["player2"]["hp"].SetInt(p->hurt(damage));
                        }
                    }
                    else
                    {
                        p->hurt(damage);
                        if(getCurrentPlayer() == p)
                        {
                            GPGSChecker::getInstance()->checkMasochist();
                            is_player1_hurt = true;
                        }
                        else
                        {
                            GPGSChecker::getInstance()->chcekFirstBlood();
                            is_player2_hurt = true;
                        }
                    }
                    showBloodLossNum(p, damage);
                }
                
                p->airborn = true;
                
                float rad = (ppos-pos).getAngle();
                float pushForce = (exRad - dist)*0.05;
                pushForce = bullet->attract? -pushForce: pushForce;
                Point mid(ppos.x-pushForce*cosf(rad), ppos.y-pushForce*sinf(rad));
                log("b pos %f, %f | m pos %f, %f", (pos-ppos).x, (pos-ppos).y, (mid-ppos).x, (mid-ppos).y);
                p->setLastPos(mid);
            }
            else
            {
                p->airborn = true;
                
                float rad = (ppos-pos).getAngle();
                float pushForce = (exRad - dist) *0.05;
                pushForce = bullet->attract? -pushForce: pushForce;
                Point mid(ppos.x-pushForce*cosf(rad), ppos.y-pushForce*sinf(rad));
                log("b pos %f, %f | m pos %f, %f", (pos-ppos).x, (pos-ppos).y, (mid-ppos).x, (mid-ppos).y);
                p->setLastPos(mid);
                
                {
                    if(_playback)
                    {
                        if(p->getTag() == TAG_MYSELF)
                        {
                            _myturn["player1"]["hp"].SetInt(p->hurt(abs(damage*((float)(exRad + p->radius -dist)/(float)(exRad +p->radius)))));
                        }
                        else
                        {
                            _myturn["player2"]["hp"].SetInt(p->hurt(abs(damage*((float)(exRad + p->radius -dist)/(float)(exRad +p->radius)))));
                        }
                    }
                    else
                    {
                        p->hurt((damage*((float)(exRad + p->radius -dist)/(float)(exRad +p->radius))));
                        if(getCurrentPlayer() == p)
                        {
                            GPGSChecker::getInstance()->checkMasochist();
                            is_player1_hurt = true;
                        }
                        else
                        {
                            GPGSChecker::getInstance()->chcekFirstBlood();
                            is_player2_hurt = true;
                        }
                    }
                    showBloodLossNum(p, damage*((float)(exRad-dist)/(float)exRad));
                }
            }
            
            if (is_player1_hurt && is_player2_hurt) {
                GPGSChecker::getInstance()->checkCollateralDamage();
            }
        }
    }
}
void GameScene::playback(std::string json)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Giant Insectoid Battle.mp3");
    tempjson = json;
    _replay.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
    
    this->setWind(Point(_replay["windx"].GetDouble(),_replay["windy"].GetDouble()));
    _eventDispatcher->dispatchCustomEvent("wind", &_wind);
    log("Wind is : %f, %f", getWind().x, getWind().y);
    
    this->initPlayers();

    p2->setPosition(_replay["player2"]["posx"].GetDouble(),_replay["player2"]["posy"].GetDouble());
    p2->_wagonPoint->setRotation(_replay["player2"]["rot"].GetDouble());
    p2->setLastPos(p2->getPosition());
    p1->setPosition(_replay["player1"]["posx"].GetDouble(),_replay["player1"]["posy"].GetDouble());
    p1->_wagonPoint->setRotation(_replay["player1"]["rot"].GetDouble());
    p1->setLastPos(p1->getPosition());
    
    p1->setLife(_replay["player1"]["hp"].GetInt());
    p2->setLife(_replay["player2"]["hp"].GetInt());
    
    p1->setName(_replay["player1"]["name"].GetString());
    p2->setName(_replay["player2"]["name"].GetString());
    
    if(!strcmp(_replay["player1"]["facing"].GetString(), "left"))
    {
        p1->flipLeft();
    }
    else
    {
        p1->flipRight();
    }
    if(!strcmp(_replay["player2"]["facing"].GetString(), "left"))
    {
        p2->flipLeft();
    }
    else
    {
        p2->flipRight();
    }
    
    _playback = true;
    _following = dynamic_cast<Node*>(getCurrentPlayer());
    if(_replay["turn"].GetInt() == 1)
    {
        //player 1 chose a wagon, player2 just chose a wagon and entered the game
        //now its player 2's turn to move
        p1->airborn = true;
        p2->airborn = true;
        _waitToClear = true;
        //        over =true;
        return;
    }
    else if(_replay["turn"].GetInt() ==2)
    {
        p1->airborn = true;
        p2->airborn = true;
    }
    _tick = 0;
    _playback = true;
    getCurrentPlayer()->showAimer();
    getCurrentPlayer()->showTurnSymbol();
    getCurrentPlayer()->setSideSymbol(false);
    _eventDispatcher->dispatchCustomEvent("touch off");
    _eventDispatcher->dispatchCustomEvent("enemy's turn");
    //copy all explosions to my turn
    if(_replay.HasMember("explosions") && _replay["explosions"].Size())
    {
        //recreate those explosions
        _level->getRT()->onBegin();
        for(int i = 0; i < _replay["explosions"].Size(); i++)
        {
            auto &jsonPos =_replay["explosions"][i];
            Point pos(jsonPos["x"].GetDouble(), jsonPos["y"].GetDouble());
            _ex->setPosition(pos);
            _ex->setScaleX(jsonPos["size"].GetDouble()/64);
            _ex->setScaleY(_ex->getScaleX()*0.7);
            _burn->setScaleX(_ex->getScaleX()*1.05);
            _burn->setScaleY(_ex->getScaleY()*1.05);
            _ex->ManualDraw();
            _burn->setPosition(pos);
            _burn->ManualDraw();
            
        }
        _level->getRT()->onEnd();
    }
    
    //get enemy name
    _eventDispatcher->dispatchCustomEvent("enemy", (void*)(getCurrentPlayer()->_nameLabel->getString().c_str()));

    // get tick sum
    rapidjson::Value &array = _replay["actions"];
    if(array.IsArray())
    {
        for(int i=0; i < array.Size(); i++)
        {
            if(i == array.Size()-1){
                _eventDispatcher->dispatchCustomEvent("tickSum", (void*)array[i]["tick"].GetInt());
            }
        }
    }
    
}

void GameScene::update(float dt)
{
    bool everythingSleep = true;
    
	if(_playback)
    {
        getCurrentPlayer()->aim->hideCrossHair();
        rapidjson::Value &array = _replay["actions"];
        if(array.IsArray())
        {
            for(int i=0; i < array.Size(); i++)
            {
                if(_tick == array[i]["tick"].GetInt())
                {
                    //execute that action
                    if(!std::strcmp(array[i]["action"].GetString(), "start angle")){
                        std::string actionName = array[i]["action"].GetString();
                        for(int j = i; j < array.Size(); j++){
                            if(!std::strcmp(array[j]["action"].GetString(), "end angle")){
                                int durTick = array[j]["tick"].GetInt() - _tick;
                                int endAngle = array[j]["value"].GetInt();
                                int array[2] = {durTick,endAngle};
                                _eventDispatcher->dispatchCustomEvent(actionName,array);
                                break;
                            }
                        }
                        
                    } else {
                        _eventDispatcher->dispatchCustomEvent(array[i]["action"].GetString());
                    }
                    
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
        setPosition(cp+(tp-cp)*0.12);
    }
    
    
    
    //hack alert::switch GL target to the map
    //kmGLPushMatrix();
    _level->getRT()->onBegin();
    //now the target is the render texture, we can begin reading the bullet and player pixels
    
    auto aabb2 = _bulletLayer->getBoundingBox();
    
    //Point offset(aabb2.origin+getPosition());
    aabb2.origin = Point::ZERO;
    auto bulletBoundary = Rect(0, 0, aabb2.size.width, aabb2.size.height+2000);
    for(Node* bullet : _bulletLayer->getChildren())
    {
        everythingSleep = false;
        Bullet *b = dynamic_cast<Bullet*>(bullet);
        auto pos = b->getPosition();
        bool coll = false;
        int bulletRadius =b->getConfig().radius;
        
        //check bounding box to see if the bullet is in the world
        auto aabb1 = b->getBoundingBox();
        if(bulletBoundary.containsPoint(Point(aabb1.origin.x,aabb1.origin.y)) &&
           bulletBoundary.containsPoint(Point(aabb1.origin.x+aabb1.size.height, aabb1.origin.y+aabb1.size.width)))
        {
            //move this
            b->setPosition(pos+(pos-b->getLastPos())+getGravity()+getWind());
            b->setLastPos(pos);
            b->setRotation(-CC_RADIANS_TO_DEGREES((b->getLastPos()-b->getPosition()).getAngle()));
            
            //check if we are colliding with a player
            if(!b->attract)//if bullet is attract mode, then don't collide with player
            {
                for(Node *player : _PlayerLayer->getChildren())
                {
                    Hero* p = dynamic_cast<Hero*>(player);
                    if(b->getPosition().getDistance(p->getPosition()) < bulletRadius + p->radius)
                    {
                        explode(b, p);
                        coll = true;
                        log("collide with player");
                        break;
                    }
                }
            }
            //if we didnt collide with player, then we have to check for the terrain
            if(aabb2.containsPoint(Point(aabb1.origin.x,aabb1.origin.y)) &&
               aabb2.containsPoint(Point(aabb1.origin.x+aabb1.size.height, aabb1.origin.y+aabb1.size.width)) && !coll)
            {
                Point pos(b->getPosition());
                //init a buffer size big enough to hold a square that can contain a circle with specified radius
                int bufferSize = pow(bulletRadius*2, 2);
                Color4B *buffer = (Color4B*)malloc(sizeof(Color4B)*bufferSize);
                glReadPixels(pos.x-bulletRadius, pos.y-bulletRadius, bulletRadius*2, bulletRadius*2, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
                for(int i = 0; i < bufferSize; i++)
                {
                    //for accuracy, we only want the pixels in the circle
                    if(buffer[i].a>0 && Helper::isInCircle(i, bulletRadius) && !(pos.y > aabb2.size.height-100))
                    {
                        explode(b,nullptr);
                        coll = true;
                        break;
                    }
                }
                free(buffer);
            }
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
            Hero* p = dynamic_cast<Hero*>(player);
            
            auto paabb = p->getBoundingBox();
            if(aabb2.intersectsRect(paabb))
            {
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
                        if(buffer[i].a > 0.5 && Helper::isInCircle(i, radius))// if pixel is not transparent, and its in the circle
                        {
                            float an = Helper::getAngleFromIndex(i, radius);
                            if(an != -999)
                            {
                                angleTotal+=an;
                                angleCount++;
                            }

                        }
                    }
                    free(buffer);
                    //check how many collision points
                    if(angleCount)
                    {
                        p->airborn = false; //we are no longer airborn
                        //set angle to average
                        float deg =CC_RADIANS_TO_DEGREES(angleTotal/angleCount);
                        if(abs(deg) > 60)//TODO: each vehicle has a climbing angle limit
                        {
                            deg = (deg>0)? 60: -60;
                            p->_wagonPoint->setRotation(deg);
                            
                            p->airborn = true;
                        }
                        else
                        {
                            p->_wagonPoint->setRotation(deg);
                            
                        }
                        if(angleCount > 8)
                        {
                            //we are colliding with too many pixels
                            float pushForce = 0.03 * angleCount;
                            Point mid(pos.x-pushForce*sinf(angleTotal/angleCount), pos.y-pushForce*cosf(angleTotal/angleCount));
                            p->setLastPos(mid);
                            p->needFix = true;
                        }
                        else
                        {
                            p->needFix = false;
                        }
                    }
                    else{
                        p->airborn = true;
                    }
                    
                }
                if(p->moveDelta.x && !p->airborn)
                {
                    p->setPosition(p->getPosition()+p->moveDelta);
                    p->setLastPos(p->getLastPos()+p->moveDelta);
                    if(p->moveDelta.x>0)
                    {
                        //TODO: replace with proper flip code
                        //p->setScaleX(1);
                        p->flipRight();
                    }
                    else{
                        //p->_heroConfig.isfacetoright = false;
                        //p->setScaleX(-1);
                        p->flipLeft();
                    }
                }
            }
            else
            {
                //player fell out side, please die
                log("player went out side");
                if (!_isWentOut) {
                    _isWentOut = true;
                    p->hurt(p->_lasthp);
                    p->setVisible(false);
                    if(getCurrentPlayer() != p && !_playback)
                    {
                        GPGSChecker::getInstance()->checkBanger();
                    }
                }
            }
        }
    }
    
    
    
    _level->getRT()->onEnd();
    _tick++;
    
    //    log("everythingSleep is %d",everythingSleep);
    if(_waitToClear && everythingSleep)
    {
        if(_playback && !over)
        {
            _eventDispatcher->dispatchCustomEvent("touch on");
            _eventDispatcher->dispatchCustomEvent("my turn");
            _waitToClear = false;
            _tick = 0;
            log("play back finished");
            //need to delete actions
            _myturn["actions"].Clear();
            runAction(Sequence::create(
                                       DelayTime::create(1),
                                       CallFunc::create([this](){
                _playback=false;
                _following = getCurrentPlayer();
                p1->hideAimer();
                p1->hideTurnSymbol();
                p2->hideAimer();
                p2->hideTurnSymbol();
                p1->aim->hideCrossHair();
                p2->aim->hideCrossHair();
                getCurrentPlayer()->showAimer();
                getCurrentPlayer()->showTurnSymbol();
                getCurrentPlayer()->aim->showCrossHair();
                getCurrentPlayer()->setSideSymbol(true);
                CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
                CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Celestial Motive m.mp3");
            }),
                                       nullptr
                                       ));
        }
        else if(!over){
            saveMatchData(false, false);
        }
    }
    
}

void GameScene::playerdead(EventCustom* event)
{
    auto hero = (Hero*)event->getUserData();
    if (!_playback) {
        if (hero == getCurrentPlayer()) {
            saveMatchData(false, true);
            GPGSChecker::getInstance()->checkSuicidal();
        }
        else
        {
            saveMatchData(true, false);
            GPGSChecker::getInstance()->checkFirstVictory();
            GPGSChecker::getInstance()->sumbitNewVictory();
        }
    }
    else
    {
        if (hero == getCurrentPlayer()) {
            showWinOrLose(true);
            over = true;
            GPGSManager::ConfirmPendingCompletion();
            runAction(Sequence::create(DelayTime::create(5.0f),
                                       CallFunc::create([]()
                                                        {
                                                            Director::getInstance()->replaceScene(MainScreenScene::createScene(true));
                                                        }), nullptr));
        }
        else
        {
            showWinOrLose(false);
            over = true;
            GPGSManager::ConfirmPendingCompletion();
            runAction(Sequence::create(DelayTime::create(5.0f),
                                       CallFunc::create([]()
                                                        {
                                                            Director::getInstance()->replaceScene(MainScreenScene::createScene(true));
                                                        }), nullptr));        }
    }
}

void GameScene::saveMatchData(bool win, bool lose)
{
    over = true;
    _myturn["turn"].SetInt(_myturn["turn"].GetInt()+1);
    printMyTurn();
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    _myturn.Accept(writer);
    
    g_gameConfig.match_string = strbuf.GetString();
    log("setup_player2_matchdata...%s",g_gameConfig.match_string.c_str());
    
    float delaytotaketurntime = 2.0f;
    if (win) {
        showWinOrLose(true);
        delaytotaketurntime = 5.0f;
    }
    else if(lose)
    {
        showWinOrLose(false);
        delaytotaketurntime = 5.0f;
    }
    
//    auto notouchlayer = NoTouchLayer::create();
//    notouchlayer->setTag(NOTOUCHTAG);
//    Director::getInstance()->getRunningScene()->addChild(notouchlayer,100);
    
//    auto popwindow = PopWindow::create();
//    Director::getInstance()->getRunningScene()->addChild(popwindow,999);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    runAction(Sequence::create(CallFunc::create([=]()
                                                {
//                                                    ((GameUI*)(getParent()->getChildByTag(76)))->_controlBoard->runAction(EaseBackIn::create(MoveTo::create(0.5, Point(g_visibleRect.visibleWidth/2 + g_visibleRect.visibleOriginX, -120))));
//                                                    getCurrentPlayer()->hideAimer();
                                                }),
                               DelayTime::create(delaytotaketurntime),
                               CallFunc::create([=](){GPGSManager::TakeTurn(win, lose);}),
                               nullptr));
#endif
}

//void GameScene::returntoMenu()
//{
//    log("call...return to menu");
//    scheduleOnce(schedule_selector(GameScene::entertoMenu), 1.0f);
//}
//
//void GameScene::entertoMenu(float dt)
//{
//    log("call...entertomenu");
//    log("call...entertomenu....22222222222");
//    auto scene = MainScreenScene::createScene(true);
//    cocos2d::Director::getInstance()->replaceScene(scene);
//}

void GameScene::showBloodLossNum(Hero* hero, int num)
{
    if(!_isBloodLabelShowing)
    {
        _isBloodLabelShowing = true;
        TTFConfig turnTTFConfig;
        turnTTFConfig.outlineSize = 7;
        turnTTFConfig.fontSize = 50;
        turnTTFConfig.fontFilePath = "britanic bold.ttf";
        bloodLossLabel = Label::createWithTTF(turnTTFConfig, Value(num).asString(), TextHAlignment::CENTER, 400);
        bloodLossLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
        bloodLossLabel->setSpacing(-5);
        bloodLossLabel->enableOutline(Color4B::BLACK);
        bloodLossLabel->setOpacity(50);
        bloodLossLabel->setScale(-10.01f);
        hero->addChild(bloodLossLabel);
        bloodLossLabel->setPosition(0, 70);
        bloodLossLabel->setRotation(CCRANDOM_MINUS1_1()*20);
        
        auto targetScale = 1.0f;
        if(num > 200)
        {
            bloodLossLabel->setColor(Color3B::RED);
        }
        else if(num > 100)
        {
            bloodLossLabel->setColor(Color3B(250,121,65));
            targetScale = 0.75f;
        }
        else
        {
            bloodLossLabel->setColor(Color3B(250,191,65));
            targetScale = 0.5f;
        }
        
        bloodLossLabel->runAction(FadeIn::create(0.3));
        bloodLossLabel->runAction(Sequence::create(
                                          EaseElasticOut::create(ScaleTo::create(1.3f,targetScale)),
                                          DelayTime::create(2.0f),
                                          FadeOut::create(0.5f),
                                          RemoveSelf::create(),
                                          CallFunc::create([=](){_isBloodLabelShowing = false;}),
                                          nullptr));
        bloodLossLabel->runAction(MoveBy::create(3.8, Point(0, 50)));
        bloodLossLabel->runAction(RotateBy::create(3.8, CCRANDOM_MINUS1_1()*40));
    }
    else
    {
        int old_num = Value(bloodLossLabel->getString()).asInt();
        int new_num = old_num + num;
        if(new_num > 200)
        {
            bloodLossLabel->setColor(Color3B::RED);
        }
        else if(new_num > 100)
        {
            bloodLossLabel->setColor(Color3B(250,121,65));
        }
        else
        {
            bloodLossLabel->setColor(Color3B(250,191,65));
        }
        bloodLossLabel->setString(Value(new_num).asString());
    }
}

void GameScene::showWinOrLose(bool isWin)
{
    if(isWin)
    {
        auto node = Node::create();
        auto you = Sprite::create("youwin_1.png");
        you->setPosition(Point(-120,0));
        you->setAnchorPoint(Point::ANCHOR_MIDDLE);
        
        auto win = Sprite::create("youwin_2.png");
        win->setPosition(Point(120,0));
        win->setAnchorPoint(Point::ANCHOR_MIDDLE);
        
        node->addChild(you);
        node->addChild(win);
        
        node->setScale(0.0001f);
        
        node->runAction(Sequence::create(Spawn::create(FadeIn::create(0.5f),
                                                       EaseBackOut::create(ScaleTo::create(1.0f,1.0f)),
                                                       nullptr),
                                         DelayTime::create(3.0f),
                                         FadeOut::create(0.5f),
                                         nullptr));
        node->setPosition(g_visibleRect.center);
        Director::getInstance()->getRunningScene()->addChild(node,99);
    }
    else
    {
        auto node = Node::create();
        auto you = Sprite::create("youlose_1.png");
        you->setPosition(Point(-120,0));
        you->setAnchorPoint(Point::ANCHOR_MIDDLE);
        
        auto lose = Sprite::create("youlose_2.png");
        lose->setPosition(Point(120,0));
        lose->setAnchorPoint(Point::ANCHOR_MIDDLE);
        
        node->addChild(you);
        node->addChild(lose);
        
        node->runAction(Sequence::create(Spawn::create(FadeIn::create(0.5f),
                                                       EaseBackOut::create(MoveBy::create(1.0f,Point(0, -20))),
                                                       nullptr),
                                         DelayTime::create(3.0f),
                                         FadeOut::create(0.5f),
                                         nullptr));
        node->setPosition(g_visibleRect.center);
        Director::getInstance()->getRunningScene()->addChild(node,99);

    }
}

void GameScene::showConnectingPopWindow()
{
    scheduleOnce(schedule_selector(GameScene::showConnectingPopWindowWithDelay), transSceneDelayTime/2);
}
void GameScene::showConnectingPopWindowWithDelay(float dt)
{
    auto popwindow = PopWindow::create();
    Director::getInstance()->getRunningScene()->addChild(popwindow,100);
}

void GameScene::showReturnBtn()
{

    returnMenu->setVisible(true);
}
