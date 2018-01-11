#ifndef INCLUDED_FLAGS
#define INCLUDED_FLAGS

/*
 * Copyright (c) 2018 Magnus Lind.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software, alter it and re-
 * distribute it freely for any non-commercial, non-profit purpose subject to
 * the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
 *   claim that you wrote the original software. If you use this software in a
 *   product, an acknowledgment in the product documentation would be
 *   appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not
 *   be misrepresented as being the original software.
 *
 *   3. This notice may not be removed or altered from any distribution.
 *
 *   4. The names of this software and/or it's copyright holders may not be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 */

#define BIT_PROTO_BITS_ORDER_LE     1
#define BIT_PROTO_BITS_SHIFT_GT_7   2
#define BIT_PROTO_BITS_PAD_END      4
#define BIT_PROTO_4_OFFSET_PARTS    8
#define BIT_PROTO_NO_IMPL_1LIT     16

#define FLAG_PROTO_BITS_ORDER_LE   (1 << BIT_PROTO_BITS_ORDER_LE)
#define FLAG_PROTO_BITS_SHIFT_GT_7 (1 << BIT_PROTO_BITS_SHIFT_GT_7)
#define FLAG_PROTO_4_OFFSET_PARTS  (1 << BIT_PROTO_4_OFFSET_PARTS)
#define FLAG_PROTO_BITS_PAD_END    (1 << BIT_PROTO_BITS_PAD_END)
#define FLAG_PROTO_NO_IMPL_1LIT    (1 << BIT_PROTO_NO_IMPL_1LIT)

#define BIT_AVOID_LIT_SEQ 1
#define BIT_AVOID_LEN1_SEQ 2
#define BIT_AVOID_LEN123_SEQ_MIRRORS 4

#define FLAG_AVOID_LIT_SEQ            (1 << BIT_AVOID_LIT_SEQ)
#define FLAG_AVOID_LEN1_SEQ           (1 << BIT_AVOID_LEN1_SEQ)
#define FLAG_AVOID_LEN123_SEQ_MIRRORS (1 << BIT_AVOID_LEN123_SEQ_MIRRORS)

#endif
