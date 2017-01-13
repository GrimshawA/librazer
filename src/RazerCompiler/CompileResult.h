#ifndef CompileResult_h__
#define CompileResult_h__

/**
	\class RzCompileResult
	\brief Internal helper structure to report the status of compilation of bits of code
*/
class RzCompileResult
{
public:
	RzCompileResult(int status = OK);

	enum {
		OK,
		FAILED
	};

	int m_status;
};

#endif // CompileResult_h__
