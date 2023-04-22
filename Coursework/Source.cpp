#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include <limits>

std::random_device rd;
std::mt19937 gen(rd());

template<class T>
int get_random_val(T min_value, T max_value) {
	std::uniform_int_distribution<T> distrib(min_value, max_value);
	return distrib(gen);
}

template<class T>
int count_units(T val)
{
	int count = 0;
	for (; val != 0; val >>= 1)
		count += val & 1;
	return count;
}

template<class T>
class Tree
{
public:
	T value;
	Tree<T>* parent = nullptr;
	Tree<T>* left = nullptr;
	Tree<T>* right = nullptr;

	Tree()
	{
	}

	Tree(T value)
	{
		this->value = value;
	}

	void build_tree(T* arr, size_t count)
	{
		value = arr[0];

		for (int i = 1; i < count; i++)
			add_leaf(arr[i]);
	}

	void build_binary_tree(T* arr, size_t count)
	{
		value = arr[0];

		for (int i = 1; i < count; i++)
			add_leaf_binary(arr[i]);
	}

	//Tree(T* arr, size_t count)
	//{
	//	value = arr[0];

	//	for (int i = 1; i < count; i++)
	//		add_leaf(arr[i]);
	//}

	void add_leaf(T value_to_place)
	{
		Tree* current_tree = this;
		while (true)
		{
			if (value_to_place >= current_tree->value)
			{
				if (current_tree->right == nullptr)
				{
					current_tree->right = new Tree<T>(value_to_place);
					break;
				}
				else
					current_tree = current_tree->right;
			}
			else
			{
				if (current_tree->left == nullptr)
				{
					current_tree->left = new Tree<T>(value_to_place);
					break;
				}
				else
					current_tree = current_tree->left;
			}
		}
	}

	void add_leaf_binary(T value_to_place)
	{
		Tree* current_tree = this;
		while (true)
		{
			if (count_units(value_to_place) >= count_units(current_tree->value))
			{
				if (current_tree->right == nullptr)
				{
					current_tree->right = new Tree<T>(value_to_place);
					break;
				}
				else
					current_tree = current_tree->right;
			}
			else
			{
				if (current_tree->left == nullptr)
				{
					current_tree->left = new Tree<T>(value_to_place);
					break;
				}
				else
					current_tree = current_tree->left;
			}
		}
	}
};


template<class T>
class BinaryTreeSortTree
{
public:
	T value;
	BinaryTreeSortTree<T>* parent = nullptr;
	BinaryTreeSortTree<T>* left = nullptr;
	BinaryTreeSortTree<T>* right = nullptr;
	bool marked = false;

	BinaryTreeSortTree(T value)
	{
		this->value = value;
	}

	BinaryTreeSortTree(T* arr, size_t count)
	{
		build_tree(arr, count);
	}

	void build_tree(T* arr, size_t count)
	{
		value = arr[0];

		for (int i = 1; i < count; i++)
			add_leaf(arr[i]);
	}

	void add_leaf(T value_to_place)
	{
		BinaryTreeSortTree* current_tree = this;
		while (true)
		{
			if (count_units(value_to_place) >= count_units(current_tree->value))
			{
				if (current_tree->right == nullptr)
				{
					current_tree->right = new BinaryTreeSortTree<T>(value_to_place);
					current_tree->right->parent = this;
					break;
				}
				else
					current_tree = current_tree->right;
			}
			else
			{
				if (current_tree->left == nullptr)
				{
					current_tree->left = new BinaryTreeSortTree<T>(value_to_place);
					current_tree->left->parent = this;
					break;
				}
				else
					current_tree = current_tree->left;
			}
		}
	}
};


template<class T>
void tree_sort(T* arr, size_t count)
{
	//T* sorted_array = new T[count];

	BinaryTreeSortTree<T>* tree = new BinaryTreeSortTree<T>(arr, count);

	BinaryTreeSortTree<T>* minTree = tree;
	while (minTree->left != nullptr)
	{
		minTree = minTree->left;
	}

	
	arr[0] = minTree->value;
	BinaryTreeSortTree<T>* currentTree = minTree;
	currentTree->marked = true;
	int id_to_put_in = 1;
	while (true)
	{
		bool left_marked = true;
		bool right_marked = true;

		if (currentTree->left != nullptr)
			left_marked = currentTree->left->marked;
		if (currentTree->right != nullptr)
			right_marked = currentTree->right->marked;

		if (left_marked && right_marked && currentTree->parent == nullptr)
			break;

		if (currentTree->left != nullptr)
		{
			if (!currentTree->left->marked)
			{
				currentTree = currentTree->left;
				continue;
			}
		}

		if (!currentTree->marked)
		{
			currentTree->marked = true;
			arr[id_to_put_in] = currentTree->value;
			id_to_put_in++;
		}

		if (currentTree->right != nullptr)
		{
			if (!currentTree->right->marked)
			{
				currentTree = currentTree->right;
				continue;
			}
		}

		currentTree = currentTree->parent;
	}
}


template<class T>
double tree_sort_time(size_t sample_size)
{
	T* arr = new T[sample_size];
	size_t max_value = pow(2, (sizeof(T) * 8)) - 1;
	for (int i = 0; i < sample_size; i++)
		arr[i] = get_random_val<unsigned long long>(0, max_value);

	auto begin = std::chrono::steady_clock::now();
	tree_sort(arr, sample_size);
	auto end = std::chrono::steady_clock::now();

	return std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() * 1e-9;
}


int main()
{
	std::cout << tree_sort_time<unsigned char>(50000) << std::endl;
	std::cout << tree_sort_time<unsigned short>(50000) << std::endl;
	std::cout << tree_sort_time<unsigned int>(50000) << std::endl;
	std::cout << tree_sort_time<unsigned long long>(50000) << std::endl;
}