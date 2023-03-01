#pragma once

#define UNICODE_ENTRIES

//#define LIBP_VERSION 1
#define LIBP_VERSION 2

struct LIBUHDR {
	unsigned char signature[4]; // "LIB" or "LIBU"
	unsigned long unknown1;
	unsigned long entry_count;
	unsigned long unknown2;
};

struct LIBUENTRY {
#ifdef UNICODE_ENTRIES
	wchar_t       filename[32 + 1];
#else
	char          filename[36];
#endif
	unsigned long length;
	unsigned long offset;   // from top of header
	unsigned long unknown2;
};

struct LIBPHDR {
	unsigned char signature[4]; // "LIBP"
	unsigned long entry1_count;
	unsigned long entry2_count;
	unsigned long unknown1;
};

struct LIBPENTRY1 {
	char          filename[20];
	unsigned long flags;
	unsigned long offset_index;
	unsigned long length;
};

struct LIBPENTRY2 {
	unsigned long offset; // in 4096 byte blocks (or 1024 byte blocks...)
};

