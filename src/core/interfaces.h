#pragma once

#include "globals.h"

#include "../valve/icliententitylist.h"
#include "../valve/iengineclient.h"
#include "../valve/ienginetraceclient.h"

namespace interfaces
{
	void Setup() noexcept;

	template <typename T>
	T* Capture(const char* name, const char* lib) noexcept;

	inline IClientEntityList *entityList = nullptr;
	inline IEngineClient* engine = nullptr;
	inline IEngineTraceClient* trace = nullptr;

	inline void* client = nullptr;
	inline void* clientMode = nullptr;
}
