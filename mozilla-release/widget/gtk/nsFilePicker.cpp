/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/Types.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <gtk/gtk.h>

#include "nsGtkUtils.h"
#include "nsIFileURL.h"
#include "nsIGIOService.h"
#include "nsIURI.h"
#include "nsIWidget.h"
#include "nsIFile.h"
#include "nsIStringBundle.h"

#include "nsArrayEnumerator.h"
#include "nsMemory.h"
#include "nsEnumeratorUtils.h"
#include "nsNetUtil.h"
#include "nsReadableUtils.h"
#include "mozcontainer.h"

#include "nsFilePicker.h"

using namespace mozilla;

#define MAX_PREVIEW_SIZE 180
// bug 1184009
#define MAX_PREVIEW_SOURCE_SIZE 4096

nsIFile* nsFilePicker::mPrevDisplayDirectory = nullptr;

void nsFilePicker::Shutdown() { NS_IF_RELEASE(mPrevDisplayDirectory); }

static GtkFileChooserAction GetGtkFileChooserAction(int16_t aMode) {
  GtkFileChooserAction action;

  switch (aMode) {
    case nsIFilePicker::modeSave:
      action = GTK_FILE_CHOOSER_ACTION_SAVE;
      break;

    case nsIFilePicker::modeGetFolder:
      action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
      break;

    case nsIFilePicker::modeOpen:
    case nsIFilePicker::modeOpenMultiple:
      action = GTK_FILE_CHOOSER_ACTION_OPEN;
      break;

    default:
      NS_WARNING("Unknown nsIFilePicker mode");
      action = GTK_FILE_CHOOSER_ACTION_OPEN;
      break;
  }

  return action;
}

<<<<<<< HEAD
static void UpdateFilePreviewWidget(GtkFileChooser *file_chooser,
                                    gpointer preview_widget_voidptr) {
  GtkImage *preview_widget = GTK_IMAGE(preview_widget_voidptr);
  char *image_filename = gtk_file_chooser_get_preview_filename(file_chooser);
||||||| merged common ancestors

static void
UpdateFilePreviewWidget(GtkFileChooser *file_chooser,
                        gpointer preview_widget_voidptr)
{
  GtkImage *preview_widget = GTK_IMAGE(preview_widget_voidptr);
  char *image_filename = gtk_file_chooser_get_preview_filename(file_chooser);
=======
static void UpdateFilePreviewWidget(GtkFileChooser* file_chooser,
                                    gpointer preview_widget_voidptr) {
  GtkImage* preview_widget = GTK_IMAGE(preview_widget_voidptr);
  char* image_filename = gtk_file_chooser_get_preview_filename(file_chooser);
>>>>>>> upstream-releases
  struct stat st_buf;

  if (!image_filename) {
    gtk_file_chooser_set_preview_widget_active(file_chooser, FALSE);
    return;
  }

  gint preview_width = 0;
  gint preview_height = 0;
  /* check type of file
   * if file is named pipe, Open is blocking which may lead to UI
   *  nonresponsiveness; if file is directory/socket, it also isn't
   *  likely to get preview */
  if (stat(image_filename, &st_buf) || (!S_ISREG(st_buf.st_mode))) {
    g_free(image_filename);
    gtk_file_chooser_set_preview_widget_active(file_chooser, FALSE);
    return; /* stat failed or file is not regular */
  }

<<<<<<< HEAD
  GdkPixbufFormat *preview_format =
      gdk_pixbuf_get_file_info(image_filename, &preview_width, &preview_height);
  if (!preview_format || preview_width <= 0 || preview_height <= 0 ||
||||||| merged common ancestors
  GdkPixbufFormat *preview_format = gdk_pixbuf_get_file_info(image_filename,
                                                             &preview_width,
                                                             &preview_height);
  if (!preview_format ||
      preview_width <= 0 || preview_height <= 0 ||
=======
  GdkPixbufFormat* preview_format =
      gdk_pixbuf_get_file_info(image_filename, &preview_width, &preview_height);
  if (!preview_format || preview_width <= 0 || preview_height <= 0 ||
>>>>>>> upstream-releases
      preview_width > MAX_PREVIEW_SOURCE_SIZE ||
      preview_height > MAX_PREVIEW_SOURCE_SIZE) {
    g_free(image_filename);
    gtk_file_chooser_set_preview_widget_active(file_chooser, FALSE);
    return;
  }

  GdkPixbuf* preview_pixbuf = nullptr;
  // Only scale down images that are too big
  if (preview_width > MAX_PREVIEW_SIZE || preview_height > MAX_PREVIEW_SIZE) {
    preview_pixbuf = gdk_pixbuf_new_from_file_at_size(
        image_filename, MAX_PREVIEW_SIZE, MAX_PREVIEW_SIZE, nullptr);
  } else {
    preview_pixbuf = gdk_pixbuf_new_from_file(image_filename, nullptr);
  }

  g_free(image_filename);

  if (!preview_pixbuf) {
    gtk_file_chooser_set_preview_widget_active(file_chooser, FALSE);
    return;
  }

  GdkPixbuf* preview_pixbuf_temp = preview_pixbuf;
  preview_pixbuf = gdk_pixbuf_apply_embedded_orientation(preview_pixbuf_temp);
  g_object_unref(preview_pixbuf_temp);

  // This is the easiest way to do center alignment without worrying about
  // containers Minimum 3px padding each side (hence the 6) just to make things
  // nice
  gint x_padding =
      (MAX_PREVIEW_SIZE + 6 - gdk_pixbuf_get_width(preview_pixbuf)) / 2;
  gtk_misc_set_padding(GTK_MISC(preview_widget), x_padding, 0);

  gtk_image_set_from_pixbuf(preview_widget, preview_pixbuf);
  g_object_unref(preview_pixbuf);
  gtk_file_chooser_set_preview_widget_active(file_chooser, TRUE);
}

<<<<<<< HEAD
static nsAutoCString MakeCaseInsensitiveShellGlob(const char *aPattern) {
||||||| merged common ancestors
static nsAutoCString
MakeCaseInsensitiveShellGlob(const char* aPattern) {
=======
static nsAutoCString MakeCaseInsensitiveShellGlob(const char* aPattern) {
>>>>>>> upstream-releases
  // aPattern is UTF8
  nsAutoCString result;
  unsigned int len = strlen(aPattern);

  for (unsigned int i = 0; i < len; i++) {
    if (!g_ascii_isalpha(aPattern[i])) {
      // non-ASCII characters will also trigger this path, so unicode
      // is safely handled albeit case-sensitively
      result.Append(aPattern[i]);
      continue;
    }

    // add the lowercase and uppercase version of a character to a bracket
    // match, so it matches either the lowercase or uppercase char.
    result.Append('[');
    result.Append(g_ascii_tolower(aPattern[i]));
    result.Append(g_ascii_toupper(aPattern[i]));
    result.Append(']');
  }

  return result;
}

NS_IMPL_ISUPPORTS(nsFilePicker, nsIFilePicker)

nsFilePicker::nsFilePicker()
    : mSelectedType(0),
      mRunning(false),
      mAllowURLs(false)
#ifdef MOZ_WIDGET_GTK
      ,
      mFileChooserDelegate(nullptr)
#endif
{
  nsCOMPtr<nsIGIOService> giovfs = do_GetService(NS_GIOSERVICE_CONTRACTID);
  giovfs->ShouldUseFlatpakPortal(&mUseNativeFileChooser);
}

nsFilePicker::~nsFilePicker() {}

void ReadMultipleFiles(gpointer filename, gpointer array) {
  nsCOMPtr<nsIFile> localfile;
<<<<<<< HEAD
  nsresult rv =
      NS_NewNativeLocalFile(nsDependentCString(static_cast<char *>(filename)),
                            false, getter_AddRefs(localfile));
||||||| merged common ancestors
  nsresult rv = NS_NewNativeLocalFile(nsDependentCString(static_cast<char*>(filename)),
                                      false,
                                      getter_AddRefs(localfile));
=======
  nsresult rv =
      NS_NewNativeLocalFile(nsDependentCString(static_cast<char*>(filename)),
                            false, getter_AddRefs(localfile));
>>>>>>> upstream-releases
  if (NS_SUCCEEDED(rv)) {
    nsCOMArray<nsIFile> &files = *static_cast<nsCOMArray<nsIFile> *>(array);
    files.AppendObject(localfile);
  }

  g_free(filename);
}

<<<<<<< HEAD
void nsFilePicker::ReadValuesFromFileChooser(void *file_chooser) {
||||||| merged common ancestors
void
nsFilePicker::ReadValuesFromFileChooser(void *file_chooser)
{
=======
void nsFilePicker::ReadValuesFromFileChooser(void* file_chooser) {
>>>>>>> upstream-releases
  mFiles.Clear();

  if (mMode == nsIFilePicker::modeOpenMultiple) {
    mFileURL.Truncate();

<<<<<<< HEAD
    GSList *list =
        gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(file_chooser));
||||||| merged common ancestors
    GSList *list = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(file_chooser));
=======
    GSList* list =
        gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(file_chooser));
>>>>>>> upstream-releases
    g_slist_foreach(list, ReadMultipleFiles, static_cast<gpointer>(&mFiles));
    g_slist_free(list);
  } else {
    gchar* filename = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(file_chooser));
    mFileURL.Assign(filename);
    g_free(filename);
  }

<<<<<<< HEAD
  GtkFileFilter *filter =
      gtk_file_chooser_get_filter(GTK_FILE_CHOOSER(file_chooser));
  GSList *filter_list =
      gtk_file_chooser_list_filters(GTK_FILE_CHOOSER(file_chooser));
||||||| merged common ancestors
  GtkFileFilter *filter = gtk_file_chooser_get_filter(GTK_FILE_CHOOSER(file_chooser));
  GSList *filter_list = gtk_file_chooser_list_filters(GTK_FILE_CHOOSER(file_chooser));
=======
  GtkFileFilter* filter =
      gtk_file_chooser_get_filter(GTK_FILE_CHOOSER(file_chooser));
  GSList* filter_list =
      gtk_file_chooser_list_filters(GTK_FILE_CHOOSER(file_chooser));
>>>>>>> upstream-releases

  mSelectedType = static_cast<int16_t>(g_slist_index(filter_list, filter));
  g_slist_free(filter_list);

  // Remember last used directory.
  nsCOMPtr<nsIFile> file;
  GetFile(getter_AddRefs(file));
  if (file) {
    nsCOMPtr<nsIFile> dir;
    file->GetParent(getter_AddRefs(dir));
    if (dir) {
      dir.swap(mPrevDisplayDirectory);
    }
  }
}

<<<<<<< HEAD
void nsFilePicker::InitNative(nsIWidget *aParent, const nsAString &aTitle) {
||||||| merged common ancestors
void
nsFilePicker::InitNative(nsIWidget *aParent,
                         const nsAString& aTitle)
{
=======
void nsFilePicker::InitNative(nsIWidget* aParent, const nsAString& aTitle) {
>>>>>>> upstream-releases
  mParentWidget = aParent;
  mTitle.Assign(aTitle);
}

NS_IMETHODIMP
nsFilePicker::AppendFilters(int32_t aFilterMask) {
  mAllowURLs = !!(aFilterMask & filterAllowURLs);
  return nsBaseFilePicker::AppendFilters(aFilterMask);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFilePicker::AppendFilter(const nsAString &aTitle, const nsAString &aFilter) {
||||||| merged common ancestors
nsFilePicker::AppendFilter(const nsAString& aTitle, const nsAString& aFilter)
{
=======
nsFilePicker::AppendFilter(const nsAString& aTitle, const nsAString& aFilter) {
>>>>>>> upstream-releases
  if (aFilter.EqualsLiteral("..apps")) {
    // No platform specific thing we can do here, really....
    return NS_OK;
  }

  nsAutoCString filter, name;
  CopyUTF16toUTF8(aFilter, filter);
  CopyUTF16toUTF8(aTitle, name);

  mFilters.AppendElement(filter);
  mFilterNames.AppendElement(name);

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFilePicker::SetDefaultString(const nsAString &aString) {
||||||| merged common ancestors
nsFilePicker::SetDefaultString(const nsAString& aString)
{
=======
nsFilePicker::SetDefaultString(const nsAString& aString) {
>>>>>>> upstream-releases
  mDefault = aString;

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFilePicker::GetDefaultString(nsAString &aString) {
||||||| merged common ancestors
nsFilePicker::GetDefaultString(nsAString& aString)
{
=======
nsFilePicker::GetDefaultString(nsAString& aString) {
>>>>>>> upstream-releases
  // Per API...
  return NS_ERROR_FAILURE;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFilePicker::SetDefaultExtension(const nsAString &aExtension) {
||||||| merged common ancestors
nsFilePicker::SetDefaultExtension(const nsAString& aExtension)
{
=======
nsFilePicker::SetDefaultExtension(const nsAString& aExtension) {
>>>>>>> upstream-releases
  mDefaultExtension = aExtension;

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFilePicker::GetDefaultExtension(nsAString &aExtension) {
||||||| merged common ancestors
nsFilePicker::GetDefaultExtension(nsAString& aExtension)
{
=======
nsFilePicker::GetDefaultExtension(nsAString& aExtension) {
>>>>>>> upstream-releases
  aExtension = mDefaultExtension;

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFilePicker::GetFilterIndex(int32_t *aFilterIndex) {
||||||| merged common ancestors
nsFilePicker::GetFilterIndex(int32_t *aFilterIndex)
{
=======
nsFilePicker::GetFilterIndex(int32_t* aFilterIndex) {
>>>>>>> upstream-releases
  *aFilterIndex = mSelectedType;

  return NS_OK;
}

NS_IMETHODIMP
nsFilePicker::SetFilterIndex(int32_t aFilterIndex) {
  mSelectedType = aFilterIndex;

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFilePicker::GetFile(nsIFile **aFile) {
||||||| merged common ancestors
nsFilePicker::GetFile(nsIFile **aFile)
{
=======
nsFilePicker::GetFile(nsIFile** aFile) {
>>>>>>> upstream-releases
  NS_ENSURE_ARG_POINTER(aFile);

  *aFile = nullptr;
  nsCOMPtr<nsIURI> uri;
  nsresult rv = GetFileURL(getter_AddRefs(uri));
  if (!uri) return rv;

  nsCOMPtr<nsIFileURL> fileURL(do_QueryInterface(uri, &rv));
  NS_ENSURE_SUCCESS(rv, rv);

  nsCOMPtr<nsIFile> file;
  rv = fileURL->GetFile(getter_AddRefs(file));
  NS_ENSURE_SUCCESS(rv, rv);

  file.forget(aFile);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFilePicker::GetFileURL(nsIURI **aFileURL) {
||||||| merged common ancestors
nsFilePicker::GetFileURL(nsIURI **aFileURL)
{
=======
nsFilePicker::GetFileURL(nsIURI** aFileURL) {
>>>>>>> upstream-releases
  *aFileURL = nullptr;
  return NS_NewURI(aFileURL, mFileURL);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFilePicker::GetFiles(nsISimpleEnumerator **aFiles) {
||||||| merged common ancestors
nsFilePicker::GetFiles(nsISimpleEnumerator **aFiles)
{
=======
nsFilePicker::GetFiles(nsISimpleEnumerator** aFiles) {
>>>>>>> upstream-releases
  NS_ENSURE_ARG_POINTER(aFiles);

  if (mMode == nsIFilePicker::modeOpenMultiple) {
    return NS_NewArrayEnumerator(aFiles, mFiles, NS_GET_IID(nsIFile));
  }

  return NS_ERROR_FAILURE;
}

<<<<<<< HEAD
nsresult nsFilePicker::Show(int16_t *aReturn) {
||||||| merged common ancestors
nsresult
nsFilePicker::Show(int16_t *aReturn)
{
=======
nsresult nsFilePicker::Show(int16_t* aReturn) {
>>>>>>> upstream-releases
  NS_ENSURE_ARG_POINTER(aReturn);

  nsresult rv = Open(nullptr);
  if (NS_FAILED(rv)) return rv;

  while (mRunning) {
    g_main_context_iteration(nullptr, TRUE);
  }

  *aReturn = mResult;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFilePicker::Open(nsIFilePickerShownCallback *aCallback) {
||||||| merged common ancestors
nsFilePicker::Open(nsIFilePickerShownCallback *aCallback)
{
=======
nsFilePicker::Open(nsIFilePickerShownCallback* aCallback) {
>>>>>>> upstream-releases
  // Can't show two dialogs concurrently with the same filepicker
  if (mRunning) return NS_ERROR_NOT_AVAILABLE;

  nsCString title;
  title.Adopt(ToNewUTF8String(mTitle));

<<<<<<< HEAD
  GtkWindow *parent_widget =
      GTK_WINDOW(mParentWidget->GetNativeData(NS_NATIVE_SHELLWIDGET));
||||||| merged common ancestors
  GtkWindow *parent_widget =
    GTK_WINDOW(mParentWidget->GetNativeData(NS_NATIVE_SHELLWIDGET));
=======
  GtkWindow* parent_widget =
      GTK_WINDOW(mParentWidget->GetNativeData(NS_NATIVE_SHELLWIDGET));
>>>>>>> upstream-releases

  GtkFileChooserAction action = GetGtkFileChooserAction(mMode);

  const gchar *accept_button;
  NS_ConvertUTF16toUTF8 buttonLabel(mOkButtonLabel);
  if (!mOkButtonLabel.IsEmpty()) {
    accept_button = buttonLabel.get();
  } else {
    accept_button = nullptr;
  }

<<<<<<< HEAD
  void *file_chooser =
      GtkFileChooserNew(title.get(), parent_widget, action, accept_button);
||||||| merged common ancestors
  void *file_chooser = GtkFileChooserNew(title.get(), parent_widget, action, accept_button);
=======
  void* file_chooser =
      GtkFileChooserNew(title.get(), parent_widget, action, accept_button);
>>>>>>> upstream-releases

  // If we have --enable-proxy-bypass-protection, then don't allow
  // remote URLs to be used.
#ifndef MOZ_PROXY_BYPASS_PROTECTION
  if (mAllowURLs) {
    gtk_file_chooser_set_local_only(GTK_FILE_CHOOSER(file_chooser), FALSE);
  }
#endif

<<<<<<< HEAD
  if (action == GTK_FILE_CHOOSER_ACTION_OPEN ||
      action == GTK_FILE_CHOOSER_ACTION_SAVE) {
    GtkWidget *img_preview = gtk_image_new();
    gtk_file_chooser_set_preview_widget(GTK_FILE_CHOOSER(file_chooser),
                                        img_preview);
    g_signal_connect(file_chooser, "update-preview",
                     G_CALLBACK(UpdateFilePreviewWidget), img_preview);
||||||| merged common ancestors
  if (action == GTK_FILE_CHOOSER_ACTION_OPEN || action == GTK_FILE_CHOOSER_ACTION_SAVE) {
    GtkWidget *img_preview = gtk_image_new();
    gtk_file_chooser_set_preview_widget(GTK_FILE_CHOOSER(file_chooser), img_preview);
    g_signal_connect(file_chooser, "update-preview", G_CALLBACK(UpdateFilePreviewWidget), img_preview);
=======
  if (action == GTK_FILE_CHOOSER_ACTION_OPEN ||
      action == GTK_FILE_CHOOSER_ACTION_SAVE) {
    GtkWidget* img_preview = gtk_image_new();
    gtk_file_chooser_set_preview_widget(GTK_FILE_CHOOSER(file_chooser),
                                        img_preview);
    g_signal_connect(file_chooser, "update-preview",
                     G_CALLBACK(UpdateFilePreviewWidget), img_preview);
>>>>>>> upstream-releases
  }

  GtkFileChooserSetModal(file_chooser, parent_widget, TRUE);

  NS_ConvertUTF16toUTF8 defaultName(mDefault);
  switch (mMode) {
    case nsIFilePicker::modeOpenMultiple:
      gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(file_chooser),
                                           TRUE);
      break;
    case nsIFilePicker::modeSave:
      gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(file_chooser),
                                        defaultName.get());
      break;
  }

  nsCOMPtr<nsIFile> defaultPath;
  if (mDisplayDirectory) {
    mDisplayDirectory->Clone(getter_AddRefs(defaultPath));
  } else if (mPrevDisplayDirectory) {
    mPrevDisplayDirectory->Clone(getter_AddRefs(defaultPath));
  }

  if (defaultPath) {
    if (!defaultName.IsEmpty() && mMode != nsIFilePicker::modeSave) {
      // Try to select the intended file. Even if it doesn't exist, GTK still
      // switches directories.
      defaultPath->AppendNative(defaultName);
      nsAutoCString path;
      defaultPath->GetNativePath(path);
      gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(file_chooser), path.get());
    } else {
      nsAutoCString directory;
      defaultPath->GetNativePath(directory);

#ifdef MOZ_WIDGET_GTK
      // Workaround for problematic refcounting in GTK3 before 3.16.
      // We need to keep a reference to the dialog's internal delegate.
      // Otherwise, if our dialog gets destroyed, we'll lose the dialog's
      // delegate by the time this gets processed in the event loop.
      // See: https://bugzilla.mozilla.org/show_bug.cgi?id=1166741
      if (GTK_IS_DIALOG(file_chooser)) {
<<<<<<< HEAD
        GtkDialog *dialog = GTK_DIALOG(file_chooser);
        GtkContainer *area = GTK_CONTAINER(gtk_dialog_get_content_area(dialog));
        gtk_container_forall(area,
                             [](GtkWidget *widget, gpointer data) {
                               if (GTK_IS_FILE_CHOOSER_WIDGET(widget)) {
                                 auto result =
                                     static_cast<GtkFileChooserWidget **>(data);
                                 *result = GTK_FILE_CHOOSER_WIDGET(widget);
                               }
                             },
                             &mFileChooserDelegate);
||||||| merged common ancestors
        GtkDialog *dialog = GTK_DIALOG(file_chooser);
        GtkContainer *area = GTK_CONTAINER(gtk_dialog_get_content_area(dialog));
        gtk_container_forall(area, [](GtkWidget *widget,
                                      gpointer data) {
            if (GTK_IS_FILE_CHOOSER_WIDGET(widget)) {
              auto result = static_cast<GtkFileChooserWidget**>(data);
              *result = GTK_FILE_CHOOSER_WIDGET(widget);
            }
        }, &mFileChooserDelegate);
=======
        GtkDialog* dialog = GTK_DIALOG(file_chooser);
        GtkContainer* area = GTK_CONTAINER(gtk_dialog_get_content_area(dialog));
        gtk_container_forall(
            area,
            [](GtkWidget* widget, gpointer data) {
              if (GTK_IS_FILE_CHOOSER_WIDGET(widget)) {
                auto result = static_cast<GtkFileChooserWidget**>(data);
                *result = GTK_FILE_CHOOSER_WIDGET(widget);
              }
            },
            &mFileChooserDelegate);
>>>>>>> upstream-releases

        if (mFileChooserDelegate != nullptr) {
          g_object_ref(mFileChooserDelegate);
        }
      }
#endif

      gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser),
                                          directory.get());
    }
  }

  if (GTK_IS_DIALOG(file_chooser)) {
    gtk_dialog_set_default_response(GTK_DIALOG(file_chooser),
                                    GTK_RESPONSE_ACCEPT);
  }

  int32_t count = mFilters.Length();
  for (int32_t i = 0; i < count; ++i) {
    // This is fun... the GTK file picker does not accept a list of filters
    // so we need to split out each string, and add it manually.

    char** patterns = g_strsplit(mFilters[i].get(), ";", -1);
    if (!patterns) {
      return NS_ERROR_OUT_OF_MEMORY;
    }

    GtkFileFilter* filter = gtk_file_filter_new();
    for (int j = 0; patterns[j] != nullptr; ++j) {
      nsAutoCString caseInsensitiveFilter =
          MakeCaseInsensitiveShellGlob(g_strstrip(patterns[j]));
      gtk_file_filter_add_pattern(filter, caseInsensitiveFilter.get());
    }

    g_strfreev(patterns);

    if (!mFilterNames[i].IsEmpty()) {
      // If we have a name for our filter, let's use that.
      const char* filter_name = mFilterNames[i].get();
      gtk_file_filter_set_name(filter, filter_name);
    } else {
      // If we don't have a name, let's just use the filter pattern.
      const char* filter_pattern = mFilters[i].get();
      gtk_file_filter_set_name(filter, filter_pattern);
    }

    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_chooser), filter);

    // Set the initially selected filter
    if (mSelectedType == i) {
      gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(file_chooser), filter);
    }
  }

  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(file_chooser),
                                                 TRUE);

  mRunning = true;
  mCallback = aCallback;
  NS_ADDREF_THIS();
  g_signal_connect(file_chooser, "response", G_CALLBACK(OnResponse), this);
  GtkFileChooserShow(file_chooser);

  return NS_OK;
}

<<<<<<< HEAD
/* static */ void nsFilePicker::OnResponse(void *file_chooser, gint response_id,
                                           gpointer user_data) {
  static_cast<nsFilePicker *>(user_data)->Done(file_chooser, response_id);
||||||| merged common ancestors
/* static */ void
nsFilePicker::OnResponse(void* file_chooser, gint response_id,
                         gpointer user_data)
{
  static_cast<nsFilePicker*>(user_data)->
    Done(file_chooser, response_id);
=======
/* static */
void nsFilePicker::OnResponse(void* file_chooser, gint response_id,
                              gpointer user_data) {
  static_cast<nsFilePicker*>(user_data)->Done(file_chooser, response_id);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ void nsFilePicker::OnDestroy(GtkWidget *file_chooser,
                                          gpointer user_data) {
  static_cast<nsFilePicker *>(user_data)->Done(file_chooser,
                                               GTK_RESPONSE_CANCEL);
||||||| merged common ancestors
/* static */ void
nsFilePicker::OnDestroy(GtkWidget* file_chooser, gpointer user_data)
{
  static_cast<nsFilePicker*>(user_data)->
    Done(file_chooser, GTK_RESPONSE_CANCEL);
=======
/* static */
void nsFilePicker::OnDestroy(GtkWidget* file_chooser, gpointer user_data) {
  static_cast<nsFilePicker*>(user_data)->Done(file_chooser,
                                              GTK_RESPONSE_CANCEL);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsFilePicker::Done(void *file_chooser, gint response) {
||||||| merged common ancestors
void
nsFilePicker::Done(void* file_chooser, gint response)
{
=======
void nsFilePicker::Done(void* file_chooser, gint response) {
>>>>>>> upstream-releases
  mRunning = false;

  int16_t result;
  switch (response) {
    case GTK_RESPONSE_OK:
    case GTK_RESPONSE_ACCEPT:
      ReadValuesFromFileChooser(file_chooser);
      result = nsIFilePicker::returnOK;
      if (mMode == nsIFilePicker::modeSave) {
        nsCOMPtr<nsIFile> file;
        GetFile(getter_AddRefs(file));
        if (file) {
          bool exists = false;
          file->Exists(&exists);
          if (exists) result = nsIFilePicker::returnReplace;
        }
      }
      break;

    case GTK_RESPONSE_CANCEL:
    case GTK_RESPONSE_CLOSE:
    case GTK_RESPONSE_DELETE_EVENT:
      result = nsIFilePicker::returnCancel;
      break;

    default:
      NS_WARNING("Unexpected response");
      result = nsIFilePicker::returnCancel;
      break;
  }

  // A "response" signal won't be sent again but "destroy" will be.
  g_signal_handlers_disconnect_by_func(file_chooser, FuncToGpointer(OnDestroy),
                                       this);

  // When response_id is GTK_RESPONSE_DELETE_EVENT or when called from
  // OnDestroy, the widget would be destroyed anyway but it is fine if
  // gtk_widget_destroy is called more than once.  gtk_widget_destroy has
  // requests that any remaining references be released, but the reference
  // count will not be decremented again if GtkWindow's reference has already
  // been released.
  GtkFileChooserDestroy(file_chooser);

#ifdef MOZ_WIDGET_GTK
  if (mFileChooserDelegate) {
    // Properly deref our acquired reference. We call this after
    // gtk_widget_destroy() to try and ensure that pending file info
    // queries caused by updating the current folder have been cancelled.
    // However, we do not know for certain when the callback will run after
    // cancelled.
    g_idle_add(
        [](gpointer data) -> gboolean {
          g_object_unref(data);
          return G_SOURCE_REMOVE;
        },
        mFileChooserDelegate);
    mFileChooserDelegate = nullptr;
  }
#endif

  if (mCallback) {
    mCallback->Done(result);
    mCallback = nullptr;
  } else {
    mResult = result;
  }
  NS_RELEASE_THIS();
}

// All below functions available as of GTK 3.20+
<<<<<<< HEAD
void *nsFilePicker::GtkFileChooserNew(const gchar *title, GtkWindow *parent,
                                      GtkFileChooserAction action,
                                      const gchar *accept_label) {
  static auto sGtkFileChooserNativeNewPtr =
      (void *(*)(const gchar *, GtkWindow *, GtkFileChooserAction,
                 const gchar *,
                 const gchar *))dlsym(RTLD_DEFAULT,
                                      "gtk_file_chooser_native_new");
  if (mUseNativeFileChooser && sGtkFileChooserNativeNewPtr != nullptr) {
    return (*sGtkFileChooserNativeNewPtr)(title, parent, action, accept_label,
                                          nullptr);
||||||| merged common ancestors

void *
nsFilePicker::GtkFileChooserNew(
        const gchar *title, GtkWindow *parent,
        GtkFileChooserAction action,
        const gchar *accept_label)
{
  static auto sGtkFileChooserNativeNewPtr = (void * (*)(
        const gchar *, GtkWindow *,
        GtkFileChooserAction,
        const gchar *, const gchar *))
      dlsym(RTLD_DEFAULT, "gtk_file_chooser_native_new");
  if (sGtkFileChooserNativeNewPtr != nullptr) {
    return (*sGtkFileChooserNativeNewPtr)(title, parent, action, accept_label, nullptr);
=======
void* nsFilePicker::GtkFileChooserNew(const gchar* title, GtkWindow* parent,
                                      GtkFileChooserAction action,
                                      const gchar* accept_label) {
  static auto sGtkFileChooserNativeNewPtr =
      (void* (*)(const gchar*, GtkWindow*, GtkFileChooserAction, const gchar*,
                 const gchar*))dlsym(RTLD_DEFAULT,
                                     "gtk_file_chooser_native_new");
  if (mUseNativeFileChooser && sGtkFileChooserNativeNewPtr != nullptr) {
    return (*sGtkFileChooserNativeNewPtr)(title, parent, action, accept_label,
                                          nullptr);
>>>>>>> upstream-releases
  }
  if (accept_label == nullptr) {
    accept_label = (action == GTK_FILE_CHOOSER_ACTION_SAVE) ? GTK_STOCK_SAVE
                                                            : GTK_STOCK_OPEN;
  }
<<<<<<< HEAD
  GtkWidget *file_chooser = gtk_file_chooser_dialog_new(
      title, parent, action, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
||||||| merged common ancestors
  GtkWidget *file_chooser = gtk_file_chooser_dialog_new(title, parent, action,
      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
=======
  GtkWidget* file_chooser = gtk_file_chooser_dialog_new(
      title, parent, action, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
>>>>>>> upstream-releases
      accept_label, GTK_RESPONSE_ACCEPT, nullptr);
  gtk_dialog_set_alternative_button_order(
      GTK_DIALOG(file_chooser), GTK_RESPONSE_ACCEPT, GTK_RESPONSE_CANCEL, -1);
  return file_chooser;
}

<<<<<<< HEAD
void nsFilePicker::GtkFileChooserShow(void *file_chooser) {
  static auto sGtkNativeDialogShowPtr =
      (void (*)(void *))dlsym(RTLD_DEFAULT, "gtk_native_dialog_show");
  if (mUseNativeFileChooser && sGtkNativeDialogShowPtr != nullptr) {
||||||| merged common ancestors
void
nsFilePicker::GtkFileChooserShow(void *file_chooser)
{
  static auto sGtkNativeDialogShowPtr = (void (*)(void *))
      dlsym(RTLD_DEFAULT, "gtk_native_dialog_show");
  if (sGtkNativeDialogShowPtr != nullptr) {
=======
void nsFilePicker::GtkFileChooserShow(void* file_chooser) {
  static auto sGtkNativeDialogShowPtr =
      (void (*)(void*))dlsym(RTLD_DEFAULT, "gtk_native_dialog_show");
  if (mUseNativeFileChooser && sGtkNativeDialogShowPtr != nullptr) {
>>>>>>> upstream-releases
    (*sGtkNativeDialogShowPtr)(file_chooser);
  } else {
    g_signal_connect(file_chooser, "destroy", G_CALLBACK(OnDestroy), this);
    gtk_widget_show(GTK_WIDGET(file_chooser));
  }
}

<<<<<<< HEAD
void nsFilePicker::GtkFileChooserDestroy(void *file_chooser) {
  static auto sGtkNativeDialogDestroyPtr =
      (void (*)(void *))dlsym(RTLD_DEFAULT, "gtk_native_dialog_destroy");
  if (mUseNativeFileChooser && sGtkNativeDialogDestroyPtr != nullptr) {
||||||| merged common ancestors
void
nsFilePicker::GtkFileChooserDestroy(void *file_chooser)
{
  static auto sGtkNativeDialogDestroyPtr = (void (*)(void *))
    dlsym(RTLD_DEFAULT, "gtk_native_dialog_destroy");
  if (sGtkNativeDialogDestroyPtr != nullptr) {
=======
void nsFilePicker::GtkFileChooserDestroy(void* file_chooser) {
  static auto sGtkNativeDialogDestroyPtr =
      (void (*)(void*))dlsym(RTLD_DEFAULT, "gtk_native_dialog_destroy");
  if (mUseNativeFileChooser && sGtkNativeDialogDestroyPtr != nullptr) {
>>>>>>> upstream-releases
    (*sGtkNativeDialogDestroyPtr)(file_chooser);
  } else {
    gtk_widget_destroy(GTK_WIDGET(file_chooser));
  }
}

<<<<<<< HEAD
void nsFilePicker::GtkFileChooserSetModal(void *file_chooser,
                                          GtkWindow *parent_widget,
                                          gboolean modal) {
  static auto sGtkNativeDialogSetModalPtr = (void (*)(void *, gboolean))dlsym(
      RTLD_DEFAULT, "gtk_native_dialog_set_modal");
  if (mUseNativeFileChooser && sGtkNativeDialogSetModalPtr != nullptr) {
||||||| merged common ancestors
void
nsFilePicker::GtkFileChooserSetModal(void *file_chooser,
        GtkWindow *parent_widget, gboolean modal)
{
  static auto sGtkNativeDialogSetModalPtr = (void (*)(void *, gboolean))
    dlsym(RTLD_DEFAULT, "gtk_native_dialog_set_modal");
  if (sGtkNativeDialogSetModalPtr != nullptr) {
=======
void nsFilePicker::GtkFileChooserSetModal(void* file_chooser,
                                          GtkWindow* parent_widget,
                                          gboolean modal) {
  static auto sGtkNativeDialogSetModalPtr = (void (*)(void*, gboolean))dlsym(
      RTLD_DEFAULT, "gtk_native_dialog_set_modal");
  if (mUseNativeFileChooser && sGtkNativeDialogSetModalPtr != nullptr) {
>>>>>>> upstream-releases
    (*sGtkNativeDialogSetModalPtr)(file_chooser, modal);
  } else {
    GtkWindow* window = GTK_WINDOW(file_chooser);
    gtk_window_set_modal(window, modal);
    if (parent_widget != nullptr) {
      gtk_window_set_destroy_with_parent(window, modal);
    }
  }
}
