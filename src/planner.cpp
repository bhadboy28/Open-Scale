#include "openscale/planner.hpp"
namespace openscale { ExecutionPlan plan(const Model&m,const std::vector<BackendCaps>&bs){ExecutionPlan p;for(auto&b:bs)if(b.available&&b.score>0){p.backend=b.kind;break;}p.threads=1;p.summary="mmap weights; backend-dispatch ready; paged KV policy enabled";if(m.info().parameter_count>0&&m.info().parameter_count>50000000000ull)p.paged_kv=true;return p;} }
