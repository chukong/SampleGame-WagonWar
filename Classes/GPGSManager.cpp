#include "GPGSManager.h"

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

#include "cocos2d.h"
#include "Configuration.h"
#include "WagonSelect.h"
#include "MainScreenScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "NoTouchLayer.h"
#include "PopWindow.h"

bool GPGSManager::isSignedIn = false;
std::unique_ptr<gpg::GameServices> GPGSManager::gameServices;
gpg::TurnBasedMatch GPGSManager::current_match_;


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

gpg::GameServices *GPGSManager::GetGameServices() {
  return gameServices.get();
}

void GPGSManager::BeginUserInitiatedSignIn() {
  if (!gameServices->IsAuthorized()) {
    LOGI("StartAuthorizationUI");
    gameServices->StartAuthorizationUI();
  }
}

void GPGSManager::SignOut() {
  if (gameServices->IsAuthorized()) {
    LOGI("SignOut");
    gameServices->SignOut();
  }
}

void GPGSManager::UnlockAchievement(const char *achievementId) {
  if (gameServices->IsAuthorized()) {
    LOGI("Achievement unlocked");
    gameServices->Achievements().Unlock(achievementId);
  }
}

//By JackyFu 2014.5.20
void GPGSManager::IncrementAchievement(const char *achievementId, uint32_t steps/* = 1*/)
{
    if (gameServices->IsAuthorized()) {
        LOGI("Achievement Increase");
        gameServices->Achievements().Increment(achievementId, steps);
    }
}

void GPGSManager::SubmitHighScore(const char *leaderboardId, uint64_t score) {
  if (gameServices->IsAuthorized()) {
    LOGI("High score submitted");
    gameServices->Leaderboards().SubmitScore(leaderboardId, score);
  }
}

void GPGSManager::ShowAchievements()
{
    if (gameServices->IsAuthorized()) {
        LOGI("Show achievement");
        gameServices->Achievements().ShowAllUI();
    }
}

void GPGSManager::ShowLeaderboard(const char *leaderboardId)
{
    if (gameServices->IsAuthorized()) {
        LOGI("Show achievement");
        gameServices->Leaderboards().ShowUI(leaderboardId);
    }    
}


void GPGSManager::InitServices(gpg::PlatformConfiguration &pc)
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
          if(!g_gameConfig.isInGame)
              ShowMatchInbox();
      })
      .SetOnMultiplayerInvitationEvent([](gpg::TurnBasedMultiplayerEvent event, std::string match_id,
                                          gpg::MultiplayerInvitation invitation) {
          LOGI("MultiplayerInvitationEvent callback");
          //Show default inbox
          if(!g_gameConfig.isInGame)
              ShowMatchInbox();
      }).Create(pc);
  }
  LOGI("Created");
}

void GPGSManager::QuickMatch()
{
    gpg::TurnBasedMultiplayerManager& manager = gameServices->TurnBasedMultiplayer();
    gpg::TurnBasedMatchConfig config = gpg::TurnBasedMatchConfig::Builder()
    .SetMinimumAutomatchingPlayers(MIN_PLAYERS)
    .SetMaximumAutomatchingPlayers(MAX_PLAYERS).Create();
    
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("showConnectingPopWindow");
    
    manager.CreateTurnBasedMatch(config,[](gpg::TurnBasedMultiplayerManager::TurnBasedMatchResponse const &matchResponse)
                                 {
                                     if (matchResponse.status == gpg::MultiplayerStatus::VALID) {
                                         LOGI("QuickMatch Game Begin...By Jacky");
//                                         PlayGame(matchResponse.match);
                                         current_match_ = matchResponse.match;
                                         ParseMatchData();
                                         if (current_match_.HasData() == false && current_match_.Data().size() == 0)
                                         {
                                             setPlayer1Name();
                                             cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("Q1Success");
                                         }
                                         else
                                         {
                                             setPlayer2Name();
                                             cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("Q2Success");
                                         }
                                     }
                                     else
                                     {
                                         LOGI("QuickMatchs Game Failed...===>%d...By Jacky", matchResponse.status);
                                         
                                         cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("QFailed");
//                                         std::string err = "QuickMatch Failed... Google Service Error Code is " + cocos2d::Value((int)matchResponse.status).asString();
//                                         NoTouchLayer* notouchlayer = ((NoTouchLayer*)(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(NOTOUCHTAG)));
//                                         if (notouchlayer) {
//                                             notouchlayer->setError(err);
//                                         }
                                     }
                                 });
}

void GPGSManager::InviteFriend()
{
    gameServices->TurnBasedMultiplayer().ShowPlayerSelectUI(MIN_PLAYERS, MAX_PLAYERS, true,
                                                        [](gpg::TurnBasedMultiplayerManager::PlayerSelectUIResponse const & response)
                                                        {
                                                            LOGI("selected match %d", response.status);

                                                            if (response.status == gpg::UIStatus::VALID) {
                                                                
//                                                                //add notouch layer.
//                                                                auto notouchlayer = NoTouchLayer::create();
//                                                                notouchlayer->setTag(NOTOUCHTAG);
//                                                                cocos2d::Director::getInstance()->getRunningScene()->addChild(notouchlayer,100);
                                                                cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("showConnectingPopWindow");

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
                                                                      //PlayGame(matchResponse.match);
                                                                        current_match_ = matchResponse.match;
                                                                        setPlayer1Name();
                                                                        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("showInviteSuccessPopWindow");
                                                                    }
                                                                    else
                                                                    {
                                                                        LOGI("InviteFriend Game Failed...===>%d...By Jacky",matchResponse.status);
                                                                        
//                                                                        std::string err = "InviteFriend Failed... Google Service Error Code is " + cocos2d::Value((int)matchResponse.status).asString();
//                                                                        NoTouchLayer* notouchlayer = ((NoTouchLayer*)(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(NOTOUCHTAG)));
//                                                                        if(notouchlayer)
//                                                                            notouchlayer->setError(err);
                                                                        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("showInviteFailedPopWindow");
                                                                    }
                                                                });
                                                            }
                                                        });
}

void GPGSManager::ShowMatchInbox()
{
    gameServices->TurnBasedMultiplayer().ShowMatchInboxUI([](gpg::TurnBasedMultiplayerManager::MatchInboxUIResponse const & response)
    {
        if (response.status == gpg::UIStatus::VALID) {
            //Show game based on the user's selection
            
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("showConnectingPopWindow");
            
            current_match_ = response.match;
            switch (response.match.Status()) {
                case gpg::MatchStatus::THEIR_TURN:
                {//Manage match with dismiss, leave and cancel options
                    LOGI("Their turn...By Jacky");
//                    auto notouchlayer = NoTouchLayer::create();
//                    notouchlayer->setTag(NOTOUCHTAG);
//                    cocos2d::Director::getInstance()->getRunningScene()->addChild(notouchlayer,100);
//                    notouchlayer->setError("It's not your turn.");
                    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("showNotYourTurnPopWindow");
                }
                    break;
                case gpg::MatchStatus::MY_TURN:
                {//Play selected game
                    LOGI("My turn...By Jacky...");
                    
                    //add notouch layer.
//                    auto notouchlayer = NoTouchLayer::create();
//                    notouchlayer->setTag(NOTOUCHTAG);
//                    cocos2d::Director::getInstance()->getRunningScene()->addChild(notouchlayer,100);
                    
                    //PlayGame(response.match);
                    current_match_ = response.match;
                    ParseMatchData();
                    //todo:is second turn?
                    int cur_match_turn = GetMatchTurn();//no found, must return 0;
                    cur_match_turn++;
                    if(cur_match_turn == 1){
                        setPlayer1Name();
                        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("enterWagonSelect_1");
                    }
                    else if(cur_match_turn ==2){
                        setPlayer2Name();
                        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("enterWagonSelect_2");
                    }
                    else if(cur_match_turn >=3){
                        LOGI("curmatchturn is %d",cur_match_turn);
                        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("enterGame");
                    }
                    else
                    {
                        LOGI("Current_match_turn is a invalid number ====>>>%d", cur_match_turn);
//                        std::string err = "Current_match_turn is a invalid number : %d" + cocos2d::Value((int)cur_match_turn).asString();
//                        NoTouchLayer* notouchlayer = ((NoTouchLayer*)(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(NOTOUCHTAG)));
//                        if(notouchlayer)
//                            notouchlayer->setError(err);
                    }
                }
                    break;
                case gpg::MatchStatus::COMPLETED:
                    //Manage match with dismiss, rematch options
                    LOGI("Completed...By Jacky");
                   cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("showItsCompletedPopWindow");
                     //cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("TakeTurnSuccess");
                    break;
                case gpg::MatchStatus::EXPIRED:
                default:
                    //Manage match with dismiss option
                    LOGI("Expired & default...By Jacky");
                    
                    //add notouch layer.
//                    auto notouchlayer = NoTouchLayer::create();
//                    notouchlayer->setTag(NOTOUCHTAG);
//                    cocos2d::Director::getInstance()->getRunningScene()->addChild(notouchlayer,100);
                    //enter game
                    current_match_ = response.match;
                    ParseMatchData();
                    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("enterGame");
                    
                    break;
            }
        } else {
            LOGI("Invalid MatchInboxUIResponse response status...%d...By Jacky", response.status);
        }
    });
}

void GPGSManager::LeaveMatch()
{
    // gpg::TurnBasedMultiplayerManager& manager = gameServices->TurnBasedMultiplayer();
    // if (current_match_.Status() == gpg::MatchStatus::MY_TURN) {
    //     //Leave a game
    //     std::vector<gpg::MultiplayerParticipant> participants = current_match_.Participants();
    //     int32_t nextPlayerIndex = GetNextParticipant();
    //     if (nextPlayerIndex == NEXT_PARTICIPANT_AUTOMATCH) {
    //         manager.LeaveMatchDuringMyTurnAndAutomatch(current_match_, [](gpg::MultiplayerStatus status) {
    //                                                        LOGI("Left the game...NEXT_PARTICIPANT_AUTOMATCH...By Jacky");
    //                                                    });
    //         return;
    //     } else {
    //         manager.LeaveMatchDuringMyTurn(current_match_, participants[nextPlayerIndex],
    //                                        [](gpg::MultiplayerStatus status) {
    //                                            LOGI("Left the game...NOT_NEXT_PARTICIPANT_AUTOMATCH...By Jacky");
    //                                        });
    //     }
    // } else {
    //     manager.LeaveMatchDuringTheirTurn(current_match_, [](gpg::MultiplayerStatus status) {
    //                                           LOGI("Left the game...NOT_MY_TURN...By Jacky");
    //                                       });
    // }

    gpg::TurnBasedMultiplayerManager &manager = gameServices->TurnBasedMultiplayer();
  if (current_match_.Status() == gpg::MatchStatus::MY_TURN) {
      //Leave a game
    manager.LeaveMatchDuringMyTurn(current_match_,
                                   current_match_.SuggestedNextParticipant(),
                                   [](gpg::MultiplayerStatus status) {
      LOGI("Left the game");
    });
  } else {
    manager.LeaveMatchDuringTheirTurn(current_match_,
                                      [](gpg::MultiplayerStatus status) {
      LOGI("Left the game");
    });
  }
  return;
}

void GPGSManager::CancelMatch()
{
    gameServices->TurnBasedMultiplayer().CancelMatch(current_match_, [](gpg::MultiplayerStatus status)
    {
        LOGI("Canceled the game...By Jacky");
    });
}

void GPGSManager::DismissMatch()
{
    gameServices->TurnBasedMultiplayer().DismissMatch(current_match_);
}

void GPGSManager::Rematch()
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

void GPGSManager::ConfirmPendingCompletion()
{
    gameServices->TurnBasedMultiplayer().ConfirmPendingCompletion(current_match_, [](gpg::TurnBasedMultiplayerManager::TurnBasedMatchResponse matchResponse)
                                                 {
                                                     LOGI("Remathing the game...By Jacky");
                                                     if (matchResponse.status == gpg::MultiplayerStatus::VALID) {
                                                         LOGI("ReMatch the game...By Jacky");
                                                     }
                                                 });
}

int32_t GPGSManager::GetNextParticipant() {
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
            participants[index].Status() == gpg::ParticipantStatus::JOINED ||
            participants[index].Status() == gpg::ParticipantStatus::NOT_INVITED_YET) {
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

void GPGSManager::ParseMatchData()
{
    if (current_match_.Data().size()) {
        g_gameConfig.match_data = current_match_.Data();
        g_gameConfig.match_string.clear();
        g_gameConfig.match_string.assign(g_gameConfig.match_data.begin(), g_gameConfig.match_data.end());
        LOGI("ParseMatchData is %s", g_gameConfig.match_string.c_str());
    }
    else
    {
        LOGI("Parse match no data");
    }
}

void GPGSManager::TakeTurn(const bool winning, const bool losing)
{
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("gameshowpopwindowlistener");
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("wagonselectshowpopwindowlistener");
    
    LOGI("StarTakeTurn...0");
    gpg::TurnBasedMultiplayerManager& manager = gameServices->TurnBasedMultiplayer();
    gpg::PlayerManager::FetchSelfResponse localPlayer = gameServices->Players().FetchSelfBlocking();
    LOGI("Taking my turnxxxxxxx. local participant name:%s", localPlayer.data.Name().c_str());
    
    
    LOGI("StarTakeTurn...1");
    //Find the participant for the local player.
    gpg::MultiplayerParticipant localParticipant;
    for (auto &participant : current_match_.Participants()) {
        if (participant.Player().Valid() && participant.Player().Id() ==
            localPlayer.data.Id()) {
            localParticipant = participant;
        }
    }
    
    LOGI("Taking my turn. local participant id:%s", localParticipant.Id().c_str());
    LOGI("Taking my turn. local participant name:%s", localParticipant.DisplayName().c_str());
    
    
    std::vector<gpg::MultiplayerParticipant> participants = current_match_.Participants();
    int32_t nextPlayerIndex = GetNextParticipant();
    
    LOGI("StarTakeTurn...2...%d",nextPlayerIndex);
    
    //std::string to std::vector
    g_gameConfig.match_data.clear();
    g_gameConfig.match_data.resize(g_gameConfig.match_string.size());
    g_gameConfig.match_data.assign(g_gameConfig.match_string.begin(), g_gameConfig.match_string.end());

    
    //By default, passing through existing participatntResults
    gpg::ParticipantResults results = current_match_.ParticipantResults();
    
    if(winning || losing)
    {
        if (winning) {
            //Create winning participants result
            results = current_match_.ParticipantResults()
            .WithResult(localParticipant.Id(),  // local player ID
                        0,                      // placing
                        gpg::MatchResult::WIN   // status
                        );
        } else if (losing) {
            //Create losing participants result
            results = current_match_.ParticipantResults()
            .WithResult(localParticipant.Id(),  // local player ID
                        0,                      // placing
                        gpg::MatchResult::LOSS  // status
                        );
        }
        
        manager.FinishMatchDuringMyTurn(
                           current_match_, g_gameConfig.match_data, results,
                           [](
                              gpg::TurnBasedMultiplayerManager::TurnBasedMatchResponse const &
                              response) {
                               LOGI("Took turn");
//                               NoTouchLayer* notouchLayer =((NoTouchLayer*)(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(NOTOUCHTAG)));
//                               if(notouchLayer)
//                                   notouchLayer->setError("Success!");
                               cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("TakeTurnSuccess");
                               //cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("returntoMenu");
                           });
        return ;
    }

    
    LOGI("StarTakeTurn...3");
    LOGI("current_match_ is %d",current_match_.Valid());
    LOGI("results is %d",results.Valid());
    LOGI("global string is --\n%s\n--\n", g_gameConfig.match_string.c_str());

    //Take normal turn
    // switch (nextPlayerIndex) {
    //     default:
            manager.TakeMyTurn(
                               current_match_, g_gameConfig.match_data, results, participants[nextPlayerIndex],
                               [](
                                      gpg::TurnBasedMultiplayerManager::TurnBasedMatchResponse const &
                                      response) {
                                   LOGI("Took turn");
//                                   NoTouchLayer* notouchLayer =((NoTouchLayer*)(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(NOTOUCHTAG)));
//                                   if(notouchLayer)
//                                       notouchLayer->setError("Success!");
                                   if(GetMatchTurn() == 0)
                                       cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("ReadySuccess");
                                    else
                                        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("TakeTurnSuccess");
//                                   cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("returntoMenu");
                               });
//             break;
//         case NEXT_PARTICIPANT_AUTOMATCH:
//             manager.TakeMyTurnAndAutomatch(
//                                            current_match_, g_gameConfig.match_data, results,
//                                            [](
//                                                   gpg::TurnBasedMultiplayerManager::TurnBasedMatchResponse const &
//                                                   response) {
//                                                LOGI("Took turn");
// //                                               NoTouchLayer* notouchLayer =((NoTouchLayer*)(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(NOTOUCHTAG)));
// //                                               if(notouchLayer)
// //                                                   notouchLayer->setError("Success!");
//                                                LOGI("what the match turn now ....%d",GetMatchTurn());
//                                                if(GetMatchTurn() == 0)
//                                                    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("ReadySuccess");
//                                                else
//                                                    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("TakeTurnSuccess");
// //                                               cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("returntoMenu");
//                                            });
//             break;
//         case NEXT_PARTICIPANT_NONE:
//             //Error case
//             manager.DismissMatch(current_match_);
//             cocos2d::Director::getInstance()->replaceScene(MainScreenScene::createScene(false));
//             break;
//     }
    
    LOGI("StarTakeTurn...4");
}

int GPGSManager::GetMatchTurn()
{
    //todo: parse json, get match turn.
    
    if(current_match_.Data().size())
    {
        g_gameConfig.match_string.clear();
        g_gameConfig.match_string.resize(current_match_.Data().size());
        g_gameConfig.match_string.assign(current_match_.Data().begin(), current_match_.Data().end());
        
        rapidjson::Document doc;
        doc.Parse<rapidjson::kParseDefaultFlags>(g_gameConfig.match_string.c_str());
        
        if(doc.HasMember("turn"))
            return doc["turn"].GetInt();
        else
            return -1;
    }
    else
        return 0;
}

void GPGSManager::setPlayer1Name()
{
    gpg::PlayerManager::FetchSelfResponse localPlayer = gameServices->Players().FetchSelfBlocking();
    g_gameConfig.player1Name = localPlayer.data.Name();
    LOGI("player1 name:%s", localPlayer.data.Name().c_str());
}

void GPGSManager::setPlayer2Name()
{
    gpg::PlayerManager::FetchSelfResponse localPlayer = gameServices->Players().FetchSelfBlocking();
    g_gameConfig.player2Name = localPlayer.data.Name();
    LOGI("player2 name:%s", localPlayer.data.Name().c_str());
}