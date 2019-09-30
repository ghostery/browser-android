<<<<<<< HEAD
import {addLocaleData, IntlProvider} from "react-intl";
import {actionCreators as ac} from "common/Actions.jsm";
import {OUTGOING_MESSAGE_NAME as AS_GENERAL_OUTGOING_MESSAGE_NAME} from "content-src/lib/init-store";
import {generateMessages} from "./rich-text-strings";
import {ImpressionsWrapper} from "./components/ImpressionsWrapper/ImpressionsWrapper";
import {LocalizationProvider} from "fluent-react";
import {OnboardingMessage} from "./templates/OnboardingMessage/OnboardingMessage";
||||||| merged common ancestors
import {actionCreators as ac} from "common/Actions.jsm";
import {OUTGOING_MESSAGE_NAME as AS_GENERAL_OUTGOING_MESSAGE_NAME} from "content-src/lib/init-store";
import {generateMessages} from "./rich-text-strings";
import {ImpressionsWrapper} from "./components/ImpressionsWrapper/ImpressionsWrapper";
import {LocalizationProvider} from "fluent-react";
import {OnboardingMessage} from "./templates/OnboardingMessage/OnboardingMessage";
=======
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import {
  actionCreators as ac,
  actionTypes as at,
  ASRouterActions as ra,
} from "common/Actions.jsm";
import { OUTGOING_MESSAGE_NAME as AS_GENERAL_OUTGOING_MESSAGE_NAME } from "content-src/lib/init-store";
import { generateBundles } from "./rich-text-strings";
import { ImpressionsWrapper } from "./components/ImpressionsWrapper/ImpressionsWrapper";
import { LocalizationProvider } from "fluent-react";
import { NEWTAB_DARK_THEME } from "content-src/lib/constants";
>>>>>>> upstream-releases
import React from "react";
import ReactDOM from "react-dom";
<<<<<<< HEAD
import {ReturnToAMO} from "./templates/ReturnToAMO/ReturnToAMO";
import {SnippetsTemplates} from "./templates/template-manifest";
import {StartupOverlay} from "./templates/StartupOverlay/StartupOverlay";
||||||| merged common ancestors
import {SnippetsTemplates} from "./templates/template-manifest";
=======
import { SnippetsTemplates } from "./templates/template-manifest";
import { FirstRun } from "./templates/FirstRun/FirstRun";
>>>>>>> upstream-releases

const INCOMING_MESSAGE_NAME = "ASRouter:parent-to-child";
const OUTGOING_MESSAGE_NAME = "ASRouter:child-to-parent";
const TEMPLATES_ABOVE_PAGE = [
  "trailhead",
  "fxa_overlay",
  "return_to_amo_overlay",
  "extended_triplets",
];
const FIRST_RUN_TEMPLATES = TEMPLATES_ABOVE_PAGE;
const TEMPLATES_BELOW_SEARCH = ["simple_below_search_snippet"];

export const ASRouterUtils = {
  addListener(listener) {
    if (global.RPMAddMessageListener) {
      global.RPMAddMessageListener(INCOMING_MESSAGE_NAME, listener);
    }
  },
  removeListener(listener) {
    if (global.RPMRemoveMessageListener) {
      global.RPMRemoveMessageListener(INCOMING_MESSAGE_NAME, listener);
    }
  },
  sendMessage(action) {
    if (global.RPMSendAsyncMessage) {
      global.RPMSendAsyncMessage(OUTGOING_MESSAGE_NAME, action);
    }
  },
  blockById(id, options) {
    ASRouterUtils.sendMessage({
      type: "BLOCK_MESSAGE_BY_ID",
      data: { id, ...options },
    });
  },
  dismissById(id) {
<<<<<<< HEAD
    ASRouterUtils.sendMessage({type: "DISMISS_MESSAGE_BY_ID", data: {id}});
  },
  dismissBundle(bundle) {
    ASRouterUtils.sendMessage({type: "DISMISS_BUNDLE", data: {bundle}});
||||||| merged common ancestors
    ASRouterUtils.sendMessage({type: "DISMISS_MESSAGE_BY_ID", data: {id}});
  },
  blockBundle(bundle) {
    ASRouterUtils.sendMessage({type: "BLOCK_BUNDLE", data: {bundle}});
=======
    ASRouterUtils.sendMessage({ type: "DISMISS_MESSAGE_BY_ID", data: { id } });
>>>>>>> upstream-releases
  },
  executeAction(button_action) {
    ASRouterUtils.sendMessage({
      type: "USER_ACTION",
      data: button_action,
    });
  },
  unblockById(id) {
    ASRouterUtils.sendMessage({ type: "UNBLOCK_MESSAGE_BY_ID", data: { id } });
  },
  unblockBundle(bundle) {
    ASRouterUtils.sendMessage({ type: "UNBLOCK_BUNDLE", data: { bundle } });
  },
  overrideMessage(id) {
    ASRouterUtils.sendMessage({ type: "OVERRIDE_MESSAGE", data: { id } });
  },
  sendTelemetry(ping) {
    if (global.RPMSendAsyncMessage) {
      const payload = ac.ASRouterUserEvent(ping);
      global.RPMSendAsyncMessage(AS_GENERAL_OUTGOING_MESSAGE_NAME, payload);
    }
  },
  getPreviewEndpoint() {
    if (global.location && global.location.href.includes("endpoint")) {
      const params = new URLSearchParams(
        global.location.href.slice(global.location.href.indexOf("endpoint"))
      );
      try {
        const endpoint = new URL(params.get("endpoint"));
        return {
          url: endpoint.href,
          snippetId: params.get("snippetId"),
          theme: this.getPreviewTheme(),
        };
      } catch (e) {}
    }

    return null;
  },
  getPreviewTheme() {
    return new URLSearchParams(
      global.location.href.slice(global.location.href.indexOf("theme"))
    ).get("theme");
  },
};

// Note: nextProps/prevProps refer to props passed to <ImpressionsWrapper />, not <ASRouterUISurface />
function shouldSendImpressionOnUpdate(nextProps, prevProps) {
  return (
    nextProps.message.id &&
    (!prevProps.message || prevProps.message.id !== nextProps.message.id)
  );
}

export class ASRouterUISurface extends React.PureComponent {
  constructor(props) {
    super(props);
    this.onMessageFromParent = this.onMessageFromParent.bind(this);
    this.sendClick = this.sendClick.bind(this);
    this.sendImpression = this.sendImpression.bind(this);
    this.sendUserActionTelemetry = this.sendUserActionTelemetry.bind(this);
    this.onUserAction = this.onUserAction.bind(this);
    this.fetchFlowParams = this.fetchFlowParams.bind(this);

    this.state = { message: {}, interruptCleared: false };
    if (props.document) {
      this.headerPortal = props.document.getElementById(
        "header-asrouter-container"
      );
      this.footerPortal = props.document.getElementById(
        "footer-asrouter-container"
      );
    }
  }

  async fetchFlowParams(params = {}) {
    let result = {};
    const { fxaEndpoint, dispatch } = this.props;
    if (!fxaEndpoint) {
      const err =
        "Tried to fetch flow params before fxaEndpoint pref was ready";
      console.error(err); // eslint-disable-line no-console
    }

    try {
      const urlObj = new URL(fxaEndpoint);
      urlObj.pathname = "metrics-flow";
      Object.keys(params).forEach(key => {
        urlObj.searchParams.append(key, params[key]);
      });
      const response = await fetch(urlObj.toString(), { credentials: "omit" });
      if (response.status === 200) {
        const { deviceId, flowId, flowBeginTime } = await response.json();
        result = { deviceId, flowId, flowBeginTime };
      } else {
        console.error("Non-200 response", response); // eslint-disable-line no-console
        dispatch(
          ac.OnlyToMain({
            type: at.TELEMETRY_UNDESIRED_EVENT,
            data: {
              event: "FXA_METRICS_FETCH_ERROR",
              value: response.status,
            },
          })
        );
      }
    } catch (error) {
      console.error(error); // eslint-disable-line no-console
      dispatch(
        ac.OnlyToMain({
          type: at.TELEMETRY_UNDESIRED_EVENT,
          data: { event: "FXA_METRICS_ERROR" },
        })
      );
    }
    return result;
  }

  sendUserActionTelemetry(extraProps = {}) {
    const { message } = this.state;
    const eventType = `${message.provider}_user_event`;
    ASRouterUtils.sendTelemetry({
      message_id: message.id,
      source: extraProps.id,
      action: eventType,
      ...extraProps,
    });
  }

  sendImpression(extraProps) {
    if (this.state.message.provider === "preview") {
      return;
    }

    ASRouterUtils.sendMessage({ type: "IMPRESSION", data: this.state.message });
    this.sendUserActionTelemetry({ event: "IMPRESSION", ...extraProps });
  }

  // If link has a `metric` data attribute send it as part of the `value`
  // telemetry field which can have arbitrary values.
  // Used for router messages with links as part of the content.
  sendClick(event) {
    const metric = {
      value: event.target.dataset.metric,
      // Used for the `source` of the event. Needed to differentiate
      // from other snippet or onboarding events that may occur.
      id: "NEWTAB_FOOTER_BAR_CONTENT",
    };
    const action = {
      type: event.target.dataset.action,
      data: { args: event.target.dataset.args },
    };
    if (action.type) {
      ASRouterUtils.executeAction(action);
    }
    if (
      !this.state.message.content.do_not_autoblock &&
      !event.target.dataset.do_not_autoblock
    ) {
      ASRouterUtils.blockById(this.state.message.id);
    }
    if (this.state.message.provider !== "preview") {
      this.sendUserActionTelemetry({ event: "CLICK_BUTTON", ...metric });
    }
  }

  onBlockById(id) {
    return options => ASRouterUtils.blockById(id, options);
  }

  onDismissById(id) {
    return () => ASRouterUtils.dismissById(id);
  }

<<<<<<< HEAD
  dismissBundle(bundle) {
    return () => ASRouterUtils.dismissBundle(bundle);
  }

  triggerOnboarding() {
    ASRouterUtils.sendMessage({type: "TRIGGER", data: {trigger: {id: "showOnboarding"}}});
||||||| merged common ancestors
  clearBundle(bundle) {
    return () => ASRouterUtils.blockBundle(bundle);
=======
  clearMessage(id) {
    if (id === this.state.message.id) {
      this.setState({ message: {} });
      // Remove any styles related to the RTAMO message
      document.body.classList.remove("welcome", "hide-main", "amo");
    }
>>>>>>> upstream-releases
  }

  onMessageFromParent({ data: action }) {
    switch (action.type) {
      case "SET_MESSAGE":
        this.setState({ message: action.data });
        break;
      case "CLEAR_INTERRUPT":
        this.setState({ interruptCleared: true });
        break;
      case "CLEAR_MESSAGE":
        this.clearMessage(action.data.id);
        break;
      case "CLEAR_PROVIDER":
        if (action.data.id === this.state.message.provider) {
          this.setState({ message: {} });
        }
        break;
      case "CLEAR_ALL":
        this.setState({ message: {} });
        break;
      case "AS_ROUTER_TARGETING_UPDATE":
        action.data.forEach(id => this.clearMessage(id));
        break;
    }
  }

  componentWillMount() {
    // Add locale data for StartupOverlay because it uses react-intl
    addLocaleData(global.document.documentElement.lang);

    const endpoint = ASRouterUtils.getPreviewEndpoint();
    if (endpoint && endpoint.theme === "dark") {
      global.window.dispatchEvent(
        new CustomEvent("LightweightTheme:Set", {
          detail: { data: NEWTAB_DARK_THEME },
        })
      );
    }
    ASRouterUtils.addListener(this.onMessageFromParent);

    // If we are loading about:welcome we want to trigger the onboarding messages
    if (
      this.props.document &&
      this.props.document.location.href === "about:welcome"
    ) {
      ASRouterUtils.sendMessage({
        type: "TRIGGER",
        data: { trigger: { id: "firstRun" } },
      });
    } else {
      ASRouterUtils.sendMessage({
        type: "NEWTAB_MESSAGE_REQUEST",
        data: { endpoint },
      });
    }
  }

  componentWillUnmount() {
    ASRouterUtils.removeListener(this.onMessageFromParent);
  }

  async getMonitorUrl({ url, flowRequestParams = {} }) {
    const flowValues = await this.fetchFlowParams(flowRequestParams);

    // Note that flowParams are actually added dynamically on the page
    const urlObj = new URL(url);
    ["deviceId", "flowId", "flowBeginTime"].forEach(key => {
      if (key in flowValues) {
        urlObj.searchParams.append(key, flowValues[key]);
      }
    });

    return urlObj.toString();
  }

  async onUserAction(action) {
    switch (action.type) {
      // This needs to be handled locally because its
      case ra.ENABLE_FIREFOX_MONITOR:
        const url = await this.getMonitorUrl(action.data.args);
        ASRouterUtils.executeAction({ type: ra.OPEN_URL, data: { args: url } });
        break;
      default:
        ASRouterUtils.executeAction(action);
    }
  }

  renderSnippets() {
<<<<<<< HEAD
    if (this.state.bundle.template === "onboarding" ||
        this.state.message.template === "fxa_overlay" ||
        this.state.message.template === "return_to_amo_overlay") {
      return null;
    }
    const SnippetComponent = SnippetsTemplates[this.state.message.template];
    const {content} = this.state.message;
||||||| merged common ancestors
    const SnippetComponent = SnippetsTemplates[this.state.message.template];
    const {content} = this.state.message;
=======
    const { message } = this.state;
    if (!SnippetsTemplates[message.template]) {
      return null;
    }
    const SnippetComponent = SnippetsTemplates[message.template];
    const { content } = this.state.message;
>>>>>>> upstream-releases

    return (
      <ImpressionsWrapper
        id="NEWTAB_FOOTER_BAR"
        message={this.state.message}
        sendImpression={this.sendImpression}
        shouldSendImpressionOnUpdate={shouldSendImpressionOnUpdate}
        // This helps with testing
<<<<<<< HEAD
        document={this.props.document}>
          <LocalizationProvider messages={generateMessages(content)}>
            <SnippetComponent
              {...this.state.message}
              UISurface="NEWTAB_FOOTER_BAR"
              onBlock={this.onBlockById(this.state.message.id)}
              onDismiss={this.onDismissById(this.state.message.id)}
              onAction={ASRouterUtils.executeAction}
              sendClick={this.sendClick}
              sendUserActionTelemetry={this.sendUserActionTelemetry} />
          </LocalizationProvider>
      </ImpressionsWrapper>);
  }

  renderOnboarding() {
    if (this.state.bundle.template === "onboarding") {
      return (
        <OnboardingMessage
          {...this.state.bundle}
          UISurface="NEWTAB_OVERLAY"
          onAction={ASRouterUtils.executeAction}
          onDoneButton={this.dismissBundle(this.state.bundle.bundle)}
          sendUserActionTelemetry={this.sendUserActionTelemetry} />);
    }
    return null;
  }

  renderFirstRunOverlay() {
    const {message} = this.state;
    if (message.template === "fxa_overlay") {
      global.document.body.classList.add("fxa");
      return (
        <IntlProvider locale={global.document.documentElement.lang} messages={global.gActivityStreamStrings}>
          <StartupOverlay
            onReady={this.triggerOnboarding}
            onBlock={this.onDismissById(message.id)}
            dispatch={this.props.activityStreamStore.dispatch}
            store={this.props.activityStreamStore} />
        </IntlProvider>
      );
    } else if (message.template === "return_to_amo_overlay") {
      global.document.body.classList.add("amo");
      return (
        <LocalizationProvider messages={generateMessages({"amo_html": message.content.text})}>
          <ReturnToAMO
            {...message}
            onReady={this.triggerOnboarding}
            onBlock={this.onDismissById(message.id)}
            onAction={ASRouterUtils.executeAction} />
        </LocalizationProvider>
      );
    }
    return null;
||||||| merged common ancestors
        document={this.props.document}>
          <LocalizationProvider messages={generateMessages(content)}>
            <SnippetComponent
              {...this.state.message}
              UISurface="NEWTAB_FOOTER_BAR"
              onBlock={this.onBlockById(this.state.message.id)}
              onDismiss={this.onDismissById(this.state.message.id)}
              onAction={ASRouterUtils.executeAction}
              sendClick={this.sendClick}
              sendUserActionTelemetry={this.sendUserActionTelemetry} />
          </LocalizationProvider>
      </ImpressionsWrapper>);
  }

  renderOnboarding() {
    return (
      <OnboardingMessage
        {...this.state.bundle}
        UISurface="NEWTAB_OVERLAY"
        onAction={ASRouterUtils.executeAction}
        onDoneButton={this.clearBundle(this.state.bundle.bundle)}
        sendUserActionTelemetry={this.sendUserActionTelemetry} />);
=======
        document={this.props.document}
      >
        <LocalizationProvider bundles={generateBundles(content)}>
          <SnippetComponent
            {...this.state.message}
            UISurface="NEWTAB_FOOTER_BAR"
            onBlock={this.onBlockById(this.state.message.id)}
            onDismiss={this.onDismissById(this.state.message.id)}
            onAction={this.onUserAction}
            sendClick={this.sendClick}
            sendUserActionTelemetry={this.sendUserActionTelemetry}
          />
        </LocalizationProvider>
      </ImpressionsWrapper>
    );
>>>>>>> upstream-releases
  }

  renderPreviewBanner() {
    if (this.state.message.provider !== "preview") {
      return null;
    }

    return (
      <div className="snippets-preview-banner">
        <span className="icon icon-small-spacer icon-info" />
        <span>Preview Purposes Only</span>
      </div>
    );
  }

<<<<<<< HEAD
  render() {
    const {message, bundle} = this.state;
    if (!message.id && !bundle.template) { return null; }
    return (
      <React.Fragment>
        {this.renderPreviewBanner()}
        {this.renderFirstRunOverlay()}
        {this.renderOnboarding()}
        {this.renderSnippets()}
      </React.Fragment>
    );
  }
}

ASRouterUISurface.defaultProps = {document: global.document};

export class ASRouterContent {
  constructor() {
    this.initialized = false;
    this.containerElement = null;
  }

  _mount() {
    this.containerElement = global.document.getElementById(ASR_CONTAINER_ID);
    if (!this.containerElement) {
      this.containerElement = global.document.createElement("div");
      this.containerElement.id = ASR_CONTAINER_ID;
      this.containerElement.style.zIndex = 1;
      global.document.body.appendChild(this.containerElement);
||||||| merged common ancestors
  render() {
    const {message, bundle} = this.state;
    if (!message.id && !bundle.template) { return null; }
    return (
      <React.Fragment>
        {this.renderPreviewBanner()}
        {bundle.template === "onboarding" ? this.renderOnboarding() : this.renderSnippets()}
      </React.Fragment>
    );
  }
}

ASRouterUISurface.defaultProps = {document: global.document};

export class ASRouterContent {
  constructor() {
    this.initialized = false;
    this.containerElement = null;
  }

  _mount() {
    this.containerElement = global.document.getElementById(ASR_CONTAINER_ID);
    if (!this.containerElement) {
      this.containerElement = global.document.createElement("div");
      this.containerElement.id = ASR_CONTAINER_ID;
      this.containerElement.style.zIndex = 1;
      global.document.body.appendChild(this.containerElement);
=======
  renderFirstRun() {
    const { message } = this.state;
    if (FIRST_RUN_TEMPLATES.includes(message.template)) {
      return (
        <ImpressionsWrapper
          id="FIRST_RUN"
          message={this.state.message}
          sendImpression={this.sendImpression}
          shouldSendImpressionOnUpdate={shouldSendImpressionOnUpdate}
          // This helps with testing
          document={this.props.document}
        >
          <FirstRun
            document={this.props.document}
            interruptCleared={this.state.interruptCleared}
            message={message}
            sendUserActionTelemetry={this.sendUserActionTelemetry}
            executeAction={ASRouterUtils.executeAction}
            dispatch={this.props.dispatch}
            onBlockById={ASRouterUtils.blockById}
            onDismiss={this.onDismissById(this.state.message.id)}
            fxaEndpoint={this.props.fxaEndpoint}
            fetchFlowParams={this.fetchFlowParams}
          />
        </ImpressionsWrapper>
      );
>>>>>>> upstream-releases
    }
<<<<<<< HEAD

    ReactDOM.render(<ASRouterUISurface activityStreamStore={this._activityStreamStore} />, this.containerElement);
  }

  _unmount() {
    ReactDOM.unmountComponentAtNode(this.containerElement);
  }

  init(store) {
    this._activityStreamStore = store;
    this._mount();
    this.initialized = true;
||||||| merged common ancestors

    ReactDOM.render(<ASRouterUISurface />, this.containerElement);
  }

  _unmount() {
    ReactDOM.unmountComponentAtNode(this.containerElement);
  }

  init() {
    this._mount();
    this.initialized = true;
=======
    return null;
>>>>>>> upstream-releases
  }

  render() {
    const { message } = this.state;
    if (!message.id) {
      return null;
    }
    const shouldRenderBelowSearch = TEMPLATES_BELOW_SEARCH.includes(
      message.template
    );
    const shouldRenderInHeader = TEMPLATES_ABOVE_PAGE.includes(
      message.template
    );

    return shouldRenderBelowSearch ? (
      // Render special below search snippets in place;
      <div className="below-search-snippet-wrapper">
        {this.renderSnippets()}
      </div>
    ) : (
      // For onboarding, regular snippets etc. we should render
      // everything in our footer container.
      ReactDOM.createPortal(
        <>
          {this.renderPreviewBanner()}
          {this.renderFirstRun()}
          {this.renderSnippets()}
        </>,
        shouldRenderInHeader ? this.headerPortal : this.footerPortal
      )
    );
  }
}

ASRouterUISurface.defaultProps = { document: global.document };
