#include "ComboPointFix.hpp"
#include "wxl/PluginApi.h"

#include <cstdint>

namespace
{
    constexpr const char* kPluginName = "wxl-combo-points";
    constexpr std::uint32_t kPluginVersion = 1;
}

extern "C" const WXL_PluginInfo* __cdecl WXL_Query(void)
{
    static const WXL_PluginInfo info = {
        sizeof(WXL_PluginInfo),
        WXL_API_VERSION,
        kPluginName,
        kPluginVersion,
        WXL_CLIENT_BUILD,
    };

    return &info;
}

extern "C" int __cdecl WXL_Load(const WXL_Api* api)
{
    if (!api || api->apiVersion != WXL_API_VERSION)
        return 0;

    if (wxl_combo_points::ApplyComboPointFix(api))
        return 1;

    api->Log(WXL_LOG_ERROR, kPluginName,
             "Combo-point fix was not applied; extension load aborted safely");
    return 0;
}
