#include <iostream>

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		unsigned long aKey[56] = { 0 };

		FILE* fp = nullptr;
		errno_t err = fopen_s(&fp, argv[1], "rb");
		if (err || !fp) return 0;

		fread(aKey, 1, 0xE0, fp);

		for (size_t iteLine = 0; iteLine < 14; iteLine++)
		{
			for (size_t iteEle = 0; iteEle < 4; iteEle++)
			{
				printf("0x%08X", aKey[(13 - iteLine) * 4 + iteEle]);
				if (!(iteLine == 13 && iteEle == 3)) putchar(',');
			}
			putchar('\n');
		}
	}
	else
	{
		printf("KeyFormat.exe key.bin");
	}

	system("pause");
}