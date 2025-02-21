/*
 * Copyright (C) 2021-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "shared/source/ail/ail_configuration.h"
#include "shared/test/common/helpers/unit_test_helper.h"
#include "shared/test/common/helpers/variable_backup.h"
#include "shared/test/common/test_macros/test.h"

namespace NEO {
using IsSKL = IsProduct<IGFX_SKYLAKE>;
using IsDG2 = IsProduct<IGFX_DG2>;

using AILTests = ::testing::Test;
template <PRODUCT_FAMILY productFamily>
class AILMock : public AILConfigurationHw<productFamily> {
  public:
    using AILConfiguration::processName;
};

HWTEST2_F(AILTests, givenUninitializedTemplateWhenGetAILConfigurationThenNullptrIsReturned, IsSKL) {
    auto ailConfiguration = AILConfiguration::get(productFamily);

    ASSERT_EQ(nullptr, ailConfiguration);
}

HWTEST2_F(AILTests, givenInitilizedTemplateWhenApplyWithBlenderIsCalledThenFP64SupportIsEnabled, IsAtLeastGen12lp) {
    VariableBackup<AILConfiguration *> ailConfigurationBackup(&ailConfigurationTable[productFamily]);

    AILMock<productFamily> ailTemp;
    ailTemp.processName = "blender";
    ailConfigurationTable[productFamily] = &ailTemp;

    auto ailConfiguration = AILConfiguration::get(productFamily);
    ASSERT_NE(nullptr, ailConfiguration);

    NEO::RuntimeCapabilityTable rtTable = {};
    rtTable.ftrSupportsFP64 = false;

    ailConfiguration->apply(rtTable);

    EXPECT_EQ(rtTable.ftrSupportsFP64, true);
}

HWTEST2_F(AILTests, givenInitilizedTemplateWhenApplyWithWondershareFilmora11IsCalledThenBlitterSupportIsDisabled, IsDG2) {
    VariableBackup<AILConfiguration *> ailConfigurationBackup(&ailConfigurationTable[productFamily]);

    AILMock<productFamily> ailTemp;
    ailTemp.processName = "Wondershare Filmora 11";
    ailConfigurationTable[productFamily] = &ailTemp;

    auto ailConfiguration = AILConfiguration::get(productFamily);
    ASSERT_NE(nullptr, ailConfiguration);

    NEO::RuntimeCapabilityTable rtTable = {};
    rtTable.blitterOperationsSupported = true;

    ailConfiguration->apply(rtTable);

    EXPECT_EQ(rtTable.blitterOperationsSupported, false);
}

} // namespace NEO
