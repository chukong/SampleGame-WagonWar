#include "Configuration.h"
#include "VisibleRect.h"
#include "SimpleAudioEngine.h"

GameConfig g_gameConfig;
gameAnimation g_gameanimations;


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
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cnm.plist", "cnm.png");
    
    char text[30];
    auto cnm_hit=Animation::create();
	cnm_hit->setDelayPerUnit(0.1f);
    for (int i=1; i<6; ++i) {
        sprintf(text, "cnm_hit0%d.png",i);
        cnm_hit->addSpriteFrame(CCSpriteFrameCache::getInstance()->getSpriteFrameByName(text));
    }
	AnimationCache::getInstance()->addAnimation(cnm_hit, g_gameanimations.cnm_hit);
    
    
}

Animate* GameConfig::getAnimate(const std::string animationame)
{
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(animationame);
    auto animate = Animate::create(animation);
    return animate;
}