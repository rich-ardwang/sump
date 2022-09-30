#pragma once 
#include <sfa/msg/msg.h>
#include <sfa/msg/msg_context.h>

namespace sfa {
namespace msg {

/*
* 功  能：将IMessage转换成二进制流
* 参  数：
*	message 需要转换的IMessage
*	context IMessage上下文环境
*	buffer	二进制流
*	len		二进制流长度
* 返回值：true成功 false失败
* 备  注：buffer需要自行delete方式释放内存
*/
SFA_API bool EncodeMessage(_sfa_in_ const IMessage* message, _sfa_in_ IMessageContext* context, _sfa_out_ uint8_t*& buffer, _sfa_out_ uint32_t& len);

/*
* 功  能：将IMessage转换成二进制流
* 参  数：
*	message 需要转换的IMessage
*	context IMessage上下文环境
*	byte_array	byte数组
* 返回值：true成功 false失败
*/
SFA_API bool EncodeMessage(_sfa_in_ const IMessage* message, _sfa_in_ IMessageContext* context, _sfa_out_ ByteArray& byte_array);

/*
* 功  能：将二进制流转换成IMessage
* 参  数：
*	buffer	需要转换的二进制流
*	len		需要转换的二进制流长度
*	context IMessage上下文环境
*	message	转换后的IMessage
* 返回值：true成功 false失败
* 备  注：IMessage需自行delete方式释放内存
*/
SFA_API bool DecodeMessage(_sfa_in_ const uint8_t* buffer, _sfa_in_ uint32_t len, _sfa_in_ IMessageContext* context, _sfa_out_ IMessage*& message);

/*
* 功  能：将二进制流转换成IMessage
* 参  数：
*	byte_array 需要转换的byte数组
*	context IMessage上下文环境
*	message	转换后的IMessage
* 返回值：true成功 false失败
* 备  注：IMessage需自行delete方式释放内存
*/
SFA_API bool DecodeMessage(_sfa_in_ const ByteArray& byte_array, _sfa_in_ IMessageContext* context, _sfa_out_ IMessage*& message);

}
}
