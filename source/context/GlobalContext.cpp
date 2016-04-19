// Own includes
#include <context/GlobalContext.h>

// std includes
// Qt includes

GlobalContext::GlobalContext()
{
}

GlobalContext::~GlobalContext()
{
}

GlobalContext*	GlobalContext::singleton()
{
	static GlobalContext s_context;
	return &s_context;
}
