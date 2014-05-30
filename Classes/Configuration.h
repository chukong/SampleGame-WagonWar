#pragma once

#include <string>
#include <vector>

enum TurnType
{
    FIRSR_TURN,
    SECOND_TURN
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

    float musicVolume;
    float sfxVolume;
    
    std::vector<uint8_t> match_data;
    std::string match_string;
};

extern GameConfig g_gameConfig;

