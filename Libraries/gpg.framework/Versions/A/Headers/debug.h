/**
 * @brief Helper functions that support logging of Game Services types.
 *
 * @brief DebugString is provided for major public types, converting them to
 * log-friendly strings.
 *
 * @brief Additionally, operator<< is provided for each of these types for easy
 * use with streams.
 *
 * @note These strings are for use only in logging and debugging. They are
 * not intended as user-facing output.
 *
 * @file gpg/debug.h
 *
 * @copyright Copyright 2014 Google Inc. All Rights Reserved.
 *
 */

#ifndef GPG_DEBUG_H_
#define GPG_DEBUG_H_

#ifndef __cplusplus
#error Header file supports C++ only
#endif  // __cplusplus

#include <string>
#include "gpg/achievement.h"
#include "gpg/common.h"
#include "gpg/leaderboard.h"
#include "gpg/player.h"
#include "gpg/score.h"
#include "gpg/score_page.h"
#include "gpg/status.h"
#include "gpg/types.h"

namespace gpg {

// Achievements

/// Returns a human-readable achievement type.
std::string GPG_EXPORT DebugString(AchievementType type);

/// Returns a human-readable achievement state.
std::string GPG_EXPORT DebugString(AchievementState state);

/// Returns a human-readable achievement.
std::string GPG_EXPORT DebugString(Achievement const &achievement);

/// Writes a human-readable achievement type to an output stream.
std::ostream GPG_EXPORT &operator<<(std::ostream &os, AchievementType type);

/// Writes a human-readable achievement state to an output stream.
std::ostream GPG_EXPORT &operator<<(std::ostream &os, AchievementState state);

/// Writes a human-readable achievement to an output stream.
std::ostream GPG_EXPORT &operator<<(std::ostream &os,
                                    Achievement const &achievement);

// Leaderboards

/// Returns the value of order in human-readable form.
std::string GPG_EXPORT DebugString(LeaderboardOrder order);

/// Returns all leaderboard data in human-redable form.
std::string GPG_EXPORT DebugString(Leaderboard const &leaderboard);

/// Returns the value of start in human-readable form.
std::string GPG_EXPORT DebugString(LeaderboardStart start);

/// Returns the value of time_span in human-readable form.
std::string GPG_EXPORT DebugString(LeaderboardTimeSpan time_span);

/// Returns the value of collection in human-readable form.
std::string GPG_EXPORT DebugString(LeaderboardCollection collection);

/// Returns all score data in human-readable form.
std::string GPG_EXPORT DebugString(Score const &score);

/// Returns all score-page data in human-readable form.
std::string GPG_EXPORT DebugString(ScorePage const &score_page);

/// Returns all score entries in human-readable form.
std::string GPG_EXPORT DebugString(ScorePage::Entry const &entry);

/// Provided for easy use of the corresponding debug string with streams.
std::ostream GPG_EXPORT &operator<<(std::ostream &os, LeaderboardOrder order);

/// Provided for easy use of the corresponding debug string with streams.
std::ostream GPG_EXPORT &operator<<(std::ostream &os, Leaderboard const &order);

/// Provided for easy use of the corresponding debug string with streams.
std::ostream GPG_EXPORT &operator<<(std::ostream &os, LeaderboardStart start);

/// Provided for easy use of the corresponding debug string with streams.
std::ostream GPG_EXPORT &operator<<(std::ostream &os,
                                    LeaderboardTimeSpan time_span);

/// Provided for easy use of the corresponding debug string with streams.
std::ostream GPG_EXPORT &operator<<(std::ostream &os,
                                    LeaderboardCollection collection);

/// Provided for easy use of the corresponding debug string with streams.
std::ostream GPG_EXPORT &operator<<(std::ostream &os, Score const &score);

/// Provided for easy use of the corresponding debug string with streams.
std::ostream GPG_EXPORT &operator<<(std::ostream &os,
                                    ScorePage const &score_page);

/// Provided for easy use of the corresponding debug string with streams.
std::ostream GPG_EXPORT &operator<<(std::ostream &os,
                                    ScorePage::Entry const &entry);

// Players

/// Returns all player data in human-readable form.
std::string GPG_EXPORT DebugString(Player const &player);

/// Provided for easy use of the corresponding debug string with streams.
std::ostream GPG_EXPORT &operator<<(std::ostream &os, Player const &player);

// Status

/// Returns authorization status in human-readable form.
std::string GPG_EXPORT DebugString(AuthStatus status);

/// Returns response status in human-readable form.
std::string GPG_EXPORT DebugString(ResponseStatus status);

/// Returns flush status in human-redable form.
std::string GPG_EXPORT DebugString(FlushStatus status);

/// Provided for easy use of the corresponding debug string with streams.
std::ostream GPG_EXPORT &operator<<(std::ostream &os, AuthStatus status);

/// Provided for easy use of the corresponding debug string with streams.
std::ostream GPG_EXPORT &operator<<(std::ostream &os, ResponseStatus status);

/// Provided for easy use of the corresponding debug string with streams.
std::ostream GPG_EXPORT &operator<<(std::ostream &os, FlushStatus status);

// Types

/// Returns data source in human-readable form.
std::string GPG_EXPORT DebugString(DataSource source);

/// Returns log level in human-readable form.
std::string GPG_EXPORT DebugString(LogLevel level);

/// Returns authorization operation in human-readable form.
std::string GPG_EXPORT DebugString(AuthOperation op);

/// Provided for easy use of the corresponding debug string with streams.
std::ostream GPG_EXPORT &operator<<(std::ostream &os, DataSource status);

/// Provided for easy use of the corresponding debug string with streams.
std::ostream GPG_EXPORT &operator<<(std::ostream &os, LogLevel status);

/// Provided for easy use of the corresponding debug string with streams.
std::ostream GPG_EXPORT &operator<<(std::ostream &os, AuthOperation op);

}  // namespace gpg

#endif  // GPG_DEBUG_H_
