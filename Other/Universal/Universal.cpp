#include <iostream>
#include <string>

#include "Universal/Universal.h"

void Universal::MASH_log(MASH_str message)
{
	MASH_cout << message << MASH_endl;
}

void Universal::MASH_log(MASH_int message)
{
	MASH_cout << message << MASH_endl;
}