/**
 * @file gpg/snapshot.h
 * @copyright Copyright 2014 Google Inc. All Rights Reserved.
 * @brief Value object that represents the completion of a task or goal.
 */

#ifndef GPG_SNAPSHOT_H_
#define GPG_SNAPSHOT_H_

#ifndef __cplusplus
#error Header file supports C++ only
#endif  // __cplusplus

#include <chrono>
#include <memory>
#include <string>
#include "gpg/common.h"
#include "gpg/types.h"

namespace gpg {

class SnapshotImpl;
class SnapshotMetadata;

/**
 * A single data structure that allows you to access data about the status of
 * a specific snapshot.
 * @ingroup ValueType
 */
class GPG_EXPORT Snapshot {
 public:
  Snapshot();
  /**
   * Constructs a <code>Snapshot</code> from a <code>shared_ptr</code> to a
   * <code>Snapshot</code>.
   * Intended for internal use by the API.
   */
  explicit Snapshot(std::shared_ptr<SnapshotImpl const> impl);

  /**
   * Creates a copy of an existing <code>Snapshot</code>.
   */
  Snapshot(Snapshot const &copy_from);

  /**
   * Moves an existing <code>Snapshot</code>.
   */
  Snapshot(Snapshot &&move_from);

  /**
   * Assigns this <code>Snapshot</code> by copying from another one.
   */
  Snapshot &operator=(Snapshot const &copy_from);

  /**
   * Assigns this <code>Snapshot</code> by moving another one into it.
   */
  Snapshot &operator=(Snapshot &&move_from);

  ~Snapshot();

  /**
   * Returns true when the returned snapshot is populated with data and is
   * accompanied by a successful response status; false for an
   * unpopulated user-created snapshot or for a populated one accompanied by
   * an unsuccessful response status.
   * This function must return true for the getter functions
   * (<code>Description</code>, etc.) on this snapshot to be
   * usable.
   */
  bool Valid() const;

  /**
   * Returns a concise description of your snapshot. Up to 500 characters.
   * <code>Snapshot::Valid()</code must return true for this function to be
   * usable.
   */
  std::string const &Description() const;

  /**
   * All of the metadata for this snapshot.
   */
  SnapshotMetadata Metadata() const;

 private:
  std::shared_ptr<SnapshotImpl const> impl_;
};

}  // namespace gpg

#endif  // GPG_SNAPSHOT_H_
