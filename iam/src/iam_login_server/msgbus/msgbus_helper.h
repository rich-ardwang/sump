/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : msgbus_helper.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-09-24
*Desc        : message helpers for msgbus.
*History     : Re-edit by Lei.Wang 2019-09-24
***************************************************************************************************/
#pragma once
#include <string>
#include <msgbus/message.h>


class MessageHelper {
public:
    static std::string getMessageDescriptor(const msgbus::Message &message);
    static std::string getMapDescriptor(const msgbus::VariantMap &map, unsigned depth = 0);
    static std::string getListDescriptor(const msgbus::VariantList &lst, unsigned depth = 0);
    static std::string getVariantDescriptor(const msgbus::Variant &var, unsigned depth = 0);
};
