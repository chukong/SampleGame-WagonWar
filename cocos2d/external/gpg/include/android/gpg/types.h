/**
 * @file gpg/types.h
 * @copyright Copyright 2014 Google Inc. All Rights Reserved.
 * @brief Assorted types.
 */

#ifndef GPG_TYPES_H_
#define GPG_TYPES_H_

#ifndef __cplusplus
#error Header file supports C++ only
#endif  // __cplusplus

// GCC Has a bug in which it throws spurios errors on "enum class" types
// when using __attribute__ with them. Suppress this.
// http://gcc.gnu.org/bugzilla/show_bug.cgi?id=43407
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif

#include <chrono>
#include "gpg/common.h"

namespace gpg {

/**
 * Values specifying where to get data from when retrieving achievement,
 * leaderboard, and other data.
 * When the setting is CACHE_OR_NETWORK, the system resorts to the local
 * cache when no network connection is available.
 */
enum class GPG_EXPORT DataSource {
  CACHE_OR_NETWORK,
  NETWORK_ONLY
};

/// Data type used in specifying timeout periods for attempted operations.
typedef std::chrono::milliseconds Timeout;

/// Values used to specify the level of logging.
enum class GPG_EXPORT LogLevel {
  VERBOSE,
  INFO,
  WARNING,
  ERROR
};

/// Values used to specify the type of authorization operation to perform.
enum class GPG_EXPORT AuthOperation {
  SIGN_IN,
  SIGN_OUT
};

/*
 * Values used to specify achievement type.
 * A player makes gradual progress (steps) toward an incremental achievement.
 * He or she completes a standard achievement in a single step.
 */
enum class GPG_EXPORT AchievementType {
  STANDARD,
  INCREMENTAL
};

enum class GPG_EXPORT AchievementState {
  // These are ordered such that only increasing transitions are possible
/*
 * Values used to specify achievement state.
 * A hidden achievement is one whose existence a player has not yet discovered.
 * make him or her aware of it.
 * A revealed achievement is one that the player knows about, but has not yet
 * earned.
 * An unlocked achievement is one that the player has earned.
 */
  HIDDEN,
  REVEALED,
  UNLOCKED,
};

// @addaon: Larger/smaller what? Score?
enum class GPG_EXPORT LeaderboardOrder {
  LARGER_IS_BETTER,
  SMALLER_IS_BETTER
};

/// Values specifying whether rankings are displayed on a leaderboard in order
/// of score or player.
enum class GPG_EXPORT LeaderboardStart {
  TOP_SCORES,
  PLAYER_CENTERED
};

/// Values that specify the period of time that a leaderboard covers.
enum class GPG_EXPORT LeaderboardTimeSpan {
  DAILY,
  WEEKLY,
  ALL_TIME
};

/// Values that show whether a leaderboard can be viewed by anyone with a
/// Google+ account (public), or only members of a player's circles (social).
// @addaon: I'm taking a wild stab at this.
enum class GPG_EXPORT LeaderboardCollection {
  PUBLIC,
  SOCIAL
};

}  // namespace gpg

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop  // GCC diagnostic ignored "-Wattributes"
#endif

#endif  // GPG_TYPES_H_
