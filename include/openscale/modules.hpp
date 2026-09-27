#pragma once
#include "backend.hpp"
#include <string>
#include <vector>
namespace openscale { struct ModuleInfo { std::string name, kind, status, detail; }; std::vector<ModuleInfo> list_modules(); }
