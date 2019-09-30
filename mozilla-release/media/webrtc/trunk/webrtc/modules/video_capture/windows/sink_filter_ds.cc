/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "modules/video_capture/windows/sink_filter_ds.h"

<<<<<<< HEAD
#include <ios>  // std::hex
||||||| merged common ancestors
#include "webrtc/modules/video_capture/windows/help_functions_ds.h"
#include "webrtc/system_wrappers/include/trace.h"
=======
#include "rtc_base/arraysize.h"
#include "rtc_base/checks.h"
#include "rtc_base/logging.h"
#include "rtc_base/platform_thread.h"
#include "rtc_base/stringutils.h"
>>>>>>> upstream-releases

<<<<<<< HEAD
#include "modules/video_capture/windows/help_functions_ds.h"
#include "rtc_base/checks.h"
#include "rtc_base/logging.h"
#include "rtc_base/platform_thread.h"
||||||| merged common ancestors
#include <dvdmedia.h> // VIDEOINFOHEADER2
#include <initguid.h>
=======
#include <dvdmedia.h>  // VIDEOINFOHEADER2
#include <initguid.h>
>>>>>>> upstream-releases

<<<<<<< HEAD
#include <dvdmedia.h>  // VIDEOINFOHEADER2
#include <initguid.h>

#define DELETE_RESET(p) \
  {                     \
    delete (p);         \
    (p) = NULL;         \
  }

DEFINE_GUID(CLSID_SINKFILTER,
            0x88cdbbdc,
            0xa73b,
            0x4afa,
            0xac,
            0xbf,
            0x15,
            0xd5,
            0xe2,
            0xce,
            0x12,
            0xc3);

using namespace mozilla::media;
using namespace mozilla;

namespace webrtc {
namespace videocapturemodule {

typedef struct tagTHREADNAME_INFO {
  DWORD dwType;      // must be 0x1000
  LPCSTR szName;     // pointer to name (in user addr space)
  DWORD dwThreadID;  // thread ID (-1=caller thread)
  DWORD dwFlags;     // reserved for future use, must be zero
} THREADNAME_INFO;

CaptureInputPin::CaptureInputPin(int32_t moduleId,
                                 IN TCHAR * szName,
                                 IN CaptureSinkFilter* pFilter,
                                 IN CriticalSection * pLock,
                                 OUT HRESULT* pHr,
                                 IN LPCWSTR pszName)
    : BaseInputPin(szName, pFilter, pLock, pHr, pszName),
      _requestedCapability(),
      _resultingCapability() {
  _moduleId=moduleId;
  _threadHandle = NULL;
}

CaptureInputPin::~CaptureInputPin() {}

HRESULT
CaptureInputPin::GetMediaType(IN int iPosition, OUT MediaType* pmt) {
  // reset the thread handle
  _threadHandle = NULL;

  if (iPosition < 0)
    return E_INVALIDARG;

  VIDEOINFOHEADER* pvi =
      (VIDEOINFOHEADER*)pmt->AllocFormatBuffer(sizeof(VIDEOINFOHEADER));
  if (NULL == pvi) {
    RTC_LOG(LS_INFO) << "CheckMediaType VIDEOINFOHEADER is NULL. Returning.";
    return (E_OUTOFMEMORY);
  }

  ZeroMemory(pvi, sizeof(VIDEOINFOHEADER));
  pvi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  pvi->bmiHeader.biPlanes = 1;
  pvi->bmiHeader.biClrImportant = 0;
  pvi->bmiHeader.biClrUsed = 0;
  if (_requestedCapability.maxFPS != 0) {
    pvi->AvgTimePerFrame = 10000000 / _requestedCapability.maxFPS;
  }

  SetRectEmpty(&(pvi->rcSource));  // we want the whole image area rendered.
  SetRectEmpty(&(pvi->rcTarget));  // no particular destination rectangle

  pmt->SetType(&MEDIATYPE_Video);
  pmt->SetFormatType(&FORMAT_VideoInfo);
  pmt->SetTemporalCompression(FALSE);

  int32_t positionOffset = 1;
  switch (iPosition + positionOffset) {
    case 0: {
      pvi->bmiHeader.biCompression = MAKEFOURCC('I', '4', '2', '0');
      pvi->bmiHeader.biBitCount = 12;  // bit per pixel
      pvi->bmiHeader.biWidth = _requestedCapability.width;
      pvi->bmiHeader.biHeight = _requestedCapability.height;
      pvi->bmiHeader.biSizeImage =
          3 * _requestedCapability.height * _requestedCapability.width / 2;
      pmt->SetSubtype(&MEDIASUBTYPE_I420);
    } break;
    case 1: {
      pvi->bmiHeader.biCompression = MAKEFOURCC('Y', 'U', 'Y', '2');
      ;
      pvi->bmiHeader.biBitCount = 16;  // bit per pixel
      pvi->bmiHeader.biWidth = _requestedCapability.width;
      pvi->bmiHeader.biHeight = _requestedCapability.height;
      pvi->bmiHeader.biSizeImage =
          2 * _requestedCapability.width * _requestedCapability.height;
      pmt->SetSubtype(&MEDIASUBTYPE_YUY2);
    } break;
    case 2: {
      pvi->bmiHeader.biCompression = BI_RGB;
      pvi->bmiHeader.biBitCount = 24;  // bit per pixel
      pvi->bmiHeader.biWidth = _requestedCapability.width;
      pvi->bmiHeader.biHeight = _requestedCapability.height;
      pvi->bmiHeader.biSizeImage =
          3 * _requestedCapability.height * _requestedCapability.width;
      pmt->SetSubtype(&MEDIASUBTYPE_RGB24);
    } break;
    case 3: {
      pvi->bmiHeader.biCompression = MAKEFOURCC('U', 'Y', 'V', 'Y');
      pvi->bmiHeader.biBitCount = 16;  // bit per pixel
      pvi->bmiHeader.biWidth = _requestedCapability.width;
      pvi->bmiHeader.biHeight = _requestedCapability.height;
      pvi->bmiHeader.biSizeImage =
          2 * _requestedCapability.height * _requestedCapability.width;
      pmt->SetSubtype(&MEDIASUBTYPE_UYVY);
    } break;
    case 4: {
      pvi->bmiHeader.biCompression = MAKEFOURCC('M', 'J', 'P', 'G');
      pvi->bmiHeader.biBitCount = 12;  // bit per pixel
      pvi->bmiHeader.biWidth = _requestedCapability.width;
      pvi->bmiHeader.biHeight = _requestedCapability.height;
      pvi->bmiHeader.biSizeImage =
          3 * _requestedCapability.height * _requestedCapability.width / 2;
      pmt->SetSubtype(&MEDIASUBTYPE_MJPG);
    } break;
    default:
      return VFW_S_NO_MORE_ITEMS;
  }
  pmt->SetSampleSize(pvi->bmiHeader.biSizeImage);
  RTC_LOG(LS_INFO) << "GetMediaType position " << iPosition << ", width "
                   << _requestedCapability.width << ", height "
                   << _requestedCapability.height << ", biCompression 0x"
                   << std::hex << pvi->bmiHeader.biCompression;
  return NOERROR;
}

HRESULT
CaptureInputPin::CheckMediaType(IN const MediaType* pMediaType) {
  // reset the thread handle
  _threadHandle = NULL;

  const GUID* type = pMediaType->Type();
  if (*type != MEDIATYPE_Video)
    return E_INVALIDARG;

  const GUID* formatType = pMediaType->FormatType();

  // Check for the subtypes we support
  const GUID* SubType = pMediaType->Subtype();
  if (SubType == NULL) {
    return E_INVALIDARG;
  }

  if (*formatType == FORMAT_VideoInfo) {
    VIDEOINFOHEADER* pvi = (VIDEOINFOHEADER*)pMediaType->Format();
    if (pvi == NULL) {
      return E_INVALIDARG;
||||||| merged common ancestors
#define DELETE_RESET(p) { delete (p) ; (p) = NULL ;}

DEFINE_GUID(CLSID_SINKFILTER, 0x88cdbbdc, 0xa73b, 0x4afa, 0xac, 0xbf, 0x15, 0xd5,
            0xe2, 0xce, 0x12, 0xc3);

using namespace mozilla::media;
using namespace mozilla;

namespace webrtc
{
namespace videocapturemodule
{

typedef struct tagTHREADNAME_INFO
{
   DWORD dwType;        // must be 0x1000
   LPCSTR szName;       // pointer to name (in user addr space)
   DWORD dwThreadID;    // thread ID (-1=caller thread)
   DWORD dwFlags;       // reserved for future use, must be zero
} THREADNAME_INFO;

CaptureInputPin::CaptureInputPin (int32_t moduleId,
                            IN TCHAR * szName,
                            IN CaptureSinkFilter* pFilter,
                            IN CriticalSection * pLock,
                            OUT HRESULT * pHr,
                            IN LPCWSTR pszName)
    : BaseInputPin (szName, pFilter, pLock, pHr, pszName),
      _requestedCapability(),
      _resultingCapability()
{
    _moduleId=moduleId;
    _threadHandle = NULL;
}

CaptureInputPin::~CaptureInputPin()
{
}

HRESULT
CaptureInputPin::GetMediaType (IN int iPosition, OUT MediaType * pmt)
{
    // reset the thread handle
    _threadHandle = NULL;

    if(iPosition < 0)
    return E_INVALIDARG;

    VIDEOINFOHEADER* pvi = (VIDEOINFOHEADER*) pmt->AllocFormatBuffer(
                            sizeof(VIDEOINFOHEADER));
    if(NULL == pvi)
    {
        WEBRTC_TRACE(webrtc::kTraceInfo, webrtc::kTraceVideoCapture, _moduleId,
                     "CheckMediaType VIDEOINFOHEADER is NULL. Returning...Line:%d\n", __LINE__);
        return(E_OUTOFMEMORY);
=======
#include <algorithm>

DEFINE_GUID(CLSID_SINKFILTER,
            0x88cdbbdc,
            0xa73b,
            0x4afa,
            0xac,
            0xbf,
            0x15,
            0xd5,
            0xe2,
            0xce,
            0x12,
            0xc3);

namespace webrtc {
namespace videocapturemodule {
namespace {

// Simple enumeration implementation that enumerates over a single pin :-/
class EnumPins : public IEnumPins {
 public:
  EnumPins(IPin* pin) : pin_(pin) {}

 protected:
  virtual ~EnumPins() {}

 private:
  STDMETHOD(QueryInterface)(REFIID riid, void** ppv) override {
    if (riid == IID_IUnknown || riid == IID_IEnumPins) {
      *ppv = static_cast<IEnumPins*>(this);
      AddRef();
      return S_OK;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD

    // Store the incoming width and height
    _resultingCapability.width = pvi->bmiHeader.biWidth;

    // Store the incoming height,
    // for RGB24 we assume the frame to be upside down
    if (*SubType == MEDIASUBTYPE_RGB24 && pvi->bmiHeader.biHeight > 0) {
      _resultingCapability.height = -(pvi->bmiHeader.biHeight);
    } else {
      _resultingCapability.height = abs(pvi->bmiHeader.biHeight);
||||||| merged common ancestors

    ZeroMemory(pvi, sizeof(VIDEOINFOHEADER));
    pvi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pvi->bmiHeader.biPlanes = 1;
    pvi->bmiHeader.biClrImportant = 0;
    pvi->bmiHeader.biClrUsed = 0;
    if (_requestedCapability.maxFPS != 0) {
        pvi->AvgTimePerFrame = 10000000/_requestedCapability.maxFPS;
=======
    return E_NOINTERFACE;
  }

  STDMETHOD(Clone)(IEnumPins** pins) {
    RTC_DCHECK(false);
    return E_NOTIMPL;
  }

  STDMETHOD(Next)(ULONG count, IPin** pins, ULONG* fetched) {
    RTC_DCHECK(count > 0);
    RTC_DCHECK(pins);
    // fetched may be NULL.

    if (pos_ > 0) {
      if (fetched)
        *fetched = 0;
      return S_FALSE;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    RTC_LOG(LS_INFO) << "CheckMediaType width:" << pvi->bmiHeader.biWidth
                     << " height:" << pvi->bmiHeader.biHeight
                     << " Compression:0x" << std::hex
                     << pvi->bmiHeader.biCompression;
||||||| merged common ancestors
    SetRectEmpty(&(pvi->rcSource)); // we want the whole image area rendered.
    SetRectEmpty(&(pvi->rcTarget)); // no particular destination rectangle
=======
    ++pos_;
    pins[0] = pin_.get();
    pins[0]->AddRef();
    if (fetched)
      *fetched = 1;

    return count == 1 ? S_OK : S_FALSE;
  }

  STDMETHOD(Skip)(ULONG count) {
    RTC_DCHECK(false);
    return E_NOTIMPL;
  }

  STDMETHOD(Reset)() {
    pos_ = 0;
    return S_OK;
  }

  rtc::scoped_refptr<IPin> pin_;
  int pos_ = 0;
};

bool IsMediaTypePartialMatch(const AM_MEDIA_TYPE& a, const AM_MEDIA_TYPE& b) {
  if (b.majortype != GUID_NULL && a.majortype != b.majortype)
    return false;

  if (b.subtype != GUID_NULL && a.subtype != b.subtype)
    return false;

  if (b.formattype != GUID_NULL) {
    // if the format block is specified then it must match exactly
    if (a.formattype != b.formattype)
      return false;

    if (a.cbFormat != b.cbFormat)
      return false;

    if (a.cbFormat != 0 && memcmp(a.pbFormat, b.pbFormat, a.cbFormat) != 0)
      return false;
  }

  return true;
}

bool IsMediaTypeFullySpecified(const AM_MEDIA_TYPE& type) {
  return type.majortype != GUID_NULL && type.formattype != GUID_NULL;
}

BYTE* AllocMediaTypeFormatBuffer(AM_MEDIA_TYPE* media_type, ULONG length) {
  RTC_DCHECK(length);
  if (media_type->cbFormat == length)
    return media_type->pbFormat;

  BYTE* buffer = static_cast<BYTE*>(CoTaskMemAlloc(length));
  if (!buffer)
    return nullptr;

  if (media_type->pbFormat) {
    RTC_DCHECK(media_type->cbFormat);
    CoTaskMemFree(media_type->pbFormat);
    media_type->pbFormat = nullptr;
  }

  media_type->cbFormat = length;
  media_type->pbFormat = buffer;
  return buffer;
}

void GetSampleProperties(IMediaSample* sample, AM_SAMPLE2_PROPERTIES* props) {
  rtc::scoped_refptr<IMediaSample2> sample2;
  if (SUCCEEDED(GetComInterface(sample, &sample2))) {
    sample2->GetProperties(sizeof(*props), reinterpret_cast<BYTE*>(props));
    return;
  }

  //  Get the properties the hard way.
  props->cbData = sizeof(*props);
  props->dwTypeSpecificFlags = 0;
  props->dwStreamId = AM_STREAM_MEDIA;
  props->dwSampleFlags = 0;

  if (sample->IsDiscontinuity() == S_OK)
    props->dwSampleFlags |= AM_SAMPLE_DATADISCONTINUITY;
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (*SubType == MEDIASUBTYPE_MJPG &&
        pvi->bmiHeader.biCompression == MAKEFOURCC('M', 'J', 'P', 'G')) {
      _resultingCapability.videoType = VideoType::kMJPEG;
      return S_OK;  // This format is acceptable.
    }
    if (*SubType == MEDIASUBTYPE_I420 &&
        pvi->bmiHeader.biCompression == MAKEFOURCC('I', '4', '2', '0')) {
      _resultingCapability.videoType = VideoType::kI420;
      return S_OK;  // This format is acceptable.
    }
    if (*SubType == MEDIASUBTYPE_YUY2 &&
        pvi->bmiHeader.biCompression == MAKEFOURCC('Y', 'U', 'Y', '2')) {
      _resultingCapability.videoType = VideoType::kYUY2;
      ::Sleep(60);  // workaround for bad driver
      return S_OK;  // This format is acceptable.
    }
    if (*SubType == MEDIASUBTYPE_UYVY &&
        pvi->bmiHeader.biCompression == MAKEFOURCC('U', 'Y', 'V', 'Y')) {
      _resultingCapability.videoType = VideoType::kUYVY;
      return S_OK;  // This format is acceptable.
    }
||||||| merged common ancestors
    pmt->SetType(&MEDIATYPE_Video);
    pmt->SetFormatType(&FORMAT_VideoInfo);
    pmt->SetTemporalCompression(FALSE);
=======
  if (sample->IsPreroll() == S_OK)
    props->dwSampleFlags |= AM_SAMPLE_PREROLL;
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (*SubType == MEDIASUBTYPE_HDYC) {
      _resultingCapability.videoType = VideoType::kUYVY;
      return S_OK;  // This format is acceptable.
    }
    if (*SubType == MEDIASUBTYPE_RGB24 &&
        pvi->bmiHeader.biCompression == BI_RGB) {
      _resultingCapability.videoType = VideoType::kRGB24;
      return S_OK;  // This format is acceptable.
||||||| merged common ancestors
    int32_t positionOffset=1;
    if(_requestedCapability.codecType!=kVideoCodecUnknown)
    {
        positionOffset=0;
=======
  if (sample->IsSyncPoint() == S_OK)
    props->dwSampleFlags |= AM_SAMPLE_SPLICEPOINT;

  if (SUCCEEDED(sample->GetTime(&props->tStart, &props->tStop)))
    props->dwSampleFlags |= AM_SAMPLE_TIMEVALID | AM_SAMPLE_STOPVALID;

  if (sample->GetMediaType(&props->pMediaType) == S_OK)
    props->dwSampleFlags |= AM_SAMPLE_TYPECHANGED;

  sample->GetPointer(&props->pbBuffer);
  props->lActual = sample->GetActualDataLength();
  props->cbBuffer = sample->GetSize();
}

// Returns true if the media type is supported, false otherwise.
// For supported types, the |capability| will be populated accordingly.
bool TranslateMediaTypeToVideoCaptureCapability(
    const AM_MEDIA_TYPE* media_type,
    VideoCaptureCapability* capability) {
  RTC_DCHECK(capability);
  if (!media_type || media_type->majortype != MEDIATYPE_Video ||
      !media_type->pbFormat) {
    return false;
  }

  const BITMAPINFOHEADER* bih = nullptr;
  if (media_type->formattype == FORMAT_VideoInfo) {
    bih = &reinterpret_cast<VIDEOINFOHEADER*>(media_type->pbFormat)->bmiHeader;
  } else if (media_type->formattype != FORMAT_VideoInfo2) {
    bih = &reinterpret_cast<VIDEOINFOHEADER2*>(media_type->pbFormat)->bmiHeader;
  } else {
    return false;
  }

  RTC_LOG(LS_INFO) << "TranslateMediaTypeToVideoCaptureCapability width:"
                   << bih->biWidth << " height:" << bih->biHeight
                   << " Compression:0x" << rtc::ToHex(bih->biCompression);

  const GUID& sub_type = media_type->subtype;
  if (sub_type == MEDIASUBTYPE_MJPG &&
      bih->biCompression == MAKEFOURCC('M', 'J', 'P', 'G')) {
    capability->videoType = VideoType::kMJPEG;
  } else if (sub_type == MEDIASUBTYPE_I420 &&
             bih->biCompression == MAKEFOURCC('I', '4', '2', '0')) {
    capability->videoType = VideoType::kI420;
  } else if (sub_type == MEDIASUBTYPE_YUY2 &&
             bih->biCompression == MAKEFOURCC('Y', 'U', 'Y', '2')) {
    capability->videoType = VideoType::kYUY2;
  } else if (sub_type == MEDIASUBTYPE_UYVY &&
             bih->biCompression == MAKEFOURCC('U', 'Y', 'V', 'Y')) {
    capability->videoType = VideoType::kUYVY;
  } else if (sub_type == MEDIASUBTYPE_HDYC) {
    capability->videoType = VideoType::kUYVY;
  } else if (sub_type == MEDIASUBTYPE_RGB24 && bih->biCompression == BI_RGB) {
    capability->videoType = VideoType::kRGB24;
  } else {
    return false;
  }

  // Store the incoming width and height
  capability->width = bih->biWidth;

  // Store the incoming height,
  // for RGB24 we assume the frame to be upside down
  if (sub_type == MEDIASUBTYPE_RGB24 && bih->biHeight > 0) {
    capability->height = -(bih->biHeight);
  } else {
    capability->height = abs(bih->biHeight);
  }

  return true;
}

class MediaTypesEnum : public IEnumMediaTypes {
 public:
  MediaTypesEnum(const VideoCaptureCapability& capability)
      : capability_(capability),
        format_preference_order_(
            {// Default preferences, sorted by cost-to-convert-to-i420.
             VideoType::kI420, VideoType::kYUY2, VideoType::kRGB24,
             VideoType::kUYVY, VideoType::kMJPEG}) {
    // Use the preferred video type, if supported.
    auto it = std::find(format_preference_order_.begin(),
                        format_preference_order_.end(), capability_.videoType);
    if (it != format_preference_order_.end()) {
      RTC_LOG(LS_INFO) << "Selected video type: " << static_cast<int32_t>(*it);
      // Move it to the front of the list, if it isn't already there.
      if (it != format_preference_order_.begin()) {
        format_preference_order_.splice(format_preference_order_.begin(),
                                        format_preference_order_, it,
                                        std::next(it));
      }
    } else {
      RTC_LOG(LS_WARNING) << "Unsupported video type: " << static_cast<int32_t>(*it)
                          << ", using default preference list.";
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
  if (*formatType == FORMAT_VideoInfo2) {
    // VIDEOINFOHEADER2 that has dwInterlaceFlags
    VIDEOINFOHEADER2* pvi = (VIDEOINFOHEADER2*)pMediaType->Format();
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (pvi == NULL) {
      return E_INVALIDARG;
    }

    RTC_LOG(LS_INFO) << "CheckMediaType width:" << pvi->bmiHeader.biWidth
                     << " height:" << pvi->bmiHeader.biHeight
                     << " Compression:0x" << std::hex
                     << pvi->bmiHeader.biCompression;

    _resultingCapability.width = pvi->bmiHeader.biWidth;

    // Store the incoming height,
    // for RGB24 we assume the frame to be upside down
    if (*SubType == MEDIASUBTYPE_RGB24 && pvi->bmiHeader.biHeight > 0) {
      _resultingCapability.height = -(pvi->bmiHeader.biHeight);
    } else {
      _resultingCapability.height = abs(pvi->bmiHeader.biHeight);
    }

    if (*SubType == MEDIASUBTYPE_MJPG &&
        pvi->bmiHeader.biCompression == MAKEFOURCC('M', 'J', 'P', 'G')) {
      _resultingCapability.videoType = VideoType::kMJPEG;
      return S_OK;  // This format is acceptable.
    }
    if (*SubType == MEDIASUBTYPE_I420 &&
        pvi->bmiHeader.biCompression == MAKEFOURCC('I', '4', '2', '0')) {
      _resultingCapability.videoType = VideoType::kI420;
      return S_OK;  // This format is acceptable.
    }
    if (*SubType == MEDIASUBTYPE_YUY2 &&
        pvi->bmiHeader.biCompression == MAKEFOURCC('Y', 'U', 'Y', '2')) {
      _resultingCapability.videoType = VideoType::kYUY2;
      return S_OK;  // This format is acceptable.
    }
    if (*SubType == MEDIASUBTYPE_UYVY &&
        pvi->bmiHeader.biCompression == MAKEFOURCC('U', 'Y', 'V', 'Y')) {
      _resultingCapability.videoType = VideoType::kUYVY;
      return S_OK;  // This format is acceptable.
||||||| merged common ancestors
    switch (iPosition+positionOffset)
    {
        case 0:
        {
            pvi->bmiHeader.biCompression = MAKEFOURCC('I','4','2','0');
            pvi->bmiHeader.biBitCount = 12; //bit per pixel
            pvi->bmiHeader.biWidth = _requestedCapability.width;
            pvi->bmiHeader.biHeight = _requestedCapability.height;
            pvi->bmiHeader.biSizeImage = 3*_requestedCapability.height
                                        *_requestedCapability.width/2;
            pmt->SetSubtype(&MEDIASUBTYPE_I420);
        }
        break;
        case 1:
        {
            pvi->bmiHeader.biCompression = MAKEFOURCC('Y','U','Y','2');;
            pvi->bmiHeader.biBitCount = 16; //bit per pixel
            pvi->bmiHeader.biWidth = _requestedCapability.width;
            pvi->bmiHeader.biHeight = _requestedCapability.height;
            pvi->bmiHeader.biSizeImage = 2*_requestedCapability.width
                                        *_requestedCapability.height;
            pmt->SetSubtype(&MEDIASUBTYPE_YUY2);
        }
        break;
        case 2:
        {
            pvi->bmiHeader.biCompression = BI_RGB;
            pvi->bmiHeader.biBitCount = 24; //bit per pixel
            pvi->bmiHeader.biWidth = _requestedCapability.width;
            pvi->bmiHeader.biHeight = _requestedCapability.height;
            pvi->bmiHeader.biSizeImage = 3*_requestedCapability.height
                                        *_requestedCapability.width;
            pmt->SetSubtype(&MEDIASUBTYPE_RGB24);
        }
        break;
        case 3:
        {
            pvi->bmiHeader.biCompression = MAKEFOURCC('U','Y','V','Y');
            pvi->bmiHeader.biBitCount = 16; //bit per pixel
            pvi->bmiHeader.biWidth = _requestedCapability.width;
            pvi->bmiHeader.biHeight = _requestedCapability.height;
            pvi->bmiHeader.biSizeImage = 2*_requestedCapability.height
                                         *_requestedCapability.width;
            pmt->SetSubtype(&MEDIASUBTYPE_UYVY);
        }
        break;
        case 4:
        {
            pvi->bmiHeader.biCompression = MAKEFOURCC('M','J','P','G');
            pvi->bmiHeader.biBitCount = 12; //bit per pixel
            pvi->bmiHeader.biWidth = _requestedCapability.width;
            pvi->bmiHeader.biHeight = _requestedCapability.height;
            pvi->bmiHeader.biSizeImage = 3*_requestedCapability.height
                                         *_requestedCapability.width/2;
            pmt->SetSubtype(&MEDIASUBTYPE_MJPG);
        }
        break;
        default :
        return VFW_S_NO_MORE_ITEMS;
    }
    pmt->SetSampleSize(pvi->bmiHeader.biSizeImage);
    WEBRTC_TRACE(webrtc::kTraceInfo, webrtc::kTraceVideoCapture, _moduleId,
             "GetMediaType position %d, width %d, height %d, biCompression 0x%x",
             iPosition, _requestedCapability.width,
             _requestedCapability.height,pvi->bmiHeader.biCompression);
    return NOERROR;
}

HRESULT
CaptureInputPin::CheckMediaType ( IN const MediaType * pMediaType)
{
    // reset the thread handle
    _threadHandle = NULL;

    const GUID *type = pMediaType->Type();
    if (*type != MEDIATYPE_Video)
    return E_INVALIDARG;

    const GUID *formatType = pMediaType->FormatType();

    // Check for the subtypes we support
    const GUID *SubType = pMediaType->Subtype();
    if (SubType == NULL)
    {
        return E_INVALIDARG;
=======
 protected:
  virtual ~MediaTypesEnum() {}

 private:
  STDMETHOD(QueryInterface)(REFIID riid, void** ppv) override {
    if (riid == IID_IUnknown || riid == IID_IEnumMediaTypes) {
      *ppv = static_cast<IEnumMediaTypes*>(this);
      AddRef();
      return S_OK;
    }
    return E_NOINTERFACE;
  }

  // IEnumMediaTypes
  STDMETHOD(Clone)(IEnumMediaTypes** pins) {
    RTC_DCHECK(false);
    return E_NOTIMPL;
  }

  STDMETHOD(Next)(ULONG count, AM_MEDIA_TYPE** types, ULONG* fetched) {
    RTC_DCHECK(count > 0);
    RTC_DCHECK(types);
    // fetched may be NULL.
    if (fetched)
      *fetched = 0;

    for (ULONG i = 0;
         i < count && pos_ < static_cast<int>(format_preference_order_.size());
         ++i) {
      AM_MEDIA_TYPE* media_type = reinterpret_cast<AM_MEDIA_TYPE*>(
          CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE)));
      ZeroMemory(media_type, sizeof(*media_type));
      types[i] = media_type;
      VIDEOINFOHEADER* vih = reinterpret_cast<VIDEOINFOHEADER*>(
          AllocMediaTypeFormatBuffer(media_type, sizeof(VIDEOINFOHEADER)));
      ZeroMemory(vih, sizeof(*vih));
      vih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      vih->bmiHeader.biPlanes = 1;
      vih->bmiHeader.biClrImportant = 0;
      vih->bmiHeader.biClrUsed = 0;
      if (capability_.maxFPS != 0)
        vih->AvgTimePerFrame = 10000000 / capability_.maxFPS;

      SetRectEmpty(&vih->rcSource);  // we want the whole image area rendered.
      SetRectEmpty(&vih->rcTarget);  // no particular destination rectangle

      media_type->majortype = MEDIATYPE_Video;
      media_type->formattype = FORMAT_VideoInfo;
      media_type->bTemporalCompression = FALSE;

      // Set format information.
      auto format_it = std::next(format_preference_order_.begin(), pos_++);
      SetMediaInfoFromVideoType(*format_it, &vih->bmiHeader, media_type);

      vih->bmiHeader.biWidth = capability_.width;
      vih->bmiHeader.biHeight = capability_.height;
      vih->bmiHeader.biSizeImage = ((vih->bmiHeader.biBitCount / 4) *
                                    capability_.height * capability_.width) /
                                   2;

      RTC_DCHECK(vih->bmiHeader.biSizeImage);
      media_type->lSampleSize = vih->bmiHeader.biSizeImage;
      media_type->bFixedSizeSamples = true;
      if (fetched)
        ++(*fetched);
    }
    return pos_ == static_cast<int>(format_preference_order_.size()) ? S_FALSE
                                                                     : S_OK;
  }

  static void SetMediaInfoFromVideoType(VideoType video_type,
                                        BITMAPINFOHEADER* bitmap_header,
                                        AM_MEDIA_TYPE* media_type) {
    switch (video_type) {
      case VideoType::kI420:
        bitmap_header->biCompression = MAKEFOURCC('I', '4', '2', '0');
        bitmap_header->biBitCount = 12;  // bit per pixel
        media_type->subtype = MEDIASUBTYPE_I420;
        break;
      case VideoType::kYUY2:
        bitmap_header->biCompression = MAKEFOURCC('Y', 'U', 'Y', '2');
        bitmap_header->biBitCount = 16;  // bit per pixel
        media_type->subtype = MEDIASUBTYPE_YUY2;
        break;
      case VideoType::kRGB24:
        bitmap_header->biCompression = BI_RGB;
        bitmap_header->biBitCount = 24;  // bit per pixel
        media_type->subtype = MEDIASUBTYPE_RGB24;
        break;
      case VideoType::kUYVY:
        bitmap_header->biCompression = MAKEFOURCC('U', 'Y', 'V', 'Y');
        bitmap_header->biBitCount = 16;  // bit per pixel
        media_type->subtype = MEDIASUBTYPE_UYVY;
        break;
      case VideoType::kMJPEG:
        bitmap_header->biCompression = MAKEFOURCC('M', 'J', 'P', 'G');
        bitmap_header->biBitCount = 12;  // bit per pixel
        media_type->subtype = MEDIASUBTYPE_MJPG;
        break;
      default:
        RTC_NOTREACHED();
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
    if (*SubType == MEDIASUBTYPE_HDYC) {
      _resultingCapability.videoType = VideoType::kUYVY;
      return S_OK;  // This format is acceptable.
    }
    if (*SubType == MEDIASUBTYPE_RGB24 &&
        pvi->bmiHeader.biCompression == BI_RGB) {
      _resultingCapability.videoType = VideoType::kRGB24;
      return S_OK;  // This format is acceptable.
    }
  }
  return E_INVALIDARG;
}

HRESULT
CaptureInputPin::Receive(IN IMediaSample* pIMediaSample) {
  HRESULT hr = S_OK;

  RTC_DCHECK(mFilter);
  RTC_DCHECK(pIMediaSample);

  // get the thread handle of the delivering thread inc its priority
  if (_threadHandle == NULL) {
    HANDLE handle = GetCurrentThread();
    SetThreadPriority(handle, THREAD_PRIORITY_HIGHEST);
    _threadHandle = handle;
    // See http://msdn.microsoft.com/en-us/library/xcb2z8hs(VS.71).aspx for details on the code
    // in this function. Name of article is "Setting a Thread Name (Unmanaged)".

    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = "capture_thread";
    info.dwThreadID = (DWORD)-1;
    info.dwFlags = 0;

    __try
    {
        RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD),
                        (DWORD_PTR*)&info );
    }
    __except (EXCEPTION_CONTINUE_EXECUTION)
    {
||||||| merged common ancestors
    if(*formatType == FORMAT_VideoInfo)
    {
        VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *) pMediaType->Format();
        if(pvi == NULL)
        {
            return E_INVALIDARG;
        }

        // Store the incoming width and height
        _resultingCapability.width = pvi->bmiHeader.biWidth;

        // Store the incoming height,
        // for RGB24 we assume the frame to be upside down
        if(*SubType == MEDIASUBTYPE_RGB24
            && pvi->bmiHeader.biHeight > 0)
        {
           _resultingCapability.height = -(pvi->bmiHeader.biHeight);
        }
        else
        {
           _resultingCapability.height = abs(pvi->bmiHeader.biHeight);
        }

        WEBRTC_TRACE(webrtc::kTraceInfo, webrtc::kTraceVideoCapture, _moduleId,
                     "CheckMediaType width:%d height:%d Compression:0x%x",
                     pvi->bmiHeader.biWidth,pvi->bmiHeader.biHeight,
                     pvi->bmiHeader.biCompression);

        if(*SubType == MEDIASUBTYPE_MJPG
            && pvi->bmiHeader.biCompression == MAKEFOURCC('M','J','P','G'))
        {
            _resultingCapability.rawType = kVideoMJPEG;
            return S_OK; // This format is acceptable.
        }
        if(*SubType == MEDIASUBTYPE_I420
            && pvi->bmiHeader.biCompression == MAKEFOURCC('I','4','2','0'))
        {
            _resultingCapability.rawType = kVideoI420;
            return S_OK; // This format is acceptable.
        }
        if(*SubType == MEDIASUBTYPE_YUY2
            && pvi->bmiHeader.biCompression == MAKEFOURCC('Y','U','Y','2'))
        {
            _resultingCapability.rawType = kVideoYUY2;
            ::Sleep(60); // workaround for bad driver
            return S_OK; // This format is acceptable.
        }
        if(*SubType == MEDIASUBTYPE_UYVY
            && pvi->bmiHeader.biCompression == MAKEFOURCC('U','Y','V','Y'))
        {
            _resultingCapability.rawType = kVideoUYVY;
            return S_OK; // This format is acceptable.
        }

        if(*SubType == MEDIASUBTYPE_HDYC)
        {
            _resultingCapability.rawType = kVideoUYVY;
            return S_OK; // This format is acceptable.
        }
        if(*SubType == MEDIASUBTYPE_RGB24
            && pvi->bmiHeader.biCompression == BI_RGB)
        {
            _resultingCapability.rawType = kVideoRGB24;
            return S_OK; // This format is acceptable.
        }
    }
    if(*formatType == FORMAT_VideoInfo2)
    {
        // VIDEOINFOHEADER2 that has dwInterlaceFlags
        VIDEOINFOHEADER2 *pvi = (VIDEOINFOHEADER2 *) pMediaType->Format();

        if(pvi == NULL)
        {
            return E_INVALIDARG;
        }

        WEBRTC_TRACE(webrtc::kTraceInfo, webrtc::kTraceVideoCapture, _moduleId,
                     "CheckMediaType width:%d height:%d Compression:0x%x",
                     pvi->bmiHeader.biWidth,pvi->bmiHeader.biHeight,
                     pvi->bmiHeader.biCompression);

        _resultingCapability.width = pvi->bmiHeader.biWidth;

        // Store the incoming height,
        // for RGB24 we assume the frame to be upside down
        if(*SubType == MEDIASUBTYPE_RGB24
            && pvi->bmiHeader.biHeight > 0)
        {
           _resultingCapability.height = -(pvi->bmiHeader.biHeight);
        }
        else
        {
           _resultingCapability.height = abs(pvi->bmiHeader.biHeight);
        }

        if(*SubType == MEDIASUBTYPE_MJPG
            && pvi->bmiHeader.biCompression == MAKEFOURCC('M','J','P','G'))
        {
            _resultingCapability.rawType = kVideoMJPEG;
            return S_OK; // This format is acceptable.
        }
        if(*SubType == MEDIASUBTYPE_I420
            && pvi->bmiHeader.biCompression == MAKEFOURCC('I','4','2','0'))
        {
            _resultingCapability.rawType = kVideoI420;
            return S_OK; // This format is acceptable.
        }
        if(*SubType == MEDIASUBTYPE_YUY2
            && pvi->bmiHeader.biCompression == MAKEFOURCC('Y','U','Y','2'))
        {
            _resultingCapability.rawType = kVideoYUY2;
            return S_OK; // This format is acceptable.
        }
        if(*SubType == MEDIASUBTYPE_UYVY
            && pvi->bmiHeader.biCompression == MAKEFOURCC('U','Y','V','Y'))
        {
            _resultingCapability.rawType = kVideoUYVY;
            return S_OK; // This format is acceptable.
        }

        if(*SubType == MEDIASUBTYPE_HDYC)
        {
            _resultingCapability.rawType = kVideoUYVY;
            return S_OK; // This format is acceptable.
        }
        if(*SubType == MEDIASUBTYPE_RGB24
            && pvi->bmiHeader.biCompression == BI_RGB)
        {
            _resultingCapability.rawType = kVideoRGB24;
            return S_OK; // This format is acceptable.
        }
    }
    return E_INVALIDARG;
}

HRESULT
CaptureInputPin::Receive ( IN IMediaSample * pIMediaSample )
{
    HRESULT hr = S_OK;

    assert (mFilter);
    assert (pIMediaSample);

    // get the thread handle of the delivering thread inc its priority
    if( _threadHandle == NULL)
    {
        HANDLE handle= GetCurrentThread();
        SetThreadPriority(handle, THREAD_PRIORITY_HIGHEST);
        _threadHandle = handle;
        // See http://msdn.microsoft.com/en-us/library/xcb2z8hs(VS.71).aspx for details on the code
        // in this function. Name of article is "Setting a Thread Name (Unmanaged)".

        THREADNAME_INFO info;
        info.dwType = 0x1000;
        info.szName = "capture_thread";
        info.dwThreadID = (DWORD)-1;
        info.dwFlags = 0;

        __try
        {
            RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD),
                            (DWORD_PTR*)&info );
        }
        __except (EXCEPTION_CONTINUE_EXECUTION)
        {
        }

    }

    reinterpret_cast <CaptureSinkFilter *>(mFilter)->LockReceive();
    hr = BaseInputPin::Receive (pIMediaSample);

    if (SUCCEEDED (hr))
    {
        const int32_t length = pIMediaSample->GetActualDataLength();

        unsigned char* pBuffer = NULL;
        if(S_OK != pIMediaSample->GetPointer(&pBuffer))
        {
            reinterpret_cast <CaptureSinkFilter *>(mFilter)->UnlockReceive();
            return S_FALSE;
        }

        // NOTE: filter unlocked within Send call
        reinterpret_cast <CaptureSinkFilter *> (mFilter)->ProcessCapturedFrame(
                                        pBuffer,length,_resultingCapability);
=======
  STDMETHOD(Skip)(ULONG count) {
    RTC_DCHECK(false);
    return E_NOTIMPL;
  }

  STDMETHOD(Reset)() {
    pos_ = 0;
    return S_OK;
  }

  int pos_ = 0;
  const VideoCaptureCapability capability_;
  std::list<VideoType> format_preference_order_;
};

}  // namespace

CaptureInputPin::CaptureInputPin(CaptureSinkFilter* filter) {
  capture_checker_.DetachFromThread();
  // No reference held to avoid circular references.
  info_.pFilter = filter;
  info_.dir = PINDIR_INPUT;
}

CaptureInputPin::~CaptureInputPin() {
  RTC_DCHECK_RUN_ON(&main_checker_);
  ResetMediaType(&media_type_);
}

HRESULT CaptureInputPin::SetRequestedCapability(
    const VideoCaptureCapability& capability) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  RTC_DCHECK(Filter()->IsStopped());
  requested_capability_ = capability;
  resulting_capability_ = VideoCaptureCapability();
  return S_OK;
}

void CaptureInputPin::OnFilterActivated() {
  RTC_DCHECK_RUN_ON(&main_checker_);
  runtime_error_ = false;
  flushing_ = false;
  capture_checker_.DetachFromThread();
  capture_thread_id_ = 0;
}

void CaptureInputPin::OnFilterDeactivated() {
  RTC_DCHECK_RUN_ON(&main_checker_);
  // Expedite shutdown by raising the flushing flag so no further processing
  // on the capture thread occurs. When the graph is stopped and all filters
  // have been told to stop, the media controller (graph) will wait for the
  // capture thread to stop.
  flushing_ = true;
  if (allocator_)
    allocator_->Decommit();
}

CaptureSinkFilter* CaptureInputPin::Filter() const {
  return static_cast<CaptureSinkFilter*>(info_.pFilter);
}

HRESULT CaptureInputPin::AttemptConnection(IPin* receive_pin,
                                           const AM_MEDIA_TYPE* media_type) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  RTC_DCHECK(Filter()->IsStopped());

  // Check that the connection is valid  -- need to do this for every
  // connect attempt since BreakConnect will undo it.
  HRESULT hr = CheckDirection(receive_pin);
  if (FAILED(hr))
    return hr;

  if (!TranslateMediaTypeToVideoCaptureCapability(media_type,
                                                  &resulting_capability_)) {
    ClearAllocator(true);
    return VFW_E_TYPE_NOT_ACCEPTED;
  }

  // See if the other pin will accept this type.
  hr = receive_pin->ReceiveConnection(static_cast<IPin*>(this), media_type);
  if (FAILED(hr)) {
    receive_pin_ = nullptr;  // Should already be null, but just in case.
    return hr;
  }

  // Should have been set as part of the connect process.
  RTC_DCHECK_EQ(receive_pin_, receive_pin);

  ResetMediaType(&media_type_);
  CopyMediaType(&media_type_, media_type);

  return S_OK;
}

std::vector<AM_MEDIA_TYPE*> CaptureInputPin::DetermineCandidateFormats(
    IPin* receive_pin,
    const AM_MEDIA_TYPE* media_type) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  RTC_DCHECK(receive_pin);
  RTC_DCHECK(media_type);

  std::vector<AM_MEDIA_TYPE*> ret;

  for (int i = 0; i < 2; i++) {
    IEnumMediaTypes* types = nullptr;
    if (i == 0) {
      // First time around, try types from receive_pin.
      receive_pin->EnumMediaTypes(&types);
    } else {
      // Then try ours.
      EnumMediaTypes(&types);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }

  reinterpret_cast<CaptureSinkFilter*>(mFilter)->LockReceive();
  hr = BaseInputPin::Receive(pIMediaSample);

  if (SUCCEEDED(hr)) {
    const int32_t length = pIMediaSample->GetActualDataLength();

    unsigned char* pBuffer = NULL;
    if (S_OK != pIMediaSample->GetPointer(&pBuffer)) {
      reinterpret_cast <CaptureSinkFilter *>(mFilter)->UnlockReceive();
      return S_FALSE;
||||||| merged common ancestors
    else
    {
        reinterpret_cast <CaptureSinkFilter *>(mFilter)->UnlockReceive();
=======

    if (types) {
      while (true) {
        ULONG fetched = 0;
        AM_MEDIA_TYPE* this_type = nullptr;
        if (types->Next(1, &this_type, &fetched) != S_OK)
          break;

        if (IsMediaTypePartialMatch(*this_type, *media_type)) {
          ret.push_back(this_type);
        } else {
          FreeMediaType(this_type);
        }
      }
      types->Release();
>>>>>>> upstream-releases
    }
  }

  return ret;
}

void CaptureInputPin::ClearAllocator(bool decommit) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  if (!allocator_)
    return;
  if (decommit)
    allocator_->Decommit();
  allocator_ = nullptr;
}

HRESULT CaptureInputPin::CheckDirection(IPin* pin) const {
  RTC_DCHECK_RUN_ON(&main_checker_);
  PIN_DIRECTION pd;
  pin->QueryDirection(&pd);
  // Fairly basic check, make sure we don't pair input with input etc.
  return pd == info_.dir ? VFW_E_INVALID_DIRECTION : S_OK;
}

STDMETHODIMP CaptureInputPin::QueryInterface(REFIID riid, void** ppv) {
  (*ppv) = nullptr;
  if (riid == IID_IUnknown || riid == IID_IMemInputPin) {
    *ppv = static_cast<IMemInputPin*>(this);
  } else if (riid == IID_IPin) {
    *ppv = static_cast<IPin*>(this);
  }

  if (!(*ppv))
    return E_NOINTERFACE;

  static_cast<IMemInputPin*>(this)->AddRef();
  return S_OK;
}

STDMETHODIMP CaptureInputPin::Connect(IPin* receive_pin,
                                      const AM_MEDIA_TYPE* media_type) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  if (!media_type || !receive_pin)
    return E_POINTER;

  if (!Filter()->IsStopped())
    return VFW_E_NOT_STOPPED;

  if (receive_pin_) {
    RTC_DCHECK(false);
    return VFW_E_ALREADY_CONNECTED;
  }

  if (IsMediaTypeFullySpecified(*media_type))
    return AttemptConnection(receive_pin, media_type);

<<<<<<< HEAD
    // NOTE: filter unlocked within Send call
    reinterpret_cast <CaptureSinkFilter *> (mFilter)->ProcessCapturedFrame(
        pBuffer,length,_resultingCapability);
  } else {
    reinterpret_cast<CaptureSinkFilter*>(mFilter)->UnlockReceive();
  }

  return hr;
||||||| merged common ancestors
    return hr;
=======
  auto types = DetermineCandidateFormats(receive_pin, media_type);
  bool connected = false;
  for (auto* type : types) {
    if (!connected && AttemptConnection(receive_pin, media_type) == S_OK)
      connected = true;

    FreeMediaType(type);
  }

  return connected ? S_OK : VFW_E_NO_ACCEPTABLE_TYPES;
}

STDMETHODIMP CaptureInputPin::ReceiveConnection(
    IPin* connector,
    const AM_MEDIA_TYPE* media_type) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  RTC_DCHECK(Filter()->IsStopped());

  if (receive_pin_) {
    RTC_DCHECK(false);
    return VFW_E_ALREADY_CONNECTED;
  }

  HRESULT hr = CheckDirection(connector);
  if (FAILED(hr))
    return hr;

  if (!TranslateMediaTypeToVideoCaptureCapability(media_type,
                                                  &resulting_capability_))
    return VFW_E_TYPE_NOT_ACCEPTED;

  // Complete the connection

  receive_pin_ = connector;
  ResetMediaType(&media_type_);
  CopyMediaType(&media_type_, media_type);

  return S_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// called under LockReceive
HRESULT CaptureInputPin::SetMatchingMediaType(
    const VideoCaptureCapability& capability) {
  _requestedCapability = capability;
  _resultingCapability = VideoCaptureCapability();
  return S_OK;
||||||| merged common ancestors
// called under LockReceive
HRESULT CaptureInputPin::SetMatchingMediaType(
                                    const VideoCaptureCapability& capability)
{

    _requestedCapability = capability;
    _resultingCapability = VideoCaptureCapability();
    return S_OK;
=======
STDMETHODIMP CaptureInputPin::Disconnect() {
  RTC_DCHECK_RUN_ON(&main_checker_);
  if (!Filter()->IsStopped())
    return VFW_E_NOT_STOPPED;

  if (!receive_pin_)
    return S_FALSE;

  ClearAllocator(true);
  receive_pin_ = nullptr;

  return S_OK;
>>>>>>> upstream-releases
}
<<<<<<< HEAD
//  ----------------------------------------------------------------------------
CaptureSinkFilter::CaptureSinkFilter(IN TCHAR* tszName,
                                     IN LPUNKNOWN punk,
                                     OUT HRESULT* phr,
                              VideoCaptureExternal& captureObserver,
                              int32_t moduleId)
    : BaseFilter(tszName, CLSID_SINKFILTER),
      m_crtFilter("CaptureSinkFilter::m_crtFilter"),
      m_crtRecv("CaptureSinkFilter::m_crtRecv"),
      m_pInput(NULL),
      _captureObserver(captureObserver),
      _moduleId(moduleId) {
  (*phr) = S_OK;
  m_pInput = new CaptureInputPin(moduleId, L"VideoCaptureInputPin",
                                 this,
                                 &m_crtFilter, phr, L"VideoCapture");
  if (m_pInput == NULL || FAILED(*phr)) {
    (*phr) = FAILED(*phr) ? (*phr) : E_OUTOFMEMORY;
    goto cleanup;
  }
cleanup:
  return;
||||||| merged common ancestors
//  ----------------------------------------------------------------------------
CaptureSinkFilter::CaptureSinkFilter (IN TCHAR * tszName,
                              IN LPUNKNOWN punk,
                              OUT HRESULT * phr,
                              VideoCaptureExternal& captureObserver,
                              int32_t moduleId)
    : BaseFilter(tszName, CLSID_SINKFILTER),
      m_crtFilter("CaptureSinkFilter::m_crtFilter"),
      m_crtRecv("CaptureSinkFilter::m_crtRecv"),
      m_pInput(NULL),
      _captureObserver(captureObserver),
      _moduleId(moduleId)
{
    (* phr) = S_OK;
    m_pInput = new CaptureInputPin(moduleId, L"VideoCaptureInputPin",
                                   this,
                                   & m_crtFilter,
                                   phr, L"VideoCapture");
    if (m_pInput == NULL || FAILED (* phr))
    {
        (* phr) = FAILED (* phr) ? (* phr) : E_OUTOFMEMORY;
        goto cleanup;
    }
    cleanup :
    return;
=======

STDMETHODIMP CaptureInputPin::ConnectedTo(IPin** pin) {
  RTC_DCHECK_RUN_ON(&main_checker_);

  if (!receive_pin_)
    return VFW_E_NOT_CONNECTED;

  *pin = receive_pin_.get();
  receive_pin_->AddRef();

  return S_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
CaptureSinkFilter::~CaptureSinkFilter() {
  delete m_pInput;
||||||| merged common ancestors
CaptureSinkFilter::~CaptureSinkFilter()
{
    delete m_pInput;
=======
STDMETHODIMP CaptureInputPin::ConnectionMediaType(AM_MEDIA_TYPE* media_type) {
  RTC_DCHECK_RUN_ON(&main_checker_);

  if (!receive_pin_)
    return VFW_E_NOT_CONNECTED;

  CopyMediaType(media_type, &media_type_);

  return S_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
int CaptureSinkFilter::GetPinCount() {
  return 1;
||||||| merged common ancestors
int CaptureSinkFilter::GetPinCount()
{
    return 1;
=======
STDMETHODIMP CaptureInputPin::QueryPinInfo(PIN_INFO* info) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  *info = info_;
  if (info_.pFilter)
    info_.pFilter->AddRef();
  return S_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
BasePin* CaptureSinkFilter::GetPin(IN int Index) {
  BasePin* pPin;
  LockFilter();
  if (Index == 0) {
    pPin = m_pInput;
  } else {
    pPin = NULL;
  }
  UnlockFilter();
  return pPin;
}

STDMETHODIMP CaptureSinkFilter::Pause() {
  LockReceive();
  LockFilter();
  if (mState == State_Stopped) {
    //  change the state, THEN activate the input pin
    mState = State_Paused;
    if (m_pInput && m_pInput->IsConnected()) {
      m_pInput->Active();
    }
    if (m_pInput && !m_pInput->IsConnected()) {
      mState = State_Running;
||||||| merged common ancestors
BasePin *
CaptureSinkFilter::GetPin(IN int Index)
{
    BasePin * pPin;
    LockFilter ();
    if (Index == 0)
    {
        pPin = m_pInput;
    }
    else
    {
        pPin = NULL;
    }
    UnlockFilter ();
    return pPin;
}

STDMETHODIMP CaptureSinkFilter::Pause()
{
    LockReceive();
    LockFilter();
    if (mState == State_Stopped)
    {
        //  change the state, THEN activate the input pin
        mState = State_Paused;
        if (m_pInput && m_pInput->IsConnected())
        {
            m_pInput->Active();
        }
        if (m_pInput && !m_pInput->IsConnected())
        {
            mState = State_Running;
        }
    }
    else if (mState == State_Running)
    {
        mState = State_Paused;
=======
STDMETHODIMP CaptureInputPin::QueryDirection(PIN_DIRECTION* pin_dir) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  *pin_dir = info_.dir;
  return S_OK;
}

STDMETHODIMP CaptureInputPin::QueryId(LPWSTR* id) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  size_t len = lstrlenW(info_.achName);
  *id = reinterpret_cast<LPWSTR>(CoTaskMemAlloc((len + 1) * sizeof(wchar_t)));
  lstrcpyW(*id, info_.achName);
  return S_OK;
}

STDMETHODIMP CaptureInputPin::QueryAccept(const AM_MEDIA_TYPE* media_type) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  RTC_DCHECK(Filter()->IsStopped());
  VideoCaptureCapability capability(resulting_capability_);
  return TranslateMediaTypeToVideoCaptureCapability(media_type, &capability)
             ? S_FALSE
             : S_OK;
}

STDMETHODIMP CaptureInputPin::EnumMediaTypes(IEnumMediaTypes** types) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  *types = new ComRefCount<MediaTypesEnum>(requested_capability_);
  (*types)->AddRef();
  return S_OK;
}

STDMETHODIMP CaptureInputPin::QueryInternalConnections(IPin** pins,
                                                       ULONG* count) {
  return E_NOTIMPL;
}

STDMETHODIMP CaptureInputPin::EndOfStream() {
  return S_OK;
}

STDMETHODIMP CaptureInputPin::BeginFlush() {
  RTC_DCHECK_RUN_ON(&main_checker_);
  flushing_ = true;
  return S_OK;
}

STDMETHODIMP CaptureInputPin::EndFlush() {
  RTC_DCHECK_RUN_ON(&main_checker_);
  flushing_ = false;
  runtime_error_ = false;
  return S_OK;
}

STDMETHODIMP CaptureInputPin::NewSegment(REFERENCE_TIME start,
                                         REFERENCE_TIME stop,
                                         double rate) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  return S_OK;
}

STDMETHODIMP CaptureInputPin::GetAllocator(IMemAllocator** allocator) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  if (allocator_ == nullptr) {
    HRESULT hr = CoCreateInstance(CLSID_MemoryAllocator, 0,
                                  CLSCTX_INPROC_SERVER, IID_IMemAllocator,
                                  reinterpret_cast<void**>(allocator));
    if (FAILED(hr))
      return hr;
    allocator_.swap(allocator);
  }
  *allocator = allocator_;
  allocator_->AddRef();
  return S_OK;
}

STDMETHODIMP CaptureInputPin::NotifyAllocator(IMemAllocator* allocator,
                                              BOOL read_only) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  allocator_.swap(&allocator);
  if (allocator_)
    allocator_->AddRef();
  if (allocator)
    allocator->Release();
  return S_OK;
}

STDMETHODIMP CaptureInputPin::GetAllocatorRequirements(
    ALLOCATOR_PROPERTIES* props) {
  return E_NOTIMPL;
}

STDMETHODIMP CaptureInputPin::Receive(IMediaSample* media_sample) {
  RTC_DCHECK_RUN_ON(&capture_checker_);

  CaptureSinkFilter* const filter = static_cast<CaptureSinkFilter*>(Filter());

  if (flushing_.load(std::memory_order_relaxed))
    return S_FALSE;

  if (runtime_error_.load(std::memory_order_relaxed))
    return VFW_E_RUNTIME_ERROR;

  if (!capture_thread_id_) {
    // Make sure we set the thread name only once.
    capture_thread_id_ = GetCurrentThreadId();
    rtc::SetCurrentThreadName("webrtc_video_capture");
  }

  AM_SAMPLE2_PROPERTIES sample_props = {};
  GetSampleProperties(media_sample, &sample_props);
  // Has the format changed in this sample?
  if (sample_props.dwSampleFlags & AM_SAMPLE_TYPECHANGED) {
    // Check the derived class accepts the new format.
    // This shouldn't fail as the source must call QueryAccept first.

    // Note: This will modify resulting_capability_.
    // That should be OK as long as resulting_capability_ is only modified
    // on this thread while it is running (filter is not stopped), and only
    // modified on the main thread when the filter is stopped (i.e. this thread
    // is not running).
    if (!TranslateMediaTypeToVideoCaptureCapability(sample_props.pMediaType,
                                                    &resulting_capability_)) {
      // Raise a runtime error if we fail the media type
      runtime_error_ = true;
      EndOfStream();
      Filter()->NotifyEvent(EC_ERRORABORT, VFW_E_TYPE_NOT_ACCEPTED, 0);
      return VFW_E_INVALIDMEDIATYPE;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  } else if (mState == State_Running) {
    mState = State_Paused;
  }
  UnlockFilter();
  UnlockReceive();
  return S_OK;
||||||| merged common ancestors
    UnlockFilter();
    UnlockReceive();
    return S_OK;
=======
  }

  filter->ProcessCapturedFrame(sample_props.pbBuffer, sample_props.lActual,
                               resulting_capability_);

  return S_OK;
}

STDMETHODIMP CaptureInputPin::ReceiveMultiple(IMediaSample** samples,
                                              long count,
                                              long* processed) {
  HRESULT hr = S_OK;
  *processed = 0;
  while (count-- > 0) {
    hr = Receive(samples[*processed]);
    if (hr != S_OK)
      break;
    ++(*processed);
  }
  return hr;
}

STDMETHODIMP CaptureInputPin::ReceiveCanBlock() {
  return S_FALSE;
}

//  ----------------------------------------------------------------------------

CaptureSinkFilter::CaptureSinkFilter(VideoCaptureImpl* capture_observer)
    : input_pin_(new ComRefCount<CaptureInputPin>(this)),
      capture_observer_(capture_observer) {}

CaptureSinkFilter::~CaptureSinkFilter() {
  RTC_DCHECK_RUN_ON(&main_checker_);
}

HRESULT CaptureSinkFilter::SetRequestedCapability(
    const VideoCaptureCapability& capability) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  // Called on the same thread as capture is started on.
  return input_pin_->SetRequestedCapability(capability);
}

STDMETHODIMP CaptureSinkFilter::GetState(DWORD msecs, FILTER_STATE* state) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  *state = state_;
  return S_OK;
}

STDMETHODIMP CaptureSinkFilter::SetSyncSource(IReferenceClock* clock) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  return S_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
STDMETHODIMP CaptureSinkFilter::Stop() {
  LockReceive();
  LockFilter();
||||||| merged common ancestors
STDMETHODIMP CaptureSinkFilter::Stop()
{
    LockReceive();
    LockFilter();
=======
STDMETHODIMP CaptureSinkFilter::GetSyncSource(IReferenceClock** clock) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  return E_NOTIMPL;
}

STDMETHODIMP CaptureSinkFilter::Pause() {
  RTC_DCHECK_RUN_ON(&main_checker_);
  state_ = State_Paused;
  return S_OK;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  //  set the state
  mState = State_Stopped;
||||||| merged common ancestors
    //  set the state
    mState = State_Stopped;
=======
STDMETHODIMP CaptureSinkFilter::Run(REFERENCE_TIME tStart) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  if (state_ == State_Stopped)
    Pause();
>>>>>>> upstream-releases

<<<<<<< HEAD
  //  inactivate the pins
  if (m_pInput)
    m_pInput->Inactive();
||||||| merged common ancestors
    //  inactivate the pins
    if (m_pInput)
        m_pInput->Inactive();
=======
  state_ = State_Running;
  input_pin_->OnFilterActivated();
>>>>>>> upstream-releases

<<<<<<< HEAD
  UnlockFilter();
  UnlockReceive();
  return S_OK;
||||||| merged common ancestors
    UnlockFilter();
    UnlockReceive();
    return S_OK;
=======
  return S_OK;
}

STDMETHODIMP CaptureSinkFilter::Stop() {
  RTC_DCHECK_RUN_ON(&main_checker_);
  if (state_ == State_Stopped)
    return S_OK;

  state_ = State_Stopped;
  input_pin_->OnFilterDeactivated();

  return S_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void CaptureSinkFilter::SetFilterGraph(IGraphBuilder* graph) {
  LockFilter();
  mGraph = graph;
  UnlockFilter();
||||||| merged common ancestors
void CaptureSinkFilter::SetFilterGraph(IGraphBuilder* graph)
{
    LockFilter();
    mGraph = graph;
    UnlockFilter();
=======
STDMETHODIMP CaptureSinkFilter::EnumPins(IEnumPins** pins) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  *pins = new ComRefCount<class EnumPins>(input_pin_.get());
  (*pins)->AddRef();
  return S_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void CaptureSinkFilter::ProcessCapturedFrame(
    unsigned char* pBuffer,
    int32_t length,
    const VideoCaptureCapability& frameInfo) {
  //  we have the receiver lock
  if (mState == State_Running) {
    _captureObserver.IncomingFrame(pBuffer, length, frameInfo);
||||||| merged common ancestors
void CaptureSinkFilter::ProcessCapturedFrame(unsigned char* pBuffer,
                                         int32_t length,
                                         const VideoCaptureCapability& frameInfo)
{
    //  we have the receiver lock
    if (mState == State_Running)
    {
        _captureObserver.IncomingFrame(pBuffer, length, frameInfo);
=======
STDMETHODIMP CaptureSinkFilter::FindPin(LPCWSTR id, IPin** pin) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  // There's no ID assigned to our input pin, so looking it up based on one
  // is pointless (and in practice, this method isn't being used).
  return VFW_E_NOT_FOUND;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    // trying to hold it since it's only a memcpy
    // IMPROVEMENT if this work move critsect
    UnlockReceive();
||||||| merged common ancestors
        // trying to hold it since it's only a memcpy
        // IMPROVEMENT if this work move critsect
        UnlockReceive();
        return;
    }
    UnlockReceive();
=======
STDMETHODIMP CaptureSinkFilter::QueryFilterInfo(FILTER_INFO* info) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  *info = info_;
  if (info->pGraph)
    info->pGraph->AddRef();
  return S_OK;
}

STDMETHODIMP CaptureSinkFilter::JoinFilterGraph(IFilterGraph* graph,
                                                LPCWSTR name) {
  RTC_DCHECK_RUN_ON(&main_checker_);
  RTC_DCHECK(IsStopped());

  // Note, since a reference to the filter is held by the graph manager,
  // filters must not hold a reference to the graph. If they would, we'd have
  // a circular reference. Instead, a pointer to the graph can be held without
  // reference. See documentation for IBaseFilter::JoinFilterGraph for more.
  info_.pGraph = graph;  // No AddRef().
  sink_ = nullptr;

  if (info_.pGraph) {
    // make sure we don't hold on to the reference we may receive.
    // Note that this assumes the same object identity, but so be it.
    rtc::scoped_refptr<IMediaEventSink> sink;
    GetComInterface(info_.pGraph, &sink);
    sink_ = sink.get();
  }

  info_.achName[0] = L'\0';
  if (name)
    lstrcpynW(info_.achName, name, arraysize(info_.achName));

  return S_OK;
}

STDMETHODIMP CaptureSinkFilter::QueryVendorInfo(LPWSTR* vendor_info) {
  return E_NOTIMPL;
}

void CaptureSinkFilter::ProcessCapturedFrame(
    unsigned char* buffer,
    size_t length,
    const VideoCaptureCapability& frame_info) {
  // Called on the capture thread.
  capture_observer_->IncomingFrame(buffer, length, frame_info);
}

void CaptureSinkFilter::NotifyEvent(long code,
                                    LONG_PTR param1,
                                    LONG_PTR param2) {
  // Called on the capture thread.
  if (!sink_)
>>>>>>> upstream-releases
    return;
<<<<<<< HEAD
  }
  UnlockReceive();
  return;
||||||| merged common ancestors
=======

  if (EC_COMPLETE == code)
    param2 = reinterpret_cast<LONG_PTR>(static_cast<IBaseFilter*>(this));
  sink_->Notify(code, param1, param2);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
STDMETHODIMP CaptureSinkFilter::SetMatchingMediaType(
    const VideoCaptureCapability& capability) {
  LockReceive();
  LockFilter();
  HRESULT hr;
  if (m_pInput) {
    hr = m_pInput->SetMatchingMediaType(capability);
  } else {
    hr = E_UNEXPECTED;
  }
  UnlockFilter();
  UnlockReceive();
  return hr;
||||||| merged common ancestors
STDMETHODIMP CaptureSinkFilter::SetMatchingMediaType(
                                        const VideoCaptureCapability& capability)
{
    LockReceive();
    LockFilter();
    HRESULT hr;
    if (m_pInput)
    {
        hr = m_pInput->SetMatchingMediaType(capability);
    }
    else
    {
        hr = E_UNEXPECTED;
    }
    UnlockFilter();
    UnlockReceive();
    return hr;
=======
bool CaptureSinkFilter::IsStopped() const {
  RTC_DCHECK_RUN_ON(&main_checker_);
  return state_ == State_Stopped;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
STDMETHODIMP CaptureSinkFilter::GetClassID(OUT CLSID* pCLSID) {
  (*pCLSID) = CLSID_SINKFILTER;
  return S_OK;
||||||| merged common ancestors
STDMETHODIMP CaptureSinkFilter::GetClassID( OUT CLSID * pCLSID )
{
    (* pCLSID) = CLSID_SINKFILTER;
    return S_OK;
=======
STDMETHODIMP CaptureSinkFilter::QueryInterface(REFIID riid, void** ppv) {
  if (riid == IID_IUnknown || riid == IID_IPersist || riid == IID_IBaseFilter) {
    *ppv = static_cast<IBaseFilter*>(this);
    AddRef();
    return S_OK;
  }
  return E_NOINTERFACE;
}

STDMETHODIMP CaptureSinkFilter::GetClassID(CLSID* clsid) {
  *clsid = CLSID_SINKFILTER;
  return S_OK;
>>>>>>> upstream-releases
}

}  // namespace videocapturemodule
}  // namespace webrtc
