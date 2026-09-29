// wxl-combo-points: enables the stock 3.3.5a combo-point path for every class.
// The patch is the same one used by WotLK-Extensions: change the conditional
// jump at client VA 0x611707 from 0x74 (JE) to 0xEB (JMP short).

#include "ComboPointFix.hpp"
#include "wxl/PluginApi.h"

#include <windows.h>
#include <cstdint>

namespace wxl_combo_points
{
namespace
{
    // WotLK-Extensions patches virtual address 0x611707 in the stock
    // 3.3.5a / 12340 client. The PE image base of the supplied client is
    // 0x00400000, so the stable value to keep in the module is the RVA.
    constexpr uintptr_t kClientImageBase = 0x00400000u;
    constexpr uintptr_t kPatchVirtualAddress = 0x00611707u;
    constexpr uintptr_t kPatchRva = kPatchVirtualAddress - kClientImageBase;

    constexpr std::uint8_t kOriginalOpcode = 0x74; // JE rel8
    constexpr std::uint8_t kPatchedOpcode = 0xEB;  // JMP rel8

    constexpr const char* kLogTag = "wxl-combo-points";
}

bool ApplyComboPointFix(const WXL_Api* api)
{
    if (!api)
        return false;

    const uintptr_t moduleBase = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if (!moduleBase)
    {
        api->Log(WXL_LOG_ERROR, kLogTag, "GetModuleHandleW(nullptr) failed");
        return false;
    }

    auto* patchAddress = reinterpret_cast<std::uint8_t*>(moduleBase + kPatchRva);
    const std::uint8_t current = *patchAddress;

    // Make the patch idempotent and fail closed if this is not the expected
    // 3.3.5a client image. We do not overwrite an unknown byte.
    if (current == kPatchedOpcode)
    {
        api->Log(WXL_LOG_INFO, kLogTag,
                 "Combo-point fix already applied at VA 0x00611707");
        return true;
    }

    if (current != kOriginalOpcode)
    {
        api->Log(WXL_LOG_ERROR, kLogTag,
                 "Refusing combo-point patch: expected 0x74 at VA 0x00611707, found 0x%02X",
                 static_cast<unsigned>(current));
        return false;
    }

    DWORD oldProtection = 0;
    if (!VirtualProtect(patchAddress, 1, PAGE_EXECUTE_READWRITE, &oldProtection))
    {
        api->Log(WXL_LOG_ERROR, kLogTag,
                 "VirtualProtect failed for combo-point patch (Win32 error %lu)",
                 static_cast<unsigned long>(GetLastError()));
        return false;
    }

    *patchAddress = kPatchedOpcode;
    FlushInstructionCache(GetCurrentProcess(), patchAddress, 1);

    DWORD ignoredProtection = 0;
    const BOOL restored = VirtualProtect(patchAddress, 1, oldProtection, &ignoredProtection);
    if (!restored)
    {
        api->Log(WXL_LOG_WARN, kLogTag,
                 "Combo-point patch applied, but restoring page protection failed (Win32 error %lu)",
                 static_cast<unsigned long>(GetLastError()));
    }

    api->Log(WXL_LOG_INFO, kLogTag,
             "Combo-point fix applied: VA 0x00611707 0x74 -> 0xEB (JE -> JMP)");
    return true;
}

} // namespace wxl_combo_points
