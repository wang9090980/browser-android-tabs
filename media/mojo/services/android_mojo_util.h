// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MOJO_SERVICES_ANDROID_MOJO_UTIL_H_
#define MEDIA_MOJO_SERVICES_ANDROID_MOJO_UTIL_H_

#include <memory>

#include "media/mojo/services/mojo_media_drm_storage.h"
#include "media/mojo/services/mojo_provision_fetcher.h"

namespace service_manager {
namespace mojom {

class InterfaceProvider;

}  // namespace mojom
}  // namespace service_manager

namespace media {
namespace android_mojo_util {

std::unique_ptr<ProvisionFetcher> CreateProvisionFetcher(
    service_manager::mojom::InterfaceProvider* host_interfaces);

std::unique_ptr<MediaDrmStorage> CreateMediaDrmStorage(
    service_manager::mojom::InterfaceProvider* host_interfaces);

}  // namespace android_mojo_util
}  // namespace media

#endif  // MEDIA_MOJO_SERVICES_ANDROID_MOJO_UTIL_H_
