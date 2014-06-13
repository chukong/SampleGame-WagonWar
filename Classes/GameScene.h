//
//  GameScene.h
//  TankMultiplayer
//
//  Created by WuHao on 14-5-13.
//
//

#ifndef __TankMultiplayer__GameScene__
#define __TankMultiplayer__GameScene__

#include "cocos2d.h"
#include "Level.h"
#include "bullet.h"
#include "Helper.h"
#include "Hero.h"
#include "json/rapidjson.h"
#include "json/document.h"

const int TAG_MYSELF = 502;
const int TAG_OTHER = 503;

class GameScene : public cocos2d::ParallaxNode
{
public:
    static cocos2d::Scene* createScene();
//    static GameScene* create();
    
    CREATE_FUNC(GameScene);
    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void filter();
    
    void update(float dt);
    
    cocos2d::Point getActualPos(cocos2d::Touch * touch);
    cocos2d::Size getMovableSize();
    
    CC_SYNTHESIZE(Level*, _level, Level);
    CC_SYNTHESIZE(cocos2d::Layer*, _PlayerLayer, PlayerLayer);
    CC_SYNTHESIZE(cocos2d::Layer*, _bulletLayer, BulletLayer);
    CC_SYNTHESIZE(cocos2d::Layer*, _effectLayer, EffectLayer);
    CC_SYNTHESIZE(cocos2d::Point, _wind, Wind);
    CC_SYNTHESIZE(cocos2d::Point, _gravity, Gravity);
    void initExplosionMasks();
    void initListeners();
    void initPlayers();
    void startShoot();
    void endShoot();
    void startAngle(cocos2d::EventCustom* event);
    void endAngle(cocos2d::EventCustom* event);
    void playerdead(cocos2d::EventCustom* event);
    Hero *getCurrentPlayer();
    Bullet* addBullet(BulletTypes type, cocos2d::Point pos, cocos2d::Point vector);
    cocos2d::Point offset;
    
    //virtual void draw(cocos2d::Renderer* renderer, const kmMat4 &transform, bool transformUpdated);
    void explode(Bullet* bullet, Hero* hero);
    
    void movePlayer(float x);
    virtual void onEnter();
    void randomWind();
    void playback(std::string json);
    void saveMatchData(bool win, bool lose);
    Hero* p1;
    Hero* p2;
    
//    void returntoMenu();
//    void entertoMenu(float dt);
    
    void showBloodLossNum(Hero* hero, int num);
    void showWinOrLose(bool isWin);
    
    void showConnectingPopWindow();
    void showConnectingPopWindowWithDelay(float dt);
    
    bool _playback;
    
    std::string tempjson;
    
    bool _isFirstAction;
    
    bool over;

protected:
    void printMyTurn();
    rapidjson::Document _replay;
    rapidjson::Document _myturn;
    void buildMyTurn();
    int _tick;
    int _tickPre;
    bool _waitToClear;
    cocos2d::Node* _following;
    cocos2d::Point _offset;
    //timeval _now;
    cocos2d::Size _movableSize;
    void _movePlayer(float x);
    GameScene():_waitToClear(false),_playback(false),_click(false),_steps(2),_following(nullptr),
    _tick(0),_replay(nullptr),_myturn(nullptr),win(false), lost(false), over(false),_isFirstAction(false),_isBloodLabelShowing(false){g_gameConfig.isInGame = true;};
    ~GameScene(){g_gameConfig.isInGame = false;};
    bool _click;
    int _steps;
    cocos2d::Sprite* _ex;
    cocos2d::Sprite* _burn;
    
    bool win;
    bool lost;
    
    bool _isWentOut;
    float incre;
    bool _isBloodLabelShowing;
    cocos2d::Label* bloodLossLabel;
    cocos2d::EventListenerTouchOneByOne* listenertouch;
    
    void showReturnBtn();
    cocos2d::Menu* returnMenu;
};

#endif /* defined(__TankMultiplayer__GameScene__) */
