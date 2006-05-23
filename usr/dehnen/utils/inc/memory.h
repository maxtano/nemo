// -*- C++ -*-                                                                  
////////////////////////////////////////////////////////////////////////////////
///                                                                             
/// \file    inc/memory.h                                                       
///                                                                             
/// \author  Walter Dehnen                                                      
///                                                                             
/// \date    2000-2006                                                          
///                                                                             
////////////////////////////////////////////////////////////////////////////////
//                                                                              
// Copyright (C) 2000-2006  Walter Dehnen                                       
//                                                                              
// This program is free software; you can redistribute it and/or modify         
// it under the terms of the GNU General Public License as published by         
// the Free Software Foundation; either version 2 of the License, or (at        
// your option) any later version.                                              
//                                                                              
// This program is distributed in the hope that it will be useful, but          
// WITHOUT ANY WARRANTY; without even the implied warranty of                   
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU            
// General Public License for more details.                                     
//                                                                              
// You should have received a copy of the GNU General Public License            
// along with this program; if not, write to the Free Software                  
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                    
//                                                                              
////////////////////////////////////////////////////////////////////////////////
#ifndef WDutils_included_memory_h
#define WDutils_included_memory_h

#ifndef WDutils_included_cstddef
#  include <cstddef>
#  define WDutils_included_cstddef
#endif
#ifndef WDutils_included_exception_h
#  include <exception.h>
#endif
#ifndef WDutils_included_inline_h
#  include <inline.h>
#endif

namespace WDutils {
  // ///////////////////////////////////////////////////////////////////////////
  //                                                                            
  /// Array allocation giving useful info in case of allocation error; mostly   
  /// used from macro WDutils_NEW.                                              
  ///                                                                           
  /// In case of allocation error we abort or throw an exception (depending on  
  /// the WDutils error handling settings).  If the debugging level exceeds 10, 
  /// we always print debugging information about memory allocation.            
  ///                                                                           
  /// \return  a valid pointer (unless an error occurs)                         
  /// \param T (template parameter) the type of array elements                  
  /// \param n number of array elements                                         
  /// \param f name of the source file where this routines is called            
  /// \param l number of the line in that file                                  
  template<typename T> inline
  T* NewArray(size_t n, const char*f, int l, const char*lib = "WDutils")
    WDutils_THROWING {
    if(RunInfo::debug(10)) {
      std::cerr<<"### "<< lib <<" Debug Info: "<<f<<':'<<l 
	       <<" allocating "<<n <<' ' 
	       << (n>1? traits<T>::names():traits<T>::name()) <<" = "
	       << n*sizeof(T)
	       << (n*sizeof(T)>1? " bytes ... " : "byte ... ") ;
      T* t;
      try {
	t = new T[n];
      } catch(std::bad_alloc E) {
	std::cerr<<'\n';
	WDutils_THROW("[%s:%d]: caught std::bad_alloc\n",f,l);
      }
      std::cerr<< "@ " << static_cast<void*>(t) << '\n';
      return t;
    }
    try {
      return new T[n];
    } catch(std::bad_alloc E) {
      WDutils_THROW("[%s:%d]: caught std::bad_alloc\n",f,l);
    }
    return 0;
  }
  // ///////////////////////////////////////////////////////////////////////////
  //                                                                            
  /// C MACRO to be used for array allocation                                   
  ///                                                                           
  /// Calling WDutils::NewArray<TYPE>(), which in case of an error generates an 
  /// error message detailing the source file and line of the call. In case     
  /// the debugging level exceeds 10, we always print debugging information     
  /// about memory allocation.                                                  
  ///                                                                           
  /// \param  TYPE name of the element type                                     
  /// \param  SIZE number of elements                                           
#define WDutils_NEW(TYPE,SIZE) WDutils::NewArray<TYPE>(SIZE,__FILE__,__LINE__)

  // ///////////////////////////////////////////////////////////////////////////
  //                                                                            
  /// Array de-allocation giving useful info in case of error; mostly used from 
  /// macro WDutils_DEL_A.                                                      
  ///                                                                           
  /// In case of a de-allocation error (if the pointer provided was not valid)  
  /// an error is generated (or an exception thrown, depending on the WDutils   
  /// error settings). If the debugging level exceeds 10, we always print       
  /// debugging information about memory de-allocation.                         
  ///                                                                           
  /// \param T  (template parameter) the type of array elements                 
  /// \param a  pointer previously allocated with WDutils::NewArray<>()         
  ///           or ::operator new[].                                            
  /// \param f  name of the source file where this routines is called           
  /// \param l  number of the line in that file                                 
  template<typename T> inline
  void DelArray(T* a, const char*f, int l, const char*lib = "WDutils")
    WDutils_THROWING {
    if(0==a) {
      warning("[%s:%d]: trying to delete zero pointer to array of '%s'",
	      f,l,traits<T>::names());
      return;
    }
    if(RunInfo::debug(10)) {
      std::cerr<<"### "<< lib << " Debug Info: "<<f<<':'<<l
	       <<" de-allocating array of " << traits<T>::names()
	       <<" @ "<<static_cast<void*>(a) << std::endl;
      try {
	delete[] a;
      } catch(...) {
	std::cerr<<'\n';
	WDutils_THROW("[%s:%d]: deleting array of '%s' @ %p' failed\n",
		      f,l,traits<T>::names(),a);
      }
    } else {
      try {
	delete[] a;
      } catch(...) {
	WDutils_THROW("[%s:%d]: deleting array of '%s' @ %p' failed\n",
		      f,l,traits<T>::names(),a);
      }
    }
  }
  // ///////////////////////////////////////////////////////////////////////////
  //                                                                            
  /// C MACRO to be used for array de-allocation                                
  ///                                                                           
  /// Calling WDutilsN::DelArray<TYPE>(), which in case of an error generates an
  /// error message detailing the source file and line of the call. In case     
  /// the debugging level exceeds 10, we always print debugging information     
  /// about memory de-allocation.                                               
  ///                                                                           
  /// \param P  pointer to be de-allocated                                      
#define WDutils_DEL_A(P) WDutils::DelArray(P,__FILE__,__LINE__)
  // ///////////////////////////////////////////////////////////////////////////
  ///                                                                           
  /// Object de-allocation giving useful info in case of error; mostly used     
  /// from macro WDutils_DEL_O.                                                 
  ///                                                                           
  /// In case of a de-allocation error (if the pointer provided was not valid)  
  /// an error is generated (or an exception thrown, depending on the WDutils   
  /// error settings).                                                          
  ///                                                                           
  /// \param T  (template parameter) the type of array elements                 
  /// \param a  pointer previously allocated with ::operator new().             
  /// \param f  name of the source file where this routines is called           
  /// \param l  number of the line in that file                                 
  template<typename T> inline
  void DelObject(T* a, const char*f, int l, const char*lib="WDutils")
    WDutils_THROWING {
    if(0==a) {
      warning("[%s:%d]: trying to delete zero pointer to object '%s'",
	      f,l,traits<T>::name());
      return;
    }
    if(RunInfo::debug(10)) {
      std::cerr<<"### "<< lib << " Debug Info: "<<f<<':'<<l
	       <<" de-allocating " << traits<T>::name()
	       <<" object @ "<<static_cast<void*>(a)<< std::endl;
      try {
	delete a;
      } catch(...) {
	std::cerr<<'\n';
	WDutils_THROW("[%s:%d]: deleting object '%s' @ %p failed\n",
		      f,l,traits<T>::name(),a);
      }
    } else {
      try {
	delete a;
      } catch(...) {
	WDutils_THROW("[%s:%d]: deleting object '%s' @ %p failed\n",
		      f,l,traits<T>::name(),a);
      }
    }
  }
  //////////////////////////////////////////////////////////////////////////////
  template<typename T> inline
  void DelObject(const T* a, const char*f, int l, const char*lib="WDutils")
    WDutils_THROWING {
    if(0==a) {
      warning("[%s:%d]: trying to delete zero pointer to object '%s'",
	      f,l,traits<T>::name());
      return;
    }
    if(RunInfo::debug(10)) {
      std::cerr<< lib << " Debug Info: "<<f<<':'<<l
	       <<" de-allocating " << traits<T>::name()
	       <<" object @ "<<static_cast<const void*>(a)<<" ... ";
      try {
	delete a;
      } catch(...) {
	std::cerr<<'\n';
	WDutils_THROW("[%s:%d]: deleting object '%s' @ %p failed\n",
		      f,l,traits<T>::name(),a);
      }
      std::cerr<<"done\n";
    } else {
      try {
	delete a;
      } catch(...) {
	WDutils_THROW("[%s:%d]: deleting object '%s' @ %p failed\n",
		      f,l,traits<T>::name(),a);
      }
    }
  }
  // ///////////////////////////////////////////////////////////////////////////
  ///                                                                           
  /// C MACRO to be used for object de-allocation                               
  ///                                                                           
  /// should be used for all object  de-allocation in WDutils.                  
  ///                                                                           
  /// Calling WDutils::DelObject<TYPE>(), which in case of an error generates   
  /// an error message detailing the source file and line of the call.          
  ///                                                                           
  /// \param P  pointer to object to be de-allocated                            
#define WDutils_DEL_O(P) WDutils::DelObject(P,__FILE__,__LINE__)
  // ///////////////////////////////////////////////////////////////////////////
  //                                                                            
  //  WDutils::block_alloc<T>                                                   
  //                                                                            
  /// allocator for elements of type T                                          
  ///                                                                           
  /// Elements are allocated in blocks and given out either singly or in small  
  /// contiguous chunks. A forward iterator for sequential access to all        
  /// elements given out sofar.                                                 
  ///                                                                           
  /// In order to (i) guarantee the correct amount of elements (not known a     
  /// priori) will be allocated and (ii) not to waste memory by allocating too  
  /// many, we use the following strategy, similar to that of std::string.      
  /// Elements are allocated in blocks, which in turn are organized in a linked 
  /// list. When a number of new elements is to be allocated and the last block 
  /// in the list cannot provide them, we allocated a new block and add it to   
  /// the list. The number of elements allocated in this new block is taken to  
  /// be a function of the total number of elements used sofar. This function   
  /// must be provided by the user, otherwise the same number as in the last    
  /// block is used.                                                            
  ///                                                                           
  // ///////////////////////////////////////////////////////////////////////////
  template<typename T>
  class block_alloc {
  public:
    ////////////////////////////////////////////////////////////////////////////
    //                                                                          
    /// \name some public typedefs                                              
    //@{                                                                        
    ////////////////////////////////////////////////////////////////////////////
    typedef T         value_type;                ///< type of elements          
    typedef size_t    size_type;                 ///< type of number of elements
    typedef ptrdiff_t difference_type;           ///< type of pointer difference
    typedef T*        pointer;                   ///< type of pointer to element
    typedef const T*  const_pointer;             ///< type of const pointer     
    typedef T&        reference;                 ///< type of reference to elem 
    typedef const T&  const_reference;           ///< type of const reference   
    //@}
    // /////////////////////////////////////////////////////////////////////////
    //                                                                          
    //  sub-type WDutils::block_alloc::block                                    
    //                                                                          
    /// allocates and manages contiguous chunk of elements                      
    ///                                                                         
    // /////////////////////////////////////////////////////////////////////////
  private:
    class block {
    private:
      /// \name data of class WDutils::block_alloc::block
      //@{
      block      *NEXT;    ///< next block in linked list
      value_type *FIRST;   ///< front element
      value_type *END;     ///< end of active elements
      value_type *ENDTOT;  ///< end of all elements
      //@}
      block();
      //------------------------------------------------------------------------
    public:
      /// constructor
      /// \param n number of elements to allocate
      explicit
      block(size_type const&n) :
	NEXT    ( 0 ),
	FIRST   ( WDutils_NEW(value_type,n) ),
	END     ( FIRST ),
	ENDTOT  ( FIRST + n ) {}
      /// destructor: de-allocate
      ~block() {
	WDutils_DEL_A(FIRST);
      }
      /// link block to next block
      void link(block* next) {
	NEXT = next;
      }
      /// give out: another element
      pointer new_element() {
	return END++;
      }
      /// give out: N new elements
      pointer new_elements(size_type n) {
	register pointer OLD_END=END;
	END += n;
	return OLD_END;
      }
      /// can we give out up to \a n elements?
      bool has_free(size_type n) const {
	return END + n <= ENDTOT;
      }
      /// is block full (no free elements)?
      bool is_full() const {
	return END>=ENDTOT;
      }
      /// return next block in linked list of blocks
      block  *next () const {
	return NEXT;
      }
      /// return first element allocated
      pointer front() const {
	return FIRST;
      }
      /// return last element used
      pointer back () const {
	return END-1;
      }
      /// return end of elements used
      pointer end  () const {
	return END;
      }
      /// return number of elements used
      difference_type N_used () const {
	return END-FIRST;
      }
      /// return number of elemnets allocated
      difference_type N_alloc() const {
	return ENDTOT-FIRST;
      }
    };
  public:
    // /////////////////////////////////////////////////////////////////////////
    //                                                                          
    //  sub-type WDutils::block_alloc::iterator                                 
    //                                                                          
    /// for forward sequential iteration through all elements used              
    ///                                                                         
    // /////////////////////////////////////////////////////////////////////////
    class iterator {
    private:
      /// \name data of WDutils::block_alloc::iterator
      //@{
      block  *B;   ///< current block
      pointer E;   ///< current element
      //@}
    public:
      /// \name construction and assignment
      //@{
      /// construct from block pointer and element pointer
      iterator(block*b, pointer e)
	: B(b),   E(e)   {}
      /// copy constructor
      iterator(iterator const&I)
	: B(I.B), E(I.E) {}
      /// copy assignment
      iterator& operator= (iterator const&I) {
	B=I.B;
	E=I.E;
	return *this;
      }
      //@}
      /// prefix ++: increment pointer; if out of block: get next block
      iterator& operator++() {
	++E;
	if(E == B->end()) {
	  B = B->next();
	  E = B? B->front() : 0;
	}
	return *this;
      }
      /// postfix ++: return temporary equal original, but increment this
      iterator  operator++(int) {
	iterator tmp(*this);
	this->operator++();
	return tmp;
      }
      /// equality: refer to the same element?
      bool operator==(const iterator&I) {
	return B == I.B  &&  E == I.E;
      }
      /// inequality: not equal
      bool operator!=(const iterator&I) {
	return ! operator==(I);
      }
      /// \name conversions to pointer to elment or reference to element
      //@{
      /// type conversion to pointer to element
      operator pointer() {
	return E;
      }
      /// type conversion to pointer to const element
      operator const_pointer() const {
	return E;
      }
      /// reference operator
      reference operator* () {
	return*E;
      }
      /// const reference operator
      const_reference operator* () const {
	return*E;
      }
      /// dereference operator
      pointer operator->() {
	return E;
      }
      /// const dereference operator
      const_pointer operator->() const {
	return E;
      }
      //@}
    };
  private:
    // /////////////////////////////////////////////////////////////////////////
    //                                                                          
    /// \name member data of class WDutils::block_alloc                         
    //@{                                                                        
    // /////////////////////////////////////////////////////////////////////////
    block       *FIRST;  ///< first block in linked list
    block       *LAST;   ///< last block in linked list
    size_type    NTOT;   ///< # elements allocated
    size_type    NUSED;  ///< # elements used
    //@}
  public:
    // /////////////////////////////////////////////////////////////////////////
    //                                                                          
    /// \name member functions of class WDutils::block_alloc                    
    //@{                                                                        
    // /////////////////////////////////////////////////////////////////////////
    /// constructor: allocate first block
    /// \param Ns (input) number of elements in 1st block
    explicit
    block_alloc(size_type Ns)
      : FIRST ( new block(Ns) ),
	LAST  ( FIRST ),
	NTOT  ( Ns ),
	NUSED ( 0 )  {}
    /// destructor: delete all blocks
    ~block_alloc() WDutils_THROWING {
      register block *A=FIRST, *N;
      while(A) {
	N = A->next();
	WDutils_DEL_O(A);
	A = N;
      }
    }
    /// give out: another element
    /// \return pointer to allocated element
    pointer new_element() {
      if(LAST->is_full()) {
	register size_type New = LAST->N_alloc();
	LAST->link(new block(New));
	LAST = LAST->next();
	NTOT+= New;
      }
      ++NUSED;
      return LAST->new_element();
    }
    /// give out: another element
    /// \return pointer to allocated element
    /// \param F estimator = function object returning N_required(N_used yet)
    template<class estimator>
    pointer new_element(estimator const&F) {
      if(LAST->is_full()) {
	register size_type New = F(NUSED);
	LAST->link(new block(New));
	LAST = LAST->next();
	NTOT+= New;
      }
      ++NUSED;
      return LAST->new_element();
    }
    /// give out: more elements
    /// \return pointer to \a Ne allocated elements
    /// \param Ne number of elements to return pointer to
    pointer new_elements(size_type Ne) {
      if(!LAST->has_free(Ne)) {
	register size_type
	  New=max(Ne,LAST->N_alloc());
	LAST->link(new block(New));
	LAST = LAST->next();
	NTOT+= New;
      }
      NUSED+=Ne;
      return LAST->new_elements(Ne);
    }
    /// give out: more elements
    /// \return pointer to \a Ne allocated elements
    /// \param Ne number of elements to return pointer to
    /// \param F estimator = function object returning N_required(N_used yet)
    template<class estimator>
    pointer new_elements(size_type       Ne,
			 estimator const&F) {
      if(!LAST->has_free(Ne)) {
	register size_type New=max(Ne,F(NUSED));
	LAST->link(new block(New));
	LAST = LAST->next();
	NTOT+= New;
      }
      NUSED+=Ne;
      return LAST->new_elements(Ne);
    }
    /// the running number of a given element
    ///
    /// if \a E does appear not to point to an element given out by this, we
    /// return a negative number
    /// \return the running number of element pointed to by \a E
    /// \param  E   (input) pointer to element
    int number_of_element(const_pointer E) const {
      if(E==0) return -99;
      register int num=0;
      for(register block*B=FIRST; B; B=B->next())
	if(E >= B->front() && E < B->end())
	  return num + int(E-B->front());
	else
	  num += B->N_used();
      return -99;
    }
    /// was a given element given out by this?
    /// \return true if a given pointer to element was given out by this
    /// \param E pointer to element
    bool is_element(const_pointer E) const {
      if(E==0) return 0;
      for(register block*B=FIRST; B; B=B->next())
	if(E >= B->front() && E < B->end())
	  return 1;
      return 0;
    }
    /// return pointer to first element
    pointer first() const {
      return FIRST->front();
    }
    /// return iterator referring to first element
    iterator front() const {
      return iterator(FIRST,FIRST->front());
    }
    /// return iterator referring to first element
    iterator begin() const {
      return iterator(FIRST,FIRST->front());
    }
    /// return iterator referring to end of elements
    iterator end() const {
      return iterator(0,0);
    }
    /// return number of elements given out sofar
    size_type N_used() const {
      return NUSED;
    }
    /// return number of elements allocated sofar
    size_type N_allocated() const {
      return NTOT;
    }
  };// class WDutils::block_alloc
  // ///////////////////////////////////////////////////////////////////////////
  template<typename T> struct traits< block_alloc<T> > {
    static const char  *name () {
      return message("block_alloc<%s>",traits<T>::name());
    }
    static const char  *names() {
      return message("block_alloc<%s>",traits<T>::name());
    }
    static const unsigned size = sizeof(block_alloc<T>);
  };
  // ///////////////////////////////////////////////////////////////////////////
  template<typename T> struct traits< typename block_alloc<T>::block > {
    static const char  *name () {
      return message("block_alloc<%s>::block",traits<T>::name());
    }
    static const char  *names() {
      return message("block_alloc<%s>::block",traits<T>::name());
    }
    static const unsigned size = sizeof(typename block_alloc<T>::block);
  };
  // ///////////////////////////////////////////////////////////////////////////
  //                                                                            
  //  class WDutils::pool                                                       
  //                                                                            
  /// allocates blocks of K bytes (=elements) in chunks of N elements           
  ///                                                                           
  /// elements are defined solely by their size K in bytes. They are allocated  
  /// in chunks of N, which are organized as linked list. Single elements can   
  /// be handed out (allocate) or freed (de-allocated). Free elements are kept  
  /// in a linked list. The actual number of bytes per element is at least the  
  /// size of a pointer. Thus, for K < sizeof(void*), this class is inefficient.
  ///                                                                           
  // ///////////////////////////////////////////////////////////////////////////
  class pool {
  public:
    // /////////////////////////////////////////////////////////////////////////
    //                                                                          
    /// \name sub-types of class WDutils::pool                                  
    //@{                                                                        
    ////////////////////////////////////////////////////////////////////////////
    typedef size_t    size_type;         ///< type of number of elements
    typedef ptrdiff_t difference_type;   ///< type of pointer difference
  public:
    /// elementary of a linked list
    struct link {
      link *NEXT;   ///< pter to next link
    };
#define LINK(NAME) static_cast<link*>(static_cast<void*>(NAME))
    /// a chunk of elements
    struct chunk {
      char   *DATA; ///< pter to allocated memory
      chunk  *NEXT; ///< pter to next chunk
      /// constructor
      /// \param N  (input) number of element in chunk
      /// \param Kp (input) sizeof(elements)
      chunk(size_type N, size_type Kp)
	: DATA ( WDutils_NEW(char,N*Kp) ),
	  NEXT ( 0 ) {
	const    char *END=DATA+N*Kp;
	register char *l,*n;
	for(l=DATA, n=DATA+Kp; n!=END; l+=Kp,n+=Kp)
	  LINK(l)->NEXT = LINK(n);
	LINK(l)->NEXT = 0;
      }
      /// destructor: de-allocate memory
      ~chunk() {
	WDutils_DEL_A(DATA);
      }
    };// struct pool::chunk
    //@}
    // /////////////////////////////////////////////////////////////////////////
    //                                                                          
    /// \name data of class WDutils::pool                                       
    //@{                                                                        
    // /////////////////////////////////////////////////////////////////////////
  private:
    const size_type N;                    ///< # elements / chunk
    const size_type Kp;                   ///< sizeof(element)
    unsigned        NC;                   ///< # chunks
    unsigned        Na, Nmax;             ///< # elements given out
    chunk          *CHUNKS;               ///< pter to 1st chunk
    link           *HEAD;                 ///< pter to 1st free element
    //@}
    // /////////////////////////////////////////////////////////////////////////
    //                                                                          
    /// \name member functions of class WDutils::pool                           
    //@{                                                                        
    // /////////////////////////////////////////////////////////////////////////
    /// grow: add another chunk
    void grow() {
      chunk *c = new chunk(N,Kp);
      c->NEXT  = CHUNKS;
      CHUNKS   = c;
      HEAD     = LINK(CHUNKS->DATA);
      ++NC;
    }
  public:
    /// construction
    /// \param n (input) desired number of elements
    /// \param k (input) sizeof elements
    pool(size_type n, size_type k)
      : N      ( n<1? 1 : n ),
	Kp     ( sizeof(link)<k? k : sizeof(link) ),
	CHUNKS ( new chunk(N,Kp) ),
	HEAD   ( LINK(CHUNKS->DATA) ),
	NC     ( 1u ),
	Na     ( 0u ),
	Nmax   ( 0u ) {}
#undef LINK
    /// destruction: delete all chunks
    ~pool() {
      register chunk *a=CHUNKS, *n;
      while(a) {
	n = a->NEXT;
	WDutils_DEL_O(a);
	a = n;
      }
    }
    /// hand out an element = pointer to Kp (=at least K) free bytes
    void *alloc() {
      if(HEAD==0) grow();
      register link*p = HEAD;
      HEAD = p->NEXT;
      Na++;
      if(Na>Nmax) Nmax=Na;
      return p;
    }
    /// take back an element (freeing)
    /// \param e pointer to element
    /// \note the pointer \a e MUST have been previously allocated from this
    void free(void* e) {
      register link *p = static_cast<link*>(e);
      p->NEXT = HEAD;
      HEAD    = p;
      Na--;
    }
    /// return number of chunks used
    unsigned const&N_chunks   () const {
      return NC;
    }
    /// return number of bytes handed out
    unsigned const&N_alloc    () const {
      return Na;
    }
    /// return number of bytes allocated
    unsigned const&N_alloc_max() const {
      return Nmax;
    }
  };// class WDutils::pool
  // ///////////////////////////////////////////////////////////////////////////
  template<> struct traits< pool > {
    static const char    *name () { return "pool"; }
    static const char    *names() { return "pool"; }
    static const unsigned size = sizeof(pool);
  };
#if(0) // gcc v 3.3.5 is buggy and doesn't like this
  template<> struct traits< pool::chunk > {
    static const char    *name () { return "pool::chunk"; }
    static const char    *names() { return "pool::chunk"; }
    static const unsigned size = sizeof(pool::chunk);
  };
#endif
  // ///////////////////////////////////////////////////////////////////////////
  //                                                                            
  //  class WDutils::Pool<>                                                     
  //                                                                            
  /// template class, based on WDutils::pool, for allocating elemnts of type T  
  //                                                                            
  // ///////////////////////////////////////////////////////////////////////////
  template<typename T>
  class Pool : private pool {
  public:
    /// constructor: allocate 1st chunk
    /// \param n (input) number of elements in first chunk
    explicit
    Pool(size_type n)
      : pool(n,sizeof(T)) {}
    /// allocation: hand out a single element
    T*   alloc()    {
      return static_cast<T*>(pool::alloc());
    }
    /// freeing: take back a single element
    void free (T*e) {
      pool::free(e);
    }
    pool::N_chunks;    ///< return number of chunks used
    pool::N_alloc;     ///< return number of bytes handed out
    pool::N_alloc_max; ///< return number of bytes allocated
  };// class WDutils::Pool<>
  // ///////////////////////////////////////////////////////////////////////////
  template<typename T> struct traits< Pool<T> > {
    static const char  *name () {
      return message("Pool<%s>",traits<T>::name());
    }
    static const char  *names () {
      return message("Pool<%s>",traits<T>::name());
    }
    static const unsigned size = sizeof(Pool<T>);
  };
  // ///////////////////////////////////////////////////////////////////////////
  //                                                                            
  /// \name simple methods for 1, 2 & 3D array allocation/deallocation          
  //                                                                            
  /// \note These routines SHOULD NOT BE USED.                                  
  /// The methods are old and require too much memory. However, they are used in
  /// some equally old code (which I eventually have to re-write). Use the      
  /// class template Array<> below instead!                                     
  //@{                                                                          
  // ///////////////////////////////////////////////////////////////////////////
  /// allocate a 1D array of length \a N
  /// \param A (output) pointer to array
  /// \param N (input) number of elements
  template <typename T>
  inline void Alloc1D(T* &A, int N) WDutils_THROWING {
    A = WDutils_NEW(T,N); 
  }
  /// allocate a 1D array of length \a N and initialze it
  /// \param A (output) pointer to array
  /// \param N (input) number of elements
  /// \param X (input) value to initiaize all ements with
  template <typename T>
  inline void Alloc1D(T* &A, int N, T const&X) WDutils_THROWING {
    A = WDutils_NEW(T,N); 
    for(T a=A; a != A+N; ++a) *a = X;
  }
  /// de-allocate 1D array previously allocated by Alloc1D()
  /// \param A (input) pointer to array
  template <typename T>
  inline void Free1D(T* A) WDutils_THROWING {
    WDutils_DEL_A(A);
  }
  //----------------------------------------------------------------------------
  /// allocate a 2D array of size \a N[]
  /// \param A (output) pointer to array
  /// \param N (input) numbers of elements in dimensions 0,1
  template <typename T>
  inline void Alloc2D(T** &A, int N[2]) WDutils_THROWING {
    register int i, iN1;
    A    = WDutils_NEW(T*,N[0]);
    A[0] = WDutils_NEW(T ,N[0]*N[1]);
    for(int i=0,iN1=0; i!=N[0]; ++i, iN1+=N[1])
      A[i] = A[0] + iN1;
  }
  /// allocate a 2D array of size \a N[] and initialze it
  /// \param A (output) pointer to array
  /// \param N (input) numbers of elements in dimensions 0,1
  /// \param X (input) value to initiaize all ements with
  template <typename T>
  inline void Alloc2D(T** &A, int N[2], T const&X) WDutils_THROWING {
    register int i, iN1;
    A    = WDutils_NEW(T*,N[0]);
    A[0] = WDutils_NEW(T ,N[0]*N[1]);
    for(int i=0,iN1=0; i!=N[0]; ++i, iN1+=N[1]) {
      A[i] = A[0] + iN1;
      for(int j=0; j!=N[1]; ++j)
	A[i][j] = X;
    }
  }
  /// de-allocate 2D array previously allocated by Alloc2D()
  /// \param A (input) pointer to array
  template <typename T>
  inline void Free2D(T** A) WDutils_THROWING {
    WDutils_DEL_A(A[0]);
    WDutils_DEL_A(A);
  }
  //----------------------------------------------------------------------------
  /// allocate a 3D array of size \a N[]
  /// \param A (output) pointer to array
  /// \param N (input) numbers of elements in dimensions 0,1,2
  template <typename T>
  inline void Alloc3D(T*** &A, int N[3]) WDutils_THROWING {
    register int i, iN1;
    A       = WDutils_NEW(T**,N[0]);
    A[0]    = WDutils_NEW(T* ,N[0]*N[1]);
    A[0][0] = WDutils_NEW(T  ,N[0]*N[1]*N[2]);
    const int N12 = N[1]*N[2];
    for(int i=0,iN1=0,iN12=0; i!=N[0]; ++i, iN1+=N[1], iN12+=N12) {
      A[i]    = A[0]    + iN1;
      A[i][0] = A[0][0] + iN12;
      for(int j=0, jN2=0; j!=N[1]; ++j, jN2+=N[2])
	A[i][j] = A[i][0] + jN2;
    }
  }
  /// allocate a 3D array of size \a N[] and initialze it
  /// \param A (output) pointer to array
  /// \param N (input) numbers of elements in dimensions 0,1,2
  /// \param X (input) value to initiaize all ements with
  template <typename T>
  inline void Alloc3D(T*** &A, int N[3], T const&X) WDutils_THROWING {
    register int i, iN1;
    A       = WDutils_NEW(T**,N[0]);
    A[0]    = WDutils_NEW(T* ,N[0]*N[1]);
    A[0][0] = WDutils_NEW(T  ,N[0]*N[1]*N[2]);
    const int N12 = N[1]*N[2];
    for(int i=0,iN1=0,iN12=0; i!=N[0]; ++i, iN1+=N[1], iN12+=N12) {
      A[i]    = A[0]    + iN1;
      A[i][0] = A[0][0] + iN12;
      for(int j=0, jN2=0; j!=N[1]; ++j, jN2+=N[2]) {
	A[i][j] = A[i][0] + jN2;
	for(int k=0; k!=N[2]; ++k) A[i][j][k] = X;
      }
    }
  }
  /// de-allocate 3D array previously allocated by Alloc3D()
  /// \param A (input) pointer to array
  template <typename T>
  inline void Free3D(T*** A) WDutils_THROWING {
    WDutils_DEL_A(A[0][0]);
    WDutils_DEL_A(A[0]);
    WDutils_DEL_A(A);
  }
  //@}
  //////////////////////////////////////////////////////////////////////////////
  //                                                                          //
  // Arrays<T,D> of type T and arbitrary dimension D                          //
  //                                                                          //
  //////////////////////////////////////////////////////////////////////////////
#if defined(DEBUG) || defined(EBUG)
#  define THROW_BAD WDutils_THROWING
#  define CHECK_BAD(M,R)					\
  if(i< 0) {							\
    warning("i=%d <  0 in Array subscript #%d",i,R);		\
    WDutils_THROW("i=%d <  0 in Array subscript #%d",i,R);	\
  }								\
  if(i>=M) {							\
    warning("i=%d >= N=%d in Array subscript #%d",i,M,R);	\
    WDutils_THROW("i=%d >= N=%d in Array subscript #%d",i,M,R);	\
  }
#else
#  define THROW_BAD
#  define CHECK_BAD(M,R)
#endif
  //////////////////////////////////////////////////////////////////////////////
  template<typename, int> class ConstPseudoArray;
  template<typename, int> class PseudoArray;
  template<typename, int> class Array;
  // ///////////////////////////////////////////////////////////////////////////
  //                                                                            
  //  class ConstPseudoArray<T,D>                                               
  //                                                                            
  /// used as return type for Array<>::operator[]                               
  ///                                                                           
  /// Apart from a function returning the size in the first dimension, the only 
  /// public member function for D>1 is the operator[], which returns a         
  /// ConstPseudoArray<T,D-1>.                                                  
  /// For D=1, the [] operator returns a 'T const&'. Moreover for D=1, there    
  /// is a type conversion to const T*.                                         
  //                                                                            
  // ///////////////////////////////////////////////////////////////////////////
  template<typename T, int D> class ConstPseudoArray {
    friend class ConstPseudoArray<T,D+1>;
    friend class PseudoArray<T,D+1>;
    friend class Array<T,D+1>;
    /// \name data
    //@{
    const int*const N; ///< sizes in all dimensions
    const int*const K; ///< offsets in all dimensions
    const T  *const A; ///< pointer to first element
    //@}
    /// constructor is private: accessible only from friends
    ConstPseudoArray(const T*a, const int*n, const int*k) : N(n), K(k), A(a) {}
  public:
    /// return size in 1st dimension
    int const& size() const {
      return N[0];
    }
    /// acts like the operator[] on a pointer const T*...*
    ConstPseudoArray<T,D-1> operator[] (int i) const THROW_BAD {
      CHECK_BAD(N[0],D);
      return ConstPseudoArray<T,D-1>(A+i*K[0],N+1,K+1);
    }
    /// same as operator[]
    ConstPseudoArray<T,D-1> element (int i) const THROW_BAD {
      CHECK_BAD(N[0],D);
      return ConstPseudoArray<T,D-1>(A+i*K[0],N+1,K+1);
    }
  };// class ConstPseudoArray<T,D>
  // ///////////////////////////////////////////////////////////////////////////
  /// special case D=1
  template<typename T> class ConstPseudoArray<T,1> {
    friend class ConstPseudoArray<T,2>;
    friend class PseudoArray<T,2>;
    friend class Array<T,2>;
    /// \name data
    //@{
    const int*const N; ///< N[0] = length
    const T  *const A; ///< pointer to first element
    //@}
    /// constructor is private: accessible only from friends
    ConstPseudoArray(const T*a, const int*n, const int*) : N(n), A(a) {}
  public:
    /// return size in 1st dimension
    int const& size() const {
      return N[0];
    }
    /// conversion to pointer to constant element
    operator const T* () const {
      return A;
    }
    /// acts like the operator[] on a pointer const T*
    T const & operator[](int i) const THROW_BAD {
      CHECK_BAD(N[0],1);
      return A[i];
    }
    /// same as operator[]
    T const & element(int i) const THROW_BAD {
      CHECK_BAD(N[0],1);
      return A[i];
    }
  };// class ConstPseudoArray<T,D=1>
  // ///////////////////////////////////////////////////////////////////////////
  //                                                                            
  //  class PseudoArray<T,D>                                                    
  //                                                                            
  /// used as return type for Array<>::operator[]                               
  ///                                                                           
  /// Apart from a function returning the size in the first dimension, the only 
  /// public member functions for D>1 are the operator[], which returns a       
  /// PseudoArray<T,D-1>, and the operator[] const,  which returns a            
  /// ConstPseudoArray<T,D-1>.                                                  
  /// For D=1, the operator[] returns a 'T&' and the operator[] const a         
  /// 'T const&'. Moreover for D=1, there are type conversions to const T*      
  /// and T*.                                                                   
  //                                                                            
  // ///////////////////////////////////////////////////////////////////////////
  template<typename T, int D> class PseudoArray {
    friend class PseudoArray<T,D+1>;
    friend class Array<T,D+1>;
    /// \name data
    //@{
    const int*const N; ///< sizes in all dimensions
    const int*const K; ///< offsets in all dimensions
    T        *const A; ///< pointer to first element
    //@}
    /// constructor is private: accessible only from friends
    PseudoArray(T*a, const int*n, const int*k) : N(n), K(k), A(a) {}
  public:
    /// return size in 1st dimension
    int const& size() const {
      return N[0];
    }
    /// acts like the operator[] on a pointer T*...*
    PseudoArray<T,D-1> operator[] (int i) THROW_BAD {
      CHECK_BAD(N[0],D);
      return PseudoArray<T,D-1>(A+i*K[0],N+1,K+1);
    }
    /// acts like the operator[] on a pointer const T*...*
    ConstPseudoArray<T,D-1> operator[] (int i) const THROW_BAD {
      CHECK_BAD(N[0],D);
      return ConstPseudoArray<T,D-1>(A+i*K[0],K+1);
    }
    /// same as operator[]
    PseudoArray<T,D-1> element (int i) THROW_BAD {
      CHECK_BAD(N[0],D);
      return PseudoArray<T,D-1>(A+i*K[0],N+1,K+1);
    }
    /// same as operator[]
    ConstPseudoArray<T,D-1> element (int i) const THROW_BAD {
      CHECK_BAD(N[0],D);
      return ConstPseudoArray<T,D-1>(A+i*K[0],K+1);
    }
  };// class PseudoArray<T,D>
  // ///////////////////////////////////////////////////////////////////////////
  /// special case D=1
  template<typename T> class PseudoArray<T,1> {
    friend class PseudoArray<T,2>;
    friend class Array<T,2>;
    /// \name data
    //@{
    const int*const N; ///< sizes in all dimensions
    T        *const A; ///< pointer to first element
    //@}
    /// constructor is private: accessible only from friends
    PseudoArray(T*a, const int*n, const int*) : N(n), A(a) {}
  public:
    /// return size in 1st dimension
    int const& size() const {
      return N[0];
    }
    /// conversion to pointer to element
    operator T* () {
      return A;
    }
    /// conversion to pointer to constant element
    operator const T* () const {
      return A;
    }
    /// acts like the operator[] on a pointer T*...*
    T       & operator[](int i)       THROW_BAD {
      CHECK_BAD(N[0],1);
      return A[i];
    }
    /// acts like the operator[] on a pointer const T*...*
    T const & operator[](int i) const THROW_BAD {
      CHECK_BAD(N[0],1);
      return A[i];
    }
    /// same as operator[]
    T       & element(int i)       THROW_BAD {
      CHECK_BAD(N[0],1);
      return A[i];
    }
    /// same as operator[]
    T const & element(int i) const THROW_BAD {
      CHECK_BAD(N[0],1);
      return A[i];
    }
  };// class PseudoArray<T,D=1>
  // ///////////////////////////////////////////////////////////////////////////
  //                                                                            
  //  class Array<T,D>                                                          
  ///                                                                           
  /// acts like an D-dimensional array T[N_1]...[N_D]; operator[] as expected   
  //                                                                            
  // ///////////////////////////////////////////////////////////////////////////
  template<typename T, int D=1> class Array {
    /// \name data
    //@{
    int N[D];  ///< N[d]: size in dimension d 
    int K[D];  ///< K[d] = Prod_i>d N[i]      
    T  *A;     ///< pointer to allocated memory 
    //@}
    /// set N[d] and K[d]
    /// \param n (input) size of array in each dimension
    void set(const int*n) {
      for(int d=0; d!=D; ++d)
	N[d] = n? n[d] : 0;
      K[D-1] = 1;
      for(int d=D-1; d!=0; --d)
	K[d-1] = K[d] * N[d];
    }
    /// is a set \a n of sizes equal to ours?
    /// \param n (input) size of array in each dimension
    bool equal(const int n[D]) const {
      for(int d=0; d!=D; ++d)
	if(N[d] != n[d]) return false;
      return true;
    }
  public:
    /// rank: number of dimensions
    static const int rank = D;
    /// type resulting from a non-const [] operation
    typedef PseudoArray<T,D-1> Sub;
    /// type resulting from a const [] operation
    typedef ConstPseudoArray<T,D-1> ConstSub;
    /// return size in dimension \a d
    /// \param d (input) dimension to return size for
    int const&size(int d) const {
      return N[d];
    }
    /// reset: destruct and construct again
    /// \param n (input) new size of array in each dimension
    void reset(const int n[D]) WDutils_THROWING {
      if(A==0 || !equal(n) ) {
	if(A) WDutils_DEL_A(A);
	set(n);
	A = WDutils_NEW(T,K[0]*N[0]);
      }
    }
    /// reset: destruct and construct again
    /// \param n (input) new size of array in each dimension
    /// \param x (input) initialize each element with this value
    void reset(const int n[D], T const&x) WDutils_THROWING {
      reset(n);
      for(int i=0; i!=K[0]*N[0]; ++i) A[i] = x;
    }
    /// construction from nothing: sizes are all equal to 0
    Array() : A(0) {
      set(0);
    }
    /// construction from sizes
    /// \param n (input) size of array in each dimension
    explicit
    Array(const int n[D]) WDutils_THROWING
    : A(0) {
      reset(n);
    }
    /// construction from sizes for D=2
    Array(int n0, int n1) WDutils_THROWING
    : A(0) {
      const int n[2] = {n0,n1};
      reset(n);
    }
    /// construction from sizes for D=2
    Array(int n0, int n1, T const&x) WDutils_THROWING
    : A(0) {
      const int n[2] = {n0,n1};
      reset(n,x);
    }
    /// construction from sizes for D=3
    Array(int n0, int n1, int n2) WDutils_THROWING
    : A(0) {
      const int n[3] = {n0,n1,n2};
      reset(n);
    }
    /// construction from sizes for D=3
    Array(int n0, int n1, int n2, T const&x) WDutils_THROWING
    : A(0) {
      const int n[3] = {n0,n1,n2};
      reset(n,x);
    }
    /// construction from sizes for D=4
    Array(int n0, int n1, int n2, int n3) WDutils_THROWING
    : A(0) {
      const int n[4] = {n0,n1,n2,n3};
      reset(n);
    }
    /// construction from sizes for D=4
    Array(int n0, int n1, int n2, int n3, T const&x) WDutils_THROWING
    : A(0) {
      const int n[4] = {n0,n1,n2,n3};
      reset(n,x);
    }
    /// construction from sizes for D=5
    Array(int n0, int n1, int n2, int n3, int n4) WDutils_THROWING
    : A(0) {
      const int n[5] = {n0,n1,n2,n3,n4};
      reset(n);
    }
    /// construction from sizes for D=5
    Array(int n0, int n1, int n2, int n3, int n4, T const&x) WDutils_THROWING
    : A(0) {
      const int n[5] = {n0,n1,n2,n3,n4};
      reset(n,x);
    }
    /// construction from sizes and initial value
    /// \param n (input) size of array in each dimension
    /// \param x (input) initialize each element with this value
    Array(const int n[D], T const&x) WDutils_THROWING : A(0) {
      reset(n,x);
    }
    /// destruction: de-allocate memory
    ~Array() WDutils_THROWING {
      if(A) {
	WDutils_DEL_A(A);
	A = 0;
      }
      set(0);
    }
    /// type conversion to pointer: return first element
    T      *const&array()       { return A; }
    /// type conversion to const pointer: return first element
    const T*const&array() const { return A; }
    /// non-const array sub-scription: return PseudoArray
    PseudoArray<T,D-1> operator[] (int i) THROW_BAD {
      CHECK_BAD(N[0],D);
      return PseudoArray<T,D-1>(A+i*K[0],N+1,K+1);
    }
    /// const array sub-scription: return ConstPseudoArray
    ConstPseudoArray<T,D-1> operator[] (int i) const THROW_BAD {
      CHECK_BAD(N[0],D);
      return ConstPseudoArray<T,D-1>(A+i*K[0],N+1,K+1);
    }
    /// same as operator[]
    PseudoArray<T,D-1> element (int i) THROW_BAD {
      CHECK_BAD(N[0],D);
      return PseudoArray<T,D-1>(A+i*K[0],N+1,K+1);
    }
    /// same as operator[]
    ConstPseudoArray<T,D-1> element (int i) const THROW_BAD {
      CHECK_BAD(N[0],D);
      return ConstPseudoArray<T,D-1>(A+i*K[0],N+1,K+1);
    }
  };// class Array<T,D>
  // ///////////////////////////////////////////////////////////////////////////
  /// special case D=1
  template<typename T> class Array<T,1> {
    /// \name data
    //@{
    int N;     ///< N[d]: size in dimension d 
    T  *A;     ///< pointer to allocated memory 
    //@}
  public:
    /// rank: number of dimensions
    static const int rank = 1;
    /// type resulting from a non-const [] operation
    typedef T& Sub;
    /// type resulting from a const [] operation
    typedef T const& ConstSub;
    /// return size of array
    int const&size() const {
      return N;
    }
    /// return size of array
    int const&size(int) const {
      return N;
    }
    /// reset: destruct and construct again
    /// \param n (input) new size of array
    void reset(int n) WDutils_THROWING {
      if(A==0 || n != N) {
	if(A) WDutils_DEL_A(A);
	N = n;
	A = WDutils_NEW(T,N);
      }
    }
    /// reset: destruct and construct again
    /// \param n (input) new size of array
    void reset(const int n[1]) WDutils_THROWING {
      reset(n[0]);
    }
    /// reset: destruct and construct again
    /// \param n (input) new size of array
    /// \param x (input) initial value for each element
    void reset(int n, T const&x) WDutils_THROWING {
      reset(n);
      for(int i=0; i!=N; ++i) A[i] = x;
    }
    /// reset: destruct and construct again
    /// \param n (input) new size of array
    /// \param x (input) initial value for each element
    void reset(const int n[1], T const&x) WDutils_THROWING {
      reset(n[0],x);
    }
    /// construction from nothing: size equal to 0
    Array() : A(0), N(0) {}
    /// construction from sizes
    /// \param n (input) size of array in each dimension
    explicit
    Array(int n) WDutils_THROWING
    : A(0) {
      reset(n);
    }
    /// construction from sizes
    /// \param n (input) size of array in each dimension
    explicit
    Array(const int n[1]) WDutils_THROWING
    : A(0) {
      reset(n);
    }
    /// construction from size and initial value
    /// \param n (input) size of array in each dimension
    /// \param x (input) initialize each element with this value
    Array(const int n, T const&x) WDutils_THROWING
    : A(0) {
      reset(n,x);
    }
    /// construction from size and initial value
    /// \param n (input) size of array in each dimension
    /// \param x (input) initialize each element with this value
    Array(const int n[1], T const&x) WDutils_THROWING
    : A(0) {
      reset(n,x);
    }
    /// destruction: de-allocate memory
    ~Array() WDutils_THROWING { 
      if(A) {
	WDutils_DEL_A(A);
	A = 0;
      }
      N = 0;
    }
    /// type conversion to pointer: return first element
    T      *const&array()       { return A; }
    /// type conversion to const pointer: return first element
    const T*const&array() const { return A; }
    /// non-const array sub-scription: return reference to element
    T      & operator[] (int i)       THROW_BAD {
      CHECK_BAD(N,1);
      return A[i];
    }
    /// const array sub-scription: return const reference to element
    T const& operator[] (int i) const THROW_BAD {
      CHECK_BAD(N,1);
      return A[i];
    }
    /// same as operator[]
    T      & element (int i)       THROW_BAD {
      CHECK_BAD(N,1);
      return A[i];
    }
    /// same as operator[]
    T const& element (int i) const THROW_BAD {
      CHECK_BAD(N,1);
      return A[i];
    }
  };// class Array<T,1>
  // ///////////////////////////////////////////////////////////////////////////
  /// special case D=0, a scalar, provided for completeness only
  template<typename T> class Array<T,0> {
    T A;
  public:
    static const int rank = 0;
    int const&size(int) const { return 1; }
    explicit Array(int*) {}
    Array(int*, T const&x) : A(x) {}
    //--------------------------------------------------------------------------
    T      & operator[] (int i)       THROW_BAD {
      CHECK_BAD(0,0);
      return A;
    }
    T const& operator[] (int i) const THROW_BAD {
      CHECK_BAD(0,0);
      return A;
    }
    T      & element (int i)       THROW_BAD {
      CHECK_BAD(0,0);
      return A;
    }
    T const& element (int i) const THROW_BAD {
      CHECK_BAD(0,0);
      return A;
    }
  };
#undef THROW_BAD
#undef CHECK_BAD
  // ///////////////////////////////////////////////////////////////////////////
  template<typename T, int D> struct traits< Array<T,D> > {
    static const char  *name () {
      return message("Array<%s,%d>",traits<T>::name(),D);
    }
    static const char  *names() {
      return message("Array<%s,%d>",traits<T>::name(),D);
    }
    static const unsigned size = sizeof(Array<T,D>);
  };
  //////////////////////////////////////////////////////////////////////////////
} // namespace WDutils {
////////////////////////////////////////////////////////////////////////////////
#endif // WDutils_included_memory_h