#pragma once

#define MASH_cout std::cout
#define MASH_uint GLuint
#define MASH_str std::string
#define MASH_ifstream std::ifstream
#define MASH_sstream std::stringstream
#define MASH_pcchar const char*
#define MASH_null nullptr
#define MASH_punchar unsigned char*
#define MASH_int int
#define MASH_endl std::endl

class Universal {
public:
	void MASH_log(MASH_str);
	void MASH_log(int);
};