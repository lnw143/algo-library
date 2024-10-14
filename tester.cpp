#include <cstdio>
#include <cstring>
#define _DEFAULT_BASE HEX
#include "lnw143/bigint.h"

using namespace lnw143::bigInt;
using namespace std;

using LL = __int1048576;

char s[1<<22],t[1<<22];

int main() {
	freopen(".in","r",stdin);
	freopen(".out","w",stdout);
	LL a,b;
	scanf("%s%s",s,t);
	a=LL(s,strlen(s),HEX);
	b=LL(t,strlen(t),HEX);
	printf("%s\n",(a*b).to_string(HEX).c_str());
	return 0;
}