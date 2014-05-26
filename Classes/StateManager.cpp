/* Copyright (c) 2014 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "StateManager.h"

#ifdef __APPLE__
//
//Logging for CoreFoundation
//
#include <CoreFoundation/CoreFoundation.h>
extern "C" void NSLog(CFStringRef format, ...);
const int32_t BUFFER_SIZE = 256;
#define LOGI(...) {char c[BUFFER_SIZE];\
    snprintf(c,BUFFER_SIZE,__VA_ARGS__);\
    CFStringRef str = CFStringCreateWithCString(kCFAllocatorDefault, c, kCFStringEncodingMacRoman);\
    NSLog(str);\
    CFRelease(str);\
    }
#else
#include "android/Log.h"
#define DEBUG_TAG "main"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, DEBUG_TAG, __VA_ARGS__))

#endif

bool StateManager::isSignedIn = false;
std::unique_ptr<gpg::GameServices> StateManager::gameServices;
gpg::TurnBasedMatch StateManager::current_match_;


void OnAuthActionStarted(gpg::AuthOperation op) {
  LOGI("OnAuthActionStarted");
  switch ( op ) {
  case gpg::AuthOperation::SIGN_IN:
    LOGI("Signing In...");
    break;
  case gpg::AuthOperation::SIGN_OUT:
    LOGI("Signing Out...");
    break;
  }
}

void OnAuthActionFinished(gpg::AuthOperation op, gpg::AuthStatus status) {
    LOGI("OnAuthActionFinished");
    if (op == gpg::AuthOperation::SIGN_IN){
        LOGI("Signing In.");
    }
    else{
        LOGI("Signing Out.");
    }
}

gpg::GameServices *StateManager::GetGameServices() {
  return gameServices.get();
}

void StateManager::BeginUserInitiatedSignIn() {
  if (!gameServices->IsAuthorized()) {
    LOGI("StartAuthorizationUI");
    gameServices->StartAuthorizationUI();
  }
}

void StateManager::SignOut() {
  if (gameServices->IsAuthorized()) {
    LOGI("SignOut");
    gameServices->SignOut();
  }
}

void StateManager::UnlockAchievement(const char *achievementId) {
  if (gameServices->IsAuthorized()) {
    LOGI("Achievement unlocked");
    gameServices->Achievements().Unlock(achievementId);
  }
}

//By JackyFu 2014.5.20
void StateManager::IncrementAchievement(const char *achievementId, uint32_t steps/* = 1*/)
{
    if (gameServices->IsAuthorized()) {
        LOGI("Achievement Increase");
        gameServices->Achievements().Increment(achievementId, steps);
    }
}

void StateManager::SubmitHighScore(const char *leaderboardId, uint64_t score) {
  if (gameServices->IsAuthorized()) {
    LOGI("High score submitted");
    gameServices->Leaderboards().SubmitScore(leaderboardId, score);
  }
}

void StateManager::ShowAchievements()
{
    if (gameServices->IsAuthorized()) {
        LOGI("Show achievement");
        gameServices->Achievements().ShowAllUI();
    }
}

void StateManager::ShowLeaderboard(const char *leaderboardId)
{
    if (gameServices->IsAuthorized()) {
        LOGI("Show achievement");
        gameServices->Leaderboards().ShowUI(leaderboardId);
    }    
}


void StateManager::InitServices(gpg::PlatformConfiguration &pc)
{
  LOGI("Initializing Services");
  if (!gameServices) {
    LOGI("Uninitialized services, so creating");
    gameServices = gpg::GameServices::Builder()
      .SetLogging(gpg::DEFAULT_ON_LOG, gpg::LogLevel::VERBOSE)
//      .SetOnAuthActionFinished(OnAuthActionFinished)
//      .SetOnAuthActionStarted(OnAuthActionStarted)
      // Add a test scope (we don't actually use this).
      //    .AddOauthScope("https://www.googleapis.com/auth/appstate")
      //    .InternalSetRootURL("https://www-googleapis-staging.sandbox.google.com/")
      .SetOnAuthActionStarted([](gpg::AuthOperation op){
          OnAuthActionStarted(op);
      })
      .SetOnAuthActionFinished([](gpg::AuthOperation op, gpg::AuthStatus status){
          LOGI("Sign in finished with a result of %d", status);
          if( status == gpg::AuthStatus::VALID )
              isSignedIn = true;
          else
              isSignedIn = false;
          OnAuthActionFinished( op, status);
      })
      .SetOnTurnBasedMatchEvent([](gpg::TurnBasedMultiplayerEvent event, std::string match_id,
                                       gpg::TurnBasedMatch match) {
                                    LOGI("TurnBasedMultiplayerEvent callback");
                                    //Show default inbox
//                                    ShowMatchInbox();
      })
      .SetOnMultiplayerInvitationEvent([](gpg::TurnBasedMultiplayerEvent event, std::string match_id,
                                              gpg::MultiplayerInvitation invitation) {
                                           LOGI("MultiplayerInvitationEvent callback");
                                           //Show default inbox
//                                           ShowMatchInbox();
      }).Create(pc);
  }
  LOGI("Created");
}

void StateManager::QuickMatch()
{
    gpg::TurnBasedMultiplayerManager& manager = gameServices->TurnBasedMultiplayer();
    gpg::TurnBasedMatchConfig config = gpg::TurnBasedMatchConfig::Builder()
    .SetMinimumAutomatchingPlayers(MIN_PLAYERS)
    .SetMaximumAutomatchingPlayers(MAX_PLAYERS).Create();
    
    manager.CreateTurnBasedMatch(config,
                                 [](gpg::TurnBasedMultiplayerManager::TurnBasedMatchResponse const &matchResponse)
                                 {
                                     if (matchResponse.status == gpg::MultiplayerStatus::VALID) {
                                         LOGI("QuickMatch Game Begin...By Jacky");
                                         //QuickMatch...
                                         //PlayGame(matchResponse.match);
                                     }
                                 });
}

void StateManager::InviteFriend()
{
    gameServices->TurnBasedMultiplayer().ShowPlayerSelectUI(MIN_PLAYERS, MAX_PLAYERS, true,
                                                        [](gpg::TurnBasedMultiplayerManager::PlayerSelectUIResponse const & response)
                                                        {
                                                            LOGI("selected match %d", response.status);
                                                            if (response.status == gpg::UIStatus::VALID) {
                                                                // Create new match with the config
                                                                gpg::TurnBasedMatchConfig config = gpg::TurnBasedMatchConfig::Builder()
                                                                .SetMinimumAutomatchingPlayers(response.minimum_automatching_players)
                                                                .SetMaximumAutomatchingPlayers(response.maximum_automatching_players)
                                                                .AddAllPlayersToInvite(response.player_ids).Create();
                                                                
                                                                gameServices->TurnBasedMultiplayer().CreateTurnBasedMatch(config, [](gpg::TurnBasedMultiplayerManager::TurnBasedMatchResponse const & matchResponse)
                                                                {
                                                                    if (matchResponse.status == gpg::MultiplayerStatus::VALID)
                                                                    {
                                                                        LOGI("InviteFriend Game Begin...By Jacky");
                                                                      //InviteFriend...
                                                                      //PlayGame(matchResponse.match);
                                                                    }
                                                                });
                                                            }
                                                        });
}

void StateManager::ShowMatchInbox()
{
    gameServices->TurnBasedMultiplayer().ShowMatchInboxUI([](gpg::TurnBasedMultiplayerManager::MatchInboxUIResponse const & response)
    {
        if (response.status == gpg::UIStatus::VALID) {
            //Show game based on the user's selection
            switch (response.match.Status()) {
                case gpg::MatchStatus::THEIR_TURN:
                    //Manage match with dismiss, leave and cancel options
                    LOGI("Their turn...By Jacky");
                    ManageGame(response.match, true, true, false);
                    break;
                case gpg::MatchStatus::MY_TURN:
                    //Play selected game
                    LOGI("My turn...By Jacky");
                    PlayGame(response.match);
                    break;
                case gpg::MatchStatus::COMPLETED:
                    //Manage match with dismiss, rematch options
                    LOGI("Completed...By Jacky");
                    ManageGame(response.match, false, false, true);
                    break;
                case gpg::MatchStatus::EXPIRED:
                default:
                    //Manage match with dismiss option
                    LOGI("Expired & default...By Jacky");
                    ManageGame(response.match, false, false, false);
                    break;
            }
        } else {
            LOGI("Invalid response status...By Jacky");
        }
    });
}

void StateManager::LeaveMatch()
{
    gpg::TurnBasedMultiplayerManager& manager = gameServices->TurnBasedMultiplayer();
    if (current_match_.Status() == gpg::MatchStatus::MY_TURN) {
        //Leave a game
        std::vector<gpg::MultiplayerParticipant> participants = current_match_.Participants();
        int32_t nextPlayerIndex = GetNextParticipant();
        if (nextPlayerIndex == NEXT_PARTICIPANT_AUTOMATCH) {
            manager.LeaveMatchDuringMyTurnAndAutomatch(current_match_, [](gpg::MultiplayerStatus status) {
                                                           LOGI("Left the game...NEXT_PARTICIPANT_AUTOMATCH...By Jacky");
                                                       });
            return;
        } else {
            manager.LeaveMatchDuringMyTurn(current_match_, participants[nextPlayerIndex],
                                           [](gpg::MultiplayerStatus status) {
                                               LOGI("Left the game...NOT_NEXT_PARTICIPANT_AUTOMATCH...By Jacky");
                                           });
        }
    } else {
        manager.LeaveMatchDuringTheirTurn(current_match_, [](gpg::MultiplayerStatus status) {
                                              LOGI("Left the game...NOT_MY_TURN...By Jacky");
                                          });
    }
}

void StateManager::CancelMatch()
{
    gameServices->TurnBasedMultiplayer().CancelMatch(current_match_, [](gpg::MultiplayerStatus status)
    {
        LOGI("Canceled the game...By Jacky");
    });
}

void StateManager::DismissMatch()
{
    gameServices->TurnBasedMultiplayer().DismissMatch(current_match_);
}

void StateManager::Rematch()
{
    gameServices->TurnBasedMultiplayer().Rematch(current_match_, [](gpg::TurnBasedMultiplayerManager::TurnBasedMatchResponse matchResponse)
                    {
                        LOGI("Remathing the game...By Jacky");
                        if (matchResponse.status == gpg::MultiplayerStatus::VALID) {
                            LOGI("ReMatch the game...By Jacky");
                            //Rematch...
                            //PlayGame(matchResponse.match);
                        }
                    });
}

int32_t StateManager::GetNextParticipant() {
    gpg::PlayerManager::FetchSelfResponse localPlayer =
    gameServices->Players().FetchSelfBlocking();
    
    //Retrieve next participant
    std::vector<gpg::MultiplayerParticipant> participants =
    current_match_.Participants();
    int32_t localPlayerIndex = -1;
    int32_t nextPlayerIndex = -1;
    int32_t size = participants.size();
    
    LOGI("# of participants: %d", size);
    for (int32_t i = 0; i < size; ++i) {
        if (participants[i].Player().Id().compare(localPlayer.data.Id()) == 0) {
            localPlayerIndex = i;
        }
        LOGI("participant: %s", participants[i].Player().Id().c_str());
    }
    if (localPlayerIndex == -1) {
        LOGI("Local player not found in a match?");
        return -1;
    }
    
    for (int32_t i = 1; i < size; ++i) {
        int32_t index = (localPlayerIndex + i) % size;
        if (participants[index].Status() == gpg::ParticipantStatus::INVITED ||
            participants[index].Status() == gpg::ParticipantStatus::JOINED) {
            LOGI("Found next participant");
            nextPlayerIndex = index;
        }
    }
    if (nextPlayerIndex == -1) {
        // No next participant found
        // Do we have Auto-match player?
        if (current_match_.AutomatchingSlotsAvailable()) {
            LOGI("Auto matching for next participant");
            return NEXT_PARTICIPANT_AUTOMATCH;
        } else
            return NEXT_PARTICIPANT_NONE;
    }
    return nextPlayerIndex;
}

void StateManager::ManageGame(gpg::TurnBasedMatch const& match, const bool leave, const bool cancel, const bool rematch)
{
    current_match_ = match;
    //...
}

void StateManager::PlayGame(gpg::TurnBasedMatch const& match)
{
    current_match_ = match;
    //...
}