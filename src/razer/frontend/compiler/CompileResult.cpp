#include <razer/frontend/compiler/CompileResult.h>

RzCompileResult RzCompileResult::ok(RzCompileResult::OK);
RzCompileResult RzCompileResult::aborted(RzCompileResult::ABORTED);
RzCompileResult RzCompileResult::failed(RzCompileResult::FAILED);

RzCompileResult::RzCompileResult(int status)
{
	m_status = status;
}
