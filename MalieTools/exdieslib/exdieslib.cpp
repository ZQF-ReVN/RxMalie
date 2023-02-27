// exdieslib.cpp, v1.81 2014/03/02
// coded by asmodean

// contact: 
//   web:   http://asmodean.reverse.net
//   email: asmodean [at] hush.com
//   irc:   asmodean on efnet (irc.efnet.net)

// This tool extracts encrypted *.lib archives used by various light games.

#include "as-util.h"
#include "camellia_decrypt128.h"
#include "exdieslib_struct.h"
#include "DecBlock_DIRA.h"

#include <direct.h>
#include <Windows.h>

#define DO_DECRYPT

static const unsigned long DECRYPT_BLOCK_LENGTH = 16;
void read_decrypt(FILE* fp, unsigned long key_index, void* buff, unsigned long len, unsigned long offset)
{
	if (key_index != -1) 
	{
		unsigned long  block_pad = offset % DECRYPT_BLOCK_LENGTH;
		unsigned long  aligned_len = (len + block_pad + 15) & ~15;
		unsigned char* aligned_buff = new unsigned char[aligned_len];

		unsigned char* block = (unsigned char*)aligned_buff;
		unsigned long  block_count = aligned_len / DECRYPT_BLOCK_LENGTH;

		fseek(fp, offset - block_pad, SEEK_SET);
		fread(aligned_buff, 0x1, aligned_len, fp);

		for (unsigned long i = 0; i < block_count; i++)
		{
			switch (key_index)
			{
			//恋音セ・ピアーチェ PKG
			case 90:
			{
				DecBlock_DIRA(offset, block, koisepi_Key);
			}
			break;

			//不運と幸運と恋占いのタロット PKG
			case 91:
			{
				DecBlock_DIRA(offset, block, fuun_Key);
			}
			break;

			//その花が咲いたら、また僕は君に出逢う PKG
			case 92:
			{
				DecBlock_DIRA(offset, block, kmh_Key);
			}
			break;

			//忘却執事と恋するお嬢様の回想録 PKG
			case 93:
			{
				DecBlock_DIRA(offset, block, koi_Key);
			}
			break;

			//初恋＊シンドローム DL
			case 94:
			{
				DecBlock_DIRA(offset, block, hat_Key);
			}
			break;

			default:
				camellia_decrypt128(key_index, block, offset);
			}

			block += DECRYPT_BLOCK_LENGTH;
			offset += DECRYPT_BLOCK_LENGTH;
		}

		memcpy(buff, aligned_buff + block_pad, len);

		delete[] aligned_buff;
	}
	else 
	{
		fseek(fp, offset, SEEK_SET);
		fread(buff, 0x1, len, fp);
	}
}

string convert_filename(wchar_t* s)
{
	char filename[4096] = { 0 };
	size_t i;
	wcstombs_s(&i, filename, 4096, s, sizeof(filename));
	return filename;
}

void extract(FILE* fd, unsigned long key_index, const string& prefix, unsigned long offset, unsigned long length, bool flatten)
{
	char dir_delim = flatten && prefix != "." ? '+' : '/';

	LIBUHDR hdr;
	read_decrypt(fd, key_index, &hdr, sizeof(hdr), offset);

	if (memcmp(hdr.signature, "LIB", 3) == 0) {
		LIBUENTRY* entries = new LIBUENTRY[hdr.entry_count];
		read_decrypt(fd, key_index, entries, sizeof(LIBUENTRY) * hdr.entry_count, offset + sizeof(hdr));

		if (!flatten) {
			int r = _mkdir(prefix.c_str());
		}

		for (unsigned long i = 0; i < hdr.entry_count; i++) {
#ifdef UNICODE_ENTRIES
			string filename = convert_filename(entries[i].filename);
#else
			string filename = entries[i].filename;
#endif

			extract(fd,
				key_index,
				prefix + dir_delim + filename,
				offset + entries[i].offset,
				entries[i].length,
				flatten);
		}

		delete[] entries;
	}
	else {
		unsigned char* buff = new unsigned char[length];
		read_decrypt(fd, key_index, buff, length, offset);

		bool is_mgf = length >= 7 && !memcmp(buff, "MalieGF\0", 8);

		string filename(prefix);
		if (is_mgf) {
			filename = as::get_file_prefix(prefix, false) + ".png";
			memcpy(buff, "\x89PNG\x0D\x0A\x1A\x0A", 8);
		}

		as::write_file(filename, buff, length);

		delete[] buff;
	}
}


class libp_t
{
public:
	libp_t(FILE* fd, unsigned long key_index, bool flatten)
		: fd(fd), key_index(key_index), flatten(flatten)
	{
		base_offset = 0;

		read_decrypt(fd, key_index, &hdr, sizeof(hdr), base_offset);
		base_offset += sizeof(hdr);

		entries1 = new LIBPENTRY1[hdr.entry1_count];
		read_decrypt(fd, key_index, entries1, sizeof(LIBPENTRY1) * hdr.entry1_count, base_offset);
		base_offset += sizeof(LIBPENTRY1) * hdr.entry1_count;

		entries2 = new LIBPENTRY2[hdr.entry2_count];
		read_decrypt(fd, key_index, entries2, sizeof(LIBPENTRY2) * hdr.entry2_count, base_offset);
		base_offset += sizeof(LIBPENTRY2) * hdr.entry2_count;

		// Was it really 4k-aligned before?  Maybe this was always a bug...
#if LIBP_VERSION >= 2
		base_offset = (base_offset + 1023) & ~1023;
#else
		base_offset = (base_offset + 4095) & ~4095;
#endif

		extract(".", 0, 1);
	}

	~libp_t(void) {
		delete[] entries1;
		delete[] entries2;
	}

private:
	void extract(const string& prefix,
		unsigned long entry1_offset,
		unsigned long entry_count)
	{
		char dir_delim = flatten && prefix != "." ? '+' : '/';

		if (!flatten) {
			int r = _mkdir(prefix.c_str());
		}

		LIBPENTRY1* curr = entries1 + entry1_offset;

		for (unsigned long i = 0; i < entry_count; i++) {
			string filename = prefix + dir_delim + curr[i].filename;

			if (curr[i].flags & LIBPENTRY_FLAG_FILE) {
				unsigned char* buff = new unsigned char[curr[i].length];
				read_decrypt(fd, key_index, buff, curr[i].length, base_offset + entries2[curr[i].offset_index].offset * 1024);

				as::write_file(filename, buff, curr[i].length);

				delete[] buff;
			}
			else {
				extract(filename, curr[i].offset_index, curr[i].length);
			}
		}
	}

	FILE*  fd;
	int  key_index;
	bool flatten;

	unsigned long base_offset;
	LIBPHDR       hdr;
	LIBPENTRY1* entries1;
	LIBPENTRY2* entries2;
};

bool PutConsole(const wchar_t* pString)
{
	static HANDLE hSTD_OUT = GetStdHandle(STD_OUTPUT_HANDLE);
	return WriteConsoleW(hSTD_OUT, pString, lstrlenW(pString), NULL, NULL);
}

int main(int argc, char* argv[])
{
	if (argc < 3) {
		PutConsole(L"exdieslib v1.81 by asmodean\n\n");
		PutConsole(L"Add new encryption support by Dir-A 2023.2.27\n\n");
		printf("usage: %s <input.lib|input.dat> <key_index> [-flatten]\n\n", argv[0]);
		PutConsole(L"\tkey_index = -1 -> no encryption\n");
		PutConsole(L"\tkey_index =  0 -> Dies irae\n");
		PutConsole(L"\tkey_index =  1 -> さかしき人にみるこころ\n");
		PutConsole(L"\tkey_index =  2 -> あるす☆まぐな！−ARS：MAGNA−\n");
		PutConsole(L"\tkey_index =  3 -> タペストリー tapestry -you will meet yourself-\n");
		PutConsole(L"\tkey_index =  4 -> どんちゃんがきゅ〜\n");
		PutConsole(L"\tkey_index =  5 -> Dies irae Also sprach Zarathustra -die Wiederkunft-\n");
		PutConsole(L"\t                  Dies irae 〜Acta est Fabula〜\n");
		PutConsole(L"\t                  Dies irae 〜Acta est Fabula〜 −Scharlachrot Grun−\n");
		PutConsole(L"\t                  Dies irae 〜Amantes amentes〜\n");
		PutConsole(L"\tkey_index =  6 -> Soranica Ele−ソラニカ エレ−\n");
		PutConsole(L"\tkey_index =  7 -> まじのコンプレックス DL版\n");
		PutConsole(L"\tkey_index =  8 -> PARADISE LOST 新装版\n");
		PutConsole(L"\tkey_index =  9 -> 神咒神威神楽 超先行 体験版\n");
		PutConsole(L"\tkey_index = 10 -> Vermilion -Bind of Blood-\n");
		PutConsole(L"\tkey_index = 11 -> オメルタ〜沈黙の掟〜\n");
		PutConsole(L"\tkey_index = 12 -> 神咒神威神楽\n");
		PutConsole(L"\tkey_index = 13 -> かみのゆ\n");
		PutConsole(L"\tkey_index = 14 -> Zero Infinity -Devil of Maxwell-\n");
		PutConsole(L"\tkey_index = 15 -> しろのぴかぴかお星さま (v2)\n");
		PutConsole(L"\tkey_index = 16 -> 神咒神威神楽 曙之光\n");
		PutConsole(L"\tkey_index = 17 -> BRAVA!! (v2)\n");
		PutConsole(L"\tkey_index = 18 -> Electro Arms -Realize Digital Dimension- (v2)\n");
		PutConsole(L"\tkey_index = 19 -> 相州戦神館學園 八命陣 (v2)\n");

		//Add By Dir-A
		PutConsole(L"\tkey_index = 90 -> 恋音セ・ピアーチェ PKG [New]\n");
		PutConsole(L"\tkey_index = 91 -> 不運と幸運と恋占いのタロット PKG [New]\n");
		PutConsole(L"\tkey_index = 92 -> その花が咲いたら、また僕は君に出逢う PKG [New]\n");
		PutConsole(L"\tkey_index = 93 -> 忘却執事と恋するお嬢様の回想録 PKG [New]\n");
		PutConsole(L"\tkey_index = 94 -> 初恋＊シンドローム DL [New]\n");

		return -1;
	}

	string in_filename(argv[1]);
	unsigned long key_index = atol(argv[2]);
	bool flatten = argc > 3 && !strcmp(argv[3], "-flatten");

	//int fd = as::open_or_die(in_filename, O_RDONLY | O_BINARY);
	FILE* fp = nullptr;
	errno_t err = fopen_s(&fp, in_filename.c_str(), "rb");
	if (!fp || err) return 0;

	LIBPHDR check;
	read_decrypt(fp, key_index, &check, sizeof(check), 0);

	if (!memcmp(check.signature, "LIBP", 4))
	{
		libp_t libp(fp, key_index, flatten);
	}
	else
	{
		extract(fp, key_index, ".", 0, 0, flatten);
	}

	fclose(fp);

	return 0;
}
