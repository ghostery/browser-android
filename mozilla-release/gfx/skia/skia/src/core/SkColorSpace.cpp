/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkColorSpace.h"
#include "SkColorSpacePriv.h"
#include "SkData.h"
#include "SkOpts.h"
#include "../../third_party/skcms/skcms.h"

<<<<<<< HEAD
bool SkColorSpacePrimaries::toXYZD50(SkMatrix44* toXYZ_D50) const {
    skcms_Matrix3x3 toXYZ;
    if (!skcms_PrimariesToXYZD50(fRX, fRY, fGX, fGY, fBX, fBY, fWX, fWY, &toXYZ)) {
        return false;
    }
    toXYZ_D50->set3x3RowMajorf(&toXYZ.vals[0][0]);
    return true;
||||||| merged common ancestors
bool SkColorSpacePrimaries::toXYZD50(SkMatrix44* toXYZ_D50) const {
    if (!is_zero_to_one(fRX) || !is_zero_to_one(fRY) ||
        !is_zero_to_one(fGX) || !is_zero_to_one(fGY) ||
        !is_zero_to_one(fBX) || !is_zero_to_one(fBY) ||
        !is_zero_to_one(fWX) || !is_zero_to_one(fWY))
    {
        return false;
    }

    // First, we need to convert xy values (primaries) to XYZ.
    SkMatrix primaries;
    primaries.setAll(             fRX,              fGX,              fBX,
                                  fRY,              fGY,              fBY,
                     1.0f - fRX - fRY, 1.0f - fGX - fGY, 1.0f - fBX - fBY);
    SkMatrix primariesInv;
    if (!primaries.invert(&primariesInv)) {
        return false;
    }

    // Assumes that Y is 1.0f.
    SkVector3 wXYZ = SkVector3::Make(fWX / fWY, 1.0f, (1.0f - fWX - fWY) / fWY);
    SkVector3 XYZ;
    XYZ.fX = primariesInv[0] * wXYZ.fX + primariesInv[1] * wXYZ.fY + primariesInv[2] * wXYZ.fZ;
    XYZ.fY = primariesInv[3] * wXYZ.fX + primariesInv[4] * wXYZ.fY + primariesInv[5] * wXYZ.fZ;
    XYZ.fZ = primariesInv[6] * wXYZ.fX + primariesInv[7] * wXYZ.fY + primariesInv[8] * wXYZ.fZ;
    SkMatrix toXYZ;
    toXYZ.setAll(XYZ.fX,   0.0f,   0.0f,
                   0.0f, XYZ.fY,   0.0f,
                   0.0f,   0.0f, XYZ.fZ);
    toXYZ.postConcat(primaries);

    // Now convert toXYZ matrix to toXYZD50.
    SkVector3 wXYZD50 = SkVector3::Make(0.96422f, 1.0f, 0.82521f);

    // Calculate the chromatic adaptation matrix.  We will use the Bradford method, thus
    // the matrices below.  The Bradford method is used by Adobe and is widely considered
    // to be the best.
    SkMatrix mA, mAInv;
    mA.setAll(+0.8951f, +0.2664f, -0.1614f,
              -0.7502f, +1.7135f, +0.0367f,
              +0.0389f, -0.0685f, +1.0296f);
    mAInv.setAll(+0.9869929f, -0.1470543f, +0.1599627f,
                 +0.4323053f, +0.5183603f, +0.0492912f,
                 -0.0085287f, +0.0400428f, +0.9684867f);

    SkVector3 srcCone;
    srcCone.fX = mA[0] * wXYZ.fX + mA[1] * wXYZ.fY + mA[2] * wXYZ.fZ;
    srcCone.fY = mA[3] * wXYZ.fX + mA[4] * wXYZ.fY + mA[5] * wXYZ.fZ;
    srcCone.fZ = mA[6] * wXYZ.fX + mA[7] * wXYZ.fY + mA[8] * wXYZ.fZ;
    SkVector3 dstCone;
    dstCone.fX = mA[0] * wXYZD50.fX + mA[1] * wXYZD50.fY + mA[2] * wXYZD50.fZ;
    dstCone.fY = mA[3] * wXYZD50.fX + mA[4] * wXYZD50.fY + mA[5] * wXYZD50.fZ;
    dstCone.fZ = mA[6] * wXYZD50.fX + mA[7] * wXYZD50.fY + mA[8] * wXYZD50.fZ;

    SkMatrix DXToD50;
    DXToD50.setIdentity();
    DXToD50[0] = dstCone.fX / srcCone.fX;
    DXToD50[4] = dstCone.fY / srcCone.fY;
    DXToD50[8] = dstCone.fZ / srcCone.fZ;
    DXToD50.postConcat(mAInv);
    DXToD50.preConcat(mA);

    toXYZ.postConcat(DXToD50);
    toXYZ_D50->set3x3(toXYZ[0], toXYZ[3], toXYZ[6],
                      toXYZ[1], toXYZ[4], toXYZ[7],
                      toXYZ[2], toXYZ[5], toXYZ[8]);
    return true;
=======
bool SkColorSpacePrimaries::toXYZD50(skcms_Matrix3x3* toXYZ_D50) const {
    return skcms_PrimariesToXYZD50(fRX, fRY, fGX, fGY, fBX, fBY, fWX, fWY, toXYZ_D50);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool xyz_almost_equal(const SkMatrix44& toXYZD50, const float m33[9]) {
    return color_space_almost_equal(toXYZD50.getFloat(0, 0), m33[0]) &&
           color_space_almost_equal(toXYZD50.getFloat(0, 1), m33[1]) &&
           color_space_almost_equal(toXYZD50.getFloat(0, 2), m33[2]) &&
           color_space_almost_equal(toXYZD50.getFloat(1, 0), m33[3]) &&
           color_space_almost_equal(toXYZD50.getFloat(1, 1), m33[4]) &&
           color_space_almost_equal(toXYZD50.getFloat(1, 2), m33[5]) &&
           color_space_almost_equal(toXYZD50.getFloat(2, 0), m33[6]) &&
           color_space_almost_equal(toXYZD50.getFloat(2, 1), m33[7]) &&
           color_space_almost_equal(toXYZD50.getFloat(2, 2), m33[8]) &&
           color_space_almost_equal(toXYZD50.getFloat(0, 3), 0.0f) &&
           color_space_almost_equal(toXYZD50.getFloat(1, 3), 0.0f) &&
           color_space_almost_equal(toXYZD50.getFloat(2, 3), 0.0f) &&
           color_space_almost_equal(toXYZD50.getFloat(3, 0), 0.0f) &&
           color_space_almost_equal(toXYZD50.getFloat(3, 1), 0.0f) &&
           color_space_almost_equal(toXYZD50.getFloat(3, 2), 0.0f) &&
           color_space_almost_equal(toXYZD50.getFloat(3, 3), 1.0f);
||||||| merged common ancestors
///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 *  Checks if our toXYZ matrix is a close match to a known color gamut.
 *
 *  @param toXYZD50 transformation matrix deduced from profile data
 *  @param standard 3x3 canonical transformation matrix
 */
static bool xyz_almost_equal(const SkMatrix44& toXYZD50, const float* standard) {
    return color_space_almost_equal(toXYZD50.getFloat(0, 0), standard[0]) &&
           color_space_almost_equal(toXYZD50.getFloat(0, 1), standard[1]) &&
           color_space_almost_equal(toXYZD50.getFloat(0, 2), standard[2]) &&
           color_space_almost_equal(toXYZD50.getFloat(1, 0), standard[3]) &&
           color_space_almost_equal(toXYZD50.getFloat(1, 1), standard[4]) &&
           color_space_almost_equal(toXYZD50.getFloat(1, 2), standard[5]) &&
           color_space_almost_equal(toXYZD50.getFloat(2, 0), standard[6]) &&
           color_space_almost_equal(toXYZD50.getFloat(2, 1), standard[7]) &&
           color_space_almost_equal(toXYZD50.getFloat(2, 2), standard[8]) &&
           color_space_almost_equal(toXYZD50.getFloat(0, 3), 0.0f) &&
           color_space_almost_equal(toXYZD50.getFloat(1, 3), 0.0f) &&
           color_space_almost_equal(toXYZD50.getFloat(2, 3), 0.0f) &&
           color_space_almost_equal(toXYZD50.getFloat(3, 0), 0.0f) &&
           color_space_almost_equal(toXYZD50.getFloat(3, 1), 0.0f) &&
           color_space_almost_equal(toXYZD50.getFloat(3, 2), 0.0f) &&
           color_space_almost_equal(toXYZD50.getFloat(3, 3), 1.0f);
=======
SkColorSpace::SkColorSpace(const float transferFn[7],
                           const skcms_Matrix3x3& toXYZD50) {
    memcpy(fToXYZD50_3x3, &toXYZD50.vals[0][0], 9*sizeof(float));
    fToXYZD50Hash = SkOpts::hash_fn(fToXYZD50_3x3, 9*sizeof(float), 0);

    memcpy(fTransferFn, transferFn, 7*sizeof(float));
    fTransferFnHash = SkOpts::hash_fn(fTransferFn, 7*sizeof(float), 0);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
SkColorSpace::SkColorSpace(SkGammaNamed gammaNamed,
                           const float transferFn[7],
                           const SkMatrix44& toXYZD50)
    : fGammaNamed(gammaNamed)
{
    for (int r = 0; r < 3; r++)
    for (int c = 0; c < 3; c++) {
        fToXYZD50_3x3[3*r+c] = toXYZD50.get(r,c);
    }
    SkASSERT(xyz_almost_equal(toXYZD50, fToXYZD50_3x3));
    fToXYZD50Hash = SkOpts::hash_fn(fToXYZD50_3x3, 9*sizeof(float), 0);

    switch (fGammaNamed) {
        case kSRGB_SkGammaNamed:        transferFn = &  gSRGB_TransferFn.fG; break;
        case k2Dot2Curve_SkGammaNamed:  transferFn = & g2Dot2_TransferFn.fG; break;
        case kLinear_SkGammaNamed:      transferFn = &gLinear_TransferFn.fG; break;
        case kNonStandard_SkGammaNamed:                                      break;
    }
    memcpy(fTransferFn, transferFn, 7*sizeof(float));
    fTransferFnHash = SkOpts::hash_fn(fTransferFn, 7*sizeof(float), 0);
}


sk_sp<SkColorSpace> SkColorSpace::MakeRGB(SkGammaNamed gammaNamed, const SkMatrix44& toXYZD50) {
    switch (gammaNamed) {
        case kSRGB_SkGammaNamed:
            if (xyz_almost_equal(toXYZD50, gSRGB_toXYZD50)) {
                return SkColorSpace::MakeSRGB();
            }
            break;
        case kLinear_SkGammaNamed:
            if (xyz_almost_equal(toXYZD50, gSRGB_toXYZD50)) {
                return SkColorSpace::MakeSRGBLinear();
            }
            break;
        case kNonStandard_SkGammaNamed:
            // This is not allowed.
            return nullptr;
        default:
            break;
||||||| merged common ancestors
sk_sp<SkColorSpace> SkColorSpace::MakeRGB(SkGammaNamed gammaNamed, const SkMatrix44& toXYZD50)
{
    switch (gammaNamed) {
        case kSRGB_SkGammaNamed:
            if (xyz_almost_equal(toXYZD50, gSRGB_toXYZD50)) {
                return SkColorSpace::MakeSRGB();
            }
            break;
#ifdef SK_SUPPORT_LEGACY_ADOBE_XYZ
        case k2Dot2Curve_SkGammaNamed:
            if (xyz_almost_equal(toXYZD50, gAdobeRGB_toXYZD50)) {
                SkMatrix44 adobe44(SkMatrix44::kUninitialized_Constructor);
                adobe44.set3x3RowMajorf(gAdobeRGB_toXYZD50);
                return sk_sp<SkColorSpace>(new SkColorSpace_XYZ(k2Dot2Curve_SkGammaNamed, adobe44));
            }
            break;
#endif
        case kLinear_SkGammaNamed:
            if (xyz_almost_equal(toXYZD50, gSRGB_toXYZD50)) {
                return SkColorSpace::MakeSRGBLinear();
            }
            break;
        case kNonStandard_SkGammaNamed:
            // This is not allowed.
            return nullptr;
        default:
            break;
=======
static bool xyz_almost_equal(const skcms_Matrix3x3& mA, const skcms_Matrix3x3& mB) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (!color_space_almost_equal(mA.vals[r][c], mB.vals[r][c])) {
                return false;
            }
        }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    return sk_sp<SkColorSpace>(new SkColorSpace(gammaNamed, nullptr, toXYZD50));
}

sk_sp<SkColorSpace> SkColorSpace::MakeRGB(RenderTargetGamma gamma, const SkMatrix44& toXYZD50) {
    switch (gamma) {
        case kLinear_RenderTargetGamma:
            return SkColorSpace::MakeRGB(kLinear_SkGammaNamed, toXYZD50);
        case kSRGB_RenderTargetGamma:
            return SkColorSpace::MakeRGB(kSRGB_SkGammaNamed, toXYZD50);
        default:
            return nullptr;
    }
||||||| merged common ancestors

    return sk_sp<SkColorSpace>(new SkColorSpace_XYZ(gammaNamed, toXYZD50));
}

sk_sp<SkColorSpace> SkColorSpace::MakeRGB(RenderTargetGamma gamma, const SkMatrix44& toXYZD50) {
    switch (gamma) {
        case kLinear_RenderTargetGamma:
            return SkColorSpace::MakeRGB(kLinear_SkGammaNamed, toXYZD50);
        case kSRGB_RenderTargetGamma:
            return SkColorSpace::MakeRGB(kSRGB_SkGammaNamed, toXYZD50);
        default:
            return nullptr;
    }
=======

    return true;
>>>>>>> upstream-releases
}

sk_sp<SkColorSpace> SkColorSpace::MakeRGB(const skcms_TransferFunction& transferFn,
                                          const skcms_Matrix3x3& toXYZ) {
    if (!is_valid_transfer_fn(transferFn)) {
        return nullptr;
    }

    const float* tf = &transferFn.g;

    if (is_almost_srgb(transferFn)) {
        if (xyz_almost_equal(toXYZ, SkNamedGamut::kSRGB)) {
            return SkColorSpace::MakeSRGB();
        }
        tf = &SkNamedTransferFn::kSRGB.g;
    } else if (is_almost_2dot2(transferFn)) {
        tf = &SkNamedTransferFn::k2Dot2.g;
    } else if (is_almost_linear(transferFn)) {
        if (xyz_almost_equal(toXYZ, SkNamedGamut::kSRGB)) {
            return SkColorSpace::MakeSRGBLinear();
        }
        tf = &SkNamedTransferFn::kLinear.g;
    }

    return sk_sp<SkColorSpace>(new SkColorSpace(tf, toXYZ));
}

<<<<<<< HEAD
    return sk_sp<SkColorSpace>(new SkColorSpace(kNonStandard_SkGammaNamed, &coeffs.fG, toXYZD50));
||||||| merged common ancestors
    void* memory = sk_malloc_throw(sizeof(SkGammas) + sizeof(SkColorSpaceTransferFn));
    sk_sp<SkGammas> gammas = sk_sp<SkGammas>(new (memory) SkGammas(3));
    SkColorSpaceTransferFn* fn = SkTAddOffset<SkColorSpaceTransferFn>(memory, sizeof(SkGammas));
    *fn = coeffs;
    SkGammas::Data data;
    data.fParamOffset = 0;
    for (int channel = 0; channel < 3; ++channel) {
        gammas->fType[channel] = SkGammas::Type::kParam_Type;
        gammas->fData[channel] = data;
    }
    return sk_sp<SkColorSpace>(new SkColorSpace_XYZ(kNonStandard_SkGammaNamed,
                                                    std::move(gammas), toXYZD50, nullptr));
=======
class SkColorSpaceSingletonFactory {
public:
    static SkColorSpace* Make(const skcms_TransferFunction& transferFn,
                              const skcms_Matrix3x3& to_xyz) {
        return new SkColorSpace(&transferFn.g, to_xyz);
    }
};

SkColorSpace* sk_srgb_singleton() {
    static SkColorSpace* cs = SkColorSpaceSingletonFactory::Make(SkNamedTransferFn::kSRGB,
                                                                 SkNamedGamut::kSRGB);
    return cs;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
sk_sp<SkColorSpace> SkColorSpace::MakeRGB(RenderTargetGamma gamma, Gamut gamut) {
    SkMatrix44 toXYZD50;
    to_xyz_d50(&toXYZD50, gamut);
    return SkColorSpace::MakeRGB(gamma, toXYZD50);
||||||| merged common ancestors
sk_sp<SkColorSpace> SkColorSpace::MakeRGB(RenderTargetGamma gamma, Gamut gamut) {
    SkMatrix44 toXYZD50(SkMatrix44::kUninitialized_Constructor);
    to_xyz_d50(&toXYZD50, gamut);
    return SkColorSpace::MakeRGB(gamma, toXYZD50);
=======
SkColorSpace* sk_srgb_linear_singleton() {
    static SkColorSpace* cs = SkColorSpaceSingletonFactory::Make(SkNamedTransferFn::kLinear,
                                                                 SkNamedGamut::kSRGB);
    return cs;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
sk_sp<SkColorSpace> SkColorSpace::MakeRGB(const SkColorSpaceTransferFn& coeffs, Gamut gamut) {
    SkMatrix44 toXYZD50;
    to_xyz_d50(&toXYZD50, gamut);
    return SkColorSpace::MakeRGB(coeffs, toXYZD50);
||||||| merged common ancestors
sk_sp<SkColorSpace> SkColorSpace::MakeRGB(const SkColorSpaceTransferFn& coeffs, Gamut gamut) {
    SkMatrix44 toXYZD50(SkMatrix44::kUninitialized_Constructor);
    to_xyz_d50(&toXYZD50, gamut);
    return SkColorSpace::MakeRGB(coeffs, toXYZD50);
=======
sk_sp<SkColorSpace> SkColorSpace::MakeSRGB() {
    return sk_ref_sp(sk_srgb_singleton());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
class SkColorSpaceSingletonFactory {
public:
    static SkColorSpace* Make(SkGammaNamed gamma, const float to_xyz[9]) {
        SkMatrix44 m44;
        m44.set3x3RowMajorf(to_xyz);
        (void)m44.getType();  // Force typemask to be computed to avoid races.
        return new SkColorSpace(gamma, nullptr, m44);
    }
};
||||||| merged common ancestors
static SkColorSpace* singleton_colorspace(SkGammaNamed gamma, const float to_xyz[9]) {
    SkMatrix44 m44(SkMatrix44::kUninitialized_Constructor);
    m44.set3x3RowMajorf(to_xyz);
    (void)m44.getType();  // Force typemask to be computed to avoid races.
    return new SkColorSpace_XYZ(gamma, m44);
}
=======
sk_sp<SkColorSpace> SkColorSpace::MakeSRGBLinear() {
    return sk_ref_sp(sk_srgb_linear_singleton());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
SkColorSpace* sk_srgb_singleton() {
    static SkColorSpace* cs = SkColorSpaceSingletonFactory::Make(kSRGB_SkGammaNamed,
                                                                 gSRGB_toXYZD50);
    return cs;
||||||| merged common ancestors
static SkColorSpace* srgb() {
    static SkColorSpace* cs = singleton_colorspace(kSRGB_SkGammaNamed, gSRGB_toXYZD50);
    return cs;
=======
void SkColorSpace::computeLazyDstFields() const {
    fLazyDstFieldsOnce([this] {

        // Invert 3x3 gamut, defaulting to sRGB if we can't.
        {
            skcms_Matrix3x3 fwd, inv;
            memcpy(&fwd, fToXYZD50_3x3, 9*sizeof(float));
            if (!skcms_Matrix3x3_invert(&fwd, &inv)) {
                SkAssertResult(skcms_Matrix3x3_invert(&skcms_sRGB_profile()->toXYZD50, &inv));
            }
            memcpy(fFromXYZD50_3x3, &inv, 9*sizeof(float));
        }

        // Invert transfer function, defaulting to sRGB if we can't.
        {
            skcms_TransferFunction fwd, inv;
            this->transferFn(&fwd.g);
            if (!skcms_TransferFunction_invert(&fwd, &inv)) {
                inv = *skcms_sRGB_Inverse_TransferFunction();
            }
            memcpy(fInvTransferFn, &inv, 7*sizeof(float));
        }

    });
>>>>>>> upstream-releases
}
<<<<<<< HEAD
SkColorSpace* sk_srgb_linear_singleton() {
    static SkColorSpace* cs = SkColorSpaceSingletonFactory::Make(kLinear_SkGammaNamed,
                                                                 gSRGB_toXYZD50);
    return cs;
||||||| merged common ancestors
static SkColorSpace* srgb_linear() {
    static SkColorSpace* cs = singleton_colorspace(kLinear_SkGammaNamed, gSRGB_toXYZD50);
    return cs;
=======

bool SkColorSpace::isNumericalTransferFn(skcms_TransferFunction* coeffs) const {
    this->transferFn(&coeffs->g);
    return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
sk_sp<SkColorSpace> SkColorSpace::MakeSRGB() {
    return sk_ref_sp(sk_srgb_singleton());
||||||| merged common ancestors
sk_sp<SkColorSpace> SkColorSpace::MakeSRGB() {
    return sk_ref_sp(srgb());
=======
void SkColorSpace::transferFn(float gabcdef[7]) const {
    memcpy(gabcdef, &fTransferFn, 7*sizeof(float));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
sk_sp<SkColorSpace> SkColorSpace::MakeSRGBLinear() {
    return sk_ref_sp(sk_srgb_linear_singleton());
||||||| merged common ancestors
sk_sp<SkColorSpace> SkColorSpace::MakeSRGBLinear() {
    return sk_ref_sp(srgb_linear());
=======
void SkColorSpace::invTransferFn(float gabcdef[7]) const {
    this->computeLazyDstFields();
    memcpy(gabcdef, &fInvTransferFn, 7*sizeof(float));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void SkColorSpace::computeLazyDstFields() const {
    fLazyDstFieldsOnce([this] {
||||||| merged common ancestors
///////////////////////////////////////////////////////////////////////////////////////////////////
=======
bool SkColorSpace::toXYZD50(SkMatrix44* toXYZD50) const {
    toXYZD50->set3x3RowMajorf(fToXYZD50_3x3);
    return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
        // Invert 3x3 gamut, defaulting to sRGB if we can't.
        {
            skcms_Matrix3x3 fwd, inv;
            memcpy(&fwd, fToXYZD50_3x3, 9*sizeof(float));
            if (!skcms_Matrix3x3_invert(&fwd, &inv)) {
                SkAssertResult(skcms_Matrix3x3_invert(&skcms_sRGB_profile()->toXYZD50, &inv));
            }
            memcpy(fFromXYZD50_3x3, &inv, 9*sizeof(float));
        }
||||||| merged common ancestors
SkColorSpace::Type SkColorSpace::type() const {
    const SkMatrix44* m = this->toXYZD50();
    if (m) {
        return m->isScale() ? kGray_Type : kRGB_Type;
    }
    return this->onIsCMYK() ? kCMYK_Type : kRGB_Type;
}
=======
bool SkColorSpace::toXYZD50(skcms_Matrix3x3* toXYZD50) const {
    memcpy(toXYZD50, fToXYZD50_3x3, 9*sizeof(float));
    return true;
}

void SkColorSpace::gamutTransformTo(const SkColorSpace* dst, float src_to_dst[9]) const {
    dst->computeLazyDstFields();

    skcms_Matrix3x3 toXYZD50,
                  fromXYZD50;

    memcpy(&  toXYZD50, this->  fToXYZD50_3x3, 9*sizeof(float));
    memcpy(&fromXYZD50, dst ->fFromXYZD50_3x3, 9*sizeof(float));

    skcms_Matrix3x3 srcToDst = skcms_Matrix3x3_concat(&fromXYZD50, &toXYZD50);
    memcpy(src_to_dst, &srcToDst, 9*sizeof(float));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
        // Invert transfer function, defaulting to sRGB if we can't.
        {
            skcms_TransferFunction fwd, inv;
            this->transferFn(&fwd.g);
            if (!skcms_TransferFunction_invert(&fwd, &inv)) {
                inv = *skcms_sRGB_Inverse_TransferFunction();
            }
            memcpy(fInvTransferFn, &inv, 7*sizeof(float));
        }

    });
||||||| merged common ancestors
SkGammaNamed SkColorSpace::gammaNamed() const {
    return this->onGammaNamed();
=======
bool SkColorSpace::isSRGB() const {
    return sk_srgb_singleton() == this;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool SkColorSpace::isNumericalTransferFn(SkColorSpaceTransferFn* coeffs) const {
    this->transferFn(&coeffs->fG);
    return true;
||||||| merged common ancestors
bool SkColorSpace::gammaCloseToSRGB() const {
    return this->onGammaCloseToSRGB();
=======
bool SkColorSpace::gammaCloseToSRGB() const {
    // Nearly-equal transfer functions were snapped at construction time, so just do an exact test
    return memcmp(fTransferFn, &SkNamedTransferFn::kSRGB.g, 7*sizeof(float)) == 0;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void SkColorSpace::transferFn(float gabcdef[7]) const {
    memcpy(gabcdef, &fTransferFn, 7*sizeof(float));
||||||| merged common ancestors
bool SkColorSpace::gammaIsLinear() const {
    return this->onGammaIsLinear();
=======
bool SkColorSpace::gammaIsLinear() const {
    // Nearly-equal transfer functions were snapped at construction time, so just do an exact test
    return memcmp(fTransferFn, &SkNamedTransferFn::kLinear.g, 7*sizeof(float)) == 0;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void SkColorSpace::invTransferFn(float gabcdef[7]) const {
    this->computeLazyDstFields();
    memcpy(gabcdef, &fInvTransferFn, 7*sizeof(float));
||||||| merged common ancestors
bool SkColorSpace::isNumericalTransferFn(SkColorSpaceTransferFn* fn) const {
    return this->onIsNumericalTransferFn(fn);
=======
sk_sp<SkColorSpace> SkColorSpace::makeLinearGamma() const {
    if (this->gammaIsLinear()) {
        return sk_ref_sp(const_cast<SkColorSpace*>(this));
    }
    skcms_Matrix3x3 gamut;
    this->toXYZD50(&gamut);
    return SkColorSpace::MakeRGB(SkNamedTransferFn::kLinear, gamut);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool SkColorSpace::toXYZD50(SkMatrix44* toXYZD50) const {
    toXYZD50->set3x3RowMajorf(fToXYZD50_3x3);
    return true;
}


void SkColorSpace::gamutTransformTo(const SkColorSpace* dst, float src_to_dst[9]) const {
    dst->computeLazyDstFields();

    skcms_Matrix3x3 toXYZD50,
                  fromXYZD50;

    memcpy(&  toXYZD50, this->  fToXYZD50_3x3, 9*sizeof(float));
    memcpy(&fromXYZD50, dst ->fFromXYZD50_3x3, 9*sizeof(float));

    skcms_Matrix3x3 srcToDst = skcms_Matrix3x3_concat(&fromXYZD50, &toXYZD50);
    memcpy(src_to_dst, &srcToDst, 9*sizeof(float));
}

bool SkColorSpace::isSRGB() const {
    return sk_srgb_singleton() == this;
||||||| merged common ancestors
bool SkColorSpace::toXYZD50(SkMatrix44* toXYZD50) const {
    const SkMatrix44* matrix = this->onToXYZD50();
    if (matrix) {
        *toXYZD50 = *matrix;
        return true;
    }

    return false;
=======
sk_sp<SkColorSpace> SkColorSpace::makeSRGBGamma() const {
    if (this->gammaCloseToSRGB()) {
        return sk_ref_sp(const_cast<SkColorSpace*>(this));
    }
    skcms_Matrix3x3 gamut;
    this->toXYZD50(&gamut);
    return SkColorSpace::MakeRGB(SkNamedTransferFn::kSRGB, gamut);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
sk_sp<SkColorSpace> SkColorSpace::makeLinearGamma() const {
    if (this->gammaIsLinear()) {
        return sk_ref_sp(const_cast<SkColorSpace*>(this));
    }
    SkMatrix44 m44;
    this->toXYZD50(&m44);
    return SkColorSpace::MakeRGB(kLinear_SkGammaNamed, m44);
}
||||||| merged common ancestors
const SkMatrix44* SkColorSpace::toXYZD50() const {
    return this->onToXYZD50();
}
=======
sk_sp<SkColorSpace> SkColorSpace::makeColorSpin() const {
    skcms_Matrix3x3 spin = {{
        { 0, 0, 1 },
        { 1, 0, 0 },
        { 0, 1, 0 },
    }};

    skcms_Matrix3x3 toXYZ;
    this->toXYZD50(&toXYZ);
>>>>>>> upstream-releases

<<<<<<< HEAD
sk_sp<SkColorSpace> SkColorSpace::makeSRGBGamma() const {
    if (this->gammaCloseToSRGB()) {
        return sk_ref_sp(const_cast<SkColorSpace*>(this));
    }
    SkMatrix44 m44;
    this->toXYZD50(&m44);
    return SkColorSpace::MakeRGB(kSRGB_SkGammaNamed, m44);
||||||| merged common ancestors
const SkMatrix44* SkColorSpace::fromXYZD50() const {
    return this->onFromXYZD50();
=======
    skcms_Matrix3x3 spun = skcms_Matrix3x3_concat(&toXYZ, &spin);

    return sk_sp<SkColorSpace>(new SkColorSpace(fTransferFn, spun));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
sk_sp<SkColorSpace> SkColorSpace::makeColorSpin() const {
    SkMatrix44 spin;
    spin.set3x3(0, 1, 0, 0, 0, 1, 1, 0, 0);

    SkMatrix44 m44;
    this->toXYZD50(&m44);
    spin.postConcat(m44);

    (void)spin.getType();  // Pre-cache spin matrix type to avoid races in future getType() calls.
    return sk_sp<SkColorSpace>(new SkColorSpace(fGammaNamed, fTransferFn, spin));
||||||| merged common ancestors
uint32_t SkColorSpace::toXYZD50Hash() const {
    return this->onToXYZD50Hash();
=======
void SkColorSpace::toProfile(skcms_ICCProfile* profile) const {
    skcms_TransferFunction tf;
    skcms_Matrix3x3        toXYZD50;

    memcpy(&tf,       fTransferFn,   7*sizeof(float));
    memcpy(&toXYZD50, fToXYZD50_3x3, 9*sizeof(float));

    skcms_Init               (profile);
    skcms_SetTransferFunction(profile, &tf);
    skcms_SetXYZD50          (profile, &toXYZD50);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void SkColorSpace::toProfile(skcms_ICCProfile* profile) const {
    skcms_TransferFunction tf;
    skcms_Matrix3x3        toXYZD50;

    memcpy(&tf,       fTransferFn,   7*sizeof(float));
    memcpy(&toXYZD50, fToXYZD50_3x3, 9*sizeof(float));

    skcms_Init               (profile);
    skcms_SetTransferFunction(profile, &tf);
    skcms_SetXYZD50          (profile, &toXYZD50);
}

sk_sp<SkColorSpace> SkColorSpace::Make(const skcms_ICCProfile& profile) {
    // TODO: move below ≈sRGB test?
    if (!profile.has_toXYZD50 || !profile.has_trc) {
        return nullptr;
    }

    if (skcms_ApproximatelyEqualProfiles(&profile, skcms_sRGB_profile())) {
        return SkColorSpace::MakeSRGB();
    }

    // TODO: can we save this work and skip lazily inverting the matrix later?
    SkMatrix44 toXYZD50;
    toXYZD50.set3x3RowMajorf(&profile.toXYZD50.vals[0][0]);
    if (!toXYZD50.invert(nullptr)) {
        return nullptr;
    }

    // We can't work with tables or mismatched parametric curves,
    // but if they all look close enough to sRGB, that's fine.
    // TODO: should we maybe do this unconditionally to snap near-sRGB parametrics to sRGB?
    const skcms_Curve* trc = profile.trc;
    if (trc[0].table_entries != 0 ||
        trc[1].table_entries != 0 ||
        trc[2].table_entries != 0 ||
        0 != memcmp(&trc[0].parametric, &trc[1].parametric, sizeof(trc[0].parametric)) ||
        0 != memcmp(&trc[0].parametric, &trc[2].parametric, sizeof(trc[0].parametric)))
    {
        if (skcms_TRCs_AreApproximateInverse(&profile, skcms_sRGB_Inverse_TransferFunction())) {
            return SkColorSpace::MakeRGB(kSRGB_SkGammaNamed, toXYZD50);
        }
        return nullptr;
    }

    SkColorSpaceTransferFn skia_tf;
    memcpy(&skia_tf, &profile.trc[0].parametric, sizeof(skia_tf));
    return SkColorSpace::MakeRGB(skia_tf, toXYZD50);
||||||| merged common ancestors
bool SkColorSpace::isSRGB() const {
    return srgb() == this;
=======
sk_sp<SkColorSpace> SkColorSpace::Make(const skcms_ICCProfile& profile) {
    // TODO: move below ≈sRGB test?
    if (!profile.has_toXYZD50 || !profile.has_trc) {
        return nullptr;
    }

    if (skcms_ApproximatelyEqualProfiles(&profile, skcms_sRGB_profile())) {
        return SkColorSpace::MakeSRGB();
    }

    // TODO: can we save this work and skip lazily inverting the matrix later?
    skcms_Matrix3x3 inv;
    if (!skcms_Matrix3x3_invert(&profile.toXYZD50, &inv)) {
        return nullptr;
    }

    // We can't work with tables or mismatched parametric curves,
    // but if they all look close enough to sRGB, that's fine.
    // TODO: should we maybe do this unconditionally to snap near-sRGB parametrics to sRGB?
    const skcms_Curve* trc = profile.trc;
    if (trc[0].table_entries != 0 ||
        trc[1].table_entries != 0 ||
        trc[2].table_entries != 0 ||
        0 != memcmp(&trc[0].parametric, &trc[1].parametric, sizeof(trc[0].parametric)) ||
        0 != memcmp(&trc[0].parametric, &trc[2].parametric, sizeof(trc[0].parametric)))
    {
        if (skcms_TRCs_AreApproximateInverse(&profile, skcms_sRGB_Inverse_TransferFunction())) {
            return SkColorSpace::MakeRGB(SkNamedTransferFn::kSRGB, profile.toXYZD50);
        }
        return nullptr;
    }

    return SkColorSpace::MakeRGB(profile.trc[0].parametric, profile.toXYZD50);
>>>>>>> upstream-releases
}

///////////////////////////////////////////////////////////////////////////////////////////////////

enum Version {
    k0_Version, // Initial version, header + flags for matrix and profile
    k1_Version, // Simple header (version tag) + 16 floats

    kCurrent_Version = k1_Version,
};

enum NamedColorSpace {
    kSRGB_NamedColorSpace,
    kAdobeRGB_NamedColorSpace,
    kSRGBLinear_NamedColorSpace,
};

enum NamedGamma {
    kLinear_NamedGamma,
    kSRGB_NamedGamma,
    k2Dot2_NamedGamma,
};

struct ColorSpaceHeader {
    // Flag values, only used by old (k0_Version) serialization
    static constexpr uint8_t kMatrix_Flag     = 1 << 0;
<<<<<<< HEAD

    /**
     *  If kICC_Flag is set, we will write an ICC profile after the header.
     *  The ICC profile will be written as a uint32 size, followed immediately
     *  by the data (padded to 4 bytes).
     *  DEPRECATED / UNUSED
     */
||||||| merged common ancestors

    /**
     *  If kICC_Flag is set, we will write an ICC profile after the header.
     *  The ICC profile will be written as a uint32 size, followed immediately
     *  by the data (padded to 4 bytes).
     */
=======
>>>>>>> upstream-releases
    static constexpr uint8_t kICC_Flag        = 1 << 1;
    static constexpr uint8_t kTransferFn_Flag = 1 << 3;

    uint8_t fVersion = kCurrent_Version;

    // Other fields are only used by k0_Version. Could be re-purposed in future versions.
    uint8_t fNamed      = 0;
    uint8_t fGammaNamed = 0;
    uint8_t fFlags      = 0;
};

size_t SkColorSpace::writeToMemory(void* memory) const {
<<<<<<< HEAD
    // If we have a named profile, only write the enum.
    const SkGammaNamed gammaNamed = this->gammaNamed();
    if (this == sk_srgb_singleton()) {
        if (memory) {
            *((ColorSpaceHeader*) memory) = ColorSpaceHeader::Pack(
                    k0_Version, kSRGB_NamedColorSpace, gammaNamed, 0);
        }
        return sizeof(ColorSpaceHeader);
    } else if (this == sk_srgb_linear_singleton()) {
        if (memory) {
            *((ColorSpaceHeader*) memory) = ColorSpaceHeader::Pack(
                    k0_Version, kSRGBLinear_NamedColorSpace, gammaNamed, 0);
        }
        return sizeof(ColorSpaceHeader);
    }

    // If we have a named gamma, write the enum and the matrix.
    switch (gammaNamed) {
        case kSRGB_SkGammaNamed:
        case k2Dot2Curve_SkGammaNamed:
        case kLinear_SkGammaNamed: {
            if (memory) {
                *((ColorSpaceHeader*) memory) =
                        ColorSpaceHeader::Pack(k0_Version, 0, gammaNamed,
                                                ColorSpaceHeader::kMatrix_Flag);
                memory = SkTAddOffset<void>(memory, sizeof(ColorSpaceHeader));
                SkMatrix44 m44;
                this->toXYZD50(&m44);
                m44.as3x4RowMajorf((float*) memory);
            }
            return sizeof(ColorSpaceHeader) + 12 * sizeof(float);
        }
        default: {
            SkColorSpaceTransferFn transferFn;
            SkAssertResult(this->isNumericalTransferFn(&transferFn));
||||||| merged common ancestors
    // Start by trying the serialization fast path.  If we haven't saved ICC profile data,
    // we must have a profile that we can serialize easily.
    if (!this->onProfileData()) {
        // Profile data is mandatory for A2B0 color spaces, so we must be XYZ.
        SkASSERT(this->toXYZD50());
        // If we have a named profile, only write the enum.
        const SkGammaNamed gammaNamed = this->gammaNamed();
        if (this == srgb()) {
            if (memory) {
                *((ColorSpaceHeader*) memory) = ColorSpaceHeader::Pack(
                        k0_Version, kSRGB_NamedColorSpace, gammaNamed, 0);
            }
            return sizeof(ColorSpaceHeader);
        } else if (this == srgb_linear()) {
            if (memory) {
                *((ColorSpaceHeader*) memory) = ColorSpaceHeader::Pack(
                        k0_Version, kSRGBLinear_NamedColorSpace, gammaNamed, 0);
            }
            return sizeof(ColorSpaceHeader);
        }

        // If we have a named gamma, write the enum and the matrix.
        switch (gammaNamed) {
            case kSRGB_SkGammaNamed:
            case k2Dot2Curve_SkGammaNamed:
            case kLinear_SkGammaNamed: {
                if (memory) {
                    *((ColorSpaceHeader*) memory) =
                            ColorSpaceHeader::Pack(k0_Version, 0, gammaNamed,
                                                   ColorSpaceHeader::kMatrix_Flag);
                    memory = SkTAddOffset<void>(memory, sizeof(ColorSpaceHeader));
                    this->toXYZD50()->as3x4RowMajorf((float*) memory);
                }
                return sizeof(ColorSpaceHeader) + 12 * sizeof(float);
            }
            default: {
                SkColorSpaceTransferFn transferFn;
                SkAssertResult(this->isNumericalTransferFn(&transferFn));

                if (memory) {
                    *((ColorSpaceHeader*) memory) =
                            ColorSpaceHeader::Pack(k0_Version, 0, gammaNamed,
                                                   ColorSpaceHeader::kTransferFn_Flag);
                    memory = SkTAddOffset<void>(memory, sizeof(ColorSpaceHeader));

                    *(((float*) memory) + 0) = transferFn.fA;
                    *(((float*) memory) + 1) = transferFn.fB;
                    *(((float*) memory) + 2) = transferFn.fC;
                    *(((float*) memory) + 3) = transferFn.fD;
                    *(((float*) memory) + 4) = transferFn.fE;
                    *(((float*) memory) + 5) = transferFn.fF;
                    *(((float*) memory) + 6) = transferFn.fG;
                    memory = SkTAddOffset<void>(memory, 7 * sizeof(float));

                    this->toXYZD50()->as3x4RowMajorf((float*) memory);
                }

                return sizeof(ColorSpaceHeader) + 19 * sizeof(float);
            }
        }
    }

    // Otherwise, serialize the ICC data.
    size_t profileSize = this->onProfileData()->size();
    if (SkAlign4(profileSize) != (uint32_t) SkAlign4(profileSize)) {
        return 0;
    }

    if (memory) {
        *((ColorSpaceHeader*) memory) = ColorSpaceHeader::Pack(k0_Version, 0,
                                                               kNonStandard_SkGammaNamed,
                                                               ColorSpaceHeader::kICC_Flag);
        memory = SkTAddOffset<void>(memory, sizeof(ColorSpaceHeader));
=======
    if (memory) {
        *((ColorSpaceHeader*) memory) = ColorSpaceHeader();
        memory = SkTAddOffset<void>(memory, sizeof(ColorSpaceHeader));
>>>>>>> upstream-releases

<<<<<<< HEAD
            if (memory) {
                *((ColorSpaceHeader*) memory) =
                        ColorSpaceHeader::Pack(k0_Version, 0, gammaNamed,
                                                ColorSpaceHeader::kTransferFn_Flag);
                memory = SkTAddOffset<void>(memory, sizeof(ColorSpaceHeader));

                *(((float*) memory) + 0) = transferFn.fA;
                *(((float*) memory) + 1) = transferFn.fB;
                *(((float*) memory) + 2) = transferFn.fC;
                *(((float*) memory) + 3) = transferFn.fD;
                *(((float*) memory) + 4) = transferFn.fE;
                *(((float*) memory) + 5) = transferFn.fF;
                *(((float*) memory) + 6) = transferFn.fG;
                memory = SkTAddOffset<void>(memory, 7 * sizeof(float));

                SkMatrix44 m44;
                this->toXYZD50(&m44);
                m44.as3x4RowMajorf((float*) memory);
            }
||||||| merged common ancestors
        *((uint32_t*) memory) = (uint32_t) SkAlign4(profileSize);
        memory = SkTAddOffset<void>(memory, sizeof(uint32_t));
=======
        memcpy(memory, fTransferFn, 7 * sizeof(float));
        memory = SkTAddOffset<void>(memory, 7 * sizeof(float));
>>>>>>> upstream-releases

<<<<<<< HEAD
            return sizeof(ColorSpaceHeader) + 19 * sizeof(float);
        }
||||||| merged common ancestors
        memcpy(memory, this->onProfileData()->data(), profileSize);
        memset(SkTAddOffset<void>(memory, profileSize), 0, SkAlign4(profileSize) - profileSize);
=======
        memcpy(memory, fToXYZD50_3x3, 9 * sizeof(float));
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
||||||| merged common ancestors
    return sizeof(ColorSpaceHeader) + sizeof(uint32_t) + SkAlign4(profileSize);
=======

    return sizeof(ColorSpaceHeader) + 16 * sizeof(float);
>>>>>>> upstream-releases
}

sk_sp<SkData> SkColorSpace::serialize() const {
    sk_sp<SkData> data = SkData::MakeUninitialized(this->writeToMemory(nullptr));
    this->writeToMemory(data->writable_data());
    return data;
}

sk_sp<SkColorSpace> SkColorSpace::Deserialize(const void* data, size_t length) {
    if (length < sizeof(ColorSpaceHeader)) {
        return nullptr;
    }

    ColorSpaceHeader header = *((const ColorSpaceHeader*) data);
    data = SkTAddOffset<const void>(data, sizeof(ColorSpaceHeader));
    length -= sizeof(ColorSpaceHeader);
    if (k1_Version == header.fVersion) {
        if (length < 16 * sizeof(float)) {
            return nullptr;
        }

        skcms_TransferFunction transferFn;
        memcpy(&transferFn, data, 7 * sizeof(float));
        data = SkTAddOffset<const void>(data, 7 * sizeof(float));

        skcms_Matrix3x3 toXYZ;
        memcpy(&toXYZ, data, 9 * sizeof(float));
        return SkColorSpace::MakeRGB(transferFn, toXYZ);
    } else if (k0_Version == header.fVersion) {
        if (0 == header.fFlags) {
            switch ((NamedColorSpace)header.fNamed) {
                case kSRGB_NamedColorSpace:
                    return SkColorSpace::MakeSRGB();
                case kSRGBLinear_NamedColorSpace:
                    return SkColorSpace::MakeSRGBLinear();
                case kAdobeRGB_NamedColorSpace:
                    return SkColorSpace::MakeRGB(SkNamedTransferFn::k2Dot2,
                                                 SkNamedGamut::kAdobeRGB);
            }
        }

        auto make_named_tf = [=](const skcms_TransferFunction& tf) {
            if (ColorSpaceHeader::kMatrix_Flag != header.fFlags || length < 12 * sizeof(float)) {
                return sk_sp<SkColorSpace>(nullptr);
            }

<<<<<<< HEAD
            SkMatrix44 toXYZ;
            toXYZ.set3x4RowMajorf((const float*) data);
            return SkColorSpace::MakeRGB((SkGammaNamed) header.fGammaNamed, toXYZ);
||||||| merged common ancestors
            SkMatrix44 toXYZ(SkMatrix44::kUninitialized_Constructor);
            toXYZ.set3x4RowMajorf((const float*) data);
            return SkColorSpace::MakeRGB((SkGammaNamed) header.fGammaNamed, toXYZ);
=======
            // Version 0 matrix is row-major 3x4
            skcms_Matrix3x3 toXYZ;
            memcpy(&toXYZ.vals[0][0], (const float*)data + 0, 3 * sizeof(float));
            memcpy(&toXYZ.vals[1][0], (const float*)data + 4, 3 * sizeof(float));
            memcpy(&toXYZ.vals[2][0], (const float*)data + 8, 3 * sizeof(float));
            return SkColorSpace::MakeRGB(tf, toXYZ);
        };

        switch ((NamedGamma) header.fGammaNamed) {
            case kSRGB_NamedGamma:
                return make_named_tf(SkNamedTransferFn::kSRGB);
            case k2Dot2_NamedGamma:
                return make_named_tf(SkNamedTransferFn::k2Dot2);
            case kLinear_NamedGamma:
                return make_named_tf(SkNamedTransferFn::kLinear);
            default:
                break;
>>>>>>> upstream-releases
        }

<<<<<<< HEAD
    switch (header.fFlags) {
        case ColorSpaceHeader::kICC_Flag: {
            // Deprecated and unsupported code path
            return nullptr;
        }
        case ColorSpaceHeader::kTransferFn_Flag: {
            if (length < 19 * sizeof(float)) {
||||||| merged common ancestors
    switch (header.fFlags) {
        case ColorSpaceHeader::kICC_Flag: {
            if (length < sizeof(uint32_t)) {
                return nullptr;
            }

            uint32_t profileSize = *((uint32_t*) data);
            data = SkTAddOffset<const void>(data, sizeof(uint32_t));
            length -= sizeof(uint32_t);
            if (length < profileSize) {
                return nullptr;
            }

            return MakeICC(data, profileSize);
        }
        case ColorSpaceHeader::kTransferFn_Flag: {
            if (length < 19 * sizeof(float)) {
=======
        switch (header.fFlags) {
            case ColorSpaceHeader::kICC_Flag: {
                // Deprecated and unsupported code path
                return nullptr;
            }
            case ColorSpaceHeader::kTransferFn_Flag: {
                if (length < 19 * sizeof(float)) {
                    return nullptr;
                }

                // Version 0 TF is in abcdefg order
                skcms_TransferFunction transferFn;
                transferFn.a = *(((const float*) data) + 0);
                transferFn.b = *(((const float*) data) + 1);
                transferFn.c = *(((const float*) data) + 2);
                transferFn.d = *(((const float*) data) + 3);
                transferFn.e = *(((const float*) data) + 4);
                transferFn.f = *(((const float*) data) + 5);
                transferFn.g = *(((const float*) data) + 6);
                data = SkTAddOffset<const void>(data, 7 * sizeof(float));

                // Version 0 matrix is row-major 3x4
                skcms_Matrix3x3 toXYZ;
                memcpy(&toXYZ.vals[0][0], (const float*)data + 0, 3 * sizeof(float));
                memcpy(&toXYZ.vals[1][0], (const float*)data + 4, 3 * sizeof(float));
                memcpy(&toXYZ.vals[2][0], (const float*)data + 8, 3 * sizeof(float));
                return SkColorSpace::MakeRGB(transferFn, toXYZ);
            }
            default:
>>>>>>> upstream-releases
                return nullptr;
<<<<<<< HEAD
            }

            SkColorSpaceTransferFn transferFn;
            transferFn.fA = *(((const float*) data) + 0);
            transferFn.fB = *(((const float*) data) + 1);
            transferFn.fC = *(((const float*) data) + 2);
            transferFn.fD = *(((const float*) data) + 3);
            transferFn.fE = *(((const float*) data) + 4);
            transferFn.fF = *(((const float*) data) + 5);
            transferFn.fG = *(((const float*) data) + 6);
            data = SkTAddOffset<const void>(data, 7 * sizeof(float));

            SkMatrix44 toXYZ;
            toXYZ.set3x4RowMajorf((const float*) data);
            return SkColorSpace::MakeRGB(transferFn, toXYZ);
||||||| merged common ancestors
            }

            SkColorSpaceTransferFn transferFn;
            transferFn.fA = *(((const float*) data) + 0);
            transferFn.fB = *(((const float*) data) + 1);
            transferFn.fC = *(((const float*) data) + 2);
            transferFn.fD = *(((const float*) data) + 3);
            transferFn.fE = *(((const float*) data) + 4);
            transferFn.fF = *(((const float*) data) + 5);
            transferFn.fG = *(((const float*) data) + 6);
            data = SkTAddOffset<const void>(data, 7 * sizeof(float));

            SkMatrix44 toXYZ(SkMatrix44::kUninitialized_Constructor);
            toXYZ.set3x4RowMajorf((const float*) data);
            return SkColorSpace::MakeRGB(transferFn, toXYZ);
=======
>>>>>>> upstream-releases
        }
    } else {
        return nullptr;
    }
}

bool SkColorSpace::Equals(const SkColorSpace* x, const SkColorSpace* y) {
    if (x == y) {
        return true;
    }

    if (!x || !y) {
        return false;
    }

    if (x->hash() == y->hash()) {
        for (int i = 0; i < 7; i++) {
            SkASSERT(x->  fTransferFn[i] == y->  fTransferFn[i] && "Hash collsion");
        }
        for (int i = 0; i < 9; i++) {
            SkASSERT(x->fToXYZD50_3x3[i] == y->fToXYZD50_3x3[i] && "Hash collsion");
        }
        return true;
    }
    return false;
}
