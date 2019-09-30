/*
 * Copyright 2019 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrBuffer_DEFINED
#define GrBuffer_DEFINED

#include "GrTypes.h"

/** Base class for a GPU buffer object or a client side arrays. */
class GrBuffer {
public:
    GrBuffer(const GrBuffer&) = delete;
    GrBuffer& operator=(const GrBuffer&) = delete;

    virtual ~GrBuffer() = default;

    // Our subclasses derive from different ref counting base classes. In order to use base
    // class pointers with sk_sp we virtualize ref() and unref().
    virtual void ref() const = 0;
    virtual void unref() const = 0;

    /** Size of the buffer in bytes. */
    virtual size_t size() const = 0;

    /** Is this an instance of GrCpuBuffer? Otherwise, an instance of GrGpuBuffer. */
    virtual bool isCpuBuffer() const = 0;

protected:
<<<<<<< HEAD
    GrBuffer(GrGpu*, size_t sizeInBytes, GrBufferType, GrAccessPattern);

    void* fMapPtr;

private:
    /**
     * Internal constructor to make a CPU-backed buffer.
     */
    GrBuffer(GrGpu*, size_t sizeInBytes, GrBufferType, void* cpuData);

    virtual void onMap() { SkASSERT(this->isCPUBacked()); fMapPtr = fCPUData; }
    virtual void onUnmap() { SkASSERT(this->isCPUBacked()); }
    virtual bool onUpdateData(const void* src, size_t srcSizeInBytes);

    size_t onGpuMemorySize() const override { return fSizeInBytes; } // TODO: zero for cpu backed?
    const char* getResourceType() const override { return "Buffer Object"; }
    void computeScratchKey(GrScratchKey* key) const override;

    size_t            fSizeInBytes;
    GrAccessPattern   fAccessPattern;
    void*             fCPUData;
    GrBufferType      fIntendedType;

    typedef GrGpuResource INHERITED;
||||||| merged common ancestors
    GrBuffer(GrGpu*, size_t sizeInBytes, GrBufferType, GrAccessPattern);

    void* fMapPtr;

private:
    /**
     * Internal constructor to make a CPU-backed buffer.
     */
    GrBuffer(GrGpu*, size_t sizeInBytes, GrBufferType, void* cpuData);

    virtual void onMap() { SkASSERT(this->isCPUBacked()); fMapPtr = fCPUData; }
    virtual void onUnmap() { SkASSERT(this->isCPUBacked()); }
    virtual bool onUpdateData(const void* src, size_t srcSizeInBytes);

    size_t onGpuMemorySize() const override { return fSizeInBytes; } // TODO: zero for cpu backed?
    void computeScratchKey(GrScratchKey* key) const override;

    size_t            fSizeInBytes;
    GrAccessPattern   fAccessPattern;
    void*             fCPUData;
    GrBufferType      fIntendedType;

    typedef GrGpuResource INHERITED;
=======
    GrBuffer() = default;
>>>>>>> upstream-releases
};

#endif
