/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_Sandbox_h
#define mozilla_Sandbox_h

#include <string>

enum MacSandboxType {
  MacSandboxType_Default = 0,
  MacSandboxType_Content,
  MacSandboxType_Flash,
  MacSandboxType_GMP,
  MacSandboxType_Utility,
  MacSandboxType_Invalid
};

<<<<<<< HEAD
enum MacSandboxPluginType {
  MacSandboxPluginType_Default = 0,
  MacSandboxPluginType_GMPlugin_Default,       // Any Gecko Media Plugin
  MacSandboxPluginType_GMPlugin_OpenH264,      // Gecko Media Plugin, OpenH264
  MacSandboxPluginType_GMPlugin_EME,           // Gecko Media Plugin, EME
  MacSandboxPluginType_GMPlugin_EME_Widevine,  // Gecko Media Plugin, Widevine
  MacSandboxPluginType_Flash,                  // Flash
  MacSandboxPluginType_Invalid
};

typedef struct _MacSandboxPluginInfo {
  _MacSandboxPluginInfo() : type(MacSandboxPluginType_Default) {}
  _MacSandboxPluginInfo(const struct _MacSandboxPluginInfo& other)
      : type(other.type),
        pluginPath(other.pluginPath),
        pluginBinaryPath(other.pluginBinaryPath) {}
  MacSandboxPluginType type;
  std::string pluginPath;
  std::string pluginBinaryPath;
} MacSandboxPluginInfo;

||||||| merged common ancestors
enum MacSandboxPluginType {
  MacSandboxPluginType_Default = 0,
  MacSandboxPluginType_GMPlugin_Default,      // Any Gecko Media Plugin
  MacSandboxPluginType_GMPlugin_OpenH264,     // Gecko Media Plugin, OpenH264
  MacSandboxPluginType_GMPlugin_EME,          // Gecko Media Plugin, EME
  MacSandboxPluginType_GMPlugin_EME_Widevine, // Gecko Media Plugin, Widevine
  MacSandboxPluginType_Flash,                 // Flash
  MacSandboxPluginType_Invalid
};

typedef struct _MacSandboxPluginInfo {
  _MacSandboxPluginInfo()
    : type(MacSandboxPluginType_Default) {}
  _MacSandboxPluginInfo(const struct _MacSandboxPluginInfo& other)
    : type(other.type), pluginPath(other.pluginPath),
      pluginBinaryPath(other.pluginBinaryPath) {}
  MacSandboxPluginType type;
  std::string pluginPath;
  std::string pluginBinaryPath;
} MacSandboxPluginInfo;

=======
>>>>>>> upstream-releases
typedef struct _MacSandboxInfo {
  _MacSandboxInfo()
      : type(MacSandboxType_Default),
        level(0),
        hasFilePrivileges(false),
        hasSandboxedProfile(false),
        hasAudio(false),
        hasWindowServer(false),
        shouldLog(false) {}
  _MacSandboxInfo(const struct _MacSandboxInfo& other) = default;

  void AppendAsParams(std::vector<std::string>& aParams) const;
  static void AppendFileAccessParam(std::vector<std::string>& aParams,
                                    bool aHasFilePrivileges);

 private:
  void AppendStartupParam(std::vector<std::string>& aParams) const;
  void AppendLoggingParam(std::vector<std::string>& aParams) const;
  void AppendAppPathParam(std::vector<std::string>& aParams) const;
  void AppendPluginPathParam(std::vector<std::string>& aParams) const;
  void AppendLevelParam(std::vector<std::string>& aParams) const;
  void AppendAudioParam(std::vector<std::string>& aParams) const;
  void AppendWindowServerParam(std::vector<std::string>& aParams) const;
  void AppendReadPathParams(std::vector<std::string>& aParams) const;
#ifdef DEBUG
  void AppendDebugWriteDirParam(std::vector<std::string>& aParams) const;
#endif

 public:
  MacSandboxType type;
  int32_t level;
  bool hasFilePrivileges;
  bool hasSandboxedProfile;
  bool hasAudio;
<<<<<<< HEAD
  bool hasWindowServer;
  MacSandboxPluginInfo pluginInfo;
||||||| merged common ancestors
  MacSandboxPluginInfo pluginInfo;
=======
  bool hasWindowServer;

>>>>>>> upstream-releases
  std::string appPath;
  std::string appBinaryPath;
  std::string appDir;
  std::string profileDir;
  std::string debugWriteDir;

  std::string pluginPath;
  std::string pluginBinaryPath;

  std::string testingReadPath1;
  std::string testingReadPath2;
  std::string testingReadPath3;
  std::string testingReadPath4;

  std::string crashServerPort;

  bool shouldLog;
} MacSandboxInfo;

namespace mozilla {

<<<<<<< HEAD
bool StartMacSandbox(MacSandboxInfo const& aInfo, std::string& aErrorMessage);
bool EarlyStartMacSandboxIfEnabled(int aArgc, char** aArgv,
                                   std::string& aErrorMessage);
#ifdef DEBUG
void AssertMacSandboxEnabled();
#endif /* DEBUG */
||||||| merged common ancestors
bool StartMacSandbox(MacSandboxInfo const &aInfo, std::string &aErrorMessage);
=======
bool StartMacSandbox(MacSandboxInfo const& aInfo, std::string& aErrorMessage);
bool StartMacSandboxIfEnabled(MacSandboxType aSandboxType, int aArgc,
                              char** aArgv, std::string& aErrorMessage);
bool IsMacSandboxStarted();
#ifdef DEBUG
void AssertMacSandboxEnabled();
#endif /* DEBUG */
>>>>>>> upstream-releases

}  // namespace mozilla

#endif  // mozilla_Sandbox_h
