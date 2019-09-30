/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

const {
  createFactory,
  PureComponent,
} = require("devtools/client/shared/vendor/react");
const dom = require("devtools/client/shared/vendor/react-dom-factories");
const PropTypes = require("devtools/client/shared/vendor/react-prop-types");

const FluentReact = require("devtools/client/shared/vendor/fluent-react");
const Localized = createFactory(FluentReact.Localized);

<<<<<<< HEAD
const { PAGE_TYPES, RUNTIMES } = require("../../constants");
const Types = require("../../types/index");
loader.lazyRequireGetter(this, "ADB_ADDON_STATES", "devtools/shared/adb/adb-addon", true);
||||||| merged common ancestors
const { PAGES, RUNTIMES } = require("../../constants");
const Types = require("../../types");
loader.lazyRequireGetter(this, "ADB_ADDON_STATES", "devtools/shared/adb/adb-addon", true);
=======
const { ICON_LABEL_LEVEL, PAGE_TYPES, RUNTIMES } = require("../../constants");
const Types = require("../../types/index");
loader.lazyRequireGetter(
  this,
  "ADB_ADDON_STATES",
  "devtools/shared/adb/adb-addon",
  true
);
>>>>>>> upstream-releases

<<<<<<< HEAD
const SidebarItem = createFactory(require("./SidebarItem"));
||||||| merged common ancestors
=======
const IconLabel = createFactory(require("../shared/IconLabel"));
const SidebarItem = createFactory(require("./SidebarItem"));
>>>>>>> upstream-releases
const SidebarFixedItem = createFactory(require("./SidebarFixedItem"));
const SidebarRuntimeItem = createFactory(require("./SidebarRuntimeItem"));
<<<<<<< HEAD
const RefreshDevicesButton = createFactory(require("./RefreshDevicesButton"));
const FIREFOX_ICON = "chrome://devtools/skin/images/aboutdebugging-firefox-logo.svg";
const CONNECT_ICON = "chrome://devtools/skin/images/aboutdebugging-connect-icon.svg";
const GLOBE_ICON = "chrome://devtools/skin/images/aboutdebugging-globe-icon.svg";
const USB_ICON = "chrome://devtools/skin/images/aboutdebugging-connect-icon.svg";
||||||| merged common ancestors
const FIREFOX_ICON = "chrome://devtools/skin/images/aboutdebugging-firefox-logo.svg";
const CONNECT_ICON = "chrome://devtools/skin/images/aboutdebugging-connect-icon.svg";
const GLOBE_ICON = "chrome://devtools/skin/images/aboutdebugging-globe-icon.svg";
const USB_ICON = "chrome://devtools/skin/images/aboutdebugging-connect-icon.svg";
=======
const RefreshDevicesButton = createFactory(require("./RefreshDevicesButton"));
const FIREFOX_ICON =
  "chrome://devtools/skin/images/aboutdebugging-firefox-logo.svg";
const CONNECT_ICON = "chrome://devtools/skin/images/settings.svg";
const GLOBE_ICON =
  "chrome://devtools/skin/images/aboutdebugging-globe-icon.svg";
const USB_ICON =
  "chrome://devtools/skin/images/aboutdebugging-connect-icon.svg";
>>>>>>> upstream-releases

class Sidebar extends PureComponent {
  static get propTypes() {
    return {
      adbAddonStatus: Types.adbAddonStatus,
      className: PropTypes.string,
      dispatch: PropTypes.func.isRequired,
<<<<<<< HEAD
      isScanningUsb: PropTypes.bool.isRequired,
||||||| merged common ancestors
=======
      isAdbReady: PropTypes.bool.isRequired,
      isScanningUsb: PropTypes.bool.isRequired,
>>>>>>> upstream-releases
      networkRuntimes: PropTypes.arrayOf(Types.runtime).isRequired,
<<<<<<< HEAD
      selectedPage: PropTypes.string,
      selectedRuntime: PropTypes.string,
||||||| merged common ancestors
      selectedPage: PropTypes.string,
=======
      selectedPage: Types.page,
      selectedRuntimeId: PropTypes.string,
>>>>>>> upstream-releases
      usbRuntimes: PropTypes.arrayOf(Types.runtime).isRequired,
    };
  }

  renderAdbStatus() {
    const isUsbEnabled =
      this.props.isAdbReady &&
      this.props.adbAddonStatus === ADB_ADDON_STATES.INSTALLED;
    const localizationId = isUsbEnabled
      ? "about-debugging-sidebar-usb-enabled"
      : "about-debugging-sidebar-usb-disabled";
    return IconLabel(
      {
        level: isUsbEnabled ? ICON_LABEL_LEVEL.OK : ICON_LABEL_LEVEL.INFO,
      },
      Localized(
        {
          id: localizationId,
        },
        dom.span(
          {
            className: "qa-sidebar-usb-status",
          },
          localizationId
        )
      )
    );
  }

<<<<<<< HEAD
  renderDevicesEmpty() {
    return SidebarItem(
      {
        isSelected: false,
      },
      Localized(
||||||| merged common ancestors
  renderDevices() {
    const { networkRuntimes, usbRuntimes } = this.props;
    if (!networkRuntimes.length && !usbRuntimes.length) {
      return Localized(
=======
  renderDevicesEmpty() {
    return SidebarItem(
      {},
      Localized(
>>>>>>> upstream-releases
        {
          id: "about-debugging-sidebar-no-devices",
        },
        dom.aside(
          {
            className: "sidebar__label qa-sidebar-no-devices",
          },
          "No devices discovered"
        )
<<<<<<< HEAD
      )
    );
  }

  renderDevices() {
    const { networkRuntimes, usbRuntimes } = this.props;
||||||| merged common ancestors
      );
    }
=======
      )
    );
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // render a "no devices" messages when the lists are empty
    if (!networkRuntimes.length && !usbRuntimes.length) {
      return this.renderDevicesEmpty();
    }
    // render all devices otherwise
||||||| merged common ancestors
=======
  renderDevices() {
    const { networkRuntimes, usbRuntimes } = this.props;

    // render a "no devices" messages when the lists are empty
    if (!networkRuntimes.length && !usbRuntimes.length) {
      return this.renderDevicesEmpty();
    }
    // render all devices otherwise
>>>>>>> upstream-releases
    return [
      ...this.renderRuntimeItems(GLOBE_ICON, networkRuntimes),
      ...this.renderRuntimeItems(USB_ICON, usbRuntimes),
    ];
  }

<<<<<<< HEAD
  renderSidebarItems(icon, runtimes) {
    const { dispatch, selectedPage, selectedRuntime } = this.props;
||||||| merged common ancestors
  renderSidebarItems(icon, runtimes) {
    const { dispatch, selectedPage } = this.props;
=======
  renderRuntimeItems(icon, runtimes) {
    const { dispatch, selectedPage, selectedRuntimeId } = this.props;
>>>>>>> upstream-releases

    return runtimes.map(runtime => {
<<<<<<< HEAD
      const keyId = `${runtime.type}-${runtime.id}`;
      const runtimeHasDetails = !!runtime.runtimeDetails;
      const isSelected = selectedPage === PAGE_TYPES.RUNTIME &&
        runtime.id === selectedRuntime;
||||||| merged common ancestors
      const pageId = runtime.type + "-" + runtime.id;
      const runtimeHasConnection = !!runtime.connection;
=======
      const keyId = `${runtime.type}-${runtime.id}`;
      const runtimeHasDetails = !!runtime.runtimeDetails;
      const isSelected =
        selectedPage === PAGE_TYPES.RUNTIME && runtime.id === selectedRuntimeId;

      let name = runtime.name;
      if (runtime.type === RUNTIMES.USB && runtimeHasDetails) {
        // Update the name to be same to the runtime page.
        name = runtime.runtimeDetails.info.name;
      }
>>>>>>> upstream-releases

      return SidebarRuntimeItem({
        deviceName: runtime.extra.deviceName,
        dispatch,
        icon,
<<<<<<< HEAD
        key: keyId,
        isConnected: runtimeHasDetails,
        isSelected,
        name: runtime.name,
||||||| merged common ancestors
        isConnected: runtimeHasConnection,
        isSelected: selectedPage === pageId,
        key: pageId,
        name: runtime.name,
=======
        key: keyId,
        isConnected: runtimeHasDetails,
        isConnecting: runtime.isConnecting,
        isConnectionFailed: runtime.isConnectionFailed,
        isConnectionNotResponding: runtime.isConnectionNotResponding,
        isConnectionTimeout: runtime.isConnectionTimeout,
        isSelected,
        isUnavailable: runtime.isUnavailable,
        isUnplugged: runtime.isUnplugged,
        name,
>>>>>>> upstream-releases
        runtimeId: runtime.id,
      });
    });
  }

  renderFooter() {
    const HELP_ICON_SRC = "chrome://global/skin/icons/help.svg";
    const SUPPORT_URL =
      "https://developer.mozilla.org/docs/Tools/about:debugging";

    return dom.footer(
      {
        className: "sidebar__footer",
      },
      dom.ul(
        {},
        SidebarItem(
          {
            className: "sidebar-item--condensed",
            to: SUPPORT_URL,
          },
          dom.span(
            {
              className: "sidebar__footer__support-help",
            },
            Localized(
              {
                id: "about-debugging-sidebar-support-icon",
                attrs: {
                  alt: true,
                },
              },
              dom.img({
                className: "sidebar__footer__icon",
                src: HELP_ICON_SRC,
              })
            ),
            Localized(
              {
                id: "about-debugging-sidebar-support",
              },
              dom.span({}, "about-debugging-sidebar-support")
            )
          )
        )
      )
    );
  }

  render() {
<<<<<<< HEAD
    const { dispatch, selectedPage, selectedRuntime, isScanningUsb } = this.props;
||||||| merged common ancestors
    const { dispatch, selectedPage } = this.props;
=======
    const {
      dispatch,
      selectedPage,
      selectedRuntimeId,
      isScanningUsb,
    } = this.props;
>>>>>>> upstream-releases

    return dom.aside(
      {
        className: `sidebar ${this.props.className || ""}`,
      },
      dom.ul(
        {},
        Localized(
          { id: "about-debugging-sidebar-setup", attrs: { name: true } },
          SidebarFixedItem({
<<<<<<< HEAD
            icon: FIREFOX_ICON,
            isSelected: PAGE_TYPES.RUNTIME === selectedPage &&
              selectedRuntime === RUNTIMES.THIS_FIREFOX,
            key: RUNTIMES.THIS_FIREFOX,
            name: "This Firefox",
            to: `/runtime/${RUNTIMES.THIS_FIREFOX}`,
||||||| merged common ancestors
            id: PAGES.THIS_FIREFOX,
            dispatch,
            icon: FIREFOX_ICON,
            isSelected: PAGES.THIS_FIREFOX === selectedPage,
            name: "This Firefox",
            runtimeId: RUNTIMES.THIS_FIREFOX,
=======
            dispatch,
            icon: CONNECT_ICON,
            isSelected: PAGE_TYPES.CONNECT === selectedPage,
            key: PAGE_TYPES.CONNECT,
            name: "Setup",
            to: "/setup",
>>>>>>> upstream-releases
          })
        ),
        Localized(
          { id: "about-debugging-sidebar-this-firefox", attrs: { name: true } },
          SidebarFixedItem({
<<<<<<< HEAD
            dispatch,
            icon: CONNECT_ICON,
            isSelected: PAGE_TYPES.CONNECT === selectedPage,
            key: PAGE_TYPES.CONNECT,
            name: "Connect",
            to: "/connect",
          })
        ),
        SidebarItem(
          {
            isSelected: false,
            key: "separator-0",
          },
          dom.hr({ className: "separator" }),
          this.renderAdbAddonStatus(),
        ),
        this.renderDevices(),
        SidebarItem(
          {
            className: "sidebar-item--breathe sidebar__refresh-usb",
            isSelected: false,
            key: "refresh-devices",
          },
          RefreshDevicesButton({
            dispatch,
            isScanning: isScanningUsb,
||||||| merged common ancestors
            id: PAGES.CONNECT,
            dispatch,
            icon: CONNECT_ICON,
            isSelected: PAGES.CONNECT === selectedPage,
            name: "Connect",
=======
            icon: FIREFOX_ICON,
            isSelected:
              PAGE_TYPES.RUNTIME === selectedPage &&
              selectedRuntimeId === RUNTIMES.THIS_FIREFOX,
            key: RUNTIMES.THIS_FIREFOX,
            name: "This Firefox",
            to: `/runtime/${RUNTIMES.THIS_FIREFOX}`,
>>>>>>> upstream-releases
          })
        ),
<<<<<<< HEAD
      )
||||||| merged common ancestors
        dom.hr(),
        this.renderAdbAddonStatus(),
        this.renderDevices()
      )
=======
        SidebarItem(
          {
            className: "sidebar__adb-status",
          },
          dom.hr({ className: "separator separator--breathe" }),
          this.renderAdbStatus()
        ),
        this.renderDevices(),
        SidebarItem(
          {
            className: "sidebar-item--breathe sidebar__refresh-usb",
            key: "refresh-devices",
          },
          RefreshDevicesButton({
            dispatch,
            isScanning: isScanningUsb,
          })
        )
      ),
      this.renderFooter()
>>>>>>> upstream-releases
    );
  }
}

module.exports = Sidebar;
