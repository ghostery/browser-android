/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkSVGCanvas.h"
#include "SkSVGDevice.h"
#include "SkMakeUnique.h"
#include "SkXMLWriter.h"

std::unique_ptr<SkCanvas> SkSVGCanvas::Make(const SkRect& bounds, SkWStream* writer) {
    // TODO: pass full bounds to the device
    SkISize size = bounds.roundOut().size();

<<<<<<< HEAD
    return skstd::make_unique<SkCanvas>(device);
}
||||||| merged common ancestors
    return skstd::make_unique<SkCanvas>(device.get());
}
=======
    auto svgDevice = SkSVGDevice::Make(size, skstd::make_unique<SkXMLStreamWriter>(writer));
>>>>>>> upstream-releases

    return svgDevice ? skstd::make_unique<SkCanvas>(svgDevice)
                     : nullptr;
}
