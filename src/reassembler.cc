#include "reassembler.hh"
#include "debug.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  if ( is_last_substring ) {
    last_substring_received_ = true;
    eof_index_ = first_index + data.size();
  }
  if (data.empty()) {
	  try_close_stream();
	  return;
  }

  uint64_t data_end = first_index + data.size();
  uint64_t stream_capacity = output_.writer().available_capacity();
  uint64_t stream_size = output_.writer().bytes_pushed();

  if ( first_index >= stream_size + stream_capacity ) {
    try_close_stream();
    return;
  }

  if ( data_end <= next_expected_index_ ) {
    try_close_stream();
    return;
  }

  if ( first_index < next_expected_index_ ) {
    uint64_t offset = next_expected_index_ - first_index;
    data = data.substr( offset );// 截取下标到末尾的位置
    first_index = next_expected_index_;
  }

  if ( first_index + data.size() > stream_size + stream_capacity ) {
    data.resize( stream_size + stream_capacity - first_index );
  }

  if ( data.empty() ) {
    try_close_stream();
    return;
  }

  merge_into_buffer( first_index, data );
  write_to_stream();
  try_close_stream();
}

uint64_t Reassembler::count_bytes_pending() const
{
  uint64_t count = 0;
  for ( const auto& [index, data] : buffer_ ) {
    count += data.size();
  }
  return count;
}

void Reassembler::merge_into_buffer( uint64_t first_index, string data )
{
  if ( data.empty() ) {
    return;
  }

  uint64_t data_end = first_index + data.size();

  auto it = buffer_.lower_bound( first_index );

  while ( it != buffer_.end() && it->first <= data_end ) {
    uint64_t existing_end = it->first + it->second.size();
    if ( existing_end > data_end ) {
      data += it->second.substr( data_end - it->first );
      data_end = existing_end;
    }
    it = buffer_.erase( it );
  }

  if ( it != buffer_.begin() ) {
    auto prev = std::prev( it );
    uint64_t prev_end = prev->first + prev->second.size();
    if ( prev_end >= first_index ) {
      if ( prev_end < data_end ) {
        prev->second += data.substr( prev_end - first_index );
      }
      return;
    }
  }

  buffer_[first_index] = data;
}

void Reassembler::write_to_stream()
{
  while ( !buffer_.empty() && buffer_.begin()->first == next_expected_index_ ) {
    auto it = buffer_.begin();
    uint64_t bytes_to_write = min( it->second.size(), output_.writer().available_capacity() );
    output_.writer().push( it->second.substr( 0, bytes_to_write ) );
    next_expected_index_ += bytes_to_write;
    has_written_ = true;
    if ( bytes_to_write < it->second.size() ) {
      buffer_[next_expected_index_] = it->second.substr( bytes_to_write );
    }
    buffer_.erase( it );
  }
}

void Reassembler::try_close_stream()
{
  if ( last_substring_received_ && buffer_.empty() && next_expected_index_ == eof_index_ )  {
    output_.writer().close();
  }
}

