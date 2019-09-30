/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsIMIMEHeaderParam.h"

#ifndef __nsmimeheaderparamimpl_h___
<<<<<<< HEAD
#define __nsmimeheaderparamimpl_h___
class nsMIMEHeaderParamImpl : public nsIMIMEHeaderParam {
 public:
||||||| merged common ancestors
#define __nsmimeheaderparamimpl_h___
class nsMIMEHeaderParamImpl : public nsIMIMEHeaderParam
{
public:
=======
#  define __nsmimeheaderparamimpl_h___
class nsMIMEHeaderParamImpl : public nsIMIMEHeaderParam {
 public:
>>>>>>> upstream-releases
  NS_DECL_ISUPPORTS
  NS_DECL_NSIMIMEHEADERPARAM

  nsMIMEHeaderParamImpl() = default;

  /**
   * Identical to calling
   * GetParameterHTTP(aHeaderVal, aParameterName, EmptyCString(), false,
   * nullptr, aResult) See nsIMIMEHeaderParam.idl for more information.
   */
<<<<<<< HEAD
  static nsresult GetParameterHTTP(const nsACString &aHeaderVal,
                                   const char *aParamName, nsAString &aResult);
||||||| merged common ancestors
  static nsresult GetParameterHTTP(const nsACString& aHeaderVal,
                                   const char *aParamName,
                                   nsAString &aResult);
=======
  static nsresult GetParameterHTTP(const nsACString& aHeaderVal,
                                   const char* aParamName, nsAString& aResult);
>>>>>>> upstream-releases

 private:
  virtual ~nsMIMEHeaderParamImpl() = default;
  enum ParamDecoding { MIME_FIELD_ENCODING = 1, HTTP_FIELD_ENCODING };

<<<<<<< HEAD
  static nsresult DoGetParameter(const nsACString &aHeaderVal,
                                 const char *aParamName,
||||||| merged common ancestors
  static nsresult DoGetParameter(const nsACString& aHeaderVal,
                                 const char *aParamName,
=======
  static nsresult DoGetParameter(const nsACString& aHeaderVal,
                                 const char* aParamName,
>>>>>>> upstream-releases
                                 ParamDecoding aDecoding,
<<<<<<< HEAD
                                 const nsACString &aFallbackCharset,
                                 bool aTryLocaleCharset, char **aLang,
                                 nsAString &aResult);
||||||| merged common ancestors
                                 const nsACString& aFallbackCharset,
                                 bool aTryLocaleCharset,
                                 char **aLang,
                                 nsAString& aResult);
=======
                                 const nsACString& aFallbackCharset,
                                 bool aTryLocaleCharset, char** aLang,
                                 nsAString& aResult);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static nsresult DoParameterInternal(const char *aHeaderValue,
                                      const char *aParamName,
                                      ParamDecoding aDecoding, char **aCharset,
                                      char **aLang, char **aResult);
||||||| merged common ancestors
  static nsresult DoParameterInternal(const char *aHeaderValue,
                                      const char *aParamName,
                                      ParamDecoding aDecoding,
                                      char **aCharset,
                                      char **aLang,
                                      char **aResult);
=======
  static nsresult DoParameterInternal(const char* aHeaderValue,
                                      const char* aParamName,
                                      ParamDecoding aDecoding, char** aCharset,
                                      char** aLang, char** aResult);
>>>>>>> upstream-releases
};

#endif
