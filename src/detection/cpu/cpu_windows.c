#include "cpu.h"
#include "util/windows/register.h"
#include "util/mallocHelper.h"

void ffDetectCPUImpl(const FFinstance* instance, FFCPUResult* cpu, bool cached)
{
    FF_UNUSED(instance);

    cpu->temperature = FF_CPU_TEMP_UNSET;

    if(cached)
        return;

    cpu->coresPhysical = cpu->coresLogical = cpu->coresOnline = 0;
    cpu->frequencyMax = cpu->frequencyMin = 0;
    ffStrbufInit(&cpu->name);
    ffStrbufInit(&cpu->vendor);

    {
        DWORD length = 0;
        GetLogicalProcessorInformationEx(RelationProcessorCore, NULL, &length);
        SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX* FF_AUTO_FREE
            pLogicalInfo = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX*)malloc(length);

        if(pLogicalInfo && GetLogicalProcessorInformationEx(RelationProcessorCore, pLogicalInfo, &length))
        {
            for(
                SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX* ptr = pLogicalInfo;
                (uint8_t*)ptr < ((uint8_t*)pLogicalInfo) + length;
                ptr = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX*)(((uint8_t*)ptr) + ptr->Size)
            )
            {
                if(ptr->Relationship == RelationProcessorCore)
                    ++cpu->coresPhysical;
            }
        }
    }
    cpu->coresOnline = (uint16_t)GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);
    cpu->coresLogical = (uint16_t)GetMaximumProcessorCount(ALL_PROCESSOR_GROUPS);

    FF_HKEY_AUTO_DESTROY hKey;
    if(!ffRegOpenKeyForRead(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", &hKey, NULL))
        return;

    {
        uint32_t mhz;
        if(ffRegReadUint(hKey, "~MHz", &mhz, NULL))
            cpu->frequencyMax = mhz / 1000.0;
    }

    ffRegReadStrbuf(hKey, "ProcessorNameString", &cpu->name, NULL);
    ffRegReadStrbuf(hKey, "VendorIdentifier", &cpu->vendor, NULL);
}
