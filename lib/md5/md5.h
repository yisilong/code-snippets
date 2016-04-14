#ifndef __MD5_H__
#define __MD5_H__

#include <string>
#include <fstream>

/* Type define */
typedef unsigned char byte;
typedef unsigned int uint32;

using std::string;
using std::ifstream;

/* CMD5 declaration. */
class CMD5 {
public:
	CMD5();
	CMD5(const void* input, size_t length);
	CMD5(const string& str);
	CMD5(ifstream& in);
	void append(const void* input, size_t length);
	void append(const string& str);
	void append(ifstream& in);
	const byte* digest();
	string toString();
	void reset();

private:
	void append(const byte* input, size_t length);
	void final();
	void transform(const byte block[64]);
	void encode(const uint32* input, byte* output, size_t length);
	void decode(const byte* input, uint32* output, size_t length);
	string bytesToHexString(const byte* input, size_t length);

	/* class uncopyable */
	CMD5(const CMD5&);
	CMD5& operator=(const CMD5&);

private:
	uint32 _state[4];	/* state (ABCD) */
	uint32 _count[2];	/* number of bits, modulo 2^64 (low-order word first) */
	byte _buffer[64];	/* input buffer */
	byte _digest[16];	/* message digest */
	bool _finished;		/* calculate finished ? */

	static const byte PADDING[64];	/* padding for calculate */
	static const char HEX[16];
	enum { BUFFER_SIZE = 1024 };
};

#endif /*__MD5_H__*/
