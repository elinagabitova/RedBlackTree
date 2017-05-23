##include <RBtree.hpp>
#include <catch.hpp>

SCENARIO("NULL")
{
	RBtree<int> a;
	REQUIRE(a.Empty());
}
SCENARIO("add")
{
	RBtree <int> a;
	a.Insert(5);
	REQUIRE(!a.Empty());
}
SCENARIO("BST del non inserted element", "[delete]")
{
	RBtree<int> tree = { 6 };
	tree.Remove(3);
	REQUIRE(!tree.Empty());
}
SCENARIO("BST del root with one element", "[delete]")
{
	RBtree<int> tree = { 6 };
	tree.Remove(6);
	REQUIRE(tree.Empty());
}
SCENARIO("BST del root without child", "[delete]")
{
	RBtree<int> tree = { 6, 4, 3 };
	tree.Remove(6);
	RBtree<int> tr = { 4, 3 };
	REQUIRE(tree == tr);
}
SCENARIO("BST del root with two children", "[delete]")
{
	RBtree<int> tree = { 6, 4, 3, 9, 8, 12, 10, 11 };
	tree.Remove(4);
	RBtree<int>tr = { 8, 6, 10, 3, 9, 12, 11 };
	REQUIRE(tree == tr);
}
SCENARIO("BST del non root without children", "[delete]")
{
	RBtree<int> tree = { 6, 4, 3, 9, 8, 12, 10, 11 };
	tree.Remove(3);
	RBtree<int>tr = { 8, 4, 10, 9, 11, 12, 6 };
	REQUIRE(tree == tr);
}
SCENARIO("BST del non root with one child", "[delete]")
{
	RBtree<int> tree = { 6, 4, 3, 9, 8, 12, 10, 11 };
	tree.Remove(11);
	RBtree<int> tr = { 8, 4, 10, 9, 4, 12, 6, 3 };
	REQUIRE(tree == tr);
