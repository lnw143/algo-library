#ifndef __LNW143_POLY
#define __LNW143_POLY

#include <algorithm>
#include <stdexcept>
#include "basic.h"

namespace lnw143::poly {

using ll = long long;

template<ll type,ll G,ll P> void NTT(ll n,ll *a) {
	static_assert(type==1||type==-1);
	static_assert(0<=G&&G<P);
	static ll omg_n[30],inv_n[30];
	if(omg_n[0]==0) {
		ll k=0,e=P-1;
		while((e&1)==0) e>>=1,++k;
		omg_n[k]=basic::mpow(G,e,P);
		for(ll i=k-1; i>=0; --i) omg_n[i]=omg_n[i+1]*omg_n[i+1]%P;
		inv_n[0]=1;
		inv_n[1]=basic::mpow(2ll,P-2,P);
		for(ll i=2; i<30; ++i) inv_n[i]=inv_n[i-1]*inv_n[1]%P;
	}
	if(n&(n-1)) throw std::invalid_argument("N is not a power of 2!");
	ll m=0;
	while((1<<m)<n) ++m;
	ll *r=new ll[n];
	memset(r,0,sizeof(ll)*n);
	for(ll i=0; i<n; ++i) r[i]=(r[i>>1]>>1)|((i&1)<<(m-1));
	for(ll i=0; i<n; ++i) if(i<r[i]) std::swap(a[i],a[r[i]]);
	delete[] r;
	for(ll i=1,p=1; i<n; i<<=1,++p)
		for(ll j=0; j<n; j+=i<<1) {
			ll omg=1;
			for(ll k=0; k<i; ++k,(omg*=omg_n[p])%=P) {
				const ll u=a[j|k],v=a[j|k|i]*omg%P;
				a[j|k]=u+v;
				if(a[j|k]>=P) a[j|k]-=P;
				a[j|k|i]=u<v?u-v+P:u-v;
			}
		}
	if(type==-1) for(ll i=0; i<n; ++i) (a[i]*=inv_n[m])%=P;
}

}

#endif