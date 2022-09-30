#include <sstream>
#include <boost/lexical_cast.hpp>
#include "msgbus_helper.h"


class SerialHelper {
public:
    static std::string getBlankByDepth(unsigned depth);
    static std::string getTypeDescriptor(int type);

    //serialize commom variable
    static void serialize_var(const msgbus::Variant &var, unsigned depth, std::stringstream &stream);
    static void serialize_var(const msgbus::VariantMap &map, unsigned depth, const std::string &key, 
        int type, std::stringstream &stream);
    //serialize list
    static void serialize_list(const msgbus::VariantList &lst, unsigned depth, std::stringstream &stream);
    //serialize map
    static void serialize_map(const msgbus::VariantMap &map, unsigned depth, std::stringstream &stream);
};

std::string SerialHelper::getBlankByDepth(unsigned depth) {
    std::string blank;
    for (unsigned i = 0; i < depth; i++) {
        blank += "--";
    }
    return blank;
}

std::string SerialHelper::getTypeDescriptor(int type) {
    switch(type) {
    case msgbus::Message::FIELD_BOOL:
        return std::string("BOOL");
    case msgbus::Message::FIELD_UINT8:
        return std::string("UINT8");
    case msgbus::Message::FIELD_UINT16:
        return std::string("UINT16");
    case msgbus::Message::FIELD_UINT32:
        return std::string("UINT32");
    case msgbus::Message::FIELD_UINT64:
        return std::string("UINT64");
    case msgbus::Message::FIELD_INT8:
        return std::string("INT8");
    case msgbus::Message::FIELD_INT16:
        return std::string("INT16");
    case msgbus::Message::FIELD_INT32:
        return std::string("INT32");
    case msgbus::Message::FIELD_INT64:
        return std::string("INT64");
    case msgbus::Message::FIELD_FLOAT:
        return std::string("FLOAT");
    case msgbus::Message::FIELD_DOUBLE:
        return std::string("DOUBLE");
    case msgbus::Message::FIELD_STRING:
        return std::string("STRING");
    case msgbus::Message::FIELD_MESSAGE:
        return std::string("MAP");
    case msgbus::Message::FIELD_LIST:
        return std::string("LIST");
    default:
        return std::string("UNKNOWN");
    }
}

void SerialHelper::serialize_var(const msgbus::Variant &var, unsigned depth, std::stringstream &stream) {
    std::string blank = getBlankByDepth(depth);
    switch (var.Type) {
    case msgbus::Variant::VAR_BOOL:
        {
            bool ret = false;
            var.GetBool(ret);
            std::string boolstr = ret ? "true" : "false";
            stream << blank << "<BOOL>" << "(" << boolstr << ")\n";
        }
        break;
    case msgbus::Variant::VAR_UINT8:
        {
            msgbus::uint8_t ret;
            var.GetUInt8(ret);
            stream << blank << "<UINT8>" << "(" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_UINT16:
        {
            msgbus::uint16_t ret;
            var.GetUInt16(ret);
            stream << blank << "<UINT16>" << "(" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_UINT32:
        {
            msgbus::uint32_t ret;
            var.GetUInt32(ret);
            stream << blank << "<UINT32>" << "(" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_UINT64:
        {
            msgbus::uint64_t ret;
            var.GetUInt64(ret);
            stream << blank << "<UINT64>" << "(" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_INT8:
        {
            msgbus::int8_t ret;
            var.GetInt8(ret);
            stream << blank << "<INT8>" << "(" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_INT16:
        {
            msgbus::int16_t ret;
            var.GetInt16(ret);
            stream << blank << "<INT16>" << "(" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_INT32:
        {
            msgbus::int32_t ret;
            var.GetInt32(ret);
            stream << blank << "<INT32>" << "(" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_INT64:
        {
            msgbus::int64_t ret;
            var.GetInt64(ret);
            stream << blank << "<INT64>" << "(" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_FLOAT:
        {
            float ret;
            var.GetFloat(ret);
            stream << blank << "<FLOAT>" << "(" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_DOUBLE:
        {
            double ret;
            var.GetDouble(ret);
            stream << blank << "<DOUBLE>" << "(" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_STRING:
        {
            std::string ret;
            var.GetString(ret);
            stream << blank << "<STRING>" << "(" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_MAP:
        {
            const msgbus::VariantMap *map = NULL;
            var.RefMap(map);
            stream << blank << "<MAP>\n";
            serialize_map(*map, depth + 1, stream);
        }
        break;
    case msgbus::Variant::VAR_LIST:
        {
            const msgbus::VariantList *lst = NULL;
            var.RefList(lst);
            stream << blank << "<LIST>\n";
            serialize_list(*lst, depth + 1, stream);
        }
        break;
    default:
        stream << blank << "<Unknown>()";
    }
}

void SerialHelper::serialize_var(const msgbus::VariantMap &map, unsigned depth, const std::string &key, 
    int type, std::stringstream &stream) {
    std::string blank = getBlankByDepth(depth);
    switch(type) {
    case msgbus::Variant::VAR_BOOL:
        {
            bool ret;
            map.getBool(key, ret);
            std::string boolstr = ret ? "true" : "false";
            stream << blank << "<BOOL>[" << key << "](" << boolstr << ")\n";
        }
        break;
    case msgbus::Variant::VAR_UINT8:
        {
            msgbus::uint8_t ret;
            map.getUint8(key, ret);
            msgbus::uint32_t ret32 = ret;
            stream << blank << "<UINT8>[" << key << "](" << ret32 << ")\n";
        }
        break;
    case msgbus::Variant::VAR_UINT16:
        {
            msgbus::uint16_t ret;
            map.getUint16(key, ret);
            stream << blank << "<UINT16>[" << key << "](" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_UINT32:
        {
            msgbus::uint32_t ret;
            map.getUint32(key, ret);
            stream << blank << "<UINT32>[" << key << "](" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_UINT64:
        {
            msgbus::uint64_t ret;
            map.getUint64(key, ret);
            stream << blank << "<UINT64>[" << key << "](" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_INT8:
        {
            msgbus::int8_t ret;
            map.getInt8(key, ret);
            msgbus::int32_t ret32 = ret;
            stream << blank << "<INT8>[" << key << "](" << ret32 << ")\n";
        }
        break;
    case msgbus::Variant::VAR_INT16:
        {
            msgbus::int16_t ret;
            map.getInt16(key, ret);
            stream << blank << "<INT16>[" << key << "](" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_INT32:
        {
            msgbus::int32_t ret;
            map.getInt32(key, ret);
            stream << blank << "<INT32>[" << key << "](" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_INT64:
        {
            msgbus::int64_t ret;
            map.getInt64(key, ret);
            stream << blank << "<INT64>[" << key << "](" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_FLOAT:
        {
            float ret;
            map.getFloat(key, ret);
            stream << blank << "<FLOAT>[" << key << "](" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_DOUBLE:
        {
            double ret;
            map.getDouble(key, ret);
            stream << blank << "<DOUBLE>[" << key << "](" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_STRING:
        {
            std::string ret;
            map.getString(key, ret);
            stream << blank << "<STRING>[" << key << "](" << ret << ")\n";
        }
        break;
    case msgbus::Variant::VAR_MAP:
        {
            const msgbus::Message::Map *ret = NULL;
            map.RefMap(key.c_str(), ret);
            if (!ret) return;
            stream << blank << "<MAP>[" << key << "]\n" ;
            serialize_map(*ret, depth + 1, stream);
        }
        break;
    case msgbus::Variant::VAR_LIST:
        {
            const msgbus::VariantList *ret = NULL;
            map.RefList(key.c_str(), ret);
            if (!ret) return;
            stream << blank << "<LIST>[" << key <<"]\n";
            serialize_list(*ret, depth + 1, stream);
        }
        break;
    default:
        stream << blank << "<Unknown>()";
    }
}

void SerialHelper::serialize_list(const msgbus::VariantList &lst, unsigned depth, std::stringstream &stream) {
    std::string blank = getBlankByDepth(depth);
    int index = 0;
    for (msgbus::Message::List::const_iterator it = lst.begin(); it != lst.end(); ++it, ++index) {
        if (it->Type == msgbus::Variant::VAR_MAP) {
            stream << blank << "<" << getTypeDescriptor(it->Type) << ">[" << index << "]\n";
            const msgbus::VariantMap *map = NULL;
            it->RefMap(map);
            serialize_map(*map, depth + 1, stream);
        }
        else {
            serialize_var(*it, depth, stream);
        }
    }
}

void SerialHelper::serialize_map(const msgbus::VariantMap &map, unsigned depth, std::stringstream &stream) {
    std::string key;
    int type;
    for (int i = 0; i < map.fieldCount(); i++) {
        map.getFieldInfo(i, key, type);
        serialize_var(map, depth, key, type, stream);
    }
}

std::string MessageHelper::getMessageDescriptor(const msgbus::Message &message) {
    std::stringstream stream;
    stream << "\n" <<
        "[Type](" << message.getType() << ")\n" <<
        "[Subject](" << message.getSubject() << ")\n" <<
        "[MessageID](" << message.getMessageID() << ")\n" <<
        "[ReplyTo](" << message.getReplyTo() << ")\n";

    SerialHelper::serialize_map(message.MapValue(), 0, stream);

    return stream.str();
}

std::string MessageHelper::getMapDescriptor(const msgbus::VariantMap &map, unsigned depth) {
    std::stringstream stream;
    SerialHelper::serialize_map(map, depth, stream);
    return stream.str();
}

std::string MessageHelper::getListDescriptor(const msgbus::VariantList &lst, unsigned depth) {
    std::stringstream stream;
    SerialHelper::serialize_list(lst, depth, stream);
    return stream.str();
}

std::string MessageHelper::getVariantDescriptor(const msgbus::Variant &var, unsigned depth) {
    std::stringstream stream;
    SerialHelper::serialize_var(var, depth, stream);
    return stream.str();
}
