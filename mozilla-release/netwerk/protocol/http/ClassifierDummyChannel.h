/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 ts=8 et tw=80 : */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_net_ClassifierDummyChannel_h
#define mozilla_net_ClassifierDummyChannel_h

#include "nsIChannel.h"
#include <functional>

<<<<<<< HEAD:mozilla-release/netwerk/protocol/http/TrackingDummyChannel.h
#define TRACKING_DUMMY_CHANNEL_IID                   \
  {                                                  \
    0x70ceb97d, 0xbfa6, 0x4255, {                    \
      0xb7, 0x08, 0xe1, 0xb4, 0x4a, 0x1e, 0x0e, 0x9a \
    }                                                \
  }
||||||| merged common ancestors
#define TRACKING_DUMMY_CHANNEL_IID \
  { 0x70ceb97d, 0xbfa6, 0x4255, \
    { 0xb7, 0x08, 0xe1, 0xb4, 0x4a, 0x1e, 0x0e, 0x9a } }
=======
#define CLASSIFIER_DUMMY_CHANNEL_IID                 \
  {                                                  \
    0x70ceb97d, 0xbfa6, 0x4255, {                    \
      0xb7, 0x08, 0xe1, 0xb4, 0x4a, 0x1e, 0x0e, 0x9a \
    }                                                \
  }
>>>>>>> upstream-releases:mozilla-release/netwerk/protocol/http/ClassifierDummyChannel.h

class nsIChannel;

namespace mozilla {
namespace net {

/**
 * In child intercept mode, the decision to intercept a channel is made in the
 * child process without consulting the parent process.  The decision is based
 * on whether there is a ServiceWorker with a scope covering the URL in question
 * and whether storage is allowed for the origin/URL.  When the
 * "network.cookie.cookieBehavior" preference is set to BEHAVIOR_REJECT_TRACKER,
 * annotated channels are denied storage which means that the ServiceWorker
 * should not intercept the channel.  However, the decision for tracking
 * protection to annotate a channel only occurs in the parent process.  The
 * dummy channel is a hack to allow the intercept decision process to ask the
 * parent process if the channel should be annotated.  Because this round-trip
 * to the parent has overhead, the dummy channel is only created 1) if the
 * ServiceWorker initially determines that the channel should be intercepted and
 * 2) it's a navigation request.
 *
 * This hack can be removed once Bug 1231208's new "parent intercept" mechanism
 * fully lands, the pref is enabled by default it stays enabled for long enough
 * to be confident we will never need/want to turn it off.  Then as part of bug
 * 1496997 we can remove this implementation.  Bug 1498259 covers removing this
 * hack in particular.
 */
<<<<<<< HEAD:mozilla-release/netwerk/protocol/http/TrackingDummyChannel.h
class TrackingDummyChannel final : public nsIChannel,
                                   public nsIHttpChannelInternal {
 public:
  NS_DECLARE_STATIC_IID_ACCESSOR(TRACKING_DUMMY_CHANNEL_IID)
||||||| merged common ancestors
class TrackingDummyChannel final : public nsIChannel
                                 , public nsIHttpChannelInternal
{
public:
  NS_DECLARE_STATIC_IID_ACCESSOR(TRACKING_DUMMY_CHANNEL_IID)
=======
class ClassifierDummyChannel final : public nsIChannel,
                                     public nsIHttpChannelInternal {
 public:
  NS_DECLARE_STATIC_IID_ACCESSOR(CLASSIFIER_DUMMY_CHANNEL_IID)
>>>>>>> upstream-releases:mozilla-release/netwerk/protocol/http/ClassifierDummyChannel.h

  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIREQUEST
  NS_DECL_NSICHANNEL
  NS_DECL_NSIHTTPCHANNELINTERNAL

  enum StorageAllowedState {
    eStorageGranted,
    eStorageDenied,
    eAsyncNeeded,
  };

  static StorageAllowedState StorageAllowed(
      nsIChannel* aChannel, const std::function<void(bool)>& aCallback);

<<<<<<< HEAD:mozilla-release/netwerk/protocol/http/TrackingDummyChannel.h
  TrackingDummyChannel(nsIURI* aURI, nsIURI* aTopWindowURI,
                       nsresult aTopWindowURIResult, nsILoadInfo* aLoadInfo);
||||||| merged common ancestors
  TrackingDummyChannel(nsIURI* aURI,
                       nsIURI* aTopWindowURI,
                       nsresult aTopWindowURIResult,
                       nsILoadInfo* aLoadInfo);
=======
  ClassifierDummyChannel(nsIURI* aURI, nsIURI* aTopWindowURI,
                         nsresult aTopWindowURIResult, nsILoadInfo* aLoadInfo);
>>>>>>> upstream-releases:mozilla-release/netwerk/protocol/http/ClassifierDummyChannel.h

<<<<<<< HEAD:mozilla-release/netwerk/protocol/http/TrackingDummyChannel.h
  bool IsTrackingResource() const;
||||||| merged common ancestors
  bool
  IsTrackingResource() const;
=======
  uint32_t ClassificationFlags() const;
>>>>>>> upstream-releases:mozilla-release/netwerk/protocol/http/ClassifierDummyChannel.h

<<<<<<< HEAD:mozilla-release/netwerk/protocol/http/TrackingDummyChannel.h
  void SetIsTrackingResource();
||||||| merged common ancestors
  void
  SetIsTrackingResource();
=======
  void AddClassificationFlags(uint32_t);
>>>>>>> upstream-releases:mozilla-release/netwerk/protocol/http/ClassifierDummyChannel.h

<<<<<<< HEAD:mozilla-release/netwerk/protocol/http/TrackingDummyChannel.h
 private:
  ~TrackingDummyChannel();
||||||| merged common ancestors
private:
  ~TrackingDummyChannel();
=======
 private:
  ~ClassifierDummyChannel();
>>>>>>> upstream-releases:mozilla-release/netwerk/protocol/http/ClassifierDummyChannel.h

  nsCOMPtr<nsILoadInfo> mLoadInfo;
  nsCOMPtr<nsIURI> mURI;
  nsCOMPtr<nsIURI> mTopWindowURI;
  nsresult mTopWindowURIResult;

  uint32_t mClassificationFlags;
};

NS_DEFINE_STATIC_IID_ACCESSOR(ClassifierDummyChannel,
                              CLASSIFIER_DUMMY_CHANNEL_IID)

}  // namespace net
}  // namespace mozilla

<<<<<<< HEAD:mozilla-release/netwerk/protocol/http/TrackingDummyChannel.h
#endif  // mozilla_net_TrackingDummyChannel_h
||||||| merged common ancestors
#endif // mozilla_net_TrackingDummyChannel_h
=======
#endif  // mozilla_net_ClassifierDummyChannel_h
>>>>>>> upstream-releases:mozilla-release/netwerk/protocol/http/ClassifierDummyChannel.h
