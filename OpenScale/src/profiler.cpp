#include "openscale/profiler.hpp"
namespace openscale { void Profiler::start(){start_=std::chrono::steady_clock::now();} double Profiler::stop_seconds(){return std::chrono::duration<double>(std::chrono::steady_clock::now()-start_).count();} }
