#include <AEON/Runtime/AEModule.h>

AEModule::AEModule()
{

}

AEModule::~AEModule()
{

}

std::string AEModule::getName()
{
	return name;
}

uint32_t AEModule::storeString(std::string s)
{
	uint32_t index = 0;
	for (auto str : stringPool)
	{
		if (str == s)
			return index;

		index++;
	}

	stringPool.push_back(s);
	return index;
}

std::string AEModule::getStringFromPool(uint32_t index)
{
	return stringPool[index];
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
			int ic = instructions.size();
			fwrite(&ic, sizeof(ic), 1, fp);
			fwrite(&instructions[0], sizeof(AEInstruction), instructions.size(), fp);
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
			instructions.resize(ic);
			fread(&instructions[0], sizeof(AEInstruction), ic, fp);
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
		for (auto i : instructions)
		{
			uint32_t opcode = getopcode(i);
			//Log("op %d", opcode);
		}
	}

int AEModule::getFunctionIndexByName(const std::string& name)
{
		for (std::size_t i = 0; i < functions.size(); ++i)
		{
			if (functions[i].m_absoluteName == name)
			{
				return i;
			}
		}

		return -1;
}