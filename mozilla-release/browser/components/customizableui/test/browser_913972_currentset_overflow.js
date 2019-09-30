/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

var navbar = document.getElementById(CustomizableUI.AREA_NAVBAR);

registerCleanupFunction(async function asyncCleanup() {
  await resetCustomization();
});

// Resize to a small window, resize back, shouldn't affect default state.
add_task(async function() {
  let originalWindowWidth = window.outerWidth;
<<<<<<< HEAD
  ok(!navbar.hasAttribute("overflowing"), "Should start with a non-overflowing toolbar.");
||||||| merged common ancestors
  let oldCurrentSet = navbar.currentSet;
  ok(!navbar.hasAttribute("overflowing"), "Should start with a non-overflowing toolbar.");
=======
  ok(
    !navbar.hasAttribute("overflowing"),
    "Should start with a non-overflowing toolbar."
  );
>>>>>>> upstream-releases
  ok(CustomizableUI.inDefaultState, "Should start in default state.");
  let navbarTarget = CustomizableUI.getCustomizationTarget(navbar);
  let oldChildCount = navbarTarget.childElementCount;
  window.resizeTo(kForceOverflowWidthPx, window.outerHeight);
  await TestUtils.waitForCondition(
    () => navbar.hasAttribute("overflowing"),
    "Navbar has a overflowing attribute"
  );
  ok(navbar.hasAttribute("overflowing"), "Should have an overflowing toolbar.");
<<<<<<< HEAD
  ok(CustomizableUI.inDefaultState, "Should still be in default state when overflowing.");
  ok(navbarTarget.childElementCount < oldChildCount, "Should have fewer children.");
||||||| merged common ancestors
  is(navbar.currentSet, oldCurrentSet, "Currentset should be the same when overflowing.");
  ok(CustomizableUI.inDefaultState, "Should still be in default state when overflowing.");
  ok(navbar.customizationTarget.childElementCount < oldChildCount, "Should have fewer children.");
=======
  ok(
    CustomizableUI.inDefaultState,
    "Should still be in default state when overflowing."
  );
  ok(
    navbarTarget.childElementCount < oldChildCount,
    "Should have fewer children."
  );
>>>>>>> upstream-releases
  window.resizeTo(originalWindowWidth, window.outerHeight);
<<<<<<< HEAD
  await waitForCondition(() => !navbar.hasAttribute("overflowing"));
  ok(!navbar.hasAttribute("overflowing"), "Should no longer have an overflowing toolbar.");
  ok(CustomizableUI.inDefaultState, "Should still be in default state now we're no longer overflowing.");
||||||| merged common ancestors
  await waitForCondition(() => !navbar.hasAttribute("overflowing"));
  ok(!navbar.hasAttribute("overflowing"), "Should no longer have an overflowing toolbar.");
  is(navbar.currentSet, oldCurrentSet, "Currentset should still be the same now we're no longer overflowing.");
  ok(CustomizableUI.inDefaultState, "Should still be in default state now we're no longer overflowing.");
=======
  await TestUtils.waitForCondition(
    () => !navbar.hasAttribute("overflowing"),
    "Navbar does not have an overflowing attribute"
  );
  ok(
    !navbar.hasAttribute("overflowing"),
    "Should no longer have an overflowing toolbar."
  );
  ok(
    CustomizableUI.inDefaultState,
    "Should still be in default state now we're no longer overflowing."
  );
>>>>>>> upstream-releases

  // Verify actual physical placements match those of the placement array:
  let placementCounter = 0;
<<<<<<< HEAD
  let placements = CustomizableUI.getWidgetIdsInArea(CustomizableUI.AREA_NAVBAR);
  for (let node of navbarTarget.children) {
||||||| merged common ancestors
  let placements = CustomizableUI.getWidgetIdsInArea(CustomizableUI.AREA_NAVBAR);
  for (let node of navbar.customizationTarget.children) {
=======
  let placements = CustomizableUI.getWidgetIdsInArea(
    CustomizableUI.AREA_NAVBAR
  );
  for (let node of navbarTarget.children) {
>>>>>>> upstream-releases
    if (node.getAttribute("skipintoolbarset") == "true") {
      continue;
    }
    is(
      placements[placementCounter++],
      node.id,
      "Nodes should match after overflow"
    );
  }
<<<<<<< HEAD
  is(placements.length, placementCounter, "Should have as many nodes as expected");
  is(navbarTarget.childElementCount, oldChildCount, "Number of nodes should match");
||||||| merged common ancestors
  is(placements.length, placementCounter, "Should have as many nodes as expected");
  is(navbar.customizationTarget.childElementCount, oldChildCount, "Number of nodes should match");
=======
  is(
    placements.length,
    placementCounter,
    "Should have as many nodes as expected"
  );
  is(
    navbarTarget.childElementCount,
    oldChildCount,
    "Number of nodes should match"
  );
>>>>>>> upstream-releases
});

// Enter and exit customization mode, check that default state is correct.
add_task(async function() {
  ok(CustomizableUI.inDefaultState, "Should start in default state.");
  await startCustomizing();
<<<<<<< HEAD
  ok(CustomizableUI.inDefaultState, "Should be in default state in customization mode.");
||||||| merged common ancestors
  ok(CustomizableUI.inDefaultState, "Should be in default state in customization mode.");
  is(navbar.currentSet, oldCurrentSet, "Currentset should be the same in customization mode.");
=======
  ok(
    CustomizableUI.inDefaultState,
    "Should be in default state in customization mode."
  );
>>>>>>> upstream-releases
  await endCustomizing();
<<<<<<< HEAD
  ok(CustomizableUI.inDefaultState, "Should be in default state after customization mode.");
});

||||||| merged common ancestors
  ok(CustomizableUI.inDefaultState, "Should be in default state after customization mode.");
  is(navbar.currentSet, oldCurrentSet, "Currentset should be the same after customization mode.");
});

add_task(async function asyncCleanup() {
  await resetCustomization();
});
=======
  ok(
    CustomizableUI.inDefaultState,
    "Should be in default state after customization mode."
  );
});
>>>>>>> upstream-releases
