#include "byte_stream.hh"
#include "debug.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

// Push data to stream, but only as much as available capacity allows.
void Writer::push( string data )
{
  // Your code here (and in each method below)
  // debug( "Writer::push({}) not yet implemented", data );
  const uint64_t bytes_to_push = min(static_cast<uint64_t>(data.size()),available_capacity());
  buffer_.append(data,0,bytes_to_push);
  bytes_pushed_+=bytes_to_push;
}

// Signal that the stream has reached its ending. Nothing more will be written.
void Writer::close()
{
  //debug( "Writer::close() not yet implemented" );
  closed_=true;
}

// Has the stream been closed?
bool Writer::is_closed() const
{
  // debug( "Writer::is_closed() not yet implemented" );
  return closed_; // Your code here.
}

// How many bytes can be pushed to the stream right now?
uint64_t Writer::available_capacity() const
{
  // debug( "Writer::available_capacity() not yet implemented" );
  return capacity_ - buffer_.size(); // Your code here.
}

// Total number of bytes cumulatively pushed to the stream
uint64_t Writer::bytes_pushed() const
{
  // debug( "Writer::bytes_pushed() not yet implemented" );
  return bytes_pushed_; // Your code here.
}

// Peek at the next bytes in the buffer -- ideally as many as possible.
// It's not required to return a string_view of the *whole* buffer, but
// if the peeked string_view is only one byte at a time, it will probably force
// the caller to do a lot of extra work.
string_view Reader::peek() const
{
  // debug( "Reader::peek() not yet implemented" );
  // Your code here.
  if(buffer_.empty()){
    return {};
  }
  return string_view(&buffer_[0],buffer_.size());
}

// Remove `len` bytes from the buffer.
void Reader::pop( uint64_t len )
{
  // debug( "Reader::pop({}) not yet implemented", len );
  const uint64_t bytes_to_pop = min(len, bytes_buffered());
  buffer_.erase(0,bytes_to_pop);
  bytes_popped_ += bytes_to_pop;
}

// Is the stream finished (closed and fully popped)?
bool Reader::is_finished() const
{
  // debug( "Reader::is_finished() not yet implemented" );
  return closed_ && buffer_.empty();
}

// Number of bytes currently buffered (pushed and not popped)
uint64_t Reader::bytes_buffered() const
{
  // debug( "Reader::bytes_buffered() not yet implemented" );
  return buffer_.size();
}

// Total number of bytes cumulatively popped from stream
uint64_t Reader::bytes_popped() const
{
  // debug( "Reader::bytes_popped() not yet implemented" );
  return bytes_popped_;
}

