#include "tcp_receiver.hh"
#include "debug.hh"
#include <optional>
#include <algorithm>

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  // Your code here.
  // debug( "unimplemented receive() called" );
  // (void)message;
  if(message.RST){
    reassembler_.reader().set_error();
    return;
  }
  if(message.SYN && !isn_.has_value()){
    isn_ = message.seqno;
  }
  if(!isn_.has_value()){
    return;
  }
  // 这里可以用bytes_buffered完全就是一个 0 - 1 based 的问题
  uint64_t checkpoint = reassembler_.first_unassembled_index() + 1;
  uint64_t abs_seqno = message.seqno.unwrap(isn_.value(), checkpoint);
  reassembler_.insert(abs_seqno - 1 + (message.SYN ? 1 : 0) , message.payload, message.FIN);
}

TCPReceiverMessage TCPReceiver::send() const {
  uint16_t window_size = reassembler_.writer().available_capacity() > UINT16_MAX ? UINT16_MAX : reassembler_.writer().available_capacity();
  if (!isn_.has_value()) {
      return {std::nullopt, window_size, reassembler_.writer().has_error()}; 
  }
  uint64_t first_unassembled = reassembler_.first_unassembled_index() + 1 + (reassembler_.writer().is_closed() ? 1 : 0);  // +1因为ackno是下一个
  Wrap32 ackno = Wrap32::wrap(first_unassembled, isn_.value());
  return {ackno, window_size, false};
}