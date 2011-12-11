// [Fog-Core]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_CORE_CPP_CONFIGCMAKE_H
#define _FOG_CORE_CPP_CONFIGCMAKE_H

// ============================================================================
// [FOG_BUILD]
// ============================================================================

#cmakedefine FOG_BUILD_STATIC

// ============================================================================
// [FOG_DEBUG]
// ============================================================================

// If FOG_DEBUG and FOG_NO_DEBUG is not defined then FOG_DEBUG will be detected
// using the compiler specific macros. This enables to set the build type using
// IDE.
#if !defined(FOG_DEBUG) && !defined(FOG_NO_DEBUG)

#if defined(_DEBUG)
#define FOG_DEBUG
#endif // _DEBUG

#endif // !FOG_DEBUG && !FOG_NO_DEBUG

// ============================================================================
// [FOG_OPTIMIZE]
// ============================================================================

//! @brief Enable support for MMX instructions.
#cmakedefine FOG_OPTIMIZE_MMX
//! @brief Enable support for 3dNow! instructions.
#cmakedefine FOG_OPTIMIZE_3DNOW

//! @brief Enable support for SSE instructions.
#cmakedefine FOG_OPTIMIZE_SSE
//! @brief Enable support for SSE2 instructions.
#cmakedefine FOG_OPTIMIZE_SSE2
//! @brief Enable support for SSSE3 instructions.
#cmakedefine FOG_OPTIMIZE_SSSE3

// ============================================================================
// [FOG_BYTE_ORDER]
// ============================================================================

//! @brief Target byteorder, can be:
//!   - @c FOG_LITTLE_ENDIAN,
//!   - @c FOG_BIG_ENDIAN.
#define FOG_BYTE_ORDER ${FOG_BYTE_ORDER}

// ============================================================================
// [Size of Types]
// ============================================================================

#define FOG_SIZEOF_VOID ${FOG_SIZEOF_VOID}
#define FOG_SIZEOF_INT ${FOG_SIZEOF_INT}
#define FOG_SIZEOF_LONG ${FOG_SIZEOF_LONG}
#define FOG_SIZEOF_WCHAR_T ${FOG_SIZEOF_WCHAR_T}

// ============================================================================
// [Header Files]
// ============================================================================

//! @brief Defined if we have fenv.h
#cmakedefine FOG_HAVE_FENV_H
//! @brief Defined if we have float.h
#cmakedefine FOG_HAVE_FLOAT_H
//! @brief Defined if we have limits.h
#cmakedefine FOG_HAVE_LIMITS_H
//! @brief Defined if we have stdarg.h
#cmakedefine FOG_HAVE_STDARG_H
//! @brief Defined if we have stdint.h
#cmakedefine FOG_HAVE_STDINT_H
//! @brief Defined if we have unistd.h
#cmakedefine FOG_HAVE_UNISTD_H

// ============================================================================
// [Libraries]
// ============================================================================

//! @brief Defined if jpeg library is available.
#cmakedefine FOG_HAVE_LIBJPEG
//! @brief Defined if png library is available.
#cmakedefine FOG_HAVE_LIBPNG

// ============================================================================
// [Fog-Font Backends]
// ============================================================================

//! @brief Whether to build Windows font support.
//!
//! This is default when using Windows.
#cmakedefine FOG_FONT_WINDOWS
//! @brief Whether to build Mac font support.
//!
//! This is default when using Mac.
#cmakedefine FOG_FONT_MAC
//! @brief Whether to build FreeType support
//!
//! This is default when using Linux/BSD. May be used together with Windows/Mac
//! font support.
#cmakedefine FOG_FONT_FREETYPE

//! @brief Whether to build FontConfig support.
//!
//! Can be enabled only if FOG_FONT_FREETYPE is defined.
#cmakedefine FOG_HAVE_FONTCONFIG

// ============================================================================
// [FOG_UI]
// ============================================================================

//! @brief Whether to build Windows Fog/UI support.
#cmakedefine FOG_UI_WINDOWS

//! @brief Defined to build MAC Fog/UI support.
#cmakedefine FOG_UI_MAC

//! @brief Whether to build X11 Fog/UI support (plugin).
#cmakedefine FOG_UI_X11

// ============================================================================
// [FOG_INSTALL]
// ============================================================================

//! @brief Install prefix specified by configure parameters.
#define FOG_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}"

// [Guard]
#endif // _FOG_CORE_CPP_CONFIGCMAKE_H