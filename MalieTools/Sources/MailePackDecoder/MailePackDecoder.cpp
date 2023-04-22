#include <iostream>
#include <fstream>

#include "../../Modules/MalieTools/Decoder.h"
#include "../../Modules/MalieTools/LIBP_Editor.h"

int main()
{
	//MalieTools::Decoder::Pack_Decodeer decoder(1, MalieTools::MalieEncryption::Key::Silverio_Ragnarok);
	//decoder.DecodePack(L"data3.dat", 0x10000);

	MalieTools::LIBP::Editor editor(L"data3.dat");
	editor.LoadPackIndex();
}

