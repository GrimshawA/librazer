#include "aeon_module.h"

uint32_t aeon_module::storeString(std::string s)
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

std::string aeon_module::getStringFromPool(uint32_t index)
{
	return stringPool[index];
}

bool aeon_module::hasFunction(const std::string& name)
{
	return false;
}

void aeon_module::write(const char* filename)
{
		FILE* fp = fopen(filename, "wb");
		if (fp)
		{
			int ic = instructions.size();
			fwrite(&ic, sizeof(ic), 1, fp);
			fwrite(&instructions[0], sizeof(aeon_instruction), instructions.size(), fp);
			fclose(fp);
		}
}

	void aeon_module::read(const char* filename)
	{
		FILE* fp = fopen(filename, "rb");
		if (fp)
		{
			int ic;
			fread(&ic, sizeof(ic), 1, fp);
			instructions.resize(ic);
			fread(&instructions[0], sizeof(aeon_instruction), ic, fp);
			fclose(fp);
		}
	}

	/// Write the program source to a human readable format ( hopefully )
	void aeon_module::dumpToFile(const std::string& filename)
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

	void aeon_module::debugCode()
	{
		for (auto i : instructions)
		{
			uint32_t opcode = getopcode(i);
			//Log("op %d", opcode);
		}
	}

int aeon_module::getFunctionIndexByName(const std::string& name)
{
		for (std::size_t i = 0; i < functions.size(); ++i)
		{
			if (functions[i].prototype == name)
			{
				return i;
			}
		}

		return -1;
}