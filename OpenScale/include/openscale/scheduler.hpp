#pragma once
#include <cstddef>
namespace openscale {
class Scheduler { public: explicit Scheduler(size_t workers=0); size_t workers() const; void set_workers(size_t workers); size_t choose_workers(double cpu_util,double ram_util) const; private: size_t workers_; };
}
