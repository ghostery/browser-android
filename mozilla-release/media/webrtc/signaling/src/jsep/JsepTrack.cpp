/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "signaling/src/jsep/JsepTrack.h"
#include "signaling/src/jsep/JsepCodecDescription.h"
#include "signaling/src/jsep/JsepTrackEncoding.h"

#include <algorithm>
#include <iostream>

namespace mozilla {
void JsepTrack::GetNegotiatedPayloadTypes(
    std::vector<uint16_t>* payloadTypes) const {
  if (!mNegotiatedDetails) {
    return;
  }

  for (const auto& encoding : mNegotiatedDetails->mEncodings) {
    GetPayloadTypes(encoding->GetCodecs(), payloadTypes);
  }

  // Prune out dupes
  std::sort(payloadTypes->begin(), payloadTypes->end());
  auto newEnd = std::unique(payloadTypes->begin(), payloadTypes->end());
  payloadTypes->erase(newEnd, payloadTypes->end());
}

/* static */
void JsepTrack::GetPayloadTypes(
    const std::vector<UniquePtr<JsepCodecDescription>>& codecs,
    std::vector<uint16_t>* payloadTypes) {
  for (const auto& codec : codecs) {
    uint16_t pt;
    if (!codec->GetPtAsInt(&pt)) {
      MOZ_ASSERT(false);
      continue;
    }
    payloadTypes->push_back(pt);
  }
}

void JsepTrack::EnsureNoDuplicatePayloadTypes(
    std::vector<UniquePtr<JsepCodecDescription>>* codecs) {
  std::set<uint16_t> uniquePayloadTypes;

  for (auto& codec : *codecs) {
    // We assume there are no dupes in negotiated codecs; unnegotiated codecs
    // need to change if there is a clash.
    if (!codec->mEnabled ||
        // We only support one datachannel per m-section
        !codec->mName.compare("webrtc-datachannel")) {
      continue;
    }

    // Disable, and only re-enable if we can ensure it has a unique pt.
    codec->mEnabled = false;

    uint16_t currentPt;
    if (!codec->GetPtAsInt(&currentPt)) {
      MOZ_ASSERT(false);
      continue;
    }

    if (!uniquePayloadTypes.count(currentPt)) {
      codec->mEnabled = true;
      uniquePayloadTypes.insert(currentPt);
      continue;
    }

    // |codec| cannot use its current payload type. Try to find another.
    for (uint16_t freePt = 96; freePt <= 127; ++freePt) {
      // Not super efficient, but readability is probably more important.
      if (!uniquePayloadTypes.count(freePt)) {
        uniquePayloadTypes.insert(freePt);
        codec->mEnabled = true;
        std::ostringstream os;
        os << freePt;
        codec->mDefaultPt = os.str();
        break;
      }
    }
  }
}

void JsepTrack::EnsureSsrcs(SsrcGenerator& ssrcGenerator) {
  if (mSsrcs.empty()) {
    uint32_t ssrc;
    if (!ssrcGenerator.GenerateSsrc(&ssrc)) {
      return;
    }
    mSsrcs.push_back(ssrc);
  }
}

<<<<<<< HEAD
void JsepTrack::PopulateCodecs(
    const std::vector<UniquePtr<JsepCodecDescription>>& prototype) {
  for (const auto& prototypeCodec : prototype) {
||||||| merged common ancestors
void
JsepTrack::PopulateCodecs(const std::vector<JsepCodecDescription*>& prototype)
{
  for (const JsepCodecDescription* prototypeCodec : prototype) {
=======
void JsepTrack::PopulateCodecs(
    const std::vector<UniquePtr<JsepCodecDescription>>& prototype) {
  mPrototypeCodecs.clear();
  for (const auto& prototypeCodec : prototype) {
>>>>>>> upstream-releases
    if (prototypeCodec->mType == mType) {
      mPrototypeCodecs.emplace_back(prototypeCodec->Clone());
      mPrototypeCodecs.back()->mDirection = mDirection;
    }
  }

  EnsureNoDuplicatePayloadTypes(&mPrototypeCodecs);
}

<<<<<<< HEAD
void JsepTrack::AddToOffer(SsrcGenerator& ssrcGenerator, bool encodeTrackId,
                           SdpMediaSection* offer) {
  AddToMsection(mPrototypeCodecs, encodeTrackId, offer);
||||||| merged common ancestors
void
JsepTrack::AddToOffer(SsrcGenerator& ssrcGenerator, SdpMediaSection* offer)
{
  AddToMsection(mPrototypeCodecs.values, offer);
=======
void JsepTrack::AddToOffer(SsrcGenerator& ssrcGenerator,
                           SdpMediaSection* offer) {
  AddToMsection(mPrototypeCodecs, offer);
>>>>>>> upstream-releases

  if (mDirection == sdp::kSend) {
    std::vector<JsConstraints> constraints;
    if (offer->IsSending()) {
      constraints = mJsEncodeConstraints;
    }
    AddToMsection(constraints, sdp::kSend, ssrcGenerator, offer);
  }
}

<<<<<<< HEAD
void JsepTrack::AddToAnswer(const SdpMediaSection& offer,
                            SsrcGenerator& ssrcGenerator, bool encodeTrackId,
                            SdpMediaSection* answer) {
||||||| merged common ancestors
void
JsepTrack::AddToAnswer(const SdpMediaSection& offer,
                       SsrcGenerator& ssrcGenerator,
                       SdpMediaSection* answer)
{
=======
void JsepTrack::AddToAnswer(const SdpMediaSection& offer,
                            SsrcGenerator& ssrcGenerator,
                            SdpMediaSection* answer) {
>>>>>>> upstream-releases
  // We do not modify mPrototypeCodecs here, since we're only creating an
  // answer. Once offer/answer concludes, we will update mPrototypeCodecs.
<<<<<<< HEAD
  std::vector<UniquePtr<JsepCodecDescription>> codecs;
  codecs = GetCodecClones();
  NegotiateCodecs(offer, &codecs);
  if (codecs.empty()) {
||||||| merged common ancestors
  PtrVector<JsepCodecDescription> codecs;
  codecs.values = GetCodecClones();
  NegotiateCodecs(offer, &codecs.values);
  if (codecs.values.empty()) {
=======
  std::vector<UniquePtr<JsepCodecDescription>> codecs =
      NegotiateCodecs(offer, true);
  if (codecs.empty()) {
>>>>>>> upstream-releases
    return;
  }

<<<<<<< HEAD
  AddToMsection(codecs, encodeTrackId, answer);
||||||| merged common ancestors
  AddToMsection(codecs.values, answer);
=======
  AddToMsection(codecs, answer);
>>>>>>> upstream-releases

  if (mDirection == sdp::kSend) {
    std::vector<JsConstraints> constraints;
    if (answer->IsSending()) {
      constraints = mJsEncodeConstraints;
      std::vector<SdpRidAttributeList::Rid> rids;
      GetRids(offer, sdp::kRecv, &rids);
      NegotiateRids(rids, &constraints);
    }
    AddToMsection(constraints, sdp::kSend, ssrcGenerator, answer);
  }
}

bool JsepTrack::SetJsConstraints(
    const std::vector<JsConstraints>& constraintsList) {
  bool constraintsChanged = mJsEncodeConstraints != constraintsList;
  mJsEncodeConstraints = constraintsList;

  // Also update negotiated details with constraints, as these can change
  // without negotiation.

  if (!mNegotiatedDetails) {
    return constraintsChanged;
  }

  for (auto& encoding : mNegotiatedDetails->mEncodings) {
    for (const JsConstraints& jsConstraints : mJsEncodeConstraints) {
      if (jsConstraints.rid == encoding->mRid) {
        encoding->mConstraints = jsConstraints.constraints;
      }
    }
  }

  return constraintsChanged;
}

<<<<<<< HEAD
void JsepTrack::AddToMsection(
    const std::vector<UniquePtr<JsepCodecDescription>>& codecs,
    bool encodeTrackId, SdpMediaSection* msection) {
||||||| merged common ancestors
void
JsepTrack::AddToMsection(const std::vector<JsepCodecDescription*>& codecs,
                         SdpMediaSection* msection)
{
=======
void JsepTrack::AddToMsection(
    const std::vector<UniquePtr<JsepCodecDescription>>& codecs,
    SdpMediaSection* msection) {
>>>>>>> upstream-releases
  MOZ_ASSERT(msection->GetMediaType() == mType);
  MOZ_ASSERT(!codecs.empty());

  for (const auto& codec : codecs) {
    codec->AddToMediaSection(*msection);
  }

  if ((mDirection == sdp::kSend) && (mType != SdpMediaSection::kApplication) &&
      msection->IsSending()) {
    if (mStreamIds.empty()) {
      msection->AddMsid("-", encodeTrackId ? mTrackId : "");
    } else {
      for (const std::string& streamId : mStreamIds) {
<<<<<<< HEAD
        msection->AddMsid(streamId, encodeTrackId ? mTrackId : "");
        // TODO(bug 1402912) Interop hack; older Firefox barfs if there is more
        // than one msid. Remove when safe.
        break;
||||||| merged common ancestors
        msection->AddMsid(streamId, mTrackId);
        // TODO(bug 1402912) Interop hack; older Firefox barfs if there is more
        // than one msid. Remove when safe.
        break;
=======
        msection->AddMsid(streamId, mTrackId);
>>>>>>> upstream-releases
      }
    }
  }
}

// Updates the |id| values in |constraintsList| with the rid values in |rids|,
// where necessary.
void JsepTrack::NegotiateRids(
    const std::vector<SdpRidAttributeList::Rid>& rids,
    std::vector<JsConstraints>* constraintsList) const {
  for (const SdpRidAttributeList::Rid& rid : rids) {
    if (!FindConstraints(rid.id, *constraintsList)) {
      // Pair up the first JsConstraints with an empty id, if it exists.
      JsConstraints* constraints = FindConstraints("", *constraintsList);
      if (constraints) {
        constraints->rid = rid.id;
      }
    }
  }
}

void JsepTrack::UpdateSsrcs(SsrcGenerator& ssrcGenerator, size_t encodings) {
  MOZ_ASSERT(mDirection == sdp::kSend);
  MOZ_ASSERT(mType != SdpMediaSection::kApplication);
  size_t numSsrcs = std::max<size_t>(encodings, 1U);

  // Right now, the spec does not permit changing the number of encodings after
  // the initial creation of the sender, so we don't need to worry about things
  // like a new encoding inserted in between two pre-existing encodings.
  while (mSsrcs.size() < numSsrcs) {
    uint32_t ssrc;
    if (!ssrcGenerator.GenerateSsrc(&ssrc)) {
      return;
    }
    mSsrcs.push_back(ssrc);
  }

  mSsrcs.resize(numSsrcs);
  MOZ_ASSERT(!mSsrcs.empty());
}

void JsepTrack::AddToMsection(const std::vector<JsConstraints>& constraintsList,
                              sdp::Direction direction,
                              SsrcGenerator& ssrcGenerator,
                              SdpMediaSection* msection) {
  UniquePtr<SdpSimulcastAttribute> simulcast(new SdpSimulcastAttribute);
  UniquePtr<SdpRidAttributeList> rids(new SdpRidAttributeList);
  for (const JsConstraints& constraints : constraintsList) {
    if (!constraints.rid.empty()) {
      SdpRidAttributeList::Rid rid;
      rid.id = constraints.rid;
      rid.direction = direction;
      rids->mRids.push_back(rid);

      SdpSimulcastAttribute::Version version;
      version.choices.push_back(constraints.rid);
      if (direction == sdp::kSend) {
        simulcast->sendVersions.push_back(version);
      } else {
        simulcast->recvVersions.push_back(version);
      }
    }
  }

  if (rids->mRids.size() > 1) {
    msection->GetAttributeList().SetAttribute(simulcast.release());
    msection->GetAttributeList().SetAttribute(rids.release());
  }

  if (mType != SdpMediaSection::kApplication && mDirection == sdp::kSend) {
    UpdateSsrcs(ssrcGenerator, constraintsList.size());
    msection->SetSsrcs(mSsrcs, mCNAME);
  }
}

void JsepTrack::GetRids(const SdpMediaSection& msection,
                        sdp::Direction direction,
                        std::vector<SdpRidAttributeList::Rid>* rids) const {
  rids->clear();
  if (!msection.GetAttributeList().HasAttribute(
          SdpAttribute::kSimulcastAttribute)) {
    return;
  }

  const SdpSimulcastAttribute& simulcast(
      msection.GetAttributeList().GetSimulcast());

  const SdpSimulcastAttribute::Versions* versions = nullptr;
  switch (direction) {
    case sdp::kSend:
      versions = &simulcast.sendVersions;
      break;
    case sdp::kRecv:
      versions = &simulcast.recvVersions;
      break;
  }

  if (!versions->IsSet()) {
    return;
  }

  if (versions->type != SdpSimulcastAttribute::Versions::kRid) {
    // No support for PT-based simulcast, yet.
    return;
  }

  for (const SdpSimulcastAttribute::Version& version : *versions) {
    if (!version.choices.empty()) {
      // We validate that rids are present (and sane) elsewhere.
      rids->push_back(*msection.FindRid(version.choices[0]));
    }
  }
}

JsepTrack::JsConstraints* JsepTrack::FindConstraints(
    const std::string& id, std::vector<JsConstraints>& constraintsList) const {
  for (JsConstraints& constraints : constraintsList) {
    if (constraints.rid == id) {
      return &constraints;
    }
  }
  return nullptr;
}

void JsepTrack::CreateEncodings(
    const SdpMediaSection& remote,
    const std::vector<UniquePtr<JsepCodecDescription>>& negotiatedCodecs,
    JsepTrackNegotiatedDetails* negotiatedDetails) {
  negotiatedDetails->mTias = remote.GetBandwidth("TIAS");
  // TODO add support for b=AS if TIAS is not set (bug 976521)

  std::vector<SdpRidAttributeList::Rid> rids;
  GetRids(remote, sdp::kRecv, &rids);  // Get rids we will send
  NegotiateRids(rids, &mJsEncodeConstraints);
  if (rids.empty()) {
    // Add dummy value with an empty id to make sure we get a single unicast
    // stream.
    rids.push_back(SdpRidAttributeList::Rid());
  }

  size_t max_streams = 1;

  if (!mJsEncodeConstraints.empty()) {
    max_streams = std::min(rids.size(), mJsEncodeConstraints.size());
  }
  // Drop SSRCs if less RIDs were offered than we have encoding constraints
  // Just in case.
  if (mSsrcs.size() > max_streams) {
    mSsrcs.resize(max_streams);
  }

  // For each stream make sure we have an encoding, and configure
  // that encoding appropriately.
  for (size_t i = 0; i < max_streams; ++i) {
    if (i == negotiatedDetails->mEncodings.size()) {
      negotiatedDetails->mEncodings.emplace_back(new JsepTrackEncoding);
    }

    auto& encoding = negotiatedDetails->mEncodings[i];

    for (const auto& codec : negotiatedCodecs) {
      if (rids[i].HasFormat(codec->mDefaultPt)) {
        encoding->AddCodec(*codec);
      }
    }

    encoding->mRid = rids[i].id;
    // If we end up supporting params for rid, we would handle that here.

    // Incorporate the corresponding JS encoding constraints, if they exist
    for (const JsConstraints& jsConstraints : mJsEncodeConstraints) {
      if (jsConstraints.rid == rids[i].id) {
        encoding->mConstraints = jsConstraints.constraints;
      }
    }
  }
}

std::vector<UniquePtr<JsepCodecDescription>> JsepTrack::GetCodecClones() const {
  std::vector<UniquePtr<JsepCodecDescription>> clones;
  for (const auto& codec : mPrototypeCodecs) {
    clones.emplace_back(codec->Clone());
  }
  return clones;
}

static bool CompareCodec(const UniquePtr<JsepCodecDescription>& lhs,
                         const UniquePtr<JsepCodecDescription>& rhs) {
  return lhs->mStronglyPreferred && !rhs->mStronglyPreferred;
}

<<<<<<< HEAD
void JsepTrack::NegotiateCodecs(
    const SdpMediaSection& remote,
    std::vector<UniquePtr<JsepCodecDescription>>* codecs,
    std::map<std::string, std::string>* formatChanges) const {
  MOZ_ASSERT(codecs->size());
  std::vector<UniquePtr<JsepCodecDescription>> unnegotiatedCodecs;
  std::swap(unnegotiatedCodecs, *codecs);
||||||| merged common ancestors
void
JsepTrack::NegotiateCodecs(
    const SdpMediaSection& remote,
    std::vector<JsepCodecDescription*>* codecs,
    std::map<std::string, std::string>* formatChanges) const
{
  MOZ_ASSERT(codecs->size());
  PtrVector<JsepCodecDescription> unnegotiatedCodecs;
  std::swap(unnegotiatedCodecs.values, *codecs);
=======
std::vector<UniquePtr<JsepCodecDescription>> JsepTrack::NegotiateCodecs(
    const SdpMediaSection& remote, bool isOffer) {
  std::vector<UniquePtr<JsepCodecDescription>> negotiatedCodecs;
  std::vector<UniquePtr<JsepCodecDescription>> newPrototypeCodecs;
>>>>>>> upstream-releases

  // Outer loop establishes the remote side's preference
  for (const std::string& fmt : remote.GetFormats()) {
<<<<<<< HEAD
    for (auto& codec : unnegotiatedCodecs) {
||||||| merged common ancestors
    for (size_t i = 0; i < unnegotiatedCodecs.values.size(); ++i) {
      JsepCodecDescription* codec = unnegotiatedCodecs.values[i];
=======
    for (auto& codec : mPrototypeCodecs) {
>>>>>>> upstream-releases
      if (!codec || !codec->mEnabled || !codec->Matches(fmt, remote)) {
        continue;
      }

<<<<<<< HEAD
      std::string originalFormat = codec->mDefaultPt;
      if (codec->Negotiate(fmt, remote)) {
        if (formatChanges) {
          (*formatChanges)[originalFormat] = codec->mDefaultPt;
        }
        codecs->push_back(std::move(codec));
||||||| merged common ancestors
      std::string originalFormat = codec->mDefaultPt;
      if(codec->Negotiate(fmt, remote)) {
        codecs->push_back(codec);
        unnegotiatedCodecs.values[i] = nullptr;
        if (formatChanges) {
          (*formatChanges)[originalFormat] = codec->mDefaultPt;
        }
=======
      // First codec of ours that matches. See if we can negotiate it.
      UniquePtr<JsepCodecDescription> clone(codec->Clone());
      if (clone->Negotiate(fmt, remote, isOffer)) {
        // If negotiation changed the payload type, remember that for later
        codec->mDefaultPt = clone->mDefaultPt;
        // Moves the codec out of mPrototypeCodecs, leaving an empty
        // UniquePtr, so we don't use it again. Also causes successfully
        // negotiated codecs to be placed up front in the future.
        newPrototypeCodecs.emplace_back(std::move(codec));
        negotiatedCodecs.emplace_back(std::move(clone));
>>>>>>> upstream-releases
        break;
      }
    }
  }

  // newPrototypeCodecs contains just the negotiated stuff so far. Add the rest.
  for (auto& codec : mPrototypeCodecs) {
    if (codec) {
      newPrototypeCodecs.emplace_back(std::move(codec));
    }
  }

  // Negotiated stuff is up front, so it will take precedence when ensuring
  // there are no duplicate payload types.
  EnsureNoDuplicatePayloadTypes(&newPrototypeCodecs);
  std::swap(newPrototypeCodecs, mPrototypeCodecs);

  // Find the (potential) red codec and ulpfec codec or telephone-event
  JsepVideoCodecDescription* red = nullptr;
  JsepVideoCodecDescription* ulpfec = nullptr;
  JsepAudioCodecDescription* dtmf = nullptr;
  // We can safely cast here since JsepTrack has a MediaType and only codecs
  // that match that MediaType (kAudio or kVideo) are added.
<<<<<<< HEAD
  for (auto& codec : *codecs) {
||||||| merged common ancestors
  for (auto codec : *codecs) {
=======
  for (auto& codec : negotiatedCodecs) {
>>>>>>> upstream-releases
    if (codec->mName == "red") {
      red = static_cast<JsepVideoCodecDescription*>(codec.get());
    } else if (codec->mName == "ulpfec") {
      ulpfec = static_cast<JsepVideoCodecDescription*>(codec.get());
    } else if (codec->mName == "telephone-event") {
      dtmf = static_cast<JsepAudioCodecDescription*>(codec.get());
    }
  }
  // if we have a red codec remove redundant encodings that don't exist
  if (red) {
    // Since we could have an externally specified redundant endcodings
    // list, we shouldn't simply rebuild the redundant encodings list
    // based on the current list of codecs.
    std::vector<uint8_t> unnegotiatedEncodings;
    std::swap(unnegotiatedEncodings, red->mRedundantEncodings);
    for (auto redundantPt : unnegotiatedEncodings) {
      std::string pt = std::to_string(redundantPt);
<<<<<<< HEAD
      for (const auto& codec : *codecs) {
||||||| merged common ancestors
      for (auto codec : *codecs) {
=======
      for (const auto& codec : negotiatedCodecs) {
>>>>>>> upstream-releases
        if (pt == codec->mDefaultPt) {
          red->mRedundantEncodings.push_back(redundantPt);
          break;
        }
      }
    }
  }
  // Video FEC is indicated by the existence of the red and ulpfec
  // codecs and not an attribute on the particular video codec (like in
  // a rtcpfb attr). If we see both red and ulpfec codecs, we enable FEC
  // on all the other codecs.
  if (red && ulpfec) {
<<<<<<< HEAD
    for (auto& codec : *codecs) {
||||||| merged common ancestors
    for (auto codec : *codecs) {
=======
    for (auto& codec : negotiatedCodecs) {
>>>>>>> upstream-releases
      if (codec->mName != "red" && codec->mName != "ulpfec") {
        JsepVideoCodecDescription* videoCodec =
            static_cast<JsepVideoCodecDescription*>(codec.get());
        videoCodec->EnableFec(red->mDefaultPt, ulpfec->mDefaultPt);
      }
    }
  }

  // Dtmf support is indicated by the existence of the telephone-event
  // codec, and not an attribute on the particular audio codec (like in a
  // rtcpfb attr). If we see the telephone-event codec, we enabled dtmf
  // support on all the other audio codecs.
  if (dtmf) {
<<<<<<< HEAD
    for (auto& codec : *codecs) {
||||||| merged common ancestors
    for (auto codec : *codecs) {
=======
    for (auto& codec : negotiatedCodecs) {
>>>>>>> upstream-releases
      JsepAudioCodecDescription* audioCodec =
          static_cast<JsepAudioCodecDescription*>(codec.get());
      audioCodec->mDtmfEnabled = true;
    }
  }

  // Make sure strongly preferred codecs are up front, overriding the remote
  // side's preference.
<<<<<<< HEAD
  std::stable_sort(codecs->begin(), codecs->end(), CompareCodec);

  // TODO(bug 814227): Remove this once we're ready to put multiple codecs in an
  // answer.  For now, remove all but the first codec unless the red codec
  // exists, in which case we include the others per RFC 5109, section 14.2.
  if (!codecs->empty() && !red) {
    std::vector<UniquePtr<JsepCodecDescription>> codecsToKeep;

    bool foundPreferredCodec = false;
    for (auto& codec : *codecs) {
      if (codec.get() == dtmf) {
        codecsToKeep.push_back(std::move(codec));
        // TODO: keep ulpfec when we enable it in Bug 875922
        // } else if (codec.get() == ulpfec) {
        //   codecsToKeep.push_back(std::move(codec));
      } else if (!foundPreferredCodec) {
        codecsToKeep.insert(codecsToKeep.begin(), std::move(codec));
        foundPreferredCodec = true;
      }
    }

    *codecs = std::move(codecsToKeep);
  }
||||||| merged common ancestors
  std::stable_sort(codecs->begin(), codecs->end(), CompareCodec);

  // TODO(bug 814227): Remove this once we're ready to put multiple codecs in an
  // answer.  For now, remove all but the first codec unless the red codec
  // exists, in which case we include the others per RFC 5109, section 14.2.
  if (!codecs->empty() && !red) {
    std::vector<JsepCodecDescription*> codecsToKeep;

    bool foundPreferredCodec = false;
    for (auto codec: *codecs) {
      if (codec == dtmf) {
        codecsToKeep.push_back(codec);
      // TODO: keep ulpfec when we enable it in Bug 875922
      // } else if (codec == ulpfec) {
      //   codecsToKeep.push_back(codec);
      } else if (!foundPreferredCodec) {
        codecsToKeep.insert(codecsToKeep.begin(), codec);
        foundPreferredCodec = true;
      } else {
        delete codec;
      }
    }

    *codecs = codecsToKeep;
  }
=======
  std::stable_sort(negotiatedCodecs.begin(), negotiatedCodecs.end(),
                   CompareCodec);

  if (!red) {
    // No red, remove ulpfec
    negotiatedCodecs.erase(
        std::remove_if(negotiatedCodecs.begin(), negotiatedCodecs.end(),
                       [ulpfec](const UniquePtr<JsepCodecDescription>& codec) {
                         return codec.get() == ulpfec;
                       }),
        negotiatedCodecs.end());
    // Make sure there's no dangling ptr here
    ulpfec = nullptr;
  }

  return negotiatedCodecs;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void JsepTrack::Negotiate(const SdpMediaSection& answer,
                          const SdpMediaSection& remote) {
  std::vector<UniquePtr<JsepCodecDescription>> negotiatedCodecs;
  negotiatedCodecs = GetCodecClones();

  std::map<std::string, std::string> formatChanges;
  NegotiateCodecs(remote, &negotiatedCodecs, &formatChanges);

  // Use |formatChanges| to update mPrototypeCodecs
  size_t insertPos = 0;
  for (auto& codec : mPrototypeCodecs) {
    if (formatChanges.count(codec->mDefaultPt)) {
      // Update the payload type to what was negotiated
      codec->mDefaultPt = formatChanges[codec->mDefaultPt];
      // Move this negotiated codec up front
      std::swap(mPrototypeCodecs[insertPos], codec);
      ++insertPos;
    }
  }

  EnsureNoDuplicatePayloadTypes(&mPrototypeCodecs);
||||||| merged common ancestors
void
JsepTrack::Negotiate(const SdpMediaSection& answer,
                     const SdpMediaSection& remote)
{
  PtrVector<JsepCodecDescription> negotiatedCodecs;
  negotiatedCodecs.values = GetCodecClones();

  std::map<std::string, std::string> formatChanges;
  NegotiateCodecs(remote,
                  &negotiatedCodecs.values,
                  &formatChanges);

  // Use |formatChanges| to update mPrototypeCodecs
  size_t insertPos = 0;
  for (size_t i = 0; i < mPrototypeCodecs.values.size(); ++i) {
    if (formatChanges.count(mPrototypeCodecs.values[i]->mDefaultPt)) {
      // Update the payload type to what was negotiated
      mPrototypeCodecs.values[i]->mDefaultPt =
        formatChanges[mPrototypeCodecs.values[i]->mDefaultPt];
      // Move this negotiated codec up front
      std::swap(mPrototypeCodecs.values[insertPos],
                mPrototypeCodecs.values[i]);
      ++insertPos;
    }
  }

  EnsureNoDuplicatePayloadTypes(&mPrototypeCodecs.values);
=======
void JsepTrack::Negotiate(const SdpMediaSection& answer,
                          const SdpMediaSection& remote) {
  std::vector<UniquePtr<JsepCodecDescription>> negotiatedCodecs =
      NegotiateCodecs(remote, &answer != &remote);
>>>>>>> upstream-releases

  UniquePtr<JsepTrackNegotiatedDetails> negotiatedDetails =
      MakeUnique<JsepTrackNegotiatedDetails>();

  CreateEncodings(remote, negotiatedCodecs, negotiatedDetails.get());

  if (answer.GetAttributeList().HasAttribute(SdpAttribute::kExtmapAttribute)) {
    for (auto& extmapAttr : answer.GetAttributeList().GetExtmap().mExtmaps) {
      SdpDirectionAttribute::Direction direction = extmapAttr.direction;
      if (&remote == &answer) {
        // Answer is remote, we need to flip this.
        direction = reverse(direction);
      }

      if (direction & mDirection) {
        negotiatedDetails->mExtmap[extmapAttr.extensionname] = extmapAttr;
      }
    }
  }

  mNegotiatedDetails = std::move(negotiatedDetails);
}

// When doing bundle, if all else fails we can try to figure out which m-line a
// given RTP packet belongs to by looking at the payload type field. This only
// works, however, if that payload type appeared in only one m-section.
// We figure that out here.
/* static */
void JsepTrack::SetUniquePayloadTypes(std::vector<JsepTrack*>& tracks) {
  // Maps to track details if no other track contains the payload type,
  // otherwise maps to nullptr.
  std::map<uint16_t, JsepTrackNegotiatedDetails*> payloadTypeToDetailsMap;

  for (JsepTrack* track : tracks) {
    if (track->GetMediaType() == SdpMediaSection::kApplication) {
      continue;
    }

    auto* details = track->GetNegotiatedDetails();
    if (!details) {
      // Can happen if negotiation fails on a track
      continue;
    }

    std::vector<uint16_t> payloadTypesForTrack;
    track->GetNegotiatedPayloadTypes(&payloadTypesForTrack);

    for (uint16_t pt : payloadTypesForTrack) {
      if (payloadTypeToDetailsMap.count(pt)) {
        // Found in more than one track, not unique
        payloadTypeToDetailsMap[pt] = nullptr;
      } else {
        payloadTypeToDetailsMap[pt] = details;
      }
    }
  }

  for (auto ptAndDetails : payloadTypeToDetailsMap) {
    uint16_t uniquePt = ptAndDetails.first;
    MOZ_ASSERT(uniquePt <= UINT8_MAX);
    auto trackDetails = ptAndDetails.second;

    if (trackDetails) {
      trackDetails->mUniquePayloadTypes.push_back(
          static_cast<uint8_t>(uniquePt));
    }
  }
}

}  // namespace mozilla
