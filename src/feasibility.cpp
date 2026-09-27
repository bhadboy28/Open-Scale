#include "openscale/feasibility.hpp"
namespace openscale {
Feasibility analyze(const ModelInfo&i,double ram,double bits,uint64_t ctx,uint64_t batch){
 Feasibility f;f.params=(long double)i.parameter_count;f.available_gb=ram;
 if(f.params<=0){f.note="Parameter count is unknown; use tensor sizes or model metadata for a tighter estimate.";}
 f.weight_gb=(f.params*bits/8.0L)/1e9L;
 // Conservative KV estimate when architecture metadata is present.
 long double kv=0;
 if(i.layers&&i.kv_heads&&i.hidden_size&&i.attention_heads){
   long double head_dim=(long double)i.hidden_size/i.attention_heads;
   kv=2.0L*i.layers*i.kv_heads*head_dim*ctx*batch*2.0L/1e9L;
 }
 f.kv_gb=kv;f.working_gb=0.5L;f.total_gb=f.weight_gb+f.kv_gb+f.working_gb;
 f.resident=f.total_gb<=ram*0.9;f.streaming=f.weight_gb>ram*0.9;
 if(f.resident)f.note="Estimated resident execution fits the requested RAM budget, subject to backend overhead.";
 else if(f.streaming)f.note="Resident execution does not fit; mmap/offload/streaming is required and storage bandwidth may dominate.";
 else f.note="Model requires careful cache/offload planning.";
 return f;
}
}
