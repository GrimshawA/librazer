#include <razer/runtime/RzEngine.h>
#include <razer/utils/FileUtils.h>
#include <razer/runtime/AEObject.h>
#include <razer/vm/VirtualMachine.h>
#include <razer/frontend/Parser/RzParser.h>
#include <razer/frontend/Parser/TokenParser.h>
#include <razer/frontend/compiler/RzCompiler.h>
#include <razer/utils/Logger.h>
#include <razer/vm/vm_api.h>

#include <RzSDK/ExportStd.h>
#include <cstring>

int RzEngine::exec(const std::string& filename, int argc, char** argv)
{
	return 0;
}

RzEngine::RzEngine()
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
	registerPrimitive("var", sizeof(RzValue));

	registerTypedef("int32", "int");
}

RzEngine::~RzEngine()
{

}

void RzEngine::setDebugger(IDebugger* dbg)
{
	_dbg = dbg;
}

void RzEngine::registerPrimitive(const std::string& name, uint32_t size)
{
    auto mod = getModule("__");
    if (!mod)
        mod = createModule("__");

    RzType* type = new RzType(*mod, name, size);
	type->m_name = name;
	type->m_absoluteName = name;
    type->setFlag(RzType::FLAG_PRIMITIVE);
	typedb.push_back(type);
}

aeLiteralId RzEngine::getIntegerLiteral(int64_t n)
{
	for (std::size_t i = 0; i < int_literals.size(); ++i)
	{
		if (int_literals[i] == n)
			return i;
	}

	int_literals.push_back(n);
	return int_literals.size() - 1;
}

aeLiteralId RzEngine::getStringLiteral(const std::string& s)
{
	for (std::size_t i = 0; i < string_literals.size(); ++i)
	{
		if (string_literals[i] == s)
			return i;
	}

	string_literals.push_back(s);
	return string_literals.size() - 1;
}

aeLiteralId RzEngine::getFloatLiteral(float v)
{
	for (std::size_t i = 0; i < m_floatTable.size(); ++i)
	{
		if (m_floatTable[i] == v)
			return i;
	}

	m_floatTable.push_back(v);
	return m_floatTable.size() - 1;
}

RzValue RzEngine::evaluate(const std::string& expression)
{
	RzValue value;
//	value.mRawValue = "null";
	return value;
}

void RzEngine::quick_build(const std::string& file)
{
	RzTokenParser lexer;
	RzParser parser;
	RzCompiler compiler;
	std::string source = getFileSource(file);

	if (source.empty())
		return;

	lexer.tokenize(source);

	parser.ctx = this;
	parser.startParse(lexer);

	RzSourceUnit* tree_root = parser.root;
	//tree_root->printSelf(0);

	RzModule* tempModule = createModule("tmp");
	tempModule->createDependency(getModule("std"));

	compiler.m_env = this;
	compiler.m_module = tempModule;
	compiler.generate(parser.root);
}

RzValue RzEngine::readValue(const std::string& filename)
{
	RzTokenParser lexer;
	RzParser parser;
	std::string src = getFileSource(filename);

	if (src.empty()){
		return RzValue();
	}
	
	lexer.tokenize(src);
	parser.i = 0;
	parser.m_tokenizer = &lexer;
	parser.getNextToken();
	return parser.parseDataValue();
}

RzModule* RzEngine::getModule(const std::string name)
{
	for (auto& mod : modules)
	{
		if (mod->m_name == name)
			return mod.get();
	}
	return nullptr;
}

AEObject* RzEngine::createObject(const std::string& typen)
{
	for (auto& heap : object_heaps)
	{
		if (heap.type->m_name == typen)
		{
			AEObject* object = new AEObject();
			object->m_obj = malloc(heap.type->getSize());
            memset(object->m_obj, 0, heap.type->getSize());
			object->m_type = heap.type;

            RZLOG("> createObject Allocated object %s of size %d at address %x\n", typen.c_str(), heap.type->getSize(), object->m_obj);

            // Running constructor
            RzVirtualMachine vm(*this);
            std::string constructorName = std::string(typen + "." + typen);


            auto* cx = &vm.m_mainContext;
            auto* func = getModule("test")->getFunction(constructorName); // to test IR

            // No constructor for this type
            if (!func)
                return object;

            RzStackValue thisVal;
            thisVal.ptr = object->m_obj;
            cx->push_value(thisVal);

            RzCall(cx, func);

			return object;
		}
	}

	RZLOG("Cannot create objects of a unknown type\n");
	return nullptr;
}

void RzEngine::destroyObject(AEObject* object)
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

RzModule* RzEngine::createModule(const std::string& name)
{
	if (getModule(name))
		return getModule(name);

	RzModule* mod = new RzModule;
	mod->m_name = name;
	mod->m_context = this;
	modules.push_back(std::move(std::unique_ptr<RzModule>(mod)));
    mod->m_moduleIndex = modules.size() - 1;
	return mod;
}

RzModule* RzEngine::resolveModule(const std::string& name)
{
	// name is a symbol identifier in the form x.y.z

	for (auto& m : modules)
	{
		std::string modName = m->getName();
		if (modName == name.substr(0, modName.size()))
		{
			return m.get();
		}
	}

	return nullptr;
}

int RzEngine::getModuleIndex(RzModule* module)
{
    for (int i = 0; i < modules.size(); ++i)
    {
        if (modules[i].get() == module)
            return i;
    }

    return -1;
}

void RzEngine::registerTypeField(const std::string& typeName, const std::string& decl, int offset) {
	auto typeInfo = getTypeInfo(typeName);
	aeField info;
	info.name = decl;
	info.offset = offset;
	typeInfo->m_fields.push_back(info);
}

void RzEngine::registerTypedef(const std::string& from, const std::string& to)
{
	aeTypedef tdef;
	tdef.from = from;
	tdef.to = to;
	m_typedefs.push_back(tdef);
}

void RzEngine::registerEnum(const std::string& enumName)
{
    //aeEnum* enumDef = new aeEnum;
//	enumDef->m_absoluteName = enumName;
//	enumDef->m_name = enumName;
//	m_enums.push_back(enumDef);
//	typedb.push_back(enumDef);
}

void RzEngine::registerEnumValue(const std::string& enumName, const std::string& valueName, int value)
{
	aeEnum* enumDef = getEnumByName(enumName);
	if (enumDef)
	{
		enumDef->table[valueName] = value;
	}
}

RzFunction* RzEngine::getFunctionByName(const std::string& fullyQualifiedName) const {
    for (std::size_t i = 0; i < modules.size(); ++i) {
        RzFunction* f = modules[i]->getFunction(fullyQualifiedName);
        if (f)
            return f;
    }
    return nullptr;
}

aeEnum* RzEngine::getEnumByName(const std::string& name)
{
//	for (auto& e : m_enums)
//	{
//		if (e->m_absoluteName == name)
//			return e;
//	}
	return nullptr;
}

RzType* RzEngine::getTypeInfo(const std::string& name)
{
	for (auto& obj : typedb)
	{
		//RZLOG("%s = %s\n", name.c_str(), obj->getSymbolName().c_str());
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

	// Search the modules loaded
	for (auto& mod : modules)
	{
		auto ti = mod->getType(name);
		if (ti)
			return ti;
	}

	return nullptr;
}

int32_t RzEngine::getTypeInfoIndex(RzType* typeInfo)
{
	for (auto i = 0; i < typedb.size(); ++i)
	{
		//RZLOG("%s = %s\n", name.c_str(), obj->getSymbolName().c_str());
		if (typedb[i] == typeInfo)
			return i;
	}

	return -1;
}

uint64_t RzEngine::getTypeModuleIndex(RzType* type)
{
	for (int i = 0; i < modules.size(); ++i)
	{
		if (modules[i]->hasType(type))
			return i;
	}

	return 1295790127591;
}
