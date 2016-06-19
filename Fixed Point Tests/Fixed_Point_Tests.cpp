#include "stdafx.h"
#include "fixedPoint.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;
using namespace Fixed_Point;

namespace FixedPointTests {
	[TestClass]
	public ref class Fixed_Point_Tests {
	private:
		TestContext^ testContextInstance;

	public:
		/// <summary>
		///Gets or sets the test context which provides
		///information about and functionality for the current test run.
		///</summary>
		property Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ TestContext
		{
			Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get() {
				return testContextInstance;
			}
			System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value) {
				testContextInstance = value;
			}
		};

#pragma region Additional test attributes
		//
		//You can use the following additional attributes as you write your tests:
		//
		//Use ClassInitialize to run code before running the first test in the class
		//[ClassInitialize()]
		//static void MyClassInitialize(TestContext^ testContext) {};
		//
		//Use ClassCleanup to run code after all tests in a class have run
		//[ClassCleanup()]
		//static void MyClassCleanup() {};
		//
		//Use TestInitialize to run code before running each test
		//[TestInitialize()]
		//void MyTestInitialize() {};
		//
		//Use TestCleanup to run code after each test has run
		//[TestCleanup()]
		//void MyTestCleanup() {};
		//
#pragma endregion 

		static const fixed fix_1 = 0x00010000, fix_n1 = -fix_1, fix_hlf = fix_1 >> 1, fix_1hlf = fix_1 | fix_hlf;

		[TestMethod]
		void mul_test() {
			fixed left = fix_1, right = fix_1 * 3;
			Assert::IsTrue((left * 3) == mul(left, right), L"\"mul(+,+)\" failed.");
			Assert::IsTrue((left * -3) == mul(left, -right), L"\"mul(+,-)\" failed.");
		}

		[TestMethod]
		void div_test() {
			fixed left = fix_1, right = (fix_1 << 1) + fix_1;
			Assert::IsTrue((left / 3) == div(left, right), L"\"mul(+,+)\" failed.");
			Assert::IsTrue((left / -3) == div(left, -right), L"\"mul(+,-)\" failed.");
		}

		[TestMethod]
		void fractionalPart_test() {
			fixed f = fix_1hlf;
			Assert::IsTrue(fix_hlf == fractionalPart(f), L"\"fractionalPart(+1)\" failed.");
			Assert::IsTrue(-fix_hlf == fractionalPart(-f), L"\"fractionalPart(-1)\" failed.");
			Assert::IsTrue(fix_hlf == fractionalPart(f + fix_1), L"\"fractionalPart(+2.5)\" failed.");
			Assert::IsTrue(-fix_hlf == fractionalPart(-f - fix_1), L"\"fractionalPart(-2.5)\" failed.");
			Assert::IsTrue(fix_hlf >> 2 == fractionalPart(fix_1 * 2 + (fix_hlf >> 2)), L"\"fractionalPart(+2.125)\" failed.");
			Assert::IsTrue(-fix_hlf >> 2 == fractionalPart(-fix_1 * 2 - (fix_hlf >> 2)), L"\"fractionalPart(-2.125)\" failed.");
		}

		[TestMethod]
		void integerPart_test() {
			fixed f = fix_1hlf;
			Assert::IsTrue(fix_1 == integerPart(f), L"\"integerPart(+1)\" failed.");
			Assert::IsTrue(-fix_1 == integerPart(-f), L"\"integerPart(-1)\" failed.");
			Assert::IsTrue(fix_1 * 2 == integerPart(f + fix_1), L"\"integerPart(+2.5)\" failed.");
			Assert::IsTrue(-fix_1 * 2 == integerPart(-f - fix_1), L"\"integerPart(-2.5)\" failed.");
		}

		[TestMethod]
		void itofix_test() {
			__int32 i = 1;
			Assert::IsTrue(fix_1 == itofix(i), L"\"itofix(+1)\" failed.");
			Assert::IsTrue(-fix_1 == itofix(-i), L"\"itofix(-1)\" failed.");
			Assert::IsTrue(fix_1 * 2 == itofix(i * 2), L"\"itofix(+2)\" failed.");
			Assert::IsTrue(-fix_1 * 2 == itofix(-i * 2), L"\"itofix(-2)\" failed.");
		}

		[TestMethod]
		void fixtoi_test() {
			fixed f = fix_1hlf;
			Assert::IsTrue(1 == fixtoi(f), L"\"fixtoi(+1)\" failed.");
			Assert::IsTrue(-1 == fixtoi(-fix_1hlf), L"\"fixtoi(-1.5)\" failed.");
			Assert::IsTrue(-1 == fixtoi(-fix_1 - (fix_hlf >> 1)), L"\"fixtoi(-1.25)\" failed.");
			Assert::IsTrue(-1 == fixtoi(-fix_1 - (fix_hlf >> 3)), L"\"fixtoi(-1.125)\" failed.");
			Assert::IsTrue(2 == fixtoi(fix_1 * 2), L"\"fixtoi(+2)\" failed.");
			Assert::IsTrue(-2 == fixtoi(-fix_1hlf - fix_1), L"\"fixtoi(-2.5)\" failed.");
			Assert::IsTrue(-2 == fixtoi(-fix_1 * 2 - (fix_hlf >> 1)), L"\"fixtoi(-2.25)\" failed.");
			Assert::IsTrue(-2 == fixtoi(-fix_1 * 2 - (fix_hlf >> 3)), L"\"fixtoi(-2.125)\" failed.");
		}

		[TestMethod]
		void north_hemesphere_test() {
			Assert::IsTrue(north_hemesphere(itofix(64)), L"\"north(64)\"");
			Assert::IsTrue(north_hemesphere(itofix(32)), L"\"north(32)\"");
			Assert::IsFalse(north_hemesphere(itofix(-64)), L"\"north(-64)\"");
			Assert::IsFalse(north_hemesphere(itofix(-32)), L"\"north(-32)\"");
		}

		[TestMethod]
		void right_hemesphere_test() {
			Assert::IsFalse(right_hemesphere(itofix(96)), L"\"right(96)\"");
			Assert::IsTrue(right_hemesphere(itofix(32)), L"\"right(32)\"");
			Assert::IsFalse(right_hemesphere(itofix(-96)), L"\"right(-96)\"");
			Assert::IsTrue(right_hemesphere(itofix(-32)), L"\"right(-32)\"");
		}

		[TestMethod]
		void quartile_test() {
			Assert::IsTrue(quartile(itofix(32)) == 0, L"\"quartile(32)\"");
			Assert::IsTrue(quartile(itofix(96)) == 1, L"\"quartile(96)\"");
			Assert::IsTrue(quartile(itofix(-96)) == 2, L"\"quartile(-96)\"");
			Assert::IsTrue(quartile(itofix(-32)) == 3, L"\"quartile(-32)\"");
		}

		[TestMethod]
		void revolutions_test() {
			Assert::IsTrue(revolutions(itofix(256 * 3)) == 3, L"\"revolutions(3 revs)\"");
			Assert::IsTrue(revolutions(itofix(256 * -3)) == -3, L"\"revolutions(-3 revs)\"");
		}

		[TestMethod]
		void fixRevolutions_test() {
			Assert::IsTrue(fixRevolutions(itofix(256 * 3)) == itofix(3), L"\"fixRevolutions(3 revs)\"");
			Assert::IsTrue(fixRevolutions(itofix(256 * -3)) == itofix(-3), L"\"fixRevolutions(-3 revs)\"");
		}

	};
}
