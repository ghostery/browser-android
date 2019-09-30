/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: sw=2 ts=8 et :
 */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/utsname.h>
#include "nsCRTGlue.h"
#include "nsExceptionHandler.h"
#include "nsICrashReporter.h"
#include "prenv.h"
#include "nsPrintfCString.h"
#include "nsWhitespaceTokenizer.h"

#include "GfxInfoX11.h"

<<<<<<< HEAD
||||||| merged common ancestors

=======
#include <gdk/gdkx.h>
#ifdef MOZ_WAYLAND
#  include "mozilla/widget/nsWaylandDisplay.h"
#endif

#ifdef DEBUG
bool fire_glxtest_process();
#endif

>>>>>>> upstream-releases
namespace mozilla {
namespace widget {

#ifdef DEBUG
NS_IMPL_ISUPPORTS_INHERITED(GfxInfo, GfxInfoBase, nsIGfxInfoDebug)
#endif

// these global variables will be set when firing the glxtest process
int glxtest_pipe = -1;
pid_t glxtest_pid = 0;

<<<<<<< HEAD
nsresult GfxInfo::Init() {
  mGLMajorVersion = 0;
  mMajorVersion = 0;
  mMinorVersion = 0;
  mRevisionVersion = 0;
  mIsMesa = false;
  mIsNVIDIA = false;
  mIsFGLRX = false;
  mIsNouveau = false;
  mIsIntel = false;
  mIsOldSwrast = false;
  mIsLlvmpipe = false;
  mHasTextureFromPixmap = false;
  return GfxInfoBase::Init();
||||||| merged common ancestors
nsresult
GfxInfo::Init()
{
    mGLMajorVersion = 0;
    mMajorVersion = 0;
    mMinorVersion = 0;
    mRevisionVersion = 0;
    mIsMesa = false;
    mIsNVIDIA = false;
    mIsFGLRX = false;
    mIsNouveau = false;
    mIsIntel = false;
    mIsOldSwrast = false;
    mIsLlvmpipe = false;
    mHasTextureFromPixmap = false;
    return GfxInfoBase::Init();
=======
nsresult GfxInfo::Init() {
  mGLMajorVersion = 0;
  mGLMinorVersion = 0;
  mHasTextureFromPixmap = false;
  mIsMesa = false;
  mIsAccelerated = true;
  mIsWayland = false;
  mIsWaylandDRM = false;
  return GfxInfoBase::Init();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void GfxInfo::GetData() {
  // to understand this function, see bug 639842. We retrieve the OpenGL driver
  // information in a separate process to protect against bad drivers.

  // if glxtest_pipe == -1, that means that we already read the information
  if (glxtest_pipe == -1) return;

  enum { buf_size = 1024 };
  char buf[buf_size];
  ssize_t bytesread = read(glxtest_pipe, &buf,
                           buf_size - 1);  // -1 because we'll append a zero
  close(glxtest_pipe);
  glxtest_pipe = -1;

  // bytesread < 0 would mean that the above read() call failed.
  // This should never happen. If it did, the outcome would be to blacklist
  // anyway.
  if (bytesread < 0) bytesread = 0;

  // let buf be a zero-terminated string
  buf[bytesread] = 0;

  // Wait for the glxtest process to finish. This serves 2 purposes:
  // * avoid having a zombie glxtest process laying around
  // * get the glxtest process status info.
  int glxtest_status = 0;
  bool wait_for_glxtest_process = true;
  bool waiting_for_glxtest_process_failed = false;
  int waitpid_errno = 0;
  while (wait_for_glxtest_process) {
    wait_for_glxtest_process = false;
    if (waitpid(glxtest_pid, &glxtest_status, 0) == -1) {
      waitpid_errno = errno;
      if (waitpid_errno == EINTR) {
        wait_for_glxtest_process = true;
      } else {
        // Bug 718629
        // ECHILD happens when the glxtest process got reaped got reaped after a
        // PR_CreateProcess as per bug 227246. This shouldn't matter, as we
        // still seem to get the data from the pipe, and if we didn't, the
        // outcome would be to blacklist anyway.
        waiting_for_glxtest_process_failed = (waitpid_errno != ECHILD);
      }
||||||| merged common ancestors
void
GfxInfo::GetData()
{
    // to understand this function, see bug 639842. We retrieve the OpenGL driver information in a
    // separate process to protect against bad drivers.

    // if glxtest_pipe == -1, that means that we already read the information
    if (glxtest_pipe == -1)
        return;

    enum { buf_size = 1024 };
    char buf[buf_size];
    ssize_t bytesread = read(glxtest_pipe,
                             &buf,
                             buf_size-1); // -1 because we'll append a zero
    close(glxtest_pipe);
    glxtest_pipe = -1;

    // bytesread < 0 would mean that the above read() call failed.
    // This should never happen. If it did, the outcome would be to blacklist anyway.
    if (bytesread < 0)
        bytesread = 0;

    // let buf be a zero-terminated string
    buf[bytesread] = 0;

    // Wait for the glxtest process to finish. This serves 2 purposes:
    // * avoid having a zombie glxtest process laying around
    // * get the glxtest process status info.
    int glxtest_status = 0;
    bool wait_for_glxtest_process = true;
    bool waiting_for_glxtest_process_failed = false;
    int waitpid_errno = 0;
    while(wait_for_glxtest_process) {
        wait_for_glxtest_process = false;
        if (waitpid(glxtest_pid, &glxtest_status, 0) == -1) {
            waitpid_errno = errno;
            if (waitpid_errno == EINTR) {
                wait_for_glxtest_process = true;
            } else {
                // Bug 718629
                // ECHILD happens when the glxtest process got reaped got reaped after a PR_CreateProcess
                // as per bug 227246. This shouldn't matter, as we still seem to get the data
                // from the pipe, and if we didn't, the outcome would be to blacklist anyway.
                waiting_for_glxtest_process_failed = (waitpid_errno != ECHILD);
            }
        }
=======
void GfxInfo::AddCrashReportAnnotations() {
  CrashReporter::AnnotateCrashReport(CrashReporter::Annotation::AdapterVendorID,
                                     mVendorId);
  CrashReporter::AnnotateCrashReport(CrashReporter::Annotation::AdapterDeviceID,
                                     mDeviceId);
  CrashReporter::AnnotateCrashReport(
      CrashReporter::Annotation::AdapterDriverVendor, mDriverVendor);
  CrashReporter::AnnotateCrashReport(
      CrashReporter::Annotation::AdapterDriverVersion, mDriverVersion);
  CrashReporter::AnnotateCrashReport(CrashReporter::Annotation::IsWayland,
                                     mIsWayland);
  CrashReporter::AnnotateCrashReport(CrashReporter::Annotation::IsWaylandDRM,
                                     mIsWaylandDRM);
}

void GfxInfo::GetData() {
  // to understand this function, see bug 639842. We retrieve the OpenGL driver
  // information in a separate process to protect against bad drivers.

  // if glxtest_pipe == -1, that means that we already read the information
  if (glxtest_pipe == -1) return;

  enum { buf_size = 1024 };
  char buf[buf_size];
  ssize_t bytesread = read(glxtest_pipe, &buf,
                           buf_size - 1);  // -1 because we'll append a zero
  close(glxtest_pipe);
  glxtest_pipe = -1;

  // bytesread < 0 would mean that the above read() call failed.
  // This should never happen. If it did, the outcome would be to blacklist
  // anyway.
  if (bytesread < 0) bytesread = 0;

  // let buf be a zero-terminated string
  buf[bytesread] = 0;

  // Wait for the glxtest process to finish. This serves 2 purposes:
  // * avoid having a zombie glxtest process laying around
  // * get the glxtest process status info.
  int glxtest_status = 0;
  bool wait_for_glxtest_process = true;
  bool waiting_for_glxtest_process_failed = false;
  int waitpid_errno = 0;
  while (wait_for_glxtest_process) {
    wait_for_glxtest_process = false;
    if (waitpid(glxtest_pid, &glxtest_status, 0) == -1) {
      waitpid_errno = errno;
      if (waitpid_errno == EINTR) {
        wait_for_glxtest_process = true;
      } else {
        // Bug 718629
        // ECHILD happens when the glxtest process got reaped got reaped after a
        // PR_CreateProcess as per bug 227246. This shouldn't matter, as we
        // still seem to get the data from the pipe, and if we didn't, the
        // outcome would be to blacklist anyway.
        waiting_for_glxtest_process_failed = (waitpid_errno != ECHILD);
      }
    }
  }

  bool exited_with_error_code = !waiting_for_glxtest_process_failed &&
                                WIFEXITED(glxtest_status) &&
                                WEXITSTATUS(glxtest_status) != EXIT_SUCCESS;
  bool received_signal =
      !waiting_for_glxtest_process_failed && WIFSIGNALED(glxtest_status);

  bool error = waiting_for_glxtest_process_failed || exited_with_error_code ||
               received_signal;

  nsCString glVendor;
  nsCString glRenderer;
  nsCString glVersion;
  nsCString textureFromPixmap;

  // Available if GLX_MESA_query_renderer is supported.
  nsCString mesaVendor;
  nsCString mesaDevice;
  nsCString mesaAccelerated;
  // Available if using a DRI-based libGL stack.
  nsCString driDriver;

  nsCString* stringToFill = nullptr;
  char* bufptr = buf;
  if (!error) {
    while (true) {
      char* line = NS_strtok("\n", &bufptr);
      if (!line) break;
      if (stringToFill) {
        stringToFill->Assign(line);
        stringToFill = nullptr;
      } else if (!strcmp(line, "VENDOR"))
        stringToFill = &glVendor;
      else if (!strcmp(line, "RENDERER"))
        stringToFill = &glRenderer;
      else if (!strcmp(line, "VERSION"))
        stringToFill = &glVersion;
      else if (!strcmp(line, "TFP"))
        stringToFill = &textureFromPixmap;
      else if (!strcmp(line, "MESA_VENDOR_ID"))
        stringToFill = &mesaVendor;
      else if (!strcmp(line, "MESA_DEVICE_ID"))
        stringToFill = &mesaDevice;
      else if (!strcmp(line, "MESA_ACCELERATED"))
        stringToFill = &mesaAccelerated;
      else if (!strcmp(line, "MESA_VRAM"))
        stringToFill = &mAdapterRAM;
      else if (!strcmp(line, "DRI_DRIVER"))
        stringToFill = &driDriver;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
  }

  if (!strcmp(textureFromPixmap.get(), "TRUE")) mHasTextureFromPixmap = true;

  // only useful for Linux kernel version check for FGLRX driver.
  // assumes X client == X server, which is sad.
  struct utsname unameobj;
  if (uname(&unameobj) >= 0) {
    mOS.Assign(unameobj.sysname);
    mOSRelease.Assign(unameobj.release);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool exited_with_error_code = !waiting_for_glxtest_process_failed &&
                                WIFEXITED(glxtest_status) &&
                                WEXITSTATUS(glxtest_status) != EXIT_SUCCESS;
  bool received_signal =
      !waiting_for_glxtest_process_failed && WIFSIGNALED(glxtest_status);

  bool error = waiting_for_glxtest_process_failed || exited_with_error_code ||
               received_signal;

  nsCString textureFromPixmap;
  nsCString *stringToFill = nullptr;
  char *bufptr = buf;
  if (!error) {
    while (true) {
      char *line = NS_strtok("\n", &bufptr);
      if (!line) break;
      if (stringToFill) {
        stringToFill->Assign(line);
        stringToFill = nullptr;
      } else if (!strcmp(line, "VENDOR"))
        stringToFill = &mVendor;
      else if (!strcmp(line, "RENDERER"))
        stringToFill = &mRenderer;
      else if (!strcmp(line, "VERSION"))
        stringToFill = &mVersion;
      else if (!strcmp(line, "TFP"))
        stringToFill = &textureFromPixmap;
||||||| merged common ancestors
    bool exited_with_error_code = !waiting_for_glxtest_process_failed &&
                                  WIFEXITED(glxtest_status) && 
                                  WEXITSTATUS(glxtest_status) != EXIT_SUCCESS;
    bool received_signal = !waiting_for_glxtest_process_failed &&
                           WIFSIGNALED(glxtest_status);

    bool error = waiting_for_glxtest_process_failed || exited_with_error_code || received_signal;

    nsCString textureFromPixmap; 
    nsCString *stringToFill = nullptr;
    char *bufptr = buf;
    if (!error) {
        while(true) {
            char *line = NS_strtok("\n", &bufptr);
            if (!line)
                break;
            if (stringToFill) {
                stringToFill->Assign(line);
                stringToFill = nullptr;
            }
            else if(!strcmp(line, "VENDOR"))
                stringToFill = &mVendor;
            else if(!strcmp(line, "RENDERER"))
                stringToFill = &mRenderer;
            else if(!strcmp(line, "VERSION"))
                stringToFill = &mVersion;
            else if(!strcmp(line, "TFP"))
                stringToFill = &textureFromPixmap;
        }
=======
  const char* spoofedVendor = PR_GetEnv("MOZ_GFX_SPOOF_GL_VENDOR");
  if (spoofedVendor) glVendor.Assign(spoofedVendor);
  const char* spoofedRenderer = PR_GetEnv("MOZ_GFX_SPOOF_GL_RENDERER");
  if (spoofedRenderer) glRenderer.Assign(spoofedRenderer);
  const char* spoofedVersion = PR_GetEnv("MOZ_GFX_SPOOF_GL_VERSION");
  if (spoofedVersion) glVersion.Assign(spoofedVersion);
  const char* spoofedOS = PR_GetEnv("MOZ_GFX_SPOOF_OS");
  if (spoofedOS) mOS.Assign(spoofedOS);
  const char* spoofedOSRelease = PR_GetEnv("MOZ_GFX_SPOOF_OS_RELEASE");
  if (spoofedOSRelease) mOSRelease.Assign(spoofedOSRelease);

  if (error || glVendor.IsEmpty() || glRenderer.IsEmpty() ||
      glVersion.IsEmpty() || mOS.IsEmpty() || mOSRelease.IsEmpty()) {
    mAdapterDescription.AppendLiteral("GLXtest process failed");
    if (waiting_for_glxtest_process_failed)
      mAdapterDescription.AppendPrintf(
          " (waitpid failed with errno=%d for pid %d)", waitpid_errno,
          glxtest_pid);
    if (exited_with_error_code)
      mAdapterDescription.AppendPrintf(" (exited with status %d)",
                                       WEXITSTATUS(glxtest_status));
    if (received_signal)
      mAdapterDescription.AppendPrintf(" (received signal %d)",
                                       WTERMSIG(glxtest_status));
    if (bytesread) {
      mAdapterDescription.AppendLiteral(": ");
      mAdapterDescription.Append(nsDependentCString(buf));
      mAdapterDescription.Append('\n');
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  if (!strcmp(textureFromPixmap.get(), "TRUE")) mHasTextureFromPixmap = true;
||||||| merged common ancestors
    if (!strcmp(textureFromPixmap.get(), "TRUE"))
        mHasTextureFromPixmap = true;

    // only useful for Linux kernel version check for FGLRX driver.
    // assumes X client == X server, which is sad.
    struct utsname unameobj;
    if (uname(&unameobj) >= 0)
    {
      mOS.Assign(unameobj.sysname);
      mOSRelease.Assign(unameobj.release);
    }
=======
    CrashReporter::AppendAppNotesToCrashReport(mAdapterDescription);
    return;
  }

  // Scan the GL_VERSION string for the GL and driver versions.
  nsCWhitespaceTokenizer tokenizer(glVersion);
  while (tokenizer.hasMoreTokens()) {
    nsCString token(tokenizer.nextToken());
    unsigned int major = 0, minor = 0, revision = 0, patch = 0;
    if (sscanf(token.get(), "%u.%u.%u.%u", &major, &minor, &revision, &patch) >=
        2) {
      // A survey of GL_VENDOR strings indicates that the first version is
      // always the GL version, the second is usually the driver version.
      if (mGLMajorVersion == 0) {
        mGLMajorVersion = major;
        mGLMinorVersion = minor;
      } else if (mDriverVersion.IsEmpty()) {  // Not already spoofed.
        mDriverVersion =
            nsPrintfCString("%u.%u.%u.%u", major, minor, revision, patch);
      }
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // only useful for Linux kernel version check for FGLRX driver.
  // assumes X client == X server, which is sad.
  struct utsname unameobj;
  if (uname(&unameobj) >= 0) {
    mOS.Assign(unameobj.sysname);
    mOSRelease.Assign(unameobj.release);
  }

  const char *spoofedVendor = PR_GetEnv("MOZ_GFX_SPOOF_GL_VENDOR");
  if (spoofedVendor) mVendor.Assign(spoofedVendor);
  const char *spoofedRenderer = PR_GetEnv("MOZ_GFX_SPOOF_GL_RENDERER");
  if (spoofedRenderer) mRenderer.Assign(spoofedRenderer);
  const char *spoofedVersion = PR_GetEnv("MOZ_GFX_SPOOF_GL_VERSION");
  if (spoofedVersion) mVersion.Assign(spoofedVersion);
  const char *spoofedOS = PR_GetEnv("MOZ_GFX_SPOOF_OS");
  if (spoofedOS) mOS.Assign(spoofedOS);
  const char *spoofedOSRelease = PR_GetEnv("MOZ_GFX_SPOOF_OS_RELEASE");
  if (spoofedOSRelease) mOSRelease.Assign(spoofedOSRelease);

  if (error || mVendor.IsEmpty() || mRenderer.IsEmpty() || mVersion.IsEmpty() ||
      mOS.IsEmpty() || mOSRelease.IsEmpty()) {
    mAdapterDescription.AppendLiteral("GLXtest process failed");
    if (waiting_for_glxtest_process_failed)
      mAdapterDescription.AppendPrintf(
          " (waitpid failed with errno=%d for pid %d)", waitpid_errno,
          glxtest_pid);
    if (exited_with_error_code)
      mAdapterDescription.AppendPrintf(" (exited with status %d)",
                                       WEXITSTATUS(glxtest_status));
    if (received_signal)
      mAdapterDescription.AppendPrintf(" (received signal %d)",
                                       WTERMSIG(glxtest_status));
    if (bytesread) {
      mAdapterDescription.AppendLiteral(": ");
      mAdapterDescription.Append(nsDependentCString(buf));
      mAdapterDescription.Append('\n');
||||||| merged common ancestors
    const char *spoofedVendor = PR_GetEnv("MOZ_GFX_SPOOF_GL_VENDOR");
    if (spoofedVendor)
        mVendor.Assign(spoofedVendor);
    const char *spoofedRenderer = PR_GetEnv("MOZ_GFX_SPOOF_GL_RENDERER");
    if (spoofedRenderer)
        mRenderer.Assign(spoofedRenderer);
    const char *spoofedVersion = PR_GetEnv("MOZ_GFX_SPOOF_GL_VERSION");
    if (spoofedVersion)
        mVersion.Assign(spoofedVersion);
    const char *spoofedOS = PR_GetEnv("MOZ_GFX_SPOOF_OS");
    if (spoofedOS)
        mOS.Assign(spoofedOS);
    const char *spoofedOSRelease = PR_GetEnv("MOZ_GFX_SPOOF_OS_RELEASE");
    if (spoofedOSRelease)
        mOSRelease.Assign(spoofedOSRelease);

    if (error ||
        mVendor.IsEmpty() ||
        mRenderer.IsEmpty() ||
        mVersion.IsEmpty() ||
        mOS.IsEmpty() ||
        mOSRelease.IsEmpty())
    {
        mAdapterDescription.AppendLiteral("GLXtest process failed");
        if (waiting_for_glxtest_process_failed)
            mAdapterDescription.AppendPrintf(" (waitpid failed with errno=%d for pid %d)", waitpid_errno, glxtest_pid);
        if (exited_with_error_code)
            mAdapterDescription.AppendPrintf(" (exited with status %d)", WEXITSTATUS(glxtest_status));
        if (received_signal)
            mAdapterDescription.AppendPrintf(" (received signal %d)", WTERMSIG(glxtest_status));
        if (bytesread) {
            mAdapterDescription.AppendLiteral(": ");
            mAdapterDescription.Append(nsDependentCString(buf));
            mAdapterDescription.Append('\n');
        }

        CrashReporter::AppendAppNotesToCrashReport(mAdapterDescription);
        return;
=======
  if (mGLMajorVersion == 0) {
    NS_WARNING("Failed to parse GL version!");
    return;
  }

  // Mesa always exposes itself in the GL_VERSION string, but not always the
  // GL_VENDOR string.
  mIsMesa = glVersion.Find("Mesa") != -1;

  // We need to use custom driver vendor IDs for mesa so we can treat them
  // differently than the proprietary drivers.
  if (mIsMesa) {
    mIsAccelerated = !mesaAccelerated.Equals("FALSE");
    // Process software rasterizers before the DRI driver string; we may be
    // forcing software rasterization on a DRI-accelerated X server by using
    // LIBGL_ALWAYS_SOFTWARE or a similar restriction.
    if (strcasestr(glRenderer.get(), "llvmpipe")) {
      CopyUTF16toUTF8(GfxDriverInfo::GetDriverVendor(DriverMesaLLVMPipe),
                      mDriverVendor);
      mIsAccelerated = false;
    } else if (strcasestr(glRenderer.get(), "softpipe")) {
      CopyUTF16toUTF8(GfxDriverInfo::GetDriverVendor(DriverMesaSoftPipe),
                      mDriverVendor);
      mIsAccelerated = false;
    } else if (strcasestr(glRenderer.get(), "software rasterizer") ||
               !mIsAccelerated) {
      // Fallback to reporting swrast if GLX_MESA_query_renderer tells us
      // we're using an unaccelerated context.
      CopyUTF16toUTF8(GfxDriverInfo::GetDriverVendor(DriverMesaSWRast),
                      mDriverVendor);
      mIsAccelerated = false;
    } else if (!driDriver.IsEmpty()) {
      mDriverVendor = nsPrintfCString("mesa/%s", driDriver.get());
    } else {
      // Some other mesa configuration where we couldn't get enough info.
      NS_WARNING("Failed to detect Mesa driver being used!");
      CopyUTF16toUTF8(GfxDriverInfo::GetDriverVendor(DriverMesaUnknown),
                      mDriverVendor);
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    CrashReporter::AppendAppNotesToCrashReport(mAdapterDescription);
    return;
  }
||||||| merged common ancestors
    mAdapterDescription.Append(mVendor);
    mAdapterDescription.AppendLiteral(" -- ");
    mAdapterDescription.Append(mRenderer);

    nsAutoCString note;
    note.AppendLiteral("OpenGL: ");
    note.Append(mAdapterDescription);
    note.AppendLiteral(" -- ");
    note.Append(mVersion);
    if (mHasTextureFromPixmap)
        note.AppendLiteral(" -- texture_from_pixmap");
    note.Append('\n');

    CrashReporter::AppendAppNotesToCrashReport(note);

    // determine the major OpenGL version. That's the first integer in the version string.
    mGLMajorVersion = strtol(mVersion.get(), 0, 10);

    // determine driver type (vendor) and where in the version string
    // the actual driver version numbers should be expected to be found (whereToReadVersionNumbers)
    const char *whereToReadVersionNumbers = nullptr;
    const char *Mesa_in_version_string = strstr(mVersion.get(), "Mesa");
    if (Mesa_in_version_string) {
        mIsMesa = true;
        // with Mesa, the version string contains "Mesa major.minor" and that's all the version information we get:
        // there is no actual driver version info.
        whereToReadVersionNumbers = Mesa_in_version_string + strlen("Mesa");
        if (strcasestr(mVendor.get(), "nouveau"))
            mIsNouveau = true;
        if (strcasestr(mRenderer.get(), "intel")) // yes, intel is in the renderer string
            mIsIntel = true;
        if (strcasestr(mRenderer.get(), "llvmpipe"))
            mIsLlvmpipe = true;
        if (strcasestr(mRenderer.get(), "software rasterizer"))
            mIsOldSwrast = true;
    } else if (strstr(mVendor.get(), "NVIDIA Corporation")) {
        mIsNVIDIA = true;
        // with the NVIDIA driver, the version string contains "NVIDIA major.minor"
        // note that here the vendor and version strings behave differently, that's why we don't put this above
        // alongside Mesa_in_version_string.
        const char *NVIDIA_in_version_string = strstr(mVersion.get(), "NVIDIA");
        if (NVIDIA_in_version_string)
            whereToReadVersionNumbers = NVIDIA_in_version_string + strlen("NVIDIA");
    } else if (strstr(mVendor.get(), "ATI Technologies Inc")) {
        mIsFGLRX = true;
        // with the FGLRX driver, the version string only gives a OpenGL version :/ so let's return that.
        // that can at least give a rough idea of how old the driver is.
        whereToReadVersionNumbers = mVersion.get();
    }
=======
    if (!mesaVendor.IsEmpty()) {
      mVendorId = mesaVendor;
    } else {
      NS_WARNING(
          "Failed to get Mesa vendor ID! GLX_MESA_query_renderer unsupported?");
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  mAdapterDescription.Append(mVendor);
  mAdapterDescription.AppendLiteral(" -- ");
  mAdapterDescription.Append(mRenderer);

  nsAutoCString note;
  note.AppendLiteral("OpenGL: ");
  note.Append(mAdapterDescription);
  note.AppendLiteral(" -- ");
  note.Append(mVersion);
  if (mHasTextureFromPixmap) note.AppendLiteral(" -- texture_from_pixmap");
  note.Append('\n');

  CrashReporter::AppendAppNotesToCrashReport(note);

  // determine the major OpenGL version. That's the first integer in the version
  // string.
  mGLMajorVersion = strtol(mVersion.get(), 0, 10);

  // determine driver type (vendor) and where in the version string
  // the actual driver version numbers should be expected to be found
  // (whereToReadVersionNumbers)
  const char *whereToReadVersionNumbers = nullptr;
  const char *Mesa_in_version_string = strstr(mVersion.get(), "Mesa");
  if (Mesa_in_version_string) {
    mIsMesa = true;
    // with Mesa, the version string contains "Mesa major.minor" and that's all
    // the version information we get: there is no actual driver version info.
    whereToReadVersionNumbers = Mesa_in_version_string + strlen("Mesa");
    if (strcasestr(mVendor.get(), "nouveau")) mIsNouveau = true;
    if (strcasestr(mRenderer.get(),
                   "intel"))  // yes, intel is in the renderer string
      mIsIntel = true;
    if (strcasestr(mRenderer.get(), "llvmpipe")) mIsLlvmpipe = true;
    if (strcasestr(mRenderer.get(), "software rasterizer")) mIsOldSwrast = true;
  } else if (strstr(mVendor.get(), "NVIDIA Corporation")) {
    mIsNVIDIA = true;
    // with the NVIDIA driver, the version string contains "NVIDIA major.minor"
    // note that here the vendor and version strings behave differently, that's
    // why we don't put this above alongside Mesa_in_version_string.
    const char *NVIDIA_in_version_string = strstr(mVersion.get(), "NVIDIA");
    if (NVIDIA_in_version_string)
      whereToReadVersionNumbers = NVIDIA_in_version_string + strlen("NVIDIA");
  } else if (strstr(mVendor.get(), "ATI Technologies Inc")) {
    mIsFGLRX = true;
    // with the FGLRX driver, the version string only gives a OpenGL version :/
    // so let's return that. that can at least give a rough idea of how old the
    // driver is.
    whereToReadVersionNumbers = mVersion.get();
  }

  // read major.minor version numbers of the driver (not to be confused with the
  // OpenGL version)
  if (whereToReadVersionNumbers) {
    // copy into writable buffer, for tokenization
    strncpy(buf, whereToReadVersionNumbers, buf_size);
    bufptr = buf;

    // now try to read major.minor version numbers. In case of failure,
    // gracefully exit: these numbers have been initialized as 0 anyways
    char *token = NS_strtok(".", &bufptr);
    if (token) {
      mMajorVersion = strtol(token, 0, 10);
      token = NS_strtok(".", &bufptr);
      if (token) {
        mMinorVersion = strtol(token, 0, 10);
        token = NS_strtok(".", &bufptr);
        if (token) mRevisionVersion = strtol(token, 0, 10);
      }
||||||| merged common ancestors
    // read major.minor version numbers of the driver (not to be confused with the OpenGL version)
    if (whereToReadVersionNumbers) {
        // copy into writable buffer, for tokenization
        strncpy(buf, whereToReadVersionNumbers, buf_size);
        bufptr = buf;

        // now try to read major.minor version numbers. In case of failure, gracefully exit: these numbers have
        // been initialized as 0 anyways
        char *token = NS_strtok(".", &bufptr);
        if (token) {
            mMajorVersion = strtol(token, 0, 10);
            token = NS_strtok(".", &bufptr);
            if (token) {
                mMinorVersion = strtol(token, 0, 10);
                token = NS_strtok(".", &bufptr);
                if (token)
                    mRevisionVersion = strtol(token, 0, 10);
            }
        }
=======
    if (!mesaDevice.IsEmpty()) {
      mDeviceId = mesaDevice;
    } else {
      NS_WARNING(
          "Failed to get Mesa device ID! GLX_MESA_query_renderer unsupported?");
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
}
||||||| merged common ancestors
}
=======
  } else if (glVendor.EqualsLiteral("NVIDIA Corporation")) {
    CopyUTF16toUTF8(GfxDriverInfo::GetDeviceVendor(VendorNVIDIA), mVendorId);
    mDriverVendor.AssignLiteral("nvidia/unknown");
    // TODO: Use NV-CONTROL X11 extension to query Device ID and VRAM.
  } else if (glVendor.EqualsLiteral("ATI Technologies Inc.")) {
    CopyUTF16toUTF8(GfxDriverInfo::GetDeviceVendor(VendorATI), mVendorId);
    mDriverVendor.AssignLiteral("ati/unknown");
    // TODO: Look into ways to find the device ID on FGLRX.
  } else {
    NS_WARNING("Failed to detect GL vendor!");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static inline uint64_t version(uint32_t major, uint32_t minor,
                               uint32_t revision = 0) {
  return (uint64_t(major) << 32) + (uint64_t(minor) << 16) + uint64_t(revision);
||||||| merged common ancestors
static inline uint64_t version(uint32_t major, uint32_t minor, uint32_t revision = 0)
{
    return (uint64_t(major) << 32) + (uint64_t(minor) << 16) + uint64_t(revision);
=======
  // Fallback to GL_VENDOR and GL_RENDERER.
  if (mVendorId.IsEmpty()) {
    mVendorId.Assign(glVendor.get());
  }
  if (mDeviceId.IsEmpty()) {
    mDeviceId.Assign(glRenderer.get());
  }

  mAdapterDescription.Assign(glRenderer);
#ifdef MOZ_WAYLAND
  mIsWayland = !GDK_IS_X11_DISPLAY(gdk_display_get_default());
  if (mIsWayland) {
    mIsWaylandDRM = nsWaylandDisplay::IsDMABufEnabled();
  }
#endif
  AddCrashReportAnnotations();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
const nsTArray<GfxDriverInfo> &GfxInfo::GetGfxDriverInfo() {
  // Nothing here yet.
  // if (!sDriverInfo->Length()) {
  //
  //}
||||||| merged common ancestors
const nsTArray<GfxDriverInfo>&
GfxInfo::GetGfxDriverInfo()
{
  // Nothing here yet.
  //if (!sDriverInfo->Length()) {
  //
  //}
=======
const nsTArray<GfxDriverInfo>& GfxInfo::GetGfxDriverInfo() {
  if (!sDriverInfo->Length()) {
    // Mesa 10.0 provides the GLX_MESA_query_renderer extension, which allows us
    // to query device IDs backing a GL context for blacklisting.
    APPEND_TO_DRIVER_BLOCKLIST(
        OperatingSystem::Linux,
        (nsAString&)GfxDriverInfo::GetDeviceVendor(VendorAll),
        (nsAString&)GfxDriverInfo::GetDriverVendor(DriverMesaAll),
        GfxDriverInfo::allDevices, GfxDriverInfo::allFeatures,
        nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION, DRIVER_LESS_THAN,
        V(10, 0, 0, 0), "FEATURE_FAILURE_OLD_MESA", "Mesa 10.0");

    // NVIDIA baseline (ported from old blocklist)
    APPEND_TO_DRIVER_BLOCKLIST(
        OperatingSystem::Linux,
        (nsAString&)GfxDriverInfo::GetDeviceVendor(VendorNVIDIA),
        (nsAString&)GfxDriverInfo::GetDriverVendor(DriverNonMesaAll),
        GfxDriverInfo::allDevices, GfxDriverInfo::allFeatures,
        nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION, DRIVER_LESS_THAN,
        V(257, 21, 0, 0), "FEATURE_FAILURE_OLD_NVIDIA", "NVIDIA 257.21");

    // fglrx baseline (chosen arbitrarily as 2013-07-22 release).
    APPEND_TO_DRIVER_BLOCKLIST(
        OperatingSystem::Linux,
        (nsAString&)GfxDriverInfo::GetDeviceVendor(VendorATI),
        (nsAString&)GfxDriverInfo::GetDriverVendor(DriverVendorAll),
        GfxDriverInfo::allDevices, GfxDriverInfo::allFeatures,
        nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION, DRIVER_LESS_THAN,
        V(13, 15, 100, 1), "FEATURE_FAILURE_OLD_FGLRX", "fglrx 13.15.100.1");

    ////////////////////////////////////
    // FEATURE_WEBRENDER

    // Intel Mesa baseline, chosen arbitrarily.
    APPEND_TO_DRIVER_BLOCKLIST(
        OperatingSystem::Linux,
        (nsAString&)GfxDriverInfo::GetDeviceVendor(VendorIntel),
        (nsAString&)GfxDriverInfo::GetDriverVendor(DriverVendorAll),
        GfxDriverInfo::allDevices, nsIGfxInfo::FEATURE_WEBRENDER,
        nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION, DRIVER_LESS_THAN,
        V(18, 0, 0, 0), "FEATURE_FAILURE_WEBRENDER_OLD_MESA", "Mesa 18.0.0.0");

    // Disable on all NVIDIA devices for now.
    APPEND_TO_DRIVER_BLOCKLIST(
        OperatingSystem::Linux,
        (nsAString&)GfxDriverInfo::GetDeviceVendor(VendorNVIDIA),
        (nsAString&)GfxDriverInfo::GetDriverVendor(DriverVendorAll),
        GfxDriverInfo::allDevices, nsIGfxInfo::FEATURE_WEBRENDER,
        nsIGfxInfo::FEATURE_BLOCKED_DEVICE, DRIVER_COMPARISON_IGNORED,
        V(0, 0, 0, 0), "FEATURE_FAILURE_WEBRENDER_NO_LINUX_NVIDIA", "");

    // ATI Mesa baseline, chosen arbitrarily.
    APPEND_TO_DRIVER_BLOCKLIST(
        OperatingSystem::Linux,
        (nsAString&)GfxDriverInfo::GetDeviceVendor(VendorATI),
        (nsAString&)GfxDriverInfo::GetDriverVendor(DriverMesaAll),
        GfxDriverInfo::allDevices, nsIGfxInfo::FEATURE_WEBRENDER,
        nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION, DRIVER_LESS_THAN,
        V(18, 0, 0, 0), "FEATURE_FAILURE_WEBRENDER_OLD_MESA", "Mesa 18.0.0.0");

    // Disable on all ATI devices not using Mesa for now.
    APPEND_TO_DRIVER_BLOCKLIST(
        OperatingSystem::Linux,
        (nsAString&)GfxDriverInfo::GetDeviceVendor(VendorATI),
        (nsAString&)GfxDriverInfo::GetDriverVendor(DriverNonMesaAll),
        GfxDriverInfo::allDevices, nsIGfxInfo::FEATURE_WEBRENDER,
        nsIGfxInfo::FEATURE_BLOCKED_DEVICE, DRIVER_COMPARISON_IGNORED,
        V(0, 0, 0, 0), "FEATURE_FAILURE_WEBRENDER_NO_LINUX_ATI", "");
  }
>>>>>>> upstream-releases
  return *sDriverInfo;
}

<<<<<<< HEAD
nsresult GfxInfo::GetFeatureStatusImpl(
    int32_t aFeature, int32_t *aStatus, nsAString &aSuggestedDriverVersion,
    const nsTArray<GfxDriverInfo> &aDriverInfo, nsACString &aFailureId,
    OperatingSystem *aOS /* = nullptr */)
||||||| merged common ancestors
nsresult
GfxInfo::GetFeatureStatusImpl(int32_t aFeature,
                              int32_t *aStatus,
                              nsAString & aSuggestedDriverVersion,
                              const nsTArray<GfxDriverInfo>& aDriverInfo,
                              nsACString& aFailureId,
                              OperatingSystem* aOS /* = nullptr */)
=======
bool GfxInfo::DoesDriverVendorMatch(const nsAString& aBlocklistVendor,
                                    const nsAString& aDriverVendor) {
  if (mIsMesa &&
      aBlocklistVendor.Equals(GfxDriverInfo::GetDriverVendor(DriverMesaAll),
                              nsCaseInsensitiveStringComparator())) {
    return true;
  }
  if (!mIsMesa &&
      aBlocklistVendor.Equals(GfxDriverInfo::GetDriverVendor(DriverNonMesaAll),
                              nsCaseInsensitiveStringComparator())) {
    return true;
  }
  return GfxInfoBase::DoesDriverVendorMatch(aBlocklistVendor, aDriverVendor);
}

nsresult GfxInfo::GetFeatureStatusImpl(
    int32_t aFeature, int32_t* aStatus, nsAString& aSuggestedDriverVersion,
    const nsTArray<GfxDriverInfo>& aDriverInfo, nsACString& aFailureId,
    OperatingSystem* aOS /* = nullptr */)
>>>>>>> upstream-releases

{
  NS_ENSURE_ARG_POINTER(aStatus);
  *aStatus = nsIGfxInfo::FEATURE_STATUS_UNKNOWN;
  aSuggestedDriverVersion.SetIsVoid(true);
  OperatingSystem os = OperatingSystem::Linux;
  if (aOS) *aOS = os;

  if (sShutdownOccurred) {
    return NS_OK;
  }

  GetData();

  if (mGLMajorVersion == 0) {
    // If we failed to get a GL version, glxtest failed.
    *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DEVICE;
    aFailureId = "FEATURE_FAILURE_GLXTEST_FAILED";
    return NS_OK;
  }

  if (mGLMajorVersion == 1) {
    // We're on OpenGL 1. In most cases that indicates really old hardware.
    // We better block them, rather than rely on them to fail gracefully,
    // because they don't! see bug 696636
    *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DEVICE;
    aFailureId = "FEATURE_FAILURE_OPENGL_1";
    return NS_OK;
  }

<<<<<<< HEAD
  // Don't evaluate any special cases if we're checking the downloaded
  // blocklist.
  if (!aDriverInfo.Length()) {
    // Blacklist software GL implementations from using layers acceleration.
    // On the test infrastructure, we'll force-enable layers acceleration.
    if (aFeature == nsIGfxInfo::FEATURE_OPENGL_LAYERS &&
        (mIsLlvmpipe || mIsOldSwrast) &&
        !PR_GetEnv("MOZ_LAYERS_ALLOW_SOFTWARE_GL")) {
      *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DEVICE;
      aFailureId = "FEATURE_FAILURE_SOFTWARE_GL";
      return NS_OK;
    }

    if (aFeature == nsIGfxInfo::FEATURE_WEBRENDER) {
      *aStatus = nsIGfxInfo::FEATURE_BLOCKED_OS_VERSION;
      aFailureId = "FEATURE_UNQUALIFIED_WEBRENDER_LINUX";
      return NS_OK;
    }

    // Only check features relevant to Linux.
    if (aFeature == nsIGfxInfo::FEATURE_OPENGL_LAYERS ||
        aFeature == nsIGfxInfo::FEATURE_WEBGL_OPENGL ||
        aFeature == nsIGfxInfo::FEATURE_WEBGL2 ||
        aFeature == nsIGfxInfo::FEATURE_WEBGL_MSAA) {
      // whitelist the linux test slaves' current configuration.
      // this is necessary as they're still using the slightly outdated 190.42
      // driver. this isn't a huge risk, as at least this is the exact setting
      // in which we do continuous testing, and this only affects GeForce 9400
      // cards on linux on this precise driver version, which is very few users.
      // We do the same thing on Windows XP, see in widget/windows/GfxInfo.cpp
      if (mIsNVIDIA && !strcmp(mRenderer.get(), "GeForce 9400/PCI/SSE2") &&
          !strcmp(mVersion.get(), "3.2.0 NVIDIA 190.42")) {
        *aStatus = nsIGfxInfo::FEATURE_STATUS_OK;
        return NS_OK;
      }

      if (mIsMesa) {
        if (mIsNouveau &&
            version(mMajorVersion, mMinorVersion) < version(8, 0)) {
          *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION;
          aFailureId = "FEATURE_FAILURE_MESA_1";
          aSuggestedDriverVersion.AssignLiteral("Mesa 8.0");
        } else if (version(mMajorVersion, mMinorVersion, mRevisionVersion) <
                   version(7, 10, 3)) {
          *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION;
          aFailureId = "FEATURE_FAILURE_MESA_2";
          aSuggestedDriverVersion.AssignLiteral("Mesa 7.10.3");
        } else if (mIsOldSwrast) {
          *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION;
          aFailureId = "FEATURE_FAILURE_SW_RAST";
        } else if (mIsLlvmpipe &&
                   version(mMajorVersion, mMinorVersion) < version(9, 1)) {
          // bug 791905, Mesa bug 57733, fixed in Mesa 9.1 according to
          // https://bugs.freedesktop.org/show_bug.cgi?id=57733#c3
          *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION;
          aFailureId = "FEATURE_FAILURE_MESA_3";
        } else if (aFeature == nsIGfxInfo::FEATURE_WEBGL_MSAA) {
          if (mIsIntel &&
              version(mMajorVersion, mMinorVersion) < version(8, 1)) {
            *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION;
            aFailureId = "FEATURE_FAILURE_MESA_4";
            aSuggestedDriverVersion.AssignLiteral("Mesa 8.1");
          }
        }

      } else if (mIsNVIDIA) {
        if (version(mMajorVersion, mMinorVersion, mRevisionVersion) <
            version(257, 21)) {
          *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION;
          aFailureId = "FEATURE_FAILURE_OLD_NV";
          aSuggestedDriverVersion.AssignLiteral("NVIDIA 257.21");
        }
      } else if (mIsFGLRX) {
        // FGLRX does not report a driver version number, so we have the OpenGL
        // version instead. by requiring OpenGL 3, we effectively require recent
        // drivers.
        if (version(mMajorVersion, mMinorVersion, mRevisionVersion) <
            version(3, 0)) {
          *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION;
          aFailureId = "FEATURE_FAILURE_OLD_FGLRX";
          aSuggestedDriverVersion.AssignLiteral("<Something recent>");
        }
        // Bug 724640: FGLRX + Linux 2.6.32 is a crashy combo
        bool unknownOS = mOS.IsEmpty() || mOSRelease.IsEmpty();
        bool badOS =
            mOS.Find("Linux", true) != -1 && mOSRelease.Find("2.6.32") != -1;
        if (unknownOS || badOS) {
          *aStatus = nsIGfxInfo::FEATURE_BLOCKED_OS_VERSION;
          aFailureId = "FEATURE_FAILURE_OLD_OS";
        }
      } else {
        // like on windows, let's block unknown vendors. Think of virtual
        // machines. Also, this case is hit whenever the GLXtest probe failed to
        // get driver info or crashed.
        *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DEVICE;
      }
    }
||||||| merged common ancestors
  // Don't evaluate any special cases if we're checking the downloaded blocklist.
  if (!aDriverInfo.Length()) {
    // Blacklist software GL implementations from using layers acceleration.
    // On the test infrastructure, we'll force-enable layers acceleration.
    if (aFeature == nsIGfxInfo::FEATURE_OPENGL_LAYERS &&
        (mIsLlvmpipe || mIsOldSwrast) &&
        !PR_GetEnv("MOZ_LAYERS_ALLOW_SOFTWARE_GL"))
    {
      *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DEVICE;
      aFailureId = "FEATURE_FAILURE_SOFTWARE_GL";
      return NS_OK;
    }

    if (aFeature == nsIGfxInfo::FEATURE_WEBRENDER) {
      *aStatus = nsIGfxInfo::FEATURE_BLOCKED_OS_VERSION;
      aFailureId = "FEATURE_UNQUALIFIED_WEBRENDER_LINUX";
      return NS_OK;
    }

    // Only check features relevant to Linux.
    if (aFeature == nsIGfxInfo::FEATURE_OPENGL_LAYERS ||
        aFeature == nsIGfxInfo::FEATURE_WEBGL_OPENGL ||
        aFeature == nsIGfxInfo::FEATURE_WEBGL2 ||
        aFeature == nsIGfxInfo::FEATURE_WEBGL_MSAA) {

      // whitelist the linux test slaves' current configuration.
      // this is necessary as they're still using the slightly outdated 190.42 driver.
      // this isn't a huge risk, as at least this is the exact setting in which we do continuous testing,
      // and this only affects GeForce 9400 cards on linux on this precise driver version, which is very few users.
      // We do the same thing on Windows XP, see in widget/windows/GfxInfo.cpp
      if (mIsNVIDIA &&
          !strcmp(mRenderer.get(), "GeForce 9400/PCI/SSE2") &&
          !strcmp(mVersion.get(), "3.2.0 NVIDIA 190.42"))
      {
        *aStatus = nsIGfxInfo::FEATURE_STATUS_OK;
        return NS_OK;
      }

      if (mIsMesa) {
        if (mIsNouveau && version(mMajorVersion, mMinorVersion) < version(8,0)) {
          *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION;
          aFailureId = "FEATURE_FAILURE_MESA_1";
          aSuggestedDriverVersion.AssignLiteral("Mesa 8.0");
        }
        else if (version(mMajorVersion, mMinorVersion, mRevisionVersion) < version(7,10,3)) {
          *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION;
          aFailureId = "FEATURE_FAILURE_MESA_2";
          aSuggestedDriverVersion.AssignLiteral("Mesa 7.10.3");
        }
        else if (mIsOldSwrast) {
          *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION;
          aFailureId = "FEATURE_FAILURE_SW_RAST";
        }
        else if (mIsLlvmpipe && version(mMajorVersion, mMinorVersion) < version(9, 1)) {
          // bug 791905, Mesa bug 57733, fixed in Mesa 9.1 according to
          // https://bugs.freedesktop.org/show_bug.cgi?id=57733#c3
          *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION;
          aFailureId = "FEATURE_FAILURE_MESA_3";
        }
        else if (aFeature == nsIGfxInfo::FEATURE_WEBGL_MSAA)
        {
          if (mIsIntel && version(mMajorVersion, mMinorVersion) < version(8,1)) {
            *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION;
            aFailureId = "FEATURE_FAILURE_MESA_4";
            aSuggestedDriverVersion.AssignLiteral("Mesa 8.1");
          }
        }

      } else if (mIsNVIDIA) {
        if (version(mMajorVersion, mMinorVersion, mRevisionVersion) < version(257,21)) {
          *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION;
          aFailureId = "FEATURE_FAILURE_OLD_NV";
          aSuggestedDriverVersion.AssignLiteral("NVIDIA 257.21");
        }
      } else if (mIsFGLRX) {
        // FGLRX does not report a driver version number, so we have the OpenGL version instead.
        // by requiring OpenGL 3, we effectively require recent drivers.
        if (version(mMajorVersion, mMinorVersion, mRevisionVersion) < version(3, 0)) {
          *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DRIVER_VERSION;
          aFailureId = "FEATURE_FAILURE_OLD_FGLRX";
          aSuggestedDriverVersion.AssignLiteral("<Something recent>");
        }
        // Bug 724640: FGLRX + Linux 2.6.32 is a crashy combo
        bool unknownOS = mOS.IsEmpty() || mOSRelease.IsEmpty();
        bool badOS = mOS.Find("Linux", true) != -1 &&
                     mOSRelease.Find("2.6.32") != -1;
        if (unknownOS || badOS) {
          *aStatus = nsIGfxInfo::FEATURE_BLOCKED_OS_VERSION;
          aFailureId = "FEATURE_FAILURE_OLD_OS";
        }
      } else {
        // like on windows, let's block unknown vendors. Think of virtual machines.
        // Also, this case is hit whenever the GLXtest probe failed to get driver info or crashed.
        *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DEVICE;
      }
    }
=======
  // Blacklist software GL implementations from using layers acceleration.
  // On the test infrastructure, we'll force-enable layers acceleration.
  if (aFeature == nsIGfxInfo::FEATURE_OPENGL_LAYERS && !mIsAccelerated &&
      !PR_GetEnv("MOZ_LAYERS_ALLOW_SOFTWARE_GL")) {
    *aStatus = nsIGfxInfo::FEATURE_BLOCKED_DEVICE;
    aFailureId = "FEATURE_FAILURE_SOFTWARE_GL";
    return NS_OK;
>>>>>>> upstream-releases
  }

  return GfxInfoBase::GetFeatureStatusImpl(
      aFeature, aStatus, aSuggestedDriverVersion, aDriverInfo, aFailureId, &os);
}

<<<<<<< HEAD
||||||| merged common ancestors

=======
NS_IMETHODIMP
GfxInfo::GetD2DEnabled(bool* aEnabled) { return NS_ERROR_FAILURE; }

>>>>>>> upstream-releases
NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetD2DEnabled(bool *aEnabled) { return NS_ERROR_FAILURE; }
||||||| merged common ancestors
GfxInfo::GetD2DEnabled(bool *aEnabled)
{
  return NS_ERROR_FAILURE;
}
=======
GfxInfo::GetDWriteEnabled(bool* aEnabled) { return NS_ERROR_FAILURE; }
>>>>>>> upstream-releases

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetDWriteEnabled(bool *aEnabled) { return NS_ERROR_FAILURE; }
||||||| merged common ancestors
GfxInfo::GetDWriteEnabled(bool *aEnabled)
{
  return NS_ERROR_FAILURE;
}
=======
GfxInfo::GetDWriteVersion(nsAString& aDwriteVersion) {
  return NS_ERROR_FAILURE;
}
>>>>>>> upstream-releases

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetDWriteVersion(nsAString &aDwriteVersion) {
||||||| merged common ancestors
GfxInfo::GetDWriteVersion(nsAString & aDwriteVersion)
{
=======
GfxInfo::GetCleartypeParameters(nsAString& aCleartypeParams) {
>>>>>>> upstream-releases
  return NS_ERROR_FAILURE;
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetCleartypeParameters(nsAString &aCleartypeParams) {
  return NS_ERROR_FAILURE;
||||||| merged common ancestors
GfxInfo::GetCleartypeParameters(nsAString & aCleartypeParams)
{
  return NS_ERROR_FAILURE;
=======
GfxInfo::GetWindowProtocol(nsAString& aWindowProtocol) {
  if (mIsWayland) {
    if (mIsWaylandDRM) {
      aWindowProtocol.AssignLiteral("wayland (drm)");
    } else {
      aWindowProtocol.AssignLiteral("wayland");
    }
    return NS_OK;
  }

  aWindowProtocol.AssignLiteral("x11");
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetAdapterDescription(nsAString &aAdapterDescription) {
||||||| merged common ancestors
GfxInfo::GetAdapterDescription(nsAString & aAdapterDescription)
{
=======
GfxInfo::GetAdapterDescription(nsAString& aAdapterDescription) {
>>>>>>> upstream-releases
  GetData();
  AppendASCIItoUTF16(mAdapterDescription, aAdapterDescription);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetAdapterDescription2(nsAString &aAdapterDescription) {
||||||| merged common ancestors
GfxInfo::GetAdapterDescription2(nsAString & aAdapterDescription)
{
=======
GfxInfo::GetAdapterDescription2(nsAString& aAdapterDescription) {
>>>>>>> upstream-releases
  return NS_ERROR_FAILURE;
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetAdapterRAM(nsAString &aAdapterRAM) {
  aAdapterRAM.Truncate();
||||||| merged common ancestors
GfxInfo::GetAdapterRAM(nsAString & aAdapterRAM)
{
  aAdapterRAM.Truncate();
=======
GfxInfo::GetAdapterRAM(nsAString& aAdapterRAM) {
  GetData();
  CopyUTF8toUTF16(mAdapterRAM, aAdapterRAM);
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetAdapterRAM2(nsAString &aAdapterRAM) { return NS_ERROR_FAILURE; }
||||||| merged common ancestors
GfxInfo::GetAdapterRAM2(nsAString & aAdapterRAM)
{
  return NS_ERROR_FAILURE;
}
=======
GfxInfo::GetAdapterRAM2(nsAString& aAdapterRAM) { return NS_ERROR_FAILURE; }

NS_IMETHODIMP
GfxInfo::GetAdapterDriver(nsAString& aAdapterDriver) {
  aAdapterDriver.Truncate();
  return NS_OK;
}

NS_IMETHODIMP
GfxInfo::GetAdapterDriver2(nsAString& aAdapterDriver) {
  return NS_ERROR_FAILURE;
}
>>>>>>> upstream-releases

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetAdapterDriver(nsAString &aAdapterDriver) {
  aAdapterDriver.Truncate();
||||||| merged common ancestors
GfxInfo::GetAdapterDriver(nsAString & aAdapterDriver)
{
  aAdapterDriver.Truncate();
=======
GfxInfo::GetAdapterDriverVendor(nsAString& aAdapterDriverVendor) {
  GetData();
  CopyASCIItoUTF16(mDriverVendor, aAdapterDriverVendor);
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetAdapterDriver2(nsAString &aAdapterDriver) {
||||||| merged common ancestors
GfxInfo::GetAdapterDriver2(nsAString & aAdapterDriver)
{
=======
GfxInfo::GetAdapterDriverVendor2(nsAString& aAdapterDriverVendor) {
>>>>>>> upstream-releases
  return NS_ERROR_FAILURE;
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetAdapterDriverVersion(nsAString &aAdapterDriverVersion) {
||||||| merged common ancestors
GfxInfo::GetAdapterDriverVersion(nsAString & aAdapterDriverVersion)
{
=======
GfxInfo::GetAdapterDriverVersion(nsAString& aAdapterDriverVersion) {
>>>>>>> upstream-releases
  GetData();
  CopyASCIItoUTF16(mDriverVersion, aAdapterDriverVersion);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetAdapterDriverVersion2(nsAString &aAdapterDriverVersion) {
||||||| merged common ancestors
GfxInfo::GetAdapterDriverVersion2(nsAString & aAdapterDriverVersion)
{
=======
GfxInfo::GetAdapterDriverVersion2(nsAString& aAdapterDriverVersion) {
>>>>>>> upstream-releases
  return NS_ERROR_FAILURE;
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetAdapterDriverDate(nsAString &aAdapterDriverDate) {
||||||| merged common ancestors
GfxInfo::GetAdapterDriverDate(nsAString & aAdapterDriverDate)
{
=======
GfxInfo::GetAdapterDriverDate(nsAString& aAdapterDriverDate) {
>>>>>>> upstream-releases
  aAdapterDriverDate.Truncate();
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetAdapterDriverDate2(nsAString &aAdapterDriverDate) {
||||||| merged common ancestors
GfxInfo::GetAdapterDriverDate2(nsAString & aAdapterDriverDate)
{
=======
GfxInfo::GetAdapterDriverDate2(nsAString& aAdapterDriverDate) {
>>>>>>> upstream-releases
  return NS_ERROR_FAILURE;
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetAdapterVendorID(nsAString &aAdapterVendorID) {
||||||| merged common ancestors
GfxInfo::GetAdapterVendorID(nsAString & aAdapterVendorID)
{
=======
GfxInfo::GetAdapterVendorID(nsAString& aAdapterVendorID) {
>>>>>>> upstream-releases
  GetData();
  CopyUTF8toUTF16(mVendorId, aAdapterVendorID);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetAdapterVendorID2(nsAString &aAdapterVendorID) {
||||||| merged common ancestors
GfxInfo::GetAdapterVendorID2(nsAString & aAdapterVendorID)
{
=======
GfxInfo::GetAdapterVendorID2(nsAString& aAdapterVendorID) {
>>>>>>> upstream-releases
  return NS_ERROR_FAILURE;
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetAdapterDeviceID(nsAString &aAdapterDeviceID) {
||||||| merged common ancestors
GfxInfo::GetAdapterDeviceID(nsAString & aAdapterDeviceID)
{
=======
GfxInfo::GetAdapterDeviceID(nsAString& aAdapterDeviceID) {
>>>>>>> upstream-releases
  GetData();
  CopyUTF8toUTF16(mDeviceId, aAdapterDeviceID);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetAdapterDeviceID2(nsAString &aAdapterDeviceID) {
||||||| merged common ancestors
GfxInfo::GetAdapterDeviceID2(nsAString & aAdapterDeviceID)
{
=======
GfxInfo::GetAdapterDeviceID2(nsAString& aAdapterDeviceID) {
>>>>>>> upstream-releases
  return NS_ERROR_FAILURE;
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetAdapterSubsysID(nsAString &aAdapterSubsysID) {
||||||| merged common ancestors
GfxInfo::GetAdapterSubsysID(nsAString & aAdapterSubsysID)
{
=======
GfxInfo::GetAdapterSubsysID(nsAString& aAdapterSubsysID) {
>>>>>>> upstream-releases
  return NS_ERROR_FAILURE;
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetAdapterSubsysID2(nsAString &aAdapterSubsysID) {
||||||| merged common ancestors
GfxInfo::GetAdapterSubsysID2(nsAString & aAdapterSubsysID)
{
=======
GfxInfo::GetAdapterSubsysID2(nsAString& aAdapterSubsysID) {
>>>>>>> upstream-releases
  return NS_ERROR_FAILURE;
}

NS_IMETHODIMP
<<<<<<< HEAD
GfxInfo::GetIsGPU2Active(bool *aIsGPU2Active) { return NS_ERROR_FAILURE; }
||||||| merged common ancestors
GfxInfo::GetIsGPU2Active(bool* aIsGPU2Active)
{
  return NS_ERROR_FAILURE;
}
=======
GfxInfo::GetIsGPU2Active(bool* aIsGPU2Active) { return NS_ERROR_FAILURE; }
>>>>>>> upstream-releases

#ifdef DEBUG

// Implement nsIGfxInfoDebug
// We don't support spoofing anything on Linux

<<<<<<< HEAD
NS_IMETHODIMP GfxInfo::SpoofVendorID(const nsAString &aVendorID) {
  CopyUTF16toUTF8(aVendorID, mVendor);
||||||| merged common ancestors
NS_IMETHODIMP GfxInfo::SpoofVendorID(const nsAString & aVendorID)
{
  CopyUTF16toUTF8(aVendorID, mVendor);
=======
NS_IMETHODIMP GfxInfo::SpoofVendorID(const nsAString& aVendorID) {
  GetData();
  CopyUTF16toUTF8(aVendorID, mVendorId);
  mIsAccelerated = true;
>>>>>>> upstream-releases
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP GfxInfo::SpoofDeviceID(const nsAString &aDeviceID) {
  CopyUTF16toUTF8(aDeviceID, mRenderer);
||||||| merged common ancestors
NS_IMETHODIMP GfxInfo::SpoofDeviceID(const nsAString & aDeviceID)
{
  CopyUTF16toUTF8(aDeviceID, mRenderer);
=======
NS_IMETHODIMP GfxInfo::SpoofDeviceID(const nsAString& aDeviceID) {
  GetData();
  CopyUTF16toUTF8(aDeviceID, mDeviceId);
>>>>>>> upstream-releases
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP GfxInfo::SpoofDriverVersion(const nsAString &aDriverVersion) {
  CopyUTF16toUTF8(aDriverVersion, mVersion);
||||||| merged common ancestors
NS_IMETHODIMP GfxInfo::SpoofDriverVersion(const nsAString & aDriverVersion)
{
  CopyUTF16toUTF8(aDriverVersion, mVersion);
=======
NS_IMETHODIMP GfxInfo::SpoofDriverVersion(const nsAString& aDriverVersion) {
  GetData();
  CopyUTF16toUTF8(aDriverVersion, mDriverVersion);
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP GfxInfo::SpoofOSVersion(uint32_t aVersion) {
  // We don't support OS versioning on Linux. There's just "Linux".
  return NS_OK;
}

NS_IMETHODIMP GfxInfo::FireTestProcess() {
  // If the pid is zero, then we have never run the test process to query for
  // driver information. This would normally be run on startup, but we need to
  // manually invoke it for XPC shell tests.
  if (glxtest_pid == 0) {
    fire_glxtest_process();
  }
  return NS_OK;
}

#endif

}  // end namespace widget
}  // end namespace mozilla
