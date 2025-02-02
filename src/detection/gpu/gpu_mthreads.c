#include "gpu_driver_specific.h"

#include "common/library.h"
#include "mtml.h"

struct FFMtmlData
{
    FF_LIBRARY_SYMBOL(mtmlDeviceGetBrand)
    FF_LIBRARY_SYMBOL(mtmlDeviceGetIndex)
    FF_LIBRARY_SYMBOL(mtmlDeviceGetName)
    FF_LIBRARY_SYMBOL(mtmlDeviceGetPciInfo)
    FF_LIBRARY_SYMBOL(mtmlDeviceGetUUID)
    FF_LIBRARY_SYMBOL(mtmlDeviceInitGpu)
    FF_LIBRARY_SYMBOL(mtmlDeviceInitMemory)
    FF_LIBRARY_SYMBOL(mtmlGpuGetMaxClock)
    FF_LIBRARY_SYMBOL(mtmlGpuGetTemperature)
    FF_LIBRARY_SYMBOL(mtmlGpuGetUtilization)
    FF_LIBRARY_SYMBOL(mtmlLibraryCountDevice)
    FF_LIBRARY_SYMBOL(mtmlLibraryInitDeviceByIndex)
    FF_LIBRARY_SYMBOL(mtmlLibraryInitDeviceByPciSbdf)
    FF_LIBRARY_SYMBOL(mtmlLibraryInitSystem)
    FF_LIBRARY_SYMBOL(mtmlMemoryGetTotal)
    FF_LIBRARY_SYMBOL(mtmlMemoryGetUsed)
    FF_LIBRARY_SYMBOL(mtmlMemoryGetUtilization)

    bool inited;
    MtmlLibrary *lib;
    MtmlSystem *sys;
} mtmlData;

const char *ffDetectMthreadsGpuCount(uint32_t *result, const char *soName)
{
#ifndef FF_DISABLE_DLOPEN

    if (!mtmlData.inited)
    {
        mtmlData.inited = true;
        FF_LIBRARY_LOAD(libmtml, NULL, "dlopen mtml failed", soName, 1);
        FF_LIBRARY_LOAD_SYMBOL_MESSAGE(libmtml, mtmlLibraryInit)
        FF_LIBRARY_LOAD_SYMBOL_MESSAGE(libmtml, mtmlLibraryShutDown)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlDeviceGetBrand)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlDeviceGetIndex)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlDeviceGetName)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlDeviceGetPciInfo)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlDeviceGetUUID)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlDeviceInitGpu)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlDeviceInitMemory)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlGpuGetMaxClock)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlGpuGetTemperature)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlGpuGetUtilization)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlLibraryCountDevice)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlLibraryInitDeviceByIndex)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlLibraryInitDeviceByPciSbdf)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlLibraryInitSystem)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlMemoryGetTotal)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlMemoryGetUsed)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlMemoryGetUtilization)

        if (ffmtmlLibraryInit(&mtmlData.lib) != MTML_SUCCESS)
        {
            mtmlData.ffmtmlLibraryInitSystem = NULL;
            return "mtmlLibraryInit failed";
        }
        if (mtmlData.ffmtmlLibraryInitSystem(mtmlData.lib, &mtmlData.sys) != MTML_SUCCESS)
        {
            mtmlData.ffmtmlLibraryInitSystem = NULL;
            return "mtmlLibraryInitSystem failed";
        }
        atexit((void *)ffmtmlLibraryShutDown);
        libmtml = NULL; // don't close mtml
    }

    if (mtmlData.ffmtmlLibraryInitSystem == NULL)
        return "loading mtml library failed";

    uint32_t count;
    if (mtmlData.ffmtmlLibraryCountDevice(mtmlData.lib, &count) != MTML_SUCCESS)
        return "mtmlLibraryCountDevice() failed";

    *result = count;
    return NULL;
#else

    FF_UNUSED(cond, result, soName);
    return "dlopen is disabled";

#endif
}

const char *ffDetectMthreadsGpuInfo(const FFGpuDriverCondition *cond, FFGpuDriverResult result, const char *soName)
{
#ifndef FF_DISABLE_DLOPEN

    if (!mtmlData.inited)
    {
        mtmlData.inited = true;
        FF_LIBRARY_LOAD(libmtml, NULL, "dlopen mtml failed", soName, 1);
        FF_LIBRARY_LOAD_SYMBOL_MESSAGE(libmtml, mtmlLibraryInit)
        FF_LIBRARY_LOAD_SYMBOL_MESSAGE(libmtml, mtmlLibraryShutDown)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlDeviceGetBrand)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlDeviceGetIndex)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlDeviceGetName)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlDeviceGetPciInfo)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlDeviceGetUUID)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlDeviceInitGpu)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlDeviceInitMemory)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlGpuGetMaxClock)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlGpuGetTemperature)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlGpuGetUtilization)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlLibraryCountDevice)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlLibraryInitDeviceByIndex)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlLibraryInitDeviceByPciSbdf)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlLibraryInitSystem)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlMemoryGetTotal)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlMemoryGetUsed)
        FF_LIBRARY_LOAD_SYMBOL_VAR_MESSAGE(libmtml, mtmlData, mtmlMemoryGetUtilization)

        if (ffmtmlLibraryInit(&mtmlData.lib) != MTML_SUCCESS)
        {
            mtmlData.ffmtmlLibraryInitSystem = NULL;
            return "mtmlLibraryInit failed";
        }
        if (mtmlData.ffmtmlLibraryInitSystem(mtmlData.lib, &mtmlData.sys) != MTML_SUCCESS)
        {
            mtmlData.ffmtmlLibraryInitSystem = NULL;
            return "mtmlLibraryInitSystem failed";
        }
        atexit((void *)ffmtmlLibraryShutDown);
        libmtml = NULL; // don't close mtml
    }

    if (mtmlData.ffmtmlLibraryInitSystem == NULL)
        return "loading mtml library failed";

    MtmlDevice *device = NULL;
    if (cond->type & FF_GPU_DRIVER_CONDITION_TYPE_BUS_ID)
    {
        char pciBusIdStr[32];
        snprintf(pciBusIdStr, sizeof(pciBusIdStr) - 1, "%04x:%02x:%02x.%d", cond->pciBusId.domain, cond->pciBusId.bus, cond->pciBusId.device, cond->pciBusId.func);

        MtmlReturn ret = mtmlData.ffmtmlLibraryInitDeviceByPciSbdf(mtmlData.lib, pciBusIdStr, &device);
        if (ret != MTML_SUCCESS)
            return "mtmlLibraryInitDeviceByPciSbdf() failed";
    }
    else if (cond->type & FF_GPU_DRIVER_CONDITION_TYPE_DEVICE_ID)
    {
        uint32_t count;
        if (mtmlData.ffmtmlLibraryCountDevice(mtmlData.lib, &count) != MTML_SUCCESS)
            return "mtmlLibraryCountDevice() failed";

        for (uint32_t i = 0; i < count; i++, device = NULL)
        {
            if (mtmlData.ffmtmlLibraryInitDeviceByIndex(mtmlData.lib, i, &device) != MTML_SUCCESS)
                continue;

            MtmlPciInfo pciInfo;
            if (mtmlData.ffmtmlDeviceGetPciInfo(device, &pciInfo) != MTML_SUCCESS)
                continue;

            if (pciInfo.pciDeviceId != ((cond->pciDeviceId.deviceId << 16u) | cond->pciDeviceId.vendorId) ||
                pciInfo.pciSubsystemId != cond->pciDeviceId.subSystemId || (cond->pciDeviceId.bus != FF_GPU_BUS_UNSET && (uint32_t)pciInfo.bus != cond->pciDeviceId.bus))
                continue;

            break;
        }
        if (!device)
            return "Device not found";
    }
    else if (cond->type & FF_GPU_DRIVER_CONDITION_TYPE_INDEX)
    {
        if (mtmlData.ffmtmlLibraryInitDeviceByIndex(mtmlData.lib, (unsigned int)cond->index, &device) != MTML_SUCCESS)
            return "mtmlLibraryInitDeviceByIndex() failed";
    }
    else
    {
        return "Unknown condition type";
    }

    MtmlBrandType brand;
    if (mtmlData.ffmtmlDeviceGetBrand(device, &brand) == MTML_SUCCESS)
    {
        switch (brand)
        {
        case MTML_BRAND_MTT:
            *result.type = FF_GPU_TYPE_DISCRETE;
            break;
        default:
            break;
        }
    }

    if (result.index)
    {
        unsigned int value;
        if (mtmlData.ffmtmlDeviceGetIndex(device, &value) == MTML_SUCCESS)
            *result.index = (uint8_t)value;
    }

    if (result.temp)
    {
        MtmlGpu *gpu = NULL;
        if (mtmlData.ffmtmlDeviceInitGpu(device, &gpu) == MTML_SUCCESS)
        {
            uint32_t value;
            if (mtmlData.ffmtmlGpuGetTemperature(gpu, &value) == MTML_SUCCESS)
                *result.temp = value;
        }
    }

    if (result.memory)
    {
        MtmlMemory *mem = NULL;
        if (mtmlData.ffmtmlDeviceInitMemory(device, &mem) == MTML_SUCCESS)
        {
            unsigned long long total;
            if (mtmlData.ffmtmlMemoryGetTotal(mem, &total) == MTML_SUCCESS)
                result.memory->total = total;

            unsigned long long used;
            if (mtmlData.ffmtmlMemoryGetUsed(mem, &used) == MTML_SUCCESS)
                result.memory->used = used;
        }
    }

    if (result.frequency)
    {
        MtmlGpu *gpu = NULL;
        if (mtmlData.ffmtmlDeviceInitGpu(device, &gpu) == MTML_SUCCESS)
        {
            uint32_t clockMHz;
            if (mtmlData.ffmtmlGpuGetMaxClock(gpu, &clockMHz) == MTML_SUCCESS)
                *result.frequency = clockMHz / 1000.;
        }
    }

    if (result.coreUtilizationRate)
    {
        MtmlGpu *gpu = NULL;
        if (mtmlData.ffmtmlDeviceInitGpu(device, &gpu) == MTML_SUCCESS)
        {
            unsigned int utilization;
            if (mtmlData.ffmtmlGpuGetUtilization(gpu, &utilization) == MTML_SUCCESS)
                *result.coreUtilizationRate = utilization;
        }
    }

    if (result.uuid)
    {
        char uuid[MTML_DEVICE_UUID_BUFFER_SIZE];
        if (mtmlData.ffmtmlDeviceGetUUID(device, uuid, sizeof(uuid)) == MTML_SUCCESS)
            ffStrbufAppendS(result.uuid, uuid);
    }

    if (result.name)
    {
        char name[MTML_DEVICE_NAME_BUFFER_SIZE];
        if (mtmlData.ffmtmlDeviceGetName(device, name, sizeof(name)) == MTML_SUCCESS)
            ffStrbufSetS(result.name, name);
    }

    return NULL;

#else

    FF_UNUSED(cond, result, soName);
    return "dlopen is disabled";

#endif
}
