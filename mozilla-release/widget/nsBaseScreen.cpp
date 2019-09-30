/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: sw=2 ts=8 et :
 */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define MOZ_FATAL_ASSERTIONS_FOR_THREAD_SAFETY

#include "nsBaseScreen.h"

NS_IMPL_ISUPPORTS(nsBaseScreen, nsIScreen)

nsBaseScreen::nsBaseScreen() {}

nsBaseScreen::~nsBaseScreen() {}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseScreen::GetRectDisplayPix(int32_t *outLeft, int32_t *outTop,
                                int32_t *outWidth, int32_t *outHeight) {
||||||| merged common ancestors
nsBaseScreen::GetRectDisplayPix(int32_t *outLeft,  int32_t *outTop,
                                int32_t *outWidth, int32_t *outHeight)
{
=======
nsBaseScreen::GetRectDisplayPix(int32_t* outLeft, int32_t* outTop,
                                int32_t* outWidth, int32_t* outHeight) {
>>>>>>> upstream-releases
  return GetRect(outLeft, outTop, outWidth, outHeight);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseScreen::GetAvailRectDisplayPix(int32_t *outLeft, int32_t *outTop,
                                     int32_t *outWidth, int32_t *outHeight) {
||||||| merged common ancestors
nsBaseScreen::GetAvailRectDisplayPix(int32_t *outLeft,  int32_t *outTop,
                                     int32_t *outWidth, int32_t *outHeight)
{
=======
nsBaseScreen::GetAvailRectDisplayPix(int32_t* outLeft, int32_t* outTop,
                                     int32_t* outWidth, int32_t* outHeight) {
>>>>>>> upstream-releases
  return GetAvailRect(outLeft, outTop, outWidth, outHeight);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseScreen::GetContentsScaleFactor(double *aContentsScaleFactor) {
||||||| merged common ancestors
nsBaseScreen::GetContentsScaleFactor(double* aContentsScaleFactor)
{
=======
nsBaseScreen::GetContentsScaleFactor(double* aContentsScaleFactor) {
>>>>>>> upstream-releases
  *aContentsScaleFactor = 1.0;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseScreen::GetDefaultCSSScaleFactor(double *aScaleFactor) {
||||||| merged common ancestors
nsBaseScreen::GetDefaultCSSScaleFactor(double* aScaleFactor)
{
=======
nsBaseScreen::GetDefaultCSSScaleFactor(double* aScaleFactor) {
>>>>>>> upstream-releases
  *aScaleFactor = 1.0;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseScreen::GetDpi(float *aDPI) {
||||||| merged common ancestors
nsBaseScreen::GetDpi(float* aDPI)
{
=======
nsBaseScreen::GetDpi(float* aDPI) {
>>>>>>> upstream-releases
  *aDPI = 96;
  return NS_OK;
}
