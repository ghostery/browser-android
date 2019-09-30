/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsIDHCPClient.h"
#include "nsIServiceManager.h"
#include "nsNetCID.h"
#include "WindowsNetworkFunctionsWrapper.h"

namespace mozilla {
namespace toolkit {
namespace system {
namespace windowsDHCPClient {

class nsWindowsDHCPClient final : public nsIDHCPClient {
 public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIDHCPCLIENT

<<<<<<< HEAD
  explicit nsWindowsDHCPClient(
      WindowsNetworkFunctionsWrapper* aNetworkFunctions =
          new WindowsNetworkFunctionsWrapper())
      : mNetworkFunctions(aNetworkFunctions){};
  nsresult Init();

 private:
  ~nsWindowsDHCPClient(){};
  WindowsNetworkFunctionsWrapper* mNetworkFunctions;
||||||| merged common ancestors
  explicit nsWindowsDHCPClient(WindowsNetworkFunctionsWrapper *aNetworkFunctions = new WindowsNetworkFunctionsWrapper())
    : mNetworkFunctions(aNetworkFunctions) {};
  nsresult Init();

private:

   ~nsWindowsDHCPClient() {};
   WindowsNetworkFunctionsWrapper* mNetworkFunctions;

=======
  explicit nsWindowsDHCPClient(
      WindowsNetworkFunctionsWrapper* aNetworkFunctions =
          new WindowsNetworkFunctionsWrapper())
      : mNetworkFunctions(aNetworkFunctions){};

 private:
  ~nsWindowsDHCPClient(){};
  WindowsNetworkFunctionsWrapper* mNetworkFunctions;
>>>>>>> upstream-releases
};

<<<<<<< HEAD
}  // namespace windowsDHCPClient
}  // namespace system
}  // namespace toolkit
}  // namespace mozilla
||||||| merged common ancestors

} // namespace windowsDHCPClient
} // namespace system
} // namespace toolkit
} // namespace mozilla
=======
}  // namespace windowsDHCPClient
}  // namespace system
}  // namespace toolkit
}  // namespace mozilla
>>>>>>> upstream-releases
