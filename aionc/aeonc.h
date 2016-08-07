#ifndef AEON_compilertool_h__
#define AEON_compilertool_h__

#include <AEON/aeon_datavalue.h>
#include <AEON/aeon_parser.h>
#include <AEON/aeon_compiler.h>
#include <AEON/aeon_bytecode.h>
#include <AEON/aeon_vm.h>
#include <AEON/aeon_env.h>

#include <vector>
#include <string>

namespace AEON
{
	/// The tool prepares a struct from all the input parameters, and sends it for execution
	struct CompileJobParams
	{
		std::vector<std::string> files;
		bool                       run;  ///< The goal can be to run the sources or to compile into lib/exe

	};

	void ExecuteCompileJob(CompileJobParams& params);
};

#endif // AEON_compilertool_h__
