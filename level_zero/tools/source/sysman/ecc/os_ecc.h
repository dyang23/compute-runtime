/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once

#include "level_zero/tools/source/sysman/os_sysman.h"
#include <level_zero/zes_api.h>

namespace L0 {

class OsEcc {
  public:
    virtual ze_result_t deviceEccAvailable(ze_bool_t *pAvailable) = 0;
    virtual ze_result_t deviceEccConfigurable(ze_bool_t *pConfigurable) = 0;
    virtual ze_result_t getEccState(zes_device_ecc_properties_t *pState) = 0;
    virtual ze_result_t setEccState(const zes_device_ecc_desc_t *newState, zes_device_ecc_properties_t *pState) = 0;
    static OsEcc *create(OsSysman *pOsSysman);
    virtual ~OsEcc() = default;
};

} // namespace L0
