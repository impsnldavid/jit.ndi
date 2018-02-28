//
// This file is part of jit.ndi https://github.com/impsnldavid/jit.ndi
// Copyright (c) 2018 David Butler / The Impersonal Stereo
// 
// This program is free software: you can redistribute it and/or modify  
// it under the terms of the GNU Lesser General Public License as 
// published by the Free Software Foundation, version 3.
//
// This program is distributed in the hope that it will be useful, but 
// WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public 
// License along with this program. 
// If not, see <http://www.gnu.org/licenses/>.

#ifndef H_JIT_NDI_RUNTIME
#define H_JIT_NDI_RUNTIME

#include <jit.common.h>
#include <Processing.NDI.Lib.h>

inline void free_ndi_runtime(HMODULE* runtimeModule)
{
	if (*runtimeModule != NULL)
	{
		FreeLibrary(*runtimeModule);
		*runtimeModule = NULL;
	}
}

inline bool load_ndi_runtime(HMODULE* runtimeModule, NDIlib_v3** ndiLib)
{
	*runtimeModule = NULL;
	*ndiLib = NULL;

#ifdef WIN_VERSION
	
	LPSTR runtimeDir[MAX_PATH];

	const DWORD runtimeDirLen = GetEnvironmentVariable(NDILIB_REDIST_FOLDER, runtimeDir, MAX_PATH);

	if (runtimeDirLen == 0)
	{
		error("Unable to load jit.ndi externals as NDI Runtime is not installed, please download and install from '%s' and restart Max.", NDILIB_REDIST_URL);
		return false;
	}

	LPSTR runtimeDllPath[MAX_PATH];

	snprintf(runtimeDllPath, MAX_PATH, "%s\\%s", runtimeDir, NDILIB_LIBRARY_NAME);

	*runtimeModule = LoadLibrary(runtimeDllPath);
	if (*runtimeModule == NULL)
	{
		error("Unable to load NDI runtime from path '%s'. Please download and reinstall NDI runtime from '%s' and restart Max", runtimeDllPath, NDILIB_REDIST_URL);
		return false;
	}

	quittask_install(free_ndi_runtime, runtimeModule);

	FARPROC loadFunction = GetProcAddress(*runtimeModule, "NDIlib_v3_load");

	if (loadFunction == NULL)
	{
		error("Unable to load NDI functions from DLL at path '%s'. Please download and reinstall NDI runtime from '%s' and restart Max", runtimeDllPath, NDILIB_REDIST_URL);
		return false;
	}

	*ndiLib = (NDIlib_v3*)loadFunction();
	 
#else

	// TODO: Load NDI runtime for Mac

#endif

	return true;
}

#endif // H_JIT_NDI_RUNTIME