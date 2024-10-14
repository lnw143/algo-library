#ifndef __LNW143_POLY
#define __LNW143_POLY

#include <algorithm>
#include <stdexcept>
#include "basic.h"

namespace lnw143::poly {

using ll = long long;

template<int type,class int_type> void NTT(ll n,int_type *a,const int_type& G,const int_type& P) {
	static_assert(type==1||type==-1);
	if(n&(n-1)) throw std::invalid_argument("N is not a power of 2!");
	ll m=0;
	while((1<<m)<n) ++m;
	ll *r=new ll[n];
	memset(r,0,sizeof(ll)*n);
	for(int i=0; i<n; ++i) r[i]=(r[i>>1]>>1)|((i&1)<<(m-1));
	for(int i=0; i<n; ++i) if(i<r[i]) std::swap(a[i],a[r[i]]);
	delete[] r;
	for(int i=1; i<n; i<<=1) {
		const int_type omgn = basic::mpow(G,(P-1)/(i<<1),P);
		for(int j=0; j<n; j+=i<<1) {
			int_type omg=1;
			for(int k=0; k<i; ++k,(omg*=omgn)%=P) {
				const int_type u=a[j+k],v=a[j+k+i]*omg%P;
				a[j+k]=(u+v)%P;
				a[j+k+i]=(u-v+P)%P;
			}
		}
	}
	if(type==-1) for(int i=0; i<n; ++i) (a[i]*=basic::mpow(int_type(n),P-2,P))%=P;
}

}

#endif