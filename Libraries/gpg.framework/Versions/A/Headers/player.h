/**
 * @file gpg/player.h
 * @copyright Copyright 2014 Google Inc. All Rights Reserved.
 * @brief Value object that represents a player.
 */

#ifndef GPG_PLAYER_H_
#define GPG_PLAYER_H_

#ifndef __cplusplus
#error Header file supports C++ only
#endif  // __cplusplus

#include <memory>
#include <string>
#include "gpg/common.h"

namespace gpg {

class PlayerImpl;

/**
 * A single data structure containing data about a specific player.
 * @ingroup ValueType
 */
class GPG_EXPORT Player {
 public:
  Player();

  /**
   * Explicit constructor.
   */
  explicit Player(std::shared_ptr<PlayerImpl const> impl);

  /**
   * Copy constructor for copying an existing player into a new one.
   */
  Player(Player const &copy_from);

  /**
   * Constructor for moving an existing player into a new one.
   * r-value-reference version.
   */
  Player(Player &&move_from);

  /**
   * Assignment operator for assigning this achievement's value from
   * another achievement.
   */
  Player &operator=(Player const &copy_from);

  /**
   * Assignment operator for assigning this achievement's value
   * from another achievement.
   * r-value-reference version
   */
  Player &operator=(Player &&move_from);
  ~Player();

  /**
   * Returns true when the returned player is populated with data and is
   * accompanied by a successful response status; false for an
   * unpopulated user-created player or for a populated one accompanied by
   * an unsuccessful response status.
   * It must be true for the getter functions on this achievement (id, Name,
   * etc.) to be usable.
   */
  bool Valid() const;

  /**
   * Returns the ID of the currently signed-in player.
   * It can only be called when Player::Valid() returns true.
   */
  std::string const &Id() const;
  /**
   * Returns the Google+ name of the currently signed-in player.
   * It can only be called when Player::Valid() returns true.
   */
  std::string const &Name() const;

  /**
   * Returns the URL leading to the image of the currently signed-in player's
   * avatar.
   * It can only be called when Player::Valid() returns true.
   */

 private:
  std::shared_ptr<PlayerImpl const> impl_;
};

}  // namespace gpg

#endif  // GPG_PLAYER_H_
