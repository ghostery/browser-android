/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef ArchiveReader_h__
#define ArchiveReader_h__

#include <stdio.h>
#include "mar.h"

#ifdef XP_WIN
typedef WCHAR NS_tchar;
#else
typedef char NS_tchar;
#endif

// This class provides an API to extract files from an update archive.
class ArchiveReader {
 public:
  ArchiveReader() {}
  ~ArchiveReader() { Close(); }

  int Open(const NS_tchar* path);
  int VerifySignature();
  int VerifyProductInformation(const char* MARChannelID,
                               const char* appVersion);
  void Close();

  int ExtractFile(const char* item, const NS_tchar* destination);
  int ExtractFileToStream(const char* item, FILE* fp);

<<<<<<< HEAD
 private:
  int ExtractItemToStream(const MarItem *item, FILE *fp);
||||||| merged common ancestors
private:
  int ExtractItemToStream(const MarItem *item, FILE *fp);
=======
 private:
  int ExtractItemToStream(const MarItem* item, FILE* fp);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MarFile *mArchive = nullptr;
  uint8_t *mInBuf = nullptr;
  uint8_t *mOutBuf = nullptr;
  size_t mInBufSize = 262144;
||||||| merged common ancestors
  MarFile *mArchive = nullptr;
  uint8_t *mInBuf = nullptr;
  uint8_t *mOutBuf = nullptr;
  size_t mInBufSize  = 262144;
=======
  MarFile* mArchive = nullptr;
  uint8_t* mInBuf = nullptr;
  uint8_t* mOutBuf = nullptr;
  size_t mInBufSize = 262144;
>>>>>>> upstream-releases
  size_t mOutBufSize = 262144;
};

#endif  // ArchiveReader_h__
