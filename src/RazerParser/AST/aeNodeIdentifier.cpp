#include <RazerParser/AST/aeNodeIdentifier.h>
#include <RazerCompiler/RzCompiler.h>

aeNodeIdentifier::aeNodeIdentifier()
{
	m_nodeType = AEN_IDENTIFIER;
}

std::string aeNodeIdentifier::str() const
{
	return m_name;
}
