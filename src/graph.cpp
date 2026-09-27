#include "openscale/graph.hpp"
#include <unordered_set>
namespace openscale { const char* op_name(Op o){switch(o){case Op::Input:return"input";case Op::Embedding:return"embedding";case Op::RMSNorm:return"rmsnorm";case Op::MatMul:return"matmul";case Op::RoPE:return"rope";case Op::Attention:return"attention";case Op::MLP:return"mlp";case Op::Add:return"add";case Op::Output:return"output";default:return"unknown";}}
bool Graph::valid(std::string&e)const{std::unordered_set<uint32_t>s;for(auto&n:nodes){if(!s.insert(n.id).second){e="duplicate graph node id";return false;}for(auto x:n.inputs)if(x>=nodes.size()){e="graph input index out of range";return false;}}return true;}}
