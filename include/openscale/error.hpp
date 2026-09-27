#pragma once
#include <string>
#include <utility>
namespace openscale {
enum class ErrorCode { Ok=0, Io, Format, InvalidArgument, Unsupported, OutOfMemory, Backend, Corrupt, Internal };
struct Error { ErrorCode code{ErrorCode::Ok}; std::string message; explicit operator bool() const { return code != ErrorCode::Ok; } };
template<class T> struct Result { T value{}; Error error{}; bool ok() const { return !error; } };
inline Error ok_error(){ return {}; }
inline Error make_error(ErrorCode c, std::string m){ return {c,std::move(m)}; }
}
