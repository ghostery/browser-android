/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_quota_ActorsParent_h
#define mozilla_dom_quota_ActorsParent_h

namespace mozilla {
namespace dom {
namespace quota {

class PQuotaParent;

<<<<<<< HEAD
PQuotaParent* AllocPQuotaParent();
||||||| merged common ancestors
PQuotaParent*
AllocPQuotaParent();
=======
void InitializeQuotaManager();
>>>>>>> upstream-releases

<<<<<<< HEAD
bool DeallocPQuotaParent(PQuotaParent* aActor);
||||||| merged common ancestors
bool
DeallocPQuotaParent(PQuotaParent* aActor);
=======
PQuotaParent* AllocPQuotaParent();
>>>>>>> upstream-releases

<<<<<<< HEAD
}  // namespace quota
}  // namespace dom
}  // namespace mozilla
||||||| merged common ancestors
} // namespace quota
} // namespace dom
} // namespace mozilla
=======
bool DeallocPQuotaParent(PQuotaParent* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
#endif  // mozilla_dom_quota_ActorsParent_h
||||||| merged common ancestors
#endif // mozilla_dom_quota_ActorsParent_h
=======
bool RecvShutdownQuotaManager();

}  // namespace quota
}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_quota_ActorsParent_h
>>>>>>> upstream-releases
