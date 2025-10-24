#include "init.h"

namespace GlobalEnvironment
{
    void Initialize()
    {
       mp::mpf_float::default_precision(1000);
       spdlog::set_level(spdlog::level::trace);
    }
}
