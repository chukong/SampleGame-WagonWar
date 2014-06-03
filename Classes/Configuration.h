#pragma once

#include <string>
#include <vector>
#include "cocos2d.h"

enum TurnType
{
    FIRSR_TURN,
    SECOND_TURN
};


typedef struct{
    //cnm
    const std::string cnm_hit = "cnm_hit";
    const std::string cnm_idle = "cnm_idle";
    const std::string cnm_move = "cnm_move";
    const std::string cnm_shoot = "cnm_shoot";
    
}gameAnimation;
extern gameAnimation g_gameanimations;

const int WAGONSELECTTAG = 777;
const int MAINLAYERTAG = 888;
const int NOTOUCHTAG = 999;

typedef struct wagonConfig
{
    int hp;
    float speed;
    int attack;
    int upperlimit;
    int lowerlimit;
    int expsize;
}WagonConfig;

const WagonConfig g_wagonConfig[4]=
{
    {1000,0.4,250,0,-90,45},//horsey
    {1000,0.4,250,0,-90,45},//tank
    {1000,0.4,250,0,-90,45},//rock
    {1000,0.4,250,0,-90,45},//mech
};

class GameConfig
{
public:
    const static std::string defaultFontName;
    const static int defaultFontSize;

    void lazyInit();
    
    void initAnimation();

    void setMusicVolume(float volume);
    void setSFXVolume(float volume);

    void saveConfig();

    cocos2d::Animate* getAnimate(const std::string animationame);

    float musicVolume;
    float sfxVolume;
    
    std::vector<uint8_t> match_data;
    std::string match_string;
        
};

extern GameConfig g_gameConfig;

//auto xxxanimation=AnimationCache::getInstance()->getAnimationByName(g_gameconfig._animation.cnm_hit);