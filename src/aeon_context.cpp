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
	register_primitive("int", 0);
	register_primitive("int32", 0);
	register_primitive("float", 0);

	/*typedb.push_back(aeon_type("int8", sizeof(int8_t)));
	typedb.push_back(aeon_type("int16", sizeof(int16_t)));
	typedb.push_back(aeon_type("int32", sizeof(int32_t)));
	typedb.push_back(aeon_type("int64", sizeof(int64_t)));

	typedb.push_back(aeon_type("uint8", sizeof(uint8_t)));
	typedb.push_back(aeon_type("uint16", sizeof(uint16_t)));
	typedb.push_back(aeon_type("uint32", sizeof(uint32_t)));
	typedb.push_back(aeon_type("uint64", sizeof(uint64_t)));

	typedb.push_back(aeon_type("float", sizeof(float)));
	typedb.push_back(aeon_type("double", sizeof(double)));
	typedb.push_back(aeon_type("int", sizeof(int32_t)));
	typedb.push_back(aeon_type("char", sizeof(uint8_t)));
	typedb.push_back(aeon_type("byte", sizeof(uint8_t)));*/
}

void aeon_context::register_primitive(const std::string& name, uint32_t size)
{
	aeon_type* type = new aeon_type;
	type->m_name = name;
//	type->m_size = size;
	typedb.push_back(type);
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
	parser.parse(lexer);

	ast_module* tree_root = parser.root;
	tree_root->printSelf(0);

	compiler.env = this;
	compiler.mod = create_module("main");
	compiler.generate(parser.root);
}

void aeon_context::registerFunction(const char* proto, void* f)
{
	aeon_nativecall nf;
	nf.f = static_cast<void (*)(atom_generic*)>(f);
	nf.prototype = proto;
	native_functions.push_back(nf);
}

void aeon_context::register_method(const std::string& name, aeon_type* atype, void* funptr)
{

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

void aeon_context::register_type(const std::string& name, std::size_t size)
{
	aeon_type* objinfo = new aeon_type(name, size);
	objinfo->m_id = typedb.size() + 1;
	typedb.push_back(objinfo);
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