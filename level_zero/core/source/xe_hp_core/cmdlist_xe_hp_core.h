/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once
#include "shared/source/xe_hp_core/hw_cmds_base.h"

#include "level_zero/core/source/cmdlist/cmdlist_hw.h"
#include "level_zero/core/source/cmdlist/cmdlist_hw_immediate.h"

namespace L0 {
template <PRODUCT_FAMILY productFamily>
struct CommandListProductFamily : public CommandListCoreFamily<IGFX_XE_HP_CORE> {
    using CommandListCoreFamily::CommandListCoreFamily;
};

template <PRODUCT_FAMILY gfxProductFamily>
struct CommandListImmediateProductFamily : public CommandListCoreFamilyImmediate<IGFX_XE_HP_CORE> {
    using CommandListCoreFamilyImmediate::CommandListCoreFamilyImmediate;
};
} // namespace L0
