/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */

/* @flow */

import { BinaryReader } from "wasmparser/dist/WasmParser";
import { WasmDisassembler, NameSectionReader } from "wasmparser/dist/WasmDis";

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/wasm.js
import type { WasmSource } from "../types";
type WasmState = {
  lines: Array<number>,
  offsets: Array<number>
};
||||||| merged common ancestors
var _WasmDis = require("devtools/client/shared/vendor/WasmDis");
=======
import type { SourceId, WasmSourceContent } from "../types";
type WasmState = {
  lines: Array<number>,
  offsets: Array<number>,
};
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/wasm.js

var wasmStates: { [string]: WasmState } = (Object.create(null): any);

function maybeWasmSectionNameResolver(data: Uint8Array) {
  try {
    const parser = new BinaryReader();
    parser.setData(data.buffer, 0, data.length);
    const reader = new NameSectionReader();
    reader.read(parser);
    return reader.hasValidNames() ? reader.getNameResolver() : null;
  } catch (ex) {
    // Ignoring any errors during names section retrival.
    return null;
  }
}

/**
 * @memberof utils/wasm
 * @static
 */
export function getWasmText(sourceId: string, data: Uint8Array) {
  const nameResolver = maybeWasmSectionNameResolver(data);
  const parser = new BinaryReader();
  parser.setData(data.buffer, 0, data.length);
  const dis = new WasmDisassembler();
  if (nameResolver) {
    dis.nameResolver = nameResolver;
  }
  dis.addOffsets = true;
  const done = dis.disassembleChunk(parser);
  let result = dis.getResult();
  if (result.lines.length === 0) {
    result = { lines: ["No luck with wast conversion"], offsets: [0], done };
  }

  const offsets = result.offsets;
  const lines = [];
  for (let i = 0; i < offsets.length; i++) {
    lines[offsets[i]] = i;
  }

  wasmStates[sourceId] = { offsets, lines };

  return { lines: result.lines, done: result.done };
}

/**
 * @memberof utils/wasm
 * @static
 */
export function getWasmLineNumberFormatter(sourceId: string) {
  const codeOf0 = 48,
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/wasm.js
    codeOfA = 65;
  const buffer = [
    codeOf0,
    codeOf0,
    codeOf0,
    codeOf0,
    codeOf0,
    codeOf0,
    codeOf0,
    codeOf0
  ];
||||||| merged common ancestors
        codeOfA = 65;
  const buffer = [codeOf0, codeOf0, codeOf0, codeOf0, codeOf0, codeOf0, codeOf0, codeOf0];
=======
    codeOfA = 65;
  const buffer = [
    codeOf0,
    codeOf0,
    codeOf0,
    codeOf0,
    codeOf0,
    codeOf0,
    codeOf0,
    codeOf0,
  ];
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/wasm.js
  let last0 = 7;
  return function(number: number) {
    const offset = lineToWasmOffset(sourceId, number - 1);
    if (offset == undefined) {
      return "";
    }
    let i = 7;
    for (let n = offset; n !== 0 && i >= 0; n >>= 4, i--) {
      const nibble = n & 15;
      buffer[i] = nibble < 10 ? codeOf0 + nibble : codeOfA - 10 + nibble;
    }
    for (let j = i; j > last0; j--) {
      buffer[j] = codeOf0;
    }
    last0 = i;
    return String.fromCharCode.apply(null, buffer);
  };
}

/**
 * @memberof utils/wasm
 * @static
 */
export function isWasm(sourceId: string) {
  return sourceId in wasmStates;
}

/**
 * @memberof utils/wasm
 * @static
 */
export function lineToWasmOffset(sourceId: string, number: number): ?number {
  const wasmState = wasmStates[sourceId];
  if (!wasmState) {
    return undefined;
  }
  let offset = wasmState.offsets[number];
  while (offset === undefined && number > 0) {
    offset = wasmState.offsets[--number];
  }
  return offset;
}

/**
 * @memberof utils/wasm
 * @static
 */
export function wasmOffsetToLine(sourceId: string, offset: number): ?number {
  const wasmState = wasmStates[sourceId];
  if (!wasmState) {
    return undefined;
  }
  return wasmState.lines[offset];
}

/**
 * @memberof utils/wasm
 * @static
 */
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/wasm.js
export function clearWasmStates() {
  wasmStates = (Object.create(null): any);
}

const wasmLines: WeakMap<WasmSource, string[]> = new WeakMap();
export function renderWasmText(source: WasmSource): string[] {
  if (wasmLines.has(source)) {
    return wasmLines.get(source) || [];
  }

  if (!source.text) {
    return [];
  }
||||||| merged common ancestors


function clearWasmStates() {
  wasmStates = Object.create(null);
}
=======
export function clearWasmStates() {
  wasmStates = (Object.create(null): any);
}
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/wasm.js

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/wasm.js
||||||| merged common ancestors
function renderWasmText(sourceId, {
  binary
}) {
=======
const wasmLines: WeakMap<WasmSourceContent, string[]> = new WeakMap();
export function renderWasmText(
  sourceId: SourceId,
  content: WasmSourceContent
): string[] {
  if (wasmLines.has(content)) {
    return wasmLines.get(content) || [];
  }

>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/wasm.js
  // binary does not survive as Uint8Array, converting from string
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/wasm.js
  const { binary } = source.text;
||||||| merged common ancestors
=======
  const { binary } = content.value;
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/wasm.js
  const data = new Uint8Array(binary.length);
  for (let i = 0; i < data.length; i++) {
    data[i] = binary.charCodeAt(i);
  }
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/wasm.js
  const { lines } = getWasmText(source.id, data);
||||||| merged common ancestors

  const {
    lines
  } = getWasmText(sourceId, data);
=======
  const { lines } = getWasmText(sourceId, data);
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/wasm.js
  const MAX_LINES = 1000000;
  if (lines.length > MAX_LINES) {
    lines.splice(MAX_LINES, lines.length - MAX_LINES);
    lines.push(";; .... text is truncated due to the size");
  }

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/wasm.js
  wasmLines.set(source, lines);
||||||| merged common ancestors
=======
  wasmLines.set(content, lines);
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/wasm.js
  return lines;
}
