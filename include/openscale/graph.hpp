#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace openscale {
enum class Op { Input, Embedding, RMSNorm, MatMul, RoPE, Attention, MLP, Add, Output, Unknown };
struct Node { uint32_t id=0; Op op=Op::Unknown; std::string name; std::vector<uint32_t> inputs; };
struct Graph { std::vector<Node> nodes; bool valid(std::string&) const; };
const char* op_name(Op);
}
