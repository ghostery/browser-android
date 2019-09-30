/* -*- Mode: IDL; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * The origin of this IDL file is
 * https://webaudio.github.io/web-audio-api/
 *
 * Copyright © 2012 W3C® (MIT, ERCIM, Keio), All Rights Reserved. W3C
 * liability, trademark and document use rules apply.
 */

<<<<<<< HEAD:mozilla-release/dom/webidl/DOMError.webidl
[Constructor(DOMString name, optional DOMString message = ""),
 Exposed=(Window,Worker)]
interface DOMError {
  [Constant, UseCounter]
  readonly attribute DOMString name;
||||||| merged common ancestors
[Constructor(DOMString name, optional DOMString message = ""),
 Exposed=(Window,Worker,System)]
interface DOMError {
  [Constant, UseCounter]
  readonly attribute DOMString name;
=======
dictionary MediaStreamTrackAudioSourceOptions {
    required MediaStreamTrack mediaStreamTrack;
};

[Pref="dom.webaudio.enabled",
 Constructor(AudioContext context, MediaStreamTrackAudioSourceOptions options)]
interface MediaStreamTrackAudioSourceNode : AudioNode {
>>>>>>> upstream-releases:mozilla-release/dom/webidl/MediaStreamTrackAudioSourceNode.webidl

};

// Mozilla extensions
MediaStreamTrackAudioSourceNode implements AudioNodePassThrough;
