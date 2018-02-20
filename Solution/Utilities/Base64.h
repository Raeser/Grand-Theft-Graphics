#pragma once
#include <string>

namespace Corgi
{
	std::string Base64Encode(unsigned char const* bytes_to_encode, size_t in_len);
	std::string Base64Decode(std::string const& encoded_string);
}