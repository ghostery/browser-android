/** @file
    @brief Header

    Must be c-safe!

    GENERATED - do not edit by hand!

    @date 2014

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

/*
// Copyright 2014 Sensics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#ifndef INCLUDED_ClientCallbackTypesC_h_GUID_4D43A675_C8A4_4BBF_516F_59E6C785E4EF
#define INCLUDED_ClientCallbackTypesC_h_GUID_4D43A675_C8A4_4BBF_516F_59E6C785E4EF

/* Internal Includes */
#include <osvr/Util/ClientReportTypesC.h>
#include <osvr/Util/ImagingReportTypesC.h>
#include <osvr/Util/ReturnCodesC.h>
#include <osvr/Util/TimeValueC.h>

/* Library/third-party includes */
/* none */

/* Standard includes */
/* none */

OSVR_EXTERN_C_BEGIN

/** @addtogroup ClientKit
    @{
*/

/** @name Report callback types
    @{
*/

/* generated file - do not edit! */
/** @brief C function type for a Pose callback */
<<<<<<< HEAD
typedef void (*OSVR_PoseCallback)(void *userdata,
                                  const struct OSVR_TimeValue *timestamp,
                                  const struct OSVR_PoseReport *report);
||||||| merged common ancestors
typedef void (*OSVR_PoseCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_PoseReport *report);
=======
typedef void (*OSVR_PoseCallback)(void* userdata,
                                  const struct OSVR_TimeValue* timestamp,
                                  const struct OSVR_PoseReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a Position callback */
<<<<<<< HEAD
typedef void (*OSVR_PositionCallback)(void *userdata,
                                      const struct OSVR_TimeValue *timestamp,
                                      const struct OSVR_PositionReport *report);
||||||| merged common ancestors
typedef void (*OSVR_PositionCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_PositionReport *report);
=======
typedef void (*OSVR_PositionCallback)(void* userdata,
                                      const struct OSVR_TimeValue* timestamp,
                                      const struct OSVR_PositionReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a Orientation callback */
<<<<<<< HEAD
typedef void (*OSVR_OrientationCallback)(
    void *userdata, const struct OSVR_TimeValue *timestamp,
    const struct OSVR_OrientationReport *report);
||||||| merged common ancestors
typedef void (*OSVR_OrientationCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_OrientationReport *report);
=======
typedef void (*OSVR_OrientationCallback)(
    void* userdata, const struct OSVR_TimeValue* timestamp,
    const struct OSVR_OrientationReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a Velocity callback */
<<<<<<< HEAD
typedef void (*OSVR_VelocityCallback)(void *userdata,
                                      const struct OSVR_TimeValue *timestamp,
                                      const struct OSVR_VelocityReport *report);
||||||| merged common ancestors
typedef void (*OSVR_VelocityCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_VelocityReport *report);
=======
typedef void (*OSVR_VelocityCallback)(void* userdata,
                                      const struct OSVR_TimeValue* timestamp,
                                      const struct OSVR_VelocityReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a LinearVelocity callback */
<<<<<<< HEAD
typedef void (*OSVR_LinearVelocityCallback)(
    void *userdata, const struct OSVR_TimeValue *timestamp,
    const struct OSVR_LinearVelocityReport *report);
||||||| merged common ancestors
typedef void (*OSVR_LinearVelocityCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_LinearVelocityReport *report);
=======
typedef void (*OSVR_LinearVelocityCallback)(
    void* userdata, const struct OSVR_TimeValue* timestamp,
    const struct OSVR_LinearVelocityReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a AngularVelocity callback */
<<<<<<< HEAD
typedef void (*OSVR_AngularVelocityCallback)(
    void *userdata, const struct OSVR_TimeValue *timestamp,
    const struct OSVR_AngularVelocityReport *report);
||||||| merged common ancestors
typedef void (*OSVR_AngularVelocityCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_AngularVelocityReport *report);
=======
typedef void (*OSVR_AngularVelocityCallback)(
    void* userdata, const struct OSVR_TimeValue* timestamp,
    const struct OSVR_AngularVelocityReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a Acceleration callback */
<<<<<<< HEAD
typedef void (*OSVR_AccelerationCallback)(
    void *userdata, const struct OSVR_TimeValue *timestamp,
    const struct OSVR_AccelerationReport *report);
||||||| merged common ancestors
typedef void (*OSVR_AccelerationCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_AccelerationReport *report);
=======
typedef void (*OSVR_AccelerationCallback)(
    void* userdata, const struct OSVR_TimeValue* timestamp,
    const struct OSVR_AccelerationReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a LinearAcceleration callback */
<<<<<<< HEAD
typedef void (*OSVR_LinearAccelerationCallback)(
    void *userdata, const struct OSVR_TimeValue *timestamp,
    const struct OSVR_LinearAccelerationReport *report);
||||||| merged common ancestors
typedef void (*OSVR_LinearAccelerationCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_LinearAccelerationReport *report);
=======
typedef void (*OSVR_LinearAccelerationCallback)(
    void* userdata, const struct OSVR_TimeValue* timestamp,
    const struct OSVR_LinearAccelerationReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a AngularAcceleration callback */
<<<<<<< HEAD
typedef void (*OSVR_AngularAccelerationCallback)(
    void *userdata, const struct OSVR_TimeValue *timestamp,
    const struct OSVR_AngularAccelerationReport *report);
||||||| merged common ancestors
typedef void (*OSVR_AngularAccelerationCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_AngularAccelerationReport *report);
=======
typedef void (*OSVR_AngularAccelerationCallback)(
    void* userdata, const struct OSVR_TimeValue* timestamp,
    const struct OSVR_AngularAccelerationReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a Button callback */
<<<<<<< HEAD
typedef void (*OSVR_ButtonCallback)(void *userdata,
                                    const struct OSVR_TimeValue *timestamp,
                                    const struct OSVR_ButtonReport *report);
||||||| merged common ancestors
typedef void (*OSVR_ButtonCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_ButtonReport *report);
=======
typedef void (*OSVR_ButtonCallback)(void* userdata,
                                    const struct OSVR_TimeValue* timestamp,
                                    const struct OSVR_ButtonReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a Analog callback */
<<<<<<< HEAD
typedef void (*OSVR_AnalogCallback)(void *userdata,
                                    const struct OSVR_TimeValue *timestamp,
                                    const struct OSVR_AnalogReport *report);
||||||| merged common ancestors
typedef void (*OSVR_AnalogCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_AnalogReport *report);
=======
typedef void (*OSVR_AnalogCallback)(void* userdata,
                                    const struct OSVR_TimeValue* timestamp,
                                    const struct OSVR_AnalogReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a Imaging callback */
typedef void (*OSVR_ImagingCallback)(void* userdata,
                                     const struct OSVR_TimeValue* timestamp,
                                     const struct OSVR_ImagingReport* report);
/** @brief C function type for a Location2D callback */
<<<<<<< HEAD
typedef void (*OSVR_Location2DCallback)(
    void *userdata, const struct OSVR_TimeValue *timestamp,
    const struct OSVR_Location2DReport *report);
||||||| merged common ancestors
typedef void (*OSVR_Location2DCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_Location2DReport *report);
=======
typedef void (*OSVR_Location2DCallback)(
    void* userdata, const struct OSVR_TimeValue* timestamp,
    const struct OSVR_Location2DReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a Direction callback */
<<<<<<< HEAD
typedef void (*OSVR_DirectionCallback)(
    void *userdata, const struct OSVR_TimeValue *timestamp,
    const struct OSVR_DirectionReport *report);
||||||| merged common ancestors
typedef void (*OSVR_DirectionCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_DirectionReport *report);
=======
typedef void (*OSVR_DirectionCallback)(
    void* userdata, const struct OSVR_TimeValue* timestamp,
    const struct OSVR_DirectionReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a EyeTracker2D callback */
<<<<<<< HEAD
typedef void (*OSVR_EyeTracker2DCallback)(
    void *userdata, const struct OSVR_TimeValue *timestamp,
    const struct OSVR_EyeTracker2DReport *report);
||||||| merged common ancestors
typedef void (*OSVR_EyeTracker2DCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_EyeTracker2DReport *report);
=======
typedef void (*OSVR_EyeTracker2DCallback)(
    void* userdata, const struct OSVR_TimeValue* timestamp,
    const struct OSVR_EyeTracker2DReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a EyeTracker3D callback */
<<<<<<< HEAD
typedef void (*OSVR_EyeTracker3DCallback)(
    void *userdata, const struct OSVR_TimeValue *timestamp,
    const struct OSVR_EyeTracker3DReport *report);
||||||| merged common ancestors
typedef void (*OSVR_EyeTracker3DCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_EyeTracker3DReport *report);
=======
typedef void (*OSVR_EyeTracker3DCallback)(
    void* userdata, const struct OSVR_TimeValue* timestamp,
    const struct OSVR_EyeTracker3DReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a EyeTrackerBlink callback */
<<<<<<< HEAD
typedef void (*OSVR_EyeTrackerBlinkCallback)(
    void *userdata, const struct OSVR_TimeValue *timestamp,
    const struct OSVR_EyeTrackerBlinkReport *report);
||||||| merged common ancestors
typedef void (*OSVR_EyeTrackerBlinkCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_EyeTrackerBlinkReport *report);
=======
typedef void (*OSVR_EyeTrackerBlinkCallback)(
    void* userdata, const struct OSVR_TimeValue* timestamp,
    const struct OSVR_EyeTrackerBlinkReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a NaviVelocity callback */
<<<<<<< HEAD
typedef void (*OSVR_NaviVelocityCallback)(
    void *userdata, const struct OSVR_TimeValue *timestamp,
    const struct OSVR_NaviVelocityReport *report);
||||||| merged common ancestors
typedef void (*OSVR_NaviVelocityCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_NaviVelocityReport *report);
=======
typedef void (*OSVR_NaviVelocityCallback)(
    void* userdata, const struct OSVR_TimeValue* timestamp,
    const struct OSVR_NaviVelocityReport* report);
>>>>>>> upstream-releases
/** @brief C function type for a NaviPosition callback */
<<<<<<< HEAD
typedef void (*OSVR_NaviPositionCallback)(
    void *userdata, const struct OSVR_TimeValue *timestamp,
    const struct OSVR_NaviPositionReport *report);
||||||| merged common ancestors
typedef void (*OSVR_NaviPositionCallback)(void *userdata,
                                     const struct OSVR_TimeValue *timestamp,
                                     const struct OSVR_NaviPositionReport *report);
=======
typedef void (*OSVR_NaviPositionCallback)(
    void* userdata, const struct OSVR_TimeValue* timestamp,
    const struct OSVR_NaviPositionReport* report);
>>>>>>> upstream-releases

/** @} */

/** @} */

OSVR_EXTERN_C_END

#endif
