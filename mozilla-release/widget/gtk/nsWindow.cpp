<<<<<<< HEAD
/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:expandtab:shiftwidth=4:tabstop=4:
||||||| merged common ancestors
/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim:expandtab:shiftwidth=4:tabstop=4:
=======
/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:expandtab:shiftwidth=2:tabstop=2:
>>>>>>> upstream-releases
 */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsWindow.h"

#include "mozilla/ArrayUtils.h"
#include "mozilla/EventForwards.h"
#include "mozilla/MiscEvents.h"
#include "mozilla/MouseEvents.h"
#include "mozilla/PresShell.h"
#include "mozilla/RefPtr.h"
#include "mozilla/TextEvents.h"
#include "mozilla/TimeStamp.h"
#include "mozilla/TouchEvents.h"
#include "mozilla/UniquePtrExtensions.h"
#include "mozilla/WidgetUtils.h"
#include "mozilla/dom/WheelEventBinding.h"
#include <algorithm>

#include "GeckoProfiler.h"

#include "prlink.h"
#include "nsGTKToolkit.h"
#include "nsIRollupListener.h"
#include "nsINode.h"

#include "nsWidgetsCID.h"
#include "nsDragService.h"
#include "nsIWidgetListener.h"
#include "nsIScreenManager.h"
#include "SystemTimeConverter.h"
<<<<<<< HEAD
#include "nsIPresShell.h"
#include "nsViewManager.h"
||||||| merged common ancestors
=======
#include "nsViewManager.h"
#include "nsMenuPopupFrame.h"
>>>>>>> upstream-releases

#include "nsGtkKeyUtils.h"
#include "nsGtkCursors.h"
#include "ScreenHelperGTK.h"

#include <gtk/gtk.h>
#include <gtk/gtkx.h>

#ifdef MOZ_WAYLAND
#  include <gdk/gdkwayland.h>
#endif /* MOZ_WAYLAND */

#ifdef MOZ_X11
#  include <gdk/gdkx.h>
#  include <X11/Xatom.h>
#  include <X11/extensions/XShm.h>
#  include <X11/extensions/shape.h>
#  include <gdk/gdkkeysyms-compat.h>
#endif /* MOZ_X11 */

#include <gdk/gdkkeysyms.h>

#if defined(MOZ_WAYLAND)
<<<<<<< HEAD
#include <gdk/gdkwayland.h>
#include "nsView.h"
||||||| merged common ancestors
#include <gdk/gdkwayland.h>
=======
#  include <gdk/gdkwayland.h>
#  include "nsView.h"
>>>>>>> upstream-releases
#endif

#include "nsGkAtoms.h"

#ifdef MOZ_ENABLE_STARTUP_NOTIFICATION
#  define SN_API_NOT_YET_FROZEN
#  include <startup-notification-1.0/libsn/sn.h>
#endif

#include "mozilla/Assertions.h"
#include "mozilla/Likely.h"
#include "mozilla/Preferences.h"
#include "nsIPrefService.h"
#include "nsIServiceManager.h"
#include "nsGfxCIID.h"
#include "nsGtkUtils.h"
#include "nsIObserverService.h"
#include "mozilla/layers/LayersTypes.h"
#include "nsIIdleServiceInternal.h"
#include "nsIPropertyBag2.h"
#include "GLContext.h"
#include "gfx2DGlue.h"

#ifdef ACCESSIBILITY
#  include "mozilla/a11y/Accessible.h"
#  include "mozilla/a11y/Platform.h"
#  include "nsAccessibilityService.h"

using namespace mozilla;
using namespace mozilla::widget;
#endif

/* For SetIcon */
#include "nsAppDirectoryServiceDefs.h"
#include "nsString.h"
#include "nsIFile.h"

/* SetCursor(imgIContainer*) */
#include <gdk/gdk.h>
#include <wchar.h>
#include "imgIContainer.h"
#include "nsGfxCIID.h"
#include "nsImageToPixbuf.h"
#include "nsIInterfaceRequestorUtils.h"
#include "ClientLayerManager.h"

#include "gfxPlatformGtk.h"
#include "gfxContext.h"
#include "gfxImageSurface.h"
#include "gfxUtils.h"
#include "Layers.h"
#include "GLContextProvider.h"
#include "mozilla/gfx/2D.h"
#include "mozilla/gfx/HelpersCairo.h"
#include "mozilla/gfx/GPUProcessManager.h"
#include "mozilla/layers/CompositorBridgeParent.h"
#include "mozilla/layers/CompositorThread.h"
#include "mozilla/layers/KnowsCompositor.h"

#include "mozilla/layers/APZInputBridge.h"
#include "mozilla/layers/IAPZCTreeManager.h"

#ifdef MOZ_X11
<<<<<<< HEAD
#include "GLContextGLX.h"  // for GLContextGLX::FindVisual()
#include "GtkCompositorWidget.h"
#include "gfxXlibSurface.h"
#include "WindowSurfaceX11Image.h"
#include "WindowSurfaceX11SHM.h"
#include "WindowSurfaceXRender.h"
#endif  // MOZ_X11
||||||| merged common ancestors
#include "GLContextGLX.h" // for GLContextGLX::FindVisual()
#include "GtkCompositorWidget.h"
#include "gfxXlibSurface.h"
#include "WindowSurfaceX11Image.h"
#include "WindowSurfaceX11SHM.h"
#include "WindowSurfaceXRender.h"
#endif // MOZ_X11
=======
#  include "GLContextGLX.h"  // for GLContextGLX::FindVisual()
#  include "GtkCompositorWidget.h"
#  include "gfxXlibSurface.h"
#  include "WindowSurfaceX11Image.h"
#  include "WindowSurfaceX11SHM.h"
#  include "WindowSurfaceXRender.h"
#endif  // MOZ_X11
>>>>>>> upstream-releases
#ifdef MOZ_WAYLAND
#  include "nsIClipboard.h"
#endif

#include "nsShmImage.h"
#include "gtkdrawing.h"

#include "NativeKeyBindings.h"

#include <dlfcn.h>

using namespace mozilla;
using namespace mozilla::gfx;
using namespace mozilla::widget;
using namespace mozilla::layers;
using mozilla::gl::GLContext;
using mozilla::gl::GLContextGLX;

// Don't put more than this many rects in the dirty region, just fluff
// out to the bounding-box if there are more
#define MAX_RECTS_IN_REGION 100

<<<<<<< HEAD
const gint kEvents =
    GDK_EXPOSURE_MASK | GDK_STRUCTURE_MASK | GDK_VISIBILITY_NOTIFY_MASK |
    GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_BUTTON_PRESS_MASK |
    GDK_BUTTON_RELEASE_MASK |
#if GTK_CHECK_VERSION(3, 4, 0)
    GDK_SMOOTH_SCROLL_MASK | GDK_TOUCH_MASK |
||||||| merged common ancestors
const gint kEvents = GDK_EXPOSURE_MASK | GDK_STRUCTURE_MASK |
                     GDK_VISIBILITY_NOTIFY_MASK |
                     GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK |
                     GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
#if GTK_CHECK_VERSION(3,4,0)
                     GDK_SMOOTH_SCROLL_MASK |
                     GDK_TOUCH_MASK |
=======
const gint kEvents =
    GDK_EXPOSURE_MASK | GDK_STRUCTURE_MASK | GDK_VISIBILITY_NOTIFY_MASK |
    GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_BUTTON_PRESS_MASK |
    GDK_BUTTON_RELEASE_MASK |
#if GTK_CHECK_VERSION(3, 4, 0)
    GDK_SMOOTH_SCROLL_MASK | GDK_TOUCH_MASK |
#endif
    GDK_SCROLL_MASK | GDK_POINTER_MOTION_MASK | GDK_PROPERTY_CHANGE_MASK;

#if !GTK_CHECK_VERSION(3, 22, 0)
typedef enum {
  GDK_ANCHOR_FLIP_X = 1 << 0,
  GDK_ANCHOR_FLIP_Y = 1 << 1,
  GDK_ANCHOR_SLIDE_X = 1 << 2,
  GDK_ANCHOR_SLIDE_Y = 1 << 3,
  GDK_ANCHOR_RESIZE_X = 1 << 4,
  GDK_ANCHOR_RESIZE_Y = 1 << 5,
  GDK_ANCHOR_FLIP = GDK_ANCHOR_FLIP_X | GDK_ANCHOR_FLIP_Y,
  GDK_ANCHOR_SLIDE = GDK_ANCHOR_SLIDE_X | GDK_ANCHOR_SLIDE_Y,
  GDK_ANCHOR_RESIZE = GDK_ANCHOR_RESIZE_X | GDK_ANCHOR_RESIZE_Y
} GdkAnchorHints;
>>>>>>> upstream-releases
#endif
<<<<<<< HEAD
    GDK_SCROLL_MASK | GDK_POINTER_MOTION_MASK | GDK_PROPERTY_CHANGE_MASK;
||||||| merged common ancestors
                     GDK_SCROLL_MASK |
                     GDK_POINTER_MOTION_MASK |
                     GDK_PROPERTY_CHANGE_MASK;
=======
>>>>>>> upstream-releases

/* utility functions */
<<<<<<< HEAD
static bool is_mouse_in_window(GdkWindow *aWindow, gdouble aMouseX,
                               gdouble aMouseY);
static nsWindow *get_window_for_gtk_widget(GtkWidget *widget);
static nsWindow *get_window_for_gdk_window(GdkWindow *window);
static GtkWidget *get_gtk_widget_for_gdk_window(GdkWindow *window);
static GdkCursor *get_gtk_cursor(nsCursor aCursor);
||||||| merged common ancestors
static bool       is_mouse_in_window(GdkWindow* aWindow,
                                     gdouble aMouseX, gdouble aMouseY);
static nsWindow  *get_window_for_gtk_widget(GtkWidget *widget);
static nsWindow  *get_window_for_gdk_window(GdkWindow *window);
static GtkWidget *get_gtk_widget_for_gdk_window(GdkWindow *window);
static GdkCursor *get_gtk_cursor(nsCursor aCursor);
=======
static bool is_mouse_in_window(GdkWindow* aWindow, gdouble aMouseX,
                               gdouble aMouseY);
static nsWindow* get_window_for_gtk_widget(GtkWidget* widget);
static nsWindow* get_window_for_gdk_window(GdkWindow* window);
static GtkWidget* get_gtk_widget_for_gdk_window(GdkWindow* window);
static GdkCursor* get_gtk_cursor(nsCursor aCursor);
>>>>>>> upstream-releases

<<<<<<< HEAD
static GdkWindow *get_inner_gdk_window(GdkWindow *aWindow, gint x, gint y,
                                       gint *retx, gint *rety);
||||||| merged common ancestors
static GdkWindow *get_inner_gdk_window (GdkWindow *aWindow,
                                        gint x, gint y,
                                        gint *retx, gint *rety);
=======
static GdkWindow* get_inner_gdk_window(GdkWindow* aWindow, gint x, gint y,
                                       gint* retx, gint* rety);
>>>>>>> upstream-releases

<<<<<<< HEAD
static int is_parent_ungrab_enter(GdkEventCrossing *aEvent);
static int is_parent_grab_leave(GdkEventCrossing *aEvent);
||||||| merged common ancestors
static int    is_parent_ungrab_enter(GdkEventCrossing *aEvent);
static int    is_parent_grab_leave(GdkEventCrossing *aEvent);
=======
static int is_parent_ungrab_enter(GdkEventCrossing* aEvent);
static int is_parent_grab_leave(GdkEventCrossing* aEvent);
>>>>>>> upstream-releases

/* callbacks from widgets */
<<<<<<< HEAD
static gboolean expose_event_cb(GtkWidget *widget, cairo_t *rect);
static gboolean configure_event_cb(GtkWidget *widget, GdkEventConfigure *event);
static void container_unrealize_cb(GtkWidget *widget);
static void size_allocate_cb(GtkWidget *widget, GtkAllocation *allocation);
static gboolean delete_event_cb(GtkWidget *widget, GdkEventAny *event);
static gboolean enter_notify_event_cb(GtkWidget *widget,
                                      GdkEventCrossing *event);
static gboolean leave_notify_event_cb(GtkWidget *widget,
                                      GdkEventCrossing *event);
static gboolean motion_notify_event_cb(GtkWidget *widget,
                                       GdkEventMotion *event);
static gboolean button_press_event_cb(GtkWidget *widget, GdkEventButton *event);
static gboolean button_release_event_cb(GtkWidget *widget,
                                        GdkEventButton *event);
static gboolean focus_in_event_cb(GtkWidget *widget, GdkEventFocus *event);
static gboolean focus_out_event_cb(GtkWidget *widget, GdkEventFocus *event);
static gboolean key_press_event_cb(GtkWidget *widget, GdkEventKey *event);
static gboolean key_release_event_cb(GtkWidget *widget, GdkEventKey *event);
static gboolean property_notify_event_cb(GtkWidget *widget,
                                         GdkEventProperty *event);
static gboolean scroll_event_cb(GtkWidget *widget, GdkEventScroll *event);
static gboolean visibility_notify_event_cb(GtkWidget *widget,
                                           GdkEventVisibility *event);
static void hierarchy_changed_cb(GtkWidget *widget,
                                 GtkWidget *previous_toplevel);
static gboolean window_state_event_cb(GtkWidget *widget,
                                      GdkEventWindowState *event);
static void settings_changed_cb(GtkSettings *settings, GParamSpec *pspec,
                                nsWindow *data);
static void check_resize_cb(GtkContainer *container, gpointer user_data);
static void screen_composited_changed_cb(GdkScreen *screen, gpointer user_data);
static void widget_composited_changed_cb(GtkWidget *widget, gpointer user_data);

static void scale_changed_cb(GtkWidget *widget, GParamSpec *aPSpec,
                             gpointer aPointer);
#if GTK_CHECK_VERSION(3, 4, 0)
static gboolean touch_event_cb(GtkWidget *aWidget, GdkEventTouch *aEvent);
||||||| merged common ancestors
static gboolean expose_event_cb           (GtkWidget *widget,
                                           cairo_t *rect);
static gboolean configure_event_cb        (GtkWidget *widget,
                                           GdkEventConfigure *event);
static void     container_unrealize_cb    (GtkWidget *widget);
static void     size_allocate_cb          (GtkWidget *widget,
                                           GtkAllocation *allocation);
static gboolean delete_event_cb           (GtkWidget *widget,
                                           GdkEventAny *event);
static gboolean enter_notify_event_cb     (GtkWidget *widget,
                                           GdkEventCrossing *event);
static gboolean leave_notify_event_cb     (GtkWidget *widget,
                                           GdkEventCrossing *event);
static gboolean motion_notify_event_cb    (GtkWidget *widget,
                                           GdkEventMotion *event);
static gboolean button_press_event_cb     (GtkWidget *widget,
                                           GdkEventButton *event);
static gboolean button_release_event_cb   (GtkWidget *widget,
                                           GdkEventButton *event);
static gboolean focus_in_event_cb         (GtkWidget *widget,
                                           GdkEventFocus *event);
static gboolean focus_out_event_cb        (GtkWidget *widget,
                                           GdkEventFocus *event);
static gboolean key_press_event_cb        (GtkWidget *widget,
                                           GdkEventKey *event);
static gboolean key_release_event_cb      (GtkWidget *widget,
                                           GdkEventKey *event);
static gboolean property_notify_event_cb  (GtkWidget *widget,
                                           GdkEventProperty *event);
static gboolean scroll_event_cb           (GtkWidget *widget,
                                           GdkEventScroll *event);
static gboolean visibility_notify_event_cb(GtkWidget *widget,
                                           GdkEventVisibility *event);
static void     hierarchy_changed_cb      (GtkWidget *widget,
                                           GtkWidget *previous_toplevel);
static gboolean window_state_event_cb     (GtkWidget *widget,
                                           GdkEventWindowState *event);
static void     settings_changed_cb       (GtkSettings *settings,
                                           GParamSpec *pspec,
                                           nsWindow *data);
static void     check_resize_cb           (GtkContainer* container,
                                           gpointer user_data);
static void     screen_composited_changed_cb     (GdkScreen* screen,
                                                  gpointer user_data);
static void     widget_composited_changed_cb     (GtkWidget* widget,
                                                  gpointer user_data);

static void     scale_changed_cb          (GtkWidget* widget,
                                           GParamSpec* aPSpec,
                                           gpointer aPointer);
#if GTK_CHECK_VERSION(3,4,0)
static gboolean touch_event_cb            (GtkWidget* aWidget,
                                           GdkEventTouch* aEvent);
=======
static gboolean expose_event_cb(GtkWidget* widget, cairo_t* rect);
static gboolean configure_event_cb(GtkWidget* widget, GdkEventConfigure* event);
static void container_unrealize_cb(GtkWidget* widget);
static void size_allocate_cb(GtkWidget* widget, GtkAllocation* allocation);
static gboolean delete_event_cb(GtkWidget* widget, GdkEventAny* event);
static gboolean enter_notify_event_cb(GtkWidget* widget,
                                      GdkEventCrossing* event);
static gboolean leave_notify_event_cb(GtkWidget* widget,
                                      GdkEventCrossing* event);
static gboolean motion_notify_event_cb(GtkWidget* widget,
                                       GdkEventMotion* event);
static gboolean button_press_event_cb(GtkWidget* widget, GdkEventButton* event);
static gboolean button_release_event_cb(GtkWidget* widget,
                                        GdkEventButton* event);
static gboolean focus_in_event_cb(GtkWidget* widget, GdkEventFocus* event);
static gboolean focus_out_event_cb(GtkWidget* widget, GdkEventFocus* event);
static gboolean key_press_event_cb(GtkWidget* widget, GdkEventKey* event);
static gboolean key_release_event_cb(GtkWidget* widget, GdkEventKey* event);
static gboolean property_notify_event_cb(GtkWidget* widget,
                                         GdkEventProperty* event);
static gboolean scroll_event_cb(GtkWidget* widget, GdkEventScroll* event);
static gboolean visibility_notify_event_cb(GtkWidget* widget,
                                           GdkEventVisibility* event);
static void hierarchy_changed_cb(GtkWidget* widget,
                                 GtkWidget* previous_toplevel);
static gboolean window_state_event_cb(GtkWidget* widget,
                                      GdkEventWindowState* event);
static void settings_changed_cb(GtkSettings* settings, GParamSpec* pspec,
                                nsWindow* data);
static void check_resize_cb(GtkContainer* container, gpointer user_data);
static void screen_composited_changed_cb(GdkScreen* screen, gpointer user_data);
static void widget_composited_changed_cb(GtkWidget* widget, gpointer user_data);

static void scale_changed_cb(GtkWidget* widget, GParamSpec* aPSpec,
                             gpointer aPointer);
#if GTK_CHECK_VERSION(3, 4, 0)
static gboolean touch_event_cb(GtkWidget* aWidget, GdkEventTouch* aEvent);
>>>>>>> upstream-releases
#endif
<<<<<<< HEAD
static nsWindow *GetFirstNSWindowForGDKWindow(GdkWindow *aGdkWindow);
||||||| merged common ancestors
static nsWindow* GetFirstNSWindowForGDKWindow (GdkWindow *aGdkWindow);
=======
static nsWindow* GetFirstNSWindowForGDKWindow(GdkWindow* aGdkWindow);
>>>>>>> upstream-releases

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#ifdef MOZ_X11
<<<<<<< HEAD
static GdkFilterReturn popup_take_focus_filter(GdkXEvent *gdk_xevent,
                                               GdkEvent *event, gpointer data);
||||||| merged common ancestors
static GdkFilterReturn popup_take_focus_filter (GdkXEvent *gdk_xevent,
                                                GdkEvent *event,
                                                gpointer data);
=======
static GdkFilterReturn popup_take_focus_filter(GdkXEvent* gdk_xevent,
                                               GdkEvent* event, gpointer data);
>>>>>>> upstream-releases
#endif /* MOZ_X11 */
#ifdef __cplusplus
}
#endif /* __cplusplus */

<<<<<<< HEAD
static gboolean drag_motion_event_cb(GtkWidget *aWidget,
                                     GdkDragContext *aDragContext, gint aX,
                                     gint aY, guint aTime, gpointer aData);
static void drag_leave_event_cb(GtkWidget *aWidget,
                                GdkDragContext *aDragContext, guint aTime,
                                gpointer aData);
static gboolean drag_drop_event_cb(GtkWidget *aWidget,
                                   GdkDragContext *aDragContext, gint aX,
                                   gint aY, guint aTime, gpointer aData);
static void drag_data_received_event_cb(GtkWidget *aWidget,
                                        GdkDragContext *aDragContext, gint aX,
                                        gint aY,
                                        GtkSelectionData *aSelectionData,
                                        guint aInfo, guint32 aTime,
                                        gpointer aData);
||||||| merged common ancestors
static gboolean drag_motion_event_cb      (GtkWidget *aWidget,
                                           GdkDragContext *aDragContext,
                                           gint aX,
                                           gint aY,
                                           guint aTime,
                                           gpointer aData);
static void     drag_leave_event_cb       (GtkWidget *aWidget,
                                           GdkDragContext *aDragContext,
                                           guint aTime,
                                           gpointer aData);
static gboolean drag_drop_event_cb        (GtkWidget *aWidget,
                                           GdkDragContext *aDragContext,
                                           gint aX,
                                           gint aY,
                                           guint aTime,
                                           gpointer aData);
static void    drag_data_received_event_cb(GtkWidget *aWidget,
                                           GdkDragContext *aDragContext,
                                           gint aX,
                                           gint aY,
                                           GtkSelectionData  *aSelectionData,
                                           guint aInfo,
                                           guint32 aTime,
                                           gpointer aData);
=======
static gboolean drag_motion_event_cb(GtkWidget* aWidget,
                                     GdkDragContext* aDragContext, gint aX,
                                     gint aY, guint aTime, gpointer aData);
static void drag_leave_event_cb(GtkWidget* aWidget,
                                GdkDragContext* aDragContext, guint aTime,
                                gpointer aData);
static gboolean drag_drop_event_cb(GtkWidget* aWidget,
                                   GdkDragContext* aDragContext, gint aX,
                                   gint aY, guint aTime, gpointer aData);
static void drag_data_received_event_cb(GtkWidget* aWidget,
                                        GdkDragContext* aDragContext, gint aX,
                                        gint aY,
                                        GtkSelectionData* aSelectionData,
                                        guint aInfo, guint32 aTime,
                                        gpointer aData);
>>>>>>> upstream-releases

/* initialization static functions */
static nsresult initialize_prefs(void);

static guint32 sLastUserInputTime = GDK_CURRENT_TIME;
static guint32 sRetryGrabTime;

<<<<<<< HEAD
static SystemTimeConverter<guint32> &TimeConverter() {
  static SystemTimeConverter<guint32> sTimeConverterSingleton;
  return sTimeConverterSingleton;
||||||| merged common ancestors
static SystemTimeConverter<guint32>&
TimeConverter() {
    static SystemTimeConverter<guint32> sTimeConverterSingleton;
    return sTimeConverterSingleton;
=======
static SystemTimeConverter<guint32>& TimeConverter() {
  static SystemTimeConverter<guint32> sTimeConverterSingleton;
  return sTimeConverterSingleton;
>>>>>>> upstream-releases
}

nsWindow::CSDSupportLevel nsWindow::sCSDSupportLevel = CSD_SUPPORT_UNKNOWN;

namespace mozilla {

<<<<<<< HEAD
class CurrentX11TimeGetter {
 public:
  explicit CurrentX11TimeGetter(GdkWindow *aWindow)
      : mWindow(aWindow), mAsyncUpdateStart() {}
||||||| merged common ancestors
class CurrentX11TimeGetter
{
public:
    explicit CurrentX11TimeGetter(GdkWindow* aWindow)
        : mWindow(aWindow)
        , mAsyncUpdateStart()
    {
    }
=======
class CurrentX11TimeGetter {
 public:
  explicit CurrentX11TimeGetter(GdkWindow* aWindow)
      : mWindow(aWindow), mAsyncUpdateStart() {}
>>>>>>> upstream-releases

  guint32 GetCurrentTime() const { return gdk_x11_get_server_time(mWindow); }

<<<<<<< HEAD
  void GetTimeAsyncForPossibleBackwardsSkew(const TimeStamp &aNow) {
    // Check for in-flight request
    if (!mAsyncUpdateStart.IsNull()) {
      return;
||||||| merged common ancestors
    void GetTimeAsyncForPossibleBackwardsSkew(const TimeStamp& aNow)
    {
        // Check for in-flight request
        if (!mAsyncUpdateStart.IsNull()) {
            return;
        }
        mAsyncUpdateStart = aNow;

        Display* xDisplay = GDK_WINDOW_XDISPLAY(mWindow);
        Window xWindow = GDK_WINDOW_XID(mWindow);
        unsigned char c = 'a';
        Atom timeStampPropAtom = TimeStampPropAtom();
        XChangeProperty(xDisplay, xWindow, timeStampPropAtom,
                        timeStampPropAtom, 8, PropModeReplace, &c, 1);
        XFlush(xDisplay);
=======
  void GetTimeAsyncForPossibleBackwardsSkew(const TimeStamp& aNow) {
    // Check for in-flight request
    if (!mAsyncUpdateStart.IsNull()) {
      return;
>>>>>>> upstream-releases
    }
    mAsyncUpdateStart = aNow;

<<<<<<< HEAD
    Display *xDisplay = GDK_WINDOW_XDISPLAY(mWindow);
    Window xWindow = GDK_WINDOW_XID(mWindow);
    unsigned char c = 'a';
    Atom timeStampPropAtom = TimeStampPropAtom();
    XChangeProperty(xDisplay, xWindow, timeStampPropAtom, timeStampPropAtom, 8,
                    PropModeReplace, &c, 1);
    XFlush(xDisplay);
  }
||||||| merged common ancestors
    gboolean PropertyNotifyHandler(GtkWidget* aWidget,
                                   GdkEventProperty* aEvent)
    {
        if (aEvent->atom !=
            gdk_x11_xatom_to_atom(TimeStampPropAtom())) {
            return FALSE;
        }

        guint32 eventTime = aEvent->time;
        TimeStamp lowerBound = mAsyncUpdateStart;
=======
    Display* xDisplay = GDK_WINDOW_XDISPLAY(mWindow);
    Window xWindow = GDK_WINDOW_XID(mWindow);
    unsigned char c = 'a';
    Atom timeStampPropAtom = TimeStampPropAtom();
    XChangeProperty(xDisplay, xWindow, timeStampPropAtom, timeStampPropAtom, 8,
                    PropModeReplace, &c, 1);
    XFlush(xDisplay);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  gboolean PropertyNotifyHandler(GtkWidget *aWidget, GdkEventProperty *aEvent) {
    if (aEvent->atom != gdk_x11_xatom_to_atom(TimeStampPropAtom())) {
      return FALSE;
||||||| merged common ancestors
        TimeConverter().CompensateForBackwardsSkew(eventTime, lowerBound);
        mAsyncUpdateStart = TimeStamp();
        return TRUE;
=======
  gboolean PropertyNotifyHandler(GtkWidget* aWidget, GdkEventProperty* aEvent) {
    if (aEvent->atom != gdk_x11_xatom_to_atom(TimeStampPropAtom())) {
      return FALSE;
>>>>>>> upstream-releases
    }

    guint32 eventTime = aEvent->time;
    TimeStamp lowerBound = mAsyncUpdateStart;

    TimeConverter().CompensateForBackwardsSkew(eventTime, lowerBound);
    mAsyncUpdateStart = TimeStamp();
    return TRUE;
  }

 private:
  static Atom TimeStampPropAtom() {
    return gdk_x11_get_xatom_by_name_for_display(gdk_display_get_default(),
                                                 "GDK_TIMESTAMP_PROP");
  }

<<<<<<< HEAD
  // This is safe because this class is stored as a member of mWindow and
  // won't outlive it.
  GdkWindow *mWindow;
  TimeStamp mAsyncUpdateStart;
||||||| merged common ancestors
    // This is safe because this class is stored as a member of mWindow and
    // won't outlive it.
    GdkWindow* mWindow;
    TimeStamp  mAsyncUpdateStart;
=======
  // This is safe because this class is stored as a member of mWindow and
  // won't outlive it.
  GdkWindow* mWindow;
  TimeStamp mAsyncUpdateStart;
>>>>>>> upstream-releases
};

}  // namespace mozilla

static NS_DEFINE_IID(kCDragServiceCID, NS_DRAGSERVICE_CID);

// The window from which the focus manager asks us to dispatch key events.
<<<<<<< HEAD
static nsWindow *gFocusWindow = nullptr;
static bool gBlockActivateEvent = false;
static bool gGlobalsInitialized = false;
static bool gRaiseWindows = true;

#if GTK_CHECK_VERSION(3, 4, 0)
static uint32_t gLastTouchID = 0;
||||||| merged common ancestors
static nsWindow         *gFocusWindow          = nullptr;
static bool              gBlockActivateEvent   = false;
static bool              gGlobalsInitialized   = false;
static bool              gRaiseWindows         = true;

#if GTK_CHECK_VERSION(3,4,0)
static uint32_t          gLastTouchID = 0;
=======
static nsWindow* gFocusWindow = nullptr;
static bool gBlockActivateEvent = false;
static bool gGlobalsInitialized = false;
static bool gRaiseWindows = true;
static GList* gVisibleWaylandPopupWindows = nullptr;

#if GTK_CHECK_VERSION(3, 4, 0)
static uint32_t gLastTouchID = 0;
>>>>>>> upstream-releases
#endif

#define NS_WINDOW_TITLE_MAX_LENGTH 4095

// If after selecting profile window, the startup fail, please refer to
// http://bugzilla.gnome.org/show_bug.cgi?id=88940

// needed for imgIContainer cursors
// GdkDisplay* was added in 2.2
typedef struct _GdkDisplay GdkDisplay;

#define kWindowPositionSlop 20

// cursor cache
static GdkCursor* gCursorCache[eCursorCount];

static GtkWidget* gInvisibleContainer = nullptr;

// Sometimes this actually also includes the state of the modifier keys, but
// only the button state bits are used.
static guint gButtonState;

static inline int32_t GetBitmapStride(int32_t width) {
#if defined(MOZ_X11)
  return (width + 7) / 8;
#else
  return cairo_format_stride_for_width(CAIRO_FORMAT_A1, width);
#endif
}

static inline bool TimestampIsNewerThan(guint32 a, guint32 b) {
  // Timestamps are just the least significant bits of a monotonically
  // increasing function, and so the use of unsigned overflow arithmetic.
  return a - b <= G_MAXUINT32 / 2;
}

<<<<<<< HEAD
static void UpdateLastInputEventTime(void *aGdkEvent) {
  nsCOMPtr<nsIIdleServiceInternal> idleService =
      do_GetService("@mozilla.org/widget/idleservice;1");
  if (idleService) {
    idleService->ResetIdleTimeOut(0);
  }
||||||| merged common ancestors
static void
UpdateLastInputEventTime(void *aGdkEvent)
{
    nsCOMPtr<nsIIdleServiceInternal> idleService =
        do_GetService("@mozilla.org/widget/idleservice;1");
    if (idleService) {
        idleService->ResetIdleTimeOut(0);
    }
=======
static void UpdateLastInputEventTime(void* aGdkEvent) {
  nsCOMPtr<nsIIdleServiceInternal> idleService =
      do_GetService("@mozilla.org/widget/idleservice;1");
  if (idleService) {
    idleService->ResetIdleTimeOut(0);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  guint timestamp = gdk_event_get_time(static_cast<GdkEvent *>(aGdkEvent));
  if (timestamp == GDK_CURRENT_TIME) return;
||||||| merged common ancestors
    guint timestamp = gdk_event_get_time(static_cast<GdkEvent*>(aGdkEvent));
    if (timestamp == GDK_CURRENT_TIME)
        return;
=======
  guint timestamp = gdk_event_get_time(static_cast<GdkEvent*>(aGdkEvent));
  if (timestamp == GDK_CURRENT_TIME) return;
>>>>>>> upstream-releases

  sLastUserInputTime = timestamp;
}

nsWindow::nsWindow() {
  mIsTopLevel = false;
  mIsDestroyed = false;
  mListenForResizes = false;
  mNeedsDispatchResized = false;
  mIsShown = false;
  mNeedsShow = false;
  mEnabled = true;
  mCreated = false;
#if GTK_CHECK_VERSION(3, 4, 0)
  mHandleTouchEvent = false;
#endif
  mIsDragPopup = false;
  mIsX11Display = GDK_IS_X11_DISPLAY(gdk_display_get_default());

  mContainer = nullptr;
  mGdkWindow = nullptr;
  mShell = nullptr;
  mCompositorWidgetDelegate = nullptr;
  mHasMappedToplevel = false;
  mIsFullyObscured = false;
  mRetryPointerGrab = false;
  mWindowType = eWindowType_child;
  mSizeState = nsSizeMode_Normal;
  mLastSizeMode = nsSizeMode_Normal;
  mSizeConstraints.mMaxSize = GetSafeWindowSize(mSizeConstraints.mMaxSize);

#ifdef MOZ_X11
  mOldFocusWindow = 0;

  mXDisplay = nullptr;
  mXWindow = X11None;
  mXVisual = nullptr;
  mXDepth = 0;
#endif /* MOZ_X11 */

<<<<<<< HEAD
  if (!gGlobalsInitialized) {
    gGlobalsInitialized = true;
||||||| merged common ancestors
    if (!gGlobalsInitialized) {
        gGlobalsInitialized = true;
=======
#ifdef MOZ_WAYLAND
  mNeedsUpdatingEGLSurface = false;
#endif

  if (!gGlobalsInitialized) {
    gGlobalsInitialized = true;
>>>>>>> upstream-releases

    // It's OK if either of these fail, but it may not be one day.
    initialize_prefs();

#ifdef MOZ_WAYLAND
    // Wayland provides clipboard data to application on focus-in event
    // so we need to init our clipboard hooks before we create window
    // and get focus.
    if (!mIsX11Display) {
      nsCOMPtr<nsIClipboard> clipboard =
          do_GetService("@mozilla.org/widget/clipboard;1");
      NS_ASSERTION(clipboard, "Failed to init clipboard!");
    }
#endif
  }

  mLastMotionPressure = 0;

#ifdef ACCESSIBILITY
  mRootAccessible = nullptr;
#endif

<<<<<<< HEAD
  mIsTransparent = false;
  mTransparencyBitmap = nullptr;
||||||| merged common ancestors
    mIsTransparent = false;
    mTransparencyBitmap = nullptr;
=======
  mIsTransparent = false;
  mTransparencyBitmap = nullptr;
  mTransparencyBitmapForTitlebar = false;
>>>>>>> upstream-releases

  mTransparencyBitmapWidth = 0;
  mTransparencyBitmapHeight = 0;

#if GTK_CHECK_VERSION(3, 4, 0)
  mLastScrollEventTime = GDK_CURRENT_TIME;
#endif
  mPendingConfigures = 0;
  mCSDSupportLevel = CSD_SUPPORT_NONE;
  mDrawInTitlebar = false;
  mTitlebarBackdropState = false;

  mHasAlphaVisual = false;
}

<<<<<<< HEAD
nsWindow::~nsWindow() {
  LOG(("nsWindow::~nsWindow() [%p]\n", (void *)this));
||||||| merged common ancestors
nsWindow::~nsWindow()
{
    LOG(("nsWindow::~nsWindow() [%p]\n", (void *)this));
=======
nsWindow::~nsWindow() {
  LOG(("nsWindow::~nsWindow() [%p]\n", (void*)this));
>>>>>>> upstream-releases

  delete[] mTransparencyBitmap;
  mTransparencyBitmap = nullptr;

  Destroy();
}

<<<<<<< HEAD
/* static */ void nsWindow::ReleaseGlobals() {
  for (auto &cursor : gCursorCache) {
||||||| merged common ancestors
/* static */ void
nsWindow::ReleaseGlobals()
{
  for (auto & cursor : gCursorCache) {
=======
/* static */
void nsWindow::ReleaseGlobals() {
  for (auto& cursor : gCursorCache) {
>>>>>>> upstream-releases
    if (cursor) {
      g_object_unref(cursor);
      cursor = nullptr;
    }
  }
}

<<<<<<< HEAD
void nsWindow::CommonCreate(nsIWidget *aParent, bool aListenForResizes) {
  mParent = aParent;
  mListenForResizes = aListenForResizes;
  mCreated = true;
||||||| merged common ancestors
void
nsWindow::CommonCreate(nsIWidget *aParent, bool aListenForResizes)
{
    mParent = aParent;
    mListenForResizes = aListenForResizes;
    mCreated = true;
=======
void nsWindow::CommonCreate(nsIWidget* aParent, bool aListenForResizes) {
  mParent = aParent;
  mListenForResizes = aListenForResizes;
  mCreated = true;
>>>>>>> upstream-releases
}

void nsWindow::DispatchActivateEvent(void) {
  NS_ASSERTION(mContainer || mIsDestroyed,
               "DispatchActivateEvent only intended for container windows");

#ifdef ACCESSIBILITY
  DispatchActivateEventAccessible();
#endif  // ACCESSIBILITY

  if (mWidgetListener) mWidgetListener->WindowActivated();
}

void nsWindow::DispatchDeactivateEvent(void) {
  if (mWidgetListener) mWidgetListener->WindowDeactivated();

#ifdef ACCESSIBILITY
  DispatchDeactivateEventAccessible();
#endif  // ACCESSIBILITY
}

void nsWindow::DispatchResized() {
  mNeedsDispatchResized = false;
  if (mWidgetListener) {
    mWidgetListener->WindowResized(this, mBounds.width, mBounds.height);
  }
  if (mAttachedWidgetListener) {
    mAttachedWidgetListener->WindowResized(this, mBounds.width, mBounds.height);
  }
}

void nsWindow::MaybeDispatchResized() {
  if (mNeedsDispatchResized && !mIsDestroyed) {
    DispatchResized();
  }
}

<<<<<<< HEAD
nsIWidgetListener *nsWindow::GetListener() {
  return mAttachedWidgetListener ? mAttachedWidgetListener : mWidgetListener;
||||||| merged common ancestors
nsIWidgetListener*
nsWindow::GetListener()
{
    return mAttachedWidgetListener ? mAttachedWidgetListener : mWidgetListener;
=======
nsIWidgetListener* nsWindow::GetListener() {
  return mAttachedWidgetListener ? mAttachedWidgetListener : mWidgetListener;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsWindow::DispatchEvent(WidgetGUIEvent *aEvent,
                                 nsEventStatus &aStatus) {
||||||| merged common ancestors
nsresult
nsWindow::DispatchEvent(WidgetGUIEvent* aEvent, nsEventStatus& aStatus)
{
=======
nsresult nsWindow::DispatchEvent(WidgetGUIEvent* aEvent,
                                 nsEventStatus& aStatus) {
>>>>>>> upstream-releases
#ifdef DEBUG
  debug_DumpEvent(stdout, aEvent->mWidget, aEvent, "something", 0);
#endif
<<<<<<< HEAD
  aStatus = nsEventStatus_eIgnore;
  nsIWidgetListener *listener = GetListener();
  if (listener) {
    aStatus = listener->HandleEvent(aEvent, mUseAttachedEvents);
  }
||||||| merged common ancestors
    aStatus = nsEventStatus_eIgnore;
    nsIWidgetListener* listener = GetListener();
    if (listener) {
      aStatus = listener->HandleEvent(aEvent, mUseAttachedEvents);
    }
=======
  aStatus = nsEventStatus_eIgnore;
  nsIWidgetListener* listener = GetListener();
  if (listener) {
    aStatus = listener->HandleEvent(aEvent, mUseAttachedEvents);
  }
>>>>>>> upstream-releases

  return NS_OK;
}

void nsWindow::OnDestroy(void) {
  if (mOnDestroyCalled) return;

  mOnDestroyCalled = true;

  // Prevent deletion.
  nsCOMPtr<nsIWidget> kungFuDeathGrip = this;

  // release references to children, device context, toolkit + app shell
  nsBaseWidget::OnDestroy();

  // Remove association between this object and its parent and siblings.
  nsBaseWidget::Destroy();
  mParent = nullptr;

  NotifyWindowDestroyed();
}

bool nsWindow::AreBoundsSane(void) {
  if (mBounds.width > 0 && mBounds.height > 0) return true;

  return false;
}

<<<<<<< HEAD
static GtkWidget *EnsureInvisibleContainer() {
  if (!gInvisibleContainer) {
    // GtkWidgets need to be anchored to a GtkWindow to be realized (to
    // have a window).  Using GTK_WINDOW_POPUP rather than
    // GTK_WINDOW_TOPLEVEL in the hope that POPUP results in less
    // initialization and window manager interaction.
    GtkWidget *window = gtk_window_new(GTK_WINDOW_POPUP);
    gInvisibleContainer = moz_container_new();
    gtk_container_add(GTK_CONTAINER(window), gInvisibleContainer);
    gtk_widget_realize(gInvisibleContainer);
  }
  return gInvisibleContainer;
||||||| merged common ancestors
static GtkWidget*
EnsureInvisibleContainer()
{
    if (!gInvisibleContainer) {
        // GtkWidgets need to be anchored to a GtkWindow to be realized (to
        // have a window).  Using GTK_WINDOW_POPUP rather than
        // GTK_WINDOW_TOPLEVEL in the hope that POPUP results in less
        // initialization and window manager interaction.
        GtkWidget* window = gtk_window_new(GTK_WINDOW_POPUP);
        gInvisibleContainer = moz_container_new();
        gtk_container_add(GTK_CONTAINER(window), gInvisibleContainer);
        gtk_widget_realize(gInvisibleContainer);

    }
    return gInvisibleContainer;
=======
static GtkWidget* EnsureInvisibleContainer() {
  if (!gInvisibleContainer) {
    // GtkWidgets need to be anchored to a GtkWindow to be realized (to
    // have a window).  Using GTK_WINDOW_POPUP rather than
    // GTK_WINDOW_TOPLEVEL in the hope that POPUP results in less
    // initialization and window manager interaction.
    GtkWidget* window = gtk_window_new(GTK_WINDOW_POPUP);
    gInvisibleContainer = moz_container_new();
    gtk_container_add(GTK_CONTAINER(window), gInvisibleContainer);
    gtk_widget_realize(gInvisibleContainer);
  }
  return gInvisibleContainer;
>>>>>>> upstream-releases
}

static void CheckDestroyInvisibleContainer() {
  MOZ_ASSERT(gInvisibleContainer, "oh, no");

  if (!gdk_window_peek_children(gtk_widget_get_window(gInvisibleContainer))) {
    // No children, so not in use.
    // Make sure to destroy the GtkWindow also.
    gtk_widget_destroy(gtk_widget_get_parent(gInvisibleContainer));
    gInvisibleContainer = nullptr;
  }
}

// Change the containing GtkWidget on a sub-hierarchy of GdkWindows belonging
// to aOldWidget and rooted at aWindow, and reparent any child GtkWidgets of
// the GdkWindow hierarchy to aNewWidget.
<<<<<<< HEAD
static void SetWidgetForHierarchy(GdkWindow *aWindow, GtkWidget *aOldWidget,
                                  GtkWidget *aNewWidget) {
  gpointer data;
  gdk_window_get_user_data(aWindow, &data);
||||||| merged common ancestors
static void
SetWidgetForHierarchy(GdkWindow *aWindow,
                      GtkWidget *aOldWidget,
                      GtkWidget *aNewWidget)
{
    gpointer data;
    gdk_window_get_user_data(aWindow, &data);
=======
static void SetWidgetForHierarchy(GdkWindow* aWindow, GtkWidget* aOldWidget,
                                  GtkWidget* aNewWidget) {
  gpointer data;
  gdk_window_get_user_data(aWindow, &data);
>>>>>>> upstream-releases

  if (data != aOldWidget) {
    if (!GTK_IS_WIDGET(data)) return;

<<<<<<< HEAD
    auto *widget = static_cast<GtkWidget *>(data);
    if (gtk_widget_get_parent(widget) != aOldWidget) return;
||||||| merged common ancestors
        auto* widget = static_cast<GtkWidget*>(data);
        if (gtk_widget_get_parent(widget) != aOldWidget)
            return;
=======
    auto* widget = static_cast<GtkWidget*>(data);
    if (gtk_widget_get_parent(widget) != aOldWidget) return;
>>>>>>> upstream-releases

    // This window belongs to a child widget, which will no longer be a
    // child of aOldWidget.
    gtk_widget_reparent(widget, aNewWidget);

    return;
  }

<<<<<<< HEAD
  GList *children = gdk_window_get_children(aWindow);
  for (GList *list = children; list; list = list->next) {
    SetWidgetForHierarchy(GDK_WINDOW(list->data), aOldWidget, aNewWidget);
  }
  g_list_free(children);
||||||| merged common ancestors
    GList *children = gdk_window_get_children(aWindow);
    for(GList *list = children; list; list = list->next) {
        SetWidgetForHierarchy(GDK_WINDOW(list->data), aOldWidget, aNewWidget);
    }
    g_list_free(children);
=======
  GList* children = gdk_window_get_children(aWindow);
  for (GList* list = children; list; list = list->next) {
    SetWidgetForHierarchy(GDK_WINDOW(list->data), aOldWidget, aNewWidget);
  }
  g_list_free(children);
>>>>>>> upstream-releases

  gdk_window_set_user_data(aWindow, aNewWidget);
}

// Walk the list of child windows and call destroy on them.
<<<<<<< HEAD
void nsWindow::DestroyChildWindows() {
  if (!mGdkWindow) return;

  while (GList *children = gdk_window_peek_children(mGdkWindow)) {
    GdkWindow *child = GDK_WINDOW(children->data);
    nsWindow *kid = get_window_for_gdk_window(child);
    if (kid) {
      kid->Destroy();
    } else {
      // This child is not an nsWindow.
      // Destroy the child GtkWidget.
      gpointer data;
      gdk_window_get_user_data(child, &data);
      if (GTK_IS_WIDGET(data)) {
        gtk_widget_destroy(static_cast<GtkWidget *>(data));
      }
||||||| merged common ancestors
void
nsWindow::DestroyChildWindows()
{
    if (!mGdkWindow)
        return;

    while (GList *children = gdk_window_peek_children(mGdkWindow)) {
        GdkWindow *child = GDK_WINDOW(children->data);
        nsWindow *kid = get_window_for_gdk_window(child);
        if (kid) {
            kid->Destroy();
        } else {
            // This child is not an nsWindow.
            // Destroy the child GtkWidget.
            gpointer data;
            gdk_window_get_user_data(child, &data);
            if (GTK_IS_WIDGET(data)) {
                gtk_widget_destroy(static_cast<GtkWidget*>(data));
            }
        }
=======
void nsWindow::DestroyChildWindows() {
  if (!mGdkWindow) return;

  while (GList* children = gdk_window_peek_children(mGdkWindow)) {
    GdkWindow* child = GDK_WINDOW(children->data);
    nsWindow* kid = get_window_for_gdk_window(child);
    if (kid) {
      kid->Destroy();
    } else {
      // This child is not an nsWindow.
      // Destroy the child GtkWidget.
      gpointer data;
      gdk_window_get_user_data(child, &data);
      if (GTK_IS_WIDGET(data)) {
        gtk_widget_destroy(static_cast<GtkWidget*>(data));
      }
>>>>>>> upstream-releases
    }
  }
}

void nsWindow::Destroy() {
  if (mIsDestroyed || !mCreated) return;

<<<<<<< HEAD
  LOG(("nsWindow::Destroy [%p]\n", (void *)this));
  mIsDestroyed = true;
  mCreated = false;
||||||| merged common ancestors
    LOG(("nsWindow::Destroy [%p]\n", (void *)this));
    mIsDestroyed = true;
    mCreated = false;
=======
  LOG(("nsWindow::Destroy [%p]\n", (void*)this));
  mIsDestroyed = true;
  mCreated = false;
>>>>>>> upstream-releases

  /** Need to clean our LayerManager up while still alive */
  if (mLayerManager) {
    mLayerManager->Destroy();
  }
  mLayerManager = nullptr;

  // It is safe to call DestroyeCompositor several times (here and
  // in the parent class) since it will take effect only once.
  // The reason we call it here is because on gtk platforms we need
  // to destroy the compositor before we destroy the gdk window (which
  // destroys the the gl context attached to it).
  DestroyCompositor();

#ifdef MOZ_X11
  // Ensure any resources assigned to the window get cleaned up first
  // to avoid double-freeing.
  mSurfaceProvider.CleanupResources();
#endif

  ClearCachedResources();

  g_signal_handlers_disconnect_by_func(
      gtk_settings_get_default(), FuncToGpointer(settings_changed_cb), this);

<<<<<<< HEAD
  nsIRollupListener *rollupListener = nsBaseWidget::GetActiveRollupListener();
  if (rollupListener) {
    nsCOMPtr<nsIWidget> rollupWidget = rollupListener->GetRollupWidget();
    if (static_cast<nsIWidget *>(this) == rollupWidget) {
      rollupListener->Rollup(0, false, nullptr, nullptr);
||||||| merged common ancestors
    nsIRollupListener* rollupListener = nsBaseWidget::GetActiveRollupListener();
    if (rollupListener) {
        nsCOMPtr<nsIWidget> rollupWidget = rollupListener->GetRollupWidget();
        if (static_cast<nsIWidget *>(this) == rollupWidget) {
            rollupListener->Rollup(0, false, nullptr, nullptr);
        }
=======
  nsIRollupListener* rollupListener = nsBaseWidget::GetActiveRollupListener();
  if (rollupListener) {
    nsCOMPtr<nsIWidget> rollupWidget = rollupListener->GetRollupWidget();
    if (static_cast<nsIWidget*>(this) == rollupWidget) {
      rollupListener->Rollup(0, false, nullptr, nullptr);
>>>>>>> upstream-releases
    }
  }

  // dragService will be null after shutdown of the service manager.
  RefPtr<nsDragService> dragService = nsDragService::GetInstance();
  if (dragService && this == dragService->GetMostRecentDestWindow()) {
    dragService->ScheduleLeaveEvent();
  }

  NativeShow(false);

  if (mIMContext) {
    mIMContext->OnDestroyWindow(this);
  }

  // make sure that we remove ourself as the focus window
  if (gFocusWindow == this) {
    LOGFOCUS(("automatically losing focus...\n"));
    gFocusWindow = nullptr;
  }

<<<<<<< HEAD
  GtkWidget *owningWidget = GetMozContainerWidget();
  if (mShell) {
    gtk_widget_destroy(mShell);
    mShell = nullptr;
    mContainer = nullptr;
    MOZ_ASSERT(!mGdkWindow,
               "mGdkWindow should be NULL when mContainer is destroyed");
  } else if (mContainer) {
    gtk_widget_destroy(GTK_WIDGET(mContainer));
    mContainer = nullptr;
    MOZ_ASSERT(!mGdkWindow,
               "mGdkWindow should be NULL when mContainer is destroyed");
  } else if (mGdkWindow) {
    // Destroy child windows to ensure that their mThebesSurfaces are
    // released and to remove references from GdkWindows back to their
    // container widget.  (OnContainerUnrealize() does this when the
    // MozContainer widget is destroyed.)
    DestroyChildWindows();

    gdk_window_set_user_data(mGdkWindow, nullptr);
    g_object_set_data(G_OBJECT(mGdkWindow), "nsWindow", nullptr);
    gdk_window_destroy(mGdkWindow);
    mGdkWindow = nullptr;
  }
||||||| merged common ancestors
    GtkWidget *owningWidget = GetMozContainerWidget();
    if (mShell) {
        gtk_widget_destroy(mShell);
        mShell = nullptr;
        mContainer = nullptr;
        MOZ_ASSERT(!mGdkWindow,
                   "mGdkWindow should be NULL when mContainer is destroyed");
    }
    else if (mContainer) {
        gtk_widget_destroy(GTK_WIDGET(mContainer));
        mContainer = nullptr;
        MOZ_ASSERT(!mGdkWindow,
                   "mGdkWindow should be NULL when mContainer is destroyed");
    }
    else if (mGdkWindow) {
        // Destroy child windows to ensure that their mThebesSurfaces are
        // released and to remove references from GdkWindows back to their
        // container widget.  (OnContainerUnrealize() does this when the
        // MozContainer widget is destroyed.)
        DestroyChildWindows();

        gdk_window_set_user_data(mGdkWindow, nullptr);
        g_object_set_data(G_OBJECT(mGdkWindow), "nsWindow", nullptr);
        gdk_window_destroy(mGdkWindow);
        mGdkWindow = nullptr;
    }
=======
#ifdef MOZ_WAYLAND
  if (mContainer) {
    moz_container_set_initial_draw_callback(mContainer, nullptr);
  }
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (gInvisibleContainer && owningWidget == gInvisibleContainer) {
    CheckDestroyInvisibleContainer();
  }
||||||| merged common ancestors
    if (gInvisibleContainer && owningWidget == gInvisibleContainer) {
        CheckDestroyInvisibleContainer();
    }
=======
  GtkWidget* owningWidget = GetMozContainerWidget();
  if (mShell) {
    gtk_widget_destroy(mShell);
    mShell = nullptr;
    mContainer = nullptr;
    MOZ_ASSERT(!mGdkWindow,
               "mGdkWindow should be NULL when mContainer is destroyed");
  } else if (mContainer) {
    gtk_widget_destroy(GTK_WIDGET(mContainer));
    mContainer = nullptr;
    MOZ_ASSERT(!mGdkWindow,
               "mGdkWindow should be NULL when mContainer is destroyed");
  } else if (mGdkWindow) {
    // Destroy child windows to ensure that their mThebesSurfaces are
    // released and to remove references from GdkWindows back to their
    // container widget.  (OnContainerUnrealize() does this when the
    // MozContainer widget is destroyed.)
    DestroyChildWindows();

    gdk_window_set_user_data(mGdkWindow, nullptr);
    g_object_set_data(G_OBJECT(mGdkWindow), "nsWindow", nullptr);
    gdk_window_destroy(mGdkWindow);
    mGdkWindow = nullptr;
  }

  if (gInvisibleContainer && owningWidget == gInvisibleContainer) {
    CheckDestroyInvisibleContainer();
  }
>>>>>>> upstream-releases

#ifdef ACCESSIBILITY
  if (mRootAccessible) {
    mRootAccessible = nullptr;
  }
#endif

  // Save until last because OnDestroy() may cause us to be deleted.
  OnDestroy();
}

<<<<<<< HEAD
nsIWidget *nsWindow::GetParent(void) { return mParent; }
||||||| merged common ancestors
nsIWidget *
nsWindow::GetParent(void)
{
    return mParent;
}
=======
nsIWidget* nsWindow::GetParent(void) { return mParent; }
>>>>>>> upstream-releases

float nsWindow::GetDPI() {
  float dpi = 96.0f;
  nsCOMPtr<nsIScreen> screen = GetWidgetScreen();
  if (screen) {
    screen->GetDpi(&dpi);
  }
  return dpi;
}

double nsWindow::GetDefaultScaleInternal() {
  return GdkScaleFactor() * gfxPlatformGtk::GetFontScaleFactor();
}

DesktopToLayoutDeviceScale nsWindow::GetDesktopToDeviceScale() {
#ifdef MOZ_WAYLAND
<<<<<<< HEAD
  GdkDisplay *gdkDisplay = gdk_display_get_default();
  if (!GDK_IS_X11_DISPLAY(gdkDisplay)) {
    return DesktopToLayoutDeviceScale(GdkScaleFactor());
  }
||||||| merged common ancestors
    GdkDisplay* gdkDisplay = gdk_display_get_default();
    if (GDK_IS_WAYLAND_DISPLAY(gdkDisplay)) {
        return DesktopToLayoutDeviceScale(GdkScaleFactor());
    }
=======
  GdkDisplay* gdkDisplay = gdk_display_get_default();
  if (!GDK_IS_X11_DISPLAY(gdkDisplay)) {
    return DesktopToLayoutDeviceScale(GdkScaleFactor());
  }
>>>>>>> upstream-releases
#endif

  // In Gtk/X11, we manage windows using device pixels.
  return DesktopToLayoutDeviceScale(1.0);
}

<<<<<<< HEAD
DesktopToLayoutDeviceScale nsWindow::GetDesktopToDeviceScaleByScreen() {
#ifdef MOZ_WAYLAND
  GdkDisplay *gdkDisplay = gdk_display_get_default();
  // In Wayland there's no way to get absolute position of the window and use it
  // to determine the screen factor of the monitor on which the window is
  // placed. The window is notified of the current scale factor but not at this
  // point, so the GdkScaleFactor can return wrong value which can lead to wrong
  // popup placement. We need to use parent's window scale factor for the new
  // one.
  if (!GDK_IS_X11_DISPLAY(gdkDisplay)) {
    nsView *view = nsView::GetViewFor(this);
    if (view) {
      nsView *parentView = view->GetParent();
      if (parentView) {
        nsIWidget *parentWidget = parentView->GetNearestWidget(nullptr);
        if (parentWidget) {
          return DesktopToLayoutDeviceScale(
              parentWidget->RoundsWidgetCoordinatesTo());
        } else {
          NS_WARNING("Widget has no parent");
        }
      }
    } else {
      NS_WARNING("Cannot find widget view");
||||||| merged common ancestors
void
nsWindow::SetParent(nsIWidget *aNewParent)
{
    if (!mGdkWindow) {
        MOZ_ASSERT_UNREACHABLE("The native window has already been destroyed");
        return;
    }

    if (mContainer) {
        // FIXME bug 1469183
        NS_ERROR("nsWindow should not have a container here");
        return;
    }

    nsCOMPtr<nsIWidget> kungFuDeathGrip = this;
    if (mParent) {
        mParent->RemoveChild(this);
=======
DesktopToLayoutDeviceScale nsWindow::GetDesktopToDeviceScaleByScreen() {
#ifdef MOZ_WAYLAND
  GdkDisplay* gdkDisplay = gdk_display_get_default();
  // In Wayland there's no way to get absolute position of the window and use it
  // to determine the screen factor of the monitor on which the window is
  // placed. The window is notified of the current scale factor but not at this
  // point, so the GdkScaleFactor can return wrong value which can lead to wrong
  // popup placement. We need to use parent's window scale factor for the new
  // one.
  if (!GDK_IS_X11_DISPLAY(gdkDisplay)) {
    nsView* view = nsView::GetViewFor(this);
    if (view) {
      nsView* parentView = view->GetParent();
      if (parentView) {
        nsIWidget* parentWidget = parentView->GetNearestWidget(nullptr);
        if (parentWidget) {
          return DesktopToLayoutDeviceScale(
              parentWidget->RoundsWidgetCoordinatesTo());
        } else {
          NS_WARNING("Widget has no parent");
        }
      }
    } else {
      NS_WARNING("Cannot find widget view");
>>>>>>> upstream-releases
    }
  }
#endif
  return nsBaseWidget::GetDesktopToDeviceScale();
}

<<<<<<< HEAD
void nsWindow::SetParent(nsIWidget *aNewParent) {
  if (!mGdkWindow) {
    MOZ_ASSERT_UNREACHABLE("The native window has already been destroyed");
    return;
  }
||||||| merged common ancestors
    mParent = aNewParent;
=======
void nsWindow::SetParent(nsIWidget* aNewParent) {
  if (!mGdkWindow) {
    MOZ_ASSERT_UNREACHABLE("The native window has already been destroyed");
    return;
  }
>>>>>>> upstream-releases

  if (mContainer) {
    // FIXME bug 1469183
    NS_ERROR("nsWindow should not have a container here");
    return;
  }

  nsCOMPtr<nsIWidget> kungFuDeathGrip = this;
  if (mParent) {
    mParent->RemoveChild(this);
  }

  mParent = aNewParent;

<<<<<<< HEAD
  GtkWidget *oldContainer = GetMozContainerWidget();
  if (!oldContainer) {
    // The GdkWindows have been destroyed so there is nothing else to
    // reparent.
    MOZ_ASSERT(gdk_window_is_destroyed(mGdkWindow),
               "live GdkWindow with no widget");
    return;
  }
||||||| merged common ancestors
void
nsWindow::ReparentNativeWidget(nsIWidget* aNewParent)
{
    MOZ_ASSERT(aNewParent, "null widget");
    NS_ASSERTION(!mIsDestroyed, "");
    NS_ASSERTION(!static_cast<nsWindow*>(aNewParent)->mIsDestroyed, "");

    GtkWidget* oldContainer = GetMozContainerWidget();
    if (!oldContainer) {
        // The GdkWindows have been destroyed so there is nothing else to
        // reparent.
        MOZ_ASSERT(gdk_window_is_destroyed(mGdkWindow),
                   "live GdkWindow with no widget");
        return;
    }
    MOZ_ASSERT(!gdk_window_is_destroyed(mGdkWindow),
               "destroyed GdkWindow with widget");

    auto* newParent = static_cast<nsWindow*>(aNewParent);
    GdkWindow* newParentWindow = newParent->mGdkWindow;
    GtkWidget* newContainer = newParent->GetMozContainerWidget();
    GtkWindow* shell = GTK_WINDOW(mShell);

    if (shell && gtk_window_get_transient_for(shell)) {
      GtkWindow* topLevelParent =
          GTK_WINDOW(gtk_widget_get_toplevel(newContainer));
      gtk_window_set_transient_for(shell, topLevelParent);
    }
=======
  GtkWidget* oldContainer = GetMozContainerWidget();
  if (!oldContainer) {
    // The GdkWindows have been destroyed so there is nothing else to
    // reparent.
    MOZ_ASSERT(gdk_window_is_destroyed(mGdkWindow),
               "live GdkWindow with no widget");
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (aNewParent) {
    aNewParent->AddChild(this);
    ReparentNativeWidget(aNewParent);
  } else {
    // aNewParent is nullptr, but reparent to a hidden window to avoid
    // destroying the GdkWindow and its descendants.
    // An invisible container widget is needed to hold descendant
    // GtkWidgets.
    GtkWidget *newContainer = EnsureInvisibleContainer();
    GdkWindow *newParentWindow = gtk_widget_get_window(newContainer);
||||||| merged common ancestors
=======
  if (aNewParent) {
    aNewParent->AddChild(this);
    ReparentNativeWidget(aNewParent);
  } else {
    // aNewParent is nullptr, but reparent to a hidden window to avoid
    // destroying the GdkWindow and its descendants.
    // An invisible container widget is needed to hold descendant
    // GtkWidgets.
    GtkWidget* newContainer = EnsureInvisibleContainer();
    GdkWindow* newParentWindow = gtk_widget_get_window(newContainer);
>>>>>>> upstream-releases
    ReparentNativeWidgetInternal(aNewParent, newContainer, newParentWindow,
                                 oldContainer);
  }
}

bool nsWindow::WidgetTypeSupportsAcceleration() { return !IsSmallPopup(); }

<<<<<<< HEAD
void nsWindow::ReparentNativeWidget(nsIWidget *aNewParent) {
  MOZ_ASSERT(aNewParent, "null widget");
  NS_ASSERTION(!mIsDestroyed, "");
  NS_ASSERTION(!static_cast<nsWindow *>(aNewParent)->mIsDestroyed, "");
||||||| merged common ancestors
            if (aOldContainer == gInvisibleContainer) {
                CheckDestroyInvisibleContainer();
            }
        }
=======
void nsWindow::ReparentNativeWidget(nsIWidget* aNewParent) {
  MOZ_ASSERT(aNewParent, "null widget");
  NS_ASSERTION(!mIsDestroyed, "");
  NS_ASSERTION(!static_cast<nsWindow*>(aNewParent)->mIsDestroyed, "");
>>>>>>> upstream-releases

<<<<<<< HEAD
  GtkWidget *oldContainer = GetMozContainerWidget();
  if (!oldContainer) {
    // The GdkWindows have been destroyed so there is nothing else to
    // reparent.
    MOZ_ASSERT(gdk_window_is_destroyed(mGdkWindow),
               "live GdkWindow with no widget");
    return;
  }
  MOZ_ASSERT(!gdk_window_is_destroyed(mGdkWindow),
             "destroyed GdkWindow with widget");

  auto *newParent = static_cast<nsWindow *>(aNewParent);
  GdkWindow *newParentWindow = newParent->mGdkWindow;
  GtkWidget *newContainer = newParent->GetMozContainerWidget();
  GtkWindow *shell = GTK_WINDOW(mShell);

  if (shell && gtk_window_get_transient_for(shell)) {
    GtkWindow *topLevelParent =
        GTK_WINDOW(gtk_widget_get_toplevel(newContainer));
    gtk_window_set_transient_for(shell, topLevelParent);
  }

  ReparentNativeWidgetInternal(aNewParent, newContainer, newParentWindow,
                               oldContainer);
}

void nsWindow::ReparentNativeWidgetInternal(nsIWidget *aNewParent,
                                            GtkWidget *aNewContainer,
                                            GdkWindow *aNewParentWindow,
                                            GtkWidget *aOldContainer) {
  if (!aNewContainer) {
    // The new parent GdkWindow has been destroyed.
    MOZ_ASSERT(!aNewParentWindow || gdk_window_is_destroyed(aNewParentWindow),
               "live GdkWindow with no widget");
    Destroy();
  } else {
    if (aNewContainer != aOldContainer) {
      MOZ_ASSERT(!gdk_window_is_destroyed(aNewParentWindow),
                 "destroyed GdkWindow with widget");
      SetWidgetForHierarchy(mGdkWindow, aOldContainer, aNewContainer);

      if (aOldContainer == gInvisibleContainer) {
        CheckDestroyInvisibleContainer();
      }
||||||| merged common ancestors
        if (!mIsTopLevel) {
            gdk_window_reparent(mGdkWindow, aNewParentWindow,
                                DevicePixelsToGdkCoordRoundDown(mBounds.x),
                                DevicePixelsToGdkCoordRoundDown(mBounds.y));
        }
=======
  GtkWidget* oldContainer = GetMozContainerWidget();
  if (!oldContainer) {
    // The GdkWindows have been destroyed so there is nothing else to
    // reparent.
    MOZ_ASSERT(gdk_window_is_destroyed(mGdkWindow),
               "live GdkWindow with no widget");
    return;
  }
  MOZ_ASSERT(!gdk_window_is_destroyed(mGdkWindow),
             "destroyed GdkWindow with widget");

  auto* newParent = static_cast<nsWindow*>(aNewParent);
  GdkWindow* newParentWindow = newParent->mGdkWindow;
  GtkWidget* newContainer = newParent->GetMozContainerWidget();
  GtkWindow* shell = GTK_WINDOW(mShell);

  if (shell && gtk_window_get_transient_for(shell)) {
    GtkWindow* topLevelParent =
        GTK_WINDOW(gtk_widget_get_toplevel(newContainer));
    gtk_window_set_transient_for(shell, topLevelParent);
  }

  ReparentNativeWidgetInternal(aNewParent, newContainer, newParentWindow,
                               oldContainer);
}

void nsWindow::ReparentNativeWidgetInternal(nsIWidget* aNewParent,
                                            GtkWidget* aNewContainer,
                                            GdkWindow* aNewParentWindow,
                                            GtkWidget* aOldContainer) {
  if (!aNewContainer) {
    // The new parent GdkWindow has been destroyed.
    MOZ_ASSERT(!aNewParentWindow || gdk_window_is_destroyed(aNewParentWindow),
               "live GdkWindow with no widget");
    Destroy();
  } else {
    if (aNewContainer != aOldContainer) {
      MOZ_ASSERT(!gdk_window_is_destroyed(aNewParentWindow),
                 "destroyed GdkWindow with widget");
      SetWidgetForHierarchy(mGdkWindow, aOldContainer, aNewContainer);

      if (aOldContainer == gInvisibleContainer) {
        CheckDestroyInvisibleContainer();
      }
>>>>>>> upstream-releases
    }

    if (!mIsTopLevel) {
      gdk_window_reparent(mGdkWindow, aNewParentWindow,
                          DevicePixelsToGdkCoordRoundDown(mBounds.x),
                          DevicePixelsToGdkCoordRoundDown(mBounds.y));
    }
<<<<<<< HEAD
  }

  auto *newParent = static_cast<nsWindow *>(aNewParent);
  bool parentHasMappedToplevel = newParent && newParent->mHasMappedToplevel;
  if (mHasMappedToplevel != parentHasMappedToplevel) {
    SetHasMappedToplevel(parentHasMappedToplevel);
  }
||||||| merged common ancestors
=======
  }

  auto* newParent = static_cast<nsWindow*>(aNewParent);
  bool parentHasMappedToplevel = newParent && newParent->mHasMappedToplevel;
  if (mHasMappedToplevel != parentHasMappedToplevel) {
    SetHasMappedToplevel(parentHasMappedToplevel);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsWindow::SetModal(bool aModal) {
  LOG(("nsWindow::SetModal [%p] %d\n", (void *)this, aModal));
  if (mIsDestroyed) return;
  if (!mIsTopLevel || !mShell) return;
  gtk_window_set_modal(GTK_WINDOW(mShell), aModal ? TRUE : FALSE);
||||||| merged common ancestors
void
nsWindow::SetModal(bool aModal)
{
    LOG(("nsWindow::SetModal [%p] %d\n", (void *)this, aModal));
    if (mIsDestroyed)
        return;
    if (!mIsTopLevel || !mShell)
        return;
    gtk_window_set_modal(GTK_WINDOW(mShell), aModal ? TRUE : FALSE);
=======
void nsWindow::SetModal(bool aModal) {
  LOG(("nsWindow::SetModal [%p] %d\n", (void*)this, aModal));
  if (mIsDestroyed) return;
  if (!mIsTopLevel || !mShell) return;
  gtk_window_set_modal(GTK_WINDOW(mShell), aModal ? TRUE : FALSE);
>>>>>>> upstream-releases
}

// nsIWidget method, which means IsShown.
bool nsWindow::IsVisible() const { return mIsShown; }

void nsWindow::RegisterTouchWindow() {
#if GTK_CHECK_VERSION(3, 4, 0)
  mHandleTouchEvent = true;
  mTouches.Clear();
#endif
}

<<<<<<< HEAD
void nsWindow::ConstrainPosition(bool aAllowSlop, int32_t *aX, int32_t *aY) {
  if (!mIsTopLevel || !mShell) return;
||||||| merged common ancestors
void
nsWindow::ConstrainPosition(bool aAllowSlop, int32_t *aX, int32_t *aY)
{
    if (!mIsTopLevel || !mShell)
      return;
=======
void nsWindow::ConstrainPosition(bool aAllowSlop, int32_t* aX, int32_t* aY) {
  if (!mIsTopLevel || !mShell) return;
>>>>>>> upstream-releases

  double dpiScale = GetDefaultScale().scale;

  // we need to use the window size in logical screen pixels
  int32_t logWidth = std::max(NSToIntRound(mBounds.width / dpiScale), 1);
  int32_t logHeight = std::max(NSToIntRound(mBounds.height / dpiScale), 1);

  /* get our playing field. use the current screen, or failing that
    for any reason, use device caps for the default screen. */
  nsCOMPtr<nsIScreen> screen;
  nsCOMPtr<nsIScreenManager> screenmgr =
      do_GetService("@mozilla.org/gfx/screenmanager;1");
  if (screenmgr) {
    screenmgr->ScreenForRect(*aX, *aY, logWidth, logHeight,
                             getter_AddRefs(screen));
  }

  // We don't have any screen so leave the coordinates as is
  if (!screen) return;

  nsIntRect screenRect;
  if (mSizeMode != nsSizeMode_Fullscreen) {
    // For normalized windows, use the desktop work area.
    screen->GetAvailRectDisplayPix(&screenRect.x, &screenRect.y,
                                   &screenRect.width, &screenRect.height);
  } else {
    // For full screen windows, use the desktop.
    screen->GetRectDisplayPix(&screenRect.x, &screenRect.y, &screenRect.width,
                              &screenRect.height);
  }

  if (aAllowSlop) {
    if (*aX < screenRect.x - logWidth + kWindowPositionSlop)
      *aX = screenRect.x - logWidth + kWindowPositionSlop;
    else if (*aX >= screenRect.XMost() - kWindowPositionSlop)
      *aX = screenRect.XMost() - kWindowPositionSlop;

    if (*aY < screenRect.y - logHeight + kWindowPositionSlop)
      *aY = screenRect.y - logHeight + kWindowPositionSlop;
    else if (*aY >= screenRect.YMost() - kWindowPositionSlop)
      *aY = screenRect.YMost() - kWindowPositionSlop;
  } else {
    if (*aX < screenRect.x)
      *aX = screenRect.x;
    else if (*aX >= screenRect.XMost() - logWidth)
      *aX = screenRect.XMost() - logWidth;

    if (*aY < screenRect.y)
      *aY = screenRect.y;
    else if (*aY >= screenRect.YMost() - logHeight)
      *aY = screenRect.YMost() - logHeight;
  }
}

<<<<<<< HEAD
void nsWindow::SetSizeConstraints(const SizeConstraints &aConstraints) {
  mSizeConstraints.mMinSize = GetSafeWindowSize(aConstraints.mMinSize);
  mSizeConstraints.mMaxSize = GetSafeWindowSize(aConstraints.mMaxSize);

  if (mShell) {
    GdkGeometry geometry;
    geometry.min_width =
        DevicePixelsToGdkCoordRoundUp(mSizeConstraints.mMinSize.width);
    geometry.min_height =
        DevicePixelsToGdkCoordRoundUp(mSizeConstraints.mMinSize.height);
    geometry.max_width =
        DevicePixelsToGdkCoordRoundDown(mSizeConstraints.mMaxSize.width);
    geometry.max_height =
        DevicePixelsToGdkCoordRoundDown(mSizeConstraints.mMaxSize.height);

    uint32_t hints = 0;
    if (aConstraints.mMinSize != LayoutDeviceIntSize(0, 0)) {
      hints |= GDK_HINT_MIN_SIZE;
||||||| merged common ancestors
void nsWindow::SetSizeConstraints(const SizeConstraints& aConstraints)
{
    mSizeConstraints.mMinSize = GetSafeWindowSize(aConstraints.mMinSize);
    mSizeConstraints.mMaxSize = GetSafeWindowSize(aConstraints.mMaxSize);

    if (mShell) {
        GdkGeometry geometry;
        geometry.min_width = DevicePixelsToGdkCoordRoundUp(
                             mSizeConstraints.mMinSize.width);
        geometry.min_height = DevicePixelsToGdkCoordRoundUp(
                              mSizeConstraints.mMinSize.height);
        geometry.max_width = DevicePixelsToGdkCoordRoundDown(
                             mSizeConstraints.mMaxSize.width);
        geometry.max_height = DevicePixelsToGdkCoordRoundDown(
                              mSizeConstraints.mMaxSize.height);

        uint32_t hints = 0;
        if (aConstraints.mMinSize != LayoutDeviceIntSize(0, 0)) {
            hints |= GDK_HINT_MIN_SIZE;
        }
        if (aConstraints.mMaxSize !=
            LayoutDeviceIntSize(NS_MAXSIZE, NS_MAXSIZE)) {
            hints |= GDK_HINT_MAX_SIZE;
        }
        gtk_window_set_geometry_hints(GTK_WINDOW(mShell), nullptr,
                                      &geometry, GdkWindowHints(hints));
=======
void nsWindow::SetSizeConstraints(const SizeConstraints& aConstraints) {
  mSizeConstraints.mMinSize = GetSafeWindowSize(aConstraints.mMinSize);
  mSizeConstraints.mMaxSize = GetSafeWindowSize(aConstraints.mMaxSize);

  if (mShell) {
    GdkGeometry geometry;
    geometry.min_width =
        DevicePixelsToGdkCoordRoundUp(mSizeConstraints.mMinSize.width);
    geometry.min_height =
        DevicePixelsToGdkCoordRoundUp(mSizeConstraints.mMinSize.height);
    geometry.max_width =
        DevicePixelsToGdkCoordRoundDown(mSizeConstraints.mMaxSize.width);
    geometry.max_height =
        DevicePixelsToGdkCoordRoundDown(mSizeConstraints.mMaxSize.height);

    uint32_t hints = 0;
    if (aConstraints.mMinSize != LayoutDeviceIntSize(0, 0)) {
      hints |= GDK_HINT_MIN_SIZE;
>>>>>>> upstream-releases
    }
    if (aConstraints.mMaxSize != LayoutDeviceIntSize(NS_MAXSIZE, NS_MAXSIZE)) {
      hints |= GDK_HINT_MAX_SIZE;
    }
    gtk_window_set_geometry_hints(GTK_WINDOW(mShell), nullptr, &geometry,
                                  GdkWindowHints(hints));
  }
}

void nsWindow::Show(bool aState) {
  if (aState == mIsShown) return;

  // Clear our cached resources when the window is hidden.
  if (mIsShown && !aState) {
    ClearCachedResources();
  }

  mIsShown = aState;

<<<<<<< HEAD
  LOG(("nsWindow::Show [%p] state %d\n", (void *)this, aState));
||||||| merged common ancestors
    LOG(("nsWindow::Show [%p] state %d\n", (void *)this, aState));
=======
  LOG(("nsWindow::Show [%p] state %d\n", (void*)this, aState));
>>>>>>> upstream-releases

  if (aState) {
    // Now that this window is shown, mHasMappedToplevel needs to be
    // tracked on viewable descendants.
    SetHasMappedToplevel(mHasMappedToplevel);
  }

  // Ok, someone called show on a window that isn't sized to a sane
  // value.  Mark this window as needing to have Show() called on it
  // and return.
  if ((aState && !AreBoundsSane()) || !mCreated) {
    LOG(("\tbounds are insane or window hasn't been created yet\n"));
    mNeedsShow = true;
    return;
  }

  // If someone is hiding this widget, clear any needing show flag.
  if (!aState) mNeedsShow = false;

#ifdef ACCESSIBILITY
  if (aState && a11y::ShouldA11yBeEnabled()) CreateRootAccessible();
#endif

  NativeShow(aState);
}

void nsWindow::Resize(double aWidth, double aHeight, bool aRepaint) {
  double scale =
      BoundsUseDesktopPixels() ? GetDesktopToDeviceScale().scale : 1.0;
  int32_t width = NSToIntRound(scale * aWidth);
  int32_t height = NSToIntRound(scale * aHeight);
  ConstrainSize(&width, &height);

  // For top-level windows, aWidth and aHeight should possibly be
  // interpreted as frame bounds, but NativeResize treats these as window
  // bounds (Bug 581866).

  mBounds.SizeTo(width, height);

  if (!mCreated) return;

  NativeResize();

  NotifyRollupGeometryChange();

  // send a resize notification if this is a toplevel
  if (mIsTopLevel || mListenForResizes) {
    DispatchResized();
  }
}

void nsWindow::Resize(double aX, double aY, double aWidth, double aHeight,
                      bool aRepaint) {
  double scale =
      BoundsUseDesktopPixels() ? GetDesktopToDeviceScale().scale : 1.0;
  int32_t width = NSToIntRound(scale * aWidth);
  int32_t height = NSToIntRound(scale * aHeight);
  ConstrainSize(&width, &height);

  int32_t x = NSToIntRound(scale * aX);
  int32_t y = NSToIntRound(scale * aY);
  mBounds.x = x;
  mBounds.y = y;
  mBounds.SizeTo(width, height);

  if (!mCreated) return;

  NativeMoveResize();

  NotifyRollupGeometryChange();

  if (mIsTopLevel || mListenForResizes) {
    DispatchResized();
  }
}

void nsWindow::Enable(bool aState) { mEnabled = aState; }

bool nsWindow::IsEnabled() const { return mEnabled; }

<<<<<<< HEAD
void nsWindow::Move(double aX, double aY) {
  LOG(("nsWindow::Move [%p] %f %f\n", (void *)this, aX, aY));
||||||| merged common ancestors
void
nsWindow::Move(double aX, double aY)
{
    LOG(("nsWindow::Move [%p] %f %f\n", (void *)this,
         aX, aY));
=======
void nsWindow::Move(double aX, double aY) {
  LOG(("nsWindow::Move [%p] %f %f\n", (void*)this, aX, aY));
>>>>>>> upstream-releases

  double scale =
      BoundsUseDesktopPixels() ? GetDesktopToDeviceScale().scale : 1.0;
  int32_t x = NSToIntRound(aX * scale);
  int32_t y = NSToIntRound(aY * scale);

  if (mWindowType == eWindowType_toplevel ||
      mWindowType == eWindowType_dialog) {
    SetSizeMode(nsSizeMode_Normal);
  }

  // Since a popup window's x/y coordinates are in relation to to
  // the parent, the parent might have moved so we always move a
  // popup window.
  if (x == mBounds.x && y == mBounds.y && mWindowType != eWindowType_popup)
    return;

  // XXX Should we do some AreBoundsSane check here?

  mBounds.x = x;
  mBounds.y = y;

  if (!mCreated) return;

  NativeMove();

  NotifyRollupGeometryChange();
}

<<<<<<< HEAD
void nsWindow::NativeMove() {
  GdkPoint point = DevicePixelsToGdkPointRoundDown(mBounds.TopLeft());

  if (mIsTopLevel) {
    gtk_window_move(GTK_WINDOW(mShell), point.x, point.y);
  } else if (mGdkWindow) {
    gdk_window_move(mGdkWindow, point.x, point.y);
  }
||||||| merged common ancestors

void
nsWindow::NativeMove()
{
    GdkPoint point = DevicePixelsToGdkPointRoundDown(mBounds.TopLeft());

    if (mIsTopLevel) {
        gtk_window_move(GTK_WINDOW(mShell), point.x, point.y);
    }
    else if (mGdkWindow) {
        gdk_window_move(mGdkWindow, point.x, point.y);
    }
=======
bool nsWindow::IsWaylandPopup() {
  return !mIsX11Display && mIsTopLevel && mWindowType == eWindowType_popup;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsWindow::SetZIndex(int32_t aZIndex) {
  nsIWidget *oldPrev = GetPrevSibling();

  nsBaseWidget::SetZIndex(aZIndex);
||||||| merged common ancestors
void
nsWindow::SetZIndex(int32_t aZIndex)
{
    nsIWidget* oldPrev = GetPrevSibling();

    nsBaseWidget::SetZIndex(aZIndex);
=======
void nsWindow::HideWaylandTooltips() {
  while (gVisibleWaylandPopupWindows) {
    nsWindow* window =
        static_cast<nsWindow*>(gVisibleWaylandPopupWindows->data);
    if (window->mPopupType != ePopupTypeTooltip) break;
    window->HideWaylandWindow();
    gVisibleWaylandPopupWindows = g_list_delete_link(
        gVisibleWaylandPopupWindows, gVisibleWaylandPopupWindows);
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (GetPrevSibling() == oldPrev) {
    return;
  }
||||||| merged common ancestors
    if (GetPrevSibling() == oldPrev) {
        return;
    }
=======
void nsWindow::HideWaylandPopupAndAllChildren() {
  if (g_list_find(gVisibleWaylandPopupWindows, this) == nullptr) {
    NS_WARNING("Popup window isn't in wayland popup list!");
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  NS_ASSERTION(!mContainer, "Expected Mozilla child widget");
||||||| merged common ancestors
    NS_ASSERTION(!mContainer, "Expected Mozilla child widget");
=======
  while (gVisibleWaylandPopupWindows) {
    nsWindow* window =
        static_cast<nsWindow*>(gVisibleWaylandPopupWindows->data);
    bool quit = gVisibleWaylandPopupWindows->data == this;
    window->HideWaylandWindow();
    gVisibleWaylandPopupWindows = g_list_delete_link(
        gVisibleWaylandPopupWindows, gVisibleWaylandPopupWindows);
    if (quit) break;
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // We skip the nsWindows that don't have mGdkWindows.
  // These are probably in the process of being destroyed.
||||||| merged common ancestors
    // We skip the nsWindows that don't have mGdkWindows.
    // These are probably in the process of being destroyed.
=======
// Wayland keeps strong popup window hierarchy. We need to track active
// (visible) popup windows and make sure we hide popup on the same level
// before we open another one on that level. It means that every open
// popup needs to have an unique parent.
GtkWidget* nsWindow::ConfigureWaylandPopupWindows() {
  // Check if we're already configured.
  if (gVisibleWaylandPopupWindows &&
      g_list_find(gVisibleWaylandPopupWindows, this)) {
    return GTK_WIDGET(gtk_window_get_transient_for(GTK_WINDOW(mShell)));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!GetNextSibling()) {
    // We're to be on top.
    if (mGdkWindow) gdk_window_raise(mGdkWindow);
  } else {
    // All the siblings before us need to be below our widget.
    for (nsWindow *w = this; w;
         w = static_cast<nsWindow *>(w->GetPrevSibling())) {
      if (w->mGdkWindow) gdk_window_lower(w->mGdkWindow);
||||||| merged common ancestors
    if (!GetNextSibling()) {
        // We're to be on top.
        if (mGdkWindow)
            gdk_window_raise(mGdkWindow);
    } else {
        // All the siblings before us need to be below our widget.
        for (nsWindow* w = this; w;
             w = static_cast<nsWindow*>(w->GetPrevSibling())) {
            if (w->mGdkWindow)
                gdk_window_lower(w->mGdkWindow);
        }
=======
  // If we're opening a new window we don't want to attach it to a tooltip
  // as it's short lived temporary window.
  HideWaylandTooltips();

  GtkWindow* parentWidget = nullptr;
  if (gVisibleWaylandPopupWindows) {
    if (mPopupType == ePopupTypeTooltip) {
      // Attach tooltip window to the latest popup window
      // to have both visible.
      nsWindow* window =
          static_cast<nsWindow*>(gVisibleWaylandPopupWindows->data);
      parentWidget = GTK_WINDOW(window->GetGtkWidget());
    } else {
      nsMenuPopupFrame* menuPopupFrame = nullptr;
      nsIFrame* frame = GetFrame();
      if (frame) {
        menuPopupFrame = do_QueryFrame(frame);
      }
      // The popup is not fully created yet (we're called from
      // nsWindow::Create()) or we're toplevel popup without parent.
      // In both cases just use parent which was passed to nsWindow::Create().
      if (!menuPopupFrame) {
        return GTK_WIDGET(gtk_window_get_transient_for(GTK_WINDOW(mShell)));
      }

      nsWindow* parentWindow =
          static_cast<nsWindow*>(menuPopupFrame->GetParentMenuWidget());
      if (!parentWindow) {
        // We're toplevel popup menu attached to another menu. Just use our
        // latest popup as a parent.
        parentWindow =
            static_cast<nsWindow*>(gVisibleWaylandPopupWindows->data);
        parentWidget = GTK_WINDOW(parentWindow->GetGtkWidget());
      } else {
        // We're a regular menu in the same frame hierarchy.
        // Close child popups on the same level as we can't have two popups
        // with one parent on Wayland.
        parentWidget = GTK_WINDOW(parentWindow->GetGtkWidget());
        nsWindow* lastChildOnTheSameLevel = nullptr;
        for (GList* popup = gVisibleWaylandPopupWindows; popup;
             popup = popup->next) {
          nsWindow* window =
              static_cast<nsWindow*>(gVisibleWaylandPopupWindows->data);
          if (GTK_WINDOW(window->GetGtkWidget()) == parentWidget) {
            break;
          } else {
            lastChildOnTheSameLevel = window;
          }
        }
        if (lastChildOnTheSameLevel) {
          lastChildOnTheSameLevel->HideWaylandPopupAndAllChildren();
        }
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
  }

  if (parentWidget) {
    gtk_window_set_transient_for(GTK_WINDOW(mShell), parentWidget);
  } else {
    parentWidget = gtk_window_get_transient_for(GTK_WINDOW(mShell));
  }
  gVisibleWaylandPopupWindows =
      g_list_prepend(gVisibleWaylandPopupWindows, this);
  return GTK_WIDGET(parentWidget);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsWindow::SetSizeMode(nsSizeMode aMode) {
  LOG(("nsWindow::SetSizeMode [%p] %d\n", (void *)this, aMode));
||||||| merged common ancestors
void
nsWindow::SetSizeMode(nsSizeMode aMode)
{
    LOG(("nsWindow::SetSizeMode [%p] %d\n", (void *)this, aMode));
=======
#ifdef DEBUG
static void NativeMoveResizeWaylandPopupCallback(
    GdkWindow* window, const GdkRectangle* flipped_rect,
    const GdkRectangle* final_rect, gboolean flipped_x, gboolean flipped_y,
    void* unused) {
  LOG(("%s flipped %d %d\n", __FUNCTION__, flipped_rect->x, flipped_rect->y));
  LOG(("%s final %d %d\n", __FUNCTION__, final_rect->x, final_rect->y));
}
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Save the requested state.
  nsBaseWidget::SetSizeMode(aMode);
||||||| merged common ancestors
    // Save the requested state.
    nsBaseWidget::SetSizeMode(aMode);
=======
void nsWindow::NativeMoveResizeWaylandPopup(GdkPoint* aPosition,
                                            GdkRectangle* aSize) {
  // Available as of GTK 3.24+
  static auto sGdkWindowMoveToRect = (void (*)(
      GdkWindow*, const GdkRectangle*, GdkGravity, GdkGravity, GdkAnchorHints,
      gint, gint))dlsym(RTLD_DEFAULT, "gdk_window_move_to_rect");
>>>>>>> upstream-releases

<<<<<<< HEAD
  // return if there's no shell or our current state is the same as
  // the mode we were just set to.
  if (!mShell || mSizeState == mSizeMode) {
    return;
  }
||||||| merged common ancestors
    // return if there's no shell or our current state is the same as
    // the mode we were just set to.
    if (!mShell || mSizeState == mSizeMode) {
        return;
    }
=======
  // Compositor may be confused by windows with width/height = 0
  // and positioning such windows leads to Bug 1555866.
  if (!AreBoundsSane()) {
    return;
  }

  if (aSize) {
    gtk_window_resize(GTK_WINDOW(mShell), aSize->width, aSize->height);
  }

  GdkWindow* gdkWindow = gtk_widget_get_window(GTK_WIDGET(mShell));

  // Use standard gtk_window_move() instead of gdk_window_move_to_rect() when:
  // - gdk_window_move_to_rect() is not available
  // - the widget doesn't have a valid GdkWindow
  if (!sGdkWindowMoveToRect || !gdkWindow) {
    gtk_window_move(GTK_WINDOW(mShell), aPosition->x, aPosition->y);
    return;
  }

  GtkWidget* parentWindow = ConfigureWaylandPopupWindows();
  LOG(("nsWindow::NativeMoveResizeWaylandPopup [%p] Set popup parent %p\n",
       (void*)this, parentWindow));

  int x_parent, y_parent;
  gdk_window_get_origin(gtk_widget_get_window(GTK_WIDGET(parentWindow)),
                        &x_parent, &y_parent);

  GdkRectangle rect = {aPosition->x - x_parent, aPosition->y - y_parent, 1, 1};
  if (aSize) {
    rect.width = aSize->width;
    rect.height = aSize->height;
  }

  LOG(("%s [%p] request position %d,%d\n", __FUNCTION__, (void*)this,
       aPosition->x, aPosition->y));
  if (aSize) {
    LOG(("  request size %d,%d\n", aSize->width, aSize->height));
  }
  LOG(("  request result %d %d\n", rect.x, rect.y));
#ifdef DEBUG
  g_signal_connect(gdkWindow, "moved-to-rect",
                   G_CALLBACK(NativeMoveResizeWaylandPopupCallback), this);
#endif

  GdkGravity rectAnchor = GDK_GRAVITY_NORTH_WEST;
  GdkGravity menuAnchor = GDK_GRAVITY_NORTH_WEST;
  if (GetTextDirection() == GTK_TEXT_DIR_RTL) {
    rectAnchor = GDK_GRAVITY_NORTH_EAST;
    menuAnchor = GDK_GRAVITY_NORTH_EAST;
  }

  GdkAnchorHints hints = GdkAnchorHints(GDK_ANCHOR_SLIDE | GDK_ANCHOR_FLIP);
  if (aSize) {
    hints = GdkAnchorHints(hints | GDK_ANCHOR_RESIZE);
  }

  // A workaround for https://gitlab.gnome.org/GNOME/gtk/issues/1986
  // gdk_window_move_to_rect() does not reposition visible windows.
  static auto sGtkWidgetIsVisible =
      (gboolean(*)(GtkWidget*))dlsym(RTLD_DEFAULT, "gtk_widget_is_visible");

  bool isWidgetVisible =
      (sGtkWidgetIsVisible != nullptr) && sGtkWidgetIsVisible(mShell);
  if (isWidgetVisible) {
    HideWaylandWindow();
  }

  sGdkWindowMoveToRect(gdkWindow, &rect, rectAnchor, menuAnchor, hints, 0, 0);

  if (isWidgetVisible) {
    // We show the popup with the same configuration so no need to call
    // ConfigureWaylandPopupWindows() before gtk_widget_show().
    gtk_widget_show(mShell);
  }
}

void nsWindow::NativeMove() {
  GdkPoint point = DevicePixelsToGdkPointRoundDown(mBounds.TopLeft());

  LOG(("nsWindow::NativeMove [%p] %d %d\n", (void*)this, point.x, point.y));

  if (IsWaylandPopup()) {
    NativeMoveResizeWaylandPopup(&point, nullptr);
  } else if (mIsTopLevel) {
    gtk_window_move(GTK_WINDOW(mShell), point.x, point.y);
  } else if (mGdkWindow) {
    gdk_window_move(mGdkWindow, point.x, point.y);
  }
}

void nsWindow::SetZIndex(int32_t aZIndex) {
  nsIWidget* oldPrev = GetPrevSibling();

  nsBaseWidget::SetZIndex(aZIndex);

  if (GetPrevSibling() == oldPrev) {
    return;
  }

  NS_ASSERTION(!mContainer, "Expected Mozilla child widget");

  // We skip the nsWindows that don't have mGdkWindows.
  // These are probably in the process of being destroyed.

  if (!GetNextSibling()) {
    // We're to be on top.
    if (mGdkWindow) gdk_window_raise(mGdkWindow);
  } else {
    // All the siblings before us need to be below our widget.
    for (nsWindow* w = this; w;
         w = static_cast<nsWindow*>(w->GetPrevSibling())) {
      if (w->mGdkWindow) gdk_window_lower(w->mGdkWindow);
    }
  }
}

void nsWindow::SetSizeMode(nsSizeMode aMode) {
  LOG(("nsWindow::SetSizeMode [%p] %d\n", (void*)this, aMode));

  // Save the requested state.
  nsBaseWidget::SetSizeMode(aMode);

  // return if there's no shell or our current state is the same as
  // the mode we were just set to.
  if (!mShell || mSizeState == mSizeMode) {
    return;
  }
>>>>>>> upstream-releases

  switch (aMode) {
    case nsSizeMode_Maximized:
      gtk_window_maximize(GTK_WINDOW(mShell));
      break;
    case nsSizeMode_Minimized:
      gtk_window_iconify(GTK_WINDOW(mShell));
      break;
    case nsSizeMode_Fullscreen:
      MakeFullScreen(true);
      break;

    default:
      // nsSizeMode_Normal, really.
      if (mSizeState == nsSizeMode_Minimized)
        gtk_window_deiconify(GTK_WINDOW(mShell));
      else if (mSizeState == nsSizeMode_Maximized)
        gtk_window_unmaximize(GTK_WINDOW(mShell));
      break;
  }

  mSizeState = mSizeMode;
}

<<<<<<< HEAD
typedef void (*SetUserTimeFunc)(GdkWindow *aWindow, guint32 aTimestamp);
||||||| merged common ancestors
typedef void (* SetUserTimeFunc)(GdkWindow* aWindow, guint32 aTimestamp);
=======
typedef void (*SetUserTimeFunc)(GdkWindow* aWindow, guint32 aTimestamp);
>>>>>>> upstream-releases

// This will become obsolete when new GTK APIs are widely supported,
// as described here: http://bugzilla.gnome.org/show_bug.cgi?id=347375
<<<<<<< HEAD
static void SetUserTimeAndStartupIDForActivatedWindow(GtkWidget *aWindow) {
  nsGTKToolkit *GTKToolkit = nsGTKToolkit::GetToolkit();
  if (!GTKToolkit) return;

  nsAutoCString desktopStartupID;
  GTKToolkit->GetDesktopStartupID(&desktopStartupID);
  if (desktopStartupID.IsEmpty()) {
    // We don't have the data we need. Fall back to an
    // approximation ... using the timestamp of the remote command
    // being received as a guess for the timestamp of the user event
    // that triggered it.
    uint32_t timestamp = GTKToolkit->GetFocusTimestamp();
    if (timestamp) {
      gdk_window_focus(gtk_widget_get_window(aWindow), timestamp);
      GTKToolkit->SetFocusTimestamp(0);
||||||| merged common ancestors
static void
SetUserTimeAndStartupIDForActivatedWindow(GtkWidget* aWindow)
{
    nsGTKToolkit* GTKToolkit = nsGTKToolkit::GetToolkit();
    if (!GTKToolkit)
        return;

    nsAutoCString desktopStartupID;
    GTKToolkit->GetDesktopStartupID(&desktopStartupID);
    if (desktopStartupID.IsEmpty()) {
        // We don't have the data we need. Fall back to an
        // approximation ... using the timestamp of the remote command
        // being received as a guess for the timestamp of the user event
        // that triggered it.
        uint32_t timestamp = GTKToolkit->GetFocusTimestamp();
        if (timestamp) {
            gdk_window_focus(gtk_widget_get_window(aWindow), timestamp);
            GTKToolkit->SetFocusTimestamp(0);
        }
        return;
=======
static void SetUserTimeAndStartupIDForActivatedWindow(GtkWidget* aWindow) {
  nsGTKToolkit* GTKToolkit = nsGTKToolkit::GetToolkit();
  if (!GTKToolkit) return;

  nsAutoCString desktopStartupID;
  GTKToolkit->GetDesktopStartupID(&desktopStartupID);
  if (desktopStartupID.IsEmpty()) {
    // We don't have the data we need. Fall back to an
    // approximation ... using the timestamp of the remote command
    // being received as a guess for the timestamp of the user event
    // that triggered it.
    uint32_t timestamp = GTKToolkit->GetFocusTimestamp();
    if (timestamp) {
      gdk_window_focus(gtk_widget_get_window(aWindow), timestamp);
      GTKToolkit->SetFocusTimestamp(0);
>>>>>>> upstream-releases
    }
    return;
  }

#if defined(MOZ_ENABLE_STARTUP_NOTIFICATION)
<<<<<<< HEAD
  // TODO - Implement for non-X11 Gtk backends (Bug 726479)
  if (GDK_IS_X11_DISPLAY(gdk_display_get_default())) {
    GdkWindow *gdkWindow = gtk_widget_get_window(aWindow);

    GdkScreen *screen = gdk_window_get_screen(gdkWindow);
    SnDisplay *snd = sn_display_new(
        gdk_x11_display_get_xdisplay(gdk_window_get_display(gdkWindow)),
        nullptr, nullptr);
    if (!snd) return;
    SnLauncheeContext *ctx = sn_launchee_context_new(
        snd, gdk_screen_get_number(screen), desktopStartupID.get());
    if (!ctx) {
      sn_display_unref(snd);
      return;
    }
||||||| merged common ancestors
    // TODO - Implement for non-X11 Gtk backends (Bug 726479)
    if (GDK_IS_X11_DISPLAY(gdk_display_get_default())) {
        GdkWindow* gdkWindow = gtk_widget_get_window(aWindow);

        GdkScreen* screen = gdk_window_get_screen(gdkWindow);
        SnDisplay* snd =
            sn_display_new(gdk_x11_display_get_xdisplay(gdk_window_get_display(gdkWindow)),
                           nullptr, nullptr);
        if (!snd)
            return;
        SnLauncheeContext* ctx =
            sn_launchee_context_new(snd, gdk_screen_get_number(screen),
                                    desktopStartupID.get());
        if (!ctx) {
            sn_display_unref(snd);
            return;
        }
=======
  // TODO - Implement for non-X11 Gtk backends (Bug 726479)
  if (GDK_IS_X11_DISPLAY(gdk_display_get_default())) {
    GdkWindow* gdkWindow = gtk_widget_get_window(aWindow);

    GdkScreen* screen = gdk_window_get_screen(gdkWindow);
    SnDisplay* snd = sn_display_new(
        gdk_x11_display_get_xdisplay(gdk_window_get_display(gdkWindow)),
        nullptr, nullptr);
    if (!snd) return;
    SnLauncheeContext* ctx = sn_launchee_context_new(
        snd, gdk_screen_get_number(screen), desktopStartupID.get());
    if (!ctx) {
      sn_display_unref(snd);
      return;
    }
>>>>>>> upstream-releases

    if (sn_launchee_context_get_id_has_timestamp(ctx)) {
      gdk_x11_window_set_user_time(gdkWindow,
                                   sn_launchee_context_get_timestamp(ctx));
    }

    sn_launchee_context_setup_window(ctx, gdk_x11_window_get_xid(gdkWindow));
    sn_launchee_context_complete(ctx);

    sn_launchee_context_unref(ctx);
    sn_display_unref(snd);
  }
#endif

<<<<<<< HEAD
  // If we used the startup ID, that already contains the focus timestamp;
  // we don't want to reuse the timestamp next time we raise the window
  GTKToolkit->SetFocusTimestamp(0);
  GTKToolkit->SetDesktopStartupID(EmptyCString());
}

/* static */ guint32 nsWindow::GetLastUserInputTime() {
  // gdk_x11_display_get_user_time/gtk_get_current_event_time tracks
  // button and key presses, DESKTOP_STARTUP_ID used to start the app,
  // drop events from external drags,
  // WM_DELETE_WINDOW delete events, but not usually mouse motion nor
  // button and key releases.  Therefore use the most recent of
  // gdk_x11_display_get_user_time and the last time that we have seen.
  GdkDisplay *gdkDisplay = gdk_display_get_default();
  guint32 timestamp = GDK_IS_X11_DISPLAY(gdkDisplay)
                          ? gdk_x11_display_get_user_time(gdkDisplay)
                          : gtk_get_current_event_time();

  if (sLastUserInputTime != GDK_CURRENT_TIME &&
      TimestampIsNewerThan(sLastUserInputTime, timestamp)) {
    return sLastUserInputTime;
  }
||||||| merged common ancestors
    // If we used the startup ID, that already contains the focus timestamp;
    // we don't want to reuse the timestamp next time we raise the window
    GTKToolkit->SetFocusTimestamp(0);
    GTKToolkit->SetDesktopStartupID(EmptyCString());
}

/* static */ guint32
nsWindow::GetLastUserInputTime()
{
    // gdk_x11_display_get_user_time/gtk_get_current_event_time tracks
    // button and key presses, DESKTOP_STARTUP_ID used to start the app,
    // drop events from external drags,
    // WM_DELETE_WINDOW delete events, but not usually mouse motion nor
    // button and key releases.  Therefore use the most recent of
    // gdk_x11_display_get_user_time and the last time that we have seen.
    GdkDisplay* gdkDisplay = gdk_display_get_default();
    guint32 timestamp = GDK_IS_X11_DISPLAY(gdkDisplay) ?
            gdk_x11_display_get_user_time(gdkDisplay) :
            gtk_get_current_event_time();

    if (sLastUserInputTime != GDK_CURRENT_TIME &&
        TimestampIsNewerThan(sLastUserInputTime, timestamp)) {
        return sLastUserInputTime;
    }
=======
  // If we used the startup ID, that already contains the focus timestamp;
  // we don't want to reuse the timestamp next time we raise the window
  GTKToolkit->SetFocusTimestamp(0);
  GTKToolkit->SetDesktopStartupID(EmptyCString());
}

/* static */
guint32 nsWindow::GetLastUserInputTime() {
  // gdk_x11_display_get_user_time/gtk_get_current_event_time tracks
  // button and key presses, DESKTOP_STARTUP_ID used to start the app,
  // drop events from external drags,
  // WM_DELETE_WINDOW delete events, but not usually mouse motion nor
  // button and key releases.  Therefore use the most recent of
  // gdk_x11_display_get_user_time and the last time that we have seen.
  GdkDisplay* gdkDisplay = gdk_display_get_default();
  guint32 timestamp = GDK_IS_X11_DISPLAY(gdkDisplay)
                          ? gdk_x11_display_get_user_time(gdkDisplay)
                          : gtk_get_current_event_time();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return timestamp;
}
||||||| merged common ancestors
    return timestamp;
}
=======
  if (sLastUserInputTime != GDK_CURRENT_TIME &&
      TimestampIsNewerThan(sLastUserInputTime, timestamp)) {
    return sLastUserInputTime;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsWindow::SetFocus(bool aRaise) {
  // Make sure that our owning widget has focus.  If it doesn't try to
  // grab it.  Note that we don't set our focus flag in this case.
||||||| merged common ancestors
nsresult
nsWindow::SetFocus(bool aRaise)
{
    // Make sure that our owning widget has focus.  If it doesn't try to
    // grab it.  Note that we don't set our focus flag in this case.
=======
  return timestamp;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  LOGFOCUS(("  SetFocus %d [%p]\n", aRaise, (void *)this));
||||||| merged common ancestors
    LOGFOCUS(("  SetFocus %d [%p]\n", aRaise, (void *)this));
=======
void nsWindow::SetFocus(Raise aRaise) {
  // Make sure that our owning widget has focus.  If it doesn't try to
  // grab it.  Note that we don't set our focus flag in this case.
>>>>>>> upstream-releases

<<<<<<< HEAD
  GtkWidget *owningWidget = GetMozContainerWidget();
  if (!owningWidget) return NS_ERROR_FAILURE;
||||||| merged common ancestors
    GtkWidget *owningWidget = GetMozContainerWidget();
    if (!owningWidget)
        return NS_ERROR_FAILURE;
=======
  LOGFOCUS(("  SetFocus %d [%p]\n", aRaise == Raise::Yes, (void*)this));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Raise the window if someone passed in true and the prefs are
  // set properly.
  GtkWidget *toplevelWidget = gtk_widget_get_toplevel(owningWidget);
||||||| merged common ancestors
    // Raise the window if someone passed in true and the prefs are
    // set properly.
    GtkWidget *toplevelWidget = gtk_widget_get_toplevel(owningWidget);

    if (gRaiseWindows && aRaise && toplevelWidget &&
        !gtk_widget_has_focus(owningWidget) &&
        !gtk_widget_has_focus(toplevelWidget)) {
        GtkWidget* top_window = GetToplevelWidget();
        if (top_window && (gtk_widget_get_visible(top_window)))
        {
            gdk_window_show_unraised(gtk_widget_get_window(top_window));
            // Unset the urgency hint if possible.
            SetUrgencyHint(top_window, false);
        }
    }

    RefPtr<nsWindow> owningWindow = get_window_for_gtk_widget(owningWidget);
    if (!owningWindow)
        return NS_ERROR_FAILURE;
=======
  GtkWidget* owningWidget = GetMozContainerWidget();
  if (!owningWidget) return;

  // Raise the window if someone passed in true and the prefs are
  // set properly.
  GtkWidget* toplevelWidget = gtk_widget_get_toplevel(owningWidget);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (gRaiseWindows && aRaise && toplevelWidget &&
      !gtk_widget_has_focus(owningWidget) &&
      !gtk_widget_has_focus(toplevelWidget)) {
    GtkWidget *top_window = GetToplevelWidget();
    if (top_window && (gtk_widget_get_visible(top_window))) {
      gdk_window_show_unraised(gtk_widget_get_window(top_window));
      // Unset the urgency hint if possible.
      SetUrgencyHint(top_window, false);
    }
  }
||||||| merged common ancestors
    if (aRaise) {
        // aRaise == true means request toplevel activation.
=======
  if (gRaiseWindows && aRaise == Raise::Yes && toplevelWidget &&
      !gtk_widget_has_focus(owningWidget) &&
      !gtk_widget_has_focus(toplevelWidget)) {
    GtkWidget* top_window = GetToplevelWidget();
    if (top_window && (gtk_widget_get_visible(top_window))) {
      gdk_window_show_unraised(gtk_widget_get_window(top_window));
      // Unset the urgency hint if possible.
      SetUrgencyHint(top_window, false);
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RefPtr<nsWindow> owningWindow = get_window_for_gtk_widget(owningWidget);
  if (!owningWindow) return NS_ERROR_FAILURE;
||||||| merged common ancestors
        // This is asynchronous.
        // If and when the window manager accepts the request, then the focus
        // widget will get a focus-in-event signal.
        if (gRaiseWindows && owningWindow->mIsShown && owningWindow->mShell &&
            !gtk_window_is_active(GTK_WINDOW(owningWindow->mShell))) {
=======
  RefPtr<nsWindow> owningWindow = get_window_for_gtk_widget(owningWidget);
  if (!owningWindow) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (aRaise) {
    // aRaise == true means request toplevel activation.
||||||| merged common ancestors
            uint32_t timestamp = GDK_CURRENT_TIME;
=======
  if (aRaise == Raise::Yes) {
    // means request toplevel activation.
>>>>>>> upstream-releases

    // This is asynchronous.
    // If and when the window manager accepts the request, then the focus
    // widget will get a focus-in-event signal.
    if (gRaiseWindows && owningWindow->mIsShown && owningWindow->mShell &&
        !gtk_window_is_active(GTK_WINDOW(owningWindow->mShell))) {
      uint32_t timestamp = GDK_CURRENT_TIME;

<<<<<<< HEAD
      nsGTKToolkit *GTKToolkit = nsGTKToolkit::GetToolkit();
      if (GTKToolkit) timestamp = GTKToolkit->GetFocusTimestamp();
||||||| merged common ancestors
            LOGFOCUS(("  requesting toplevel activation [%p]\n", (void *)this));
            NS_ASSERTION(owningWindow->mWindowType != eWindowType_popup
                         || mParent,
                         "Presenting an override-redirect window");
            gtk_window_present_with_time(GTK_WINDOW(owningWindow->mShell), timestamp);
=======
      nsGTKToolkit* GTKToolkit = nsGTKToolkit::GetToolkit();
      if (GTKToolkit) timestamp = GTKToolkit->GetFocusTimestamp();
>>>>>>> upstream-releases

<<<<<<< HEAD
      LOGFOCUS(("  requesting toplevel activation [%p]\n", (void *)this));
      NS_ASSERTION(owningWindow->mWindowType != eWindowType_popup || mParent,
                   "Presenting an override-redirect window");
      gtk_window_present_with_time(GTK_WINDOW(owningWindow->mShell), timestamp);
||||||| merged common ancestors
            if (GTKToolkit)
                GTKToolkit->SetFocusTimestamp(0);
        }
=======
      LOGFOCUS(("  requesting toplevel activation [%p]\n", (void*)this));
      NS_ASSERTION(owningWindow->mWindowType != eWindowType_popup || mParent,
                   "Presenting an override-redirect window");
      gtk_window_present_with_time(GTK_WINDOW(owningWindow->mShell), timestamp);
>>>>>>> upstream-releases

      if (GTKToolkit) GTKToolkit->SetFocusTimestamp(0);
    }
    return;
  }

<<<<<<< HEAD
    return NS_OK;
  }

  // aRaise == false means that keyboard events should be dispatched
  // from this widget.

  // Ensure owningWidget is the focused GtkWidget within its toplevel window.
  //
  // For eWindowType_popup, this GtkWidget may not actually be the one that
  // receives the key events as it may be the parent window that is active.
  if (!gtk_widget_is_focus(owningWidget)) {
    // This is synchronous.  It takes focus from a plugin or from a widget
    // in an embedder.  The focus manager already knows that this window
    // is active so gBlockActivateEvent avoids another (unnecessary)
    // activate notification.
    gBlockActivateEvent = true;
    gtk_widget_grab_focus(owningWidget);
    gBlockActivateEvent = false;
  }
||||||| merged common ancestors
    // aRaise == false means that keyboard events should be dispatched
    // from this widget.

    // Ensure owningWidget is the focused GtkWidget within its toplevel window.
    //
    // For eWindowType_popup, this GtkWidget may not actually be the one that
    // receives the key events as it may be the parent window that is active.
    if (!gtk_widget_is_focus(owningWidget)) {
        // This is synchronous.  It takes focus from a plugin or from a widget
        // in an embedder.  The focus manager already knows that this window
        // is active so gBlockActivateEvent avoids another (unnecessary)
        // activate notification.
        gBlockActivateEvent = true;
        gtk_widget_grab_focus(owningWidget);
        gBlockActivateEvent = false;
    }
=======
  // aRaise == No means that keyboard events should be dispatched from this
  // widget.

  // Ensure owningWidget is the focused GtkWidget within its toplevel window.
  //
  // For eWindowType_popup, this GtkWidget may not actually be the one that
  // receives the key events as it may be the parent window that is active.
  if (!gtk_widget_is_focus(owningWidget)) {
    // This is synchronous.  It takes focus from a plugin or from a widget
    // in an embedder.  The focus manager already knows that this window
    // is active so gBlockActivateEvent avoids another (unnecessary)
    // activate notification.
    gBlockActivateEvent = true;
    gtk_widget_grab_focus(owningWidget);
    gBlockActivateEvent = false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If this is the widget that already has focus, return.
  if (gFocusWindow == this) {
    LOGFOCUS(("  already have focus [%p]\n", (void *)this));
    return NS_OK;
  }
||||||| merged common ancestors
    // If this is the widget that already has focus, return.
    if (gFocusWindow == this) {
        LOGFOCUS(("  already have focus [%p]\n", (void *)this));
        return NS_OK;
    }
=======
  // If this is the widget that already has focus, return.
  if (gFocusWindow == this) {
    LOGFOCUS(("  already have focus [%p]\n", (void*)this));
    return;
  }
>>>>>>> upstream-releases

  // Set this window to be the focused child window
  gFocusWindow = this;

  if (mIMContext) {
    mIMContext->OnFocusWindow(this);
  }

<<<<<<< HEAD
  LOGFOCUS(("  widget now has focus in SetFocus() [%p]\n", (void *)this));
||||||| merged common ancestors
    LOGFOCUS(("  widget now has focus in SetFocus() [%p]\n",
              (void *)this));
=======
  LOGFOCUS(("  widget now has focus in SetFocus() [%p]\n", (void*)this));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return NS_OK;
||||||| merged common ancestors
    return NS_OK;
=======
LayoutDeviceIntRect nsWindow::GetScreenBounds() {
  LayoutDeviceIntRect rect;
  if (mIsTopLevel && mContainer) {
    // use the point including window decorations
    gint x, y;
    gdk_window_get_root_origin(gtk_widget_get_window(GTK_WIDGET(mContainer)),
                               &x, &y);
    rect.MoveTo(GdkPointToDevicePixels({x, y}));
  } else {
    rect.MoveTo(WidgetToScreenOffset());
  }
  // mBounds.Size() is the window bounds, not the window-manager frame
  // bounds (bug 581863).  gdk_window_get_frame_extents would give the
  // frame bounds, but mBounds.Size() is returned here for consistency
  // with Resize.
  rect.SizeTo(mBounds.Size());
  LOG(("GetScreenBounds %d,%d | %dx%d\n", rect.x, rect.y, rect.width,
       rect.height));
  return rect;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
LayoutDeviceIntRect nsWindow::GetScreenBounds() {
  LayoutDeviceIntRect rect;
  if (mIsTopLevel && mContainer) {
    // use the point including window decorations
    gint x, y;
    gdk_window_get_root_origin(gtk_widget_get_window(GTK_WIDGET(mContainer)),
                               &x, &y);
    rect.MoveTo(GdkPointToDevicePixels({x, y}));
  } else {
    rect.MoveTo(WidgetToScreenOffset());
  }
  // mBounds.Size() is the window bounds, not the window-manager frame
  // bounds (bug 581863).  gdk_window_get_frame_extents would give the
  // frame bounds, but mBounds.Size() is returned here for consistency
  // with Resize.
  rect.SizeTo(mBounds.Size());
  LOG(("GetScreenBounds %d,%d | %dx%d\n", rect.x, rect.y, rect.width,
       rect.height));
  return rect;
||||||| merged common ancestors
LayoutDeviceIntRect
nsWindow::GetScreenBounds()
{
    LayoutDeviceIntRect rect;
    if (mIsTopLevel && mContainer) {
        // use the point including window decorations
        gint x, y;
        gdk_window_get_root_origin(gtk_widget_get_window(GTK_WIDGET(mContainer)), &x, &y);
        rect.MoveTo(GdkPointToDevicePixels({ x, y }));
    } else {
        rect.MoveTo(WidgetToScreenOffset());
    }
    // mBounds.Size() is the window bounds, not the window-manager frame
    // bounds (bug 581863).  gdk_window_get_frame_extents would give the
    // frame bounds, but mBounds.Size() is returned here for consistency
    // with Resize.
    rect.SizeTo(mBounds.Size());
    LOG(("GetScreenBounds %d,%d | %dx%d\n",
         rect.x, rect.y, rect.width, rect.height));
    return rect;
}

LayoutDeviceIntSize
nsWindow::GetClientSize()
{
  return LayoutDeviceIntSize(mBounds.width, mBounds.height);
=======
LayoutDeviceIntSize nsWindow::GetClientSize() {
  return LayoutDeviceIntSize(mBounds.width, mBounds.height);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
LayoutDeviceIntSize nsWindow::GetClientSize() {
  return LayoutDeviceIntSize(mBounds.width, mBounds.height);
||||||| merged common ancestors
LayoutDeviceIntRect
nsWindow::GetClientBounds()
{
    // GetBounds returns a rect whose top left represents the top left of the
    // outer bounds, but whose width/height represent the size of the inner
    // bounds (which is messed up).
    LayoutDeviceIntRect rect = GetBounds();
    rect.MoveBy(GetClientOffset());
    return rect;
=======
LayoutDeviceIntRect nsWindow::GetClientBounds() {
  // GetBounds returns a rect whose top left represents the top left of the
  // outer bounds, but whose width/height represent the size of the inner
  // bounds (which is messed up).
  LayoutDeviceIntRect rect = GetBounds();
  rect.MoveBy(GetClientOffset());
  return rect;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
LayoutDeviceIntRect nsWindow::GetClientBounds() {
  // GetBounds returns a rect whose top left represents the top left of the
  // outer bounds, but whose width/height represent the size of the inner
  // bounds (which is messed up).
  LayoutDeviceIntRect rect = GetBounds();
  rect.MoveBy(GetClientOffset());
  return rect;
}

void nsWindow::UpdateClientOffset() {
  AUTO_PROFILER_LABEL("nsWindow::UpdateClientOffset", OTHER);
||||||| merged common ancestors
void
nsWindow::UpdateClientOffset()
{
    AUTO_PROFILER_LABEL("nsWindow::UpdateClientOffset", OTHER);

    if (!mIsTopLevel || !mShell || !mIsX11Display ||
        gtk_window_get_window_type(GTK_WINDOW(mShell)) == GTK_WINDOW_POPUP) {
        mClientOffset = nsIntPoint(0, 0);
        return;
    }
=======
void nsWindow::UpdateClientOffset() {
  AUTO_PROFILER_LABEL("nsWindow::UpdateClientOffset", OTHER);
>>>>>>> upstream-releases

  if (!mIsTopLevel || !mShell || !mIsX11Display ||
      gtk_window_get_window_type(GTK_WINDOW(mShell)) == GTK_WINDOW_POPUP) {
    mClientOffset = nsIntPoint(0, 0);
    return;
  }

<<<<<<< HEAD
  GdkAtom cardinal_atom = gdk_x11_xatom_to_atom(XA_CARDINAL);

  GdkAtom type_returned;
  int format_returned;
  int length_returned;
  long *frame_extents;

  if (!gdk_property_get(gtk_widget_get_window(mShell),
                        gdk_atom_intern("_NET_FRAME_EXTENTS", FALSE),
                        cardinal_atom,
                        0,      // offset
                        4 * 4,  // length
                        FALSE,  // delete
                        &type_returned, &format_returned, &length_returned,
                        (guchar **)&frame_extents) ||
      length_returned / sizeof(glong) != 4) {
    mClientOffset = nsIntPoint(0, 0);
    return;
  }
||||||| merged common ancestors
    GdkAtom type_returned;
    int format_returned;
    int length_returned;
    long *frame_extents;

    if (!gdk_property_get(gtk_widget_get_window(mShell),
                          gdk_atom_intern ("_NET_FRAME_EXTENTS", FALSE),
                          cardinal_atom,
                          0, // offset
                          4*4, // length
                          FALSE, // delete
                          &type_returned,
                          &format_returned,
                          &length_returned,
                          (guchar **) &frame_extents) ||
        length_returned/sizeof(glong) != 4) {
        mClientOffset = nsIntPoint(0, 0);
        return;
    }
=======
  GdkAtom cardinal_atom = gdk_x11_xatom_to_atom(XA_CARDINAL);

  GdkAtom type_returned;
  int format_returned;
  int length_returned;
  long* frame_extents;

  if (!gdk_property_get(gtk_widget_get_window(mShell),
                        gdk_atom_intern("_NET_FRAME_EXTENTS", FALSE),
                        cardinal_atom,
                        0,      // offset
                        4 * 4,  // length
                        FALSE,  // delete
                        &type_returned, &format_returned, &length_returned,
                        (guchar**)&frame_extents) ||
      length_returned / sizeof(glong) != 4) {
    mClientOffset = nsIntPoint(0, 0);
    return;
  }
>>>>>>> upstream-releases

  // data returned is in the order left, right, top, bottom
  auto left = int32_t(frame_extents[0]);
  auto top = int32_t(frame_extents[2]);

  g_free(frame_extents);

  mClientOffset = nsIntPoint(left, top);
}

LayoutDeviceIntPoint nsWindow::GetClientOffset() {
  return LayoutDeviceIntPoint::FromUnknownPoint(mClientOffset);
}

<<<<<<< HEAD
gboolean nsWindow::OnPropertyNotifyEvent(GtkWidget *aWidget,
                                         GdkEventProperty *aEvent) {
||||||| merged common ancestors
gboolean
nsWindow::OnPropertyNotifyEvent(GtkWidget* aWidget, GdkEventProperty* aEvent)
{
=======
gboolean nsWindow::OnPropertyNotifyEvent(GtkWidget* aWidget,
                                         GdkEventProperty* aEvent) {
>>>>>>> upstream-releases
  if (aEvent->atom == gdk_atom_intern("_NET_FRAME_EXTENTS", FALSE)) {
    UpdateClientOffset();

    // Send a WindowMoved notification. This ensures that BrowserParent
    // picks up the new client offset and sends it to the child process
    // if appropriate.
    NotifyWindowMoved(mBounds.x, mBounds.y);
    return FALSE;
  }

  if (GetCurrentTimeGetter()->PropertyNotifyHandler(aWidget, aEvent)) {
    return TRUE;
  }

  return FALSE;
}

<<<<<<< HEAD
void nsWindow::SetCursor(nsCursor aCursor) {
  // if we're not the toplevel window pass up the cursor request to
  // the toplevel window to handle it.
  if (!mContainer && mGdkWindow) {
    nsWindow *window = GetContainerWindow();
    if (!window) return;

    window->SetCursor(aCursor);
    return;
  }
||||||| merged common ancestors
void
nsWindow::SetCursor(nsCursor aCursor)
{
    // if we're not the toplevel window pass up the cursor request to
    // the toplevel window to handle it.
    if (!mContainer && mGdkWindow) {
        nsWindow *window = GetContainerWindow();
        if (!window)
            return;

        window->SetCursor(aCursor);
        return;
    }

    // Only change cursor if it's actually been changed
    if (aCursor != mCursor || mUpdateCursor) {
        GdkCursor *newCursor = nullptr;
        mUpdateCursor = false;
=======
static GdkCursor* GetCursorForImage(imgIContainer* aCursorImage,
                                    uint32_t aHotspotX, uint32_t aHotspotY) {
  if (!aCursorImage) {
    return nullptr;
  }
  GdkPixbuf* pixbuf = nsImageToPixbuf::ImageToPixbuf(aCursorImage);
  if (!pixbuf) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Only change cursor if it's actually been changed
  if (aCursor != mCursor || mUpdateCursor) {
    GdkCursor *newCursor = nullptr;
    mUpdateCursor = false;
||||||| merged common ancestors
        newCursor = get_gtk_cursor(aCursor);
=======
  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);
>>>>>>> upstream-releases

<<<<<<< HEAD
    newCursor = get_gtk_cursor(aCursor);
||||||| merged common ancestors
        if (nullptr != newCursor) {
            mCursor = aCursor;
=======
  auto CleanupPixBuf =
      mozilla::MakeScopeExit([&]() { g_object_unref(pixbuf); });
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (nullptr != newCursor) {
      mCursor = aCursor;
||||||| merged common ancestors
            if (!mContainer)
                return;
=======
  // Reject cursors greater than 128 pixels in some direction, to prevent
  // spoofing.
  // XXX ideally we should rescale. Also, we could modify the API to
  // allow trusted content to set larger cursors.
  //
  // TODO(emilio, bug 1445844): Unify the solution for this with other
  // platforms.
  if (width > 128 || height > 128) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (!mContainer) return;

      gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(mContainer)),
                            newCursor);
||||||| merged common ancestors
            gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(mContainer)), newCursor);
        }
=======
  // Looks like all cursors need an alpha channel (tested on Gtk 2.4.4). This
  // is of course not documented anywhere...
  // So add one if there isn't one yet
  if (!gdk_pixbuf_get_has_alpha(pixbuf)) {
    GdkPixbuf* alphaBuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);
    g_object_unref(pixbuf);
    pixbuf = alphaBuf;
    if (!alphaBuf) {
      return nullptr;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
  }

  return gdk_cursor_new_from_pixbuf(gdk_display_get_default(), pixbuf,
                                    aHotspotX, aHotspotY);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsWindow::SetCursor(imgIContainer *aCursor, uint32_t aHotspotX,
                             uint32_t aHotspotY) {
  // if we're not the toplevel window pass up the cursor request to
  // the toplevel window to handle it.
  if (!mContainer && mGdkWindow) {
    nsWindow *window = GetContainerWindow();
    if (!window) return NS_ERROR_FAILURE;
||||||| merged common ancestors
nsresult
nsWindow::SetCursor(imgIContainer* aCursor,
                    uint32_t aHotspotX, uint32_t aHotspotY)
{
    // if we're not the toplevel window pass up the cursor request to
    // the toplevel window to handle it.
    if (!mContainer && mGdkWindow) {
        nsWindow *window = GetContainerWindow();
        if (!window)
            return NS_ERROR_FAILURE;

        return window->SetCursor(aCursor, aHotspotX, aHotspotY);
    }
=======
void nsWindow::SetCursor(nsCursor aDefaultCursor, imgIContainer* aCursorImage,
                         uint32_t aHotspotX, uint32_t aHotspotY) {
  // if we're not the toplevel window pass up the cursor request to
  // the toplevel window to handle it.
  if (!mContainer && mGdkWindow) {
    nsWindow* window = GetContainerWindow();
    if (!window) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
    return window->SetCursor(aCursor, aHotspotX, aHotspotY);
  }
||||||| merged common ancestors
    mCursor = eCursorInvalid;

    // Get the image's current frame
    GdkPixbuf* pixbuf = nsImageToPixbuf::ImageToPixbuf(aCursor);
    if (!pixbuf)
        return NS_ERROR_NOT_AVAILABLE;

    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);
    // Reject cursors greater than 128 pixels in some direction, to prevent
    // spoofing.
    // XXX ideally we should rescale. Also, we could modify the API to
    // allow trusted content to set larger cursors.
    if (width > 128 || height > 128) {
        g_object_unref(pixbuf);
        return NS_ERROR_NOT_AVAILABLE;
    }
=======
    window->SetCursor(aDefaultCursor, aCursorImage, aHotspotX, aHotspotY);
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  mCursor = eCursorInvalid;
||||||| merged common ancestors
    // Looks like all cursors need an alpha channel (tested on Gtk 2.4.4). This
    // is of course not documented anywhere...
    // So add one if there isn't one yet
    if (!gdk_pixbuf_get_has_alpha(pixbuf)) {
        GdkPixbuf* alphaBuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);
        g_object_unref(pixbuf);
        if (!alphaBuf) {
            return NS_ERROR_OUT_OF_MEMORY;
        }
        pixbuf = alphaBuf;
    }
=======
  // Only change cursor if it's actually been changed
  if (!aCursorImage && aDefaultCursor == mCursor && !mUpdateCursor) {
    return;
  }

  mUpdateCursor = false;
  mCursor = eCursorInvalid;

  // Try to set the cursor image first, and fall back to the numeric cursor.
  GdkCursor* newCursor = GetCursorForImage(aCursorImage, aHotspotX, aHotspotY);
  if (!newCursor) {
    newCursor = get_gtk_cursor(aDefaultCursor);
    if (newCursor) {
      mCursor = aDefaultCursor;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Get the image's current frame
  GdkPixbuf *pixbuf = nsImageToPixbuf::ImageToPixbuf(aCursor);
  if (!pixbuf) return NS_ERROR_NOT_AVAILABLE;

  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);
  // Reject cursors greater than 128 pixels in some direction, to prevent
  // spoofing.
  // XXX ideally we should rescale. Also, we could modify the API to
  // allow trusted content to set larger cursors.
  if (width > 128 || height > 128) {
    g_object_unref(pixbuf);
    return NS_ERROR_NOT_AVAILABLE;
  }

  // Looks like all cursors need an alpha channel (tested on Gtk 2.4.4). This
  // is of course not documented anywhere...
  // So add one if there isn't one yet
  if (!gdk_pixbuf_get_has_alpha(pixbuf)) {
    GdkPixbuf *alphaBuf = gdk_pixbuf_add_alpha(pixbuf, FALSE, 0, 0, 0);
    g_object_unref(pixbuf);
    if (!alphaBuf) {
      return NS_ERROR_OUT_OF_MEMORY;
    }
    pixbuf = alphaBuf;
  }

  GdkCursor *cursor = gdk_cursor_new_from_pixbuf(gdk_display_get_default(),
                                                 pixbuf, aHotspotX, aHotspotY);
  g_object_unref(pixbuf);
  nsresult rv = NS_ERROR_OUT_OF_MEMORY;
  if (cursor) {
    if (mContainer) {
      gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(mContainer)),
                            cursor);
      rv = NS_OK;
||||||| merged common ancestors
    GdkCursor* cursor = gdk_cursor_new_from_pixbuf(gdk_display_get_default(),
                                                   pixbuf,
                                                   aHotspotX, aHotspotY);
    g_object_unref(pixbuf);
    nsresult rv = NS_ERROR_OUT_OF_MEMORY;
    if (cursor) {
        if (mContainer) {
            gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(mContainer)), cursor);
            rv = NS_OK;
        }
        g_object_unref(cursor);
=======
  auto CleanupCursor = mozilla::MakeScopeExit([&]() {
    // get_gtk_cursor returns a weak reference, which we shouldn't unref.
    if (newCursor && mCursor == eCursorInvalid) {
      g_object_unref(newCursor);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    g_object_unref(cursor);
  }
||||||| merged common ancestors
=======
  });

  if (!newCursor || !mContainer) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return rv;
||||||| merged common ancestors
    return rv;
=======
  gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(mContainer)),
                        newCursor);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsWindow::Invalidate(const LayoutDeviceIntRect &aRect) {
  if (!mGdkWindow) return;
||||||| merged common ancestors
void
nsWindow::Invalidate(const LayoutDeviceIntRect& aRect)
{
    if (!mGdkWindow)
        return;
=======
void nsWindow::Invalidate(const LayoutDeviceIntRect& aRect) {
  if (!mGdkWindow) return;
>>>>>>> upstream-releases

  GdkRectangle rect = DevicePixelsToGdkRectRoundOut(aRect);
  gdk_window_invalidate_rect(mGdkWindow, &rect, FALSE);

<<<<<<< HEAD
  LOGDRAW(("Invalidate (rect) [%p]: %d %d %d %d\n", (void *)this, rect.x,
           rect.y, rect.width, rect.height));
||||||| merged common ancestors
    LOGDRAW(("Invalidate (rect) [%p]: %d %d %d %d\n", (void *)this,
             rect.x, rect.y, rect.width, rect.height));
=======
  LOGDRAW(("Invalidate (rect) [%p]: %d %d %d %d\n", (void*)this, rect.x, rect.y,
           rect.width, rect.height));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void *nsWindow::GetNativeData(uint32_t aDataType) {
  switch (aDataType) {
||||||| merged common ancestors
void*
nsWindow::GetNativeData(uint32_t aDataType)
{
    switch (aDataType) {
=======
void* nsWindow::GetNativeData(uint32_t aDataType) {
  switch (aDataType) {
>>>>>>> upstream-releases
    case NS_NATIVE_WINDOW:
    case NS_NATIVE_WIDGET: {
      if (!mGdkWindow) return nullptr;

      return mGdkWindow;
    }

    case NS_NATIVE_DISPLAY: {
#ifdef MOZ_X11
<<<<<<< HEAD
      GdkDisplay *gdkDisplay = gdk_display_get_default();
      if (GDK_IS_X11_DISPLAY(gdkDisplay)) {
        return GDK_DISPLAY_XDISPLAY(gdkDisplay);
      }
||||||| merged common ancestors
        GdkDisplay* gdkDisplay = gdk_display_get_default();
        if (GDK_IS_X11_DISPLAY(gdkDisplay)) {
            return GDK_DISPLAY_XDISPLAY(gdkDisplay);
        }
=======
      GdkDisplay* gdkDisplay = gdk_display_get_default();
      if (GDK_IS_X11_DISPLAY(gdkDisplay)) {
        return GDK_DISPLAY_XDISPLAY(gdkDisplay);
      }
>>>>>>> upstream-releases
#endif /* MOZ_X11 */
      // Don't bother to return native display on Wayland as it's for
      // X11 only NPAPI plugins.
      return nullptr;
    }
    case NS_NATIVE_SHELLWIDGET:
      return GetToplevelWidget();

    case NS_NATIVE_SHAREABLE_WINDOW:
<<<<<<< HEAD
      if (mIsX11Display) {
        return (void *)GDK_WINDOW_XID(gdk_window_get_toplevel(mGdkWindow));
      }
      NS_WARNING(
          "nsWindow::GetNativeData(): NS_NATIVE_SHAREABLE_WINDOW is not "
          "handled on Wayland!");
      return nullptr;
||||||| merged common ancestors
        if (mIsX11Display) {
            return (void *) GDK_WINDOW_XID(gdk_window_get_toplevel(mGdkWindow));
        }
        NS_WARNING("nsWindow::GetNativeData(): NS_NATIVE_SHAREABLE_WINDOW is not handled on Wayland!");
        return nullptr;
=======
      if (mIsX11Display) {
        return (void*)GDK_WINDOW_XID(gdk_window_get_toplevel(mGdkWindow));
      }
      NS_WARNING(
          "nsWindow::GetNativeData(): NS_NATIVE_SHAREABLE_WINDOW is not "
          "handled on Wayland!");
      return nullptr;
>>>>>>> upstream-releases
    case NS_RAW_NATIVE_IME_CONTEXT: {
<<<<<<< HEAD
      void *pseudoIMEContext = GetPseudoIMEContext();
      if (pseudoIMEContext) {
        return pseudoIMEContext;
      }
      // If IME context isn't available on this widget, we should set |this|
      // instead of nullptr.
      if (!mIMContext) {
        return this;
      }
      return mIMContext.get();
||||||| merged common ancestors
        void* pseudoIMEContext = GetPseudoIMEContext();
        if (pseudoIMEContext) {
            return pseudoIMEContext;
        }
        // If IME context isn't available on this widget, we should set |this|
        // instead of nullptr.
        if (!mIMContext) {
            return this;
        }
        return mIMContext.get();
=======
      void* pseudoIMEContext = GetPseudoIMEContext();
      if (pseudoIMEContext) {
        return pseudoIMEContext;
      }
      // If IME context isn't available on this widget, we should set |this|
      // instead of nullptr.
      if (!mIMContext) {
        return this;
      }
      return mIMContext.get();
>>>>>>> upstream-releases
    }
    case NS_NATIVE_OPENGL_CONTEXT:
      return nullptr;
#ifdef MOZ_X11
    case NS_NATIVE_COMPOSITOR_DISPLAY:
      return gfxPlatformGtk::GetPlatform()->GetCompositorDisplay();
#endif  // MOZ_X11
    case NS_NATIVE_EGL_WINDOW: {
<<<<<<< HEAD
      if (mIsX11Display)
        return mGdkWindow ? (void *)GDK_WINDOW_XID(mGdkWindow) : nullptr;
||||||| merged common ancestors
        if (mIsX11Display)
            return mGdkWindow ? (void*)GDK_WINDOW_XID(mGdkWindow) : nullptr;
=======
      if (mIsX11Display)
        return mGdkWindow ? (void*)GDK_WINDOW_XID(mGdkWindow) : nullptr;
>>>>>>> upstream-releases
#ifdef MOZ_WAYLAND
      if (mContainer) return moz_container_get_wl_egl_window(mContainer);
#endif
      return nullptr;
    }
    default:
      NS_WARNING("nsWindow::GetNativeData called with bad value");
      return nullptr;
  }
}

<<<<<<< HEAD
nsresult nsWindow::SetTitle(const nsAString &aTitle) {
  if (!mShell) return NS_OK;
||||||| merged common ancestors
nsresult
nsWindow::SetTitle(const nsAString& aTitle)
{
    if (!mShell)
        return NS_OK;
=======
nsresult nsWindow::SetTitle(const nsAString& aTitle) {
  if (!mShell) return NS_OK;
>>>>>>> upstream-releases

    // convert the string into utf8 and set the title.
<<<<<<< HEAD
#define UTF8_FOLLOWBYTE(ch) (((ch)&0xC0) == 0x80)
  NS_ConvertUTF16toUTF8 titleUTF8(aTitle);
  if (titleUTF8.Length() > NS_WINDOW_TITLE_MAX_LENGTH) {
    // Truncate overlong titles (bug 167315). Make sure we chop after a
    // complete sequence by making sure the next char isn't a follow-byte.
    uint32_t len = NS_WINDOW_TITLE_MAX_LENGTH;
    while (UTF8_FOLLOWBYTE(titleUTF8[len])) --len;
    titleUTF8.Truncate(len);
  }
  gtk_window_set_title(GTK_WINDOW(mShell), (const char *)titleUTF8.get());
||||||| merged common ancestors
#define UTF8_FOLLOWBYTE(ch) (((ch) & 0xC0) == 0x80)
    NS_ConvertUTF16toUTF8 titleUTF8(aTitle);
    if (titleUTF8.Length() > NS_WINDOW_TITLE_MAX_LENGTH) {
        // Truncate overlong titles (bug 167315). Make sure we chop after a
        // complete sequence by making sure the next char isn't a follow-byte.
        uint32_t len = NS_WINDOW_TITLE_MAX_LENGTH;
        while(UTF8_FOLLOWBYTE(titleUTF8[len]))
            --len;
        titleUTF8.Truncate(len);
    }
    gtk_window_set_title(GTK_WINDOW(mShell), (const char *)titleUTF8.get());
=======
#define UTF8_FOLLOWBYTE(ch) (((ch)&0xC0) == 0x80)
  NS_ConvertUTF16toUTF8 titleUTF8(aTitle);
  if (titleUTF8.Length() > NS_WINDOW_TITLE_MAX_LENGTH) {
    // Truncate overlong titles (bug 167315). Make sure we chop after a
    // complete sequence by making sure the next char isn't a follow-byte.
    uint32_t len = NS_WINDOW_TITLE_MAX_LENGTH;
    while (UTF8_FOLLOWBYTE(titleUTF8[len])) --len;
    titleUTF8.Truncate(len);
  }
  gtk_window_set_title(GTK_WINDOW(mShell), (const char*)titleUTF8.get());
>>>>>>> upstream-releases

  return NS_OK;
}

<<<<<<< HEAD
void nsWindow::SetIcon(const nsAString &aIconSpec) {
  if (!mShell) return;
||||||| merged common ancestors
void
nsWindow::SetIcon(const nsAString& aIconSpec)
{
    if (!mShell)
        return;
=======
void nsWindow::SetIcon(const nsAString& aIconSpec) {
  if (!mShell) return;
>>>>>>> upstream-releases

  nsAutoCString iconName;

  if (aIconSpec.EqualsLiteral("default")) {
    nsAutoString brandName;
    WidgetUtils::GetBrandShortName(brandName);
    if (brandName.IsEmpty()) {
      brandName.AssignLiteral(u"Mozilla");
    }
    AppendUTF16toUTF8(brandName, iconName);
    ToLowerCase(iconName);
  } else {
    AppendUTF16toUTF8(aIconSpec, iconName);
  }

<<<<<<< HEAD
  nsCOMPtr<nsIFile> iconFile;
  nsAutoCString path;

  gint *iconSizes = gtk_icon_theme_get_icon_sizes(gtk_icon_theme_get_default(),
                                                  iconName.get());
  bool foundIcon = (iconSizes[0] != 0);
  g_free(iconSizes);

  if (!foundIcon) {
    // Look for icons with the following suffixes appended to the base name
    // The last two entries (for the old XPM format) will be ignored unless
    // no icons are found using other suffixes. XPM icons are deprecated.

    const char16_t extensions[9][8] = {u".png",    u"16.png", u"32.png",
                                       u"48.png",  u"64.png", u"128.png",
                                       u"256.png", u".xpm",   u"16.xpm"};

    for (uint32_t i = 0; i < ArrayLength(extensions); i++) {
      // Don't bother looking for XPM versions if we found a PNG.
      if (i == ArrayLength(extensions) - 2 && foundIcon) break;

      ResolveIconName(aIconSpec, nsDependentString(extensions[i]),
                      getter_AddRefs(iconFile));
      if (iconFile) {
        iconFile->GetNativePath(path);
        GdkPixbuf *icon = gdk_pixbuf_new_from_file(path.get(), nullptr);
        if (icon) {
          gtk_icon_theme_add_builtin_icon(iconName.get(),
                                          gdk_pixbuf_get_height(icon), icon);
          g_object_unref(icon);
          foundIcon = true;
||||||| merged common ancestors
    nsCOMPtr<nsIFile> iconFile;
    nsAutoCString path;

    gint *iconSizes =
        gtk_icon_theme_get_icon_sizes(gtk_icon_theme_get_default(),
                                      iconName.get());
    bool foundIcon = (iconSizes[0] != 0);
    g_free(iconSizes);

    if (!foundIcon) {
        // Look for icons with the following suffixes appended to the base name
        // The last two entries (for the old XPM format) will be ignored unless
        // no icons are found using other suffixes. XPM icons are deprecated.

        const char16_t extensions[9][8] = { u".png", u"16.png", u"32.png",
                                            u"48.png", u"64.png", u"128.png",
                                            u"256.png",
                                            u".xpm", u"16.xpm" };

        for (uint32_t i = 0; i < ArrayLength(extensions); i++) {
            // Don't bother looking for XPM versions if we found a PNG.
            if (i == ArrayLength(extensions) - 2 && foundIcon)
                break;

            ResolveIconName(aIconSpec, nsDependentString(extensions[i]),
                            getter_AddRefs(iconFile));
            if (iconFile) {
                iconFile->GetNativePath(path);
                GdkPixbuf *icon = gdk_pixbuf_new_from_file(path.get(), nullptr);
                if (icon) {
                    gtk_icon_theme_add_builtin_icon(iconName.get(),
                                                    gdk_pixbuf_get_height(icon),
                                                    icon);
                    g_object_unref(icon);
                    foundIcon = true;
                }
            }
=======
  nsCOMPtr<nsIFile> iconFile;
  nsAutoCString path;

  gint* iconSizes = gtk_icon_theme_get_icon_sizes(gtk_icon_theme_get_default(),
                                                  iconName.get());
  bool foundIcon = (iconSizes[0] != 0);
  g_free(iconSizes);

  if (!foundIcon) {
    // Look for icons with the following suffixes appended to the base name
    // The last two entries (for the old XPM format) will be ignored unless
    // no icons are found using other suffixes. XPM icons are deprecated.

    const char16_t extensions[9][8] = {u".png",    u"16.png", u"32.png",
                                       u"48.png",  u"64.png", u"128.png",
                                       u"256.png", u".xpm",   u"16.xpm"};

    for (uint32_t i = 0; i < ArrayLength(extensions); i++) {
      // Don't bother looking for XPM versions if we found a PNG.
      if (i == ArrayLength(extensions) - 2 && foundIcon) break;

      ResolveIconName(aIconSpec, nsDependentString(extensions[i]),
                      getter_AddRefs(iconFile));
      if (iconFile) {
        iconFile->GetNativePath(path);
        GdkPixbuf* icon = gdk_pixbuf_new_from_file(path.get(), nullptr);
        if (icon) {
          gtk_icon_theme_add_builtin_icon(iconName.get(),
                                          gdk_pixbuf_get_height(icon), icon);
          g_object_unref(icon);
          foundIcon = true;
>>>>>>> upstream-releases
        }
      }
    }
  }

  // leave the default icon intact if no matching icons were found
  if (foundIcon) {
    gtk_window_set_icon_name(GTK_WINDOW(mShell), iconName.get());
  }
}

LayoutDeviceIntPoint nsWindow::WidgetToScreenOffset() {
  gint x = 0, y = 0;

  if (mGdkWindow) {
    gdk_window_get_origin(mGdkWindow, &x, &y);
  }

  return GdkPointToDevicePixels({x, y});
}

<<<<<<< HEAD
void nsWindow::CaptureMouse(bool aCapture) {
  LOG(("CaptureMouse %p\n", (void *)this));
||||||| merged common ancestors
void
nsWindow::CaptureMouse(bool aCapture)
{
    LOG(("CaptureMouse %p\n", (void *)this));
=======
void nsWindow::CaptureMouse(bool aCapture) {
  LOG(("CaptureMouse %p\n", (void*)this));
>>>>>>> upstream-releases

  if (!mGdkWindow) return;

  if (!mContainer) return;

  if (aCapture) {
    gtk_grab_add(GTK_WIDGET(mContainer));
    GrabPointer(GetLastUserInputTime());
  } else {
    ReleaseGrabs();
    gtk_grab_remove(GTK_WIDGET(mContainer));
  }
}

<<<<<<< HEAD
void nsWindow::CaptureRollupEvents(nsIRollupListener *aListener,
                                   bool aDoCapture) {
  if (!mGdkWindow) return;
||||||| merged common ancestors
void
nsWindow::CaptureRollupEvents(nsIRollupListener *aListener,
                              bool               aDoCapture)
{
    if (!mGdkWindow)
        return;
=======
void nsWindow::CaptureRollupEvents(nsIRollupListener* aListener,
                                   bool aDoCapture) {
  if (!mGdkWindow) return;
>>>>>>> upstream-releases

  if (!mContainer) return;

  LOG(("CaptureRollupEvents %p %i\n", this, int(aDoCapture)));

  if (aDoCapture) {
    gRollupListener = aListener;
    // Don't add a grab if a drag is in progress, or if the widget is a drag
    // feedback popup. (panels with type="drag").
    if (!mIsDragPopup && !nsWindow::DragInProgress()) {
      gtk_grab_add(GTK_WIDGET(mContainer));
      GrabPointer(GetLastUserInputTime());
    }
  } else {
    if (!nsWindow::DragInProgress()) {
      ReleaseGrabs();
    }
    // There may not have been a drag in process when aDoCapture was set,
    // so make sure to remove any added grab.  This is a no-op if the grab
    // was not added to this widget.
    gtk_grab_remove(GTK_WIDGET(mContainer));
    gRollupListener = nullptr;
  }
}

<<<<<<< HEAD
nsresult nsWindow::GetAttention(int32_t aCycleCount) {
  LOG(("nsWindow::GetAttention [%p]\n", (void *)this));
||||||| merged common ancestors
nsresult
nsWindow::GetAttention(int32_t aCycleCount)
{
    LOG(("nsWindow::GetAttention [%p]\n", (void *)this));
=======
nsresult nsWindow::GetAttention(int32_t aCycleCount) {
  LOG(("nsWindow::GetAttention [%p]\n", (void*)this));
>>>>>>> upstream-releases

<<<<<<< HEAD
  GtkWidget *top_window = GetToplevelWidget();
  GtkWidget *top_focused_window =
      gFocusWindow ? gFocusWindow->GetToplevelWidget() : nullptr;
||||||| merged common ancestors
    GtkWidget* top_window = GetToplevelWidget();
    GtkWidget* top_focused_window =
        gFocusWindow ? gFocusWindow->GetToplevelWidget() : nullptr;
=======
  GtkWidget* top_window = GetToplevelWidget();
  GtkWidget* top_focused_window =
      gFocusWindow ? gFocusWindow->GetToplevelWidget() : nullptr;
>>>>>>> upstream-releases

  // Don't get attention if the window is focused anyway.
  if (top_window && (gtk_widget_get_visible(top_window)) &&
      top_window != top_focused_window) {
    SetUrgencyHint(top_window, true);
  }

  return NS_OK;
}

bool nsWindow::HasPendingInputEvent() {
  // This sucks, but gtk/gdk has no way to answer the question we want while
  // excluding paint events, and there's no X API that will let us peek
  // without blocking or removing.  To prevent event reordering, peek
  // anything except expose events.  Reordering expose and others should be
  // ok, hopefully.
  bool haveEvent = false;
#ifdef MOZ_X11
<<<<<<< HEAD
  XEvent ev;
  if (mIsX11Display) {
    Display *display = GDK_DISPLAY_XDISPLAY(gdk_display_get_default());
    haveEvent = XCheckMaskEvent(
        display,
        KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
            EnterWindowMask | LeaveWindowMask | PointerMotionMask |
            PointerMotionHintMask | Button1MotionMask | Button2MotionMask |
            Button3MotionMask | Button4MotionMask | Button5MotionMask |
            ButtonMotionMask | KeymapStateMask | VisibilityChangeMask |
            StructureNotifyMask | ResizeRedirectMask | SubstructureNotifyMask |
            SubstructureRedirectMask | FocusChangeMask | PropertyChangeMask |
            ColormapChangeMask | OwnerGrabButtonMask,
        &ev);
    if (haveEvent) {
      XPutBackEvent(display, &ev);
||||||| merged common ancestors
    XEvent ev;
    if (mIsX11Display) {
        Display *display = GDK_DISPLAY_XDISPLAY(gdk_display_get_default());
        haveEvent =
            XCheckMaskEvent(display,
                            KeyPressMask | KeyReleaseMask | ButtonPressMask |
                            ButtonReleaseMask | EnterWindowMask | LeaveWindowMask |
                            PointerMotionMask | PointerMotionHintMask |
                            Button1MotionMask | Button2MotionMask |
                            Button3MotionMask | Button4MotionMask |
                            Button5MotionMask | ButtonMotionMask | KeymapStateMask |
                            VisibilityChangeMask | StructureNotifyMask |
                            ResizeRedirectMask | SubstructureNotifyMask |
                            SubstructureRedirectMask | FocusChangeMask |
                            PropertyChangeMask | ColormapChangeMask |
                            OwnerGrabButtonMask, &ev);
        if (haveEvent) {
            XPutBackEvent(display, &ev);
        }
=======
  XEvent ev;
  if (mIsX11Display) {
    Display* display = GDK_DISPLAY_XDISPLAY(gdk_display_get_default());
    haveEvent = XCheckMaskEvent(
        display,
        KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
            EnterWindowMask | LeaveWindowMask | PointerMotionMask |
            PointerMotionHintMask | Button1MotionMask | Button2MotionMask |
            Button3MotionMask | Button4MotionMask | Button5MotionMask |
            ButtonMotionMask | KeymapStateMask | VisibilityChangeMask |
            StructureNotifyMask | ResizeRedirectMask | SubstructureNotifyMask |
            SubstructureRedirectMask | FocusChangeMask | PropertyChangeMask |
            ColormapChangeMask | OwnerGrabButtonMask,
        &ev);
    if (haveEvent) {
      XPutBackEvent(display, &ev);
>>>>>>> upstream-releases
    }
  }
#endif
  return haveEvent;
}

#if 0
#  ifdef DEBUG
// Paint flashing code (disabled for cairo - see below)

<<<<<<< HEAD
#define CAPS_LOCK_IS_ON \
  (KeymapWrapper::AreModifiersCurrentlyActive(KeymapWrapper::CAPS_LOCK))
||||||| merged common ancestors
#define CAPS_LOCK_IS_ON \
(KeymapWrapper::AreModifiersCurrentlyActive(KeymapWrapper::CAPS_LOCK))
=======
#    define CAPS_LOCK_IS_ON \
      (KeymapWrapper::AreModifiersCurrentlyActive(KeymapWrapper::CAPS_LOCK))
>>>>>>> upstream-releases

<<<<<<< HEAD
#define WANT_PAINT_FLASHING (debug_WantPaintFlashing() && CAPS_LOCK_IS_ON)
||||||| merged common ancestors
#define WANT_PAINT_FLASHING \
(debug_WantPaintFlashing() && CAPS_LOCK_IS_ON)
=======
#    define WANT_PAINT_FLASHING (debug_WantPaintFlashing() && CAPS_LOCK_IS_ON)
>>>>>>> upstream-releases

#    ifdef MOZ_X11
static void
gdk_window_flash(GdkWindow *    aGdkWindow,
                 unsigned int   aTimes,
                 unsigned int   aInterval,  // Milliseconds
                 GdkRegion *    aRegion)
{
  gint         x;
  gint         y;
  gint         width;
  gint         height;
  guint        i;
  GdkGC *      gc = 0;
  GdkColor     white;

  gdk_window_get_geometry(aGdkWindow,nullptr,nullptr,&width,&height);

  gdk_window_get_origin (aGdkWindow,
                         &x,
                         &y);

  gc = gdk_gc_new(gdk_get_default_root_window());

  white.pixel = WhitePixel(gdk_display,DefaultScreen(gdk_display));

  gdk_gc_set_foreground(gc,&white);
  gdk_gc_set_function(gc,GDK_XOR);
  gdk_gc_set_subwindow(gc,GDK_INCLUDE_INFERIORS);

  gdk_region_offset(aRegion, x, y);
  gdk_gc_set_clip_region(gc, aRegion);

  /*
   * Need to do this twice so that the XOR effect can replace
   * the original window contents.
   */
  for (i = 0; i < aTimes * 2; i++)
  {
    gdk_draw_rectangle(gdk_get_default_root_window(),
                       gc,
                       TRUE,
                       x,
                       y,
                       width,
                       height);

    gdk_flush();

    PR_Sleep(PR_MillisecondsToInterval(aInterval));
  }

  gdk_gc_destroy(gc);

  gdk_region_offset(aRegion, -x, -y);
}
<<<<<<< HEAD
#endif  /* MOZ_X11 */
#endif  // DEBUG
||||||| merged common ancestors
#endif /* MOZ_X11 */
#endif // DEBUG
=======
#    endif /* MOZ_X11 */
#  endif   // DEBUG
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
#ifdef cairo_copy_clip_rectangle_list
#error "Looks like we're including Mozilla's cairo instead of system cairo"
#endif
static bool ExtractExposeRegion(LayoutDeviceIntRegion &aRegion, cairo_t *cr) {
  cairo_rectangle_list_t *rects = cairo_copy_clip_rectangle_list(cr);
||||||| merged common ancestors
# ifdef cairo_copy_clip_rectangle_list
#  error "Looks like we're including Mozilla's cairo instead of system cairo"
# endif
static bool
ExtractExposeRegion(LayoutDeviceIntRegion& aRegion, cairo_t* cr)
{
  cairo_rectangle_list_t* rects = cairo_copy_clip_rectangle_list(cr);
=======
#ifdef cairo_copy_clip_rectangle_list
#  error "Looks like we're including Mozilla's cairo instead of system cairo"
#endif
static bool ExtractExposeRegion(LayoutDeviceIntRegion& aRegion, cairo_t* cr) {
  cairo_rectangle_list_t* rects = cairo_copy_clip_rectangle_list(cr);
>>>>>>> upstream-releases
  if (rects->status != CAIRO_STATUS_SUCCESS) {
    NS_WARNING("Failed to obtain cairo rectangle list.");
    return false;
  }

<<<<<<< HEAD
  for (int i = 0; i < rects->num_rectangles; i++) {
    const cairo_rectangle_t &r = rects->rectangles[i];
    aRegion.Or(aRegion,
               LayoutDeviceIntRect::Truncate(r.x, r.y, r.width, r.height));
    LOGDRAW(("\t%f %f %f %f\n", r.x, r.y, r.width, r.height));
||||||| merged common ancestors
  for (int i = 0; i < rects->num_rectangles; i++)  {
      const cairo_rectangle_t& r = rects->rectangles[i];
      aRegion.Or(aRegion, LayoutDeviceIntRect::Truncate(r.x, r.y, r.width, r.height));
      LOGDRAW(("\t%f %f %f %f\n", r.x, r.y, r.width, r.height));
=======
  for (int i = 0; i < rects->num_rectangles; i++) {
    const cairo_rectangle_t& r = rects->rectangles[i];
    aRegion.Or(aRegion,
               LayoutDeviceIntRect::Truncate(r.x, r.y, r.width, r.height));
    LOGDRAW(("\t%f %f %f %f\n", r.x, r.y, r.width, r.height));
>>>>>>> upstream-releases
  }

  cairo_rectangle_list_destroy(rects);
  return true;
}

<<<<<<< HEAD
gboolean nsWindow::OnExposeEvent(cairo_t *cr) {
  // Send any pending resize events so that layout can update.
  // May run event loop.
  MaybeDispatchResized();
||||||| merged common ancestors
gboolean
nsWindow::OnExposeEvent(cairo_t *cr)
{
    // Send any pending resize events so that layout can update.
    // May run event loop.
    MaybeDispatchResized();

    if (mIsDestroyed) {
        return FALSE;
    }
=======
#ifdef MOZ_WAYLAND
void nsWindow::WaylandEGLSurfaceForceRedraw() {
  MOZ_RELEASE_ASSERT(NS_IsMainThread());

  if (mIsDestroyed || !mNeedsUpdatingEGLSurface) {
    return;
  }

  if (CompositorBridgeChild* remoteRenderer = GetRemoteRenderer()) {
    MOZ_ASSERT(mCompositorWidgetDelegate);
    if (mCompositorWidgetDelegate) {
      mNeedsUpdatingEGLSurface = false;
      mCompositorWidgetDelegate->RequestsUpdatingEGLSurface();
    }
    remoteRenderer->SendForcePresent();
  }
}
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mIsDestroyed) {
    return FALSE;
  }
||||||| merged common ancestors
    // Windows that are not visible will be painted after they become visible.
    if (!mGdkWindow || mIsFullyObscured || !mHasMappedToplevel)
        return FALSE;
=======
gboolean nsWindow::OnExposeEvent(cairo_t* cr) {
  // Send any pending resize events so that layout can update.
  // May run event loop.
  MaybeDispatchResized();

  if (mIsDestroyed) {
    return FALSE;
  }
>>>>>>> upstream-releases

  // Windows that are not visible will be painted after they become visible.
  if (!mGdkWindow || mIsFullyObscured || !mHasMappedToplevel) return FALSE;
#ifdef MOZ_WAYLAND
  if (mContainer && !mContainer->ready_to_draw) return FALSE;
#endif

<<<<<<< HEAD
  nsIWidgetListener *listener = GetListener();
  if (!listener) return FALSE;
||||||| merged common ancestors
    nsIWidgetListener *listener = GetListener();
    if (!listener)
        return FALSE;
=======
  nsIWidgetListener* listener = GetListener();
  if (!listener) return FALSE;
>>>>>>> upstream-releases

  LayoutDeviceIntRegion exposeRegion;
  if (!ExtractExposeRegion(exposeRegion, cr)) {
    return FALSE;
  }

  gint scale = GdkScaleFactor();
  LayoutDeviceIntRegion region = exposeRegion;
  region.ScaleRoundOut(scale, scale);

  if (GetLayerManager()->AsKnowsCompositor() && mCompositorSession) {
    // We need to paint to the screen even if nothing changed, since if we
    // don't have a compositing window manager, our pixels could be stale.
    GetLayerManager()->SetNeedsComposite(true);
    GetLayerManager()->SendInvalidRegion(region.ToUnknownRegion());
  }

  RefPtr<nsWindow> strongThis(this);

  // Dispatch WillPaintWindow notification to allow scripts etc. to run
  // before we paint
  {
    listener->WillPaintWindow(this);

    // If the window has been destroyed during the will paint notification,
    // there is nothing left to do.
    if (!mGdkWindow) return TRUE;

    // Re-get the listener since the will paint notification might have
    // killed it.
    listener = GetListener();
    if (!listener) return FALSE;
  }

  if (GetLayerManager()->AsKnowsCompositor() &&
      GetLayerManager()->NeedsComposite()) {
    GetLayerManager()->ScheduleComposite();
    GetLayerManager()->SetNeedsComposite(false);
  }

<<<<<<< HEAD
  LOGDRAW(("sending expose event [%p] %p 0x%lx (rects follow):\n", (void *)this,
           (void *)mGdkWindow,
           mIsX11Display ? gdk_x11_window_get_xid(mGdkWindow) : 0));

  // Our bounds may have changed after calling WillPaintWindow.  Clip
  // to the new bounds here.  The region is relative to this
  // window.
  region.And(region, LayoutDeviceIntRect(0, 0, mBounds.width, mBounds.height));

  bool shaped = false;
  if (eTransparencyTransparent == GetTransparencyMode()) {
    if (mHasAlphaVisual) {
      // Remove possible shape mask from when window manger was not
      // previously compositing.
      static_cast<nsWindow *>(GetTopLevelWidget())->ClearTransparencyBitmap();
    } else {
      shaped = true;
||||||| merged common ancestors
    LOGDRAW(("sending expose event [%p] %p 0x%lx (rects follow):\n",
             (void *)this, (void *)mGdkWindow,
             mIsX11Display ? gdk_x11_window_get_xid(mGdkWindow) : 0));

    // Our bounds may have changed after calling WillPaintWindow.  Clip
    // to the new bounds here.  The region is relative to this
    // window.
    region.And(region, LayoutDeviceIntRect(0, 0, mBounds.width, mBounds.height));

    bool shaped = false;
    if (eTransparencyTransparent == GetTransparencyMode()) {
        if (mHasAlphaVisual) {
            // Remove possible shape mask from when window manger was not
            // previously compositing.
            static_cast<nsWindow*>(GetTopLevelWidget())->
                ClearTransparencyBitmap();
        } else {
            shaped = true;
        }
=======
  LOGDRAW(("sending expose event [%p] %p 0x%lx (rects follow):\n", (void*)this,
           (void*)mGdkWindow,
           mIsX11Display ? gdk_x11_window_get_xid(mGdkWindow) : 0));

  // Our bounds may have changed after calling WillPaintWindow.  Clip
  // to the new bounds here.  The region is relative to this
  // window.
  region.And(region, LayoutDeviceIntRect(0, 0, mBounds.width, mBounds.height));

  bool shaped = false;
  if (eTransparencyTransparent == GetTransparencyMode()) {
    auto window = static_cast<nsWindow*>(GetTopLevelWidget());
    if (mTransparencyBitmapForTitlebar) {
      if (mSizeState == nsSizeMode_Normal) {
        window->UpdateTitlebarTransparencyBitmap();
      } else {
        window->ClearTransparencyBitmap();
      }
    } else {
      if (mHasAlphaVisual) {
        // Remove possible shape mask from when window manger was not
        // previously compositing.
        window->ClearTransparencyBitmap();
      } else {
        shaped = true;
      }
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  if (!shaped) {
    GList *children = gdk_window_peek_children(mGdkWindow);
    while (children) {
      GdkWindow *gdkWin = GDK_WINDOW(children->data);
      nsWindow *kid = get_window_for_gdk_window(gdkWin);
      if (kid && gdk_window_is_visible(gdkWin)) {
        AutoTArray<LayoutDeviceIntRect, 1> clipRects;
        kid->GetWindowClipRegion(&clipRects);
        LayoutDeviceIntRect bounds = kid->GetBounds();
        for (uint32_t i = 0; i < clipRects.Length(); ++i) {
          LayoutDeviceIntRect r = clipRects[i] + bounds.TopLeft();
          region.Sub(region, r);
||||||| merged common ancestors
    if (!shaped) {
        GList *children =
            gdk_window_peek_children(mGdkWindow);
        while (children) {
            GdkWindow *gdkWin = GDK_WINDOW(children->data);
            nsWindow *kid = get_window_for_gdk_window(gdkWin);
            if (kid && gdk_window_is_visible(gdkWin)) {
                AutoTArray<LayoutDeviceIntRect,1> clipRects;
                kid->GetWindowClipRegion(&clipRects);
                LayoutDeviceIntRect bounds = kid->GetBounds();
                for (uint32_t i = 0; i < clipRects.Length(); ++i) {
                    LayoutDeviceIntRect r = clipRects[i] + bounds.TopLeft();
                    region.Sub(region, r);
                }
            }
            children = children->next;
=======
  if (!shaped) {
    GList* children = gdk_window_peek_children(mGdkWindow);
    while (children) {
      GdkWindow* gdkWin = GDK_WINDOW(children->data);
      nsWindow* kid = get_window_for_gdk_window(gdkWin);
      if (kid && gdk_window_is_visible(gdkWin)) {
        AutoTArray<LayoutDeviceIntRect, 1> clipRects;
        kid->GetWindowClipRegion(&clipRects);
        LayoutDeviceIntRect bounds = kid->GetBounds();
        for (uint32_t i = 0; i < clipRects.Length(); ++i) {
          LayoutDeviceIntRect r = clipRects[i] + bounds.TopLeft();
          region.Sub(region, r);
>>>>>>> upstream-releases
        }
      }
      children = children->next;
    }
  }

  if (region.IsEmpty()) {
    return TRUE;
  }

  // If this widget uses OMTC...
  if (GetLayerManager()->GetBackendType() == LayersBackend::LAYERS_CLIENT ||
      GetLayerManager()->GetBackendType() == LayersBackend::LAYERS_WR) {
    listener->PaintWindow(this, region);

    // Re-get the listener since the will paint notification might have
    // killed it.
    listener = GetListener();
    if (!listener) return TRUE;

    listener->DidPaintWindow();
    return TRUE;
  }

  BufferMode layerBuffering = BufferMode::BUFFERED;
  RefPtr<DrawTarget> dt = StartRemoteDrawingInRegion(region, &layerBuffering);
  if (!dt || !dt->IsValid()) {
    return FALSE;
  }
  RefPtr<gfxContext> ctx;
  IntRect boundsRect = region.GetBounds().ToUnknownRect();
  IntPoint offset(0, 0);
  if (dt->GetSize() == boundsRect.Size()) {
    offset = boundsRect.TopLeft();
    dt->SetTransform(Matrix::Translation(-offset));
  }

#ifdef MOZ_X11
  if (shaped) {
    // Collapse update area to the bounding box. This is so we only have to
    // call UpdateTranslucentWindowAlpha once. After we have dropped
    // support for non-Thebes graphics, UpdateTranslucentWindowAlpha will be
    // our private interface so we can rework things to avoid this.
    dt->PushClipRect(Rect(boundsRect));

    // The double buffering is done here to extract the shape mask.
    // (The shape mask won't be necessary when a visual with an alpha
    // channel is used on compositing window managers.)
    layerBuffering = BufferMode::BUFFER_NONE;
    RefPtr<DrawTarget> destDT =
        dt->CreateSimilarDrawTarget(boundsRect.Size(), SurfaceFormat::B8G8R8A8);
    if (!destDT || !destDT->IsValid()) {
      return FALSE;
    }
    destDT->SetTransform(Matrix::Translation(-boundsRect.TopLeft()));
    ctx = gfxContext::CreatePreservingTransformOrNull(destDT);
  } else {
    gfxUtils::ClipToRegion(dt, region.ToUnknownRegion());
    ctx = gfxContext::CreatePreservingTransformOrNull(dt);
  }
  MOZ_ASSERT(ctx);  // checked both dt and destDT valid draw target above

#  if 0
    // NOTE: Paint flashing region would be wrong for cairo, since
    // cairo inflates the update region, etc.  So don't paint flash
    // for cairo.
#    ifdef DEBUG
    // XXX aEvent->region may refer to a newly-invalid area.  FIXME
    if (0 && WANT_PAINT_FLASHING && gtk_widget_get_window(aEvent))
        gdk_window_flash(mGdkWindow, 1, 100, aEvent->region);
<<<<<<< HEAD
#endif
#endif

#endif  // MOZ_X11
||||||| merged common ancestors
#endif
#endif

#endif // MOZ_X11

    bool painted = false;
    {
      if (GetLayerManager()->GetBackendType() == LayersBackend::LAYERS_BASIC) {
        if (GetTransparencyMode() == eTransparencyTransparent &&
            layerBuffering == BufferMode::BUFFER_NONE &&
            mHasAlphaVisual) {
          // If our draw target is unbuffered and we use an alpha channel,
          // clear the image beforehand to ensure we don't get artifacts from a
          // reused SHM image. See bug 1258086.
          dt->ClearRect(Rect(boundsRect));
        }
        AutoLayerManagerSetup setupLayerManager(this, ctx, layerBuffering);
        painted = listener->PaintWindow(this, region);

        // Re-get the listener since the will paint notification might have
        // killed it.
        listener = GetListener();
        if (!listener)
            return TRUE;
=======
#    endif
#  endif

#endif  // MOZ_X11
>>>>>>> upstream-releases

  bool painted = false;
  {
    if (GetLayerManager()->GetBackendType() == LayersBackend::LAYERS_BASIC) {
      if (GetTransparencyMode() == eTransparencyTransparent &&
          layerBuffering == BufferMode::BUFFER_NONE && mHasAlphaVisual) {
        // If our draw target is unbuffered and we use an alpha channel,
        // clear the image beforehand to ensure we don't get artifacts from a
        // reused SHM image. See bug 1258086.
        dt->ClearRect(Rect(boundsRect));
      }
      AutoLayerManagerSetup setupLayerManager(this, ctx, layerBuffering);
      painted = listener->PaintWindow(this, region);

      // Re-get the listener since the will paint notification might have
      // killed it.
      listener = GetListener();
      if (!listener) return TRUE;
    }
  }

#ifdef MOZ_X11
  // PaintWindow can Destroy us (bug 378273), avoid doing any paint
  // operations below if that happened - it will lead to XError and exit().
  if (shaped) {
    if (MOZ_LIKELY(!mIsDestroyed)) {
      if (painted) {
        RefPtr<SourceSurface> surf = ctx->GetDrawTarget()->Snapshot();

        UpdateAlpha(surf, boundsRect);

        dt->DrawSurface(surf, Rect(boundsRect),
                        Rect(0, 0, boundsRect.width, boundsRect.height),
                        DrawSurfaceOptions(SamplingFilter::POINT),
                        DrawOptions(1.0f, CompositionOp::OP_SOURCE));
      }
    }
  }

<<<<<<< HEAD
  ctx = nullptr;
  dt->PopClip();
||||||| merged common ancestors
    ctx = nullptr;
    dt->PopClip();

#endif // MOZ_X11
=======
  ctx = nullptr;
  dt->PopClip();

#endif  // MOZ_X11
>>>>>>> upstream-releases

<<<<<<< HEAD
#endif  // MOZ_X11
||||||| merged common ancestors
    EndRemoteDrawingInRegion(dt, region);
=======
  EndRemoteDrawingInRegion(dt, region);
>>>>>>> upstream-releases

<<<<<<< HEAD
  EndRemoteDrawingInRegion(dt, region);
||||||| merged common ancestors
    listener->DidPaintWindow();
=======
  listener->DidPaintWindow();
>>>>>>> upstream-releases

<<<<<<< HEAD
  listener->DidPaintWindow();
||||||| merged common ancestors
    // Synchronously flush any new dirty areas
    cairo_region_t* dirtyArea = gdk_window_get_update_area(mGdkWindow);
=======
  // Synchronously flush any new dirty areas
  cairo_region_t* dirtyArea = gdk_window_get_update_area(mGdkWindow);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Synchronously flush any new dirty areas
  cairo_region_t *dirtyArea = gdk_window_get_update_area(mGdkWindow);
||||||| merged common ancestors
    if (dirtyArea) {
        gdk_window_invalidate_region(mGdkWindow, dirtyArea, false);
        cairo_region_destroy(dirtyArea);
        gdk_window_process_updates(mGdkWindow, false);
    }
=======
  if (dirtyArea) {
    gdk_window_invalidate_region(mGdkWindow, dirtyArea, false);
    cairo_region_destroy(dirtyArea);
    gdk_window_process_updates(mGdkWindow, false);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (dirtyArea) {
    gdk_window_invalidate_region(mGdkWindow, dirtyArea, false);
    cairo_region_destroy(dirtyArea);
    gdk_window_process_updates(mGdkWindow, false);
  }
||||||| merged common ancestors
    // check the return value!
    return TRUE;
}
=======
  // check the return value!
  return TRUE;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // check the return value!
  return TRUE;
}
||||||| merged common ancestors
void
nsWindow::UpdateAlpha(SourceSurface* aSourceSurface, nsIntRect aBoundsRect)
{
    // We need to create our own buffer to force the stride to match the
    // expected stride.
    int32_t stride = GetAlignedStride<4>(aBoundsRect.width,
                                         BytesPerPixel(SurfaceFormat::A8));
    if (stride == 0) {
        return;
    }
    int32_t bufferSize = stride * aBoundsRect.height;
    auto imageBuffer = MakeUniqueFallible<uint8_t[]>(bufferSize);
    {
        RefPtr<DrawTarget> drawTarget = gfxPlatform::CreateDrawTargetForData(
                                              imageBuffer.get(),
                                              aBoundsRect.Size(),
                                              stride, SurfaceFormat::A8);

        if (drawTarget) {
            drawTarget->DrawSurface(aSourceSurface, Rect(0, 0, aBoundsRect.width, aBoundsRect.height),
                                    Rect(0, 0, aSourceSurface->GetSize().width, aSourceSurface->GetSize().height),
                                    DrawSurfaceOptions(SamplingFilter::POINT),
                                    DrawOptions(1.0f, CompositionOp::OP_SOURCE));
        }
    }
    UpdateTranslucentWindowAlphaInternal(aBoundsRect, imageBuffer.get(), stride);
}
=======
void nsWindow::UpdateAlpha(SourceSurface* aSourceSurface,
                           nsIntRect aBoundsRect) {
  // We need to create our own buffer to force the stride to match the
  // expected stride.
  int32_t stride =
      GetAlignedStride<4>(aBoundsRect.width, BytesPerPixel(SurfaceFormat::A8));
  if (stride == 0) {
    return;
  }
  int32_t bufferSize = stride * aBoundsRect.height;
  auto imageBuffer = MakeUniqueFallible<uint8_t[]>(bufferSize);
  {
    RefPtr<DrawTarget> drawTarget = gfxPlatform::CreateDrawTargetForData(
        imageBuffer.get(), aBoundsRect.Size(), stride, SurfaceFormat::A8);

    if (drawTarget) {
      drawTarget->DrawSurface(aSourceSurface,
                              Rect(0, 0, aBoundsRect.width, aBoundsRect.height),
                              Rect(0, 0, aSourceSurface->GetSize().width,
                                   aSourceSurface->GetSize().height),
                              DrawSurfaceOptions(SamplingFilter::POINT),
                              DrawOptions(1.0f, CompositionOp::OP_SOURCE));
    }
  }
  UpdateTranslucentWindowAlphaInternal(aBoundsRect, imageBuffer.get(), stride);
}

gboolean nsWindow::OnConfigureEvent(GtkWidget* aWidget,
                                    GdkEventConfigure* aEvent) {
  // These events are only received on toplevel windows.
  //
  // GDK ensures that the coordinates are the client window top-left wrt the
  // root window.
  //
  //   GDK calculates the cordinates for real ConfigureNotify events on
  //   managed windows (that would normally be relative to the parent
  //   window).
  //
  //   Synthetic ConfigureNotify events are from the window manager and
  //   already relative to the root window.  GDK creates all X windows with
  //   border_width = 0, so synthetic events also indicate the top-left of
  //   the client window.
  //
  //   Override-redirect windows are children of the root window so parent
  //   coordinates are root coordinates.

  LOG(("configure event [%p] %d %d %d %d\n", (void*)this, aEvent->x, aEvent->y,
       aEvent->width, aEvent->height));

  if (mPendingConfigures > 0) {
    mPendingConfigures--;
  }

  LayoutDeviceIntRect screenBounds = GetScreenBounds();

  if (mWindowType == eWindowType_toplevel ||
      mWindowType == eWindowType_dialog) {
    // This check avoids unwanted rollup on spurious configure events from
    // Cygwin/X (bug 672103).
    if (mBounds.x != screenBounds.x || mBounds.y != screenBounds.y) {
      CheckForRollup(0, 0, false, true);
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void nsWindow::UpdateAlpha(SourceSurface *aSourceSurface,
                           nsIntRect aBoundsRect) {
  // We need to create our own buffer to force the stride to match the
  // expected stride.
  int32_t stride =
      GetAlignedStride<4>(aBoundsRect.width, BytesPerPixel(SurfaceFormat::A8));
  if (stride == 0) {
    return;
  }
  int32_t bufferSize = stride * aBoundsRect.height;
  auto imageBuffer = MakeUniqueFallible<uint8_t[]>(bufferSize);
  {
    RefPtr<DrawTarget> drawTarget = gfxPlatform::CreateDrawTargetForData(
        imageBuffer.get(), aBoundsRect.Size(), stride, SurfaceFormat::A8);

    if (drawTarget) {
      drawTarget->DrawSurface(aSourceSurface,
                              Rect(0, 0, aBoundsRect.width, aBoundsRect.height),
                              Rect(0, 0, aSourceSurface->GetSize().width,
                                   aSourceSurface->GetSize().height),
                              DrawSurfaceOptions(SamplingFilter::POINT),
                              DrawOptions(1.0f, CompositionOp::OP_SOURCE));
    }
  }
  UpdateTranslucentWindowAlphaInternal(aBoundsRect, imageBuffer.get(), stride);
}

gboolean nsWindow::OnConfigureEvent(GtkWidget *aWidget,
                                    GdkEventConfigure *aEvent) {
  // These events are only received on toplevel windows.
  //
  // GDK ensures that the coordinates are the client window top-left wrt the
  // root window.
  //
  //   GDK calculates the cordinates for real ConfigureNotify events on
  //   managed windows (that would normally be relative to the parent
  //   window).
  //
  //   Synthetic ConfigureNotify events are from the window manager and
  //   already relative to the root window.  GDK creates all X windows with
  //   border_width = 0, so synthetic events also indicate the top-left of
  //   the client window.
  //
  //   Override-redirect windows are children of the root window so parent
  //   coordinates are root coordinates.

  LOG(("configure event [%p] %d %d %d %d\n", (void *)this, aEvent->x, aEvent->y,
       aEvent->width, aEvent->height));

  if (mPendingConfigures > 0) {
    mPendingConfigures--;
  }
||||||| merged common ancestors
gboolean
nsWindow::OnConfigureEvent(GtkWidget *aWidget, GdkEventConfigure *aEvent)
{
    // These events are only received on toplevel windows.
    //
    // GDK ensures that the coordinates are the client window top-left wrt the
    // root window.
    //
    //   GDK calculates the cordinates for real ConfigureNotify events on
    //   managed windows (that would normally be relative to the parent
    //   window).
    //
    //   Synthetic ConfigureNotify events are from the window manager and
    //   already relative to the root window.  GDK creates all X windows with
    //   border_width = 0, so synthetic events also indicate the top-left of
    //   the client window.
    //
    //   Override-redirect windows are children of the root window so parent
    //   coordinates are root coordinates.

    LOG(("configure event [%p] %d %d %d %d\n", (void *)this,
         aEvent->x, aEvent->y, aEvent->width, aEvent->height));

    if (mPendingConfigures > 0) {
        mPendingConfigures--;
    }
=======
  // This event indicates that the window position may have changed.
  // mBounds.Size() is updated in OnSizeAllocate().

  NS_ASSERTION(GTK_IS_WINDOW(aWidget),
               "Configure event on widget that is not a GtkWindow");
  if (gtk_window_get_window_type(GTK_WINDOW(aWidget)) == GTK_WINDOW_POPUP) {
    // Override-redirect window
    //
    // These windows should not be moved by the window manager, and so any
    // change in position is a result of our direction.  mBounds has
    // already been set in std::move() or Resize(), and that is more
    // up-to-date than the position in the ConfigureNotify event if the
    // event is from an earlier window move.
    //
    // Skipping the WindowMoved call saves context menus from an infinite
    // loop when nsXULPopupManager::PopupMoved moves the window to the new
    // position and nsMenuPopupFrame::SetPopupPosition adds
    // offsetForContextMenu on each iteration.
    return FALSE;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  LayoutDeviceIntRect screenBounds = GetScreenBounds();
||||||| merged common ancestors
    LayoutDeviceIntRect screenBounds = GetScreenBounds();

    if (mWindowType == eWindowType_toplevel || mWindowType == eWindowType_dialog) {
        // This check avoids unwanted rollup on spurious configure events from
        // Cygwin/X (bug 672103).
        if (mBounds.x != screenBounds.x ||
            mBounds.y != screenBounds.y) {
            CheckForRollup(0, 0, false, true);
        }
    }
=======
  mBounds.MoveTo(screenBounds.TopLeft());

  // XXX mozilla will invalidate the entire window after this move
  // complete.  wtf?
  NotifyWindowMoved(mBounds.x, mBounds.y);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mWindowType == eWindowType_toplevel ||
      mWindowType == eWindowType_dialog) {
    // This check avoids unwanted rollup on spurious configure events from
    // Cygwin/X (bug 672103).
    if (mBounds.x != screenBounds.x || mBounds.y != screenBounds.y) {
      CheckForRollup(0, 0, false, true);
    }
  }
||||||| merged common ancestors
    // This event indicates that the window position may have changed.
    // mBounds.Size() is updated in OnSizeAllocate().

    NS_ASSERTION(GTK_IS_WINDOW(aWidget),
                 "Configure event on widget that is not a GtkWindow");
    if (gtk_window_get_window_type(GTK_WINDOW(aWidget)) == GTK_WINDOW_POPUP) {
        // Override-redirect window
        //
        // These windows should not be moved by the window manager, and so any
        // change in position is a result of our direction.  mBounds has
        // already been set in std::move() or Resize(), and that is more
        // up-to-date than the position in the ConfigureNotify event if the
        // event is from an earlier window move.
        //
        // Skipping the WindowMoved call saves context menus from an infinite
        // loop when nsXULPopupManager::PopupMoved moves the window to the new
        // position and nsMenuPopupFrame::SetPopupPosition adds
        // offsetForContextMenu on each iteration.
        return FALSE;
    }
=======
  return FALSE;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // This event indicates that the window position may have changed.
  // mBounds.Size() is updated in OnSizeAllocate().
||||||| merged common ancestors
    mBounds.MoveTo(screenBounds.TopLeft());

    // XXX mozilla will invalidate the entire window after this move
    // complete.  wtf?
    NotifyWindowMoved(mBounds.x, mBounds.y);
=======
void nsWindow::OnContainerUnrealize() {
  // The GdkWindows are about to be destroyed (but not deleted), so remove
  // their references back to their container widget while the GdkWindow
  // hierarchy is still available.

  if (mGdkWindow) {
    DestroyChildWindows();
>>>>>>> upstream-releases

<<<<<<< HEAD
  NS_ASSERTION(GTK_IS_WINDOW(aWidget),
               "Configure event on widget that is not a GtkWindow");
  if (gtk_window_get_window_type(GTK_WINDOW(aWidget)) == GTK_WINDOW_POPUP) {
    // Override-redirect window
    //
    // These windows should not be moved by the window manager, and so any
    // change in position is a result of our direction.  mBounds has
    // already been set in std::move() or Resize(), and that is more
    // up-to-date than the position in the ConfigureNotify event if the
    // event is from an earlier window move.
    //
    // Skipping the WindowMoved call saves context menus from an infinite
    // loop when nsXULPopupManager::PopupMoved moves the window to the new
    // position and nsMenuPopupFrame::SetPopupPosition adds
    // offsetForContextMenu on each iteration.
    return FALSE;
  }
||||||| merged common ancestors
    return FALSE;
}
=======
    g_object_set_data(G_OBJECT(mGdkWindow), "nsWindow", nullptr);
    mGdkWindow = nullptr;
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  mBounds.MoveTo(screenBounds.TopLeft());
||||||| merged common ancestors
void
nsWindow::OnContainerUnrealize()
{
    // The GdkWindows are about to be destroyed (but not deleted), so remove
    // their references back to their container widget while the GdkWindow
    // hierarchy is still available.
=======
void nsWindow::OnSizeAllocate(GtkAllocation* aAllocation) {
  LOG(("size_allocate [%p] %d %d %d %d\n", (void*)this, aAllocation->x,
       aAllocation->y, aAllocation->width, aAllocation->height));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // XXX mozilla will invalidate the entire window after this move
  // complete.  wtf?
  NotifyWindowMoved(mBounds.x, mBounds.y);
||||||| merged common ancestors
    if (mGdkWindow) {
        DestroyChildWindows();
=======
  LayoutDeviceIntSize size = GdkRectToDevicePixels(*aAllocation).Size();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return FALSE;
}
||||||| merged common ancestors
        g_object_set_data(G_OBJECT(mGdkWindow), "nsWindow", nullptr);
        mGdkWindow = nullptr;
    }
}
=======
  if (mBounds.Size() == size) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
void nsWindow::OnContainerUnrealize() {
  // The GdkWindows are about to be destroyed (but not deleted), so remove
  // their references back to their container widget while the GdkWindow
  // hierarchy is still available.
||||||| merged common ancestors
void
nsWindow::OnSizeAllocate(GtkAllocation *aAllocation)
{
    LOG(("size_allocate [%p] %d %d %d %d\n",
         (void *)this, aAllocation->x, aAllocation->y,
         aAllocation->width, aAllocation->height));

    LayoutDeviceIntSize size = GdkRectToDevicePixels(*aAllocation).Size();

    if (mBounds.Size() == size)
        return;

    // Invalidate the new part of the window now for the pending paint to
    // minimize background flashes (GDK does not do this for external resizes
    // of toplevels.)
    if (mBounds.width < size.width) {
        GdkRectangle rect = DevicePixelsToGdkRectRoundOut(
            LayoutDeviceIntRect(mBounds.width, 0,
                                size.width - mBounds.width, size.height));
        gdk_window_invalidate_rect(mGdkWindow, &rect, FALSE);
    }
    if (mBounds.height < size.height) {
        GdkRectangle rect = DevicePixelsToGdkRectRoundOut(
            LayoutDeviceIntRect(0, mBounds.height,
                                size.width, size.height - mBounds.height));
        gdk_window_invalidate_rect(mGdkWindow, &rect, FALSE);
    }

    mBounds.SizeTo(size);

#ifdef MOZ_X11
    // Notify the GtkCompositorWidget of a ClientSizeChange
    if (mCompositorWidgetDelegate) {
      mCompositorWidgetDelegate->NotifyClientSizeChanged(GetClientSize());
    }
#endif
=======
  // Invalidate the new part of the window now for the pending paint to
  // minimize background flashes (GDK does not do this for external resizes
  // of toplevels.)
  if (mBounds.width < size.width) {
    GdkRectangle rect = DevicePixelsToGdkRectRoundOut(LayoutDeviceIntRect(
        mBounds.width, 0, size.width - mBounds.width, size.height));
    gdk_window_invalidate_rect(mGdkWindow, &rect, FALSE);
  }
  if (mBounds.height < size.height) {
    GdkRectangle rect = DevicePixelsToGdkRectRoundOut(LayoutDeviceIntRect(
        0, mBounds.height, size.width, size.height - mBounds.height));
    gdk_window_invalidate_rect(mGdkWindow, &rect, FALSE);
  }

  mBounds.SizeTo(size);

#ifdef MOZ_X11
  // Notify the GtkCompositorWidget of a ClientSizeChange
  if (mCompositorWidgetDelegate) {
    mCompositorWidgetDelegate->NotifyClientSizeChanged(GetClientSize());
  }
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mGdkWindow) {
    DestroyChildWindows();
||||||| merged common ancestors
    // Gecko permits running nested event loops during processing of events,
    // GtkWindow callers of gtk_widget_size_allocate expect the signal
    // handlers to return sometime in the near future.
    mNeedsDispatchResized = true;
    NS_DispatchToCurrentThread(NewRunnableMethod(
      "nsWindow::MaybeDispatchResized", this, &nsWindow::MaybeDispatchResized));
}
=======
  // Gecko permits running nested event loops during processing of events,
  // GtkWindow callers of gtk_widget_size_allocate expect the signal
  // handlers to return sometime in the near future.
  mNeedsDispatchResized = true;
  NS_DispatchToCurrentThread(NewRunnableMethod(
      "nsWindow::MaybeDispatchResized", this, &nsWindow::MaybeDispatchResized));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    g_object_set_data(G_OBJECT(mGdkWindow), "nsWindow", nullptr);
    mGdkWindow = nullptr;
  }
||||||| merged common ancestors
void
nsWindow::OnDeleteEvent()
{
    if (mWidgetListener)
        mWidgetListener->RequestWindowClose(this);
=======
void nsWindow::OnDeleteEvent() {
  if (mWidgetListener) mWidgetListener->RequestWindowClose(this);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsWindow::OnSizeAllocate(GtkAllocation *aAllocation) {
  LOG(("size_allocate [%p] %d %d %d %d\n", (void *)this, aAllocation->x,
       aAllocation->y, aAllocation->width, aAllocation->height));
||||||| merged common ancestors
void
nsWindow::OnEnterNotifyEvent(GdkEventCrossing *aEvent)
{
    // This skips NotifyVirtual and NotifyNonlinearVirtual enter notify events
    // when the pointer enters a child window.  If the destination window is a
    // Gecko window then we'll catch the corresponding event on that window,
    // but we won't notice when the pointer directly enters a foreign (plugin)
    // child window without passing over a visible portion of a Gecko window.
    if (aEvent->subwindow != nullptr)
        return;
=======
void nsWindow::OnEnterNotifyEvent(GdkEventCrossing* aEvent) {
  // This skips NotifyVirtual and NotifyNonlinearVirtual enter notify events
  // when the pointer enters a child window.  If the destination window is a
  // Gecko window then we'll catch the corresponding event on that window,
  // but we won't notice when the pointer directly enters a foreign (plugin)
  // child window without passing over a visible portion of a Gecko window.
  if (aEvent->subwindow != nullptr) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
  LayoutDeviceIntSize size = GdkRectToDevicePixels(*aAllocation).Size();
||||||| merged common ancestors
    // Check before is_parent_ungrab_enter() as the button state may have
    // changed while a non-Gecko ancestor window had a pointer grab.
    DispatchMissedButtonReleases(aEvent);
=======
  // Check before is_parent_ungrab_enter() as the button state may have
  // changed while a non-Gecko ancestor window had a pointer grab.
  DispatchMissedButtonReleases(aEvent);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mBounds.Size() == size) return;
||||||| merged common ancestors
    if (is_parent_ungrab_enter(aEvent))
        return;
=======
  if (is_parent_ungrab_enter(aEvent)) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Invalidate the new part of the window now for the pending paint to
  // minimize background flashes (GDK does not do this for external resizes
  // of toplevels.)
  if (mBounds.width < size.width) {
    GdkRectangle rect = DevicePixelsToGdkRectRoundOut(LayoutDeviceIntRect(
        mBounds.width, 0, size.width - mBounds.width, size.height));
    gdk_window_invalidate_rect(mGdkWindow, &rect, FALSE);
  }
  if (mBounds.height < size.height) {
    GdkRectangle rect = DevicePixelsToGdkRectRoundOut(LayoutDeviceIntRect(
        0, mBounds.height, size.width, size.height - mBounds.height));
    gdk_window_invalidate_rect(mGdkWindow, &rect, FALSE);
  }
||||||| merged common ancestors
    WidgetMouseEvent event(true, eMouseEnterIntoWidget, this,
                           WidgetMouseEvent::eReal);
=======
  WidgetMouseEvent event(true, eMouseEnterIntoWidget, this,
                         WidgetMouseEvent::eReal);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mBounds.SizeTo(size);
||||||| merged common ancestors
    event.mRefPoint = GdkEventCoordsToDevicePixels(aEvent->x, aEvent->y);
    event.AssignEventTime(GetWidgetEventTime(aEvent->time));
=======
  event.mRefPoint = GdkEventCoordsToDevicePixels(aEvent->x, aEvent->y);
  event.AssignEventTime(GetWidgetEventTime(aEvent->time));
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef MOZ_X11
  // Notify the GtkCompositorWidget of a ClientSizeChange
  if (mCompositorWidgetDelegate) {
    mCompositorWidgetDelegate->NotifyClientSizeChanged(GetClientSize());
  }
#endif
||||||| merged common ancestors
    LOG(("OnEnterNotify: %p\n", (void *)this));
=======
  LOG(("OnEnterNotify: %p\n", (void*)this));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Gecko permits running nested event loops during processing of events,
  // GtkWindow callers of gtk_widget_size_allocate expect the signal
  // handlers to return sometime in the near future.
  mNeedsDispatchResized = true;
  NS_DispatchToCurrentThread(NewRunnableMethod(
      "nsWindow::MaybeDispatchResized", this, &nsWindow::MaybeDispatchResized));
||||||| merged common ancestors
    DispatchInputEvent(&event);
=======
  DispatchInputEvent(&event);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsWindow::OnDeleteEvent() {
  if (mWidgetListener) mWidgetListener->RequestWindowClose(this);
||||||| merged common ancestors
// XXX Is this the right test for embedding cases?
static bool
is_top_level_mouse_exit(GdkWindow* aWindow, GdkEventCrossing *aEvent)
{
    auto x = gint(aEvent->x_root);
    auto y = gint(aEvent->y_root);
    GdkDisplay* display = gdk_window_get_display(aWindow);
    GdkWindow* winAtPt = gdk_display_get_window_at_pointer(display, &x, &y);
    if (!winAtPt)
        return true;
    GdkWindow* topLevelAtPt = gdk_window_get_toplevel(winAtPt);
    GdkWindow* topLevelWidget = gdk_window_get_toplevel(aWindow);
    return topLevelAtPt != topLevelWidget;
=======
// XXX Is this the right test for embedding cases?
static bool is_top_level_mouse_exit(GdkWindow* aWindow,
                                    GdkEventCrossing* aEvent) {
  auto x = gint(aEvent->x_root);
  auto y = gint(aEvent->y_root);
  GdkDisplay* display = gdk_window_get_display(aWindow);
  GdkWindow* winAtPt = gdk_display_get_window_at_pointer(display, &x, &y);
  if (!winAtPt) return true;
  GdkWindow* topLevelAtPt = gdk_window_get_toplevel(winAtPt);
  GdkWindow* topLevelWidget = gdk_window_get_toplevel(aWindow);
  return topLevelAtPt != topLevelWidget;
}

void nsWindow::OnLeaveNotifyEvent(GdkEventCrossing* aEvent) {
  // This ignores NotifyVirtual and NotifyNonlinearVirtual leave notify
  // events when the pointer leaves a child window.  If the destination
  // window is a Gecko window then we'll catch the corresponding event on
  // that window.
  //
  // XXXkt However, we will miss toplevel exits when the pointer directly
  // leaves a foreign (plugin) child window without passing over a visible
  // portion of a Gecko window.
  if (aEvent->subwindow != nullptr) return;

  WidgetMouseEvent event(true, eMouseExitFromWidget, this,
                         WidgetMouseEvent::eReal);

  event.mRefPoint = GdkEventCoordsToDevicePixels(aEvent->x, aEvent->y);
  event.AssignEventTime(GetWidgetEventTime(aEvent->time));

  event.mExitFrom = is_top_level_mouse_exit(mGdkWindow, aEvent)
                        ? WidgetMouseEvent::eTopLevel
                        : WidgetMouseEvent::eChild;

  LOG(("OnLeaveNotify: %p\n", (void*)this));

  DispatchInputEvent(&event);
}

template <typename Event>
static LayoutDeviceIntPoint GetRefPoint(nsWindow* aWindow, Event* aEvent) {
  if (aEvent->window == aWindow->GetGdkWindow()) {
    // we are the window that the event happened on so no need for expensive
    // WidgetToScreenOffset
    return aWindow->GdkEventCoordsToDevicePixels(aEvent->x, aEvent->y);
  }
  // XXX we're never quite sure which GdkWindow the event came from due to our
  // custom bubbling in scroll_event_cb(), so use ScreenToWidget to translate
  // the screen root coordinates into coordinates relative to this widget.
  return aWindow->GdkEventCoordsToDevicePixels(aEvent->x_root, aEvent->y_root) -
         aWindow->WidgetToScreenOffset();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsWindow::OnEnterNotifyEvent(GdkEventCrossing *aEvent) {
  // This skips NotifyVirtual and NotifyNonlinearVirtual enter notify events
  // when the pointer enters a child window.  If the destination window is a
  // Gecko window then we'll catch the corresponding event on that window,
  // but we won't notice when the pointer directly enters a foreign (plugin)
  // child window without passing over a visible portion of a Gecko window.
  if (aEvent->subwindow != nullptr) return;

  // Check before is_parent_ungrab_enter() as the button state may have
  // changed while a non-Gecko ancestor window had a pointer grab.
  DispatchMissedButtonReleases(aEvent);

  if (is_parent_ungrab_enter(aEvent)) return;

  WidgetMouseEvent event(true, eMouseEnterIntoWidget, this,
                         WidgetMouseEvent::eReal);

  event.mRefPoint = GdkEventCoordsToDevicePixels(aEvent->x, aEvent->y);
  event.AssignEventTime(GetWidgetEventTime(aEvent->time));
||||||| merged common ancestors
void
nsWindow::OnLeaveNotifyEvent(GdkEventCrossing *aEvent)
{
    // This ignores NotifyVirtual and NotifyNonlinearVirtual leave notify
    // events when the pointer leaves a child window.  If the destination
    // window is a Gecko window then we'll catch the corresponding event on
    // that window.
    //
    // XXXkt However, we will miss toplevel exits when the pointer directly
    // leaves a foreign (plugin) child window without passing over a visible
    // portion of a Gecko window.
    if (aEvent->subwindow != nullptr)
        return;

    WidgetMouseEvent event(true, eMouseExitFromWidget, this,
                           WidgetMouseEvent::eReal);

    event.mRefPoint = GdkEventCoordsToDevicePixels(aEvent->x, aEvent->y);
    event.AssignEventTime(GetWidgetEventTime(aEvent->time));

    event.mExitFrom = is_top_level_mouse_exit(mGdkWindow, aEvent)
        ? WidgetMouseEvent::eTopLevel : WidgetMouseEvent::eChild;

    LOG(("OnLeaveNotify: %p\n", (void *)this));
=======
void nsWindow::OnMotionNotifyEvent(GdkEventMotion* aEvent) {
  if (mWindowShouldStartDragging) {
    mWindowShouldStartDragging = false;
    // find the top-level window
    GdkWindow* gdk_window = gdk_window_get_toplevel(mGdkWindow);
    MOZ_ASSERT(gdk_window, "gdk_window_get_toplevel should not return null");
>>>>>>> upstream-releases

<<<<<<< HEAD
  LOG(("OnEnterNotify: %p\n", (void *)this));
||||||| merged common ancestors
    DispatchInputEvent(&event);
}
=======
    bool canDrag = true;
    if (mIsX11Display) {
      // Workaround for https://bugzilla.gnome.org/show_bug.cgi?id=789054
      // To avoid crashes disable double-click on WM without _NET_WM_MOVERESIZE.
      // See _should_perform_ewmh_drag() at gdkwindow-x11.c
      GdkScreen* screen = gdk_window_get_screen(gdk_window);
      GdkAtom atom = gdk_atom_intern("_NET_WM_MOVERESIZE", FALSE);
      if (!gdk_x11_screen_supports_net_wm_hint(screen, atom)) {
        canDrag = false;
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  DispatchInputEvent(&event);
}
||||||| merged common ancestors
template <typename Event> static LayoutDeviceIntPoint
GetRefPoint(nsWindow* aWindow, Event* aEvent)
{
    if (aEvent->window == aWindow->GetGdkWindow()) {
        // we are the window that the event happened on so no need for expensive WidgetToScreenOffset
        return aWindow->GdkEventCoordsToDevicePixels(aEvent->x, aEvent->y);
    }
    // XXX we're never quite sure which GdkWindow the event came from due to our custom bubbling
    // in scroll_event_cb(), so use ScreenToWidget to translate the screen root coordinates into
    // coordinates relative to this widget.
    return aWindow->GdkEventCoordsToDevicePixels(
        aEvent->x_root, aEvent->y_root) - aWindow->WidgetToScreenOffset();
}
=======
    if (canDrag) {
      gdk_window_begin_move_drag(gdk_window, 1, aEvent->x_root, aEvent->y_root,
                                 aEvent->time);
      return;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
// XXX Is this the right test for embedding cases?
static bool is_top_level_mouse_exit(GdkWindow *aWindow,
                                    GdkEventCrossing *aEvent) {
  auto x = gint(aEvent->x_root);
  auto y = gint(aEvent->y_root);
  GdkDisplay *display = gdk_window_get_display(aWindow);
  GdkWindow *winAtPt = gdk_display_get_window_at_pointer(display, &x, &y);
  if (!winAtPt) return true;
  GdkWindow *topLevelAtPt = gdk_window_get_toplevel(winAtPt);
  GdkWindow *topLevelWidget = gdk_window_get_toplevel(aWindow);
  return topLevelAtPt != topLevelWidget;
}

void nsWindow::OnLeaveNotifyEvent(GdkEventCrossing *aEvent) {
  // This ignores NotifyVirtual and NotifyNonlinearVirtual leave notify
  // events when the pointer leaves a child window.  If the destination
  // window is a Gecko window then we'll catch the corresponding event on
  // that window.
  //
  // XXXkt However, we will miss toplevel exits when the pointer directly
  // leaves a foreign (plugin) child window without passing over a visible
  // portion of a Gecko window.
  if (aEvent->subwindow != nullptr) return;

  WidgetMouseEvent event(true, eMouseExitFromWidget, this,
                         WidgetMouseEvent::eReal);

  event.mRefPoint = GdkEventCoordsToDevicePixels(aEvent->x, aEvent->y);
  event.AssignEventTime(GetWidgetEventTime(aEvent->time));

  event.mExitFrom = is_top_level_mouse_exit(mGdkWindow, aEvent)
                        ? WidgetMouseEvent::eTopLevel
                        : WidgetMouseEvent::eChild;

  LOG(("OnLeaveNotify: %p\n", (void *)this));

  DispatchInputEvent(&event);
}

template <typename Event>
static LayoutDeviceIntPoint GetRefPoint(nsWindow *aWindow, Event *aEvent) {
  if (aEvent->window == aWindow->GetGdkWindow()) {
    // we are the window that the event happened on so no need for expensive
    // WidgetToScreenOffset
    return aWindow->GdkEventCoordsToDevicePixels(aEvent->x, aEvent->y);
  }
  // XXX we're never quite sure which GdkWindow the event came from due to our
  // custom bubbling in scroll_event_cb(), so use ScreenToWidget to translate
  // the screen root coordinates into coordinates relative to this widget.
  return aWindow->GdkEventCoordsToDevicePixels(aEvent->x_root, aEvent->y_root) -
         aWindow->WidgetToScreenOffset();
}

void nsWindow::OnMotionNotifyEvent(GdkEventMotion *aEvent) {
  // see if we can compress this event
  // XXXldb Why skip every other motion event when we have multiple,
  // but not more than that?
  bool synthEvent = false;
||||||| merged common ancestors
void
nsWindow::OnMotionNotifyEvent(GdkEventMotion *aEvent)
{
    // see if we can compress this event
    // XXXldb Why skip every other motion event when we have multiple,
    // but not more than that?
    bool synthEvent = false;
=======
  // see if we can compress this event
  // XXXldb Why skip every other motion event when we have multiple,
  // but not more than that?
  bool synthEvent = false;
>>>>>>> upstream-releases
#ifdef MOZ_X11
<<<<<<< HEAD
  XEvent xevent;

  if (mIsX11Display) {
    while (XPending(GDK_WINDOW_XDISPLAY(aEvent->window))) {
      XEvent peeked;
      XPeekEvent(GDK_WINDOW_XDISPLAY(aEvent->window), &peeked);
      if (peeked.xany.window != gdk_x11_window_get_xid(aEvent->window) ||
          peeked.type != MotionNotify)
        break;

      synthEvent = true;
      XNextEvent(GDK_WINDOW_XDISPLAY(aEvent->window), &xevent);
||||||| merged common ancestors
    XEvent xevent;

    if (mIsX11Display) {
        while (XPending (GDK_WINDOW_XDISPLAY(aEvent->window))) {
            XEvent peeked;
            XPeekEvent (GDK_WINDOW_XDISPLAY(aEvent->window), &peeked);
            if (peeked.xany.window != gdk_x11_window_get_xid(aEvent->window)
                || peeked.type != MotionNotify)
                break;

            synthEvent = true;
            XNextEvent (GDK_WINDOW_XDISPLAY(aEvent->window), &xevent);
        }
=======
  XEvent xevent;

  if (mIsX11Display) {
    while (XPending(GDK_WINDOW_XDISPLAY(aEvent->window))) {
      XEvent peeked;
      XPeekEvent(GDK_WINDOW_XDISPLAY(aEvent->window), &peeked);
      if (peeked.xany.window != gdk_x11_window_get_xid(aEvent->window) ||
          peeked.type != MotionNotify)
        break;

      synthEvent = true;
      XNextEvent(GDK_WINDOW_XDISPLAY(aEvent->window), &xevent);
>>>>>>> upstream-releases
    }
  }
#endif /* MOZ_X11 */

  WidgetMouseEvent event(true, eMouseMove, this, WidgetMouseEvent::eReal);

<<<<<<< HEAD
  gdouble pressure = 0;
  gdk_event_get_axis((GdkEvent *)aEvent, GDK_AXIS_PRESSURE, &pressure);
  // Sometime gdk generate 0 pressure value between normal values
  // We have to ignore that and use last valid value
  if (pressure) mLastMotionPressure = pressure;
  event.pressure = mLastMotionPressure;
||||||| merged common ancestors
    gdouble pressure = 0;
    gdk_event_get_axis ((GdkEvent*)aEvent, GDK_AXIS_PRESSURE, &pressure);
    // Sometime gdk generate 0 pressure value between normal values
    // We have to ignore that and use last valid value
    if (pressure)
      mLastMotionPressure = pressure;
    event.pressure = mLastMotionPressure;
=======
  gdouble pressure = 0;
  gdk_event_get_axis((GdkEvent*)aEvent, GDK_AXIS_PRESSURE, &pressure);
  // Sometime gdk generate 0 pressure value between normal values
  // We have to ignore that and use last valid value
  if (pressure) mLastMotionPressure = pressure;
  event.mPressure = mLastMotionPressure;
>>>>>>> upstream-releases

  guint modifierState;
  if (synthEvent) {
#ifdef MOZ_X11
    event.mRefPoint.x = nscoord(xevent.xmotion.x);
    event.mRefPoint.y = nscoord(xevent.xmotion.y);

    modifierState = xevent.xmotion.state;

    event.AssignEventTime(GetWidgetEventTime(xevent.xmotion.time));
#else
    event.mRefPoint = GdkEventCoordsToDevicePixels(aEvent->x, aEvent->y);

    modifierState = aEvent->state;

    event.AssignEventTime(GetWidgetEventTime(aEvent->time));
#endif /* MOZ_X11 */
  } else {
    event.mRefPoint = GetRefPoint(this, aEvent);

    modifierState = aEvent->state;

    event.AssignEventTime(GetWidgetEventTime(aEvent->time));
  }

  KeymapWrapper::InitInputEvent(event, modifierState);

  DispatchInputEvent(&event);
}

// If the automatic pointer grab on ButtonPress has deactivated before
// ButtonRelease, and the mouse button is released while the pointer is not
// over any a Gecko window, then the ButtonRelease event will not be received.
// (A similar situation exists when the pointer is grabbed with owner_events
// True as the ButtonRelease may be received on a foreign [plugin] window).
// Use this method to check for released buttons when the pointer returns to a
// Gecko window.
<<<<<<< HEAD
void nsWindow::DispatchMissedButtonReleases(GdkEventCrossing *aGdkEvent) {
  guint changed = aGdkEvent->state ^ gButtonState;
  // Only consider button releases.
  // (Ignore button presses that occurred outside Gecko.)
  guint released = changed & gButtonState;
  gButtonState = aGdkEvent->state;

  // Loop over each button, excluding mouse wheel buttons 4 and 5 for which
  // GDK ignores releases.
  for (guint buttonMask = GDK_BUTTON1_MASK; buttonMask <= GDK_BUTTON3_MASK;
       buttonMask <<= 1) {
    if (released & buttonMask) {
      int16_t buttonType;
      switch (buttonMask) {
        case GDK_BUTTON1_MASK:
          buttonType = WidgetMouseEvent::eLeftButton;
          break;
        case GDK_BUTTON2_MASK:
          buttonType = WidgetMouseEvent::eMiddleButton;
          break;
        default:
          NS_ASSERTION(buttonMask == GDK_BUTTON3_MASK,
                       "Unexpected button mask");
          buttonType = WidgetMouseEvent::eRightButton;
      }
||||||| merged common ancestors
void
nsWindow::DispatchMissedButtonReleases(GdkEventCrossing *aGdkEvent)
{
    guint changed = aGdkEvent->state ^ gButtonState;
    // Only consider button releases.
    // (Ignore button presses that occurred outside Gecko.)
    guint released = changed & gButtonState;
    gButtonState = aGdkEvent->state;

    // Loop over each button, excluding mouse wheel buttons 4 and 5 for which
    // GDK ignores releases.
    for (guint buttonMask = GDK_BUTTON1_MASK;
         buttonMask <= GDK_BUTTON3_MASK;
         buttonMask <<= 1) {

        if (released & buttonMask) {
            int16_t buttonType;
            switch (buttonMask) {
            case GDK_BUTTON1_MASK:
                buttonType = WidgetMouseEvent::eLeftButton;
                break;
            case GDK_BUTTON2_MASK:
                buttonType = WidgetMouseEvent::eMiddleButton;
                break;
            default:
                NS_ASSERTION(buttonMask == GDK_BUTTON3_MASK,
                             "Unexpected button mask");
                buttonType = WidgetMouseEvent::eRightButton;
            }
=======
void nsWindow::DispatchMissedButtonReleases(GdkEventCrossing* aGdkEvent) {
  guint changed = aGdkEvent->state ^ gButtonState;
  // Only consider button releases.
  // (Ignore button presses that occurred outside Gecko.)
  guint released = changed & gButtonState;
  gButtonState = aGdkEvent->state;

  // Loop over each button, excluding mouse wheel buttons 4 and 5 for which
  // GDK ignores releases.
  for (guint buttonMask = GDK_BUTTON1_MASK; buttonMask <= GDK_BUTTON3_MASK;
       buttonMask <<= 1) {
    if (released & buttonMask) {
      int16_t buttonType;
      switch (buttonMask) {
        case GDK_BUTTON1_MASK:
          buttonType = MouseButton::eLeft;
          break;
        case GDK_BUTTON2_MASK:
          buttonType = MouseButton::eMiddle;
          break;
        default:
          NS_ASSERTION(buttonMask == GDK_BUTTON3_MASK,
                       "Unexpected button mask");
          buttonType = MouseButton::eRight;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      LOG(("Synthesized button %u release on %p\n", guint(buttonType + 1),
           (void *)this));

      // Dispatch a synthesized button up event to tell Gecko about the
      // change in state.  This event is marked as synthesized so that
      // it is not dispatched as a DOM event, because we don't know the
      // position, widget, modifiers, or time/order.
      WidgetMouseEvent synthEvent(true, eMouseUp, this,
                                  WidgetMouseEvent::eSynthesized);
      synthEvent.button = buttonType;
      DispatchInputEvent(&synthEvent);
||||||| merged common ancestors
            LOG(("Synthesized button %u release on %p\n",
                 guint(buttonType + 1), (void *)this));

            // Dispatch a synthesized button up event to tell Gecko about the
            // change in state.  This event is marked as synthesized so that
            // it is not dispatched as a DOM event, because we don't know the
            // position, widget, modifiers, or time/order.
            WidgetMouseEvent synthEvent(true, eMouseUp, this,
                                        WidgetMouseEvent::eSynthesized);
            synthEvent.button = buttonType;
            DispatchInputEvent(&synthEvent);
        }
=======
      LOG(("Synthesized button %u release on %p\n", guint(buttonType + 1),
           (void*)this));

      // Dispatch a synthesized button up event to tell Gecko about the
      // change in state.  This event is marked as synthesized so that
      // it is not dispatched as a DOM event, because we don't know the
      // position, widget, modifiers, or time/order.
      WidgetMouseEvent synthEvent(true, eMouseUp, this,
                                  WidgetMouseEvent::eSynthesized);
      synthEvent.mButton = buttonType;
      DispatchInputEvent(&synthEvent);
>>>>>>> upstream-releases
    }
  }
}

<<<<<<< HEAD
void nsWindow::InitButtonEvent(WidgetMouseEvent &aEvent,
                               GdkEventButton *aGdkEvent) {
  aEvent.mRefPoint = GetRefPoint(this, aGdkEvent);

  guint modifierState = aGdkEvent->state;
  // aEvent's state includes the button state from immediately before this
  // event.  If aEvent is a mousedown or mouseup event, we need to update
  // the button state.
  guint buttonMask = 0;
  switch (aGdkEvent->button) {
    case 1:
      buttonMask = GDK_BUTTON1_MASK;
      break;
    case 2:
      buttonMask = GDK_BUTTON2_MASK;
      break;
    case 3:
      buttonMask = GDK_BUTTON3_MASK;
      break;
  }
  if (aGdkEvent->type == GDK_BUTTON_RELEASE) {
    modifierState &= ~buttonMask;
  } else {
    modifierState |= buttonMask;
  }
||||||| merged common ancestors
void
nsWindow::InitButtonEvent(WidgetMouseEvent& aEvent,
                          GdkEventButton* aGdkEvent)
{
    aEvent.mRefPoint = GetRefPoint(this, aGdkEvent);

    guint modifierState = aGdkEvent->state;
    // aEvent's state includes the button state from immediately before this
    // event.  If aEvent is a mousedown or mouseup event, we need to update
    // the button state.
    guint buttonMask = 0;
    switch (aGdkEvent->button) {
        case 1:
            buttonMask = GDK_BUTTON1_MASK;
            break;
        case 2:
            buttonMask = GDK_BUTTON2_MASK;
            break;
        case 3:
            buttonMask = GDK_BUTTON3_MASK;
            break;
    }
    if (aGdkEvent->type == GDK_BUTTON_RELEASE) {
        modifierState &= ~buttonMask;
    } else {
        modifierState |= buttonMask;
    }
=======
void nsWindow::InitButtonEvent(WidgetMouseEvent& aEvent,
                               GdkEventButton* aGdkEvent) {
  aEvent.mRefPoint = GetRefPoint(this, aGdkEvent);

  guint modifierState = aGdkEvent->state;
  // aEvent's state includes the button state from immediately before this
  // event.  If aEvent is a mousedown or mouseup event, we need to update
  // the button state.
  guint buttonMask = 0;
  switch (aGdkEvent->button) {
    case 1:
      buttonMask = GDK_BUTTON1_MASK;
      break;
    case 2:
      buttonMask = GDK_BUTTON2_MASK;
      break;
    case 3:
      buttonMask = GDK_BUTTON3_MASK;
      break;
  }
  if (aGdkEvent->type == GDK_BUTTON_RELEASE) {
    modifierState &= ~buttonMask;
  } else {
    modifierState |= buttonMask;
  }
>>>>>>> upstream-releases

  KeymapWrapper::InitInputEvent(aEvent, modifierState);

  aEvent.AssignEventTime(GetWidgetEventTime(aGdkEvent->time));

  switch (aGdkEvent->type) {
    case GDK_2BUTTON_PRESS:
      aEvent.mClickCount = 2;
      break;
    case GDK_3BUTTON_PRESS:
      aEvent.mClickCount = 3;
      break;
      // default is one click
    default:
      aEvent.mClickCount = 1;
  }
}

static guint ButtonMaskFromGDKButton(guint button) {
  return GDK_BUTTON1_MASK << (button - 1);
}

<<<<<<< HEAD
void nsWindow::DispatchContextMenuEventFromMouseEvent(uint16_t domButton,
                                                      GdkEventButton *aEvent) {
  if (domButton == WidgetMouseEvent::eRightButton &&
      MOZ_LIKELY(!mIsDestroyed)) {
    WidgetMouseEvent contextMenuEvent(true, eContextMenu, this,
                                      WidgetMouseEvent::eReal);
    InitButtonEvent(contextMenuEvent, aEvent);
    contextMenuEvent.pressure = mLastMotionPressure;
    DispatchInputEvent(&contextMenuEvent);
  }
||||||| merged common ancestors
void
nsWindow::DispatchContextMenuEventFromMouseEvent(uint16_t domButton,
                                                 GdkEventButton *aEvent)
{
    if (domButton == WidgetMouseEvent::eRightButton && MOZ_LIKELY(!mIsDestroyed)) {
        WidgetMouseEvent contextMenuEvent(true, eContextMenu, this,
                                          WidgetMouseEvent::eReal);
        InitButtonEvent(contextMenuEvent, aEvent);
        contextMenuEvent.pressure = mLastMotionPressure;
        DispatchInputEvent(&contextMenuEvent);
    }
=======
void nsWindow::DispatchContextMenuEventFromMouseEvent(uint16_t domButton,
                                                      GdkEventButton* aEvent) {
  if (domButton == MouseButton::eRight && MOZ_LIKELY(!mIsDestroyed)) {
    WidgetMouseEvent contextMenuEvent(true, eContextMenu, this,
                                      WidgetMouseEvent::eReal);
    InitButtonEvent(contextMenuEvent, aEvent);
    contextMenuEvent.mPressure = mLastMotionPressure;
    DispatchInputEvent(&contextMenuEvent);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsWindow::OnButtonPressEvent(GdkEventButton *aEvent) {
  LOG(("Button %u press on %p\n", aEvent->button, (void *)this));

  // If you double click in GDK, it will actually generate a second
  // GDK_BUTTON_PRESS before sending the GDK_2BUTTON_PRESS, and this is
  // different than the DOM spec.  GDK puts this in the queue
  // programatically, so it's safe to assume that if there's a
  // double click in the queue, it was generated so we can just drop
  // this click.
  GdkEvent *peekedEvent = gdk_event_peek();
  if (peekedEvent) {
    GdkEventType type = peekedEvent->any.type;
    gdk_event_free(peekedEvent);
    if (type == GDK_2BUTTON_PRESS || type == GDK_3BUTTON_PRESS) return;
  }
||||||| merged common ancestors
void
nsWindow::OnButtonPressEvent(GdkEventButton *aEvent)
{
    LOG(("Button %u press on %p\n", aEvent->button, (void *)this));

    // If you double click in GDK, it will actually generate a second
    // GDK_BUTTON_PRESS before sending the GDK_2BUTTON_PRESS, and this is
    // different than the DOM spec.  GDK puts this in the queue
    // programatically, so it's safe to assume that if there's a
    // double click in the queue, it was generated so we can just drop
    // this click.
    GdkEvent *peekedEvent = gdk_event_peek();
    if (peekedEvent) {
        GdkEventType type = peekedEvent->any.type;
        gdk_event_free(peekedEvent);
        if (type == GDK_2BUTTON_PRESS || type == GDK_3BUTTON_PRESS)
            return;
    }
=======
void nsWindow::OnButtonPressEvent(GdkEventButton* aEvent) {
  LOG(("Button %u press on %p\n", aEvent->button, (void*)this));

  // If you double click in GDK, it will actually generate a second
  // GDK_BUTTON_PRESS before sending the GDK_2BUTTON_PRESS, and this is
  // different than the DOM spec.  GDK puts this in the queue
  // programatically, so it's safe to assume that if there's a
  // double click in the queue, it was generated so we can just drop
  // this click.
  GdkEvent* peekedEvent = gdk_event_peek();
  if (peekedEvent) {
    GdkEventType type = peekedEvent->any.type;
    gdk_event_free(peekedEvent);
    if (type == GDK_2BUTTON_PRESS || type == GDK_3BUTTON_PRESS) return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsWindow *containerWindow = GetContainerWindow();
  if (!gFocusWindow && containerWindow) {
    containerWindow->DispatchActivateEvent();
  }
||||||| merged common ancestors
    nsWindow *containerWindow = GetContainerWindow();
    if (!gFocusWindow && containerWindow) {
        containerWindow->DispatchActivateEvent();
    }
=======
  nsWindow* containerWindow = GetContainerWindow();
  if (!gFocusWindow && containerWindow) {
    containerWindow->DispatchActivateEvent();
  }
>>>>>>> upstream-releases

  // check to see if we should rollup
  if (CheckForRollup(aEvent->x_root, aEvent->y_root, false, false)) return;

<<<<<<< HEAD
  gdouble pressure = 0;
  gdk_event_get_axis((GdkEvent *)aEvent, GDK_AXIS_PRESSURE, &pressure);
  mLastMotionPressure = pressure;
||||||| merged common ancestors
    gdouble pressure = 0;
    gdk_event_get_axis ((GdkEvent*)aEvent, GDK_AXIS_PRESSURE, &pressure);
    mLastMotionPressure = pressure;
=======
  gdouble pressure = 0;
  gdk_event_get_axis((GdkEvent*)aEvent, GDK_AXIS_PRESSURE, &pressure);
  mLastMotionPressure = pressure;
>>>>>>> upstream-releases

  uint16_t domButton;
  switch (aEvent->button) {
    case 1:
<<<<<<< HEAD
      domButton = WidgetMouseEvent::eLeftButton;
      break;
||||||| merged common ancestors
        domButton = WidgetMouseEvent::eLeftButton;
        break;
=======
      domButton = MouseButton::eLeft;
      break;
>>>>>>> upstream-releases
    case 2:
<<<<<<< HEAD
      domButton = WidgetMouseEvent::eMiddleButton;
      break;
||||||| merged common ancestors
        domButton = WidgetMouseEvent::eMiddleButton;
        break;
=======
      domButton = MouseButton::eMiddle;
      break;
>>>>>>> upstream-releases
    case 3:
<<<<<<< HEAD
      domButton = WidgetMouseEvent::eRightButton;
      break;
||||||| merged common ancestors
        domButton = WidgetMouseEvent::eRightButton;
        break;
=======
      domButton = MouseButton::eRight;
      break;
>>>>>>> upstream-releases
    // These are mapped to horizontal scroll
    case 6:
    case 7:
      NS_WARNING("We're not supporting legacy horizontal scroll event");
      return;
    // Map buttons 8-9 to back/forward
    case 8:
      DispatchCommandEvent(nsGkAtoms::Back);
      return;
    case 9:
      DispatchCommandEvent(nsGkAtoms::Forward);
      return;
    default:
      return;
  }

  gButtonState |= ButtonMaskFromGDKButton(aEvent->button);

<<<<<<< HEAD
  WidgetMouseEvent event(true, eMouseDown, this, WidgetMouseEvent::eReal);
  event.button = domButton;
  InitButtonEvent(event, aEvent);
  event.pressure = mLastMotionPressure;
||||||| merged common ancestors
    WidgetMouseEvent event(true, eMouseDown, this, WidgetMouseEvent::eReal);
    event.button = domButton;
    InitButtonEvent(event, aEvent);
    event.pressure = mLastMotionPressure;
=======
  WidgetMouseEvent event(true, eMouseDown, this, WidgetMouseEvent::eReal);
  event.mButton = domButton;
  InitButtonEvent(event, aEvent);
  event.mPressure = mLastMotionPressure;
>>>>>>> upstream-releases

<<<<<<< HEAD
  DispatchInputEvent(&event);
||||||| merged common ancestors
    DispatchInputEvent(&event);
=======
  nsEventStatus eventStatus = DispatchInputEvent(&event);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // right menu click on linux should also pop up a context menu
  if (!nsBaseWidget::ShowContextMenuAfterMouseUp()) {
    DispatchContextMenuEventFromMouseEvent(domButton, aEvent);
  }
||||||| merged common ancestors
    // right menu click on linux should also pop up a context menu
    if (!nsBaseWidget::ShowContextMenuAfterMouseUp()) {
        DispatchContextMenuEventFromMouseEvent(domButton, aEvent);
    }
=======
  LayoutDeviceIntPoint refPoint =
      GdkEventCoordsToDevicePixels(aEvent->x, aEvent->y);
  if (mDraggableRegion.Contains(refPoint.x, refPoint.y) &&
      domButton == MouseButton::eLeft &&
      eventStatus != nsEventStatus_eConsumeNoDefault) {
    mWindowShouldStartDragging = true;
  }

  // right menu click on linux should also pop up a context menu
  if (!nsBaseWidget::ShowContextMenuAfterMouseUp()) {
    DispatchContextMenuEventFromMouseEvent(domButton, aEvent);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsWindow::OnButtonReleaseEvent(GdkEventButton *aEvent) {
  LOG(("Button %u release on %p\n", aEvent->button, (void *)this));
||||||| merged common ancestors
void
nsWindow::OnButtonReleaseEvent(GdkEventButton *aEvent)
{
    LOG(("Button %u release on %p\n", aEvent->button, (void *)this));
=======
void nsWindow::OnButtonReleaseEvent(GdkEventButton* aEvent) {
  LOG(("Button %u release on %p\n", aEvent->button, (void*)this));

  if (mWindowShouldStartDragging) {
    mWindowShouldStartDragging = false;
  }
>>>>>>> upstream-releases

  uint16_t domButton;
  switch (aEvent->button) {
    case 1:
<<<<<<< HEAD
      domButton = WidgetMouseEvent::eLeftButton;
      break;
||||||| merged common ancestors
        domButton = WidgetMouseEvent::eLeftButton;
        break;
=======
      domButton = MouseButton::eLeft;
      break;
>>>>>>> upstream-releases
    case 2:
<<<<<<< HEAD
      domButton = WidgetMouseEvent::eMiddleButton;
      break;
||||||| merged common ancestors
        domButton = WidgetMouseEvent::eMiddleButton;
        break;
=======
      domButton = MouseButton::eMiddle;
      break;
>>>>>>> upstream-releases
    case 3:
<<<<<<< HEAD
      domButton = WidgetMouseEvent::eRightButton;
      break;
||||||| merged common ancestors
        domButton = WidgetMouseEvent::eRightButton;
        break;
=======
      domButton = MouseButton::eRight;
      break;
>>>>>>> upstream-releases
    default:
      return;
  }

<<<<<<< HEAD
  gButtonState &= ~ButtonMaskFromGDKButton(aEvent->button);

  WidgetMouseEvent event(true, eMouseUp, this, WidgetMouseEvent::eReal);
  event.button = domButton;
  InitButtonEvent(event, aEvent);
  gdouble pressure = 0;
  gdk_event_get_axis((GdkEvent *)aEvent, GDK_AXIS_PRESSURE, &pressure);
  event.pressure = pressure ? pressure : mLastMotionPressure;

  // The mRefPoint is manipulated in DispatchInputEvent, we're saving it
  // to use it for the doubleclick position check.
  LayoutDeviceIntPoint pos = event.mRefPoint;

  nsEventStatus eventStatus = DispatchInputEvent(&event);

  bool defaultPrevented = (eventStatus == nsEventStatus_eConsumeNoDefault);
  // Check if mouse position in titlebar and doubleclick happened to
  // trigger restore/maximize.
  if (!defaultPrevented && mDrawInTitlebar &&
      event.button == WidgetMouseEvent::eLeftButton && event.mClickCount == 2 &&
      mDraggableRegion.Contains(pos.x, pos.y)) {
    if (mSizeState == nsSizeMode_Maximized) {
      SetSizeMode(nsSizeMode_Normal);
    } else {
      SetSizeMode(nsSizeMode_Maximized);
||||||| merged common ancestors
    gButtonState &= ~ButtonMaskFromGDKButton(aEvent->button);

    WidgetMouseEvent event(true, eMouseUp, this,
                           WidgetMouseEvent::eReal);
    event.button = domButton;
    InitButtonEvent(event, aEvent);
    gdouble pressure = 0;
    gdk_event_get_axis ((GdkEvent*)aEvent, GDK_AXIS_PRESSURE, &pressure);
    event.pressure = pressure ? pressure : mLastMotionPressure;

    // The mRefPoint is manipulated in DispatchInputEvent, we're saving it
    // to use it for the doubleclick position check.
    LayoutDeviceIntPoint pos = event.mRefPoint;

    nsEventStatus eventStatus = DispatchInputEvent(&event);

    bool defaultPrevented = (eventStatus == nsEventStatus_eConsumeNoDefault);
    // Check if mouse position in titlebar and doubleclick happened to
    // trigger restore/maximize.
    if (!defaultPrevented
             && mDrawInTitlebar
             && event.button == WidgetMouseEvent::eLeftButton
             && event.mClickCount == 2
             && mDraggableRegion.Contains(pos.x, pos.y)) {

        if (mSizeState == nsSizeMode_Maximized) {
            SetSizeMode(nsSizeMode_Normal);
        } else {
            SetSizeMode(nsSizeMode_Maximized);
        }
=======
  gButtonState &= ~ButtonMaskFromGDKButton(aEvent->button);

  WidgetMouseEvent event(true, eMouseUp, this, WidgetMouseEvent::eReal);
  event.mButton = domButton;
  InitButtonEvent(event, aEvent);
  gdouble pressure = 0;
  gdk_event_get_axis((GdkEvent*)aEvent, GDK_AXIS_PRESSURE, &pressure);
  event.mPressure = pressure ? pressure : mLastMotionPressure;

  // The mRefPoint is manipulated in DispatchInputEvent, we're saving it
  // to use it for the doubleclick position check.
  LayoutDeviceIntPoint pos = event.mRefPoint;

  nsEventStatus eventStatus = DispatchInputEvent(&event);

  bool defaultPrevented = (eventStatus == nsEventStatus_eConsumeNoDefault);
  // Check if mouse position in titlebar and doubleclick happened to
  // trigger restore/maximize.
  if (!defaultPrevented && mDrawInTitlebar &&
      event.mButton == MouseButton::eLeft && event.mClickCount == 2 &&
      mDraggableRegion.Contains(pos.x, pos.y)) {
    if (mSizeState == nsSizeMode_Maximized) {
      SetSizeMode(nsSizeMode_Normal);
    } else {
      SetSizeMode(nsSizeMode_Maximized);
>>>>>>> upstream-releases
    }
  }
  mLastMotionPressure = pressure;

  // right menu click on linux should also pop up a context menu
  if (nsBaseWidget::ShowContextMenuAfterMouseUp()) {
    DispatchContextMenuEventFromMouseEvent(domButton, aEvent);
  }
}

<<<<<<< HEAD
void nsWindow::OnContainerFocusInEvent(GdkEventFocus *aEvent) {
  LOGFOCUS(("OnContainerFocusInEvent [%p]\n", (void *)this));
||||||| merged common ancestors
void
nsWindow::OnContainerFocusInEvent(GdkEventFocus *aEvent)
{
    LOGFOCUS(("OnContainerFocusInEvent [%p]\n", (void *)this));

    // Unset the urgency hint, if possible
    GtkWidget* top_window = GetToplevelWidget();
    if (top_window && (gtk_widget_get_visible(top_window)))
        SetUrgencyHint(top_window, false);

    // Return if being called within SetFocus because the focus manager
    // already knows that the window is active.
    if (gBlockActivateEvent) {
        LOGFOCUS(("activated notification is blocked [%p]\n", (void *)this));
        return;
    }
=======
void nsWindow::OnContainerFocusInEvent(GdkEventFocus* aEvent) {
  LOGFOCUS(("OnContainerFocusInEvent [%p]\n", (void*)this));

  // Unset the urgency hint, if possible
  GtkWidget* top_window = GetToplevelWidget();
  if (top_window && (gtk_widget_get_visible(top_window)))
    SetUrgencyHint(top_window, false);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Unset the urgency hint, if possible
  GtkWidget *top_window = GetToplevelWidget();
  if (top_window && (gtk_widget_get_visible(top_window)))
    SetUrgencyHint(top_window, false);

  // Return if being called within SetFocus because the focus manager
  // already knows that the window is active.
  if (gBlockActivateEvent) {
    LOGFOCUS(("activated notification is blocked [%p]\n", (void *)this));
    return;
  }
||||||| merged common ancestors
    // If keyboard input will be accepted, the focus manager will call
    // SetFocus to set the correct window.
    gFocusWindow = nullptr;
=======
  // Return if being called within SetFocus because the focus manager
  // already knows that the window is active.
  if (gBlockActivateEvent) {
    LOGFOCUS(("activated notification is blocked [%p]\n", (void*)this));
    return;
  }
>>>>>>> upstream-releases

  // If keyboard input will be accepted, the focus manager will call
  // SetFocus to set the correct window.
  gFocusWindow = nullptr;

<<<<<<< HEAD
  DispatchActivateEvent();

  if (!gFocusWindow) {
    // We don't really have a window for dispatching key events, but
    // setting a non-nullptr value here prevents OnButtonPressEvent() from
    // dispatching an activation notification if the widget is already
    // active.
    gFocusWindow = this;
  }
||||||| merged common ancestors
    if (!gFocusWindow) {
        // We don't really have a window for dispatching key events, but
        // setting a non-nullptr value here prevents OnButtonPressEvent() from
        // dispatching an activation notification if the widget is already
        // active.
        gFocusWindow = this;
    }
=======
  DispatchActivateEvent();
>>>>>>> upstream-releases

<<<<<<< HEAD
  LOGFOCUS(("Events sent from focus in event [%p]\n", (void *)this));
||||||| merged common ancestors
    LOGFOCUS(("Events sent from focus in event [%p]\n", (void *)this));
=======
  if (!gFocusWindow) {
    // We don't really have a window for dispatching key events, but
    // setting a non-nullptr value here prevents OnButtonPressEvent() from
    // dispatching an activation notification if the widget is already
    // active.
    gFocusWindow = this;
  }

  LOGFOCUS(("Events sent from focus in event [%p]\n", (void*)this));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsWindow::OnContainerFocusOutEvent(GdkEventFocus *aEvent) {
  LOGFOCUS(("OnContainerFocusOutEvent [%p]\n", (void *)this));
||||||| merged common ancestors
void
nsWindow::OnContainerFocusOutEvent(GdkEventFocus *aEvent)
{
    LOGFOCUS(("OnContainerFocusOutEvent [%p]\n", (void *)this));

    if (mWindowType == eWindowType_toplevel || mWindowType == eWindowType_dialog) {
        nsCOMPtr<nsIDragService> dragService = do_GetService(kCDragServiceCID);
        nsCOMPtr<nsIDragSession> dragSession;
        dragService->GetCurrentSession(getter_AddRefs(dragSession));

        // Rollup popups when a window is focused out unless a drag is occurring.
        // This check is because drags grab the keyboard and cause a focus out on
        // versions of GTK before 2.18.
        bool shouldRollup = !dragSession;
        if (!shouldRollup) {
            // we also roll up when a drag is from a different application
            nsCOMPtr<nsINode> sourceNode;
            dragSession->GetSourceNode(getter_AddRefs(sourceNode));
            shouldRollup = (sourceNode == nullptr);
        }
=======
void nsWindow::OnContainerFocusOutEvent(GdkEventFocus* aEvent) {
  LOGFOCUS(("OnContainerFocusOutEvent [%p]\n", (void*)this));
>>>>>>> upstream-releases

  if (mWindowType == eWindowType_toplevel ||
      mWindowType == eWindowType_dialog) {
    nsCOMPtr<nsIDragService> dragService = do_GetService(kCDragServiceCID);
    nsCOMPtr<nsIDragSession> dragSession;
    dragService->GetCurrentSession(getter_AddRefs(dragSession));

    // Rollup popups when a window is focused out unless a drag is occurring.
    // This check is because drags grab the keyboard and cause a focus out on
    // versions of GTK before 2.18.
    bool shouldRollup = !dragSession;
    if (!shouldRollup) {
      // we also roll up when a drag is from a different application
      nsCOMPtr<nsINode> sourceNode;
      dragSession->GetSourceNode(getter_AddRefs(sourceNode));
      shouldRollup = (sourceNode == nullptr);
    }

    if (shouldRollup) {
      CheckForRollup(0, 0, false, true);
    }
  }

  if (gFocusWindow) {
    RefPtr<nsWindow> kungFuDeathGrip = gFocusWindow;
    if (gFocusWindow->mIMContext) {
      gFocusWindow->mIMContext->OnBlurWindow(gFocusWindow);
    }
    gFocusWindow = nullptr;
  }

  DispatchDeactivateEvent();

<<<<<<< HEAD
  LOGFOCUS(("Done with container focus out [%p]\n", (void *)this));
||||||| merged common ancestors
    LOGFOCUS(("Done with container focus out [%p]\n", (void *)this));
=======
  LOGFOCUS(("Done with container focus out [%p]\n", (void*)this));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool nsWindow::DispatchCommandEvent(nsAtom *aCommand) {
  nsEventStatus status;
  WidgetCommandEvent appCommandEvent(true, aCommand, this);
  DispatchEvent(&appCommandEvent, status);
  return TRUE;
||||||| merged common ancestors
bool
nsWindow::DispatchCommandEvent(nsAtom* aCommand)
{
    nsEventStatus status;
    WidgetCommandEvent appCommandEvent(true, aCommand, this);
    DispatchEvent(&appCommandEvent, status);
    return TRUE;
=======
bool nsWindow::DispatchCommandEvent(nsAtom* aCommand) {
  nsEventStatus status;
  WidgetCommandEvent appCommandEvent(true, aCommand, this);
  DispatchEvent(&appCommandEvent, status);
  return TRUE;
>>>>>>> upstream-releases
}

bool nsWindow::DispatchContentCommandEvent(EventMessage aMsg) {
  nsEventStatus status;
  WidgetContentCommandEvent event(true, aMsg, this);
  DispatchEvent(&event, status);
  return TRUE;
}

<<<<<<< HEAD
static bool IsCtrlAltTab(GdkEventKey *aEvent) {
  return aEvent->keyval == GDK_Tab &&
         KeymapWrapper::AreModifiersActive(
             KeymapWrapper::CTRL | KeymapWrapper::ALT, aEvent->state);
}

bool nsWindow::DispatchKeyDownOrKeyUpEvent(GdkEventKey *aEvent,
                                           bool aIsProcessedByIME,
                                           bool *aIsCancelled) {
  MOZ_ASSERT(aIsCancelled, "aIsCancelled must not be nullptr");

  *aIsCancelled = false;

  if (aEvent->type == GDK_KEY_PRESS && IsCtrlAltTab(aEvent)) {
    return false;
  }

  EventMessage message = aEvent->type == GDK_KEY_PRESS ? eKeyDown : eKeyUp;
  WidgetKeyboardEvent keyEvent(true, message, this);
  KeymapWrapper::InitKeyEvent(keyEvent, aEvent, aIsProcessedByIME);
  return DispatchKeyDownOrKeyUpEvent(keyEvent, aIsCancelled);
||||||| merged common ancestors
static bool
IsCtrlAltTab(GdkEventKey *aEvent)
{
    return aEvent->keyval == GDK_Tab &&
        KeymapWrapper::AreModifiersActive(
            KeymapWrapper::CTRL | KeymapWrapper::ALT, aEvent->state);
}

bool
nsWindow::DispatchKeyDownOrKeyUpEvent(GdkEventKey* aEvent,
                                      bool aIsProcessedByIME,
                                      bool* aIsCancelled)
{
    MOZ_ASSERT(aIsCancelled, "aIsCancelled must not be nullptr");

    *aIsCancelled = false;

    if (aEvent->type == GDK_KEY_PRESS && IsCtrlAltTab(aEvent)) {
        return false;
    }

    EventMessage message =
        aEvent->type == GDK_KEY_PRESS ? eKeyDown : eKeyUp;
    WidgetKeyboardEvent keyEvent(true, message, this);
    KeymapWrapper::InitKeyEvent(keyEvent, aEvent, aIsProcessedByIME);
    return DispatchKeyDownOrKeyUpEvent(keyEvent, aIsCancelled);
=======
WidgetEventTime nsWindow::GetWidgetEventTime(guint32 aEventTime) {
  return WidgetEventTime(aEventTime, GetEventTimeStamp(aEventTime));
>>>>>>> upstream-releases
}
<<<<<<< HEAD
bool nsWindow::DispatchKeyDownOrKeyUpEvent(WidgetKeyboardEvent &aKeyboardEvent,
                                           bool *aIsCancelled) {
  MOZ_ASSERT(aIsCancelled, "aIsCancelled must not be nullptr");
||||||| merged common ancestors
bool
nsWindow::DispatchKeyDownOrKeyUpEvent(WidgetKeyboardEvent& aKeyboardEvent,
                                      bool* aIsCancelled)
{
    MOZ_ASSERT(aIsCancelled, "aIsCancelled must not be nullptr");
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
  *aIsCancelled = false;
||||||| merged common ancestors
    *aIsCancelled = false;
=======
TimeStamp nsWindow::GetEventTimeStamp(guint32 aEventTime) {
  if (MOZ_UNLIKELY(!mGdkWindow)) {
    // nsWindow has been Destroy()ed.
    return TimeStamp::Now();
  }
  if (aEventTime == 0) {
    // Some X11 and GDK events may be received with a time of 0 to indicate
    // that they are synthetic events. Some input method editors do this.
    // In this case too, just return the current timestamp.
    return TimeStamp::Now();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RefPtr<TextEventDispatcher> dispatcher = GetTextEventDispatcher();
  nsresult rv = dispatcher->BeginNativeInputTransaction();
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return FALSE;
  }
||||||| merged common ancestors
    RefPtr<TextEventDispatcher> dispatcher = GetTextEventDispatcher();
    nsresult rv = dispatcher->BeginNativeInputTransaction();
    if (NS_WARN_IF(NS_FAILED(rv))) {
        return FALSE;
    }
=======
  TimeStamp eventTimeStamp;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsEventStatus status = nsEventStatus_eIgnore;
  bool dispatched = dispatcher->DispatchKeyboardEvent(
      aKeyboardEvent.mMessage, aKeyboardEvent, status, nullptr);
  *aIsCancelled = (status == nsEventStatus_eConsumeNoDefault);
  return dispatched;
||||||| merged common ancestors
    nsEventStatus status = nsEventStatus_eIgnore;
    bool dispatched =
        dispatcher->DispatchKeyboardEvent(aKeyboardEvent.mMessage,
                                          aKeyboardEvent, status, nullptr);
    *aIsCancelled = (status == nsEventStatus_eConsumeNoDefault);
    return dispatched;
=======
  if (!mIsX11Display) {
    // Wayland compositors use monotonic time to set timestamps.
    int64_t timestampTime = g_get_monotonic_time() / 1000;
    guint32 refTimeTruncated = guint32(timestampTime);

    timestampTime -= refTimeTruncated - aEventTime;
    int64_t tick =
        BaseTimeDurationPlatformUtils::TicksFromMilliseconds(timestampTime);
    eventTimeStamp = TimeStamp::FromSystemTime(tick);
  } else {
    CurrentX11TimeGetter* getCurrentTime = GetCurrentTimeGetter();
    MOZ_ASSERT(getCurrentTime,
               "Null current time getter despite having a window");
    eventTimeStamp =
        TimeConverter().GetTimeStampFromSystemTime(aEventTime, *getCurrentTime);
  }
  return eventTimeStamp;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
WidgetEventTime nsWindow::GetWidgetEventTime(guint32 aEventTime) {
  return WidgetEventTime(aEventTime, GetEventTimeStamp(aEventTime));
||||||| merged common ancestors
WidgetEventTime
nsWindow::GetWidgetEventTime(guint32 aEventTime)
{
  return WidgetEventTime(aEventTime, GetEventTimeStamp(aEventTime));
=======
mozilla::CurrentX11TimeGetter* nsWindow::GetCurrentTimeGetter() {
  MOZ_ASSERT(mGdkWindow, "Expected mGdkWindow to be set");
  if (MOZ_UNLIKELY(!mCurrentTimeGetter)) {
    mCurrentTimeGetter = MakeUnique<CurrentX11TimeGetter>(mGdkWindow);
  }
  return mCurrentTimeGetter.get();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
TimeStamp nsWindow::GetEventTimeStamp(guint32 aEventTime) {
  if (MOZ_UNLIKELY(!mGdkWindow)) {
    // nsWindow has been Destroy()ed.
    return TimeStamp::Now();
  }
  if (aEventTime == 0) {
    // Some X11 and GDK events may be received with a time of 0 to indicate
    // that they are synthetic events. Some input method editors do this.
    // In this case too, just return the current timestamp.
    return TimeStamp::Now();
  }

  TimeStamp eventTimeStamp;

  if (!mIsX11Display) {
    // Wayland compositors use monotonic time to set timestamps.
    int64_t timestampTime = g_get_monotonic_time() / 1000;
    guint32 refTimeTruncated = guint32(timestampTime);

    timestampTime -= refTimeTruncated - aEventTime;
    int64_t tick =
        BaseTimeDurationPlatformUtils::TicksFromMilliseconds(timestampTime);
    eventTimeStamp = TimeStamp::FromSystemTime(tick);
  } else {
    CurrentX11TimeGetter *getCurrentTime = GetCurrentTimeGetter();
    MOZ_ASSERT(getCurrentTime,
               "Null current time getter despite having a window");
    eventTimeStamp =
        TimeConverter().GetTimeStampFromSystemTime(aEventTime, *getCurrentTime);
  }
  return eventTimeStamp;
}
||||||| merged common ancestors
TimeStamp
nsWindow::GetEventTimeStamp(guint32 aEventTime)
{
    if (MOZ_UNLIKELY(!mGdkWindow)) {
        // nsWindow has been Destroy()ed.
        return TimeStamp::Now();
    }
    if (aEventTime == 0) {
        // Some X11 and GDK events may be received with a time of 0 to indicate
        // that they are synthetic events. Some input method editors do this.
        // In this case too, just return the current timestamp.
        return TimeStamp::Now();
    }

    TimeStamp eventTimeStamp;

    if (!mIsX11Display) {
        // Wayland compositors use monotonic time to set timestamps.
        int64_t timestampTime = g_get_monotonic_time() / 1000;
        guint32 refTimeTruncated = guint32(timestampTime);

        timestampTime -= refTimeTruncated - aEventTime;
        int64_t tick =
            BaseTimeDurationPlatformUtils::TicksFromMilliseconds(timestampTime);
        eventTimeStamp = TimeStamp::FromSystemTime(tick);
    } else {
        CurrentX11TimeGetter* getCurrentTime = GetCurrentTimeGetter();
        MOZ_ASSERT(getCurrentTime,
                   "Null current time getter despite having a window");
        eventTimeStamp = TimeConverter().GetTimeStampFromSystemTime(aEventTime,
                                                              *getCurrentTime);
    }
    return eventTimeStamp;
}
=======
gboolean nsWindow::OnKeyPressEvent(GdkEventKey* aEvent) {
  LOGFOCUS(("OnKeyPressEvent [%p]\n", (void*)this));
>>>>>>> upstream-releases

<<<<<<< HEAD
mozilla::CurrentX11TimeGetter *nsWindow::GetCurrentTimeGetter() {
  MOZ_ASSERT(mGdkWindow, "Expected mGdkWindow to be set");
  if (MOZ_UNLIKELY(!mCurrentTimeGetter)) {
    mCurrentTimeGetter = MakeUnique<CurrentX11TimeGetter>(mGdkWindow);
  }
  return mCurrentTimeGetter.get();
||||||| merged common ancestors
mozilla::CurrentX11TimeGetter*
nsWindow::GetCurrentTimeGetter() {
    MOZ_ASSERT(mGdkWindow, "Expected mGdkWindow to be set");
    if (MOZ_UNLIKELY(!mCurrentTimeGetter)) {
        mCurrentTimeGetter = MakeUnique<CurrentX11TimeGetter>(mGdkWindow);
    }
    return mCurrentTimeGetter.get();
=======
  RefPtr<nsWindow> self(this);
  KeymapWrapper::HandleKeyPressEvent(self, aEvent);
  return TRUE;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
gboolean nsWindow::OnKeyPressEvent(GdkEventKey *aEvent) {
  LOGFOCUS(("OnKeyPressEvent [%p]\n", (void *)this));
||||||| merged common ancestors
gboolean
nsWindow::OnKeyPressEvent(GdkEventKey *aEvent)
{
    LOGFOCUS(("OnKeyPressEvent [%p]\n", (void *)this));

    // if we are in the middle of composing text, XIM gets to see it
    // before mozilla does.
    // FYI: Don't dispatch keydown event before notifying IME of the event
    //      because IME may send a key event synchronously and consume the
    //      original event.
    bool IMEWasEnabled = false;
    if (mIMContext) {
        IMEWasEnabled = mIMContext->IsEnabled();
        if (mIMContext->OnKeyEvent(this, aEvent)) {
            return TRUE;
        }
    }
=======
gboolean nsWindow::OnKeyReleaseEvent(GdkEventKey* aEvent) {
  LOGFOCUS(("OnKeyReleaseEvent [%p]\n", (void*)this));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // if we are in the middle of composing text, XIM gets to see it
  // before mozilla does.
  // FYI: Don't dispatch keydown event before notifying IME of the event
  //      because IME may send a key event synchronously and consume the
  //      original event.
  bool IMEWasEnabled = false;
  KeyHandlingState handlingState = KeyHandlingState::eNotHandled;
  if (mIMContext) {
    IMEWasEnabled = mIMContext->IsEnabled();
    handlingState = mIMContext->OnKeyEvent(this, aEvent);
    if (handlingState == KeyHandlingState::eHandled) {
      return TRUE;
    }
  }

  // work around for annoying things.
  if (IsCtrlAltTab(aEvent)) {
    return TRUE;
  }
||||||| merged common ancestors
    // work around for annoying things.
    if (IsCtrlAltTab(aEvent)) {
        return TRUE;
    }
=======
  RefPtr<nsWindow> self(this);
  if (NS_WARN_IF(!KeymapWrapper::HandleKeyReleaseEvent(self, aEvent))) {
    return FALSE;
  }
  return TRUE;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIWidget> kungFuDeathGrip = this;
||||||| merged common ancestors
    nsCOMPtr<nsIWidget> kungFuDeathGrip = this;
=======
void nsWindow::OnScrollEvent(GdkEventScroll* aEvent) {
  // check to see if we should rollup
  if (CheckForRollup(aEvent->x_root, aEvent->y_root, true, false)) return;
#if GTK_CHECK_VERSION(3, 4, 0)
  // check for duplicate legacy scroll event, see GNOME bug 726878
  if (aEvent->direction != GDK_SCROLL_SMOOTH &&
      mLastScrollEventTime == aEvent->time) {
    LOG(("[%d] duplicate legacy scroll event %d\n", aEvent->time,
         aEvent->direction));
    return;
  }
#endif
  WidgetWheelEvent wheelEvent(true, eWheel, this);
  wheelEvent.mDeltaMode = dom::WheelEvent_Binding::DOM_DELTA_LINE;
  switch (aEvent->direction) {
#if GTK_CHECK_VERSION(3, 4, 0)
    case GDK_SCROLL_SMOOTH: {
      // As of GTK 3.4, all directional scroll events are provided by
      // the GDK_SCROLL_SMOOTH direction on XInput2 and Wayland devices.
      mLastScrollEventTime = aEvent->time;

      // Special handling for touchpads to support flings
      // (also known as kinetic/inertial/momentum scrolling)
      GdkDevice* device = gdk_event_get_source_device((GdkEvent*)aEvent);
      GdkInputSource source = gdk_device_get_source(device);
      if (source == GDK_SOURCE_TOUCHSCREEN || source == GDK_SOURCE_TOUCHPAD) {
        if (StaticPrefs::apz_gtk_kinetic_scroll_enabled() &&
            gtk_check_version(3, 20, 0) == nullptr) {
          static auto sGdkEventIsScrollStopEvent =
              (gboolean(*)(const GdkEvent*))dlsym(
                  RTLD_DEFAULT, "gdk_event_is_scroll_stop_event");

          LOG(("[%d] pan smooth event dx=%f dy=%f inprogress=%d\n",
               aEvent->time, aEvent->delta_x, aEvent->delta_y, mPanInProgress));
          PanGestureInput::PanGestureType eventType =
              PanGestureInput::PANGESTURE_PAN;
          if (sGdkEventIsScrollStopEvent((GdkEvent*)aEvent)) {
            eventType = PanGestureInput::PANGESTURE_END;
            mPanInProgress = false;
          } else if (!mPanInProgress) {
            eventType = PanGestureInput::PANGESTURE_START;
            mPanInProgress = true;
          }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Dispatch keydown event always.  At auto repeating, we should send
  // KEYDOWN -> KEYPRESS -> KEYDOWN -> KEYPRESS ... -> KEYUP
  // However, old distributions (e.g., Ubuntu 9.10) sent native key
  // release event, so, on such platform, the DOM events will be:
  // KEYDOWN -> KEYPRESS -> KEYUP -> KEYDOWN -> KEYPRESS -> KEYUP...
||||||| merged common ancestors
    // Dispatch keydown event always.  At auto repeating, we should send
    // KEYDOWN -> KEYPRESS -> KEYDOWN -> KEYPRESS ... -> KEYUP
    // However, old distributions (e.g., Ubuntu 9.10) sent native key
    // release event, so, on such platform, the DOM events will be:
    // KEYDOWN -> KEYPRESS -> KEYUP -> KEYDOWN -> KEYPRESS -> KEYUP...
=======
          LayoutDeviceIntPoint touchPoint = GetRefPoint(this, aEvent);
          PanGestureInput panEvent(
              eventType, aEvent->time, GetEventTimeStamp(aEvent->time),
              ScreenPoint(touchPoint.x, touchPoint.y),
              ScreenPoint(aEvent->delta_x, aEvent->delta_y),
              KeymapWrapper::ComputeKeyModifiers(aEvent->state));
          panEvent.mDeltaType = PanGestureInput::PANDELTA_PAGE;
          panEvent.mSimulateMomentum = true;
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isKeyDownCancelled = false;
  if (handlingState == KeyHandlingState::eNotHandled) {
    if (DispatchKeyDownOrKeyUpEvent(aEvent, false, &isKeyDownCancelled) &&
        (MOZ_UNLIKELY(mIsDestroyed) || isKeyDownCancelled)) {
      return TRUE;
    }
    handlingState = KeyHandlingState::eNotHandledButEventDispatched;
  }
||||||| merged common ancestors
    bool isKeyDownCancelled = false;
    if (DispatchKeyDownOrKeyUpEvent(aEvent, false, &isKeyDownCancelled) &&
        (MOZ_UNLIKELY(mIsDestroyed) || isKeyDownCancelled)) {
        return TRUE;
    }
=======
          DispatchPanGestureInput(panEvent);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If a keydown event handler causes to enable IME, i.e., it moves
  // focus from IME unusable content to IME usable editor, we should
  // send the native key event to IME for the first input on the editor.
  if (!IMEWasEnabled && mIMContext && mIMContext->IsEnabled()) {
    // Notice our keydown event was already dispatched.  This prevents
    // unnecessary DOM keydown event in the editor.
    handlingState = mIMContext->OnKeyEvent(this, aEvent, true);
    if (handlingState == KeyHandlingState::eHandled) {
      return TRUE;
    }
  }
||||||| merged common ancestors
    // If a keydown event handler causes to enable IME, i.e., it moves
    // focus from IME unusable content to IME usable editor, we should
    // send the native key event to IME for the first input on the editor.
    if (!IMEWasEnabled && mIMContext && mIMContext->IsEnabled()) {
        // Notice our keydown event was already dispatched.  This prevents
        // unnecessary DOM keydown event in the editor.
        if (mIMContext->OnKeyEvent(this, aEvent, true)) {
            return TRUE;
        }
    }
=======
          return;
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Look for specialized app-command keys
  switch (aEvent->keyval) {
    case GDK_Back:
      return DispatchCommandEvent(nsGkAtoms::Back);
    case GDK_Forward:
      return DispatchCommandEvent(nsGkAtoms::Forward);
    case GDK_Refresh:
      return DispatchCommandEvent(nsGkAtoms::Reload);
    case GDK_Stop:
      return DispatchCommandEvent(nsGkAtoms::Stop);
    case GDK_Search:
      return DispatchCommandEvent(nsGkAtoms::Search);
    case GDK_Favorites:
      return DispatchCommandEvent(nsGkAtoms::Bookmarks);
    case GDK_HomePage:
      return DispatchCommandEvent(nsGkAtoms::Home);
    case GDK_Copy:
    case GDK_F16:  // F16, F20, F18, F14 are old keysyms for Copy Cut Paste Undo
      return DispatchContentCommandEvent(eContentCommandCopy);
    case GDK_Cut:
    case GDK_F20:
      return DispatchContentCommandEvent(eContentCommandCut);
    case GDK_Paste:
    case GDK_F18:
      return DispatchContentCommandEvent(eContentCommandPaste);
    case GDK_Redo:
      return DispatchContentCommandEvent(eContentCommandRedo);
    case GDK_Undo:
    case GDK_F14:
      return DispatchContentCommandEvent(eContentCommandUndo);
  }
||||||| merged common ancestors
    // Look for specialized app-command keys
    switch (aEvent->keyval) {
        case GDK_Back:
            return DispatchCommandEvent(nsGkAtoms::Back);
        case GDK_Forward:
            return DispatchCommandEvent(nsGkAtoms::Forward);
        case GDK_Refresh:
            return DispatchCommandEvent(nsGkAtoms::Reload);
        case GDK_Stop:
            return DispatchCommandEvent(nsGkAtoms::Stop);
        case GDK_Search:
            return DispatchCommandEvent(nsGkAtoms::Search);
        case GDK_Favorites:
            return DispatchCommandEvent(nsGkAtoms::Bookmarks);
        case GDK_HomePage:
            return DispatchCommandEvent(nsGkAtoms::Home);
        case GDK_Copy:
        case GDK_F16:  // F16, F20, F18, F14 are old keysyms for Copy Cut Paste Undo
            return DispatchContentCommandEvent(eContentCommandCopy);
        case GDK_Cut:
        case GDK_F20:
            return DispatchContentCommandEvent(eContentCommandCut);
        case GDK_Paste:
        case GDK_F18:
            return DispatchContentCommandEvent(eContentCommandPaste);
        case GDK_Redo:
            return DispatchContentCommandEvent(eContentCommandRedo);
        case GDK_Undo:
        case GDK_F14:
            return DispatchContentCommandEvent(eContentCommandUndo);
    }
=======
        // Older GTK doesn't support stop events, so we can't support fling
        wheelEvent.mScrollType = WidgetWheelEvent::SCROLL_ASYNCHRONOUSELY;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  WidgetKeyboardEvent keypressEvent(true, eKeyPress, this);
  KeymapWrapper::InitKeyEvent(keypressEvent, aEvent, false);
||||||| merged common ancestors
    WidgetKeyboardEvent keypressEvent(true, eKeyPress, this);
    KeymapWrapper::InitKeyEvent(keypressEvent, aEvent, false);
=======
      // TODO - use a more appropriate scrolling unit than lines.
      // Multiply event deltas by 3 to emulate legacy behaviour.
      wheelEvent.mDeltaX = aEvent->delta_x * 3;
      wheelEvent.mDeltaY = aEvent->delta_y * 3;
      wheelEvent.mIsNoLineOrPageDelta = true;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // before we dispatch a key, check if it's the context menu key.
  // If so, send a context menu key event instead.
  if (MaybeDispatchContextMenuEvent(aEvent)) {
    return TRUE;
  }
||||||| merged common ancestors
    // before we dispatch a key, check if it's the context menu key.
    // If so, send a context menu key event instead.
    if (MaybeDispatchContextMenuEvent(aEvent)) {
        return TRUE;
    }
=======
      break;
    }
#endif
    case GDK_SCROLL_UP:
      wheelEvent.mDeltaY = wheelEvent.mLineOrPageDeltaY = -3;
      break;
    case GDK_SCROLL_DOWN:
      wheelEvent.mDeltaY = wheelEvent.mLineOrPageDeltaY = 3;
      break;
    case GDK_SCROLL_LEFT:
      wheelEvent.mDeltaX = wheelEvent.mLineOrPageDeltaX = -1;
      break;
    case GDK_SCROLL_RIGHT:
      wheelEvent.mDeltaX = wheelEvent.mLineOrPageDeltaX = 1;
      break;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RefPtr<TextEventDispatcher> dispatcher = GetTextEventDispatcher();
  nsresult rv = dispatcher->BeginNativeInputTransaction();
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return TRUE;
  }
||||||| merged common ancestors
    RefPtr<TextEventDispatcher> dispatcher = GetTextEventDispatcher();
    nsresult rv = dispatcher->BeginNativeInputTransaction();
    if (NS_WARN_IF(NS_FAILED(rv))) {
        return TRUE;
    }
=======
  wheelEvent.mRefPoint = GetRefPoint(this, aEvent);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If the character code is in the BMP, send the key press event.
  // Otherwise, send a compositionchange event with the equivalent UTF-16
  // string.
  // TODO: Investigate other browser's behavior in this case because
  //       this hack is odd for UI Events.
  nsEventStatus status = nsEventStatus_eIgnore;
  if (keypressEvent.mKeyNameIndex != KEY_NAME_INDEX_USE_STRING ||
      keypressEvent.mKeyValue.Length() == 1) {
    dispatcher->MaybeDispatchKeypressEvents(keypressEvent, status, aEvent);
  } else {
    WidgetEventTime eventTime = GetWidgetEventTime(aEvent->time);
    dispatcher->CommitComposition(status, &keypressEvent.mKeyValue, &eventTime);
  }
||||||| merged common ancestors
    // If the character code is in the BMP, send the key press event.
    // Otherwise, send a compositionchange event with the equivalent UTF-16
    // string.
    // TODO: Investigate other browser's behavior in this case because
    //       this hack is odd for UI Events.
    nsEventStatus status = nsEventStatus_eIgnore;
    if (keypressEvent.mKeyNameIndex != KEY_NAME_INDEX_USE_STRING ||
        keypressEvent.mKeyValue.Length() == 1) {
        dispatcher->MaybeDispatchKeypressEvents(keypressEvent,
                                                status, aEvent);
    } else {
        WidgetEventTime eventTime = GetWidgetEventTime(aEvent->time);
        dispatcher->CommitComposition(status, &keypressEvent.mKeyValue,
                                      &eventTime);
    }
=======
  KeymapWrapper::InitInputEvent(wheelEvent, aEvent->state);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return TRUE;
||||||| merged common ancestors
    return TRUE;
=======
  wheelEvent.AssignEventTime(GetWidgetEventTime(aEvent->time));

  DispatchInputEvent(&wheelEvent);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool nsWindow::MaybeDispatchContextMenuEvent(const GdkEventKey *aEvent) {
  KeyNameIndex keyNameIndex = KeymapWrapper::ComputeDOMKeyNameIndex(aEvent);
||||||| merged common ancestors
bool
nsWindow::MaybeDispatchContextMenuEvent(const GdkEventKey* aEvent)
{
    KeyNameIndex keyNameIndex = KeymapWrapper::ComputeDOMKeyNameIndex(aEvent);
=======
void nsWindow::OnVisibilityNotifyEvent(GdkEventVisibility* aEvent) {
  LOGDRAW(("Visibility event %i on [%p] %p\n", aEvent->state, this,
           aEvent->window));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Shift+F10 and ContextMenu should cause eContextMenu event.
  if (keyNameIndex != KEY_NAME_INDEX_F10 &&
      keyNameIndex != KEY_NAME_INDEX_ContextMenu) {
    return false;
  }

  WidgetMouseEvent contextMenuEvent(true, eContextMenu, this,
                                    WidgetMouseEvent::eReal,
                                    WidgetMouseEvent::eContextMenuKey);

  contextMenuEvent.mRefPoint = LayoutDeviceIntPoint(0, 0);
  contextMenuEvent.AssignEventTime(GetWidgetEventTime(aEvent->time));
  contextMenuEvent.mClickCount = 1;
  KeymapWrapper::InitInputEvent(contextMenuEvent, aEvent->state);

  if (contextMenuEvent.IsControl() || contextMenuEvent.IsMeta() ||
      contextMenuEvent.IsAlt()) {
    return false;
  }

  // If the key is ContextMenu, then an eContextMenu mouse event is
  // dispatched regardless of the state of the Shift modifier.  When it is
  // pressed without the Shift modifier, a web page can prevent the default
  // context menu action.  When pressed with the Shift modifier, the web page
  // cannot prevent the default context menu action.
  // (PresShell::HandleEventInternal() sets mOnlyChromeDispatch to true.)

  // If the key is F10, it needs Shift state because Shift+F10 is well-known
  // shortcut key on Linux.  However, eContextMenu with Shift state is
  // special.  It won't fire "contextmenu" event in the web content for
  // blocking web page to prevent its default.  Therefore, this combination
  // should work same as ContextMenu key.
  // XXX Should we allow to block web page to prevent its default with
  //     Ctrl+Shift+F10 or Alt+Shift+F10 instead?
  if (keyNameIndex == KEY_NAME_INDEX_F10) {
    if (!contextMenuEvent.IsShift()) {
      return false;
    }
    contextMenuEvent.mModifiers &= ~MODIFIER_SHIFT;
  }

  DispatchInputEvent(&contextMenuEvent);
  return true;
}

gboolean nsWindow::OnKeyReleaseEvent(GdkEventKey *aEvent) {
  LOGFOCUS(("OnKeyReleaseEvent [%p]\n", (void *)this));

  if (mIMContext) {
    KeyHandlingState handlingState = mIMContext->OnKeyEvent(this, aEvent);
    if (handlingState != KeyHandlingState::eNotHandled) {
      return TRUE;
    }
  }

  bool isCancelled = false;
  if (NS_WARN_IF(!DispatchKeyDownOrKeyUpEvent(aEvent, false, &isCancelled))) {
    return FALSE;
  }

  return TRUE;
}

void nsWindow::OnScrollEvent(GdkEventScroll *aEvent) {
  // check to see if we should rollup
  if (CheckForRollup(aEvent->x_root, aEvent->y_root, true, false)) return;
#if GTK_CHECK_VERSION(3, 4, 0)
  // check for duplicate legacy scroll event, see GNOME bug 726878
  if (aEvent->direction != GDK_SCROLL_SMOOTH &&
      mLastScrollEventTime == aEvent->time)
    return;
#endif
  WidgetWheelEvent wheelEvent(true, eWheel, this);
  wheelEvent.mDeltaMode = dom::WheelEvent_Binding::DOM_DELTA_LINE;
  switch (aEvent->direction) {
#if GTK_CHECK_VERSION(3, 4, 0)
    case GDK_SCROLL_SMOOTH: {
      // As of GTK 3.4, all directional scroll events are provided by
      // the GDK_SCROLL_SMOOTH direction on XInput2 devices.
      mLastScrollEventTime = aEvent->time;
      // TODO - use a more appropriate scrolling unit than lines.
      // Multiply event deltas by 3 to emulate legacy behaviour.
      wheelEvent.mDeltaX = aEvent->delta_x * 3;
      wheelEvent.mDeltaY = aEvent->delta_y * 3;
      wheelEvent.mIsNoLineOrPageDelta = true;
      // This next step manually unsets smooth scrolling for touch devices
      // that trigger GDK_SCROLL_SMOOTH. We use the slave device, which
      // represents the actual input.
      GdkDevice *device = gdk_event_get_source_device((GdkEvent *)aEvent);
      GdkInputSource source = gdk_device_get_source(device);
      if (source == GDK_SOURCE_TOUCHSCREEN || source == GDK_SOURCE_TOUCHPAD) {
        wheelEvent.mScrollType = WidgetWheelEvent::SCROLL_ASYNCHRONOUSELY;
      }
      break;
    }
#endif
    case GDK_SCROLL_UP:
      wheelEvent.mDeltaY = wheelEvent.mLineOrPageDeltaY = -3;
      break;
    case GDK_SCROLL_DOWN:
      wheelEvent.mDeltaY = wheelEvent.mLineOrPageDeltaY = 3;
      break;
    case GDK_SCROLL_LEFT:
      wheelEvent.mDeltaX = wheelEvent.mLineOrPageDeltaX = -1;
      break;
    case GDK_SCROLL_RIGHT:
      wheelEvent.mDeltaX = wheelEvent.mLineOrPageDeltaX = 1;
      break;
  }

  wheelEvent.mRefPoint = GetRefPoint(this, aEvent);

  KeymapWrapper::InitInputEvent(wheelEvent, aEvent->state);

  wheelEvent.AssignEventTime(GetWidgetEventTime(aEvent->time));

  DispatchInputEvent(&wheelEvent);
}

void nsWindow::OnVisibilityNotifyEvent(GdkEventVisibility *aEvent) {
  LOGDRAW(("Visibility event %i on [%p] %p\n", aEvent->state, this,
           aEvent->window));

  if (!mGdkWindow) return;
||||||| merged common ancestors
    // Shift+F10 and ContextMenu should cause eContextMenu event.
    if (keyNameIndex != KEY_NAME_INDEX_F10 &&
        keyNameIndex != KEY_NAME_INDEX_ContextMenu) {
      return false;
    }

    WidgetMouseEvent contextMenuEvent(true, eContextMenu, this,
                                      WidgetMouseEvent::eReal,
                                      WidgetMouseEvent::eContextMenuKey);

    contextMenuEvent.mRefPoint = LayoutDeviceIntPoint(0, 0);
    contextMenuEvent.AssignEventTime(GetWidgetEventTime(aEvent->time));
    contextMenuEvent.mClickCount = 1;
    KeymapWrapper::InitInputEvent(contextMenuEvent, aEvent->state);

    if (contextMenuEvent.IsControl() || contextMenuEvent.IsMeta() ||
        contextMenuEvent.IsAlt()) {
        return false;
    }

    // If the key is ContextMenu, then an eContextMenu mouse event is
    // dispatched regardless of the state of the Shift modifier.  When it is
    // pressed without the Shift modifier, a web page can prevent the default
    // context menu action.  When pressed with the Shift modifier, the web page
    // cannot prevent the default context menu action.
    // (PresShell::HandleEventInternal() sets mOnlyChromeDispatch to true.)

    // If the key is F10, it needs Shift state because Shift+F10 is well-known
    // shortcut key on Linux.  However, eContextMenu with Shift state is
    // special.  It won't fire "contextmenu" event in the web content for
    // blocking web page to prevent its default.  Therefore, this combination
    // should work same as ContextMenu key.
    // XXX Should we allow to block web page to prevent its default with
    //     Ctrl+Shift+F10 or Alt+Shift+F10 instead?
    if (keyNameIndex == KEY_NAME_INDEX_F10) {
        if (!contextMenuEvent.IsShift()) {
            return false;
        }
        contextMenuEvent.mModifiers &= ~MODIFIER_SHIFT;
    }

    DispatchInputEvent(&contextMenuEvent);
    return true;
}

gboolean
nsWindow::OnKeyReleaseEvent(GdkEventKey* aEvent)
{
    LOGFOCUS(("OnKeyReleaseEvent [%p]\n", (void *)this));

    if (mIMContext && mIMContext->OnKeyEvent(this, aEvent)) {
        return TRUE;
    }

    bool isCancelled = false;
    if (NS_WARN_IF(!DispatchKeyDownOrKeyUpEvent(aEvent, false, &isCancelled))) {
        return FALSE;
    }

    return TRUE;
}

void
nsWindow::OnScrollEvent(GdkEventScroll *aEvent)
{
    // check to see if we should rollup
    if (CheckForRollup(aEvent->x_root, aEvent->y_root, true, false))
        return;
#if GTK_CHECK_VERSION(3,4,0)
    // check for duplicate legacy scroll event, see GNOME bug 726878
    if (aEvent->direction != GDK_SCROLL_SMOOTH &&
        mLastScrollEventTime == aEvent->time)
        return;
#endif
    WidgetWheelEvent wheelEvent(true, eWheel, this);
    wheelEvent.mDeltaMode = dom::WheelEvent_Binding::DOM_DELTA_LINE;
    switch (aEvent->direction) {
#if GTK_CHECK_VERSION(3,4,0)
    case GDK_SCROLL_SMOOTH:
    {
        // As of GTK 3.4, all directional scroll events are provided by
        // the GDK_SCROLL_SMOOTH direction on XInput2 devices.
        mLastScrollEventTime = aEvent->time;
        // TODO - use a more appropriate scrolling unit than lines.
        // Multiply event deltas by 3 to emulate legacy behaviour.
        wheelEvent.mDeltaX = aEvent->delta_x * 3;
        wheelEvent.mDeltaY = aEvent->delta_y * 3;
        wheelEvent.mIsNoLineOrPageDelta = true;
        // This next step manually unsets smooth scrolling for touch devices
        // that trigger GDK_SCROLL_SMOOTH. We use the slave device, which
        // represents the actual input.
        GdkDevice *device = gdk_event_get_source_device((GdkEvent*)aEvent);
        GdkInputSource source = gdk_device_get_source(device);
        if (source == GDK_SOURCE_TOUCHSCREEN ||
            source == GDK_SOURCE_TOUCHPAD) {
            wheelEvent.mScrollType = WidgetWheelEvent::SCROLL_ASYNCHRONOUSELY;
        }
        break;
    }
#endif
    case GDK_SCROLL_UP:
        wheelEvent.mDeltaY = wheelEvent.mLineOrPageDeltaY = -3;
        break;
    case GDK_SCROLL_DOWN:
        wheelEvent.mDeltaY = wheelEvent.mLineOrPageDeltaY = 3;
        break;
    case GDK_SCROLL_LEFT:
        wheelEvent.mDeltaX = wheelEvent.mLineOrPageDeltaX = -1;
        break;
    case GDK_SCROLL_RIGHT:
        wheelEvent.mDeltaX = wheelEvent.mLineOrPageDeltaX = 1;
        break;
    }

    wheelEvent.mRefPoint = GetRefPoint(this, aEvent);

    KeymapWrapper::InitInputEvent(wheelEvent, aEvent->state);

    wheelEvent.AssignEventTime(GetWidgetEventTime(aEvent->time));

    DispatchInputEvent(&wheelEvent);
}

void
nsWindow::OnVisibilityNotifyEvent(GdkEventVisibility *aEvent)
{
    LOGDRAW(("Visibility event %i on [%p] %p\n",
             aEvent->state, this, aEvent->window));

    if (!mGdkWindow)
        return;
=======
  if (!mGdkWindow) return;
>>>>>>> upstream-releases

  switch (aEvent->state) {
    case GDK_VISIBILITY_UNOBSCURED:
    case GDK_VISIBILITY_PARTIAL:
      if (mIsFullyObscured && mHasMappedToplevel) {
        // GDK_EXPOSE events have been ignored, so make sure GDK
        // doesn't think that the window has already been painted.
        gdk_window_invalidate_rect(mGdkWindow, nullptr, FALSE);
      }

      mIsFullyObscured = false;

      // if we have to retry the grab, retry it.
      EnsureGrabs();
      break;
    default:  // includes GDK_VISIBILITY_FULLY_OBSCURED
      mIsFullyObscured = true;
      break;
  }
}

<<<<<<< HEAD
void nsWindow::OnWindowStateEvent(GtkWidget *aWidget,
                                  GdkEventWindowState *aEvent) {
  LOG(("nsWindow::OnWindowStateEvent [%p] changed %d new_window_state %d\n",
       (void *)this, aEvent->changed_mask, aEvent->new_window_state));
||||||| merged common ancestors
void
nsWindow::OnWindowStateEvent(GtkWidget *aWidget, GdkEventWindowState *aEvent)
{
    LOG(("nsWindow::OnWindowStateEvent [%p] changed %d new_window_state %d\n",
         (void *)this, aEvent->changed_mask, aEvent->new_window_state));

    if (IS_MOZ_CONTAINER(aWidget)) {
        // This event is notifying the container widget of changes to the
        // toplevel window.  Just detect changes affecting whether windows are
        // viewable.
        //
        // (A visibility notify event is sent to each window that becomes
        // viewable when the toplevel is mapped, but we can't rely on that for
        // setting mHasMappedToplevel because these toplevel window state
        // events are asynchronous.  The windows in the hierarchy now may not
        // be the same windows as when the toplevel was mapped, so they may
        // not get VisibilityNotify events.)
        bool mapped =
            !(aEvent->new_window_state &
              (GDK_WINDOW_STATE_ICONIFIED|GDK_WINDOW_STATE_WITHDRAWN));
        if (mHasMappedToplevel != mapped) {
            SetHasMappedToplevel(mapped);
        }
        return;
    }
    // else the widget is a shell widget.
=======
void nsWindow::OnWindowStateEvent(GtkWidget* aWidget,
                                  GdkEventWindowState* aEvent) {
  LOG(("nsWindow::OnWindowStateEvent [%p] changed %d new_window_state %d\n",
       (void*)this, aEvent->changed_mask, aEvent->new_window_state));
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (IS_MOZ_CONTAINER(aWidget)) {
    // This event is notifying the container widget of changes to the
    // toplevel window.  Just detect changes affecting whether windows are
    // viewable.
    //
    // (A visibility notify event is sent to each window that becomes
    // viewable when the toplevel is mapped, but we can't rely on that for
    // setting mHasMappedToplevel because these toplevel window state
    // events are asynchronous.  The windows in the hierarchy now may not
    // be the same windows as when the toplevel was mapped, so they may
    // not get VisibilityNotify events.)
    bool mapped = !(aEvent->new_window_state &
                    (GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_WITHDRAWN));
    if (mHasMappedToplevel != mapped) {
      SetHasMappedToplevel(mapped);
||||||| merged common ancestors
    // The block below is a bit evil.
    //
    // When a window is resized before it is shown, gtk_window_resize() delays
    // resizes until the window is shown.  If gtk_window_state_event() sees a
    // GDK_WINDOW_STATE_MAXIMIZED change [1] before the window is shown, then
    // gtk_window_compute_configure_request_size() ignores the values from the
    // resize [2].  See bug 1449166 for an example of how this could happen.
    //
    // [1] https://gitlab.gnome.org/GNOME/gtk/blob/3.22.30/gtk/gtkwindow.c#L7967
    // [2] https://gitlab.gnome.org/GNOME/gtk/blob/3.22.30/gtk/gtkwindow.c#L9377
    //
    // In order to provide a sensible size for the window when the user exits
    // maximized state, we hide the GDK_WINDOW_STATE_MAXIMIZED change from
    // gtk_window_state_event() so as to trick GTK into using the values from
    // gtk_window_resize() in its configure request.
    //
    // We instead notify gtk_window_state_event() of the maximized state change
    // once the window is shown.
    if (!mIsShown) {
        aEvent->changed_mask = static_cast<GdkWindowState>
            (aEvent->changed_mask & ~GDK_WINDOW_STATE_MAXIMIZED);
    } else if (aEvent->changed_mask & GDK_WINDOW_STATE_WITHDRAWN &&
               aEvent->new_window_state & GDK_WINDOW_STATE_MAXIMIZED) {
        aEvent->changed_mask = static_cast<GdkWindowState>
            (aEvent->changed_mask | GDK_WINDOW_STATE_MAXIMIZED);
=======
  if (IS_MOZ_CONTAINER(aWidget)) {
    // This event is notifying the container widget of changes to the
    // toplevel window.  Just detect changes affecting whether windows are
    // viewable.
    //
    // (A visibility notify event is sent to each window that becomes
    // viewable when the toplevel is mapped, but we can't rely on that for
    // setting mHasMappedToplevel because these toplevel window state
    // events are asynchronous.  The windows in the hierarchy now may not
    // be the same windows as when the toplevel was mapped, so they may
    // not get VisibilityNotify events.)
    bool mapped = !(aEvent->new_window_state &
                    (GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_WITHDRAWN));
    if (mHasMappedToplevel != mapped) {
      SetHasMappedToplevel(mapped);
>>>>>>> upstream-releases
    }
    return;
  }
  // else the widget is a shell widget.

  // The block below is a bit evil.
  //
  // When a window is resized before it is shown, gtk_window_resize() delays
  // resizes until the window is shown.  If gtk_window_state_event() sees a
  // GDK_WINDOW_STATE_MAXIMIZED change [1] before the window is shown, then
  // gtk_window_compute_configure_request_size() ignores the values from the
  // resize [2].  See bug 1449166 for an example of how this could happen.
  //
  // [1] https://gitlab.gnome.org/GNOME/gtk/blob/3.22.30/gtk/gtkwindow.c#L7967
  // [2] https://gitlab.gnome.org/GNOME/gtk/blob/3.22.30/gtk/gtkwindow.c#L9377
  //
  // In order to provide a sensible size for the window when the user exits
  // maximized state, we hide the GDK_WINDOW_STATE_MAXIMIZED change from
  // gtk_window_state_event() so as to trick GTK into using the values from
  // gtk_window_resize() in its configure request.
  //
  // We instead notify gtk_window_state_event() of the maximized state change
  // once the window is shown.
  if (!mIsShown) {
    aEvent->changed_mask = static_cast<GdkWindowState>(
        aEvent->changed_mask & ~GDK_WINDOW_STATE_MAXIMIZED);
  } else if (aEvent->changed_mask & GDK_WINDOW_STATE_WITHDRAWN &&
             aEvent->new_window_state & GDK_WINDOW_STATE_MAXIMIZED) {
    aEvent->changed_mask = static_cast<GdkWindowState>(
        aEvent->changed_mask | GDK_WINDOW_STATE_MAXIMIZED);
  }

<<<<<<< HEAD
  // This is a workaround for https://gitlab.gnome.org/GNOME/gtk/issues/1395
  // Gtk+ controls window active appearance by window-state-event signal.
  if (mDrawInTitlebar && (aEvent->changed_mask & GDK_WINDOW_STATE_FOCUSED)) {
    // Emulate what Gtk+ does at gtk_window_state_event().
    // We can't check GTK_STATE_FLAG_BACKDROP directly as it's set by Gtk+
    // *after* this window-state-event handler.
    mTitlebarBackdropState =
        !(aEvent->new_window_state & GDK_WINDOW_STATE_FOCUSED);

    ForceTitlebarRedraw();
    return;
  }

  // We don't care about anything but changes in the maximized/icon/fullscreen
  // states
  if ((aEvent->changed_mask &
       (GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_MAXIMIZED |
        GDK_WINDOW_STATE_FULLSCREEN)) == 0) {
    return;
  }
||||||| merged common ancestors
    // We don't care about anything but changes in the maximized/icon/fullscreen
    // states
    if ((aEvent->changed_mask
         & (GDK_WINDOW_STATE_ICONIFIED |
            GDK_WINDOW_STATE_MAXIMIZED |
            GDK_WINDOW_STATE_FULLSCREEN)) == 0) {
        return;
    }
=======
  // This is a workaround for https://gitlab.gnome.org/GNOME/gtk/issues/1395
  // Gtk+ controls window active appearance by window-state-event signal.
  if (mDrawInTitlebar && (aEvent->changed_mask & GDK_WINDOW_STATE_FOCUSED)) {
    // Emulate what Gtk+ does at gtk_window_state_event().
    // We can't check GTK_STATE_FLAG_BACKDROP directly as it's set by Gtk+
    // *after* this window-state-event handler.
    mTitlebarBackdropState =
        !(aEvent->new_window_state & GDK_WINDOW_STATE_FOCUSED);

    ForceTitlebarRedraw();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (aEvent->new_window_state & GDK_WINDOW_STATE_ICONIFIED) {
    LOG(("\tIconified\n"));
    mSizeState = nsSizeMode_Minimized;
||||||| merged common ancestors
    if (aEvent->new_window_state & GDK_WINDOW_STATE_ICONIFIED) {
        LOG(("\tIconified\n"));
        mSizeState = nsSizeMode_Minimized;
=======
  // We don't care about anything but changes in the maximized/icon/fullscreen
  // states but we need a workaround for bug in Wayland:
  // https://gitlab.gnome.org/GNOME/gtk/issues/67
  // Under wayland the gtk_window_iconify implementation does NOT synthetize
  // window_state_event where the GDK_WINDOW_STATE_ICONIFIED is set.
  // During restore we  won't get aEvent->changed_mask with
  // the GDK_WINDOW_STATE_ICONIFIED so to detect that change we use the stored
  // mSizeState and obtaining a focus.
  bool waylandWasIconified =
      (!mIsX11Display && aEvent->changed_mask & GDK_WINDOW_STATE_FOCUSED &&
       aEvent->new_window_state & GDK_WINDOW_STATE_FOCUSED &&
       mSizeState == nsSizeMode_Minimized);
  if (!waylandWasIconified &&
      (aEvent->changed_mask &
       (GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_MAXIMIZED |
        GDK_WINDOW_STATE_FULLSCREEN)) == 0) {
    return;
  }

  if (aEvent->new_window_state & GDK_WINDOW_STATE_ICONIFIED) {
    LOG(("\tIconified\n"));
    mSizeState = nsSizeMode_Minimized;
>>>>>>> upstream-releases
#ifdef ACCESSIBILITY
    DispatchMinimizeEventAccessible();
#endif  // ACCESSIBILITY
  } else if (aEvent->new_window_state & GDK_WINDOW_STATE_FULLSCREEN) {
    LOG(("\tFullscreen\n"));
    mSizeState = nsSizeMode_Fullscreen;
  } else if (aEvent->new_window_state & GDK_WINDOW_STATE_MAXIMIZED) {
    LOG(("\tMaximized\n"));
    mSizeState = nsSizeMode_Maximized;
#ifdef ACCESSIBILITY
    DispatchMaximizeEventAccessible();
#endif  // ACCESSIBILITY
  } else {
    LOG(("\tNormal\n"));
    mSizeState = nsSizeMode_Normal;
#ifdef ACCESSIBILITY
    DispatchRestoreEventAccessible();
#endif  // ACCESSIBILITY
  }

  if (mWidgetListener) {
    mWidgetListener->SizeModeChanged(mSizeState);
    if (aEvent->changed_mask & GDK_WINDOW_STATE_FULLSCREEN) {
      mWidgetListener->FullscreenChanged(aEvent->new_window_state &
                                         GDK_WINDOW_STATE_FULLSCREEN);
    }
  }

  if (mDrawInTitlebar && mCSDSupportLevel == CSD_SUPPORT_CLIENT) {
    UpdateClientOffsetForCSDWindow();
  }
}

<<<<<<< HEAD
void nsWindow::ThemeChanged() {
  NotifyThemeChanged();
||||||| merged common ancestors
void
nsWindow::ThemeChanged()
{
    NotifyThemeChanged();

    if (!mGdkWindow || MOZ_UNLIKELY(mIsDestroyed))
        return;
=======
void nsWindow::ThemeChanged() {
  NotifyThemeChanged();

  if (!mGdkWindow || MOZ_UNLIKELY(mIsDestroyed)) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!mGdkWindow || MOZ_UNLIKELY(mIsDestroyed)) return;
||||||| merged common ancestors
    // Dispatch theme change notification to all child windows
    GList *children =
        gdk_window_peek_children(mGdkWindow);
    while (children) {
        GdkWindow *gdkWin = GDK_WINDOW(children->data);
=======
  // Dispatch theme change notification to all child windows
  GList* children = gdk_window_peek_children(mGdkWindow);
  while (children) {
    GdkWindow* gdkWin = GDK_WINDOW(children->data);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Dispatch theme change notification to all child windows
  GList *children = gdk_window_peek_children(mGdkWindow);
  while (children) {
    GdkWindow *gdkWin = GDK_WINDOW(children->data);
||||||| merged common ancestors
        auto *win = (nsWindow*) g_object_get_data(G_OBJECT(gdkWin),
                                                      "nsWindow");
=======
    auto* win = (nsWindow*)g_object_get_data(G_OBJECT(gdkWin), "nsWindow");
>>>>>>> upstream-releases

<<<<<<< HEAD
    auto *win = (nsWindow *)g_object_get_data(G_OBJECT(gdkWin), "nsWindow");

    if (win && win != this) {  // guard against infinite recursion
      RefPtr<nsWindow> kungFuDeathGrip = win;
      win->ThemeChanged();
||||||| merged common ancestors
        if (win && win != this) { // guard against infinite recursion
            RefPtr<nsWindow> kungFuDeathGrip = win;
            win->ThemeChanged();
        }

        children = children->next;
=======
    if (win && win != this) {  // guard against infinite recursion
      RefPtr<nsWindow> kungFuDeathGrip = win;
      win->ThemeChanged();
>>>>>>> upstream-releases
    }

    children = children->next;
  }

  IMContextWrapper::OnThemeChanged();
}

void nsWindow::OnDPIChanged() {
  if (mWidgetListener) {
<<<<<<< HEAD
    nsIPresShell *presShell = mWidgetListener->GetPresShell();
    if (presShell) {
||||||| merged common ancestors
    nsIPresShell* presShell = mWidgetListener->GetPresShell();
    if (presShell) {
=======
    if (PresShell* presShell = mWidgetListener->GetPresShell()) {
>>>>>>> upstream-releases
      presShell->BackingScaleFactorChanged();
      // Update menu's font size etc
      presShell->ThemeChanged();
    }
    mWidgetListener->UIResolutionChanged();
  }
}

void nsWindow::OnCheckResize() { mPendingConfigures++; }

void nsWindow::OnCompositedChanged() {
  if (mWidgetListener) {
<<<<<<< HEAD
    nsIPresShell *presShell = mWidgetListener->GetPresShell();
    if (presShell) {
||||||| merged common ancestors
    nsIPresShell* presShell = mWidgetListener->GetPresShell();
    if (presShell) {
=======
    if (PresShell* presShell = mWidgetListener->GetPresShell()) {
>>>>>>> upstream-releases
      // Update CSD after the change in alpha visibility
      presShell->ThemeChanged();
    }
  }
}

<<<<<<< HEAD
void nsWindow::DispatchDragEvent(EventMessage aMsg,
                                 const LayoutDeviceIntPoint &aRefPoint,
                                 guint aTime) {
  WidgetDragEvent event(true, aMsg, this);
||||||| merged common ancestors
void
nsWindow::DispatchDragEvent(EventMessage aMsg, const LayoutDeviceIntPoint& aRefPoint,
                            guint aTime)
{
    WidgetDragEvent event(true, aMsg, this);
=======
void nsWindow::OnScaleChanged(GtkAllocation* aAllocation) {
#ifdef MOZ_WAYLAND
  if (mContainer && moz_container_has_wl_egl_window(mContainer)) {
    // We need to resize wl_egl_window when scale changes.
    moz_container_scale_changed(mContainer, aAllocation);
  }
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  InitDragEvent(event);
||||||| merged common ancestors
    InitDragEvent(event);
=======
  // This eventually propagate new scale to the PuppetWidgets
  OnDPIChanged();
>>>>>>> upstream-releases

<<<<<<< HEAD
  event.mRefPoint = aRefPoint;
  event.AssignEventTime(GetWidgetEventTime(aTime));
||||||| merged common ancestors
    event.mRefPoint = aRefPoint;
    event.AssignEventTime(GetWidgetEventTime(aTime));
=======
  // configure_event is already fired before scale-factor signal,
  // but size-allocate isn't fired by changing scale
  OnSizeAllocate(aAllocation);
}

void nsWindow::DispatchDragEvent(EventMessage aMsg,
                                 const LayoutDeviceIntPoint& aRefPoint,
                                 guint aTime) {
  WidgetDragEvent event(true, aMsg, this);

  InitDragEvent(event);
>>>>>>> upstream-releases

<<<<<<< HEAD
  DispatchInputEvent(&event);
||||||| merged common ancestors
    DispatchInputEvent(&event);
=======
  event.mRefPoint = aRefPoint;
  event.AssignEventTime(GetWidgetEventTime(aTime));

  DispatchInputEvent(&event);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsWindow::OnDragDataReceivedEvent(GtkWidget *aWidget,
                                       GdkDragContext *aDragContext, gint aX,
                                       gint aY,
                                       GtkSelectionData *aSelectionData,
                                       guint aInfo, guint aTime,
                                       gpointer aData) {
  LOGDRAG(("nsWindow::OnDragDataReceived(%p)\n", (void *)this));
||||||| merged common ancestors
void
nsWindow::OnDragDataReceivedEvent(GtkWidget *aWidget,
                                  GdkDragContext *aDragContext,
                                  gint aX,
                                  gint aY,
                                  GtkSelectionData  *aSelectionData,
                                  guint aInfo,
                                  guint aTime,
                                  gpointer aData)
{
    LOGDRAG(("nsWindow::OnDragDataReceived(%p)\n", (void*)this));
=======
void nsWindow::OnDragDataReceivedEvent(GtkWidget* aWidget,
                                       GdkDragContext* aDragContext, gint aX,
                                       gint aY,
                                       GtkSelectionData* aSelectionData,
                                       guint aInfo, guint aTime,
                                       gpointer aData) {
  LOGDRAG(("nsWindow::OnDragDataReceived(%p)\n", (void*)this));
>>>>>>> upstream-releases

  RefPtr<nsDragService> dragService = nsDragService::GetInstance();
  dragService->TargetDataReceived(aWidget, aDragContext, aX, aY, aSelectionData,
                                  aInfo, aTime);
}

<<<<<<< HEAD
nsWindow *nsWindow::GetTransientForWindowIfPopup() {
  if (mWindowType != eWindowType_popup) {
||||||| merged common ancestors
nsWindow*
nsWindow::GetTransientForWindowIfPopup()
{
    if (mWindowType != eWindowType_popup) {
        return nullptr;
    }
    GtkWindow* toplevel = gtk_window_get_transient_for(GTK_WINDOW(mShell));
    if (toplevel) {
        return get_window_for_gtk_widget(GTK_WIDGET(toplevel));
    }
=======
nsWindow* nsWindow::GetTransientForWindowIfPopup() {
  if (mWindowType != eWindowType_popup) {
>>>>>>> upstream-releases
    return nullptr;
<<<<<<< HEAD
  }
  GtkWindow *toplevel = gtk_window_get_transient_for(GTK_WINDOW(mShell));
  if (toplevel) {
    return get_window_for_gtk_widget(GTK_WIDGET(toplevel));
  }
  return nullptr;
||||||| merged common ancestors
=======
  }
  GtkWindow* toplevel = gtk_window_get_transient_for(GTK_WINDOW(mShell));
  if (toplevel) {
    return get_window_for_gtk_widget(GTK_WIDGET(toplevel));
  }
  return nullptr;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool nsWindow::IsHandlingTouchSequence(GdkEventSequence *aSequence) {
  return mHandleTouchEvent && mTouches.Contains(aSequence);
||||||| merged common ancestors
bool
nsWindow::IsHandlingTouchSequence(GdkEventSequence* aSequence)
{
    return mHandleTouchEvent && mTouches.Contains(aSequence);
=======
bool nsWindow::IsHandlingTouchSequence(GdkEventSequence* aSequence) {
  return mHandleTouchEvent && mTouches.Contains(aSequence);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
#if GTK_CHECK_VERSION(3, 4, 0)
gboolean nsWindow::OnTouchEvent(GdkEventTouch *aEvent) {
  if (!mHandleTouchEvent) {
    // If a popup window was spawned (e.g. as the result of a long-press)
    // and touch events got diverted to that window within a touch sequence,
    // ensure the touch event gets sent to the original window instead. We
    // keep the checks here very conservative so that we only redirect
    // events in this specific scenario.
    nsWindow *targetWindow = GetTransientForWindowIfPopup();
    if (targetWindow &&
        targetWindow->IsHandlingTouchSequence(aEvent->sequence)) {
      return targetWindow->OnTouchEvent(aEvent);
||||||| merged common ancestors
#if GTK_CHECK_VERSION(3,4,0)
gboolean
nsWindow::OnTouchEvent(GdkEventTouch* aEvent)
{
    if (!mHandleTouchEvent) {
        // If a popup window was spawned (e.g. as the result of a long-press)
        // and touch events got diverted to that window within a touch sequence,
        // ensure the touch event gets sent to the original window instead. We
        // keep the checks here very conservative so that we only redirect
        // events in this specific scenario.
        nsWindow* targetWindow = GetTransientForWindowIfPopup();
        if (targetWindow &&
            targetWindow->IsHandlingTouchSequence(aEvent->sequence)) {
            return targetWindow->OnTouchEvent(aEvent);
        }

        return FALSE;
=======
#if GTK_CHECK_VERSION(3, 4, 0)
gboolean nsWindow::OnTouchEvent(GdkEventTouch* aEvent) {
  if (!mHandleTouchEvent) {
    // If a popup window was spawned (e.g. as the result of a long-press)
    // and touch events got diverted to that window within a touch sequence,
    // ensure the touch event gets sent to the original window instead. We
    // keep the checks here very conservative so that we only redirect
    // events in this specific scenario.
    nsWindow* targetWindow = GetTransientForWindowIfPopup();
    if (targetWindow &&
        targetWindow->IsHandlingTouchSequence(aEvent->sequence)) {
      return targetWindow->OnTouchEvent(aEvent);
>>>>>>> upstream-releases
    }

    return FALSE;
  }

  EventMessage msg;
  switch (aEvent->type) {
    case GDK_TOUCH_BEGIN:
      msg = eTouchStart;
      break;
    case GDK_TOUCH_UPDATE:
      msg = eTouchMove;
      break;
    case GDK_TOUCH_END:
      msg = eTouchEnd;
      break;
    case GDK_TOUCH_CANCEL:
      msg = eTouchCancel;
      break;
    default:
      return FALSE;
  }

  LayoutDeviceIntPoint touchPoint = GetRefPoint(this, aEvent);

  int32_t id;
  RefPtr<dom::Touch> touch;
  if (mTouches.Remove(aEvent->sequence, getter_AddRefs(touch))) {
    id = touch->mIdentifier;
  } else {
    id = ++gLastTouchID & 0x7FFFFFFF;
  }

  touch =
      new dom::Touch(id, touchPoint, LayoutDeviceIntPoint(1, 1), 0.0f, 0.0f);

  WidgetTouchEvent event(true, msg, this);
  KeymapWrapper::InitInputEvent(event, aEvent->state);
  event.mTime = aEvent->time;

  if (aEvent->type == GDK_TOUCH_BEGIN || aEvent->type == GDK_TOUCH_UPDATE) {
    mTouches.Put(aEvent->sequence, touch.forget());
    // add all touch points to event object
    for (auto iter = mTouches.Iter(); !iter.Done(); iter.Next()) {
      event.mTouches.AppendElement(new dom::Touch(*iter.UserData()));
    }
  } else if (aEvent->type == GDK_TOUCH_END ||
             aEvent->type == GDK_TOUCH_CANCEL) {
    *event.mTouches.AppendElement() = touch.forget();
  }

  DispatchInputEvent(&event);
  return TRUE;
}
#endif

<<<<<<< HEAD
static GdkWindow *CreateGdkWindow(GdkWindow *parent, GtkWidget *widget) {
  GdkWindowAttr attributes;
  gint attributes_mask = GDK_WA_VISUAL;
||||||| merged common ancestors
static GdkWindow *
CreateGdkWindow(GdkWindow *parent, GtkWidget *widget)
{
    GdkWindowAttr attributes;
    gint          attributes_mask = GDK_WA_VISUAL;
=======
static GdkWindow* CreateGdkWindow(GdkWindow* parent, GtkWidget* widget) {
  GdkWindowAttr attributes;
  gint attributes_mask = GDK_WA_VISUAL;
>>>>>>> upstream-releases

  attributes.event_mask = kEvents;

  attributes.width = 1;
  attributes.height = 1;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.visual = gtk_widget_get_visual(widget);
  attributes.window_type = GDK_WINDOW_CHILD;

<<<<<<< HEAD
  GdkWindow *window = gdk_window_new(parent, &attributes, attributes_mask);
  gdk_window_set_user_data(window, widget);
||||||| merged common ancestors
    GdkWindow *window = gdk_window_new(parent, &attributes, attributes_mask);
    gdk_window_set_user_data(window, widget);
=======
  GdkWindow* window = gdk_window_new(parent, &attributes, attributes_mask);
  gdk_window_set_user_data(window, widget);
>>>>>>> upstream-releases

  return window;
}

<<<<<<< HEAD
nsresult nsWindow::Create(nsIWidget *aParent, nsNativeWidget aNativeParent,
                          const LayoutDeviceIntRect &aRect,
                          nsWidgetInitData *aInitData) {
  // only set the base parent if we're going to be a dialog or a
  // toplevel
  nsIWidget *baseParent =
      aInitData && (aInitData->mWindowType == eWindowType_dialog ||
                    aInitData->mWindowType == eWindowType_toplevel ||
                    aInitData->mWindowType == eWindowType_invisible)
          ? nullptr
          : aParent;
||||||| merged common ancestors
nsresult
nsWindow::Create(nsIWidget* aParent,
                 nsNativeWidget aNativeParent,
                 const LayoutDeviceIntRect& aRect,
                 nsWidgetInitData* aInitData)
{
    // only set the base parent if we're going to be a dialog or a
    // toplevel
    nsIWidget *baseParent = aInitData &&
        (aInitData->mWindowType == eWindowType_dialog ||
         aInitData->mWindowType == eWindowType_toplevel ||
         aInitData->mWindowType == eWindowType_invisible) ?
        nullptr : aParent;
=======
nsresult nsWindow::Create(nsIWidget* aParent, nsNativeWidget aNativeParent,
                          const LayoutDeviceIntRect& aRect,
                          nsWidgetInitData* aInitData) {
  // only set the base parent if we're going to be a dialog or a
  // toplevel
  nsIWidget* baseParent =
      aInitData && (aInitData->mWindowType == eWindowType_dialog ||
                    aInitData->mWindowType == eWindowType_toplevel ||
                    aInitData->mWindowType == eWindowType_invisible)
          ? nullptr
          : aParent;
>>>>>>> upstream-releases

#ifdef ACCESSIBILITY
  // Send a DBus message to check whether a11y is enabled
  a11y::PreInit();
#endif

<<<<<<< HEAD
  // Ensure that the toolkit is created.
  nsGTKToolkit::GetToolkit();

  // initialize all the common bits of this class
  BaseCreate(baseParent, aInitData);

  // Do we need to listen for resizes?
  bool listenForResizes = false;
  ;
  if (aNativeParent || (aInitData && aInitData->mListenForResizes))
    listenForResizes = true;

  // and do our common creation
  CommonCreate(aParent, listenForResizes);

  // save our bounds
  mBounds = aRect;
  ConstrainSize(&mBounds.width, &mBounds.height);

  // figure out our parent window
  GtkWidget *parentMozContainer = nullptr;
  GtkContainer *parentGtkContainer = nullptr;
  GdkWindow *parentGdkWindow = nullptr;
  GtkWindow *topLevelParent = nullptr;
  nsWindow *parentnsWindow = nullptr;
  GtkWidget *eventWidget = nullptr;
  bool drawToContainer = false;
  bool needsAlphaVisual =
      (mWindowType == eWindowType_popup && aInitData->mSupportTranslucency);

  if (aParent) {
    parentnsWindow = static_cast<nsWindow *>(aParent);
    parentGdkWindow = parentnsWindow->mGdkWindow;
  } else if (aNativeParent && GDK_IS_WINDOW(aNativeParent)) {
    parentGdkWindow = GDK_WINDOW(aNativeParent);
    parentnsWindow = get_window_for_gdk_window(parentGdkWindow);
    if (!parentnsWindow) return NS_ERROR_FAILURE;

  } else if (aNativeParent && GTK_IS_CONTAINER(aNativeParent)) {
    parentGtkContainer = GTK_CONTAINER(aNativeParent);
  }
||||||| merged common ancestors
    // Ensure that the toolkit is created.
    nsGTKToolkit::GetToolkit();

    // initialize all the common bits of this class
    BaseCreate(baseParent, aInitData);

    // Do we need to listen for resizes?
    bool listenForResizes = false;;
    if (aNativeParent || (aInitData && aInitData->mListenForResizes))
        listenForResizes = true;

    // and do our common creation
    CommonCreate(aParent, listenForResizes);

    // save our bounds
    mBounds = aRect;
    ConstrainSize(&mBounds.width, &mBounds.height);

    // figure out our parent window
    GtkWidget      *parentMozContainer = nullptr;
    GtkContainer   *parentGtkContainer = nullptr;
    GdkWindow      *parentGdkWindow = nullptr;
    GtkWindow      *topLevelParent = nullptr;
    nsWindow       *parentnsWindow = nullptr;
    GtkWidget      *eventWidget = nullptr;
    bool            drawToContainer = false;
    bool            needsAlphaVisual = (mWindowType == eWindowType_popup &&
                                       aInitData->mSupportTranslucency);

    // Some Gtk+ themes use non-rectangular toplevel windows. To fully support
    // such themes we need to make toplevel window transparent with ARGB visual.
    // It may cause performanance issue so make it configurable
    // and enable it by default for selected window managers.
    if (mWindowType == eWindowType_toplevel) {
        needsAlphaVisual = TopLevelWindowUseARGBVisual();
    }

    if (aParent) {
        parentnsWindow = static_cast<nsWindow*>(aParent);
        parentGdkWindow = parentnsWindow->mGdkWindow;
    } else if (aNativeParent && GDK_IS_WINDOW(aNativeParent)) {
        parentGdkWindow = GDK_WINDOW(aNativeParent);
        parentnsWindow = get_window_for_gdk_window(parentGdkWindow);
        if (!parentnsWindow)
            return NS_ERROR_FAILURE;

    } else if (aNativeParent && GTK_IS_CONTAINER(aNativeParent)) {
        parentGtkContainer = GTK_CONTAINER(aNativeParent);
    }
=======
  // Ensure that the toolkit is created.
  nsGTKToolkit::GetToolkit();

  // initialize all the common bits of this class
  BaseCreate(baseParent, aInitData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (parentGdkWindow) {
    // get the widget for the window - it should be a moz container
    parentMozContainer = parentnsWindow->GetMozContainerWidget();
    if (!parentMozContainer) return NS_ERROR_FAILURE;
||||||| merged common ancestors
    if (parentGdkWindow) {
        // get the widget for the window - it should be a moz container
        parentMozContainer = parentnsWindow->GetMozContainerWidget();
        if (!parentMozContainer)
            return NS_ERROR_FAILURE;
=======
  // Do we need to listen for resizes?
  bool listenForResizes = false;
  ;
  if (aNativeParent || (aInitData && aInitData->mListenForResizes))
    listenForResizes = true;
>>>>>>> upstream-releases

<<<<<<< HEAD
    // get the toplevel window just in case someone needs to use it
    // for setting transients or whatever.
    topLevelParent = GTK_WINDOW(gtk_widget_get_toplevel(parentMozContainer));
  }
||||||| merged common ancestors
        // get the toplevel window just in case someone needs to use it
        // for setting transients or whatever.
        topLevelParent =
            GTK_WINDOW(gtk_widget_get_toplevel(parentMozContainer));
    }
=======
  // and do our common creation
  CommonCreate(aParent, listenForResizes);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // ok, create our windows
  switch (mWindowType) {
||||||| merged common ancestors
    // ok, create our windows
    switch (mWindowType) {
=======
  // save our bounds
  mBounds = aRect;
  ConstrainSize(&mBounds.width, &mBounds.height);

  // eWindowType_child is not supported on Wayland. Just switch to toplevel
  // as a workaround.
  if (!mIsX11Display && mWindowType == eWindowType_child) {
    mWindowType = eWindowType_toplevel;
  }

  // figure out our parent window
  GtkWidget* parentMozContainer = nullptr;
  GtkContainer* parentGtkContainer = nullptr;
  GdkWindow* parentGdkWindow = nullptr;
  GtkWindow* topLevelParent = nullptr;
  nsWindow* parentnsWindow = nullptr;
  GtkWidget* eventWidget = nullptr;
  bool drawToContainer = false;
  bool needsAlphaVisual =
      (mWindowType == eWindowType_popup && aInitData->mSupportTranslucency);

  if (aParent) {
    parentnsWindow = static_cast<nsWindow*>(aParent);
    parentGdkWindow = parentnsWindow->mGdkWindow;
  } else if (aNativeParent && GDK_IS_WINDOW(aNativeParent)) {
    parentGdkWindow = GDK_WINDOW(aNativeParent);
    parentnsWindow = get_window_for_gdk_window(parentGdkWindow);
    if (!parentnsWindow) return NS_ERROR_FAILURE;

  } else if (aNativeParent && GTK_IS_CONTAINER(aNativeParent)) {
    parentGtkContainer = GTK_CONTAINER(aNativeParent);
  }

  if (parentGdkWindow) {
    // get the widget for the window - it should be a moz container
    parentMozContainer = parentnsWindow->GetMozContainerWidget();
    if (!parentMozContainer) return NS_ERROR_FAILURE;

    // get the toplevel window just in case someone needs to use it
    // for setting transients or whatever.
    topLevelParent = GTK_WINDOW(gtk_widget_get_toplevel(parentMozContainer));
  }

  // ok, create our windows
  switch (mWindowType) {
>>>>>>> upstream-releases
    case eWindowType_dialog:
    case eWindowType_popup:
    case eWindowType_toplevel:
    case eWindowType_invisible: {
<<<<<<< HEAD
      mIsTopLevel = true;

      // Popups that are not noautohide are only temporary. The are used
      // for menus and the like and disappear when another window is used.
      // For most popups, use the standard GtkWindowType GTK_WINDOW_POPUP,
      // which will use a Window with the override-redirect attribute
      // (for temporary windows).
      // For long-lived windows, their stacking order is managed by the
      // window manager, as indicated by GTK_WINDOW_TOPLEVEL.
      // For Wayland we have to always use GTK_WINDOW_POPUP to control
      // popup window position.
      GtkWindowType type = GTK_WINDOW_TOPLEVEL;
      if (mWindowType == eWindowType_popup) {
        type = (mIsX11Display && aInitData->mNoAutoHide) ? GTK_WINDOW_TOPLEVEL
                                                         : GTK_WINDOW_POPUP;
      }
      mShell = gtk_window_new(type);

      // Ensure gfxPlatform is initialized, since that is what initializes
      // gfxVars, used below.
      Unused << gfxPlatform::GetPlatform();

      bool useWebRender =
          gfx::gfxVars::UseWebRender() && AllowWebRenderForThisWindow();

      bool shouldAccelerate = ComputeShouldAccelerate();
      MOZ_ASSERT(shouldAccelerate | !useWebRender);

      // Some Gtk+ themes use non-rectangular toplevel windows. To fully support
      // such themes we need to make toplevel window transparent with ARGB visual.
      // It may cause performanance issue so make it configurable
      // and enable it by default for selected window managers.
      // Also disable it for X11 SW rendering (Bug 1516224) by default.
      if (mWindowType == eWindowType_toplevel &&
          (shouldAccelerate || !mIsX11Display ||
            Preferences::HasUserValue("mozilla.widget.use-argb-visuals"))) {
          needsAlphaVisual = TopLevelWindowUseARGBVisual();
      }

      bool isSetVisual = false;
      // If using WebRender on X11, we need to select a visual with a depth
      // buffer, as well as an alpha channel if transparency is requested. This
      // must be done before the widget is realized.

      // Use GL/WebRender compatible visual only when it is necessary, since
      // the visual consumes more memory.
      if (mIsX11Display && shouldAccelerate) {
        auto display = GDK_DISPLAY_XDISPLAY(gtk_widget_get_display(mShell));
        auto screen = gtk_widget_get_screen(mShell);
        int screenNumber = GDK_SCREEN_XNUMBER(screen);
        int visualId = 0;
        if (GLContextGLX::FindVisual(display, screenNumber, useWebRender,
                                     needsAlphaVisual, &visualId)) {
          // If we're using CSD, rendering will go through mContainer, but
          // it will inherit this visual as it is a child of mShell.
          gtk_widget_set_visual(mShell,
                                gdk_x11_screen_lookup_visual(screen, visualId));
          mHasAlphaVisual = needsAlphaVisual;
          isSetVisual = true;
        } else {
          NS_WARNING("We're missing X11 Visual!");
||||||| merged common ancestors
        mIsTopLevel = true;

        // Popups that are not noautohide are only temporary. The are used
        // for menus and the like and disappear when another window is used.
        // For most popups, use the standard GtkWindowType GTK_WINDOW_POPUP,
        // which will use a Window with the override-redirect attribute
        // (for temporary windows).
        // For long-lived windows, their stacking order is managed by the
        // window manager, as indicated by GTK_WINDOW_TOPLEVEL.
        // For Wayland we have to always use GTK_WINDOW_POPUP to control
        // popup window position.
        GtkWindowType type = GTK_WINDOW_TOPLEVEL;
        if (mWindowType == eWindowType_popup) {
            type = (mIsX11Display && aInitData->mNoAutoHide) ?
                GTK_WINDOW_TOPLEVEL : GTK_WINDOW_POPUP;
        }
        mShell = gtk_window_new(type);

        bool isSetVisual = false;
#ifdef MOZ_X11
        // Ensure gfxPlatform is initialized, since that is what initializes
        // gfxVars, used below.
        Unused << gfxPlatform::GetPlatform();

        bool useWebRender = gfx::gfxVars::UseWebRender() &&
             AllowWebRenderForThisWindow();

        bool shouldAccelerate = ComputeShouldAccelerate();
        MOZ_ASSERT(shouldAccelerate | !useWebRender);

        // If using WebRender on X11, we need to select a visual with a depth buffer,
        // as well as an alpha channel if transparency is requested. This must be done
        // before the widget is realized.

        // Use GL/WebRender compatible visual only when it is necessary, since
        // the visual consumes more memory.
        if (mIsX11Display && shouldAccelerate) {
            auto display =
                GDK_DISPLAY_XDISPLAY(gtk_widget_get_display(mShell));
            auto screen = gtk_widget_get_screen(mShell);
            int screenNumber = GDK_SCREEN_XNUMBER(screen);
            int visualId = 0;
            if (GLContextGLX::FindVisual(display, screenNumber,
                                         useWebRender, needsAlphaVisual,
                                         &visualId)) {
                // If we're using CSD, rendering will go through mContainer, but
                // it will inherit this visual as it is a child of mShell.
                gtk_widget_set_visual(mShell,
                                      gdk_x11_screen_lookup_visual(screen,
                                                                   visualId));
                mHasAlphaVisual = needsAlphaVisual;
                isSetVisual = true;
            } else {
                NS_WARNING("We're missing X11 Visual!");
            }
        }
#endif // MOZ_X11

        if (!isSetVisual && needsAlphaVisual) {
            GdkScreen *screen = gtk_widget_get_screen(mShell);
            if (gdk_screen_is_composited(screen)) {
                GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
                if (visual) {
                    gtk_widget_set_visual(mShell, visual);
                    mHasAlphaVisual = true;
                }
            }
=======
      mIsTopLevel = true;

      // Popups that are not noautohide are only temporary. The are used
      // for menus and the like and disappear when another window is used.
      // For most popups, use the standard GtkWindowType GTK_WINDOW_POPUP,
      // which will use a Window with the override-redirect attribute
      // (for temporary windows).
      // For long-lived windows, their stacking order is managed by the
      // window manager, as indicated by GTK_WINDOW_TOPLEVEL.
      // For Wayland we have to always use GTK_WINDOW_POPUP to control
      // popup window position.
      GtkWindowType type = GTK_WINDOW_TOPLEVEL;
      if (mWindowType == eWindowType_popup) {
        type = (mIsX11Display && aInitData->mNoAutoHide) ? GTK_WINDOW_TOPLEVEL
                                                         : GTK_WINDOW_POPUP;
      }
      mShell = gtk_window_new(type);

      // Ensure gfxPlatform is initialized, since that is what initializes
      // gfxVars, used below.
      Unused << gfxPlatform::GetPlatform();

      bool useWebRender =
          gfx::gfxVars::UseWebRender() && AllowWebRenderForThisWindow();

      bool shouldAccelerate = ComputeShouldAccelerate();
      MOZ_ASSERT(shouldAccelerate | !useWebRender);

      if (mWindowType == eWindowType_toplevel) {
        // We enable titlebar rendering for toplevel windows only.
        mCSDSupportLevel = GetSystemCSDSupportLevel();

        // There's no point to configure transparency
        // on non-composited screens.
        GdkScreen* screen = gdk_screen_get_default();
        if (gdk_screen_is_composited(screen)) {
          // Some Gtk+ themes use non-rectangular toplevel windows. To fully
          // support such themes we need to make toplevel window transparent
          // with ARGB visual.
          // It may cause performanance issue so make it configurable
          // and enable it by default for selected window managers.
          if (Preferences::HasUserValue("mozilla.widget.use-argb-visuals")) {
            // argb visual is explicitly required so use it
            needsAlphaVisual =
                Preferences::GetBool("mozilla.widget.use-argb-visuals");
          } else if (!mIsX11Display) {
            // Wayland uses ARGB visual by default
            needsAlphaVisual = true;
          } else if (mCSDSupportLevel != CSD_SUPPORT_NONE) {
            if (shouldAccelerate) {
              needsAlphaVisual = true;
            } else {
              // We want to draw a transparent titlebar but we can't use
              // ARGB visual due to Bug 1516224.
              // If we're on Mutter/X.org (Bug 1530252) just give up
              // and don't render transparent corners at all.
              mTransparencyBitmapForTitlebar = TitlebarCanUseShapeMask();
            }
          }
>>>>>>> upstream-releases
        }
      }

<<<<<<< HEAD
      if (!isSetVisual && needsAlphaVisual) {
        GdkScreen *screen = gtk_widget_get_screen(mShell);
        if (gdk_screen_is_composited(screen)) {
          GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
          if (visual) {
            gtk_widget_set_visual(mShell, visual);
            mHasAlphaVisual = true;
          }
||||||| merged common ancestors
        // We have a toplevel window with transparency. Mark it as transparent
        // now as nsWindow::SetTransparencyMode() can't be called after
        // nsWindow is created (Bug 1344839).
        if (mWindowType == eWindowType_toplevel && mHasAlphaVisual) {
            mIsTransparent = true;
=======
      bool isSetVisual = false;
      // If using WebRender on X11, we need to select a visual with a depth
      // buffer, as well as an alpha channel if transparency is requested. This
      // must be done before the widget is realized.

      // Use GL/WebRender compatible visual only when it is necessary, since
      // the visual consumes more memory.
      if (mIsX11Display && shouldAccelerate) {
        auto display = GDK_DISPLAY_XDISPLAY(gtk_widget_get_display(mShell));
        auto screen = gtk_widget_get_screen(mShell);
        int screenNumber = GDK_SCREEN_XNUMBER(screen);
        int visualId = 0;
        if (useWebRender) {
          // WebRender rquests AlphaVisual for making readback to work
          // correctly.
          needsAlphaVisual = true;
        }
        if (GLContextGLX::FindVisual(display, screenNumber, useWebRender,
                                     needsAlphaVisual, &visualId)) {
          // If we're using CSD, rendering will go through mContainer, but
          // it will inherit this visual as it is a child of mShell.
          gtk_widget_set_visual(mShell,
                                gdk_x11_screen_lookup_visual(screen, visualId));
          mHasAlphaVisual = needsAlphaVisual;
          isSetVisual = true;
        } else {
          NS_WARNING("We're missing X11 Visual!");
>>>>>>> upstream-releases
        }
      }

<<<<<<< HEAD
      // We have a toplevel window with transparency. Mark it as transparent
      // now as nsWindow::SetTransparencyMode() can't be called after
      // nsWindow is created (Bug 1344839).
      if (mWindowType == eWindowType_toplevel && mHasAlphaVisual) {
        mIsTransparent = true;
      }
||||||| merged common ancestors
        // We only move a general managed toplevel window if someone has
        // actually placed the window somewhere.  If no placement has taken
        // place, we just let the window manager Do The Right Thing.
        NativeResize();

        if (mWindowType == eWindowType_dialog) {
            SetDefaultIcon();
            gtk_window_set_wmclass(GTK_WINDOW(mShell), "Dialog",
                                   gdk_get_program_class());
            gtk_window_set_type_hint(GTK_WINDOW(mShell),
                                     GDK_WINDOW_TYPE_HINT_DIALOG);
            gtk_window_set_transient_for(GTK_WINDOW(mShell),
                                         topLevelParent);
        }
        else if (mWindowType == eWindowType_popup) {
            // With popup windows, we want to control their position, so don't
            // wait for the window manager to place them (which wouldn't
            // happen with override-redirect windows anyway).
            NativeMove();

            gtk_window_set_wmclass(GTK_WINDOW(mShell), "Popup",
                                   gdk_get_program_class());

            if (aInitData->mNoAutoHide) {
                // ... but the window manager does not decorate this window,
                // nor provide a separate taskbar icon.
                if (mBorderStyle == eBorderStyle_default) {
                  gtk_window_set_decorated(GTK_WINDOW(mShell), FALSE);
                }
                else {
                  bool decorate = mBorderStyle & eBorderStyle_title;
                  gtk_window_set_decorated(GTK_WINDOW(mShell), decorate);
                  if (decorate) {
                    gtk_window_set_deletable(GTK_WINDOW(mShell), mBorderStyle & eBorderStyle_close);
                  }
                }
                gtk_window_set_skip_taskbar_hint(GTK_WINDOW(mShell), TRUE);
                // Element focus is managed by the parent window so the
                // WM_HINTS input field is set to False to tell the window
                // manager not to set input focus to this window ...
                gtk_window_set_accept_focus(GTK_WINDOW(mShell), FALSE);
#ifdef MOZ_X11
                // ... but when the window manager offers focus through
                // WM_TAKE_FOCUS, focus is requested on the parent window.
                if (mIsX11Display) {
                    gtk_widget_realize(mShell);
                    gdk_window_add_filter(gtk_widget_get_window(mShell),
                                          popup_take_focus_filter, nullptr);
                }
#endif
            }
=======
      if (!isSetVisual && needsAlphaVisual) {
        GdkScreen* screen = gtk_widget_get_screen(mShell);
        if (gdk_screen_is_composited(screen)) {
          GdkVisual* visual = gdk_screen_get_rgba_visual(screen);
          if (visual) {
            gtk_widget_set_visual(mShell, visual);
            mHasAlphaVisual = true;
          }
        }
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // We only move a general managed toplevel window if someone has
      // actually placed the window somewhere.  If no placement has taken
      // place, we just let the window manager Do The Right Thing.
      NativeResize();

      if (mWindowType == eWindowType_dialog) {
        SetDefaultIcon();
        gtk_window_set_wmclass(GTK_WINDOW(mShell), "Dialog",
                               gdk_get_program_class());
        gtk_window_set_type_hint(GTK_WINDOW(mShell),
                                 GDK_WINDOW_TYPE_HINT_DIALOG);
        gtk_window_set_transient_for(GTK_WINDOW(mShell), topLevelParent);
      } else if (mWindowType == eWindowType_popup) {
        // With popup windows, we want to control their position, so don't
        // wait for the window manager to place them (which wouldn't
        // happen with override-redirect windows anyway).
        NativeMove();
||||||| merged common ancestors
            GdkWindowTypeHint gtkTypeHint;
            if (aInitData->mIsDragPopup) {
                gtkTypeHint = GDK_WINDOW_TYPE_HINT_DND;
                mIsDragPopup = true;
            }
            else {
                switch (aInitData->mPopupHint) {
                    case ePopupTypeMenu:
                        // Use GDK_WINDOW_TYPE_HINT_UTILITY on Wayland which
                        // guides Gtk to create the popup as subsurface
                        // instead of xdg_shell popup (see Bug 1423598).
                        gtkTypeHint = mIsX11Display ? GDK_WINDOW_TYPE_HINT_POPUP_MENU :
                                                      GDK_WINDOW_TYPE_HINT_UTILITY;
                        break;
                    case ePopupTypeTooltip:
                        gtkTypeHint = GDK_WINDOW_TYPE_HINT_TOOLTIP;
                        break;
                    default:
                        gtkTypeHint = GDK_WINDOW_TYPE_HINT_UTILITY;
                        break;
                }
            }
            gtk_window_set_type_hint(GTK_WINDOW(mShell), gtkTypeHint);
=======
      // We have a toplevel window with transparency. Mark it as transparent
      // now as nsWindow::SetTransparencyMode() can't be called after
      // nsWindow is created (Bug 1344839).
      if (mWindowType == eWindowType_toplevel &&
          (mHasAlphaVisual || mTransparencyBitmapForTitlebar)) {
        mIsTransparent = true;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
        gtk_window_set_wmclass(GTK_WINDOW(mShell), "Popup",
                               gdk_get_program_class());

        if (aInitData->mNoAutoHide) {
          // ... but the window manager does not decorate this window,
          // nor provide a separate taskbar icon.
          if (mBorderStyle == eBorderStyle_default) {
            gtk_window_set_decorated(GTK_WINDOW(mShell), FALSE);
          } else {
            bool decorate = mBorderStyle & eBorderStyle_title;
            gtk_window_set_decorated(GTK_WINDOW(mShell), decorate);
            if (decorate) {
              gtk_window_set_deletable(GTK_WINDOW(mShell),
                                       mBorderStyle & eBorderStyle_close);
||||||| merged common ancestors
            if (topLevelParent) {
                gtk_window_set_transient_for(GTK_WINDOW(mShell),
                                            topLevelParent);
=======
      // We only move a general managed toplevel window if someone has
      // actually placed the window somewhere.  If no placement has taken
      // place, we just let the window manager Do The Right Thing.
      NativeResize();

      if (mWindowType == eWindowType_dialog) {
        SetDefaultIcon();
        gtk_window_set_wmclass(GTK_WINDOW(mShell), "Dialog",
                               gdk_get_program_class());
        gtk_window_set_type_hint(GTK_WINDOW(mShell),
                                 GDK_WINDOW_TYPE_HINT_DIALOG);
        gtk_window_set_transient_for(GTK_WINDOW(mShell), topLevelParent);
      } else if (mWindowType == eWindowType_popup) {
        gtk_window_set_wmclass(GTK_WINDOW(mShell), "Popup",
                               gdk_get_program_class());

        if (aInitData->mNoAutoHide) {
          // ... but the window manager does not decorate this window,
          // nor provide a separate taskbar icon.
          if (mBorderStyle == eBorderStyle_default) {
            gtk_window_set_decorated(GTK_WINDOW(mShell), FALSE);
          } else {
            bool decorate = mBorderStyle & eBorderStyle_title;
            gtk_window_set_decorated(GTK_WINDOW(mShell), decorate);
            if (decorate) {
              gtk_window_set_deletable(GTK_WINDOW(mShell),
                                       mBorderStyle & eBorderStyle_close);
>>>>>>> upstream-releases
            }
          }
          gtk_window_set_skip_taskbar_hint(GTK_WINDOW(mShell), TRUE);
          // Element focus is managed by the parent window so the
          // WM_HINTS input field is set to False to tell the window
          // manager not to set input focus to this window ...
          gtk_window_set_accept_focus(GTK_WINDOW(mShell), FALSE);
#ifdef MOZ_X11
          // ... but when the window manager offers focus through
          // WM_TAKE_FOCUS, focus is requested on the parent window.
          if (mIsX11Display) {
            gtk_widget_realize(mShell);
            gdk_window_add_filter(gtk_widget_get_window(mShell),
                                  popup_take_focus_filter, nullptr);
          }
#endif
        }
<<<<<<< HEAD
||||||| merged common ancestors
        else { // must be eWindowType_toplevel
            SetDefaultIcon();
            gtk_window_set_wmclass(GTK_WINDOW(mShell), "Toplevel",
                                   gdk_get_program_class());

            // each toplevel window gets its own window group
            GtkWindowGroup *group = gtk_window_group_new();
            gtk_window_group_add_window(group, GTK_WINDOW(mShell));
            g_object_unref(group);

            // We enable titlebar rendering for toplevel windows only.
            mCSDSupportLevel = GetSystemCSDSupportLevel();
        }

        // Create a container to hold child windows and child GtkWidgets.
        GtkWidget *container = moz_container_new();
        mContainer = MOZ_CONTAINER(container);
=======

        GdkWindowTypeHint gtkTypeHint;
        if (aInitData->mIsDragPopup) {
          gtkTypeHint = GDK_WINDOW_TYPE_HINT_DND;
          mIsDragPopup = true;
        } else {
          switch (aInitData->mPopupHint) {
            case ePopupTypeMenu:
              gtkTypeHint = GDK_WINDOW_TYPE_HINT_POPUP_MENU;
              break;
            case ePopupTypeTooltip:
              gtkTypeHint = GDK_WINDOW_TYPE_HINT_TOOLTIP;
              break;
            default:
              gtkTypeHint = GDK_WINDOW_TYPE_HINT_UTILITY;
              break;
          }
        }
        gtk_window_set_type_hint(GTK_WINDOW(mShell), gtkTypeHint);

        if (topLevelParent) {
          LOG(("nsWindow::Create [%p] Set popup parent %p\n", (void*)this,
               topLevelParent));
          gtk_window_set_transient_for(GTK_WINDOW(mShell), topLevelParent);
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
        GdkWindowTypeHint gtkTypeHint;
        if (aInitData->mIsDragPopup) {
          gtkTypeHint = GDK_WINDOW_TYPE_HINT_DND;
          mIsDragPopup = true;
        } else {
          switch (aInitData->mPopupHint) {
            case ePopupTypeMenu:
              // Use GDK_WINDOW_TYPE_HINT_UTILITY on Wayland which
              // guides Gtk to create the popup as subsurface
              // instead of xdg_shell popup (see Bug 1423598).
              gtkTypeHint = mIsX11Display ? GDK_WINDOW_TYPE_HINT_POPUP_MENU
                                          : GDK_WINDOW_TYPE_HINT_UTILITY;
              break;
            case ePopupTypeTooltip:
              gtkTypeHint = GDK_WINDOW_TYPE_HINT_TOOLTIP;
              break;
            default:
              gtkTypeHint = GDK_WINDOW_TYPE_HINT_UTILITY;
              break;
          }
        }
        gtk_window_set_type_hint(GTK_WINDOW(mShell), gtkTypeHint);
||||||| merged common ancestors
        // "csd" style is set when widget is realized so we need to call
        // it explicitly now.
        gtk_widget_realize(mShell);

        /* There are several cases here:
         *
         * 1) We're running on Gtk+ without client side decorations.
         *    Content is rendered to mShell window and we listen
         *    to the Gtk+ events on mShell
         * 2) We're running on Gtk+ and client side decorations
         *    are drawn by Gtk+ to mShell. Content is rendered to mContainer
         *    and we listen to the Gtk+ events on mContainer.
         * 3) We're running on Wayland. All gecko content is rendered
         *    to mContainer and we listen to the Gtk+ events on mContainer.
         */
        GtkStyleContext* style = gtk_widget_get_style_context(mShell);
        drawToContainer =
            !mIsX11Display ||
            (mCSDSupportLevel == CSD_SUPPORT_CLIENT) ||
            gtk_style_context_has_class(style, "csd");
        eventWidget = (drawToContainer) ? container : mShell;

        gtk_widget_add_events(eventWidget, kEvents);
        if (drawToContainer)
            gtk_widget_add_events(mShell, GDK_PROPERTY_CHANGE_MASK);
=======
        // We need realized mShell at NativeMove().
        gtk_widget_realize(mShell);

        // With popup windows, we want to control their position, so don't
        // wait for the window manager to place them (which wouldn't
        // happen with override-redirect windows anyway).
        NativeMove();
      } else {  // must be eWindowType_toplevel
        SetDefaultIcon();
        gtk_window_set_wmclass(GTK_WINDOW(mShell), "Toplevel",
                               gdk_get_program_class());

        // each toplevel window gets its own window group
        GtkWindowGroup* group = gtk_window_group_new();
        gtk_window_group_add_window(group, GTK_WINDOW(mShell));
        g_object_unref(group);
      }

      if (aInitData->mAlwaysOnTop) {
        gtk_window_set_keep_above(GTK_WINDOW(mShell), TRUE);
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (topLevelParent) {
          gtk_window_set_transient_for(GTK_WINDOW(mShell), topLevelParent);
        }
      } else {  // must be eWindowType_toplevel
        SetDefaultIcon();
        gtk_window_set_wmclass(GTK_WINDOW(mShell), "Toplevel",
                               gdk_get_program_class());

        // each toplevel window gets its own window group
        GtkWindowGroup *group = gtk_window_group_new();
        gtk_window_group_add_window(group, GTK_WINDOW(mShell));
        g_object_unref(group);

        // We enable titlebar rendering for toplevel windows only.
        mCSDSupportLevel = GetSystemCSDSupportLevel();
      }
||||||| merged common ancestors
        // Prevent GtkWindow from painting a background to avoid flickering.
        gtk_widget_set_app_paintable(eventWidget, TRUE);
=======
      // Create a container to hold child windows and child GtkWidgets.
      GtkWidget* container = moz_container_new();
      mContainer = MOZ_CONTAINER(container);
#ifdef MOZ_WAYLAND
      if (!mIsX11Display && ComputeShouldAccelerate()) {
        RefPtr<nsWindow> self(this);
        moz_container_set_initial_draw_callback(mContainer, [self]() -> void {
          self->mNeedsUpdatingEGLSurface = true;
          self->WaylandEGLSurfaceForceRedraw();
        });
      }
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
      // Create a container to hold child windows and child GtkWidgets.
      GtkWidget *container = moz_container_new();
      mContainer = MOZ_CONTAINER(container);

      // "csd" style is set when widget is realized so we need to call
      // it explicitly now.
      gtk_widget_realize(mShell);

      /* There are several cases here:
       *
       * 1) We're running on Gtk+ without client side decorations.
       *    Content is rendered to mShell window and we listen
       *    to the Gtk+ events on mShell
       * 2) We're running on Gtk+ and client side decorations
       *    are drawn by Gtk+ to mShell. Content is rendered to mContainer
       *    and we listen to the Gtk+ events on mContainer.
       * 3) We're running on Wayland. All gecko content is rendered
       *    to mContainer and we listen to the Gtk+ events on mContainer.
       */
      GtkStyleContext *style = gtk_widget_get_style_context(mShell);
      drawToContainer = !mIsX11Display ||
                        (mCSDSupportLevel == CSD_SUPPORT_CLIENT) ||
                        gtk_style_context_has_class(style, "csd");
      eventWidget = (drawToContainer) ? container : mShell;

      // Prevent GtkWindow from painting a background to avoid flickering.
      gtk_widget_set_app_paintable(eventWidget, TRUE);

      gtk_widget_add_events(eventWidget, kEvents);
      if (drawToContainer) {
        gtk_widget_add_events(mShell, GDK_PROPERTY_CHANGE_MASK);
        gtk_widget_set_app_paintable(mShell, TRUE);
      }
||||||| merged common ancestors
        // If we draw to mContainer window then configure it now because
        // gtk_container_add() realizes the child widget.
        gtk_widget_set_has_window(container, drawToContainer);
=======
      // "csd" style is set when widget is realized so we need to call
      // it explicitly now.
      gtk_widget_realize(mShell);

      /* There are several cases here:
       *
       * 1) We're running on Gtk+ without client side decorations.
       *    Content is rendered to mShell window and we listen
       *    to the Gtk+ events on mShell
       * 2) We're running on Gtk+ and client side decorations
       *    are drawn by Gtk+ to mShell. Content is rendered to mContainer
       *    and we listen to the Gtk+ events on mContainer.
       * 3) We're running on Wayland. All gecko content is rendered
       *    to mContainer and we listen to the Gtk+ events on mContainer.
       */
      GtkStyleContext* style = gtk_widget_get_style_context(mShell);
      drawToContainer = !mIsX11Display ||
                        (mCSDSupportLevel == CSD_SUPPORT_CLIENT) ||
                        gtk_style_context_has_class(style, "csd");
      eventWidget = (drawToContainer) ? container : mShell;

      // Prevent GtkWindow from painting a background to avoid flickering.
      gtk_widget_set_app_paintable(eventWidget, TRUE);

      gtk_widget_add_events(eventWidget, kEvents);
      if (drawToContainer) {
        gtk_widget_add_events(mShell, GDK_PROPERTY_CHANGE_MASK);
        gtk_widget_set_app_paintable(mShell, TRUE);
      }
      if (mTransparencyBitmapForTitlebar) {
        moz_container_force_default_visual(mContainer);
      }
>>>>>>> upstream-releases

      // If we draw to mContainer window then configure it now because
      // gtk_container_add() realizes the child widget.
      gtk_widget_set_has_window(container, drawToContainer);

      gtk_container_add(GTK_CONTAINER(mShell), container);
      gtk_widget_realize(container);

      // make sure this is the focus widget in the container
      gtk_widget_show(container);
      gtk_widget_grab_focus(container);

      // the drawing window
      mGdkWindow = gtk_widget_get_window(eventWidget);

      if (mWindowType == eWindowType_popup) {
        // gdk does not automatically set the cursor for "temporary"
        // windows, which are what gtk uses for popups.

<<<<<<< HEAD
        mCursor = eCursor_wait;  // force SetCursor to actually set the
                                 // cursor, even though our internal state
                                 // indicates that we already have the
                                 // standard cursor.
        SetCursor(eCursor_standard);
||||||| merged common ancestors
            if (aInitData->mNoAutoHide) {
                gint wmd = ConvertBorderStyles(mBorderStyle);
                if (wmd != -1)
                  gdk_window_set_decorations(mGdkWindow, (GdkWMDecoration) wmd);
            }
=======
        mCursor = eCursor_wait;  // force SetCursor to actually set the
                                 // cursor, even though our internal state
                                 // indicates that we already have the
                                 // standard cursor.
        SetCursor(eCursor_standard, nullptr, 0, 0);
>>>>>>> upstream-releases

        if (aInitData->mNoAutoHide) {
          gint wmd = ConvertBorderStyles(mBorderStyle);
          if (wmd != -1)
            gdk_window_set_decorations(mGdkWindow, (GdkWMDecoration)wmd);
        }

<<<<<<< HEAD
        // If the popup ignores mouse events, set an empty input shape.
        if (aInitData->mMouseTransparent) {
          cairo_rectangle_int_t rect = {0, 0, 0, 0};
          cairo_region_t *region = cairo_region_create_rectangle(&rect);
||||||| merged common ancestors
            // If the popup ignores mouse events, set an empty input shape.
            if (aInitData->mMouseTransparent) {
              cairo_rectangle_int_t rect = { 0, 0, 0, 0 };
              cairo_region_t *region = cairo_region_create_rectangle(&rect);
=======
        // If the popup ignores mouse events, set an empty input shape.
        if (aInitData->mMouseTransparent) {
          cairo_rectangle_int_t rect = {0, 0, 0, 0};
          cairo_region_t* region = cairo_region_create_rectangle(&rect);
>>>>>>> upstream-releases

          gdk_window_input_shape_combine_region(mGdkWindow, region, 0, 0);
          cairo_region_destroy(region);
        }
      }
    } break;

    case eWindowType_plugin:
    case eWindowType_plugin_ipc_chrome:
    case eWindowType_plugin_ipc_content:
      MOZ_ASSERT_UNREACHABLE("Unexpected eWindowType_plugin*");
      return NS_ERROR_FAILURE;

    case eWindowType_child: {
<<<<<<< HEAD
      if (parentMozContainer) {
        mGdkWindow = CreateGdkWindow(parentGdkWindow, parentMozContainer);
        mHasMappedToplevel = parentnsWindow->mHasMappedToplevel;
      } else if (parentGtkContainer) {
        // This MozContainer has its own window for drawing and receives
        // events because there is no mShell widget (corresponding to this
        // nsWindow).
        GtkWidget *container = moz_container_new();
        mContainer = MOZ_CONTAINER(container);
        eventWidget = container;
        gtk_widget_add_events(eventWidget, kEvents);
        gtk_container_add(parentGtkContainer, container);
        gtk_widget_realize(container);
||||||| merged common ancestors
        if (parentMozContainer) {
            mGdkWindow = CreateGdkWindow(parentGdkWindow, parentMozContainer);
            mHasMappedToplevel = parentnsWindow->mHasMappedToplevel;
        }
        else if (parentGtkContainer) {
            // This MozContainer has its own window for drawing and receives
            // events because there is no mShell widget (corresponding to this
            // nsWindow).
            GtkWidget *container = moz_container_new();
            mContainer = MOZ_CONTAINER(container);
            eventWidget = container;
            gtk_widget_add_events(eventWidget, kEvents);
            gtk_container_add(parentGtkContainer, container);
            gtk_widget_realize(container);

            mGdkWindow = gtk_widget_get_window(container);
        }
        else {
            NS_WARNING("Warning: tried to create a new child widget with no parent!");
            return NS_ERROR_FAILURE;
        }
    }
        break;
    default:
        break;
    }
=======
      if (parentMozContainer) {
        mGdkWindow = CreateGdkWindow(parentGdkWindow, parentMozContainer);
        mHasMappedToplevel = parentnsWindow->mHasMappedToplevel;
      } else if (parentGtkContainer) {
        // This MozContainer has its own window for drawing and receives
        // events because there is no mShell widget (corresponding to this
        // nsWindow).
        GtkWidget* container = moz_container_new();
        mContainer = MOZ_CONTAINER(container);
        eventWidget = container;
        gtk_widget_add_events(eventWidget, kEvents);
        gtk_container_add(parentGtkContainer, container);
        gtk_widget_realize(container);
>>>>>>> upstream-releases

        mGdkWindow = gtk_widget_get_window(container);
      } else {
        NS_WARNING(
            "Warning: tried to create a new child widget with no parent!");
        return NS_ERROR_FAILURE;
      }
    } break;
    default:
      break;
  }

<<<<<<< HEAD
  // label the drawing window with this object so we can find our way home
  g_object_set_data(G_OBJECT(mGdkWindow), "nsWindow", this);
  if (drawToContainer) {
    // Also label mShell toplevel window,
    // property_notify_event_cb callback also needs to find its way home
    g_object_set_data(G_OBJECT(gtk_widget_get_window(mShell)), "nsWindow",
                      this);
  }

  if (mContainer) g_object_set_data(G_OBJECT(mContainer), "nsWindow", this);

  if (mShell) g_object_set_data(G_OBJECT(mShell), "nsWindow", this);

  // attach listeners for events
  if (mShell) {
    g_signal_connect(mShell, "configure_event", G_CALLBACK(configure_event_cb),
                     nullptr);
    g_signal_connect(mShell, "delete_event", G_CALLBACK(delete_event_cb),
                     nullptr);
    g_signal_connect(mShell, "window_state_event",
                     G_CALLBACK(window_state_event_cb), nullptr);
    g_signal_connect(mShell, "check-resize", G_CALLBACK(check_resize_cb),
                     nullptr);
    g_signal_connect(mShell, "composited-changed",
                     G_CALLBACK(widget_composited_changed_cb), nullptr);
    g_signal_connect(mShell, "property-notify-event",
                     G_CALLBACK(property_notify_event_cb), nullptr);

    GdkScreen *screen = gtk_widget_get_screen(mShell);
    if (!g_signal_handler_find(screen, G_SIGNAL_MATCH_FUNC, 0, 0, nullptr,
                               FuncToGpointer(screen_composited_changed_cb),
                               0)) {
      g_signal_connect(screen, "composited-changed",
                       G_CALLBACK(screen_composited_changed_cb), nullptr);
    }

    GtkSettings *default_settings = gtk_settings_get_default();
    g_signal_connect_after(default_settings, "notify::gtk-theme-name",
                           G_CALLBACK(settings_changed_cb), this);
    g_signal_connect_after(default_settings, "notify::gtk-font-name",
                           G_CALLBACK(settings_changed_cb), this);
    g_signal_connect_after(default_settings, "notify::gtk-enable-animations",
                           G_CALLBACK(settings_changed_cb), this);
  }
||||||| merged common ancestors
    if (mContainer)
        g_object_set_data(G_OBJECT(mContainer), "nsWindow", this);

    if (mShell)
        g_object_set_data(G_OBJECT(mShell), "nsWindow", this);

    // attach listeners for events
    if (mShell) {
        g_signal_connect(mShell, "configure_event",
                         G_CALLBACK(configure_event_cb), nullptr);
        g_signal_connect(mShell, "delete_event",
                         G_CALLBACK(delete_event_cb), nullptr);
        g_signal_connect(mShell, "window_state_event",
                         G_CALLBACK(window_state_event_cb), nullptr);
        g_signal_connect(mShell, "check-resize",
                         G_CALLBACK(check_resize_cb), nullptr);
        g_signal_connect(mShell, "composited-changed",
                         G_CALLBACK(widget_composited_changed_cb), nullptr);
        g_signal_connect(mShell, "property-notify-event",
                         G_CALLBACK(property_notify_event_cb), nullptr);

        GdkScreen *screen = gtk_widget_get_screen(mShell);
        if (!g_signal_handler_find(screen, G_SIGNAL_MATCH_FUNC,
                                   0, 0, nullptr,
                                   FuncToGpointer(screen_composited_changed_cb), 0)) {
            g_signal_connect(screen, "composited-changed",
                             G_CALLBACK(screen_composited_changed_cb), nullptr);
        }
=======
  // label the drawing window with this object so we can find our way home
  g_object_set_data(G_OBJECT(mGdkWindow), "nsWindow", this);
  if (drawToContainer) {
    // Also label mShell toplevel window,
    // property_notify_event_cb callback also needs to find its way home
    g_object_set_data(G_OBJECT(gtk_widget_get_window(mShell)), "nsWindow",
                      this);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mContainer) {
    // Widget signals
    g_signal_connect(mContainer, "unrealize",
                     G_CALLBACK(container_unrealize_cb), nullptr);
    g_signal_connect_after(mContainer, "size_allocate",
                           G_CALLBACK(size_allocate_cb), nullptr);
    g_signal_connect(mContainer, "hierarchy-changed",
                     G_CALLBACK(hierarchy_changed_cb), nullptr);
    g_signal_connect(mContainer, "notify::scale-factor",
                     G_CALLBACK(scale_changed_cb), nullptr);
    // Initialize mHasMappedToplevel.
    hierarchy_changed_cb(GTK_WIDGET(mContainer), nullptr);
    // Expose, focus, key, and drag events are sent even to GTK_NO_WINDOW
    // widgets.
    g_signal_connect(G_OBJECT(mContainer), "draw", G_CALLBACK(expose_event_cb),
                     nullptr);
    g_signal_connect(mContainer, "focus_in_event",
                     G_CALLBACK(focus_in_event_cb), nullptr);
    g_signal_connect(mContainer, "focus_out_event",
                     G_CALLBACK(focus_out_event_cb), nullptr);
    g_signal_connect(mContainer, "key_press_event",
                     G_CALLBACK(key_press_event_cb), nullptr);
    g_signal_connect(mContainer, "key_release_event",
                     G_CALLBACK(key_release_event_cb), nullptr);

    gtk_drag_dest_set((GtkWidget *)mContainer, (GtkDestDefaults)0, nullptr, 0,
                      (GdkDragAction)0);

    g_signal_connect(mContainer, "drag_motion",
                     G_CALLBACK(drag_motion_event_cb), nullptr);
    g_signal_connect(mContainer, "drag_leave", G_CALLBACK(drag_leave_event_cb),
                     nullptr);
    g_signal_connect(mContainer, "drag_drop", G_CALLBACK(drag_drop_event_cb),
                     nullptr);
    g_signal_connect(mContainer, "drag_data_received",
                     G_CALLBACK(drag_data_received_event_cb), nullptr);

    GtkWidget *widgets[] = {GTK_WIDGET(mContainer),
                            !drawToContainer ? mShell : nullptr};
    for (size_t i = 0; i < ArrayLength(widgets) && widgets[i]; ++i) {
      // Visibility events are sent to the owning widget of the relevant
      // window but do not propagate to parent widgets so connect on
      // mShell (if it exists) as well as mContainer.
      g_signal_connect(widgets[i], "visibility-notify-event",
                       G_CALLBACK(visibility_notify_event_cb), nullptr);
      // Similarly double buffering is controlled by the window's owning
      // widget.  Disable double buffering for painting directly to the
      // X Window.
      gtk_widget_set_double_buffered(widgets[i], FALSE);
    }

    // We create input contexts for all containers, except for
    // toplevel popup windows
    if (mWindowType != eWindowType_popup) {
      mIMContext = new IMContextWrapper(this);
    }
  } else if (!mIMContext) {
    nsWindow *container = GetContainerWindow();
    if (container) {
      mIMContext = container->mIMContext;
||||||| merged common ancestors
        GtkSettings* default_settings = gtk_settings_get_default();
        g_signal_connect_after(default_settings,
                               "notify::gtk-theme-name",
                               G_CALLBACK(settings_changed_cb), this);
        g_signal_connect_after(default_settings,
                               "notify::gtk-font-name",
                               G_CALLBACK(settings_changed_cb), this);
        g_signal_connect_after(default_settings,
                               "notify::gtk-enable-animations",
                               G_CALLBACK(settings_changed_cb), this);
    }

    if (mContainer) {
        // Widget signals
        g_signal_connect(mContainer, "unrealize",
                         G_CALLBACK(container_unrealize_cb), nullptr);
        g_signal_connect_after(mContainer, "size_allocate",
                               G_CALLBACK(size_allocate_cb), nullptr);
        g_signal_connect(mContainer, "hierarchy-changed",
                         G_CALLBACK(hierarchy_changed_cb), nullptr);
        g_signal_connect(mContainer, "notify::scale-factor",
                         G_CALLBACK(scale_changed_cb), nullptr);
        // Initialize mHasMappedToplevel.
        hierarchy_changed_cb(GTK_WIDGET(mContainer), nullptr);
        // Expose, focus, key, and drag events are sent even to GTK_NO_WINDOW
        // widgets.
        g_signal_connect(G_OBJECT(mContainer), "draw",
                         G_CALLBACK(expose_event_cb), nullptr);
        g_signal_connect(mContainer, "focus_in_event",
                         G_CALLBACK(focus_in_event_cb), nullptr);
        g_signal_connect(mContainer, "focus_out_event",
                         G_CALLBACK(focus_out_event_cb), nullptr);
        g_signal_connect(mContainer, "key_press_event",
                         G_CALLBACK(key_press_event_cb), nullptr);
        g_signal_connect(mContainer, "key_release_event",
                         G_CALLBACK(key_release_event_cb), nullptr);

        gtk_drag_dest_set((GtkWidget *)mContainer,
                          (GtkDestDefaults)0,
                          nullptr,
                          0,
                          (GdkDragAction)0);

        g_signal_connect(mContainer, "drag_motion",
                         G_CALLBACK(drag_motion_event_cb), nullptr);
        g_signal_connect(mContainer, "drag_leave",
                         G_CALLBACK(drag_leave_event_cb), nullptr);
        g_signal_connect(mContainer, "drag_drop",
                         G_CALLBACK(drag_drop_event_cb), nullptr);
        g_signal_connect(mContainer, "drag_data_received",
                         G_CALLBACK(drag_data_received_event_cb), nullptr);

        GtkWidget *widgets[] = { GTK_WIDGET(mContainer),
                                 !drawToContainer ? mShell : nullptr };
        for (size_t i = 0; i < ArrayLength(widgets) && widgets[i]; ++i) {
            // Visibility events are sent to the owning widget of the relevant
            // window but do not propagate to parent widgets so connect on
            // mShell (if it exists) as well as mContainer.
            g_signal_connect(widgets[i], "visibility-notify-event",
                             G_CALLBACK(visibility_notify_event_cb), nullptr);
            // Similarly double buffering is controlled by the window's owning
            // widget.  Disable double buffering for painting directly to the
            // X Window.
            gtk_widget_set_double_buffered(widgets[i], FALSE);
        }

        // We create input contexts for all containers, except for
        // toplevel popup windows
        if (mWindowType != eWindowType_popup) {
            mIMContext = new IMContextWrapper(this);
        }
    } else if (!mIMContext) {
        nsWindow *container = GetContainerWindow();
        if (container) {
            mIMContext = container->mIMContext;
        }
=======
  if (mContainer) g_object_set_data(G_OBJECT(mContainer), "nsWindow", this);

  if (mShell) g_object_set_data(G_OBJECT(mShell), "nsWindow", this);

  // attach listeners for events
  if (mShell) {
    g_signal_connect(mShell, "configure_event", G_CALLBACK(configure_event_cb),
                     nullptr);
    g_signal_connect(mShell, "delete_event", G_CALLBACK(delete_event_cb),
                     nullptr);
    g_signal_connect(mShell, "window_state_event",
                     G_CALLBACK(window_state_event_cb), nullptr);
    g_signal_connect(mShell, "check-resize", G_CALLBACK(check_resize_cb),
                     nullptr);
    g_signal_connect(mShell, "composited-changed",
                     G_CALLBACK(widget_composited_changed_cb), nullptr);
    g_signal_connect(mShell, "property-notify-event",
                     G_CALLBACK(property_notify_event_cb), nullptr);

    GdkScreen* screen = gtk_widget_get_screen(mShell);
    if (!g_signal_handler_find(screen, G_SIGNAL_MATCH_FUNC, 0, 0, nullptr,
                               FuncToGpointer(screen_composited_changed_cb),
                               0)) {
      g_signal_connect(screen, "composited-changed",
                       G_CALLBACK(screen_composited_changed_cb), nullptr);
    }

    GtkSettings* default_settings = gtk_settings_get_default();
    g_signal_connect_after(default_settings, "notify::gtk-theme-name",
                           G_CALLBACK(settings_changed_cb), this);
    g_signal_connect_after(default_settings, "notify::gtk-font-name",
                           G_CALLBACK(settings_changed_cb), this);
    g_signal_connect_after(default_settings, "notify::gtk-enable-animations",
                           G_CALLBACK(settings_changed_cb), this);
    g_signal_connect_after(default_settings, "notify::gtk-decoration-layout",
                           G_CALLBACK(settings_changed_cb), this);
  }

  if (mContainer) {
    // Widget signals
    g_signal_connect(mContainer, "unrealize",
                     G_CALLBACK(container_unrealize_cb), nullptr);
    g_signal_connect_after(mContainer, "size_allocate",
                           G_CALLBACK(size_allocate_cb), nullptr);
    g_signal_connect(mContainer, "hierarchy-changed",
                     G_CALLBACK(hierarchy_changed_cb), nullptr);
    g_signal_connect(mContainer, "notify::scale-factor",
                     G_CALLBACK(scale_changed_cb), nullptr);
    // Initialize mHasMappedToplevel.
    hierarchy_changed_cb(GTK_WIDGET(mContainer), nullptr);
    // Expose, focus, key, and drag events are sent even to GTK_NO_WINDOW
    // widgets.
    g_signal_connect(G_OBJECT(mContainer), "draw", G_CALLBACK(expose_event_cb),
                     nullptr);
    g_signal_connect(mContainer, "focus_in_event",
                     G_CALLBACK(focus_in_event_cb), nullptr);
    g_signal_connect(mContainer, "focus_out_event",
                     G_CALLBACK(focus_out_event_cb), nullptr);
    g_signal_connect(mContainer, "key_press_event",
                     G_CALLBACK(key_press_event_cb), nullptr);
    g_signal_connect(mContainer, "key_release_event",
                     G_CALLBACK(key_release_event_cb), nullptr);

    gtk_drag_dest_set((GtkWidget*)mContainer, (GtkDestDefaults)0, nullptr, 0,
                      (GdkDragAction)0);

    g_signal_connect(mContainer, "drag_motion",
                     G_CALLBACK(drag_motion_event_cb), nullptr);
    g_signal_connect(mContainer, "drag_leave", G_CALLBACK(drag_leave_event_cb),
                     nullptr);
    g_signal_connect(mContainer, "drag_drop", G_CALLBACK(drag_drop_event_cb),
                     nullptr);
    g_signal_connect(mContainer, "drag_data_received",
                     G_CALLBACK(drag_data_received_event_cb), nullptr);

    GtkWidget* widgets[] = {GTK_WIDGET(mContainer),
                            !drawToContainer ? mShell : nullptr};
    for (size_t i = 0; i < ArrayLength(widgets) && widgets[i]; ++i) {
      // Visibility events are sent to the owning widget of the relevant
      // window but do not propagate to parent widgets so connect on
      // mShell (if it exists) as well as mContainer.
      g_signal_connect(widgets[i], "visibility-notify-event",
                       G_CALLBACK(visibility_notify_event_cb), nullptr);
      // Similarly double buffering is controlled by the window's owning
      // widget.  Disable double buffering for painting directly to the
      // X Window.
      gtk_widget_set_double_buffered(widgets[i], FALSE);
    }

    // We create input contexts for all containers, except for
    // toplevel popup windows
    if (mWindowType != eWindowType_popup) {
      mIMContext = new IMContextWrapper(this);
    }
  } else if (!mIMContext) {
    nsWindow* container = GetContainerWindow();
    if (container) {
      mIMContext = container->mIMContext;
>>>>>>> upstream-releases
    }
  }

  if (eventWidget) {
    // These events are sent to the owning widget of the relevant window
    // and propagate up to the first widget that handles the events, so we
    // need only connect on mShell, if it exists, to catch events on its
    // window and windows of mContainer.
    g_signal_connect(eventWidget, "enter-notify-event",
                     G_CALLBACK(enter_notify_event_cb), nullptr);
    g_signal_connect(eventWidget, "leave-notify-event",
                     G_CALLBACK(leave_notify_event_cb), nullptr);
    g_signal_connect(eventWidget, "motion-notify-event",
                     G_CALLBACK(motion_notify_event_cb), nullptr);
    g_signal_connect(eventWidget, "button-press-event",
                     G_CALLBACK(button_press_event_cb), nullptr);
    g_signal_connect(eventWidget, "button-release-event",
                     G_CALLBACK(button_release_event_cb), nullptr);
    g_signal_connect(eventWidget, "scroll-event", G_CALLBACK(scroll_event_cb),
                     nullptr);
#if GTK_CHECK_VERSION(3, 4, 0)
    g_signal_connect(eventWidget, "touch-event", G_CALLBACK(touch_event_cb),
                     nullptr);
#endif
  }

<<<<<<< HEAD
  LOG(("nsWindow [%p]\n", (void *)this));
  if (mShell) {
    LOG(("\tmShell %p mContainer %p mGdkWindow %p 0x%lx\n", mShell, mContainer,
         mGdkWindow, mIsX11Display ? gdk_x11_window_get_xid(mGdkWindow) : 0));
  } else if (mContainer) {
    LOG(("\tmContainer %p mGdkWindow %p\n", mContainer, mGdkWindow));
  } else if (mGdkWindow) {
    LOG(("\tmGdkWindow %p parent %p\n", mGdkWindow,
         gdk_window_get_parent(mGdkWindow)));
  }
||||||| merged common ancestors
    LOG(("nsWindow [%p]\n", (void *)this));
    if (mShell) {
        LOG(("\tmShell %p mContainer %p mGdkWindow %p 0x%lx\n",
             mShell, mContainer, mGdkWindow,
             mIsX11Display ? gdk_x11_window_get_xid(mGdkWindow) : 0));
    } else if (mContainer) {
        LOG(("\tmContainer %p mGdkWindow %p\n", mContainer, mGdkWindow));
    }
    else if (mGdkWindow) {
        LOG(("\tmGdkWindow %p parent %p\n",
             mGdkWindow, gdk_window_get_parent(mGdkWindow)));
    }
=======
  LOG(("nsWindow [%p]\n", (void*)this));
  if (mShell) {
    LOG(("\tmShell %p mContainer %p mGdkWindow %p 0x%lx\n", mShell, mContainer,
         mGdkWindow, mIsX11Display ? gdk_x11_window_get_xid(mGdkWindow) : 0));
  } else if (mContainer) {
    LOG(("\tmContainer %p mGdkWindow %p\n", mContainer, mGdkWindow));
  } else if (mGdkWindow) {
    LOG(("\tmGdkWindow %p parent %p\n", mGdkWindow,
         gdk_window_get_parent(mGdkWindow)));
  }
>>>>>>> upstream-releases

  // resize so that everything is set to the right dimensions
  if (!mIsTopLevel)
    Resize(mBounds.x, mBounds.y, mBounds.width, mBounds.height, false);

#ifdef MOZ_X11
  if (mIsX11Display && mGdkWindow) {
    mXDisplay = GDK_WINDOW_XDISPLAY(mGdkWindow);
    mXWindow = gdk_x11_window_get_xid(mGdkWindow);

<<<<<<< HEAD
    GdkVisual *gdkVisual = gdk_window_get_visual(mGdkWindow);
    mXVisual = gdk_x11_visual_get_xvisual(gdkVisual);
    mXDepth = gdk_visual_get_depth(gdkVisual);
    bool shaped = needsAlphaVisual && !mHasAlphaVisual;
||||||| merged common ancestors
      GdkVisual* gdkVisual = gdk_window_get_visual(mGdkWindow);
      mXVisual = gdk_x11_visual_get_xvisual(gdkVisual);
      mXDepth = gdk_visual_get_depth(gdkVisual);
      bool shaped = needsAlphaVisual && !mHasAlphaVisual;
=======
    GdkVisual* gdkVisual = gdk_window_get_visual(mGdkWindow);
    mXVisual = gdk_x11_visual_get_xvisual(gdkVisual);
    mXDepth = gdk_visual_get_depth(gdkVisual);
    bool shaped = needsAlphaVisual && !mHasAlphaVisual;
>>>>>>> upstream-releases

<<<<<<< HEAD
    mSurfaceProvider.Initialize(mXDisplay, mXWindow, mXVisual, mXDepth, shaped);

    if (mIsTopLevel) {
      // Set window manager hint to keep fullscreen windows composited.
      //
      // If the window were to get unredirected, there could be visible
      // tearing because Gecko does not align its framebuffer updates with
      // vblank.
      SetCompositorHint(GTK_WIDGET_COMPOSIDED_ENABLED);
    }
  }
#ifdef MOZ_WAYLAND
  else if (!mIsX11Display) {
    mSurfaceProvider.Initialize(this);
  }
||||||| merged common ancestors
      mSurfaceProvider.Initialize(mXDisplay, mXWindow, mXVisual, mXDepth,
                                  shaped);
    }
#ifdef MOZ_WAYLAND
    else if (!mIsX11Display) {
      mSurfaceProvider.Initialize(this);
    }
=======
    mSurfaceProvider.Initialize(mXDisplay, mXWindow, mXVisual, mXDepth, shaped);

    if (mIsTopLevel) {
      // Set window manager hint to keep fullscreen windows composited.
      //
      // If the window were to get unredirected, there could be visible
      // tearing because Gecko does not align its framebuffer updates with
      // vblank.
      SetCompositorHint(GTK_WIDGET_COMPOSIDED_ENABLED);
    }
  }
#  ifdef MOZ_WAYLAND
  else if (!mIsX11Display) {
    mSurfaceProvider.Initialize(this);
  }
#  endif
>>>>>>> upstream-releases
#endif
<<<<<<< HEAD
#endif
  return NS_OK;
||||||| merged common ancestors
#endif
    return NS_OK;
=======
  return NS_OK;
>>>>>>> upstream-releases
}

void nsWindow::RefreshWindowClass(void) {
  if (mGtkWindowTypeName.IsEmpty() || mGtkWindowRoleName.IsEmpty()) return;

<<<<<<< HEAD
  GdkWindow *gdkWindow = gtk_widget_get_window(mShell);
  gdk_window_set_role(gdkWindow, mGtkWindowRoleName.get());
||||||| merged common ancestors
    GdkWindow* gdkWindow = gtk_widget_get_window(mShell);
    gdk_window_set_role(gdkWindow, mGtkWindowRoleName.get());
=======
  GdkWindow* gdkWindow = gtk_widget_get_window(mShell);
  gdk_window_set_role(gdkWindow, mGtkWindowRoleName.get());
>>>>>>> upstream-releases

#ifdef MOZ_X11
<<<<<<< HEAD
  if (mIsX11Display) {
    XClassHint *class_hint = XAllocClassHint();
    if (!class_hint) {
      return;
    }
    const char *res_class = gdk_get_program_class();
    if (!res_class) return;

    class_hint->res_name = const_cast<char *>(mGtkWindowTypeName.get());
    class_hint->res_class = const_cast<char *>(res_class);

    // Can't use gtk_window_set_wmclass() for this; it prints
    // a warning & refuses to make the change.
    GdkDisplay *display = gdk_display_get_default();
    XSetClassHint(GDK_DISPLAY_XDISPLAY(display),
                  gdk_x11_window_get_xid(gdkWindow), class_hint);
    XFree(class_hint);
  }
||||||| merged common ancestors
    if (mIsX11Display) {
        XClassHint *class_hint = XAllocClassHint();
        if (!class_hint) {
          return;
        }
        const char *res_class = gdk_get_program_class();
        if (!res_class)
          return;

        class_hint->res_name = const_cast<char*>(mGtkWindowTypeName.get());
        class_hint->res_class = const_cast<char*>(res_class);

        // Can't use gtk_window_set_wmclass() for this; it prints
        // a warning & refuses to make the change.
        GdkDisplay *display = gdk_display_get_default();
        XSetClassHint(GDK_DISPLAY_XDISPLAY(display),
                      gdk_x11_window_get_xid(gdkWindow),
                      class_hint);
        XFree(class_hint);
    }
=======
  if (mIsX11Display) {
    XClassHint* class_hint = XAllocClassHint();
    if (!class_hint) {
      return;
    }
    const char* res_class = gdk_get_program_class();
    if (!res_class) return;

    class_hint->res_name = const_cast<char*>(mGtkWindowTypeName.get());
    class_hint->res_class = const_cast<char*>(res_class);

    // Can't use gtk_window_set_wmclass() for this; it prints
    // a warning & refuses to make the change.
    GdkDisplay* display = gdk_display_get_default();
    XSetClassHint(GDK_DISPLAY_XDISPLAY(display),
                  gdk_x11_window_get_xid(gdkWindow), class_hint);
    XFree(class_hint);
  }
>>>>>>> upstream-releases
#endif /* MOZ_X11 */
}

<<<<<<< HEAD
void nsWindow::SetWindowClass(const nsAString &xulWinType) {
  if (!mShell) return;
||||||| merged common ancestors
void
nsWindow::SetWindowClass(const nsAString &xulWinType)
{
    if (!mShell)
      return;
=======
void nsWindow::SetWindowClass(const nsAString& xulWinType) {
  if (!mShell) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
  char *res_name = ToNewCString(xulWinType);
  if (!res_name) return;
||||||| merged common ancestors
    char *res_name = ToNewCString(xulWinType);
    if (!res_name)
      return;
=======
  char* res_name = ToNewCString(xulWinType);
  if (!res_name) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
  const char *role = nullptr;
||||||| merged common ancestors
    const char *role = nullptr;
=======
  const char* role = nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Parse res_name into a name and role. Characters other than
  // [A-Za-z0-9_-] are converted to '_'. Anything after the first
  // colon is assigned to role; if there's no colon, assign the
  // whole thing to both role and res_name.
  for (char *c = res_name; *c; c++) {
    if (':' == *c) {
      *c = 0;
      role = c + 1;
    } else if (!isascii(*c) || (!isalnum(*c) && ('_' != *c) && ('-' != *c)))
      *c = '_';
  }
  res_name[0] = toupper(res_name[0]);
  if (!role) role = res_name;
||||||| merged common ancestors
    // Parse res_name into a name and role. Characters other than
    // [A-Za-z0-9_-] are converted to '_'. Anything after the first
    // colon is assigned to role; if there's no colon, assign the
    // whole thing to both role and res_name.
    for (char *c = res_name; *c; c++) {
      if (':' == *c) {
        *c = 0;
        role = c + 1;
      }
      else if (!isascii(*c) || (!isalnum(*c) && ('_' != *c) && ('-' != *c)))
        *c = '_';
    }
    res_name[0] = toupper(res_name[0]);
    if (!role) role = res_name;
=======
  // Parse res_name into a name and role. Characters other than
  // [A-Za-z0-9_-] are converted to '_'. Anything after the first
  // colon is assigned to role; if there's no colon, assign the
  // whole thing to both role and res_name.
  for (char* c = res_name; *c; c++) {
    if (':' == *c) {
      *c = 0;
      role = c + 1;
    } else if (!isascii(*c) || (!isalnum(*c) && ('_' != *c) && ('-' != *c)))
      *c = '_';
  }
  res_name[0] = toupper(res_name[0]);
  if (!role) role = res_name;
>>>>>>> upstream-releases

  mGtkWindowTypeName = res_name;
  mGtkWindowRoleName = role;
  free(res_name);

  RefreshWindowClass();
}

void nsWindow::NativeResize() {
  if (!AreBoundsSane()) {
    // If someone has set this so that the needs show flag is false
    // and it needs to be hidden, update the flag and hide the
    // window.  This flag will be cleared the next time someone
    // hides the window or shows it.  It also prevents us from
    // calling NativeShow(false) excessively on the window which
    // causes unneeded X traffic.
    if (!mNeedsShow && mIsShown) {
      mNeedsShow = true;
      NativeShow(false);
    }
    return;
  }

  GdkRectangle size = DevicePixelsToGdkSizeRoundUp(mBounds.Size());

<<<<<<< HEAD
  LOG(("nsWindow::NativeResize [%p] %d %d\n", (void *)this, size.width,
       size.height));
||||||| merged common ancestors
    LOG(("nsWindow::NativeResize [%p] %d %d\n", (void *)this,
         size.width, size.height));
=======
  LOG(("nsWindow::NativeResize [%p] %d %d\n", (void*)this, size.width,
       size.height));
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mIsTopLevel) {
    MOZ_ASSERT(size.width > 0 && size.height > 0,
               "Can't resize window smaller than 1x1.");
    gtk_window_resize(GTK_WINDOW(mShell), size.width, size.height);
  } else if (mContainer) {
    GtkWidget *widget = GTK_WIDGET(mContainer);
    GtkAllocation allocation, prev_allocation;
    gtk_widget_get_allocation(widget, &prev_allocation);
    allocation.x = prev_allocation.x;
    allocation.y = prev_allocation.y;
    allocation.width = size.width;
    allocation.height = size.height;
    gtk_widget_size_allocate(widget, &allocation);
  } else if (mGdkWindow) {
    gdk_window_resize(mGdkWindow, size.width, size.height);
  }
||||||| merged common ancestors
    if (mIsTopLevel) {
        MOZ_ASSERT(size.width > 0 && size.height > 0,
                   "Can't resize window smaller than 1x1.");
        gtk_window_resize(GTK_WINDOW(mShell), size.width, size.height);
    }
    else if (mContainer) {
        GtkWidget *widget = GTK_WIDGET(mContainer);
        GtkAllocation allocation, prev_allocation;
        gtk_widget_get_allocation(widget, &prev_allocation);
        allocation.x = prev_allocation.x;
        allocation.y = prev_allocation.y;
        allocation.width = size.width;
        allocation.height = size.height;
        gtk_widget_size_allocate(widget, &allocation);
    }
    else if (mGdkWindow) {
        gdk_window_resize(mGdkWindow, size.width, size.height);
    }
=======
  if (mIsTopLevel) {
    MOZ_ASSERT(size.width > 0 && size.height > 0,
               "Can't resize window smaller than 1x1.");
    gtk_window_resize(GTK_WINDOW(mShell), size.width, size.height);
  } else if (mContainer) {
    GtkWidget* widget = GTK_WIDGET(mContainer);
    GtkAllocation allocation, prev_allocation;
    gtk_widget_get_allocation(widget, &prev_allocation);
    allocation.x = prev_allocation.x;
    allocation.y = prev_allocation.y;
    allocation.width = size.width;
    allocation.height = size.height;
    gtk_widget_size_allocate(widget, &allocation);
  } else if (mGdkWindow) {
    gdk_window_resize(mGdkWindow, size.width, size.height);
  }
>>>>>>> upstream-releases

#ifdef MOZ_X11
  // Notify the GtkCompositorWidget of a ClientSizeChange
  // This is different than OnSizeAllocate to catch initial sizing
  if (mCompositorWidgetDelegate) {
    mCompositorWidgetDelegate->NotifyClientSizeChanged(GetClientSize());
  }
#endif

  // Does it need to be shown because bounds were previously insane?
  if (mNeedsShow && mIsShown) {
    NativeShow(true);
  }
}

void nsWindow::NativeMoveResize() {
  if (!AreBoundsSane()) {
    // If someone has set this so that the needs show flag is false
    // and it needs to be hidden, update the flag and hide the
    // window.  This flag will be cleared the next time someone
    // hides the window or shows it.  It also prevents us from
    // calling NativeShow(false) excessively on the window which
    // causes unneeded X traffic.
    if (!mNeedsShow && mIsShown) {
      mNeedsShow = true;
      NativeShow(false);
    }
    NativeMove();

<<<<<<< HEAD
    return;
  }
||||||| merged common ancestors
    GdkRectangle size = DevicePixelsToGdkSizeRoundUp(mBounds.Size());
    GdkPoint topLeft = DevicePixelsToGdkPointRoundDown(mBounds.TopLeft());
=======
    return;
  }

  GdkRectangle size = DevicePixelsToGdkSizeRoundUp(mBounds.Size());
  GdkPoint topLeft = DevicePixelsToGdkPointRoundDown(mBounds.TopLeft());
>>>>>>> upstream-releases

<<<<<<< HEAD
  GdkRectangle size = DevicePixelsToGdkSizeRoundUp(mBounds.Size());
  GdkPoint topLeft = DevicePixelsToGdkPointRoundDown(mBounds.TopLeft());
||||||| merged common ancestors
    LOG(("nsWindow::NativeMoveResize [%p] %d %d %d %d\n", (void *)this,
         topLeft.x, topLeft.y, size.width, size.height));
=======
  LOG(("nsWindow::NativeMoveResize [%p] %d %d %d %d\n", (void*)this, topLeft.x,
       topLeft.y, size.width, size.height));
>>>>>>> upstream-releases

<<<<<<< HEAD
  LOG(("nsWindow::NativeMoveResize [%p] %d %d %d %d\n", (void *)this, topLeft.x,
       topLeft.y, size.width, size.height));

  if (mIsTopLevel) {
    // x and y give the position of the window manager frame top-left.
    gtk_window_move(GTK_WINDOW(mShell), topLeft.x, topLeft.y);
    // This sets the client window size.
    MOZ_ASSERT(size.width > 0 && size.height > 0,
               "Can't resize window smaller than 1x1.");
    gtk_window_resize(GTK_WINDOW(mShell), size.width, size.height);
  } else if (mContainer) {
    GtkAllocation allocation;
    allocation.x = topLeft.x;
    allocation.y = topLeft.y;
    allocation.width = size.width;
    allocation.height = size.height;
    gtk_widget_size_allocate(GTK_WIDGET(mContainer), &allocation);
  } else if (mGdkWindow) {
    gdk_window_move_resize(mGdkWindow, topLeft.x, topLeft.y, size.width,
                           size.height);
  }
||||||| merged common ancestors
    if (mIsTopLevel) {
        // x and y give the position of the window manager frame top-left.
        gtk_window_move(GTK_WINDOW(mShell), topLeft.x, topLeft.y);
        // This sets the client window size.
        MOZ_ASSERT(size.width > 0 && size.height > 0,
                   "Can't resize window smaller than 1x1.");
        gtk_window_resize(GTK_WINDOW(mShell), size.width, size.height);
    }
    else if (mContainer) {
        GtkAllocation allocation;
        allocation.x = topLeft.x;
        allocation.y = topLeft.y;
        allocation.width = size.width;
        allocation.height = size.height;
        gtk_widget_size_allocate(GTK_WIDGET(mContainer), &allocation);
    }
    else if (mGdkWindow) {
        gdk_window_move_resize(mGdkWindow,
                               topLeft.x, topLeft.y, size.width, size.height);
    }
=======
  if (IsWaylandPopup()) {
    NativeMoveResizeWaylandPopup(&topLeft, &size);
  } else {
    if (mIsTopLevel) {
      // x and y give the position of the window manager frame top-left.
      gtk_window_move(GTK_WINDOW(mShell), topLeft.x, topLeft.y);
      // This sets the client window size.
      MOZ_ASSERT(size.width > 0 && size.height > 0,
                 "Can't resize window smaller than 1x1.");
      gtk_window_resize(GTK_WINDOW(mShell), size.width, size.height);
    } else if (mContainer) {
      GtkAllocation allocation;
      allocation.x = topLeft.x;
      allocation.y = topLeft.y;
      allocation.width = size.width;
      allocation.height = size.height;
      gtk_widget_size_allocate(GTK_WIDGET(mContainer), &allocation);
    } else if (mGdkWindow) {
      gdk_window_move_resize(mGdkWindow, topLeft.x, topLeft.y, size.width,
                             size.height);
    }
  }
>>>>>>> upstream-releases

#ifdef MOZ_X11
  // Notify the GtkCompositorWidget of a ClientSizeChange
  // This is different than OnSizeAllocate to catch initial sizing
  if (mCompositorWidgetDelegate) {
    mCompositorWidgetDelegate->NotifyClientSizeChanged(GetClientSize());
  }
#endif

  // Does it need to be shown because bounds were previously insane?
  if (mNeedsShow && mIsShown) {
    NativeShow(true);
  }
}

<<<<<<< HEAD
void nsWindow::NativeShow(bool aAction) {
  if (aAction) {
    // unset our flag now that our window has been shown
    mNeedsShow = false;

    if (mIsTopLevel) {
      // Set up usertime/startupID metadata for the created window.
      if (mWindowType != eWindowType_invisible) {
        SetUserTimeAndStartupIDForActivatedWindow(mShell);
      }

      gtk_widget_show(mShell);
    } else if (mContainer) {
      gtk_widget_show(GTK_WIDGET(mContainer));
    } else if (mGdkWindow) {
      gdk_window_show_unraised(mGdkWindow);
    }
  } else {
||||||| merged common ancestors
void
nsWindow::NativeShow(bool aAction)
{
    if (aAction) {
        // unset our flag now that our window has been shown
        mNeedsShow = false;

        if (mIsTopLevel) {
            // Set up usertime/startupID metadata for the created window.
            if (mWindowType != eWindowType_invisible) {
                SetUserTimeAndStartupIDForActivatedWindow(mShell);
            }

            gtk_widget_show(mShell);
        }
        else if (mContainer) {
            gtk_widget_show(GTK_WIDGET(mContainer));
        }
        else if (mGdkWindow) {
            gdk_window_show_unraised(mGdkWindow);
        }
    }
    else {
=======
void nsWindow::HideWaylandWindow() {
>>>>>>> upstream-releases
#ifdef MOZ_WAYLAND
<<<<<<< HEAD
    if (mContainer && moz_container_has_wl_egl_window(mContainer)) {
      // Because wl_egl_window is destroyed on moz_container_unmap(),
      // the current compositor cannot use it anymore. To avoid crash,
      // destroy the compositor & recreate a new compositor on next
      // expose event.
      DestroyLayerManager();
    }
||||||| merged common ancestors
        if (mContainer && moz_container_has_wl_egl_window(mContainer)) {
            // Because wl_egl_window is destroyed on moz_container_unmap(),
            // the current compositor cannot use it anymore. To avoid crash,
            // destroy the compositor & recreate a new compositor on next
            // expose event.
            DestroyLayerManager();
        }
=======
  if (mContainer && moz_container_has_wl_egl_window(mContainer)) {
    // Because wl_egl_window is destroyed on moz_container_unmap(),
    // the current compositor cannot use it anymore. To avoid crash,
    // destroy the compositor & recreate a new compositor on next
    // expose event.
    DestroyLayerManager();
  }
>>>>>>> upstream-releases
#endif
  gtk_widget_hide(mShell);
}

<<<<<<< HEAD
    if (mIsTopLevel) {
      // Workaround window freezes on GTK versions before 3.21.2 by
      // ensuring that configure events get dispatched to windows before
      // they are unmapped. See bug 1225044.
      if (gtk_check_version(3, 21, 2) != nullptr && mPendingConfigures > 0) {
        GtkAllocation allocation;
        gtk_widget_get_allocation(GTK_WIDGET(mShell), &allocation);
||||||| merged common ancestors
        if (mIsTopLevel) {
            // Workaround window freezes on GTK versions before 3.21.2 by
            // ensuring that configure events get dispatched to windows before
            // they are unmapped. See bug 1225044.
            if (gtk_check_version(3, 21, 2) != nullptr && mPendingConfigures > 0) {
                GtkAllocation allocation;
                gtk_widget_get_allocation(GTK_WIDGET(mShell), &allocation);

                GdkEventConfigure event;
                PodZero(&event);
                event.type = GDK_CONFIGURE;
                event.window = mGdkWindow;
                event.send_event = TRUE;
                event.x = allocation.x;
                event.y = allocation.y;
                event.width = allocation.width;
                event.height = allocation.height;

                auto shellClass = GTK_WIDGET_GET_CLASS(mShell);
                for (unsigned int i = 0; i < mPendingConfigures; i++) {
                    Unused << shellClass->configure_event(mShell, &event);
                }
                mPendingConfigures = 0;
            }

            gtk_widget_hide(mShell);
=======
void nsWindow::NativeShow(bool aAction) {
  if (aAction) {
    // unset our flag now that our window has been shown
    mNeedsShow = false;

    if (mIsTopLevel) {
      // Set up usertime/startupID metadata for the created window.
      if (mWindowType != eWindowType_invisible) {
        SetUserTimeAndStartupIDForActivatedWindow(mShell);
      }
      // Update popup window hierarchy run-time on Wayland.
      if (IsWaylandPopup()) {
        ConfigureWaylandPopupWindows();
      }
      gtk_widget_show(mShell);
    } else if (mContainer) {
      gtk_widget_show(GTK_WIDGET(mContainer));
    } else if (mGdkWindow) {
      gdk_window_show_unraised(mGdkWindow);
    }
  } else {
    if (!mIsX11Display) {
      if (IsWaylandPopup()) {
        HideWaylandPopupAndAllChildren();
      } else {
        HideWaylandWindow();
      }
    } else if (mIsTopLevel) {
      // Workaround window freezes on GTK versions before 3.21.2 by
      // ensuring that configure events get dispatched to windows before
      // they are unmapped. See bug 1225044.
      if (gtk_check_version(3, 21, 2) != nullptr && mPendingConfigures > 0) {
        GtkAllocation allocation;
        gtk_widget_get_allocation(GTK_WIDGET(mShell), &allocation);
>>>>>>> upstream-releases

        GdkEventConfigure event;
        PodZero(&event);
        event.type = GDK_CONFIGURE;
        event.window = mGdkWindow;
        event.send_event = TRUE;
        event.x = allocation.x;
        event.y = allocation.y;
        event.width = allocation.width;
        event.height = allocation.height;

        auto shellClass = GTK_WIDGET_GET_CLASS(mShell);
        for (unsigned int i = 0; i < mPendingConfigures; i++) {
          Unused << shellClass->configure_event(mShell, &event);
        }
<<<<<<< HEAD
        mPendingConfigures = 0;
      }
||||||| merged common ancestors
    }
}
=======
        mPendingConfigures = 0;
      }
      gtk_widget_hide(mShell);

      ClearTransparencyBitmap();  // Release some resources
    } else if (mContainer) {
      gtk_widget_hide(GTK_WIDGET(mContainer));
    } else if (mGdkWindow) {
      gdk_window_hide(mGdkWindow);
    }
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
      gtk_widget_hide(mShell);

      ClearTransparencyBitmap();  // Release some resources
    } else if (mContainer) {
      gtk_widget_hide(GTK_WIDGET(mContainer));
    } else if (mGdkWindow) {
      gdk_window_hide(mGdkWindow);
    }
  }
}
||||||| merged common ancestors
void
nsWindow::SetHasMappedToplevel(bool aState)
{
    // Even when aState == mHasMappedToplevel (as when this method is called
    // from Show()), child windows need to have their state checked, so don't
    // return early.
    bool oldState = mHasMappedToplevel;
    mHasMappedToplevel = aState;

    // mHasMappedToplevel is not updated for children of windows that are
    // hidden; GDK knows not to send expose events for these windows.  The
    // state is recorded on the hidden window itself, but, for child trees of
    // hidden windows, their state essentially becomes disconnected from their
    // hidden parent.  When the hidden parent gets shown, the child trees are
    // reconnected, and the state of the window being shown can be easily
    // propagated.
    if (!mIsShown || !mGdkWindow)
        return;

    if (aState && !oldState && !mIsFullyObscured) {
        // GDK_EXPOSE events have been ignored but the window is now visible,
        // so make sure GDK doesn't think that the window has already been
        // painted.
        gdk_window_invalidate_rect(mGdkWindow, nullptr, FALSE);

        // Check that a grab didn't fail due to the window not being
        // viewable.
        EnsureGrabs();
    }
=======
void nsWindow::SetHasMappedToplevel(bool aState) {
  // Even when aState == mHasMappedToplevel (as when this method is called
  // from Show()), child windows need to have their state checked, so don't
  // return early.
  bool oldState = mHasMappedToplevel;
  mHasMappedToplevel = aState;

  // mHasMappedToplevel is not updated for children of windows that are
  // hidden; GDK knows not to send expose events for these windows.  The
  // state is recorded on the hidden window itself, but, for child trees of
  // hidden windows, their state essentially becomes disconnected from their
  // hidden parent.  When the hidden parent gets shown, the child trees are
  // reconnected, and the state of the window being shown can be easily
  // propagated.
  if (!mIsShown || !mGdkWindow) return;

  if (aState && !oldState && !mIsFullyObscured) {
    // GDK_EXPOSE events have been ignored but the window is now visible,
    // so make sure GDK doesn't think that the window has already been
    // painted.
    gdk_window_invalidate_rect(mGdkWindow, nullptr, FALSE);

    // Check that a grab didn't fail due to the window not being
    // viewable.
    EnsureGrabs();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void nsWindow::SetHasMappedToplevel(bool aState) {
  // Even when aState == mHasMappedToplevel (as when this method is called
  // from Show()), child windows need to have their state checked, so don't
  // return early.
  bool oldState = mHasMappedToplevel;
  mHasMappedToplevel = aState;

  // mHasMappedToplevel is not updated for children of windows that are
  // hidden; GDK knows not to send expose events for these windows.  The
  // state is recorded on the hidden window itself, but, for child trees of
  // hidden windows, their state essentially becomes disconnected from their
  // hidden parent.  When the hidden parent gets shown, the child trees are
  // reconnected, and the state of the window being shown can be easily
  // propagated.
  if (!mIsShown || !mGdkWindow) return;

  if (aState && !oldState && !mIsFullyObscured) {
    // GDK_EXPOSE events have been ignored but the window is now visible,
    // so make sure GDK doesn't think that the window has already been
    // painted.
    gdk_window_invalidate_rect(mGdkWindow, nullptr, FALSE);

    // Check that a grab didn't fail due to the window not being
    // viewable.
    EnsureGrabs();
  }
||||||| merged common ancestors
    for (GList *children = gdk_window_peek_children(mGdkWindow);
         children;
         children = children->next) {
        GdkWindow *gdkWin = GDK_WINDOW(children->data);
        nsWindow *child = get_window_for_gdk_window(gdkWin);
=======
  for (GList* children = gdk_window_peek_children(mGdkWindow); children;
       children = children->next) {
    GdkWindow* gdkWin = GDK_WINDOW(children->data);
    nsWindow* child = get_window_for_gdk_window(gdkWin);
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (GList *children = gdk_window_peek_children(mGdkWindow); children;
       children = children->next) {
    GdkWindow *gdkWin = GDK_WINDOW(children->data);
    nsWindow *child = get_window_for_gdk_window(gdkWin);

    if (child && child->mHasMappedToplevel != aState) {
      child->SetHasMappedToplevel(aState);
||||||| merged common ancestors
        if (child && child->mHasMappedToplevel != aState) {
            child->SetHasMappedToplevel(aState);
        }
=======
    if (child && child->mHasMappedToplevel != aState) {
      child->SetHasMappedToplevel(aState);
>>>>>>> upstream-releases
    }
  }
}

<<<<<<< HEAD
LayoutDeviceIntSize nsWindow::GetSafeWindowSize(LayoutDeviceIntSize aSize) {
  // The X protocol uses CARD32 for window sizes, but the server (1.11.3)
  // reads it as CARD16.  Sizes of pixmaps, used for drawing, are (unsigned)
  // CARD16 in the protocol, but the server's ProcCreatePixmap returns
  // BadAlloc if dimensions cannot be represented by signed shorts.
  LayoutDeviceIntSize result = aSize;
  const int32_t kInt16Max = 32767;
  if (result.width > kInt16Max) {
    result.width = kInt16Max;
  }
  if (result.height > kInt16Max) {
    result.height = kInt16Max;
  }
  return result;
||||||| merged common ancestors
LayoutDeviceIntSize
nsWindow::GetSafeWindowSize(LayoutDeviceIntSize aSize)
{
    // The X protocol uses CARD32 for window sizes, but the server (1.11.3)
    // reads it as CARD16.  Sizes of pixmaps, used for drawing, are (unsigned)
    // CARD16 in the protocol, but the server's ProcCreatePixmap returns
    // BadAlloc if dimensions cannot be represented by signed shorts.
    LayoutDeviceIntSize result = aSize;
    const int32_t kInt16Max = 32767;
    if (result.width > kInt16Max) {
        result.width = kInt16Max;
    }
    if (result.height > kInt16Max) {
        result.height = kInt16Max;
    }
    return result;
=======
LayoutDeviceIntSize nsWindow::GetSafeWindowSize(LayoutDeviceIntSize aSize) {
  // The X protocol uses CARD32 for window sizes, but the server (1.11.3)
  // reads it as CARD16.  Sizes of pixmaps, used for drawing, are (unsigned)
  // CARD16 in the protocol, but the server's ProcCreatePixmap returns
  // BadAlloc if dimensions cannot be represented by signed shorts.
  // Because we are creating Cairo surfaces to represent window buffers,
  // we also must ensure that the window can fit in a Cairo surface.
  LayoutDeviceIntSize result = aSize;
  int32_t maxSize = 32767;
  if (mLayerManager && mLayerManager->AsKnowsCompositor()) {
    maxSize = std::min(maxSize,
                       mLayerManager->AsKnowsCompositor()->GetMaxTextureSize());
  }
  if (result.width > maxSize) {
    result.width = maxSize;
  }
  if (result.height > maxSize) {
    result.height = maxSize;
  }
  return result;
>>>>>>> upstream-releases
}

void nsWindow::EnsureGrabs(void) {
  if (mRetryPointerGrab) GrabPointer(sRetryGrabTime);
}

void nsWindow::CleanLayerManagerRecursive(void) {
  if (mLayerManager) {
    mLayerManager->Destroy();
    mLayerManager = nullptr;
  }

  DestroyCompositor();

<<<<<<< HEAD
  GList *children = gdk_window_peek_children(mGdkWindow);
  for (GList *list = children; list; list = list->next) {
    nsWindow *window = get_window_for_gdk_window(GDK_WINDOW(list->data));
    if (window) {
      window->CleanLayerManagerRecursive();
||||||| merged common ancestors
    GList* children = gdk_window_peek_children(mGdkWindow);
    for (GList* list = children; list; list = list->next) {
        nsWindow* window = get_window_for_gdk_window(GDK_WINDOW(list->data));
        if (window) {
            window->CleanLayerManagerRecursive();
        }
=======
  GList* children = gdk_window_peek_children(mGdkWindow);
  for (GList* list = children; list; list = list->next) {
    nsWindow* window = get_window_for_gdk_window(GDK_WINDOW(list->data));
    if (window) {
      window->CleanLayerManagerRecursive();
>>>>>>> upstream-releases
    }
  }
}

<<<<<<< HEAD
void nsWindow::SetTransparencyMode(nsTransparencyMode aMode) {
  if (!mShell) {
    // Pass the request to the toplevel window
    GtkWidget *topWidget = GetToplevelWidget();
    if (!topWidget) return;
||||||| merged common ancestors
void
nsWindow::SetTransparencyMode(nsTransparencyMode aMode)
{
    if (!mShell) {
        // Pass the request to the toplevel window
        GtkWidget *topWidget = GetToplevelWidget();
        if (!topWidget)
            return;

        nsWindow *topWindow = get_window_for_gtk_widget(topWidget);
        if (!topWindow)
            return;

        topWindow->SetTransparencyMode(aMode);
        return;
    }
=======
void nsWindow::SetTransparencyMode(nsTransparencyMode aMode) {
  if (!mShell) {
    // Pass the request to the toplevel window
    GtkWidget* topWidget = GetToplevelWidget();
    if (!topWidget) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
    nsWindow *topWindow = get_window_for_gtk_widget(topWidget);
    if (!topWindow) return;
||||||| merged common ancestors
    bool isTransparent = aMode == eTransparencyTransparent;
=======
    nsWindow* topWindow = get_window_for_gtk_widget(topWidget);
    if (!topWindow) return;
>>>>>>> upstream-releases

    topWindow->SetTransparencyMode(aMode);
    return;
  }

  bool isTransparent = aMode == eTransparencyTransparent;

  if (mIsTransparent == isTransparent) {
    return;
  } else if (mWindowType != eWindowType_popup) {
    NS_WARNING("Cannot set transparency mode on non-popup windows.");
    return;
  }

  if (!isTransparent) {
    ClearTransparencyBitmap();
  }  // else the new default alpha values are "all 1", so we don't
  // need to change anything yet

  mIsTransparent = isTransparent;

<<<<<<< HEAD
  // Need to clean our LayerManager up while still alive because
  // we don't want to use layers acceleration on shaped windows
  CleanLayerManagerRecursive();
}

nsTransparencyMode nsWindow::GetTransparencyMode() {
  if (!mShell) {
    // Pass the request to the toplevel window
    GtkWidget *topWidget = GetToplevelWidget();
    if (!topWidget) {
      return eTransparencyOpaque;
    }
||||||| merged common ancestors
        nsWindow *topWindow = get_window_for_gtk_widget(topWidget);
        if (!topWindow) {
            return eTransparencyOpaque;
        }
=======
  // Need to clean our LayerManager up while still alive because
  // we don't want to use layers acceleration on shaped windows
  CleanLayerManagerRecursive();
}

nsTransparencyMode nsWindow::GetTransparencyMode() {
  if (!mShell) {
    // Pass the request to the toplevel window
    GtkWidget* topWidget = GetToplevelWidget();
    if (!topWidget) {
      return eTransparencyOpaque;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    nsWindow *topWindow = get_window_for_gtk_widget(topWidget);
    if (!topWindow) {
      return eTransparencyOpaque;
||||||| merged common ancestors
        return topWindow->GetTransparencyMode();
=======
    nsWindow* topWindow = get_window_for_gtk_widget(topWidget);
    if (!topWindow) {
      return eTransparencyOpaque;
>>>>>>> upstream-releases
    }

    return topWindow->GetTransparencyMode();
  }

  return mIsTransparent ? eTransparencyTransparent : eTransparencyOpaque;
}

// For setting the draggable titlebar region from CSS
// with -moz-window-dragging: drag.
<<<<<<< HEAD
void nsWindow::UpdateWindowDraggingRegion(
    const LayoutDeviceIntRegion &aRegion) {
||||||| merged common ancestors
void
nsWindow::UpdateWindowDraggingRegion(const LayoutDeviceIntRegion& aRegion)
{
=======
void nsWindow::UpdateWindowDraggingRegion(
    const LayoutDeviceIntRegion& aRegion) {
>>>>>>> upstream-releases
  if (mDraggableRegion != aRegion) {
    mDraggableRegion = aRegion;
  }
}

<<<<<<< HEAD
void nsWindow::UpdateOpaqueRegion(const LayoutDeviceIntRegion &aOpaqueRegion) {
  // Available as of GTK 3.10+
  static auto sGdkWindowSetOpaqueRegion =
      (void (*)(GdkWindow *, cairo_region_t *))dlsym(
          RTLD_DEFAULT, "gdk_window_set_opaque_region");
||||||| merged common ancestors
#if (MOZ_WIDGET_GTK >= 3)
void nsWindow::UpdateOpaqueRegion(const LayoutDeviceIntRegion& aOpaqueRegion)
{
    // Available as of GTK 3.10+
    static auto sGdkWindowSetOpaqueRegion =
        (void (*)(GdkWindow*, cairo_region_t*))
            dlsym(RTLD_DEFAULT, "gdk_window_set_opaque_region");

    if (sGdkWindowSetOpaqueRegion && mGdkWindow &&
        gdk_window_get_window_type(mGdkWindow) == GDK_WINDOW_TOPLEVEL) {
        if (aOpaqueRegion.IsEmpty()) {
            (*sGdkWindowSetOpaqueRegion)(mGdkWindow, nullptr);
        } else {
            cairo_region_t *region = cairo_region_create();
            for (auto iter = aOpaqueRegion.RectIter(); !iter.Done();
                 iter.Next()) {
                const LayoutDeviceIntRect &r = iter.Get();
                cairo_rectangle_int_t rect = { r.x, r.y, r.width, r.height };
                cairo_region_union_rectangle(region, &rect);
            }
            (*sGdkWindowSetOpaqueRegion)(mGdkWindow, region);
            cairo_region_destroy(region);
        }
    }
}
#endif
=======
void nsWindow::UpdateOpaqueRegion(const LayoutDeviceIntRegion& aOpaqueRegion) {
  // Available as of GTK 3.10+
  static auto sGdkWindowSetOpaqueRegion =
      (void (*)(GdkWindow*, cairo_region_t*))dlsym(
          RTLD_DEFAULT, "gdk_window_set_opaque_region");
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (sGdkWindowSetOpaqueRegion && mGdkWindow &&
      gdk_window_get_window_type(mGdkWindow) == GDK_WINDOW_TOPLEVEL) {
    if (aOpaqueRegion.IsEmpty()) {
      (*sGdkWindowSetOpaqueRegion)(mGdkWindow, nullptr);
    } else {
      cairo_region_t *region = cairo_region_create();
      for (auto iter = aOpaqueRegion.RectIter(); !iter.Done(); iter.Next()) {
        const LayoutDeviceIntRect &r = iter.Get();
        cairo_rectangle_int_t rect = {r.x, r.y, r.width, r.height};
        cairo_region_union_rectangle(region, &rect);
      }
      (*sGdkWindowSetOpaqueRegion)(mGdkWindow, region);
      cairo_region_destroy(region);
||||||| merged common ancestors
nsresult
nsWindow::ConfigureChildren(const nsTArray<Configuration>& aConfigurations)
{
    // If this is a remotely updated widget we receive clipping, position, and
    // size information from a source other than our owner. Don't let our parent
    // update this information.
    if (mWindowType == eWindowType_plugin_ipc_chrome) {
      return NS_OK;
=======
  if (sGdkWindowSetOpaqueRegion && mGdkWindow &&
      gdk_window_get_window_type(mGdkWindow) == GDK_WINDOW_TOPLEVEL) {
    if (aOpaqueRegion.IsEmpty()) {
      (*sGdkWindowSetOpaqueRegion)(mGdkWindow, nullptr);
    } else {
      cairo_region_t* region = cairo_region_create();
      for (auto iter = aOpaqueRegion.RectIter(); !iter.Done(); iter.Next()) {
        const LayoutDeviceIntRect& r = iter.Get();
        cairo_rectangle_int_t rect = {r.x, r.y, r.width, r.height};
        cairo_region_union_rectangle(region, &rect);
      }
      (*sGdkWindowSetOpaqueRegion)(mGdkWindow, region);
      cairo_region_destroy(region);
>>>>>>> upstream-releases
    }
  }
}

<<<<<<< HEAD
nsresult nsWindow::ConfigureChildren(
    const nsTArray<Configuration> &aConfigurations) {
  // If this is a remotely updated widget we receive clipping, position, and
  // size information from a source other than our owner. Don't let our parent
  // update this information.
  if (mWindowType == eWindowType_plugin_ipc_chrome) {
||||||| merged common ancestors
    for (uint32_t i = 0; i < aConfigurations.Length(); ++i) {
        const Configuration& configuration = aConfigurations[i];
        auto* w = static_cast<nsWindow*>(configuration.mChild.get());
        NS_ASSERTION(w->GetParent() == this,
                     "Configured widget is not a child");
        w->SetWindowClipRegion(configuration.mClipRegion, true);
        if (w->mBounds.Size() != configuration.mBounds.Size()) {
            w->Resize(configuration.mBounds.x, configuration.mBounds.y,
                      configuration.mBounds.width, configuration.mBounds.height,
                      true);
        } else if (w->mBounds.TopLeft() != configuration.mBounds.TopLeft()) {
            w->Move(configuration.mBounds.x, configuration.mBounds.y);
        }
        w->SetWindowClipRegion(configuration.mClipRegion, false);
    }
=======
nsresult nsWindow::ConfigureChildren(
    const nsTArray<Configuration>& aConfigurations) {
  // If this is a remotely updated widget we receive clipping, position, and
  // size information from a source other than our owner. Don't let our parent
  // update this information.
  if (mWindowType == eWindowType_plugin_ipc_chrome) {
>>>>>>> upstream-releases
    return NS_OK;
<<<<<<< HEAD
  }

  for (uint32_t i = 0; i < aConfigurations.Length(); ++i) {
    const Configuration &configuration = aConfigurations[i];
    auto *w = static_cast<nsWindow *>(configuration.mChild.get());
    NS_ASSERTION(w->GetParent() == this, "Configured widget is not a child");
    w->SetWindowClipRegion(configuration.mClipRegion, true);
    if (w->mBounds.Size() != configuration.mBounds.Size()) {
      w->Resize(configuration.mBounds.x, configuration.mBounds.y,
                configuration.mBounds.width, configuration.mBounds.height,
                true);
    } else if (w->mBounds.TopLeft() != configuration.mBounds.TopLeft()) {
      w->Move(configuration.mBounds.x, configuration.mBounds.y);
    }
    w->SetWindowClipRegion(configuration.mClipRegion, false);
  }
  return NS_OK;
||||||| merged common ancestors
=======
  }

  for (uint32_t i = 0; i < aConfigurations.Length(); ++i) {
    const Configuration& configuration = aConfigurations[i];
    auto* w = static_cast<nsWindow*>(configuration.mChild.get());
    NS_ASSERTION(w->GetParent() == this, "Configured widget is not a child");
    w->SetWindowClipRegion(configuration.mClipRegion, true);
    if (w->mBounds.Size() != configuration.mBounds.Size()) {
      w->Resize(configuration.mBounds.x, configuration.mBounds.y,
                configuration.mBounds.width, configuration.mBounds.height,
                true);
    } else if (w->mBounds.TopLeft() != configuration.mBounds.TopLeft()) {
      w->Move(configuration.mBounds.x, configuration.mBounds.y);
    }
    w->SetWindowClipRegion(configuration.mClipRegion, false);
  }
  return NS_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsWindow::SetWindowClipRegion(
    const nsTArray<LayoutDeviceIntRect> &aRects, bool aIntersectWithExisting) {
  const nsTArray<LayoutDeviceIntRect> *newRects = &aRects;
||||||| merged common ancestors
nsresult
nsWindow::SetWindowClipRegion(const nsTArray<LayoutDeviceIntRect>& aRects,
                              bool aIntersectWithExisting)
{
    const nsTArray<LayoutDeviceIntRect>* newRects = &aRects;

    AutoTArray<LayoutDeviceIntRect,1> intersectRects;
    if (aIntersectWithExisting) {
        AutoTArray<LayoutDeviceIntRect,1> existingRects;
        GetWindowClipRegion(&existingRects);

        LayoutDeviceIntRegion existingRegion = RegionFromArray(existingRects);
        LayoutDeviceIntRegion newRegion = RegionFromArray(aRects);
        LayoutDeviceIntRegion intersectRegion;
        intersectRegion.And(newRegion, existingRegion);

        // If mClipRects is null we haven't set a clip rect yet, so we
        // need to set the clip even if it is equal.
        if (mClipRects && intersectRegion.IsEqual(existingRegion)) {
            return NS_OK;
        }
=======
nsresult nsWindow::SetWindowClipRegion(
    const nsTArray<LayoutDeviceIntRect>& aRects, bool aIntersectWithExisting) {
  const nsTArray<LayoutDeviceIntRect>* newRects = &aRects;
>>>>>>> upstream-releases

  AutoTArray<LayoutDeviceIntRect, 1> intersectRects;
  if (aIntersectWithExisting) {
    AutoTArray<LayoutDeviceIntRect, 1> existingRects;
    GetWindowClipRegion(&existingRects);

    LayoutDeviceIntRegion existingRegion = RegionFromArray(existingRects);
    LayoutDeviceIntRegion newRegion = RegionFromArray(aRects);
    LayoutDeviceIntRegion intersectRegion;
    intersectRegion.And(newRegion, existingRegion);

    // If mClipRects is null we haven't set a clip rect yet, so we
    // need to set the clip even if it is equal.
    if (mClipRects && intersectRegion.IsEqual(existingRegion)) {
      return NS_OK;
    }

    if (!intersectRegion.IsEqual(newRegion)) {
      ArrayFromRegion(intersectRegion, intersectRects);
      newRects = &intersectRects;
    }
  }

  if (IsWindowClipRegionEqual(*newRects)) return NS_OK;

  StoreWindowClipRegion(*newRects);

  if (!mGdkWindow) return NS_OK;

<<<<<<< HEAD
  cairo_region_t *region = cairo_region_create();
  for (uint32_t i = 0; i < newRects->Length(); ++i) {
    const LayoutDeviceIntRect &r = newRects->ElementAt(i);
    cairo_rectangle_int_t rect = {r.x, r.y, r.width, r.height};
    cairo_region_union_rectangle(region, &rect);
  }
||||||| merged common ancestors
    gdk_window_shape_combine_region(mGdkWindow, region, 0, 0);
    cairo_region_destroy(region);
=======
  cairo_region_t* region = cairo_region_create();
  for (uint32_t i = 0; i < newRects->Length(); ++i) {
    const LayoutDeviceIntRect& r = newRects->ElementAt(i);
    cairo_rectangle_int_t rect = {r.x, r.y, r.width, r.height};
    cairo_region_union_rectangle(region, &rect);
  }
>>>>>>> upstream-releases

  gdk_window_shape_combine_region(mGdkWindow, region, 0, 0);
  cairo_region_destroy(region);

  return NS_OK;
}

void nsWindow::ResizeTransparencyBitmap() {
  if (!mTransparencyBitmap) return;

<<<<<<< HEAD
  if (mBounds.width == mTransparencyBitmapWidth &&
      mBounds.height == mTransparencyBitmapHeight)
    return;

  int32_t newRowBytes = GetBitmapStride(mBounds.width);
  int32_t newSize = newRowBytes * mBounds.height;
  auto *newBits = new gchar[newSize];
  // fill new mask with "transparent", first
  memset(newBits, 0, newSize);

  // Now copy the intersection of the old and new areas into the new mask
  int32_t copyWidth = std::min(mBounds.width, mTransparencyBitmapWidth);
  int32_t copyHeight = std::min(mBounds.height, mTransparencyBitmapHeight);
  int32_t oldRowBytes = GetBitmapStride(mTransparencyBitmapWidth);
  int32_t copyBytes = GetBitmapStride(copyWidth);

  int32_t i;
  gchar *fromPtr = mTransparencyBitmap;
  gchar *toPtr = newBits;
  for (i = 0; i < copyHeight; i++) {
    memcpy(toPtr, fromPtr, copyBytes);
    fromPtr += oldRowBytes;
    toPtr += newRowBytes;
  }

  delete[] mTransparencyBitmap;
  mTransparencyBitmap = newBits;
  mTransparencyBitmapWidth = mBounds.width;
  mTransparencyBitmapHeight = mBounds.height;
||||||| merged common ancestors
    delete[] mTransparencyBitmap;
    mTransparencyBitmap = newBits;
    mTransparencyBitmapWidth = mBounds.width;
    mTransparencyBitmapHeight = mBounds.height;
=======
  if (mBounds.width == mTransparencyBitmapWidth &&
      mBounds.height == mTransparencyBitmapHeight)
    return;

  int32_t newRowBytes = GetBitmapStride(mBounds.width);
  int32_t newSize = newRowBytes * mBounds.height;
  auto* newBits = new gchar[newSize];
  // fill new mask with "transparent", first
  memset(newBits, 0, newSize);

  // Now copy the intersection of the old and new areas into the new mask
  int32_t copyWidth = std::min(mBounds.width, mTransparencyBitmapWidth);
  int32_t copyHeight = std::min(mBounds.height, mTransparencyBitmapHeight);
  int32_t oldRowBytes = GetBitmapStride(mTransparencyBitmapWidth);
  int32_t copyBytes = GetBitmapStride(copyWidth);

  int32_t i;
  gchar* fromPtr = mTransparencyBitmap;
  gchar* toPtr = newBits;
  for (i = 0; i < copyHeight; i++) {
    memcpy(toPtr, fromPtr, copyBytes);
    fromPtr += oldRowBytes;
    toPtr += newRowBytes;
  }

  delete[] mTransparencyBitmap;
  mTransparencyBitmap = newBits;
  mTransparencyBitmapWidth = mBounds.width;
  mTransparencyBitmapHeight = mBounds.height;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool ChangedMaskBits(gchar *aMaskBits, int32_t aMaskWidth,
                            int32_t aMaskHeight, const nsIntRect &aRect,
                            uint8_t *aAlphas, int32_t aStride) {
  int32_t x, y, xMax = aRect.XMost(), yMax = aRect.YMost();
  int32_t maskBytesPerRow = GetBitmapStride(aMaskWidth);
  for (y = aRect.y; y < yMax; y++) {
    gchar *maskBytes = aMaskBits + y * maskBytesPerRow;
    uint8_t *alphas = aAlphas;
    for (x = aRect.x; x < xMax; x++) {
      bool newBit = *alphas > 0x7f;
      alphas++;

      gchar maskByte = maskBytes[x >> 3];
      bool maskBit = (maskByte & (1 << (x & 7))) != 0;

      if (maskBit != newBit) {
        return true;
      }
||||||| merged common ancestors
static bool
ChangedMaskBits(gchar* aMaskBits, int32_t aMaskWidth, int32_t aMaskHeight,
        const nsIntRect& aRect, uint8_t* aAlphas, int32_t aStride)
{
    int32_t x, y, xMax = aRect.XMost(), yMax = aRect.YMost();
    int32_t maskBytesPerRow = GetBitmapStride(aMaskWidth);
    for (y = aRect.y; y < yMax; y++) {
        gchar* maskBytes = aMaskBits + y*maskBytesPerRow;
        uint8_t* alphas = aAlphas;
        for (x = aRect.x; x < xMax; x++) {
            bool newBit = *alphas > 0x7f;
            alphas++;

            gchar maskByte = maskBytes[x >> 3];
            bool maskBit = (maskByte & (1 << (x & 7))) != 0;

            if (maskBit != newBit) {
                return true;
            }
        }
        aAlphas += aStride;
=======
static bool ChangedMaskBits(gchar* aMaskBits, int32_t aMaskWidth,
                            int32_t aMaskHeight, const nsIntRect& aRect,
                            uint8_t* aAlphas, int32_t aStride) {
  int32_t x, y, xMax = aRect.XMost(), yMax = aRect.YMost();
  int32_t maskBytesPerRow = GetBitmapStride(aMaskWidth);
  for (y = aRect.y; y < yMax; y++) {
    gchar* maskBytes = aMaskBits + y * maskBytesPerRow;
    uint8_t* alphas = aAlphas;
    for (x = aRect.x; x < xMax; x++) {
      bool newBit = *alphas > 0x7f;
      alphas++;

      gchar maskByte = maskBytes[x >> 3];
      bool maskBit = (maskByte & (1 << (x & 7))) != 0;

      if (maskBit != newBit) {
        return true;
      }
>>>>>>> upstream-releases
    }
    aAlphas += aStride;
  }

  return false;
}

<<<<<<< HEAD
static void UpdateMaskBits(gchar *aMaskBits, int32_t aMaskWidth,
                           int32_t aMaskHeight, const nsIntRect &aRect,
                           uint8_t *aAlphas, int32_t aStride) {
  int32_t x, y, xMax = aRect.XMost(), yMax = aRect.YMost();
  int32_t maskBytesPerRow = GetBitmapStride(aMaskWidth);
  for (y = aRect.y; y < yMax; y++) {
    gchar *maskBytes = aMaskBits + y * maskBytesPerRow;
    uint8_t *alphas = aAlphas;
    for (x = aRect.x; x < xMax; x++) {
      bool newBit = *alphas > 0x7f;
      alphas++;

      gchar mask = 1 << (x & 7);
      gchar maskByte = maskBytes[x >> 3];
      // Note: '-newBit' turns 0 into 00...00 and 1 into 11...11
      maskBytes[x >> 3] = (maskByte & ~mask) | (-newBit & mask);
||||||| merged common ancestors
static
void UpdateMaskBits(gchar* aMaskBits, int32_t aMaskWidth, int32_t aMaskHeight,
        const nsIntRect& aRect, uint8_t* aAlphas, int32_t aStride)
{
    int32_t x, y, xMax = aRect.XMost(), yMax = aRect.YMost();
    int32_t maskBytesPerRow = GetBitmapStride(aMaskWidth);
    for (y = aRect.y; y < yMax; y++) {
        gchar* maskBytes = aMaskBits + y*maskBytesPerRow;
        uint8_t* alphas = aAlphas;
        for (x = aRect.x; x < xMax; x++) {
            bool newBit = *alphas > 0x7f;
            alphas++;

            gchar mask = 1 << (x & 7);
            gchar maskByte = maskBytes[x >> 3];
            // Note: '-newBit' turns 0 into 00...00 and 1 into 11...11
            maskBytes[x >> 3] = (maskByte & ~mask) | (-newBit & mask);
        }
        aAlphas += aStride;
=======
static void UpdateMaskBits(gchar* aMaskBits, int32_t aMaskWidth,
                           int32_t aMaskHeight, const nsIntRect& aRect,
                           uint8_t* aAlphas, int32_t aStride) {
  int32_t x, y, xMax = aRect.XMost(), yMax = aRect.YMost();
  int32_t maskBytesPerRow = GetBitmapStride(aMaskWidth);
  for (y = aRect.y; y < yMax; y++) {
    gchar* maskBytes = aMaskBits + y * maskBytesPerRow;
    uint8_t* alphas = aAlphas;
    for (x = aRect.x; x < xMax; x++) {
      bool newBit = *alphas > 0x7f;
      alphas++;

      gchar mask = 1 << (x & 7);
      gchar maskByte = maskBytes[x >> 3];
      // Note: '-newBit' turns 0 into 00...00 and 1 into 11...11
      maskBytes[x >> 3] = (maskByte & ~mask) | (-newBit & mask);
>>>>>>> upstream-releases
    }
    aAlphas += aStride;
  }
}

void nsWindow::ApplyTransparencyBitmap() {
#ifdef MOZ_X11
<<<<<<< HEAD
  // We use X11 calls where possible, because GDK handles expose events
  // for shaped windows in a way that's incompatible with us (Bug 635903).
  // It doesn't occur when the shapes are set through X.
  Display *xDisplay = GDK_WINDOW_XDISPLAY(mGdkWindow);
  Window xDrawable = GDK_WINDOW_XID(mGdkWindow);
  Pixmap maskPixmap = XCreateBitmapFromData(
      xDisplay, xDrawable, mTransparencyBitmap, mTransparencyBitmapWidth,
      mTransparencyBitmapHeight);
  XShapeCombineMask(xDisplay, xDrawable, ShapeBounding, 0, 0, maskPixmap,
                    ShapeSet);
  XFreePixmap(xDisplay, maskPixmap);
||||||| merged common ancestors
    // We use X11 calls where possible, because GDK handles expose events
    // for shaped windows in a way that's incompatible with us (Bug 635903).
    // It doesn't occur when the shapes are set through X.
    Display* xDisplay = GDK_WINDOW_XDISPLAY(mGdkWindow);
    Window xDrawable = GDK_WINDOW_XID(mGdkWindow);
    Pixmap maskPixmap = XCreateBitmapFromData(xDisplay,
                                              xDrawable,
                                              mTransparencyBitmap,
                                              mTransparencyBitmapWidth,
                                              mTransparencyBitmapHeight);
    XShapeCombineMask(xDisplay, xDrawable,
                      ShapeBounding, 0, 0,
                      maskPixmap, ShapeSet);
    XFreePixmap(xDisplay, maskPixmap);
=======
  // We use X11 calls where possible, because GDK handles expose events
  // for shaped windows in a way that's incompatible with us (Bug 635903).
  // It doesn't occur when the shapes are set through X.
  Display* xDisplay = GDK_WINDOW_XDISPLAY(mGdkWindow);
  Window xDrawable = GDK_WINDOW_XID(mGdkWindow);
  Pixmap maskPixmap = XCreateBitmapFromData(
      xDisplay, xDrawable, mTransparencyBitmap, mTransparencyBitmapWidth,
      mTransparencyBitmapHeight);
  XShapeCombineMask(xDisplay, xDrawable, ShapeBounding, 0, 0, maskPixmap,
                    ShapeSet);
  XFreePixmap(xDisplay, maskPixmap);
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
  cairo_surface_t *maskBitmap;
  maskBitmap = cairo_image_surface_create_for_data(
      (unsigned char *)mTransparencyBitmap, CAIRO_FORMAT_A1,
      mTransparencyBitmapWidth, mTransparencyBitmapHeight,
      GetBitmapStride(mTransparencyBitmapWidth));
  if (!maskBitmap) return;
||||||| merged common ancestors
    cairo_surface_t *maskBitmap;
    maskBitmap = cairo_image_surface_create_for_data((unsigned char*)mTransparencyBitmap,
                                                     CAIRO_FORMAT_A1,
                                                     mTransparencyBitmapWidth,
                                                     mTransparencyBitmapHeight,
                                                     GetBitmapStride(mTransparencyBitmapWidth));
    if (!maskBitmap)
        return;

    cairo_region_t * maskRegion = gdk_cairo_region_create_from_surface(maskBitmap);
    gtk_widget_shape_combine_region(mShell, maskRegion);
    cairo_region_destroy(maskRegion);
    cairo_surface_destroy(maskBitmap);
#endif // MOZ_X11
}

void
nsWindow::ClearTransparencyBitmap()
{
    if (!mTransparencyBitmap)
        return;
=======
  cairo_surface_t* maskBitmap;
  maskBitmap = cairo_image_surface_create_for_data(
      (unsigned char*)mTransparencyBitmap, CAIRO_FORMAT_A1,
      mTransparencyBitmapWidth, mTransparencyBitmapHeight,
      GetBitmapStride(mTransparencyBitmapWidth));
  if (!maskBitmap) return;

  cairo_region_t* maskRegion = gdk_cairo_region_create_from_surface(maskBitmap);
  gtk_widget_shape_combine_region(mShell, maskRegion);
  cairo_region_destroy(maskRegion);
  cairo_surface_destroy(maskBitmap);
#endif  // MOZ_X11
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  cairo_region_t *maskRegion = gdk_cairo_region_create_from_surface(maskBitmap);
  gtk_widget_shape_combine_region(mShell, maskRegion);
  cairo_region_destroy(maskRegion);
  cairo_surface_destroy(maskBitmap);
#endif  // MOZ_X11
}
||||||| merged common ancestors
    delete[] mTransparencyBitmap;
    mTransparencyBitmap = nullptr;
    mTransparencyBitmapWidth = 0;
    mTransparencyBitmapHeight = 0;
=======
void nsWindow::ClearTransparencyBitmap() {
  if (!mTransparencyBitmap) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
void nsWindow::ClearTransparencyBitmap() {
  if (!mTransparencyBitmap) return;

  delete[] mTransparencyBitmap;
  mTransparencyBitmap = nullptr;
  mTransparencyBitmapWidth = 0;
  mTransparencyBitmapHeight = 0;

  if (!mShell) return;
||||||| merged common ancestors
    if (!mShell)
        return;
=======
  delete[] mTransparencyBitmap;
  mTransparencyBitmap = nullptr;
  mTransparencyBitmapWidth = 0;
  mTransparencyBitmapHeight = 0;

  if (!mShell) return;
>>>>>>> upstream-releases

#ifdef MOZ_X11
  if (!mGdkWindow) return;

<<<<<<< HEAD
  Display *xDisplay = GDK_WINDOW_XDISPLAY(mGdkWindow);
  Window xWindow = gdk_x11_window_get_xid(mGdkWindow);
||||||| merged common ancestors
    Display* xDisplay = GDK_WINDOW_XDISPLAY(mGdkWindow);
    Window xWindow = gdk_x11_window_get_xid(mGdkWindow);
=======
  Display* xDisplay = GDK_WINDOW_XDISPLAY(mGdkWindow);
  Window xWindow = gdk_x11_window_get_xid(mGdkWindow);
>>>>>>> upstream-releases

  XShapeCombineMask(xDisplay, xWindow, ShapeBounding, 0, 0, X11None, ShapeSet);
#endif
}

<<<<<<< HEAD
nsresult nsWindow::UpdateTranslucentWindowAlphaInternal(const nsIntRect &aRect,
                                                        uint8_t *aAlphas,
                                                        int32_t aStride) {
  if (!mShell) {
    // Pass the request to the toplevel window
    GtkWidget *topWidget = GetToplevelWidget();
    if (!topWidget) return NS_ERROR_FAILURE;
||||||| merged common ancestors
nsresult
nsWindow::UpdateTranslucentWindowAlphaInternal(const nsIntRect& aRect,
                                               uint8_t* aAlphas, int32_t aStride)
{
    if (!mShell) {
        // Pass the request to the toplevel window
        GtkWidget *topWidget = GetToplevelWidget();
        if (!topWidget)
            return NS_ERROR_FAILURE;
=======
nsresult nsWindow::UpdateTranslucentWindowAlphaInternal(const nsIntRect& aRect,
                                                        uint8_t* aAlphas,
                                                        int32_t aStride) {
  if (!mShell) {
    // Pass the request to the toplevel window
    GtkWidget* topWidget = GetToplevelWidget();
    if (!topWidget) return NS_ERROR_FAILURE;
>>>>>>> upstream-releases

<<<<<<< HEAD
    nsWindow *topWindow = get_window_for_gtk_widget(topWidget);
    if (!topWindow) return NS_ERROR_FAILURE;
||||||| merged common ancestors
        nsWindow *topWindow = get_window_for_gtk_widget(topWidget);
        if (!topWindow)
            return NS_ERROR_FAILURE;
=======
    nsWindow* topWindow = get_window_for_gtk_widget(topWidget);
    if (!topWindow) return NS_ERROR_FAILURE;
>>>>>>> upstream-releases

    return topWindow->UpdateTranslucentWindowAlphaInternal(aRect, aAlphas,
                                                           aStride);
  }

<<<<<<< HEAD
  NS_ASSERTION(mIsTransparent, "Window is not transparent");
||||||| merged common ancestors
    NS_ASSERTION(mIsTransparent, "Window is not transparent");
=======
  NS_ASSERTION(mIsTransparent, "Window is not transparent");
  NS_ASSERTION(!mTransparencyBitmapForTitlebar,
               "Transparency bitmap is already used for titlebar rendering");
>>>>>>> upstream-releases

  if (mTransparencyBitmap == nullptr) {
    int32_t size = GetBitmapStride(mBounds.width) * mBounds.height;
    mTransparencyBitmap = new gchar[size];
    memset(mTransparencyBitmap, 255, size);
    mTransparencyBitmapWidth = mBounds.width;
    mTransparencyBitmapHeight = mBounds.height;
  } else {
    ResizeTransparencyBitmap();
  }

  nsIntRect rect;
  rect.IntersectRect(aRect, nsIntRect(0, 0, mBounds.width, mBounds.height));

  if (!ChangedMaskBits(mTransparencyBitmap, mBounds.width, mBounds.height, rect,
                       aAlphas, aStride))
    // skip the expensive stuff if the mask bits haven't changed; hopefully
    // this is the common case
    return NS_OK;

  UpdateMaskBits(mTransparencyBitmap, mBounds.width, mBounds.height, rect,
                 aAlphas, aStride);

  if (!mNeedsShow) {
    ApplyTransparencyBitmap();
  }
  return NS_OK;
}

<<<<<<< HEAD
void nsWindow::GrabPointer(guint32 aTime) {
  LOG(("GrabPointer time=0x%08x retry=%d\n", (unsigned int)aTime,
       mRetryPointerGrab));
||||||| merged common ancestors
void
nsWindow::GrabPointer(guint32 aTime)
{
    LOG(("GrabPointer time=0x%08x retry=%d\n",
         (unsigned int)aTime, mRetryPointerGrab));

    mRetryPointerGrab = false;
    sRetryGrabTime = aTime;

    // If the window isn't visible, just set the flag to retry the
    // grab.  When this window becomes visible, the grab will be
    // retried.
    if (!mHasMappedToplevel || mIsFullyObscured) {
        LOG(("GrabPointer: window not visible\n"));
        mRetryPointerGrab = true;
        return;
    }
=======
// We need to shape only a few pixels of the titlebar as we care about
// the corners only
#define TITLEBAR_SHAPE_MASK_HEIGHT 10
>>>>>>> upstream-releases

<<<<<<< HEAD
  mRetryPointerGrab = false;
  sRetryGrabTime = aTime;
||||||| merged common ancestors
    if (!mGdkWindow)
        return;
=======
void nsWindow::UpdateTitlebarTransparencyBitmap() {
  NS_ASSERTION(mTransparencyBitmapForTitlebar,
               "Transparency bitmap is already used to draw window shape");
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If the window isn't visible, just set the flag to retry the
  // grab.  When this window becomes visible, the grab will be
  // retried.
  if (!mHasMappedToplevel || mIsFullyObscured) {
    LOG(("GrabPointer: window not visible\n"));
    mRetryPointerGrab = true;
    return;
  }
||||||| merged common ancestors
    if (!mIsX11Display) {
        // Don't to the grab on Wayland as it causes a regression
        // from Bug 1377084.
        return;
    }
=======
  if (!mDrawInTitlebar || (mBounds.width == mTransparencyBitmapWidth &&
                           mBounds.height == mTransparencyBitmapHeight)) {
    return;
  }

  bool maskCreate =
      !mTransparencyBitmap || mBounds.width > mTransparencyBitmapWidth;

  bool maskUpdate =
      !mTransparencyBitmap || mBounds.width != mTransparencyBitmapWidth;

  if (maskCreate) {
    if (mTransparencyBitmap) {
      delete[] mTransparencyBitmap;
    }
    int32_t size = GetBitmapStride(mBounds.width) * TITLEBAR_SHAPE_MASK_HEIGHT;
    mTransparencyBitmap = new gchar[size];
    mTransparencyBitmapWidth = mBounds.width;
  } else {
    mTransparencyBitmapWidth = mBounds.width;
  }
  mTransparencyBitmapHeight = mBounds.height;

  if (maskUpdate) {
    cairo_surface_t* surface = cairo_image_surface_create(
        CAIRO_FORMAT_A8, mTransparencyBitmapWidth, TITLEBAR_SHAPE_MASK_HEIGHT);
    if (!surface) return;

    cairo_t* cr = cairo_create(surface);

    GtkWidgetState state;
    memset((void*)&state, 0, sizeof(state));
    GdkRectangle rect = {0, 0, mTransparencyBitmapWidth,
                         TITLEBAR_SHAPE_MASK_HEIGHT};
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!mGdkWindow) return;

  if (!mIsX11Display) {
    // Don't to the grab on Wayland as it causes a regression
    // from Bug 1377084.
    return;
  }

  gint retval;
  // Note that we need GDK_TOUCH_MASK below to work around a GDK/X11 bug that
  // causes touch events that would normally be received by this client on
  // other windows to be discarded during the grab.
  retval = gdk_pointer_grab(
      mGdkWindow, TRUE,
      (GdkEventMask)(GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
                     GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK |
                     GDK_POINTER_MOTION_MASK | GDK_TOUCH_MASK),
      (GdkWindow *)nullptr, nullptr, aTime);

  if (retval == GDK_GRAB_NOT_VIEWABLE) {
    LOG(("GrabPointer: window not viewable; will retry\n"));
    mRetryPointerGrab = true;
  } else if (retval != GDK_GRAB_SUCCESS) {
    LOG(("GrabPointer: pointer grab failed: %i\n", retval));
    // A failed grab indicates that another app has grabbed the pointer.
    // Check for rollup now, because, without the grab, we likely won't
    // get subsequent button press events. Do this with an event so that
    // popups don't rollup while potentially adjusting the grab for
    // this popup.
    nsCOMPtr<nsIRunnable> event =
        NewRunnableMethod("nsWindow::CheckForRollupDuringGrab", this,
                          &nsWindow::CheckForRollupDuringGrab);
    NS_DispatchToCurrentThread(event.forget());
  }
||||||| merged common ancestors
    gint retval;
    // Note that we need GDK_TOUCH_MASK below to work around a GDK/X11 bug that
    // causes touch events that would normally be received by this client on
    // other windows to be discarded during the grab.
    retval = gdk_pointer_grab(mGdkWindow, TRUE,
                              (GdkEventMask)(GDK_BUTTON_PRESS_MASK |
                                             GDK_BUTTON_RELEASE_MASK |
                                             GDK_ENTER_NOTIFY_MASK |
                                             GDK_LEAVE_NOTIFY_MASK |
                                             GDK_POINTER_MOTION_MASK |
                                             GDK_TOUCH_MASK),
                              (GdkWindow *)nullptr, nullptr, aTime);

    if (retval == GDK_GRAB_NOT_VIEWABLE) {
        LOG(("GrabPointer: window not viewable; will retry\n"));
        mRetryPointerGrab = true;
    } else if (retval != GDK_GRAB_SUCCESS) {
        LOG(("GrabPointer: pointer grab failed: %i\n", retval));
        // A failed grab indicates that another app has grabbed the pointer.
        // Check for rollup now, because, without the grab, we likely won't
        // get subsequent button press events. Do this with an event so that
        // popups don't rollup while potentially adjusting the grab for
        // this popup.
        nsCOMPtr<nsIRunnable> event =
          NewRunnableMethod("nsWindow::CheckForRollupDuringGrab",
                            this,
                            &nsWindow::CheckForRollupDuringGrab);
        NS_DispatchToCurrentThread(event.forget());
    }
=======
    moz_gtk_widget_paint(MOZ_GTK_HEADER_BAR, cr, &rect, &state, 0,
                         GTK_TEXT_DIR_NONE);

    cairo_destroy(cr);
    cairo_surface_mark_dirty(surface);
    cairo_surface_flush(surface);

    UpdateMaskBits(
        mTransparencyBitmap, mTransparencyBitmapWidth,
        TITLEBAR_SHAPE_MASK_HEIGHT,
        nsIntRect(0, 0, mTransparencyBitmapWidth, TITLEBAR_SHAPE_MASK_HEIGHT),
        cairo_image_surface_get_data(surface),
        cairo_format_stride_for_width(CAIRO_FORMAT_A8,
                                      mTransparencyBitmapWidth));

    cairo_surface_destroy(surface);
  }

  if (!mNeedsShow) {
    Display* xDisplay = GDK_WINDOW_XDISPLAY(mGdkWindow);
    Window xDrawable = GDK_WINDOW_XID(mGdkWindow);

    Pixmap maskPixmap = XCreateBitmapFromData(
        xDisplay, xDrawable, mTransparencyBitmap, mTransparencyBitmapWidth,
        TITLEBAR_SHAPE_MASK_HEIGHT);

    XShapeCombineMask(xDisplay, xDrawable, ShapeBounding, 0, 0, maskPixmap,
                      ShapeSet);

    if (mTransparencyBitmapHeight > TITLEBAR_SHAPE_MASK_HEIGHT) {
      XRectangle rect = {0, 0, (unsigned short)mTransparencyBitmapWidth,
                         (unsigned short)(mTransparencyBitmapHeight -
                                          TITLEBAR_SHAPE_MASK_HEIGHT)};
      XShapeCombineRectangles(xDisplay, xDrawable, ShapeBounding, 0,
                              TITLEBAR_SHAPE_MASK_HEIGHT, &rect, 1, ShapeUnion,
                              0);
    }

    XFreePixmap(xDisplay, maskPixmap);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsWindow::ReleaseGrabs(void) {
  LOG(("ReleaseGrabs\n"));
||||||| merged common ancestors
void
nsWindow::ReleaseGrabs(void)
{
    LOG(("ReleaseGrabs\n"));
=======
void nsWindow::GrabPointer(guint32 aTime) {
  LOG(("GrabPointer time=0x%08x retry=%d\n", (unsigned int)aTime,
       mRetryPointerGrab));
>>>>>>> upstream-releases

<<<<<<< HEAD
  mRetryPointerGrab = false;
||||||| merged common ancestors
    mRetryPointerGrab = false;
=======
  mRetryPointerGrab = false;
  sRetryGrabTime = aTime;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!mIsX11Display) {
    // Don't to the ungrab on Wayland as it causes a regression
    // from Bug 1377084.
    return;
  }
||||||| merged common ancestors
    if (!mIsX11Display) {
        // Don't to the ungrab on Wayland as it causes a regression
        // from Bug 1377084.
        return;
    }
=======
  // If the window isn't visible, just set the flag to retry the
  // grab.  When this window becomes visible, the grab will be
  // retried.
  if (!mHasMappedToplevel || mIsFullyObscured) {
    LOG(("GrabPointer: window not visible\n"));
    mRetryPointerGrab = true;
    return;
  }

  if (!mGdkWindow) return;

  if (!mIsX11Display) {
    // Don't to the grab on Wayland as it causes a regression
    // from Bug 1377084.
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  gdk_pointer_ungrab(GDK_CURRENT_TIME);
||||||| merged common ancestors
    gdk_pointer_ungrab(GDK_CURRENT_TIME);
=======
  gint retval;
  // Note that we need GDK_TOUCH_MASK below to work around a GDK/X11 bug that
  // causes touch events that would normally be received by this client on
  // other windows to be discarded during the grab.
  retval = gdk_pointer_grab(
      mGdkWindow, TRUE,
      (GdkEventMask)(GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
                     GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK |
                     GDK_POINTER_MOTION_MASK | GDK_TOUCH_MASK),
      (GdkWindow*)nullptr, nullptr, aTime);

  if (retval == GDK_GRAB_NOT_VIEWABLE) {
    LOG(("GrabPointer: window not viewable; will retry\n"));
    mRetryPointerGrab = true;
  } else if (retval != GDK_GRAB_SUCCESS) {
    LOG(("GrabPointer: pointer grab failed: %i\n", retval));
    // A failed grab indicates that another app has grabbed the pointer.
    // Check for rollup now, because, without the grab, we likely won't
    // get subsequent button press events. Do this with an event so that
    // popups don't rollup while potentially adjusting the grab for
    // this popup.
    nsCOMPtr<nsIRunnable> event =
        NewRunnableMethod("nsWindow::CheckForRollupDuringGrab", this,
                          &nsWindow::CheckForRollupDuringGrab);
    NS_DispatchToCurrentThread(event.forget());
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
GtkWidget *nsWindow::GetToplevelWidget() {
  if (mShell) {
    return mShell;
  }
||||||| merged common ancestors
GtkWidget *
nsWindow::GetToplevelWidget()
{
    if (mShell) {
        return mShell;
    }
=======
void nsWindow::ReleaseGrabs(void) {
  LOG(("ReleaseGrabs\n"));

  mRetryPointerGrab = false;
>>>>>>> upstream-releases

<<<<<<< HEAD
  GtkWidget *widget = GetMozContainerWidget();
  if (!widget) return nullptr;
||||||| merged common ancestors
    GtkWidget *widget = GetMozContainerWidget();
    if (!widget)
        return nullptr;
=======
  if (!mIsX11Display) {
    // Don't to the ungrab on Wayland as it causes a regression
    // from Bug 1377084.
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return gtk_widget_get_toplevel(widget);
||||||| merged common ancestors
    return gtk_widget_get_toplevel(widget);
=======
  gdk_pointer_ungrab(GDK_CURRENT_TIME);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
GtkWidget *nsWindow::GetMozContainerWidget() {
  if (!mGdkWindow) return nullptr;
||||||| merged common ancestors
GtkWidget *
nsWindow::GetMozContainerWidget()
{
    if (!mGdkWindow)
        return nullptr;
=======
GtkWidget* nsWindow::GetToplevelWidget() {
  if (mShell) {
    return mShell;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mContainer) return GTK_WIDGET(mContainer);
||||||| merged common ancestors
    if (mContainer)
        return GTK_WIDGET(mContainer);
=======
  GtkWidget* widget = GetMozContainerWidget();
  if (!widget) return nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  GtkWidget *owningWidget = get_gtk_widget_for_gdk_window(mGdkWindow);
  return owningWidget;
||||||| merged common ancestors
    GtkWidget *owningWidget =
        get_gtk_widget_for_gdk_window(mGdkWindow);
    return owningWidget;
=======
  return gtk_widget_get_toplevel(widget);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsWindow *nsWindow::GetContainerWindow() {
  GtkWidget *owningWidget = GetMozContainerWidget();
  if (!owningWidget) return nullptr;
||||||| merged common ancestors
nsWindow *
nsWindow::GetContainerWindow()
{
    GtkWidget *owningWidget = GetMozContainerWidget();
    if (!owningWidget)
        return nullptr;
=======
GtkWidget* nsWindow::GetMozContainerWidget() {
  if (!mGdkWindow) return nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsWindow *window = get_window_for_gtk_widget(owningWidget);
  NS_ASSERTION(window, "No nsWindow for container widget");
  return window;
||||||| merged common ancestors
    nsWindow *window = get_window_for_gtk_widget(owningWidget);
    NS_ASSERTION(window, "No nsWindow for container widget");
    return window;
=======
  if (mContainer) return GTK_WIDGET(mContainer);

  GtkWidget* owningWidget = get_gtk_widget_for_gdk_window(mGdkWindow);
  return owningWidget;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsWindow::SetUrgencyHint(GtkWidget *top_window, bool state) {
  if (!top_window) return;
||||||| merged common ancestors
void
nsWindow::SetUrgencyHint(GtkWidget *top_window, bool state)
{
    if (!top_window)
        return;
=======
nsWindow* nsWindow::GetContainerWindow() {
  GtkWidget* owningWidget = GetMozContainerWidget();
  if (!owningWidget) return nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  gdk_window_set_urgency_hint(gtk_widget_get_window(top_window), state);
||||||| merged common ancestors
    gdk_window_set_urgency_hint(gtk_widget_get_window(top_window), state);
=======
  nsWindow* window = get_window_for_gtk_widget(owningWidget);
  NS_ASSERTION(window, "No nsWindow for container widget");
  return window;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsWindow::SetDefaultIcon(void) { SetIcon(NS_LITERAL_STRING("default")); }
||||||| merged common ancestors
void
nsWindow::SetDefaultIcon(void)
{
    SetIcon(NS_LITERAL_STRING("default"));
}
=======
void nsWindow::SetUrgencyHint(GtkWidget* top_window, bool state) {
  if (!top_window) return;

  gdk_window_set_urgency_hint(gtk_widget_get_window(top_window), state);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
gint nsWindow::ConvertBorderStyles(nsBorderStyle aStyle) {
  gint w = 0;
||||||| merged common ancestors
gint
nsWindow::ConvertBorderStyles(nsBorderStyle aStyle)
{
    gint w = 0;

    if (aStyle == eBorderStyle_default)
        return -1;

    // note that we don't handle eBorderStyle_close yet
    if (aStyle & eBorderStyle_all)
        w |= GDK_DECOR_ALL;
    if (aStyle & eBorderStyle_border)
        w |= GDK_DECOR_BORDER;
    if (aStyle & eBorderStyle_resizeh)
        w |= GDK_DECOR_RESIZEH;
    if (aStyle & eBorderStyle_title)
        w |= GDK_DECOR_TITLE;
    if (aStyle & eBorderStyle_menu)
        w |= GDK_DECOR_MENU;
    if (aStyle & eBorderStyle_minimize)
        w |= GDK_DECOR_MINIMIZE;
    if (aStyle & eBorderStyle_maximize)
        w |= GDK_DECOR_MAXIMIZE;

    return w;
}

class FullscreenTransitionWindow final : public nsISupports
{
public:
    NS_DECL_ISUPPORTS
=======
void nsWindow::SetDefaultIcon(void) { SetIcon(NS_LITERAL_STRING("default")); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (aStyle == eBorderStyle_default) return -1;
||||||| merged common ancestors
    explicit FullscreenTransitionWindow(GtkWidget* aWidget);
=======
gint nsWindow::ConvertBorderStyles(nsBorderStyle aStyle) {
  gint w = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // note that we don't handle eBorderStyle_close yet
  if (aStyle & eBorderStyle_all) w |= GDK_DECOR_ALL;
  if (aStyle & eBorderStyle_border) w |= GDK_DECOR_BORDER;
  if (aStyle & eBorderStyle_resizeh) w |= GDK_DECOR_RESIZEH;
  if (aStyle & eBorderStyle_title) w |= GDK_DECOR_TITLE;
  if (aStyle & eBorderStyle_menu) w |= GDK_DECOR_MENU;
  if (aStyle & eBorderStyle_minimize) w |= GDK_DECOR_MINIMIZE;
  if (aStyle & eBorderStyle_maximize) w |= GDK_DECOR_MAXIMIZE;
||||||| merged common ancestors
    GtkWidget* mWindow;
=======
  if (aStyle == eBorderStyle_default) return -1;
>>>>>>> upstream-releases

<<<<<<< HEAD
  return w;
}
||||||| merged common ancestors
private:
    ~FullscreenTransitionWindow();
};
=======
  // note that we don't handle eBorderStyle_close yet
  if (aStyle & eBorderStyle_all) w |= GDK_DECOR_ALL;
  if (aStyle & eBorderStyle_border) w |= GDK_DECOR_BORDER;
  if (aStyle & eBorderStyle_resizeh) w |= GDK_DECOR_RESIZEH;
  if (aStyle & eBorderStyle_title) w |= GDK_DECOR_TITLE;
  if (aStyle & eBorderStyle_menu) w |= GDK_DECOR_MENU;
  if (aStyle & eBorderStyle_minimize) w |= GDK_DECOR_MINIMIZE;
  if (aStyle & eBorderStyle_maximize) w |= GDK_DECOR_MAXIMIZE;
>>>>>>> upstream-releases

<<<<<<< HEAD
class FullscreenTransitionWindow final : public nsISupports {
 public:
  NS_DECL_ISUPPORTS
||||||| merged common ancestors
NS_IMPL_ISUPPORTS0(FullscreenTransitionWindow)
=======
  return w;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  explicit FullscreenTransitionWindow(GtkWidget *aWidget);
||||||| merged common ancestors
FullscreenTransitionWindow::FullscreenTransitionWindow(GtkWidget* aWidget)
{
    mWindow = gtk_window_new(GTK_WINDOW_POPUP);
    GtkWindow* gtkWin = GTK_WINDOW(mWindow);

    gtk_window_set_type_hint(gtkWin, GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);
    gtk_window_set_transient_for(gtkWin, GTK_WINDOW(aWidget));
    gtk_window_set_decorated(gtkWin, false);

    GdkWindow* gdkWin = gtk_widget_get_window(aWidget);
    GdkScreen* screen = gtk_widget_get_screen(aWidget);
    gint monitorNum = gdk_screen_get_monitor_at_window(screen, gdkWin);
    GdkRectangle monitorRect;
    gdk_screen_get_monitor_geometry(screen, monitorNum, &monitorRect);
    gtk_window_set_screen(gtkWin, screen);
    gtk_window_move(gtkWin, monitorRect.x, monitorRect.y);
    MOZ_ASSERT(monitorRect.width > 0 && monitorRect.height > 0,
               "Can't resize window smaller than 1x1.");
    gtk_window_resize(gtkWin, monitorRect.width, monitorRect.height);
=======
class FullscreenTransitionWindow final : public nsISupports {
 public:
  NS_DECL_ISUPPORTS
>>>>>>> upstream-releases

<<<<<<< HEAD
  GtkWidget *mWindow;
||||||| merged common ancestors
    GdkColor bgColor;
    bgColor.red = bgColor.green = bgColor.blue = 0;
    gtk_widget_modify_bg(mWindow, GTK_STATE_NORMAL, &bgColor);
=======
  explicit FullscreenTransitionWindow(GtkWidget* aWidget);
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  ~FullscreenTransitionWindow();
};
||||||| merged common ancestors
    gtk_window_set_opacity(gtkWin, 0.0);
    gtk_widget_show(mWindow);
}
=======
  GtkWidget* mWindow;
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMPL_ISUPPORTS0(FullscreenTransitionWindow)
||||||| merged common ancestors
FullscreenTransitionWindow::~FullscreenTransitionWindow()
{
    gtk_widget_destroy(mWindow);
}
=======
 private:
  ~FullscreenTransitionWindow();
};
>>>>>>> upstream-releases

<<<<<<< HEAD
FullscreenTransitionWindow::FullscreenTransitionWindow(GtkWidget *aWidget) {
  mWindow = gtk_window_new(GTK_WINDOW_POPUP);
  GtkWindow *gtkWin = GTK_WINDOW(mWindow);

  gtk_window_set_type_hint(gtkWin, GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);
  gtk_window_set_transient_for(gtkWin, GTK_WINDOW(aWidget));
  gtk_window_set_decorated(gtkWin, false);

  GdkWindow *gdkWin = gtk_widget_get_window(aWidget);
  GdkScreen *screen = gtk_widget_get_screen(aWidget);
  gint monitorNum = gdk_screen_get_monitor_at_window(screen, gdkWin);
  GdkRectangle monitorRect;
  gdk_screen_get_monitor_geometry(screen, monitorNum, &monitorRect);
  gtk_window_set_screen(gtkWin, screen);
  gtk_window_move(gtkWin, monitorRect.x, monitorRect.y);
  MOZ_ASSERT(monitorRect.width > 0 && monitorRect.height > 0,
             "Can't resize window smaller than 1x1.");
  gtk_window_resize(gtkWin, monitorRect.width, monitorRect.height);

  GdkColor bgColor;
  bgColor.red = bgColor.green = bgColor.blue = 0;
  gtk_widget_modify_bg(mWindow, GTK_STATE_NORMAL, &bgColor);

  gtk_window_set_opacity(gtkWin, 0.0);
  gtk_widget_show(mWindow);
}

FullscreenTransitionWindow::~FullscreenTransitionWindow() {
  gtk_widget_destroy(mWindow);
}

class FullscreenTransitionData {
 public:
  FullscreenTransitionData(nsIWidget::FullscreenTransitionStage aStage,
                           uint16_t aDuration, nsIRunnable *aCallback,
                           FullscreenTransitionWindow *aWindow)
      : mStage(aStage),
        mStartTime(TimeStamp::Now()),
        mDuration(TimeDuration::FromMilliseconds(aDuration)),
        mCallback(aCallback),
        mWindow(aWindow) {}

  static const guint sInterval = 1000 / 30;  // 30fps
  static gboolean TimeoutCallback(gpointer aData);

 private:
  nsIWidget::FullscreenTransitionStage mStage;
  TimeStamp mStartTime;
  TimeDuration mDuration;
  nsCOMPtr<nsIRunnable> mCallback;
  RefPtr<FullscreenTransitionWindow> mWindow;
||||||| merged common ancestors
class FullscreenTransitionData
{
public:
    FullscreenTransitionData(nsIWidget::FullscreenTransitionStage aStage,
                             uint16_t aDuration, nsIRunnable* aCallback,
                             FullscreenTransitionWindow* aWindow)
        : mStage(aStage)
        , mStartTime(TimeStamp::Now())
        , mDuration(TimeDuration::FromMilliseconds(aDuration))
        , mCallback(aCallback)
        , mWindow(aWindow) { }

    static const guint sInterval = 1000 / 30; // 30fps
    static gboolean TimeoutCallback(gpointer aData);

private:
    nsIWidget::FullscreenTransitionStage mStage;
    TimeStamp mStartTime;
    TimeDuration mDuration;
    nsCOMPtr<nsIRunnable> mCallback;
    RefPtr<FullscreenTransitionWindow> mWindow;
=======
NS_IMPL_ISUPPORTS0(FullscreenTransitionWindow)

FullscreenTransitionWindow::FullscreenTransitionWindow(GtkWidget* aWidget) {
  mWindow = gtk_window_new(GTK_WINDOW_POPUP);
  GtkWindow* gtkWin = GTK_WINDOW(mWindow);

  gtk_window_set_type_hint(gtkWin, GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);
  gtk_window_set_transient_for(gtkWin, GTK_WINDOW(aWidget));
  gtk_window_set_decorated(gtkWin, false);

  GdkWindow* gdkWin = gtk_widget_get_window(aWidget);
  GdkScreen* screen = gtk_widget_get_screen(aWidget);
  gint monitorNum = gdk_screen_get_monitor_at_window(screen, gdkWin);
  GdkRectangle monitorRect;
  gdk_screen_get_monitor_geometry(screen, monitorNum, &monitorRect);
  gtk_window_set_screen(gtkWin, screen);
  gtk_window_move(gtkWin, monitorRect.x, monitorRect.y);
  MOZ_ASSERT(monitorRect.width > 0 && monitorRect.height > 0,
             "Can't resize window smaller than 1x1.");
  gtk_window_resize(gtkWin, monitorRect.width, monitorRect.height);

  GdkColor bgColor;
  bgColor.red = bgColor.green = bgColor.blue = 0;
  gtk_widget_modify_bg(mWindow, GTK_STATE_NORMAL, &bgColor);

  gtk_window_set_opacity(gtkWin, 0.0);
  gtk_widget_show(mWindow);
}

FullscreenTransitionWindow::~FullscreenTransitionWindow() {
  gtk_widget_destroy(mWindow);
}

class FullscreenTransitionData {
 public:
  FullscreenTransitionData(nsIWidget::FullscreenTransitionStage aStage,
                           uint16_t aDuration, nsIRunnable* aCallback,
                           FullscreenTransitionWindow* aWindow)
      : mStage(aStage),
        mStartTime(TimeStamp::Now()),
        mDuration(TimeDuration::FromMilliseconds(aDuration)),
        mCallback(aCallback),
        mWindow(aWindow) {}

  static const guint sInterval = 1000 / 30;  // 30fps
  static gboolean TimeoutCallback(gpointer aData);

 private:
  nsIWidget::FullscreenTransitionStage mStage;
  TimeStamp mStartTime;
  TimeDuration mDuration;
  nsCOMPtr<nsIRunnable> mCallback;
  RefPtr<FullscreenTransitionWindow> mWindow;
>>>>>>> upstream-releases
};

<<<<<<< HEAD
/* static */ gboolean FullscreenTransitionData::TimeoutCallback(
    gpointer aData) {
  bool finishing = false;
  auto data = static_cast<FullscreenTransitionData *>(aData);
  gdouble opacity = (TimeStamp::Now() - data->mStartTime) / data->mDuration;
  if (opacity >= 1.0) {
    opacity = 1.0;
    finishing = true;
  }
  if (data->mStage == nsIWidget::eAfterFullscreenToggle) {
    opacity = 1.0 - opacity;
  }
  gtk_window_set_opacity(GTK_WINDOW(data->mWindow->mWindow), opacity);
||||||| merged common ancestors
/* static */ gboolean
FullscreenTransitionData::TimeoutCallback(gpointer aData)
{
    bool finishing = false;
    auto data = static_cast<FullscreenTransitionData*>(aData);
    gdouble opacity = (TimeStamp::Now() - data->mStartTime) / data->mDuration;
    if (opacity >= 1.0) {
        opacity = 1.0;
        finishing = true;
    }
    if (data->mStage == nsIWidget::eAfterFullscreenToggle) {
        opacity = 1.0 - opacity;
    }
    gtk_window_set_opacity(GTK_WINDOW(data->mWindow->mWindow), opacity);
=======
/* static */
gboolean FullscreenTransitionData::TimeoutCallback(gpointer aData) {
  bool finishing = false;
  auto data = static_cast<FullscreenTransitionData*>(aData);
  gdouble opacity = (TimeStamp::Now() - data->mStartTime) / data->mDuration;
  if (opacity >= 1.0) {
    opacity = 1.0;
    finishing = true;
  }
  if (data->mStage == nsIWidget::eAfterFullscreenToggle) {
    opacity = 1.0 - opacity;
  }
  gtk_window_set_opacity(GTK_WINDOW(data->mWindow->mWindow), opacity);
>>>>>>> upstream-releases

  if (!finishing) {
    return TRUE;
  }
  NS_DispatchToMainThread(data->mCallback.forget());
  delete data;
  return FALSE;
}

<<<<<<< HEAD
/* virtual */ bool nsWindow::PrepareForFullscreenTransition(
    nsISupports **aData) {
  GdkScreen *screen = gtk_widget_get_screen(mShell);
  if (!gdk_screen_is_composited(screen)) {
    return false;
  }
  *aData = do_AddRef(new FullscreenTransitionWindow(mShell)).take();
  return true;
||||||| merged common ancestors
/* virtual */ bool
nsWindow::PrepareForFullscreenTransition(nsISupports** aData)
{
    GdkScreen* screen = gtk_widget_get_screen(mShell);
    if (!gdk_screen_is_composited(screen)) {
        return false;
    }
    *aData = do_AddRef(new FullscreenTransitionWindow(mShell)).take();
    return true;
=======
/* virtual */
bool nsWindow::PrepareForFullscreenTransition(nsISupports** aData) {
  GdkScreen* screen = gtk_widget_get_screen(mShell);
  if (!gdk_screen_is_composited(screen)) {
    return false;
  }
  *aData = do_AddRef(new FullscreenTransitionWindow(mShell)).take();
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* virtual */ void nsWindow::PerformFullscreenTransition(
    FullscreenTransitionStage aStage, uint16_t aDuration, nsISupports *aData,
    nsIRunnable *aCallback) {
  auto data = static_cast<FullscreenTransitionWindow *>(aData);
  // This will be released at the end of the last timeout callback for it.
  auto transitionData =
      new FullscreenTransitionData(aStage, aDuration, aCallback, data);
  g_timeout_add_full(G_PRIORITY_HIGH, FullscreenTransitionData::sInterval,
                     FullscreenTransitionData::TimeoutCallback, transitionData,
                     nullptr);
}

already_AddRefed<nsIScreen> nsWindow::GetWidgetScreen() {
||||||| merged common ancestors
/* virtual */ void
nsWindow::PerformFullscreenTransition(FullscreenTransitionStage aStage,
                                      uint16_t aDuration, nsISupports* aData,
                                      nsIRunnable* aCallback)
{
    auto data = static_cast<FullscreenTransitionWindow*>(aData);
    // This will be released at the end of the last timeout callback for it.
    auto transitionData = new FullscreenTransitionData(aStage, aDuration,
                                                       aCallback, data);
    g_timeout_add_full(G_PRIORITY_HIGH,
                       FullscreenTransitionData::sInterval,
                       FullscreenTransitionData::TimeoutCallback,
                       transitionData, nullptr);
}

already_AddRefed<nsIScreen>
nsWindow::GetWidgetScreen()
{
=======
/* virtual */
void nsWindow::PerformFullscreenTransition(FullscreenTransitionStage aStage,
                                           uint16_t aDuration,
                                           nsISupports* aData,
                                           nsIRunnable* aCallback) {
  auto data = static_cast<FullscreenTransitionWindow*>(aData);
  // This will be released at the end of the last timeout callback for it.
  auto transitionData =
      new FullscreenTransitionData(aStage, aDuration, aCallback, data);
  g_timeout_add_full(G_PRIORITY_HIGH, FullscreenTransitionData::sInterval,
                     FullscreenTransitionData::TimeoutCallback, transitionData,
                     nullptr);
}

already_AddRefed<nsIScreen> nsWindow::GetWidgetScreen() {
>>>>>>> upstream-releases
  nsCOMPtr<nsIScreenManager> screenManager;
  screenManager = do_GetService("@mozilla.org/gfx/screenmanager;1");
  if (!screenManager) {
    return nullptr;
  }

  // GetScreenBounds() is slow for the GTK port so we override and use
  // mBounds directly.
  LayoutDeviceIntRect bounds = mBounds;
  if (!mIsTopLevel) {
    bounds.MoveTo(WidgetToScreenOffset());
  }

  DesktopIntRect deskBounds = RoundedToInt(bounds / GetDesktopToDeviceScale());
  nsCOMPtr<nsIScreen> screen;
  screenManager->ScreenForRect(deskBounds.x, deskBounds.y, deskBounds.width,
                               deskBounds.height, getter_AddRefs(screen));
  return screen.forget();
}

<<<<<<< HEAD
static bool IsFullscreenSupported(GtkWidget *aShell) {
||||||| merged common ancestors
static bool
IsFullscreenSupported(GtkWidget* aShell)
{
=======
static bool IsFullscreenSupported(GtkWidget* aShell) {
>>>>>>> upstream-releases
#ifdef MOZ_X11
<<<<<<< HEAD
  GdkScreen *screen = gtk_widget_get_screen(aShell);
  GdkAtom atom = gdk_atom_intern("_NET_WM_STATE_FULLSCREEN", FALSE);
  if (!gdk_x11_screen_supports_net_wm_hint(screen, atom)) {
    return false;
  }
||||||| merged common ancestors
    GdkScreen* screen = gtk_widget_get_screen(aShell);
    GdkAtom atom = gdk_atom_intern("_NET_WM_STATE_FULLSCREEN", FALSE);
    if (!gdk_x11_screen_supports_net_wm_hint(screen, atom)) {
        return false;
    }
=======
  GdkScreen* screen = gtk_widget_get_screen(aShell);
  GdkAtom atom = gdk_atom_intern("_NET_WM_STATE_FULLSCREEN", FALSE);
  if (!gdk_x11_screen_supports_net_wm_hint(screen, atom)) {
    return false;
  }
>>>>>>> upstream-releases
#endif
  return true;
}

<<<<<<< HEAD
nsresult nsWindow::MakeFullScreen(bool aFullScreen, nsIScreen *aTargetScreen) {
  LOG(("nsWindow::MakeFullScreen [%p] aFullScreen %d\n", (void *)this,
       aFullScreen));
||||||| merged common ancestors
nsresult
nsWindow::MakeFullScreen(bool aFullScreen, nsIScreen* aTargetScreen)
{
    LOG(("nsWindow::MakeFullScreen [%p] aFullScreen %d\n",
         (void *)this, aFullScreen));
=======
nsresult nsWindow::MakeFullScreen(bool aFullScreen, nsIScreen* aTargetScreen) {
  LOG(("nsWindow::MakeFullScreen [%p] aFullScreen %d\n", (void*)this,
       aFullScreen));
>>>>>>> upstream-releases

  if (mIsX11Display && !IsFullscreenSupported(mShell)) {
    return NS_ERROR_NOT_AVAILABLE;
  }

  if (aFullScreen) {
    if (mSizeMode != nsSizeMode_Fullscreen) mLastSizeMode = mSizeMode;

    mSizeMode = nsSizeMode_Fullscreen;
    gtk_window_fullscreen(GTK_WINDOW(mShell));
  } else {
    mSizeMode = mLastSizeMode;
    gtk_window_unfullscreen(GTK_WINDOW(mShell));
  }

  NS_ASSERTION(mLastSizeMode != nsSizeMode_Fullscreen,
               "mLastSizeMode should never be fullscreen");
  return NS_OK;
}

<<<<<<< HEAD
void nsWindow::SetWindowDecoration(nsBorderStyle aStyle) {
  if (!mShell) {
    // Pass the request to the toplevel window
    GtkWidget *topWidget = GetToplevelWidget();
    if (!topWidget) return;
||||||| merged common ancestors
void
nsWindow::SetWindowDecoration(nsBorderStyle aStyle)
{
    if (!mShell) {
        // Pass the request to the toplevel window
        GtkWidget *topWidget = GetToplevelWidget();
        if (!topWidget)
            return;

        nsWindow *topWindow = get_window_for_gtk_widget(topWidget);
        if (!topWindow)
            return;

        topWindow->SetWindowDecoration(aStyle);
        return;
    }
=======
void nsWindow::SetWindowDecoration(nsBorderStyle aStyle) {
  if (!mShell) {
    // Pass the request to the toplevel window
    GtkWidget* topWidget = GetToplevelWidget();
    if (!topWidget) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
    nsWindow *topWindow = get_window_for_gtk_widget(topWidget);
    if (!topWindow) return;

    topWindow->SetWindowDecoration(aStyle);
    return;
  }
||||||| merged common ancestors
    // We can't use mGdkWindow directly here as it can be
    // derived from mContainer which is not a top-level GdkWindow.
    GdkWindow *window = gtk_widget_get_window(mShell);

    // Sawfish, metacity, and presumably other window managers get
    // confused if we change the window decorations while the window
    // is visible.
    bool wasVisible = false;
    if (gdk_window_is_visible(window)) {
        gdk_window_hide(window);
        wasVisible = true;
    }
=======
    nsWindow* topWindow = get_window_for_gtk_widget(topWidget);
    if (!topWindow) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // We can't use mGdkWindow directly here as it can be
  // derived from mContainer which is not a top-level GdkWindow.
  GdkWindow *window = gtk_widget_get_window(mShell);

  // Sawfish, metacity, and presumably other window managers get
  // confused if we change the window decorations while the window
  // is visible.
  bool wasVisible = false;
  if (gdk_window_is_visible(window)) {
    gdk_window_hide(window);
    wasVisible = true;
  }

  gint wmd = ConvertBorderStyles(aStyle);
  if (wmd != -1) gdk_window_set_decorations(window, (GdkWMDecoration)wmd);
||||||| merged common ancestors
    gint wmd = ConvertBorderStyles(aStyle);
    if (wmd != -1)
      gdk_window_set_decorations(window, (GdkWMDecoration) wmd);
=======
    topWindow->SetWindowDecoration(aStyle);
    return;
  }

  // We can't use mGdkWindow directly here as it can be
  // derived from mContainer which is not a top-level GdkWindow.
  GdkWindow* window = gtk_widget_get_window(mShell);

  // Sawfish, metacity, and presumably other window managers get
  // confused if we change the window decorations while the window
  // is visible.
  bool wasVisible = false;
  if (gdk_window_is_visible(window)) {
    gdk_window_hide(window);
    wasVisible = true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (wasVisible) gdk_window_show(window);
||||||| merged common ancestors
    if (wasVisible)
        gdk_window_show(window);
=======
  gint wmd = ConvertBorderStyles(aStyle);
  if (wmd != -1) gdk_window_set_decorations(window, (GdkWMDecoration)wmd);

  if (wasVisible) gdk_window_show(window);
>>>>>>> upstream-releases

    // For some window managers, adding or removing window decorations
    // requires unmapping and remapping our toplevel window.  Go ahead
    // and flush the queue here so that we don't end up with a BadWindow
    // error later when this happens (when the persistence timer fires
    // and GetWindowPos is called)
#ifdef MOZ_X11
<<<<<<< HEAD
  if (mIsX11Display) {
    XSync(GDK_DISPLAY_XDISPLAY(gdk_display_get_default()), False);
  } else
||||||| merged common ancestors
    if (mIsX11Display) {
        XSync(GDK_DISPLAY_XDISPLAY(gdk_display_get_default()) , False);
    } else
=======
  if (mIsX11Display) {
    XSync(GDK_DISPLAY_XDISPLAY(gdk_display_get_default()), X11False);
  } else
>>>>>>> upstream-releases
#endif /* MOZ_X11 */
  {
    gdk_flush();
  }
}

void nsWindow::HideWindowChrome(bool aShouldHide) {
  SetWindowDecoration(aShouldHide ? eBorderStyle_none : mBorderStyle);
}

<<<<<<< HEAD
bool nsWindow::CheckForRollup(gdouble aMouseX, gdouble aMouseY, bool aIsWheel,
                              bool aAlwaysRollup) {
  nsIRollupListener *rollupListener = GetActiveRollupListener();
  nsCOMPtr<nsIWidget> rollupWidget;
  if (rollupListener) {
    rollupWidget = rollupListener->GetRollupWidget();
  }
  if (!rollupWidget) {
    nsBaseWidget::gRollupListener = nullptr;
    return false;
  }
||||||| merged common ancestors
bool
nsWindow::CheckForRollup(gdouble aMouseX, gdouble aMouseY,
                         bool aIsWheel, bool aAlwaysRollup)
{
    nsIRollupListener* rollupListener = GetActiveRollupListener();
    nsCOMPtr<nsIWidget> rollupWidget;
    if (rollupListener) {
        rollupWidget = rollupListener->GetRollupWidget();
    }
    if (!rollupWidget) {
        nsBaseWidget::gRollupListener = nullptr;
        return false;
    }
=======
bool nsWindow::CheckForRollup(gdouble aMouseX, gdouble aMouseY, bool aIsWheel,
                              bool aAlwaysRollup) {
  nsIRollupListener* rollupListener = GetActiveRollupListener();
  nsCOMPtr<nsIWidget> rollupWidget;
  if (rollupListener) {
    rollupWidget = rollupListener->GetRollupWidget();
  }
  if (!rollupWidget) {
    nsBaseWidget::gRollupListener = nullptr;
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool retVal = false;
  auto *currentPopup =
      (GdkWindow *)rollupWidget->GetNativeData(NS_NATIVE_WINDOW);
  if (aAlwaysRollup || !is_mouse_in_window(currentPopup, aMouseX, aMouseY)) {
    bool rollup = true;
    if (aIsWheel) {
      rollup = rollupListener->ShouldRollupOnMouseWheelEvent();
      retVal = rollupListener->ShouldConsumeOnMouseWheelEvent();
    }
    // if we're dealing with menus, we probably have submenus and
    // we don't want to rollup if the click is in a parent menu of
    // the current submenu
    uint32_t popupsToRollup = UINT32_MAX;
    if (!aAlwaysRollup) {
      AutoTArray<nsIWidget *, 5> widgetChain;
      uint32_t sameTypeCount =
          rollupListener->GetSubmenuWidgetChain(&widgetChain);
      for (uint32_t i = 0; i < widgetChain.Length(); ++i) {
        nsIWidget *widget = widgetChain[i];
        auto *currWindow = (GdkWindow *)widget->GetNativeData(NS_NATIVE_WINDOW);
        if (is_mouse_in_window(currWindow, aMouseX, aMouseY)) {
          // don't roll up if the mouse event occurred within a
          // menu of the same type. If the mouse event occurred
          // in a menu higher than that, roll up, but pass the
          // number of popups to Rollup so that only those of the
          // same type close up.
          if (i < sameTypeCount) {
            rollup = false;
          } else {
            popupsToRollup = sameTypeCount;
          }
          break;
||||||| merged common ancestors
    bool retVal = false;
    auto *currentPopup =
        (GdkWindow *)rollupWidget->GetNativeData(NS_NATIVE_WINDOW);
    if (aAlwaysRollup || !is_mouse_in_window(currentPopup, aMouseX, aMouseY)) {
        bool rollup = true;
        if (aIsWheel) {
            rollup = rollupListener->ShouldRollupOnMouseWheelEvent();
            retVal = rollupListener->ShouldConsumeOnMouseWheelEvent();
        }
        // if we're dealing with menus, we probably have submenus and
        // we don't want to rollup if the click is in a parent menu of
        // the current submenu
        uint32_t popupsToRollup = UINT32_MAX;
        if (!aAlwaysRollup) {
            AutoTArray<nsIWidget*, 5> widgetChain;
            uint32_t sameTypeCount = rollupListener->GetSubmenuWidgetChain(&widgetChain);
            for (uint32_t i=0; i<widgetChain.Length(); ++i) {
                nsIWidget* widget = widgetChain[i];
                auto* currWindow =
                    (GdkWindow*) widget->GetNativeData(NS_NATIVE_WINDOW);
                if (is_mouse_in_window(currWindow, aMouseX, aMouseY)) {
                  // don't roll up if the mouse event occurred within a
                  // menu of the same type. If the mouse event occurred
                  // in a menu higher than that, roll up, but pass the
                  // number of popups to Rollup so that only those of the
                  // same type close up.
                  if (i < sameTypeCount) {
                    rollup = false;
                  }
                  else {
                    popupsToRollup = sameTypeCount;
                  }
                  break;
                }
            } // foreach parent menu widget
        } // if rollup listener knows about menus

        // if we've determined that we should still rollup, do it.
        bool usePoint = !aIsWheel && !aAlwaysRollup;
        IntPoint point = IntPoint::Truncate(aMouseX, aMouseY);
        if (rollup && rollupListener->Rollup(popupsToRollup, true, usePoint ? &point : nullptr, nullptr)) {
            retVal = true;
=======
  bool retVal = false;
  auto* currentPopup =
      (GdkWindow*)rollupWidget->GetNativeData(NS_NATIVE_WINDOW);
  if (aAlwaysRollup || !is_mouse_in_window(currentPopup, aMouseX, aMouseY)) {
    bool rollup = true;
    if (aIsWheel) {
      rollup = rollupListener->ShouldRollupOnMouseWheelEvent();
      retVal = rollupListener->ShouldConsumeOnMouseWheelEvent();
    }
    // if we're dealing with menus, we probably have submenus and
    // we don't want to rollup if the click is in a parent menu of
    // the current submenu
    uint32_t popupsToRollup = UINT32_MAX;
    if (!aAlwaysRollup) {
      AutoTArray<nsIWidget*, 5> widgetChain;
      uint32_t sameTypeCount =
          rollupListener->GetSubmenuWidgetChain(&widgetChain);
      for (uint32_t i = 0; i < widgetChain.Length(); ++i) {
        nsIWidget* widget = widgetChain[i];
        auto* currWindow = (GdkWindow*)widget->GetNativeData(NS_NATIVE_WINDOW);
        if (is_mouse_in_window(currWindow, aMouseX, aMouseY)) {
          // don't roll up if the mouse event occurred within a
          // menu of the same type. If the mouse event occurred
          // in a menu higher than that, roll up, but pass the
          // number of popups to Rollup so that only those of the
          // same type close up.
          if (i < sameTypeCount) {
            rollup = false;
          } else {
            popupsToRollup = sameTypeCount;
          }
          break;
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
      }  // foreach parent menu widget
    }    // if rollup listener knows about menus

    // if we've determined that we should still rollup, do it.
    bool usePoint = !aIsWheel && !aAlwaysRollup;
    IntPoint point = IntPoint::Truncate(aMouseX, aMouseY);
    if (rollup &&
        rollupListener->Rollup(popupsToRollup, true,
                               usePoint ? &point : nullptr, nullptr)) {
      retVal = true;
||||||| merged common ancestors
=======
      }  // foreach parent menu widget
    }    // if rollup listener knows about menus

    // if we've determined that we should still rollup, do it.
    bool usePoint = !aIsWheel && !aAlwaysRollup;
    IntPoint point;
    if (usePoint) {
      LayoutDeviceIntPoint p = GdkEventCoordsToDevicePixels(aMouseX, aMouseY);
      point = p.ToUnknownPoint();
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
  return retVal;
||||||| merged common ancestors
    return retVal;
=======
    if (rollup &&
        rollupListener->Rollup(popupsToRollup, true,
                               usePoint ? &point : nullptr, nullptr)) {
      retVal = true;
    }
  }
  return retVal;
>>>>>>> upstream-releases
}

/* static */
bool nsWindow::DragInProgress(void) {
  nsCOMPtr<nsIDragService> dragService = do_GetService(kCDragServiceCID);

  if (!dragService) return false;

  nsCOMPtr<nsIDragSession> currentDragSession;
  dragService->GetCurrentSession(getter_AddRefs(currentDragSession));

  return currentDragSession != nullptr;
}

<<<<<<< HEAD
static bool is_mouse_in_window(GdkWindow *aWindow, gdouble aMouseX,
                               gdouble aMouseY) {
  gint x = 0;
  gint y = 0;
  gint w, h;
||||||| merged common ancestors
static bool
is_mouse_in_window (GdkWindow* aWindow, gdouble aMouseX, gdouble aMouseY)
{
    gint x = 0;
    gint y = 0;
    gint w, h;

    gint offsetX = 0;
    gint offsetY = 0;
=======
static bool is_mouse_in_window(GdkWindow* aWindow, gdouble aMouseX,
                               gdouble aMouseY) {
  gint x = 0;
  gint y = 0;
  gint w, h;
>>>>>>> upstream-releases

  gint offsetX = 0;
  gint offsetY = 0;

<<<<<<< HEAD
  GdkWindow *window = aWindow;
||||||| merged common ancestors
    while (window) {
        gint tmpX = 0;
        gint tmpY = 0;
=======
  GdkWindow* window = aWindow;
>>>>>>> upstream-releases

  while (window) {
    gint tmpX = 0;
    gint tmpY = 0;

<<<<<<< HEAD
    gdk_window_get_position(window, &tmpX, &tmpY);
    GtkWidget *widget = get_gtk_widget_for_gdk_window(window);
||||||| merged common ancestors
        // if this is a window, compute x and y given its origin and our
        // offset
        if (GTK_IS_WINDOW(widget)) {
            x = tmpX + offsetX;
            y = tmpY + offsetY;
            break;
        }
=======
    gdk_window_get_position(window, &tmpX, &tmpY);
    GtkWidget* widget = get_gtk_widget_for_gdk_window(window);
>>>>>>> upstream-releases

    // if this is a window, compute x and y given its origin and our
    // offset
    if (GTK_IS_WINDOW(widget)) {
      x = tmpX + offsetX;
      y = tmpY + offsetY;
      break;
    }

<<<<<<< HEAD
    offsetX += tmpX;
    offsetY += tmpY;
    window = gdk_window_get_parent(window);
  }
||||||| merged common ancestors
    w = gdk_window_get_width(aWindow);
    h = gdk_window_get_height(aWindow);
=======
    offsetX += tmpX;
    offsetY += tmpY;
    window = gdk_window_get_parent(window);
  }

  w = gdk_window_get_width(aWindow);
  h = gdk_window_get_height(aWindow);
>>>>>>> upstream-releases

<<<<<<< HEAD
  w = gdk_window_get_width(aWindow);
  h = gdk_window_get_height(aWindow);
||||||| merged common ancestors
    if (aMouseX > x && aMouseX < x + w &&
        aMouseY > y && aMouseY < y + h)
        return true;
=======
  if (aMouseX > x && aMouseX < x + w && aMouseY > y && aMouseY < y + h)
    return true;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (aMouseX > x && aMouseX < x + w && aMouseY > y && aMouseY < y + h)
    return true;

  return false;
||||||| merged common ancestors
    return false;
=======
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static nsWindow *get_window_for_gtk_widget(GtkWidget *widget) {
  gpointer user_data = g_object_get_data(G_OBJECT(widget), "nsWindow");
||||||| merged common ancestors
static nsWindow *
get_window_for_gtk_widget(GtkWidget *widget)
{
    gpointer user_data = g_object_get_data(G_OBJECT(widget), "nsWindow");
=======
static nsWindow* get_window_for_gtk_widget(GtkWidget* widget) {
  gpointer user_data = g_object_get_data(G_OBJECT(widget), "nsWindow");
>>>>>>> upstream-releases

<<<<<<< HEAD
  return static_cast<nsWindow *>(user_data);
||||||| merged common ancestors
    return static_cast<nsWindow *>(user_data);
=======
  return static_cast<nsWindow*>(user_data);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static nsWindow *get_window_for_gdk_window(GdkWindow *window) {
  gpointer user_data = g_object_get_data(G_OBJECT(window), "nsWindow");
||||||| merged common ancestors
static nsWindow *
get_window_for_gdk_window(GdkWindow *window)
{
    gpointer user_data = g_object_get_data(G_OBJECT(window), "nsWindow");
=======
static nsWindow* get_window_for_gdk_window(GdkWindow* window) {
  gpointer user_data = g_object_get_data(G_OBJECT(window), "nsWindow");
>>>>>>> upstream-releases

<<<<<<< HEAD
  return static_cast<nsWindow *>(user_data);
||||||| merged common ancestors
    return static_cast<nsWindow *>(user_data);
=======
  return static_cast<nsWindow*>(user_data);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static GtkWidget *get_gtk_widget_for_gdk_window(GdkWindow *window) {
  gpointer user_data = nullptr;
  gdk_window_get_user_data(window, &user_data);
||||||| merged common ancestors
static GtkWidget *
get_gtk_widget_for_gdk_window(GdkWindow *window)
{
    gpointer user_data = nullptr;
    gdk_window_get_user_data(window, &user_data);
=======
static GtkWidget* get_gtk_widget_for_gdk_window(GdkWindow* window) {
  gpointer user_data = nullptr;
  gdk_window_get_user_data(window, &user_data);
>>>>>>> upstream-releases

  return GTK_WIDGET(user_data);
}

<<<<<<< HEAD
static GdkCursor *get_gtk_cursor(nsCursor aCursor) {
  GdkCursor *gdkcursor = nullptr;
  uint8_t newType = 0xff;
||||||| merged common ancestors
static GdkCursor *
get_gtk_cursor(nsCursor aCursor)
{
    GdkCursor *gdkcursor = nullptr;
    uint8_t newType = 0xff;
=======
static GdkCursor* get_gtk_cursor(nsCursor aCursor) {
  GdkCursor* gdkcursor = nullptr;
  uint8_t newType = 0xff;
>>>>>>> upstream-releases

  if ((gdkcursor = gCursorCache[aCursor])) {
    return gdkcursor;
  }

<<<<<<< HEAD
  GdkDisplay *defaultDisplay = gdk_display_get_default();
||||||| merged common ancestors
    GdkDisplay *defaultDisplay = gdk_display_get_default();
=======
  GdkDisplay* defaultDisplay = gdk_display_get_default();
>>>>>>> upstream-releases

  // The strategy here is to use standard GDK cursors, and, if not available,
  // load by standard name with gdk_cursor_new_from_name.
  // Spec is here: http://www.freedesktop.org/wiki/Specifications/cursor-spec/
  switch (aCursor) {
    case eCursor_standard:
      gdkcursor = gdk_cursor_new_for_display(defaultDisplay, GDK_LEFT_PTR);
      break;
    case eCursor_wait:
      gdkcursor = gdk_cursor_new_for_display(defaultDisplay, GDK_WATCH);
      break;
    case eCursor_select:
      gdkcursor = gdk_cursor_new_for_display(defaultDisplay, GDK_XTERM);
      break;
    case eCursor_hyperlink:
      gdkcursor = gdk_cursor_new_for_display(defaultDisplay, GDK_HAND2);
      break;
    case eCursor_n_resize:
      gdkcursor = gdk_cursor_new_for_display(defaultDisplay, GDK_TOP_SIDE);
      break;
    case eCursor_s_resize:
      gdkcursor = gdk_cursor_new_for_display(defaultDisplay, GDK_BOTTOM_SIDE);
      break;
    case eCursor_w_resize:
      gdkcursor = gdk_cursor_new_for_display(defaultDisplay, GDK_LEFT_SIDE);
      break;
    case eCursor_e_resize:
      gdkcursor = gdk_cursor_new_for_display(defaultDisplay, GDK_RIGHT_SIDE);
      break;
    case eCursor_nw_resize:
      gdkcursor =
          gdk_cursor_new_for_display(defaultDisplay, GDK_TOP_LEFT_CORNER);
      break;
    case eCursor_se_resize:
      gdkcursor =
          gdk_cursor_new_for_display(defaultDisplay, GDK_BOTTOM_RIGHT_CORNER);
      break;
    case eCursor_ne_resize:
      gdkcursor =
          gdk_cursor_new_for_display(defaultDisplay, GDK_TOP_RIGHT_CORNER);
      break;
    case eCursor_sw_resize:
      gdkcursor =
          gdk_cursor_new_for_display(defaultDisplay, GDK_BOTTOM_LEFT_CORNER);
      break;
    case eCursor_crosshair:
      gdkcursor = gdk_cursor_new_for_display(defaultDisplay, GDK_CROSSHAIR);
      break;
    case eCursor_move:
      gdkcursor = gdk_cursor_new_for_display(defaultDisplay, GDK_FLEUR);
      break;
    case eCursor_help:
      gdkcursor =
          gdk_cursor_new_for_display(defaultDisplay, GDK_QUESTION_ARROW);
      break;
    case eCursor_copy:  // CSS3
      gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "copy");
      if (!gdkcursor) newType = MOZ_CURSOR_COPY;
      break;
    case eCursor_alias:
      gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "alias");
      if (!gdkcursor) newType = MOZ_CURSOR_ALIAS;
      break;
    case eCursor_context_menu:
      gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "context-menu");
      if (!gdkcursor) newType = MOZ_CURSOR_CONTEXT_MENU;
      break;
    case eCursor_cell:
      gdkcursor = gdk_cursor_new_for_display(defaultDisplay, GDK_PLUS);
      break;
    // Those two aren’t standardized. Trying both KDE’s and GNOME’s names
    case eCursor_grab:
      gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "openhand");
      if (!gdkcursor) newType = MOZ_CURSOR_HAND_GRAB;
      break;
    case eCursor_grabbing:
      gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "closedhand");
      if (!gdkcursor)
        gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "grabbing");
      if (!gdkcursor) newType = MOZ_CURSOR_HAND_GRABBING;
      break;
    case eCursor_spinning:
      gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "progress");
      if (!gdkcursor) newType = MOZ_CURSOR_SPINNING;
      break;
    case eCursor_zoom_in:
      gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "zoom-in");
      if (!gdkcursor) newType = MOZ_CURSOR_ZOOM_IN;
      break;
    case eCursor_zoom_out:
      gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "zoom-out");
      if (!gdkcursor) newType = MOZ_CURSOR_ZOOM_OUT;
      break;
    case eCursor_not_allowed:
      gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "not-allowed");
      if (!gdkcursor)  // nonstandard, yet common
        gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "crossed_circle");
      if (!gdkcursor) newType = MOZ_CURSOR_NOT_ALLOWED;
      break;
    case eCursor_no_drop:
      gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "no-drop");
      if (!gdkcursor)  // this nonstandard sequence makes it work on KDE and
                       // GNOME
        gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "forbidden");
      if (!gdkcursor)
        gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "circle");
      if (!gdkcursor) newType = MOZ_CURSOR_NOT_ALLOWED;
      break;
    case eCursor_vertical_text:
      newType = MOZ_CURSOR_VERTICAL_TEXT;
      break;
    case eCursor_all_scroll:
      gdkcursor = gdk_cursor_new_for_display(defaultDisplay, GDK_FLEUR);
      break;
    case eCursor_nesw_resize:
      gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "size_bdiag");
      if (!gdkcursor) newType = MOZ_CURSOR_NESW_RESIZE;
      break;
    case eCursor_nwse_resize:
      gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "size_fdiag");
      if (!gdkcursor) newType = MOZ_CURSOR_NWSE_RESIZE;
      break;
    case eCursor_ns_resize:
      gdkcursor =
          gdk_cursor_new_for_display(defaultDisplay, GDK_SB_V_DOUBLE_ARROW);
      break;
    case eCursor_ew_resize:
      gdkcursor =
          gdk_cursor_new_for_display(defaultDisplay, GDK_SB_H_DOUBLE_ARROW);
      break;
    // Here, two better fitting cursors exist in some cursor themes. Try those
    // first
    case eCursor_row_resize:
      gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "split_v");
      if (!gdkcursor)
        gdkcursor =
            gdk_cursor_new_for_display(defaultDisplay, GDK_SB_V_DOUBLE_ARROW);
      break;
    case eCursor_col_resize:
      gdkcursor = gdk_cursor_new_from_name(defaultDisplay, "split_h");
      if (!gdkcursor)
        gdkcursor =
            gdk_cursor_new_for_display(defaultDisplay, GDK_SB_H_DOUBLE_ARROW);
      break;
    case eCursor_none:
      newType = MOZ_CURSOR_NONE;
      break;
    default:
      NS_ASSERTION(aCursor, "Invalid cursor type");
      gdkcursor = gdk_cursor_new_for_display(defaultDisplay, GDK_LEFT_PTR);
      break;
  }

  // If by now we don't have a xcursor, this means we have to make a custom
  // one. First, we try creating a named cursor based on the hash of our
  // custom bitmap, as libXcursor has some magic to convert bitmapped cursors
  // to themed cursors
  if (newType != 0xFF && GtkCursors[newType].hash) {
    gdkcursor =
        gdk_cursor_new_from_name(defaultDisplay, GtkCursors[newType].hash);
  }

<<<<<<< HEAD
  // If we still don't have a xcursor, we now really create a bitmap cursor
  if (newType != 0xff && !gdkcursor) {
    GdkPixbuf *cursor_pixbuf =
        gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, 32, 32);
    if (!cursor_pixbuf) return nullptr;

    guchar *data = gdk_pixbuf_get_pixels(cursor_pixbuf);

    // Read data from GtkCursors and compose RGBA surface from 1bit bitmap and
    // mask GtkCursors bits and mask are 32x32 monochrome bitmaps (1 bit for
    // each pixel) so it's 128 byte array (4 bytes for are one bitmap row and
    // there are 32 rows here).
    const unsigned char *bits = GtkCursors[newType].bits;
    const unsigned char *mask_bits = GtkCursors[newType].mask_bits;

    for (int i = 0; i < 128; i++) {
      char bit = *bits++;
      char mask = *mask_bits++;
      for (int j = 0; j < 8; j++) {
        unsigned char pix = ~(((bit >> j) & 0x01) * 0xff);
        *data++ = pix;
        *data++ = pix;
        *data++ = pix;
        *data++ = (((mask >> j) & 0x01) * 0xff);
      }
    }
||||||| merged common ancestors
    // If we still don't have a xcursor, we now really create a bitmap cursor
    if (newType != 0xff && !gdkcursor) {
        GdkPixbuf * cursor_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, 32, 32);
        if (!cursor_pixbuf)
            return nullptr;

        guchar *data = gdk_pixbuf_get_pixels(cursor_pixbuf);

        // Read data from GtkCursors and compose RGBA surface from 1bit bitmap and mask
        // GtkCursors bits and mask are 32x32 monochrome bitmaps (1 bit for each pixel)
        // so it's 128 byte array (4 bytes for are one bitmap row and there are 32 rows here).
        const unsigned char *bits = GtkCursors[newType].bits;
        const unsigned char *mask_bits = GtkCursors[newType].mask_bits;

        for (int i = 0; i < 128; i++) {
            char bit = *bits++;
            char mask = *mask_bits++;
            for (int j = 0; j < 8; j++) {
                unsigned char pix = ~(((bit >> j) & 0x01) * 0xff);
                *data++ = pix;
                *data++ = pix;
                *data++ = pix;
                *data++ = (((mask >> j) & 0x01) * 0xff);
            }
        }
=======
  // If we still don't have a xcursor, we now really create a bitmap cursor
  if (newType != 0xff && !gdkcursor) {
    GdkPixbuf* cursor_pixbuf =
        gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, 32, 32);
    if (!cursor_pixbuf) return nullptr;

    guchar* data = gdk_pixbuf_get_pixels(cursor_pixbuf);

    // Read data from GtkCursors and compose RGBA surface from 1bit bitmap and
    // mask GtkCursors bits and mask are 32x32 monochrome bitmaps (1 bit for
    // each pixel) so it's 128 byte array (4 bytes for are one bitmap row and
    // there are 32 rows here).
    const unsigned char* bits = GtkCursors[newType].bits;
    const unsigned char* mask_bits = GtkCursors[newType].mask_bits;

    for (int i = 0; i < 128; i++) {
      char bit = *bits++;
      char mask = *mask_bits++;
      for (int j = 0; j < 8; j++) {
        unsigned char pix = ~(((bit >> j) & 0x01) * 0xff);
        *data++ = pix;
        *data++ = pix;
        *data++ = pix;
        *data++ = (((mask >> j) & 0x01) * 0xff);
      }
    }
>>>>>>> upstream-releases

    gdkcursor = gdk_cursor_new_from_pixbuf(
        gdk_display_get_default(), cursor_pixbuf, GtkCursors[newType].hot_x,
        GtkCursors[newType].hot_y);

    g_object_unref(cursor_pixbuf);
  }

  gCursorCache[aCursor] = gdkcursor;

  return gdkcursor;
}

// gtk callbacks

<<<<<<< HEAD
void draw_window_of_widget(GtkWidget *widget, GdkWindow *aWindow, cairo_t *cr) {
  if (gtk_cairo_should_draw_window(cr, aWindow)) {
    RefPtr<nsWindow> window = get_window_for_gdk_window(aWindow);
    if (!window) {
      NS_WARNING("Cannot get nsWindow from GtkWidget");
    } else {
      cairo_save(cr);
      gtk_cairo_transform_to_window(cr, widget, aWindow);
      // TODO - window->OnExposeEvent() can destroy this or other windows,
      // do we need to handle it somehow?
      window->OnExposeEvent(cr);
      cairo_restore(cr);
||||||| merged common ancestors
void
draw_window_of_widget(GtkWidget *widget, GdkWindow *aWindow, cairo_t *cr)
{
    if (gtk_cairo_should_draw_window(cr, aWindow)) {
        RefPtr<nsWindow> window = get_window_for_gdk_window(aWindow);
        if (!window) {
            NS_WARNING("Cannot get nsWindow from GtkWidget");
        }
        else {
            cairo_save(cr);
            gtk_cairo_transform_to_window(cr, widget, aWindow);
            // TODO - window->OnExposeEvent() can destroy this or other windows,
            // do we need to handle it somehow?
            window->OnExposeEvent(cr);
            cairo_restore(cr);
        }
=======
void draw_window_of_widget(GtkWidget* widget, GdkWindow* aWindow, cairo_t* cr) {
  if (gtk_cairo_should_draw_window(cr, aWindow)) {
    RefPtr<nsWindow> window = get_window_for_gdk_window(aWindow);
    if (!window) {
      NS_WARNING("Cannot get nsWindow from GtkWidget");
    } else {
      cairo_save(cr);
      gtk_cairo_transform_to_window(cr, widget, aWindow);
      // TODO - window->OnExposeEvent() can destroy this or other windows,
      // do we need to handle it somehow?
      window->OnExposeEvent(cr);
      cairo_restore(cr);
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  GList *children = gdk_window_get_children(aWindow);
  GList *child = children;
  while (child) {
    GdkWindow *window = GDK_WINDOW(child->data);
    gpointer windowWidget;
    gdk_window_get_user_data(window, &windowWidget);
    if (windowWidget == widget) {
      draw_window_of_widget(widget, window, cr);
||||||| merged common ancestors
    GList *children = gdk_window_get_children(aWindow);
    GList *child = children;
    while (child) {
        GdkWindow *window = GDK_WINDOW(child->data);
        gpointer windowWidget;
        gdk_window_get_user_data(window, &windowWidget);
        if (windowWidget == widget) {
            draw_window_of_widget(widget, window, cr);
        }
        child = g_list_next(child);
=======
  GList* children = gdk_window_get_children(aWindow);
  GList* child = children;
  while (child) {
    GdkWindow* window = GDK_WINDOW(child->data);
    gpointer windowWidget;
    gdk_window_get_user_data(window, &windowWidget);
    if (windowWidget == widget) {
      draw_window_of_widget(widget, window, cr);
>>>>>>> upstream-releases
    }
    child = g_list_next(child);
  }
  g_list_free(children);
}

/* static */
<<<<<<< HEAD
gboolean expose_event_cb(GtkWidget *widget, cairo_t *cr) {
  draw_window_of_widget(widget, gtk_widget_get_window(widget), cr);

  // A strong reference is already held during "draw" signal emission,
  // but GTK+ 3.4 wants the object to live a little longer than that
  // (bug 1225970).
  g_object_ref(widget);
  g_idle_add(
      [](gpointer data) -> gboolean {
        g_object_unref(data);
        return G_SOURCE_REMOVE;
      },
      widget);
||||||| merged common ancestors
gboolean
expose_event_cb(GtkWidget *widget, cairo_t *cr)
{
    draw_window_of_widget(widget, gtk_widget_get_window(widget), cr);

    // A strong reference is already held during "draw" signal emission,
    // but GTK+ 3.4 wants the object to live a little longer than that
    // (bug 1225970).
    g_object_ref(widget);
    g_idle_add(
        [](gpointer data) -> gboolean {
            g_object_unref(data);
            return G_SOURCE_REMOVE;
        },
        widget);
=======
gboolean expose_event_cb(GtkWidget* widget, cairo_t* cr) {
  draw_window_of_widget(widget, gtk_widget_get_window(widget), cr);

  // A strong reference is already held during "draw" signal emission,
  // but GTK+ 3.4 wants the object to live a little longer than that
  // (bug 1225970).
  g_object_ref(widget);
  g_idle_add(
      [](gpointer data) -> gboolean {
        g_object_unref(data);
        return G_SOURCE_REMOVE;
      },
      widget);
>>>>>>> upstream-releases

  return FALSE;
}

<<<<<<< HEAD
static gboolean configure_event_cb(GtkWidget *widget,
                                   GdkEventConfigure *event) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) return FALSE;
||||||| merged common ancestors
static gboolean
configure_event_cb(GtkWidget *widget,
                   GdkEventConfigure *event)
{
    RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
    if (!window)
        return FALSE;
=======
static gboolean configure_event_cb(GtkWidget* widget,
                                   GdkEventConfigure* event) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) return FALSE;
>>>>>>> upstream-releases

  return window->OnConfigureEvent(widget, event);
}

<<<<<<< HEAD
static void container_unrealize_cb(GtkWidget *widget) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) return;
||||||| merged common ancestors
static void
container_unrealize_cb (GtkWidget *widget)
{
    RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
    if (!window)
        return;
=======
static void container_unrealize_cb(GtkWidget* widget) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) return;
>>>>>>> upstream-releases

  window->OnContainerUnrealize();
}

<<<<<<< HEAD
static void size_allocate_cb(GtkWidget *widget, GtkAllocation *allocation) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) return;
||||||| merged common ancestors
static void
size_allocate_cb (GtkWidget *widget, GtkAllocation *allocation)
{
    RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
    if (!window)
        return;
=======
static void size_allocate_cb(GtkWidget* widget, GtkAllocation* allocation) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) return;
>>>>>>> upstream-releases

  window->OnSizeAllocate(allocation);
}

<<<<<<< HEAD
static gboolean delete_event_cb(GtkWidget *widget, GdkEventAny *event) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) return FALSE;
||||||| merged common ancestors
static gboolean
delete_event_cb(GtkWidget *widget, GdkEventAny *event)
{
    RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
    if (!window)
        return FALSE;
=======
static gboolean delete_event_cb(GtkWidget* widget, GdkEventAny* event) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) return FALSE;
>>>>>>> upstream-releases

  window->OnDeleteEvent();

  return TRUE;
}

<<<<<<< HEAD
static gboolean enter_notify_event_cb(GtkWidget *widget,
                                      GdkEventCrossing *event) {
  RefPtr<nsWindow> window = get_window_for_gdk_window(event->window);
  if (!window) return TRUE;
||||||| merged common ancestors
static gboolean
enter_notify_event_cb(GtkWidget *widget,
                      GdkEventCrossing *event)
{
    RefPtr<nsWindow> window = get_window_for_gdk_window(event->window);
    if (!window)
        return TRUE;
=======
static gboolean enter_notify_event_cb(GtkWidget* widget,
                                      GdkEventCrossing* event) {
  RefPtr<nsWindow> window = get_window_for_gdk_window(event->window);
  if (!window) return TRUE;
>>>>>>> upstream-releases

  window->OnEnterNotifyEvent(event);

  return TRUE;
}

<<<<<<< HEAD
static gboolean leave_notify_event_cb(GtkWidget *widget,
                                      GdkEventCrossing *event) {
  if (is_parent_grab_leave(event)) {
    return TRUE;
  }
||||||| merged common ancestors
static gboolean
leave_notify_event_cb(GtkWidget *widget,
                      GdkEventCrossing *event)
{
    if (is_parent_grab_leave(event)) {
        return TRUE;
    }
=======
static gboolean leave_notify_event_cb(GtkWidget* widget,
                                      GdkEventCrossing* event) {
  if (is_parent_grab_leave(event)) {
    return TRUE;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // bug 369599: Suppress LeaveNotify events caused by pointer grabs to
  // avoid generating spurious mouse exit events.
  auto x = gint(event->x_root);
  auto y = gint(event->y_root);
  GdkDisplay *display = gtk_widget_get_display(widget);
  GdkWindow *winAtPt = gdk_display_get_window_at_pointer(display, &x, &y);
  if (winAtPt == event->window) {
    return TRUE;
  }
||||||| merged common ancestors
    // bug 369599: Suppress LeaveNotify events caused by pointer grabs to
    // avoid generating spurious mouse exit events.
    auto x = gint(event->x_root);
    auto y = gint(event->y_root);
    GdkDisplay* display = gtk_widget_get_display(widget);
    GdkWindow* winAtPt = gdk_display_get_window_at_pointer(display, &x, &y);
    if (winAtPt == event->window) {
        return TRUE;
    }
=======
  // bug 369599: Suppress LeaveNotify events caused by pointer grabs to
  // avoid generating spurious mouse exit events.
  auto x = gint(event->x_root);
  auto y = gint(event->y_root);
  GdkDisplay* display = gtk_widget_get_display(widget);
  GdkWindow* winAtPt = gdk_display_get_window_at_pointer(display, &x, &y);
  if (winAtPt == event->window) {
    return TRUE;
  }
>>>>>>> upstream-releases

  RefPtr<nsWindow> window = get_window_for_gdk_window(event->window);
  if (!window) return TRUE;

  window->OnLeaveNotifyEvent(event);

  return TRUE;
}

<<<<<<< HEAD
static nsWindow *GetFirstNSWindowForGDKWindow(GdkWindow *aGdkWindow) {
  nsWindow *window;
  while (!(window = get_window_for_gdk_window(aGdkWindow))) {
    // The event has bubbled to the moz_container widget as passed into each
    // caller's *widget parameter, but its corresponding nsWindow is an ancestor
    // of the window that we need.  Instead, look at event->window and find the
    // first ancestor nsWindow of it because event->window may be in a plugin.
    aGdkWindow = gdk_window_get_parent(aGdkWindow);
    if (!aGdkWindow) {
      window = nullptr;
      break;
||||||| merged common ancestors
static nsWindow*
GetFirstNSWindowForGDKWindow(GdkWindow *aGdkWindow)
{
    nsWindow* window;
    while (!(window = get_window_for_gdk_window(aGdkWindow))) {
        // The event has bubbled to the moz_container widget as passed into each caller's *widget parameter,
        // but its corresponding nsWindow is an ancestor of the window that we need.  Instead, look at
        // event->window and find the first ancestor nsWindow of it because event->window may be in a plugin.
        aGdkWindow = gdk_window_get_parent(aGdkWindow);
        if (!aGdkWindow) {
            window = nullptr;
            break;
        }
=======
static nsWindow* GetFirstNSWindowForGDKWindow(GdkWindow* aGdkWindow) {
  nsWindow* window;
  while (!(window = get_window_for_gdk_window(aGdkWindow))) {
    // The event has bubbled to the moz_container widget as passed into each
    // caller's *widget parameter, but its corresponding nsWindow is an ancestor
    // of the window that we need.  Instead, look at event->window and find the
    // first ancestor nsWindow of it because event->window may be in a plugin.
    aGdkWindow = gdk_window_get_parent(aGdkWindow);
    if (!aGdkWindow) {
      window = nullptr;
      break;
>>>>>>> upstream-releases
    }
  }
  return window;
}

<<<<<<< HEAD
static gboolean motion_notify_event_cb(GtkWidget *widget,
                                       GdkEventMotion *event) {
  UpdateLastInputEventTime(event);
||||||| merged common ancestors
static gboolean
motion_notify_event_cb(GtkWidget *widget, GdkEventMotion *event)
{
    UpdateLastInputEventTime(event);
=======
static gboolean motion_notify_event_cb(GtkWidget* widget,
                                       GdkEventMotion* event) {
  UpdateLastInputEventTime(event);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsWindow *window = GetFirstNSWindowForGDKWindow(event->window);
  if (!window) return FALSE;
||||||| merged common ancestors
    nsWindow *window = GetFirstNSWindowForGDKWindow(event->window);
    if (!window)
        return FALSE;
=======
  nsWindow* window = GetFirstNSWindowForGDKWindow(event->window);
  if (!window) return FALSE;
>>>>>>> upstream-releases

  window->OnMotionNotifyEvent(event);

  return TRUE;
}

<<<<<<< HEAD
static gboolean button_press_event_cb(GtkWidget *widget,
                                      GdkEventButton *event) {
  UpdateLastInputEventTime(event);
||||||| merged common ancestors
static gboolean
button_press_event_cb(GtkWidget *widget, GdkEventButton *event)
{
    UpdateLastInputEventTime(event);
=======
static gboolean button_press_event_cb(GtkWidget* widget,
                                      GdkEventButton* event) {
  UpdateLastInputEventTime(event);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsWindow *window = GetFirstNSWindowForGDKWindow(event->window);
  if (!window) return FALSE;
||||||| merged common ancestors
    nsWindow *window = GetFirstNSWindowForGDKWindow(event->window);
    if (!window)
        return FALSE;
=======
  nsWindow* window = GetFirstNSWindowForGDKWindow(event->window);
  if (!window) return FALSE;
>>>>>>> upstream-releases

  window->OnButtonPressEvent(event);

  return TRUE;
}

<<<<<<< HEAD
static gboolean button_release_event_cb(GtkWidget *widget,
                                        GdkEventButton *event) {
  UpdateLastInputEventTime(event);
||||||| merged common ancestors
static gboolean
button_release_event_cb(GtkWidget *widget, GdkEventButton *event)
{
    UpdateLastInputEventTime(event);
=======
static gboolean button_release_event_cb(GtkWidget* widget,
                                        GdkEventButton* event) {
  UpdateLastInputEventTime(event);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsWindow *window = GetFirstNSWindowForGDKWindow(event->window);
  if (!window) return FALSE;
||||||| merged common ancestors
    nsWindow *window = GetFirstNSWindowForGDKWindow(event->window);
    if (!window)
        return FALSE;
=======
  nsWindow* window = GetFirstNSWindowForGDKWindow(event->window);
  if (!window) return FALSE;
>>>>>>> upstream-releases

  window->OnButtonReleaseEvent(event);

  return TRUE;
}

<<<<<<< HEAD
static gboolean focus_in_event_cb(GtkWidget *widget, GdkEventFocus *event) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) return FALSE;
||||||| merged common ancestors
static gboolean
focus_in_event_cb(GtkWidget *widget, GdkEventFocus *event)
{
    RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
    if (!window)
        return FALSE;
=======
static gboolean focus_in_event_cb(GtkWidget* widget, GdkEventFocus* event) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) return FALSE;
>>>>>>> upstream-releases

  window->OnContainerFocusInEvent(event);

  return FALSE;
}

<<<<<<< HEAD
static gboolean focus_out_event_cb(GtkWidget *widget, GdkEventFocus *event) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) return FALSE;
||||||| merged common ancestors
static gboolean
focus_out_event_cb(GtkWidget *widget, GdkEventFocus *event)
{
    RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
    if (!window)
        return FALSE;
=======
static gboolean focus_out_event_cb(GtkWidget* widget, GdkEventFocus* event) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) return FALSE;
>>>>>>> upstream-releases

  window->OnContainerFocusOutEvent(event);

  return FALSE;
}

#ifdef MOZ_X11
// For long-lived popup windows that don't really take focus themselves but
// may have elements that accept keyboard input when the parent window is
// active, focus is handled specially.  These windows include noautohide
// panels.  (This special handling is not necessary for temporary popups where
// the keyboard is grabbed.)
//
// Mousing over or clicking on these windows should not cause them to steal
// focus from their parent windows, so, the input field of WM_HINTS is set to
// False to request that the window manager not set the input focus to this
// window.  http://tronche.com/gui/x/icccm/sec-4.html#s-4.1.7
//
// However, these windows can still receive WM_TAKE_FOCUS messages from the
// window manager, so they can still detect when the user has indicated that
// they wish to direct keyboard input at these windows.  When the window
// manager offers focus to these windows (after a mouse over or click, for
// example), a request to make the parent window active is issued.  When the
// parent window becomes active, keyboard events will be received.

<<<<<<< HEAD
static GdkFilterReturn popup_take_focus_filter(GdkXEvent *gdk_xevent,
                                               GdkEvent *event, gpointer data) {
  auto *xevent = static_cast<XEvent *>(gdk_xevent);
  if (xevent->type != ClientMessage) return GDK_FILTER_CONTINUE;
||||||| merged common ancestors
static GdkFilterReturn
popup_take_focus_filter(GdkXEvent *gdk_xevent,
                        GdkEvent *event,
                        gpointer data)
{
    auto* xevent = static_cast<XEvent*>(gdk_xevent);
    if (xevent->type != ClientMessage)
        return GDK_FILTER_CONTINUE;
=======
static GdkFilterReturn popup_take_focus_filter(GdkXEvent* gdk_xevent,
                                               GdkEvent* event, gpointer data) {
  auto* xevent = static_cast<XEvent*>(gdk_xevent);
  if (xevent->type != ClientMessage) return GDK_FILTER_CONTINUE;
>>>>>>> upstream-releases

<<<<<<< HEAD
  XClientMessageEvent &xclient = xevent->xclient;
  if (xclient.message_type != gdk_x11_get_xatom_by_name("WM_PROTOCOLS"))
    return GDK_FILTER_CONTINUE;
||||||| merged common ancestors
    XClientMessageEvent& xclient = xevent->xclient;
    if (xclient.message_type != gdk_x11_get_xatom_by_name("WM_PROTOCOLS"))
        return GDK_FILTER_CONTINUE;
=======
  XClientMessageEvent& xclient = xevent->xclient;
  if (xclient.message_type != gdk_x11_get_xatom_by_name("WM_PROTOCOLS"))
    return GDK_FILTER_CONTINUE;
>>>>>>> upstream-releases

  Atom atom = xclient.data.l[0];
  if (atom != gdk_x11_get_xatom_by_name("WM_TAKE_FOCUS"))
    return GDK_FILTER_CONTINUE;

  guint32 timestamp = xclient.data.l[1];

<<<<<<< HEAD
  GtkWidget *widget = get_gtk_widget_for_gdk_window(event->any.window);
  if (!widget) return GDK_FILTER_CONTINUE;
||||||| merged common ancestors
    GtkWidget* widget = get_gtk_widget_for_gdk_window(event->any.window);
    if (!widget)
        return GDK_FILTER_CONTINUE;
=======
  GtkWidget* widget = get_gtk_widget_for_gdk_window(event->any.window);
  if (!widget) return GDK_FILTER_CONTINUE;
>>>>>>> upstream-releases

<<<<<<< HEAD
  GtkWindow *parent = gtk_window_get_transient_for(GTK_WINDOW(widget));
  if (!parent) return GDK_FILTER_CONTINUE;
||||||| merged common ancestors
    GtkWindow* parent = gtk_window_get_transient_for(GTK_WINDOW(widget));
    if (!parent)
        return GDK_FILTER_CONTINUE;
=======
  GtkWindow* parent = gtk_window_get_transient_for(GTK_WINDOW(widget));
  if (!parent) return GDK_FILTER_CONTINUE;
>>>>>>> upstream-releases

  if (gtk_window_is_active(parent))
    return GDK_FILTER_REMOVE;  // leave input focus on the parent

<<<<<<< HEAD
  GdkWindow *parent_window = gtk_widget_get_window(GTK_WIDGET(parent));
  if (!parent_window) return GDK_FILTER_CONTINUE;
||||||| merged common ancestors
    GdkWindow* parent_window = gtk_widget_get_window(GTK_WIDGET(parent));
    if (!parent_window)
        return GDK_FILTER_CONTINUE;
=======
  GdkWindow* parent_window = gtk_widget_get_window(GTK_WIDGET(parent));
  if (!parent_window) return GDK_FILTER_CONTINUE;
>>>>>>> upstream-releases

  // In case the parent has not been deconified.
  gdk_window_show_unraised(parent_window);

  // Request focus on the parent window.
  // Use gdk_window_focus rather than gtk_window_present to avoid
  // raising the parent window.
  gdk_window_focus(parent_window, timestamp);
  return GDK_FILTER_REMOVE;
}
#endif /* MOZ_X11 */

<<<<<<< HEAD
static gboolean key_press_event_cb(GtkWidget *widget, GdkEventKey *event) {
  LOG(("key_press_event_cb\n"));
||||||| merged common ancestors
static gboolean
key_press_event_cb(GtkWidget *widget, GdkEventKey *event)
{
    LOG(("key_press_event_cb\n"));
=======
static gboolean key_press_event_cb(GtkWidget* widget, GdkEventKey* event) {
  LOG(("key_press_event_cb\n"));
>>>>>>> upstream-releases

  UpdateLastInputEventTime(event);

<<<<<<< HEAD
  // find the window with focus and dispatch this event to that widget
  nsWindow *window = get_window_for_gtk_widget(widget);
  if (!window) return FALSE;
||||||| merged common ancestors
    // find the window with focus and dispatch this event to that widget
    nsWindow *window = get_window_for_gtk_widget(widget);
    if (!window)
        return FALSE;
=======
  // find the window with focus and dispatch this event to that widget
  nsWindow* window = get_window_for_gtk_widget(widget);
  if (!window) return FALSE;
>>>>>>> upstream-releases

  RefPtr<nsWindow> focusWindow = gFocusWindow ? gFocusWindow : window;

#ifdef MOZ_X11
<<<<<<< HEAD
  // Keyboard repeat can cause key press events to queue up when there are
  // slow event handlers (bug 301029).  Throttle these events by removing
  // consecutive pending duplicate KeyPress events to the same window.
  // We use the event time of the last one.
  // Note: GDK calls XkbSetDetectableAutorepeat so that KeyRelease events
  // are generated only when the key is physically released.
#define NS_GDKEVENT_MATCH_MASK 0x1FFF /* GDK_SHIFT_MASK .. GDK_BUTTON5_MASK */
  GdkDisplay *gdkDisplay = gtk_widget_get_display(widget);
  if (GDK_IS_X11_DISPLAY(gdkDisplay)) {
    Display *dpy = GDK_DISPLAY_XDISPLAY(gdkDisplay);
    while (XPending(dpy)) {
      XEvent next_event;
      XPeekEvent(dpy, &next_event);
      GdkWindow *nextGdkWindow =
          gdk_x11_window_lookup_for_display(gdkDisplay, next_event.xany.window);
      if (nextGdkWindow != event->window || next_event.type != KeyPress ||
          next_event.xkey.keycode != event->hardware_keycode ||
          next_event.xkey.state != (event->state & NS_GDKEVENT_MATCH_MASK)) {
        break;
      }
      XNextEvent(dpy, &next_event);
      event->time = next_event.xkey.time;
||||||| merged common ancestors
    // Keyboard repeat can cause key press events to queue up when there are
    // slow event handlers (bug 301029).  Throttle these events by removing
    // consecutive pending duplicate KeyPress events to the same window.
    // We use the event time of the last one.
    // Note: GDK calls XkbSetDetectableAutorepeat so that KeyRelease events
    // are generated only when the key is physically released.
#define NS_GDKEVENT_MATCH_MASK 0x1FFF /* GDK_SHIFT_MASK .. GDK_BUTTON5_MASK */
    GdkDisplay* gdkDisplay = gtk_widget_get_display(widget);
    if (GDK_IS_X11_DISPLAY(gdkDisplay)) {
        Display* dpy = GDK_DISPLAY_XDISPLAY(gdkDisplay);
        while (XPending(dpy)) {
            XEvent next_event;
            XPeekEvent(dpy, &next_event);
            GdkWindow* nextGdkWindow =
                gdk_x11_window_lookup_for_display(gdkDisplay, next_event.xany.window);
            if (nextGdkWindow != event->window ||
                next_event.type != KeyPress ||
                next_event.xkey.keycode != event->hardware_keycode ||
                next_event.xkey.state != (event->state & NS_GDKEVENT_MATCH_MASK)) {
                break;
            }
            XNextEvent(dpy, &next_event);
            event->time = next_event.xkey.time;
        }
=======
  // Keyboard repeat can cause key press events to queue up when there are
  // slow event handlers (bug 301029).  Throttle these events by removing
  // consecutive pending duplicate KeyPress events to the same window.
  // We use the event time of the last one.
  // Note: GDK calls XkbSetDetectableAutorepeat so that KeyRelease events
  // are generated only when the key is physically released.
#  define NS_GDKEVENT_MATCH_MASK 0x1FFF  // GDK_SHIFT_MASK .. GDK_BUTTON5_MASK
  // Our headers undefine X11 KeyPress - let's redefine it here.
#  ifndef KeyPress
#    define KeyPress 2
#  endif
  GdkDisplay* gdkDisplay = gtk_widget_get_display(widget);
  if (GDK_IS_X11_DISPLAY(gdkDisplay)) {
    Display* dpy = GDK_DISPLAY_XDISPLAY(gdkDisplay);
    while (XPending(dpy)) {
      XEvent next_event;
      XPeekEvent(dpy, &next_event);
      GdkWindow* nextGdkWindow =
          gdk_x11_window_lookup_for_display(gdkDisplay, next_event.xany.window);
      if (nextGdkWindow != event->window || next_event.type != KeyPress ||
          next_event.xkey.keycode != event->hardware_keycode ||
          next_event.xkey.state != (event->state & NS_GDKEVENT_MATCH_MASK)) {
        break;
      }
      XNextEvent(dpy, &next_event);
      event->time = next_event.xkey.time;
>>>>>>> upstream-releases
    }
  }
#endif

  return focusWindow->OnKeyPressEvent(event);
}

<<<<<<< HEAD
static gboolean key_release_event_cb(GtkWidget *widget, GdkEventKey *event) {
  LOG(("key_release_event_cb\n"));
||||||| merged common ancestors
static gboolean
key_release_event_cb(GtkWidget *widget, GdkEventKey *event)
{
    LOG(("key_release_event_cb\n"));
=======
static gboolean key_release_event_cb(GtkWidget* widget, GdkEventKey* event) {
  LOG(("key_release_event_cb\n"));
>>>>>>> upstream-releases

  UpdateLastInputEventTime(event);

<<<<<<< HEAD
  // find the window with focus and dispatch this event to that widget
  nsWindow *window = get_window_for_gtk_widget(widget);
  if (!window) return FALSE;
||||||| merged common ancestors
    // find the window with focus and dispatch this event to that widget
    nsWindow *window = get_window_for_gtk_widget(widget);
    if (!window)
        return FALSE;
=======
  // find the window with focus and dispatch this event to that widget
  nsWindow* window = get_window_for_gtk_widget(widget);
  if (!window) return FALSE;
>>>>>>> upstream-releases

  RefPtr<nsWindow> focusWindow = gFocusWindow ? gFocusWindow : window;

  return focusWindow->OnKeyReleaseEvent(event);
}

<<<<<<< HEAD
static gboolean property_notify_event_cb(GtkWidget *aWidget,
                                         GdkEventProperty *aEvent) {
  RefPtr<nsWindow> window = get_window_for_gdk_window(aEvent->window);
  if (!window) return FALSE;
||||||| merged common ancestors
static gboolean
property_notify_event_cb(GtkWidget* aWidget, GdkEventProperty* aEvent)
{
    RefPtr<nsWindow> window = get_window_for_gdk_window(aEvent->window);
    if (!window)
        return FALSE;
=======
static gboolean property_notify_event_cb(GtkWidget* aWidget,
                                         GdkEventProperty* aEvent) {
  RefPtr<nsWindow> window = get_window_for_gdk_window(aEvent->window);
  if (!window) return FALSE;
>>>>>>> upstream-releases

  return window->OnPropertyNotifyEvent(aWidget, aEvent);
}

<<<<<<< HEAD
static gboolean scroll_event_cb(GtkWidget *widget, GdkEventScroll *event) {
  nsWindow *window = GetFirstNSWindowForGDKWindow(event->window);
  if (!window) return FALSE;
||||||| merged common ancestors
static gboolean
scroll_event_cb(GtkWidget *widget, GdkEventScroll *event)
{
    nsWindow *window = GetFirstNSWindowForGDKWindow(event->window);
    if (!window)
        return FALSE;
=======
static gboolean scroll_event_cb(GtkWidget* widget, GdkEventScroll* event) {
  nsWindow* window = GetFirstNSWindowForGDKWindow(event->window);
  if (!window) return FALSE;
>>>>>>> upstream-releases

  window->OnScrollEvent(event);

  return TRUE;
}

<<<<<<< HEAD
static gboolean visibility_notify_event_cb(GtkWidget *widget,
                                           GdkEventVisibility *event) {
  RefPtr<nsWindow> window = get_window_for_gdk_window(event->window);
  if (!window) return FALSE;
||||||| merged common ancestors
static gboolean
visibility_notify_event_cb (GtkWidget *widget, GdkEventVisibility *event)
{
    RefPtr<nsWindow> window = get_window_for_gdk_window(event->window);
    if (!window)
        return FALSE;
=======
static gboolean visibility_notify_event_cb(GtkWidget* widget,
                                           GdkEventVisibility* event) {
  RefPtr<nsWindow> window = get_window_for_gdk_window(event->window);
  if (!window) return FALSE;
>>>>>>> upstream-releases

  window->OnVisibilityNotifyEvent(event);

  return TRUE;
}

<<<<<<< HEAD
static void hierarchy_changed_cb(GtkWidget *widget,
                                 GtkWidget *previous_toplevel) {
  GtkWidget *toplevel = gtk_widget_get_toplevel(widget);
  GdkWindowState old_window_state = GDK_WINDOW_STATE_WITHDRAWN;
  GdkEventWindowState event;

  event.new_window_state = GDK_WINDOW_STATE_WITHDRAWN;

  if (GTK_IS_WINDOW(previous_toplevel)) {
    g_signal_handlers_disconnect_by_func(
        previous_toplevel, FuncToGpointer(window_state_event_cb), widget);
    GdkWindow *win = gtk_widget_get_window(previous_toplevel);
    if (win) {
      old_window_state = gdk_window_get_state(win);
||||||| merged common ancestors
static void
hierarchy_changed_cb (GtkWidget *widget,
                      GtkWidget *previous_toplevel)
{
    GtkWidget *toplevel = gtk_widget_get_toplevel(widget);
    GdkWindowState old_window_state = GDK_WINDOW_STATE_WITHDRAWN;
    GdkEventWindowState event;

    event.new_window_state = GDK_WINDOW_STATE_WITHDRAWN;

    if (GTK_IS_WINDOW(previous_toplevel)) {
        g_signal_handlers_disconnect_by_func(previous_toplevel,
                                             FuncToGpointer(window_state_event_cb),
                                             widget);
        GdkWindow *win = gtk_widget_get_window(previous_toplevel);
        if (win) {
            old_window_state = gdk_window_get_state(win);
        }
=======
static void hierarchy_changed_cb(GtkWidget* widget,
                                 GtkWidget* previous_toplevel) {
  GtkWidget* toplevel = gtk_widget_get_toplevel(widget);
  GdkWindowState old_window_state = GDK_WINDOW_STATE_WITHDRAWN;
  GdkEventWindowState event;

  event.new_window_state = GDK_WINDOW_STATE_WITHDRAWN;

  if (GTK_IS_WINDOW(previous_toplevel)) {
    g_signal_handlers_disconnect_by_func(
        previous_toplevel, FuncToGpointer(window_state_event_cb), widget);
    GdkWindow* win = gtk_widget_get_window(previous_toplevel);
    if (win) {
      old_window_state = gdk_window_get_state(win);
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  if (GTK_IS_WINDOW(toplevel)) {
    g_signal_connect_swapped(toplevel, "window-state-event",
                             G_CALLBACK(window_state_event_cb), widget);
    GdkWindow *win = gtk_widget_get_window(toplevel);
    if (win) {
      event.new_window_state = gdk_window_get_state(win);
||||||| merged common ancestors
    if (GTK_IS_WINDOW(toplevel)) {
        g_signal_connect_swapped(toplevel, "window-state-event",
                                 G_CALLBACK(window_state_event_cb), widget);
        GdkWindow *win = gtk_widget_get_window(toplevel);
        if (win) {
            event.new_window_state = gdk_window_get_state(win);
        }
=======
  if (GTK_IS_WINDOW(toplevel)) {
    g_signal_connect_swapped(toplevel, "window-state-event",
                             G_CALLBACK(window_state_event_cb), widget);
    GdkWindow* win = gtk_widget_get_window(toplevel);
    if (win) {
      event.new_window_state = gdk_window_get_state(win);
>>>>>>> upstream-releases
    }
  }

  event.changed_mask =
      static_cast<GdkWindowState>(old_window_state ^ event.new_window_state);

  if (event.changed_mask) {
    event.type = GDK_WINDOW_STATE;
    event.window = nullptr;
    event.send_event = TRUE;
    window_state_event_cb(widget, &event);
  }
}

<<<<<<< HEAD
static gboolean window_state_event_cb(GtkWidget *widget,
                                      GdkEventWindowState *event) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) return FALSE;
||||||| merged common ancestors
static gboolean
window_state_event_cb (GtkWidget *widget, GdkEventWindowState *event)
{
    RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
    if (!window)
        return FALSE;
=======
static gboolean window_state_event_cb(GtkWidget* widget,
                                      GdkEventWindowState* event) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) return FALSE;
>>>>>>> upstream-releases

  window->OnWindowStateEvent(widget, event);

  return FALSE;
}

<<<<<<< HEAD
static void settings_changed_cb(GtkSettings *settings, GParamSpec *pspec,
                                nsWindow *data) {
  RefPtr<nsWindow> window = data;
  window->ThemeChanged();
||||||| merged common ancestors
static void
settings_changed_cb (GtkSettings *settings, GParamSpec *pspec, nsWindow *data)
{
    RefPtr<nsWindow> window = data;
    window->ThemeChanged();
=======
static void settings_changed_cb(GtkSettings* settings, GParamSpec* pspec,
                                nsWindow* data) {
  RefPtr<nsWindow> window = data;
  window->ThemeChanged();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void check_resize_cb(GtkContainer *container, gpointer user_data) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(GTK_WIDGET(container));
  if (!window) {
    return;
  }
  window->OnCheckResize();
||||||| merged common ancestors
static void
check_resize_cb (GtkContainer* container, gpointer user_data)
{
    RefPtr<nsWindow> window = get_window_for_gtk_widget(GTK_WIDGET(container));
    if (!window) {
      return;
    }
    window->OnCheckResize();
=======
static void check_resize_cb(GtkContainer* container, gpointer user_data) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(GTK_WIDGET(container));
  if (!window) {
    return;
  }
  window->OnCheckResize();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void screen_composited_changed_cb(GdkScreen *screen,
                                         gpointer user_data) {
  // This callback can run before gfxPlatform::Init() in rare
  // cases involving the profile manager. When this happens,
  // we have no reason to reset any compositors as graphics
  // hasn't been initialized yet.
  if (GPUProcessManager::Get()) {
    GPUProcessManager::Get()->ResetCompositors();
  }
||||||| merged common ancestors
static void
screen_composited_changed_cb (GdkScreen* screen, gpointer user_data)
{
    // This callback can run before gfxPlatform::Init() in rare
    // cases involving the profile manager. When this happens,
    // we have no reason to reset any compositors as graphics
    // hasn't been initialized yet.
    if (GPUProcessManager::Get()) {
        GPUProcessManager::Get()->ResetCompositors();
    }
=======
static void screen_composited_changed_cb(GdkScreen* screen,
                                         gpointer user_data) {
  // This callback can run before gfxPlatform::Init() in rare
  // cases involving the profile manager. When this happens,
  // we have no reason to reset any compositors as graphics
  // hasn't been initialized yet.
  if (GPUProcessManager::Get()) {
    GPUProcessManager::Get()->ResetCompositors();
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void widget_composited_changed_cb(GtkWidget *widget,
                                         gpointer user_data) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) {
    return;
  }
  window->OnCompositedChanged();
||||||| merged common ancestors
static void
widget_composited_changed_cb (GtkWidget* widget, gpointer user_data)
{
    RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
    if (!window) {
      return;
    }
    window->OnCompositedChanged();
=======
static void widget_composited_changed_cb(GtkWidget* widget,
                                         gpointer user_data) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) {
    return;
  }
  window->OnCompositedChanged();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void scale_changed_cb(GtkWidget *widget, GParamSpec *aPSpec,
                             gpointer aPointer) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) {
    return;
  }
  // This eventually propagate new scale to the PuppetWidgets
  window->OnDPIChanged();
||||||| merged common ancestors
static void
scale_changed_cb (GtkWidget* widget, GParamSpec* aPSpec, gpointer aPointer)
{
    RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
    if (!window) {
      return;
    }
    // This eventually propagate new scale to the PuppetWidgets
    window->OnDPIChanged();
=======
static void scale_changed_cb(GtkWidget* widget, GParamSpec* aPSpec,
                             gpointer aPointer) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(widget);
  if (!window) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // configure_event is already fired before scale-factor signal,
  // but size-allocate isn't fired by changing scale
  GtkAllocation allocation;
  gtk_widget_get_allocation(widget, &allocation);
  window->OnSizeAllocate(&allocation);
||||||| merged common ancestors
    // configure_event is already fired before scale-factor signal,
    // but size-allocate isn't fired by changing scale
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    window->OnSizeAllocate(&allocation);
=======
  GtkAllocation allocation;
  gtk_widget_get_allocation(widget, &allocation);
  window->OnScaleChanged(&allocation);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
#if GTK_CHECK_VERSION(3, 4, 0)
static gboolean touch_event_cb(GtkWidget *aWidget, GdkEventTouch *aEvent) {
  UpdateLastInputEventTime(aEvent);
||||||| merged common ancestors
#if GTK_CHECK_VERSION(3,4,0)
static gboolean
touch_event_cb(GtkWidget* aWidget, GdkEventTouch* aEvent)
{
    UpdateLastInputEventTime(aEvent);
=======
#if GTK_CHECK_VERSION(3, 4, 0)
static gboolean touch_event_cb(GtkWidget* aWidget, GdkEventTouch* aEvent) {
  UpdateLastInputEventTime(aEvent);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsWindow *window = GetFirstNSWindowForGDKWindow(aEvent->window);
  if (!window) {
    return FALSE;
  }
||||||| merged common ancestors
    nsWindow* window = GetFirstNSWindowForGDKWindow(aEvent->window);
    if (!window) {
        return FALSE;
    }
=======
  nsWindow* window = GetFirstNSWindowForGDKWindow(aEvent->window);
  if (!window) {
    return FALSE;
  }
>>>>>>> upstream-releases

  return window->OnTouchEvent(aEvent);
}
#endif

//////////////////////////////////////////////////////////////////////
// These are all of our drag and drop operations

<<<<<<< HEAD
void nsWindow::InitDragEvent(WidgetDragEvent &aEvent) {
  // set the keyboard modifiers
  guint modifierState = KeymapWrapper::GetCurrentModifierState();
  KeymapWrapper::InitInputEvent(aEvent, modifierState);
}
||||||| merged common ancestors
void
nsWindow::InitDragEvent(WidgetDragEvent &aEvent)
{
    // set the keyboard modifiers
    guint modifierState = KeymapWrapper::GetCurrentModifierState();
    KeymapWrapper::InitInputEvent(aEvent, modifierState);
}

gboolean
WindowDragMotionHandler(GtkWidget *aWidget,
                        GdkDragContext *aDragContext,
                        nsWaylandDragContext *aWaylandDragContext,
                        gint aX,
                        gint aY,
                        guint aTime)
{
    RefPtr<nsWindow> window = get_window_for_gtk_widget(aWidget);
    if (!window)
        return FALSE;
=======
void nsWindow::InitDragEvent(WidgetDragEvent& aEvent) {
  // set the keyboard modifiers
  guint modifierState = KeymapWrapper::GetCurrentModifierState();
  KeymapWrapper::InitInputEvent(aEvent, modifierState);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
gboolean WindowDragMotionHandler(GtkWidget *aWidget,
                                 GdkDragContext *aDragContext,
                                 nsWaylandDragContext *aWaylandDragContext,
                                 gint aX, gint aY, guint aTime) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(aWidget);
  if (!window) return FALSE;
||||||| merged common ancestors
    // figure out which internal widget this drag motion actually happened on
    nscoord retx = 0;
    nscoord rety = 0;
=======
gboolean WindowDragMotionHandler(GtkWidget* aWidget,
                                 GdkDragContext* aDragContext,
                                 nsWaylandDragContext* aWaylandDragContext,
                                 gint aX, gint aY, guint aTime) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(aWidget);
  if (!window) return FALSE;
>>>>>>> upstream-releases

  // figure out which internal widget this drag motion actually happened on
  nscoord retx = 0;
  nscoord rety = 0;

<<<<<<< HEAD
  GdkWindow *innerWindow = get_inner_gdk_window(gtk_widget_get_window(aWidget),
                                                aX, aY, &retx, &rety);
  RefPtr<nsWindow> innerMostWindow = get_window_for_gdk_window(innerWindow);
||||||| merged common ancestors
    if (!innerMostWindow) {
        innerMostWindow = window;
    }
=======
  GdkWindow* innerWindow = get_inner_gdk_window(gtk_widget_get_window(aWidget),
                                                aX, aY, &retx, &rety);
  RefPtr<nsWindow> innerMostWindow = get_window_for_gdk_window(innerWindow);

  if (!innerMostWindow) {
    innerMostWindow = window;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!innerMostWindow) {
    innerMostWindow = window;
  }

  LOGDRAG(("nsWindow drag-motion signal for %p\n", (void *)innerMostWindow));
||||||| merged common ancestors
    LOGDRAG(("nsWindow drag-motion signal for %p\n", (void*)innerMostWindow));
=======
  LOGDRAG(("nsWindow drag-motion signal for %p\n", (void*)innerMostWindow));
>>>>>>> upstream-releases

  LayoutDeviceIntPoint point = window->GdkPointToDevicePixels({retx, rety});

  RefPtr<nsDragService> dragService = nsDragService::GetInstance();
  return dragService->ScheduleMotionEvent(innerMostWindow, aDragContext,
                                          aWaylandDragContext, point, aTime);
}

<<<<<<< HEAD
static gboolean drag_motion_event_cb(GtkWidget *aWidget,
                                     GdkDragContext *aDragContext, gint aX,
                                     gint aY, guint aTime, gpointer aData) {
  return WindowDragMotionHandler(aWidget, aDragContext, nullptr, aX, aY, aTime);
||||||| merged common ancestors
static gboolean
drag_motion_event_cb(GtkWidget *aWidget,
                     GdkDragContext *aDragContext,
                     gint aX,
                     gint aY,
                     guint aTime,
                     gpointer aData)
{
    return WindowDragMotionHandler(aWidget, aDragContext, nullptr,
                                   aX, aY, aTime);
=======
static gboolean drag_motion_event_cb(GtkWidget* aWidget,
                                     GdkDragContext* aDragContext, gint aX,
                                     gint aY, guint aTime, gpointer aData) {
  return WindowDragMotionHandler(aWidget, aDragContext, nullptr, aX, aY, aTime);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void WindowDragLeaveHandler(GtkWidget *aWidget) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(aWidget);
  if (!window) return;
||||||| merged common ancestors
void
WindowDragLeaveHandler(GtkWidget *aWidget)
{
    RefPtr<nsWindow> window = get_window_for_gtk_widget(aWidget);
    if (!window)
        return;

    RefPtr<nsDragService> dragService = nsDragService::GetInstance();

    nsWindow *mostRecentDragWindow = dragService->GetMostRecentDestWindow();
    if (!mostRecentDragWindow) {
        // This can happen when the target will not accept a drop.  A GTK drag
        // source sends the leave message to the destination before the
        // drag-failed signal on the source widget, but the leave message goes
        // via the X server, and so doesn't get processed at least until the
        // event loop runs again.
        return;
    }
=======
void WindowDragLeaveHandler(GtkWidget* aWidget) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(aWidget);
  if (!window) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
  RefPtr<nsDragService> dragService = nsDragService::GetInstance();

  nsWindow *mostRecentDragWindow = dragService->GetMostRecentDestWindow();
  if (!mostRecentDragWindow) {
    // This can happen when the target will not accept a drop.  A GTK drag
    // source sends the leave message to the destination before the
    // drag-failed signal on the source widget, but the leave message goes
    // via the X server, and so doesn't get processed at least until the
    // event loop runs again.
    return;
  }
||||||| merged common ancestors
    GtkWidget *mozContainer = mostRecentDragWindow->GetMozContainerWidget();
    if (aWidget != mozContainer)
    {
        // When the drag moves between widgets, GTK can send leave signal for
        // the old widget after the motion or drop signal for the new widget.
        // We'll send the leave event when the motion or drop event is run.
        return;
    }
=======
  RefPtr<nsDragService> dragService = nsDragService::GetInstance();

  nsWindow* mostRecentDragWindow = dragService->GetMostRecentDestWindow();
  if (!mostRecentDragWindow) {
    // This can happen when the target will not accept a drop.  A GTK drag
    // source sends the leave message to the destination before the
    // drag-failed signal on the source widget, but the leave message goes
    // via the X server, and so doesn't get processed at least until the
    // event loop runs again.
    return;
  }

  GtkWidget* mozContainer = mostRecentDragWindow->GetMozContainerWidget();
  if (aWidget != mozContainer) {
    // When the drag moves between widgets, GTK can send leave signal for
    // the old widget after the motion or drop signal for the new widget.
    // We'll send the leave event when the motion or drop event is run.
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  GtkWidget *mozContainer = mostRecentDragWindow->GetMozContainerWidget();
  if (aWidget != mozContainer) {
    // When the drag moves between widgets, GTK can send leave signal for
    // the old widget after the motion or drop signal for the new widget.
    // We'll send the leave event when the motion or drop event is run.
    return;
  }
||||||| merged common ancestors
    LOGDRAG(("nsWindow drag-leave signal for %p\n",
             (void*)mostRecentDragWindow));
=======
  LOGDRAG(("nsWindow drag-leave signal for %p\n", (void*)mostRecentDragWindow));
>>>>>>> upstream-releases

<<<<<<< HEAD
  LOGDRAG(
      ("nsWindow drag-leave signal for %p\n", (void *)mostRecentDragWindow));

  dragService->ScheduleLeaveEvent();
||||||| merged common ancestors
    dragService->ScheduleLeaveEvent();
=======
  dragService->ScheduleLeaveEvent();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void drag_leave_event_cb(GtkWidget *aWidget,
                                GdkDragContext *aDragContext, guint aTime,
                                gpointer aData) {
  WindowDragLeaveHandler(aWidget);
||||||| merged common ancestors
static void
drag_leave_event_cb(GtkWidget *aWidget,
                    GdkDragContext *aDragContext,
                    guint aTime,
                    gpointer aData)
{
    WindowDragLeaveHandler(aWidget);
=======
static void drag_leave_event_cb(GtkWidget* aWidget,
                                GdkDragContext* aDragContext, guint aTime,
                                gpointer aData) {
  WindowDragLeaveHandler(aWidget);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
gboolean WindowDragDropHandler(GtkWidget *aWidget, GdkDragContext *aDragContext,
                               nsWaylandDragContext *aWaylandDragContext,
                               gint aX, gint aY, guint aTime) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(aWidget);
  if (!window) return FALSE;
||||||| merged common ancestors
gboolean
WindowDragDropHandler(GtkWidget *aWidget,
                      GdkDragContext *aDragContext,
                      nsWaylandDragContext *aWaylandDragContext,
                      gint aX,
                      gint aY,
                      guint aTime)
{
    RefPtr<nsWindow> window = get_window_for_gtk_widget(aWidget);
    if (!window)
        return FALSE;
=======
gboolean WindowDragDropHandler(GtkWidget* aWidget, GdkDragContext* aDragContext,
                               nsWaylandDragContext* aWaylandDragContext,
                               gint aX, gint aY, guint aTime) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(aWidget);
  if (!window) return FALSE;
>>>>>>> upstream-releases

  // figure out which internal widget this drag motion actually happened on
  nscoord retx = 0;
  nscoord rety = 0;

<<<<<<< HEAD
  GdkWindow *innerWindow = get_inner_gdk_window(gtk_widget_get_window(aWidget),
                                                aX, aY, &retx, &rety);
  RefPtr<nsWindow> innerMostWindow = get_window_for_gdk_window(innerWindow);
||||||| merged common ancestors
    GdkWindow *innerWindow =
        get_inner_gdk_window(gtk_widget_get_window(aWidget), aX, aY,
                             &retx, &rety);
    RefPtr<nsWindow> innerMostWindow = get_window_for_gdk_window(innerWindow);
=======
  GdkWindow* innerWindow = get_inner_gdk_window(gtk_widget_get_window(aWidget),
                                                aX, aY, &retx, &rety);
  RefPtr<nsWindow> innerMostWindow = get_window_for_gdk_window(innerWindow);
>>>>>>> upstream-releases

  if (!innerMostWindow) {
    innerMostWindow = window;
  }

<<<<<<< HEAD
  LOGDRAG(("nsWindow drag-drop signal for %p\n", (void *)innerMostWindow));
||||||| merged common ancestors
    LOGDRAG(("nsWindow drag-drop signal for %p\n", (void*)innerMostWindow));
=======
  LOGDRAG(("nsWindow drag-drop signal for %p\n", (void*)innerMostWindow));
>>>>>>> upstream-releases

  LayoutDeviceIntPoint point = window->GdkPointToDevicePixels({retx, rety});

  RefPtr<nsDragService> dragService = nsDragService::GetInstance();
  return dragService->ScheduleDropEvent(innerMostWindow, aDragContext,
                                        aWaylandDragContext, point, aTime);
}

<<<<<<< HEAD
static gboolean drag_drop_event_cb(GtkWidget *aWidget,
                                   GdkDragContext *aDragContext, gint aX,
                                   gint aY, guint aTime, gpointer aData) {
  return WindowDragDropHandler(aWidget, aDragContext, nullptr, aX, aY, aTime);
||||||| merged common ancestors
static gboolean
drag_drop_event_cb(GtkWidget *aWidget,
                   GdkDragContext *aDragContext,
                   gint aX,
                   gint aY,
                   guint aTime,
                   gpointer aData)
{
    return WindowDragDropHandler(aWidget, aDragContext, nullptr, aX, aY, aTime);
=======
static gboolean drag_drop_event_cb(GtkWidget* aWidget,
                                   GdkDragContext* aDragContext, gint aX,
                                   gint aY, guint aTime, gpointer aData) {
  return WindowDragDropHandler(aWidget, aDragContext, nullptr, aX, aY, aTime);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void drag_data_received_event_cb(GtkWidget *aWidget,
                                        GdkDragContext *aDragContext, gint aX,
                                        gint aY,
                                        GtkSelectionData *aSelectionData,
                                        guint aInfo, guint aTime,
                                        gpointer aData) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(aWidget);
  if (!window) return;
||||||| merged common ancestors
static void
drag_data_received_event_cb(GtkWidget *aWidget,
                            GdkDragContext *aDragContext,
                            gint aX,
                            gint aY,
                            GtkSelectionData  *aSelectionData,
                            guint aInfo,
                            guint aTime,
                            gpointer aData)
{
    RefPtr<nsWindow> window = get_window_for_gtk_widget(aWidget);
    if (!window)
        return;
=======
static void drag_data_received_event_cb(GtkWidget* aWidget,
                                        GdkDragContext* aDragContext, gint aX,
                                        gint aY,
                                        GtkSelectionData* aSelectionData,
                                        guint aInfo, guint aTime,
                                        gpointer aData) {
  RefPtr<nsWindow> window = get_window_for_gtk_widget(aWidget);
  if (!window) return;
>>>>>>> upstream-releases

  window->OnDragDataReceivedEvent(aWidget, aDragContext, aX, aY, aSelectionData,
                                  aInfo, aTime, aData);
}

static nsresult initialize_prefs(void) {
  gRaiseWindows =
      Preferences::GetBool("mozilla.widget.raise-on-setfocus", true);

  return NS_OK;
}

<<<<<<< HEAD
static GdkWindow *get_inner_gdk_window(GdkWindow *aWindow, gint x, gint y,
                                       gint *retx, gint *rety) {
  gint cx, cy, cw, ch;
  GList *children = gdk_window_peek_children(aWindow);
  for (GList *child = g_list_last(children); child;
       child = g_list_previous(child)) {
    auto *childWindow = (GdkWindow *)child->data;
    if (get_window_for_gdk_window(childWindow)) {
      gdk_window_get_geometry(childWindow, &cx, &cy, &cw, &ch);
      if ((cx < x) && (x < (cx + cw)) && (cy < y) && (y < (cy + ch)) &&
          gdk_window_is_visible(childWindow)) {
        return get_inner_gdk_window(childWindow, x - cx, y - cy, retx, rety);
      }
||||||| merged common ancestors
static GdkWindow *
get_inner_gdk_window (GdkWindow *aWindow,
                      gint x, gint y,
                      gint *retx, gint *rety)
{
    gint cx, cy, cw, ch;
    GList *children = gdk_window_peek_children(aWindow);
    for (GList *child = g_list_last(children);
         child;
         child = g_list_previous(child)) {
        auto *childWindow = (GdkWindow *) child->data;
        if (get_window_for_gdk_window(childWindow)) {
            gdk_window_get_geometry(childWindow, &cx, &cy, &cw, &ch);
            if ((cx < x) && (x < (cx + cw)) &&
                (cy < y) && (y < (cy + ch)) &&
                gdk_window_is_visible(childWindow)) {
                return get_inner_gdk_window(childWindow,
                                            x - cx, y - cy,
                                            retx, rety);
            }
        }
=======
static GdkWindow* get_inner_gdk_window(GdkWindow* aWindow, gint x, gint y,
                                       gint* retx, gint* rety) {
  gint cx, cy, cw, ch;
  GList* children = gdk_window_peek_children(aWindow);
  for (GList* child = g_list_last(children); child;
       child = g_list_previous(child)) {
    auto* childWindow = (GdkWindow*)child->data;
    if (get_window_for_gdk_window(childWindow)) {
      gdk_window_get_geometry(childWindow, &cx, &cy, &cw, &ch);
      if ((cx < x) && (x < (cx + cw)) && (cy < y) && (y < (cy + ch)) &&
          gdk_window_is_visible(childWindow)) {
        return get_inner_gdk_window(childWindow, x - cx, y - cy, retx, rety);
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
  *retx = x;
  *rety = y;
  return aWindow;
}

static int is_parent_ungrab_enter(GdkEventCrossing *aEvent) {
  return (GDK_CROSSING_UNGRAB == aEvent->mode) &&
         ((GDK_NOTIFY_ANCESTOR == aEvent->detail) ||
          (GDK_NOTIFY_VIRTUAL == aEvent->detail));
||||||| merged common ancestors
    *retx = x;
    *rety = y;
    return aWindow;
}

static int
is_parent_ungrab_enter(GdkEventCrossing *aEvent)
{
    return (GDK_CROSSING_UNGRAB == aEvent->mode) &&
        ((GDK_NOTIFY_ANCESTOR == aEvent->detail) ||
         (GDK_NOTIFY_VIRTUAL == aEvent->detail));

=======
  }
  *retx = x;
  *rety = y;
  return aWindow;
}

static int is_parent_ungrab_enter(GdkEventCrossing* aEvent) {
  return (GDK_CROSSING_UNGRAB == aEvent->mode) &&
         ((GDK_NOTIFY_ANCESTOR == aEvent->detail) ||
          (GDK_NOTIFY_VIRTUAL == aEvent->detail));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static int is_parent_grab_leave(GdkEventCrossing *aEvent) {
  return (GDK_CROSSING_GRAB == aEvent->mode) &&
         ((GDK_NOTIFY_ANCESTOR == aEvent->detail) ||
          (GDK_NOTIFY_VIRTUAL == aEvent->detail));
||||||| merged common ancestors
static int
is_parent_grab_leave(GdkEventCrossing *aEvent)
{
    return (GDK_CROSSING_GRAB == aEvent->mode) &&
        ((GDK_NOTIFY_ANCESTOR == aEvent->detail) ||
            (GDK_NOTIFY_VIRTUAL == aEvent->detail));
=======
static int is_parent_grab_leave(GdkEventCrossing* aEvent) {
  return (GDK_CROSSING_GRAB == aEvent->mode) &&
         ((GDK_NOTIFY_ANCESTOR == aEvent->detail) ||
          (GDK_NOTIFY_VIRTUAL == aEvent->detail));
>>>>>>> upstream-releases
}

#ifdef ACCESSIBILITY
void nsWindow::CreateRootAccessible() {
  if (mIsTopLevel && !mRootAccessible) {
    LOG(("nsWindow:: Create Toplevel Accessibility\n"));
    mRootAccessible = GetRootAccessible();
  }
}

void nsWindow::DispatchEventToRootAccessible(uint32_t aEventType) {
  if (!a11y::ShouldA11yBeEnabled()) {
    return;
  }

<<<<<<< HEAD
  nsAccessibilityService *accService = GetOrCreateAccService();
  if (!accService) {
    return;
  }
||||||| merged common ancestors
    nsAccessibilityService* accService = GetOrCreateAccService();
    if (!accService) {
        return;
    }
=======
  nsAccessibilityService* accService = GetOrCreateAccService();
  if (!accService) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Get the root document accessible and fire event to it.
  a11y::Accessible *acc = GetRootAccessible();
  if (acc) {
    accService->FireAccessibleEvent(aEventType, acc);
  }
||||||| merged common ancestors
    // Get the root document accessible and fire event to it.
    a11y::Accessible* acc = GetRootAccessible();
    if (acc) {
        accService->FireAccessibleEvent(aEventType, acc);
    }
=======
  // Get the root document accessible and fire event to it.
  a11y::Accessible* acc = GetRootAccessible();
  if (acc) {
    accService->FireAccessibleEvent(aEventType, acc);
  }
>>>>>>> upstream-releases
}

void nsWindow::DispatchActivateEventAccessible(void) {
  DispatchEventToRootAccessible(nsIAccessibleEvent::EVENT_WINDOW_ACTIVATE);
}

void nsWindow::DispatchDeactivateEventAccessible(void) {
  DispatchEventToRootAccessible(nsIAccessibleEvent::EVENT_WINDOW_DEACTIVATE);
}

void nsWindow::DispatchMaximizeEventAccessible(void) {
  DispatchEventToRootAccessible(nsIAccessibleEvent::EVENT_WINDOW_MAXIMIZE);
}

void nsWindow::DispatchMinimizeEventAccessible(void) {
  DispatchEventToRootAccessible(nsIAccessibleEvent::EVENT_WINDOW_MINIMIZE);
}

void nsWindow::DispatchRestoreEventAccessible(void) {
  DispatchEventToRootAccessible(nsIAccessibleEvent::EVENT_WINDOW_RESTORE);
}

#endif /* #ifdef ACCESSIBILITY */

<<<<<<< HEAD
void nsWindow::SetInputContext(const InputContext &aContext,
                               const InputContextAction &aAction) {
  if (!mIMContext) {
    return;
  }
  mIMContext->SetInputContext(this, &aContext, &aAction);
||||||| merged common ancestors
void
nsWindow::SetInputContext(const InputContext& aContext,
                          const InputContextAction& aAction)
{
    if (!mIMContext) {
        return;
    }
    mIMContext->SetInputContext(this, &aContext, &aAction);
=======
void nsWindow::SetInputContext(const InputContext& aContext,
                               const InputContextAction& aAction) {
  if (!mIMContext) {
    return;
  }
  mIMContext->SetInputContext(this, &aContext, &aAction);
>>>>>>> upstream-releases
}

InputContext nsWindow::GetInputContext() {
  InputContext context;
  if (!mIMContext) {
    context.mIMEState.mEnabled = IMEState::DISABLED;
    context.mIMEState.mOpen = IMEState::OPEN_STATE_NOT_SUPPORTED;
  } else {
    context = mIMContext->GetInputContext();
  }
  return context;
}

<<<<<<< HEAD
TextEventDispatcherListener *nsWindow::GetNativeTextEventDispatcherListener() {
  if (NS_WARN_IF(!mIMContext)) {
    return nullptr;
  }
  return mIMContext;
}

void nsWindow::GetEditCommandsRemapped(NativeKeyBindingsType aType,
                                       const WidgetKeyboardEvent &aEvent,
                                       nsTArray<CommandInt> &aCommands,
                                       uint32_t aGeckoKeyCode,
                                       uint32_t aNativeKeyCode) {
  // If aEvent.mNativeKeyEvent is nullptr, the event was created by chrome
  // script.  In such case, we shouldn't expose the OS settings to it.
  // So, just ignore such events here.
  if (!aEvent.mNativeKeyEvent) {
    return;
  }
  WidgetKeyboardEvent modifiedEvent(aEvent);
  modifiedEvent.mKeyCode = aGeckoKeyCode;
  static_cast<GdkEventKey *>(modifiedEvent.mNativeKeyEvent)->keyval =
      aNativeKeyCode;

  NativeKeyBindings *keyBindings = NativeKeyBindings::GetInstance(aType);
  keyBindings->GetEditCommands(modifiedEvent, aCommands);
}

void nsWindow::GetEditCommands(NativeKeyBindingsType aType,
                               const WidgetKeyboardEvent &aEvent,
                               nsTArray<CommandInt> &aCommands) {
  // Validate the arguments.
  nsIWidget::GetEditCommands(aType, aEvent, aCommands);

  if (aEvent.mKeyCode >= NS_VK_LEFT && aEvent.mKeyCode <= NS_VK_DOWN) {
    // Check if we're targeting content with vertical writing mode,
    // and if so remap the arrow keys.
    // XXX This may be expensive.
    WidgetQueryContentEvent query(true, eQuerySelectedText, this);
    nsEventStatus status;
    DispatchEvent(&query, status);

    if (query.mSucceeded && query.mReply.mWritingMode.IsVertical()) {
      uint32_t geckoCode = 0;
      uint32_t gdkCode = 0;
      switch (aEvent.mKeyCode) {
        case NS_VK_LEFT:
          if (query.mReply.mWritingMode.IsVerticalLR()) {
            geckoCode = NS_VK_UP;
            gdkCode = GDK_Up;
          } else {
            geckoCode = NS_VK_DOWN;
            gdkCode = GDK_Down;
          }
          break;
||||||| merged common ancestors
TextEventDispatcherListener*
nsWindow::GetNativeTextEventDispatcherListener()
{
    if (NS_WARN_IF(!mIMContext)) {
        return nullptr;
    }
    return mIMContext;
}
=======
TextEventDispatcherListener* nsWindow::GetNativeTextEventDispatcherListener() {
  if (NS_WARN_IF(!mIMContext)) {
    return nullptr;
  }
  return mIMContext;
}

void nsWindow::GetEditCommandsRemapped(NativeKeyBindingsType aType,
                                       const WidgetKeyboardEvent& aEvent,
                                       nsTArray<CommandInt>& aCommands,
                                       uint32_t aGeckoKeyCode,
                                       uint32_t aNativeKeyCode) {
  // If aEvent.mNativeKeyEvent is nullptr, the event was created by chrome
  // script.  In such case, we shouldn't expose the OS settings to it.
  // So, just ignore such events here.
  if (!aEvent.mNativeKeyEvent) {
    return;
  }
  WidgetKeyboardEvent modifiedEvent(aEvent);
  modifiedEvent.mKeyCode = aGeckoKeyCode;
  static_cast<GdkEventKey*>(modifiedEvent.mNativeKeyEvent)->keyval =
      aNativeKeyCode;

  NativeKeyBindings* keyBindings = NativeKeyBindings::GetInstance(aType);
  keyBindings->GetEditCommands(modifiedEvent, aCommands);
}

void nsWindow::GetEditCommands(NativeKeyBindingsType aType,
                               const WidgetKeyboardEvent& aEvent,
                               nsTArray<CommandInt>& aCommands) {
  // Validate the arguments.
  nsIWidget::GetEditCommands(aType, aEvent, aCommands);

  if (aEvent.mKeyCode >= NS_VK_LEFT && aEvent.mKeyCode <= NS_VK_DOWN) {
    // Check if we're targeting content with vertical writing mode,
    // and if so remap the arrow keys.
    // XXX This may be expensive.
    WidgetQueryContentEvent query(true, eQuerySelectedText, this);
    nsEventStatus status;
    DispatchEvent(&query, status);

    if (query.mSucceeded && query.mReply.mWritingMode.IsVertical()) {
      uint32_t geckoCode = 0;
      uint32_t gdkCode = 0;
      switch (aEvent.mKeyCode) {
        case NS_VK_LEFT:
          if (query.mReply.mWritingMode.IsVerticalLR()) {
            geckoCode = NS_VK_UP;
            gdkCode = GDK_Up;
          } else {
            geckoCode = NS_VK_DOWN;
            gdkCode = GDK_Down;
          }
          break;
>>>>>>> upstream-releases

        case NS_VK_RIGHT:
          if (query.mReply.mWritingMode.IsVerticalLR()) {
            geckoCode = NS_VK_DOWN;
            gdkCode = GDK_Down;
          } else {
            geckoCode = NS_VK_UP;
            gdkCode = GDK_Up;
          }
          break;

        case NS_VK_UP:
          geckoCode = NS_VK_LEFT;
          gdkCode = GDK_Left;
          break;

        case NS_VK_DOWN:
          geckoCode = NS_VK_RIGHT;
          gdkCode = GDK_Right;
          break;
      }

      GetEditCommandsRemapped(aType, aEvent, aCommands, geckoCode, gdkCode);
      return;
    }
  }

<<<<<<< HEAD
  NativeKeyBindings *keyBindings = NativeKeyBindings::GetInstance(aType);
  keyBindings->GetEditCommands(aEvent, aCommands);
||||||| merged common ancestors
    NativeKeyBindings* keyBindings = NativeKeyBindings::GetInstance(aType);
    keyBindings->GetEditCommands(aEvent, aCommands);
=======
  NativeKeyBindings* keyBindings = NativeKeyBindings::GetInstance(aType);
  keyBindings->GetEditCommands(aEvent, aCommands);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
already_AddRefed<DrawTarget> nsWindow::StartRemoteDrawingInRegion(
    LayoutDeviceIntRegion &aInvalidRegion, BufferMode *aBufferMode) {
  return mSurfaceProvider.StartRemoteDrawingInRegion(aInvalidRegion,
                                                     aBufferMode);
||||||| merged common ancestors
already_AddRefed<DrawTarget>
nsWindow::StartRemoteDrawingInRegion(LayoutDeviceIntRegion& aInvalidRegion, BufferMode* aBufferMode)
{
  return mSurfaceProvider.StartRemoteDrawingInRegion(aInvalidRegion, aBufferMode);
=======
already_AddRefed<DrawTarget> nsWindow::StartRemoteDrawingInRegion(
    LayoutDeviceIntRegion& aInvalidRegion, BufferMode* aBufferMode) {
  return mSurfaceProvider.StartRemoteDrawingInRegion(aInvalidRegion,
                                                     aBufferMode);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsWindow::EndRemoteDrawingInRegion(DrawTarget *aDrawTarget,
                                        LayoutDeviceIntRegion &aInvalidRegion) {
||||||| merged common ancestors
void
nsWindow::EndRemoteDrawingInRegion(DrawTarget* aDrawTarget,
                                   LayoutDeviceIntRegion& aInvalidRegion)
{
=======
void nsWindow::EndRemoteDrawingInRegion(DrawTarget* aDrawTarget,
                                        LayoutDeviceIntRegion& aInvalidRegion) {
>>>>>>> upstream-releases
  mSurfaceProvider.EndRemoteDrawingInRegion(aDrawTarget, aInvalidRegion);
}

// Code shared begin BeginMoveDrag and BeginResizeDrag
<<<<<<< HEAD
bool nsWindow::GetDragInfo(WidgetMouseEvent *aMouseEvent, GdkWindow **aWindow,
                           gint *aButton, gint *aRootX, gint *aRootY) {
  if (aMouseEvent->button != WidgetMouseEvent::eLeftButton) {
    // we can only begin a move drag with the left mouse button
    return false;
  }
  *aButton = 1;
||||||| merged common ancestors
bool
nsWindow::GetDragInfo(WidgetMouseEvent* aMouseEvent,
                      GdkWindow** aWindow, gint* aButton,
                      gint* aRootX, gint* aRootY)
{
    if (aMouseEvent->button != WidgetMouseEvent::eLeftButton) {
        // we can only begin a move drag with the left mouse button
        return false;
    }
    *aButton = 1;
=======
bool nsWindow::GetDragInfo(WidgetMouseEvent* aMouseEvent, GdkWindow** aWindow,
                           gint* aButton, gint* aRootX, gint* aRootY) {
  if (aMouseEvent->mButton != MouseButton::eLeft) {
    // we can only begin a move drag with the left mouse button
    return false;
  }
  *aButton = 1;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // get the gdk window for this widget
  GdkWindow *gdk_window = mGdkWindow;
  if (!gdk_window) {
    return false;
  }
||||||| merged common ancestors
    // get the gdk window for this widget
    GdkWindow* gdk_window = mGdkWindow;
    if (!gdk_window) {
        return false;
    }
=======
  // get the gdk window for this widget
  GdkWindow* gdk_window = mGdkWindow;
  if (!gdk_window) {
    return false;
  }
>>>>>>> upstream-releases
#ifdef DEBUG
  // GDK_IS_WINDOW(...) expands to a statement-expression, and
  // statement-expressions are not allowed in template-argument lists. So we
  // have to make the MOZ_ASSERT condition indirect.
  if (!GDK_IS_WINDOW(gdk_window)) {
    MOZ_ASSERT(false, "must really be window");
  }
#endif

  // find the top-level window
  gdk_window = gdk_window_get_toplevel(gdk_window);
  MOZ_ASSERT(gdk_window, "gdk_window_get_toplevel should not return null");
  *aWindow = gdk_window;

  if (!aMouseEvent->mWidget) {
    return false;
  }

<<<<<<< HEAD
  if (mIsX11Display) {
    // Workaround for https://bugzilla.gnome.org/show_bug.cgi?id=789054
    // To avoid crashes disable double-click on WM without _NET_WM_MOVERESIZE.
    // See _should_perform_ewmh_drag() at gdkwindow-x11.c
    GdkScreen *screen = gdk_window_get_screen(gdk_window);
    GdkAtom atom = gdk_atom_intern("_NET_WM_MOVERESIZE", FALSE);
    if (!gdk_x11_screen_supports_net_wm_hint(screen, atom)) {
      static unsigned int lastTimeStamp = 0;
      if (lastTimeStamp != aMouseEvent->mTime) {
        lastTimeStamp = aMouseEvent->mTime;
      } else {
        return false;
||||||| merged common ancestors
    if (mIsX11Display) {
      // Workaround for https://bugzilla.gnome.org/show_bug.cgi?id=789054
      // To avoid crashes disable double-click on WM without _NET_WM_MOVERESIZE.
      // See _should_perform_ewmh_drag() at gdkwindow-x11.c
      GdkScreen* screen = gdk_window_get_screen(gdk_window);
      GdkAtom atom = gdk_atom_intern("_NET_WM_MOVERESIZE", FALSE);
      if (!gdk_x11_screen_supports_net_wm_hint(screen, atom)) {
          static unsigned int lastTimeStamp = 0;
          if (lastTimeStamp != aMouseEvent->mTime) {
              lastTimeStamp = aMouseEvent->mTime;
          } else {
              return false;
          }
=======
  if (mIsX11Display) {
    // Workaround for https://bugzilla.gnome.org/show_bug.cgi?id=789054
    // To avoid crashes disable double-click on WM without _NET_WM_MOVERESIZE.
    // See _should_perform_ewmh_drag() at gdkwindow-x11.c
    GdkScreen* screen = gdk_window_get_screen(gdk_window);
    GdkAtom atom = gdk_atom_intern("_NET_WM_MOVERESIZE", FALSE);
    if (!gdk_x11_screen_supports_net_wm_hint(screen, atom)) {
      static unsigned int lastTimeStamp = 0;
      if (lastTimeStamp != aMouseEvent->mTime) {
        lastTimeStamp = aMouseEvent->mTime;
      } else {
        return false;
>>>>>>> upstream-releases
      }
    }
  }

  // FIXME: It would be nice to have the widget position at the time
  // of the event, but it's relatively unlikely that the widget has
  // moved since the mousedown.  (On the other hand, it's quite likely
  // that the mouse has moved, which is why we use the mouse position
  // from the event.)
  LayoutDeviceIntPoint offset = aMouseEvent->mWidget->WidgetToScreenOffset();
  *aRootX = aMouseEvent->mRefPoint.x + offset.x;
  *aRootY = aMouseEvent->mRefPoint.y + offset.y;

  return true;
}

<<<<<<< HEAD
nsresult nsWindow::BeginMoveDrag(WidgetMouseEvent *aEvent) {
  MOZ_ASSERT(aEvent, "must have event");
  MOZ_ASSERT(aEvent->mClass == eMouseEventClass,
             "event must have correct struct type");

  GdkWindow *gdk_window;
  gint button, screenX, screenY;
  if (!GetDragInfo(aEvent, &gdk_window, &button, &screenX, &screenY)) {
    return NS_ERROR_FAILURE;
  }

  // tell the window manager to start the move
  screenX = DevicePixelsToGdkCoordRoundDown(screenX);
  screenY = DevicePixelsToGdkCoordRoundDown(screenY);
  gdk_window_begin_move_drag(gdk_window, button, screenX, screenY,
                             aEvent->mTime);
||||||| merged common ancestors
nsresult
nsWindow::BeginMoveDrag(WidgetMouseEvent* aEvent)
{
    MOZ_ASSERT(aEvent, "must have event");
    MOZ_ASSERT(aEvent->mClass == eMouseEventClass,
               "event must have correct struct type");

    GdkWindow *gdk_window;
    gint button, screenX, screenY;
    if (!GetDragInfo(aEvent, &gdk_window, &button, &screenX, &screenY)) {
        return NS_ERROR_FAILURE;
    }

    // tell the window manager to start the move
    screenX = DevicePixelsToGdkCoordRoundDown(screenX);
    screenY = DevicePixelsToGdkCoordRoundDown(screenY);
    gdk_window_begin_move_drag(gdk_window, button, screenX, screenY,
                               aEvent->mTime);
=======
nsresult nsWindow::BeginResizeDrag(WidgetGUIEvent* aEvent, int32_t aHorizontal,
                                   int32_t aVertical) {
  NS_ENSURE_ARG_POINTER(aEvent);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return NS_OK;
}
||||||| merged common ancestors
    return NS_OK;
}
=======
  if (aEvent->mClass != eMouseEventClass) {
    // you can only begin a resize drag with a mouse event
    return NS_ERROR_INVALID_ARG;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsWindow::BeginResizeDrag(WidgetGUIEvent *aEvent, int32_t aHorizontal,
                                   int32_t aVertical) {
  NS_ENSURE_ARG_POINTER(aEvent);
||||||| merged common ancestors
nsresult
nsWindow::BeginResizeDrag(WidgetGUIEvent* aEvent,
                          int32_t aHorizontal,
                          int32_t aVertical)
{
    NS_ENSURE_ARG_POINTER(aEvent);
=======
  GdkWindow* gdk_window;
  gint button, screenX, screenY;
  if (!GetDragInfo(aEvent->AsMouseEvent(), &gdk_window, &button, &screenX,
                   &screenY)) {
    return NS_ERROR_FAILURE;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (aEvent->mClass != eMouseEventClass) {
    // you can only begin a resize drag with a mouse event
    return NS_ERROR_INVALID_ARG;
  }

  GdkWindow *gdk_window;
  gint button, screenX, screenY;
  if (!GetDragInfo(aEvent->AsMouseEvent(), &gdk_window, &button, &screenX,
                   &screenY)) {
    return NS_ERROR_FAILURE;
  }

  // work out what GdkWindowEdge we're talking about
  GdkWindowEdge window_edge;
  if (aVertical < 0) {
    if (aHorizontal < 0) {
      window_edge = GDK_WINDOW_EDGE_NORTH_WEST;
    } else if (aHorizontal == 0) {
      window_edge = GDK_WINDOW_EDGE_NORTH;
||||||| merged common ancestors
    if (aEvent->mClass != eMouseEventClass) {
        // you can only begin a resize drag with a mouse event
        return NS_ERROR_INVALID_ARG;
    }

    GdkWindow *gdk_window;
    gint button, screenX, screenY;
    if (!GetDragInfo(aEvent->AsMouseEvent(), &gdk_window, &button,
                     &screenX, &screenY)) {
        return NS_ERROR_FAILURE;
    }

    // work out what GdkWindowEdge we're talking about
    GdkWindowEdge window_edge;
    if (aVertical < 0) {
        if (aHorizontal < 0) {
            window_edge = GDK_WINDOW_EDGE_NORTH_WEST;
        } else if (aHorizontal == 0) {
            window_edge = GDK_WINDOW_EDGE_NORTH;
        } else {
            window_edge = GDK_WINDOW_EDGE_NORTH_EAST;
        }
    } else if (aVertical == 0) {
        if (aHorizontal < 0) {
            window_edge = GDK_WINDOW_EDGE_WEST;
        } else if (aHorizontal == 0) {
            return NS_ERROR_INVALID_ARG;
        } else {
            window_edge = GDK_WINDOW_EDGE_EAST;
        }
=======
  // work out what GdkWindowEdge we're talking about
  GdkWindowEdge window_edge;
  if (aVertical < 0) {
    if (aHorizontal < 0) {
      window_edge = GDK_WINDOW_EDGE_NORTH_WEST;
    } else if (aHorizontal == 0) {
      window_edge = GDK_WINDOW_EDGE_NORTH;
    } else {
      window_edge = GDK_WINDOW_EDGE_NORTH_EAST;
    }
  } else if (aVertical == 0) {
    if (aHorizontal < 0) {
      window_edge = GDK_WINDOW_EDGE_WEST;
    } else if (aHorizontal == 0) {
      return NS_ERROR_INVALID_ARG;
    } else {
      window_edge = GDK_WINDOW_EDGE_EAST;
    }
  } else {
    if (aHorizontal < 0) {
      window_edge = GDK_WINDOW_EDGE_SOUTH_WEST;
    } else if (aHorizontal == 0) {
      window_edge = GDK_WINDOW_EDGE_SOUTH;
>>>>>>> upstream-releases
    } else {
<<<<<<< HEAD
      window_edge = GDK_WINDOW_EDGE_NORTH_EAST;
||||||| merged common ancestors
        if (aHorizontal < 0) {
            window_edge = GDK_WINDOW_EDGE_SOUTH_WEST;
        } else if (aHorizontal == 0) {
            window_edge = GDK_WINDOW_EDGE_SOUTH;
        } else {
            window_edge = GDK_WINDOW_EDGE_SOUTH_EAST;
        }
=======
      window_edge = GDK_WINDOW_EDGE_SOUTH_EAST;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  } else if (aVertical == 0) {
    if (aHorizontal < 0) {
      window_edge = GDK_WINDOW_EDGE_WEST;
    } else if (aHorizontal == 0) {
      return NS_ERROR_INVALID_ARG;
    } else {
      window_edge = GDK_WINDOW_EDGE_EAST;
    }
  } else {
    if (aHorizontal < 0) {
      window_edge = GDK_WINDOW_EDGE_SOUTH_WEST;
    } else if (aHorizontal == 0) {
      window_edge = GDK_WINDOW_EDGE_SOUTH;
    } else {
      window_edge = GDK_WINDOW_EDGE_SOUTH_EAST;
    }
  }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

  // tell the window manager to start the resize
  gdk_window_begin_resize_drag(gdk_window, window_edge, button, screenX,
                               screenY, aEvent->mTime);

  return NS_OK;
}

<<<<<<< HEAD
nsIWidget::LayerManager *nsWindow::GetLayerManager(
    PLayerTransactionChild *aShadowManager, LayersBackend aBackendHint,
    LayerManagerPersistence aPersistence) {
  if (mIsDestroyed) {
    // Prevent external code from triggering the re-creation of the
    // LayerManager/Compositor during shutdown. Just return what we currently
    // have, which is most likely null.
    return mLayerManager;
  }
||||||| merged common ancestors
nsIWidget::LayerManager*
nsWindow::GetLayerManager(PLayerTransactionChild* aShadowManager,
                          LayersBackend aBackendHint,
                          LayerManagerPersistence aPersistence)
{
    if (mIsDestroyed) {
      // Prevent external code from triggering the re-creation of the LayerManager/Compositor
      // during shutdown. Just return what we currently have, which is most likely null.
      return mLayerManager;
    }
=======
nsIWidget::LayerManager* nsWindow::GetLayerManager(
    PLayerTransactionChild* aShadowManager, LayersBackend aBackendHint,
    LayerManagerPersistence aPersistence) {
  if (mIsDestroyed) {
    // Prevent external code from triggering the re-creation of the
    // LayerManager/Compositor during shutdown. Just return what we currently
    // have, which is most likely null.
    return mLayerManager;
  }
>>>>>>> upstream-releases

  return nsBaseWidget::GetLayerManager(aShadowManager, aBackendHint,
                                       aPersistence);
}

<<<<<<< HEAD
void nsWindow::SetCompositorWidgetDelegate(CompositorWidgetDelegate *delegate) {
  if (delegate) {
    mCompositorWidgetDelegate = delegate->AsPlatformSpecificDelegate();
    MOZ_ASSERT(mCompositorWidgetDelegate,
               "nsWindow::SetCompositorWidgetDelegate called with a "
               "non-PlatformCompositorWidgetDelegate");
  } else {
    mCompositorWidgetDelegate = nullptr;
  }
||||||| merged common ancestors
void
nsWindow::SetCompositorWidgetDelegate(CompositorWidgetDelegate* delegate)
{
    if (delegate) {
        mCompositorWidgetDelegate = delegate->AsPlatformSpecificDelegate();
        MOZ_ASSERT(mCompositorWidgetDelegate,
                   "nsWindow::SetCompositorWidgetDelegate called with a non-PlatformCompositorWidgetDelegate");
    } else {
        mCompositorWidgetDelegate = nullptr;
    }
=======
void nsWindow::SetCompositorWidgetDelegate(CompositorWidgetDelegate* delegate) {
  if (delegate) {
    mCompositorWidgetDelegate = delegate->AsPlatformSpecificDelegate();
    MOZ_ASSERT(mCompositorWidgetDelegate,
               "nsWindow::SetCompositorWidgetDelegate called with a "
               "non-PlatformCompositorWidgetDelegate");
#ifdef MOZ_WAYLAND
    WaylandEGLSurfaceForceRedraw();
#endif
  } else {
    mCompositorWidgetDelegate = nullptr;
  }
>>>>>>> upstream-releases
}

void nsWindow::ClearCachedResources() {
  if (mLayerManager && mLayerManager->GetBackendType() ==
                           mozilla::layers::LayersBackend::LAYERS_BASIC) {
    mLayerManager->ClearCachedResources();
  }

<<<<<<< HEAD
  GList *children = gdk_window_peek_children(mGdkWindow);
  for (GList *list = children; list; list = list->next) {
    nsWindow *window = get_window_for_gdk_window(GDK_WINDOW(list->data));
    if (window) {
      window->ClearCachedResources();
||||||| merged common ancestors
    GList* children = gdk_window_peek_children(mGdkWindow);
    for (GList* list = children; list; list = list->next) {
        nsWindow* window = get_window_for_gdk_window(GDK_WINDOW(list->data));
        if (window) {
            window->ClearCachedResources();
        }
=======
  GList* children = gdk_window_peek_children(mGdkWindow);
  for (GList* list = children; list; list = list->next) {
    nsWindow* window = get_window_for_gdk_window(GDK_WINDOW(list->data));
    if (window) {
      window->ClearCachedResources();
>>>>>>> upstream-releases
    }
  }
}

/* nsWindow::UpdateClientOffsetForCSDWindow() is designed to be called from
 * paint code to update mClientOffset any time. It also propagates
 * the mClientOffset to child tabs.
 *
 * It works only for CSD decorated GtkWindow.
 */
void nsWindow::UpdateClientOffsetForCSDWindow() {
  // We update window offset on X11 as the window position is calculated
  // relatively to mShell. We don't do that on Wayland as our wl_subsurface
  // is attached to mContainer and mShell is ignored.
  if (!mIsX11Display) {
    return;
  }

  // _NET_FRAME_EXTENTS is not set on client decorated windows,
  // so we need to read offset between mContainer and toplevel mShell
  // window.
  if (mSizeState == nsSizeMode_Normal) {
    GtkBorder decorationSize;
    GetCSDDecorationSize(GTK_WINDOW(mShell), &decorationSize);
    mClientOffset = nsIntPoint(decorationSize.left, decorationSize.top);
  } else {
    mClientOffset = nsIntPoint(0, 0);
  }

<<<<<<< HEAD
  // Send a WindowMoved notification. This ensures that TabParent
  // picks up the new client offset and sends it to the child process
  // if appropriate.
  NotifyWindowMoved(mBounds.x, mBounds.y);
||||||| merged common ancestors
    // Send a WindowMoved notification. This ensures that TabParent
    // picks up the new client offset and sends it to the child process
    // if appropriate.
    NotifyWindowMoved(mBounds.x, mBounds.y);
=======
  // Send a WindowMoved notification. This ensures that BrowserParent
  // picks up the new client offset and sends it to the child process
  // if appropriate.
  NotifyWindowMoved(mBounds.x, mBounds.y);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsWindow::SetNonClientMargins(LayoutDeviceIntMargin &aMargins) {
  SetDrawsInTitlebar(aMargins.top == 0);
  return NS_OK;
||||||| merged common ancestors
nsresult
nsWindow::SetNonClientMargins(LayoutDeviceIntMargin &aMargins)
{
    SetDrawsInTitlebar(aMargins.top == 0);
    return NS_OK;
=======
nsresult nsWindow::SetNonClientMargins(LayoutDeviceIntMargin& aMargins) {
  SetDrawsInTitlebar(aMargins.top == 0);
  return NS_OK;
>>>>>>> upstream-releases
}

void nsWindow::SetDrawsInTitlebar(bool aState) {
  if (!mShell || mCSDSupportLevel == CSD_SUPPORT_NONE ||
      aState == mDrawInTitlebar) {
    return;
  }

<<<<<<< HEAD
  if (mCSDSupportLevel == CSD_SUPPORT_SYSTEM) {
    SetWindowDecoration(aState ? eBorderStyle_border : mBorderStyle);
  } else if (mCSDSupportLevel == CSD_SUPPORT_CLIENT) {
    /* Window manager does not support GDK_DECOR_BORDER,
     * emulate it by CSD.
     *
     * gtk_window_set_titlebar() works on unrealized widgets only,
     * we need to handle mShell carefully here.
     * When CSD is enabled mGdkWindow is owned by mContainer which is good
     * as we can't delete our mGdkWindow. To make mShell unrealized while
     * mContainer is preserved we temporary reparent mContainer to an
     * invisible GtkWindow.
     */
    NativeShow(false);

    // Using GTK_WINDOW_POPUP rather than
    // GTK_WINDOW_TOPLEVEL in the hope that POPUP results in less
    // initialization and window manager interaction.
    GtkWidget *tmpWindow = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_widget_realize(tmpWindow);
||||||| merged common ancestors
    if (mCSDSupportLevel == CSD_SUPPORT_SYSTEM) {
        SetWindowDecoration(aState ? eBorderStyle_border : mBorderStyle);
    }
    else if (mCSDSupportLevel == CSD_SUPPORT_CLIENT) {
        /* Window manager does not support GDK_DECOR_BORDER,
         * emulate it by CSD.
         *
         * gtk_window_set_titlebar() works on unrealized widgets only,
         * we need to handle mShell carefully here.
         * When CSD is enabled mGdkWindow is owned by mContainer which is good
         * as we can't delete our mGdkWindow. To make mShell unrealized while
         * mContainer is preserved we temporary reparent mContainer to an
         * invisible GtkWindow.
         */
        NativeShow(false);

        // Using GTK_WINDOW_POPUP rather than
        // GTK_WINDOW_TOPLEVEL in the hope that POPUP results in less
        // initialization and window manager interaction.
        GtkWidget* tmpWindow = gtk_window_new(GTK_WINDOW_POPUP);
        gtk_widget_realize(tmpWindow);

        gtk_widget_reparent(GTK_WIDGET(mContainer), tmpWindow);
        gtk_widget_unrealize(GTK_WIDGET(mShell));

        // Available as of GTK 3.10+
        static auto sGtkWindowSetTitlebar = (void (*)(GtkWindow*, GtkWidget*))
            dlsym(RTLD_DEFAULT, "gtk_window_set_titlebar");
        MOZ_ASSERT(sGtkWindowSetTitlebar,
            "Missing gtk_window_set_titlebar(), old Gtk+ library?");

        if (aState) {
            // Add a hidden titlebar widget to trigger CSD, but disable the default
            // titlebar.  GtkFixed is a somewhat random choice for a simple unused
            // widget. gtk_window_set_titlebar() takes ownership of the titlebar
            // widget.
            sGtkWindowSetTitlebar(GTK_WINDOW(mShell), gtk_fixed_new());
        } else {
            sGtkWindowSetTitlebar(GTK_WINDOW(mShell), nullptr);
        }
=======
  if (mCSDSupportLevel == CSD_SUPPORT_SYSTEM) {
    SetWindowDecoration(aState ? eBorderStyle_border : mBorderStyle);
  } else if (mCSDSupportLevel == CSD_SUPPORT_CLIENT) {
    /* Window manager does not support GDK_DECOR_BORDER,
     * emulate it by CSD.
     *
     * gtk_window_set_titlebar() works on unrealized widgets only,
     * we need to handle mShell carefully here.
     * When CSD is enabled mGdkWindow is owned by mContainer which is good
     * as we can't delete our mGdkWindow. To make mShell unrealized while
     * mContainer is preserved we temporary reparent mContainer to an
     * invisible GtkWindow.
     */
    NativeShow(false);

    // Using GTK_WINDOW_POPUP rather than
    // GTK_WINDOW_TOPLEVEL in the hope that POPUP results in less
    // initialization and window manager interaction.
    GtkWidget* tmpWindow = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_widget_realize(tmpWindow);
>>>>>>> upstream-releases

<<<<<<< HEAD
    gtk_widget_reparent(GTK_WIDGET(mContainer), tmpWindow);
    gtk_widget_unrealize(GTK_WIDGET(mShell));

    // Available as of GTK 3.10+
    static auto sGtkWindowSetTitlebar =
        (void (*)(GtkWindow *, GtkWidget *))dlsym(RTLD_DEFAULT,
                                                  "gtk_window_set_titlebar");
    MOZ_ASSERT(sGtkWindowSetTitlebar,
               "Missing gtk_window_set_titlebar(), old Gtk+ library?");

    if (aState) {
      // Add a hidden titlebar widget to trigger CSD, but disable the default
      // titlebar.  GtkFixed is a somewhat random choice for a simple unused
      // widget. gtk_window_set_titlebar() takes ownership of the titlebar
      // widget.
      sGtkWindowSetTitlebar(GTK_WINDOW(mShell), gtk_fixed_new());
    } else {
      sGtkWindowSetTitlebar(GTK_WINDOW(mShell), nullptr);
    }

    /* A workaround for https://bugzilla.gnome.org/show_bug.cgi?id=791081
     * gtk_widget_realize() throws:
     * "In pixman_region32_init_rect: Invalid rectangle passed"
     * when mShell has default 1x1 size.
     */
    GtkAllocation allocation = {0, 0, 0, 0};
    gtk_widget_get_preferred_width(GTK_WIDGET(mShell), nullptr,
                                   &allocation.width);
    gtk_widget_get_preferred_height(GTK_WIDGET(mShell), nullptr,
                                    &allocation.height);
    gtk_widget_size_allocate(GTK_WIDGET(mShell), &allocation);

    gtk_widget_realize(GTK_WIDGET(mShell));
    gtk_widget_reparent(GTK_WIDGET(mContainer), GTK_WIDGET(mShell));
    mNeedsShow = true;
    NativeResize();

    // Label mShell toplevel window so property_notify_event_cb callback
    // can find its way home.
    g_object_set_data(G_OBJECT(gtk_widget_get_window(mShell)), "nsWindow",
                      this);
||||||| merged common ancestors
        /* A workaround for https://bugzilla.gnome.org/show_bug.cgi?id=791081
         * gtk_widget_realize() throws:
         * "In pixman_region32_init_rect: Invalid rectangle passed"
         * when mShell has default 1x1 size.
         */
        GtkAllocation allocation = {0, 0, 0, 0};
        gtk_widget_get_preferred_width(GTK_WIDGET(mShell), nullptr,
                                       &allocation.width);
        gtk_widget_get_preferred_height(GTK_WIDGET(mShell), nullptr,
                                        &allocation.height);
        gtk_widget_size_allocate(GTK_WIDGET(mShell), &allocation);

        gtk_widget_realize(GTK_WIDGET(mShell));
        gtk_widget_reparent(GTK_WIDGET(mContainer), GTK_WIDGET(mShell));
        mNeedsShow = true;
        NativeResize();

        // Label mShell toplevel window so property_notify_event_cb callback
        // can find its way home.
        g_object_set_data(G_OBJECT(gtk_widget_get_window(mShell)),
                          "nsWindow", this);
=======
    gtk_widget_reparent(GTK_WIDGET(mContainer), tmpWindow);
    gtk_widget_unrealize(GTK_WIDGET(mShell));

    // Available as of GTK 3.10+
    static auto sGtkWindowSetTitlebar = (void (*)(GtkWindow*, GtkWidget*))dlsym(
        RTLD_DEFAULT, "gtk_window_set_titlebar");
    MOZ_ASSERT(sGtkWindowSetTitlebar,
               "Missing gtk_window_set_titlebar(), old Gtk+ library?");

    if (aState) {
      // Add a hidden titlebar widget to trigger CSD, but disable the default
      // titlebar.  GtkFixed is a somewhat random choice for a simple unused
      // widget. gtk_window_set_titlebar() takes ownership of the titlebar
      // widget.
      sGtkWindowSetTitlebar(GTK_WINDOW(mShell), gtk_fixed_new());
    } else {
      sGtkWindowSetTitlebar(GTK_WINDOW(mShell), nullptr);
    }

    /* A workaround for https://bugzilla.gnome.org/show_bug.cgi?id=791081
     * gtk_widget_realize() throws:
     * "In pixman_region32_init_rect: Invalid rectangle passed"
     * when mShell has default 1x1 size.
     */
    GtkAllocation allocation = {0, 0, 0, 0};
    gtk_widget_get_preferred_width(GTK_WIDGET(mShell), nullptr,
                                   &allocation.width);
    gtk_widget_get_preferred_height(GTK_WIDGET(mShell), nullptr,
                                    &allocation.height);
    gtk_widget_size_allocate(GTK_WIDGET(mShell), &allocation);

    gtk_widget_realize(GTK_WIDGET(mShell));
    gtk_widget_reparent(GTK_WIDGET(mContainer), GTK_WIDGET(mShell));
    mNeedsShow = true;
    NativeResize();

    // Label mShell toplevel window so property_notify_event_cb callback
    // can find its way home.
    g_object_set_data(G_OBJECT(gtk_widget_get_window(mShell)), "nsWindow",
                      this);
>>>>>>> upstream-releases
#ifdef MOZ_X11
    SetCompositorHint(GTK_WIDGET_COMPOSIDED_ENABLED);
#endif
    RefreshWindowClass();

    // When we use system titlebar setup managed by Gtk+ we also get
    // _NET_FRAME_EXTENTS property for our toplevel window so we can't
    // update the client offset it here.
    if (aState) {
      UpdateClientOffsetForCSDWindow();
    }

    gtk_widget_destroy(tmpWindow);
  }

<<<<<<< HEAD
  mDrawInTitlebar = aState;
||||||| merged common ancestors
gint
nsWindow::GdkScaleFactor()
{
#if (MOZ_WIDGET_GTK >= 3)
    // Available as of GTK 3.10+
    static auto sGdkWindowGetScaleFactorPtr = (gint (*)(GdkWindow*))
        dlsym(RTLD_DEFAULT, "gdk_window_get_scale_factor");
    if (sGdkWindowGetScaleFactorPtr && mGdkWindow)
        return (*sGdkWindowGetScaleFactorPtr)(mGdkWindow);
#endif
    return ScreenHelperGTK::GetGTKMonitorScaleFactor();
=======
  mDrawInTitlebar = aState;

  if (mTransparencyBitmapForTitlebar) {
    if (mDrawInTitlebar && mSizeState == nsSizeMode_Normal) {
      UpdateTitlebarTransparencyBitmap();
    } else {
      ClearTransparencyBitmap();
    }
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
gint nsWindow::GdkScaleFactor() {
  // Available as of GTK 3.10+
  static auto sGdkWindowGetScaleFactorPtr =
      (gint(*)(GdkWindow *))dlsym(RTLD_DEFAULT, "gdk_window_get_scale_factor");
  if (sGdkWindowGetScaleFactorPtr && mGdkWindow)
    return (*sGdkWindowGetScaleFactorPtr)(mGdkWindow);
  return ScreenHelperGTK::GetGTKMonitorScaleFactor();
}
||||||| merged common ancestors
=======
gint nsWindow::GdkScaleFactor() {
  // Available as of GTK 3.10+
  static auto sGdkWindowGetScaleFactorPtr =
      (gint(*)(GdkWindow*))dlsym(RTLD_DEFAULT, "gdk_window_get_scale_factor");
  if (sGdkWindowGetScaleFactorPtr && mGdkWindow)
    return (*sGdkWindowGetScaleFactorPtr)(mGdkWindow);
  return ScreenHelperGTK::GetGTKMonitorScaleFactor();
}
>>>>>>> upstream-releases

gint nsWindow::DevicePixelsToGdkCoordRoundUp(int pixels) {
  gint scale = GdkScaleFactor();
  return (pixels + scale - 1) / scale;
}

gint nsWindow::DevicePixelsToGdkCoordRoundDown(int pixels) {
  gint scale = GdkScaleFactor();
  return pixels / scale;
}

GdkPoint nsWindow::DevicePixelsToGdkPointRoundDown(LayoutDeviceIntPoint point) {
  gint scale = GdkScaleFactor();
  return {point.x / scale, point.y / scale};
}

GdkRectangle nsWindow::DevicePixelsToGdkRectRoundOut(LayoutDeviceIntRect rect) {
  gint scale = GdkScaleFactor();
  int x = rect.x / scale;
  int y = rect.y / scale;
  int right = (rect.x + rect.width + scale - 1) / scale;
  int bottom = (rect.y + rect.height + scale - 1) / scale;
  return {x, y, right - x, bottom - y};
}

GdkRectangle nsWindow::DevicePixelsToGdkSizeRoundUp(
    LayoutDeviceIntSize pixelSize) {
  gint scale = GdkScaleFactor();
  gint width = (pixelSize.width + scale - 1) / scale;
  gint height = (pixelSize.height + scale - 1) / scale;
  return {0, 0, width, height};
}

int nsWindow::GdkCoordToDevicePixels(gint coord) {
  return coord * GdkScaleFactor();
}

LayoutDeviceIntPoint nsWindow::GdkEventCoordsToDevicePixels(gdouble x,
                                                            gdouble y) {
  gint scale = GdkScaleFactor();
  return LayoutDeviceIntPoint::Round(x * scale, y * scale);
}

LayoutDeviceIntPoint nsWindow::GdkPointToDevicePixels(GdkPoint point) {
  gint scale = GdkScaleFactor();
  return LayoutDeviceIntPoint(point.x * scale, point.y * scale);
}

LayoutDeviceIntRect nsWindow::GdkRectToDevicePixels(GdkRectangle rect) {
  gint scale = GdkScaleFactor();
  return LayoutDeviceIntRect(rect.x * scale, rect.y * scale, rect.width * scale,
                             rect.height * scale);
}

<<<<<<< HEAD
nsresult nsWindow::SynthesizeNativeMouseEvent(LayoutDeviceIntPoint aPoint,
                                              uint32_t aNativeMessage,
                                              uint32_t aModifierFlags,
                                              nsIObserver *aObserver) {
||||||| merged common ancestors
nsresult
nsWindow::SynthesizeNativeMouseEvent(LayoutDeviceIntPoint aPoint,
                                     uint32_t aNativeMessage,
                                     uint32_t aModifierFlags,
                                     nsIObserver* aObserver)
{
=======
nsresult nsWindow::SynthesizeNativeMouseEvent(LayoutDeviceIntPoint aPoint,
                                              uint32_t aNativeMessage,
                                              uint32_t aModifierFlags,
                                              nsIObserver* aObserver) {
>>>>>>> upstream-releases
  AutoObserverNotifier notifier(aObserver, "mouseevent");

  if (!mGdkWindow) {
    return NS_OK;
  }

  GdkDisplay *display = gdk_window_get_display(mGdkWindow);

  // When a button-press/release event is requested, create it here and put it
  // in the event queue. This will not emit a motion event - this needs to be
  // done explicitly *before* requesting a button-press/release. You will also
  // need to wait for the motion event to be dispatched before requesting a
  // button-press/release event to maintain the desired event order.
  if (aNativeMessage == GDK_BUTTON_PRESS ||
      aNativeMessage == GDK_BUTTON_RELEASE) {
    GdkEvent event;
    memset(&event, 0, sizeof(GdkEvent));
    event.type = (GdkEventType)aNativeMessage;
    event.button.button = 1;
    event.button.window = mGdkWindow;
    event.button.time = GDK_CURRENT_TIME;

    // Get device for event source
    GdkDeviceManager* device_manager = gdk_display_get_device_manager(display);
    event.button.device = gdk_device_manager_get_client_pointer(device_manager);

    event.button.x_root = DevicePixelsToGdkCoordRoundDown(aPoint.x);
    event.button.y_root = DevicePixelsToGdkCoordRoundDown(aPoint.y);

    LayoutDeviceIntPoint pointInWindow = aPoint - WidgetToScreenOffset();
    event.button.x = DevicePixelsToGdkCoordRoundDown(pointInWindow.x);
    event.button.y = DevicePixelsToGdkCoordRoundDown(pointInWindow.y);

    gdk_event_put(&event);
  } else {
    // We don't support specific events other than button-press/release. In all
    // other cases we'll synthesize a motion event that will be emitted by
    // gdk_display_warp_pointer().
    GdkScreen *screen = gdk_window_get_screen(mGdkWindow);
    GdkPoint point = DevicePixelsToGdkPointRoundDown(aPoint);
    gdk_display_warp_pointer(display, screen, point.x, point.y);
  }

  return NS_OK;
}

<<<<<<< HEAD
nsresult nsWindow::SynthesizeNativeMouseScrollEvent(
    mozilla::LayoutDeviceIntPoint aPoint, uint32_t aNativeMessage,
    double aDeltaX, double aDeltaY, double aDeltaZ, uint32_t aModifierFlags,
    uint32_t aAdditionalFlags, nsIObserver *aObserver) {
||||||| merged common ancestors
nsresult
nsWindow::SynthesizeNativeMouseScrollEvent(mozilla::LayoutDeviceIntPoint aPoint,
                                           uint32_t aNativeMessage,
                                           double aDeltaX,
                                           double aDeltaY,
                                           double aDeltaZ,
                                           uint32_t aModifierFlags,
                                           uint32_t aAdditionalFlags,
                                           nsIObserver* aObserver)
{
=======
nsresult nsWindow::SynthesizeNativeMouseScrollEvent(
    mozilla::LayoutDeviceIntPoint aPoint, uint32_t aNativeMessage,
    double aDeltaX, double aDeltaY, double aDeltaZ, uint32_t aModifierFlags,
    uint32_t aAdditionalFlags, nsIObserver* aObserver) {
>>>>>>> upstream-releases
  AutoObserverNotifier notifier(aObserver, "mousescrollevent");

  if (!mGdkWindow) {
    return NS_OK;
  }

  GdkEvent event;
  memset(&event, 0, sizeof(GdkEvent));
  event.type = GDK_SCROLL;
  event.scroll.window = mGdkWindow;
  event.scroll.time = GDK_CURRENT_TIME;
  // Get device for event source
<<<<<<< HEAD
  GdkDisplay *display = gdk_window_get_display(mGdkWindow);
  GdkDeviceManager *device_manager = gdk_display_get_device_manager(display);
||||||| merged common ancestors
  GdkDisplay* display = gdk_window_get_display(mGdkWindow);
  GdkDeviceManager *device_manager = gdk_display_get_device_manager(display);
=======
  GdkDisplay* display = gdk_window_get_display(mGdkWindow);
  GdkDeviceManager* device_manager = gdk_display_get_device_manager(display);
>>>>>>> upstream-releases
  event.scroll.device = gdk_device_manager_get_client_pointer(device_manager);
  event.scroll.x_root = DevicePixelsToGdkCoordRoundDown(aPoint.x);
  event.scroll.y_root = DevicePixelsToGdkCoordRoundDown(aPoint.y);

  LayoutDeviceIntPoint pointInWindow = aPoint - WidgetToScreenOffset();
  event.scroll.x = DevicePixelsToGdkCoordRoundDown(pointInWindow.x);
  event.scroll.y = DevicePixelsToGdkCoordRoundDown(pointInWindow.y);

  // The delta values are backwards on Linux compared to Windows and Cocoa,
  // hence the negation.
#if GTK_CHECK_VERSION(3, 4, 0)
  // TODO: is this correct? I don't have GTK 3.4+ so I can't check
  event.scroll.direction = GDK_SCROLL_SMOOTH;
  event.scroll.delta_x = -aDeltaX;
  event.scroll.delta_y = -aDeltaY;
#else
  if (aDeltaX < 0) {
    event.scroll.direction = GDK_SCROLL_RIGHT;
  } else if (aDeltaX > 0) {
    event.scroll.direction = GDK_SCROLL_LEFT;
  } else if (aDeltaY < 0) {
    event.scroll.direction = GDK_SCROLL_DOWN;
  } else if (aDeltaY > 0) {
    event.scroll.direction = GDK_SCROLL_UP;
  } else {
    return NS_OK;
  }
#endif

  gdk_event_put(&event);

  return NS_OK;
}

<<<<<<< HEAD
#if GTK_CHECK_VERSION(3, 4, 0)
nsresult nsWindow::SynthesizeNativeTouchPoint(uint32_t aPointerId,
                                              TouchPointerState aPointerState,
                                              LayoutDeviceIntPoint aPoint,
                                              double aPointerPressure,
                                              uint32_t aPointerOrientation,
                                              nsIObserver *aObserver) {
||||||| merged common ancestors
#if GTK_CHECK_VERSION(3,4,0)
nsresult
nsWindow::SynthesizeNativeTouchPoint(uint32_t aPointerId,
                                     TouchPointerState aPointerState,
                                     LayoutDeviceIntPoint aPoint,
                                     double aPointerPressure,
                                     uint32_t aPointerOrientation,
                                     nsIObserver* aObserver)
{
=======
#if GTK_CHECK_VERSION(3, 4, 0)
nsresult nsWindow::SynthesizeNativeTouchPoint(uint32_t aPointerId,
                                              TouchPointerState aPointerState,
                                              LayoutDeviceIntPoint aPoint,
                                              double aPointerPressure,
                                              uint32_t aPointerOrientation,
                                              nsIObserver* aObserver) {
>>>>>>> upstream-releases
  AutoObserverNotifier notifier(aObserver, "touchpoint");

  if (!mGdkWindow) {
    return NS_OK;
  }

  GdkEvent event;
  memset(&event, 0, sizeof(GdkEvent));

  static std::map<uint32_t, GdkEventSequence *> sKnownPointers;

  auto result = sKnownPointers.find(aPointerId);
  switch (aPointerState) {
<<<<<<< HEAD
    case TOUCH_CONTACT:
      if (result == sKnownPointers.end()) {
        // GdkEventSequence isn't a thing we can instantiate, and never gets
        // dereferenced in the gtk code. It's an opaque pointer, the only
        // requirement is that it be distinct from other instances of
        // GdkEventSequence*.
        event.touch.sequence = (GdkEventSequence *)((uintptr_t)aPointerId);
        sKnownPointers[aPointerId] = event.touch.sequence;
        event.type = GDK_TOUCH_BEGIN;
      } else {
        event.touch.sequence = result->second;
        event.type = GDK_TOUCH_UPDATE;
      }
      break;
    case TOUCH_REMOVE:
      event.type = GDK_TOUCH_END;
      if (result == sKnownPointers.end()) {
        NS_WARNING("Tried to synthesize touch-end for unknown pointer!");
        return NS_ERROR_UNEXPECTED;
      }
||||||| merged common ancestors
  case TOUCH_CONTACT:
    if (result == sKnownPointers.end()) {
      // GdkEventSequence isn't a thing we can instantiate, and never gets
      // dereferenced in the gtk code. It's an opaque pointer, the only
      // requirement is that it be distinct from other instances of
      // GdkEventSequence*.
      event.touch.sequence = (GdkEventSequence*)((uintptr_t)aPointerId);
      sKnownPointers[aPointerId] = event.touch.sequence;
      event.type = GDK_TOUCH_BEGIN;
    } else {
=======
    case TOUCH_CONTACT:
      if (result == sKnownPointers.end()) {
        // GdkEventSequence isn't a thing we can instantiate, and never gets
        // dereferenced in the gtk code. It's an opaque pointer, the only
        // requirement is that it be distinct from other instances of
        // GdkEventSequence*.
        event.touch.sequence = (GdkEventSequence*)((uintptr_t)aPointerId);
        sKnownPointers[aPointerId] = event.touch.sequence;
        event.type = GDK_TOUCH_BEGIN;
      } else {
        event.touch.sequence = result->second;
        event.type = GDK_TOUCH_UPDATE;
      }
      break;
    case TOUCH_REMOVE:
      event.type = GDK_TOUCH_END;
      if (result == sKnownPointers.end()) {
        NS_WARNING("Tried to synthesize touch-end for unknown pointer!");
        return NS_ERROR_UNEXPECTED;
      }
>>>>>>> upstream-releases
      event.touch.sequence = result->second;
      sKnownPointers.erase(result);
      break;
    case TOUCH_CANCEL:
      event.type = GDK_TOUCH_CANCEL;
      if (result == sKnownPointers.end()) {
        NS_WARNING("Tried to synthesize touch-cancel for unknown pointer!");
        return NS_ERROR_UNEXPECTED;
      }
      event.touch.sequence = result->second;
      sKnownPointers.erase(result);
      break;
    case TOUCH_HOVER:
    default:
      return NS_ERROR_NOT_IMPLEMENTED;
  }

  event.touch.window = mGdkWindow;
  event.touch.time = GDK_CURRENT_TIME;

  GdkDisplay *display = gdk_window_get_display(mGdkWindow);
  GdkDeviceManager *device_manager = gdk_display_get_device_manager(display);
  event.touch.device = gdk_device_manager_get_client_pointer(device_manager);

  event.touch.x_root = DevicePixelsToGdkCoordRoundDown(aPoint.x);
  event.touch.y_root = DevicePixelsToGdkCoordRoundDown(aPoint.y);

  LayoutDeviceIntPoint pointInWindow = aPoint - WidgetToScreenOffset();
  event.touch.x = DevicePixelsToGdkCoordRoundDown(pointInWindow.x);
  event.touch.y = DevicePixelsToGdkCoordRoundDown(pointInWindow.y);

  gdk_event_put(&event);

  return NS_OK;
}
#endif

<<<<<<< HEAD
nsWindow::CSDSupportLevel nsWindow::GetSystemCSDSupportLevel() {
  if (sCSDSupportLevel != CSD_SUPPORT_UNKNOWN) {
    return sCSDSupportLevel;
  }
||||||| merged common ancestors
nsWindow::CSDSupportLevel
nsWindow::GetSystemCSDSupportLevel() {
    if (sCSDSupportLevel != CSD_SUPPORT_UNKNOWN) {
        return sCSDSupportLevel;
    }
=======
nsWindow::CSDSupportLevel nsWindow::GetSystemCSDSupportLevel() {
  if (sCSDSupportLevel != CSD_SUPPORT_UNKNOWN) {
    return sCSDSupportLevel;
  }

  // Require GTK 3.10 for GtkHeaderBar support and compatible window manager.
  if (gtk_check_version(3, 10, 0) != nullptr) {
    sCSDSupportLevel = CSD_SUPPORT_NONE;
    return sCSDSupportLevel;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Require GTK 3.10 for GtkHeaderBar support and compatible window manager.
  if (gtk_check_version(3, 10, 0) != nullptr) {
    sCSDSupportLevel = CSD_SUPPORT_NONE;
    return sCSDSupportLevel;
  }
||||||| merged common ancestors
    // Require GTK 3.10 for GtkHeaderBar support and compatible window manager.
    if (gtk_check_version(3, 10, 0) != nullptr) {
        sCSDSupportLevel = CSD_SUPPORT_NONE;
        return sCSDSupportLevel;
    }
=======
  // Allow MOZ_GTK_TITLEBAR_DECORATION to override our heuristics
  const char* decorationOverride = getenv("MOZ_GTK_TITLEBAR_DECORATION");
  if (decorationOverride) {
    if (strcmp(decorationOverride, "none") == 0) {
      sCSDSupportLevel = CSD_SUPPORT_NONE;
    } else if (strcmp(decorationOverride, "client") == 0) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
    } else if (strcmp(decorationOverride, "system") == 0) {
      sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
    }
    return sCSDSupportLevel;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  const char *currentDesktop = getenv("XDG_CURRENT_DESKTOP");
  if (currentDesktop) {
    // GNOME Flashback (fallback)
    if (strstr(currentDesktop, "GNOME-Flashback:GNOME") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
      // gnome-shell
    } else if (strstr(currentDesktop, "GNOME") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
    } else if (strstr(currentDesktop, "XFCE") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
    } else if (strstr(currentDesktop, "X-Cinnamon") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
      // KDE Plasma
    } else if (strstr(currentDesktop, "KDE") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
    } else if (strstr(currentDesktop, "Enlightenment") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
    } else if (strstr(currentDesktop, "LXDE") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
    } else if (strstr(currentDesktop, "openbox") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
    } else if (strstr(currentDesktop, "i3") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_NONE;
    } else if (strstr(currentDesktop, "MATE") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
      // Ubuntu Unity
    } else if (strstr(currentDesktop, "Unity") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
      // Elementary OS
    } else if (strstr(currentDesktop, "Pantheon") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
    } else if (strstr(currentDesktop, "LXQt") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
    } else if (strstr(currentDesktop, "Deepin") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
    } else {
||||||| merged common ancestors
    const char* currentDesktop = getenv("XDG_CURRENT_DESKTOP");
    if (currentDesktop) {
        // GNOME Flashback (fallback)
        if (strstr(currentDesktop, "GNOME-Flashback:GNOME") != nullptr) {
            sCSDSupportLevel = CSD_SUPPORT_CLIENT;
        // gnome-shell
        } else if (strstr(currentDesktop, "GNOME") != nullptr) {
            sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
        } else if (strstr(currentDesktop, "XFCE") != nullptr) {
            sCSDSupportLevel = CSD_SUPPORT_CLIENT;
        } else if (strstr(currentDesktop, "X-Cinnamon") != nullptr) {
            sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
        // KDE Plasma
        } else if (strstr(currentDesktop, "KDE") != nullptr) {
            sCSDSupportLevel = CSD_SUPPORT_CLIENT;
        } else if (strstr(currentDesktop, "Enlightenment") != nullptr) {
            sCSDSupportLevel = CSD_SUPPORT_CLIENT;
        } else if (strstr(currentDesktop, "LXDE") != nullptr) {
            sCSDSupportLevel = CSD_SUPPORT_CLIENT;
        } else if (strstr(currentDesktop, "openbox") != nullptr) {
            sCSDSupportLevel = CSD_SUPPORT_CLIENT;
        } else if (strstr(currentDesktop, "i3") != nullptr) {
            sCSDSupportLevel = CSD_SUPPORT_NONE;
        } else if (strstr(currentDesktop, "MATE") != nullptr) {
            sCSDSupportLevel = CSD_SUPPORT_CLIENT;
        // Ubuntu Unity
        } else if (strstr(currentDesktop, "Unity") != nullptr) {
            sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
        // Elementary OS
        } else if (strstr(currentDesktop, "Pantheon") != nullptr) {
            sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
        } else if (strstr(currentDesktop, "LXQt") != nullptr) {
            sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
        } else if (strstr(currentDesktop, "Deepin") != nullptr) {
            sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
        } else {
=======
  // We use CSD titlebar mode on Wayland only
  if (!GDK_IS_X11_DISPLAY(gdk_display_get_default())) {
    sCSDSupportLevel = CSD_SUPPORT_CLIENT;
    return sCSDSupportLevel;
  }

  const char* currentDesktop = getenv("XDG_CURRENT_DESKTOP");
  if (currentDesktop) {
    // GNOME Flashback (fallback)
    if (strstr(currentDesktop, "GNOME-Flashback:GNOME") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
      // gnome-shell
    } else if (strstr(currentDesktop, "GNOME") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
    } else if (strstr(currentDesktop, "XFCE") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
    } else if (strstr(currentDesktop, "X-Cinnamon") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
      // KDE Plasma
    } else if (strstr(currentDesktop, "KDE") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
    } else if (strstr(currentDesktop, "Enlightenment") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
    } else if (strstr(currentDesktop, "LXDE") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
    } else if (strstr(currentDesktop, "openbox") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
    } else if (strstr(currentDesktop, "i3") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_NONE;
    } else if (strstr(currentDesktop, "MATE") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
      // Ubuntu Unity
    } else if (strstr(currentDesktop, "Unity") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
      // Elementary OS
    } else if (strstr(currentDesktop, "Pantheon") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
    } else if (strstr(currentDesktop, "LXQt") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
    } else if (strstr(currentDesktop, "Deepin") != nullptr) {
      sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
    } else {
>>>>>>> upstream-releases
// Release or beta builds are not supposed to be broken
// so disable titlebar rendering on untested/unknown systems.
#if defined(RELEASE_OR_BETA)
      sCSDSupportLevel = CSD_SUPPORT_NONE;
#else
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
#endif
    }
  } else {
    sCSDSupportLevel = CSD_SUPPORT_NONE;
  }

<<<<<<< HEAD
  // We don't support CSD_SUPPORT_SYSTEM on Wayland
  if (!GDK_IS_X11_DISPLAY(gdk_display_get_default()) &&
      sCSDSupportLevel == CSD_SUPPORT_SYSTEM) {
    sCSDSupportLevel = CSD_SUPPORT_CLIENT;
  }
||||||| merged common ancestors
    // We don't support CSD_SUPPORT_SYSTEM on Wayland
    if (!GDK_IS_X11_DISPLAY(gdk_display_get_default()) &&
        sCSDSupportLevel == CSD_SUPPORT_SYSTEM) {
        sCSDSupportLevel = CSD_SUPPORT_CLIENT;
    }
=======
  // GTK_CSD forces CSD mode - use also CSD because window manager
  // decorations does not work with CSD.
  // We check GTK_CSD as well as gtk_window_should_use_csd() does.
  if (sCSDSupportLevel == CSD_SUPPORT_SYSTEM) {
    const char* csdOverride = getenv("GTK_CSD");
    if (csdOverride && g_strcmp0(csdOverride, "1") == 0) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // GTK_CSD forces CSD mode - use also CSD because window manager
  // decorations does not work with CSD.
  // We check GTK_CSD as well as gtk_window_should_use_csd() does.
  if (sCSDSupportLevel == CSD_SUPPORT_SYSTEM) {
    const char *csdOverride = getenv("GTK_CSD");
    if (csdOverride && g_strcmp0(csdOverride, "1") == 0) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
    }
  }
||||||| merged common ancestors
    // GTK_CSD forces CSD mode - use also CSD because window manager
    // decorations does not work with CSD.
    // We check GTK_CSD as well as gtk_window_should_use_csd() does.
    if (sCSDSupportLevel == CSD_SUPPORT_SYSTEM) {
        const char* csdOverride = getenv("GTK_CSD");
        if (csdOverride && g_strcmp0(csdOverride, "1") == 0) {
            sCSDSupportLevel = CSD_SUPPORT_CLIENT;
        }
    }
=======
  return sCSDSupportLevel;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Allow MOZ_GTK_TITLEBAR_DECORATION to override our heuristics
  const char *decorationOverride = getenv("MOZ_GTK_TITLEBAR_DECORATION");
  if (decorationOverride) {
    if (strcmp(decorationOverride, "none") == 0) {
      sCSDSupportLevel = CSD_SUPPORT_NONE;
    } else if (strcmp(decorationOverride, "client") == 0) {
      sCSDSupportLevel = CSD_SUPPORT_CLIENT;
    } else if (strcmp(decorationOverride, "system") == 0) {
      sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
    }
  }
||||||| merged common ancestors
    // Allow MOZ_GTK_TITLEBAR_DECORATION to override our heuristics
    const char* decorationOverride = getenv("MOZ_GTK_TITLEBAR_DECORATION");
    if (decorationOverride) {
        if (strcmp(decorationOverride, "none") == 0) {
            sCSDSupportLevel = CSD_SUPPORT_NONE;
        } else if (strcmp(decorationOverride, "client") == 0) {
            sCSDSupportLevel = CSD_SUPPORT_CLIENT;
        } else if (strcmp(decorationOverride, "system") == 0) {
            sCSDSupportLevel = CSD_SUPPORT_SYSTEM;
        }
    }
=======
// Check for Mutter regression on X.org (Bug 1530252). In that case we
// don't hide system titlebar by default as we can't draw transparent
// corners reliably.
bool nsWindow::TitlebarCanUseShapeMask() {
  static int canUseShapeMask = -1;
  if (canUseShapeMask != -1) {
    return canUseShapeMask;
  }
  canUseShapeMask = true;
>>>>>>> upstream-releases

<<<<<<< HEAD
  return sCSDSupportLevel;
||||||| merged common ancestors
    return sCSDSupportLevel;
=======
  const char* currentDesktop = getenv("XDG_CURRENT_DESKTOP");
  if (!currentDesktop) {
    return canUseShapeMask;
  }

  if (strstr(currentDesktop, "GNOME-Flashback:GNOME") != nullptr ||
      strstr(currentDesktop, "GNOME") != nullptr) {
    const char* sessionType = getenv("XDG_SESSION_TYPE");
    canUseShapeMask = (sessionType && strstr(sessionType, "x11") == nullptr);
  }

  return canUseShapeMask;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool nsWindow::TopLevelWindowUseARGBVisual() {
  static int useARGBVisual = -1;
  if (useARGBVisual != -1) {
    return useARGBVisual;
  }
||||||| merged common ancestors
bool
nsWindow::TopLevelWindowUseARGBVisual()
{
    static int useARGBVisual = -1;
    if (useARGBVisual != -1) {
        return useARGBVisual;
    }
=======
bool nsWindow::HideTitlebarByDefault() {
  static int hideTitlebar = -1;
  if (hideTitlebar != -1) {
    return hideTitlebar;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (Preferences::HasUserValue("mozilla.widget.use-argb-visuals")) {
    useARGBVisual =
        Preferences::GetBool("mozilla.widget.use-argb-visuals", false);
  } else {
    const char *currentDesktop = getenv("XDG_CURRENT_DESKTOP");
    useARGBVisual =
        (currentDesktop && GetSystemCSDSupportLevel() != CSD_SUPPORT_NONE);

    if (useARGBVisual) {
      useARGBVisual =
          (strstr(currentDesktop, "GNOME-Flashback:GNOME") != nullptr ||
           strstr(currentDesktop, "GNOME") != nullptr);
    }
  }
||||||| merged common ancestors
    if (Preferences::HasUserValue("mozilla.widget.use-argb-visuals")) {
        useARGBVisual =
            Preferences::GetBool("mozilla.widget.use-argb-visuals", false);
    } else {
        const char* currentDesktop = getenv("XDG_CURRENT_DESKTOP");
        useARGBVisual =
            (currentDesktop &&
             GetSystemCSDSupportLevel() != CSD_SUPPORT_NONE);

        if (useARGBVisual) {
            useARGBVisual =
                (strstr(currentDesktop, "GNOME-Flashback:GNOME") != nullptr ||
                 strstr(currentDesktop, "GNOME") != nullptr);
        }
    }
=======
  // When user defined widget.default-hidden-titlebar don't do any
  // heuristics and just follow it.
  if (Preferences::HasUserValue("widget.default-hidden-titlebar")) {
    hideTitlebar =
        Preferences::GetBool("widget.default-hidden-titlebar", false);
    return hideTitlebar;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return useARGBVisual;
}
||||||| merged common ancestors
    return useARGBVisual;
}
=======
  const char* currentDesktop = getenv("XDG_CURRENT_DESKTOP");
  hideTitlebar =
      (currentDesktop && GetSystemCSDSupportLevel() != CSD_SUPPORT_NONE);
>>>>>>> upstream-releases

<<<<<<< HEAD
int32_t nsWindow::RoundsWidgetCoordinatesTo() { return GdkScaleFactor(); }
||||||| merged common ancestors
int32_t
nsWindow::RoundsWidgetCoordinatesTo()
{
    return GdkScaleFactor();
}
=======
  // Disable system titlebar for Gnome only for now. It uses window
  // manager decorations and does not suffer from CSD Bugs #1525850, #1527837.
  if (hideTitlebar) {
    hideTitlebar =
        (strstr(currentDesktop, "GNOME-Flashback:GNOME") != nullptr ||
         strstr(currentDesktop, "GNOME") != nullptr);
  }

  // We use shape mask to render the titlebar by default so check for it.
  if (hideTitlebar) {
    hideTitlebar = TitlebarCanUseShapeMask();
  }

  return hideTitlebar;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
void nsWindow::GetCompositorWidgetInitData(
    mozilla::widget::CompositorWidgetInitData *aInitData) {
||||||| merged common ancestors
void nsWindow::GetCompositorWidgetInitData(mozilla::widget::CompositorWidgetInitData* aInitData)
{
=======
int32_t nsWindow::RoundsWidgetCoordinatesTo() { return GdkScaleFactor(); }

void nsWindow::GetCompositorWidgetInitData(
    mozilla::widget::CompositorWidgetInitData* aInitData) {
>>>>>>> upstream-releases
  // Make sure the window XID is propagated to X server, we can fail otherwise
  // in GPU process (Bug 1401634).
  if (mXDisplay && mXWindow != X11None) {
    XFlush(mXDisplay);
  }

  *aInitData = mozilla::widget::GtkCompositorWidgetInitData(
<<<<<<< HEAD
      (mXWindow != X11None) ? mXWindow : (uintptr_t) nullptr,
      mXDisplay ? nsCString(XDisplayString(mXDisplay)) : nsCString(),
      mIsTransparent && !mHasAlphaVisual, GetClientSize());
||||||| merged common ancestors
                                (mXWindow != X11None) ? mXWindow : (uintptr_t)nullptr,
                                mXDisplay ? nsCString(XDisplayString(mXDisplay)) : nsCString(),
                                mIsTransparent && !mHasAlphaVisual,
                                GetClientSize());
=======
      (mXWindow != X11None) ? mXWindow : (uintptr_t) nullptr,
      mXDisplay ? nsCString(XDisplayString(mXDisplay)) : nsCString(),
      mIsTransparent && !mHasAlphaVisual && !mTransparencyBitmapForTitlebar,
      GetClientSize());
>>>>>>> upstream-releases
}

#ifdef MOZ_WAYLAND
<<<<<<< HEAD
wl_surface *nsWindow::GetWaylandSurface() {
||||||| merged common ancestors
wl_display*
nsWindow::GetWaylandDisplay()
{
  // Available as of GTK 3.8+
  static auto sGdkWaylandDisplayGetWlDisplay =
      (wl_display *(*)(GdkDisplay *))
      dlsym(RTLD_DEFAULT, "gdk_wayland_display_get_wl_display");

  GdkDisplay* gdkDisplay = gdk_display_get_default();
  return mIsX11Display ? nullptr :
                         sGdkWaylandDisplayGetWlDisplay(gdkDisplay);
}

wl_surface*
nsWindow::GetWaylandSurface()
{
=======
wl_surface* nsWindow::GetWaylandSurface() {
>>>>>>> upstream-releases
  if (mContainer)
    return moz_container_get_wl_surface(MOZ_CONTAINER(mContainer));

  NS_WARNING(
      "nsWindow::GetWaylandSurfaces(): We don't have any mContainer for "
      "drawing!");
  return nullptr;
}
<<<<<<< HEAD

bool nsWindow::WaylandSurfaceNeedsClear() {
  if (mContainer) {
    return moz_container_surface_needs_clear(MOZ_CONTAINER(mContainer));
  }

  NS_WARNING(
      "nsWindow::WaylandSurfaceNeedsClear(): We don't have any mContainer!");
  return false;
}
||||||| merged common ancestors
=======

bool nsWindow::WaylandSurfaceNeedsClear() {
  if (mContainer) {
    return moz_container_surface_needs_clear(MOZ_CONTAINER(mContainer));
  }
  return false;
}
>>>>>>> upstream-releases
#endif

#ifdef MOZ_X11
/* XApp progress support currently works by setting a property
 * on a window with this Atom name.  A supporting window manager
 * will notice this and pass it along to whatever handling has
 * been implemented on that end (e.g. passing it on to a taskbar
 * widget.)  There is no issue if WM support is lacking, this is
 * simply ignored in that case.
 *
 * See https://github.com/linuxmint/xapps/blob/master/libxapp/xapp-gtk-window.c
 * for further details.
 */

<<<<<<< HEAD
#define PROGRESS_HINT "_NET_WM_XAPP_PROGRESS"
||||||| merged common ancestors
#define PROGRESS_HINT  "_NET_WM_XAPP_PROGRESS"
=======
#  define PROGRESS_HINT "_NET_WM_XAPP_PROGRESS"
>>>>>>> upstream-releases

<<<<<<< HEAD
static void set_window_hint_cardinal(Window xid, const gchar *atom_name,
                                     gulong cardinal) {
  GdkDisplay *display;
||||||| merged common ancestors
static void
set_window_hint_cardinal (Window       xid,
                          const gchar *atom_name,
                          gulong       cardinal)
{
  GdkDisplay *display;
=======
static void set_window_hint_cardinal(Window xid, const gchar* atom_name,
                                     gulong cardinal) {
  GdkDisplay* display;
>>>>>>> upstream-releases

  display = gdk_display_get_default();

<<<<<<< HEAD
  if (cardinal > 0) {
    XChangeProperty(GDK_DISPLAY_XDISPLAY(display), xid,
                    gdk_x11_get_xatom_by_name_for_display(display, atom_name),
                    XA_CARDINAL, 32, PropModeReplace, (guchar *)&cardinal, 1);
  } else {
    XDeleteProperty(GDK_DISPLAY_XDISPLAY(display), xid,
                    gdk_x11_get_xatom_by_name_for_display(display, atom_name));
||||||| merged common ancestors
  if (cardinal > 0)
  {
    XChangeProperty (GDK_DISPLAY_XDISPLAY (display),
                     xid,
                     gdk_x11_get_xatom_by_name_for_display (display, atom_name),
                     XA_CARDINAL, 32,
                     PropModeReplace,
                     (guchar *) &cardinal, 1);
  }
  else
  {
    XDeleteProperty (GDK_DISPLAY_XDISPLAY (display),
                     xid,
                     gdk_x11_get_xatom_by_name_for_display (display, atom_name));
=======
  if (cardinal > 0) {
    XChangeProperty(GDK_DISPLAY_XDISPLAY(display), xid,
                    gdk_x11_get_xatom_by_name_for_display(display, atom_name),
                    XA_CARDINAL, 32, PropModeReplace, (guchar*)&cardinal, 1);
  } else {
    XDeleteProperty(GDK_DISPLAY_XDISPLAY(display), xid,
                    gdk_x11_get_xatom_by_name_for_display(display, atom_name));
>>>>>>> upstream-releases
  }
}
#endif  // MOZ_X11

void nsWindow::SetProgress(unsigned long progressPercent) {
#ifdef MOZ_X11

  if (!mIsX11Display) {
    return;
  }

  if (!mShell) {
    return;
  }

  progressPercent = MIN(progressPercent, 100);

  set_window_hint_cardinal(GDK_WINDOW_XID(gtk_widget_get_window(mShell)),
                           PROGRESS_HINT, progressPercent);
#endif  // MOZ_X11
}

#ifdef MOZ_X11
<<<<<<< HEAD
void nsWindow::SetCompositorHint(WindowComposeRequest aState) {
  if (mIsX11Display &&
      (!GetLayerManager() ||
       GetLayerManager()->GetBackendType() == LayersBackend::LAYERS_BASIC)) {
    gulong value = aState;
    GdkAtom cardinal_atom = gdk_x11_xatom_to_atom(XA_CARDINAL);
    gdk_property_change(gtk_widget_get_window(mShell),
                        gdk_atom_intern("_NET_WM_BYPASS_COMPOSITOR", FALSE),
                        cardinal_atom,
                        32,  // format
                        GDK_PROP_MODE_REPLACE, (guchar *)&value, 1);
  }
||||||| merged common ancestors
void
nsWindow::SetCompositorHint(WindowComposeRequest aState)
{
    if (mIsX11Display) {
        gulong value = aState;
        GdkAtom cardinal_atom = gdk_x11_xatom_to_atom(XA_CARDINAL);
        gdk_property_change(gtk_widget_get_window(mShell),
                            gdk_atom_intern("_NET_WM_BYPASS_COMPOSITOR", FALSE),
                            cardinal_atom,
                            32, // format
                            GDK_PROP_MODE_REPLACE,
                            (guchar*)&value,
                            1);
    }
=======
void nsWindow::SetCompositorHint(WindowComposeRequest aState) {
  if (mIsX11Display &&
      (!GetLayerManager() ||
       GetLayerManager()->GetBackendType() == LayersBackend::LAYERS_BASIC)) {
    gulong value = aState;
    GdkAtom cardinal_atom = gdk_x11_xatom_to_atom(XA_CARDINAL);
    gdk_property_change(gtk_widget_get_window(mShell),
                        gdk_atom_intern("_NET_WM_BYPASS_COMPOSITOR", FALSE),
                        cardinal_atom,
                        32,  // format
                        GDK_PROP_MODE_REPLACE, (guchar*)&value, 1);
  }
>>>>>>> upstream-releases
}
#endif

<<<<<<< HEAD
nsresult nsWindow::SetSystemFont(const nsCString &aFontName) {
  GtkSettings *settings = gtk_settings_get_default();
  g_object_set(settings, "gtk-font-name", aFontName.get(), nullptr);
  return NS_OK;
||||||| merged common ancestors
nsresult
nsWindow::SetSystemFont(const nsCString& aFontName)
{
    GtkSettings* settings = gtk_settings_get_default();
    g_object_set(settings, "gtk-font-name", aFontName.get(), nullptr);
    return NS_OK;
=======
nsresult nsWindow::SetSystemFont(const nsCString& aFontName) {
  GtkSettings* settings = gtk_settings_get_default();
  g_object_set(settings, "gtk-font-name", aFontName.get(), nullptr);
  return NS_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsWindow::GetSystemFont(nsCString &aFontName) {
  GtkSettings *settings = gtk_settings_get_default();
  gchar *fontName = nullptr;
  g_object_get(settings, "gtk-font-name", &fontName, nullptr);
  if (fontName) {
    aFontName.Assign(fontName);
    g_free(fontName);
  }
  return NS_OK;
||||||| merged common ancestors
nsresult
nsWindow::GetSystemFont(nsCString& aFontName)
{
    GtkSettings* settings = gtk_settings_get_default();
    gchar* fontName = nullptr;
    g_object_get(settings,
                 "gtk-font-name", &fontName,
                 nullptr);
    if (fontName) {
        aFontName.Assign(fontName);
        g_free(fontName);
    }
    return NS_OK;
=======
nsresult nsWindow::GetSystemFont(nsCString& aFontName) {
  GtkSettings* settings = gtk_settings_get_default();
  gchar* fontName = nullptr;
  g_object_get(settings, "gtk-font-name", &fontName, nullptr);
  if (fontName) {
    aFontName.Assign(fontName);
    g_free(fontName);
  }
  return NS_OK;
>>>>>>> upstream-releases
}

already_AddRefed<nsIWidget> nsIWidget::CreateTopLevelWindow() {
  nsCOMPtr<nsIWidget> window = new nsWindow();
  return window.forget();
}

already_AddRefed<nsIWidget> nsIWidget::CreateChildWindow() {
  nsCOMPtr<nsIWidget> window = new nsWindow();
  return window.forget();
}

<<<<<<< HEAD
bool nsWindow::GetTopLevelWindowActiveState(nsIFrame *aFrame) {
||||||| merged common ancestors
bool
nsWindow::GetTopLevelWindowActiveState(nsIFrame *aFrame)
{
=======
bool nsWindow::GetTopLevelWindowActiveState(nsIFrame* aFrame) {
>>>>>>> upstream-releases
  // Used by window frame and button box rendering. We can end up in here in
  // the content process when rendering one of these moz styles freely in a
  // page. Fail in this case, there is no applicable window focus state.
  if (!XRE_IsParentProcess()) {
    return false;
  }
  // All headless windows are considered active so they are painted.
  if (gfxPlatform::IsHeadless()) {
    return true;
  }
  // Get the widget. nsIFrame's GetNearestWidget walks up the view chain
  // until it finds a real window.
  nsWindow *window = static_cast<nsWindow *>(aFrame->GetNearestWidget());
  if (!window) {
    return false;
  }

  // Get our toplevel nsWindow.
  if (!window->mIsTopLevel) {
<<<<<<< HEAD
    GtkWidget *widget = window->GetMozContainerWidget();
    if (!widget) {
      return false;
    }
||||||| merged common ancestors
      GtkWidget *widget = window->GetMozContainerWidget();
      if (!widget) {
        return false;
      }
=======
    GtkWidget* widget = window->GetMozContainerWidget();
    if (!widget) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    GtkWidget *toplevelWidget = gtk_widget_get_toplevel(widget);
    window = get_window_for_gtk_widget(toplevelWidget);
    if (!window) {
      return false;
    }
  }

  return !window->mTitlebarBackdropState;
}

static nsIFrame *FindTitlebarFrame(nsIFrame *aFrame) {
  for (nsIFrame *childFrame : aFrame->PrincipalChildList()) {
    const nsStyleDisplay *frameDisp = childFrame->StyleDisplay();
    if (frameDisp->mAppearance == StyleAppearance::MozWindowTitlebar ||
        frameDisp->mAppearance == StyleAppearance::MozWindowTitlebarMaximized) {
      return childFrame;
    }

    if (nsIFrame *foundFrame = FindTitlebarFrame(childFrame)) {
      return foundFrame;
    }
  }
  return nullptr;
}

void nsWindow::ForceTitlebarRedraw(void) {
  MOZ_ASSERT(mDrawInTitlebar, "We should not redraw invisible titlebar.");

  nsIPresShell *shell =
      mWidgetListener ? mWidgetListener->GetPresShell() : nullptr;
  if (!shell) {
    return;
  }
  nsView *view = nsView::GetViewFor(this);
  if (!view) {
    return;
  }
  nsIFrame *frame = view->GetFrame();
  if (!frame) {
    return;
||||||| merged common ancestors
      GtkWidget *toplevelWidget = gtk_widget_get_toplevel(widget);
      window = get_window_for_gtk_widget(toplevelWidget);
      if (!window) {
        return false;
      }
=======
    GtkWidget* toplevelWidget = gtk_widget_get_toplevel(widget);
    window = get_window_for_gtk_widget(toplevelWidget);
    if (!window) {
      return false;
    }
  }

  return !window->mTitlebarBackdropState;
}

static nsIFrame* FindTitlebarFrame(nsIFrame* aFrame) {
  for (nsIFrame* childFrame : aFrame->PrincipalChildList()) {
    const nsStyleDisplay* frameDisp = childFrame->StyleDisplay();
    if (frameDisp->mAppearance == StyleAppearance::MozWindowTitlebar ||
        frameDisp->mAppearance == StyleAppearance::MozWindowTitlebarMaximized) {
      return childFrame;
    }

    if (nsIFrame* foundFrame = FindTitlebarFrame(childFrame)) {
      return foundFrame;
    }
  }
  return nullptr;
}

nsIFrame* nsWindow::GetFrame(void) {
  nsView* view = nsView::GetViewFor(this);
  if (!view) {
    return nullptr;
  }
  return view->GetFrame();
}

void nsWindow::ForceTitlebarRedraw(void) {
  MOZ_ASSERT(mDrawInTitlebar, "We should not redraw invisible titlebar.");

  if (!mWidgetListener || !mWidgetListener->GetPresShell()) {
    return;
  }

  nsIFrame* frame = GetFrame();
  if (!frame) {
    return;
  }

  frame = FindTitlebarFrame(frame);
  if (frame) {
    nsLayoutUtils::PostRestyleEvent(frame->GetContent()->AsElement(),
                                    RestyleHint{0}, nsChangeHint_RepaintFrame);
  }
}

GtkTextDirection nsWindow::GetTextDirection() {
  nsIFrame* frame = GetFrame();
  if (!frame) {
    return GTK_TEXT_DIR_LTR;
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  frame = FindTitlebarFrame(frame);
  if (frame) {
    nsLayoutUtils::PostRestyleEvent(frame->GetContent()->AsElement(),
                                    nsRestyleHint(0),
                                    nsChangeHint_RepaintFrame);
  }
||||||| merged common ancestors
  return (gFocusWindow == window);
=======
  WritingMode wm = frame->GetWritingMode();
  bool isFrameRTL = !(wm.IsVertical() ? wm.IsVerticalLR() : wm.IsBidiLTR());
  return isFrameRTL ? GTK_TEXT_DIR_RTL : GTK_TEXT_DIR_LTR;
>>>>>>> upstream-releases
}
