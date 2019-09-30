/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

add_task(async function test() {
  await SpecialPowers.pushPrefEnv({
    set: [["browser.ctrlTab.recentlyUsedOrder", true]],
  });

  let [origTab] = gBrowser.visibleTabs;
  let tabOne = BrowserTestUtils.addTab(gBrowser);
  let tabTwo = BrowserTestUtils.addTab(gBrowser);

  // test the ctrlTab.tabList
  pressCtrlTab();
  ok(ctrlTab.isOpen, "With 3 tab open, Ctrl+Tab opens the preview panel");
  is(ctrlTab.tabList.length, 3, "Ctrl+Tab panel displays all visible tabs");
  releaseCtrl();

  gBrowser.showOnlyTheseTabs([origTab]);
  pressCtrlTab();
<<<<<<< HEAD
  ok(!ctrlTab.isOpen, "With 1 tab open, Ctrl+Tab doesn't open the preview panel");
  releaseCtrl();
||||||| merged common ancestors
  ok(ctrlTab.tabList.length, 1, "Show 1 tab in tab preview");
  ok(!ctrlTab.isOpen, "With 1 tab open, Ctrl+Tab doesn't open the preview panel");
=======
  ok(
    !ctrlTab.isOpen,
    "With 1 tab open, Ctrl+Tab doesn't open the preview panel"
  );
  releaseCtrl();
>>>>>>> upstream-releases

  gBrowser.showOnlyTheseTabs([origTab, tabOne, tabTwo]);
  pressCtrlTab();
<<<<<<< HEAD
  ok(ctrlTab.isOpen, "Ctrl+Tab opens the preview panel after re-showing hidden tabs");
  is(ctrlTab.tabList.length, 3, "Ctrl+Tab panel displays all visible tabs after re-showing hidden ones");
||||||| merged common ancestors
  ok(ctrlTab.isOpen, "With 3 tabs open, Ctrl+Tab does open the preview panel");
=======
  ok(
    ctrlTab.isOpen,
    "Ctrl+Tab opens the preview panel after re-showing hidden tabs"
  );
  is(
    ctrlTab.tabList.length,
    3,
    "Ctrl+Tab panel displays all visible tabs after re-showing hidden ones"
  );
>>>>>>> upstream-releases
  releaseCtrl();

  // cleanup
  gBrowser.removeTab(tabOne);
  gBrowser.removeTab(tabTwo);
});

function pressCtrlTab(aShiftKey) {
  EventUtils.synthesizeKey("VK_TAB", { ctrlKey: true, shiftKey: !!aShiftKey });
}

function releaseCtrl() {
  EventUtils.synthesizeKey("VK_CONTROL", { type: "keyup" });
}
