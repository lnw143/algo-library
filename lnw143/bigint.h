#ifndef LNW143_BIGINT
#define LNW143_BIGINT

#include <cstring>
#include <algorithm>
#include <string>
#include <cstdint>

#ifndef _DEFAULT_BASE
#define _DEFAULT_BASE BIN
#endif

namespace lnw143::bigInt {

enum _Base {
	BIN = 2,
	OCT = 8,
	DEC = 10,
	HEX = 16,
};

template<class intType>
class bigInt {

public:
	using iterator = intType *;
	using const_iterator = const intType *;
	static constexpr int width = sizeof(intType)*__CHAR_BIT__;
	static_assert(4<=width&&(width&(width-1))==0, "bit width must be 2^x (x>=2)");

private:
	int n,m,sgn;
	iterator ptr;

public:

	// Member Function

	iterator begin() const {
		return ptr;
	}

	iterator end() const {
		return ptr+n;
	}

	int size() const {
		return n;
	}

	int capacity() const {
		return m;
	}

	const intType& operator[](int x) const {
		return ptr[x];
	}

	void resize(int m_) {
		delete[] ptr;
		m=m_;
		ptr=new intType[m+1];
	}

	std::string to_string(_Base base) const {
		std::string result="";
		if(base==BIN) {
			for(int i=n-1,exi=0; i>=0; --i)
				for(int j=width-1; j>=0; --j)
					if(exi|=(ptr[i]>>j)&1)
						result+="01"[(ptr[i]>>j)&1];
		} else if(base==OCT) {
			for(int i=n-1,exi=0; i>=0; --i)
				for(int j=width-(i*width+width-1)%3-1; j>=0; j-=3) {
					intType x=(ptr[i]>>j)&1;
					if(i==n-1) x=(ptr[i]>>j)&7;
					else {
						if(j<width-1) x|=((ptr[i]>>(j+1))&1)<<1;
						else x|=((ptr[i+1]>>(j+1-width))&1)<<1;
						if(j<width-2) x|=((ptr[i]>>(j+2))&1)<<2;
						else x|=((ptr[i+1]>>(j+2-width))&1)<<2;
					}
					if(exi|=x)
						result+="01234567"[x];
				}
		} else if(base==HEX) {
			for(int i=n-1,exi=0; i>=0; --i)
				for(int j=width-4; j>=0; j-=4)
					if(exi|=(ptr[i]>>j)&0xF)
						result+="0123456789ABCDEF"[(ptr[i]>>j)&0xF];
		} else
			throw "Not Supported Base!";
		return result.empty()?"0":result;
	}

	// Construct Function

	bigInt(int n_,int sgn_,const_iterator ptr_):n(n_),m(n_),sgn(sgn_),ptr(nullptr) {
		while(n>1&&ptr_[n-1]==0) --n;
		m=n;
		ptr=new intType[m+1];
		memcpy(ptr,ptr_,sizeof(intType)*n);
		if(n==1&&ptr[0]==0) sgn=0;
	}

	bigInt():sgn(0),n(1),m(1) {
		ptr=new intType[2];
		ptr[0]=0;
	}

	bigInt(const char *s,int len,_Base base):n(1),m(1),sgn(0),ptr(nullptr) {
		if(len==0) {
			ptr=new intType[2];
			ptr[0]=0;
		} else if(base==BIN) {
			if(*s=='-') sgn=-1,++s,--len;
			else {
				if(*s=='+') ++s,--len;
				sgn=1;
			}
			m=len/width+1;
			ptr=new intType[m+1];
			iterator cur=ptr;
			for(int i=len; i>=width; i-=width) {
				intType x=0;
				for(int j=1; j<=width; ++j)
					x|=(s[i-j]=='1')<<(j-1);
				*cur++=x;
			}
			if(len%width) {
				intType x=0;
				for(int i=0; i<len%width; ++i)
					x=(x<<1)|(s[i]=='1');
				*cur++=x;
			}
			n=cur-ptr;
		} else
			throw "Not Supported Base!";
	}

	bigInt(const bigInt& t):n(t.n),m(t.n),sgn(t.sgn) {
		ptr=new intType[m+1];
		memcpy(ptr,t.ptr,sizeof(intType)*n);
		while(n>1&&ptr[n-1]==0) --n;
		if(n==1&&ptr[0]==0) sgn=0;
	}

	// Deconstruct Function

	~bigInt() {
		delete[] ptr;
	}

};

template<class intType> bigInt<intType> operator&(const bigInt<intType>& x,const bigInt<intType>& y) {
	const int n=std::min(x.size(),y.size());
	intType *z=new intType[n+1];
	for(int i=0; i<n; ++i) z[i]=x[i]&y[i];
	return bigInt<intType>(n,1,z);
}

template<class intType> bigInt<intType> operator|(const bigInt<intType>& x,const bigInt<intType>& y) {
	const int n=std::max(x.size(),y.size());
	intType *z=new intType[n+1];
	for(int i=0; i<n; ++i) {
		z[i]=0;
		if(i<x.size()) z[i]|=x[i];
		if(i<y.size()) z[i]|=y[i];
	}
	return bigInt<intType>(n,1,z);
}

template<class intType> bigInt<intType> operator^(const bigInt<intType>& x,const bigInt<intType>& y) {
	const int n=std::max(x.size(),y.size());
	intType *z=new intType[n+1];
	for(int i=0; i<n; ++i) {
		z[i]=0;
		if(i<x.size()) z[i]^=x[i];
		if(i<y.size()) z[i]^=y[i];
	}
	return bigInt<intType>(n,1,z);
}

template<class intType> std::ostream& operator<<(std::ostream& out,const bigInt<intType>& num) {
	out<<num.to_string(_DEFAULT_BASE);
	return out;
}

}

using __int1048576 = lnw143::bigInt::bigInt<uint_fast64_t>;
using lnw143::bigInt::_Base;

__int1048576 operator "" _bin(const char *str,std::size_t len) {
	return __int1048576(str,len,_Base::BIN);
}

__int1048576 operator "" _oct(const char *str,std::size_t len) {
	return __int1048576(str,len,_Base::OCT);
}

__int1048576 operator "" _dec(const char *str,std::size_t len) {
	return __int1048576(str,len,_Base::DEC);
}

__int1048576 operator "" _hex(const char *str,std::size_t len) {
	return __int1048576(str,len,_Base::HEX);
}

#endif