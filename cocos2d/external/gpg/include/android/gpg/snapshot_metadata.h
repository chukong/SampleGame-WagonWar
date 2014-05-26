/**
 * @file gpg/snapshot_metadata.h
 * @copyright Copyright 2014 Google Inc. All Rights Reserved.
 * @brief Value object that represents the completion of a task or goal.
 */

#ifndef GPG_SNAPSHOT_METADATA_H_
#define GPG_SNAPSHOT_METADATA_H_

#ifndef __cplusplus
#error Header file supports C++ only
#endif  // __cplusplus

#include <chrono>
#include <memory>
#include <string>
#include "gpg/common.h"
#include "gpg/types.h"

namespace gpg {

class SnapshotMetadataImpl;

/**
 * A single data structure that allows you to access data about the status of
 * a specific snapshot.
 * @ingroup ValueType
 */
class GPG_EXPORT SnapshotMetadata {
 public:
  SnapshotMetadata();
  /**
   * Constructs a <code>SnapshotMetadata</code> object from a
   * <code>shared_ptr</code> to a <code>SnapshotMetadataImpl</code>.
   * Intended for internal use by the API.
   */
  explicit SnapshotMetadata(std::shared_ptr<SnapshotMetadataImpl const> impl);

  /**
   * Creates a copy of an existing <code>SnapshotMetadata</code> object.
   */
  SnapshotMetadata(SnapshotMetadata const &copy_from);

  /**
   * Moves an existing <code>SnapshotMetadata</code> object.
   */
  SnapshotMetadata(SnapshotMetadata &&move_from);

  /**
   * Assigns this <code>SnapshotMetadata</code> object by copying from another
   * one.
   */
  SnapshotMetadata &operator=(SnapshotMetadata const &copy_from);

  /**
   * Assigns this <code>SnapshotMetadata</code> object by moving another one
   * into it.
   */
  SnapshotMetadata &operator=(SnapshotMetadata &&move_from);
  ~SnapshotMetadata();

  /**
   * Returns true when the returned snapshot is populated with data and is
   * accompanied by a successful response status; false for an
   * unpopulated user-created snapshot or for a populated one accompanied by
   * an unsuccessful response status.
   * This function must return true for the getter functions (<code>id</code>,
   * <code>Name</code>, <code>Description</code>, etc.) on this snapshot to be
   * usable.
   */
  bool Valid() const;

  /**
   * Returns the title of the snapshot.
   */
  std::string const &Title() const;

  // TODO(jleahey): Update with recommended description (b/14966475).
  /**
   * Returns a concise description of your snapshot.
   * <code>SnapshotMetadata::Valid()</code> must return true for this function
   * to be usable.
   */
  std::string const &Description() const;

  /**
   * Returns the duration associated with this snapshot.
   */
  gpg::Duration Duration() const;

  /**
   * Returns the time at which the entry was last modified (expressed as
   * milliseconds since the Unix epoch.)
   */
  Timestamp LastModifiedTime() const;

  /**
   * Returns a concise description of your snapshot.
   * <code>SnapshotMetadata::Valid()</code> must return true for this function
   * to be usable.
   */
  SnapshotMetadata const &Metadata() const;

 private:
  friend class SnapshotMetadataImpl;
  std::shared_ptr<SnapshotMetadataImpl const> impl_;
};

}  // namespace gpg

#endif  // GPG_SNAPSHOT_METADATA_H_
