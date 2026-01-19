#include "wrapping_integers.hh"
#include "debug.hh"

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  // Your code here.
  // debug( "unimplemented wrap( {}, {} ) called", n, zero_point.raw_value_ );
  // return Wrap32 { 0 };
  return Wrap32{static_cast<uint32_t>((n+zero_point.raw_value_)%(1ULL<<32))};
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  int32_t diff = raw_value_ - zero_point.raw_value_ - static_cast<uint32_t>(checkpoint);
  int64_t res = checkpoint + diff;
  if (res < 0) {
    res += (1ULL << 32);
  }
  return res;
}
