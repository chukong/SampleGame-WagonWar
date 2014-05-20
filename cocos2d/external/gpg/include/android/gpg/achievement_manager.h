/**
 * @file gpg/achievement_manager.h
 * @copyright Copyright 2014 Google Inc. All Rights Reserved.
 * @brief Entry points for Play Games Achievement functionality.
 */

#ifndef GPG_ACHIEVEMENT_MANAGER_H_
#define GPG_ACHIEVEMENT_MANAGER_H_

#ifndef __cplusplus
#error Header file supports C++ only
#endif  // __cplusplus

#include <memory>
#include <vector>
#include "gpg/achievement.h"
#include "gpg/common.h"
#include "gpg/game_services.h"
#include "gpg/types.h"

namespace gpg {

/**
 * Gets and sets various achievement-related data.
 * @ingroup Managers
 */
class GPG_EXPORT AchievementManager {
 public:
  /**
   * An explicit constructor.
   */
  explicit AchievementManager(GameServicesImpl *game_services_impl);
  ~AchievementManager();

  /**
   * Holds all data for all achievements, along with a response status.
   * @ingroup ResponseType
   */
  struct FetchAllResponse {
   /**
    * Can be: VALID, STALE, ERROR_LICENSE_CHECK_FAILED, ERROR_INTERNAL
    * ERROR_NOT_AUTHORIZED, or ERROR_TIMEOUT.
    * If the response is unsuccessful, FetchAllResponse's data vector will be
    * empty.
    */
    ResponseStatus status;

    /**
     * A vector containing data for all achievements associated with the
     * application.
     */
    std::vector<Achievement> data;
  };

  /**
   * Defines a callback type that receives a FetchAllResponse. This callback
   * type is provided to the FetchAll(*) functions below.
   * @ingroup Callbacks
   */
  typedef std::function<void(FetchAllResponse const &)> FetchAllCallback;

  /**
   * Asynchronously loads all achievement data for the currently signed-in
   * player. Calls the provided FetchAllCallback on operation completion.
   * Not specifying data_source makes this function call equivalent to calling
   * FetchAll(DataSource data_source, FetchAllCallback), with data_source
   * specified as CACHE_OR_NETWORK.
   */
  void FetchAll(FetchAllCallback callback);

  /**
   * Asynchronously loads all achievement data for the currently signed-in
   * player. Calls the provided FetchAllCallback on operation completion.
   * Specify data_source as CACHE_OR_NETWORK or NETWORK_ONLY.
   */
  void FetchAll(DataSource data_source, FetchAllCallback callback);

  /**
   * Synchronously loads all achievement data for the currently signed-in
   * player, directly returning the FetchAllResponse. Specifying neither
   * data_source nor timeout makes this function call equivalent to calling
   * FetchAllResponse FetchAllBlocking(DataSource data_source, Timeout timeout),
   * with data_source specified as CACHE_OR_NETWORK, and timeout specified as 10
   * years.
   */
  FetchAllResponse FetchAllBlocking();

  /**
   * Synchronously loads all achievement data for the currently signed-in
   * player, directly returning the FetchAllResponse. Specify data_source
   * as CACHE_OR_NETWORK or NETWORK_ONLY. Not specifying timeout makes this
   * function call equivalent to calling
   * FetchAllBlocking FetchAllResponse(DataSource data_source, Timeout timeout),
   * with your specified data_source value, and timeout specified as 10 years.
   */
  FetchAllResponse FetchAllBlocking(DataSource data_source);

  /**
   * Synchronously loads all achievement data for the currently signed-in
   * player, directly returning the FetchAllResponse. Specify timeout as an
   * arbitrary number of milliseconds. Not specifying data_source makes
   * this function call equivalent to calling
   * FetchAllResponse FetchAllBlocking(DataSource data_source, Timeout timeout),
   * with data_source specified as CACHE_OR_NETWORK, and timeout containing your
   * specified value.
   */
  FetchAllResponse FetchAllBlocking(Timeout timeout);

  /**
   * Synchronously loads all achievement data for the currently signed-in
   * player, directly returning the FetchAllResponse. Specify data_source
   * as CACHE_OR_NETWORK or NETWORK_ONLY. Specify timeout as an arbitrary
   * number of milliseconds.
   */
  FetchAllResponse FetchAllBlocking(DataSource data_source, Timeout timeout);

  /**
   * Contains data and response status for a single achievement.
   * @ingroup ResponseType
   */
  struct FetchResponse {
    /**
     * Can be: VALID, STALE, ERROR_LICENSE_CHECK_FAILED, ERROR_INTERNAL
     * ERROR_NOT_AUTHORIZED, or ERROR_TIMEOUT.
     * If the response is unsuccessful, FetchResponse's data vector will be
     * empty.
     */
    ResponseStatus status;

    /**
     * All data for a specific achievement.
     */
    Achievement data;
  };

  /**
   * Defines a callback type that receives a FetchResponse. This callback type
   * is provided to the Fetch(*) functions below.
   * @ingroup Callbacks
   */
  typedef std::function<void(FetchResponse const &)> FetchCallback;

  /**
   * Asynchronously loads data for a specific achievement for the currently
   * signed-in player.
   * Calls the provided FetchCallback on operation completion.
   * Not specifying data_source makes this function call equivalent to
   * calling Fetch(DataSource data_source, std::string const &id,
   * FetchCallback), with data_source specified as CACHE_OR_NETWORK.
   */
  void Fetch(std::string const &achievement_id, FetchCallback callback);

  /**
   * Asynchronously loads data for a specific achievement for the currently
   * signed-in player Calls the provided FetchCallback on operation completion.
   * Specify data_source as CACHE_OR_NETWORK or NETWORK_ONLY.
   */
  void Fetch(DataSource data_source,
             std::string const &achievement_id,
             FetchCallback callback);

  /**
   * Synchronously loads data for a specific achievement, identified by string
   * id, for the currently signed-in player; directly returns the FetchResponse.
   * Leaving data_source, and timeout unspecified makes this function call
   * equivalent to calling
   * FetchResponse FetchBlocking(DataSource data_source, Timeout timeout,
   * std::string const &id), with data_source specified as CACHE_OR_NETWORK,
   * and timeout specified as 10 years.
   */
  FetchResponse FetchBlocking(std::string const &achievement_id);

  /**
   * Synchronously loads data for a specific achievement, identified by string
   * id, for the currently signed-in player; directly returns the FetchResponse.
   * Specify data_source as CACHE_OR_NETWORK or NETWORK_ONLY. Leaving
   * timeout unspecified makes this function call equivalent to calling
   * FetchResponse FetchBlocking(DataSource data_source, Timeout timeout,
   * std::string const &id), with your specified data_source value, and timeout
   * specified as 10 years.
   */
  FetchResponse FetchBlocking(DataSource data_source,
                              std::string const &achievement_id);

  /**
   * Synchronously loads data for a specific achievement, identified by string
   * id, for the currently signed-in player; directly returns the FetchResponse.
   * Specify timeout as an arbitrary number of milliseconds. Leaving data_source
   * unspecified makes this function call equivalent to calling
   * FetchResponse FetchBlocking(DataSource data_source, Timeout timeout,
   * std::string const &id), with data_source specified as CACHE_OR_NETWORK and
   * timeout containing your specified value.
   */
  FetchResponse FetchBlocking(Timeout timeout,
                              std::string const &achievement_id);

  /**
   * Synchronously loads data for a specific achievement, identified by string
   * id, for the currently signed-in player; directly returns the FetchResponse.
   * Specify DataSource as CACHE_OR_NETWORK or NETWORK_ONLY. Specify
   * timeout as an arbitrary number of milliseconds.
   */
  FetchResponse FetchBlocking(DataSource data_source,
                              Timeout timeout,
                              std::string const &achievement_id);

  /**
   * Increments an achievement by the given number of steps. Leaving the
   * increment undefined causes its value to default to 1. The achievement
   * must be an incremental achievement. Once an achievement reaches the
   * maximum number of steps, it is unlocked automatically. Any further
   * increments are ignored.
   */
  void Increment(std::string const &achievement_id);

  /**
   * Increments an achievement by the given number of steps. The achievement
   * must be an incremental achievement. Once an achievement reaches at least
   * the maximum number of steps, it will be unlocked automatically. Any further
   * increments will be ignored.
   */
  void Increment(std::string const &achievement_id, uint32_t steps);

  /**
   * Reveal a hidden achievement to the currently signed-in player. If the
   * achievement has already been unlocked, this will have no effect.
   */
  void Reveal(std::string const &achievement_id);

  /**
   * Set an achievement to have at least the given number of steps completed.
   * Calling this method while the achievement already has more steps than the
   * provided value is a no-op. Once the achievement reaches the maximum number
   * of steps, the achievement is automatically unlocked, and any further
   * mutation operations are ignored.
   */
  void SetStepsAtLeast(std::string const &achievement_id, uint32_t steps);

  /**
   * Unlock an achievement for the currently signed in player. If the
   * achievement is hidden, this functions reveals it to the player.
   */
  void Unlock(std::string const &achievement_id);

  /**
   * Presents a UI to the user that displays information about all achievements.
   * Returns immediately when the user has dismissed the UI.
   */
  void ShowAllUI();

 private:
  AchievementManager(AchievementManager const &) = delete;
  AchievementManager &operator=(AchievementManager const &) = delete;

  GameServicesImpl *const impl_;
};

}  // namespace gpg

#endif  // GPG_ACHIEVEMENT_MANAGER_H_
