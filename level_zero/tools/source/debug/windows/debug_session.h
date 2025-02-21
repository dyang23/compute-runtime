/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once

#include "shared/source/os_interface/windows/wddm/wddm.h"

#include "level_zero/core/source/device/device.h"
#include "level_zero/tools/source/debug/debug_session.h"
#include "level_zero/tools/source/debug/debug_session_imp.h"

#include "KmEscape.h"

#include <unordered_set>

namespace L0 {

struct DebugSessionWindows : DebugSessionImp {
    DebugSessionWindows(const zet_debug_config_t &config, Device *device) : DebugSessionImp(config, device), processId(config.pid) {}
    ~DebugSessionWindows() override;

    ze_result_t initialize() override;
    bool closeConnection() override;

    ze_result_t readEvent(uint64_t timeout, zet_debug_event_t *event) override;
    ze_result_t readMemory(ze_device_thread_t thread, const zet_debug_memory_space_desc_t *desc, size_t size, void *buffer) override;
    ze_result_t writeMemory(ze_device_thread_t thread, const zet_debug_memory_space_desc_t *desc, size_t size, const void *buffer) override;
    ze_result_t acknowledgeEvent(const zet_debug_event_t *event) override;

    static ze_result_t translateEscapeReturnStatusToZeResult(uint32_t escapeErrorStatus);

  protected:
    ze_result_t resumeImp(std::vector<ze_device_thread_t> threads, uint32_t deviceIndex) override;
    ze_result_t interruptImp(uint32_t deviceIndex) override;

    ze_result_t readGpuMemory(uint64_t memoryHandle, char *output, size_t size, uint64_t gpuVa) override;
    ze_result_t writeGpuMemory(uint64_t memoryHandle, const char *input, size_t size, uint64_t gpuVa) override;

    ze_result_t readSbaBuffer(EuThread::ThreadId, SbaTrackedAddresses &sbaBuffer) override;

    MOCKABLE_VIRTUAL ze_result_t readAndHandleEvent(uint64_t timeoutMs);
    ze_result_t handleModuleCreateEvent(DBGUMD_READ_EVENT_MODULE_CREATE_EVENT_PARAMS &moduleCreateParams);
    ze_result_t handleEuAttentionBitsEvent(DBGUMD_READ_EVENT_EU_ATTN_BIT_SET_PARAMS &euAttentionBitsParams);
    ze_result_t handleAllocationDataEvent(DBGUMD_READ_EVENT_READ_ALLOCATION_DATA_PARAMS &allocationDataParams);
    ze_result_t handleContextCreateDestroyEvent(DBGUMD_READ_EVENT_CONTEXT_CREATE_DESTROY_EVENT_PARAMS &contextCreateDestroyParams);
    ze_result_t handleDeviceCreateDestroyEvent(DBGUMD_READ_EVENT_DEVICE_CREATE_DESTROY_EVENT_PARAMS &deviceCreateDestroyParams);
    ze_result_t handleCreateDebugDataEvent(DBGUMD_READ_EVENT_CREATE_DEBUG_DATA_PARAMS &createDebugDataParams);

    void enqueueApiEvent(zet_debug_event_t &debugEvent) override;
    bool readSystemRoutineIdent(EuThread *thread, uint64_t vmHandle, SIP::sr_ident &srMagic) override;
    bool readModuleDebugArea() override;
    void startAsyncThread() override;
    void closeAsyncThread();
    static void *asyncThreadFunction(void *arg);

    ThreadHelper asyncThread;
    std::mutex asyncThreadMutex;

    NTSTATUS runEscape(KM_ESCAPE_INFO &escapeInfo);

    bool moduleDebugAreaCaptured = false;
    uint32_t processId = 0;
    uint64_t debugHandle = 0;
    NEO::Wddm *wddm = nullptr;

    struct ElfRange {
        uint64_t startVA;
        uint64_t endVA;
    };

    std::unordered_set<uint64_t> allContexts;
    std::vector<ElfRange> allElfs;
};

} // namespace L0
