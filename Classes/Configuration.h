#pragma once

#include <string>
#include <vector>

#define FIRSR_TURN 1;
#define SECOND_TURN 2;

class GameConfig
{
public:
    const static std::string defaultFontName;
    const static int defaultFontSize;

    void lazyInit();

    void setMusicVolume(float volume);
    void setSFXVolume(float volume);

    void saveConfig();

    float musicVolume;
    float sfxVolume;
    
    std::vector<uint8_t> match_data;
    std::string match_string;
};

extern GameConfig g_gameConfig;

