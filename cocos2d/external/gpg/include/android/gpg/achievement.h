/**
 * @file gpg/achievement.h
 * @copyright Copyright 2014 Google Inc. All Rights Reserved.
 * @brief Value object that represents the completion of a task or goal.
 */

#ifndef GPG_ACHIEVEMENT_H_
#define GPG_ACHIEVEMENT_H_

#ifndef __cplusplus
#error Header file supports C++ only
#endif  // __cplusplus

#include <chrono>
#include <memory>
#include <string>
#include "gpg/common.h"
#include "gpg/types.h"

namespace gpg {

class AchievementImpl;

/**
 * A single data structure which allows you to access data about the status of
 * a specific achievement. Data comprise two types: user-specific (e.g.,
 * whether the user has unlocked the achievement, etc.) and global (e.g.,
 * achievement name) achievement has been revealed, unlocked, etc.) and global
 * (e.g., achievement name).
 * @ingroup ValueType
 */
class GPG_EXPORT Achievement {
 public:
  Achievement();
  /**
   * Explicit constructor.
   */
  explicit Achievement(std::shared_ptr<AchievementImpl const> impl);

  /**
   * Copy constructor for copying an existing achievement into a new one.
   */
  Achievement(Achievement const &copy_from);

  /**
   * Constructor for moving an existing achievement into a new one.
   * r-value-reference version.
   */
  Achievement(Achievement &&move_from);

  /**
   * Assignment operator for assigning this achievement's value from another
   * achievement.
   */
  Achievement &operator=(Achievement const &copy_from);

  /**
   * Assignment operator for assigning this achievement's value from another
   * achievement.
   * r-value-reference version.
   */
  Achievement &operator=(Achievement &&move_from);
  ~Achievement();

  /**
   * Returns true when the returned achievement is populated with data and is
   * accompanied by a successful response status; false for an
   * unpopulated user-created achievement or for a populated one accompanied by
   * an unsuccessful response status.
   * It must be true for the getter functions on this achievement (id, Name,
   * Description, etc.) to be usable.
   */
  bool Valid() const;

  /**
   * Returns the unique string that the Google Play Developer Console generated
   * beforehand. Use it to refer to an achievement in your game client.
   * It can only be called when Achievement::Valid() returns true.
   * More information is available at
   * https://developers.google.com/games/services/console/enabling#client_id .
   */
  std::string const &Id() const;

  /**
   * Returns the short name of the achievement. Up to 100 characters.
   * It can only be called when Achievement::Valid() returns true.
   */
  std::string const &Name() const;

  /**
   * Returns a concise description of your achievement. Usually tells player how
   * to earn the achievement. Up to 500 characters.
   * It can only be called when Achievement::Valid() returns true.
   */
  std::string const &Description() const;

  /**
   * Returns the achievement type: incremental or standard.
   * It can only be called when Achievement::Valid() returns true. More
   * information is available at
   * https://developers.google.com/games/services/common/concepts/achievements#incremental_achievements .
   */
  AchievementType Type() const;

  /**
   * Returns the achievement state: hidden, revealed, or unlocked.
   * It can only be called when Achievement::Valid() returns true. More
   * information is available at
   * https://developers.google.com/games/services/common/concepts/achievements#state .
   */
  AchievementState State() const;

  /**
   * Returns the number of steps the player has taken toward unlocking an
   * incremental achievement.
   * It can only be called when Achievement::Valid() returns true.
   */
  uint32_t CurrentSteps() const;

  /**
   * Returns the number of steps required, in total, for the player to unlock a
   * given incremental achievement.
   * It can only be called when Achievement::Valid() returns true.
   */
  uint32_t TotalSteps() const;

    /**
     * Returns time at which the entry was last modified (expressed as
     * milliseconds since the Unix epoch).
     * It can only be called when Achievement::Valid() returns true.
     */

  std::chrono::milliseconds LastModified() const;

 private:
  std::shared_ptr<AchievementImpl const> impl_;
};

}  // namespace gpg

#endif  // GPG_ACHIEVEMENT_H_
