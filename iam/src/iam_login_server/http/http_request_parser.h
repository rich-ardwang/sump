/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : http_request_parser.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-02
*Desc        : parse the header and body content of http request.
*History     : Add by Lei.Wang 2019-10-02
***************************************************************************************************/
#ifndef __HTTP_REQUEST_PARSER_H__
#define __HTTP_REQUEST_PARSER_H__
#include <tuple>

#define content_length_name         "Content-Length"


namespace http {
namespace server {

struct request;

//parser for incoming requests.
class request_parser {
public:
    //construct ready to parse the request method.
    request_parser();

    //reset to initial parser state.
    void reset();

    //result of parse.
    enum result_type { good, bad, indeterminate };

    //parse some data. The enum return value is good when a complete request has
    //been parsed, bad if the data is invalid, indeterminate when more data is
    //required. The InputIterator return value indicates how much of the input
    //has been consumed.
    template <typename InputIterator>
    std::tuple<result_type, InputIterator> parse(request& req, 
        InputIterator begin, InputIterator end) {
        while (begin != end) {
            result_type result = consume(req, *begin++);
            if (result == good || result == bad)
                return std::make_tuple(result, begin);
        }
        return std::make_tuple(indeterminate, begin);
    }

private:
    //handle the next character of input.
    result_type consume(request& req, char input);

    //check if a byte is an HTTP character.
    static bool is_char(int c);

    //check if a byte is an HTTP control character.
    static bool is_ctl(int c);

    //check if a byte is defined as an HTTP tspecial character.
    static bool is_tspecial(int c);

    //check if a byte is a digit.
    static bool is_digit(int c);

    //check if two characters are equal, without regard to case.
    static bool tolower_compare(char a, char b);

    //check whether the two request header names match.
    bool headers_equal(const std::string& a, const std::string& b);

private:
    //content length as decoded from headers. Defaults to 0.
    std::size_t content_length_;

    //the current state of the parser.
    enum state {
        method_start,
        method,
        uri,
        http_version_h,
        http_version_t_1,
        http_version_t_2,
        http_version_p,
        http_version_slash,
        http_version_major_start,
        http_version_major,
        http_version_minor_start,
        http_version_minor,
        expecting_newline_1,
        header_line_start,
        header_lws,
        header_name,
        space_before_header_value,
        header_value,
        expecting_newline_2,
        expecting_newline_3,
        http_content
    } state_;
};

} //namespace server
} //namespace http

#endif /* __HTTP_REQUEST_PARSER_H__ */
