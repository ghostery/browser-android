/* vim: set ft=javascript ts=2 et sw=2 tw=80: */
/* Any copyright is dedicated to the Public Domain.
 http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

// Test that class panel updates on markup mutations

add_task(async function() {
  await addTab("data:text/html;charset=utf-8,<div class='c1 c2'>");
  const { inspector, view, testActor } = await openRuleView();

  await selectNode("div", inspector);

  info("Open the class panel");
  view.showClassPanel();

  info("Trigger an unrelated mutation on the div (id attribute change)");
  let onMutation = view.inspector.once("markupmutation");
  await testActor.setAttribute("div", "id", "test-id");
  await onMutation;

  info("Check that the panel still contains the right classes");
  checkClassPanelContent(view, [
<<<<<<< HEAD
    {name: "c1", state: true},
    {name: "c2", state: true},
||||||| merged common ancestors
    {name: "c1", state: true},
    {name: "c2", state: true}
=======
    { name: "c1", state: true },
    { name: "c2", state: true },
>>>>>>> upstream-releases
  ]);

  info("Trigger a class mutation on a different, unknown, node");
  onMutation = view.inspector.once("markupmutation");
  await testActor.setAttribute("body", "class", "test-class");
  await onMutation;

  info("Check that the panel still contains the right classes");
  checkClassPanelContent(view, [
<<<<<<< HEAD
    {name: "c1", state: true},
    {name: "c2", state: true},
||||||| merged common ancestors
    {name: "c1", state: true},
    {name: "c2", state: true}
=======
    { name: "c1", state: true },
    { name: "c2", state: true },
>>>>>>> upstream-releases
  ]);

  info("Trigger a class mutation on the current node");
  onMutation = view.inspector.once("markupmutation");
  await testActor.setAttribute("div", "class", "c3 c4");
  await onMutation;

  info("Check that the panel now contains the new classes");
  checkClassPanelContent(view, [
<<<<<<< HEAD
    {name: "c3", state: true},
    {name: "c4", state: true},
||||||| merged common ancestors
    {name: "c3", state: true},
    {name: "c4", state: true}
=======
    { name: "c3", state: true },
    { name: "c4", state: true },
>>>>>>> upstream-releases
  ]);

  info("Change the state of one of the new classes");
  await toggleClassPanelCheckBox(view, "c4");
  checkClassPanelContent(view, [
<<<<<<< HEAD
    {name: "c3", state: true},
    {name: "c4", state: false},
||||||| merged common ancestors
    {name: "c3", state: true},
    {name: "c4", state: false}
=======
    { name: "c3", state: true },
    { name: "c4", state: false },
>>>>>>> upstream-releases
  ]);

  info("Select another node");
  await selectNode("body", inspector);

  info("Trigger a class mutation on the div");
  onMutation = view.inspector.once("markupmutation");
  await testActor.setAttribute("div", "class", "c5 c6 c7");
  await onMutation;

  info(
    "Go back to the previous node and check the content of the class panel." +
      "Even if hidden, it should have refreshed when we changed the DOM"
  );
  await selectNode("div", inspector);
  checkClassPanelContent(view, [
<<<<<<< HEAD
    {name: "c5", state: true},
    {name: "c6", state: true},
    {name: "c7", state: true},
||||||| merged common ancestors
    {name: "c5", state: true},
    {name: "c6", state: true},
    {name: "c7", state: true}
=======
    { name: "c5", state: true },
    { name: "c6", state: true },
    { name: "c7", state: true },
>>>>>>> upstream-releases
  ]);
});
