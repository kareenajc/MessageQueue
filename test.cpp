#include <iostream>
#include <typeinfo>
#include <sys/types.h>	//for getpid()
#include <unistd.h>	//for getpid()

using namespace std;

int main(){



cout << typeid(getpid());

return 0;
}
