/**
 * @file gpg/quest_manager.h
 * @copyright Copyright 2014 Google Inc. All Rights Reserved.
 * @brief Entry points for Play Games Quest functionality.
 */

#ifndef GPG_QUEST_MANAGER_H_
#define GPG_QUEST_MANAGER_H_

#ifndef __cplusplus
#error Header file supports C++ only
#endif  // __cplusplus

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "gpg/common.h"
#include "gpg/game_services.h"
#include "gpg/quest.h"
#include "gpg/types.h"

namespace gpg {

/**
 * Gets and sets various quest-related data.
 * @ingroup Managers
 */
class GPG_EXPORT QuestManager {
 public:
  /**
   * Constructs a <code>QuestManager</code> from a
   * <code>GameServicesImpl</code>. This function is not intended to be called
   * by consumers of this API. Instead, an app should retrieve the
   * <code>QuestManager</code> via the
   * <code>GameServices::QuestManager</code> call.
   */
  explicit QuestManager(GameServicesImpl *game_services_impl);
  ~QuestManager();

  /**
   * Holds data for a quest, along with a response status.
   * @ingroup ResponseType
   */
  struct FetchResponse {
  /**
   * Can be one of the values enumerated in {@link ResponseStatus}.
   */
    ResponseStatus status;

    /**
     * Data associated with this app.
     */
    Quest data;
  };

  /**
   * Defines a callback type that receives a <code>FetchResponse</code>. This
   * callback type is provided to the <code>Fetch(*)</code> functions below.
   * @ingroup Callbacks
   */
  typedef std::function<void(FetchResponse const &)> FetchCallback;

  /**
   * Asynchronously loads data for a specific achievement for the currently
   * signed-in player.
   * Calls the provided <code>FetchCallback</code> upon operation completion.
   * Not specifying <code>data_source</code> makes this function call
   * equivalent to calling
   * <code>Fetch(DataSource data_source, std::string const &quest_id, </code>
   * <code>FetchCallback callback)</code>, with <code>data_source</code>
   * specified as <code>CACHE_OR_NETWORK</code>.
   */
  void Fetch(std::string const &quest_id, FetchCallback callback);

  /**
   * Asynchronously loads quest data for the currently signed-in player.
   * Calls the provided <code>FetchCallback</code> upon operation completion.
   * Specify <code>data_source</code> as <code>CACHE_OR_NETWORK</code> or
   * <code>NETWORK_ONLY</code>.
   */
  void Fetch(DataSource data_source,
             std::string const &quest_id,
             FetchCallback callback);

  /**
   * Synchronously loads quest data for the currently signed-in player,
   * directly returning the <code>FetchResponse</code>. Specifying neither
   * <code>data_source</code> nor <code>timeout</code> makes this function
   * call equivalent to calling
   * <code>FetchResponse FetchBlocking(DataSource data_source, </code>
   * <code>Timeout timeout)</code>,
   * with <code>data_source</code> specified as <code>CACHE_OR_NETWORK</code>
   * and <code>timeout</code> specified as 10 years.
   */
  FetchResponse FetchBlocking(std::string const &quest_id);

  /**
   * Synchronously loads all quest data for the currently signed-in player,
   * directly returning the <code>FetchResponse</code>. Specify
   * <code>data_source</code> as <code>CACHE_OR_NETWORK</code> or
   * <code>NETWORK_ONLY</code>. Not specifying timeout makes this
   * function call equivalent to calling
   * <code>FetchResponse FetchBlocking(DataSource data_source, </code>
   * <code>Timeout timeout)</code>, with your specified value for
   * <code>data_source</code>, and <code>timeout</code> specified as 10
   * years.
   */
  FetchResponse FetchBlocking(DataSource data_source,
                              std::string const &quest_id);

  /**
   * Synchronously loads quest data for the currently signed-in player,
   * directly returning the <code>FetchResponse</code>. Specify
   * <code>timeout</code> as an arbitrary number of milliseconds. Not
   * specifying <code>data_source</code> makes this function call equivalent to calling
   * <code>FetchResponse FetchBlocking(DataSource data_source, </code>
   * <code>Timeout timeout)</code>,
   * with <code>data_source</code> specified as
   * <code>CACHE_OR_NETWORK</code>, and <code>timeout</code> containing the
   * value you specified.
   */
  FetchResponse FetchBlocking(Timeout timeout, std::string const &quest_id);

  /**
   * Synchronously loads quest data for the currently signed-in player.
   * directly returning the <code>FetchResponse</code>. Specify
   * <code>data_source</code> as <code>CACHE_OR_NETWORK</code> or
   * <code>NETWORK_ONLY</code>. Specify <code>timeout</code> as an arbitrary
   * number of milliseconds.
   */
  FetchResponse FetchBlocking(DataSource data_source,
                              Timeout timeout,
                              std::string const &quest_id);

  /**
   * Contains data and response statuses for all quests.
   * @ingroup ResponseType
   */
  struct FetchAllResponse {
    /**
   * Can be one of the values enumerated in {@link ResponseStatus}.
     */
    ResponseStatus status;

    /**
     * A vector containing all quest data:
     */
    std::vector<Quest> data;
  };

  /**
   * Defines a callback type that receives a <code>FetchAllResponse</code>.
   * This callback type is provided to the <code>FetchAll(*)</code> functions
   * below.
   * @ingroup Callbacks
   */
  typedef std::function<void(FetchAllResponse const &)> FetchAllCallback;

  /**
   * Asynchronously loads data for all quests of certain states for the
   * currently signed-in player.
   * Not specifying <code>data_source</code> makes this function call
   * equivalent to calling
   * <code>FetchAll(DataSource data_source, FetchAllCallback callback)</code>,
   * with <code>data_source</code> specified as <code>CACHE_OR_NETWORK</code>.
   */
  void FetchAll(FetchAllCallback callback, int32_t fetch_flags);

  /**
   * Asynchronously loads data for all quests of certain states for the
   * currently signed-in player.
   * Specify <code>data_source</code> as <code>CACHE_OR_NETWORK</code> or
   * <code>NETWORK_ONLY</code>.
   */
  void FetchAll(DataSource data_source,
                FetchAllCallback callback,
                int32_t fetch_flags);

  /**
   * Synchronously loads data for all quests of certain states for the
   * currently signed-in player, directly returning the
   * <code>FetchAllResponse</code>. Specifying neither
   * <code>data_source</code> nor <code>timeout</code> makes this function
   * call equivalent to calling
   * <code>FetchAllResponse FetchAllBlocking (DataSource data_source, </code>
   * <code>Timeout timeout)</code>, with <code>data_source</code> specified as
   * <code>CACHE_OR_NETWORK</code>, and <code>timeout</code> specified as 10
   * years.
   */
  FetchAllResponse FetchAllBlocking(int32_t fetch_flags);

  /**
   * Synchronously loads data for all quests of certain states for the
   * currently signed-in player, directly returning the
   * <code>FetchAllResponse</code>. Specify <code>data_source</code> as
   * <code>CACHE_OR_NETWORK</code> or <code>NETWORK_ONLY</code>. Not
   * specifying <code>timeout</code> makes this function call equivalent to
   * calling
   * <code>FetchAllResponse FetchAllBlocking(DataSource data_source, </code>
   * <code>Timeout timeout)</code>,
   * with your specified <code>data_source<code> value, and
   * <code>timeout</code> specified as 10 years.
   */
  FetchAllResponse FetchAllBlocking(DataSource data_source,
                                    int32_t fetch_flags);

  /**
   * Synchronously loads data for all quests of certain states for the currently
   * signed-in player, directly returning the <code>FetchAllResponse</code>.
   * Specify <code>timeout</code> as an arbitrary number of milliseconds. Not
   * specifying <code>data_source</code> makes
   * this function call equivalent to calling
   * <code>FetchAllResponse FetchAllBlocking(DataSource data_source, </code>
   * <code>Timeout timeout)</code>, with <code>data_source</code> specified
   * as <code>CACHE_OR_NETWORK</code>, and <code>timeout</code> containing the
   * value you specified.
   */
  FetchAllResponse FetchAllBlocking(Timeout timeout, int32_t fetch_flags);

  /**
   * Synchronously loads data for all quests of certain states for the
   * currently signed-in player, directly returning the
   * <code>FetchAllResponse</code>. Specify <code>data_source</code> as
   * <code>CACHE_OR_NETWORK</code> or <code>NETWORK_ONLY</code>. Specify
   * <code>timeout</code> as an arbitrary number of milliseconds.
   */
  FetchAllResponse FetchAllBlocking(DataSource data_source,
                                    Timeout timeout,
                                    int32_t fetch_flags);

  /**
   * Asynchronously loads data for all quests, regardless of state, for the
   * currently signed-in player.
   * Not specifying <code>data_source</code> makes this function call
   * equivalent to calling
   * <code>FetchAll(DataSource data_source, FetchAllCallback callback)</code>,
   * with <code>data_source</code> specified as <code>CACHE_OR_NETWORK</code>.
   */
  void FetchAll(FetchAllCallback callback);

  /**
   * Asynchronously loads data for all quests, regardless of state, for the
   * currently signed-in player.
   * Specify <code>data_source</code> as <code>CACHE_OR_NETWORK</code>
   * or <code>NETWORK_ONLY</code>.
   */
  void FetchAll(DataSource data_source, FetchAllCallback callback);

  /**
   * Synchronously loads data for all quests, regardless of state, for the
   * currently signed-in player, directly returning the
   * <code>FetchAllResponse</code>.
   * Specifying neither <code>data_source</code> nor <code>timeout</code>
   * makes this function call equivalent to calling
   * <code>FetchAllResponse FetchAllBlocking (DataSource data_source, <code>
   * <code>Timeout timeout)</code>, with <code>data_source</code> specified as
   * <code>CACHE_OR_NETWORK</code>, and <code>timeout</code> specified as 10
   * years.
   */
  FetchAllResponse FetchAllBlocking();

  /**
   * Synchronously loads data for all quests, regardless of state, for the
   * currently signed-in player, directly returning the
   * <code>FetchAllResponse</code>.
   * Specify <code>data_source</code> as <code>CACHE_OR_NETWORK</code> or
   * <code>NETWORK_ONLY</code>. Not specifying <code>timeout</code> makes
   * this function call equivalent to calling
   * <code>FetchAllResponse FetchAllBlocking(DataSource data_source, </code>
   * <code>Timeout timeout)</code>,
   * with your specified <code>data_source</code> value, and
   * <code>timeout</code> specified as 10 years.
   */
  FetchAllResponse FetchAllBlocking(DataSource data_source);

  /**
   * Synchronously loads data for all quests, regardless of state, for the
   * currently signed-in player, directly returning the
   * <code>FetchAllResponse</code>.
   * Specify <code>timeout</code> as an arbitrary number of milliseconds. Not
   * specifying <code>data_source</code> makes this function call equivalent
   * to calling
   * <code>FetchAllResponse FetchAllBlocking(DataSource data_source, </code>
   * <code>Timeout timeout)</code>,
   * with <code>data_source</code> specified as <code>CACHE_OR_NETWORK</code>,
   * and <code>timeout</code> containing the value you specified.
   */
  FetchAllResponse FetchAllBlocking(Timeout timeout);

  /**
   * Synchronously loads data for all quests, regardless of state, for the
   * currently signed-in player, directly returning the
   * <code>FetchAllResponse</code>.
   * Specify <code>data_source</code> as <code>CACHE_OR_NETWORK</code> or
   * <code>NETWORK_ONLY</code>. Specify <code>timeout</code> as
   * an arbitrary number of milliseconds.
   */
  FetchAllResponse FetchAllBlocking(DataSource data_source, Timeout timeout);

  /**
   * Defines a callback that can be used to receive an
   * <code>AcceptQuestStatus</code>. Used by the <code>Accept*</code>
     functions.
   */
  typedef std::function<void(QuestAcceptStatus)> AcceptCallback;

  /**
   * Asynchronously accept a quest. The quest must have a state
   * <code>QuestState::OPEN</code>. Incrementing the associated events will
   * start tracking progress toward the milestone goal.
   */
  void Accept(Quest const &quest, AcceptCallback callback);

  /**
   * Synchronously accept a quest. The quest must have a state
   * <code>QuestState::OPEN</code>. Incrementing the associated events will
   * start tracking progress toward the milestone goal.
   * Not specifying timeout makes this function call equivalent to calling
   * <code>QuestAcceptStatus AcceptBlocking(Timeout timeout, </code>
   * <code>                                 std::string const &quest_id),
   * </code>, with <code>timeout</code> specified as 10 years.
   */
  QuestAcceptStatus AcceptBlocking(Quest const &quest);

  /**
   * Synchronously accept a quest. The quest must have a state
   * <code>QuestState::OPEN</code>. Incrementing the associated events will
   * start tracking progress toward the milestone goal.
   * Specify <code>timeout</code> as an arbitrary number of milliseconds.
   */
  QuestAcceptStatus AcceptBlocking(Timeout timeout,
                                   Quest const &quest);

  /**
   * Defines a callback which can be used to receive a
   * <code>QuestClaimMilestoneStatus</code>.Used by the
   * <code>ClaimMilestone*</code> functions.
   */
  typedef std::function<void(QuestClaimMilestoneStatus)>
      ClaimMilestoneCallback;

  /**
   * Asynchronously claims the milestone. Doing so calls the server, marking
   * the milestone as completed, enforcing (TODO (erickrk) "enforces,"
   * not "ensures"?) that the milestone is currently claimable, and that it
   * hasn't been claimed already on this or another device. If this call
   * returns true for <code>QuestClaimMilestoneStatus::VALID</code>, you (as
   * developer) must still reward the player. Use the milestone
   * <code>CompletionRewardData</code> to do so.
   */
  void ClaimMilestone(QuestMilestone const &milestone,
                      ClaimMilestoneCallback callback);

  /**
   * Synchronously claim the milestone. Doing so calls the server, marking the
   * milestone as completed. It also enforces (TODO (erickrk) "enforces,"
   * not "ensures"?) that the milestone is currently claimable, and that it
   * hasn't been claimed already on this or another device. If this call
   * returns true for <code>QuestClaimMilestoneStatus::VALID</code>, you (as
   * developer) must still reward the player. Use the milestone
   * <code>CompletionRewardData</code> to do so.
   * Not specifying <code>timeout</code> makes this function call equivalent
   * to calling <code>QuestClaimMilestoneStatus</code>
   * ClaimMilestoneBlocking(
   *     Timeout timeout,
   *     std::string const &quest_id,
   *     std::string const &milestone_id);
   * with <code>timeout</code> specified as 10 years.
   */
  QuestClaimMilestoneStatus ClaimMilestoneBlocking(
      QuestMilestone const &milestone);

  /**
   * Synchronously claim the milestone. Doing so will call the server, marking
   * the milestone as completed. It also enforces (TODO (ericrk): same
   * question as above) that the milestone is
   * currently claimable, and that it hasn't been claimed already on one or
   * another device. If this call returns true for
   * <code>QuestClaimMilestoneStatus::VALID</code>, you (as developer) must
   * still reward the player. Use the milestone
   * <code>CompletionRewardData</code> to do so.
   * Specify <code>timeout</code> as an arbitrary number of milliseconds.
   */
  QuestClaimMilestoneStatus ClaimMilestoneBlocking(
      Timeout timeout,
      QuestMilestone const &milestone);

 /**
   * Presents to the user a UI that displays information about all quests.
   * Returns when the user has dismissed the UI.
   */
  void ShowAllUI();

  /**
   * Presents to the user a UI that displays information about a specific
   * quest. Returns when the user has dismissed the UI.
   */
  void ShowUI(std::string const &quest_id);

 private:
  QuestManager(QuestManager const &) = delete;
  QuestManager &operator=(QuestManager const &) = delete;

  GameServicesImpl *const impl_;
};

}  // namespace gpg

#endif  // GPG_QUEST_MANAGER_H_
