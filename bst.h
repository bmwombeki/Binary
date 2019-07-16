#ifndef _BST_H
#define _BST_H

#include <iostream>
#include <vector>

template <typename T>
class bst {
    
private:
    struct bst_node {
        T      val;
        bst_node *left;
        bst_node *right;
        int numLeftSubtrees;  // keep track of number of nodes in the left  subtree
        int numRightSubtrees;  // keep track of number of nodes in the right subtree
        
        
        bst_node ( const T & _val = T{}, bst_node * l = nullptr, bst_node *r = nullptr,int numLeftSubtree = 0,  int numRightSubtree = 0 )
        : val { _val },  left { l }, right {r} , numLeftSubtrees {numLeftSubtree}, numRightSubtrees {numRightSubtree}
        { }
    };
    
  
    
public:
    // constructor:  initializes an empty tree
    bst(){
        root = nullptr;
    }
    
    
private:
    // helper function which recursively deallocates nodes
    //   in a tree.
    static void delete_nodes(bst_node *r){
        if(r==nullptr) return;
        delete_nodes(r->left);
        delete_nodes(r->right);
        delete r;
    }
    
public:
    // destructor
    ~bst() {
        delete_nodes(root);
    }
    
private:
    
    // ************ Helper functions**********************************************************************
    
    // This function returns the maximum number of nodes
    static int maxNumberOfNodes (bst_node *root) {
        int numLeft = root->numLeftSubtrees;
        int numRight = root->numRightSubtrees;
        if(numLeft > numRight) {
            return numLeft;
        }
        else {
            return numRight;
        }
    } // end of  maxNumberOfNodes
    
    
    // This function returns the minimum number of nodes
    static int minNumberOfNodes (bst_node *root) {
        int numLeft = root->numLeftSubtrees;
        int numRight = root->numRightSubtrees;
        if(numLeft < numRight) {
            return numLeft;
        }
        else {
            return numRight;
        }
       
    } // end of minNumberOfNodes ()-----
    

    // function to populate the vector;it is a helper function for to-vec function
    void populateVector (std::vector<T> *(&thevector), bst_node*r) {
        
        // if r is null, the tree is empty
        if (r == nullptr) {
            return;
        }
        
        // traversing in order
        populateVector (thevector, r->left);
        thevector->push_back(r->val);
        populateVector(thevector, r->right);
    } // end of  populateVector
    
    
    // This function balances the vector;it is a helper function for _insert and _remove functions
    static void vectorBalance( std::vector<T> (&vector), bst_node*r) {
        
        // if r is null that means the tree is empty
        if (r == nullptr) {
            return;
        }
        
        else {
            // perform  in order traversal
            vectorBalance(vector, r->left);
            vector.push_back(r->val);
            vectorBalance(vector, r->right);
        }
    } // end of vectorBalance().....
    
    
public:
    
    /**
     * function:  insert()
     * desc:      recursive helper function inserting x into
     *            binary search tree rooted  at r.
     *
     * returns:   pointer to root of tree after insertion.
     *
     * notes:     if x is already in tree, no modifications are made.
     */
    static bst_node * _insert(bst_node *r, T & x, bool &success){
        if(r == nullptr){
            success = true;
            return new bst_node(x, nullptr, nullptr, 0, 0);
        }
        // x is equal to the root val
        if(r->val == x){
            success = false;
            return r;
        }
        
        // if x is less than the root val  must go to the left subtree
        if(x < r->val){
            r->left = _insert(r->left, x, success);
            if(success) {
                r->numLeftSubtrees++;  // increment the number of nodes in the left subtree
            }
            
            // return r;
        }
        
        // if x is greater than the root val must go to the right subtree
        else {
            r->right = _insert(r->right, x, success);
            if(success){
                r->numRightSubtrees++;   // increment the number of nodes in the Right subtree
            }
            //return r;
        }
        
        int max,  min;
        max = maxNumberOfNodes(r);   // getting the max number of nodes in subtrees
        min =  minNumberOfNodes (r);  // getting the min number of nodes in a subtrees
        
        // checking if the tree needs to be rebalanced
        if (max > (2 * min) + 1) {
            std::vector<T> tempVector;   // temporary vector
            vectorBalance(tempVector, r);  // populating a temporary vector elements/nodes in the subtree in sorted order
            
             // delete the old tree  and set it to null
            delete_nodes(r);
             r = nullptr;
            
            // rebuild the tree
            r = _from_vec(tempVector, 0,tempVector.size() - 1);  // since the length- 1, we minus 1
            
        }
        return r;
    } // end of _insert()..............
    
    
public:
    /**
     * function:  insert
     * desc:      inserts x into BST given by t.  Note that
     *            a BST stores a SET -- no duplicates.  Thus,
     *            if x is already in t when call made, no
     *            modifications to tree result.
     *
     * note:      helper function does most of the work.
     *
     */
    bool insert(T & x){
        bool success;
        root = _insert(root, x, success);
        return success;
    }
    
    /**
     * function:  contains()
     * desc:      returns true or false depending on whether x is an
     *            element of BST (calling object)
     *
     */
    bool contains(const T & x){
        bst_node *p = root;
        
        while(p != nullptr){
            
            if(p->val == x)
                return true;
            if(x < p->val){
                p = p->left;
            }
            else
                p = p->right;
        }
        return false;
    }
    
    
private:
    // returns pointer to node containing
    //   smallest value in tree rooted at r
    static bst_node * _min_node(bst_node *r ){
        if(r==nullptr)
            return nullptr; // should never happen!
        while(r->left != nullptr)
            r = r->left;
        return r;
    }
    
    // returns pointer to node containing
    //   smallest value in tree rooted at r
    static bst_node * _max_node(bst_node *r ){
        if(r==nullptr)
            return nullptr; // should never happen!
        while(r->right != nullptr)
            r = r->right;
        return r;
    }
    
    
public:
    // recursive helper function for node removal
    //   returns root of resulting tree after removal.
    static bst_node * _remove(bst_node *r, T & x, bool &success){
        bst_node *tmp;
        bool sanity;
        
        if(r==nullptr){
            success = false;
            return nullptr;
        }
        
        if(r->val == x){
            success = true;
            
            if(r->left == nullptr){
                tmp = r->right;
                delete r;
                return tmp;
            }
            
            if(r->right == nullptr){
                tmp = r->left;
                delete r;
                return tmp;
            }
            
            // if we get here, r has two children
            r->val = _min_node(r->right)->val;
            r->right = _remove(r->right, r->val, sanity);
            if(sanity){
                r->numRightSubtrees--;  // decrement the number of nodes in a right subtree
            }
            
            if(!sanity)
                std::cerr << "ERROR:  remove() failed to delete promoted value?\n";
            return r;
        }
        
        if(x < r->val){
            r->left = _remove(r->left, x, success);
            if(success){
                r->numLeftSubtrees--;
            }
        }
        
        else {
            r->right = _remove(r->right, x, success);
            if(success){
                r->numRightSubtrees--;
            }
        }
        
        int max, min;
        max = maxNumberOfNodes(r);   // getting the max number of nodes in subtrees
        min =  minNumberOfNodes(r);  // getting the min number of nodes in a subtrees
        
        // checking if the tree needs to be rebalanced
        if  (max > (2 * min) + 1) {
            std::vector<T> tempVector;   // temporary vector
            vectorBalance(tempVector, r);  // populating a temporary vector elements/nodes in the subtree in sorted order
            
            // delete the old tree  and set it to null
             delete_nodes(r);
              r = nullptr;
            
            // rebuild the tree
            r = _from_vec(tempVector, 0,tempVector.size() - 1);  // since the length- 1, we minus 1
            
            
        }
        return r;
        
    } // end of _remove().........
    
    
public:
    
    bool remove(T & x){
        bool success;
        root = _remove(root, x, success);
        return success;
    }
    
    
private:
    // recursive helper function to compute size of
    //   tree rooted at r
    static int _size(bst_node *r){
        if(r==nullptr) return 0;
        return _size(r->left) + _size(r->right) + 1;
    }
    
public:
    int size() {
        return _size(root);
    }
    
private:
    
    static int _height(bst_node *r){
        int l_h, r_h;
        
        if(r==nullptr) return -1;
        l_h = _height(r->left);
        r_h = _height(r->right);
        return 1 + (l_h > r_h ? l_h : r_h);
    }
    
public:
    
    int height() {      return _height(root);
    }
    
    bool min(T & answer) {
        if(root == nullptr){
            return false;
        }
        answer = _min_node(root)->val;
        return true;
    }
    
    T max() {
        return _max_node(root)->val;
    }
    
    /******************************************
     *
     * "stubs" for assigned TODO functions below
     *
     *****************************************/
    
    /* TODO
     * Function:  to_vector
     * Description:  allocates a vector of type T and populates
     *               it with the elements of the tree in sorted
     *               order.  A pointer to the vector is returned.
     *
     * runtime:  O(n) where n is the tree size.
     *
     */
    std::vector<T> * to_vector() {
        // create new vector to return
        std::vector<T> *populated_vector = new std::vector <T>;
        
        // populate the vector
        populateVector (populated_vector, root);
        
        return populated_vector;
    }
    
    
    /* TODO
     * Function:  get_ith
     * Description:  determines the ith smallest element in t and
     *    "passes it back" to the caller via the reference parameter x.
     *    i ranges from 1..n where n is the number of elements in the
     *    tree.
     *
     *    If i is outside this range, false is returned.
     *    Otherwise, true is returned (indicating "success").
     *
     * Runtime:  O(h) where h is the tree height
     */
private:
    // This function determines if the ith smallest element exists
    bst_node *get_ith_helper(bst_node* r, int y ) {
        if(r == nullptr){
            return nullptr;
        }
        
        if (y == r->numLeftSubtrees+1){
            return r;
        }
        
        else if(y < r->numLeftSubtrees+1 ){
            return get_ith_helper(r->left,y);
        }
        
        else  {
            return  get_ith_helper(r->right,(y-(r->numLeftSubtrees+1)));
        }
    }  // get_ith_helper()------
    
public:
    
    bool get_ith(int i, T &x) {
        bst_node* newNode = get_ith_helper(root, i);
        int rSize = root->numLeftSubtrees + root->numRightSubtrees + 1;
        
        if(newNode == nullptr){
            return false;
        }
        else if((i > rSize) || (i<1)){
            return false;
        }
        
        else {
            x = newNode->val;
            return true;
        }
    } // end of get_ith function....
    
    
    
    
    /*
     * get_ith_SLOW:  slow version of get_ith which gives the right answer, but
     *   does so by "walking the entire tree".
     * might be helpful for debugging?
     * Private helper below does most of the work
     */
    /*bool get_ith_SLOW(int i, T &x) {
     int n = size();
     int sofar=0;
     
     if(i < 1 || i > n)
     return false;
     
     _get_ith_SLOW(root, i, x, sofar);
     return true;
     }
     
     private:
     // recursive helper function that does most of the work
     static void _get_ith_SLOW(bst_node *t, int i, T &x, int &sofar) {
     if(t==nullptr)
     return;
     _get_ith_SLOW(t->left, i, x, sofar);
     
     if(sofar==i)
     return;
     sofar++;
     if(sofar==i) {
     x = t->val;
     return;
     }
     _get_ith_SLOW(t->right, i, x, sofar);
     }*/
    
public:
    
    /* TODO
     * Function: position_of
     * Description:  this is like the inverse of
     *       get_ith:  given a value x, determine the
     *       position ("i") where x would appear in a
     *       sorted list of the elements and return
     *       the position as an integer.
     *       If x is not in the tree, -1 is returned.
     *       Examples:
     *           if x happens to be the minimum, 1 is returned
     *           if x happens to be the maximum, n is returned where
     *               n is the tree size.
     *
     *       Notice the following property:  for a bst t with n nodes,
     *           pick an integer pos:1 <= pos <= n.
     *           Now consider the following:
     *
     T x;
     int pos, pos2;
     // set pos to a value in {1..n}
     t.get_ith(pos, x);  // must return true since pos is in {1..n}
     
     // now let's find the position of x (just retrieved)
     pos2 = t.position_of(x);
     if(pos != pos2) {
     std::cout << "THERE MUST BE A BUG!\n";
     }
     
     See how position_of performs the inverse operation of get_ith?
     *
     * Return:  -1 if x is not in the tree; otherwise, returns the position where x
     *          would appear in the sorted sequence of the elements of the tree (a
     *          value in {1..n}
     *
     * Runtime:  O(h) where h is the tree height
     */
    int position_of(const T & x) {
        
        if(root == nullptr) {
            return -1;
        }
        else {
            return  position_of_helper (root, (root->numLeftSubtrees +1), x); // call  helper function
        }
    } // end of position_of().....
        
private:
    // position-of () helper function
    int position_of_helper (bst_node * t, const T & y, int z)
    {
        if(y == t->val) {
            return z; // index is returned
        }
        
        if(t->left != nullptr) {
            if(t->val > y) {
                z = z - (t->left->numRightSubtrees + 1);  // update t->left index
                return position_of_helper(t->left, y, z);
            }
        }
        
        if (t->right != nullptr) {
            if(y > t->val) {
                z = z + (t->right->numLeftSubtrees + 1);  // update t->right index
                return position_of_helper(t->right, y, z);
            }
        }
        else {
            return -1; // means y is not in the tree
        }
    }   // end position_of_helper ()......

    
    
    /* TODO
     * Function:  num_geq
     * Description:  returns the number of elements in tree which are
     *       greater than or equal to x.
     *
     * Runtime:  O(h) where h is the tree height
     */
    
public:
    int num_geq(const T & x) {
        int total = 0;
        return helperOfNum_geq (root, x, total); // helper function
        
    } // end num_geq().....
    
    
private:
    // helper function for num_geq function
    int helperOfNum_geq (bst_node *t, const T &x, int &total) {
        
        // if the tree is empty 0 is returned
        if (t == nullptr) {
            return 0;
        }
        
        // if x is less or equal to root value , then calculate the total of all the right subtree
        if (x <= t->val) {
            total = total + t->numRightSubtrees + 1;
            helperOfNum_geq(t->left, x, total);
        }
        
        // if x is greater than  root value go to the right
        if (x > t->val) {
            helperOfNum_geq(t->right, x, total);
        }
        return total;
    }
    
    /*
     * function:     num_geq_SLOW
     * description:  same functionality as num_geq but sloooow (linear time)
     *                (private helper does most of the work)
     *
     */
    int num_geq_SLOW(const T & x) {
        return _num_geq_SLOW(root, x);
    }
    
private:
    static int _num_geq_SLOW(bst_node * t, const T & x) {
        int total;
        
        if(t==nullptr) return 0;
        total =_num_geq_SLOW(t->left, x) + _num_geq_SLOW(t->right, x);
        
        if(t->val >= x)
            total++;
        return total;
    }
    
public:
    
    /* TODO
     * Function:  num_leq
     * Description:  returns the number of elements in tree which are less
     *      than or equal to x.
     *
     * Runtime:  O(h) where h is the tree height
     *
     **/
    int num_leq(const T &x) {
        int total = 0;
        return  helperOfNum_leq(root, x, total);  // helper function
    }
    
    
private:
    
    // helper function for num_leg function
    int helperOfNum_leq (bst_node *t, const T &x, int &total) {
        
        // if the tree is empty 0 is returned
        if (t == nullptr) {
            return 0;
        }
        
        // if x is less or equal to root value , then calculate the total of all the left subtree
        if (x >= t->val) {
            total = total + t->numLeftSubtrees + 1;
            helperOfNum_leq(t->right, x, total);
        }
        
        // if x is less than  root value go to the left
        if (x < t->val) {
            helperOfNum_leq(t->left, x, total);
        }
        return total;
    }
    
   
    /*
     * function:     num_leq_SLOW
     * description:  same functionality as num_leq but sloooow (linear time)
     *               (private helper _num_leq_SLOW does most of the work)
     */
    int num_leq_SLOW(const T & x) {
        return _num_leq_SLOW(root, x);
    }
private:
    
    // helper for num_leq_SLOW
    static int _num_leq_SLOW(bst_node *t, const T &x) {
        int total;
        
        if(t==nullptr) return 0;
        total =_num_leq_SLOW(t->left, x) + _num_leq_SLOW(t->right, x);
        
        if(t->val <= x)
            total++;
        return total;
    }
    
    
public:
    
    /* TODO
     * Function:  num_range
     * Description:  returns the number of elements in tree which are
     *       between min and max (inclusive).
     *
     * Runtime:  O(h) where h is the tree height
     *
     **/
    int num_range(const T & min, const T & max) {
        int  valueMax = num_leq(max);  // set to the maximun value returned from num-leg
        int  valueMin = num_leq(min);   // set to the minmun value returned from num_leg
        int  difference = valueMax - valueMin;  // find the difference
        
        if(contains(min)) {
            difference++;
        }
        return difference;
        
    } // end of num_range
    
private:
    
    int num_range_SLOW(const T & min, const T & max) {
        return _num_range_SLOW(root, min, max);
    }
    
private:
    static int _num_range_SLOW(bst_node *t, const T &min, const T &max) {
        int total;
        
        if(t==nullptr) return 0;
        total =_num_range_SLOW(t->left, min, max) +
        _num_range_SLOW(t->right, min, max);
        
        if(t->val >= min && t->val <= max)
            total++;
        return total;
    }
    
public:
    
    /*
     * TODO
     * function:     extract_range
     * Description:  allocates a vector of element type T
     *               and populates it with the tree elements
     *               between min and max (inclusive) in order.
     *               A pointer to the allocated and populated
     *               is returned.
     *
     * notes/comments:  even if the specified range is empty, a
     *                  vector is still allocated and returned;
     *                  that vector just happens to be empty.
     *                  (The function NEVER returns nullptr).
     *
     * runtime:  the runtime requirement is "output dependent".
     *           Let k be the number of elements in the specified range
     *           (and so the length of the resulting vector) and let h
     *           be the height of the tree.  The runtime must be:
     *
     *                  O(h + k)
     *
     *           So...while k can be as large as n, it can be as small
     *           as zero.
     *
     */
    
   
    
    std::vector<T> * extract_range(const T & min, const T & max) {
       
        std::vector<T> * theVector = new std::vector<T>;
        extract_rangeHelper(root, theVector,max, min);   // call helper function
        return theVector;
    } // end of vector ().............
    
    
private:
    // Helper function for extra_range
    void extract_rangeHelper(bst_node * rNode, std::vector<T> *theVector, const T &max, const T &min)
    {
        //if the rNode is empty return
        if (rNode == nullptr) {
            return;
        }
        
       // check if root value is greater than the min value and if it is, go to the left
        if (rNode->val > min) {
            extract_rangeHelper(rNode->left, theVector, max, min); // recursion
        }
        
        /* check if root value is greater or equal to the  min value and less or equal to max value
        if the condition is true push the root value to the vector*/
        if ((rNode->val >= min) && (rNode->val <= max)) {
            (*theVector).push_back(rNode->val);
        }
        
        // check if root value is less than the max value and if it is, go to the right
        if (rNode->val < max){
            extract_rangeHelper(rNode->right,theVector, max, min);
        }
    }   // end of extract_rangeHelper().........
    
    
    /***************************************************
     *  PREORDER AND INORDER TREE TRAVERSALS BELOW      *
     *                                                  *
     ***************************************************/
    
private:
    static void indent(int m){
        int i;
        for(i=0; i<m; i++)
            std::cout << "-";
    }
    
    static void _inorder(bst_node *r){
        if(r==nullptr) return;
        _inorder(r->left);
        std::cout << "[ " << r->val << " ]\n";
        _inorder(r->right);
    }
    
    static void _preorder(bst_node *r, int margin){
        if(r==nullptr) {
            indent(margin);
            std::cout << " nullptr \n";
        }
        else {
            indent(margin);
            std::cout << "[ " << r->val << " ]\n";
            _preorder(r->left, margin+3);
            _preorder(r->right, margin+3);
        }
    }
    
public:
    void inorder() {
        std::cout << "\n======== BEGIN INORDER ============\n";
        _inorder(root);
        std::cout << "\n========  END INORDER  ============\n";
    }
    
    
    void preorder() {
        
        std::cout << "\n======== BEGIN PREORDER ============\n";
        _preorder(root, 0);
        std::cout << "\n========  END PREORDER  ============\n";
        
    }
    
    /***************************************************
     *  END PREORDER AND INORDER TREE TRAVERSALS        *
     *                                                  *
     ***************************************************/
    
private:
    /*
     * Recursive  helper function _from_vec, used by
     * from_sorted_arr(...). The function must return a sub-tree that is
     * perfectly balanced, given a sorted array of elements a.
     */
    static bst_node * _from_vec(const std::vector<T> &a, int low, int hi){
        int m;
        bst_node *root;
        
        if(hi < low) return nullptr;
        m = (low+hi)/2;
        root = new bst_node(a[m]);
        root->numLeftSubtrees = m - low;
        
        root->left  = _from_vec(a, low, m-1);
        root->numRightSubtrees = hi - m;
        root->right = _from_vec(a, m+1, hi);
        return root;
        
    }
    
public:
    static bst * from_sorted_vec(const std::vector<T> &a, int n){
        
        bst * t = new bst();
        t->root = _from_vec(a, 0, n-1);
        return t;
    }
    
private:
    
    /*
     * DO NOT MODIFY THE TWO FUNCTIONS BELOW!!!
     */
    static void _to_preorder_vec(bst_node *t, std::vector<T> &vec) {
        if(t==nullptr) return;
        vec.push_back(t->val);
        _to_preorder_vec(t->left, vec);
        _to_preorder_vec(t->right, vec);
    }
    std::vector<T> * to_preorder_vec() {
        std::vector<T> *vec = new std::vector<T>();
        _to_preorder_vec(root, *vec);
        return vec;
    }
    
    /*
     * Finally, the data members of the bst class
     */
private:
    bst_node *root;
    
    
}; // end class bst

#endif
