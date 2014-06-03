#pragma once

#include <string>
#include <vector>
#include "cocos2d.h"

enum TurnType
{
    FIRST_TURN,
    SECOND_TURN
};


typedef struct{
    
    //cnm
    const std::string cnm_hit = "cnm_hit";
    const std::string cnm_idle = "cnm_idle";
    const std::string cnm_move = "cnm_move";
    const std::string cnm_shoot = "cnm_shoot";
    
    //tank
    const std::string tank_hit = "tank_hit";
    const std::string tank_idle = "tank_idle";
    const std::string tank_move = "tank_move";
    const std::string tank_shoot = "tank_shoot";
    
    //rock
    const std::string rock_hit = "rock_hit";
    const std::string rock_idle = "rock_idle";
    const std::string rock_move = "rock_move";
    const std::string rock_shoot = "rock_shoot";
    
    //mech
    const std::string mech_hit = "mech_hit";
    const std::string mech_idle = "mech_idle";
    const std::string mech_move = "mech_move";
    const std::string mech_shoot = "mech_shoot";
    const std::string mechgun_hit = "mechgun_hit";
    const std::string mechgun_idle = "mechgun_idle";
    const std::string mechgun_move = "mechgun_move";
    const std::string mechgun_shoot = "mechgun_shoot";
    
}gameAnimation;

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
extern gameAnimation g_gameAnimation;
//auto xxxanimation=AnimationCache::getInstance()->getAnimationByName(g_gameconfig._animation.cnm_hit);