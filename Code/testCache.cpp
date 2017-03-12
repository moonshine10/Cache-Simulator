#include "cache.hpp"

//#include "back_func.hpp"
//#include "default.hpp"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

#define debug

cache *LIcache;
void test ()
{
	
    LIcache=new cache(8192,  1,  32);
	unsigned long long int cache_index = LIcache->get_addr_index(0x7f81ce2240Fc);
	printf(cache_index);

	
}
