#include "avl-map.h"

// static data members
template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE,VALUE_TYPE>::BSTmap_iterator        
		CS280::BSTmap<KEY_TYPE,VALUE_TYPE>::end_it        = CS280::BSTmap<KEY_TYPE,VALUE_TYPE>::BSTmap_iterator(nullptr);

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE,VALUE_TYPE>::BSTmap_iterator_const  
		CS280::BSTmap<KEY_TYPE,VALUE_TYPE>::const_end_it  = CS280::BSTmap<KEY_TYPE,VALUE_TYPE>::BSTmap_iterator_const(nullptr);

//private functions
template< typename KEY_TYPE, typename VALUE_TYPE >
void CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::del(Node* node) {
	if (node == nullptr) return;

	// First, delete everything below this node
	del(node->left);
	del(node->right);

	// Now it is safe to delete this node
	delete node;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node* CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::copy(Node* source, Node* parent)
{
	if (source == nullptr) return nullptr;

	Node* node = new Node(source->key, source->value, parent, source->height, source->balance, nullptr, nullptr);

	node->left = copy(source->left, node);
	node->right = copy(source->right, node);

	return node;
}


//for the node class
template< typename KEY_TYPE, typename VALUE_TYPE >
CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node::Node(KEY_TYPE k, VALUE_TYPE val, Node* p, int h, int b, Node* l, Node* r) :
	key(k), value(val), height(h), balance(b), parent(p), left(l), right(r)
{
}

template< typename KEY_TYPE, typename VALUE_TYPE >
KEY_TYPE const& CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node::Key() const {
	return key;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
VALUE_TYPE& CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node::Value() {
	return value;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE,VALUE_TYPE>::Node* CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node::first() {
	Node* node = this;
	while (node->left != nullptr) {
		node = node->left;
	}
	return node;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node* CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node::last() {
	Node* node = this;
	while (node->right != nullptr) {
		node = node->right;
	}
	return node;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node* CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node::increment() {
	if (right != nullptr) {
		return right->first();
	}
	Node* prev = this;
	Node* node = parent;
	while (node != nullptr && prev == node->right) {
		prev = node;
		node = node->parent;
	}
	return node;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node* CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node::decrement() {
	if (left != nullptr) {
		return left->last();
	}
	Node* prev = this;
	Node* node = parent;
	while (node != nullptr && prev == node->left) {
		prev = node;
		node = node->parent;
	}
	return node;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
void CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node::print(std::ostream& os) const {
	os << '(' << key << ',' << value << ')';
}

//start of bstmap implementation
//in implementation file 
//method's name and return value have to be fully qualified with
//all required namespace and class names (like CS280::BSTmap<KEY_TYPE,VALUE_TYPE>::BSTmap)
//but the argument list does not have to be:
//function argument list automatically falls into the method's namespace (Koenig lookup)
template< typename KEY_TYPE, typename VALUE_TYPE >
CS280::BSTmap<KEY_TYPE,VALUE_TYPE>::BSTmap(const BSTmap& rhs) {
	pRoot = nullptr;
	size_ = 0;

	if (rhs.pRoot != nullptr) {
		pRoot = copy(rhs.pRoot, nullptr);
		size_ = rhs.size_;
	}
}


template< typename KEY_TYPE, typename VALUE_TYPE >
CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap() :
	pRoot(nullptr), size_(0)
{
}

template< typename KEY_TYPE, typename VALUE_TYPE >
CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::~BSTmap() {
	del(pRoot);
	pRoot = nullptr;
	size_ = 0;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
CS280::BSTmap<KEY_TYPE,VALUE_TYPE>& CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::operator=(const BSTmap& rhs) {
	BSTmap bst(rhs);
	std::swap(pRoot, bst.pRoot);
	std::swap(size_, bst.size_);
	return *this;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
unsigned int CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::size() {
	return size_;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
VALUE_TYPE& CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::operator[](KEY_TYPE const& rhs) {
	if (pRoot == nullptr) {
		size_++;
		pRoot = new Node(rhs, VALUE_TYPE(), nullptr, 0, 0, nullptr, nullptr);
		return pRoot->value;
	}

	Node* par = nullptr;
	Node* node = pRoot;

	while (node != nullptr) {
		if (node->key < rhs) {
			par = node;
			node = node->right;
		}
		else if (node->key > rhs) {
			par = node;
			node = node->left;

		}
		else {
			return node->value;
		}
	}
	node = new Node(rhs, VALUE_TYPE(), par, 0, 0, nullptr, nullptr);
	if (par->key < rhs)
		par->right = node;
	else
		par->left = node;
	size_++;
	return node->value;
}

//bstmap_interator
template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator& CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator::operator=(const BSTmap_iterator& rhs) {
	this->p_node = rhs.p_node;
	return *this;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator& CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator::operator++() {
	if (this->p_node != nullptr) {
		this->p_node = this->p_node->increment();
	}
	return *this;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator::operator++(int) {
	BSTmap_iterator temp = *this;
	++(*this);
	return temp;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node& CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator::operator*() {
	return *p_node;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node* CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator::operator->() {
	return p_node;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
bool CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator::operator!=(const BSTmap_iterator& rhs) {
	return this->p_node != rhs.p_node;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
bool CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator::operator==(const BSTmap_iterator& rhs) {
	return this->p_node == rhs.p_node;
}

////////////////////////////////////////////////////////////
// typename is used to help compiler to parse
template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE,VALUE_TYPE>::BSTmap_iterator CS280::BSTmap<KEY_TYPE,VALUE_TYPE>::begin() {
	if (pRoot) return BSTmap<KEY_TYPE,VALUE_TYPE>::BSTmap_iterator(pRoot->first());
	else       return end_it;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::end() {
	return end_it;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::find(KEY_TYPE const& key) {
	Node* node = pRoot;
	while (node != nullptr) {
		if (key == node->key) {
			return iterator(node);
		}
		else if (key < node->key) {
			node = node->left;
		}
		else {
			node = node->right;
		}
	}
	return end();
}

template< typename KEY_TYPE, typename VALUE_TYPE >
void CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::erase(BSTmap_iterator it) {
	Node* node = it.p_node;
	if (node == nullptr) {
		return;
	}

	Node* suc = (node->left == nullptr || node->right == nullptr) ? node : node->increment();
	Node* temp = (suc->left != nullptr) ? suc->left : suc->right;

	if (temp != nullptr) {
		temp->parent = suc->parent;
	}

	if (suc->parent == nullptr) {
		pRoot = temp;
	}
	else if (suc == suc->parent->left) {
		suc->parent->left = temp;
	}
	else {
		suc->parent->right = temp;
	}

	if (suc != node) {
		const_cast<KEY_TYPE&>(node->key) = suc->key;
		node->value = suc->value;
	}

	delete suc;
	size_--;
}

//bstmap_interator_const
template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const& CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const::operator=(const BSTmap_iterator_const& rhs) {
	this->p_node = rhs.p_node;
	return *this;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const& CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const::operator++() {
	if (this->p_node != nullptr) {
		this->p_node = this->p_node->increment();
	}
	return *this;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const::operator++(int) {
	BSTmap_iterator_const temp = *this;
	++(*this);
	return temp;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node const& CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const::operator*() {
	return *p_node;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::Node const* CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const::operator->() {
	return p_node;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
bool CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const::operator!=(const BSTmap_iterator_const& rhs) {
	return this->p_node != rhs.p_node;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
bool CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const::operator==(const BSTmap_iterator_const& rhs) {
	return this->p_node == rhs.p_node;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::begin() const {
	if (pRoot == nullptr) {
		return end();
	}
	return const_iterator(pRoot->first());
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::end() const {
	return const_end_it;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::BSTmap_iterator_const CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::find(KEY_TYPE const& key) const {
	Node* node = pRoot;
	while (node != nullptr) {
		if (key == node->key) {
			return const_iterator(node);
		}
		else if (key < node->key) {
			node = node->left;
		}
		else {
			node = node->right;
		}
	}
	return end();
}


template< typename KEY_TYPE, typename VALUE_TYPE >
int CS280::BSTmap<KEY_TYPE, VALUE_TYPE>::getdepth(const Node* node) const {
	int depth = 0;
	// Use a local pointer so we don't modify the argument
	const Node* curr = node;
	while (curr && curr->parent) {
		curr = curr->parent;
		depth++;
	}
	return depth;
}

////////////////////////////////////////////////////////////
// do not change this code from here to the end of the file
/* figure out whether node is left or right child or root 
 * used in print_backwards_padded 
 */
template< typename KEY_TYPE, typename VALUE_TYPE >
char CS280::BSTmap<KEY_TYPE,VALUE_TYPE>::getedgesymbol(const Node* node) const {
	const Node* parent = node->parent;
	if ( parent == nullptr) return '-';
	else                 return ( parent->left == node)?'\\':'/';
}

/* this is another "ASCII-graphical" print, but using 
 * iterative function. 
 * Left branch of the tree is at the bottom
 */
template< typename KEY_TYPE, typename VALUE_TYPE >
std::ostream& CS280::operator<<(std::ostream& os, BSTmap<KEY_TYPE,VALUE_TYPE> const& map) {
	map.print(os);
	return os;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
void CS280::BSTmap<KEY_TYPE,VALUE_TYPE>::print(std::ostream& os, bool print_value) const {
	if (pRoot) {
		BSTmap<KEY_TYPE,VALUE_TYPE>::Node* b = pRoot->last();
		while ( b ) { 
			int depth = getdepth(b);
			int i;
			/* printf(b); */

			char edge = getedgesymbol(b);
			switch (edge) {
				case '-': 
					for (i=0; i<depth; ++i) std::printf("       ");
					os << b->key;
                    if ( print_value ) { os << " -> " << b->value; }
                    os << std::endl;
					break;
				case '\\': 
					for (i=0; i<depth; ++i) std::printf("       ");
					os << edge << std::endl;
					for (i=0; i<depth; ++i) std::printf("       ");
					os << b->key;
                    if ( print_value ) { os << " -> " << b->value; }
                    os << std::endl;
					break;
				case '/': 
					for (i=0; i<depth; ++i) std::printf("       ");
					os << b->key;
                    if ( print_value ) { os << " -> " << b->value; }
                    os << std::endl;
					for (i=0; i<depth; ++i) std::printf("       ");
					os << edge << std::endl;
					break;
			}
			b = b->decrement();
		}
	}
	std::printf("\n");
}

//AVL stuff
template< typename KEY_TYPE, typename VALUE_TYPE >
VALUE_TYPE& CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::operator[](KEY_TYPE const& rhs) {
	Node* par = nullptr;
	Node* node = this->pRoot;

	while (node != nullptr) {
		if (node->key < rhs) {
			par = node;
			node = node->right;
		}
		else if (node->key > rhs) {
			par = node;
			node = node->left;

		}
		else {
			return node->value;
		}
	}

	node = new Node(rhs, VALUE_TYPE(), par, 0, 0, nullptr, nullptr);
	if (par == nullptr) {
		this->pRoot = node;
	}
	else if (par->key < rhs) {
		par->right = node;
	}
	else {
		par->left = node;
	}

	this->size_++;
	BalanceUp(par);

	return node->value;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
void CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Erase(typename BSTmap<KEY_TYPE, VALUE_TYPE>::iterator it) {
	Node* node = it.p_node;
	if (node == nullptr) {
		return;
	}

	Node* suc = (node->left == nullptr || node->right == nullptr) ? node : node->increment();
	Node* temp = (suc->left != nullptr) ? suc->left : suc->right;
	Node* par_suc = suc->parent;

	if (temp != nullptr) {
		temp->parent = suc->parent;
	}

	if (suc->parent == nullptr) {
		this->pRoot = temp;
	}
	else if (suc == suc->parent->left) {
		suc->parent->left = temp;
	}
	else {
		suc->parent->right = temp;
	}

	if (suc != node) {
		const_cast<KEY_TYPE&>(node->key) = suc->key;
		node->value = suc->value;
	}

	delete suc;
	this->size_--;

	if (par_suc) {
		balance_up(par_suc);
	}
	else if (this->pRoot) {
		update_node_stats(this->pRoot);
	}
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node* CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::RotateLeft(Node* n) {
	Node* t = n->right;
	Node* t2 = t->left;

	t->left = n;
	n->right = t2;

	t->parent = n->parent;
	n->parent = t;
	if (t2) {
		t2->parent = n;
	}

	UpdateNodeStats(n);
	UpdateNodeStats(t);

	return t;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::Node* CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::RotateRight(Node* n) {
	Node* t = n->left;
	Node* t2 = t->right;

	t->right = n;
	n->left = t2;

	t->parent = n->parent;
	n->parent = t;
	if (t2) {
		t2->parent = n;
	}

	UpdateNodeStats(n);
	UpdateNodeStats(t);

	return t;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
void CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::BalanceUp(Node* n) {
	while (n != nullptr) {
		UpdateNodeStats(n);
		int bal = GetBalance(n);
		Node* sub = n;

		if (bal > 1) {
			if (GetBalance(n->left) < 0) {
				n->left = RotateLeft(n->left);
			}
			sub = RotateRight(n);
		}
		else if (bal < -1) {
			if (GetBalance(n->right) > 0) {
				n->right = RotateRight(n->right);
			}
			sub = RotateLeft(n);
		}

		if (sub->parent == nullptr) {
			this->pRoot = sub;
		}
		else {
			if (sub->parent->left == n) {
				sub->parent->left = sub;
			}
			else {
				sub->parent->right = sub;
			}
		}

		n = sub->parent;
	}
}

template< typename KEY_TYPE, typename VALUE_TYPE >
int CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::GetHeight(Node* n) {
	return n ? n->height : -1;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
void CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::UpdateNodeStats(Node* n) {
	if (n == nullptr) {
		return;
	}

	int lh = GetHeight(n->left);
	int rh = GetHeight(n->right);

	n->height = 1 + std::max(lh, rh);
	n->balance = lh - rh;
}

template< typename KEY_TYPE, typename VALUE_TYPE >
int CS280::AVLmap<KEY_TYPE, VALUE_TYPE>::GetBalance(Node* n) {
	if (n == nullptr) {
		return 0;
	}

	return GetHeight(n->left) - GetHeight(n->right);
}