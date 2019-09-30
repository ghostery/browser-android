/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GDKDND_WRAPPER_H
#define GDKDND_WRAPPER_H

#define gdk_drag_context_get_actions gdk_drag_context_get_actions_
#define gdk_drag_context_list_targets gdk_drag_context_list_targets_
#define gdk_drag_context_get_dest_window gdk_drag_context_get_dest_window_
#include_next <gdk/gdkdnd.h>
#undef gdk_drag_context_get_actions
#undef gdk_drag_context_list_targets
#undef gdk_drag_context_get_dest_window

<<<<<<< HEAD
static inline GdkDragAction gdk_drag_context_get_actions(
    GdkDragContext *context) {
||||||| merged common ancestors
static inline GdkDragAction
gdk_drag_context_get_actions(GdkDragContext *context)
{
=======
static inline GdkDragAction gdk_drag_context_get_actions(
    GdkDragContext* context) {
>>>>>>> upstream-releases
  return context->actions;
}

<<<<<<< HEAD
static inline GList *gdk_drag_context_list_targets(GdkDragContext *context) {
||||||| merged common ancestors
static inline GList *
gdk_drag_context_list_targets(GdkDragContext *context)
{
=======
static inline GList* gdk_drag_context_list_targets(GdkDragContext* context) {
>>>>>>> upstream-releases
  return context->targets;
}

<<<<<<< HEAD
static inline GdkWindow *gdk_drag_context_get_dest_window(
    GdkDragContext *context) {
||||||| merged common ancestors
static inline GdkWindow *
gdk_drag_context_get_dest_window(GdkDragContext *context)
{
=======
static inline GdkWindow* gdk_drag_context_get_dest_window(
    GdkDragContext* context) {
>>>>>>> upstream-releases
  return context->dest_window;
}
#endif /* GDKDND_WRAPPER_H */
