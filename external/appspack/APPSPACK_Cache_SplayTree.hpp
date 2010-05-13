// $Id: APPSPACK_Cache_SplayTree.hpp,v 1.5 2003/11/26 16:27:11 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Cache_SplayTree.hpp,v $ 

//@HEADER
// ************************************************************************
// 
//          APPSPACK: Asynchronous Parallel Pattern Search
//                 Copyright (2003) Sandia Corporation
// 
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
// 
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//                                                                                 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.                                                                           .
// 
// Questions? Contact Tammy Kolda (tgkolda@sandia.gov) 
// 
// ************************************************************************
//@HEADER

/*! \file APPSPACK_Cache_SplayTree.hpp 
    \brief Template definitions for APPSPACK::Cache::SplayTreeNode and
    APPSPACK::Cache::SplayTree.
*/

#ifndef APPSPACK_CACHE_SPLAYTREE_HPP
#define APPSPACK_CACHE_SPLAYTREE_HPP

namespace APPSPACK {

namespace Cache
{

// Forward declaration 
template <class Comparable> class SplayTree;
}
}

namespace APPSPACK {
namespace Cache {

/*! 
  \brief Templated splay tree node.

  See SplayTree for more details.

  \author H. Alton Patrick, Summer 2000. 
*/

template <class Comparable> 
class SplayTreeNode
{

private:

  //! Default Constructor
  SplayTreeNode() : left(NULL), right(NULL) {};
  /*! \brief Constructor with a comparable element and the
    specification of the left and right sub-trees. */
  SplayTreeNode(const Comparable& e, SplayTreeNode* l = NULL, SplayTreeNode* r = NULL)
    : element(e), left(l), right(r) {};

  //! Destructor
  ~SplayTreeNode() {};

  //! The comparable element
  Comparable element;
  //! Left subtree
  SplayTreeNode* left;
  //! Right subtree
  SplayTreeNode* right;

  //! Friend
  friend class SplayTree<Comparable>;
};

/*! 
  \brief The splay tree is a binary storage structure. 

  A templated splay tree. The template takes is based on a
  <em>Comparable</em> class that must support the following five
  operations.

  <ul>
  <li> Empty Constructor

       <tt>Comparable();</tt>

       Called indirectly by #splay, which creates a SplayTreeNode that
       doesn't actually have an element in it. 

  <li> Copy Constructor

       <tt>Comparable(const Comparable& source);</tt>

       The copy constructor is used only when #insert is called.

  <li> Copy Data

       <tt> void copyData(const Comparable& source);</tt>

       The copy data is used only when #find is called and a match is
       found in the splay tree. It is used to copy the relevant data
       from the matching entry to the input to #find.
      
  <li> Not Equal

       <tt> bool operator!=(const Comparable& source);</tt>

       This is used only when #find is called.

  <li> Less Than 

       <tt> bool operator<(const Comparable& source);</tt>

       This is used by #insert and repeatedly by #splay.

  <li> Greater Than

       <tt> bool operator>(const Comparable& source);</tt>

       This is used by #insert and repeatedly by #splay.

  </ul>

  \author H. Alton Patrick, Summer 2000. 
  \author Tammy Kolda
*/

template <class Comparable> 
class SplayTree
{
public:

  //! Construct an empty splay tree.
  SplayTree();
  //! Destruct the splay tree.
  ~SplayTree();
  /*! \brief Return true if x is in the splay tree. Furthermore, call
    x.copyData() with the splay tree's matching entry (which may not
    be exactly the same) */
  bool find(Comparable& x);
  /*! \brief Insert x into the splay tree if it is not already there. 
    \retval Returns true if x is inserted into the tree, false otherwise.
  */
  bool insert(const Comparable& x);

private:

  /*! 
    \brief Re-organize the splay tree.

    If x is in the tree rooted at r, moves x to the root.  If x is not
    in the tree rooted at r, then the node placed at the root is the one
    which would have been reached directly before x in a normal binary
    search.
    
    This top-down splay is based on D. Sleator's implementation, which
    can be found at
    ftp://ftp.cs.cmu.edu/user/sleator/splaying/top-down-splay.c.
  */
  void splay(const Comparable& x, SplayTreeNode<Comparable>*& r);

  //! Return true if the splay tree is empty.
  bool isEmpty();

  //! Root node of the splay tree.
  SplayTreeNode<Comparable>* root;


};

template <class Comparable> 
SplayTree<Comparable>::SplayTree()
{
  root = NULL;
}

template <class Comparable> 
SplayTree<Comparable>::~SplayTree()
{
  // Keep deleting the root node until everything is gone.
  SplayTreeNode<Comparable>* newroot;

  while(!isEmpty()){
    if(root->left == NULL)
      newroot = root->right;
    else{
      // This trick for joining the left and right subtrees works because 
      // splaying on an element larger than any other in the subtree (as
      // splay(root->element, root->left) does) moves the largest element
      // in the subtree to the root of the subtree.
      newroot = root->left;
      splay(root->element, newroot);
      newroot->right = root->right;
    }
    delete root;
    root = newroot;
  }
}

template <class Comparable> 
bool SplayTree<Comparable>::find(Comparable& x)
{
  // Find x in the tree.  If it is present, replace x with the matching
  // node in the tree and return true.  Otherwise, return false.

  if(isEmpty())
    return false;

  splay(x, root);
  // If x is in the tree, it will be at the root.
  if(x != root->element)
    return false;
  x.copyData(root->element);
  return true;
}

template <class Comparable> 
bool SplayTree<Comparable>::insert(const Comparable& x)
{
  if (isEmpty())
  {
    root = new SplayTreeNode<Comparable>(x);
    return true;
  }

  splay(x, root);
  if (x < root->element)
  {
    SplayTreeNode<Comparable>* newNode = new SplayTreeNode<Comparable>(x);
    newNode->left = root->left;
    newNode->right = root;
    root->left = NULL;    
    root = newNode;
    return true;
  }
  else if (x > root->element)
  {
    SplayTreeNode<Comparable>* newNode = new SplayTreeNode<Comparable>(x);
    newNode->right = root->right;
    newNode->left = root;
    root->right = NULL;
    root = newNode;
    return true;
  }

  // else x already in tree, do nothing
  return false;
}  

template <class Comparable> 
void SplayTree<Comparable>::splay(const Comparable& x, SplayTreeNode<Comparable>*& r)
{
  // header's left and right branches hold, respectively, the *right* and
  // *left* subtrees of the reorganized tree.
  SplayTreeNode<Comparable> header;

  // SplayTreeNode being inspected in the main tree.
  SplayTreeNode<Comparable> *current = r;

  // SplayTreeNodes being used in the left and right subtrees.
  SplayTreeNode<Comparable> *leftcur, *rightcur;  
  SplayTreeNode<Comparable> *temp;

  if (r == NULL)
    return;

  leftcur = rightcur = &header;

  for (;;){
    if (x < current->element){
      if (current->left == NULL)
        break;
      if (x < current->left->element){
        // Rotate current with its left child.
        temp = current->left;
        current->left = temp->right;
        temp->right = current;
        current = temp;
        if (current->left == NULL)
          break;
      }
      // Place current in the tree of elements greater than x.
      rightcur->left = current;
      rightcur = current;
      current = current->left;
    }
    else if (x > current->element){
      if (current->right ==  NULL)
        break;
      if (x > current->right->element){
        // Rotate current with its right child.
        temp = current->right;
        current->right = temp->left;
        temp->left = current;
        current = temp;
        if (current->right == NULL)
          break;
      }
      // Place current in the tree of elements less than x.
        // Rotate current with its right child.
      leftcur->right = current;
      leftcur = current;
      current = current->right;
    }
    else
      break;
  }

  // Assemble the tree.
  leftcur->right = current->left;
  rightcur->left = current->right;
  current->left = header.right;
  current->right = header.left;
  r = current;
  
}

template <class Comparable> 
bool SplayTree<Comparable>::isEmpty()
{
  return (root == NULL);
}

}
}

#endif
