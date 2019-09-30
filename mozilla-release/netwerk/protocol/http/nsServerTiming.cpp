/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsServerTiming.h"

NS_IMPL_ISUPPORTS(nsServerTiming, nsIServerTiming)

NS_IMETHODIMP
<<<<<<< HEAD
nsServerTiming::GetName(nsACString &aName) {
||||||| merged common ancestors
nsServerTiming::GetName(nsACString &aName)
{
=======
nsServerTiming::GetName(nsACString& aName) {
>>>>>>> upstream-releases
  aName.Assign(mName);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsServerTiming::GetDuration(double *aDuration) {
||||||| merged common ancestors
nsServerTiming::GetDuration(double *aDuration)
{
=======
nsServerTiming::GetDuration(double* aDuration) {
>>>>>>> upstream-releases
  *aDuration = mDuration;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsServerTiming::GetDescription(nsACString &aDescription) {
||||||| merged common ancestors
nsServerTiming::GetDescription(nsACString &aDescription)
{
=======
nsServerTiming::GetDescription(nsACString& aDescription) {
>>>>>>> upstream-releases
  aDescription.Assign(mDescription);
  return NS_OK;
}

namespace mozilla {
namespace net {

<<<<<<< HEAD
static double ParseDouble(const nsACString &aString) {
||||||| merged common ancestors
static double
ParseDouble(const nsACString& aString)
{
=======
static double ParseDouble(const nsACString& aString) {
>>>>>>> upstream-releases
  nsresult rv;
  double val = PromiseFlatCString(aString).ToDouble(&rv);
  return NS_FAILED(rv) ? 0.0f : val;
}

void ServerTimingParser::Parse() {
  // https://w3c.github.io/server-timing/#the-server-timing-header-field
  // Server-Timing             = #server-timing-metric
  // server-timing-metric      = metric-name *( OWS ";" OWS server-timing-param
  // ) metric-name               = token server-timing-param       =
  // server-timing-param-name OWS "=" OWS
  //                             server-timing-param-value
  // server-timing-param-name  = token
  // server-timing-param-value = token / quoted-string

  ParsedHeaderValueListList parsedHeader(mValue, false);
  for (uint32_t index = 0; index < parsedHeader.mValues.Length(); ++index) {
    if (parsedHeader.mValues[index].mValues.IsEmpty()) {
      continue;
    }

    // According to spec, the first ParsedHeaderPair's name is metric-name.
    RefPtr<nsServerTiming> timingHeader = new nsServerTiming();
    mServerTimingHeaders.AppendElement(timingHeader);
    timingHeader->SetName(parsedHeader.mValues[index].mValues[0].mName);

    if (parsedHeader.mValues[index].mValues.Length() == 1) {
      continue;
    }

    // Try to find duration and description from the rest ParsedHeaderPairs.
    bool foundDuration = false;
    bool foundDescription = false;
    for (uint32_t pairIndex = 1;
         pairIndex < parsedHeader.mValues[index].mValues.Length();
         ++pairIndex) {
<<<<<<< HEAD
      nsDependentCSubstring &currentName =
          parsedHeader.mValues[index].mValues[pairIndex].mName;
      nsDependentCSubstring &currentValue =
          parsedHeader.mValues[index].mValues[pairIndex].mValue;
||||||| merged common ancestors
      nsDependentCSubstring &currentName =
        parsedHeader.mValues[index].mValues[pairIndex].mName;
      nsDependentCSubstring &currentValue =
        parsedHeader.mValues[index].mValues[pairIndex].mValue;
=======
      nsDependentCSubstring& currentName =
          parsedHeader.mValues[index].mValues[pairIndex].mName;
      nsDependentCSubstring& currentValue =
          parsedHeader.mValues[index].mValues[pairIndex].mValue;
>>>>>>> upstream-releases

      // We should only take the value from the first
      // occurrence of server-timing-param-name ("dur" and "desc").
      // This is true whether or not the value makes any sense (or, indeed, if
      // there even is a value).
      if (currentName.LowerCaseEqualsASCII("dur") && !foundDuration) {
        if (currentValue.BeginReading()) {
          timingHeader->SetDuration(ParseDouble(currentValue));
        } else {
          timingHeader->SetDuration(0.0);
        }
        foundDuration = true;
      } else if (currentName.LowerCaseEqualsASCII("desc") &&
                 !foundDescription) {
        if (!currentValue.IsEmpty()) {
          timingHeader->SetDescription(currentValue);
        } else {
          timingHeader->SetDescription(EmptyCString());
        }
        foundDescription = true;
      }

      if (foundDuration && foundDescription) {
        break;
      }
    }
  }
}

<<<<<<< HEAD
nsTArray<nsCOMPtr<nsIServerTiming>>
    &&ServerTimingParser::TakeServerTimingHeaders() {
||||||| merged common ancestors
nsTArray<nsCOMPtr<nsIServerTiming>>&&
ServerTimingParser::TakeServerTimingHeaders()
{
=======
nsTArray<nsCOMPtr<nsIServerTiming>>&&
ServerTimingParser::TakeServerTimingHeaders() {
>>>>>>> upstream-releases
  return std::move(mServerTimingHeaders);
}

}  // namespace net
}  // namespace mozilla
