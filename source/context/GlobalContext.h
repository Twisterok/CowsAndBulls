#ifndef __GLOBAL_CONTEXT_H__
#define __GLOBAL_CONTEXT_H__
// Own includes
#include <network/PacketFactory.h>

// std includes
#include <string>
#include <vector>
#include <memory>




class GlobalContext
{
	GlobalContext();
	~GlobalContext();

public:
	std::unique_ptr<PacketFactory>	m_factory;	

	static GlobalContext*	singleton();
};

#endif
