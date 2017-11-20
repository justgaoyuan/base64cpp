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
	unsigned char in[3];
	unsigned char out[4];

	while (in_len--) {
		in[i++] = *(bytes_to_encode++);
		if (i == 3) {
			out[0] = (in[0] & 0xfc) >> 2;
			out[1] = static_cast<unsigned char>(((in[0] & 0x03) << 4) + ((in[1] & 0xf0) >> 4));
			out[2] = static_cast<unsigned char>(((in[1] & 0x0f) << 2) + ((in[2] & 0xc0) >> 6));
			out[3] = in[2] & 0x3f;

			for (i = 0; i < 4; i++)
				ret += base64_chars[out[i]];
			i = 0;
		}
	}

	if (i) {
		for(j = i; j < 3; j++)
			in[j] = '\0';

		out[0] = (in[0] & 0xfc) >> 2;
		out[1] = static_cast<unsigned char>(((in[0] & 0x03) << 4) + ((in[1] & 0xf0) >> 4));
		out[2] = static_cast<unsigned char>(((in[1] & 0x0f) << 2) + ((in[2] & 0xc0) >> 6));

		for (j = 0; (j <= i); j++)
			ret += base64_chars[out[j]];

		while ((i++ < 3))
			ret += '=';
	}

	return ret;

}

std::string decode(const char * bytes_to_decode, std::size_t in_len) {
	std::string ret;
	uint i = 0;
	uint j = 0;
	int in_ = 0;
	unsigned char in[4]
	unsigned char out[3];


	while (in_len-- && ( bytes_to_decode[in_] != '=') && is_base64(bytes_to_decode[in_])) {
		in[i++] = bytes_to_decode[in_]; in_++;
		if (i == 4) {
			for (i = 0; i < 4; i++)
				in[i] = static_cast<unsigned char>(base64_chars.find(in[i]));

			out[0] = static_cast<unsigned char>(( in[0] << 2       ) + ((in[1] & 0x30) >> 4));
			out[1] = static_cast<unsigned char>(((in[1] & 0xf) << 4) + ((in[2] & 0x3c) >> 2));
			out[2] = static_cast<unsigned char>(((in[2] & 0x3) << 6) +   in[3]);

			for (i = 0; i < 3; i++)
				ret += out[i];
			i = 0;
		}
	}

	if (i) {
		for (j = 0; j < i; j++)
			in[j] = static_cast<unsigned char>(base64_chars.find(in[j]));

		out[0] = static_cast<unsigned char>((in[0] << 2) + ((in[1] & 0x30) >> 4));
		out[1] = static_cast<unsigned char>(((in[1] & 0xf) << 4) + ((in[2] & 0x3c) >> 2));

		for (j = 0; (j < i - 1); j++)
			ret += out[j];
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
