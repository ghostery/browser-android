/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/basictypes.h"
#include "mozilla/ArrayUtils.h"

#include "nsAboutProtocolHandler.h"
#include "nsIURI.h"
#include "nsIAboutModule.h"
#include "nsString.h"
#include "nsNetCID.h"
#include "nsAboutProtocolUtils.h"
#include "nsError.h"
#include "nsNetUtil.h"
#include "nsIObjectInputStream.h"
#include "nsIObjectOutputStream.h"
#include "nsAutoPtr.h"
#include "nsIWritablePropertyBag2.h"
#include "nsIChannel.h"
#include "nsIScriptError.h"
#include "nsIEnterprisePolicies.h"

namespace mozilla {
namespace net {

static NS_DEFINE_CID(kSimpleURICID, NS_SIMPLEURI_CID);
static NS_DEFINE_CID(kNestedAboutURICID, NS_NESTEDABOUTURI_CID);

static bool IsSafeForUntrustedContent(nsIAboutModule* aModule, nsIURI* aURI) {
  uint32_t flags;
  nsresult rv = aModule->GetURIFlags(aURI, &flags);
  NS_ENSURE_SUCCESS(rv, false);

  return (flags & nsIAboutModule::URI_SAFE_FOR_UNTRUSTED_CONTENT) != 0;
}

<<<<<<< HEAD
static bool IsSafeToLinkForUntrustedContent(nsIAboutModule *aModule,
                                            nsIURI *aURI) {
  uint32_t flags;
  nsresult rv = aModule->GetURIFlags(aURI, &flags);
  NS_ENSURE_SUCCESS(rv, false);
||||||| merged common ancestors
static bool IsSafeToLinkForUntrustedContent(nsIAboutModule *aModule, nsIURI *aURI) {
  uint32_t flags;
  nsresult rv = aModule->GetURIFlags(aURI, &flags);
  NS_ENSURE_SUCCESS(rv, false);
=======
static bool IsSafeToLinkForUntrustedContent(nsIURI* aURI) {
  nsAutoCString path;
  aURI->GetPathQueryRef(path);

  int32_t f = path.FindChar('#');
  if (f >= 0) {
    path.SetLength(f);
  }

  f = path.FindChar('?');
  if (f >= 0) {
    path.SetLength(f);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return (flags & nsIAboutModule::URI_SAFE_FOR_UNTRUSTED_CONTENT) &&
         (flags & nsIAboutModule::MAKE_LINKABLE);
||||||| merged common ancestors
  return (flags & nsIAboutModule::URI_SAFE_FOR_UNTRUSTED_CONTENT) && (flags & nsIAboutModule::MAKE_LINKABLE);
=======
  ToLowerCase(path);

  // The about modules for these URL types have the
  // URI_SAFE_FOR_UNTRUSTED_CONTENT and MAKE_LINKABLE flags set.
  if (path.EqualsLiteral("blank") || path.EqualsLiteral("logo") ||
      path.EqualsLiteral("srcdoc")) {
    return true;
  }

  return false;
>>>>>>> upstream-releases
}

////////////////////////////////////////////////////////////////////////////////

NS_IMPL_ISUPPORTS(nsAboutProtocolHandler, nsIProtocolHandler,
                  nsIProtocolHandlerWithDynamicFlags, nsISupportsWeakReference)

////////////////////////////////////////////////////////////////////////////////
// nsIProtocolHandler methods:

NS_IMETHODIMP
<<<<<<< HEAD
nsAboutProtocolHandler::GetScheme(nsACString &result) {
  result.AssignLiteral("about");
  return NS_OK;
||||||| merged common ancestors
nsAboutProtocolHandler::GetScheme(nsACString &result)
{
    result.AssignLiteral("about");
    return NS_OK;
=======
nsAboutProtocolHandler::GetScheme(nsACString& result) {
  result.AssignLiteral("about");
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsAboutProtocolHandler::GetDefaultPort(int32_t *result) {
  *result = -1;  // no port for about: URLs
  return NS_OK;
||||||| merged common ancestors
nsAboutProtocolHandler::GetDefaultPort(int32_t *result)
{
    *result = -1;        // no port for about: URLs
    return NS_OK;
=======
nsAboutProtocolHandler::GetDefaultPort(int32_t* result) {
  *result = -1;  // no port for about: URLs
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsAboutProtocolHandler::GetProtocolFlags(uint32_t *result) {
  *result = URI_NORELATIVE | URI_NOAUTH | URI_DANGEROUS_TO_LOAD |
            URI_SCHEME_NOT_SELF_LINKABLE;
  return NS_OK;
||||||| merged common ancestors
nsAboutProtocolHandler::GetProtocolFlags(uint32_t *result)
{
    *result = URI_NORELATIVE | URI_NOAUTH | URI_DANGEROUS_TO_LOAD | URI_SCHEME_NOT_SELF_LINKABLE;
    return NS_OK;
=======
nsAboutProtocolHandler::GetProtocolFlags(uint32_t* result) {
  *result = URI_NORELATIVE | URI_NOAUTH | URI_DANGEROUS_TO_LOAD |
            URI_SCHEME_NOT_SELF_LINKABLE;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsAboutProtocolHandler::GetFlagsForURI(nsIURI *aURI, uint32_t *aFlags) {
  // First use the default (which is "unsafe for content"):
  GetProtocolFlags(aFlags);

  // Now try to see if this URI overrides the default:
  nsCOMPtr<nsIAboutModule> aboutMod;
  nsresult rv = NS_GetAboutModule(aURI, getter_AddRefs(aboutMod));
  if (NS_FAILED(rv)) {
    // Swallow this and just tell the consumer the default:
||||||| merged common ancestors
nsAboutProtocolHandler::GetFlagsForURI(nsIURI* aURI, uint32_t* aFlags)
{
    // First use the default (which is "unsafe for content"):
    GetProtocolFlags(aFlags);

    // Now try to see if this URI overrides the default:
    nsCOMPtr<nsIAboutModule> aboutMod;
    nsresult rv = NS_GetAboutModule(aURI, getter_AddRefs(aboutMod));
    if (NS_FAILED(rv)) {
      // Swallow this and just tell the consumer the default:
      return NS_OK;
    }
    uint32_t aboutModuleFlags = 0;
    rv = aboutMod->GetURIFlags(aURI, &aboutModuleFlags);
    // This should never happen, so pass back the error:
    NS_ENSURE_SUCCESS(rv, rv);

    // Secure (https) pages can load safe about pages without becoming
    // mixed content.
    if (aboutModuleFlags & nsIAboutModule::URI_SAFE_FOR_UNTRUSTED_CONTENT) {
        *aFlags |= URI_IS_POTENTIALLY_TRUSTWORTHY;
        // about: pages can only be loaded by unprivileged principals
        // if they are marked as LINKABLE
        if (aboutModuleFlags & nsIAboutModule::MAKE_LINKABLE) {
            // Replace URI_DANGEROUS_TO_LOAD with URI_LOADABLE_BY_ANYONE.
            *aFlags &= ~URI_DANGEROUS_TO_LOAD;
            *aFlags |= URI_LOADABLE_BY_ANYONE;
        }
    }
=======
nsAboutProtocolHandler::GetFlagsForURI(nsIURI* aURI, uint32_t* aFlags) {
  // First use the default (which is "unsafe for content"):
  GetProtocolFlags(aFlags);

  // Now try to see if this URI overrides the default:
  nsCOMPtr<nsIAboutModule> aboutMod;
  nsresult rv = NS_GetAboutModule(aURI, getter_AddRefs(aboutMod));
  if (NS_FAILED(rv)) {
    // Swallow this and just tell the consumer the default:
>>>>>>> upstream-releases
    return NS_OK;
  }
  uint32_t aboutModuleFlags = 0;
  rv = aboutMod->GetURIFlags(aURI, &aboutModuleFlags);
  // This should never happen, so pass back the error:
  NS_ENSURE_SUCCESS(rv, rv);

  // Secure (https) pages can load safe about pages without becoming
  // mixed content.
  if (aboutModuleFlags & nsIAboutModule::URI_SAFE_FOR_UNTRUSTED_CONTENT) {
    *aFlags |= URI_IS_POTENTIALLY_TRUSTWORTHY;
    // about: pages can only be loaded by unprivileged principals
    // if they are marked as LINKABLE
    if (aboutModuleFlags & nsIAboutModule::MAKE_LINKABLE) {
      // Replace URI_DANGEROUS_TO_LOAD with URI_LOADABLE_BY_ANYONE.
      *aFlags &= ~URI_DANGEROUS_TO_LOAD;
      *aFlags |= URI_LOADABLE_BY_ANYONE;
    }
  }
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP
nsAboutProtocolHandler::NewURI(const nsACString &aSpec,
                               const char *aCharset,  // ignore charset info
                               nsIURI *aBaseURI, nsIURI **result) {
  *result = nullptr;
  nsresult rv;
||||||| merged common ancestors
NS_IMETHODIMP
nsAboutProtocolHandler::NewURI(const nsACString &aSpec,
                               const char *aCharset, // ignore charset info
                               nsIURI *aBaseURI,
                               nsIURI **result)
{
    *result = nullptr;
    nsresult rv;


    // Use a simple URI to parse out some stuff first
    nsCOMPtr<nsIURI> url;
    rv = NS_MutateURI(new nsSimpleURI::Mutator())
           .SetSpec(aSpec)
           .Finalize(url);

    if (NS_FAILED(rv)) {
        return rv;
    }
=======
// static
nsresult nsAboutProtocolHandler::CreateNewURI(const nsACString& aSpec,
                                              const char* aCharset,
                                              nsIURI* aBaseURI,
                                              nsIURI** aResult) {
  *aResult = nullptr;
  nsresult rv;
>>>>>>> upstream-releases

  // Use a simple URI to parse out some stuff first
  nsCOMPtr<nsIURI> url;
  rv = NS_MutateURI(new nsSimpleURI::Mutator()).SetSpec(aSpec).Finalize(url);

<<<<<<< HEAD
  if (NS_FAILED(rv)) {
    return rv;
  }

  // Unfortunately, people create random about: URIs that don't correspond to
  // about: modules...  Since those URIs will never open a channel, might as
  // well consider them unsafe for better perf, and just in case.
  bool isSafe = false;

  nsCOMPtr<nsIAboutModule> aboutMod;
  rv = NS_GetAboutModule(url, getter_AddRefs(aboutMod));
  if (NS_SUCCEEDED(rv)) {
    isSafe = IsSafeToLinkForUntrustedContent(aboutMod, url);
  }

  if (isSafe) {
    // We need to indicate that this baby is safe.  Use an inner URI that
    // no one but the security manager will see.  Make sure to preserve our
    // path, in case someone decides to hardcode checks for particular
    // about: URIs somewhere.
    nsAutoCString spec;
    rv = url->GetPathQueryRef(spec);
    NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
    nsCOMPtr<nsIAboutModule> aboutMod;
    rv = NS_GetAboutModule(url, getter_AddRefs(aboutMod));
    if (NS_SUCCEEDED(rv)) {
        isSafe = IsSafeToLinkForUntrustedContent(aboutMod, url);
    }
=======
  if (NS_FAILED(rv)) {
    return rv;
  }

  if (IsSafeToLinkForUntrustedContent(url)) {
    // We need to indicate that this baby is safe.  Use an inner URI that
    // no one but the security manager will see.  Make sure to preserve our
    // path, in case someone decides to hardcode checks for particular
    // about: URIs somewhere.
    nsAutoCString spec;
    rv = url->GetPathQueryRef(spec);
    NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

    spec.InsertLiteral("moz-safe-about:", 0);

<<<<<<< HEAD
    nsCOMPtr<nsIURI> inner;
    rv = NS_NewURI(getter_AddRefs(inner), spec);
    NS_ENSURE_SUCCESS(rv, rv);

    nsCOMPtr<nsIURI> base(aBaseURI);
    rv = NS_MutateURI(new nsNestedAboutURI::Mutator())
             .Apply(NS_MutatorMethod(&nsINestedAboutURIMutator::InitWithBase,
                                     inner, base))
             .SetSpec(aSpec)
             .Finalize(url);
    NS_ENSURE_SUCCESS(rv, rv);
  }

  url.swap(*result);
  return NS_OK;
||||||| merged common ancestors
    url.swap(*result);
    return NS_OK;
=======
    nsCOMPtr<nsIURI> inner;
    rv = NS_NewURI(getter_AddRefs(inner), spec);
    NS_ENSURE_SUCCESS(rv, rv);

    nsCOMPtr<nsIURI> base(aBaseURI);
    rv = NS_MutateURI(new nsNestedAboutURI::Mutator())
             .Apply(NS_MutatorMethod(&nsINestedAboutURIMutator::InitWithBase,
                                     inner, base))
             .SetSpec(aSpec)
             .Finalize(url);
    NS_ENSURE_SUCCESS(rv, rv);
  }

  url.swap(*aResult);
  return NS_OK;
>>>>>>> upstream-releases
}

// The list of about: paths which are always allowed, regardless of enterprise
// policies.
//
// Note: This is stored as a two-dimensional array, with each element the size
// of the longest string in the list, for space efficiency. An array of
// character pointers would consume more space than the extra padding in
// shorter strings, and would require per-process relocations at load time.
//
// Important: This list MUST be kept sorted!
static const char kAboutPageEnterpriseWhitelist[][10] = {
    // clang-format off
    "blank",
    "certerror",
    "home",
    "neterror",
    "newtab",
    "welcome",
    // clang-format on
};

NS_IMETHODIMP
<<<<<<< HEAD
nsAboutProtocolHandler::NewChannel2(nsIURI *uri, nsILoadInfo *aLoadInfo,
                                    nsIChannel **result) {
  NS_ENSURE_ARG_POINTER(uri);

  // about:what you ask?
  nsCOMPtr<nsIAboutModule> aboutMod;
  nsresult rv = NS_GetAboutModule(uri, getter_AddRefs(aboutMod));

  bool aboutPageAllowed = true;
  nsAutoCString path;
  nsresult rv2 = NS_GetAboutModuleName(uri, path);
  if (NS_SUCCEEDED(rv2)) {
    if (path.EqualsLiteral("srcdoc")) {
      // about:srcdoc is meant to be unresolvable, yet is included in the
      // about lookup tables so that it can pass security checks when used in
      // a srcdoc iframe.  To ensure that it stays unresolvable, we pretend
      // that it doesn't exist.
      rv = NS_ERROR_FACTORY_NOT_REGISTERED;
    } else if (!path.EqualsLiteral("blank") &&
               !path.EqualsLiteral("neterror") && !path.EqualsLiteral("home") &&
               !path.EqualsLiteral("welcome") &&
               !path.EqualsLiteral("newtab") &&
               !path.EqualsLiteral("certerror")) {
      nsCOMPtr<nsIEnterprisePolicies> policyManager =
          do_GetService("@mozilla.org/browser/enterprisepolicies;1", &rv2);
      if (NS_SUCCEEDED(rv2)) {
        nsAutoCString normalizedURL;
        normalizedURL.AssignLiteral("about:");
        normalizedURL.Append(path);
        rv2 = policyManager->IsAllowed(normalizedURL, &aboutPageAllowed);
        if (NS_FAILED(rv2)) {
          aboutPageAllowed = false;
||||||| merged common ancestors
nsAboutProtocolHandler::NewChannel2(nsIURI* uri,
                                    nsILoadInfo* aLoadInfo,
                                    nsIChannel** result)
{
    NS_ENSURE_ARG_POINTER(uri);

    // about:what you ask?
    nsCOMPtr<nsIAboutModule> aboutMod;
    nsresult rv = NS_GetAboutModule(uri, getter_AddRefs(aboutMod));

    bool aboutPageAllowed = true;
    nsAutoCString path;
    nsresult rv2 = NS_GetAboutModuleName(uri, path);
    if (NS_SUCCEEDED(rv2)) {
        if (path.EqualsLiteral("srcdoc")) {
            // about:srcdoc is meant to be unresolvable, yet is included in the
            // about lookup tables so that it can pass security checks when used in
            // a srcdoc iframe.  To ensure that it stays unresolvable, we pretend
            // that it doesn't exist.
            rv = NS_ERROR_FACTORY_NOT_REGISTERED;
        } else if (!path.EqualsLiteral("blank") &&
                   !path.EqualsLiteral("neterror") &&
                   !path.EqualsLiteral("home") &&
                   !path.EqualsLiteral("welcome") &&
                   !path.EqualsLiteral("newtab") &&
                   !path.EqualsLiteral("certerror")) {
            nsCOMPtr<nsIEnterprisePolicies> policyManager =
                do_GetService("@mozilla.org/browser/enterprisepolicies;1", &rv2);
            if (NS_SUCCEEDED(rv2)) {
                nsAutoCString normalizedURL;
                normalizedURL.AssignLiteral("about:");
                normalizedURL.Append(path);
                rv2 = policyManager->IsAllowed(normalizedURL, &aboutPageAllowed);
                if (NS_FAILED(rv2)) {
                    aboutPageAllowed = false;
                }
            }
=======
nsAboutProtocolHandler::NewChannel(nsIURI* uri, nsILoadInfo* aLoadInfo,
                                   nsIChannel** result) {
  NS_ENSURE_ARG_POINTER(uri);

  // about:what you ask?
  nsCOMPtr<nsIAboutModule> aboutMod;
  nsresult rv = NS_GetAboutModule(uri, getter_AddRefs(aboutMod));

  bool aboutPageAllowed = true;
  nsAutoCString path;
  nsresult rv2 = NS_GetAboutModuleName(uri, path);
  if (NS_SUCCEEDED(rv2)) {
    size_t matchIdx;
    if (path.EqualsLiteral("srcdoc")) {
      // about:srcdoc is meant to be unresolvable, yet is included in the
      // about lookup tables so that it can pass security checks when used in
      // a srcdoc iframe.  To ensure that it stays unresolvable, we pretend
      // that it doesn't exist.
      rv = NS_ERROR_FACTORY_NOT_REGISTERED;
    } else if (!BinarySearchIf(
                   kAboutPageEnterpriseWhitelist, 0,
                   ArrayLength(kAboutPageEnterpriseWhitelist),
                   [&path](const char* aOther) { return path.Compare(aOther); },
                   &matchIdx)) {
      nsCOMPtr<nsIEnterprisePolicies> policyManager =
          do_GetService("@mozilla.org/enterprisepolicies;1", &rv2);
      if (NS_SUCCEEDED(rv2)) {
        nsAutoCString normalizedURL;
        normalizedURL.AssignLiteral("about:");
        normalizedURL.Append(path);
        rv2 = policyManager->IsAllowed(normalizedURL, &aboutPageAllowed);
        if (NS_FAILED(rv2)) {
          aboutPageAllowed = false;
>>>>>>> upstream-releases
        }
      }
    }
  }

  if (NS_SUCCEEDED(rv)) {
    // The standard return case:
    rv = aboutMod->NewChannel(uri, aLoadInfo, result);
    if (NS_SUCCEEDED(rv)) {
<<<<<<< HEAD
      // Not all implementations of nsIAboutModule::NewChannel()
      // set the LoadInfo on the newly created channel yet, as
      // an interim solution we set the LoadInfo here if not
      // available on the channel. Bug 1087720
      nsCOMPtr<nsILoadInfo> loadInfo = (*result)->GetLoadInfo();
      if (aLoadInfo != loadInfo) {
        if (loadInfo) {
          NS_ASSERTION(false,
                       "nsIAboutModule->newChannel(aURI, aLoadInfo) needs to "
                       "set LoadInfo");
          const char16_t *params[] = {
              u"nsIAboutModule->newChannel(aURI)",
              u"nsIAboutModule->newChannel(aURI, aLoadInfo)"};
          nsContentUtils::ReportToConsole(
              nsIScriptError::warningFlag,
              NS_LITERAL_CSTRING("Security by Default"),
              nullptr,  // aDocument
              nsContentUtils::eNECKO_PROPERTIES, "APIDeprecationWarning",
              params, mozilla::ArrayLength(params));
||||||| merged common ancestors
        // The standard return case:
        rv = aboutMod->NewChannel(uri, aLoadInfo, result);
        if (NS_SUCCEEDED(rv)) {
            // Not all implementations of nsIAboutModule::NewChannel()
            // set the LoadInfo on the newly created channel yet, as
            // an interim solution we set the LoadInfo here if not
            // available on the channel. Bug 1087720
            nsCOMPtr<nsILoadInfo> loadInfo = (*result)->GetLoadInfo();
            if (aLoadInfo != loadInfo) {
                if (loadInfo) {
                    NS_ASSERTION(false,
                        "nsIAboutModule->newChannel(aURI, aLoadInfo) needs to set LoadInfo");
                    const char16_t* params[] = {
                        u"nsIAboutModule->newChannel(aURI)",
                        u"nsIAboutModule->newChannel(aURI, aLoadInfo)"
                    };
                    nsContentUtils::ReportToConsole(
                        nsIScriptError::warningFlag,
                        NS_LITERAL_CSTRING("Security by Default"),
                        nullptr, // aDocument
                        nsContentUtils::eNECKO_PROPERTIES,
                        "APIDeprecationWarning",
                        params, mozilla::ArrayLength(params));
                }
                (*result)->SetLoadInfo(aLoadInfo);
            }

            // If this URI is safe for untrusted content, enforce that its
            // principal be based on the channel's originalURI by setting the
            // owner to null.
            // Note: this relies on aboutMod's newChannel implementation
            // having set the proper originalURI, which probably isn't ideal.
            if (IsSafeForUntrustedContent(aboutMod, uri)) {
                (*result)->SetOwner(nullptr);
            }

            RefPtr<nsNestedAboutURI> aboutURI;
            nsresult rv2 = uri->QueryInterface(kNestedAboutURICID,
                                               getter_AddRefs(aboutURI));
            if (NS_SUCCEEDED(rv2) && aboutURI->GetBaseURI()) {
                nsCOMPtr<nsIWritablePropertyBag2> writableBag =
                    do_QueryInterface(*result);
                if (writableBag) {
                    writableBag->
                        SetPropertyAsInterface(NS_LITERAL_STRING("baseURI"),
                                               aboutURI->GetBaseURI());
                }
            }
            if (!aboutPageAllowed) {
                (*result)->Cancel(NS_ERROR_BLOCKED_BY_POLICY);
            }
=======
      // Not all implementations of nsIAboutModule::NewChannel()
      // set the LoadInfo on the newly created channel yet, as
      // an interim solution we set the LoadInfo here if not
      // available on the channel. Bug 1087720
      nsCOMPtr<nsILoadInfo> loadInfo = (*result)->LoadInfo();
      if (aLoadInfo != loadInfo) {
        NS_ASSERTION(false,
                     "nsIAboutModule->newChannel(aURI, aLoadInfo) needs to "
                     "set LoadInfo");
        AutoTArray<nsString, 2> params = {
            NS_LITERAL_STRING("nsIAboutModule->newChannel(aURI)"),
            NS_LITERAL_STRING("nsIAboutModule->newChannel(aURI, aLoadInfo)")};
        nsContentUtils::ReportToConsole(
            nsIScriptError::warningFlag,
            NS_LITERAL_CSTRING("Security by Default"),
            nullptr,  // aDocument
            nsContentUtils::eNECKO_PROPERTIES, "APIDeprecationWarning", params);
        (*result)->SetLoadInfo(aLoadInfo);
      }

      // If this URI is safe for untrusted content, enforce that its
      // principal be based on the channel's originalURI by setting the
      // owner to null.
      // Note: this relies on aboutMod's newChannel implementation
      // having set the proper originalURI, which probably isn't ideal.
      if (IsSafeForUntrustedContent(aboutMod, uri)) {
        (*result)->SetOwner(nullptr);
      }

      RefPtr<nsNestedAboutURI> aboutURI;
      nsresult rv2 =
          uri->QueryInterface(kNestedAboutURICID, getter_AddRefs(aboutURI));
      if (NS_SUCCEEDED(rv2) && aboutURI->GetBaseURI()) {
        nsCOMPtr<nsIWritablePropertyBag2> writableBag =
            do_QueryInterface(*result);
        if (writableBag) {
          writableBag->SetPropertyAsInterface(NS_LITERAL_STRING("baseURI"),
                                              aboutURI->GetBaseURI());
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        (*result)->SetLoadInfo(aLoadInfo);
      }

      // If this URI is safe for untrusted content, enforce that its
      // principal be based on the channel's originalURI by setting the
      // owner to null.
      // Note: this relies on aboutMod's newChannel implementation
      // having set the proper originalURI, which probably isn't ideal.
      if (IsSafeForUntrustedContent(aboutMod, uri)) {
        (*result)->SetOwner(nullptr);
      }

      RefPtr<nsNestedAboutURI> aboutURI;
      nsresult rv2 =
          uri->QueryInterface(kNestedAboutURICID, getter_AddRefs(aboutURI));
      if (NS_SUCCEEDED(rv2) && aboutURI->GetBaseURI()) {
        nsCOMPtr<nsIWritablePropertyBag2> writableBag =
            do_QueryInterface(*result);
        if (writableBag) {
          writableBag->SetPropertyAsInterface(NS_LITERAL_STRING("baseURI"),
                                              aboutURI->GetBaseURI());
        }
      }
      if (!aboutPageAllowed) {
        (*result)->Cancel(NS_ERROR_BLOCKED_BY_POLICY);
      }
||||||| merged common ancestors
        return rv;
=======
      }
      if (!aboutPageAllowed) {
        (*result)->Cancel(NS_ERROR_BLOCKED_BY_POLICY);
      }
>>>>>>> upstream-releases
    }
    return rv;
  }

<<<<<<< HEAD
  // mumble...

  if (rv == NS_ERROR_FACTORY_NOT_REGISTERED) {
    // This looks like an about: we don't know about.  Convert
    // this to an invalid URI error.
    rv = NS_ERROR_MALFORMED_URI;
  }
||||||| merged common ancestors
    // mumble...

    if (rv == NS_ERROR_FACTORY_NOT_REGISTERED) {
        // This looks like an about: we don't know about.  Convert
        // this to an invalid URI error.
        rv = NS_ERROR_MALFORMED_URI;
    }
=======
  // mumble...
>>>>>>> upstream-releases

<<<<<<< HEAD
  return rv;
}
||||||| merged common ancestors
    return rv;
}
=======
  if (rv == NS_ERROR_FACTORY_NOT_REGISTERED) {
    // This looks like an about: we don't know about.  Convert
    // this to an invalid URI error.
    rv = NS_ERROR_MALFORMED_URI;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
nsAboutProtocolHandler::NewChannel(nsIURI *uri, nsIChannel **result) {
  return NewChannel2(uri, nullptr, result);
||||||| merged common ancestors
NS_IMETHODIMP
nsAboutProtocolHandler::NewChannel(nsIURI* uri, nsIChannel* *result)
{
    return NewChannel2(uri, nullptr, result);
=======
  return rv;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsAboutProtocolHandler::AllowPort(int32_t port, const char *scheme,
                                  bool *_retval) {
  // don't override anything.
  *_retval = false;
  return NS_OK;
||||||| merged common ancestors
nsAboutProtocolHandler::AllowPort(int32_t port, const char *scheme, bool *_retval)
{
    // don't override anything.
    *_retval = false;
    return NS_OK;
=======
nsAboutProtocolHandler::AllowPort(int32_t port, const char* scheme,
                                  bool* _retval) {
  // don't override anything.
  *_retval = false;
  return NS_OK;
>>>>>>> upstream-releases
}

////////////////////////////////////////////////////////////////////////////////
// Safe about protocol handler impl

NS_IMPL_ISUPPORTS(nsSafeAboutProtocolHandler, nsIProtocolHandler,
                  nsISupportsWeakReference)

// nsIProtocolHandler methods:

NS_IMETHODIMP
<<<<<<< HEAD
nsSafeAboutProtocolHandler::GetScheme(nsACString &result) {
  result.AssignLiteral("moz-safe-about");
  return NS_OK;
}

NS_IMETHODIMP
nsSafeAboutProtocolHandler::GetDefaultPort(int32_t *result) {
  *result = -1;  // no port for moz-safe-about: URLs
  return NS_OK;
}

NS_IMETHODIMP
nsSafeAboutProtocolHandler::GetProtocolFlags(uint32_t *result) {
  *result = URI_NORELATIVE | URI_NOAUTH | URI_LOADABLE_BY_ANYONE |
            URI_IS_POTENTIALLY_TRUSTWORTHY;
  return NS_OK;
||||||| merged common ancestors
nsSafeAboutProtocolHandler::GetScheme(nsACString &result)
{
    result.AssignLiteral("moz-safe-about");
    return NS_OK;
}

NS_IMETHODIMP
nsSafeAboutProtocolHandler::GetDefaultPort(int32_t *result)
{
    *result = -1;        // no port for moz-safe-about: URLs
    return NS_OK;
}

NS_IMETHODIMP
nsSafeAboutProtocolHandler::GetProtocolFlags(uint32_t *result)
{
    *result = URI_NORELATIVE | URI_NOAUTH | URI_LOADABLE_BY_ANYONE | URI_IS_POTENTIALLY_TRUSTWORTHY;
    return NS_OK;
=======
nsSafeAboutProtocolHandler::GetScheme(nsACString& result) {
  result.AssignLiteral("moz-safe-about");
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsSafeAboutProtocolHandler::NewURI(const nsACString &aSpec,
                                   const char *aCharset,  // ignore charset info
                                   nsIURI *aBaseURI, nsIURI **result) {
  nsresult rv =
      NS_MutateURI(new nsSimpleURI::Mutator()).SetSpec(aSpec).Finalize(result);
  if (NS_FAILED(rv)) {
    return rv;
  }

  return NS_OK;
||||||| merged common ancestors
nsSafeAboutProtocolHandler::NewURI(const nsACString &aSpec,
                                   const char *aCharset, // ignore charset info
                                   nsIURI *aBaseURI,
                                   nsIURI **result)
{
    nsresult rv = NS_MutateURI(new nsSimpleURI::Mutator())
                    .SetSpec(aSpec)
                    .Finalize(result);
    if (NS_FAILED(rv)) {
        return rv;
    }

    return NS_OK;
=======
nsSafeAboutProtocolHandler::GetDefaultPort(int32_t* result) {
  *result = -1;  // no port for moz-safe-about: URLs
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsSafeAboutProtocolHandler::NewChannel2(nsIURI *uri, nsILoadInfo *aLoadInfo,
                                        nsIChannel **result) {
  *result = nullptr;
  return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
nsSafeAboutProtocolHandler::NewChannel2(nsIURI* uri,
                                        nsILoadInfo* aLoadInfo,
                                        nsIChannel** result)
{
    *result = nullptr;
    return NS_ERROR_NOT_AVAILABLE;
=======
nsSafeAboutProtocolHandler::GetProtocolFlags(uint32_t* result) {
  *result = URI_NORELATIVE | URI_NOAUTH | URI_LOADABLE_BY_ANYONE |
            URI_IS_POTENTIALLY_TRUSTWORTHY;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsSafeAboutProtocolHandler::NewChannel(nsIURI *uri, nsIChannel **result) {
  *result = nullptr;
  return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
nsSafeAboutProtocolHandler::NewChannel(nsIURI* uri, nsIChannel* *result)
{
    *result = nullptr;
    return NS_ERROR_NOT_AVAILABLE;
=======
nsSafeAboutProtocolHandler::NewChannel(nsIURI* uri, nsILoadInfo* aLoadInfo,
                                       nsIChannel** result) {
  *result = nullptr;
  return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsSafeAboutProtocolHandler::AllowPort(int32_t port, const char *scheme,
                                      bool *_retval) {
  // don't override anything.
  *_retval = false;
  return NS_OK;
||||||| merged common ancestors
nsSafeAboutProtocolHandler::AllowPort(int32_t port, const char *scheme, bool *_retval)
{
    // don't override anything.
    *_retval = false;
    return NS_OK;
=======
nsSafeAboutProtocolHandler::AllowPort(int32_t port, const char* scheme,
                                      bool* _retval) {
  // don't override anything.
  *_retval = false;
  return NS_OK;
>>>>>>> upstream-releases
}

////////////////////////////////////////////////////////////
// nsNestedAboutURI implementation
NS_INTERFACE_MAP_BEGIN(nsNestedAboutURI)
  if (aIID.Equals(kNestedAboutURICID))
<<<<<<< HEAD
    foundInterface = static_cast<nsIURI *>(this);
||||||| merged common ancestors
      foundInterface = static_cast<nsIURI*>(this);
=======
    foundInterface = static_cast<nsIURI*>(this);
>>>>>>> upstream-releases
  else
NS_INTERFACE_MAP_END_INHERITING(nsSimpleNestedURI)

// nsISerializable

NS_IMETHODIMP
<<<<<<< HEAD
nsNestedAboutURI::Read(nsIObjectInputStream *aStream) {
  MOZ_ASSERT_UNREACHABLE("Use nsIURIMutator.read() instead");
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
nsNestedAboutURI::Read(nsIObjectInputStream *aStream)
{
    MOZ_ASSERT_UNREACHABLE("Use nsIURIMutator.read() instead");
    return NS_ERROR_NOT_IMPLEMENTED;
=======
nsNestedAboutURI::Read(nsIObjectInputStream* aStream) {
  MOZ_ASSERT_UNREACHABLE("Use nsIURIMutator.read() instead");
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsNestedAboutURI::ReadPrivate(nsIObjectInputStream *aStream) {
  nsresult rv = nsSimpleNestedURI::ReadPrivate(aStream);
  if (NS_FAILED(rv)) return rv;
||||||| merged common ancestors
nsresult
nsNestedAboutURI::ReadPrivate(nsIObjectInputStream *aStream)
{
    nsresult rv = nsSimpleNestedURI::ReadPrivate(aStream);
    if (NS_FAILED(rv)) return rv;
=======
nsresult nsNestedAboutURI::ReadPrivate(nsIObjectInputStream* aStream) {
  nsresult rv = nsSimpleNestedURI::ReadPrivate(aStream);
  if (NS_FAILED(rv)) return rv;
>>>>>>> upstream-releases

  bool haveBase;
  rv = aStream->ReadBoolean(&haveBase);
  if (NS_FAILED(rv)) return rv;

  if (haveBase) {
    nsCOMPtr<nsISupports> supports;
    rv = aStream->ReadObject(true, getter_AddRefs(supports));
    if (NS_FAILED(rv)) return rv;

    mBaseURI = do_QueryInterface(supports, &rv);
    if (NS_FAILED(rv)) return rv;
  }

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsNestedAboutURI::Write(nsIObjectOutputStream *aStream) {
  nsresult rv = nsSimpleNestedURI::Write(aStream);
  if (NS_FAILED(rv)) return rv;

  rv = aStream->WriteBoolean(mBaseURI != nullptr);
  if (NS_FAILED(rv)) return rv;

  if (mBaseURI) {
    // A previous iteration of this code wrote out mBaseURI as nsISupports
    // and then read it in as nsIURI, which is non-kosher when mBaseURI
    // implements more than just a single line of interfaces and the
    // canonical nsISupports* isn't the one a static_cast<> of mBaseURI
    // would produce.  For backwards compatibility with existing
    // serializations we continue to write mBaseURI as nsISupports but
    // switch to reading it as nsISupports, with a post-read QI to get to
    // nsIURI.
    rv = aStream->WriteCompoundObject(mBaseURI, NS_GET_IID(nsISupports), true);
||||||| merged common ancestors
nsNestedAboutURI::Write(nsIObjectOutputStream* aStream)
{
    nsresult rv = nsSimpleNestedURI::Write(aStream);
    if (NS_FAILED(rv)) return rv;

    rv = aStream->WriteBoolean(mBaseURI != nullptr);
=======
nsNestedAboutURI::Write(nsIObjectOutputStream* aStream) {
  nsresult rv = nsSimpleNestedURI::Write(aStream);
  if (NS_FAILED(rv)) return rv;

  rv = aStream->WriteBoolean(mBaseURI != nullptr);
  if (NS_FAILED(rv)) return rv;

  if (mBaseURI) {
    // A previous iteration of this code wrote out mBaseURI as nsISupports
    // and then read it in as nsIURI, which is non-kosher when mBaseURI
    // implements more than just a single line of interfaces and the
    // canonical nsISupports* isn't the one a static_cast<> of mBaseURI
    // would produce.  For backwards compatibility with existing
    // serializations we continue to write mBaseURI as nsISupports but
    // switch to reading it as nsISupports, with a post-read QI to get to
    // nsIURI.
    rv = aStream->WriteCompoundObject(mBaseURI, NS_GET_IID(nsISupports), true);
>>>>>>> upstream-releases
    if (NS_FAILED(rv)) return rv;
  }

  return NS_OK;
}

// nsSimpleURI
<<<<<<< HEAD
/* virtual */ nsSimpleURI *nsNestedAboutURI::StartClone(
    nsSimpleURI::RefHandlingEnum aRefHandlingMode, const nsACString &aNewRef) {
  // Sadly, we can't make use of nsSimpleNestedURI::StartClone here.
  // However, this function is expected to exactly match that function,
  // aside from the "new ns***URI()" call.
  NS_ENSURE_TRUE(mInnerURI, nullptr);

  nsCOMPtr<nsIURI> innerClone;
  nsresult rv = NS_OK;
  if (aRefHandlingMode == eHonorRef) {
    innerClone = mInnerURI;
  } else if (aRefHandlingMode == eReplaceRef) {
    rv = NS_GetURIWithNewRef(mInnerURI, aNewRef, getter_AddRefs(innerClone));
  } else {
    rv = NS_GetURIWithoutRef(mInnerURI, getter_AddRefs(innerClone));
  }

  if (NS_FAILED(rv)) {
    return nullptr;
  }

  nsNestedAboutURI *url = new nsNestedAboutURI(innerClone, mBaseURI);
  SetRefOnClone(url, aRefHandlingMode, aNewRef);

  return url;
||||||| merged common ancestors
/* virtual */ nsSimpleURI*
nsNestedAboutURI::StartClone(nsSimpleURI::RefHandlingEnum aRefHandlingMode,
                             const nsACString& aNewRef)
{
    // Sadly, we can't make use of nsSimpleNestedURI::StartClone here.
    // However, this function is expected to exactly match that function,
    // aside from the "new ns***URI()" call.
    NS_ENSURE_TRUE(mInnerURI, nullptr);

    nsCOMPtr<nsIURI> innerClone;
    nsresult rv = NS_OK;
    if (aRefHandlingMode == eHonorRef) {
        innerClone = mInnerURI;
    } else if (aRefHandlingMode == eReplaceRef) {
        rv = NS_GetURIWithNewRef(mInnerURI, aNewRef, getter_AddRefs(innerClone));
    } else {
        rv = NS_GetURIWithoutRef(mInnerURI, getter_AddRefs(innerClone));
    }

    if (NS_FAILED(rv)) {
        return nullptr;
    }

    nsNestedAboutURI* url = new nsNestedAboutURI(innerClone, mBaseURI);
    SetRefOnClone(url, aRefHandlingMode, aNewRef);

    return url;
=======
/* virtual */ nsSimpleURI* nsNestedAboutURI::StartClone(
    nsSimpleURI::RefHandlingEnum aRefHandlingMode, const nsACString& aNewRef) {
  // Sadly, we can't make use of nsSimpleNestedURI::StartClone here.
  // However, this function is expected to exactly match that function,
  // aside from the "new ns***URI()" call.
  NS_ENSURE_TRUE(mInnerURI, nullptr);

  nsCOMPtr<nsIURI> innerClone;
  nsresult rv = NS_OK;
  if (aRefHandlingMode == eHonorRef) {
    innerClone = mInnerURI;
  } else if (aRefHandlingMode == eReplaceRef) {
    rv = NS_GetURIWithNewRef(mInnerURI, aNewRef, getter_AddRefs(innerClone));
  } else {
    rv = NS_GetURIWithoutRef(mInnerURI, getter_AddRefs(innerClone));
  }

  if (NS_FAILED(rv)) {
    return nullptr;
  }

  nsNestedAboutURI* url = new nsNestedAboutURI(innerClone, mBaseURI);
  SetRefOnClone(url, aRefHandlingMode, aNewRef);

  return url;
>>>>>>> upstream-releases
}

// Queries this list of interfaces. If none match, it queries mURI.
NS_IMPL_NSIURIMUTATOR_ISUPPORTS(nsNestedAboutURI::Mutator, nsIURISetters,
                                nsIURIMutator, nsISerializable,
                                nsINestedAboutURIMutator)

NS_IMETHODIMP
<<<<<<< HEAD
nsNestedAboutURI::Mutate(nsIURIMutator **aMutator) {
  RefPtr<nsNestedAboutURI::Mutator> mutator = new nsNestedAboutURI::Mutator();
  nsresult rv = mutator->InitFromURI(this);
  if (NS_FAILED(rv)) {
    return rv;
  }
  mutator.forget(aMutator);
  return NS_OK;
||||||| merged common ancestors
nsNestedAboutURI::Mutate(nsIURIMutator** aMutator)
{
    RefPtr<nsNestedAboutURI::Mutator> mutator = new nsNestedAboutURI::Mutator();
    nsresult rv = mutator->InitFromURI(this);
    if (NS_FAILED(rv)) {
        return rv;
    }
    mutator.forget(aMutator);
    return NS_OK;
=======
nsNestedAboutURI::Mutate(nsIURIMutator** aMutator) {
  RefPtr<nsNestedAboutURI::Mutator> mutator = new nsNestedAboutURI::Mutator();
  nsresult rv = mutator->InitFromURI(this);
  if (NS_FAILED(rv)) {
    return rv;
  }
  mutator.forget(aMutator);
  return NS_OK;
>>>>>>> upstream-releases
}

// nsIClassInfo
NS_IMETHODIMP
<<<<<<< HEAD
nsNestedAboutURI::GetClassIDNoAlloc(nsCID *aClassIDNoAlloc) {
  *aClassIDNoAlloc = kNestedAboutURICID;
  return NS_OK;
||||||| merged common ancestors
nsNestedAboutURI::GetClassIDNoAlloc(nsCID *aClassIDNoAlloc)
{
    *aClassIDNoAlloc = kNestedAboutURICID;
    return NS_OK;
=======
nsNestedAboutURI::GetClassIDNoAlloc(nsCID* aClassIDNoAlloc) {
  *aClassIDNoAlloc = kNestedAboutURICID;
  return NS_OK;
>>>>>>> upstream-releases
}

}  // namespace net
}  // namespace mozilla
