#ifndef OFFSETS_H
#define OFFSETS_H

#include "include.h"

inline Wmemory cs2(L"cs2.exe");
//github.com/a2x/cs2-dumper/ - thx
namespace Offsets
{
	adr64 client = cs2.GetModuleBaseAddress(L"client.dll");

	const uintptr_t dwForceJump = 0x1730530;//bhop
}

#endif
