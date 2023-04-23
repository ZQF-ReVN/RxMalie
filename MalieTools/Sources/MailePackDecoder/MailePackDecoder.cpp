#include <iostream>
#include <fstream>

#include "../../Modules/MalieTools/Decoder.h"
#include "../../Modules/MalieTools/LIBP.h"

int main()
{
	//MalieTools::Pack::Decoder decoder(1, MalieTools::MalieDecryption::Key::Silverio_Vendetta_Verse_of_Orpheus);
	//decoder.DecodePack(L"data2.dat", 0x10000);

	MalieTools::Pack::LIBP editor(L"data3.dat");
	editor.LoadPackIndex();
}

