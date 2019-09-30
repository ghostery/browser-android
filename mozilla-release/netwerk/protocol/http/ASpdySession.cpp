/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 ts=8 et tw=80 : */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// HttpLog.h should generally be included first
#include "HttpLog.h"

/*
  Currently supported is h2
*/

#include "nsHttp.h"
#include "nsHttpHandler.h"

#include "ASpdySession.h"
#include "PSpdyPush.h"
#include "Http2Push.h"
#include "Http2Session.h"

#include "mozilla/Telemetry.h"

namespace mozilla {
namespace net {

<<<<<<< HEAD
ASpdySession *ASpdySession::NewSpdySession(net::SpdyVersion version,
                                           nsISocketTransport *aTransport,
                                           bool attemptingEarlyData) {
||||||| merged common ancestors
ASpdySession *
ASpdySession::NewSpdySession(net::SpdyVersion version,
                             nsISocketTransport *aTransport,
                             bool attemptingEarlyData)
{
=======
ASpdySession* ASpdySession::NewSpdySession(net::SpdyVersion version,
                                           nsISocketTransport* aTransport,
                                           bool attemptingEarlyData) {
>>>>>>> upstream-releases
  // This is a necko only interface, so we can enforce version
  // requests as a precondition
  MOZ_ASSERT(version == SpdyVersion::HTTP_2, "Unsupported spdy version");

  // Don't do a runtime check of IsSpdyV?Enabled() here because pref value
  // may have changed since starting negotiation. The selected protocol comes
  // from a list provided in the SERVER HELLO filtered by our acceptable
  // versions, so there is no risk of the server ignoring our prefs.

<<<<<<< HEAD
  Telemetry::Accumulate(Telemetry::SPDY_VERSION2,
                        static_cast<uint32_t>(version));

||||||| merged common ancestors
  Telemetry::Accumulate(Telemetry::SPDY_VERSION2, static_cast<uint32_t>(version));

=======
>>>>>>> upstream-releases
  return new Http2Session(aTransport, version, attemptingEarlyData);
}

SpdyInformation::SpdyInformation() {
  // highest index of enabled protocols is the
  // most preferred for ALPN negotiaton
  Version[0] = SpdyVersion::HTTP_2;
  VersionString[0] = NS_LITERAL_CSTRING("h2");
  ALPNCallbacks[0] = Http2Session::ALPNCallback;
}

bool SpdyInformation::ProtocolEnabled(uint32_t index) const {
  MOZ_ASSERT(index < kCount, "index out of range");

  return gHttpHandler->IsHttp2Enabled();
}

<<<<<<< HEAD
nsresult SpdyInformation::GetNPNIndex(const nsACString &npnString,
                                      uint32_t *result) const {
  if (npnString.IsEmpty()) return NS_ERROR_FAILURE;
||||||| merged common ancestors
nsresult
SpdyInformation::GetNPNIndex(const nsACString &npnString,
                             uint32_t *result) const
{
  if (npnString.IsEmpty())
    return NS_ERROR_FAILURE;
=======
nsresult SpdyInformation::GetNPNIndex(const nsACString& npnString,
                                      uint32_t* result) const {
  if (npnString.IsEmpty()) return NS_ERROR_FAILURE;
>>>>>>> upstream-releases

  for (uint32_t index = 0; index < kCount; ++index) {
    if (npnString.Equals(VersionString[index])) {
      *result = index;
      return NS_OK;
    }
  }

  return NS_ERROR_FAILURE;
}

//////////////////////////////////////////
// SpdyPushCache
//////////////////////////////////////////

SpdyPushCache::~SpdyPushCache() { mHashHttp2.Clear(); }

<<<<<<< HEAD
bool SpdyPushCache::RegisterPushedStreamHttp2(const nsCString &key,
                                              Http2PushedStream *stream) {
  LOG3(("SpdyPushCache::RegisterPushedStreamHttp2 %s 0x%X\n", key.get(),
        stream->StreamID()));
  if (mHashHttp2.Get(key)) {
||||||| merged common ancestors
bool
SpdyPushCache::RegisterPushedStreamHttp2(const nsCString& key,
                                         Http2PushedStream *stream)
{
  LOG3(("SpdyPushCache::RegisterPushedStreamHttp2 %s 0x%X\n",
        key.get(), stream->StreamID()));
  if(mHashHttp2.Get(key)) {
=======
bool SpdyPushCache::RegisterPushedStreamHttp2(const nsCString& key,
                                              Http2PushedStream* stream) {
  LOG3(("SpdyPushCache::RegisterPushedStreamHttp2 %s 0x%X\n", key.get(),
        stream->StreamID()));
  if (mHashHttp2.Get(key)) {
>>>>>>> upstream-releases
    LOG3(("SpdyPushCache::RegisterPushedStreamHttp2 %s 0x%X duplicate key\n",
          key.get(), stream->StreamID()));
    return false;
  }
  mHashHttp2.Put(key, stream);
  return true;
}

<<<<<<< HEAD
Http2PushedStream *SpdyPushCache::RemovePushedStreamHttp2(
    const nsCString &key) {
  Http2PushedStream *rv = mHashHttp2.Get(key);
  LOG3(("SpdyPushCache::RemovePushedStreamHttp2 %s 0x%X\n", key.get(),
        rv ? rv->StreamID() : 0));
  if (rv) mHashHttp2.Remove(key);
||||||| merged common ancestors
Http2PushedStream *
SpdyPushCache::RemovePushedStreamHttp2(const nsCString& key)
{
  Http2PushedStream *rv = mHashHttp2.Get(key);
  LOG3(("SpdyPushCache::RemovePushedStreamHttp2 %s 0x%X\n",
        key.get(), rv ? rv->StreamID() : 0));
  if (rv)
    mHashHttp2.Remove(key);
=======
Http2PushedStream* SpdyPushCache::RemovePushedStreamHttp2(
    const nsCString& key) {
  Http2PushedStream* rv = mHashHttp2.Get(key);
  LOG3(("SpdyPushCache::RemovePushedStreamHttp2 %s 0x%X\n", key.get(),
        rv ? rv->StreamID() : 0));
  if (rv) mHashHttp2.Remove(key);
>>>>>>> upstream-releases
  return rv;
}

<<<<<<< HEAD
Http2PushedStream *SpdyPushCache::RemovePushedStreamHttp2ByID(
    const nsCString &key, const uint32_t &streamID) {
  Http2PushedStream *rv = mHashHttp2.Get(key);
  LOG3(("SpdyPushCache::RemovePushedStreamHttp2ByID %s 0x%X 0x%X", key.get(),
        rv ? rv->StreamID() : 0, streamID));
||||||| merged common ancestors
Http2PushedStream *
SpdyPushCache::RemovePushedStreamHttp2ByID(const nsCString& key, const uint32_t& streamID)
{
  Http2PushedStream *rv = mHashHttp2.Get(key);
  LOG3(("SpdyPushCache::RemovePushedStreamHttp2ByID %s 0x%X 0x%X",
        key.get(), rv ? rv->StreamID() : 0, streamID));
=======
Http2PushedStream* SpdyPushCache::RemovePushedStreamHttp2ByID(
    const nsCString& key, const uint32_t& streamID) {
  Http2PushedStream* rv = mHashHttp2.Get(key);
  LOG3(("SpdyPushCache::RemovePushedStreamHttp2ByID %s 0x%X 0x%X", key.get(),
        rv ? rv->StreamID() : 0, streamID));
>>>>>>> upstream-releases
  if (rv && streamID == rv->StreamID()) {
    mHashHttp2.Remove(key);
  } else {
    // Ensure we overwrite our rv with null in case the stream IDs don't match
    rv = nullptr;
  }
  return rv;
}

}  // namespace net
}  // namespace mozilla
