#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include <limits>

struct Node
{
    int key;
    struct Node* left, * right;
};


struct Node* newNode(int item)
{
    struct Node* temp = new Node;
    temp->key = item;
    temp->left = temp->right = nullptr;
    return temp;
}


void storeSorted(Node* root, int arr[], int& i)
{
    if (root != nullptr)
    {
        storeSorted(root->left, arr, i);
        arr[i++] = root->key;
        storeSorted(root->right, arr, i);
    }
}

Node* insert(Node* node, int key)
{

    if (node == nullptr) return newNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);

    return node;
}

void treeSort(int arr[], int n)
{
    struct Node* root = nullptr;

    root = insert(root, arr[0]);
    for (int i = 1; i < n; i++)
        root = insert(root, arr[i]);

    int i = 0;
    storeSorted(root, arr, i);
}

std::random_device rd;
std::mt19937 gen(rd());

template<class T>
int get_random_val(T min_value, T max_value) {
	std::uniform_int_distribution<T> distrib(min_value, max_value);
	return distrib(gen);
}

double tree_sort_time(size_t sample_size)
{
	int* arr = new int[sample_size];
	size_t max_value = pow(2, (sizeof(int) * 8)) - 1;
	for (int i = 0; i < sample_size; i++)
		arr[i] = get_random_val<unsigned long long>(0, max_value);

    treeSort(arr, sample_size);

	auto begin = std::chrono::steady_clock::now();
    treeSort(arr, sample_size);
	auto end = std::chrono::steady_clock::now();

	return std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() * 1e-9;
}


int main()
{
    int iterations_count = 50;
    int increment = 250;
    int sample_size = 0;
    for (int i = 0; i < iterations_count; i++)
    {
        double avarage_time = 0;
        for (int j = 0; j < 50; j++)
            avarage_time += tree_sort_time(sample_size);
        avarage_time /= 50;

        std::cout << avarage_time << std::endl;
        sample_size += increment;
    }

    return 0;
}