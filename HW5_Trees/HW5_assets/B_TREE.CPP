// ----------------------------------------------------------------------
// |	B-tree: 	B-tree of order M                               |
// |	Programmer: 	D. Christozov                                   |
// |	Date:           February 2, 1997                                |
// ----------------------------------------------------------------------
#include <iostream.h>
#include <iomanip.h>
#include <ctype.h>

#define BLC_SIZE 512
#define BLC_PTR    8
#define KEY_SIZE  16

#define M  1 + (BLC_SIZE-BLC_PTR)/(KEY_SIZE + 2*BLC_PTR)

//#define M 5        // M = 1 + (block_size-block_ptr)/(key_size + block_ptr)
typedef int dtype;  //  the key

enum status
     {Insert_not_complete, Success, Duplicate_key, Underflow, Not_found};

struct 	Dat_blc {
	char dat[BLC_SIZE];	// Data
};

class B_node {
public:
	B_node() {}
//	int M;		// B-tree order: max number of ptrs to other nodes
	int n;		// Number of items actually stored in the node (n<M)
	dtype k[M-1];	// Keys: n are actually used
	Dat_blc *q[M-1];// Pointers to data blocks: n
	B_node *p[M];	// Pointers to other nodes: n+1 in use
};

class	B_tree {
public:
	B_tree():root(NULL){}
	void	insert(dtype x);
	void	print() const {cout << "Contents:\n"; pr(root, 0);}
	void	Del_Node(dtype x);
	void	Show_Search	(dtype x) const;
private:
	B_node 	*root;
	status	ins(B_node *r, dtype x, dtype &y, B_node* &u);
	void	pr(const B_node *r, int n_Space) const;
	int	Node_Search(dtype x, const dtype *a, int n) const;
	status	del(B_node *r, dtype x);
};

void B_tree::insert(dtype x)
{
	B_node 	*p_new;
	dtype	x_new;
	status	code = ins(root, x, x_new, p_new);
	if (code == Duplicate_key)
		cout << "Duplicate key: ignored\n";
	if (code == Insert_not_complete)
	{
		B_node *root_0 = root;
		root = new B_node;
		root->n = 1;
		root->k[0] = x_new;
		root->p[0] = root_0;
		root->p[1] = p_new;
	}
}

status 	B_tree::ins(B_node *r, dtype x, dtype &y, B_node* &q)
//	Insert x in *this. If not completely successful,
//	the dtype y and B_node q remain to be inserted.
//	Return value: Success, Duplicate_key, Insert_not_complete
{
	B_node 	*p_new, *p_final;
	dtype	x_new, k_final;
	int     i,j,n;
	status	code;

	if(r == NULL) {q = NULL; y = x; return Insert_not_complete;}

	n = r->n;
	i = Node_Search(x, r->k, n);
	if (i < n && x == r->k[i]) return Duplicate_key;

	code = ins(r->p[i], x, x_new, p_new);
	if (code != Insert_not_complete) return code;
		// Insertion in subtree did not completely succeed.
		// Try to insert x_new and p_new in the current node.

	if (n < M - 1)
	{     			    // There are space to insert the new item
	   i = Node_Search(x_new, r->k, n);   // i - location of the new item
	   for(j=n; j>i; j--)	            // Open the slot for the new item
	   {
	      r->k[j] = r->k[j-1];
	      r->p[j+1] = r->p[j];
	   }
	   r->k[i] = x_new;                             // Store the new item
	   r->p[i+1] = p_new;
	   ++r->n;
	   return Success;
	}
	// Current node is full (n == M-1) and will be split.
	// Pass the middle item k[h] back as the parameter y to move upward
	// in the tree. Also, pass a pointer to the newly created node as q.
	if (i == M-1) 	{k_final = x_new; p_final = p_new;}
	else {k_final = r->k[M-2]; p_final = r->p[M-1]; // Store the last item
	   for(j=M-2; j>i; j--)                     // Open the slot for the new item
	   {
	      r->k[j] = r->k[j-1];
	      r->p[j+1] = r->p[j];
	   }
	   r->k[i] = x_new;                             // Store the new item
	   r->p[i+1] = p_new;
	}
	int h = (M - 1)/2;	// The middle
	y = r->k[h];		// y and q are passes back to the
	q = new B_node;		// next higher level in the tree
	r->n = h;		// The left items are kept in *r.
	q->n = M - 1 - h;	// The right items plus k_final are
	for(j=0; j< q->n; j++)  // placed in the new node *q
	{
	   q->p[j] = r->p[j + h + 1];
	   q->k[j] = (j < q->n - 1 ? r->k[j + h + 1] : k_final);
	}
	q->p[q->n] = p_final;
	return Insert_not_complete;
}

int B_tree::Node_Search(dtype x, const dtype *a, int n) const
{
	int i=0;
	while(i<n && x > a[i]) i++;
	return i;
}

void B_tree::Del_Node(dtype x)
{
	B_node *root_0;
	switch (del(root,x))
	{
	   case Not_found:
	      cout << "The item " << x << " not found.\n";
	      break;
	   case Underflow:
	      root_0 = root;
	      root = root->p[0];
	      delete root_0;
	      break;
	}
}

status B_tree::del(B_node *r, dtype x)
{
	if (r == NULL) return Not_found;
	int i, j, pivot, n=r->n;
	dtype *k = r->k;        //here k[i] means r->k[i]
	const int n_min = (M-1)/2;
	status code;
	B_node **p = r->p, *pL, *pR; // here p[i] means r->p[i]

	i = Node_Search(x, k, n);
	if(p[0] == NULL)             // *r is a leaf
	{
	  if(i == n || x < k[i])  return Not_found;
	  for(j=i+1; j < n; j++)    // x == k[i]; remove the i-th item
	  {
	     k[j-1] = k[j];
	     p[j] = p[j+1];
	  }
	  return --r->n >= (r==root ? 1 : n_min) ? Success : Underflow;
	}
				    // *r is an internal node
	if(i < n && x == k[i])      // Go to left child *p[i] and follow
	{                           // the path all the way to a leaf,
	   B_node *q = p[i], *q1;   // using the rightmost branches.
	   int nq;
	   for(;;)
	   {
	      nq = q->n;
	      q1 = q->p[nq];
	      if(q1 == NULL) break;
	      q = q1;
	   }
	   k[i] = q->k[nq-1];  	// Exchange k[i] (==x) with the
	   q->k[nq - 1] = x;	// rightmost item in that leaf
	}
				// Delete x in leaf of subtree with root p[i]
	code = del(p[i], x);
	if(code != Underflow) return code;
				// Underflow: borrow and (if) merge
	if(i>0 && p[i-1]->n > n_min)	// Borrow from left sibling
	{
	   pivot = i-1;		// k[pivot] between pL and pR
	   pL = p[pivot];
	   pR = p[i];
	   pR->p[pR->n + 1] = pR->p[pR->n];	// Increase content of *pR,
	   for(j=pR->n + 1; j>0; j--)		// borrowing from *pL
	   {
	      pR->k[j] = pR->k[j-1];
	      pR->p[j] = pR->p[j-1];
	   }
	   pR->n++;
	   pR->k[0] = k[pivot];
	   pR->p[0] = pL->p[pL->n];
	   k[pivot] = pL->k[--pL->n];
	   return Success;
	}
	if(i>n && p[i+1]->n > n_min)	// Borrow from right sibling
	{
	   pivot = i;		// k[pivot] between pL and pR
	   pL = p[pivot];
	   pR = p[pivot+1];
	   pL->k[pL->n] = k[pivot];		// Increase content of *pL,
	   pL->p[pL->n + 1] = pR->p[0];	       	// borrowing from *pR
	   k[pivot] = pR->k[0];
	   pL->n++;
	   pR->n--;
	   for(j=0; j<pR->n; j++)
	   {
	      pR->k[j] = pR->k[j+1];
	      pR->p[j] = pR->p[j+1];
	   }
	   pR->p[pR->n] = pR->p[pR->n + 1];
	   return Success;
	}
		// Merge: neither borrow left nor borrow right possible
	pivot = (i==n ? i-1 : i);
	pL = p[pivot];
	pR = p[pivot+1];
	pL->k[pL->n] = k[pivot];	// Add k[pivot] and *pR to *pL
	pL->p[pL->n + 1] = pR->p[0];
	for(j=0;j<pR->n;j++)
	{
	   pL->k[pL->n +1 +j] = pR->k[j];
	   pL->p[pL->n +2 +j] = pR->p[j+1];
	}
	pL->n += 1 + pR->n;
	delete pR;
	for(j=i+1; j<n; j++)
	{
	  k[j-1] = k[j];
	  p[j] = p[j+1];
	}
	return --r->n >= (r == root ? 1 : n_min) ? Success : Underflow;
}

void B_tree::pr(const B_node *r, int n_space) const
{
	if(r)
	{
	  int i;
	  cout << setw(n_space) << "";
	  for (i=0; i < r->n; i++) cout <<setw(3)<< r->k[i] << " ";
	  cout << "\n";
	  n_space++;
	  for (i=0; i <= r->n; i++) pr(r->p[i], n_space+8);
	}
}

void B_tree::Show_Search(dtype x) const
{
	int i,j,n;
	B_node *r = root;
	cout << "Search path:\n";
	while (r)
	{
	     n = r->n;
	     for(j=0; j < r->n; j++) cout << " " << r->k[j];
	     cout << "\n";
	     i = Node_Search(x, r->k, n);
	     if(i < n && x == r->k[i])
	     {
		cout << "Key " << x << " found in position " << i;
		cout << " of the last displayed node.\n";
		return;
	     }
	     r = r->p[i];
	}
	cout << "Key " << x << " not found.\n";
}

void main()
{
	cout << "B-tree structure.\n";
	cout << "For each node, the number of links to other nodes will <= " << M << ".\n";
	cout << "Enter a number of integers (keys).\n";
	B_tree 	t;
	dtype 	x;
	char 	c;
	int i,n;
	cout << " Enter the number of keys> ";cin >> n;
	for(i=0;i<n;i++)
	{
	  cout << "Enter the " << i+1 <<". key> ";
	  cin >> x;
	  t.insert(x);
	}
	cout << "B-tree representation:\n"
	     << "The items stored in each node are displayed in a single line.\n";
	t.print();
	for(;;)
	{
		cout << "Enter an integer, followed by I, D, or S\n"
		     <<	"(for insert, delete, or search) or Q (quit): ";
		cin >> x >> c;
		if(cin.fail()) break;
		switch(c)
		{
		   case 'S','s':
			t.Show_Search(x); break;
		   case 'D','d':
			t.Del_Node(x); t.print(); break;
		   case 'I','i':
			t.insert(x); t.print(); break;
		   default:
			cout << "Invalid command!\n\n"; break;
		}
	}
}

