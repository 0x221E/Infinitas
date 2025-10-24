#include <memory>
#include <variant>
#include <boost/multiprecision/gmp.hpp>

namespace mp = boost::multiprecision;

namespace shared{
    struct Object;
}

using ObjectPtr = std::unique_ptr<shared::Object>;
using Types = std::variant<mp::mpz_int, mp::mpf_float, std::string, ObjectPtr>;
