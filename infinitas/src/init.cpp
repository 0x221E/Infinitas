#include "init.h"

namespace GlobalEnvironment
{
    void Initialize()
    {
       mp::mpf_float::default_precision(1000);
    }
}
