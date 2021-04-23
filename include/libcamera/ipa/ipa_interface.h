/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2019, Google Inc.
 *
 * ipa_interface.h - Image Processing Algorithm interface
 */
#ifndef __LIBCAMERA_IPA_INTERFACE_H__
#define __LIBCAMERA_IPA_INTERFACE_H__

#include <stddef.h>
#include <stdint.h>

#include <map>
#include <vector>

#include <libcamera/buffer.h>
#include <libcamera/controls.h>
#include <libcamera/geometry.h>
#include <libcamera/signal.h>

#include "libcamera/internal/camera_sensor.h"

namespace libcamera {

/*
 * Structs that are defined in core.mojom and have the skipHeader tag must be
 * #included here.
 */

class IPAInterface
{
public:
	virtual ~IPAInterface() = default;
};

} /* namespace libcamera */

extern "C" {
libcamera::IPAInterface *ipaCreate();
}

#endif /* __LIBCAMERA_IPA_INTERFACE_H__ */
