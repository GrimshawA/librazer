#include <razer/runtime/Module.h>
#include <razer/runtime/RzEngine.h>
#include <razer/frontend/Parser/RzParser.h>
#include <razer/frontend/Parser/TokenParser.h>
#include <razer/utils/Logger.h>

RzModule::RzModule() {
	// Starts with one invalid dependency which is reserved for self dependencies
	m_dependencies.resize(1);
}

RzModule::~RzModule() {

}

int RzModule::index() {
	return m_moduleIndex;
}

RzFunction* RzModule::createFunction(const std::string& name) {
	m_functions.push_back(RzFunction());
	//m_functions.back().name = name;
    auto* func = &m_functions.back();

    func->m_absoluteName = name;
    return func;
}

RzFunction* RzModule::getFunction(const std::string& name) {
	for (int i = 0; i < m_functions.size(); ++i) {
		if (m_functions[i].m_absoluteName == name) {
			return &m_functions[i];
		}
	}

	return nullptr;
}

RzFunction* RzModule::getFunction(FnIndex index) {
	return &m_functions[index];
}

FnIndex RzModule::getFunctionIndex(const std::string& name) {
	for (int i = 0; i < m_functions.size(); ++i) {
		if (m_functions[i].m_absoluteName == name) {
			return i;
		}
	}
	return -1;
}

FnIndex RzModule::getFunctionIndex(RzFunction* func)
{
    for (int i = 0; i < m_functions.size(); ++i)
    {
        if (&m_functions[i] == func)
            return i;
    }
    return -1;
}

uint64_t RzModule::getFunctionOffset(const std::string& name) {
	return getFunctionOffset(getFunctionIndex(name));
}

uint64_t RzModule::getFunctionOffset(FnIndex functionIndex) {
	return m_functions[functionIndex].m_offset;
}

int64_t RzModule::getTypeIndex(const std::string& name) {
	for (int i = 0; i < m_types.size(); ++i) {
		if (m_types[i]->getSymbolName() == name)
			return i;
	}

	return -1;
}

int64_t RzModule::getTypeIndex(RzType* type) {
	for (int i = 0; i < m_types.size(); ++i) {
		if (m_types[i] == type)
			return i;
	}

	return -1;
}

RzType* RzModule::getType(const std::string& name) {
	for (int i = 0; i < m_types.size(); ++i) {
		if (m_types[i]->getSymbolName() == name)
			return m_types[i];
	}

	return nullptr;
}

RzType* RzModule::getType(int64_t index) {
	return m_types[index];
}

RzType* RzModule::resolveType(int dependency_id, int type_id) {
	if (dependency_id == 0)
		return getType(type_id);

	return m_dependencies[dependency_id].module->getType(type_id);
}

std::string RzModule::getStringFromPool(uint32_t index) {
	return m_stringPool[index];
}

double RzModule::getDoubleLiteral(uint32_t index) {
	return m_doublePool[index];
}

uint64_t RzModule::getIntegerLiteral(uint32_t index) {
	return m_intPool[index];
}

int RzModule::identifierPoolIndex(const std::string& identifier) {
	for (int i = 0; i < m_identifierPool.size(); ++i) {
		if (m_identifierPool[i] == identifier)
			return i;
	}

	m_identifierPool.push_back(identifier);
	return m_identifierPool.size() - 1;
}

void RzModule::registerFunction(const std::string& sig, aeBindMethod fnPtr) {
    RzTokenParser lex; lex.tokenize(sig);
    RzParser parser; parser.m_tokenizer = &lex; parser.i = 0; parser.ctx = m_context; parser.getNextToken();

    RzQualType retType = parser.parseQualType();
    std::string methodName = parser.Tok.text;

    AENativeFunctionWrapper wrapper;
    wrapper.f = fnPtr;
    m_nativeFunctions.push_back(wrapper);


    RzFunction fn;
    fn.returnType = retType;
    fn.m_name = methodName;
    fn.m_absoluteName = methodName;
    //fn.decl = name;
    fn.fn = fnPtr;
    fn.methodCallback = fnPtr;
    fn.m_native = true;
    fn.m_module = this;
    parser.getNextToken();

    while (parser.Tok.text != ")") {
        parser.getNextToken();

        if (parser.Tok.text == ")")
            break;

        RzQualType paramType = parser.parseQualType();
        fn.params.push_back(paramType);
        //printf("param %s\n", paramType.str().c_str());
        if (parser.Tok.text != ",")
            break;
    }
    m_functions.push_back(fn);

    //typeInfo->m_methods.push_back(&m_functions.back());

    RZLOG("EXPORTED FUNC %s: returns %s\n", fn.m_absoluteName.c_str(), fn.returnType.str().c_str());
}

void RzModule::registerGlobal(const std::string& sig, void* memory) {

}

RzType* RzModule::registerType(const std::string& name, std::size_t size) {
    if (getType(name))
        return nullptr;

    RzType* typeInfo = new RzType(*this, name, size);

	auto parser = RzParser::create(name, this->m_context);
	std::string canonicalName = parser->Tok.text;
	parser->getNextToken();
	if (parser->Tok.text == "<") {
		parser->getNextToken();
		while (parser->Tok.type == RZTK_IDENTIFIER) {
			typeInfo->m_templateParams.push_back(parser->Tok.text);
			parser->getNextToken(); if (parser->Tok.type == RZTK_COMMA) parser->getNextToken();
		}
	}

	typeInfo->is_native = true;
	typeInfo->m_absoluteName = canonicalName;
	m_types.push_back(typeInfo);

    return typeInfo;
}

void RzModule::registerTypeConstructor(const std::string& name, aeConstructorMethod constructor) {
	auto typeInfo = getType(name);
	if (!typeInfo) {
		RZLOG("No such type %s\n", name.c_str());
		return;
	}

    RzFunction* func = new RzFunction();
    func->constructorCallback = constructor;
    func->m_native = true;
    func->m_offset = -1;
    func->m_compiled = true;
    func->m_name = name;
    func->m_module = this;
    func->m_isconstructor = true;
    func->m_absoluteName = name + "." + name;
    typeInfo->m_methods.push_back(func);

    m_functions.push_back(*func);
}

void RzModule::registerTypeConstructor(const std::string& name, RzTemplateConstructorMethod constructor)
{
    auto typeInfo = getType(name);
    if (!typeInfo) {
        RZLOG("No such type %s\n", name.c_str());
        return;
    }

    typeInfo->is_templated = true;

    RzFunction* func = new RzFunction();
    func->m_isconstructor = true;
    func->templatedConstructor = constructor;
    func->m_native = true;
    func->m_offset = -1;
    func->m_compiled = true;
    func->m_module = this;
    func->m_absoluteName = name + "." + name;
    func->m_name = name;
    func->m_module = this;
    typeInfo->m_methods.push_back(func);

    m_functions.push_back(*func);
}

void RzModule::registerTypeDestructor()
{

}

void RzModule::registerMethod(const std::string& name, const std::string& sig, aeBindMethod fnPtr) {
	RzTokenParser lex; lex.tokenize(sig);
	RzParser parser; parser.m_tokenizer = &lex; parser.i = 0; parser.ctx = m_context; parser.getNextToken();

    RzQualType retType = parser.parseQualType();
	std::string methodName = parser.Tok.text;

	auto typeInfo = getType(name);

	AENativeFunctionWrapper wrapper;
	wrapper.f = fnPtr;
	m_nativeFunctions.push_back(wrapper);


	RzFunction fn;
	fn.returnType = retType;
    fn.m_name = methodName;
	fn.m_absoluteName = name + "." + methodName;
	fn.decl = name;
	fn.fn = fnPtr;
    fn.methodCallback = fnPtr;
	fn.m_native = true;
    fn.m_module = this;
	parser.getNextToken();

	while (parser.Tok.text != ")") {
		parser.getNextToken();

		if (parser.Tok.text == ")")
			break;

		RzQualType paramType = parser.parseQualType();
		fn.params.push_back(paramType);
		//printf("param %s\n", paramType.str().c_str());
		if (parser.Tok.text != ",")
			break;
	}
	m_functions.push_back(fn);

    typeInfo->m_methods.push_back(&m_functions.back());

	RZLOG("EXPORTED %s: returns %s\n", fn.m_absoluteName.c_str(), fn.returnType.str().c_str());
}

void RzModule::registerField() {

}

void RzModule::registerPropertyGet() {

}

void RzModule::registerPropertySet() {

}

void RzModule::registerEnum() {

}

void RzModule::registerEnumValue() {

}

void RzModule::registerTypedef() {

}

int RzModule::resolveTypeModuleIndex(RzType* type) {
	if (hasType(type->getName()))
		return 0;

	for (int i = 1; i < m_dependencies.size(); ++i) {
		if (m_dependencies[i].module->hasType(type->getName())) {
			return i;
		}
	}

	return -1;
}

void RzModule::resolveType(RzQualType& type) {
	// Fill type information based on the module dependency graph

}

bool RzModule::hasType(const std::string& name) {
	for (auto& t : m_types) {
		if (t->getName() == name)
			return true;
	}

	return false;
}

bool RzModule::hasType(RzType* type)
{
	for (auto& t : m_types)
	{
		if (t == type)
			return true;
	}
	return false;
}

int RzModule::getDependencyId(const std::string& name) {
	for (int i = 0; i < m_dependencies.size(); ++i) {
		if (m_dependencies[i].name == name)
			return i;
	}

	return -1;
}

int RzModule::getDependencyId(RzModule* module) {
	for (int i = 0; i < m_dependencies.size(); ++i) {
		if (m_dependencies[i].module == module)
			return i;
	}

	return -1;
}

bool RzModule::depends(const std::string& name) const {
	for (auto& dep : m_dependencies) {
		if (dep.name == name)
			return true;
	}

	return false;
}

bool RzModule::depends(RzModule* module) const {
	for (auto& dep : m_dependencies) {
		if (dep.module == module)
			return true;
	}

	return false;
}

int RzModule::createDependency(const std::string& name) {
	// This overload creates a unlinked dependency, potentially loaded later

	for (int i = 0; i < m_dependencies.size(); ++i) {
		if (m_dependencies[i].name == name) {
			return i;
		}
	}

	ModuleDependency dep;
	dep.module = nullptr;
	dep.name = name;
	m_dependencies.push_back(dep);

	return m_dependencies.size() - 1;
}

int RzModule::createDependency(RzModule* module) {
	if (!module) {
		RZLOG("Can't create dependencies to null modules\n");
		return -1;
	}

	for (int i = 0; i < m_dependencies.size(); ++i) {
		if (m_dependencies[i].name == module->getName()) {
			return i;
		}
	}

	ModuleDependency dep;
	dep.module = module;
	dep.name = module->getName();
	m_dependencies.push_back(dep);

	RZLOG("Module '%s' now depends on '%s'\n", getName().c_str(), module->getName().c_str());

	return m_dependencies.size() - 1;
}

RzModule* RzModule::getDependantModule(int index) {
	return m_dependencies[index].module;
}

std::string RzModule::getName() {
	return m_name;
}

uint32_t RzModule::storeString(std::string s) {
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

bool RzModule::hasFunction(const std::string& name) {
	return false;
}

void RzModule::write(const char* filename) {
	FILE* fp = fopen(filename, "wb");
	if (fp)
	{
		int ic = m_code.size();
		fwrite(&ic, sizeof(ic), 1, fp);
		fwrite(&m_code[0], sizeof(RzInstruction), m_code.size(), fp);
		fclose(fp);
	}
}

void RzModule::read(const char* filename) {
	FILE* fp = fopen(filename, "rb");
	if (fp)
	{
		int ic;
		fread(&ic, sizeof(ic), 1, fp);
		m_code.resize(ic);
		fread(&m_code[0], sizeof(RzInstruction), ic, fp);
		fclose(fp);
	}
}

void RzModule::dumpToFile(const std::string& filename) {
	FILE* f = fopen(filename.c_str(), "w");

	if (f)
	{
		for (int pc = 0; pc < m_code.size(); ++pc)
		{
			for (std::size_t i = 0; i < m_functions.size(); ++i)
			{
				if (pc == m_functions[i].m_offset)
				{

					fprintf(f, "\n");
                    fprintf(f, "%s:\n", m_functions[i].m_absoluteName.c_str());
				}
			}

			auto prettyInstruction = getPrettyInstructionStr(m_code[pc]);
            fprintf(f, "%d %s\n", pc, prettyInstruction.c_str());
		}
	}

	fclose(f);
}

void RzModule::debugCode() {
	for (auto i : m_code) {
		uint32_t opcode = getopcode(i);
		//Log("op %d", opcode);
	}
}

int RzModule::getFunctionIndexByName(const std::string& name) {
	for (std::size_t i = 0; i < m_functions.size(); ++i) {
		if (m_functions[i].m_absoluteName == name) {
			return i;
		}
	}

	return -1;
}
