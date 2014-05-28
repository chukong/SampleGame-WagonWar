#pragma once

#include <string>

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
};

extern GameConfig g_gameConfig;

