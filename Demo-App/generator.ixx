module;
#include "generator.hpp"

export module generator;

// export everything from this namespace segment
export namespace std {
using generator = std::generator;
namespace ranges {
using elements_of = std::ranges::elements_of;
}
} // namespace std
