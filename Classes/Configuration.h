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

const int NOTOUCHTAG = 999;

typedef struct wagonConfig
{
    int hp;
    float speed;
    int attack;
    int upperlimit;
    int lowerlimit;
    int expsize;
    int offsetx;
    int offsety;
    float anchorx;
    float anchory;
    float gunx;
    float guny;
}WagonConfig;

const WagonConfig g_wagonConfig[4]=
{
    {600,0.4,250,-60,0,45,20,69,0.5,0.24731182795699, -45,30},//horsey
    {600,0.4,250,-40,-5,45,13,62,0.52,0.26744186046512, -48,40},//tank
    {600,0.4,250,-90,-30,45,18,66,0.54477611940299,0.21951219512195, -59, 48},//rock
    {600,0.4,250,-75,-15,45,12,69,0.5,0.25, -75, 48},//mech
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
    
    std::string player1Name;
    std::string player2Name;
    
    bool isInGame;
};

extern GameConfig g_gameConfig;
extern gameAnimation g_gameAnimation;
//auto xxxanimation=AnimationCache::getInstance()->getAnimationByName(g_gameconfig._animation.cnm_hit);