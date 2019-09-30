/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsHtml5PlainTextUtils.h"
#include "mozilla/StaticPrefs.h"
#include "nsHtml5AttributeName.h"
#include "nsHtml5Portability.h"
#include "nsHtml5String.h"
#include "nsGkAtoms.h"

// static
nsHtml5HtmlAttributes* nsHtml5PlainTextUtils::NewLinkAttributes() {
  nsHtml5HtmlAttributes* linkAttrs = new nsHtml5HtmlAttributes(0);
<<<<<<< HEAD
  nsHtml5String rel =
      nsHtml5Portability::newStringFromLiteral("alternate stylesheet");
||||||| merged common ancestors
  nsHtml5String rel =
    nsHtml5Portability::newStringFromLiteral("alternate stylesheet");
=======
  nsHtml5String rel = nsHtml5Portability::newStringFromLiteral("stylesheet");
>>>>>>> upstream-releases
  linkAttrs->addAttribute(nsHtml5AttributeName::ATTR_REL, rel, -1);
  nsHtml5String href = nsHtml5Portability::newStringFromLiteral(
      "resource://content-accessible/plaintext.css");
  linkAttrs->addAttribute(nsHtml5AttributeName::ATTR_HREF, href, -1);
  return linkAttrs;
}

<<<<<<< HEAD
  nsresult rv;
  nsCOMPtr<nsIStringBundleService> bundleService =
      do_GetService(NS_STRINGBUNDLE_CONTRACTID, &rv);
  NS_ASSERTION(NS_SUCCEEDED(rv) && bundleService,
               "The bundle service could not be loaded");
  nsCOMPtr<nsIStringBundle> bundle;
  rv = bundleService->CreateBundle("chrome://global/locale/browser.properties",
                                   getter_AddRefs(bundle));
  NS_ASSERTION(NS_SUCCEEDED(rv) && bundle,
               "chrome://global/locale/browser.properties could not be loaded");
  nsAutoString title;
  if (bundle) {
    bundle->GetStringFromName("plainText.wordWrap", title);
||||||| merged common ancestors
  nsresult rv;
  nsCOMPtr<nsIStringBundleService> bundleService =
    do_GetService(NS_STRINGBUNDLE_CONTRACTID, &rv);
  NS_ASSERTION(NS_SUCCEEDED(rv) && bundleService,
               "The bundle service could not be loaded");
  nsCOMPtr<nsIStringBundle> bundle;
  rv = bundleService->CreateBundle("chrome://global/locale/browser.properties",
                                   getter_AddRefs(bundle));
  NS_ASSERTION(NS_SUCCEEDED(rv) && bundle,
               "chrome://global/locale/browser.properties could not be loaded");
  nsAutoString title;
  if (bundle) {
    bundle->GetStringFromName("plainText.wordWrap", title);
=======
// static
nsHtml5HtmlAttributes* nsHtml5PlainTextUtils::NewBodyAttributes() {
  if (mozilla::StaticPrefs::plain_text_wrap_long_lines()) {
    return nsHtml5HtmlAttributes::EMPTY_ATTRIBUTES;
>>>>>>> upstream-releases
  }
<<<<<<< HEAD

  linkAttrs->addAttribute(nsHtml5AttributeName::ATTR_TITLE,
                          nsHtml5String::FromString(title), -1);
  return linkAttrs;
||||||| merged common ancestors

  linkAttrs->addAttribute(
    nsHtml5AttributeName::ATTR_TITLE, nsHtml5String::FromString(title), -1);
  return linkAttrs;
=======
  nsHtml5HtmlAttributes* bodyAttrs = new nsHtml5HtmlAttributes(0);
  RefPtr<nsAtom> nowrap = nsGkAtoms::nowrap;
  bodyAttrs->addAttribute(nsHtml5AttributeName::ATTR_CLASS,
                          nsHtml5String::FromAtom(nowrap.forget()), -1);
  return bodyAttrs;
>>>>>>> upstream-releases
}
