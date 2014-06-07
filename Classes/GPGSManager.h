#ifndef GPGS_MANAGER_H
#define GPGS_MANAGER_H

#ifdef __OBJC__
#include <objc/NSObjCRuntime.h>
#endif

#include "gpg/gpg.h"

const int32_t MIN_PLAYERS = 1;
const int32_t MAX_PLAYERS = 1;

enum NEXT_PARTICIPANT {
    NEXT_PARTICIPANT_AUTOMATCH = -1,
    NEXT_PARTICIPANT_NONE = -2,
};

class GPGSManager {
    
public:
    static void InitServices(gpg::PlatformConfiguration &pc);
    static gpg::GameServices *GetGameServices();
    static void BeginUserInitiatedSignIn();
    static void SignOut();
    static void UnlockAchievement(const char *achievementId);
    static void IncrementAchievement(const char *achievementId, uint32_t steps = 1);
    static void SubmitHighScore(const char *leaderboardId, uint64_t score);
    static void ShowAchievements();
    static void ShowLeaderboard(const char *leaderboardId);
    static bool IsSignedIn() { return isSignedIn; }
    
    //by Jacky 2014.05.22
    static void QuickMatch();
    static void InviteFriend();
    static void ShowMatchInbox();
    
    static void LeaveMatch();
    static void CancelMatch();
    static void DismissMatch();
    static void Rematch();
    static void ConfirmPendingCompletion();
    
    static void TakeTurn(const bool winning, const bool losing);
    static int32_t GetNextParticipant();
    static void ParseMatchData();
    
    static int GetMatchTurn();
    static void setPlayer1Name();
    static void setPlayer2Name();

private:
    static bool isSignedIn;
    static std::unique_ptr<gpg::GameServices> gameServices;
    static gpg::TurnBasedMatch current_match_;

};


#endif // GPGS_MANAGER_HPP
