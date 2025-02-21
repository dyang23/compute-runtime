/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "shared/source/helpers/product_config_helper.h"
#include "shared/test/common/test_macros/test.h"

using namespace NEO;
using ProductConfigHelperXeHpCoreTests = ::testing::Test;

XE_HP_CORE_TEST_F(ProductConfigHelperXeHpCoreTests, givenVariousVariantsOfXeHpAcronymsWhenGetReleaseThenCorrectValueIsReturned) {
    std::vector<std::string> acronymsVariants = {"xe_hp_core", "xe_hp", "xehp", "XeHp"};
    for (auto &acronym : acronymsVariants) {
        ProductConfigHelper::adjustDeviceName(acronym);
        auto ret = ProductConfigHelper::returnReleaseForAcronym(acronym);
        EXPECT_EQ(ret, AOT::XE_HP_RELEASE);
    }
}