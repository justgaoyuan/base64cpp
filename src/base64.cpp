#include <fstream>

#include "base64.hpp"

namespace base64 {

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string encode(const char * bytes_to_encode, std::size_t in_len) {
	std::string ret;
	uint i = 0;
	uint j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = static_cast<unsigned char>(((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4));
			char_array_4[2] = static_cast<unsigned char>(((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6));
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i) {
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = static_cast<unsigned char>(((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4));
		char_array_4[2] = static_cast<unsigned char>(((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6));

		for (j = 0; (j <= i); j++)
			ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
			ret += '=';
	}

	return ret;

}

std::string decode(const char * bytes_to_decode, std::size_t in_len) {
	std::string ret;
	uint i = 0;
	uint j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];


	while (in_len-- && ( bytes_to_decode[in_] != '=') && is_base64(bytes_to_decode[in_])) {
		char_array_4[i++] = bytes_to_decode[in_]; in_++;
		if (i ==4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));

			char_array_3[0] = static_cast<unsigned char>(( char_array_4[0] << 2       ) + ((char_array_4[1] & 0x30) >> 4));
			char_array_3[1] = static_cast<unsigned char>(((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2));
			char_array_3[2] = static_cast<unsigned char>(((char_array_4[2] & 0x3) << 6) +   char_array_4[3]);

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = 0; j < i; j++)
			char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

		char_array_3[0] = static_cast<unsigned char>((char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4));
		char_array_3[1] = static_cast<unsigned char>(((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2));

		for (j = 0; (j < i - 1); j++)
			ret += char_array_3[j];
	}

	return ret;
}

std::string encode(const std::string & src)
{
  return encode(src.c_str(), src.length());
}

std::string decode(const std::string & src)
{
  return decode(src.c_str(), src.length());
}


void save_base64(const char * path, const std::string & base64_str)
{
	std::ofstream file(path, std::ios::trunc | std::ios::binary);
	file.write(base64_str.c_str(), base64_str.length());
	file.close();
}

}
