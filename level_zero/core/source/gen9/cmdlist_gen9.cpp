/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "level_zero/core/source/gen9/cmdlist_gen9.inl"

#include "shared/source/gen9/hw_cmds_base.h"

#include "level_zero/core/source/cmdlist/cmdlist_hw.h"
#include "level_zero/core/source/cmdlist/cmdlist_hw.inl"
#include "level_zero/core/source/cmdlist/cmdlist_hw_base.inl"
#include "level_zero/core/source/cmdlist/cmdlist_hw_immediate.h"
#include "level_zero/core/source/cmdlist/cmdlist_hw_immediate.inl"

#include "cmdlist_extended.inl"

namespace L0 {
template struct CommandListCoreFamily<IGFX_GEN9_CORE>;
template struct CommandListCoreFamilyImmediate<IGFX_GEN9_CORE>;
} // namespace L0
