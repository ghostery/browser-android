/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_a11y_DocAccessibleWrap_h__
#define mozilla_a11y_DocAccessibleWrap_h__

#include "DocAccessible.h"

namespace mozilla {

class PresShell;

namespace a11y {

<<<<<<< HEAD
class DocAccessibleWrap : public DocAccessible {
 public:
  DocAccessibleWrap(nsIDocument* aDocument, nsIPresShell* aPresShell);
||||||| merged common ancestors
class DocAccessibleWrap : public DocAccessible
{
public:
  DocAccessibleWrap(nsIDocument* aDocument, nsIPresShell* aPresShell);
=======
class DocAccessibleWrap : public DocAccessible {
 public:
  DocAccessibleWrap(dom::Document* aDocument, PresShell* aPresShell);
>>>>>>> upstream-releases
  virtual ~DocAccessibleWrap();
};

}  // namespace a11y
}  // namespace mozilla

#endif
