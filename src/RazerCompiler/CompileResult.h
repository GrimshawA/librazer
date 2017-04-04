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
        FAILED,
        ABORTED
	};

    bool operator==(const RzCompileResult& o)
    {
        return o.m_status == m_status;
    }

    static RzCompileResult ok;
    static RzCompileResult aborted;
    static RzCompileResult failed;

	int m_status;
};

#endif // CompileResult_h__
