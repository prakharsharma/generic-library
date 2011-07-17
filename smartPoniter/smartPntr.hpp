#ifndef _SMART_PNTR_H
#define _SMART_PNTR_H

#include <iostream>
#include <string>
/*a generic implementation for a reference counted smart-auto pointer
 * implementation. => it will not have short comings of std::auto_ptr stemming
 * from ownership issues*/

template<class T>
struct typeTraits {
    static bool isPntrType () {
	return false;
    }
};

template<class T>/*partial specialization for pointer types*/
struct typeTraits<T*> {
    static bool isPntrType () {
	return true;
    }
};

template<class T>
struct scalarResource {
    static const bool scalar = true;
    static void release (T **resource) {
	delete *resource;
	*resource = NULL;
	std::cout<<"Inside delete pntr"<<std::endl;
	return;
    }
    static void print (int line) {
	std::cout<<"Inside scalarResource::print(): "<<line<<std::endl;
    }
};

template<class T>
struct vectorResource {
    static const bool scalar = false;
    static void release (T **resource) {
	delete [] *resource;
	*resource = NULL;
	std::cout<<"Inside delete [] pntr"<<std::endl;
	return;
    }
    static void print (int line) {
	std::cout<<"Inside vectorResource::print() :"<<line<<std::endl;
    }
};

struct defltRefTraits {
    static void deRefer (int *rc) throw(std::string) {
	if (!*rc)
	    return;
	if (*rc < 0)
	    throw std::string("BAD PONITER OPERATION: no owners");
	--(*rc);
	return;
    }
    static bool canRelease (int rc) {
	return rc ? false : true;
    }
    static void refer (int *rc) {
	++(*rc);
	return;
    }
};

template<class T, class scalarTraits = scalarResource<T>, class rfTraits =
defltRefTraits >
class mngdMem {
    public:
	mngdMem() { pointee = NULL; rc = 0; scalarTraits::print(__LINE__); }
	mngdMem (T *data) { pointee = data; rc = 1; scalarTraits::print(__LINE__); }
	T& operator* () throw(std::string) {
	    if (!pointee)
		throw std::string("Attempt to dereference a NULL pointer");
	    return static_cast<T>(pointee);
	}
	~mngdMem () {
	    if (rc > 1)
		std::cerr<<"Some one still holding reference to pointer"<<std::endl;
	    scalarTraits::release(&pointee);
	    rc = 0;
	}
	void deRefer () throw(std::string) {
	    rfTraits::deRefer(&rc);
	    if (rfTraits::canRelease(rc))
		scalarTraits::release(&pointee);
	    return;
	}
	void refer () {
	    rfTraits::refer(&rc);
	    return;
	}

	/*data members*/
	T *pointee;
	int rc;
    private:
	mngdMem(const mngdMem &);
	mngdMem& operator= (mngdMem &);
};

/*rfTraits is not templatized @the moment, but can easily be done if need
 * arises. Currently what I have in my mind, is to make refCnt-ing operations
 * thread safe or not?*/
template<class T, class scalarTraits = scalarResource<T>, class rfTraits =
defltRefTraits >
class smartPntr {
    public:
	smartPntr () {  pointee = NULL; scalarTraits::print(__LINE__); }
	smartPntr<T, scalarTraits, rfTraits>& operator= (smartPntr<T> &src) {
	    if (this == &src)
		return *this;
	    if (pointee)
		pointee->deRefer();
	    src.pointee->refer();
	    pointee = src.pointee;/*normal pntr copy here. No overloading*/
	    scalarTraits::print(__LINE__);
	    return *this;
	}
	void attach (T *data) {
	    pointee = new mngdMem<T, scalarTraits, rfTraits>(data);
	    return;
	}
	void operator() (T *data) {
	    std::cout<<"Inside smartPntr::operator()"<<std::endl;
	    pointee = new mngdMem<T, scalarTraits, rfTraits>(data);
	    return;
	}
	T& operator* () {
	    return *pointee;
	}
	~smartPntr () {
	    if (pointee)
		pointee->deRefer();
	    pointee = NULL;
	}

	mngdMem<T, scalarTraits, rfTraits> *pointee;
    private:
	smartPntr (const smartPntr<T> &);
};

#endif
