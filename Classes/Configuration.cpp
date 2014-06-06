#include "Configuration.h"
#include "VisibleRect.h"
#include "SimpleAudioEngine.h"

GameConfig g_gameConfig;
gameAnimation g_gameAnimation;

const std::string GameConfig::defaultFontName = "Arial";
const int GameConfig::defaultFontSize = 30;

USING_NS_CC;

void GameConfig::lazyInit()
{
    g_visibleRect.lazyInit();
    match_data.clear();
    
    initAnimation();
}


void GameConfig::saveConfig()
{
    
}

void GameConfig::setMusicVolume(float volume)
{

}

void GameConfig::setSFXVolume(float volume)
{

}

void GameConfig::initAnimation()
{
    char text[30];
    
    // cnm
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cnm.plist", "cnm.png");
    auto cnm_hit = Animation::create();
	cnm_hit->setDelayPerUnit(0.14f);
    for (int i=1; i<=5; ++i) {
        sprintf(text, "cnm_hit0%d.png",i);
        cnm_hit->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
	AnimationCache::getInstance()->addAnimation(cnm_hit,g_gameAnimation.cnm_hit);
    
    auto cnm_idle = Animation::create();
    cnm_idle->setDelayPerUnit(0.14f);
    for (int i=1; i<=6 ; i++) {
        sprintf(text, "cnm_idle0%d.png",i);
        cnm_idle->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
    AnimationCache::getInstance()->addAnimation(cnm_idle,g_gameAnimation.cnm_idle);
    
    auto cnm_move = Animation::create();
    cnm_move->setDelayPerUnit(0.14f);
    for (int i=1; i<=8 ; i++) {
        sprintf(text, "cnm_move0%d.png",i);
        cnm_move->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
    AnimationCache::getInstance()->addAnimation(cnm_move,g_gameAnimation.cnm_move);
    
    auto cnm_shoot = Animation::create();
    cnm_shoot->setDelayPerUnit(0.14f);
    for (int i=1; i<=10 ; i++) {
        if(i >= 10){
            sprintf(text, "cnm_shoot%d.png",i);
        } else {
            sprintf(text, "cnm_shoot0%d.png",i);
        }
        
        cnm_shoot->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
    AnimationCache::getInstance()->addAnimation(cnm_shoot,g_gameAnimation.cnm_shoot);
    
    // tank
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tank.plist", "tank.png");
    auto tank_hit = Animation::create();
	tank_hit->setDelayPerUnit(0.14f);
    for (int i=1; i<=2; ++i) {
        sprintf(text, "tankhit0%d.png",i);
        tank_hit->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
	AnimationCache::getInstance()->addAnimation(tank_hit,g_gameAnimation.tank_hit);
    
    auto tank_idle = Animation::create();
    tank_idle->setDelayPerUnit(0.14f);
    for (int i=1; i<=5 ; i++) {
        sprintf(text, "tankidle0%d.png",i);
        tank_idle->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
    AnimationCache::getInstance()->addAnimation(tank_idle,g_gameAnimation.tank_idle);
    
    auto tank_move = Animation::create();
    tank_move->setDelayPerUnit(0.14f);
    for (int i=1; i<=4 ; i++) {
        sprintf(text, "tankmove0%d.png",i);
        tank_move->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }

    AnimationCache::getInstance()->addAnimation(tank_move,g_gameAnimation.tank_move);
    
    auto tank_shoot = Animation::create();
    tank_shoot->setDelayPerUnit(0.14f);
    for (int i=1; i<=4 ; i++) {
        sprintf(text, "tankshoot0%d.png",i);
        tank_shoot->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
    AnimationCache::getInstance()->addAnimation(tank_shoot,g_gameAnimation.tank_shoot);
    
    // rock
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("rock.plist", "rock.png");
    auto rock_hit = Animation::create();
	rock_hit->setDelayPerUnit(0.14f);
    for (int i=1; i<=5; ++i) {
        sprintf(text, "rockhit0%d.png",i);
        rock_hit->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
	AnimationCache::getInstance()->addAnimation(rock_hit,g_gameAnimation.rock_hit);
    
    auto rock_idle = Animation::create();
    rock_idle->setDelayPerUnit(0.14f);
    for (int i=1; i<=5 ; i++) {
        sprintf(text, "rockidle0%d.png",i);
        rock_idle->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
    AnimationCache::getInstance()->addAnimation(rock_idle,g_gameAnimation.rock_idle);
    
    auto rock_move = Animation::create();
    rock_move->setDelayPerUnit(0.14f);
    for (int i=1; i<=8 ; i++) {
        sprintf(text, "rockmove0%d.png",i);
        rock_move->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
    AnimationCache::getInstance()->addAnimation(rock_move,g_gameAnimation.rock_move);
    
    auto rock_shoot = Animation::create();
    rock_shoot->setDelayPerUnit(0.14f);
    for (int i=1; i<=10 ; i++) {
        if(i >= 10){
            sprintf(text, "rockshoot%d.png",i);
        } else {
            sprintf(text, "rockshoot0%d.png",i);
        }
        
        rock_shoot->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
    AnimationCache::getInstance()->addAnimation(rock_shoot,g_gameAnimation.rock_shoot);
    
    //mech
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("mech.plist", "mech.png");
    auto mech_hit = Animation::create();
	mech_hit->setDelayPerUnit(0.14f);
    for (int i=1; i<=5; ++i) {
        sprintf(text, "mechhit0%d.png",i);
        mech_hit->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
	AnimationCache::getInstance()->addAnimation(mech_hit,g_gameAnimation.mech_hit);

    auto mechgun_hit = Animation::create();
	mechgun_hit->setDelayPerUnit(0.14f);
    for (int i=1; i<=5; ++i) {
        sprintf(text, "mechgunhit0%d.png",i);
        mechgun_hit->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
	AnimationCache::getInstance()->addAnimation(mechgun_hit,g_gameAnimation.mechgun_hit);
    
    auto mech_idle = Animation::create();
    mech_idle->setDelayPerUnit(0.14f);
    for (int i=1; i<=5 ; i++) {
        sprintf(text, "mechidle0%d.png",i);
        mech_idle->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
    AnimationCache::getInstance()->addAnimation(mech_idle,g_gameAnimation.mech_idle);
    
    auto mechgun_idle = Animation::create();
    mechgun_idle->setDelayPerUnit(0.14f);
    for (int i=1; i<=5 ; i++) {
        sprintf(text, "mechgunidle0%d.png",i);
        mechgun_idle->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
    AnimationCache::getInstance()->addAnimation(mechgun_idle,g_gameAnimation.mechgun_idle);
    
    auto mech_move = Animation::create();
    mech_move->setDelayPerUnit(0.14f);
    for (int i=1; i<=7 ; i++) {
        sprintf(text, "mechmove0%d.png",i);
        mech_move->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
    AnimationCache::getInstance()->addAnimation(mech_move,g_gameAnimation.mech_move);

    auto mechgun_move = Animation::create();
    mechgun_move->setDelayPerUnit(0.14f);
    for (int i=1; i<=7 ; i++) {
        sprintf(text, "mechgunmove0%d.png",i);
        mechgun_move->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
    AnimationCache::getInstance()->addAnimation(mechgun_move,g_gameAnimation.mechgun_move);
    
    auto mech_shoot = Animation::create();
    mech_shoot->setDelayPerUnit(0.14f);
    for (int i=1; i<=10 ; i++) {
        if(i >= 10){
            sprintf(text, "mechshoot%d.png",i);
        } else {
            sprintf(text, "mechshoot0%d.png",i);
        }
        
        mech_shoot->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
    AnimationCache::getInstance()->addAnimation(mech_shoot,g_gameAnimation.mech_shoot);
    
    auto mechgun_shoot = Animation::create();
    mechgun_shoot->setDelayPerUnit(0.14f);
    for (int i=1; i<=10 ; i++) {
        if(i >= 10){
            sprintf(text, "mechgunshoot%d.png",i);
        } else {
            sprintf(text, "mechgunshoot0%d.png",i);
        }
        
        mechgun_shoot->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
    AnimationCache::getInstance()->addAnimation(mechgun_shoot,g_gameAnimation.mechgun_shoot);
}

Animate* GameConfig::getAnimate(const std::string animationame)
{
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(animationame);
    auto animate = Animate::create(animation);
    return animate;
}