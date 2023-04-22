#include "LIBP_Editor.h"
#include "Decoder.h"
#include "MalieEncryption.h"

namespace MalieTools
{
	namespace LIBP
	{
		bool Editor::LoadPackIndex()
		{
			Decoder::Pack_Decodeer decoder(1, MalieEncryption::Key::Silverio_Ragnarok);
			if (!decoder.OpenPack(m_wsPack)) { return false; }

			Header header = { 0 };
			memcpy((void*)&header, decoder.DecodeBuffer(0, sizeof(Header)), sizeof(Header));

			EleEntry* p_ele_entry = nullptr;
			uint8_t* p_ele_index = decoder.DecodeBuffer(sizeof(Header), header.uiEleEntryCount * sizeof(EleEntry));
			for (size_t iteEntry = 0; iteEntry < header.uiEleEntryCount; iteEntry++)
			{
				p_ele_entry = (EleEntry*)(p_ele_index + iteEntry * sizeof(EleEntry));
				m_vecEleIndex.emplace_back(*p_ele_entry);
			}

			ResEntry* p_res_entry = nullptr;
			uint8_t* p_res_index = decoder.DecodeBuffer(sizeof(Header) + header.uiEleEntryCount * sizeof(EleEntry), header.uiResEntryCount * sizeof(EleEntry));
			for (size_t iteEntry = 0; iteEntry < header.uiResEntryCount; iteEntry++)
			{
				p_res_entry = (ResEntry*)(p_res_index + iteEntry * sizeof(ResEntry));
				m_vecResIndex.emplace_back(*p_res_entry);
			}

			return false;
		}

		bool Editor::LoadCustIndex()
		{
			return false;
		}
	}
}