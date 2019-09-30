# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

from __future__ import absolute_import

import types

from marionette_driver import errors

from marionette_harness import MarionetteTestCase, WindowManagerMixin


class TestWindowHandles(WindowManagerMixin, MarionetteTestCase):

    def setUp(self):
        super(TestWindowHandles, self).setUp()

        self.xul_dialog = "chrome://marionette/content/test_dialog.xul"

        self.marionette.set_context("chrome")

    def tearDown(self):
        self.close_all_windows()
        self.close_all_tabs()

        super(TestWindowHandles, self).tearDown()

    def assert_window_handles(self):
        try:
            self.assertIsInstance(self.marionette.current_chrome_window_handle, types.StringTypes)
            self.assertIsInstance(self.marionette.current_window_handle, types.StringTypes)
        except errors.NoSuchWindowException:
            pass

        for handle in self.marionette.chrome_window_handles:
            self.assertIsInstance(handle, types.StringTypes)

        for handle in self.marionette.window_handles:
            self.assertIsInstance(handle, types.StringTypes)

    def test_chrome_window_handles_with_scopes(self):
<<<<<<< HEAD
        # Open a browser and a non-browser (about window) chrome window
        self.open_window(
            trigger=lambda: self.marionette.execute_script("OpenBrowserWindow();"))
||||||| merged common ancestors
        # Open a browser and a non-browser (about window) chrome window
        self.open_window(
            trigger=lambda: self.marionette.execute_script("window.open();"))
=======
        new_browser = self.open_window()
>>>>>>> upstream-releases
        self.assert_window_handles()
        self.assertEqual(len(self.marionette.chrome_window_handles), len(self.start_windows) + 1)
        self.assertIn(new_browser, self.marionette.chrome_window_handles)
        self.assertEqual(self.marionette.current_chrome_window_handle, self.start_window)

        new_dialog = self.open_chrome_window(self.xul_dialog)
        self.assert_window_handles()
        self.assertEqual(len(self.marionette.chrome_window_handles), len(self.start_windows) + 2)
        self.assertIn(new_dialog, self.marionette.chrome_window_handles)
        self.assertEqual(self.marionette.current_chrome_window_handle, self.start_window)

        chrome_window_handles_in_chrome_scope = self.marionette.chrome_window_handles
        window_handles_in_chrome_scope = self.marionette.window_handles

        with self.marionette.using_context("content"):
            self.assertEqual(self.marionette.chrome_window_handles,
                             chrome_window_handles_in_chrome_scope)
            self.assertEqual(self.marionette.window_handles,
                             window_handles_in_chrome_scope)

<<<<<<< HEAD
    def test_chrome_window_handles_after_opening_new_dialog(self):
        xul_dialog = "chrome://marionette/content/test_dialog.xul"
        new_win = self.open_chrome_window(xul_dialog)
||||||| merged common ancestors
    def test_chrome_window_handles_after_opening_new_dialog(self):
        xul_dialog = "chrome://marionette/content/test_dialog.xul"

        def open_via_js():
            self.marionette.execute_script("""
                window.openDialog(arguments[0]);
            """, script_args=(xul_dialog,))

        new_win = self.open_window(trigger=open_via_js)
=======
    def test_chrome_window_handles_after_opening_new_chrome_window(self):
        new_window = self.open_chrome_window(self.xul_dialog)
>>>>>>> upstream-releases
        self.assert_window_handles()
        self.assertEqual(len(self.marionette.chrome_window_handles), len(self.start_windows) + 1)
        self.assertIn(new_window, self.marionette.chrome_window_handles)
        self.assertEqual(self.marionette.current_chrome_window_handle, self.start_window)

        # Check that the new chrome window has the correct URL loaded
        self.marionette.switch_to_window(new_window)
        self.assert_window_handles()
        self.assertEqual(self.marionette.current_chrome_window_handle, new_window)
        self.assertEqual(self.marionette.get_url(), self.xul_dialog)

        # Close the chrome window, and carry on in our original window.
        self.marionette.close_chrome_window()
        self.assert_window_handles()
        self.assertEqual(len(self.marionette.chrome_window_handles), len(self.start_windows))
        self.assertNotIn(new_window, self.marionette.chrome_window_handles)

        self.marionette.switch_to_window(self.start_window)
        self.assert_window_handles()
        self.assertEqual(self.marionette.current_chrome_window_handle, self.start_window)

    def test_chrome_window_handles_after_opening_new_window(self):
        new_window = self.open_window()
        self.assert_window_handles()
        self.assertEqual(len(self.marionette.chrome_window_handles), len(self.start_windows) + 1)
        self.assertIn(new_window, self.marionette.chrome_window_handles)
        self.assertEqual(self.marionette.current_chrome_window_handle, self.start_window)

        self.marionette.switch_to_window(new_window)
        self.assert_window_handles()
        self.assertEqual(self.marionette.current_chrome_window_handle, new_window)

        # Close the opened window and carry on in our original window.
        self.marionette.close()
        self.assert_window_handles()
        self.assertEqual(len(self.marionette.chrome_window_handles), len(self.start_windows))
        self.assertNotIn(new_window, self.marionette.chrome_window_handles)

        self.marionette.switch_to_window(self.start_window)
        self.assert_window_handles()
        self.assertEqual(self.marionette.current_chrome_window_handle, self.start_window)

    def test_window_handles_after_opening_new_tab(self):
        with self.marionette.using_context("content"):
            new_tab = self.open_tab()
        self.assert_window_handles()
        self.assertEqual(len(self.marionette.window_handles), len(self.start_tabs) + 1)
        self.assertIn(new_tab, self.marionette.window_handles)
        self.assertEqual(self.marionette.current_window_handle, self.start_tab)

        self.marionette.switch_to_window(new_tab)
        self.assert_window_handles()
        self.assertEqual(self.marionette.current_window_handle, new_tab)

        self.marionette.switch_to_window(self.start_tab)
        self.assert_window_handles()
        self.assertEqual(self.marionette.current_window_handle, self.start_tab)

        self.marionette.switch_to_window(new_tab)
        self.marionette.close()
        self.assert_window_handles()
        self.assertEqual(len(self.marionette.window_handles), len(self.start_tabs))
        self.assertNotIn(new_tab, self.marionette.window_handles)

        self.marionette.switch_to_window(self.start_tab)
        self.assert_window_handles()
        self.assertEqual(self.marionette.current_window_handle, self.start_tab)

<<<<<<< HEAD
    def test_window_handles_after_opening_new_dialog(self):
        xul_dialog = "chrome://marionette/content/test_dialog.xul"
        new_win = self.open_chrome_window(xul_dialog)
||||||| merged common ancestors
    def test_window_handles_after_opening_new_dialog(self):
        xul_dialog = "chrome://marionette/content/test_dialog.xul"

        def open_via_js():
            self.marionette.execute_script("""
                window.openDialog(arguments[0]);
            """, script_args=(xul_dialog,))

        new_win = self.open_window(trigger=open_via_js)
=======
    def test_window_handles_after_opening_new_foreground_tab(self):
        with self.marionette.using_context("content"):
            new_tab = self.open_tab(focus=True)
>>>>>>> upstream-releases
        self.assert_window_handles()
        self.assertEqual(len(self.marionette.window_handles), len(self.start_tabs) + 1)
        self.assertIn(new_tab, self.marionette.window_handles)
        self.assertEqual(self.marionette.current_window_handle, self.start_tab)

        # We still have the default tab set as our window handle. This
        # get_url command should be sent immediately, and not be forever-queued.
        with self.marionette.using_context("content"):
            self.marionette.get_url()

        self.marionette.switch_to_window(new_tab)
        self.assert_window_handles()
        self.assertEqual(self.marionette.current_window_handle, new_tab)

        self.marionette.close()
        self.assert_window_handles()
        self.assertEqual(len(self.marionette.window_handles), len(self.start_tabs))
        self.assertNotIn(new_tab, self.marionette.window_handles)

        self.marionette.switch_to_window(self.start_tab)
        self.assert_window_handles()
        self.assertEqual(self.marionette.current_window_handle, self.start_tab)

    def test_window_handles_after_opening_new_chrome_window(self):
        new_window = self.open_chrome_window(self.xul_dialog)
        self.assert_window_handles()
        self.assertEqual(len(self.marionette.window_handles), len(self.start_tabs))
        self.assertNotIn(new_window, self.marionette.window_handles)
        self.assertEqual(self.marionette.current_window_handle, self.start_tab)

        self.marionette.switch_to_window(new_window)
        self.assert_window_handles()
        self.assertEqual(self.marionette.get_url(), self.xul_dialog)

        # Check that the opened dialog is not accessible via window handles
        with self.assertRaises(errors.NoSuchWindowException):
            self.marionette.current_window_handle
        with self.assertRaises(errors.NoSuchWindowException):
            self.marionette.close()

        # Close the dialog and carry on in our original tab.
        self.marionette.close_chrome_window()
        self.assert_window_handles()
        self.assertEqual(len(self.marionette.window_handles), len(self.start_tabs))

        self.marionette.switch_to_window(self.start_tab)
        self.assert_window_handles()
        self.assertEqual(self.marionette.current_window_handle, self.start_tab)

    def test_window_handles_after_closing_original_tab(self):
        with self.marionette.using_context("content"):
            new_tab = self.open_tab()
        self.assert_window_handles()
        self.assertEqual(len(self.marionette.window_handles), len(self.start_tabs) + 1)
        self.assertIn(new_tab, self.marionette.window_handles)
        self.assertEqual(self.marionette.current_window_handle, self.start_tab)

        self.marionette.close()
        self.assert_window_handles()
        self.assertEqual(len(self.marionette.window_handles), len(self.start_tabs))
        self.assertIn(new_tab, self.marionette.window_handles)

        self.marionette.switch_to_window(new_tab)
        self.assert_window_handles()
        self.assertEqual(self.marionette.current_window_handle, new_tab)

    def test_window_handles_after_closing_last_window(self):
        self.close_all_windows()
        self.assertEqual(self.marionette.close_chrome_window(), [])
