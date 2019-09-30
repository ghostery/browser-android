<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/selectors/breakpointAtLocation.js
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */

import { getSelectedSource } from "../reducers/sources";
import { getBreakpointsList } from "../reducers/breakpoints";
import { isGenerated } from "../utils/source";

||||||| merged common ancestors
"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports.getBreakpointAtLocation = getBreakpointAtLocation;
exports.getBreakpointsAtLine = getBreakpointsAtLine;

var _sources = require("../reducers/sources");

var _breakpoints = require("../reducers/breakpoints");

var _source = require("../utils/source");

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */
=======
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */

// @flow

import { getSelectedSource } from "../reducers/sources";
import { getBreakpointsList } from "../reducers/breakpoints";
import { isGenerated } from "../utils/source";

import type { Breakpoint } from "../types";
import type { State } from "../reducers/types";

>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/selectors/breakpointAtLocation.js
function getColumn(column, selectedSource) {
  if (column) {
    return column;
  }

  return isGenerated(selectedSource) ? undefined : 0;
}

function getLocation(bp, selectedSource) {
  return isGenerated(selectedSource)
    ? bp.generatedLocation || bp.location
    : bp.location;
}

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/selectors/breakpointAtLocation.js
function getBreakpointsForSource(
  state: OuterState,
  selectedSource: Source
): Breakpoint[] {
  const breakpoints = getBreakpointsList(state);

||||||| merged common ancestors
function getBreakpointsForSource(state, selectedSource) {
  const breakpoints = (0, _breakpoints.getBreakpoints)(state);
=======
function getBreakpointsForSource(state: State, selectedSource): Breakpoint[] {
  const breakpoints = getBreakpointsList(state);

>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/selectors/breakpointAtLocation.js
  return breakpoints.filter(bp => {
    const location = getLocation(bp, selectedSource);
    return location.sourceId === selectedSource.id;
  });
}

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/selectors/breakpointAtLocation.js
function findBreakpointAtLocation(
  breakpoints,
  selectedSource,
  { line, column }
) {
||||||| merged common ancestors
function findBreakpointAtLocation(breakpoints, selectedSource, {
  line,
  column
}) {
=======
type LineColumn = { line: number, column: ?number };

function findBreakpointAtLocation(
  breakpoints,
  selectedSource,
  { line, column }: LineColumn
) {
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/selectors/breakpointAtLocation.js
  return breakpoints.find(breakpoint => {
    const location = getLocation(breakpoint, selectedSource);
    const sameLine = location.line === line;
    if (!sameLine) {
      return false;
    }

    if (column === undefined) {
      return true;
    }

    return location.column === getColumn(column, selectedSource);
  });
}

/*
 * Finds a breakpoint at a location (line, column) of the
 * selected source.
 *
 * This is useful for finding a breakpoint when the
 * user clicks in the gutter or on a token.
 */
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/selectors/breakpointAtLocation.js
export function getBreakpointAtLocation(state, location) {
  const selectedSource = getSelectedSource(state);
||||||| merged common ancestors


function getBreakpointAtLocation(state, location) {
  const selectedSource = (0, _sources.getSelectedSource)(state);
=======
export function getBreakpointAtLocation(state: State, location: LineColumn) {
  const selectedSource = getSelectedSource(state);
  if (!selectedSource) {
    throw new Error("no selectedSource");
  }
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/selectors/breakpointAtLocation.js
  const breakpoints = getBreakpointsForSource(state, selectedSource);

  return findBreakpointAtLocation(breakpoints, selectedSource, location);
}

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/selectors/breakpointAtLocation.js
export function getBreakpointsAtLine(state: OuterState, line: number) {
  const selectedSource = getSelectedSource(state);
||||||| merged common ancestors
function getBreakpointsAtLine(state, line) {
  const selectedSource = (0, _sources.getSelectedSource)(state);
=======
export function getBreakpointsAtLine(state: State, line: number): Breakpoint[] {
  const selectedSource = getSelectedSource(state);
  if (!selectedSource) {
    throw new Error("no selectedSource");
  }
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/selectors/breakpointAtLocation.js
  const breakpoints = getBreakpointsForSource(state, selectedSource);

  return breakpoints.filter(
    breakpoint => getLocation(breakpoint, selectedSource).line === line
  );
}
