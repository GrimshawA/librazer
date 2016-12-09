#ifndef CompileResult_h__
#define CompileResult_h__

/**
	\class RzCompileResult
	\brief Internal helper structure to report the status of compilation of bits of code
*/
class RzCompileResult
{
public:

	enum {
		OK,
		FAILED
	};

	int status;
};

#endif // CompileResult_h__
