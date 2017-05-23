#include <iostream>

using namespace std;

template <class T>
class RBtree {
	struct node_T
	{
		node_T *left, *right;
		T value;
		bool Red;
		void Show(ostream &out, int level) const;
	};
	node_T *tree_root;
	int nodes_count;
private:
	node_T* NewNode(T value)
	{
		nodes_count++;
		node_T *node = new node_T;
		node->value = value;
		node->left = nullptr;
		node->right = nullptr;
		node->Red = true;
		return node;
	}
	void DelNode(node_T*);
	void Clean(node_T*);
	node_T* Rotate21(node_T* node)
	{
		node_T *right = node->right;
		node_T *root_st = right->left;
		right->left = node;
		node->right = root_st;
		return right;
	}
	node_T* Rotate12(node_T* node)
	{
		node_T *left = node->left;
		node_T *root_st = left->right;
		left->right = node;
		node->left = root_st;
		return left;
	}
	void b_ins(node_T**);
	bool b_rem(node_T**);
	bool b_rem2(node_T**);
	bool Insert(T&, node_T**);
	bool GetMin(node_T**, node_T**);
	bool Remove(node_T**, T&);
	bool Equal(node_T* root2, const node_T* root1)
	{
		return (root2&&root1 ? root2->value == root1->value&&Equal(root2->left, root1->left) && Equal(root2->right, root1->right) : !root2 && !root1);
	};

public:
	bool Empty()
	{
		node_T* root1 = nullptr;
		return Equal(tree_root, root1);
	}
	void Show(ostream &out) const;
	RBtree(std::initializer_list<T> list)
	{
		tree_root = nullptr;
		for (auto& item : list)
		{
			Insert(item);
		}

	}
	RBtree();
	~RBtree();
	void Clean();
	void Insert(T);
	void Remove(T);
	int GetNodesCount() const;
	bool operator ==(const RBtree<T> &a)
	{
		return Equal(tree_root, a.tree_root);
	}
};


template <class T>
RBtree<T>::RBtree()
{
	tree_root = nullptr;
	nodes_count = 0;
}
template <class T>
RBtree<T>::~RBtree()
{
	Clean(tree_root);
}
template <class T>
int RBtree<T>::GetNodesCount() const
{
	return nodes_count;
}
template <class T>
void RBtree<T>::DelNode(node_T *node)
{
	nodes_count--;
	delete node;
}
template <class T>
void RBtree<T>::Clean(node_T *node)
{
	if (!node) return;
	Clean(node->left);
	Clean(node->right);
	DelNode(node);
}
template <class T>
void RBtree<T>::node_T::Show(ostream &out, int level) const
{
	const node_T *tr = this;

	if (tr) tr->right->Show(out, level + 1);
	for (int i = 0; i<level; i++)
		out << "   ";

	if (tr)
	{
		char *is_Red = "BLACK";
		if (Red) is_Red = "Red";
		out << tr->value << "\n";
		for (int i = 0; i<level; i++)
			out << "   ";
		out << is_Red << endl;
	}
	else out << "End" << endl;
	if (tr) tr->left->Show(out, level + 1);
}
template <class T>
void RBtree<T>::b_ins(node_T **root)
{
	node_T *left, *right, *lx1, *rx1;
	node_T *node = *root;
	if (node->Red) return;
	left = node->left;
	right = node->right;
	if (left && left->Red)
	{
		rx1 = left->right;
		if (rx1 && rx1->Red) left = node->left = Rotate21(left);
		lx1 = left->left;
		if (lx1 && lx1->Red) {
			node->Red = true;
			left->Red = false;
			if (right && right->Red) {
				lx1->Red = true;
				right->Red = false;
				return;
			}
			*root = Rotate12(node);
			return;
		}
	}

	if (right && right->Red) {
		lx1 = right->left;
		if (lx1 && lx1->Red) right = node->right = Rotate12(right);
		rx1 = right->right;
		if (rx1 &&rx1->Red) {
			node->Red = true;
			right->Red = false;
			if (left && left->Red) {
				rx1->Red = true;
				left->Red = false;
				return;
			}
			*root = Rotate21(node);
			return;
		}
	}
}
template <class T>
bool RBtree<T>::b_rem1(node_T **root)
{
	node_T *node = *root;
	node_T *left = node->left;
	node_T *right = node->right;
	if (left && left->Red)
	{
		left->Red = false;
		return false;
	}
	if (right && right->Red)
	{
		node->Red = true;
		right->Red = false;
		node = *root = Rotate21(node);
		if (b_rem1(&node->left)) node->left->Red = false;
		return false;
	}

	unsigned int mask = 0;
	node_T *pa = right->left;
	node_T *pb = right->right;
	if (pa && pa->Red) mask |= 1;
	if (pb && pb->Red) mask |= 2;
	switch (mask)
	{
	case 0:
		right->Red = true;
		return true;
	case 1:
	case 3:
		right->Red = true;
		pa->Red = false;
		right = node->right = Rotate12(right);
		pb = right->right;
	case 2:
		right->Red = node->Red;
		pb->Red = node->Red = false;
		*root = Rotate21(node);
	}
	return false;
}
template <class T>
bool RBtree<T>::b_rem2(node_T **root)
{
	node_T *node = *root;
	node_T *left = node->left;
	node_T *right = node->right;
	if (right && right->Red)
	{
		right->Red = false;
		return false;
	}
	if (left && left->Red) {
		node->Red = true;
		left->Red = false;
		node = *root = Rotate12(node);
		if (b_rem2(&node->right)) node->right->Red = false;
		return false;
	}
	unsigned int mask = 0;
	node_T *pa1 = left->left;
	node_T *pa2 = left->right;
	if (pa1 && pa1->Red) mask |= 1;
	if (pa2 && pa2->Red) mask |= 2;
	switch (mask) {
	case 0:
		left->Red = true;
		return true;
	case 2:
	case 3:
		left->Red = true;
		pa2->Red = false;
		left = node->left = Rotate21(left);
		pa1 = left->left;
	case 1:
		left->Red = node->Red;
		pa1->Red = node->Red = false;
		*root = Rotate12(node);
	}
	return false;
}
template <class T>
bool RBtree<T>::Insert(T &value, node_T **root)
{
	node_T *node = *root;
	if (!node) *root = NewNode(value);
	else {
		if (value == node->value) return true;
		if (Insert(value, value<node->value ? &node->left : &node->right)) return true;
		b_ins(root);
	}
	return false;
}
template <class T>
bool RBtree<T>::GetMin(node_T **root, node_T **res)
{
	node_T *node = *root;
	if (node->left) {
		if (GetMin(&node->left, res)) return b_rem1(root);
	}
	else {
		*root = node->right;
		*res = node;
		return !node->Red;
	}
	return false;
}
template <class T>
bool RBtree<T>::Remove(node_T **root, T &value)
{
	node_T *t, *node = *root;
	if (!node) return false;
	if (node->value<value) {
		if (Remove(&node->right, value))
			return b_rem2(root);
	}
	else if (node->value>value) {
		if (Remove(&node->left, value))	return b_rem1(root);
	}
	else {
		bool res;
		if (!node->right) {
			*root = node->left;
			res = !node->Red;
		}
		else {
			res = GetMin(&node->right, root);
			t = *root;
			t->Red = node->Red;
			t->left = node->left;
			t->right = node->right;
			if (res) res = b_rem2(root);
		}
		DelNode(node);
		return res;
	}
	return 0;
}
template <class T>
void RBtree<T>::Show(ostream &out) const
{
	tree_root->Show(out, 0);
}
template <class T>
void RBtree<T>::Insert(T value)
{
	Insert(value, &tree_root);
	if (tree_root) tree_root->Red = false;
}
template <class T>
void RBtree<T>::Remove(T value)
{
	Remove(&tree_root, value);
}
template <class T>
void RBtree<T>::Clean()
{
	Clean(tree_root);
	tree_root = nullptr;
}
