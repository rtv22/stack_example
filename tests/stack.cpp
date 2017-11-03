#include <stack.hpp>
#include <catch.hpp>

SCENARIO("init", "[init]") {
	stack<int> A;
	REQUIRE(A.count() == 0);
}

SCENARIO("Push", "[push]") {
	stack<int> A;
	A.push(1);
	A.push(2);
	REQUIRE(A.count() == 2);
}

SCENARIO("Pop", "[pop]") {
	stack<int> A;
	A.push(1);
	A.push(2);
	A.pop();
	REQUIRE(A.count() == 1);
}

SCENARIO("oper=", "[oper]"){
	stack<int> A;
	stack<int> B;
	A.push(1);
	A.push(2);
	REQUIRE(A.count() == 2);
	B = A;
	REQUIRE(B.count() == 2);
}
