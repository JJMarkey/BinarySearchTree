/***************************************************************
<John Markey>
<BinaryTreeSymbolTable.hpp>
<Program 3>
<By implementing a symbol table using a Binary Search Tree, 
the following functions are:

O(n): put, get, remove, keys(low, high), min, max, deleteMin(), 
	  deleteMax(), rank, floor, ceiling

O(1): empty, size(BST node), size, 
>
***************************************************************/
#ifndef TREE_SYMBOL_TABLE_H
#define TREE_SYMBOL_TABLE_H

#include <algorithm>

#include "SymbolTable.hpp"

template <typename Key, typename Value>
class BinaryTreeSymbolTable : public SymbolTable<Key, Value>
{
protected:

	struct BinaryTreeNode
	{
		Key _key;
		Value _value;
		BinaryTreeNode* _left;
		BinaryTreeNode* _right;
		unsigned _size;

		BinaryTreeNode(const Key& key = Key{},
			const Value& value = Value{},
			unsigned size = 0,
			BinaryTreeNode* ell = nullptr,
			BinaryTreeNode* r = nullptr)
			: _key{ key }, _value{ value }, _size{ size }, _left{ ell }, _right{ r } {}

		BinaryTreeNode(const BinaryTreeNode& that)
			: _key{ that._key }, _value{ that._value }, _size{ that._size }, _left{ that._left }, _right{ that._right } {}

		~BinaryTreeNode()
		{
			if (_left != nullptr)
			{
				delete _left;
				_left = nullptr;
			}
			if (_right != nullptr)
			{
				delete _right;
				_right = nullptr;
			}
			_size = 0;
		}
	};

	// Key value comparison (less than)
	bool keyLessThan(const Key& lhs, const Key& rhs) const { return lhs < rhs; }

	// Equality of key values
	bool keyEquals(const Key& lhs, const Key& rhs) const { return lhs == rhs; }

	// Equality of key values
	bool keyLessThanOrEquals(const Key& lhs, const Key& rhs) const
	{
		return keyEquals(lhs, rhs) || keyLessThan(lhs, rhs);
	}

	// The container of the <key, value> pairs
	BinaryTreeNode* _root;
public:

	BinaryTreeSymbolTable() : _root{ nullptr } {}

	virtual ~BinaryTreeSymbolTable() { delete _root; }

	// Puts key-value pair into the table
	virtual void put(const Key& key, const Value& val = Value{})
	{
		_root = putHelper(_root, key, val);
	}
	
	//removes key value pair
	virtual void remove(const Key& key)
	{
		removeHelper(key, _root);
	}	

	// acquire the value paired with key
	virtual bool get(const Key& key, Value& val = Value{}) const
	{
		if (empty()) return false;
		BinaryTreeNode* temp = _root;
		if (temp == nullptr) return false;

		//runs until either a return statement is met, or until temp is nullptr
		while (temp != nullptr)
		{

			//if temp key is equivalent to key, val = temp val and returns true
			if (keyEquals(temp->_key, key))
			{
				val = temp->_value;
				return true;
			}

			else if (keyLessThan(key, temp->_key)) temp = temp->_left;
			else if (keyLessThan(temp->_key, key)) temp = temp->_right;
		}
		return false;
	}

	// Is there a value paired with key?
	virtual bool contains(const Key& key) const
	{
		return containsHelper(key, _root);
	}

	// Is the table empty?
	virtual bool empty() const
	{
		if (_root == nullptr) return true;
		return false;
	}

	//Number of key-value pairs.
	virtual int size() const
	{
		if (_root == nullptr) return 0;
		return _root->_size;
	}

	// Delete the smallest key
	virtual bool deleteMin()
	{
		if (empty()) return false;
		Key keys;
		min(keys);
		remove(keys);
		return true;
	}

	// Delete the largest key
	virtual bool deleteMax()
	{
		if (empty()) return false;
		Key keys;
		max(keys);
		remove(keys);
		return true;
	}

	//returns true if lowest value found
	virtual bool min(Key& key) const
	{
		if (empty()) return false;
		BinaryTreeNode* temp = _root;
		while (temp->_left != nullptr) temp = temp->_left;
		key = temp->_key;
		return true;
	}

	//returns true if highest val found
	virtual bool max(Key& key) const
	{
		if (empty()) return false;
		BinaryTreeNode* temp = findMax(_root);
		key = temp->_key;
		return true;
	}

	// Largest key less than or equal to key
	virtual bool floor(const Key& key, Key& floorKey) const
	{
		if (empty()) return false;

		Key mins;
		min(mins);

		if (keyLessThan(key, mins)) return false;
		BinaryTreeNode* temp = _root;

		while (temp != nullptr)
		{
			if (keyEquals(key, temp->_key))
			{
				floorKey = temp->_key;
				return true;
			}
			if (keyLessThan(key, temp->_key)) temp = temp->_left;

			else 
			{
				floorKey = temp->_key;
				temp = temp->_right;
			}
		}
		return true;
	}

	// Smallest key greater than or equal to key
	virtual bool ceiling(const Key& key, Key& ceilingKey) const
	{
		if (empty()) return false;

		Key maxes;
		max(maxes);
		if (keyLessThan(maxes, key)) return false;

		int k = 0;
		k = rank(key);

		select(k, ceilingKey); 
		return true;
	}

	//returns number of keys lower than key
	virtual int rank(const Key& key) const
	{
		if (empty()) return 0;

		BinaryTreeNode* temp = _root;
		int count = 0;

		while (temp != nullptr)
		{
			//if key is less than temp key, temp is temp left
			if (keyLessThan(key, temp->_key)) temp = temp->_left;

			//if temp is less than key and left is not nullptr, count is count + temp left size
			//temp shifts right
			else if (keyLessThan(temp->_key, key))
			{
				if (temp->_left != nullptr) count = count + temp->_left->_size;
				count++;
				temp = temp->_right;
			}

			//if temp equals key and left is not null, count is count + temp left size
			else if (keyEquals(temp->_key, key))
			{
				if (temp->_left != nullptr)
					count = count + temp->_left->_size;
				break;
			}
		}
		return count;
	}

	// key of rank k
	virtual bool select(int k = 0, Key& key = Key{}) const
	{
		BinaryTreeNode* temp = _root;
		if (empty()) return false;
		while (temp != nullptr)
		{
			//if k is greater than the rank of temp and temp right is not null
			//temp is temp ->_right
			if (k > rank(temp->_key))
			{
				if (temp->_right != nullptr)
					temp = temp->_right;
			}

			//if k is less than the rank of temp and temp left is not null
			//temp is temp ->_left
			else if (k < rank(temp->_key))
			{
				if (temp->_left != nullptr)
					temp = temp->_left;
			}
			//if k is equal to rank of temp, key is equal to temp key and return true
			else  if (k == rank(temp->_key))
			{
				key = temp->_key;
				return true;
			}
		}
	}
	// number of keys in [low, high] (including low, high)
	virtual int size(const Key& low, const Key& high) const
	{
		Key mins;
		min(mins);
		if (empty()) return 0;
		//if high > low, return 0
		else if (keyLessThan(high, low)) return 0;
		//if low and high are equal but low isnt in tree, return 0
		else if (keyEquals(low, high) && !contains(low)) return 0;
		//if low and high are equal, and low is in tree, return 1
		else if (keyEquals(low, high) && contains(low)) return 1;
		//if low and high are in tree, return rank of high - low + 1
		else if (contains(low) && contains(high)) return (rank(high) - rank(low) + 1);
		//if low is in tree, but high is out of bound, return rank of  high - low +1
		else if (contains(high) && !contains(low)) return (rank(high) - rank(low) + 1);
		//return rank of high and low
		return (rank(high) - rank(low));
	}

	// keys in [low, high] (including low, high), in sorted order
	virtual std::vector<Key> keys(const Key& low, const Key& high) const
	{
		std::vector<Key> keyset;
		Key temp;
		if (rank(low) == rank(high)) return keyset;
		for (int i = rank(low); i <= rank(high); i++)
		{
			select(i, temp);
			keyset.push_back(temp);
		}
		return keyset;
	}
	

public:
	// all keys in the table, in sorted order
	virtual std::vector<Key> keys() const
	{
		Key minimum, maximum;
		min(minimum);
		max(maximum);
		return keys(minimum, maximum);
	}

public:
	// Removes all elements from the table
	virtual void clear()
	{
		if (_root != nullptr) delete _root;
		_root = nullptr;
	}


private:

	// Returns the height of the BST (for debugging).
	// @return the height of the BST (a 1-node tree has height 0)
	int height() const { return height(root); }
	int height(BinaryTreeNode* node) const
	{
		if (node == nullptr) return -1;

		return 1 + std::max(height(node->_left), height(node->_right));
	}

	// Non-recursive node deletion.
	BinaryTreeNode* Delete(BinaryTreeNode* node) const
	{
		node->_left = nullptr;
		node->_right = nullptr;
		delete node;
		node = nullptr;
		return node;
	}

	//
	///////////////////////////////////////////////////////////////////////////////
	// Check integrity of BST data structure.
	///////////////////////////////////////////////////////////////////////////////
	//
	bool check() const
	{
		if (!isBST())            std::cout << "Not in symmetric order" << std::endl;
		if (!isSizeConsistent()) std::cout << "Subtree counts not consistent" << std::endl;
		if (!isRankConsistent()) std::cout << "Ranks not consistent" << std::endl;

		return isBST() && isSizeConsistent() && isRankConsistent();
	}

	//
	// does this binary tree satisfy symmetric order?
	// Note: this test also ensures that data structure is a binary tree since order is strict
	//
	bool isBST() const { return isBST(_root, nullptr, nullptr); }
	// is the tree rooted at x a BST with all keys strictly between min and max
	// (if min or max is null, treat as empty constraint)
	bool isBST(BinaryTreeNode* node, Key* min, Key* max) const
	{
		if (node == nullptr) return true;

		if (min != nullptr && keyLessThanOrEquals(node->_key, *min)) return false;

		if (max != nullptr && keyLessThanOrEquals(*max, node->_key)) return false;

		return isBST(node->_left, min, &(node->_key)) && isBST(node->_right, &(node->_key), max);
	}

	// are the size fields correct?
	bool isSizeConsistent() const { return isSizeConsistent(_root); }
	bool isSizeConsistent(BinaryTreeNode* node) const
	{
		if (node == nullptr) return true;

		// The size of this need must equate to the sum of its children (plus itself)
		if (node->_size != size(node->_left) + size(node->_right) + 1) return false;

		return isSizeConsistent(node->_left) && isSizeConsistent(node->_right);
	}

	// check that ranks are consistent
	bool isRankConsistent() const
	{
		// The i th node should be rank i
		for (int i = 0; i < size(); i++)
		{
			Key key;
			select(i, key);
			if (i != rank(key)) return false;
		}

		// All keys must equate to the key acquired at its rank 
		for (Key key : keys())
		{
			Key acquired;
			select(rank(key), acquired);

			if (!keyEquals(key, acquired)) return false;
		}

		return true;
	}
	//CONTAINS ALL HELPERS
private:

	BinaryTreeNode* putHelper(BinaryTreeNode* node, const Key& key, const Value& value)
	{
		if (node == nullptr) return new BinaryTreeNode(key, value, 1);

		else if (keyLessThan(key, node->_key)) node->_left = putHelper(node->_left, key, value);

		else if (keyLessThan(node->_key, key)) node->_right = putHelper(node->_right, key, value);

		else node->_value = value; // Duplicate: overwrite the value

		node->_size = 1 + size(node->_left) + size(node->_right);

		return node;
	}
	
	//returns the bool value if key is found
	bool containsHelper(const Key& key, BinaryTreeNode* node) const
	{
		if (node == nullptr) return false;
		else if (keyLessThan(key, node->_key)) return containsHelper(key, node->_left);
		else if (keyLessThan(node->_key, key)) return containsHelper(key, node->_right);
		else return true;
	}

	//returns max node
	BinaryTreeNode* findMax(BinaryTreeNode* node) const
	{
		if (node != nullptr)
			while (node->_right != nullptr)
				node = node->_right;
		return node;
	}

	//returns the size of a node
	int size(const BinaryTreeNode* const node) const { return node == nullptr ? 0 : node->_size; }

	// remove key (and its value) from table
	void removeHelper(const Key& key, BinaryTreeNode* node)
	{
		//if empty, end remove
		if (node == nullptr) return;

		//if key less than node key, shift into node's left node, then
		//scale node size
		else if (keyLessThan(key, node->_key) && node->_left != nullptr)
		{
			removeHelper(key, node->_left);
			node->_size = 1 + size(node->_left) + size(node->_right);
		}

		//if key less than node key, shift into node's left node, then
		//rescale node size
		else if (keyLessThan(node->_key, key) && node->_right != nullptr)
		{
			removeHelper(key, node->_right);
			node->_size = 1 + size(node->_left) + size(node->_right);
		}

		//if key equals node key, then determine the children
		else if (keyEquals(node->_key, key))
		{
			//NO CHILDREN
			if (node->_left == nullptr && node->_right == nullptr)
			{
				node = Delete(node);
			}

			//1 CHILD(right subtree)
			else if (node->_left == nullptr && node->_right != nullptr)
			{
				BinaryTreeNode* temp = node;
				node = node->_right;
				temp = Delete(temp);
			}

			//1 CHILD (left subtree)
			else if (node->_left != nullptr && node->_right == nullptr)
			{
				BinaryTreeNode* temp = node;
				node = node->_left;
				temp = Delete(temp);
			}

			//2 CHILDREN
			//finds the max of the left subtree, then sets node to max
			//rescales node size
			else if (node->_left != nullptr && node->_right != nullptr)
			{
				BinaryTreeNode* temp = node;
				BinaryTreeNode* max = findMax(node->_left);
				node->_key = max->_key;
				node->_value = max->_value;
				temp = Delete(temp);
				max = Delete(max);

				node->_size = 1 + size(node->_left) + size(node->_right);
			}
		}
	}
};

#endif