#ifndef __LNW143_BASIC
#define __LNW143_BASIC

namespace lnw143::basic {

template<class int_type> int_type qpow(int_type a,int_type n) {
	int_type x=1;
	for(; n; n>>=1,a=a*a) if(n&1) x=x*a;
	return x;
}

template<class int_type> int_type mpow(int_type a,int_type n,const int_type& p) {
	int_type x=1;
	for(; n; n>>=1,a=a*a%p) if(n&1) x=x*a%p;
	return x;
}

}

#endif