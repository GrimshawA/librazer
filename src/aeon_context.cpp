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
	aeon_type* type = new aeon_type(name, size);
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

void aeon_context::registerFunction(const char* proto, void* f)
{
/*	aeon_nativecall nf;
	nf.f = static_cast<void (*)(atom_generic*)>(f);
	nf.prototype = proto;
	native_functions.push_back(nf);*/
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

int32_t aeon_context::getNativeFunctionIndex(const std::string& name)
{
	for (std::size_t i = 0; i < native_functions.size(); ++i)
	{
		if (native_functions[i].prototype == name)
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
	aeon_type* objinfo = new aeon_type(name, size);
	objinfo->m_id = typedb.size() + 1;
	objinfo->is_native = true;
	typedb.push_back(objinfo);
}

void aeon_context::registerTypeMethod(const std::string& typeName, const std::string& name, aeBindMethod method)
{
	auto typeInfo = getTypeInfo(typeName);
	aeon_type::MethodInfo info;
	info.methodCallback = method;
	info.name = name;
	typeInfo->m_methods.push_back(info);

	aeon_nativecall ncall;
	ncall.prototype = name;
	ncall.fn = method;
	ncall.typeName = typeName;
	m_table_nmethods.push_back(ncall);
}

void aeon_context::registerTypeBehavior(const std::string& typeName, const std::string& behavName, aeBindMethod constructor)
{
	auto typeInfo = getTypeInfo(typeName);
	aeon_type::MethodInfo info;
	//info.constructorCallback = constructor;
	info.name = "constructor";
	typeInfo->m_methods.push_back(info);

	aeon_nativecall ncall;
	ncall.prototype = behavName;
	ncall.fn = constructor;
	ncall.typeName = typeName;
	m_table_nmethods.push_back(ncall);
}

void aeon_context::registerTypeDestructor(const std::string& typeName, aeDestructorMethod dest)
{
	auto typeInfo = getTypeInfo(typeName);
	typeInfo->m_destructor = dest;
}

void aeon_context::registerTypeField(const std::string& typeName, const std::string& decl, int offset)
{
	auto typeInfo = getTypeInfo(typeName);
	aeon_type::FieldInfo info;
	info.name = decl;
	info.offset = offset;
	typeInfo->m_fields.push_back(info);
}

aeFunctionId aeon_context::getNativeBehaviorIndex(const std::string& typeName, const std::string& behavior)
{
	for (std::size_t i = 0; i < m_table_nmethods.size(); ++i)
	{
		if (m_table_nmethods[i].prototype == behavior && m_table_nmethods[i].typeName == typeName)
		{
			return i;
		}
	}

	return -1;
}

aeon_type* aeon_context::getTypeInfo(const std::string& name)
{
	for (auto& obj : typedb)
	{
		if (obj->m_name == name)
			return obj;
	}

	return nullptr;
}