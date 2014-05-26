#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

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

class StateManager {
    
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

private:
    static int32_t GetNextParticipant();
    static void ManageGame(gpg::TurnBasedMatch const& match, const bool leave, const bool cancel, const bool rematch);
    static void PlayGame(gpg::TurnBasedMatch const& match);

private:
    static bool isSignedIn;
    static std::unique_ptr<gpg::GameServices> gameServices;
    static gpg::TurnBasedMatch current_match_;

};


#endif // STATE_MANAGER_HPP
