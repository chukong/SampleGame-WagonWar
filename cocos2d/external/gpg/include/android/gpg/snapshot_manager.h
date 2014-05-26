/**
 * @file gpg/snapshot_manager.h
 * @copyright Copyright 2014 Google Inc. All Rights Reserved.
 * @brief Value object that represents the completion of a task or goal.
 */

#ifndef GPG_SNAPSHOT_MANAGER_H_
#define GPG_SNAPSHOT_MANAGER_H_

#ifndef __cplusplus
#error Header file supports C++ only
#endif  // __cplusplus

#include <memory>
#include <vector>
#include "gpg/common.h"
#include "gpg/game_services.h"
#include "gpg/snapshot.h"
#include "gpg/snapshot_metadata.h"
#include "gpg/snapshot_metadata_change.h"
#include "gpg/types.h"

namespace gpg {

/**
 * For Snapshots to be used, the app must request kSnapshotScope scope (equal to
 * <code>https://www.googleapis.com/auth/drive.appdata</code>) at authentication
 * time.
 * This can be done via {@link GameServices::Builder::EnableSnapshots}.
 */
extern std::string const kSnapshotScope;

/**
 * Gets and sets various snapshot-related data. If the app does not enable
 * snapshots at authentication time (see
 * {@link GameServices::Builder::EnableSnapshots}), most methods on
 * <code>SnapshotManager</code> will fail.
 *
 * @ingroup Managers
 */
class GPG_EXPORT SnapshotManager {
 public:
  /**
   * Constructs a <code>SnapshotManager</code> from a
   * <code>GameServicesImpl</code>. This function is not intended to be called
   * by consumers of this API. Instead, an app should retrieve the
   * <code>SnapshotManager</code> via the
   * <code>GameServices::SnapshotManager</code> call.
   */
  explicit SnapshotManager(GameServicesImpl *game_services_impl);
  ~SnapshotManager();

  /**
   * Holds all data for all snapshots, along with a response status.
   * @ingroup ResponseType
   */
  struct FetchAllResponse {
    /**
     * Can be one of the values enumerated in {@link ResponseStatus}.
     * If the response is unsuccessful, <code>FetchAllResponse</code>'s data
     * vector is empty.
     */

    ResponseStatus status;

    /**
     * A vector containing metadata for all snapshots associated with the
     * application.
     */
    std::vector<SnapshotMetadata> data;
  };

  /**
   * Defines a callback type that receives a <code>FetchAllResponse</code>. This
   * callback type is provided to the <code>FetchAll(*)</code> functions below.
   * @ingroup Callbacks
   */
  typedef std::function<void(FetchAllResponse const &)> FetchAllCallback;

  /**
   * Asynchronously loads all snapshot data for the currently signed-in
   * player. Calls the provided <code>FetchAllCallback</code> upon operation
   * completion. Not specifying <code>data_source</code> makes this function
   * call equivalent to calling
   * <code>FetchAll(DataSource data_source, FetchAllCallback)</code>, with
   * <code>data_source</code> specified as <code>CACHE_OR_NETWORK.</code>
   */
  void FetchAll(FetchAllCallback callback);

  /**
   * Asynchronously loads all snapshot data for the currently signed-in
   * player. Calls the provided <code>FetchAllCallback</code> upon operation
   * completion. Specify <code>data_source</code> as
   * <code>CACHE_OR_NETWORK</code> or <code>NETWORK_ONLY</code>.
   */
  void FetchAll(DataSource data_source, FetchAllCallback callback);

  /**
   * Synchronously loads all snapshot data for the currently signed-in
   * player, directly returning the <code>FetchAllResponse</code>. Specifying
   * neither <code>data_source</code> nor <code>timeout</code> makes this
   * function call equivalent to calling
   * <code>FetchAllResponse FetchAllBlocking(DataSource data_source, </code>
   * <code>Timeout timeout)</code>,
   * with <code>data_source</code> specified as
   * <code>CACHE_OR_NETWORK</code>, and <code>timeout</code> specified as 10
   * years.
   */
  FetchAllResponse FetchAllBlocking();

  /**
   * Synchronously loads all snapshot data for the currently signed-in
   * player, directly returning the <code>FetchAllResponse</code>.
   * Specify <code>data_source</code> as <code>CACHE_OR_NETWORK</code> or
   * <code>NETWORK_ONLY</code>. Not specifying <code>timeout</code> makes this
   * function call equivalent to calling
   * <code>FetchAllBlocking FetchAllResponse(DataSource data_source, </code>
   * <code>Timeout timeout)</code>,
   * with your specified <code>data_source</code> value, and <code>timeout</code>
   * specified as 10 years.

   */
  FetchAllResponse FetchAllBlocking(DataSource data_source);

  /**
   * Synchronously loads all snapshot data for the currently signed-in
   * player, directly returning the <code>FetchAllResponse</code>. Specify
   * <code>timeout</code> as an arbitrary number of milliseconds.
   * Not specifying <code>data_source</code> makes this function call equivalent
   * to calling
   * <code>FetchAllResponse FetchAllBlocking(DataSource data_source, </code>
   * <code>Timeout timeout)</code>,
   * with <code>data_source</code> specified as <code>CACHE_OR_NETWORK</code>,
   * and <code>timeout</code> containing your specified value.
   */
  FetchAllResponse FetchAllBlocking(Timeout timeout);

  /**
   * Synchronously loads all snapshot data for the currently signed-in
   * player, directly returning the <code>FetchAllResponse</code>. Specify
   * <code>data_source</code> as <code>CACHE_OR_NETWORK</code> or
   * <code>NETWORK_ONLY</code>. Specify timeout as an arbitrary number of
   * milliseconds.

   */
  FetchAllResponse FetchAllBlocking(DataSource data_source, Timeout timeout);

  /**
   * Holds the data for a particular requested snapshot along with a response
   * status. If the snapshot is in a conflicted state, <code>conflict_id</code>
   * stores a string identifying the conflict. The conflict must be resolved
   * before the snapshot can receive future modifications.
   *
   * @ingroup ResponseType
   */
  struct OpenResponse {
    /**
     * Can be one of the values enumerated in {@link ResponseStatus}.
     * If the response is unsuccessful, <code>OpenResponse</code>'s data vector
     * is empty.
     */
    ResponseStatus status;

    /**
     * A <code>Snapshot</code>.
     */
    Snapshot data;

    /**
     * The identifier of this conflict. If this string is empty, there is no
     * conflict.
     */
    std::string conflict_id;

    /**
     * Empty if <code>conflict_id</code> is empty.
     */
    Snapshot conflict;
  };

  /**
   * Defines a callback type that receives an <code>OpenResponse</code>. This
   * callback type is provided to the <code>Open(*)</code> functions below.
   * @ingroup Callbacks
   */
  typedef std::function<void(OpenResponse const &)> OpenCallback;

  /**
   * Asynchronously loads and opens a snapshot for modification by the dev.
   * Calls the provided <code>OpenCallback</code> upon operation completion.
   * Not specifying <code>data_source</code> makes this function call equivalent
   * to calling <code>Open(DataSource data_source, OpenCallback)</code>, with
   * <code>data_source</code> specified as <code>CACHE_OR_NETWORK</code>.
   */
  void Open(std::string const &snapshot_id, OpenCallback callback);

  /**
   * Asynchronously loads and opens a snapshot for modification by the dev.
   * Calls the provided <code>OpenCallback</code> upon operation completion.
   * Specify <code>data_source</code> as <code>CACHE_OR_NETWORK</code> or
   * <code>NETWORK_ONLY</code>.
   */
  void Open(DataSource data_source,
            std::string const &snapshot_id,
            OpenCallback callback);

  /**
   * Synchronously loads and opens a snapshot for modification,
   * directly returning the <code>OpenResponse</code>. Specifying neither
   * <code>data_source</code> nor <code>timeout</code> makes this function call
   * equivalent to calling
   * <code>OpenResponse OpenBlocking(DataSource data_source, </code>
   * <code>Timeout timeout)</code>,
   * with <code>data_source</code> specified as <code>CACHE_OR_NETWORK</code>,
   * and <code>timeout</code> specified as 10
   * years.
   */
  OpenResponse OpenBlocking(std::string const &snapshot_id);

  /**
   * Synchronously loads and opens a snapshot for modification,
   * directly returning the <code>OpenResponse</code>. Specify
   * <code>data_source</code> as <code>CACHE_OR_NETWORK</code> or
   * <code>NETWORK_ONLY</code>. Not specifying <code>timeout</code> makes this
   * function call equivalent to calling
   * <code>OpenBlocking OpenResponse(DataSource data_source, </code>
   * <code>Timeout timeout),</code>
   * with your specified <code>data_source</code> value,
   * and <code>timeout</code> specified as 10 years.
   */
  OpenResponse OpenBlocking(
      DataSource data_source, std::string const &snapshot_id);

  /**
   * Synchronously loads and opens a snapshot for modification,
   * directly returning the <code>OpenResponse</code>. Specify
   * <code>timeout</code> as an arbitrary number of milliseconds. Not specifying
   * <code>data_source</code> makes this function call equivalent to calling
   * OpenResponse OpenBlocking(DataSource data_source, </code>
   * <code>Timeout timeout)</code>,
   * with <code>data_source</code> specified as <code>CACHE_OR_NETWORK</code>,
   * and <code>timeout</code> containing your
   * specified value.
   */
  OpenResponse OpenBlocking(Timeout timeout, std::string const &snapshot_id);

  /**
   * Synchronously loads and opens a snapshot for modification,
   * directly returning the <code>OpenResponse</code>. Specify
   * <code>data_source</code> as <code>CACHE_OR_NETWORK</code> or
   * <code>NETWORK_ONLY</code>. Specify <code>timeout</code> as an arbitrary
   * number of milliseconds.
   */
  OpenResponse OpenBlocking(
      DataSource data_source, Timeout timeout, std::string const &snapshot_id);

  /**
   * Holds the data for an updated snapshot, along with a response status.
   * @ingroup ResponseType
   */
  struct CommitResponse {
    /**
     * Can be one of the values enumerated in {@link ResponseStatus}.
     * If the response is unsuccessful, <code>CommitResponse</code>'s
     * data is empty.
     */
    ResponseStatus status;

    /**
     * A <code>SnapshotMetadata</code> object.
     */
    SnapshotMetadata data;
  };

  /// UNDOCUMENTED
  typedef std::function<void(CommitResponse const &)> CommitCallback;

  /**
   * Asynchronously commits the data provided to the snapshot, and updates the
   * metadata of the snapshot using the provided metadata object.
   * Calls the provided <code>CommitCallback</code> upon operation completion.
   */
  void Commit(
      Snapshot const &snapshot,
      SnapshotMetadataChange const &metadata_change,
      std::vector<uint8_t> data,
      CommitCallback callback);

  /**
   * Synchronously commits the data provided to the snapshot, and updates the
   * metadata of the snapshot using the provided metadata object.
   * Calls the provided <code>CommitCallback</code> upon operation completion.
   * Not setting <code>timeout</code> results in a <code>timeout</code>
   * specified as 10 years.
   */
  CommitResponse CommitBlocking(
      Snapshot const &snapshot,
      SnapshotMetadataChange const &metadata_change,
      std::vector<uint8_t> data);

  /**
   * Synchronously commits the data provided to the snapshot and updates the
   * metadata of the snapshot using the provided metadata object.
   * Calls the provided <code>CommitCallback</code> upon operation completion.
   * Not setting <code>timeout</code> will result in a <code>timeout</code>
   * specified as 10 years.
   */
  CommitResponse CommitBlocking(
      Timeout timeout,
      Snapshot const &snapshot,
      SnapshotMetadataChange const &metadata_change,
      std::vector<uint8_t> data);

  /**
   * Holds the status for a delete request.
   * @ingroup ResponseType
   */
  struct DeleteResponse {
    /**
     * Can be one of the values enumerated in {@link ResponseStatus}.
     * If the response is unsuccessful, <code>CommitResponse</code>'s
     * data is empty.
     */
    ResponseStatus status;
  };

  /**
   * Defines a callback type that receives an <code>DeleteResponse</code>. This
   * callback type is provided to the <code>Delete(*)</code>
   * functions below.
   * @ingroup Callbacks
   */
  typedef std::function<void(DeleteResponse const &)> DeleteCallback;

  /**
   * Asynchronously loads and deletes a snapshot by the dev.
   * Calls the provided <code>DeleteCallback</code> upon operation completion.
   * Not specifying <code>data_source</code> makes this function call equivalent
   * to calling <code>Delete(DataSource data_source, DeleteCallback)</code>,
   * with <code>data_source</code> specified as <code>CACHE_OR_NETWORK</code>.
   */
  void Delete(std::string const &snapshot_id, DeleteCallback callback);

  /**
   * Asynchronously loads and deletes a snapshot by the dev.
   * Calls the provided <code>DeleteCallback</code> upon operation completion.
   * Specify data_source as <code>CACHE_OR_NETWORK</code> or
   * <code>NETWORK_ONLY</code>.
   */
  void Delete(DataSource data_source,
              std::string const &snapshot_id,
              DeleteCallback callback);

  /**
   * Synchronously loads and deletes a snapshot,
   * directly returning the DeleteResponse. Specifying neither
   * <code>data_source</code> nor <code>timeout</code> makes this function call
   * equivalent to calling
   * <code>DeleteResponse DeleteBlocking(DataSource data_source, </code>
   * <code>Timeout timeout)</code>,
   * with <code>data_source</code> specified as <code>CACHE_OR_NETWORK</code>,
   * and <code>timeout</code> specified as 10
   * years.
   */
  DeleteResponse DeleteBlocking(std::string const &snapshot_id);

  /**
   * Synchronously loads and deletes a snapshot,
   * directly returning the <code>DeleteResponse</code>. Specify
   * <code>data_source</code> as <code>CACHE_OR_NETWORK</code> or
   * <code>NETWORK_ONLY</code>. Not specifying <code>timeout</code> makes this
   * function call equivalent to calling
   * <code>DeleteBlocking DeleteResponse(DataSource data_source, </code>
   * <code>Timeout timeout)</code>,
   * with your specified <code>data_source</code> value, and
   * <code>timeout</code> specified as 10 years.
   */
  DeleteResponse DeleteBlocking(
      DataSource data_source, std::string const &snapshot_id);

  /**
   * Synchronously loads and deletes a snapshot,
   * directly returning the <code>DeleteResponse</code>. Specify
   * <code>timeout</code> as an arbitrary number of milliseconds. Not specifying
   * <code>data_source</code> makes this function call equivalent to calling
   * <code>DeleteResponse DeleteBlocking(DataSource data_source, </code>
   * <code>Timeout timeout)</code>,
   * with <code>data_source</code> specified as <code>CACHE_OR_NETWORK</code>,
   * and <code>timeout</code> containing your specified value.
   */
  DeleteResponse DeleteBlocking(Timeout timeout,
                                std::string const &snapshot_id);

  /**
   * Synchronously loads and deletes a snapshot,
   * directly returning the <code>DeleteResponse</code>. Specify data_source
   * as <code>CACHE_OR_NETWORK</code> or <code>NETWORK_ONLY</code>. Specify
   * <code>timeout</code> as an arbitrary number of milliseconds.
   */
  DeleteResponse DeleteBlocking(
      DataSource data_source, Timeout timeout, std::string const &snapshot_id);

  /**
   * Reads.
   * @ingroup ResponseType
   */
  struct ReadResponse {
    /**
     * Can be one of the values enumerated in {@link ResponseStatus}.
     * If the response is unsuccessful, <code>ReadResponse</code>'s data vector
     * is empty.
     */
    ResponseStatus status;

    /**
     * A vector containing the data in the snapshot.
     */
    std::vector<uint8_t> data;
  };

  /**
   * Defines a callback type that receives a <code>ReadResponse</code>.
   * This callback type is provided to the <code>Read(*)</code> functions below.
   * @ingroup Callbacks
   */
  typedef std::function<void(ReadResponse const &)> ReadCallback;

  /**
   * Asynchronously reads a snapshot off of the disk, and copies it into memory.
   * The data is passed back by value for easier modification. Each call to this
   * function results in a full read. This means that it is typically best only
   * to read a snapshot once.
   * Calls the provided <code>ReadCallback</code> upon operation completion.
   */
  void Read(Snapshot const &snapshot, ReadCallback callback);

  /**
   * Synchronously reads a snapshot off of the disk, and copies it into memory.
   * The data is passed back by value for easier modification. Each call to this
   * function results in a full read. This means that it is typically best only
   * to read a snapshot once. Not specifying <code>timeout</code> makes this
   * function call equivalent to calling
   * <code>ReadBlocking ReadBlocking(Timeout timeout, </code>
   * <code>Snapshot const &snapshot)</code>,
   * with <code>timeout</code> specified as 10 years.
   */
  ReadResponse ReadBlocking(Snapshot const &snapshot);

  /**
   * Synchronously reads a snapshot off of the disk and copies it into memory.
   * The data is passed back by value for easier modification. Each call to this
   * does a full read so typically only read a snapshot once.
   */
  ReadResponse ReadBlocking(Timeout timeout, Snapshot const &snapshot);

 private:
  SnapshotManager(SnapshotManager const &) = delete;
  SnapshotManager &operator=(SnapshotManager const &) = delete;

  GameServicesImpl *const impl_;
};

}  // namespace gpg

#endif  // GPG_SNAPSHOT_MANAGER_H_
