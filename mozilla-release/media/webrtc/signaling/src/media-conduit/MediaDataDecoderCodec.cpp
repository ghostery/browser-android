/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MediaDataDecoderCodec.h"
#include "WebrtcMediaDataDecoderCodec.h"
#include "mozilla/StaticPrefs.h"

namespace mozilla {

<<<<<<< HEAD
/* static */ WebrtcVideoEncoder* MediaDataDecoderCodec::CreateEncoder(
    webrtc::VideoCodecType aCodecType) {
||||||| merged common ancestors
/* static */ WebrtcVideoEncoder*
MediaDataDecoderCodec::CreateEncoder(
  webrtc::VideoCodecType aCodecType)
{
=======
/* static */
WebrtcVideoEncoder* MediaDataDecoderCodec::CreateEncoder(
    webrtc::VideoCodecType aCodecType) {
>>>>>>> upstream-releases
  return nullptr;
}

<<<<<<< HEAD
/* static */ WebrtcVideoDecoder* MediaDataDecoderCodec::CreateDecoder(
    webrtc::VideoCodecType aCodecType) {
||||||| merged common ancestors
/* static */ WebrtcVideoDecoder*
MediaDataDecoderCodec::CreateDecoder(
  webrtc::VideoCodecType aCodecType)
{
  if (!StaticPrefs::MediaNavigatorMediadatadecoderEnabled() &&
      !StaticPrefs::MediaNavigatorMediadatadecoderH264Enabled()) {
    return nullptr;
  }

=======
/* static */
WebrtcVideoDecoder* MediaDataDecoderCodec::CreateDecoder(
    webrtc::VideoCodecType aCodecType) {
>>>>>>> upstream-releases
  switch (aCodecType) {
    case webrtc::VideoCodecType::kVideoCodecVP8:
    case webrtc::VideoCodecType::kVideoCodecVP9:
<<<<<<< HEAD
      if (!StaticPrefs::MediaNavigatorMediadatadecoderVPXEnabled()) {
||||||| merged common ancestors
      if (!StaticPrefs::MediaNavigatorMediadatadecoderEnabled()) {
=======
      if (!StaticPrefs::media_navigator_mediadatadecoder_vpx_enabled()) {
>>>>>>> upstream-releases
        return nullptr;
      }
      break;
    case webrtc::VideoCodecType::kVideoCodecH264:
<<<<<<< HEAD
      if (!StaticPrefs::MediaNavigatorMediadatadecoderH264Enabled()) {
        return nullptr;
      }
||||||| merged common ancestors
=======
      if (!StaticPrefs::media_navigator_mediadatadecoder_h264_enabled()) {
        return nullptr;
      }
>>>>>>> upstream-releases
      break;
    default:
      return nullptr;
  }
  return new WebrtcMediaDataDecoder();
}

}  // namespace mozilla
