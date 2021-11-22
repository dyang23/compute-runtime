/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once
#include "shared/test/common/helpers/test_traits.h"

template <>
struct TestTraits<IGFX_XE_HPG_CORE> {
    static constexpr bool surfaceStateCompressionParamsSupported = true;
    static constexpr bool clearColorAddressMatcher = true;
    static constexpr bool auxBuiltinsSupported = true;
    static constexpr bool localMemCompressionAubsSupported = true;
    static constexpr bool systemMemCompressionAubsSupported = false;
    static constexpr bool l3ControlSupported = true;
    static constexpr bool forceNonCoherentSupported = true;
    static constexpr bool threadPreemptionDisableBitMatcher = true;
    static constexpr bool programOnlyChangedFieldsInComputeStateMode = true;
    static constexpr bool iohInSbaSupported = false;
    static constexpr bool auxTranslationSupported = true;
    static constexpr bool deviceEnqueueSupport = false;
    static constexpr bool isUsingNonDefaultIoctls = true;
};
