#include <AEON/AEModule.h>
#include <AEON/aeParser.h>
#include <AEON/aeTokenizer.h>

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

void AEModule::exportType(const std::string& name, int32_t size)
{

}

void AEModule::exportMethod(const std::string& name, const std::string& signature, aeBindMethod fn)
{

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

void AEModule::registerFunction(const std::string& sig, aeBindMethod fn)
{

}

void AEModule::registerGlobal(const std::string& sig, void* memory)
{

}

void AEModule::registerType(const std::string& name, std::size_t size)
{
	AEType* typeInfo = new AEType(name, size);
	auto parser = aeParser::create(name, this->m_context);
	std::string canonicalName = parser->Tok.text;
	parser->getNextToken();
	if (parser->Tok.text == "<")
	{
		parser->getNextToken();
		while (parser->Tok.type == AETK_IDENTIFIER)
		{
			typeInfo->m_templateParams.push_back(parser->Tok.text);
			parser->getNextToken(); if (parser->Tok.type == AETK_COMMA) parser->getNextToken();
		}
	}

	typeInfo->is_native = true;
	typeInfo->m_absoluteName = canonicalName;
	m_types.push_back(typeInfo);
}

void AEModule::registerTypeConstructor()
{

}

void AEModule::registerTypeDestructor()
{

}

void AEModule::registerMethod(const std::string& name, const std::string& sig, aeBindMethod fnPtr)
{
	aeon_lexer lex; lex.tokenize(sig);
	aeParser parser; parser.m_tokenizer = &lex; parser.i = 0; parser.ctx = m_context; parser.getNextToken();

	auto typeInfo = getType(name);

	AEType::MethodInfo info;
	info.methodCallback = fnPtr;
	info.name = sig;
	typeInfo->m_methods.push_back(info);

	AEFunction fn;
	fn.returnType = parser.parseQualType();
	fn.m_absoluteName = name + "." + parser.Tok.text;
	fn.decl = name;
	fn.fn = fnPtr;
	fn.m_native = true;
	parser.getNextToken(); parser.getNextToken();
	while (parser.Tok.text != ")")
	{
		aeQualType paramType = parser.parseQualType();
		fn.params.push_back(paramType);
		//printf("param %s\n", paramType.str().c_str());
		if (parser.getNextToken().text != ",")
			break;
	}
	m_functions.push_back(fn);

	printf("EXPORTED %s: returns %s\n", fn.m_absoluteName.c_str(), fn.returnType.str().c_str());
}

void AEModule::registerField()
{

}

void AEModule::registerPropertyGet()
{

}

void AEModule::registerPropertySet()
{

}

void AEModule::registerEnum()
{

}

void AEModule::registerEnumValue()
{

}

void AEModule::registerTypedef()
{

}

int AEModule::getDependencyId(const std::string& name)
{
	for (int i = 0; i < m_dependencies.size(); ++i)
	{
		if (m_dependencies[i].name == name)
			return i;
	}

	return -1;
}

int AEModule::getDependencyId(AEModule* module)
{
	for (int i = 0; i < m_dependencies.size(); ++i)
	{
		if (m_dependencies[i].module == module)
			return i;
	}

	return -1;
}

bool AEModule::depends(const std::string& name) const
{
	for (auto& dep : m_dependencies)
	{
		if (dep.name == name)
			return true;
	}

	return false;
}

bool AEModule::depends(AEModule* module) const
{
	for (auto& dep : m_dependencies)
	{
		if (dep.module == module)
			return true;
	}

	return false;
}

int AEModule::createDependency(const std::string& name)
{
	// This overload creates a unlinked dependency, potentially loaded later

	for (int i = 0; i < m_dependencies.size(); ++i)
	{
		if (m_dependencies[i].name == name)
		{
			return i;
		}
	}

	ModuleDependency dep;
	dep.module = nullptr;
	dep.name = name;
	m_dependencies.push_back(dep);

	return m_dependencies.size() - 1;
}

int AEModule::createDependency(AEModule* module)
{
	if (!module)
	{
		printf("Can't create dependencies to null modules\n");
		return -1;
	}

	for (int i = 0; i < m_dependencies.size(); ++i)
	{
		if (m_dependencies[i].name == module->getName())
		{
			return i;
		}
	}

	ModuleDependency dep;
	dep.module = module;
	dep.name = module->getName();
	m_dependencies.push_back(dep);

	return m_dependencies.size() - 1;
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
