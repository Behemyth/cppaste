#pragma once

enum class CompilerID { VS, GCC, CLANG, EMSCRIPTEN, UNKNOWN };

class Compiler {

public:

	constexpr static CompilerID GetCompiler();
	constexpr static bool Debug();


private:

	#if defined(_MSC_VER)

	constexpr static CompilerID compiler_ = CompilerID::VS;

	#elif defined(__GNUC__)

	constexpr static CompilerID compiler_ = CompilerID::GCC;

	#elif defined(__clang__)

	constexpr static CompilerID compiler_ = CompilerID::CLANG;

	#elif defined(__EMSCRIPTEN__) 

	constexpr static CompilerID compiler_ = CompilerID::EMSCRIPTEN;

	#else

	constexpr static CompilerID compiler_ = CompilerID::UNKNOWN;

	#endif

	//TODO: Update debug macro
	#if defined(NDEBUG)

	constexpr static bool debug_ = false;

	#else

	constexpr static bool debug_ = true;

	#endif

};
