#include <string>
#include "http_reply.h"


namespace http {
namespace server {

namespace status_strings {

const std::string ok = 
    "HTTP/1.0 200 OK\r\n";
const std::string created = 
    "HTTP/1.0 201 Created\r\n";
const std::string accepted = 
    "HTTP/1.0 202 Accepted\r\n";
const std::string no_content = 
    "HTTP/1.0 204 No Content\r\n";
const std::string no_data = 
    "HTTP/1.0 205 No Data\r\n";
const std::string multiple_choices = 
    "HTTP/1.0 300 Multiple Choices\r\n";
const std::string moved_permanently = 
    "HTTP/1.0 301 Moved Permanently\r\n";
const std::string moved_temporarily = 
    "HTTP/1.0 302 Moved Temporarily\r\n";
const std::string not_modified = 
    "HTTP/1.0 304 Not Modified\r\n";
const std::string bad_request = 
    "HTTP/1.0 400 Bad Request\r\n";
const std::string unauthorized = 
    "HTTP/1.0 401 Unauthorized\r\n";
const std::string forbidden = 
    "HTTP/1.0 403 Forbidden\r\n";
const std::string not_found = 
    "HTTP/1.0 404 Not Found\r\n";
const std::string internal_server_error = 
    "HTTP/1.0 500 Internal Server Error\r\n";
const std::string not_implemented_s = 
    "HTTP/1.0 501 Not Implemented\r\n";
const std::string bad_gateway = 
    "HTTP/1.0 502 Bad Gateway\r\n";
const std::string service_unavailable = 
    "HTTP/1.0 503 Service Unavailable\r\n";

asio::const_buffer to_buffer(reply::status_type status) {
    switch (status) {
    case reply::ok:
        return asio::buffer(ok);
    case reply::created:
        return asio::buffer(created);
    case reply::accepted:
        return asio::buffer(accepted);
    case reply::no_content:
        return asio::buffer(no_content);
    case reply::no_data:
        return asio::buffer(no_data);
    case reply::multiple_choices:
        return asio::buffer(multiple_choices);
    case reply::moved_permanently:
        return asio::buffer(moved_permanently);
    case reply::moved_temporarily:
        return asio::buffer(moved_temporarily);
    case reply::not_modified:
        return asio::buffer(not_modified);
    case reply::bad_request:
        return asio::buffer(bad_request);
    case reply::unauthorized:
        return asio::buffer(unauthorized);
    case reply::forbidden:
        return asio::buffer(forbidden);
    case reply::not_found:
        return asio::buffer(not_found);
    case reply::internal_server_error:
        return asio::buffer(internal_server_error);
    case reply::not_implement:
        return asio::buffer(not_implemented_s);
    case reply::bad_gateway:
        return asio::buffer(bad_gateway);
    case reply::service_unavailable:
        return asio::buffer(service_unavailable);
    default:
        return asio::buffer(internal_server_error);
    }
}

} //namespace status_strings

namespace misc_strings {

const char name_value_separator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };

} //namespace misc_strings

std::vector<asio::const_buffer> reply::to_buffers() {
    std::vector<asio::const_buffer> buffers;
    buffers.push_back(status_strings::to_buffer(status));
    for (std::size_t i = 0; i < headers.size(); ++i) {
        header& h = headers[i];
        buffers.push_back(asio::buffer(h.name));
        buffers.push_back(asio::buffer(misc_strings::name_value_separator));
        buffers.push_back(asio::buffer(h.value));
        buffers.push_back(asio::buffer(misc_strings::crlf));
    }
    buffers.push_back(asio::buffer(misc_strings::crlf));
    buffers.push_back(asio::buffer(content));
    return buffers;
}

namespace stock_replies {

const char ok[] = ""
    "{\"status\":\"OK\",\"code\":200,";
const char created[] = 
    "{\"status\":\"Created\",\"code\":201,";
const char accepted[] = 
    "{\"status\":\"Accepted\",\"code\":202,";
const char no_content[] = 
    "{\"status\":\"No Content\",\"code\":204,";
const char no_data[] = 
    "{\"status\":\"No Data\",\"code\":205}";
const char multiple_choices[] = 
    "{\"status\":\"Multiple Choices\",\"code\":300,";
const char moved_permanently[] = 
    "{\"status\":\"Moved Permanently\",\"code\":301,";
const char moved_temporarily[] = 
    "{\"status\":\"Moved Temporarily\",\"code\":302,";
const char not_modified[] = 
    "{\"status\":\"Not Modified\",\"code\":304,";
const char bad_request[] = 
    "{\"status\":\"Bad Request\",\"code\":400,";
const char unauthorized[] = 
    "{\"status\":\"Unauthorized\",\"code\":401,";
const char forbidden[] = 
    "{\"status\":\"Forbidden\",\"code\":403,";
const char not_found[] = 
    "{\"status\":\"Not Found\",\"code\":404,";
const char internal_server_error[] = 
    "{\"status\":\"Internal Server Error\",\"code\":500,";
const char not_implemented_s[] = 
    "{\"status\":\"Not Implemented\",\"code\":501,";
const char bad_gateway[] = 
    "{\"status\":\"Bad Gateway\",\"code\":502,";
const char service_unavailable[] = 
    "{\"status\":\"Service Unavailable\",\"code\":503,";

std::string to_string(reply::status_type status, HttpAuthAckPtr auth_ack_ptr) {
    if (nullptr == auth_ack_ptr)
        return no_data;

    //create left_json string
    std::string left_json;
    left_json += "\"data\":{\"ret_code\":";
    left_json += std::to_string(auth_ack_ptr->result_code_);
    left_json += ",\"ret_message\":\"";
    left_json += auth_ack_ptr->result_msg_;
    left_json += "\",\"user_name\":\"";
    left_json += auth_ack_ptr->user_name_;
    left_json += "\",\"user_id\":\"";
    left_json += auth_ack_ptr->user_id_;
    left_json += "\",\"token\":\"";
    left_json += auth_ack_ptr->token_;
    left_json += "\",\"server_time\":\"";
    left_json += auth_ack_ptr->server_sys_time_;
    left_json += "\"}}";

    std::string ret_val;
    switch (status) {
    case reply::ok:
        ret_val = ok + left_json;
        break;
    case reply::created:
        ret_val = created + left_json;
        break;
    case reply::accepted:
        ret_val = accepted + left_json;
        break;
    case reply::no_content:
        ret_val = no_content + left_json;
        break;
    case reply::no_data:
        ret_val = no_data + left_json;
        break;
    case reply::multiple_choices:
        ret_val = multiple_choices + left_json;
        break;
    case reply::moved_permanently:
        ret_val = moved_permanently + left_json;
        break;
    case reply::moved_temporarily:
        ret_val = moved_temporarily + left_json;
        break;
    case reply::not_modified:
        ret_val = not_modified + left_json;
        break;
    case reply::bad_request:
        ret_val = bad_request + left_json;
        break;
    case reply::unauthorized:
        ret_val = unauthorized + left_json;
        break;
    case reply::forbidden:
        ret_val = forbidden + left_json;
        break;
    case reply::not_found:
        ret_val = not_found + left_json;
        break;
    case reply::internal_server_error:
        ret_val = internal_server_error + left_json;
        break;
    case reply::not_implement:
        ret_val = not_implemented_s + left_json;
        break;
    case reply::bad_gateway:
        ret_val = bad_gateway + left_json;
        break;
    case reply::service_unavailable:
        ret_val = service_unavailable + left_json;
        break;
    default:
        ret_val = internal_server_error + left_json;
        break;
    }

    return ret_val;
}

} //namespace stock_replies

reply reply::stock_reply(reply::status_type status, HttpAuthAckPtr auth_ack_ptr) {
    reply rep;
    rep.status = status;
    rep.content = stock_replies::to_string(status, auth_ack_ptr);
    rep.headers.resize(2);
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = std::to_string(rep.content.size());
    rep.headers[1].name = "Content-Type";
    rep.headers[1].value = "application/json";
    return rep;
}

} //namespace server
} //namespace http
