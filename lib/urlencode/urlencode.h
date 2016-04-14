// Uri encode and decode.
// RFC1630, RFC1738, RFC2396
#ifndef __URL_ENCODE__
#define __URL_ENCODE__

#include <string>
class CUrlEncode
{
public:
	static std::string Encode(const std::string & strSrc);
	static std::string Decode(const std::string & strSrc);
};
#endif