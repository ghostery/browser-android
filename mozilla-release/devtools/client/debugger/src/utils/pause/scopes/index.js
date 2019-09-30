/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/pause/scopes/index.js

// @flow

import { getScope, type RenderableScope } from "./getScope";

import type { Frame, Why, BindingContents } from "../../../types";

export type NamedValue = {
  name: string,
  generatedName?: string,
  path: string,
  contents: BindingContents | NamedValue[]
};

export function getScopes(
  why: Why,
  selectedFrame: Frame,
  frameScopes: ?RenderableScope
): ?(NamedValue[]) {
||||||| merged common ancestors
function getScopes(why, selectedFrame, frameScopes) {
=======

// @flow

import { getScope, type RenderableScope } from "./getScope";

import type { Frame, Why, BindingContents } from "../../../types";

export type NamedValue = {
  name: string,
  generatedName?: string,
  path: string,
  contents: BindingContents | NamedValue[],
};

export function getScopes(
  why: Why,
  selectedFrame: Frame,
  frameScopes: ?RenderableScope
): ?(NamedValue[]) {
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/pause/scopes/index.js
  if (!why || !selectedFrame) {
    return null;
  }

  if (!frameScopes) {
    return null;
  }

  const scopes = [];

  let scope = frameScopes;
  let scopeIndex = 1;

  while (scope) {
    const scopeItem = getScope(
      scope,
      selectedFrame,
      frameScopes,
      why,
      scopeIndex
    );

    if (scopeItem) {
      scopes.push(scopeItem);
    }
    scopeIndex++;
    scope = scope.parent;
  }

  return scopes;
}
