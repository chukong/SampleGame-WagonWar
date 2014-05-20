/**
 * @file gpg/default_callbacks.h
 * @copyright Copyright 2014 Google Inc. All Rights Reserved.
 * @brief Default implementation of callbacks used by GameServices.
 */

#ifndef GPG_DEFAULT_CALLBACKS_H_
#define GPG_DEFAULT_CALLBACKS_H_

#ifndef __cplusplus
#error Header file supports C++ only
#endif  // __cplusplus

#include <string>
#include "gpg/common.h"
#include "gpg/status.h"
#include "gpg/types.h"

namespace gpg {

enum class LogLevel;  // Defined in gpg/types.h

/** This is the default value for Builder::SetLogging. By default, logs are
 * written in a platform-specific manner (i.e., tot he Android log or NSLog).
 */
void GPG_EXPORT DEFAULT_ON_LOG(LogLevel level, std::string const &message);

/**
 * This is the default value for Builder:SetOnAuthActionStarted.
 * By default, this function logs the name of the auth operation that has
 * started.
 */
void GPG_EXPORT DEFAULT_ON_AUTH_ACTION_STARTED(AuthOperation op);

/**
 * This is the default value for Builder::SetOnAuthActionFinished.
 * By default, this function logs the name of the auth operation that has
 * finished, as well as the completion status of the operation.
 */
void GPG_EXPORT DEFAULT_ON_AUTH_ACTION_FINISHED(AuthOperation op,
                                                AuthStatus status);

}  // namespace gpg

#endif  // GPG_DEFAULT_CALLBACKS_H_
