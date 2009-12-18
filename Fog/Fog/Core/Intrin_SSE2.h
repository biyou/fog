// [Fog/Core Library - C++ API]
//
// [Licence]
// MIT, See COPYING file in package

// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// [Guard]
#ifndef _FOG_CORE_INTRIN_SSE2_H
#define _FOG_CORE_INTRIN_SSE2_H

// [Dependencies]
#include <Fog/Build/Build.h>
#include <Fog/Core/Intrin_SSE.h>
#include <emmintrin.h>

// New VisualStudio2008 contains _mm_castsi128_ps() and _mm_castps_si128(), for
// older ones we need to implement these casts ourself.
#if defined(_MSC_VER) && (_MSC_VER <= 1400)
// I hope that MSVC is intelligent to optimize this to NOP!
static FOG_INLINE __m128 _mm_castsi128_ps(__m128i n) { return *(__m128 *)&n; }
static FOG_INLINE __m128i _mm_castps_si128(__m128 n) { return *(__m128i*)&n; }
#endif // _MSC_VER

// Our extensions (can't be inline, because MSVC complains about constant expression).
#define _mm_shuffle_epi32_f(src, imm) _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(src), imm));

//! @addtogroup Fog_Core
//! @{

namespace Fog {

//! @brief SSE2 data type that can be used to store / load data to / from
//! xmm register.
union FOG_ALIGN(16) sse2_t
{
  __m128d m128d;
  __m128i m128i;
  uint64_t uq[2];
  int64_t  sq[2];
  uint32_t ud[4];
  int32_t  sd[4];
  uint16_t uw[8];
  int16_t  sw[8];
  uint8_t  ub[16];
  int8_t   sb[16];
  float    f[4];
  double   d[2];
};

}

//! @}

// [Guard]
#endif // _FOG_CORE_INTRIN_SSE2_H