/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrCCQuadraticShader.h"

#include "glsl/GrGLSLFragmentShaderBuilder.h"
#include "glsl/GrGLSLVertexGeoBuilder.h"

void GrCCQuadraticShader::emitSetupCode(GrGLSLVertexGeoBuilder* s, const char* pts,
                                        const char* wind, const char** outHull4) const {
    s->declareGlobal(fQCoordMatrix);
    s->codeAppendf("%s = float2x2(1, 1, .5, 0) * inverse(float2x2(%s[2] - %s[0], %s[1] - %s[0]));",
                   fQCoordMatrix.c_str(), pts, pts, pts, pts);

    s->declareGlobal(fQCoord0);
    s->codeAppendf("%s = %s[0];", fQCoord0.c_str(), pts);

    s->declareGlobal(fEdgeDistanceEquation);
    s->codeAppendf("float2 edgept0 = %s[%s > 0 ? 2 : 0];", pts, wind);
    s->codeAppendf("float2 edgept1 = %s[%s > 0 ? 0 : 2];", pts, wind);
    Shader::EmitEdgeDistanceEquation(s, "edgept0", "edgept1", fEdgeDistanceEquation.c_str());

    if (outHull4) {
        // Clip the bezier triangle by the tangent line at maximum height. Quadratics have the nice
        // property that maximum height always occurs at T=.5. This is a simple application for
        // De Casteljau's algorithm.
        s->codeAppend ("float2 quadratic_hull[4];");
        s->codeAppendf("quadratic_hull[0] = %s[0];", pts);
        s->codeAppendf("quadratic_hull[1] = (%s[0] + %s[1]) * .5;", pts, pts);
        s->codeAppendf("quadratic_hull[2] = (%s[1] + %s[2]) * .5;", pts, pts);
        s->codeAppendf("quadratic_hull[3] = %s[2];", pts);
        *outHull4 = "quadratic_hull";
    }
}

void GrCCQuadraticShader::onEmitVaryings(GrGLSLVaryingHandler* varyingHandler,
                                         GrGLSLVarying::Scope scope, SkString* code,
                                         const char* position, const char* coverage,
                                         const char* cornerCoverage) {
    fCoord_fGrad.reset(kFloat4_GrSLType, scope);
    varyingHandler->addVarying("coord_and_grad", &fCoord_fGrad);
    code->appendf("%s.xy = %s * (%s - %s);", // Quadratic coords.
                  OutName(fCoord_fGrad), fQCoordMatrix.c_str(), position, fQCoord0.c_str());
    code->appendf("%s.zw = 2*bloat * float2(2 * %s.x, -1) * %s;", // Gradient.
                  OutName(fCoord_fGrad), OutName(fCoord_fGrad), fQCoordMatrix.c_str());

    // Coverages need full precision since distance to the opposite edge can be large.
    fEdge_fWind_fCorner.reset(cornerCoverage ? kFloat4_GrSLType : kFloat2_GrSLType, scope);
    varyingHandler->addVarying("edge_and_wind_and_corner", &fEdge_fWind_fCorner);
    code->appendf("float edge = dot(%s, float3(%s, 1));", // Distance to flat opposite edge.
                  fEdgeDistanceEquation.c_str(), position);
    code->appendf("%s.x = edge;", OutName(fEdge_fWind_fCorner));
    code->appendf("%s.y = %s;", OutName(fEdge_fWind_fCorner), coverage); // coverage == wind.

    if (cornerCoverage) {
        code->appendf("half hull_coverage;");
        this->calcHullCoverage(code, OutName(fCoord_fGrad), "edge", "hull_coverage");
        code->appendf("%s.zw = half2(hull_coverage, 1) * %s;",
                      OutName(fEdge_fWind_fCorner), cornerCoverage);
    }
}

void GrCCQuadraticShader::onEmitFragmentCode(GrGLSLFPFragmentBuilder* f,
                                             const char* outputCoverage) const {
<<<<<<< HEAD
    this->calcHullCoverage(&AccessCodeString(f), fCoord_fGrad.fsIn(),
                           SkStringPrintf("%s.x", fEdge_fWind_fCorner.fsIn()).c_str(),
                           outputCoverage);
    f->codeAppendf("%s *= %s.y;", outputCoverage, fEdge_fWind_fCorner.fsIn()); // Wind.

    if (kFloat4_GrSLType == fEdge_fWind_fCorner.type()) {
        f->codeAppendf("%s = %s.z * %s.w + %s;",// Attenuated corner coverage.
                       outputCoverage, fEdge_fWind_fCorner.fsIn(), fEdge_fWind_fCorner.fsIn(),
                       outputCoverage);
    }
||||||| merged common ancestors
    this->emitCoverage(f, outputCoverage);
    f->codeAppendf("%s *= %s.w;", outputCoverage, fXYDW.fsIn()); // Sign by wind.
=======
    this->calcHullCoverage(&AccessCodeString(f), fCoord_fGrad.fsIn(),
                           SkStringPrintf("%s.x", fEdge_fWind_fCorner.fsIn()).c_str(),
                           outputCoverage);
    f->codeAppendf("%s *= half(%s.y);", outputCoverage, fEdge_fWind_fCorner.fsIn()); // Wind.

    if (kFloat4_GrSLType == fEdge_fWind_fCorner.type()) {
        f->codeAppendf("%s = half(%s.z * %s.w) + %s;",// Attenuated corner coverage.
                       outputCoverage, fEdge_fWind_fCorner.fsIn(), fEdge_fWind_fCorner.fsIn(),
                       outputCoverage);
    }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void GrCCQuadraticShader::calcHullCoverage(SkString* code, const char* coordAndGrad,
                                           const char* edge, const char* outputCoverage) const {
    code->appendf("float x = %s.x, y = %s.y;", coordAndGrad, coordAndGrad);
    code->appendf("float2 grad = %s.zw;", coordAndGrad);
    code->append ("float f = x*x - y;");
    code->append ("float fwidth = abs(grad.x) + abs(grad.y);");
    code->appendf("%s = min(0.5 - f/fwidth, 1);", outputCoverage); // Curve coverage.
    code->appendf("half d = min(%s, 0);", edge); // Flat edge opposite the curve.
    code->appendf("%s = max(%s + d, 0);", outputCoverage, outputCoverage); // Total hull coverage.
||||||| merged common ancestors
void GrCCQuadraticHullShader::onEmitSetupCode(GrGLSLVertexGeoBuilder* s, const char* pts,
                                              const char* /*repetitionID*/,
                                              GeometryVars* vars) const {
    // Find the T value whose tangent is halfway between the tangents at the endpionts.
    s->codeAppendf("float2 tan0 = %s[1] - %s[0];", pts, pts);
    s->codeAppendf("float2 tan1 = %s[2] - %s[1];", pts, pts);
    s->codeAppend ("float2 midnorm = normalize(tan0) - normalize(tan1);");
    s->codeAppend ("float2 T = midnorm * float2x2(tan0 - tan1, tan0);");
    s->codeAppend ("float t = clamp(T.t / T.s, 0, 1);"); // T.s != 0; we cull flat curves on CPU.

    // Clip the bezier triangle by the tangent at our new t value. This is a simple application for
    // De Casteljau's algorithm.
    s->codeAppendf("float4x2 quadratic_hull = float4x2(%s[0], "
                                                      "%s[0] + tan0 * t, "
                                                      "%s[1] + tan1 * t, "
                                                      "%s[2]);", pts, pts, pts, pts);
    vars->fHullVars.fAlternatePoints = "quadratic_hull";
}

void GrCCQuadraticHullShader::onEmitVaryings(GrGLSLVaryingHandler* varyingHandler,
                                             GrGLSLVarying::Scope scope, SkString* code) {
    fGrad.reset(kFloat2_GrSLType, scope);
    varyingHandler->addVarying("grad", &fGrad);
    code->appendf("%s = float2(2 * %s.x, -1) * float2x2(%s);",
                  OutName(fGrad), OutName(fXYDW), fCanonicalMatrix.c_str());
}

void GrCCQuadraticHullShader::emitCoverage(GrGLSLFPFragmentBuilder* f,
                                           const char* outputCoverage) const {
    f->codeAppendf("float d = (%s.x * %s.x - %s.y) * inversesqrt(dot(%s, %s));",
                   fXYDW.fsIn(), fXYDW.fsIn(), fXYDW.fsIn(), fGrad.fsIn(), fGrad.fsIn());
    f->codeAppendf("%s = clamp(0.5 - d, 0, 1);", outputCoverage);
    f->codeAppendf("%s += min(%s.z, 0);", outputCoverage, fXYDW.fsIn()); // Flat closing edge.
}

void GrCCQuadraticCornerShader::onEmitSetupCode(GrGLSLVertexGeoBuilder* s, const char* pts,
                                                const char* repetitionID,
                                                GeometryVars* vars) const {
    s->codeAppendf("float2 corner = %s[%s * 2];", pts, repetitionID);
    vars->fCornerVars.fPoint = "corner";
}

void GrCCQuadraticCornerShader::onEmitVaryings(GrGLSLVaryingHandler* varyingHandler,
                                               GrGLSLVarying::Scope scope, SkString* code) {
    using Interpolation = GrGLSLVaryingHandler::Interpolation;

    fdXYDdx.reset(kFloat3_GrSLType, scope);
    varyingHandler->addVarying("dXYDdx", &fdXYDdx, Interpolation::kCanBeFlat);
    code->appendf("%s = float3(%s[0].x, %s[0].y, %s.x);",
                  OutName(fdXYDdx), fCanonicalMatrix.c_str(), fCanonicalMatrix.c_str(),
                  fEdgeDistanceEquation.c_str());

    fdXYDdy.reset(kFloat3_GrSLType, scope);
    varyingHandler->addVarying("dXYDdy", &fdXYDdy, Interpolation::kCanBeFlat);
    code->appendf("%s = float3(%s[1].x, %s[1].y, %s.y);",
                  OutName(fdXYDdy), fCanonicalMatrix.c_str(), fCanonicalMatrix.c_str(),
                  fEdgeDistanceEquation.c_str());
}

void GrCCQuadraticCornerShader::emitCoverage(GrGLSLFPFragmentBuilder* f,
                                             const char* outputCoverage) const {
    f->codeAppendf("float x = %s.x, y = %s.y, d = %s.z;",
                   fXYDW.fsIn(), fXYDW.fsIn(), fXYDW.fsIn());
    f->codeAppendf("float2x3 grad_xyd = float2x3(%s, %s);", fdXYDdx.fsIn(), fdXYDdy.fsIn());

    // Erase what the previous hull shader wrote. We don't worry about the two corners falling on
    // the same pixel because those cases should have been weeded out by this point.
    f->codeAppend ("float f = x*x - y;");
    f->codeAppend ("float2 grad_f = float2(2*x, -1) * float2x2(grad_xyd);");
    f->codeAppendf("%s = -(0.5 - f * inversesqrt(dot(grad_f, grad_f)));", outputCoverage);
    f->codeAppendf("%s -= d;", outputCoverage);

    // Use software msaa to approximate coverage at the corner pixels.
    int sampleCount = Shader::DefineSoftSampleLocations(f, "samples");
    f->codeAppendf("float3 xyd_center = float3(%s.xy, %s.z + 0.5);", fXYDW.fsIn(), fXYDW.fsIn());
    f->codeAppendf("for (int i = 0; i < %i; ++i) {", sampleCount);
    f->codeAppend (    "float3 xyd = grad_xyd * samples[i] + xyd_center;");
    f->codeAppend (    "half f = xyd.y - xyd.x * xyd.x;"); // f > 0 -> inside curve.
    f->codeAppendf(    "%s += all(greaterThan(float2(f,xyd.z), float2(0))) ? %f : 0;",
                       outputCoverage, 1.0 / sampleCount);
    f->codeAppendf("}");
=======
void GrCCQuadraticShader::calcHullCoverage(SkString* code, const char* coordAndGrad,
                                           const char* edge, const char* outputCoverage) const {
    code->appendf("float x = %s.x, y = %s.y;", coordAndGrad, coordAndGrad);
    code->appendf("float2 grad = %s.zw;", coordAndGrad);
    code->append ("float f = x*x - y;");
    code->append ("float fwidth = abs(grad.x) + abs(grad.y);");
    code->appendf("float curve_coverage = min(0.5 - f/fwidth, 1);");
    // Flat edge opposite the curve.
    code->appendf("float edge_coverage = min(%s, 0);", edge);
    // Total hull coverage.
    code->appendf("%s = max(half(curve_coverage + edge_coverage), 0);", outputCoverage);
>>>>>>> upstream-releases
}
