/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_StorageUtils_h
#define mozilla_dom_StorageUtils_h

#include "nsStringFwd.h"

class nsIPrincipal;

namespace mozilla {
namespace dom {
namespace StorageUtils {

<<<<<<< HEAD
nsresult GenerateOriginKey(nsIPrincipal* aPrincipal,
                           nsACString& aOriginAttrSuffix,
                           nsACString& aOriginKey);
||||||| merged common ancestors
nsresult
GenerateOriginKey(nsIPrincipal* aPrincipal, nsACString& aOriginAttrSuffix,
                  nsACString& aOriginKey);
=======
nsresult GenerateOriginKey(nsIPrincipal* aPrincipal,
                           nsACString& aOriginAttrSuffix,
                           nsACString& aOriginKey);

bool PrincipalsEqual(nsIPrincipal* aObjectPrincipal,
                     nsIPrincipal* aSubjectPrincipal);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool PrincipalsEqual(nsIPrincipal* aObjectPrincipal,
                     nsIPrincipal* aSubjectPrincipal);
||||||| merged common ancestors
bool
PrincipalsEqual(nsIPrincipal* aObjectPrincipal,
                nsIPrincipal* aSubjectPrincipal);
=======
void ReverseString(const nsACString& aSource, nsACString& aResult);
>>>>>>> upstream-releases

<<<<<<< HEAD
void ReverseString(const nsACString& aSource, nsACString& aResult);
||||||| merged common ancestors
void
ReverseString(const nsACString& aSource, nsACString& aResult);
=======
nsresult CreateReversedDomain(const nsACString& aAsciiDomain, nsACString& aKey);
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult CreateReversedDomain(const nsACString& aAsciiDomain, nsACString& aKey);
||||||| merged common ancestors
nsresult
CreateReversedDomain(const nsACString& aAsciiDomain,
                     nsACString& aKey);
=======
nsCString Scheme0Scope(const nsACString& aOriginSuffix,
                       const nsACString& aOriginNoSuffix);
>>>>>>> upstream-releases

<<<<<<< HEAD
nsCString Scheme0Scope(const nsACString& aOriginSuffix,
                       const nsACString& aOriginNoSuffix);
||||||| merged common ancestors
} // StorageUtils namespace
} // dom namespace
} // mozilla namespace
=======
}  // namespace StorageUtils
}  // namespace dom
}  // namespace mozilla
>>>>>>> upstream-releases

<<<<<<< HEAD
}  // namespace StorageUtils
}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_StorageUtils_h
||||||| merged common ancestors
#endif // mozilla_dom_StorageUtils_h
=======
#endif  // mozilla_dom_StorageUtils_h
>>>>>>> upstream-releases
