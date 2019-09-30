/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdio.h>
#include <gtk/gtk.h>
#include <unistd.h>
#include "mozilla/Sprintf.h"
#include "progressui.h"
#include "readstrings.h"
#include "updatererrors.h"

#define TIMER_INTERVAL 100

static float sProgressVal;  // between 0 and 100
static gboolean sQuit = FALSE;
static gboolean sEnableUI;
static guint sTimerID;

static GtkWidget* sWin;
static GtkWidget* sLabel;
static GtkWidget* sProgressBar;
static GdkPixbuf* sPixbuf;

StringTable sStrings;

static gboolean UpdateDialog(gpointer data) {
  if (sQuit) {
    gtk_widget_hide(sWin);
    gtk_main_quit();
  }

  float progress = sProgressVal;

  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(sProgressBar),
                                progress / 100.0);

  return TRUE;
}

<<<<<<< HEAD
static gboolean OnDeleteEvent(GtkWidget *widget, GdkEvent *event,
                              gpointer user_data) {
||||||| merged common ancestors
static gboolean
OnDeleteEvent(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
=======
static gboolean OnDeleteEvent(GtkWidget* widget, GdkEvent* event,
                              gpointer user_data) {
>>>>>>> upstream-releases
  return TRUE;
}

<<<<<<< HEAD
int InitProgressUI(int *pargc, char ***pargv) {
  sProgramPath = (*pargv)[0];

||||||| merged common ancestors
int
InitProgressUI(int *pargc, char ***pargv)
{
  sProgramPath = (*pargv)[0];

=======
int InitProgressUI(int* pargc, char*** pargv) {
>>>>>>> upstream-releases
  sEnableUI = gtk_init_check(pargc, pargv);
  if (sEnableUI) {
    // Prepare to show the UI here in case the files are modified by the update.
    char ini_path[PATH_MAX];
    SprintfLiteral(ini_path, "%s.ini", (*pargv)[0]);
    if (ReadStrings(ini_path, &sStrings) != OK) {
      sEnableUI = false;
      return -1;
    }

    char icon_path[PATH_MAX];
    SprintfLiteral(icon_path, "%s/icons/updater.png", (*pargv)[2]);
    sPixbuf = gdk_pixbuf_new_from_file(icon_path, nullptr);
  }
  return 0;
}

<<<<<<< HEAD
int ShowProgressUI() {
  if (!sEnableUI) return -1;
||||||| merged common ancestors
int
ShowProgressUI()
{
  if (!sEnableUI)
    return -1;
=======
int ShowProgressUI() {
  if (!sEnableUI) {
    return -1;
  }
>>>>>>> upstream-releases

  // Only show the Progress UI if the process is taking a significant amount of
  // time where a significant amount of time is defined as .5 seconds after
  // ShowProgressUI is called sProgress is less than 70.
  usleep(500000);

<<<<<<< HEAD
  if (sQuit || sProgressVal > 70.0f) return 0;

  char ini_path[PATH_MAX];
  SprintfLiteral(ini_path, "%s.ini", sProgramPath);

  StringTable strings;
  if (ReadStrings(ini_path, &strings) != OK) return -1;
||||||| merged common ancestors
  if (sQuit || sProgressVal > 70.0f)
    return 0;

  char ini_path[PATH_MAX];
  SprintfLiteral(ini_path, "%s.ini", sProgramPath);

  StringTable strings;
  if (ReadStrings(ini_path, &strings) != OK)
    return -1;
=======
  if (sQuit || sProgressVal > 70.0f) {
    return 0;
  }
>>>>>>> upstream-releases

  sWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
<<<<<<< HEAD
  if (!sWin) return -1;
||||||| merged common ancestors
  if (!sWin)
    return -1;
=======
  if (!sWin) {
    return -1;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static GdkPixbuf *pixbuf;
  char icon_path[PATH_MAX];
  SprintfLiteral(icon_path, "%s.png", sProgramPath);

  g_signal_connect(G_OBJECT(sWin), "delete_event", G_CALLBACK(OnDeleteEvent),
                   nullptr);
||||||| merged common ancestors
  static GdkPixbuf *pixbuf;
  char icon_path[PATH_MAX];
  SprintfLiteral(icon_path, "%s.png", sProgramPath);

  g_signal_connect(G_OBJECT(sWin), "delete_event",
                   G_CALLBACK(OnDeleteEvent), nullptr);
=======
  g_signal_connect(G_OBJECT(sWin), "delete_event", G_CALLBACK(OnDeleteEvent),
                   nullptr);
>>>>>>> upstream-releases

  gtk_window_set_title(GTK_WINDOW(sWin), sStrings.title);
  gtk_window_set_type_hint(GTK_WINDOW(sWin), GDK_WINDOW_TYPE_HINT_DIALOG);
  gtk_window_set_position(GTK_WINDOW(sWin), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_window_set_resizable(GTK_WINDOW(sWin), FALSE);
  gtk_window_set_decorated(GTK_WINDOW(sWin), TRUE);
<<<<<<< HEAD
  gtk_window_set_deletable(GTK_WINDOW(sWin), FALSE);
  pixbuf = gdk_pixbuf_new_from_file(icon_path, nullptr);
  gtk_window_set_icon(GTK_WINDOW(sWin), pixbuf);
  g_object_unref(pixbuf);
||||||| merged common ancestors
  gtk_window_set_deletable(GTK_WINDOW(sWin),FALSE);
  pixbuf = gdk_pixbuf_new_from_file (icon_path, nullptr);
  gtk_window_set_icon(GTK_WINDOW(sWin), pixbuf);
  g_object_unref(pixbuf);
=======
  gtk_window_set_deletable(GTK_WINDOW(sWin), FALSE);
  gtk_window_set_icon(GTK_WINDOW(sWin), sPixbuf);
  g_object_unref(sPixbuf);
>>>>>>> upstream-releases

  GtkWidget* vbox = gtk_vbox_new(TRUE, 6);
  sLabel = gtk_label_new(sStrings.info);
  gtk_misc_set_alignment(GTK_MISC(sLabel), 0.0f, 0.0f);
  sProgressBar = gtk_progress_bar_new();

  gtk_box_pack_start(GTK_BOX(vbox), sLabel, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), sProgressBar, TRUE, TRUE, 0);

  sTimerID = g_timeout_add(TIMER_INTERVAL, UpdateDialog, nullptr);

  gtk_container_set_border_width(GTK_CONTAINER(sWin), 10);
  gtk_container_add(GTK_CONTAINER(sWin), vbox);
  gtk_widget_show_all(sWin);

  gtk_main();
  return 0;
}

// Called on a background thread
void QuitProgressUI() { sQuit = TRUE; }

// Called on a background thread
void UpdateProgressUI(float progress) {
  sProgressVal = progress;  // 32-bit writes are atomic
}
