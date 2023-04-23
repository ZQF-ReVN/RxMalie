#include "LIBP.h"
#include "Decoder.h"
#include "MalieDecryption.h"

namespace MalieTools
{
	namespace Pack
	{
		bool LIBP::LoadPackIndex()
		{
			Pack::Decoder decoder(1, MalieDecryption::Key::Silverio_Ragnarok);
			if (!decoder.OpenPack(m_wsPack)) { return false; }

			LIBP_Header header = { 0 };
			memcpy((void*)&header, decoder.DecodeBuffer(0, sizeof(LIBP_Header)), sizeof(LIBP_Header));

			LIBP_EleEntry* p_ele_entry = nullptr;
			uint8_t* p_ele_index = decoder.DecodeBuffer(sizeof(LIBP_Header), header.uiEleEntryCount * sizeof(LIBP_EleEntry));
			for (size_t iteEntry = 0; iteEntry < header.uiEleEntryCount; iteEntry++)
			{
				p_ele_entry = (LIBP_EleEntry*)(p_ele_index + iteEntry * sizeof(LIBP_EleEntry));
				m_vecEleIndex.emplace_back(*p_ele_entry);
			}

			LIBP_ResEntry* p_res_entry = nullptr;
			uint8_t* p_res_index = decoder.DecodeBuffer(sizeof(LIBP_Header) + header.uiEleEntryCount * sizeof(LIBP_EleEntry), header.uiResEntryCount * sizeof(LIBP_EleEntry));
			for (size_t iteEntry = 0; iteEntry < header.uiResEntryCount; iteEntry++)
			{
				p_res_entry = (LIBP_ResEntry*)(p_res_index + iteEntry * sizeof(LIBP_ResEntry));
				m_vecResIndex.emplace_back(*p_res_entry);
			}

			return false;
		}

		bool LIBP::LoadCustIndex()
		{
			return false;
		}
	}
}