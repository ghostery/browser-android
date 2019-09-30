/* vim: set ft=javascript ts=2 et sw=2 tw=80: */
/* Any copyright is dedicated to the Public Domain.
 http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

// Test cycling color types in the rule view.

const TEST_URI = `
  <style type="text/css">
    body {
      color: #f00;
    }
    span {
      color: blue;
      border-color: #ff000080;
    }
    div {
      color: green;
    }
  </style>
  <body>
    <span>Test</span>
    <div>cycling color types in the rule view!</div>
  </body>
`;

add_task(async function() {
  await addTab("data:text/html;charset=utf-8," + encodeURIComponent(TEST_URI));
  const { inspector, view } = await openRuleView();

  await checkColorCycling(view);
  await checkAlphaColorCycling(inspector, view);
<<<<<<< HEAD
  await checkColorCyclingPersist(inspector, view);
  await checkColorCyclingWithDifferentDefaultType(inspector, view);
||||||| merged common ancestors
  await checkColorCyclingPersist(inspector, view);
=======
  await checkColorCyclingWithDifferentDefaultType(inspector, view);
>>>>>>> upstream-releases
});

<<<<<<< HEAD
async function checkColorCycling(view) {
  const { valueSpan } = getRuleViewProperty(view, "body", "color");

  checkColorValue(valueSpan, "#f00", "Color displayed as a hex value, its authored type");

  await runSwatchShiftClickTests(view, valueSpan, [{
    value: "hsl(0, 100%, 50%)",
    comment: "Color displayed as an HSL value",
  }, {
    value: "rgb(255, 0, 0)",
    comment: "Color displayed as an RGB value",
  }, {
    value: "red",
    comment: "Color displayed as a color name",
  }, {
    value: "#f00",
    comment: "Color displayed as an authored value",
  }, {
    value: "hsl(0, 100%, 50%)",
    comment: "Color displayed as an HSL value again",
  }]);
||||||| merged common ancestors
async function checkColorCycling(container, view) {
  const valueNode = container.querySelector(".ruleview-color");
  const win = view.styleWindow;

  // Hex
  is(valueNode.textContent, "#f00", "Color displayed as a hex value.");

  const tests = [{
    value: "hsl(0, 100%, 50%)",
    comment: "Color displayed as an HSL value."
  }, {
    value: "rgb(255, 0, 0)",
    comment: "Color displayed as an RGB value."
  }, {
    value: "red",
    comment: "Color displayed as a color name."
  }, {
    value: "#f00",
    comment: "Color displayed as an authored value."
  }, {
    value: "hsl(0, 100%, 50%)",
    comment: "Color displayed as an HSL value again."
  }];

  for (const test of tests) {
    await checkSwatchShiftClick(container, win, test.value, test.comment);
  }
=======
async function checkColorCycling(view) {
  const { valueSpan } = getRuleViewProperty(view, "body", "color");

  checkColorValue(
    valueSpan,
    "#f00",
    "Color displayed as a hex value, its authored type"
  );

  await runSwatchShiftClickTests(view, valueSpan, [
    {
      value: "hsl(0, 100%, 50%)",
      comment: "Color displayed as an HSL value",
    },
    {
      value: "rgb(255, 0, 0)",
      comment: "Color displayed as an RGB value",
    },
    {
      value: "red",
      comment: "Color displayed as a color name",
    },
    {
      value: "#f00",
      comment: "Color displayed as an authored value",
    },
    {
      value: "hsl(0, 100%, 50%)",
      comment: "Color displayed as an HSL value again",
    },
  ]);
>>>>>>> upstream-releases
}

async function checkAlphaColorCycling(inspector, view) {
  await selectNode("span", inspector);
<<<<<<< HEAD
  const { valueSpan } = getRuleViewProperty(view, "span", "border-color");

  checkColorValue(valueSpan, "#ff000080",
    "Color displayed as an alpha hex value, its authored type");

  await runSwatchShiftClickTests(view, valueSpan, [{
    value: "hsla(0, 100%, 50%, 0.5)",
    comment: "Color displayed as an HSLa value",
  }, {
    value: "rgba(255, 0, 0, 0.5)",
    comment: "Color displayed as an RGBa value",
  }, {
    value: "#ff000080",
    comment: "Color displayed as an alpha hex value again",
  }]);
||||||| merged common ancestors
  const container = getRuleViewProperty(view, "span", "border-color").valueSpan;
  const valueNode = container.querySelector(".ruleview-color");
  const win = view.styleWindow;

  is(valueNode.textContent, "#ff000080",
    "Color displayed as an alpha hex value.");

  const tests = [{
    value: "hsla(0, 100%, 50%, 0.5)",
    comment: "Color displayed as an HSLa value."
  }, {
    value: "rgba(255, 0, 0, 0.5)",
    comment: "Color displayed as an RGBa value."
  }, {
    value: "#ff000080",
    comment: "Color displayed as an alpha hex value again."
  }];

  for (const test of tests) {
    await checkSwatchShiftClick(container, win, test.value, test.comment);
  }
=======
  const { valueSpan } = getRuleViewProperty(view, "span", "border-color");

  checkColorValue(
    valueSpan,
    "#ff000080",
    "Color displayed as an alpha hex value, its authored type"
  );

  await runSwatchShiftClickTests(view, valueSpan, [
    {
      value: "hsla(0, 100%, 50%, 0.5)",
      comment: "Color displayed as an HSLa value",
    },
    {
      value: "rgba(255, 0, 0, 0.5)",
      comment: "Color displayed as an RGBa value",
    },
    {
      value: "#ff000080",
      comment: "Color displayed as an alpha hex value again",
    },
  ]);
}

async function checkColorCyclingWithDifferentDefaultType(inspector, view) {
  info("Change the default color type pref to hex");
  await pushPref("devtools.defaultColorUnit", "hex");

  info(
    "Select a new node that would normally have a color with a different type"
  );
  await selectNode("div", inspector);
  const { valueSpan } = getRuleViewProperty(view, "div", "color");

  checkColorValue(
    valueSpan,
    "#008000",
    "Color displayed as a hex value, which is the type just selected"
  );

  info("Cycle through color types again");
  await runSwatchShiftClickTests(view, valueSpan, [
    {
      value: "hsl(120, 100%, 25.1%)",
      comment: "Color displayed as an HSL value",
    },
    {
      value: "rgb(0, 128, 0)",
      comment: "Color displayed as an RGB value",
    },
    {
      value: "green",
      comment: "Color displayed as a color name",
    },
    {
      value: "#008000",
      comment: "Color displayed as an authored value",
    },
    {
      value: "hsl(120, 100%, 25.1%)",
      comment: "Color displayed as an HSL value again",
    },
  ]);
}

async function runSwatchShiftClickTests(view, valueSpan, tests) {
  for (const { value, comment } of tests) {
    await checkSwatchShiftClick(view, valueSpan, value, comment);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
async function checkColorCyclingPersist(inspector, view) {
  await selectNode("span", inspector);
  let { valueSpan } = getRuleViewProperty(view, "span", "color");
||||||| merged common ancestors
async function checkColorCyclingPersist(inspector, view) {
  await selectNode("span", inspector);
  let container = getRuleViewProperty(view, "span", "color").valueSpan;
  let valueNode = container.querySelector(".ruleview-color");
  const win = view.styleWindow;
=======
async function checkSwatchShiftClick(view, valueSpan, expectedValue, comment) {
  const swatchNode = valueSpan.querySelector(".ruleview-colorswatch");
  const colorNode = valueSpan.querySelector(".ruleview-color");
>>>>>>> upstream-releases

<<<<<<< HEAD
  checkColorValue(valueSpan, "blue", "Color displayed as color name, its authored type");
||||||| merged common ancestors
  is(valueNode.textContent, "blue", "Color displayed as a color name.");
=======
  info(
    "Shift-click the color swatch and wait for the color type and ruleview to update"
  );
  const onUnitChange = swatchNode.once("unit-change");
>>>>>>> upstream-releases

<<<<<<< HEAD
  await checkSwatchShiftClick(view, valueSpan, "#00f", "Color displayed as a hex value");
||||||| merged common ancestors
  await checkSwatchShiftClick(container, win, "#00f",
    "Color displayed as a hex value.");
=======
  EventUtils.synthesizeMouseAtCenter(
    swatchNode,
    {
      type: "mousedown",
      shiftKey: true,
    },
    view.styleWindow
  );
>>>>>>> upstream-releases

<<<<<<< HEAD
  info("Select the body and reselect the span to see if the new color unit persisted");
  await selectNode("body", inspector);
  await selectNode("span", inspector);
||||||| merged common ancestors
  // Select the body and reselect the span to see
  // if the new color unit persisted
  await selectNode("body", inspector);
  await selectNode("span", inspector);
=======
  await onUnitChange;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // We have to query for the value span and the swatch again because they've been
  // re-generated.
  ({ valueSpan } = getRuleViewProperty(view, "span", "color"));
  checkColorValue(valueSpan, "#00f", "Color is still displayed as a hex value");
||||||| merged common ancestors
  // We have to query for the container and the swatch because
  // they've been re-generated
  container = getRuleViewProperty(view, "span", "color").valueSpan;
  valueNode = container.querySelector(".ruleview-color");
  is(valueNode.textContent, "#00f",
    "Color  is still displayed as a hex value.");
=======
  is(colorNode.textContent, expectedValue, comment);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
async function checkColorCyclingWithDifferentDefaultType(inspector, view) {
  info("Change the default color type pref to hex");
  await pushPref("devtools.defaultColorUnit", "hex");

  info("Select a new node that would normally have a color with a different type");
  await selectNode("div", inspector);
  const { valueSpan } = getRuleViewProperty(view, "div", "color");

  checkColorValue(valueSpan, "#008000",
    "Color displayed as a hex value, which is the type just selected");

  info("Cycle through color types again");
  await runSwatchShiftClickTests(view, valueSpan, [{
    value: "hsl(120, 100%, 25.1%)",
    comment: "Color displayed as an HSL value",
  }, {
    value: "rgb(0, 128, 0)",
    comment: "Color displayed as an RGB value",
  }, {
    value: "green",
    comment: "Color displayed as a color name",
  }, {
    value: "#008000",
    comment: "Color displayed as an authored value",
  }, {
    value: "hsl(120, 100%, 25.1%)",
    comment: "Color displayed as an HSL value again",
  }]);
}

async function runSwatchShiftClickTests(view, valueSpan, tests) {
  for (const { value, comment } of tests) {
    await checkSwatchShiftClick(view, valueSpan, value, comment);
  }
}

async function checkSwatchShiftClick(view, valueSpan, expectedValue, comment) {
  const swatchNode = valueSpan.querySelector(".ruleview-colorswatch");
  const colorNode = valueSpan.querySelector(".ruleview-color");

  info("Shift-click the color swatch and wait for the color type and ruleview to update");
  const onRuleViewChanged = view.once("ruleview-changed");
  const onUnitChange = swatchNode.once("unit-change");

  EventUtils.synthesizeMouseAtCenter(swatchNode, {
    type: "mousedown",
    shiftKey: true,
  }, view.styleWindow);

  await onUnitChange;
  await onRuleViewChanged;

  is(colorNode.textContent, expectedValue, comment);
}

function checkColorValue(valueSpan, expectedColorValue, comment) {
  const colorNode = valueSpan.querySelector(".ruleview-color");
  is(colorNode.textContent, expectedColorValue, comment);
||||||| merged common ancestors
async function checkSwatchShiftClick(container, win, expectedValue, comment) {
  const swatch = container.querySelector(".ruleview-colorswatch");
  const valueNode = container.querySelector(".ruleview-color");

  const onUnitChange = swatch.once("unit-change");
  EventUtils.synthesizeMouseAtCenter(swatch, {
    type: "mousedown",
    shiftKey: true
  }, win);
  await onUnitChange;
  is(valueNode.textContent, expectedValue, comment);
=======
function checkColorValue(valueSpan, expectedColorValue, comment) {
  const colorNode = valueSpan.querySelector(".ruleview-color");
  is(colorNode.textContent, expectedColorValue, comment);
>>>>>>> upstream-releases
}
