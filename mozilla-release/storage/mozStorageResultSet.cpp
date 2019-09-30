/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: sw=2 ts=2 et lcs=trail\:.,tab\:>~ :
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozStorageRow.h"
#include "mozStorageResultSet.h"

namespace mozilla {
namespace storage {

////////////////////////////////////////////////////////////////////////////////
//// ResultSet

ResultSet::ResultSet() : mCurrentIndex(0) {}

ResultSet::~ResultSet() { mData.Clear(); }

<<<<<<< HEAD
nsresult ResultSet::add(mozIStorageRow *aRow) {
||||||| merged common ancestors
nsresult
ResultSet::add(mozIStorageRow *aRow)
{
=======
nsresult ResultSet::add(mozIStorageRow* aRow) {
>>>>>>> upstream-releases
  return mData.AppendObject(aRow) ? NS_OK : NS_ERROR_OUT_OF_MEMORY;
}

/**
 * Note:  This object is only ever accessed on one thread at a time.  It it not
 *        threadsafe, but it does need threadsafe AddRef and Release.
 */
NS_IMPL_ISUPPORTS(ResultSet, mozIStorageResultSet)

////////////////////////////////////////////////////////////////////////////////
//// mozIStorageResultSet

NS_IMETHODIMP
<<<<<<< HEAD
ResultSet::GetNextRow(mozIStorageRow **_row) {
||||||| merged common ancestors
ResultSet::GetNextRow(mozIStorageRow **_row)
{
=======
ResultSet::GetNextRow(mozIStorageRow** _row) {
>>>>>>> upstream-releases
  NS_ENSURE_ARG_POINTER(_row);

  if (mCurrentIndex >= mData.Count()) {
    // Just return null here
    return NS_OK;
  }

  NS_ADDREF(*_row = mData.ObjectAt(mCurrentIndex++));
  return NS_OK;
}

}  // namespace storage
}  // namespace mozilla
