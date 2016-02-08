#include "aeon_context.h"
#include "aeon_object.h"
#include "aeon_vm.h"
#include "aeon_parser.h"
#include "aeon_lexer.h"
#include "aeon_compiler.h"
#include "aeon_stdlib.h"
#include "aeon.h"

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

aeon_context::aeon_context()
{
	m_config.allowNativeInheritance = true;
	m_config.allowMultipleInheritance = false;

	register_primitive("void", 0);
	register_primitive("bool", sizeof(int32_t));
	register_primitive("int32", sizeof(int32_t));
	register_primitive("uint32", sizeof(uint32_t));
	register_primitive("float", sizeof(float));
	register_primitive("double", sizeof(double));
}

void aeon_context::register_primitive(const std::string& name, uint32_t size)
{
	aeType* type = new aeType(name, size);
	type->m_name = name;
	typedb.push_back(type);
}

aeLiteralId aeon_context::getIntegerLiteral(int64_t n)
{
	for (std::size_t i = 0; i < int_literals.size(); ++i)
	{
		if (int_literals[i] == n)
			return i;
	}

	int_literals.push_back(n);
	return int_literals.size() - 1;
}

aeLiteralId aeon_context::getStringLiteral(const std::string& s)
{
	for (std::size_t i = 0; i < string_literals.size(); ++i)
	{
		if (string_literals[i] == s)
			return i;
	}

	string_literals.push_back(s);
	return string_literals.size() - 1;
}

aeLiteralId aeon_context::getFloatLiteral(float v)
{
	for (std::size_t i = 0; i < m_floatTable.size(); ++i)
	{
		if (m_floatTable[i] == v)
			return i;
	}

	m_floatTable.push_back(v);
	return m_floatTable.size() - 1;
}

aeon_value aeon_context::evaluate(const std::string& expression)
{
	aeon_value value;
	value.mRawValue = "null";
	return value;
}

void aeon_context::init_all()
{
	register_stdlib(this);
}

void aeon_context::quick_build(const std::string& file)
{
	aeon_lexer lexer;
	aeon_parser parser;
	aeon_compiler compiler;
	std::string source = getFileSource(file);

	lexer.tokenize(source);

	parser.ctx = this;
	parser.startParse(lexer);

	aeNodeModule* tree_root = parser.root;
	tree_root->printSelf(0);

	compiler.m_env = this;
	compiler.m_module = create_module("main");
	compiler.generate(parser.root);
}

aeon_module* aeon_context::getModule(const std::string name)
{
	for (auto& mod : modules)
	{
		if (mod->name == name)
			return mod.get();
	}
	return nullptr;
}

aeon_object* aeon_context::createObject(const std::string& typen)
{
	for (auto& heap : object_heaps)
	{
		if (heap.type->m_name == typen)
		{
			aeon_object* object = new aeon_object();
			object->addr = malloc(heap.type->getSize());
			object->m_type = heap.type;
			return object;
		}
	}

	printf("Cannot create objects of a unknown type\n");
	return nullptr;
}

void aeon_context::destroyObject(aeon_object* object)
{
	free(object->addr);
	for (auto& heap : object_heaps)
	{
		if (heap.type == object->m_type)
		{
			delete object;
			
		}
	}
}

aeFunction* aeon_context::createFunction(const std::string& name)
{
	aeFunction* fn = getFunctionByName(name);
	if (!fn)
	{
		fn = new aeFunction();
		fn->m_absoluteName = name;
		m_functionTable.push_back(fn);
	}
	return fn;
}

aeFunction* aeon_context::getFunctionByIndex(uint32_t index)
{
	return m_functionTable[index];
}

aeFunction* aeon_context::getFunctionByName(const std::string& name)
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

uint32_t aeon_context::getFunctionIndexByName(const std::string& name)
{
	for (uint32_t i = 0; i < m_functionTable.size(); ++i)
	{
		if (m_functionTable[i]->m_absoluteName == name)
			return i;
	}
	return -1;
}

aeon_module* aeon_context::create_module(const std::string& name)
{
	if (getModule(name))
		return getModule(name);


	aeon_module* mod = new aeon_module;
	mod->name = name;
	modules.push_back(std::move(std::unique_ptr<aeon_module>(mod)));
	return mod;
}

void aeon_context::registerType(const std::string& name, std::size_t size, const std::string& namespc)
{
	aeType* objinfo = new aeType(name, size);
	objinfo->m_id = typedb.size() + 1;
	objinfo->is_native = true;
	typedb.push_back(objinfo);
}

void aeon_context::registerTypeMethod(const std::string& typeName, const std::string& name, aeBindMethod method)
{
	auto typeInfo = getTypeInfo(typeName);

	aeType::MethodInfo info;
	info.methodCallback = method;
	info.name = name;
	typeInfo->m_methods.push_back(info);

	aeFunction* ncall = new aeFunction;
	ncall->decl = name;
	ncall->fn = method;
	ncall->m_native = true;
	m_functionTable.push_back(ncall);
}

void aeon_context::registerFunction(const std::string& decl, aeBindMethod func)
{
	aeon_lexer lex; lex.tokenize(decl);
	aeon_parser parser; parser.lex = &lex; parser.i = 0; parser.ctx = this; parser.getNextToken();

	aeFunction* nf = new aeFunction;
	nf->fn = func;
	nf->decl = decl;
	nf->m_native = true;
	nf->returnType = parser.parseQualType();
	nf->m_absoluteName = parser.Tok.text;
	parser.getNextToken(); parser.getNextToken();
	while (parser.Tok.text != ")")
	{
		aeQualType paramType = parser.parseQualType();
		nf->params.push_back(paramType);
		printf("param %s\n", paramType.str().c_str());
		if (parser.getNextToken().text != ",")
			break;
	}

	m_functionTable.push_back(nf);

	printf("EXPORTED %s: returns %s\n", nf->m_absoluteName.c_str(), nf->returnType.str().c_str());
}

void aeon_context::registerTypeBehavior(const std::string& typeName, const std::string& behavName, aeBindMethod constructor)
{
	auto typeInfo = getTypeInfo(typeName);
	aeType::MethodInfo info;
	//info.constructorCallback = constructor;
	info.name = "constructor";
	typeInfo->m_methods.push_back(info);

	aeFunction* ncall = new aeFunction;
	ncall->decl = behavName;
	ncall->fn = constructor;
	ncall->m_native = true;
	m_functionTable.push_back(ncall);
}

void aeon_context::registerTypeDestructor(const std::string& typeName, aeDestructorMethod dest)
{
	auto typeInfo = getTypeInfo(typeName);
	typeInfo->m_destructor = dest;
}

void aeon_context::registerTypeField(const std::string& typeName, const std::string& decl, int offset)
{
	auto typeInfo = getTypeInfo(typeName);
	aeType::FieldInfo info;
	info.name = decl;
	info.offset = offset;
	typeInfo->m_fields.push_back(info);
}

aeFunctionId aeon_context::getNativeBehaviorIndex(const std::string& typeName, const std::string& behavior)
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

aeType* aeon_context::getTypeInfo(const std::string& name)
{
	for (auto& obj : typedb)
	{
		if (obj->m_name == name)
			return obj;
	}

	return nullptr;
}