#include <boost/sml.hpp>

#include <boost/sml.hpp>
#include <cassert>

namespace sml = boost::sml;

namespace
{
struct release
{
};
struct timeout
{
};

const auto send_fin = [] {
};
const auto send_ack = [] {
};

struct hello_world
{
  auto operator()() const
  {
    using namespace sml;
    return make_transition_table(*"established"_s + event<release> / send_fin =
                                   "fin wait 1"_s,
                                 "timed wait"_s + event<timeout> / send_ack = X);
  }
};
} // namespace
