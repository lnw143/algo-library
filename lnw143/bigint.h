#ifndef __LNW143_BIGINT
#define __LNW143_BIGINT

#include <cstring>
#include <algorithm>
#include <string>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <stdexcept>

#include "poly.h"

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

using ll = long long;

template<class int_type>
class bigInt {

public:
	using iterator = int_type *;
	using const_iterator = const int_type *;
	using based_ptr = std::shared_ptr<int_type[]>;
	static constexpr int W = sizeof(int_type)*__CHAR_BIT__;
	static_assert(std::is_integral<int_type>::value&&~int_type(0)>=0, "int_type must be unsigned integer");
	static_assert(64<=W&&(W&(W-1))==0, "bit width must be 2^x (x>=6)");

private:
	ll n,sgn;
	based_ptr a;

public:

	// Member Function

	const_iterator begin() const { return a.get(); }
	const_iterator end() const { return a.get()+n; }
	ll size() const { return n; }
	int get_sign() const { return sgn; }
	const int_type& operator[](ll x) const { return a[x]; }
	operator bool() const { return sgn!=0; }

	std::string to_string(_Base base) const {
		if(sgn==0) return "0";
		std::string result="";
		if(sgn==-1) result+='-';
		if(base==BIN) {
			for(int i=n-1,exi=0; i>=0; --i)
				for(int j=W-1; j>=0; --j)
					if(exi|=(a[i]>>j)&1)
						result+="01"[(a[i]>>j)&1];
		} else if(base==OCT) {
			for(int i=n-1,exi=0; i>=0; --i)
				for(int j=W-(i*W+W-1)%3-1; j>=0; j-=3) {
					int_type x=(a[i]>>j)&1;
					if(i==n-1) x=(a[i]>>j)&7;
					else {
						if(j<W-1) x|=((a[i]>>(j+1))&1)<<1;
						else x|=((a[i+1]>>(j+1-W))&1)<<1;
						if(j<W-2) x|=((a[i]>>(j+2))&1)<<2;
						else x|=((a[i+1]>>(j+2-W))&1)<<2;
					}
					if(exi|=x)
						result+="01234567"[x];
				}
		} else if(base==HEX) {
			for(int i=n-1,exi=0; i>=0; --i)
				for(int j=W-4; j>=0; j-=4)
					if(exi|=(a[i]>>j)&0xF)
						result+="0123456789ABCDEF"[(a[i]>>j)&0xF];
		} else
			throw std::invalid_argument("Not Supported Base!");
		return result;
	}

	// Construct Function

	bigInt(ll n_,ll sgn_,const based_ptr& a_):n(n_),sgn(sgn_),a(a_) {
		if(!a) {
			a=based_ptr(new int_type[2]{0,0});
			n=1;
			sgn=0;
		} else {
			while(n>1&&a[n-1]==0) --n;
			if(n==1&&a[0]==0) sgn=0;
		}
	}

	bigInt():n(1),sgn(0),a(based_ptr(new int_type[2]{0,0})) {}

	bigInt(const char *s,ll len,_Base base):n(1),sgn(0),a(nullptr) {
		if(len==0) {
			a=based_ptr(new int_type[2]{0,0});
		} else {
			if(*s=='-') sgn=-1,++s,--len;
			else {
				if(*s=='+') ++s,--len;
				sgn=1;
			}
			if(base==BIN) {
				for(ll i=0; i<len; ++i) if(s[i]!='0'&&s[i]!='1') throw std::invalid_argument("This is not binary string!");
				a=based_ptr(new int_type[len/W+2]);
				iterator t=a.get();
				for(ll i=len; i>=W; i-=W) {
					int_type x=0;
					for(ll j=W; j>=1; --j)
						x=(x<<1)|(s[i-j]&1);
					*t++=x;
				}
				if(len%W) {
					int_type x=0;
					for(ll i=0; i<len%W; ++i)
						x=(x<<1)|(s[i]&1);
					*t++=x;
				}
				n=t-a.get();
			} else if(base==HEX) {
				static constexpr ll S = W / 4;
				for(ll i=0; i<len; ++i)
					if(!('0'<=s[i]&&s[i]<='9')&&!('A'<=s[i]&&s[i]<='F')&&!('a'<=s[i]&&s[i]<='f'))
						throw std::invalid_argument("This is not hexadecimal string!");
				a=based_ptr(new int_type[len/S+2]);
				int_type *t=a.get();
				for(ll i=len; i>=S; i-=S) {
					int_type x=0;
					for(ll j=S; j>=1; --j)
						if(isdigit(s[i-j]))
							x=(x<<4)|(s[i-j]&15);
						else if(isupper(s[i-j]))
							x=(x<<4)|(s[i-j]-'A'+10);
						else
							x=(x<<4)|(s[i-j]-'a'+10);
					*t++=x;
				}
				if(len%S) {
					int_type x=0;
					for(ll i=0; i<len%S; ++i)
						if(isdigit(s[i]))
							x=(x<<4)|(s[i]&15);
						else if(isupper(s[i]))
							x=(x<<4)|(s[i]-'A'+10);
						else
							x=(x<<4)|(s[i]-'a'+10);
					*t++=x;
				}
				n=t-a.get();
			} else
				throw std::invalid_argument("Not Supported Base!");
		}
	}

};

template<class int_type> bigInt<int_type> operator&(const bigInt<int_type>& x,const bigInt<int_type>& y) {
	const ll n=std::min(x.size(),y.size());
	typename bigInt<int_type>::based_ptr z(new int_type[n+1]);
	for(int i=0; i<n; ++i) z[i]=x[i]&y[i];
	return bigInt<int_type>(n,x.get_sign()*y.get_sign(),z);
}

template<class int_type> bigInt<int_type> operator|(const bigInt<int_type>& x,const bigInt<int_type>& y) {
	const ll n=std::max(x.size(),y.size());
	typename bigInt<int_type>::based_ptr z(new int_type[n+1]);
	for(int i=0; i<n; ++i) {
		z[i]=0;
		if(i<x.size()) z[i]|=x[i];
		if(i<y.size()) z[i]|=y[i];
	}
	return bigInt<int_type>(n,x.get_sign()*y.get_sign(),z);
}

template<class int_type> bigInt<int_type> operator^(const bigInt<int_type>& x,const bigInt<int_type>& y) {
	const ll n=std::max(x.size(),y.size());
	typename bigInt<int_type>::based_ptr z(new int_type[n+1]);
	for(int i=0; i<n; ++i) {
		z[i]=0;
		if(i<x.size()) z[i]^=x[i];
		if(i<y.size()) z[i]^=y[i];
	}
	return bigInt<int_type>(n,x.get_sign()*y.get_sign(),z);
}

template<class int_type> static bool _Less_than(const bigInt<int_type>& x,const bigInt<int_type>& y) {
	if(x.size()!=y.size()) return x.size()<y.size();
	for(int i=x.size()-1; i>=0; --i)
		if(x[i]!=y[i])
			return x[i]<y[i];
	return false;
}

template<class int_type> bool operator<(const bigInt<int_type>& x,const bigInt<int_type>& y) {
	if(x.get_sign()!=y.get_sign()) return x.get_sign()<y.get_sign();
	return _Less_than(x,y);
}

template<class int_type> bool operator>(const bigInt<int_type>& x,const bigInt<int_type>& y) {
	return y<x;
}

template<class int_type> bool operator<=(const bigInt<int_type>& x,const bigInt<int_type>& y) {
	return !(y<x);
}

template<class int_type> bool operator>=(const bigInt<int_type>& x,const bigInt<int_type>& y) {
	return !(x<y);
}

template<class int_type> bool operator==(const bigInt<int_type>& x,const bigInt<int_type>& y) {
	if(x.size()!=y.size()||x.get_sign()!=y.get_sign()) return false;
	for(int i=0; i<x.size(); ++i)
		if(x[i]!=y[i])
			return false;
	return true;
}

template<class int_type> bool operator!=(const bigInt<int_type>& x,const bigInt<int_type>& y) {
	return !(x==y);
}

template<class int_type> bigInt<int_type> operator+(const bigInt<int_type>& x,const bigInt<int_type>& y) {
	static constexpr int_type all=~int_type(0)>>1;
	static constexpr int W=bigInt<int_type>::W;
	const ll n=std::max(x.size(),y.size())+1;
	typename bigInt<int_type>::based_ptr z(new int_type[n+1]);
	for(int i=0,addi=0; i<n; ++i) {
		int_type a=i<x.size()?x[i]:0,b=i<y.size()?y[i]:0;
		z[i]=(a&all)+(b&all)+addi;
		int_type u=a>>(W-1),v=b>>(W-1),w=z[i]>>(W-1);
		z[i]^=(u^v)<<(W-1);
		addi=(u&v)|((u|v)&w);
	}
	return bigInt<int_type>(n,1,z);
}

template<class int_type> bigInt<int_type> operator*(const bigInt<int_type>& x,const bigInt<int_type>& y) {
	static constexpr int W=bigInt<int_type>::W;
	static constexpr ll P = 998244353, G = 3, G_inv = 332748118;
	static constexpr ll S = W / 4;
	static_assert(G*G_inv%P==1);
	ll n=1;
	while(n<=x.size()+y.size()) n<<=1;
	n*=S;
	ll *a=new ll[n],*b=new ll[n];
	memset(a,0,sizeof(ll)*n);
	memset(b,0,sizeof(ll)*n);
	for(ll i=0; i<x.size(); ++i)
		for(ll j=0; j<W; j+=4)
			a[(i*W+j)>>2]=(x[i]>>j)&0xF;
	for(ll i=0; i<y.size(); ++i)
		for(ll j=0; j<W; j+=4)
			b[(i*W+j)>>2]=(y[i]>>j)&0xF;
	poly::NTT<1>(n,a,G,P);
	poly::NTT<1>(n,b,G,P);
	for(ll i=0; i<n; ++i) a[i]=a[i]*b[i]%P;
	poly::NTT<-1>(n,a,G_inv,P);
	ll addi=0;
	for(ll i=0; i<n; ++i) {
		addi+=a[i];
		a[i]=addi&0xF;
		addi>>=4;
	}
	typename bigInt<int_type>::based_ptr z(new int_type[n/S+2]);
	for(ll i=0; i<n/S; ++i) {
		z[i]=0;
		for(ll j=0; j<S; ++j)
			z[i]|=a[i*S+j]<<(j*4);
	}
	delete[] a;
	delete[] b;
	return bigInt<int_type>(n/S,x.get_sign()*y.get_sign(),z);
}

template<class int_type> std::istream& operator>>(std::istream& in,bigInt<int_type>& num) {
	std::string str;
	in>>str;
	num=bigInt<int_type>(str.c_str(),str.size(),_DEFAULT_BASE);
	return in;
}

template<class int_type> std::ostream& operator<<(std::ostream& out,const bigInt<int_type>& num) {
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