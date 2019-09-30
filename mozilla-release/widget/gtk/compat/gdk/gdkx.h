/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GDKX_WRAPPER_H
#define GDKX_WRAPPER_H

#include <gtk/gtkversion.h>

#define gdk_x11_window_foreign_new_for_display \
  gdk_x11_window_foreign_new_for_display_
#define gdk_x11_window_lookup_for_display gdk_x11_window_lookup_for_display_
#define gdk_x11_window_get_xid gdk_x11_window_get_xid_
<<<<<<< HEAD
#if !GTK_CHECK_VERSION(2, 24, 0)
#define gdk_x11_set_sm_client_id gdk_x11_set_sm_client_id_
||||||| merged common ancestors
#if !GTK_CHECK_VERSION(2,24,0)
#define gdk_x11_set_sm_client_id gdk_x11_set_sm_client_id_
=======
#if !GTK_CHECK_VERSION(2, 24, 0)
#  define gdk_x11_set_sm_client_id gdk_x11_set_sm_client_id_
>>>>>>> upstream-releases
#endif
#include_next <gdk/gdkx.h>
#undef gdk_x11_window_foreign_new_for_display
#undef gdk_x11_window_lookup_for_display
#undef gdk_x11_window_get_xid

<<<<<<< HEAD
static inline GdkWindow *gdk_x11_window_foreign_new_for_display(
    GdkDisplay *display, Window window) {
||||||| merged common ancestors
static inline GdkWindow *
gdk_x11_window_foreign_new_for_display(GdkDisplay *display, Window window)
{
=======
static inline GdkWindow* gdk_x11_window_foreign_new_for_display(
    GdkDisplay* display, Window window) {
>>>>>>> upstream-releases
  return gdk_window_foreign_new_for_display(display, window);
}

<<<<<<< HEAD
static inline GdkWindow *gdk_x11_window_lookup_for_display(GdkDisplay *display,
                                                           Window window) {
||||||| merged common ancestors
static inline GdkWindow *
gdk_x11_window_lookup_for_display(GdkDisplay *display, Window window)
{
=======
static inline GdkWindow* gdk_x11_window_lookup_for_display(GdkDisplay* display,
                                                           Window window) {
>>>>>>> upstream-releases
  return gdk_window_lookup_for_display(display, window);
}

<<<<<<< HEAD
static inline Window gdk_x11_window_get_xid(GdkWindow *window) {
  return (GDK_WINDOW_XWINDOW(window));
||||||| merged common ancestors
static inline Window
gdk_x11_window_get_xid(GdkWindow *window)
{
  return(GDK_WINDOW_XWINDOW(window));
=======
static inline Window gdk_x11_window_get_xid(GdkWindow* window) {
  return (GDK_WINDOW_XWINDOW(window));
>>>>>>> upstream-releases
}

#ifndef GDK_IS_X11_DISPLAY
<<<<<<< HEAD
#define GDK_IS_X11_DISPLAY(a) (true)
||||||| merged common ancestors
#define GDK_IS_X11_DISPLAY(a)   (true)
=======
#  define GDK_IS_X11_DISPLAY(a) (true)
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
#if !GTK_CHECK_VERSION(2, 24, 0)
#undef gdk_x11_set_sm_client_id
static inline void gdk_x11_set_sm_client_id(const gchar *sm_client_id) {
||||||| merged common ancestors
#if !GTK_CHECK_VERSION(2,24,0)
#undef gdk_x11_set_sm_client_id
static inline void
gdk_x11_set_sm_client_id (const gchar *sm_client_id)
{
=======
#if !GTK_CHECK_VERSION(2, 24, 0)
#  undef gdk_x11_set_sm_client_id
static inline void gdk_x11_set_sm_client_id(const gchar* sm_client_id) {
>>>>>>> upstream-releases
  gdk_set_sm_client_id(sm_client_id);
}
#endif
#endif /* GDKX_WRAPPER_H */
