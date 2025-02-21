/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "level_zero/tools/source/sysman/ecc/linux/os_ecc_imp.h"

#include "level_zero/tools/source/sysman/ecc/ecc_imp.h"

namespace L0 {

zes_device_ecc_state_t LinuxEccImp::getEccState(uint8_t state) {
    switch (state) {
    case eccStateEnable:
        return ZES_DEVICE_ECC_STATE_ENABLED;
    case eccStateDisable:
        return ZES_DEVICE_ECC_STATE_DISABLED;
    default:
        return ZES_DEVICE_ECC_STATE_UNAVAILABLE;
    }
}

ze_result_t LinuxEccImp::deviceEccAvailable(ze_bool_t *pAvailable) {
    FirmwareUtil *pFwInterface = pLinuxSysmanImp->getFwUtilInterface();
    if (pFwInterface == nullptr) {
        return ZE_RESULT_ERROR_UNSUPPORTED_FEATURE;
    }

    *pAvailable = false;
    uint8_t currentState = 0;
    uint8_t pendingState = 0;
    ze_result_t result = pFwInterface->fwGetEccConfig(&currentState, &pendingState);
    if (ZE_RESULT_SUCCESS == result) {
        if ((currentState != eccStateNone) && (pendingState != eccStateNone)) {
            *pAvailable = true;
        }
    }

    return result;
}

ze_result_t LinuxEccImp::deviceEccConfigurable(ze_bool_t *pConfigurable) {
    return deviceEccAvailable(pConfigurable);
}

ze_result_t LinuxEccImp::getEccState(zes_device_ecc_properties_t *pState) {
    FirmwareUtil *pFwInterface = pLinuxSysmanImp->getFwUtilInterface();
    if (pFwInterface == nullptr) {
        return ZE_RESULT_ERROR_UNSUPPORTED_FEATURE;
    }

    uint8_t currentState = 0;
    uint8_t pendingState = 0;
    ze_result_t result = pFwInterface->fwGetEccConfig(&currentState, &pendingState);
    if (result != ZE_RESULT_SUCCESS) {
        return result;
    }
    pState->currentState = getEccState(currentState);
    pState->pendingState = getEccState(pendingState);

    pState->pendingAction = ZES_DEVICE_ACTION_WARM_CARD_RESET;
    if (pState->currentState == pState->pendingState) {
        pState->pendingAction = ZES_DEVICE_ACTION_NONE;
    }

    return result;
}

ze_result_t LinuxEccImp::setEccState(const zes_device_ecc_desc_t *newState, zes_device_ecc_properties_t *pState) {
    FirmwareUtil *pFwInterface = pLinuxSysmanImp->getFwUtilInterface();
    if (pFwInterface == nullptr) {
        return ZE_RESULT_ERROR_UNSUPPORTED_FEATURE;
    }

    uint8_t state = 0;
    uint8_t currentState = 0;
    uint8_t pendingState = 0;
    if (newState->state == ZES_DEVICE_ECC_STATE_ENABLED) {
        state = eccStateEnable;
    } else if (newState->state == ZES_DEVICE_ECC_STATE_DISABLED) {
        state = eccStateDisable;
    } else {
        return ZE_RESULT_ERROR_INVALID_ENUMERATION;
    }

    ze_result_t result = pFwInterface->fwSetEccConfig(state, &currentState, &pendingState);
    if (result != ZE_RESULT_SUCCESS) {
        return result;
    }

    pState->currentState = getEccState(currentState);
    pState->pendingState = getEccState(pendingState);

    pState->pendingAction = ZES_DEVICE_ACTION_WARM_CARD_RESET;
    if (pState->currentState == pState->pendingState) {
        pState->pendingAction = ZES_DEVICE_ACTION_NONE;
    }

    return result;
}

LinuxEccImp::LinuxEccImp(OsSysman *pOsSysman) {
    pLinuxSysmanImp = static_cast<LinuxSysmanImp *>(pOsSysman);
}

OsEcc *OsEcc::create(OsSysman *pOsSysman) {
    LinuxEccImp *pLinuxEccImp = new LinuxEccImp(pOsSysman);
    return static_cast<OsEcc *>(pLinuxEccImp);
}

} // namespace L0
