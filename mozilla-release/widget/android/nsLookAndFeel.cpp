/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/ContentChild.h"
#include "nsStyleConsts.h"
#include "nsXULAppAPI.h"
#include "nsLookAndFeel.h"
#include "gfxFont.h"
#include "gfxFontConstants.h"
#include "mozilla/FontPropertyTypes.h"
#include "mozilla/gfx/2D.h"

using namespace mozilla;
using mozilla::dom::ContentChild;

bool nsLookAndFeel::mInitializedSystemColors = false;
AndroidSystemColors nsLookAndFeel::mSystemColors;

bool nsLookAndFeel::mInitializedShowPassword = false;
bool nsLookAndFeel::mShowPassword = true;

static const char16_t UNICODE_BULLET = 0x2022;

nsLookAndFeel::nsLookAndFeel() : nsXPLookAndFeel() {}

nsLookAndFeel::~nsLookAndFeel() {}

<<<<<<< HEAD
#define BG_PRELIGHT_COLOR NS_RGB(0xee, 0xee, 0xee)
#define FG_PRELIGHT_COLOR NS_RGB(0x77, 0x77, 0x77)
#define BLACK_COLOR NS_RGB(0x00, 0x00, 0x00)
#define DARK_GRAY_COLOR NS_RGB(0x40, 0x40, 0x40)
#define GRAY_COLOR NS_RGB(0x80, 0x80, 0x80)
#define LIGHT_GRAY_COLOR NS_RGB(0xa0, 0xa0, 0xa0)
#define RED_COLOR NS_RGB(0xff, 0x00, 0x00)

nsresult nsLookAndFeel::GetSystemColors() {
  if (!AndroidBridge::Bridge()) return NS_ERROR_FAILURE;
||||||| merged common ancestors
#define BG_PRELIGHT_COLOR      NS_RGB(0xee,0xee,0xee)
#define FG_PRELIGHT_COLOR      NS_RGB(0x77,0x77,0x77)
#define BLACK_COLOR            NS_RGB(0x00,0x00,0x00)
#define DARK_GRAY_COLOR        NS_RGB(0x40,0x40,0x40)
#define GRAY_COLOR             NS_RGB(0x80,0x80,0x80)
#define LIGHT_GRAY_COLOR       NS_RGB(0xa0,0xa0,0xa0)
#define RED_COLOR              NS_RGB(0xff,0x00,0x00)

nsresult
nsLookAndFeel::GetSystemColors()
{
    if (!AndroidBridge::Bridge())
        return NS_ERROR_FAILURE;
=======
#define BG_PRELIGHT_COLOR NS_RGB(0xee, 0xee, 0xee)
#define FG_PRELIGHT_COLOR NS_RGB(0x77, 0x77, 0x77)
#define BLACK_COLOR NS_RGB(0x00, 0x00, 0x00)
#define DARK_GRAY_COLOR NS_RGB(0x40, 0x40, 0x40)
#define GRAY_COLOR NS_RGB(0x80, 0x80, 0x80)
#define LIGHT_GRAY_COLOR NS_RGB(0xa0, 0xa0, 0xa0)
#define RED_COLOR NS_RGB(0xff, 0x00, 0x00)

nsresult nsLookAndFeel::GetSystemColors() {
  if (!AndroidBridge::Bridge()) return NS_ERROR_FAILURE;
>>>>>>> upstream-releases

  AndroidBridge::Bridge()->GetSystemColors(&mSystemColors);

  return NS_OK;
}

void nsLookAndFeel::NativeInit() {
  EnsureInitSystemColors();
  EnsureInitShowPassword();
}

/* virtual */
void nsLookAndFeel::RefreshImpl() {
  nsXPLookAndFeel::RefreshImpl();

  mInitializedSystemColors = false;
  mInitializedShowPassword = false;
}

<<<<<<< HEAD
nsresult nsLookAndFeel::NativeGetColor(ColorID aID, nscolor& aColor) {
  nsresult rv = NS_OK;
||||||| merged common ancestors
nsresult
nsLookAndFeel::NativeGetColor(ColorID aID, nscolor &aColor)
{
    nsresult rv = NS_OK;
=======
nsresult nsLookAndFeel::NativeGetColor(ColorID aID, nscolor& aColor) {
  nsresult rv = NS_OK;

  EnsureInitSystemColors();
  if (!mInitializedSystemColors) {
    // Failure to initialize colors is an error condition. Return black.
    aColor = 0;
    return NS_ERROR_FAILURE;
  }

  // XXX we'll want to use context.obtainStyledAttributes on the java side to
  // get all of these; see TextView.java for a good exmaple.

  switch (aID) {
      // These colors don't seem to be used for anything anymore in Mozilla
      // (except here at least TextSelectBackground and TextSelectForeground)
      // The CSS2 colors below are used.
    case ColorID::WindowBackground:
      aColor = NS_RGB(0xFF, 0xFF, 0xFF);
      break;
    case ColorID::WindowForeground:
      aColor = mSystemColors.textColorPrimary;
      break;
    case ColorID::WidgetBackground:
      aColor = mSystemColors.colorBackground;
      break;
    case ColorID::WidgetForeground:
      aColor = mSystemColors.colorForeground;
      break;
    case ColorID::WidgetSelectBackground:
      aColor = mSystemColors.textColorHighlight;
      break;
    case ColorID::WidgetSelectForeground:
      aColor = mSystemColors.textColorPrimaryInverse;
      break;
    case ColorID::Widget3DHighlight:
      aColor = LIGHT_GRAY_COLOR;
      break;
    case ColorID::Widget3DShadow:
      aColor = DARK_GRAY_COLOR;
      break;
    case ColorID::TextBackground:
      // not used?
      aColor = mSystemColors.colorBackground;
      break;
    case ColorID::TextForeground:
      // not used?
      aColor = mSystemColors.textColorPrimary;
      break;
    case ColorID::TextSelectBackground:
    case ColorID::IMESelectedRawTextBackground:
    case ColorID::IMESelectedConvertedTextBackground:
      // still used
      aColor = mSystemColors.textColorHighlight;
      break;
    case ColorID::TextSelectForeground:
    case ColorID::IMESelectedRawTextForeground:
    case ColorID::IMESelectedConvertedTextForeground:
      // still used
      aColor = mSystemColors.textColorPrimaryInverse;
      break;
    case ColorID::IMERawInputBackground:
    case ColorID::IMEConvertedTextBackground:
      aColor = NS_TRANSPARENT;
      break;
    case ColorID::IMERawInputForeground:
    case ColorID::IMEConvertedTextForeground:
      aColor = NS_SAME_AS_FOREGROUND_COLOR;
      break;
    case ColorID::IMERawInputUnderline:
    case ColorID::IMEConvertedTextUnderline:
      aColor = NS_SAME_AS_FOREGROUND_COLOR;
      break;
    case ColorID::IMESelectedRawTextUnderline:
    case ColorID::IMESelectedConvertedTextUnderline:
      aColor = NS_TRANSPARENT;
      break;
    case ColorID::SpellCheckerUnderline:
      aColor = RED_COLOR;
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
  EnsureInitSystemColors();
  if (!mInitializedSystemColors) {
    // Failure to initialize colors is an error condition. Return black.
    aColor = 0;
    return NS_ERROR_FAILURE;
  }
||||||| merged common ancestors
    EnsureInitSystemColors();
    if (!mInitializedSystemColors) {
        // Failure to initialize colors is an error condition. Return black.
        aColor = 0;
        return NS_ERROR_FAILURE;
    }
=======
      // css2  http://www.w3.org/TR/REC-CSS2/ui.html#system-colors
    case ColorID::Activeborder:
      // active window border
      aColor = mSystemColors.colorBackground;
      break;
    case ColorID::Activecaption:
      // active window caption background
      aColor = mSystemColors.colorBackground;
      break;
    case ColorID::Appworkspace:
      // MDI background color
      aColor = mSystemColors.colorBackground;
      break;
    case ColorID::Background:
      // desktop background
      aColor = mSystemColors.colorBackground;
      break;
    case ColorID::Captiontext:
      // text in active window caption, size box, and scrollbar arrow box (!)
      aColor = mSystemColors.colorForeground;
      break;
    case ColorID::Graytext:
      // disabled text in windows, menus, etc.
      aColor = mSystemColors.textColorTertiary;
      break;
    case ColorID::Highlight:
      // background of selected item
      aColor = mSystemColors.textColorHighlight;
      break;
    case ColorID::Highlighttext:
      // text of selected item
      aColor = mSystemColors.textColorPrimaryInverse;
      break;
    case ColorID::Inactiveborder:
      // inactive window border
      aColor = mSystemColors.colorBackground;
      break;
    case ColorID::Inactivecaption:
      // inactive window caption
      aColor = mSystemColors.colorBackground;
      break;
    case ColorID::Inactivecaptiontext:
      // text in inactive window caption
      aColor = mSystemColors.textColorTertiary;
      break;
    case ColorID::Infobackground:
      // tooltip background color
      aColor = mSystemColors.colorBackground;
      break;
    case ColorID::Infotext:
      // tooltip text color
      aColor = mSystemColors.colorForeground;
      break;
    case ColorID::Menu:
      // menu background
      aColor = mSystemColors.colorBackground;
      break;
    case ColorID::Menutext:
      // menu text
      aColor = mSystemColors.colorForeground;
      break;
    case ColorID::Scrollbar:
      // scrollbar gray area
      aColor = mSystemColors.colorBackground;
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // XXX we'll want to use context.obtainStyledAttributes on the java side to
  // get all of these; see TextView.java for a good exmaple.
||||||| merged common ancestors
    // XXX we'll want to use context.obtainStyledAttributes on the java side to
    // get all of these; see TextView.java for a good exmaple.
=======
    case ColorID::Threedface:
    case ColorID::Buttonface:
      // 3-D face color
      aColor = mSystemColors.colorBackground;
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
  switch (aID) {
      // These colors don't seem to be used for anything anymore in Mozilla
      // (except here at least TextSelectBackground and TextSelectForeground)
      // The CSS2 colors below are used.
    case eColorID_WindowBackground:
      aColor = NS_RGB(0xFF, 0xFF, 0xFF);
      break;
    case eColorID_WindowForeground:
      aColor = mSystemColors.textColorPrimary;
      break;
    case eColorID_WidgetBackground:
      aColor = mSystemColors.colorBackground;
      break;
    case eColorID_WidgetForeground:
      aColor = mSystemColors.colorForeground;
      break;
    case eColorID_WidgetSelectBackground:
      aColor = mSystemColors.textColorHighlight;
      break;
    case eColorID_WidgetSelectForeground:
      aColor = mSystemColors.textColorPrimaryInverse;
      break;
    case eColorID_Widget3DHighlight:
      aColor = LIGHT_GRAY_COLOR;
      break;
    case eColorID_Widget3DShadow:
      aColor = DARK_GRAY_COLOR;
      break;
    case eColorID_TextBackground:
      // not used?
      aColor = mSystemColors.colorBackground;
      break;
    case eColorID_TextForeground:
      // not used?
      aColor = mSystemColors.textColorPrimary;
      break;
    case eColorID_TextSelectBackground:
    case eColorID_IMESelectedRawTextBackground:
    case eColorID_IMESelectedConvertedTextBackground:
      // still used
      aColor = mSystemColors.textColorHighlight;
      break;
    case eColorID_TextSelectForeground:
    case eColorID_IMESelectedRawTextForeground:
    case eColorID_IMESelectedConvertedTextForeground:
      // still used
      aColor = mSystemColors.textColorPrimaryInverse;
      break;
    case eColorID_IMERawInputBackground:
    case eColorID_IMEConvertedTextBackground:
      aColor = NS_TRANSPARENT;
      break;
    case eColorID_IMERawInputForeground:
    case eColorID_IMEConvertedTextForeground:
      aColor = NS_SAME_AS_FOREGROUND_COLOR;
      break;
    case eColorID_IMERawInputUnderline:
    case eColorID_IMEConvertedTextUnderline:
      aColor = NS_SAME_AS_FOREGROUND_COLOR;
      break;
    case eColorID_IMESelectedRawTextUnderline:
    case eColorID_IMESelectedConvertedTextUnderline:
      aColor = NS_TRANSPARENT;
      break;
    case eColorID_SpellCheckerUnderline:
      aColor = RED_COLOR;
||||||| merged common ancestors
    switch (aID) {
        // These colors don't seem to be used for anything anymore in Mozilla
        // (except here at least TextSelectBackground and TextSelectForeground)
        // The CSS2 colors below are used.
    case eColorID_WindowBackground:
        aColor = NS_RGB(0xFF, 0xFF, 0xFF);
        break;
    case eColorID_WindowForeground:
        aColor = mSystemColors.textColorPrimary;
        break;
    case eColorID_WidgetBackground:
        aColor = mSystemColors.colorBackground;
        break;
    case eColorID_WidgetForeground:
        aColor = mSystemColors.colorForeground;
        break;
    case eColorID_WidgetSelectBackground:
        aColor = mSystemColors.textColorHighlight;
        break;
    case eColorID_WidgetSelectForeground:
        aColor = mSystemColors.textColorPrimaryInverse;
        break;
    case eColorID_Widget3DHighlight:
        aColor = LIGHT_GRAY_COLOR;
        break;
    case eColorID_Widget3DShadow:
        aColor = DARK_GRAY_COLOR;
        break;
    case eColorID_TextBackground:
        // not used?
        aColor = mSystemColors.colorBackground;
        break;
    case eColorID_TextForeground:
        // not used?
        aColor = mSystemColors.textColorPrimary;
        break;
    case eColorID_TextSelectBackground:
    case eColorID_IMESelectedRawTextBackground:
    case eColorID_IMESelectedConvertedTextBackground:
        // still used
        aColor = mSystemColors.textColorHighlight;
        break;
    case eColorID_TextSelectForeground:
    case eColorID_IMESelectedRawTextForeground:
    case eColorID_IMESelectedConvertedTextForeground:
        // still used
        aColor = mSystemColors.textColorPrimaryInverse;
        break;
    case eColorID_IMERawInputBackground:
    case eColorID_IMEConvertedTextBackground:
        aColor = NS_TRANSPARENT;
        break;
    case eColorID_IMERawInputForeground:
    case eColorID_IMEConvertedTextForeground:
        aColor = NS_SAME_AS_FOREGROUND_COLOR;
        break;
    case eColorID_IMERawInputUnderline:
    case eColorID_IMEConvertedTextUnderline:
        aColor = NS_SAME_AS_FOREGROUND_COLOR;
        break;
    case eColorID_IMESelectedRawTextUnderline:
    case eColorID_IMESelectedConvertedTextUnderline:
        aColor = NS_TRANSPARENT;
        break;
    case eColorID_SpellCheckerUnderline:
      aColor = RED_COLOR;
=======
    case ColorID::Buttontext:
      // text on push buttons
      aColor = mSystemColors.colorForeground;
>>>>>>> upstream-releases
      break;

<<<<<<< HEAD
      // css2  http://www.w3.org/TR/REC-CSS2/ui.html#system-colors
    case eColorID_activeborder:
      // active window border
      aColor = mSystemColors.colorBackground;
      break;
    case eColorID_activecaption:
      // active window caption background
      aColor = mSystemColors.colorBackground;
      break;
    case eColorID_appworkspace:
      // MDI background color
      aColor = mSystemColors.colorBackground;
      break;
    case eColorID_background:
      // desktop background
      aColor = mSystemColors.colorBackground;
      break;
    case eColorID_captiontext:
      // text in active window caption, size box, and scrollbar arrow box (!)
      aColor = mSystemColors.colorForeground;
      break;
    case eColorID_graytext:
      // disabled text in windows, menus, etc.
      aColor = mSystemColors.textColorTertiary;
      break;
    case eColorID_highlight:
      // background of selected item
      aColor = mSystemColors.textColorHighlight;
      break;
    case eColorID_highlighttext:
      // text of selected item
      aColor = mSystemColors.textColorPrimaryInverse;
      break;
    case eColorID_inactiveborder:
      // inactive window border
      aColor = mSystemColors.colorBackground;
      break;
    case eColorID_inactivecaption:
      // inactive window caption
      aColor = mSystemColors.colorBackground;
      break;
    case eColorID_inactivecaptiontext:
      // text in inactive window caption
      aColor = mSystemColors.textColorTertiary;
      break;
    case eColorID_infobackground:
      // tooltip background color
      aColor = mSystemColors.colorBackground;
      break;
    case eColorID_infotext:
      // tooltip text color
      aColor = mSystemColors.colorForeground;
      break;
    case eColorID_menu:
      // menu background
      aColor = mSystemColors.colorBackground;
      break;
    case eColorID_menutext:
      // menu text
      aColor = mSystemColors.colorForeground;
      break;
    case eColorID_scrollbar:
      // scrollbar gray area
      aColor = mSystemColors.colorBackground;
      break;
||||||| merged common ancestors
        // css2  http://www.w3.org/TR/REC-CSS2/ui.html#system-colors
    case eColorID_activeborder:
        // active window border
        aColor = mSystemColors.colorBackground;
        break;
    case eColorID_activecaption:
        // active window caption background
        aColor = mSystemColors.colorBackground;
        break;
    case eColorID_appworkspace:
        // MDI background color
        aColor = mSystemColors.colorBackground;
        break;
    case eColorID_background:
        // desktop background
        aColor = mSystemColors.colorBackground;
        break;
    case eColorID_captiontext:
        // text in active window caption, size box, and scrollbar arrow box (!)
        aColor = mSystemColors.colorForeground;
        break;
    case eColorID_graytext:
        // disabled text in windows, menus, etc.
        aColor = mSystemColors.textColorTertiary;
        break;
    case eColorID_highlight:
        // background of selected item
        aColor = mSystemColors.textColorHighlight;
        break;
    case eColorID_highlighttext:
        // text of selected item
        aColor = mSystemColors.textColorPrimaryInverse;
        break;
    case eColorID_inactiveborder:
        // inactive window border
        aColor = mSystemColors.colorBackground;
        break;
    case eColorID_inactivecaption:
        // inactive window caption
        aColor = mSystemColors.colorBackground;
        break;
    case eColorID_inactivecaptiontext:
        // text in inactive window caption
        aColor = mSystemColors.textColorTertiary;
        break;
    case eColorID_infobackground:
        // tooltip background color
        aColor = mSystemColors.colorBackground;
        break;
    case eColorID_infotext:
        // tooltip text color
        aColor = mSystemColors.colorForeground;
        break;
    case eColorID_menu:
        // menu background
        aColor = mSystemColors.colorBackground;
        break;
    case eColorID_menutext:
        // menu text
        aColor = mSystemColors.colorForeground;
        break;
    case eColorID_scrollbar:
        // scrollbar gray area
        aColor = mSystemColors.colorBackground;
        break;
=======
    case ColorID::Buttonhighlight:
      // 3-D highlighted edge color
    case ColorID::Threedhighlight:
      // 3-D highlighted outer edge color
      aColor = LIGHT_GRAY_COLOR;
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
    case eColorID_threedface:
    case eColorID_buttonface:
      // 3-D face color
      aColor = mSystemColors.colorBackground;
      break;
||||||| merged common ancestors
    case eColorID_threedface:
    case eColorID_buttonface:
        // 3-D face color
        aColor = mSystemColors.colorBackground;
        break;
=======
    case ColorID::Threedlightshadow:
      // 3-D highlighted inner edge color
      aColor = mSystemColors.colorBackground;
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
    case eColorID_buttontext:
      // text on push buttons
      aColor = mSystemColors.colorForeground;
      break;
||||||| merged common ancestors
    case eColorID_buttontext:
        // text on push buttons
        aColor = mSystemColors.colorForeground;
        break;
=======
    case ColorID::Buttonshadow:
      // 3-D shadow edge color
    case ColorID::Threedshadow:
      // 3-D shadow inner edge color
      aColor = GRAY_COLOR;
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
    case eColorID_buttonhighlight:
      // 3-D highlighted edge color
    case eColorID_threedhighlight:
      // 3-D highlighted outer edge color
      aColor = LIGHT_GRAY_COLOR;
      break;
||||||| merged common ancestors
    case eColorID_buttonhighlight:
        // 3-D highlighted edge color
    case eColorID_threedhighlight:
        // 3-D highlighted outer edge color
        aColor = LIGHT_GRAY_COLOR;
        break;
=======
    case ColorID::Threeddarkshadow:
      // 3-D shadow outer edge color
      aColor = BLACK_COLOR;
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
    case eColorID_threedlightshadow:
      // 3-D highlighted inner edge color
      aColor = mSystemColors.colorBackground;
      break;
||||||| merged common ancestors
    case eColorID_threedlightshadow:
        // 3-D highlighted inner edge color
        aColor = mSystemColors.colorBackground;
        break;
=======
    case ColorID::Window:
    case ColorID::Windowframe:
      aColor = mSystemColors.colorBackground;
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
    case eColorID_buttonshadow:
      // 3-D shadow edge color
    case eColorID_threedshadow:
      // 3-D shadow inner edge color
      aColor = GRAY_COLOR;
      break;
||||||| merged common ancestors
    case eColorID_buttonshadow:
        // 3-D shadow edge color
    case eColorID_threedshadow:
        // 3-D shadow inner edge color
        aColor = GRAY_COLOR;
        break;
=======
    case ColorID::Windowtext:
      aColor = mSystemColors.textColorPrimary;
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
    case eColorID_threeddarkshadow:
      // 3-D shadow outer edge color
      aColor = BLACK_COLOR;
      break;
||||||| merged common ancestors
    case eColorID_threeddarkshadow:
        // 3-D shadow outer edge color
        aColor = BLACK_COLOR;
        break;
=======
    case ColorID::MozEventreerow:
    case ColorID::MozField:
      aColor = mSystemColors.colorBackground;
      break;
    case ColorID::MozFieldtext:
      aColor = mSystemColors.textColorPrimary;
      break;
    case ColorID::MozDialog:
      aColor = mSystemColors.colorBackground;
      break;
    case ColorID::MozDialogtext:
      aColor = mSystemColors.colorForeground;
      break;
    case ColorID::MozDragtargetzone:
      aColor = mSystemColors.textColorHighlight;
      break;
    case ColorID::MozButtondefault:
      // default button border color
      aColor = BLACK_COLOR;
      break;
    case ColorID::MozButtonhoverface:
      aColor = BG_PRELIGHT_COLOR;
      break;
    case ColorID::MozButtonhovertext:
      aColor = FG_PRELIGHT_COLOR;
      break;
    case ColorID::MozCellhighlight:
    case ColorID::MozHtmlCellhighlight:
      aColor = mSystemColors.textColorHighlight;
      break;
    case ColorID::MozCellhighlighttext:
    case ColorID::MozHtmlCellhighlighttext:
      aColor = mSystemColors.textColorPrimaryInverse;
      break;
    case ColorID::MozMenuhover:
      aColor = BG_PRELIGHT_COLOR;
      break;
    case ColorID::MozMenuhovertext:
      aColor = FG_PRELIGHT_COLOR;
      break;
    case ColorID::MozOddtreerow:
      aColor = NS_TRANSPARENT;
      break;
    case ColorID::MozNativehyperlinktext:
      aColor = NS_SAME_AS_FOREGROUND_COLOR;
      break;
    case ColorID::MozComboboxtext:
      aColor = mSystemColors.colorForeground;
      break;
    case ColorID::MozCombobox:
      aColor = mSystemColors.colorBackground;
      break;
    case ColorID::MozMenubartext:
      aColor = mSystemColors.colorForeground;
      break;
    case ColorID::MozMenubarhovertext:
      aColor = FG_PRELIGHT_COLOR;
      break;
    default:
      /* default color is BLACK */
      aColor = 0;
      rv = NS_ERROR_FAILURE;
      break;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case eColorID_window:
    case eColorID_windowframe:
      aColor = mSystemColors.colorBackground;
      break;
||||||| merged common ancestors
    case eColorID_window:
    case eColorID_windowframe:
        aColor = mSystemColors.colorBackground;
        break;
=======
  return rv;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    case eColorID_windowtext:
      aColor = mSystemColors.textColorPrimary;
      break;
||||||| merged common ancestors
    case eColorID_windowtext:
        aColor = mSystemColors.textColorPrimary;
        break;
=======
nsresult nsLookAndFeel::GetIntImpl(IntID aID, int32_t& aResult) {
  nsresult rv = nsXPLookAndFeel::GetIntImpl(aID, aResult);
  if (NS_SUCCEEDED(rv)) return rv;
>>>>>>> upstream-releases

<<<<<<< HEAD
    case eColorID__moz_eventreerow:
    case eColorID__moz_field:
      aColor = mSystemColors.colorBackground;
      break;
    case eColorID__moz_fieldtext:
      aColor = mSystemColors.textColorPrimary;
      break;
    case eColorID__moz_dialog:
      aColor = mSystemColors.colorBackground;
      break;
    case eColorID__moz_dialogtext:
      aColor = mSystemColors.colorForeground;
      break;
    case eColorID__moz_dragtargetzone:
      aColor = mSystemColors.textColorHighlight;
      break;
    case eColorID__moz_buttondefault:
      // default button border color
      aColor = BLACK_COLOR;
      break;
    case eColorID__moz_buttonhoverface:
      aColor = BG_PRELIGHT_COLOR;
      break;
    case eColorID__moz_buttonhovertext:
      aColor = FG_PRELIGHT_COLOR;
      break;
    case eColorID__moz_cellhighlight:
    case eColorID__moz_html_cellhighlight:
      aColor = mSystemColors.textColorHighlight;
      break;
    case eColorID__moz_cellhighlighttext:
    case eColorID__moz_html_cellhighlighttext:
      aColor = mSystemColors.textColorPrimaryInverse;
      break;
    case eColorID__moz_menuhover:
      aColor = BG_PRELIGHT_COLOR;
      break;
    case eColorID__moz_menuhovertext:
      aColor = FG_PRELIGHT_COLOR;
      break;
    case eColorID__moz_oddtreerow:
      aColor = NS_TRANSPARENT;
      break;
    case eColorID__moz_nativehyperlinktext:
      aColor = NS_SAME_AS_FOREGROUND_COLOR;
      break;
    case eColorID__moz_comboboxtext:
      aColor = mSystemColors.colorForeground;
      break;
    case eColorID__moz_combobox:
      aColor = mSystemColors.colorBackground;
      break;
    case eColorID__moz_menubartext:
      aColor = mSystemColors.colorForeground;
      break;
    case eColorID__moz_menubarhovertext:
      aColor = FG_PRELIGHT_COLOR;
      break;
||||||| merged common ancestors
    case eColorID__moz_eventreerow:
    case eColorID__moz_field:
        aColor = mSystemColors.colorBackground;
        break;
    case eColorID__moz_fieldtext:
        aColor = mSystemColors.textColorPrimary;
        break;
    case eColorID__moz_dialog:
        aColor = mSystemColors.colorBackground;
        break;
    case eColorID__moz_dialogtext:
        aColor = mSystemColors.colorForeground;
        break;
    case eColorID__moz_dragtargetzone:
        aColor = mSystemColors.textColorHighlight;
        break;
    case eColorID__moz_buttondefault:
        // default button border color
        aColor = BLACK_COLOR;
        break;
    case eColorID__moz_buttonhoverface:
        aColor = BG_PRELIGHT_COLOR;
        break;
    case eColorID__moz_buttonhovertext:
        aColor = FG_PRELIGHT_COLOR;
        break;
    case eColorID__moz_cellhighlight:
    case eColorID__moz_html_cellhighlight:
        aColor = mSystemColors.textColorHighlight;
        break;
    case eColorID__moz_cellhighlighttext:
    case eColorID__moz_html_cellhighlighttext:
        aColor = mSystemColors.textColorPrimaryInverse;
        break;
    case eColorID__moz_menuhover:
        aColor = BG_PRELIGHT_COLOR;
        break;
    case eColorID__moz_menuhovertext:
        aColor = FG_PRELIGHT_COLOR;
        break;
    case eColorID__moz_oddtreerow:
        aColor = NS_TRANSPARENT;
        break;
    case eColorID__moz_nativehyperlinktext:
        aColor = NS_SAME_AS_FOREGROUND_COLOR;
        break;
    case eColorID__moz_comboboxtext:
        aColor = mSystemColors.colorForeground;
        break;
    case eColorID__moz_combobox:
        aColor = mSystemColors.colorBackground;
        break;
    case eColorID__moz_menubartext:
        aColor = mSystemColors.colorForeground;
        break;
    case eColorID__moz_menubarhovertext:
        aColor = FG_PRELIGHT_COLOR;
        break;
=======
  rv = NS_OK;

  switch (aID) {
    case eIntID_CaretBlinkTime:
      aResult = 500;
      break;

    case eIntID_CaretWidth:
      aResult = 1;
      break;

    case eIntID_ShowCaretDuringSelection:
      aResult = 0;
      break;

    case eIntID_SelectTextfieldsOnKeyFocus:
      // Select textfield content when focused by kbd
      // used by EventStateManager::sTextfieldSelectModel
      aResult = 1;
      break;

    case eIntID_SubmenuDelay:
      aResult = 200;
      break;

    case eIntID_TooltipDelay:
      aResult = 500;
      break;

    case eIntID_MenusCanOverlapOSBar:
      // we want XUL popups to be able to overlap the task bar.
      aResult = 1;
      break;

    case eIntID_ScrollArrowStyle:
      aResult = eScrollArrowStyle_Single;
      break;

    case eIntID_ScrollSliderStyle:
      aResult = eScrollThumbStyle_Proportional;
      break;

    case eIntID_TouchEnabled:
      aResult = 1;
      break;

    case eIntID_WindowsDefaultTheme:
    case eIntID_WindowsThemeIdentifier:
    case eIntID_OperatingSystemVersionIdentifier:
      aResult = 0;
      rv = NS_ERROR_NOT_IMPLEMENTED;
      break;

    case eIntID_SpellCheckerUnderlineStyle:
      aResult = NS_STYLE_TEXT_DECORATION_STYLE_WAVY;
      break;

    case eIntID_ScrollbarButtonAutoRepeatBehavior:
      aResult = 0;
      break;

    case eIntID_ContextMenuOffsetVertical:
    case eIntID_ContextMenuOffsetHorizontal:
      aResult = 2;
      break;

    case eIntID_PrefersReducedMotion:
      if (sIsInPrefersReducedMotionForTest) {
        aResult = sPrefersReducedMotionForTest ? 1 : 0;
        break;
      }
      aResult = java::GeckoSystemStateListener::PrefersReducedMotion() ? 1 : 0;
      break;

    case eIntID_PrimaryPointerCapabilities:
      aResult = java::GeckoAppShell::GetPrimaryPointerCapabilities();
      break;
    case eIntID_AllPointerCapabilities:
      aResult = java::GeckoAppShell::GetAllPointerCapabilities();
      break;

    case eIntID_SystemUsesDarkTheme:
      // Bail out if AndroidBridge hasn't initialized since we try to query
      // this vailue via nsMediaFeatures::InitSystemMetrics without initializing
      // AndroidBridge on xpcshell tests.
      if (!jni::IsAvailable()) {
        return NS_ERROR_FAILURE;
      }
      aResult = java::GeckoSystemStateListener::IsNightMode() ? 1 : 0;
      break;

>>>>>>> upstream-releases
    default:
<<<<<<< HEAD
      /* default color is BLACK */
      aColor = 0;
      rv = NS_ERROR_FAILURE;
      break;
  }
||||||| merged common ancestors
        /* default color is BLACK */
        aColor = 0;
        rv = NS_ERROR_FAILURE;
        break;
    }
=======
      aResult = 0;
      rv = NS_ERROR_FAILURE;
  }
>>>>>>> upstream-releases

  return rv;
}

<<<<<<< HEAD
nsresult nsLookAndFeel::GetIntImpl(IntID aID, int32_t& aResult) {
  nsresult rv = nsXPLookAndFeel::GetIntImpl(aID, aResult);
  if (NS_SUCCEEDED(rv)) return rv;

  rv = NS_OK;

  switch (aID) {
    case eIntID_CaretBlinkTime:
      aResult = 500;
      break;

    case eIntID_CaretWidth:
      aResult = 1;
      break;

    case eIntID_ShowCaretDuringSelection:
      aResult = 0;
      break;

    case eIntID_SelectTextfieldsOnKeyFocus:
      // Select textfield content when focused by kbd
      // used by EventStateManager::sTextfieldSelectModel
      aResult = 1;
      break;
||||||| merged common ancestors
nsresult
nsLookAndFeel::GetIntImpl(IntID aID, int32_t &aResult)
{
    nsresult rv = nsXPLookAndFeel::GetIntImpl(aID, aResult);
    if (NS_SUCCEEDED(rv))
        return rv;

    rv = NS_OK;

    switch (aID) {
        case eIntID_CaretBlinkTime:
            aResult = 500;
            break;

        case eIntID_CaretWidth:
            aResult = 1;
            break;

        case eIntID_ShowCaretDuringSelection:
            aResult = 0;
            break;

        case eIntID_SelectTextfieldsOnKeyFocus:
            // Select textfield content when focused by kbd
            // used by EventStateManager::sTextfieldSelectModel
            aResult = 1;
            break;

        case eIntID_SubmenuDelay:
            aResult = 200;
            break;

        case eIntID_TooltipDelay:
            aResult = 500;
            break;

        case eIntID_MenusCanOverlapOSBar:
            // we want XUL popups to be able to overlap the task bar.
            aResult = 1;
            break;

        case eIntID_ScrollArrowStyle:
            aResult = eScrollArrowStyle_Single;
            break;

        case eIntID_ScrollSliderStyle:
            aResult = eScrollThumbStyle_Proportional;
            break;

        case eIntID_TouchEnabled:
            aResult = 1;
            break;

        case eIntID_WindowsDefaultTheme:
        case eIntID_WindowsThemeIdentifier:
        case eIntID_OperatingSystemVersionIdentifier:
            aResult = 0;
            rv = NS_ERROR_NOT_IMPLEMENTED;
            break;

        case eIntID_SpellCheckerUnderlineStyle:
            aResult = NS_STYLE_TEXT_DECORATION_STYLE_WAVY;
            break;

        case eIntID_ScrollbarButtonAutoRepeatBehavior:
            aResult = 0;
            break;

        case eIntID_ContextMenuOffsetVertical:
        case eIntID_ContextMenuOffsetHorizontal:
            aResult = 2;
            break;

        case eIntID_PrefersReducedMotion:
            if (mIsInPrefersReducedMotionForTest) {
              aResult = mPrefersReducedMotionForTest ? 1 : 0;
              break;
            }
            aResult =
              java::GeckoSystemStateListener::PrefersReducedMotion() ? 1 : 0;
            break;

        case eIntID_PrimaryPointerCapabilities:
            aResult = java::GeckoAppShell::GetPrimaryPointerCapabilities();
            break;
        case eIntID_AllPointerCapabilities:
            aResult = java::GeckoAppShell::GetAllPointerCapabilities();
            break;

        default:
            aResult = 0;
            rv = NS_ERROR_FAILURE;
    }
=======
nsresult nsLookAndFeel::GetFloatImpl(FloatID aID, float& aResult) {
  nsresult rv = nsXPLookAndFeel::GetFloatImpl(aID, aResult);
  if (NS_SUCCEEDED(rv)) return rv;
  rv = NS_OK;
>>>>>>> upstream-releases

<<<<<<< HEAD
    case eIntID_SubmenuDelay:
      aResult = 200;
      break;

    case eIntID_TooltipDelay:
      aResult = 500;
      break;

    case eIntID_MenusCanOverlapOSBar:
      // we want XUL popups to be able to overlap the task bar.
      aResult = 1;
      break;

    case eIntID_ScrollArrowStyle:
      aResult = eScrollArrowStyle_Single;
      break;

    case eIntID_ScrollSliderStyle:
      aResult = eScrollThumbStyle_Proportional;
      break;

    case eIntID_TouchEnabled:
      aResult = 1;
      break;

    case eIntID_WindowsDefaultTheme:
    case eIntID_WindowsThemeIdentifier:
    case eIntID_OperatingSystemVersionIdentifier:
      aResult = 0;
      rv = NS_ERROR_NOT_IMPLEMENTED;
      break;

    case eIntID_SpellCheckerUnderlineStyle:
      aResult = NS_STYLE_TEXT_DECORATION_STYLE_WAVY;
      break;

    case eIntID_ScrollbarButtonAutoRepeatBehavior:
      aResult = 0;
      break;

    case eIntID_ContextMenuOffsetVertical:
    case eIntID_ContextMenuOffsetHorizontal:
      aResult = 2;
      break;

    case eIntID_PrefersReducedMotion:
      if (mIsInPrefersReducedMotionForTest) {
        aResult = mPrefersReducedMotionForTest ? 1 : 0;
        break;
      }
      aResult = java::GeckoSystemStateListener::PrefersReducedMotion() ? 1 : 0;
      break;

    case eIntID_PrimaryPointerCapabilities:
      aResult = java::GeckoAppShell::GetPrimaryPointerCapabilities();
      break;
    case eIntID_AllPointerCapabilities:
      aResult = java::GeckoAppShell::GetAllPointerCapabilities();
      break;

    default:
      aResult = 0;
      rv = NS_ERROR_FAILURE;
  }

  return rv;
}
||||||| merged common ancestors
    return rv;
}
=======
  switch (aID) {
    case eFloatID_IMEUnderlineRelativeSize:
      aResult = 1.0f;
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsLookAndFeel::GetFloatImpl(FloatID aID, float& aResult) {
  nsresult rv = nsXPLookAndFeel::GetFloatImpl(aID, aResult);
  if (NS_SUCCEEDED(rv)) return rv;
  rv = NS_OK;

  switch (aID) {
    case eFloatID_IMEUnderlineRelativeSize:
      aResult = 1.0f;
      break;

    case eFloatID_SpellCheckerUnderlineRelativeSize:
      aResult = 1.0f;
      break;

    default:
      aResult = -1.0;
      rv = NS_ERROR_FAILURE;
      break;
  }
  return rv;
||||||| merged common ancestors
nsresult
nsLookAndFeel::GetFloatImpl(FloatID aID, float &aResult)
{
    nsresult rv = nsXPLookAndFeel::GetFloatImpl(aID, aResult);
    if (NS_SUCCEEDED(rv))
        return rv;
    rv = NS_OK;

    switch (aID) {
        case eFloatID_IMEUnderlineRelativeSize:
            aResult = 1.0f;
            break;

        case eFloatID_SpellCheckerUnderlineRelativeSize:
            aResult = 1.0f;
            break;

        default:
            aResult = -1.0;
            rv = NS_ERROR_FAILURE;
            break;
    }
    return rv;
=======
    case eFloatID_SpellCheckerUnderlineRelativeSize:
      aResult = 1.0f;
      break;

    default:
      aResult = -1.0;
      rv = NS_ERROR_FAILURE;
      break;
  }
  return rv;
>>>>>>> upstream-releases
}

/*virtual*/
<<<<<<< HEAD
bool nsLookAndFeel::GetFontImpl(FontID aID, nsString& aFontName,
                                gfxFontStyle& aFontStyle,
                                float aDevPixPerCSSPixel) {
  aFontName.AssignLiteral("\"Roboto\"");
  aFontStyle.style = FontSlantStyle::Normal();
  aFontStyle.weight = FontWeight::Normal();
  aFontStyle.stretch = FontStretch::Normal();
  aFontStyle.size = 9.0 * 96.0f / 72.0f * aDevPixPerCSSPixel;
  aFontStyle.systemFont = true;
  return true;
||||||| merged common ancestors
bool
nsLookAndFeel::GetFontImpl(FontID aID, nsString& aFontName,
                           gfxFontStyle& aFontStyle,
                           float aDevPixPerCSSPixel)
{
    aFontName.AssignLiteral("\"Roboto\"");
    aFontStyle.style = FontSlantStyle::Normal();
    aFontStyle.weight = FontWeight::Normal();
    aFontStyle.stretch = FontStretch::Normal();
    aFontStyle.size = 9.0 * 96.0f / 72.0f * aDevPixPerCSSPixel;
    aFontStyle.systemFont = true;
    return true;
=======
bool nsLookAndFeel::GetFontImpl(FontID aID, nsString& aFontName,
                                gfxFontStyle& aFontStyle) {
  aFontName.AssignLiteral("\"Roboto\"");
  aFontStyle.style = FontSlantStyle::Normal();
  aFontStyle.weight = FontWeight::Normal();
  aFontStyle.stretch = FontStretch::Normal();
  aFontStyle.size = 9.0 * 96.0f / 72.0f;
  aFontStyle.systemFont = true;
  return true;
>>>>>>> upstream-releases
}

/*virtual*/
bool nsLookAndFeel::GetEchoPasswordImpl() {
  EnsureInitShowPassword();
  return mShowPassword;
}

uint32_t nsLookAndFeel::GetPasswordMaskDelayImpl() {
  // This value is hard-coded in Android OS's PasswordTransformationMethod.java
  return 1500;
}

/* virtual */
char16_t nsLookAndFeel::GetPasswordCharacterImpl() {
  // This value is hard-coded in Android OS's PasswordTransformationMethod.java
  return UNICODE_BULLET;
}

void nsLookAndFeel::EnsureInitSystemColors() {
  if (!mInitializedSystemColors) {
    if (XRE_IsParentProcess()) {
      nsresult rv = GetSystemColors();
      mInitializedSystemColors = NS_SUCCEEDED(rv);
    }
    // Child process will set system color cache from ContentParent.
  }
}

void nsLookAndFeel::EnsureInitShowPassword() {
  if (!mInitializedShowPassword) {
    if (XRE_IsParentProcess()) {
      mShowPassword =
          jni::IsAvailable() && java::GeckoAppShell::GetShowPasswordSetting();
    } else {
      ContentChild::GetSingleton()->SendGetShowPasswordSetting(&mShowPassword);
    }
    mInitializedShowPassword = true;
  }
}

nsTArray<LookAndFeelInt> nsLookAndFeel::GetIntCacheImpl() {
  MOZ_ASSERT(XRE_IsParentProcess());
  EnsureInitSystemColors();
  MOZ_ASSERT(mInitializedSystemColors);

  nsTArray<LookAndFeelInt> lookAndFeelCache =
      nsXPLookAndFeel::GetIntCacheImpl();
  lookAndFeelCache.SetCapacity(sizeof(AndroidSystemColors) / sizeof(nscolor));

<<<<<<< HEAD
  LookAndFeelInt laf;
  laf.id = eColorID_WindowForeground;
  laf.colorValue = mSystemColors.textColorPrimary;
  lookAndFeelCache.AppendElement(laf);
||||||| merged common ancestors
    LookAndFeelInt laf;
    laf.id = eColorID_WindowForeground;
    laf.colorValue = mSystemColors.textColorPrimary;
    lookAndFeelCache.AppendElement(laf);
=======
  LookAndFeelInt laf;
  laf.id = int32_t(ColorID::WindowForeground);
  laf.colorValue = mSystemColors.textColorPrimary;
  lookAndFeelCache.AppendElement(laf);
>>>>>>> upstream-releases

<<<<<<< HEAD
  laf.id = eColorID_WidgetBackground;
  laf.colorValue = mSystemColors.colorBackground;
  lookAndFeelCache.AppendElement(laf);
||||||| merged common ancestors
    laf.id = eColorID_WidgetBackground;
    laf.colorValue = mSystemColors.colorBackground;
    lookAndFeelCache.AppendElement(laf);
=======
  laf.id = int32_t(ColorID::WidgetBackground);
  laf.colorValue = mSystemColors.colorBackground;
  lookAndFeelCache.AppendElement(laf);
>>>>>>> upstream-releases

<<<<<<< HEAD
  laf.id = eColorID_WidgetForeground;
  laf.colorValue = mSystemColors.colorForeground;
  lookAndFeelCache.AppendElement(laf);
||||||| merged common ancestors
    laf.id = eColorID_WidgetForeground;
    laf.colorValue = mSystemColors.colorForeground;
    lookAndFeelCache.AppendElement(laf);
=======
  laf.id = int32_t(ColorID::WidgetForeground);
  laf.colorValue = mSystemColors.colorForeground;
  lookAndFeelCache.AppendElement(laf);
>>>>>>> upstream-releases

<<<<<<< HEAD
  laf.id = eColorID_WidgetSelectBackground;
  laf.colorValue = mSystemColors.textColorHighlight;
  lookAndFeelCache.AppendElement(laf);
||||||| merged common ancestors
    laf.id = eColorID_WidgetSelectBackground;
    laf.colorValue = mSystemColors.textColorHighlight;
    lookAndFeelCache.AppendElement(laf);
=======
  laf.id = int32_t(ColorID::WidgetSelectBackground);
  laf.colorValue = mSystemColors.textColorHighlight;
  lookAndFeelCache.AppendElement(laf);
>>>>>>> upstream-releases

<<<<<<< HEAD
  laf.id = eColorID_WidgetSelectForeground;
  laf.colorValue = mSystemColors.textColorPrimaryInverse;
  lookAndFeelCache.AppendElement(laf);
||||||| merged common ancestors
    laf.id = eColorID_WidgetSelectForeground;
    laf.colorValue = mSystemColors.textColorPrimaryInverse;
    lookAndFeelCache.AppendElement(laf);
=======
  laf.id = int32_t(ColorID::WidgetSelectForeground);
  laf.colorValue = mSystemColors.textColorPrimaryInverse;
  lookAndFeelCache.AppendElement(laf);
>>>>>>> upstream-releases

<<<<<<< HEAD
  laf.id = eColorID_inactivecaptiontext;
  laf.colorValue = mSystemColors.textColorTertiary;
  lookAndFeelCache.AppendElement(laf);
||||||| merged common ancestors
    laf.id = eColorID_inactivecaptiontext;
    laf.colorValue = mSystemColors.textColorTertiary;
    lookAndFeelCache.AppendElement(laf);
=======
  laf.id = int32_t(ColorID::Inactivecaptiontext);
  laf.colorValue = mSystemColors.textColorTertiary;
  lookAndFeelCache.AppendElement(laf);
>>>>>>> upstream-releases

<<<<<<< HEAD
  laf.id = eColorID_windowtext;
  laf.colorValue = mSystemColors.textColorPrimary;
  lookAndFeelCache.AppendElement(laf);
||||||| merged common ancestors
    laf.id = eColorID_windowtext;
    laf.colorValue = mSystemColors.textColorPrimary;
    lookAndFeelCache.AppendElement(laf);
=======
  laf.id = int32_t(ColorID::Windowtext);
  laf.colorValue = mSystemColors.textColorPrimary;
  lookAndFeelCache.AppendElement(laf);
>>>>>>> upstream-releases

  // XXX The following colors are unused.
  // - textColorTertiaryInverse
  // - panelColorForeground
  // - panelColorBackground

  return lookAndFeelCache;
}

<<<<<<< HEAD
void nsLookAndFeel::SetIntCacheImpl(
    const nsTArray<LookAndFeelInt>& aLookAndFeelCache) {
  for (auto entry : aLookAndFeelCache) {
    switch (entry.id) {
      case eColorID_WindowForeground:
        mSystemColors.textColorPrimary = entry.colorValue;
        break;
      case eColorID_WidgetBackground:
        mSystemColors.colorBackground = entry.colorValue;
        break;
      case eColorID_WidgetForeground:
        mSystemColors.colorForeground = entry.colorValue;
        break;
      case eColorID_WidgetSelectBackground:
        mSystemColors.textColorHighlight = entry.colorValue;
        break;
      case eColorID_WidgetSelectForeground:
        mSystemColors.textColorPrimaryInverse = entry.colorValue;
        break;
      case eColorID_inactivecaptiontext:
        mSystemColors.textColorTertiary = entry.colorValue;
        break;
      case eColorID_windowtext:
        mSystemColors.textColorPrimary = entry.colorValue;
        break;
      default:
        MOZ_ASSERT(false);
||||||| merged common ancestors
void
nsLookAndFeel::SetIntCacheImpl(const nsTArray<LookAndFeelInt>& aLookAndFeelCache)
{
    for (auto entry : aLookAndFeelCache) {
        switch (entry.id) {
        case eColorID_WindowForeground:
            mSystemColors.textColorPrimary = entry.colorValue;
            break;
        case eColorID_WidgetBackground:
            mSystemColors.colorBackground = entry.colorValue;
            break;
        case eColorID_WidgetForeground:
            mSystemColors.colorForeground = entry.colorValue;
            break;
        case eColorID_WidgetSelectBackground:
            mSystemColors.textColorHighlight = entry.colorValue;
            break;
        case eColorID_WidgetSelectForeground:
            mSystemColors.textColorPrimaryInverse = entry.colorValue;
            break;
        case eColorID_inactivecaptiontext:
            mSystemColors.textColorTertiary = entry.colorValue;
            break;
        case eColorID_windowtext:
            mSystemColors.textColorPrimary = entry.colorValue;
            break;
        default:
            MOZ_ASSERT(false);
        }
=======
void nsLookAndFeel::SetIntCacheImpl(
    const nsTArray<LookAndFeelInt>& aLookAndFeelCache) {
  for (auto entry : aLookAndFeelCache) {
    switch (ColorID(entry.id)) {
      case ColorID::WindowForeground:
        mSystemColors.textColorPrimary = entry.colorValue;
        break;
      case ColorID::WidgetBackground:
        mSystemColors.colorBackground = entry.colorValue;
        break;
      case ColorID::WidgetForeground:
        mSystemColors.colorForeground = entry.colorValue;
        break;
      case ColorID::WidgetSelectBackground:
        mSystemColors.textColorHighlight = entry.colorValue;
        break;
      case ColorID::WidgetSelectForeground:
        mSystemColors.textColorPrimaryInverse = entry.colorValue;
        break;
      case ColorID::Inactivecaptiontext:
        mSystemColors.textColorTertiary = entry.colorValue;
        break;
      case ColorID::Windowtext:
        mSystemColors.textColorPrimary = entry.colorValue;
        break;
      default:
        MOZ_ASSERT(false);
>>>>>>> upstream-releases
    }
  }
  mInitializedSystemColors = true;
}
