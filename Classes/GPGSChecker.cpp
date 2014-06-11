//
//  GPGSChecker.cpp
//  TankMultiplayer
//
//  Created by Jacky on 14-6-7.
//
//


#include "GPGSChecker.h"

#define ACH_FIRST_BLOOD "CgkIt8qQwKsFEAIQAg"
#define ACH_TASTE_OF_VICTORY "CgkIt8qQwKsFEAIQAw"
#define ACH_SUICIDAL "CgkIt8qQwKsFEAIQBA"
#define ACH_MASOCHIST "CgkIt8qQwKsFEAIQBQ"
#define ACH_BANGER "CgkIt8qQwKsFEAIQBg"
#define ACH_COLLATERAL_DAMAGE "CgkIt8qQwKsFEAIQBw"
#define ACH_INVINCIBLE "CgkIt8qQwKsFEAIQCA"
#define LEAD_VICTORIES "CgkIt8qQwKsFEAIQCQ"

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
        GPGSManager::UnlockAchievement(ACH_FIRST_BLOOD);
        _playerLocalConfig.firstblood = true;
        CCUserDefault::getInstance()->setBoolForKey("firstblood", true);
        CCUserDefault::getInstance()->flush();
    }
}

void GPGSChecker::checkFirstVictory()
{
    if (!_playerLocalConfig.firstvictory) {
        GPGSManager::UnlockAchievement(ACH_TASTE_OF_VICTORY);
        _playerLocalConfig.firstvictory = true;
        CCUserDefault::getInstance()->setBoolForKey("firstvictory", true);
        CCUserDefault::getInstance()->flush();
    }
}

void GPGSChecker::checkSuicidal()
{
    if (!_playerLocalConfig.sucidal) {
        GPGSManager::UnlockAchievement(ACH_SUICIDAL);
        _playerLocalConfig.sucidal = true;
        CCUserDefault::getInstance()->setBoolForKey("sucidal", true);
        CCUserDefault::getInstance()->flush();
    }
}

void GPGSChecker::checkMasochist()
{
    if (!_playerLocalConfig.masochist) {
        GPGSManager::UnlockAchievement(ACH_MASOCHIST);
        _playerLocalConfig.masochist = true;
        CCUserDefault::getInstance()->setBoolForKey("masochist", true);
        CCUserDefault::getInstance()->flush();
    }
}

void GPGSChecker::checkBanger()
{
    if (!_playerLocalConfig.banger) {
        GPGSManager::UnlockAchievement(ACH_BANGER);
        _playerLocalConfig.banger = true;
        CCUserDefault::getInstance()->setBoolForKey("banger", true);
        CCUserDefault::getInstance()->flush();
    }
}

void GPGSChecker::checkCollateralDamage()
{
    if (!_playerLocalConfig.collarteraldamage) {
        GPGSManager::UnlockAchievement(ACH_COLLATERAL_DAMAGE);
        _playerLocalConfig.collarteraldamage = true;
        CCUserDefault::getInstance()->setBoolForKey("collarteraldamage", true);
        CCUserDefault::getInstance()->flush();
    }
}

void GPGSChecker::checkInvincible()
{
    if (!_playerLocalConfig.invincible) {
        GPGSManager::UnlockAchievement(ACH_INVINCIBLE);
        _playerLocalConfig.invincible = true;
        CCUserDefault::getInstance()->setBoolForKey("invincible", true);
        CCUserDefault::getInstance()->flush();
    }
}

void GPGSChecker::sumbitNewVictory()
{
    _playerLocalConfig.victorys++;
    GPGSManager::SubmitHighScore(LEAD_VICTORIES, _playerLocalConfig.victorys);
    CCUserDefault::getInstance()->setIntegerForKey("victorys", _playerLocalConfig.victorys);
    CCUserDefault::getInstance()->flush();
}
