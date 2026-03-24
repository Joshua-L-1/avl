#ifndef AVLMAP_H
#define AVLMAP_H

#include <iostream>

namespace CS280 {

    template< typename KEY_TYPE, typename VALUE_TYPE >
    class AVLmap {
		public:
			class Node {
				public:
					Node( KEY_TYPE k, VALUE_TYPE val, Node* p, int h, int b, Node* l, Node* r);

					Node(const Node&)               = delete;
					Node* operator=(const Node&)    = delete;
					
                    KEY_TYPE const & Key() const;   // return a const reference
                    VALUE_TYPE  &    Value();       // return a reference

					Node*  first(); // minimum - follow left as far as possible
					Node*  last(); // maximum - follow right as far as possible
					Node*  increment(); // successor
					Node*  decrement(); // predecessor
					void   print(std::ostream& os ) const;
				private:
                    KEY_TYPE    key;
					VALUE_TYPE  value;
					int         height,balance; // optional
					Node        *parent;
					Node        *left;
					Node        *right;

					friend class BSTmap;
			};
        private:
			struct AVLmap_iterator {
				private:
					Node* p_node;
				public:
					AVLmap_iterator(Node* p = nullptr) : p_node(p) {}
					AVLmap_iterator(const AVLmap_iterator&) = default;
					AVLmap_iterator& operator=(const AVLmap_iterator& rhs);
					AVLmap_iterator& operator++();
					AVLmap_iterator operator++(int);
                    Node& operator*();
					Node* operator->();
					bool operator!=(const AVLmap_iterator& rhs);
                    bool operator==(const AVLmap_iterator& rhs);
					friend class AVLmap;
			};
			struct AVLmap_iterator_const {
				private:
					Node* p_node;
				public:
					AVLmap_iterator_const(Node* p=nullptr) : p_node(p) {}
					AVLmap_iterator_const(const AVLmap_iterator_const&) = default;
					AVLmap_iterator_const& operator=(const AVLmap_iterator_const& rhs);
					AVLmap_iterator_const& operator++();
					AVLmap_iterator_const operator++(int);
                    Node const& operator*();
					Node const* operator->();
					bool operator!=(const AVLmap_iterator_const& rhs);
					bool operator==(const AVLmap_iterator_const& rhs);
					friend class AVLmap;
			};
            // AVLmap implementation
			Node* pRoot = nullptr;
            unsigned int size_ = 0;
			// end iterators are same for all AVLmaps, thus static
			// make AVLmap_iterator a friend
			// to allow AVLmap_iterator to access end iterators 
			static AVLmap_iterator end_it;
			static AVLmap_iterator_const const_end_it;
		public:
			//BIG FOUR
			AVLmap();
			AVLmap(const AVLmap& rhs);
			AVLmap& operator=(const AVLmap& rhs);
			virtual ~AVLmap();

            unsigned int size();

			//value setter and getter
			VALUE_TYPE& operator[](KEY_TYPE const& key);
			//next method doesn't make sense
			//because operator[] inserts a non-existing element
			//which is not allowed on const maps
			//VALUE_TYPE operator[](int key) const;

			//standard names for iterator types
			typedef AVLmap_iterator       iterator;
			typedef AVLmap_iterator_const const_iterator;

			//AVLmap methods dealing with non-const iterator 
			AVLmap_iterator begin();
			AVLmap_iterator end();
			AVLmap_iterator find(KEY_TYPE const& key);
			void erase(AVLmap_iterator it);

			//AVLmap methods dealing with const iterator 
			AVLmap_iterator_const begin() const;
			AVLmap_iterator_const end() const;
			AVLmap_iterator_const find(KEY_TYPE const& key) const;
			//do not need this one (why)
			//AVLmap_iterator_const erase(AVLmap_iterator& it) const;

			int getdepth(const Node* node) const;
			char getedgesymbol(const Node* node) const;
			void print(std::ostream& os, bool print_value = false) const;
			//bool sanityCheck();

			//inner class (AVLmap_iterator) doesn't have any special priveleges
			//in accessing private data/methods of the outer class (AVLmap)
			//so need friendship to allow AVLmap_iterator to access private "AVLmap::end_it"
			//BTW - same is true for outer class accessing inner class private data
			friend class AVLmap_iterator;
			friend class AVLmap_iterator_const;
		private:
			void del(Node* node);
			Node* copy(Node* source_node, Node* parent_ptr);
	};

	//notice that it doesn't need to be friend
    template< typename KEY_TYPE, typename VALUE_TYPE >
	std::ostream& operator<<(std::ostream& os, AVLmap<KEY_TYPE, VALUE_TYPE> const& map);
}

#include "avl-map.cpp"
#endif
