#ifndef WINDOWS_STUB_H
#define WINDOWS_STUB_H

// Cross-platform compatibility layer for DLPUtilLib2
// This file redirects to windows.h stub for Linux/GCC builds

// Include the comprehensive windows.h stub
#include "windows.h"

// Alias for backwards compatibility
#define WINDOWS_STUB_H_INCLUDED

#endif // WINDOWS_STUB_H
