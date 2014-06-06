//
//  GPGSChecker.h
//  TankMultiplayer
//
//  Created by Jacky on 14-6-7.
//
//

#ifndef __TankMultiplayer__GPGSChecker__
#define __TankMultiplayer__GPGSChecker__


#include "GPGSChecker.h"
#include "cocos2d.h"
#include "GPGSManager.h"

USING_NS_CC;

typedef struct _playerlocalconfig
{
    bool firstblood;
    bool firstvictory;
    bool sucidal;
    bool masochist;
    bool banger;
    bool collarteraldamage;
    bool invincible;
    int victorys;
}PlayerLocalConfig;


class GPGSChecker
{
public:
    
    static GPGSChecker* getInstance()
    {
        if ( m_pInstance == nullptr )
            m_pInstance = new GPGSChecker();
        return m_pInstance;
    }
    
private:
    
    GPGSChecker();
    
    PlayerLocalConfig _playerLocalConfig;
    
    static GPGSChecker* m_pInstance;
    
    class CGarbo
    {
    public:
        ~CGarbo()
        {
            if (GPGSChecker::m_pInstance!= nullptr)
            {
                delete GPGSChecker::m_pInstance;
            }
        }
    };
    
    static CGarbo m_garbo;
    
public:
    
    void chcekFirstBlood();
    void checkFirstVictory();
    void checkSuicidal();
    void checkMasochist();
    void checkBanger();
    void checkCollateralDamage();
    void checkInvincible();
    void sumbitNewVictory();
    bool isHaveSaveFile();
};


#endif /* defined(__TankMultiplayer__GPGSChecker__) */
