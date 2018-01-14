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

#define PBIT_BITS_ORDER_BE     0
#define PBIT_BITS_COPY_GT_7    1
#define PBIT_IMPL_1LITERAL     2
#define PBIT_BITS_ALIGN_START  3
#define PBIT_4_OFFSET_TABLES   4

#define PFLAG_BITS_ORDER_BE    (1 << PBIT_BITS_ORDER_BE)
#define PFLAG_BITS_COPY_GT_7   (1 << PBIT_BITS_COPY_GT_7)
#define PFLAG_IMPL_1LITERAL    (1 << PBIT_IMPL_1LITERAL)
#define PFLAG_BITS_ALIGN_START (1 << PBIT_BITS_ALIGN_START)
#define PFLAG_4_OFFSET_TABLES  (1 << PBIT_4_OFFSET_TABLES)

#define TBIT_NO_LIT_SEQ 0
#define TBIT_NO_LEN1_SEQ 1
#define TBIT_NO_LEN123_SEQ_MIRRORS 2

#define TFLAG_NO_LIT_SEQ            (1 << TBIT_NO_LIT_SEQ)
#define TFLAG_NO_LEN1_SEQ           (1 << TBIT_NO_LEN1_SEQ)
#define TFLAG_NO_LEN123_SEQ_MIRRORS (1 << TBIT_NO_LEN123_SEQ_MIRRORS)

#endif
