#include "pch.h"
#include "CppUnitTest.h"
#include "../Project9.2B/Source.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest92B
{
	TEST_CLASS(UnitTest92B)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Specialty specialty;
			specialty.value = 0;
			const char* str = specialtiesToString(specialty);
			Assert::AreEqual("Mathematics", str);
		}
	};
}
