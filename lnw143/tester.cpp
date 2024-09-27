#include <iostream>
#define _DEFAULT_BASE OCT
#include "bigint.h"

// using namespace lnw143::bigInt;
using namespace std;

using LL = __int1048576;

int main() {
	LL a="101010101011101010101010100110100"_bin,b="10101010101001001111101000100010101101110011"_bin;
	LL c=a|b,d=a&b,e=a^b;
	cout<<c<<' '<<d<<' '<<e<<endl;
	return 0;
}