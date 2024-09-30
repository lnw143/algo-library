#include <iostream>
#define _DEFAULT_BASE BIN
#include "lnw143/bigint.h"

// using namespace lnw143::bigInt;
using namespace std;

using LL = __int1048576;

int main() {
	LL a,b;
	cin>>a>>b;
	cout<<a+b<<endl;
	lnw143::bigInt::less_than(a,b);
	return 0;
}