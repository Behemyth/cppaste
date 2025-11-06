import std;
import synodic.honesty.test;
import synodic.cppaste.composition;

using namespace honesty::test::literals;

namespace
{
	honesty::test::Suite STATIC_SERVICE_SUITE(
		"static_service",
		[]() -> honesty::test::Generator
		{
			co_yield "static_service_integration"_test = [](const honesty::test::Requirements& requirements)
			{
				// Add static service integration tests here
				requirements.Assert(true);
			};
		});

	honesty::test::Suite DYNAMIC_SERVICE_SUITE(
		"dynamic_service_integration",
		[]() -> honesty::test::Generator
		{
			co_yield "dynamic_service_integration"_test = [](const honesty::test::Requirements& requirements)
			{
				// Add dynamic service integration tests here
				requirements.Assert(true);
			};
		});

	honesty::test::SuiteRegistrar _1(STATIC_SERVICE_SUITE);
	honesty::test::SuiteRegistrar _2(DYNAMIC_SERVICE_SUITE);
}
