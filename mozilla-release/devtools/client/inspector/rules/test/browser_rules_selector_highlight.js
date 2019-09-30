/* vim: set ft=javascript ts=2 et sw=2 tw=80: */
/* Any copyright is dedicated to the Public Domain.
 http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

// Tests that the rule view selector text is highlighted correctly according
// to the components of the selector.

const TEST_URI = [
  "<style type='text/css'>",
  "  h1 {}",
  "  h1#testid {}",
  "  h1 + p {}",
  '  div[hidden="true"] {}',
  '  div[title="test"][checked=true] {}',
  "  p:empty {}",
  "  p:lang(en) {}",
  "  .testclass:active {}",
  "  .testclass:focus {}",
  "  .testclass:hover {}",
  "</style>",
  "<h1>Styled Node</h1>",
  "<p>Paragraph</p>",
  '<h1 id="testid">Styled Node</h1>',
  '<div hidden="true"></div>',
  '<div title="test" checked="true"></div>',
  "<p></p>",
<<<<<<< HEAD
  "<p lang=\"en\">Paragraph<p>",
  "<div class=\"testclass\">Styled Node</div>",
||||||| merged common ancestors
  "<p lang=\"en\">Paragraph<p>",
  "<div class=\"testclass\">Styled Node</div>"
=======
  '<p lang="en">Paragraph<p>',
  '<div class="testclass">Styled Node</div>',
>>>>>>> upstream-releases
].join("\n");

const SELECTOR_ATTRIBUTE = "ruleview-selector-attribute";
const SELECTOR_ELEMENT = "ruleview-selector";
const SELECTOR_PSEUDO_CLASS = "ruleview-selector-pseudo-class";
const SELECTOR_PSEUDO_CLASS_LOCK = "ruleview-selector-pseudo-class-lock";

const TEST_DATA = [
  {
    node: "h1",
<<<<<<< HEAD
    expected: [
      { value: "h1", class: SELECTOR_ELEMENT },
    ],
||||||| merged common ancestors
    expected: [
      { value: "h1", class: SELECTOR_ELEMENT }
    ]
=======
    expected: [{ value: "h1", class: SELECTOR_ELEMENT }],
>>>>>>> upstream-releases
  },
  {
    node: "h1 + p",
<<<<<<< HEAD
    expected: [
      { value: "h1 + p", class: SELECTOR_ELEMENT },
    ],
||||||| merged common ancestors
    expected: [
      { value: "h1 + p", class: SELECTOR_ELEMENT }
    ]
=======
    expected: [{ value: "h1 + p", class: SELECTOR_ELEMENT }],
>>>>>>> upstream-releases
  },
  {
    node: "h1#testid",
<<<<<<< HEAD
    expected: [
      { value: "h1#testid", class: SELECTOR_ELEMENT },
    ],
||||||| merged common ancestors
    expected: [
      { value: "h1#testid", class: SELECTOR_ELEMENT }
    ]
=======
    expected: [{ value: "h1#testid", class: SELECTOR_ELEMENT }],
>>>>>>> upstream-releases
  },
  {
    node: "div[hidden='true']",
    expected: [
      { value: "div", class: SELECTOR_ELEMENT },
<<<<<<< HEAD
      { value: "[hidden=\"true\"]", class: SELECTOR_ATTRIBUTE },
    ],
||||||| merged common ancestors
      { value: "[hidden=\"true\"]", class: SELECTOR_ATTRIBUTE }
    ]
=======
      { value: '[hidden="true"]', class: SELECTOR_ATTRIBUTE },
    ],
>>>>>>> upstream-releases
  },
  {
    node: 'div[title="test"][checked="true"]',
    expected: [
      { value: "div", class: SELECTOR_ELEMENT },
<<<<<<< HEAD
      { value: "[title=\"test\"]", class: SELECTOR_ATTRIBUTE },
      { value: "[checked=\"true\"]", class: SELECTOR_ATTRIBUTE },
    ],
||||||| merged common ancestors
      { value: "[title=\"test\"]", class: SELECTOR_ATTRIBUTE },
      { value: "[checked=\"true\"]", class: SELECTOR_ATTRIBUTE }
    ]
=======
      { value: '[title="test"]', class: SELECTOR_ATTRIBUTE },
      { value: '[checked="true"]', class: SELECTOR_ATTRIBUTE },
    ],
>>>>>>> upstream-releases
  },
  {
    node: "p:empty",
    expected: [
      { value: "p", class: SELECTOR_ELEMENT },
      { value: ":empty", class: SELECTOR_PSEUDO_CLASS },
    ],
  },
  {
    node: "p:lang(en)",
    expected: [
      { value: "p", class: SELECTOR_ELEMENT },
      { value: ":lang(en)", class: SELECTOR_PSEUDO_CLASS },
    ],
  },
  {
    node: ".testclass",
    pseudoClass: ":active",
    expected: [
      { value: ".testclass", class: SELECTOR_ELEMENT },
      { value: ":active", class: SELECTOR_PSEUDO_CLASS_LOCK },
    ],
  },
  {
    node: ".testclass",
    pseudoClass: ":focus",
    expected: [
      { value: ".testclass", class: SELECTOR_ELEMENT },
      { value: ":focus", class: SELECTOR_PSEUDO_CLASS_LOCK },
    ],
  },
  {
    node: ".testclass",
    pseudoClass: ":hover",
    expected: [
      { value: ".testclass", class: SELECTOR_ELEMENT },
      { value: ":hover", class: SELECTOR_PSEUDO_CLASS_LOCK },
    ],
  },
];

add_task(async function() {
  await addTab("data:text/html;charset=utf-8," + encodeURIComponent(TEST_URI));
  const { inspector, view } = await openRuleView();

  for (const { node, pseudoClass, expected } of TEST_DATA) {
    await selectNode(node, inspector);

    if (pseudoClass) {
      const onRefresh = inspector.once("rule-view-refreshed");
      inspector.togglePseudoClass(pseudoClass);
      await onRefresh;
    }

    const selectorContainer = getRuleViewRuleEditor(view, 1).selectorText
      .firstChild;

    if (selectorContainer.children.length === expected.length) {
      for (let i = 0; i < expected.length; i++) {
        is(
          expected[i].value,
          selectorContainer.children[i].textContent,
          "Got expected selector value: " +
            expected[i].value +
            " == " +
            selectorContainer.children[i].textContent
        );
        is(
          expected[i].class,
          selectorContainer.children[i].className,
          "Got expected class name: " +
            expected[i].class +
            " == " +
            selectorContainer.children[i].className
        );
      }
    } else {
      for (const selector of selectorContainer.children) {
        info(
          "Actual selector components: { value: " +
            selector.textContent +
            ", class: " +
            selector.className +
            " }\n"
        );
      }
    }
  }
});
