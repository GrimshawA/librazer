#include <AEON/Runtime/AEModule.h>

AEModule::AEModule()
{

}

AEModule::~AEModule()
{

}

AEFunction* AEModule::getFunction(const std::string& name)
{
	for (int i = 0; i < m_functions.size(); ++i)
	{
		if (m_functions[i].m_absoluteName == name)
		{
			return &m_functions[i];
		}
	}

	return nullptr;
}

AEFunction* AEModule::getFunction(FnIndex index)
{
	return &m_functions[index];
}

FnIndex AEModule::getFunctionIndex(const std::string& name)
{
	for (int i = 0; i < m_functions.size(); ++i)
	{
		if (m_functions[i].m_absoluteName == name)
		{
			return i;
		}
	}
	return -1;
}

uint64_t AEModule::getFunctionOffset(const std::string& name)
{
	return getFunctionOffset(getFunctionIndex(name));
}

uint64_t AEModule::getFunctionOffset(FnIndex functionIndex)
{
	return m_functions[functionIndex].m_offset;
}

int64_t AEModule::getTypeIndex(const std::string& name)
{
	for (int i = 0; i < m_types.size(); ++i)
	{
		if (m_types[i]->getSymbolName() == name)
			return i;
	}

	return -1;
}

int64_t AEModule::getTypeIndex(AEType* type)
{
	for (int i = 0; i < m_types.size(); ++i)
	{
		if (m_types[i] == type)
			return i;
	}

	return -1;
}

AEType* AEModule::getType(const std::string& name)
{
	for (int i = 0; i < m_types.size(); ++i)
	{
		if (m_types[i]->getSymbolName() == name)
			return m_types[i];
	}

	return nullptr;
}

AEType* AEModule::getType(int64_t index)
{
	return m_types[index];
}

std::string AEModule::getStringFromPool(uint32_t index)
{
	return m_stringPool[index];
}

double AEModule::getDoubleLiteral(uint32_t index)
{
	return m_doublePool[index];
}

uint64_t AEModule::getIntegerLiteral(uint32_t index)
{
	return m_intPool[index];
}

int AEModule::identifierPoolIndex(const std::string& identifier)
{
	for (int i = 0; i < m_identifierPool.size(); ++i)
	{
		if (m_identifierPool[i] == identifier)
			return i;
	}

	m_identifierPool.push_back(identifier);
	return m_identifierPool.size() - 1;
}

std::string AEModule::getName()
{
	return m_name;
}

uint32_t AEModule::storeString(std::string s)
{
	uint32_t index = 0;
	for (auto str : m_stringPool)
	{
		if (str == s)
			return index;

		index++;
	}

	m_stringPool.push_back(s);
	return index;
}

bool AEModule::hasFunction(const std::string& name)
{
	return false;
}

void AEModule::write(const char* filename)
{
		FILE* fp = fopen(filename, "wb");
		if (fp)
		{
			int ic = m_code.size();
			fwrite(&ic, sizeof(ic), 1, fp);
			fwrite(&m_code[0], sizeof(AEInstruction), m_code.size(), fp);
			fclose(fp);
		}
}

	void AEModule::read(const char* filename)
	{
		FILE* fp = fopen(filename, "rb");
		if (fp)
		{
			int ic;
			fread(&ic, sizeof(ic), 1, fp);
			m_code.resize(ic);
			fread(&m_code[0], sizeof(AEInstruction), ic, fp);
			fclose(fp);
		}
	}

	/// Write the program source to a human readable format ( hopefully )
	void AEModule::dumpToFile(const std::string& filename)
	{
		/*File outFile(filename, IODevice::TextWrite);
		if (outFile)
		{
			TextStream strm(outFile);

			for (int pc = 0; pc < instructions.size(); ++pc)
			{
				for (std::size_t i = 0; i < functions.size(); ++i)
				{
					if (pc == functions[i].offset)
					{
						fprintf(outFile.getHandle(), "\n");
						fprintf(outFile.getHandle(), "%s:\n", functions[i].prototype.c_str());
					}
				}

				fprintf(outFile.getHandle(), "%s\n", getopname(getopcode(instructions[pc])));
			}
		}*/
	}

	void AEModule::debugCode()
	{
		for (auto i : m_code)
		{
			uint32_t opcode = getopcode(i);
			//Log("op %d", opcode);
		}
	}

int AEModule::getFunctionIndexByName(const std::string& name)
{
		for (std::size_t i = 0; i < m_functions.size(); ++i)
		{
			if (m_functions[i].m_absoluteName == name)
			{
				return i;
			}
		}

		return -1;
}