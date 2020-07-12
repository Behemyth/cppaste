#include "compiler.h"

constexpr CompilerID Compiler::GetCompiler()
{
	return compiler_;
}

constexpr bool Compiler::Debug()
{
	return debug_;
}