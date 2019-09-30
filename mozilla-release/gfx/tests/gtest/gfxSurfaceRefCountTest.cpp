/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdio.h>

#include "gtest/gtest.h"

#include "gfxASurface.h"
#include "gfxImageSurface.h"

#include "cairo.h"

<<<<<<< HEAD
int GetASurfaceRefCount(gfxASurface *s) {
  NS_ADDREF(s);
  return s->Release();
||||||| merged common ancestors
int
GetASurfaceRefCount(gfxASurface *s) {
    NS_ADDREF(s);
    return s->Release();
=======
static int GetASurfaceRefCount(gfxASurface* s) {
  NS_ADDREF(s);
  return s->Release();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
int CheckInt(int value, int expected) {
  if (value != expected) {
    fprintf(stderr, "Expected %d got %d\n", expected, value);
    return 1;
  }
||||||| merged common ancestors
int
CheckInt (int value, int expected) {
    if (value != expected) {
        fprintf (stderr, "Expected %d got %d\n", expected, value);
        return 1;
    }
=======
static int CheckInt(int value, int expected) {
  if (value != expected) {
    fprintf(stderr, "Expected %d got %d\n", expected, value);
    return 1;
  }
>>>>>>> upstream-releases

  return 0;
}

<<<<<<< HEAD
int CheckPointer(void *value, void *expected) {
  if (value != expected) {
    fprintf(stderr, "Expected %p got %p\n", expected, value);
    return 1;
  }
||||||| merged common ancestors
int
CheckPointer (void *value, void *expected) {
    if (value != expected) {
        fprintf (stderr, "Expected %p got %p\n", expected, value);
        return 1;
    }
=======
static int CheckPointer(void* value, void* expected) {
  if (value != expected) {
    fprintf(stderr, "Expected %p got %p\n", expected, value);
    return 1;
  }
>>>>>>> upstream-releases

  return 0;
}

static cairo_user_data_key_t destruction_key;
<<<<<<< HEAD
void SurfaceDestroyNotifier(void *data) { *(int *)data = 1; }
||||||| merged common ancestors
void
SurfaceDestroyNotifier (void *data) {
    *(int *)data = 1;
}
=======
static void SurfaceDestroyNotifier(void* data) { *(int*)data = 1; }
>>>>>>> upstream-releases

<<<<<<< HEAD
int TestNewSurface() {
  int failures = 0;
  int destroyed = 0;
||||||| merged common ancestors
int
TestNewSurface () {
    int failures = 0;
    int destroyed = 0;
=======
static int TestNewSurface() {
  int failures = 0;
  int destroyed = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
  RefPtr<gfxASurface> s = new gfxImageSurface(mozilla::gfx::IntSize(10, 10),
                                              SurfaceFormat::A8R8G8B8_UINT32);
  cairo_surface_t *cs = s->CairoSurface();
||||||| merged common ancestors
    RefPtr<gfxASurface> s = new gfxImageSurface (mozilla::gfx::IntSize(10, 10), SurfaceFormat::A8R8G8B8_UINT32);
    cairo_surface_t *cs = s->CairoSurface();
=======
  RefPtr<gfxASurface> s = new gfxImageSurface(mozilla::gfx::IntSize(10, 10),
                                              SurfaceFormat::A8R8G8B8_UINT32);
  cairo_surface_t* cs = s->CairoSurface();
>>>>>>> upstream-releases

  cairo_surface_set_user_data(cs, &destruction_key, &destroyed,
                              SurfaceDestroyNotifier);

  failures += CheckInt(GetASurfaceRefCount(s.get()), 1);
  failures += CheckInt(cairo_surface_get_reference_count(cs), 1);
  failures += CheckInt(destroyed, 0);

  cairo_surface_reference(cs);

  failures += CheckInt(GetASurfaceRefCount(s.get()), 2);
  failures += CheckInt(cairo_surface_get_reference_count(cs), 2);
  failures += CheckInt(destroyed, 0);

<<<<<<< HEAD
  gfxASurface *savedWrapper = s.get();
||||||| merged common ancestors
    gfxASurface *savedWrapper = s.get();
=======
  gfxASurface* savedWrapper = s.get();
>>>>>>> upstream-releases

  s = nullptr;

  failures += CheckInt(cairo_surface_get_reference_count(cs), 1);
  failures += CheckInt(destroyed, 0);

  s = gfxASurface::Wrap(cs);

  failures += CheckPointer(s.get(), savedWrapper);
  failures += CheckInt(GetASurfaceRefCount(s.get()), 2);
  failures += CheckInt(cairo_surface_get_reference_count(cs), 2);
  failures += CheckInt(destroyed, 0);

  cairo_surface_destroy(cs);

  failures += CheckInt(GetASurfaceRefCount(s.get()), 1);
  failures += CheckInt(cairo_surface_get_reference_count(cs), 1);
  failures += CheckInt(destroyed, 0);

  s = nullptr;

  failures += CheckInt(destroyed, 1);

  return failures;
}

<<<<<<< HEAD
int TestExistingSurface() {
  int failures = 0;
  int destroyed = 0;
||||||| merged common ancestors
int
TestExistingSurface () {
    int failures = 0;
    int destroyed = 0;
=======
static int TestExistingSurface() {
  int failures = 0;
  int destroyed = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
  cairo_surface_t *cs = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 10, 10);
||||||| merged common ancestors
    cairo_surface_t *cs = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 10, 10);
=======
  cairo_surface_t* cs = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 10, 10);
>>>>>>> upstream-releases

  cairo_surface_set_user_data(cs, &destruction_key, &destroyed,
                              SurfaceDestroyNotifier);

  failures += CheckInt(cairo_surface_get_reference_count(cs), 1);
  failures += CheckInt(destroyed, 0);

  RefPtr<gfxASurface> s = gfxASurface::Wrap(cs);

  failures += CheckInt(GetASurfaceRefCount(s.get()), 2);

  cairo_surface_reference(cs);

  failures += CheckInt(GetASurfaceRefCount(s.get()), 3);
  failures += CheckInt(cairo_surface_get_reference_count(cs), 3);
  failures += CheckInt(destroyed, 0);

<<<<<<< HEAD
  gfxASurface *savedWrapper = s.get();
||||||| merged common ancestors
    gfxASurface *savedWrapper = s.get();
=======
  gfxASurface* savedWrapper = s.get();
>>>>>>> upstream-releases

  s = nullptr;

  failures += CheckInt(cairo_surface_get_reference_count(cs), 2);
  failures += CheckInt(destroyed, 0);

  s = gfxASurface::Wrap(cs);

  failures += CheckPointer(s.get(), savedWrapper);
  failures += CheckInt(GetASurfaceRefCount(s.get()), 3);
  failures += CheckInt(cairo_surface_get_reference_count(cs), 3);
  failures += CheckInt(destroyed, 0);

  cairo_surface_destroy(cs);

  failures += CheckInt(GetASurfaceRefCount(s.get()), 2);
  failures += CheckInt(cairo_surface_get_reference_count(cs), 2);
  failures += CheckInt(destroyed, 0);

  s = nullptr;

  failures += CheckInt(cairo_surface_get_reference_count(cs), 1);
  failures += CheckInt(destroyed, 0);

  cairo_surface_destroy(cs);

  failures += CheckInt(destroyed, 1);

  return failures;
}

<<<<<<< HEAD
TEST(Gfx, SurfaceRefCount) {
  int fail;
||||||| merged common ancestors
TEST(Gfx, SurfaceRefCount) {
    int fail;
=======
TEST(Gfx, SurfaceRefCount)
{
  int fail;
>>>>>>> upstream-releases

  fail = TestNewSurface();
  EXPECT_TRUE(fail == 0) << "TestNewSurface: " << fail << " failures";
  fail = TestExistingSurface();
  EXPECT_TRUE(fail == 0) << "TestExistingSurface: " << fail << " failures";
}
