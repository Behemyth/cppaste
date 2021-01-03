#pragma once

enum class CompilerID
{
	VS,
	GCC,
	CLANG,
	EMSCRIPTEN,
	UNKNOWN
};

#define SYNODIC_COMPILER(X)					  SYNODIC_COMPILER_PRIVATE_##X()

class Compiler
{

public:
	constexpr static CompilerID GetCompiler();
	constexpr static bool Debug();


private:
#if defined(_MSC_VER)

	constexpr static CompilerID compiler_ = CompilerID::VS;
#	define SYNODIC_COMPILER_PRIVATE_VS()		  1
#	define SYNODIC_COMPILER_PRIVATE_GCC()		  0
#	define SYNODIC_COMPILER_PRIVATE_CLANG()	  0
#	define SYNODIC_COMPILER_PRIVATE_EMSCRIPTEN() 0
#	define SYNODIC_COMPILER_PRIVATE_UNKNOWN()	  0

#elif defined(__GNUC__)

	constexpr static CompilerID compiler_ = CompilerID::GCC;
#	define SYNODIC_COMPILER_PRIVATE_VS()		  0
#	define SYNODIC_COMPILER_PRIVATE_GCC()		  1
#	define SYNODIC_COMPILER_PRIVATE_CLANG()	  0
#	define SYNODIC_COMPILER_PRIVATE_EMSCRIPTEN() 0
#	define SYNODIC_COMPILER_PRIVATE_UNKNOWN()	  0

#elif defined(__clang__)

	constexpr static CompilerID compiler_ = CompilerID::CLANG;
#	define SYNODIC_COMPILER_PRIVATE_VS()		  0
#	define SYNODIC_COMPILER_PRIVATE_GCC()		  0
#	define SYNODIC_COMPILER_PRIVATE_CLANG()	  1
#	define SYNODIC_COMPILER_PRIVATE_EMSCRIPTEN() 0
#	define SYNODIC_COMPILER_PRIVATE_UNKNOWN()	  0

#elif defined(__EMSCRIPTEN__)

	constexpr static CompilerID compiler_ = CompilerID::EMSCRIPTEN;
#	define SYNODIC_COMPILER_PRIVATE_VS()		  0
#	define SYNODIC_COMPILER_PRIVATE_GCC()		  0
#	define SYNODIC_COMPILER_PRIVATE_CLANG()	  0
#	define SYNODIC_COMPILER_PRIVATE_EMSCRIPTEN() 1
#	define SYNODIC_COMPILER_PRIVATE_UNKNOWN()	  0

#else

	constexpr static CompilerID compiler_ = CompilerID::UNKNOWN;
#	define SYNODIC_COMPILER_PRIVATE_VS()		  0
#	define SYNODIC_COMPILER_PRIVATE_GCC()		  0
#	define SYNODIC_COMPILER_PRIVATE_CLANG()	  0
#	define SYNODIC_COMPILER_PRIVATE_EMSCRIPTEN() 0
#	define SYNODIC_COMPILER_PRIVATE_UNKNOWN()	  1

#endif

// TODO: Update debug macro
#if defined(NDEBUG)

	constexpr static bool debug_ = false;

#else

	constexpr static bool debug_ = true;

#endif
};
