/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* base class for ruby rendering objects that directly contain content */

#include "nsRubyContentFrame.h"

#include "mozilla/ComputedStyle.h"
#include "nsPresContext.h"
#include "nsCSSAnonBoxes.h"

using namespace mozilla;

//----------------------------------------------------------------------

// nsRubyContentFrame Method Implementations
// ======================================

<<<<<<< HEAD
/* virtual */ bool nsRubyContentFrame::IsFrameOfType(uint32_t aFlags) const {
||||||| merged common ancestors
/* virtual */ bool
nsRubyContentFrame::IsFrameOfType(uint32_t aFlags) const
{
=======
/* virtual */
bool nsRubyContentFrame::IsFrameOfType(uint32_t aFlags) const {
>>>>>>> upstream-releases
  if (aFlags & eBidiInlineContainer) {
    return false;
  }
  return nsInlineFrame::IsFrameOfType(aFlags);
}

<<<<<<< HEAD
bool nsRubyContentFrame::IsIntraLevelWhitespace() const {
  nsAtom* pseudoType = Style()->GetPseudo();
  if (pseudoType != nsCSSAnonBoxes::rubyBase() &&
      pseudoType != nsCSSAnonBoxes::rubyText()) {
||||||| merged common ancestors
bool
nsRubyContentFrame::IsIntraLevelWhitespace() const
{
  nsAtom* pseudoType = Style()->GetPseudo();
  if (pseudoType != nsCSSAnonBoxes::rubyBase() &&
      pseudoType != nsCSSAnonBoxes::rubyText()) {
=======
bool nsRubyContentFrame::IsIntraLevelWhitespace() const {
  auto pseudoType = Style()->GetPseudoType();
  if (pseudoType != PseudoStyleType::rubyBase &&
      pseudoType != PseudoStyleType::rubyText) {
>>>>>>> upstream-releases
    return false;
  }

  nsIFrame* child = mFrames.OnlyChild();
  return child && child->GetContent()->TextIsOnlyWhitespace();
}
