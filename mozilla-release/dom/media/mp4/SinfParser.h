/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef SINF_PARSER_H_
#define SINF_PARSER_H_

#include "mozilla/ResultExtensions.h"
#include "Atom.h"
#include "AtomType.h"

namespace mozilla {

class Box;

<<<<<<< HEAD
class Sinf : public Atom {
 public:
  Sinf() : mDefaultIVSize(0), mDefaultEncryptionType() {}
||||||| merged common ancestors
class Sinf : public Atom
{
public:
  Sinf()
    : mDefaultIVSize(0)
    , mDefaultEncryptionType()
  {}
=======
class Sinf : public Atom {
 public:
  Sinf()
      : mDefaultIVSize(0),
        mDefaultEncryptionType(),
        mDefaultCryptByteBlock(0),
        mDefaultSkipByteBlock(0) {}
>>>>>>> upstream-releases
  explicit Sinf(Box& aBox);

<<<<<<< HEAD
  virtual bool IsValid() override {
    return !!mDefaultIVSize && !!mDefaultEncryptionType;
||||||| merged common ancestors
  virtual bool IsValid() override
  {
    return !!mDefaultIVSize && !!mDefaultEncryptionType;
=======
  bool IsValid() override {
    return !!mDefaultEncryptionType &&  // Should have an encryption scheme
           (mDefaultIVSize > 0 ||       // and either a default IV size
            mDefaultConstantIV.Length() > 0);  // or a constant IV.
>>>>>>> upstream-releases
  }

  uint8_t mDefaultIVSize;
  AtomType mDefaultEncryptionType;
  uint8_t mDefaultKeyID[16];
  uint8_t mDefaultCryptByteBlock;
  uint8_t mDefaultSkipByteBlock;
  nsTArray<uint8_t> mDefaultConstantIV;
};

class SinfParser {
 public:
  explicit SinfParser(Box& aBox);

  Sinf& GetSinf() { return mSinf; }

 private:
  Result<Ok, nsresult> ParseSchm(Box& aBox);
  Result<Ok, nsresult> ParseSchi(Box& aBox);
  Result<Ok, nsresult> ParseTenc(Box& aBox);

  Sinf mSinf;
};

}  // namespace mozilla

#endif  // SINF_PARSER_H_
