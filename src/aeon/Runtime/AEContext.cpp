#include <AEON/AEContext.h>
#include <AEON/Runtime/AEObject.h>
#include <AEON/VM/AEVm.h>
#include <AEON/aeParser.h>
#include <AEON/aeTokenizer.h>
#include <AEON/Compiler/aeCompiler.h>

#include <AEON/STD/aeStdIo.h>

std::string getFileSource(const std::string& filename)
{
	FILE* fp = fopen(filename.c_str(), "r");
	std::string s;
	if (fp)
	{
		while (!feof(fp))
		{
			s += fgetc(fp);
		}
	}
	return s;
}

int AEContext::exec(const std::string& filename)
{
	return 0;
}

AEContext::AEContext()
{
	m_config.allowNativeInheritance = true;
	m_config.allowMultipleInheritance = false;

	registerPrimitive("void", 0);
	registerPrimitive("bool", sizeof(int32_t));
	registerPrimitive("uint8", sizeof(uint8_t));
	registerPrimitive("int8", sizeof(int8_t));
	registerPrimitive("uint16", sizeof(uint16_t));
	registerPrimitive("int16", sizeof(int16_t));
	registerPrimitive("uint32", sizeof(uint32_t));
	registerPrimitive("int32", sizeof(int32_t));
	registerPrimitive("uint64", sizeof(uint64_t));
	registerPrimitive("int64", sizeof(int64_t));
	registerPrimitive("float", sizeof(float));
	registerPrimitive("double", sizeof(double));
	registerPrimitive("string", sizeof(int32_t));
	registerPrimitive("var", sizeof(AEValue));

	registerTypedef("int32", "int");
}

void AEContext::registerPrimitive(const std::string& name, uint32_t size)
{
	AEType* type = new AEType(name, size);
	type->m_name = name;
	type->m_absoluteName = name;
	typedb.push_back(type);
}

aeLiteralId AEContext::getIntegerLiteral(int64_t n)
{
	for (std::size_t i = 0; i < int_literals.size(); ++i)
	{
		if (int_literals[i] == n)
			return i;
	}

	int_literals.push_back(n);
	return int_literals.size() - 1;
}

aeLiteralId AEContext::getStringLiteral(const std::string& s)
{
	for (std::size_t i = 0; i < string_literals.size(); ++i)
	{
		if (string_literals[i] == s)
			return i;
	}

	string_literals.push_back(s);
	return string_literals.size() - 1;
}

aeLiteralId AEContext::getFloatLiteral(float v)
{
	for (std::size_t i = 0; i < m_floatTable.size(); ++i)
	{
		if (m_floatTable[i] == v)
			return i;
	}

	m_floatTable.push_back(v);
	return m_floatTable.size() - 1;
}

AEValue AEContext::evaluate(const std::string& expression)
{
	AEValue value;
//	value.mRawValue = "null";
	return value;
}

void AEContext::init_all()
{
	register_stdlib(this);
}

void AEContext::quick_build(const std::string& file)
{
	aeon_lexer lexer;
	aeParser parser;
	AECompiler compiler;
	std::string source = getFileSource(file);

	if (source.empty())
		return;

	lexer.tokenize(source);

	parser.ctx = this;
	parser.startParse(lexer);

	aeNodeModule* tree_root = parser.root;
	//tree_root->printSelf(0);

	compiler.m_env = this;
	compiler.m_module = createModule("main");
	compiler.generate(parser.root);
}

AEValue AEContext::readValue(const std::string& filename)
{
	aeon_lexer lexer;
	aeParser parser;
	std::string src = getFileSource(filename);
	
	lexer.tokenize(src);
	parser.i = 0;
	parser.m_tokenizer = &lexer;
	parser.getNextToken();
	return parser.parseDataValue();
}

AEModule* AEContext::getModule(const std::string name)
{
	for (auto& mod : modules)
	{
		if (mod->m_name == name)
			return mod.get();
	}
	return nullptr;
}

AEObject* AEContext::createObject(const std::string& typen)
{
	for (auto& heap : object_heaps)
	{
		if (heap.type->m_name == typen)
		{
			AEObject* object = new AEObject();
			object->m_obj = malloc(heap.type->getSize());
			object->m_type = heap.type;
			AEVirtualMachine vm(this);
			vm.callMethod(object, typen + "." + typen);
			return object;
		}
	}

	printf("Cannot create objects of a unknown type\n");
	return nullptr;
}

void AEContext::destroyObject(AEObject* object)
{
	free(object->m_obj);
	for (auto& heap : object_heaps)
	{
		if (heap.type == object->m_type)
		{
			delete object;
			
		}
	}
}

AEFunction* AEContext::createFunction(const std::string& name)
{
	AEFunction* fn = getFunctionByName(name);
	if (!fn)
	{
		fn = new AEFunction();
		fn->m_absoluteName = name;
		m_functionTable.push_back(fn);
	}
	return fn;
}

AEFunction* AEContext::getFunctionByIndex(uint32_t index)
{
	return m_functionTable[index];
}

AEFunction* AEContext::getFunctionByName(const std::string& name)
{
	for (auto fn : m_functionTable)
	{
		//printf("comp %s %s\n", name.c_str(), fn->getSymbolName().c_str());
		if (fn->getSymbolName() == name)
		{
			return fn;
		}
	}
	return nullptr;
}

uint32_t AEContext::getFunctionIndexByName(const std::string& name)
{
	for (uint32_t i = 0; i < m_functionTable.size(); ++i)
	{
		if (m_functionTable[i]->m_absoluteName == name)
			return i;
	}
	return -1;
}

AEModule* AEContext::createModule(const std::string& name)
{
	if (getModule(name))
		return getModule(name);


	AEModule* mod = new AEModule;
	mod->m_name = name;
	mod->m_context = this;
	modules.push_back(std::move(std::unique_ptr<AEModule>(mod)));
	return mod;
}

void AEContext::registerType(const std::string& name, std::size_t size, const std::string& namespc)
{
	AEType* objinfo = new AEType(name, size);

	auto parser = aeParser::create(name, this);
	std::string canonicalName = parser->Tok.text;
	parser->getNextToken();
	if (parser->Tok.text == "<")
	{
		parser->getNextToken();
		while (parser->Tok.type == AETK_IDENTIFIER)
		{
			objinfo->m_templateParams.push_back(parser->Tok.text);
			parser->getNextToken(); if (parser->Tok.type == AETK_COMMA) parser->getNextToken();
		}
	}

	objinfo->m_id = typedb.size() + 1;
	objinfo->is_native = true;
	objinfo->m_absoluteName = canonicalName;
	typedb.push_back(objinfo);
}

void AEContext::registerTypeMethod(const std::string& typeName, const std::string& decl, aeBindMethod method)
{
	aeon_lexer lex; lex.tokenize(decl);
	aeParser parser; parser.m_tokenizer = &lex; parser.i = 0; parser.ctx = this; parser.getNextToken();

	auto typeInfo = getTypeInfo(typeName);

	AEType::MethodInfo info;
	info.methodCallback = method;
	info.name = decl;
	typeInfo->m_methods.push_back(info);

	AEFunction* fn = new AEFunction;
	fn->returnType = parser.parseQualType();
	fn->m_absoluteName = typeName + "." + parser.Tok.text;
	fn->decl = decl;
	fn->fn = method;
	fn->m_native = true;
	parser.getNextToken(); parser.getNextToken();
	while (parser.Tok.text != ")")
	{
		aeQualType paramType = parser.parseQualType();
		fn->params.push_back(paramType);
		//printf("param %s\n", paramType.str().c_str());
		if (parser.getNextToken().text != ",")
			break;
	}
	m_functionTable.push_back(fn);

	printf("EXPORTED %s: returns %s\n", fn->m_absoluteName.c_str(), fn->returnType.str().c_str());
}

void AEContext::registerFunction(const std::string& decl, aeBindMethod func)
{
	aeon_lexer lex; lex.tokenize(decl);
	aeParser parser; parser.m_tokenizer = &lex; parser.i = 0; parser.ctx = this; parser.getNextToken();

	AEFunction* fn = new AEFunction;
	fn->fn = func;
	fn->decl = decl;
	fn->m_native = true;
	fn->returnType = parser.parseQualType();
	fn->m_absoluteName = parser.Tok.text;
	parser.getNextToken(); parser.getNextToken();
	while (parser.Tok.text != ")")
	{
		aeQualType paramType = parser.parseQualType();
		fn->params.push_back(paramType);
		//printf("param %s\n", paramType.str().c_str());
		if (parser.getNextToken().text != ",")
			break;
	}

	m_functionTable.push_back(fn);

	//printf("EXPORTED %s: returns %s\n", fn->m_absoluteName.c_str(), fn->returnType.str().c_str());
}

void AEContext::registerTypeBehavior(const std::string& typeName, const std::string& behavName, aeBindMethod constructor)
{
	auto typeInfo = getTypeInfo(typeName);
	AEType::MethodInfo info;
	//info.constructorCallback = constructor;
	info.name = "constructor";
	typeInfo->m_methods.push_back(info);

	AEFunction* ncall = new AEFunction;
	ncall->decl = behavName;
	ncall->fn = constructor;
	ncall->m_native = true;
	m_functionTable.push_back(ncall);
}

void AEContext::registerTypeDestructor(const std::string& typeName, aeDestructorMethod dest)
{
	auto typeInfo = getTypeInfo(typeName);
	typeInfo->m_destructor = dest;
}

void AEContext::registerTypeField(const std::string& typeName, const std::string& decl, int offset)
{
	auto typeInfo = getTypeInfo(typeName);
	aeField info;
	info.name = decl;
	info.offset = offset;
	typeInfo->m_fields.push_back(info);
}

void AEContext::registerTypedef(const std::string& from, const std::string& to)
{
	aeTypedef tdef;
	tdef.from = from;
	tdef.to = to;
	m_typedefs.push_back(tdef);
}

void AEContext::registerEnum(const std::string& enumName)
{
	aeEnum* enumDef = new aeEnum;
	enumDef->m_absoluteName = enumName;
	enumDef->m_name = enumName;
	m_enums.push_back(enumDef);
	typedb.push_back(enumDef);
}

void AEContext::registerEnumValue(const std::string& enumName, const std::string& valueName, int value)
{
	aeEnum* enumDef = getEnumByName(enumName);
	if (enumDef)
	{
		enumDef->table[valueName] = value;
	}
}

aeEnum* AEContext::getEnumByName(const std::string& name)
{
	for (auto& e : m_enums)
	{
		if (e->m_absoluteName == name)
			return e;
	}
	return nullptr;
}

aeFunctionId AEContext::getNativeBehaviorIndex(const std::string& typeName, const std::string& behavior)
{
	for (std::size_t i = 0; i < m_functionTable.size(); ++i)
	{
		if (m_functionTable[i]->decl == behavior/* && m_functionTable[i]->typeName == typeName*/)
		{
			return i;
		}
	}

	return -1;
}

AEType* AEContext::getTypeInfo(const std::string& name)
{
	for (auto& obj : typedb)
	{
		//printf("%s = %s\n", name.c_str(), obj->getSymbolName().c_str());
		if (obj->m_absoluteName == name)
			return obj;
	}

	for (auto& td : m_typedefs)
	{
		if (td.to == name)
		{
			return getTypeInfo(td.from);
		}
	}

	return nullptr;
}

int32_t AEContext::getTypeInfoIndex(AEType* typeInfo)
{
	for (auto i = 0; i < typedb.size(); ++i)
	{
		//printf("%s = %s\n", name.c_str(), obj->getSymbolName().c_str());
		if (typedb[i] == typeInfo)
			return i;
	}

	return -1;
}