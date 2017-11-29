#pragma once

#include <string>

namespace base64 {

	std::string encode(const char * bytes_to_encode, std::size_t in_len);
	std::string decode(const char * bytes_to_decode, std::size_t in_len);

	std::string encode(const std::string & );
	std::string decode(const std::string & );

	void save(const char * path, const std::string & base64_str);
	void load(const char * path, std::string & content);
}
