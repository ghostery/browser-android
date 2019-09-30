/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef WindowDbg_h__
#define WindowDbg_h__

/*
 * nsWindowDbg - Debug related utilities for nsWindow.
 */

#include "nsWindowDefs.h"

// Enabled main event loop debug event output
//#define EVENT_DEBUG_OUTPUT

// Enables debug output for popup rollup hooks
//#define POPUP_ROLLUP_DEBUG_OUTPUT

// Enable window size and state debug output
//#define WINSTATE_DEBUG_OUTPUT

// nsIWidget defines a set of debug output statements
// that are called in various places within the code.
//#define WIDGET_DEBUG_OUTPUT

// Enable IS_VK_DOWN debug output
//#define DEBUG_VK

// Main event loop debug output flags
#if defined(EVENT_DEBUG_OUTPUT)
<<<<<<< HEAD
#define SHOW_REPEAT_EVENTS true
#define SHOW_MOUSEMOVE_EVENTS false
#endif  // defined(EVENT_DEBUG_OUTPUT)
||||||| merged common ancestors
#define SHOW_REPEAT_EVENTS      true
#define SHOW_MOUSEMOVE_EVENTS   false
#endif // defined(EVENT_DEBUG_OUTPUT)
=======
#  define SHOW_REPEAT_EVENTS true
#  define SHOW_MOUSEMOVE_EVENTS false
#endif  // defined(EVENT_DEBUG_OUTPUT)
>>>>>>> upstream-releases

void PrintEvent(UINT msg, bool aShowAllEvents, bool aShowMouseMoves);

#if defined(POPUP_ROLLUP_DEBUG_OUTPUT)
typedef struct {
<<<<<<< HEAD
  char *mStr;
  int mId;
||||||| merged common ancestors
  char * mStr;
  int    mId;
=======
  char* mStr;
  int mId;
>>>>>>> upstream-releases
} MSGFEventMsgInfo;

<<<<<<< HEAD
#define DISPLAY_NMM_PRT(_arg) \
  MOZ_LOG(gWindowsLog, mozilla::LogLevel::Info, ((_arg)));
||||||| merged common ancestors
#define DISPLAY_NMM_PRT(_arg) MOZ_LOG(gWindowsLog, mozilla::LogLevel::Info, ((_arg)));
=======
#  define DISPLAY_NMM_PRT(_arg) \
    MOZ_LOG(gWindowsLog, mozilla::LogLevel::Info, ((_arg)));
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
#define DISPLAY_NMM_PRT(_arg)
#endif  // defined(POPUP_ROLLUP_DEBUG_OUTPUT)
||||||| merged common ancestors
#define DISPLAY_NMM_PRT(_arg)
#endif // defined(POPUP_ROLLUP_DEBUG_OUTPUT)
=======
#  define DISPLAY_NMM_PRT(_arg)
#endif  // defined(POPUP_ROLLUP_DEBUG_OUTPUT)
>>>>>>> upstream-releases

#if defined(DEBUG)
<<<<<<< HEAD
void DDError(const char *msg, HRESULT hr);
#endif  // defined(DEBUG)
||||||| merged common ancestors
void DDError(const char *msg, HRESULT hr);
#endif // defined(DEBUG)
=======
void DDError(const char* msg, HRESULT hr);
#endif  // defined(DEBUG)
>>>>>>> upstream-releases

#if defined(DEBUG_VK)
bool is_vk_down(int vk);
#  define IS_VK_DOWN is_vk_down
#else
<<<<<<< HEAD
#define IS_VK_DOWN(a) (GetKeyState(a) < 0)
#endif  // defined(DEBUG_VK)
||||||| merged common ancestors
#define IS_VK_DOWN(a) (GetKeyState(a) < 0)
#endif // defined(DEBUG_VK)
=======
#  define IS_VK_DOWN(a) (GetKeyState(a) < 0)
#endif  // defined(DEBUG_VK)
>>>>>>> upstream-releases

#endif /* WindowDbg_h__ */
