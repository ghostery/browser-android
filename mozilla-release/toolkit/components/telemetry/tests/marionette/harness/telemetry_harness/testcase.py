# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

import contextlib
import os
<<<<<<< HEAD
import re
import simplejson as json
||||||| merged common ancestors
import simplejson as json
=======
import re
import textwrap
>>>>>>> upstream-releases
import time

<<<<<<< HEAD
from firefox_puppeteer import PuppeteerMixin
||||||| merged common ancestors
from multiprocessing import Process

from firefox_puppeteer import PuppeteerMixin
=======
>>>>>>> upstream-releases
from marionette_driver.addons import Addons
from marionette_driver.errors import MarionetteException
from marionette_driver.wait import Wait
from marionette_driver import By, keys
from marionette_harness import MarionetteTestCase
from marionette_harness.runner.mixins.window_manager import WindowManagerMixin

from ping_server import PingServer


<<<<<<< HEAD
CANARY_CLIENT_ID = "c0ffeec0-ffee-c0ff-eec0-ffeec0ffeec0"
UUID_PATTERN = re.compile(
    r"^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$"
)


class TelemetryTestCase(PuppeteerMixin, MarionetteTestCase):
||||||| merged common ancestors
class TelemetryTestCase(PuppeteerMixin, MarionetteTestCase):
=======
CANARY_CLIENT_ID = "c0ffeec0-ffee-c0ff-eec0-ffeec0ffeec0"
UUID_PATTERN = re.compile(
    r"^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$"
)
>>>>>>> upstream-releases


class TelemetryTestCase(WindowManagerMixin, MarionetteTestCase):
    def __init__(self, *args, **kwargs):
        """Initialize the test case and create a ping server."""
        super(TelemetryTestCase, self).__init__(*args, **kwargs)

        self.ping_server = PingServer(
            self.testvars["server_root"], self.testvars["server_url"]
        )

    def setUp(self, *args, **kwargs):
        """Set up the test case and start the ping server."""
        super(TelemetryTestCase, self).setUp(*args, **kwargs)

        # Store IDs of addons installed via self.install_addon()
        self.addon_ids = []

        with self.marionette.using_context(self.marionette.CONTEXT_CONTENT):
            self.marionette.navigate("about:about")

        self.ping_server.start()

    @contextlib.contextmanager
    def new_tab(self):
        """Perform operations in a new tab and then close the new tab."""

        with self.marionette.using_context(self.marionette.CONTEXT_CHROME):
            start_tab = self.marionette.current_window_handle
            new_tab = self.open_tab(focus=True)
            self.marionette.switch_to_window(new_tab)

            yield

            self.marionette.close()
            self.marionette.switch_to_window(start_tab)

    def search(self, text):
        """Perform a search via the browser's URL bar."""

        with self.marionette.using_context(self.marionette.CONTEXT_CHROME):
            urlbar = self.marionette.find_element(By.ID, "urlbar")
            urlbar.send_keys(keys.Keys.DELETE)
            urlbar.send_keys(text + keys.Keys.ENTER)

        # Wait for 0.1 seconds before proceeding to decrease the chance
        # of Firefox being shut down before Telemetry is recorded
        time.sleep(0.1)

    def search_in_new_tab(self, text):
        """Open a new tab and perform a search via the browser's URL bar,
        then close the new tab."""

        with self.new_tab():
            self.search(text)

    def assertIsValidUUID(self, value):
        """Check if the given UUID is valid."""

<<<<<<< HEAD
        telemetry_prefs = {
            'toolkit.telemetry.server': self.ping_server_url,
            'toolkit.telemetry.initDelay': 1,
            'toolkit.telemetry.minSubsessionLength': 0,
            'datareporting.healthreport.uploadEnabled': True,
            'datareporting.policy.dataSubmissionEnabled': True,
            'datareporting.policy.dataSubmissionPolicyBypassNotification': True,
            'toolkit.telemetry.log.level': 'Trace',
            'toolkit.telemetry.log.dump': True,
            'toolkit.telemetry.send.overrideOfficialCheck': True,
            'toolkit.telemetry.testing.disableFuzzingDelay': True,
        }
||||||| merged common ancestors
        telemetry_prefs = {
            'toolkit.telemetry.server': self.ping_server_url,
            'toolkit.telemetry.initDelay': 1,
            'toolkit.telemetry.minSubsessionLength': 0,
            'datareporting.healthreport.uploadEnabled': True,
            'datareporting.policy.dataSubmissionEnabled': True,
            'datareporting.policy.dataSubmissionPolicyBypassNotification': True,
            'toolkit.telemetry.log.level': 0,
            'toolkit.telemetry.log.dump': True,
            'toolkit.telemetry.send.overrideOfficialCheck': True
        }
=======
        self.assertIsNotNone(value)
        self.assertNotEqual(value, "")
>>>>>>> upstream-releases

        # Check for client ID that is used when Telemetry upload is disabled
        self.assertNotEqual(
            value, CANARY_CLIENT_ID, msg="UUID is CANARY CLIENT ID"
        )

        self.assertIsNotNone(
            re.match(UUID_PATTERN, value),
            msg="UUID does not match regular expression",
        )

<<<<<<< HEAD
    def assertIsValidUUID(self, value):
        """Check if the given UUID is valid."""
        self.assertIsNotNone(value)
        self.assertNotEqual(value, "")

        # Check for client ID that is used when Telemetry upload is disabled
        self.assertNotEqual(
            value, CANARY_CLIENT_ID, msg="UUID is CANARY CLIENT ID"
        )

        self.assertIsNotNone(
            re.match(UUID_PATTERN, value),
            msg="UUID does not match regular expression",
        )

    def wait_for_pings(self, action_func, ping_filter, count):
||||||| merged common ancestors
    def wait_for_pings(self, action_func, ping_filter_func, count):
=======
    def wait_for_pings(self, action_func, ping_filter, count):
>>>>>>> upstream-releases
        """Call the given action and wait for pings to come in and return
        the `count` number of pings, that match the given filter.
        """
        # Keep track of the current number of pings
        current_num_pings = len(self.ping_server.pings)

        # New list to store new pings that satisfy the filter
        filtered_pings = []

        def wait_func(*args, **kwargs):
            # Ignore existing pings in self.ping_server.pings
            new_pings = self.ping_server.pings[current_num_pings:]

            # Filter pings to make sure we wait for the correct ping type
            filtered_pings[:] = [p for p in new_pings if ping_filter(p)]

            return len(filtered_pings) >= count

        self.logger.info(
            "wait_for_pings running action '{action}'.".format(
                action=action_func.__name__
            )
        )

        # Call given action and wait for a ping
        action_func()

        try:
            Wait(self.marionette, 60).until(wait_func)
        except Exception as e:
            self.fail("Error waiting for ping: {}".format(e.message))

        return filtered_pings[:count]

    def wait_for_ping(self, action_func, ping_filter):
        """Call wait_for_pings() with the given action_func and ping_filter and
        return the first result.
        """
        [ping] = self.wait_for_pings(action_func, ping_filter, 1)
        return ping

    def restart_browser(self):
        """Restarts browser while maintaining the same profile."""
        return self.marionette.restart(clean=False, in_app=True)

    def install_addon(self):
<<<<<<< HEAD
        """Install a minimal addon."""

        resources_dir = os.path.join(
            os.path.dirname(os.path.dirname(__file__)), "resources"
        )

        addon_path = os.path.abspath(os.path.join(resources_dir, "helloworld"))

||||||| merged common ancestors
        trigger = Process(target=self._install_addon)
        trigger.start()

    def _install_addon(self):
        # The addon that gets installed here is the easyscreenshot addon taken from AMO.
        # It has high compatibility with firefox and doesn't cause any adverse side affects that
        # could affect our tests like tabs opening, etc.
        # Developed by: MozillaOnline
        # Addon URL: https://addons.mozilla.org/en-US/firefox/addon/easyscreenshot/
=======
        """Install a minimal addon and add its ID to self.addon_ids."""

        resources_dir = os.path.join(os.path.dirname(__file__), "resources")
        addon_path = os.path.abspath(os.path.join(resources_dir, "helloworld"))

>>>>>>> upstream-releases
        try:
<<<<<<< HEAD
||||||| merged common ancestors
            # TODO: Replace Resources_dir with default directory
            addon_path = os.path.join('resources_dir', 'easyscreenshot.xpi')
=======
            # Ensure the Environment has init'd so the installed addon
            # triggers an "environment-change" ping.
            script = """\
            let [resolve] = arguments;
            Cu.import("resource://gre/modules/TelemetryEnvironment.jsm");
            TelemetryEnvironment.onInitialized().then(resolve);
            """

            with self.marionette.using_context(self.marionette.CONTEXT_CHROME):
                self.marionette.execute_async_script(textwrap.dedent(script))

>>>>>>> upstream-releases
            addons = Addons(self.marionette)
<<<<<<< HEAD
            addons.install(addon_path, temp=True)
||||||| merged common ancestors
            addons.install(addon_path)
=======
            addon_id = addons.install(addon_path, temp=True)
>>>>>>> upstream-releases
        except MarionetteException as e:
<<<<<<< HEAD
            self.fail(
                "{} - Error installing addon: {} - ".format(e.cause, e.message)
            )
||||||| merged common ancestors
            self.fail('{} - Error installing addon: {} - '.format(e.cause, e.message))
=======
            self.fail(
                "{} - Error installing addon: {} - ".format(e.cause, e.message)
            )
        else:
            self.addon_ids.append(addon_id)
>>>>>>> upstream-releases

    @property
    def client_id(self):
        """Return the ID of the current client."""
        with self.marionette.using_context(self.marionette.CONTEXT_CHROME):
            return self.marionette.execute_script(
                'Cu.import("resource://gre/modules/ClientID.jsm");'
                "return ClientID.getCachedClientID();"
            )

    @property
    def subsession_id(self):
        """Return the ID of the current subsession."""
        with self.marionette.using_context(self.marionette.CONTEXT_CHROME):
            ping_data = self.marionette.execute_script(
                'Cu.import("resource://gre/modules/TelemetryController.jsm");'
                "return TelemetryController.getCurrentPingData(true);"
            )
            return ping_data[u"payload"][u"info"][u"subsessionId"]

    def tearDown(self, *args, **kwargs):
        """Stop the ping server and tear down the testcase."""
        super(TelemetryTestCase, self).tearDown()
        self.ping_server.stop()
        self.marionette.quit(clean=True)
