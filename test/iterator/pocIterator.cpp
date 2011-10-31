// //////////////////////////////////////////////////////////////////////////
// Proof-of-concept for STL iterators on Business Object Model (BOM) objects
// //////////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>

/** Base class. */
class BaseClass {
public:
  /** Constructor. */
  BaseClass (const std::string& iName) : _name (iName) {}
  /** Destructor. */
  ~BaseClass () {}
  /** Get the serialised version of the Object. */
  virtual void toStream (std::ostream& ioOut) const = 0;
protected:
  /** Name. */
  std::string _name;
};

/** Standard display function. */
template <class charT, class traits>
inline
std::basic_ostream<charT, traits>&
operator<< (std::basic_ostream<charT, traits>& ioOut,
            const BaseClass& iBaseClass) {
  std::basic_ostringstream<charT,traits> ostr;
  ostr.copyfmt (ioOut);
  ostr.width (0);
  // Fill string stream
  iBaseClass.toStream (ostr);
  // Print string stream
  ioOut << ostr.str();
  return ioOut;
}

/** Child class. */
class Child : public BaseClass {
public:
  /** Constructor. */
  Child (const std::string& iName) : BaseClass (iName) {}
  /** Destructor. */
  ~Child () {}
  /** Get the serialised version of the Object. */
  void toStream (std::ostream& ioOut) const { ioOut << "Child: " << _name; }
};

/** List of pointers on children objects. */
typedef std::vector<Child*> ChildList_T;

/** Parent class. */
class Parent : public BaseClass {
public:
  /** STL iterators on the list of (pointers on) children objects. */
  typedef ChildList_T::const_iterator const_iterator;
  typedef ChildList_T::iterator iterator;
  typedef ChildList_T::reverse_iterator reverse_iterator;
  typedef ChildList_T::const_reverse_iterator const_reverse_iterator;

  /** Constructor. */
  Parent (const std::string& iName) : BaseClass (iName) {}
  /** Destructor. */
  ~Parent () {}

  /** Get the serialised version of the Object. */
  void toStream (std::ostream& ioOut) const { ioOut << "Parent: " << _name; }

  /** Add a child in the dedicated list. */
  void push_back (Child& ioChild) { _childList.push_back (&ioChild); }
  
  /** Return the iterator instantiated on the first element of the
      list of children objects. */
  const_iterator begin() const { return _childList.begin(); }

  /** Return the iterator instantiated beyond the last element of the
      list of children objects. */
  const_iterator end() const { return _childList.end(); }
  
  /** Return the iterator instantiated on the last element of the
      list of children objects. */
  const_reverse_iterator rbegin() const { return _childList.rbegin(); }

  /** Return the iterator instantiated beyond the first element of the
      list of children objects. */
  const_reverse_iterator rend() const { return _childList.rend(); }
  
private:
  /** List of pointers on children objects. */
  ChildList_T _childList;
};

// ///////////// M A I N /////////////
int main (int argc, char* argv[]) {

  // Initialisation
  Parent* lParent_ptr = new Parent ("parent");

  Child* lChild1_ptr = new Child ("child1");
  lParent_ptr->push_back (*lChild1_ptr);
  
  Child* lChild2_ptr = new Child ("child2");
  lParent_ptr->push_back (*lChild2_ptr);

  // ///////////// Usage (as a proof of concept) /////////////
  //
  // Ascending order
  std::cout << *lParent_ptr << " in the ascending order:" << std::endl;
  unsigned short idx = 1;
  for (Parent::const_iterator itChild = lParent_ptr->begin();
       itChild != lParent_ptr->end(); ++itChild, ++idx) {
    if (idx != 1) {
      std::cout << "; ";
    }
    
    const Child* lChild_ptr = *itChild;
    assert (lChild_ptr != NULL);
    
    std::cout << *lChild_ptr;
  }
  std::cout << std::endl;
  
  //
  // Descending order
  std::cout << *lParent_ptr << " in the descending order:" << std::endl;
  idx = 1;
  for (Parent::const_reverse_iterator itChild = lParent_ptr->rbegin();
       itChild != lParent_ptr->rend(); ++itChild, ++idx) {
    if (idx != 1) {
      std::cout << "; ";
    }
    
    const Child* lChild_ptr = *itChild;
    assert (lChild_ptr != NULL);
    
    std::cout << *lChild_ptr;
  }
  std::cout << std::endl;
  
  return 0;
}
