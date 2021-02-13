#include "compiler.h"

constexpr auto Compiler::GetCompiler() -> CompilerID
{
	return compiler_;
}

constexpr auto Compiler::Debug() -> bool
{
	return debug_;
}
