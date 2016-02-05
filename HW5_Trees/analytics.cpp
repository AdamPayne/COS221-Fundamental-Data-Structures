#include <iostream>
#include <iomanip>
#include <vector>
#include <ctype.h>
#include <time.h>
using namespace std;

//implementation of Binary Search Tree
template <typename Comparable>
class BinarySearchTree{
public:
	BinarySearchTree() :root(NULL){}

	BinarySearchTree(const BinarySearchTree & rhs) : root(NULL){
		*this = rhs;
	}

	~BinarySearchTree(){
		makeEmpty();
	}

	bool isEmpty() const{
		return root == NULL;
	}

	void makeEmpty(){
		makeEmpty(root);
	}

	void insert(const Comparable & x,int& count){
		insert(x, root, count);
	}
private:
	struct BinaryNode{
		Comparable element;
		BinaryNode *left;
		BinaryNode *right;

		BinaryNode(const Comparable & theElement, BinaryNode *lt, BinaryNode *rt)
			: element(theElement), left(lt), right(rt) { }
	};

	BinaryNode *root;

	void insert(const Comparable & x, BinaryNode * & t, int& count){
		if (t == NULL){
			count++;		// 1 creation of a new node
			t = new BinaryNode(x, NULL, NULL);
		}
		else if (x < t->element){
			count++;		// 1 comparison (x < t->element)
			insert(x, t->left, count);
		}
		else if (t->element < x){
			count+=2;		// 2 comparisons (x < t->element and t->element < x)
			insert(x, t->right, count);
		}
		else
			;  // Duplicate; do nothing
		// The vectors are full with different numbers so there is no point in writing count+=3
	}

	void makeEmpty(BinaryNode * & t){
		if (t != NULL){
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}
		t = NULL;
	}
};

template <typename Comparable>
class AvlTree{
public:
	AvlTree() : root(NULL){ }

	AvlTree(const AvlTree & rhs) : root(NULL){
		*this = rhs;
	}

	~AvlTree(){
		makeEmpty();
	}

	bool isEmpty() const{
		return root == NULL;
	}

	void makeEmpty(){
		makeEmpty(root);
	}

	void insert(const Comparable & x, int& count){
		insert(x, root, count);
	}
private:
	struct AvlNode{
		Comparable element;
		AvlNode   *left;
		AvlNode   *right;
		int       height;

		AvlNode(const Comparable & theElement, AvlNode *lt,
			AvlNode *rt, int h = 0)
			: element(theElement), left(lt), right(rt), height(h) { }
	};

	AvlNode *root;

	void insert(const Comparable & x, AvlNode * & t, int& count){
		if (t == NULL){
			count++;				// 1 creation of a new node
			t = new AvlNode(x, NULL, NULL);
		}
		else if (x < t->element){
			count++;				// 1 comparison (x < t->element)
			insert(x, t->left, count);
			if (height(t->left) - height(t->right) == 2){
				count++;			// the if that follows
				if (x < t->left->element){
					rotateWithLeftChild(t);
				}
				else{
					doubleWithLeftChild(t);
				}
			}
		}
		else if (t->element < x){
			count += 2;				// 2 comparisons (x < t->element and t->element < x)
			insert(x, t->right, count);
			if (height(t->right) - height(t->left) == 2){
				count++;			// the if that follows
				if (t->right->element < x){
					rotateWithRightChild(t);
				}
				else{
					doubleWithRightChild(t);
				}
			}
		}
		else
			;  // Duplicate; do nothing
		// The vectors are full with different numbers so there is no point in writing count+=3
		t->height = max(height(t->left), height(t->right)) + 1;
	}

	// used with nodes for which it is unknown whether they really exist (nodes pointing to NULL)
	int height(AvlNode *t) const{
		return t == NULL ? -1 : t->height;
	}

	// return maximum of two integer numbers
	int max(int lhs, int rhs) const{
		return lhs > rhs ? lhs : rhs;
	}

	// implement single rotation functions
	void rotateWithLeftChild(AvlNode * & k2){
		AvlNode *k1 = k2->left;
		k2->left = k1->right;
		k1->right = k2;
		k2->height = max(height(k2->left), height(k2->right)) + 1;
		k1->height = max(height(k1->left), k2->height) + 1;
		k2 = k1;
	}

	void rotateWithRightChild(AvlNode * & k1){
		AvlNode *k2 = k1->right;
		k1->right = k2->left;
		k2->left = k1;
		k1->height = max(height(k1->left), height(k1->right)) + 1;
		k2->height = max(height(k2->right), k1->height) + 1;
		k1 = k2;
	}

	// implement double rotation functions
	void doubleWithLeftChild(AvlNode * & k3){
		rotateWithRightChild(k3->left);
		rotateWithLeftChild(k3);
	}

	void doubleWithRightChild(AvlNode * & k1){
		rotateWithLeftChild(k1->right);
		rotateWithRightChild(k1);
	}

	void makeEmpty(AvlNode * & t){
		if (t != NULL){
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}
		t = NULL;
	}
};

#define BLC_SIZE 512
#define BLC_PTR 8
#define KEY_SIZE 16
#define M 7			// the order

typedef int dtype;	// the key

enum status
	{Insert_not_complete, Success, Duplicate_key, Underflow, Not_found};

struct Dat_blc {
	char dat[BLC_SIZE];
};

class B_node{
public:
	B_node() {}
//  int M;		// B-tree order: max number of ptrs to other nodes
	int n;		// Number of items actually stored in the node (n<M)
	dtype k[M - 1];	// Keys: n are actually used
	Dat_blc *q[M - 1];// Pointers to data blocks: n
	B_node *p[M];	// Pointers to other nodes: n+1 in use
};

class	B_tree {
public:
	B_tree() :root(NULL){}
	void	insert(dtype x, int& countN, int& countJ);
private:
	B_node 	*root;
	status	ins(B_node *r, dtype x, dtype &y, B_node* &u, int& countN, int& countJ);
	int	Node_Search(dtype x, const dtype *a, int n) const;
};

// use countN to count the number of comparisons within nodes
// use countJ to count the jumps in the tree (both up and down)
void B_tree::insert(dtype x, int& countN, int& countJ)
{
	B_node 	*p_new;
	dtype	x_new;
	status	code = ins(root, x, x_new, p_new, countN, countJ);
	if (code == Duplicate_key)
		cout << "Duplicate key: ignored\n";
	if (code == Insert_not_complete)
	{
		countJ++;			// there is a jump since the original root is split and a new one is created along with two children so the tree grows in height
		B_node *root_0 = root;
		root = new B_node;
		root->n = 1;
		root->k[0] = x_new;
		root->p[0] = root_0;
		root->p[1] = p_new;
	}
}

status 	B_tree::ins(B_node *r, dtype x, dtype &y, B_node* &q, int& countN, int& countJ)
//	Insert x in *this. If not completely successful,
//	the dtype y and B_node q remain to be inserted.
//	Return value: Success, Duplicate_key, Insert_not_complete
{
	B_node 	*p_new, *p_final;
	dtype	x_new, k_final;
	int     i, j, n;
	status	code;

	if (r == NULL) { q = NULL; y = x; return Insert_not_complete; } //empty root

	n = r->n;
	i = Node_Search(x, r->k, n);
	countN += (i + (i == n ? 0 : 1));		// add the number of comparisons in Node_Search
	countN += (i==n ? 0 : 1);				// the number of comparisons in the if that follows
	if (i < n && x == r->k[i]) return Duplicate_key;

	countJ++;		// a jump to the next level down is made
	code = ins(r->p[i], x, x_new, p_new, countN, countJ);
	if (code != Insert_not_complete) return code;
	// Insertion in subtree did not completely succeed.
	// Try to insert x_new and p_new in the current node.
	countJ++;	// a jump 1 level up is made
	if (n < M - 1)
	{     			    // There are space to insert the new item
		i = Node_Search(x_new, r->k, n);   // i - location of the new item
		countN += (i + (i == n ? 0 : 1));		// add the number of comparisons in Node_Search
		for (j = n; j>i; j--)	            // Open the slot for the new item
		{
			r->k[j] = r->k[j - 1];
			r->p[j + 1] = r->p[j];
		}
		r->k[i] = x_new;                             // Store the new item
		r->p[i + 1] = p_new;
		++r->n;
		return Success;
	}
	// Current node is full (n == M-1) and will be split.
	// Pass the middle item k[h] back as the parameter y to move upward
	// in the tree. Also, pass a pointer to the newly created node as q.
	if (i == M - 1) 	{ k_final = x_new; p_final = p_new; }
	else {
		k_final = r->k[M - 2]; p_final = r->p[M - 1]; // Store the last item
		for (j = M - 2; j>i; j--)                     // Open the slot for the new item
		{
			r->k[j] = r->k[j - 1];
			r->p[j + 1] = r->p[j];
		}
		r->k[i] = x_new;                             // Store the new item
		r->p[i + 1] = p_new;
	}
	int h = (M - 1) / 2;	// The middle
	y = r->k[h];		// y and q are passes back to the
	q = new B_node;		// next higher level in the tree
	r->n = h;		// The left items are kept in *r.
	q->n = M - 1 - h;	// The right items plus k_final are
	for (j = 0; j< q->n; j++)  // placed in the new node *q
	{
		q->p[j] = r->p[j + h + 1];
		q->k[j] = (j < q->n - 1 ? r->k[j + h + 1] : k_final);
	}
	q->p[q->n] = p_final;
	return Insert_not_complete;
}

int B_tree::Node_Search(dtype x, const dtype *a, int n) const
{
	int i = 0;
	while (i<n && x > a[i]) i++;
	return i;
}

// a function to generate vectors with random sequence of non-repeating integers
void generateVec(int);

int main(){
	generateVec(1000);
	generateVec(10000);
	generateVec(20000);
	system("pause");
	return 0;
}

void generateVec(int size){
	// use the current time to generate the random sequence
	srand(unsigned(time(0)));
	// maintain the number of comparisons in the following variables
	int countBST = 0, countAVL = 0, countBTn = 0, countBTj = 0;
	//create 5 different vectors of the same size
	for (int i = 0; i < 5; i++){
		vector<int> vec;
		for (int j = 0; j < size; j++){
			int temp = rand(); // generate a random number using rand
			// check if the number is not already contained in the vector
			if (find(vec.begin(), vec.end(), temp) != vec.end()){
				j--;
			}
			else{
				vec.push_back(temp);
			}
		}
		// create the trees
		BinarySearchTree<int> bst;
		AvlTree<int> avl;
		B_tree bt;
		// insert the numbers in the trees
		for (vector<int>::iterator it=vec.begin(); it != vec.end(); ++it){
			bst.insert(*it, countBST);
			avl.insert(*it, countAVL);
			bt.insert(*it, countBTn, countBTj);
		}
		bst.makeEmpty();
		avl.makeEmpty();
	}
	// output the statistics:
	cout << "Size: " << size << "\t\tAverages:\n";
	cout << "BST: " << (countBST / 5) << "\tAVL: " << (countAVL / 5) << "\tBTree within nodes: " << setw(6) << (countBTn / 5) << " jumps: " << countBTj / 5 << '\n';
	cout << "\n\n";
}