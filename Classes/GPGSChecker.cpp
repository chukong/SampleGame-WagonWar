//
//  GPGSChecker.cpp
//  TankMultiplayer
//
//  Created by Jacky on 14-6-7.
//
//


#include "GPGSChecker.h"

GPGSChecker* GPGSChecker::m_pInstance = nullptr;
GPGSChecker::CGarbo GPGSChecker::m_garbo;

GPGSChecker::GPGSChecker()
{
    if (isHaveSaveFile()) {
        _playerLocalConfig = {
            CCUserDefault::getInstance()->getBoolForKey("firstblood",false),
            CCUserDefault::getInstance()->getBoolForKey("firstvictory",false),
            CCUserDefault::getInstance()->getBoolForKey("sucidal",false),
            CCUserDefault::getInstance()->getBoolForKey("masochist",false),
            CCUserDefault::getInstance()->getBoolForKey("banger",false),
            CCUserDefault::getInstance()->getBoolForKey("collarteraldamage",false),
            CCUserDefault::getInstance()->getBoolForKey("invincible",false),
            CCUserDefault::getInstance()->getIntegerForKey("victorys",0)
        };
    }
}


bool GPGSChecker::isHaveSaveFile()
{
    if(!CCUserDefault::getInstance()->getBoolForKey("isHaveSaveFileXml"))
    {
        CCUserDefault::getInstance()->setBoolForKey("isHaveSaveFileXml", true);
        CCUserDefault::getInstance()->setBoolForKey("firstblood", false);
        CCUserDefault::getInstance()->setBoolForKey("firstvictory", false);
        CCUserDefault::getInstance()->setBoolForKey("sucidal", false);
        CCUserDefault::getInstance()->setBoolForKey("masochist", false);
        CCUserDefault::getInstance()->setBoolForKey("banger", false);
        CCUserDefault::getInstance()->setBoolForKey("collarteraldamage", false);
        CCUserDefault::getInstance()->setBoolForKey("invincible", false);
        CCUserDefault::getInstance()->setIntegerForKey("victorys",0);
        
        CCUserDefault::getInstance()->flush();
        
        _playerLocalConfig = {false, false, false, false, false, false, false, 0};
        
        return false;
    }
    else
    {
        return true;
    }
}

void GPGSChecker::chcekFirstBlood()
{
    if (!_playerLocalConfig.firstblood) {
        GPGSManager::UnlockAchievement("123");
        _playerLocalConfig.firstblood = true;
        CCUserDefault::getInstance()->setBoolForKey("firstblood", true);
        CCUserDefault::getInstance()->flush();
    }
}

void GPGSChecker::checkFirstVictory()
{
    if (!_playerLocalConfig.firstvictory) {
        GPGSManager::UnlockAchievement("123");
        _playerLocalConfig.firstvictory = true;
        CCUserDefault::getInstance()->setBoolForKey("firstvictory", true);
        CCUserDefault::getInstance()->flush();
    }
}

void GPGSChecker::checkSuicidal()
{
    if (!_playerLocalConfig.sucidal) {
        GPGSManager::UnlockAchievement("123");
        _playerLocalConfig.sucidal = true;
        CCUserDefault::getInstance()->setBoolForKey("sucidal", true);
        CCUserDefault::getInstance()->flush();
    }
}

void GPGSChecker::checkMasochist()
{
    if (!_playerLocalConfig.masochist) {
        GPGSManager::UnlockAchievement("123");
        _playerLocalConfig.masochist = true;
        CCUserDefault::getInstance()->setBoolForKey("masochist", true);
        CCUserDefault::getInstance()->flush();
    }
}

void GPGSChecker::checkBanger()
{
    if (!_playerLocalConfig.banger) {
        GPGSManager::UnlockAchievement("123");
        _playerLocalConfig.banger = true;
        CCUserDefault::getInstance()->setBoolForKey("banger", true);
        CCUserDefault::getInstance()->flush();
    }
}

void GPGSChecker::checkCollateralDamage()
{
    if (!_playerLocalConfig.collarteraldamage) {
        GPGSManager::UnlockAchievement("123");
        _playerLocalConfig.collarteraldamage = true;
        CCUserDefault::getInstance()->setBoolForKey("collarteraldamage", true);
        CCUserDefault::getInstance()->flush();
    }
}

void GPGSChecker::checkInvincible()
{
    if (!_playerLocalConfig.invincible) {
        GPGSManager::UnlockAchievement("123");
        _playerLocalConfig.invincible = true;
        CCUserDefault::getInstance()->setBoolForKey("invincible", true);
        CCUserDefault::getInstance()->flush();
    }
}

void GPGSChecker::sumbitNewVictory()
{
    _playerLocalConfig.victorys++;
    GPGSManager::SubmitHighScore("345", _playerLocalConfig.victorys);
    CCUserDefault::getInstance()->setIntegerForKey("victorys", _playerLocalConfig.victorys);
    CCUserDefault::getInstance()->flush();
}
