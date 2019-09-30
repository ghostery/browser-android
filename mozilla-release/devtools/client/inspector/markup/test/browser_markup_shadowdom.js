/* vim: set ts=2 et sw=2 tw=80: */
/* Any copyright is dedicated to the Public Domain.
 http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

requestLongerTimeout(2);

// Test a few static pages using webcomponents and check that they are displayed as
// expected in the markup view.

const TEST_DATA = [
  {
    // Test that expanding a shadow host shows a shadow root node and direct children.
    // Test that expanding a shadow root shows the shadow dom.
    // Test that slotted elements are visible in the shadow dom.
    title: "generic shadow dom test",
    url: `data:text/html;charset=utf-8,
      <test-component>
        <div slot="slot1">slotted-1<div>inner</div></div>
        <div slot="slot2">slotted-2<div>inner</div></div>
        <div class="no-slot-class">no-slot-text<div>inner</div></div>
      </test-component>

      <script>
        'use strict';
        customElements.define('test-component', class extends HTMLElement {
          constructor() {
            super();
            let shadowRoot = this.attachShadow({mode: "#MODE#"});
            shadowRoot.innerHTML = \`
                <slot name="slot1"></slot>
                <slot name="slot2"></slot>
                <slot></slot>
              \`;
          }
        });
      </script>`,
    tree: `
      test-component
        #shadow-root
          name="slot1"
            div!slotted
          name="slot2"
            div!slotted
          slot
            div!slotted
        slot="slot1"
          slotted-1
          inner
        slot="slot2"
          slotted-2
          inner
        class="no-slot-class"
          no-slot-text
<<<<<<< HEAD
          inner`,

  }, {
||||||| merged common ancestors
          inner`

  }, {
=======
          inner`,
  },
  {
>>>>>>> upstream-releases
    // Test that components without any direct children still display a shadow root node,
    // if a shadow root is attached to the host.
    title: "shadow root without direct children",
    url: `data:text/html;charset=utf-8,
      <test-component></test-component>
      <script>
        "use strict";
        customElements.define("test-component", class extends HTMLElement {
          constructor() {
            super();
            let shadowRoot = this.attachShadow({mode: "#MODE#"});
            shadowRoot.innerHTML = "<slot><div>fallback-content</div></slot>";
          }
        });
      </script>`,
    tree: `
      test-component
        #shadow-root
          slot
<<<<<<< HEAD
            fallback-content`,

  }, {
||||||| merged common ancestors
            fallback-content`

  }, {
=======
            fallback-content`,
  },
  {
>>>>>>> upstream-releases
    // Test that markup view is correctly displayed for non-trivial shadow DOM nesting.
    title: "nested components",
    url: `data:text/html;charset=utf-8,
      <test-component >
        <div slot="slot1">slot1-1</div>
        <third-component slot="slot2"></third-component>
      </test-component>

      <script>
      (function() {
        'use strict';

        function defineComponent(name, html) {
          customElements.define(name, class extends HTMLElement {
            constructor() {
              super();
              let shadowRoot = this.attachShadow({mode: "#MODE#"});
              shadowRoot.innerHTML = html;
            }
          });
        }

        defineComponent('test-component', \`
          <div id="test-container">
            <slot name="slot1"></slot>
            <slot name="slot2"></slot>
            <other-component><div slot="other1">other1-content</div></other-component>
          </div>\`);
        defineComponent('other-component',
          '<div id="other-container"><slot id="other1" name="other1"></slot></div>');
        defineComponent('third-component', '<div>Third component</div>');
      })();
      </script>`,
    tree: `
      test-component
        #shadow-root
          test-container
            slot
              div!slotted
            slot
              third-component!slotted
            other-component
              #shadow-root
                div
                  slot
                    div!slotted
              div
        div
        third-component
          #shadow-root
<<<<<<< HEAD
            div`,

  }, {
||||||| merged common ancestors
            div`

  }, {
=======
            div`,
  },
  {
>>>>>>> upstream-releases
    // Test that ::before and ::after pseudo elements are correctly displayed in host
    // components and in slot elements.
    title: "pseudo elements",
    url: `data:text/html;charset=utf-8,
      <style>
        test-component::before { content: "before-host" }
        test-component::after { content: "after-host" }
      </style>

      <test-component>
        <div class="light-dom"></div>
      </test-component>

      <script>
        "use strict";
        customElements.define("test-component", class extends HTMLElement {
          constructor() {
            super();
            let shadowRoot = this.attachShadow({mode: "#MODE#"});
            shadowRoot.innerHTML = \`
              <style>
                slot { display: block } /* avoid whitespace nodes */
                slot::before { content: "before-slot" }
                slot::after { content: "after-slot" }
              </style>
              <slot>default content</slot>
            \`;
          }
        });
      </script>`,
    tree: `
      test-component
        #shadow-root
          style
            slot { display: block }
          slot
            ::before
            div!slotted
            ::after
        ::before
        class="light-dom"
<<<<<<< HEAD
        ::after`,

  }, {
||||||| merged common ancestors
        ::after`

  }, {
=======
        ::after`,
  },
  {
>>>>>>> upstream-releases
    // Test empty web components are still displayed correctly.
    title: "empty components",
    url: `data:text/html;charset=utf-8,
      <test-component></test-component>

      <script>
        "use strict";
        customElements.define("test-component", class extends HTMLElement {
          constructor() {
            super();
            let shadowRoot = this.attachShadow({mode: "#MODE#"});
            shadowRoot.innerHTML = "";
          }
        });
      </script>`,
    tree: `
      test-component
<<<<<<< HEAD
        #shadow-root`,
  }, {
    // Test shadow hosts show their shadow root even if they contain just a short text.
    title: "shadow host with inline-text-child",
    url: `data:text/html;charset=utf-8,
      <test-component>
        <inner-component>short-text-outside</inner-component>
      </test-component>

      <script>
        "use strict";

        customElements.define("test-component", class extends HTMLElement {
          constructor() {
            super();
            let shadowRoot = this.attachShadow({mode: "#MODE#"});
            shadowRoot.innerHTML = "<div><slot></slot></div>";
          }
        });

        customElements.define("inner-component", class extends HTMLElement {
          constructor() {
            super();
            let shadowRoot = this.attachShadow({mode: "#MODE#"});
            shadowRoot.innerHTML = "short-text-inside";
          }
        });
      </script>`,
    tree: `
      test-component
        #shadow-root
          div
            slot
              inner-component!slotted
        inner-component
          #shadow-root
            short-text-inside
          short-text-outside`,
  },
||||||| merged common ancestors
        #shadow-root`
  }
=======
        #shadow-root`,
  },
  {
    // Test shadow hosts show their shadow root even if they contain just a short text.
    title: "shadow host with inline-text-child",
    url: `data:text/html;charset=utf-8,
      <test-component>
        <inner-component>short-text-outside</inner-component>
      </test-component>

      <script>
        "use strict";

        customElements.define("test-component", class extends HTMLElement {
          constructor() {
            super();
            let shadowRoot = this.attachShadow({mode: "#MODE#"});
            shadowRoot.innerHTML = "<div><slot></slot></div>";
          }
        });

        customElements.define("inner-component", class extends HTMLElement {
          constructor() {
            super();
            let shadowRoot = this.attachShadow({mode: "#MODE#"});
            shadowRoot.innerHTML = "short-text-inside";
          }
        });
      </script>`,
    tree: `
      test-component
        #shadow-root
          div
            slot
              inner-component!slotted
        inner-component
          #shadow-root
            short-text-inside
          short-text-outside`,
  },
  {
    // Test for Bug 1537877, crash with nested custom elements without slot.
    title: "nested components without slot",
    url: `data:text/html;charset=utf-8,
      <test-component>
        <inner-component slot="non-existing-slot"></inner-component>
      </test-component>

      <script>
        "use strict";

        customElements.define('test-component', class extends HTMLElement {
          constructor() {
            super();
            let shadowRoot = this.attachShadow({mode: "#MODE#"});
            shadowRoot.innerHTML = '<div>test-component-content</div>'
          }
        });

        customElements.define('inner-component', class extends HTMLElement {
          constructor() {
            super();
            let shadowRoot = this.attachShadow({mode: "#MODE#"});
            shadowRoot.innerHTML = 'inner-component-content'
          }
        });
      </script>`,
    tree: `
      test-component
        #shadow-root
          div
        inner-component
          #shadow-root
            inner-component-content`,
  },
>>>>>>> upstream-releases
];

for (const { url, tree, title } of TEST_DATA) {
  // Test each configuration in both open and closed modes
  add_task(async function() {
    info(`Testing: [${title}] in OPEN mode`);
<<<<<<< HEAD
    const {inspector, tab} = await openInspectorForURL(url.replace(/#MODE#/g, "open"));
||||||| merged common ancestors
    await enableWebComponents();
    const {inspector} = await openInspectorForURL(url.replace("#MODE#", "open"));
=======
    const { inspector, tab } = await openInspectorForURL(
      url.replace(/#MODE#/g, "open")
    );
>>>>>>> upstream-releases
    await assertMarkupViewAsTree(tree, "test-component", inspector);
    await removeTab(tab);
  });
  add_task(async function() {
    info(`Testing: [${title}] in CLOSED mode`);
<<<<<<< HEAD
    const {inspector, tab} = await openInspectorForURL(url.replace(/#MODE#/g, "closed"));
||||||| merged common ancestors
    await enableWebComponents();
    const {inspector} = await openInspectorForURL(url.replace("#MODE#", "closed"));
=======
    const { inspector, tab } = await openInspectorForURL(
      url.replace(/#MODE#/g, "closed")
    );
>>>>>>> upstream-releases
    await assertMarkupViewAsTree(tree, "test-component", inspector);
    await removeTab(tab);
  });
}
