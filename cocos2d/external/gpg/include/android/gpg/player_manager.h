/**
 * @file gpg/player_manager.h
 * @copyright Copyright 2014 Google Inc. All Rights Reserved.
 * @brief Entry points for Play Games Player functionality.
 */

#ifndef GPG_PLAYER_MANAGER_H_
#define GPG_PLAYER_MANAGER_H_

#ifndef __cplusplus
#error Header file supports C++ only
#endif  // __cplusplus

#include <memory>
#include "gpg/common.h"
#include "gpg/game_services.h"
#include "gpg/player.h"
#include "gpg/types.h"

namespace gpg {

/**
 * Gets and sets various player-related data.
 * @ingroup Managers
 */
class GPG_EXPORT PlayerManager {
 public:
  /// An explicit constructor.
  explicit PlayerManager(GameServicesImpl *game_services_impl);
  ~PlayerManager();

  /**
   * Holds all player data, along with a response status.
   * @ingroup ResponseType
   */
  struct FetchSelfResponse {
    /**
     * Can be: VALID, STALE, ERROR_LICENSE_CHECK_FAILED, ERROR_INTERNAL
     * ERROR_NOT_AUTHORIZED, or ERROR_TIMEOUT.
     * If the response is unsuccessful, FetchSelfResponse data object comes
     * back empty.
     */
    ResponseStatus status;

    /**
     * Data associated with this player.
     */
    Player data;
  };

  /**
   * Defines a callback type that receives a FetchSelfResponse. This callback
   * type is provided to the FetchSelf(*) functions below.
   * @ingroup Callbacks
   */
  typedef std::function<void(FetchSelfResponse const &)> FetchSelfCallback;

  /**
   * Asynchronously loads all data for all currently signed-in players.
   * Calls the provided FetchSelfCallback on operation completion.
   * Not specifying data_sourcemakes this function call equivalent to calling
   * FetchSelf(DataSource data_source, FetchSelfCallback), with data_source
   * specified as CACHE_OR_NETWORK.
   */
  void FetchSelf(FetchSelfCallback callback);

  /**
   * Asynchronously loads all data for all currently signed-in players.
   * Calls the provided FetchSelfCallback on operation completion.
   * Specify data_source as CACHE_OR_NETWORK or NETWORK_ONLY.
   */
  void FetchSelf(DataSource data_source, FetchSelfCallback callback);

  /**
   * Synchronously loads all data for all currently signed-in players,
   * directly returning the FetchSelfResponse.
   * Not specifying data_source or timeout makes this function call
   * equivalent to calling
   * FetchSelfResponse FetchSelfBlocking(DataSource data_source, Timeout
   * timeout), DataSource specified as CACHE_OR_NETWORK, and timeout specified
   * as 10 years.
   */
  FetchSelfResponse FetchSelfBlocking();

  /**
   * Synchronously loads all data for all currently signed-in players,
   * directly returning the FetchSelfResponse. Specify data_source as
   * CACHE_OR_NETWORK or NETWORK_ONLY. Not specifying timeout makes this
   * function call equivalent to calling
   * FetchSelfResponse FetchSelfBlocking(DataSource data_source,
   * Timeout timeout), with your specified data_source value, and timeout
   * specified as 10 years.
   */
  FetchSelfResponse FetchSelfBlocking(DataSource data_source);

  /**
   * Synchronously loads all data for all currently signed-in players,
   * directly returning the FetchSelfResponse. Specify timeout as
   * an arbitrary number of milliseconds. Not specifying data_source makes this
   * function call equivalent to calling
   * FetchSelfResponse FetchSelfBlocking(DataSource data_source, Timeout
   * timeout), with data_source specified as CACHE_OR_NETWORK, and timeout
   * containing your specified value.
   */
  FetchSelfResponse FetchSelfBlocking(Timeout timeout);

  /**
   * Synchronously loads all data for all currently signed-in
   * players, directly returning the FetchSelfResponse. Specify
   * data_source as CACHE_OR_NETWORK or NETWORK_ONLY. Specify timeout as an
   * arbitrary number of milliseconds.
   */
  FetchSelfResponse FetchSelfBlocking(DataSource data_source, Timeout timeout);

  /**
   * @ingroup ResponseType
   */
  struct FetchResponse {
    ResponseStatus status;
    Player data;
  };

  /**
   * Defines a callback type that receives a FetchResponse. This callback type
   * is provided to the Fetch(*) functions below.
   * @ingroup Callbacks
   */
  typedef std::function<void(FetchResponse const &)> FetchCallback;

  /**
   * Asynchronously loads all data for a specific player.
   * Calls the provided FetchCallback on operation completion.
   * Not specifying data_source makes this function call equivalent to
   * calling Fetch(DataSource data_source std::string const &player_id,
   * FetchCallback callback), with data_source specified as CACHE_OR_NETWORK.
   */
  void Fetch(std::string const &player_id, FetchCallback callback);

  /**
   * Asynchronously loads all data for a specific player.
   * Calls the provided FetchCallback on operation completion.
   * Specify data_source as CACHE_OR_NETWORK or NETWORK_ONLY.
   */
  void Fetch(DataSource data_source,
             std::string const &player_id,
             FetchCallback callback);

  /**
   * Synchronously loads all data for a specific player, directly returning
   * the FetchResponse.
   * Specifying neither data_source nor timeout makes this
   * function call equivalent to calling
   * FetchResponse FetchBlocking(DataSource data_source, timeout timeout),
   * with DataSource specified as CACHE_OR_NETWORK, and timeout specified as 10
   * years.
   */
  FetchResponse FetchBlocking(std::string const &player_id);

  /**
   * Synchronously loads all data for a specific player, directly returning
   * the FetchResponse. Specify data_source as
   * CACHE_OR_NETWORK or NETWORK_ONLY. Leaving timeout unspecified makes this
   * function call equivalent to calling
   * FetchResponse FetchBlocking(DataSource data_source, Timeout timeout),
   * with your specified data_source value, and timeout specified as 10 years.
   */
  FetchResponse FetchBlocking(DataSource data_source,
                              std::string const &player_id);

  /**
   * Synchronously loads all data for a specific player, directly returning
   * the FetchResponse. Specify timeout as an arbitrary number of
   * milliseconds.
   * Leaving data_source unspecified makes this function call equivalent to
   * calling FetchResponse FetchBlocking(DataSource data_source, Timeout
   * timeout), with data_source specified as CACHE_OR_NETWORK and timeout,
   * containing your specified value.
   */
  FetchResponse FetchBlocking(Timeout timeout, std::string const &player_id);

  /**
   * Synchronously loads all data for a specific player, directly returning
   * returning the FetchResponse. Specify data_source
   * as CACHE_OR_NETWORK or NETWORK_ONLY. Specify timeout as an arbitrary number
   * of milliseconds.
   */
  FetchResponse FetchBlocking(DataSource data_source,
                              Timeout timeout,
                              std::string const &player_id);

 private:
  PlayerManager(PlayerManager const &) = delete;
  PlayerManager &operator=(PlayerManager const &) = delete;

  GameServicesImpl *const impl_;
};

}  // namespace gpg

#endif  // GPG_PLAYER_MANAGER_H_
