#pragma once

#include <string>
#include <vector>

enum TurnType
{
    FIRSR_TURN,
    SECOND_TURN
};

const int WAGONSELECTTAG = 777;
const int MAINLAYERTAG = 888;
const int NOTOUCHTAG = 999;


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

