/**
 * @file gpg/builder.h
 * @copyright Copyright 2014 Google Inc. All Rights Reserved.
 * @brief Used to construct a GameServices object.
 */

#ifndef GPG_BUILDER_H_
#define GPG_BUILDER_H_

#ifndef __cplusplus
#error Header file supports C++ only
#endif  // __cplusplus

#include <functional>
#include <memory>
#include <string>
#include "gpg/common.h"
#include "gpg/game_services.h"
#include "gpg/platform_configuration.h"

namespace gpg {

class BuilderImpl;
enum class LogLevel;  // Defined in gpg/types.h

/**
 * Used for creating and configuring an instance of the GameServices class.
 */
class GPG_EXPORT GameServices::Builder {
 public:
  Builder();
  ~Builder();

  /**
   * Specifies the platform for which Builder will create the GameServices
   * object. For more information, see the documentation on platform configuration.
   */
  std::unique_ptr<GameServices> Create(PlatformConfiguration const &platform);

  /**
   * The type of logging callback that can be provided to the SDK.
   * @ingroup Callbacks
   */
  typedef std::function<void(LogLevel level, std::string const &message)>
      OnLogCallback;

  /**
   * Registers a callback which will be used to perform logging.
   * min_level specifies the minimum log level at which the callback is invoked.
   * Possible levels are: VERBOSE, INFO, WARNING, and ERROR.
   */
  Builder &SetLogging(OnLogCallback callback, LogLevel min_level);

  /**
   * Registers a callback which will be used to perform logging.
   * the same as calling SetLogging(OnLogCallback, LogLevel) with a LogLevel of
   * INFO.
   */
  Builder &SetLogging(OnLogCallback callback);

  /**
   * The type of the logging callback that can be provided to the SDK.
   * @ingroup Callbacks
   */
  typedef std::function<void(AuthOperation op)> OnAuthActionStartedCallback;

  /**
   * Registers a callback called when authorization has begun.
   */
  Builder &SetOnAuthActionStarted(OnAuthActionStartedCallback callback);

  /**
   * The type of the logging callback that can be provided to the SDK.
   * @ingroup Callbacks
   */
  typedef std::function <void(AuthOperation op, AuthStatus status)>
      OnAuthActionFinishedCallback;

  /**
   * Registers a callback called when authorization has finished.
   */
  Builder &SetOnAuthActionFinished(OnAuthActionFinishedCallback callback);

  /**
   * Scopes beyond the required Play Games scope to request.
   * Details on authorization scopes at https://developers.google.com/+/api/oauth#scopes.
   */
  Builder &AddOauthScope(std::string const &scope);

 private:
  Builder(Builder const &) = delete;
  Builder &operator=(Builder const &) = delete;

  std::unique_ptr<BuilderImpl> impl_;
};

}  // namespace gpg

#endif  // GPG_BUILDER_H_
