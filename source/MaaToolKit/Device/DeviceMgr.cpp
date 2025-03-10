#include "DeviceMgr.h"

#include "Utils/Logger.h"

MAA_TOOLKIT_DEVICE_NS_BEGIN

std::string_view DeviceMgr::device_name(size_t index) const
{
    if (index >= devices_.size()) {
        LogError << "Out of range" << VAR(index);
        return {};
    }

    return devices_.at(index).name;
}

std::string_view DeviceMgr::device_adb_path(size_t index) const
{
    if (index >= devices_.size()) {
        LogError << "Out of range" << VAR(index);
        return {};
    }

    return devices_.at(index).adb_path;
}

std::string_view DeviceMgr::device_adb_serial(size_t index) const
{
    if (index >= devices_.size()) {
        LogError << "Out of range" << VAR(index);
        return {};
    }

    return devices_.at(index).adb_serial;
}

MaaAdbControllerType DeviceMgr::device_adb_controller_type(size_t index) const
{
    if (index >= devices_.size()) {
        LogError << "Out of range" << VAR(index);
        return MaaAdbControllerType_Invalid;
    }

    return devices_.at(index).adb_controller_type;
}

std::string_view DeviceMgr::device_adb_config(size_t index) const
{
    if (index >= devices_.size()) {
        LogError << "Out of range" << VAR(index);
        return {};
    }

    return devices_.at(index).adb_config;
}

MAA_TOOLKIT_DEVICE_NS_END
