#include "Configuration.h"
#include "VisibleRect.h"
#include "SimpleAudioEngine.h"

GameConfig g_gameConfig;

const std::string GameConfig::defaultFontName = "Arial";
const int GameConfig::defaultFontSize = 30;

void GameConfig::lazyInit()
{
    g_visibleRect.lazyInit();
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