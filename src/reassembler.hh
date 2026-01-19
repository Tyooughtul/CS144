#pragma once

#include "byte_stream.hh"
#include <map>
#include <string>

class Reassembler
{
public:
  explicit Reassembler( ByteStream&& output ) : output_( std::move( output ) ) {}

  void insert( uint64_t first_index, std::string data, bool is_last_substring );

  uint64_t count_bytes_pending() const;

  uint64_t first_unassembled_index() const { return next_expected_index_; }

  Reader& reader() { return output_.reader(); }
  const Reader& reader() const { return output_.reader(); }

  const Writer& writer() const { return output_.writer(); }

private:
  ByteStream output_;
  uint64_t next_expected_index_ = 0;
  uint64_t eof_index_ = 0;
  std::map<uint64_t, std::string> buffer_ {};
  bool last_substring_received_ = false;
  bool has_written_ = false;
  
  void merge_into_buffer( uint64_t first_index, std::string data );
  void write_to_stream();
  void try_close_stream();
};

