/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:expandtab:shiftwidth=4:tabstop=4:
 */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozcontainer.h"
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
<<<<<<< HEAD
#ifdef MOZ_WAYLAND
#include "nsWaylandDisplay.h"
#include <wayland-egl.h>
||||||| merged common ancestors
#include <gdk/gdkwayland.h>
#include <wayland-egl.h>
=======
#ifdef MOZ_WAYLAND
#  include "nsWaylandDisplay.h"
#  include <wayland-egl.h>
>>>>>>> upstream-releases
#endif
#include <stdio.h>
#include <dlfcn.h>

#ifdef ACCESSIBILITY
#  include <atk/atk.h>
#  include "maiRedundantObjectFactory.h"
#endif

#undef LOG
#ifdef MOZ_LOGGING

#  include "mozilla/Logging.h"
#  include "nsTArray.h"
#  include "Units.h"
extern mozilla::LazyLogModule gWidgetLog;
extern mozilla::LazyLogModule gWidgetWaylandLog;
#  define LOG(args) MOZ_LOG(gWidgetLog, mozilla::LogLevel::Debug, args)
#  define LOGWAYLAND(args) \
    MOZ_LOG(gWidgetWaylandLog, mozilla::LogLevel::Debug, args)
#else
#  define LOG(args)
#  define LOGWAYLAND(args)
#endif /* MOZ_LOGGING */

#ifdef MOZ_WAYLAND
using namespace mozilla;
using namespace mozilla::widget;
#endif

#ifdef MOZ_WAYLAND
using namespace mozilla;
using namespace mozilla::widget;
#endif

/* init methods */
<<<<<<< HEAD
static void moz_container_class_init(MozContainerClass *klass);
static void moz_container_init(MozContainer *container);
||||||| merged common ancestors
static void moz_container_class_init          (MozContainerClass *klass);
static void moz_container_init                (MozContainer      *container);
=======
static void moz_container_class_init(MozContainerClass* klass);
static void moz_container_init(MozContainer* container);
>>>>>>> upstream-releases

/* widget class methods */
<<<<<<< HEAD
static void moz_container_map(GtkWidget *widget);
#if defined(MOZ_WAYLAND)
static gboolean moz_container_map_wayland(GtkWidget *widget, GdkEventAny *event);
#endif
static void moz_container_unmap(GtkWidget *widget);
static void moz_container_realize(GtkWidget *widget);
static void moz_container_size_allocate(GtkWidget *widget,
                                        GtkAllocation *allocation);
||||||| merged common ancestors
static void moz_container_map                 (GtkWidget         *widget);
static void moz_container_unmap               (GtkWidget         *widget);
static void moz_container_realize             (GtkWidget         *widget);
static void moz_container_size_allocate       (GtkWidget         *widget,
                                               GtkAllocation     *allocation);
=======
static void moz_container_map(GtkWidget* widget);
#if defined(MOZ_WAYLAND)
static gboolean moz_container_map_wayland(GtkWidget* widget,
                                          GdkEventAny* event);
#endif
static void moz_container_unmap(GtkWidget* widget);
static void moz_container_realize(GtkWidget* widget);
static void moz_container_size_allocate(GtkWidget* widget,
                                        GtkAllocation* allocation);
>>>>>>> upstream-releases

/* container class methods */
<<<<<<< HEAD
static void moz_container_remove(GtkContainer *container,
                                 GtkWidget *child_widget);
static void moz_container_forall(GtkContainer *container,
                                 gboolean include_internals,
                                 GtkCallback callback, gpointer callback_data);
static void moz_container_add(GtkContainer *container, GtkWidget *widget);
||||||| merged common ancestors
static void moz_container_remove      (GtkContainer      *container,
                                       GtkWidget         *child_widget);
static void moz_container_forall      (GtkContainer      *container,
                                       gboolean           include_internals,
                                       GtkCallback        callback,
                                       gpointer           callback_data);
static void moz_container_add         (GtkContainer      *container,
                                        GtkWidget        *widget);
=======
static void moz_container_remove(GtkContainer* container,
                                 GtkWidget* child_widget);
static void moz_container_forall(GtkContainer* container,
                                 gboolean include_internals,
                                 GtkCallback callback, gpointer callback_data);
static void moz_container_add(GtkContainer* container, GtkWidget* widget);
>>>>>>> upstream-releases

typedef struct _MozContainerChild MozContainerChild;

struct _MozContainerChild {
<<<<<<< HEAD
  GtkWidget *widget;
  gint x;
  gint y;
||||||| merged common ancestors
    GtkWidget *widget;
    gint x;
    gint y;
=======
  GtkWidget* widget;
  gint x;
  gint y;
>>>>>>> upstream-releases
};

<<<<<<< HEAD
static void moz_container_allocate_child(MozContainer *container,
                                         MozContainerChild *child);
static MozContainerChild *moz_container_get_child(MozContainer *container,
                                                  GtkWidget *child);
||||||| merged common ancestors
static void moz_container_allocate_child (MozContainer      *container,
                                          MozContainerChild *child);
static MozContainerChild *
moz_container_get_child (MozContainer *container, GtkWidget *child);
=======
static void moz_container_allocate_child(MozContainer* container,
                                         MozContainerChild* child);
static MozContainerChild* moz_container_get_child(MozContainer* container,
                                                  GtkWidget* child);
>>>>>>> upstream-releases

/* public methods */

GType moz_container_get_type(void) {
  static GType moz_container_type = 0;

  if (!moz_container_type) {
    static GTypeInfo moz_container_info = {
        sizeof(MozContainerClass),                /* class_size */
        NULL,                                     /* base_init */
        NULL,                                     /* base_finalize */
        (GClassInitFunc)moz_container_class_init, /* class_init */
        NULL,                                     /* class_destroy */
        NULL,                                     /* class_data */
        sizeof(MozContainer),                     /* instance_size */
        0,                                        /* n_preallocs */
        (GInstanceInitFunc)moz_container_init,    /* instance_init */
        NULL,                                     /* value_table */
    };

    moz_container_type =
        g_type_register_static(GTK_TYPE_CONTAINER, "MozContainer",
                               &moz_container_info, static_cast<GTypeFlags>(0));
#ifdef ACCESSIBILITY
    /* Set a factory to return accessible object with ROLE_REDUNDANT for
     * MozContainer, so that gail won't send focus notification for it */
    atk_registry_set_factory_type(atk_get_default_registry(),
                                  moz_container_type,
                                  mai_redundant_object_factory_get_type());
#endif
  }

  return moz_container_type;
}

<<<<<<< HEAD
GtkWidget *moz_container_new(void) {
  MozContainer *container;
||||||| merged common ancestors
GtkWidget *
moz_container_new (void)
{
    MozContainer *container;
=======
GtkWidget* moz_container_new(void) {
  MozContainer* container;
>>>>>>> upstream-releases

<<<<<<< HEAD
  container =
      static_cast<MozContainer *>(g_object_new(MOZ_CONTAINER_TYPE, nullptr));
||||||| merged common ancestors
    container = static_cast<MozContainer*>(g_object_new (MOZ_CONTAINER_TYPE, nullptr));
=======
  container =
      static_cast<MozContainer*>(g_object_new(MOZ_CONTAINER_TYPE, nullptr));
>>>>>>> upstream-releases

  return GTK_WIDGET(container);
}

<<<<<<< HEAD
void moz_container_put(MozContainer *container, GtkWidget *child_widget, gint x,
                       gint y) {
  MozContainerChild *child;
||||||| merged common ancestors
void
moz_container_put (MozContainer *container, GtkWidget *child_widget,
                   gint x, gint y)
{
    MozContainerChild *child;
=======
void moz_container_put(MozContainer* container, GtkWidget* child_widget, gint x,
                       gint y) {
  MozContainerChild* child;
>>>>>>> upstream-releases

  child = g_new(MozContainerChild, 1);

  child->widget = child_widget;
  child->x = x;
  child->y = y;

  /*  printf("moz_container_put %p %p %d %d\n", (void *)container,
      (void *)child_widget, x, y); */

  container->children = g_list_append(container->children, child);

  /* we assume that the caller of this function will have already set
     the parent GdkWindow because we can have many anonymous children. */
  gtk_widget_set_parent(child_widget, GTK_WIDGET(container));
}

/* static methods */

<<<<<<< HEAD
void moz_container_class_init(MozContainerClass *klass) {
  /*GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    GtkObjectClass *object_class = GTK_OBJECT_CLASS (klass); */
  GtkContainerClass *container_class = GTK_CONTAINER_CLASS(klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
||||||| merged common ancestors
    new_allocation.x = x;
    new_allocation.y = y;
    new_allocation.width = width;
    new_allocation.height = height;
=======
void moz_container_class_init(MozContainerClass* klass) {
  /*GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    GtkObjectClass *object_class = GTK_OBJECT_CLASS (klass); */
  GtkContainerClass* container_class = GTK_CONTAINER_CLASS(klass);
  GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(klass);
>>>>>>> upstream-releases

<<<<<<< HEAD
  widget_class->map = moz_container_map;
#if defined(MOZ_WAYLAND)
    if (!GDK_IS_X11_DISPLAY(gdk_display_get_default())) {
      widget_class->map_event = moz_container_map_wayland;
    }
#endif
  widget_class->unmap = moz_container_unmap;
  widget_class->realize = moz_container_realize;
  widget_class->size_allocate = moz_container_size_allocate;
||||||| merged common ancestors
    /* printf("moz_container_move %p %p will allocate to %d %d %d %d\n",
       (void *)container, (void *)child_widget,
       new_allocation.x, new_allocation.y,
       new_allocation.width, new_allocation.height); */
=======
  widget_class->map = moz_container_map;
#if defined(MOZ_WAYLAND)
  if (!GDK_IS_X11_DISPLAY(gdk_display_get_default())) {
    widget_class->map_event = moz_container_map_wayland;
  }
#endif
  widget_class->unmap = moz_container_unmap;
  widget_class->realize = moz_container_realize;
  widget_class->size_allocate = moz_container_size_allocate;
>>>>>>> upstream-releases

  container_class->remove = moz_container_remove;
  container_class->forall = moz_container_forall;
  container_class->add = moz_container_add;
}

<<<<<<< HEAD
void moz_container_init(MozContainer *container) {
  gtk_widget_set_can_focus(GTK_WIDGET(container), TRUE);
  gtk_container_set_resize_mode(GTK_CONTAINER(container), GTK_RESIZE_IMMEDIATE);
  gtk_widget_set_redraw_on_allocate(GTK_WIDGET(container), FALSE);
||||||| merged common ancestors
/* static methods */
=======
void moz_container_init(MozContainer* container) {
  gtk_widget_set_can_focus(GTK_WIDGET(container), TRUE);
  gtk_container_set_resize_mode(GTK_CONTAINER(container), GTK_RESIZE_IMMEDIATE);
  gtk_widget_set_redraw_on_allocate(GTK_WIDGET(container), FALSE);
>>>>>>> upstream-releases

<<<<<<< HEAD
#if defined(MOZ_WAYLAND)
  container->surface = nullptr;
  container->subsurface = nullptr;
  container->eglwindow = nullptr;
  container->frame_callback_handler = nullptr;
  // We can draw to x11 window any time.
  container->ready_to_draw = GDK_IS_X11_DISPLAY(gdk_display_get_default());
  container->surface_needs_clear = true;
#endif
||||||| merged common ancestors
void
moz_container_class_init (MozContainerClass *klass)
{
    /*GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
      GtkObjectClass *object_class = GTK_OBJECT_CLASS (klass); */
    GtkContainerClass *container_class = GTK_CONTAINER_CLASS (klass);
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

    widget_class->map = moz_container_map;
    widget_class->unmap = moz_container_unmap;
    widget_class->realize = moz_container_realize;
    widget_class->size_allocate = moz_container_size_allocate;

    container_class->remove = moz_container_remove;
    container_class->forall = moz_container_forall;
    container_class->add = moz_container_add;
=======
#if defined(MOZ_WAYLAND)
  container->surface = nullptr;
  container->subsurface = nullptr;
  container->eglwindow = nullptr;
  container->frame_callback_handler = nullptr;
  container->frame_callback_handler_surface_id = -1;
  // We can draw to x11 window any time.
  container->ready_to_draw = GDK_IS_X11_DISPLAY(gdk_display_get_default());
  container->surface_needs_clear = true;
  container->inital_draw_cb = nullptr;
#endif

  LOG(("%s [%p]\n", __FUNCTION__, (void*)container));
>>>>>>> upstream-releases
}

#if defined(MOZ_WAYLAND)
<<<<<<< HEAD
static wl_surface *moz_container_get_gtk_container_surface(
    MozContainer *container) {
  static auto sGdkWaylandWindowGetWlSurface = (wl_surface * (*)(GdkWindow *))
      dlsym(RTLD_DEFAULT, "gdk_wayland_window_get_wl_surface");

  GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(container));
  return sGdkWaylandWindowGetWlSurface(window);
||||||| merged common ancestors
static void
registry_handle_global (void *data,
                        struct wl_registry *registry,
                        uint32_t name,
                        const char *interface,
                        uint32_t version)
{
    MozContainer *container = MOZ_CONTAINER(data);
    if(strcmp(interface, "wl_subcompositor") == 0) {
        container->subcompositor =
            static_cast<wl_subcompositor*>(wl_registry_bind(registry,
                                           name,
                                           &wl_subcompositor_interface,
                                           1));
    }
=======
void moz_container_set_initial_draw_callback(
    MozContainer* container, std::function<void(void)> inital_draw_cb) {
  container->inital_draw_cb = inital_draw_cb;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void frame_callback_handler(void *data, struct wl_callback *callback,
                                   uint32_t time) {
  MozContainer *container = MOZ_CONTAINER(data);
  g_clear_pointer(&container->frame_callback_handler, wl_callback_destroy);
  container->ready_to_draw = true;
||||||| merged common ancestors
static void
registry_handle_global_remove (void *data,
                               struct wl_registry *registry,
                               uint32_t name)
{
=======
static wl_surface* moz_container_get_gtk_container_surface(
    MozContainer* container) {
  static auto sGdkWaylandWindowGetWlSurface = (wl_surface * (*)(GdkWindow*))
      dlsym(RTLD_DEFAULT, "gdk_wayland_window_get_wl_surface");

  GdkWindow* window = gtk_widget_get_window(GTK_WIDGET(container));
  wl_surface* surface = sGdkWaylandWindowGetWlSurface(window);

  LOGWAYLAND(("%s [%p] wl_surface %p ID %d\n", __FUNCTION__, (void*)container,
              (void*)surface,
              surface ? wl_proxy_get_id((struct wl_proxy*)surface) : -1));

  return surface;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static const struct wl_callback_listener frame_listener = {
    frame_callback_handler};
||||||| merged common ancestors
static const struct wl_registry_listener registry_listener = {
    registry_handle_global,
    registry_handle_global_remove
};
#endif
=======
static void frame_callback_handler(void* data, struct wl_callback* callback,
                                   uint32_t time) {
  MozContainer* container = MOZ_CONTAINER(data);
>>>>>>> upstream-releases

<<<<<<< HEAD
static gboolean moz_container_map_wayland(GtkWidget *widget, GdkEventAny *event) {
  MozContainer* container = MOZ_CONTAINER(widget);
||||||| merged common ancestors
void
moz_container_init (MozContainer *container)
{
    gtk_widget_set_can_focus(GTK_WIDGET(container), TRUE);
    gtk_container_set_resize_mode(GTK_CONTAINER(container), GTK_RESIZE_IMMEDIATE);
    gtk_widget_set_redraw_on_allocate(GTK_WIDGET(container), FALSE);
=======
  LOGWAYLAND(
      ("%s [%p] frame_callback_handler %p ready_to_draw %d (set to true)"
       " inital_draw callback %d\n",
       __FUNCTION__, (void*)container, (void*)container->frame_callback_handler,
       container->ready_to_draw, container->inital_draw_cb ? 1 : 0));
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (container->ready_to_draw || container->frame_callback_handler) {
    return FALSE;
  }
||||||| merged common ancestors
#if defined(MOZ_WAYLAND)
    {
      container->subcompositor = nullptr;
      container->surface = nullptr;
      container->subsurface = nullptr;
      container->eglwindow = nullptr;
      container->parent_surface_committed = false;

      GdkDisplay *gdk_display = gtk_widget_get_display(GTK_WIDGET(container));
      if (GDK_IS_WAYLAND_DISPLAY (gdk_display)) {
          // Available as of GTK 3.8+
          static auto sGdkWaylandDisplayGetWlDisplay =
              (wl_display *(*)(GdkDisplay *))
              dlsym(RTLD_DEFAULT, "gdk_wayland_display_get_wl_display");

          wl_display* display = sGdkWaylandDisplayGetWlDisplay(gdk_display);
          wl_registry* registry = wl_display_get_registry(display);
          wl_registry_add_listener(registry, &registry_listener, container);
          wl_display_dispatch(display);
          wl_display_roundtrip(display);
        }
    }
#endif
}
=======
  g_clear_pointer(&container->frame_callback_handler, wl_callback_destroy);
  container->frame_callback_handler_surface_id = -1;

  if (!container->ready_to_draw && container->inital_draw_cb) {
    container->inital_draw_cb();
  }
  container->ready_to_draw = true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  wl_surface *gtk_container_surface =
      moz_container_get_gtk_container_surface(container);
||||||| merged common ancestors
#if defined(MOZ_WAYLAND)
static void
moz_container_commited_handler(GdkFrameClock *clock, MozContainer *container)
{
    container->parent_surface_committed = true;
    g_signal_handler_disconnect(clock,
                                container->parent_surface_committed_handler);
    container->parent_surface_committed_handler = 0;
}
=======
static const struct wl_callback_listener frame_listener = {
    frame_callback_handler};

static void moz_container_request_parent_frame_callback(
    MozContainer* container) {
  wl_surface* gtk_container_surface =
      moz_container_get_gtk_container_surface(container);
  int gtk_container_surface_id =
      gtk_container_surface
          ? wl_proxy_get_id((struct wl_proxy*)gtk_container_surface)
          : -1;

  LOGWAYLAND(
      ("%s [%p] frame_callback_handler %p "
       "frame_callback_handler_surface_id %d\n",
       __FUNCTION__, (void*)container, container->frame_callback_handler,
       container->frame_callback_handler_surface_id));

  if (container->frame_callback_handler &&
      container->frame_callback_handler_surface_id ==
          gtk_container_surface_id) {
    return;
  }

  // If there's pending frame callback, delete it.
  if (container->frame_callback_handler) {
    g_clear_pointer(&container->frame_callback_handler, wl_callback_destroy);
  }

  if (gtk_container_surface) {
    container->frame_callback_handler_surface_id = gtk_container_surface_id;
    container->frame_callback_handler = wl_surface_frame(gtk_container_surface);
    wl_callback_add_listener(container->frame_callback_handler, &frame_listener,
                             container);
  } else {
    container->frame_callback_handler_surface_id = -1;
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (gtk_container_surface) {
    container->frame_callback_handler = wl_surface_frame(gtk_container_surface);
    wl_callback_add_listener(container->frame_callback_handler, &frame_listener,
                             container);
  }

  return FALSE;
}
||||||| merged common ancestors
/* We want to draw to GdkWindow owned by mContainer from Compositor thread but
 * Gtk+ can be used in main thread only. So we create wayland wl_surface
 * and attach it as an overlay to GdkWindow.
 *
 * see gtk_clutter_embed_ensure_subsurface() at gtk-clutter-embed.c
 * for reference.
 */
static gboolean
moz_container_map_surface(MozContainer *container)
{
    // Available as of GTK 3.8+
    static auto sGdkWaylandDisplayGetWlCompositor =
        (wl_compositor *(*)(GdkDisplay *))
        dlsym(RTLD_DEFAULT, "gdk_wayland_display_get_wl_compositor");
    static auto sGdkWaylandWindowGetWlSurface =
        (wl_surface *(*)(GdkWindow *))
        dlsym(RTLD_DEFAULT, "gdk_wayland_window_get_wl_surface");
    static auto sGdkWindowGetFrameClock =
        (GdkFrameClock *(*)(GdkWindow *))
        dlsym(RTLD_DEFAULT, "gdk_window_get_frame_clock");

    GdkDisplay *display = gtk_widget_get_display(GTK_WIDGET(container));
    if (GDK_IS_X11_DISPLAY(display))
        return false;

    if (container->subsurface && container->surface)
        return true;

    if (!container->parent_surface_committed) {
        if (!container->parent_surface_committed_handler) {
            GdkWindow* window = gtk_widget_get_window(GTK_WIDGET(container));
            GdkFrameClock *clock = sGdkWindowGetFrameClock(window);
            container->parent_surface_committed_handler =
                g_signal_connect_after(clock, "after-paint",
                                       G_CALLBACK(moz_container_commited_handler),
                                       container);
        }
        return false;
    }
=======
static gboolean moz_container_map_wayland(GtkWidget* widget,
                                          GdkEventAny* event) {
  MozContainer* container = MOZ_CONTAINER(widget);
>>>>>>> upstream-releases

<<<<<<< HEAD
static void moz_container_unmap_wayland(MozContainer *container) {
  g_clear_pointer(&container->eglwindow, wl_egl_window_destroy);
  g_clear_pointer(&container->subsurface, wl_subsurface_destroy);
  g_clear_pointer(&container->surface, wl_surface_destroy);
  g_clear_pointer(&container->frame_callback_handler, wl_callback_destroy);
||||||| merged common ancestors
    if (!container->surface) {
        struct wl_compositor *compositor;
        compositor = sGdkWaylandDisplayGetWlCompositor(display);
        container->surface = wl_compositor_create_surface(compositor);
    }
=======
  LOGWAYLAND(("%s begin [%p] ready_to_draw %d\n", __FUNCTION__,
              (void*)container, container->ready_to_draw));
>>>>>>> upstream-releases

<<<<<<< HEAD
  container->surface_needs_clear = true;
  container->ready_to_draw = false;
||||||| merged common ancestors
    if (!container->subsurface) {
        GdkWindow* window = gtk_widget_get_window(GTK_WIDGET(container));
        wl_surface* gtk_surface = sGdkWaylandWindowGetWlSurface(window);
        if (!gtk_surface) {
          // We requested the underlying wl_surface too early when container
          // is not realized yet. We'll try again before first rendering
          // to mContainer.
          return false;
        }

        container->subsurface =
          wl_subcompositor_get_subsurface (container->subcompositor,
                                           container->surface,
                                           gtk_surface);
        gint x, y;
        gdk_window_get_position(window, &x, &y);
        wl_subsurface_set_position(container->subsurface, x, y);
        wl_subsurface_set_desync(container->subsurface);

        // Route input to parent wl_surface owned by Gtk+ so we get input
        // events from Gtk+.
        GdkDisplay* display = gtk_widget_get_display(GTK_WIDGET (container));
        wl_compositor* compositor = sGdkWaylandDisplayGetWlCompositor(display);
        wl_region* region = wl_compositor_create_region(compositor);
        wl_surface_set_input_region(container->surface, region);
        wl_region_destroy(region);
    }
    return true;
=======
  if (container->ready_to_draw) {
    return FALSE;
  }

  moz_container_request_parent_frame_callback(MOZ_CONTAINER(widget));
  return FALSE;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static gint moz_container_get_scale(MozContainer *container) {
    static auto sGdkWindowGetScaleFactorPtr = (gint(*)(GdkWindow *))dlsym(
        RTLD_DEFAULT, "gdk_window_get_scale_factor");

    if (sGdkWindowGetScaleFactorPtr) {
      GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(container));
      return (*sGdkWindowGetScaleFactorPtr)(window);
    }
||||||| merged common ancestors
static void
moz_container_unmap_surface(MozContainer *container)
{
    g_clear_pointer(&container->eglwindow, wl_egl_window_destroy);
    g_clear_pointer(&container->subsurface, wl_subsurface_destroy);
    g_clear_pointer(&container->surface, wl_surface_destroy);

    if (container->parent_surface_committed_handler) {
        static auto sGdkWindowGetFrameClock =
            (GdkFrameClock *(*)(GdkWindow *))
            dlsym(RTLD_DEFAULT, "gdk_window_get_frame_clock");
        GdkWindow* window = gtk_widget_get_window(GTK_WIDGET(container));
        GdkFrameClock *clock = sGdkWindowGetFrameClock(window);

        g_signal_handler_disconnect(clock,
                                    container->parent_surface_committed_handler);
        container->parent_surface_committed_handler = 0;
    }
    container->parent_surface_committed = false;
}
=======
static void moz_container_unmap_wayland(MozContainer* container) {
  g_clear_pointer(&container->eglwindow, wl_egl_window_destroy);
  g_clear_pointer(&container->subsurface, wl_subsurface_destroy);
  g_clear_pointer(&container->surface, wl_surface_destroy);
  g_clear_pointer(&container->frame_callback_handler, wl_callback_destroy);
  container->frame_callback_handler_surface_id = -1;

  container->surface_needs_clear = true;
  container->ready_to_draw = false;

  LOGWAYLAND(("%s [%p]\n", __FUNCTION__, (void*)container));
}

static gint moz_container_get_scale(MozContainer* container) {
  static auto sGdkWindowGetScaleFactorPtr =
      (gint(*)(GdkWindow*))dlsym(RTLD_DEFAULT, "gdk_window_get_scale_factor");

  if (sGdkWindowGetScaleFactorPtr) {
    GdkWindow* window = gtk_widget_get_window(GTK_WIDGET(container));
    return (*sGdkWindowGetScaleFactorPtr)(window);
  }

  return 1;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    return 1;
}
||||||| merged common ancestors
=======
void moz_container_scale_changed(MozContainer* container,
                                 GtkAllocation* aAllocation) {
  LOGWAYLAND(("%s [%p] surface %p eglwindow %p\n", __FUNCTION__,
              (void*)container, (void*)container->surface,
              (void*)container->eglwindow));

  if (!container->surface) {
    return;
  }

  // Set correct scaled/unscaled mozcontainer offset
  // especially when wl_egl is used but we don't recreate it as Gtk+ does.
  gint x, y;
  gdk_window_get_position(gtk_widget_get_window(GTK_WIDGET(container)), &x, &y);
  wl_subsurface_set_position(container->subsurface, x, y);

  // Try to only resize wl_egl_window on scale factor change.
  // It's a bit risky as Gtk+ recreates it at that event.
  if (container->eglwindow) {
    gint scale = moz_container_get_scale(container);
    wl_surface_set_buffer_scale(container->surface,
                                moz_container_get_scale(container));
    wl_egl_window_resize(container->eglwindow, aAllocation->width * scale,
                         aAllocation->height * scale, 0, 0);
  }
}
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
void moz_container_map(GtkWidget *widget) {
  MozContainer *container;
  GList *tmp_list;
  GtkWidget *tmp_child;
||||||| merged common ancestors
void
moz_container_map (GtkWidget *widget)
{
    MozContainer *container;
    GList *tmp_list;
    GtkWidget *tmp_child;
=======
void moz_container_map(GtkWidget* widget) {
  MozContainer* container;
  GList* tmp_list;
  GtkWidget* tmp_child;
>>>>>>> upstream-releases

  g_return_if_fail(IS_MOZ_CONTAINER(widget));
  container = MOZ_CONTAINER(widget);

  gtk_widget_set_mapped(widget, TRUE);

<<<<<<< HEAD
  tmp_list = container->children;
  while (tmp_list) {
    tmp_child = ((MozContainerChild *)tmp_list->data)->widget;
||||||| merged common ancestors
    tmp_list = container->children;
    while (tmp_list) {
        tmp_child = ((MozContainerChild *)tmp_list->data)->widget;
=======
  tmp_list = container->children;
  while (tmp_list) {
    tmp_child = ((MozContainerChild*)tmp_list->data)->widget;
>>>>>>> upstream-releases

    if (gtk_widget_get_visible(tmp_child)) {
      if (!gtk_widget_get_mapped(tmp_child)) gtk_widget_map(tmp_child);
    }
    tmp_list = tmp_list->next;
  }

  if (gtk_widget_get_has_window(widget)) {
    gdk_window_show(gtk_widget_get_window(widget));
#if defined(MOZ_WAYLAND)
    if (!GDK_IS_X11_DISPLAY(gdk_display_get_default())) {
      moz_container_map_wayland(widget, nullptr);
    }
#endif
  }
}

<<<<<<< HEAD
void moz_container_unmap(GtkWidget *widget) {
  g_return_if_fail(IS_MOZ_CONTAINER(widget));
||||||| merged common ancestors
void
moz_container_unmap (GtkWidget *widget)
{
    g_return_if_fail (IS_MOZ_CONTAINER (widget));
=======
void moz_container_unmap(GtkWidget* widget) {
  g_return_if_fail(IS_MOZ_CONTAINER(widget));
>>>>>>> upstream-releases

  gtk_widget_set_mapped(widget, FALSE);

  if (gtk_widget_get_has_window(widget)) {
    gdk_window_hide(gtk_widget_get_window(widget));
#if defined(MOZ_WAYLAND)
    if (!GDK_IS_X11_DISPLAY(gdk_display_get_default())) {
      moz_container_unmap_wayland(MOZ_CONTAINER(widget));
    }
#endif
  }
}

<<<<<<< HEAD
void moz_container_realize(GtkWidget *widget) {
  GdkWindow *parent = gtk_widget_get_parent_window(widget);
  GdkWindow *window;

  gtk_widget_set_realized(widget, TRUE);

  if (gtk_widget_get_has_window(widget)) {
    GdkWindowAttr attributes;
    gint attributes_mask = GDK_WA_VISUAL | GDK_WA_X | GDK_WA_Y;
    GtkAllocation allocation;

    gtk_widget_get_allocation(widget, &allocation);
    attributes.event_mask = gtk_widget_get_events(widget);
    attributes.x = allocation.x;
    attributes.y = allocation.y;
    attributes.width = allocation.width;
    attributes.height = allocation.height;
    attributes.wclass = GDK_INPUT_OUTPUT;
    attributes.visual = gtk_widget_get_visual(widget);
    attributes.window_type = GDK_WINDOW_CHILD;

    window = gdk_window_new(parent, &attributes, attributes_mask);
    gdk_window_set_user_data(window, widget);
  } else {
    window = parent;
    g_object_ref(window);
  }

  gtk_widget_set_window(widget, window);
}
||||||| merged common ancestors
void
moz_container_realize (GtkWidget *widget)
{
    GdkWindow *parent = gtk_widget_get_parent_window (widget);
    GdkWindow *window;

    gtk_widget_set_realized(widget, TRUE);

    if (gtk_widget_get_has_window (widget)) {
        GdkWindowAttr attributes;
        gint attributes_mask = GDK_WA_VISUAL | GDK_WA_X | GDK_WA_Y;
        GtkAllocation allocation;

        gtk_widget_get_allocation (widget, &allocation);
        attributes.event_mask = gtk_widget_get_events (widget);
        attributes.x = allocation.x;
        attributes.y = allocation.y;
        attributes.width = allocation.width;
        attributes.height = allocation.height;
        attributes.wclass = GDK_INPUT_OUTPUT;
        attributes.visual = gtk_widget_get_visual (widget);
        attributes.window_type = GDK_WINDOW_CHILD;

        window = gdk_window_new (parent, &attributes, attributes_mask);
        gdk_window_set_user_data (window, widget);
    } else {
        window = parent;
        g_object_ref (window);
    }
=======
void moz_container_realize(GtkWidget* widget) {
  GdkWindow* parent = gtk_widget_get_parent_window(widget);
  GdkWindow* window;

  gtk_widget_set_realized(widget, TRUE);

  if (gtk_widget_get_has_window(widget)) {
    GdkWindowAttr attributes;
    gint attributes_mask = GDK_WA_VISUAL | GDK_WA_X | GDK_WA_Y;
    GtkAllocation allocation;

    gtk_widget_get_allocation(widget, &allocation);
    attributes.event_mask = gtk_widget_get_events(widget);
    attributes.x = allocation.x;
    attributes.y = allocation.y;
    attributes.width = allocation.width;
    attributes.height = allocation.height;
    attributes.wclass = GDK_INPUT_OUTPUT;
    attributes.window_type = GDK_WINDOW_CHILD;
    MozContainer* container = MOZ_CONTAINER(widget);
    attributes.visual =
        container->force_default_visual
            ? gdk_screen_get_system_visual(gtk_widget_get_screen(widget))
            : gtk_widget_get_visual(widget);

    window = gdk_window_new(parent, &attributes, attributes_mask);

    LOG(("moz_container_realize() [%p] GdkWindow %p\n", (void*)container,
         (void*)window));

    gdk_window_set_user_data(window, widget);
  } else {
    window = parent;
    g_object_ref(window);
  }

  gtk_widget_set_window(widget, window);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
void moz_container_size_allocate(GtkWidget *widget, GtkAllocation *allocation) {
  MozContainer *container;
  GList *tmp_list;
  GtkAllocation tmp_allocation;
||||||| merged common ancestors
    gtk_widget_set_window (widget, window);
=======
void moz_container_size_allocate(GtkWidget* widget, GtkAllocation* allocation) {
  MozContainer* container;
  GList* tmp_list;
  GtkAllocation tmp_allocation;
>>>>>>> upstream-releases

  g_return_if_fail(IS_MOZ_CONTAINER(widget));

<<<<<<< HEAD
  /*  printf("moz_container_size_allocate %p %d %d %d %d\n",
      (void *)widget,
      allocation->x,
      allocation->y,
      allocation->width,
      allocation->height); */
||||||| merged common ancestors
void
moz_container_size_allocate (GtkWidget     *widget,
                             GtkAllocation *allocation)
{
    MozContainer   *container;
    GList          *tmp_list;
    GtkAllocation   tmp_allocation;

    g_return_if_fail (IS_MOZ_CONTAINER (widget));

    /*  printf("moz_container_size_allocate %p %d %d %d %d\n",
        (void *)widget,
        allocation->x,
        allocation->y,
        allocation->width,
        allocation->height); */

    /* short circuit if you can */
    container = MOZ_CONTAINER (widget);
    gtk_widget_get_allocation(widget, &tmp_allocation);
    if (!container->children &&
        tmp_allocation.x == allocation->x &&
        tmp_allocation.y == allocation->y &&
        tmp_allocation.width == allocation->width &&
        tmp_allocation.height == allocation->height) {
        return;
    }
=======
  LOG(("%s [%p] %d %d %d %d\n", __FUNCTION__, (void*)widget, allocation->x,
       allocation->y, allocation->width, allocation->height));
>>>>>>> upstream-releases

  /* short circuit if you can */
  container = MOZ_CONTAINER(widget);
  gtk_widget_get_allocation(widget, &tmp_allocation);
  if (!container->children && tmp_allocation.x == allocation->x &&
      tmp_allocation.y == allocation->y &&
      tmp_allocation.width == allocation->width &&
      tmp_allocation.height == allocation->height) {
    return;
  }

  gtk_widget_set_allocation(widget, allocation);

  tmp_list = container->children;

<<<<<<< HEAD
  while (tmp_list) {
    MozContainerChild *child = static_cast<MozContainerChild *>(tmp_list->data);
||||||| merged common ancestors
        moz_container_allocate_child (container, child);
=======
  while (tmp_list) {
    MozContainerChild* child = static_cast<MozContainerChild*>(tmp_list->data);
>>>>>>> upstream-releases

    moz_container_allocate_child(container, child);

    tmp_list = tmp_list->next;
  }

  if (gtk_widget_get_has_window(widget) && gtk_widget_get_realized(widget)) {
    gdk_window_move_resize(gtk_widget_get_window(widget), allocation->x,
                           allocation->y, allocation->width,
                           allocation->height);
  }

#if defined(MOZ_WAYLAND)
<<<<<<< HEAD
  // We need to position our subsurface according to GdkWindow
  // when offset changes (GdkWindow is maximized for instance).
  // see gtk-clutter-embed.c for reference.
  if (container->subsurface) {
    gint x, y;
    gdk_window_get_position(gtk_widget_get_window(widget), &x, &y);
    wl_subsurface_set_position(container->subsurface, x, y);
  }
  if (container->eglwindow) {
    gint scale = moz_container_get_scale(container);
    wl_egl_window_resize(container->eglwindow,
                         allocation->width * scale,
                         allocation->height * scale, 0, 0);
  }
||||||| merged common ancestors
    // We need to position our subsurface according to GdkWindow
    // when offset changes (GdkWindow is maximized for instance).
    // see gtk-clutter-embed.c for reference.
    if (container->subsurface) {
        gint x, y;
        gdk_window_get_position(gtk_widget_get_window(widget), &x, &y);
        wl_subsurface_set_position(container->subsurface, x, y);
    }
    if (container->eglwindow) {
        wl_egl_window_resize(container->eglwindow,
                             allocation->width, allocation->height,
                             0, 0);
    }
=======
  // We need to position our subsurface according to GdkWindow
  // when offset changes (GdkWindow is maximized for instance).
  // see gtk-clutter-embed.c for reference.
  if (container->subsurface) {
    gint x, y;
    gdk_window_get_position(gtk_widget_get_window(widget), &x, &y);
    wl_subsurface_set_position(container->subsurface, x, y);
  }
  if (container->eglwindow) {
    gint scale = moz_container_get_scale(container);
    wl_egl_window_resize(container->eglwindow, allocation->width * scale,
                         allocation->height * scale, 0, 0);
  }
>>>>>>> upstream-releases
#endif
}

<<<<<<< HEAD
void moz_container_remove(GtkContainer *container, GtkWidget *child_widget) {
  MozContainerChild *child;
  MozContainer *moz_container;
  GdkWindow *parent_window;

  g_return_if_fail(IS_MOZ_CONTAINER(container));
  g_return_if_fail(GTK_IS_WIDGET(child_widget));

  moz_container = MOZ_CONTAINER(container);

  child = moz_container_get_child(moz_container, child_widget);
  g_return_if_fail(child);

  /* gtk_widget_unparent will remove the parent window (as well as the
   * parent widget), but, in Mozilla's window hierarchy, the parent window
   * may need to be kept because it may be part of a GdkWindow sub-hierarchy
   * that is being moved to another MozContainer.
   *
   * (In a conventional GtkWidget hierarchy, GdkWindows being reparented
   * would have their own GtkWidget and that widget would be the one being
   * reparented.  In Mozilla's hierarchy, the parent_window needs to be
   * retained so that the GdkWindow sub-hierarchy is maintained.)
   */
  parent_window = gtk_widget_get_parent_window(child_widget);
  if (parent_window) g_object_ref(parent_window);

  gtk_widget_unparent(child_widget);

  if (parent_window) {
    /* The child_widget will always still exist because g_signal_emit,
     * which invokes this function, holds a reference.
||||||| merged common ancestors
void
moz_container_remove (GtkContainer *container, GtkWidget *child_widget)
{
    MozContainerChild *child;
    MozContainer *moz_container;
    GdkWindow* parent_window;

    g_return_if_fail (IS_MOZ_CONTAINER(container));
    g_return_if_fail (GTK_IS_WIDGET(child_widget));

    moz_container = MOZ_CONTAINER(container);

    child = moz_container_get_child (moz_container, child_widget);
    g_return_if_fail (child);

    /* gtk_widget_unparent will remove the parent window (as well as the
     * parent widget), but, in Mozilla's window hierarchy, the parent window
     * may need to be kept because it may be part of a GdkWindow sub-hierarchy
     * that is being moved to another MozContainer.
=======
void moz_container_remove(GtkContainer* container, GtkWidget* child_widget) {
  MozContainerChild* child;
  MozContainer* moz_container;
  GdkWindow* parent_window;

  g_return_if_fail(IS_MOZ_CONTAINER(container));
  g_return_if_fail(GTK_IS_WIDGET(child_widget));

  moz_container = MOZ_CONTAINER(container);

  child = moz_container_get_child(moz_container, child_widget);
  g_return_if_fail(child);

  /* gtk_widget_unparent will remove the parent window (as well as the
   * parent widget), but, in Mozilla's window hierarchy, the parent window
   * may need to be kept because it may be part of a GdkWindow sub-hierarchy
   * that is being moved to another MozContainer.
   *
   * (In a conventional GtkWidget hierarchy, GdkWindows being reparented
   * would have their own GtkWidget and that widget would be the one being
   * reparented.  In Mozilla's hierarchy, the parent_window needs to be
   * retained so that the GdkWindow sub-hierarchy is maintained.)
   */
  parent_window = gtk_widget_get_parent_window(child_widget);
  if (parent_window) g_object_ref(parent_window);

  gtk_widget_unparent(child_widget);

  if (parent_window) {
    /* The child_widget will always still exist because g_signal_emit,
     * which invokes this function, holds a reference.
>>>>>>> upstream-releases
     *
     * If parent_window is the container's root window then it will not be
     * the parent_window if the child_widget is placed in another
     * container.
     */
    if (parent_window != gtk_widget_get_window(GTK_WIDGET(container)))
      gtk_widget_set_parent_window(child_widget, parent_window);

    g_object_unref(parent_window);
  }

  moz_container->children = g_list_remove(moz_container->children, child);
  g_free(child);
}

<<<<<<< HEAD
void moz_container_forall(GtkContainer *container, gboolean include_internals,
                          GtkCallback callback, gpointer callback_data) {
  MozContainer *moz_container;
  GList *tmp_list;
||||||| merged common ancestors
void
moz_container_forall (GtkContainer *container, gboolean include_internals,
                      GtkCallback  callback, gpointer callback_data)
{
    MozContainer *moz_container;
    GList *tmp_list;
=======
void moz_container_forall(GtkContainer* container, gboolean include_internals,
                          GtkCallback callback, gpointer callback_data) {
  MozContainer* moz_container;
  GList* tmp_list;
>>>>>>> upstream-releases

  g_return_if_fail(IS_MOZ_CONTAINER(container));
  g_return_if_fail(callback != NULL);

  moz_container = MOZ_CONTAINER(container);

<<<<<<< HEAD
  tmp_list = moz_container->children;
  while (tmp_list) {
    MozContainerChild *child;
    child = static_cast<MozContainerChild *>(tmp_list->data);
    tmp_list = tmp_list->next;
    (*callback)(child->widget, callback_data);
  }
||||||| merged common ancestors
    tmp_list = moz_container->children;
    while (tmp_list) {
        MozContainerChild *child;
        child = static_cast<MozContainerChild*>(tmp_list->data);
        tmp_list = tmp_list->next;
        (* callback) (child->widget, callback_data);
    }
=======
  tmp_list = moz_container->children;
  while (tmp_list) {
    MozContainerChild* child;
    child = static_cast<MozContainerChild*>(tmp_list->data);
    tmp_list = tmp_list->next;
    (*callback)(child->widget, callback_data);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void moz_container_allocate_child(MozContainer *container,
                                         MozContainerChild *child) {
  GtkAllocation allocation;
||||||| merged common ancestors
static void
moz_container_allocate_child (MozContainer *container,
                              MozContainerChild *child)
{
    GtkAllocation  allocation;
=======
static void moz_container_allocate_child(MozContainer* container,
                                         MozContainerChild* child) {
  GtkAllocation allocation;
>>>>>>> upstream-releases

  gtk_widget_get_allocation(child->widget, &allocation);
  allocation.x = child->x;
  allocation.y = child->y;

  gtk_widget_size_allocate(child->widget, &allocation);
}

<<<<<<< HEAD
MozContainerChild *moz_container_get_child(MozContainer *container,
                                           GtkWidget *child_widget) {
  GList *tmp_list;
||||||| merged common ancestors
MozContainerChild *
moz_container_get_child (MozContainer *container, GtkWidget *child_widget)
{
    GList *tmp_list;
=======
MozContainerChild* moz_container_get_child(MozContainer* container,
                                           GtkWidget* child_widget) {
  GList* tmp_list;
>>>>>>> upstream-releases

<<<<<<< HEAD
  tmp_list = container->children;
  while (tmp_list) {
    MozContainerChild *child;
||||||| merged common ancestors
    tmp_list = container->children;
    while (tmp_list) {
        MozContainerChild *child;
=======
  tmp_list = container->children;
  while (tmp_list) {
    MozContainerChild* child;
>>>>>>> upstream-releases

<<<<<<< HEAD
    child = static_cast<MozContainerChild *>(tmp_list->data);
    tmp_list = tmp_list->next;
||||||| merged common ancestors
        child = static_cast<MozContainerChild*>(tmp_list->data);
        tmp_list = tmp_list->next;
=======
    child = static_cast<MozContainerChild*>(tmp_list->data);
    tmp_list = tmp_list->next;
>>>>>>> upstream-releases

    if (child->widget == child_widget) return child;
  }

  return NULL;
}

<<<<<<< HEAD
static void moz_container_add(GtkContainer *container, GtkWidget *widget) {
  moz_container_put(MOZ_CONTAINER(container), widget, 0, 0);
||||||| merged common ancestors
static void
moz_container_add(GtkContainer *container, GtkWidget *widget)
{
    moz_container_put(MOZ_CONTAINER(container), widget, 0, 0);
=======
static void moz_container_add(GtkContainer* container, GtkWidget* widget) {
  moz_container_put(MOZ_CONTAINER(container), widget, 0, 0);
>>>>>>> upstream-releases
}

#ifdef MOZ_WAYLAND
<<<<<<< HEAD
struct wl_surface *moz_container_get_wl_surface(MozContainer *container) {
  if (!container->surface) {
    if (!container->ready_to_draw) {
      return nullptr;
||||||| merged common ancestors
struct wl_surface*
moz_container_get_wl_surface(MozContainer *container)
{
    if (!container->subsurface || !container->surface) {
        GdkWindow* window = gtk_widget_get_window(GTK_WIDGET(container));
        if (!gdk_window_is_visible(window))
            return nullptr;

        moz_container_map_surface(container);
        // Set the scale factor for the buffer right after we create it.
        if (container->surface) {
            static auto sGdkWindowGetScaleFactorPtr = (gint (*)(GdkWindow*))
            dlsym(RTLD_DEFAULT, "gdk_window_get_scale_factor");
            if (sGdkWindowGetScaleFactorPtr && window) {
              gint scaleFactor = (*sGdkWindowGetScaleFactorPtr)(window);
              wl_surface_set_buffer_scale(container->surface, scaleFactor);
            }
        }
=======
struct wl_surface* moz_container_get_wl_surface(MozContainer* container) {
  LOGWAYLAND(("%s [%p] surface %p ready_to_draw %d\n", __FUNCTION__,
              (void*)container, (void*)container->surface,
              container->ready_to_draw));

  if (!container->surface) {
    if (!container->ready_to_draw) {
      moz_container_request_parent_frame_callback(container);
      return nullptr;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    GdkDisplay *display = gtk_widget_get_display(GTK_WIDGET(container));
||||||| merged common ancestors
=======
    GdkDisplay* display = gtk_widget_get_display(GTK_WIDGET(container));
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Available as of GTK 3.8+
    static auto sGdkWaylandDisplayGetWlCompositor =
        (wl_compositor * (*)(GdkDisplay *))
            dlsym(RTLD_DEFAULT, "gdk_wayland_display_get_wl_compositor");
    struct wl_compositor *compositor =
        sGdkWaylandDisplayGetWlCompositor(display);
    container->surface = wl_compositor_create_surface(compositor);

    nsWaylandDisplay *waylandDisplay = WaylandDisplayGet(display);
    container->subsurface = wl_subcompositor_get_subsurface(
        waylandDisplay->GetSubcompositor(), container->surface,
        moz_container_get_gtk_container_surface(container));
    WaylandDisplayRelease(waylandDisplay);

    GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(container));
    gint x, y;
    gdk_window_get_position(window, &x, &y);
    wl_subsurface_set_position(container->subsurface, x, y);
    wl_subsurface_set_desync(container->subsurface);

    // Route input to parent wl_surface owned by Gtk+ so we get input
    // events from Gtk+.
    wl_region *region = wl_compositor_create_region(compositor);
    wl_surface_set_input_region(container->surface, region);
    wl_region_destroy(region);

    wl_surface_set_buffer_scale(container->surface,
                                moz_container_get_scale(container));
  }

  return container->surface;
||||||| merged common ancestors
    return container->surface;
=======
    // Available as of GTK 3.8+
    static auto sGdkWaylandDisplayGetWlCompositor =
        (wl_compositor * (*)(GdkDisplay*))
            dlsym(RTLD_DEFAULT, "gdk_wayland_display_get_wl_compositor");
    struct wl_compositor* compositor =
        sGdkWaylandDisplayGetWlCompositor(display);
    container->surface = wl_compositor_create_surface(compositor);

    nsWaylandDisplay* waylandDisplay = WaylandDisplayGet(display);
    container->subsurface = wl_subcompositor_get_subsurface(
        waylandDisplay->GetSubcompositor(), container->surface,
        moz_container_get_gtk_container_surface(container));

    GdkWindow* window = gtk_widget_get_window(GTK_WIDGET(container));
    gint x, y;
    gdk_window_get_position(window, &x, &y);
    wl_subsurface_set_position(container->subsurface, x, y);
    wl_subsurface_set_desync(container->subsurface);

    // Route input to parent wl_surface owned by Gtk+ so we get input
    // events from Gtk+.
    wl_region* region = wl_compositor_create_region(compositor);
    wl_surface_set_input_region(container->surface, region);
    wl_region_destroy(region);

    wl_surface_set_buffer_scale(container->surface,
                                moz_container_get_scale(container));

    wl_surface_commit(container->surface);
    wl_display_flush(waylandDisplay->GetDisplay());

    LOGWAYLAND(("%s [%p] created surface %p\n", __FUNCTION__, (void*)container,
                (void*)container->surface));
  }

  return container->surface;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
struct wl_egl_window *moz_container_get_wl_egl_window(MozContainer *container) {
  if (!container->eglwindow) {
    wl_surface *surface = moz_container_get_wl_surface(container);
    if (!surface) {
      return nullptr;
||||||| merged common ancestors
struct wl_egl_window *
moz_container_get_wl_egl_window(MozContainer *container)
{
    if (!container->eglwindow) {
        struct wl_surface *wlsurf = moz_container_get_wl_surface(container);
        if (!wlsurf)
            return nullptr;

      GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(container));
      container->eglwindow
            = wl_egl_window_create(wlsurf,
                                   gdk_window_get_width(window),
                                   gdk_window_get_height(window));
=======
struct wl_egl_window* moz_container_get_wl_egl_window(MozContainer* container) {
  LOGWAYLAND(("%s [%p] eglwindow %p\n", __FUNCTION__, (void*)container,
              (void*)container->eglwindow));

  if (!container->eglwindow) {
    wl_surface* surface = moz_container_get_wl_surface(container);
    if (!surface) {
      return nullptr;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD

    GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(container));
    gint scale = moz_container_get_scale(container);
    container->eglwindow = wl_egl_window_create(
        surface, gdk_window_get_width(window) * scale,
                 gdk_window_get_height(window) * scale);
  }
  return container->eglwindow;
}

gboolean moz_container_has_wl_egl_window(MozContainer *container) {
  return container->eglwindow ? true : false;
||||||| merged common ancestors
    return container->eglwindow;
=======

    GdkWindow* window = gtk_widget_get_window(GTK_WIDGET(container));
    gint scale = moz_container_get_scale(container);
    container->eglwindow =
        wl_egl_window_create(surface, gdk_window_get_width(window) * scale,
                             gdk_window_get_height(window) * scale);
    wl_surface_set_buffer_scale(surface, scale);

    LOGWAYLAND(("%s [%p] created eglwindow %p\n", __FUNCTION__,
                (void*)container, (void*)container->eglwindow));
  }

  return container->eglwindow;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
gboolean moz_container_surface_needs_clear(MozContainer *container) {
  gboolean state = container->surface_needs_clear;
  container->surface_needs_clear = false;
  return state;
||||||| merged common ancestors
gboolean
moz_container_has_wl_egl_window(MozContainer *container)
{
    return container->eglwindow ? true : false;
=======
gboolean moz_container_has_wl_egl_window(MozContainer* container) {
  return container->eglwindow ? true : false;
}

gboolean moz_container_surface_needs_clear(MozContainer* container) {
  int ret = container->surface_needs_clear;
  container->surface_needs_clear = false;
  return ret;
>>>>>>> upstream-releases
}
#endif

void moz_container_force_default_visual(MozContainer* container) {
  container->force_default_visual = true;
}
