/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */

// @flow

import { get, findIndex } from "lodash";

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/pause/frames/collapseFrames.js
// eslint-disable-next-line max-len
import type { LocalFrame } from "../../../components/SecondaryPanes/Frames/types";
import { getFrameUrl } from "./getFrameUrl";
||||||| merged common ancestors
var _getFrameUrl = require("./getFrameUrl");
=======
// eslint-disable-next-line max-len
import type { Frame } from "../../../types";
import { getFrameUrl } from "./getFrameUrl";
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/pause/frames/collapseFrames.js

function collapseLastFrames(frames) {
  const index = findIndex(frames, frame =>
    getFrameUrl(frame).match(/webpack\/bootstrap/i)
  );

  if (index == -1) {
    return { newFrames: frames, lastGroup: [] };
  }

  const newFrames = frames.slice(0, index);
  const lastGroup = frames.slice(index);
  return { newFrames, lastGroup };
}

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/pause/frames/collapseFrames.js
type FrameGroup = LocalFrame[];
type GroupedFrames = Array<FrameGroup | LocalFrame>;

export function collapseFrames(frames: LocalFrame[]): GroupedFrames {
||||||| merged common ancestors
function collapseFrames(frames) {
=======
type FrameGroup = Frame[];
type GroupedFrames = Array<FrameGroup | Frame>;

export function collapseFrames(frames: Frame[]): GroupedFrames {
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/pause/frames/collapseFrames.js
  // We collapse groups of one so that user frames
  // are not in a group of one
  function addGroupToList(group, list) {
    if (!group) {
      return list;
    }

    if (group.length > 1) {
      list.push(group);
    } else {
      list = list.concat(group);
    }

    return list;
  }
  const { newFrames, lastGroup } = collapseLastFrames(frames);
  frames = newFrames;
  let items = [];
  let currentGroup = null;
  let prevItem = null;
  for (const frame of frames) {
    const prevLibrary = get(prevItem, "library");

    if (!currentGroup) {
      currentGroup = [frame];
    } else if (prevLibrary && prevLibrary == frame.library) {
      currentGroup.push(frame);
    } else {
      items = addGroupToList(currentGroup, items);
      currentGroup = [frame];
    }

    prevItem = frame;
  }

  items = addGroupToList(currentGroup, items);
  items = addGroupToList(lastGroup, items);
  return items;
}
