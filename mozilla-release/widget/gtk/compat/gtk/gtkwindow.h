/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GTKWINDOW_WRAPPER_H
#define GTKWINDOW_WRAPPER_H

#define gtk_window_group_get_current_grab gtk_window_group_get_current_grab_
#define gtk_window_get_window_type gtk_window_get_window_type_
#include_next <gtk/gtkwindow.h>
#undef gtk_window_group_get_current_grab
#undef gtk_window_get_window_type

<<<<<<< HEAD
static inline GtkWidget *gtk_window_group_get_current_grab(
    GtkWindowGroup *window_group) {
  if (!window_group->grabs) return NULL;
||||||| merged common ancestors
static inline GtkWidget *
gtk_window_group_get_current_grab(GtkWindowGroup *window_group)
{
  if (!window_group->grabs)
    return NULL;
=======
static inline GtkWidget* gtk_window_group_get_current_grab(
    GtkWindowGroup* window_group) {
  if (!window_group->grabs) return NULL;
>>>>>>> upstream-releases

  return GTK_WIDGET(window_group->grabs->data);
}

<<<<<<< HEAD
static inline GtkWindowType gtk_window_get_window_type(GtkWindow *window) {
||||||| merged common ancestors
static inline GtkWindowType
gtk_window_get_window_type(GtkWindow *window)
{
=======
static inline GtkWindowType gtk_window_get_window_type(GtkWindow* window) {
>>>>>>> upstream-releases
  gint type;
  g_object_get(window, "type", &type, (void *)NULL);
  return (GtkWindowType)type;
}
#endif /* GTKWINDOW_WRAPPER_H */
