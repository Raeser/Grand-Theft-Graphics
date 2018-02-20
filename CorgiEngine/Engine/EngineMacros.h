#pragma once
#define dontUse_STRINGIME2(X) #X
#define dontUse_STRINGME(X) dontUse_STRINGIME2(X)
#define BUILDTYPE() dontUse_STRINGME(BUILD)

#ifdef _DEBUG
#define BUILD Debug
#endif // DEBUG

#ifdef _RELEASE
#define BUILD Release
#endif // DEBUG

#ifdef _RETAIL
#define BUILD Retail
#endif // DEBUG


