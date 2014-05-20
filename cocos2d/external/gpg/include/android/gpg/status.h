/**
 * @file gpg/status.h
 * @copyright Copyright 2014 Google Inc. All Rights Reserved.
 * @brief Enumerations used to represent the status of a request.
 */

#ifndef GPG_STATUS_H_
#define GPG_STATUS_H_

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

#include "gpg/common.h"

namespace gpg {

/// The set of possible values representing the result of an attempted
/// operation.
enum class GPG_EXPORT ResponseStatus {
  VALID,
  VALID_BUT_STALE,
  ERROR_LICENSE_CHECK_FAILED,
  ERROR_INTERNAL,
  ERROR_NOT_AUTHORIZED,
  ERROR_VERSION_UPDATE_REQUIRED,
  ERROR_TIMEOUT,
};

/// The set of possible values representing the result of a flush
/// attempt.
enum class GPG_EXPORT FlushStatus {
  FLUSHED,
  ERROR_INTERNAL = static_cast<int>(ResponseStatus::ERROR_INTERNAL),
  ERROR_NOT_AUTHORIZED = static_cast<int>(ResponseStatus::ERROR_NOT_AUTHORIZED),
  ERROR_VERSION_UPDATE_REQUIRED =
      static_cast<int>(ResponseStatus::ERROR_VERSION_UPDATE_REQUIRED),
  ERROR_TIMEOUT = static_cast<int>(ResponseStatus::ERROR_TIMEOUT),
};

/// The set of possible values representing the result of an authorization
/// attempt.
enum class GPG_EXPORT AuthStatus {
  VALID = static_cast<int>(ResponseStatus::VALID),
  ERROR_INTERNAL = static_cast<int>(ResponseStatus::ERROR_INTERNAL),
  ERROR_NOT_AUTHORIZED = static_cast<int>(ResponseStatus::ERROR_NOT_AUTHORIZED),
  ERROR_VERSION_UPDATE_REQUIRED =
      static_cast<int>(ResponseStatus::ERROR_VERSION_UPDATE_REQUIRED),
  ERROR_TIMEOUT = static_cast<int>(ResponseStatus::ERROR_TIMEOUT),
};

/// Returns true if authorization completed succesfully.
bool GPG_EXPORT IsSuccess(AuthStatus);

/// Returns true if an attempted operation resulted in a successful response.
bool GPG_EXPORT IsSuccess(ResponseStatus);

/// Returns true if a flush completed successfully.
bool GPG_EXPORT IsSuccess(FlushStatus);

/// Returns true if auhorization did not complete successfully.
bool GPG_EXPORT IsError(AuthStatus);

/// Returns true if an attempted operation does not result in a successful
/// response.
bool GPG_EXPORT IsError(ResponseStatus);

/// Returns true if a flush did not complete successfully.
bool GPG_EXPORT IsError(FlushStatus);

/** Returns a human-readable string showing the result of an
 * authorization attempt.
 * The result returned is one of the enums shown above for AuthStatus.
 */
char GPG_EXPORT const *StatusString(AuthStatus);

/** Returns a human-readable string showing the result of an
 * authorization attempt.
 * The result returned is one of the enums shown above for ResponseStatus.
 */
char GPG_EXPORT const *StatusString(ResponseStatus);

/** Returns a human-readable string showing the result of an
 * authorization attempt.
 * The result returned is one of the enums shown above for FlushStatus.
 */
char GPG_EXPORT const *StatusString(FlushStatus);

}  // namespace gpg

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop  // GCC diagnostic ignored "-Wattributes"
#endif

#endif  // GPG_STATUS_H_
