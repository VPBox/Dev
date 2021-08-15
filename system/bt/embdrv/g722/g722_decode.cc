/*
 * SpanDSP - a series of DSP components for telephony
 *
 * g722_decode.c - The ITU G.722 codec, decode part.
 *
 * Written by Steve Underwood <steveu@coppice.org>
 *
 * Copyright (C) 2005 Steve Underwood
 *
 *  Despite my general liking of the GPL, I place my own contributions 
 *  to this code in the public domain for the benefit of all mankind -
 *  even the slimy ones who might try to proprietize my work and use it
 *  to my detriment.
 *
 * Based in part on a single channel G.722 codec which is:
 *
 * Copyright (c) CMU 1993
 * Computer Science, Speech Group
 * Chengxiang Lu and Alex Hauptmann
 *
 * $Id: g722_decode.c 194722 2009-05-15 17:59:08Z russell $
 */

/*! \file */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>

#include "g722_typedefs.h"
#include "g722_enc_dec.h"

#if !defined(FALSE)
#define FALSE 0
#endif
#if !defined(TRUE)
#define TRUE (!FALSE)
#endif

#define PACKED_INPUT    (0)
#define BITS_PER_SAMPLE (8)

#ifndef BUILD_FEATURE_G722_USE_INTRINSIC_SAT
static __inline int16_t __ssat16(int32_t amp)
{
    int16_t amp16;

    /* Hopefully this is optimised for the common case - not clipping */
    amp16 = (int16_t) amp;
    if (amp == amp16)
        return amp16;
    if (amp > 0x7fff)
        return  0x7fff;
    return  0x8000;
}
/*- End of function --------------------------------------------------------*/
#else
static __inline int16_t __ssat16( int32_t val)
{
    register int32_t res;
    __asm volatile (
        "SSAT %0, #16, %1\n\t"
        :"=r"(res)
        :"r"(val)
        :);
    return (int16_t)res;
}
#endif

/*- End of function --------------------------------------------------------*/

static void block4(g722_band_t *band, int d);

static void block4(g722_band_t *band, int d)
{
    int wd1;
    int wd2;
    int wd3;
    int i;
    int sg[7];
    int ap1, ap2;
    int sg0, sgi;
    int sz;

    /* Block 4, RECONS */
    band->d[0] = d;
    band->r[0] = __ssat16(band->s + d);

    /* Block 4, PARREC */
    band->p[0] = __ssat16(band->sz + d);

    /* Block 4, UPPOL2 */
    for (i = 0;  i < 3;  i++)
    {
        sg[i] = band->p[i] >> 15;
    }
    wd1 = __ssat16(band->a[1] << 2);

    wd2 = (sg[0] == sg[1])  ?  -wd1  :  wd1;
    if (wd2 > 32767)
        wd2 = 32767;

    ap2 = (sg[0] == sg[2])  ?  128  :  -128;
    ap2 += (wd2 >> 7);
    ap2 += (band->a[2]*32512) >> 15;
    if (ap2 > 12288)
        ap2 = 12288;
    else if (ap2 < -12288)
        ap2 = -12288;
    band->ap[2] = ap2;

    /* Block 4, UPPOL1 */
    sg[0] = band->p[0] >> 15;
    sg[1] = band->p[1] >> 15;
    wd1 = (sg[0] == sg[1])  ?  192  :  -192;
    wd2 = (band->a[1]*32640) >> 15;

    ap1 = __ssat16(wd1 + wd2);
    wd3 = __ssat16(15360 - band->ap[2]);
    if (ap1 > wd3)
        ap1 = wd3;
    else if (ap1 < -wd3)
        ap1 = -wd3;
    band->ap[1] = ap1;

    /* Block 4, UPZERO */
    /* Block 4, FILTEZ */
    wd1 = (d == 0)  ?  0  :  128;

    sg0 = sg[0] = d >> 15;
    for (i = 1;  i < 7;  i++)
    {
	sgi = band->d[i] >> 15;
        wd2 = (sgi == sg0)  ?  wd1  :  -wd1;
        wd3 = (band->b[i]*32640) >> 15;
        band->bp[i] = __ssat16(wd2 + wd3);
    }

    /* Block 4, DELAYA */
    sz = 0;
    for (i = 6;  i > 0;  i--)
    {
	int bi;

        band->d[i] = band->d[i - 1];
	bi = band->b[i] = band->bp[i];
	wd1 = __ssat16(band->d[i] + band->d[i]);
	sz += (bi*wd1) >> 15;
    }
    band->sz = sz;
    
    for (i = 2;  i > 0;  i--)
    {
        band->r[i] = band->r[i - 1];
        band->p[i] = band->p[i - 1];
        band->a[i] = band->ap[i];
    }

    /* Block 4, FILTEP */
    wd1 = __ssat16(band->r[1] + band->r[1]);
    wd1 = (band->a[1]*wd1) >> 15;
    wd2 = __ssat16(band->r[2] + band->r[2]);
    wd2 = (band->a[2]*wd2) >> 15;
    band->sp = __ssat16(wd1 + wd2);

    /* Block 4, PREDIC */
    band->s = __ssat16(band->sp + band->sz);
}
/*- End of function --------------------------------------------------------*/

g722_decode_state_t *g722_decode_init(g722_decode_state_t *s, unsigned int rate, int options)
{
    if (s == NULL)
    {
#ifdef G722_SUPPORT_MALLOC
        if ((s = (g722_decode_state_t *) malloc(sizeof(*s))) == NULL)
#endif
            return NULL;
    }
    memset(s, 0, sizeof(*s));
    if (rate == 48000)
        s->bits_per_sample = 6;
    else if (rate == 56000)
        s->bits_per_sample = 7;
    else
        s->bits_per_sample = 8;
    s->dac_pcm = options & G722_FORMAT_DAC12;
    s->band[0].det = 32;
    s->band[1].det = 8;
    return s;
}
/*- End of function --------------------------------------------------------*/

int g722_decode_release(g722_decode_state_t *s)
{
    free(s);
    return 0;
}
/*- End of function --------------------------------------------------------*/

static int16_t wl[8] = {-60, -30, 58, 172, 334, 538, 1198, 3042 };
static int16_t rl42[16] = {0, 7, 6, 5, 4, 3, 2, 1, 7, 6, 5, 4, 3,  2, 1, 0 };
static int16_t ilb[32] =
{
    2048, 2093, 2139, 2186, 2233, 2282, 2332,
    2383, 2435, 2489, 2543, 2599, 2656, 2714,
    2774, 2834, 2896, 2960, 3025, 3091, 3158,
    3228, 3298, 3371, 3444, 3520, 3597, 3676,
    3756, 3838, 3922, 4008
};

static int16_t wh[3] = {0, -214, 798};
static int16_t rh2[4] = {2, 1, 2, 1};
static int16_t qm2[4] = {-7408, -1616,  7408,   1616};
static int16_t qm4[16] = 
{
        0, -20456, -12896,  -8968, 
    -6288,  -4240,  -2584,  -1200,
    20456,  12896,   8968,   6288,
     4240,   2584,   1200,      0
};
#if 0
static const int qm5[32] =
{
      -280,   -280, -23352, -17560,
    -14120, -11664,  -9752,  -8184,
     -6864,  -5712,  -4696,  -3784,
     -2960,  -2208,  -1520,   -880,
     23352,  17560,  14120,  11664,
      9752,   8184,   6864,   5712,
      4696,   3784,   2960,   2208,
      1520,    880,    280,   -280
};
#endif
static int16_t qm6[64] =
{
      -136,   -136,   -136,   -136,
    -24808, -21904, -19008, -16704,
    -14984, -13512, -12280, -11192,
    -10232,  -9360,  -8576,  -7856,
     -7192,  -6576,  -6000,  -5456,
     -4944,  -4464,  -4008,  -3576,
     -3168,  -2776,  -2400,  -2032,
     -1688,  -1360,  -1040,   -728,
     24808,  21904,  19008,  16704,
     14984,  13512,  12280,  11192,
     10232,   9360,   8576,   7856,
      7192,   6576,   6000,   5456,
      4944,   4464,   4008,   3576,
      3168,   2776,   2400,   2032,
      1688,   1360,   1040,    728,
       432,    136,   -432,   -136
};
static int16_t qmf_coeffs_even[12] =
{
      3,  -11,   12,   32, -210,  951, 3876, -805,  362, -156,   53,  -11,
};
static int16_t qmf_coeffs_odd[12] =
{
    -11,   53, -156,  362, -805, 3876, 951,  -210,   32,   12,  -11,    3
};

uint32_t g722_decode(g722_decode_state_t *s, int16_t amp[], const uint8_t g722_data[], int len, uint16_t gain)
{

    int dlowt;
    int rlow;
    int ihigh;
    int dhigh;
    int rhigh;
    int xout1;
    int xout2;
    int wd1;
    int wd2;
    int wd3;
    int code;
    uint32_t outlen;
    int i;
    int j;

    outlen = 0;
    rhigh = 0;

    for (j = 0;  j < len;  )
    {
#if PACKED_INPUT == 1
        /* Unpack the code bits */
        if (s->in_bits < s->bits_per_sample)
        {
            s->in_buffer |= (g722_data[j++] << s->in_bits);
            s->in_bits += 8;
        }
        code = s->in_buffer & ((1 << s->bits_per_sample) - 1);
        s->in_buffer >>= s->bits_per_sample;
        s->in_bits -= s->bits_per_sample;
#else
        code = g722_data[j++];
#endif

#if BITS_PER_SAMPLE == 8
        wd1 = code & 0x3F;
        ihigh = (code >> 6) & 0x03;
        wd2 = qm6[wd1];
        wd1 >>= 2;
#elif BITS_PER_SAMPLE == 7
        wd1 = code & 0x1F;
        ihigh = (code >> 5) & 0x03;
        wd2 = qm5[wd1];
        wd1 >>= 1;
#elif BITS_PER_SAMPLE == 6
       wd1 = code & 0x0F;
       ihigh = (code >> 4) & 0x03;
       wd2 = qm4[wd1];
#endif
        /* Block 5L, LOW BAND INVQBL */
        wd2 = (s->band[0].det*wd2) >> 15;
        /* Block 5L, RECONS */
        rlow = s->band[0].s + wd2;
        /* Block 6L, LIMIT */

        // ANDREA
        // rlow=ssat(rlow,2<<14)
        if (rlow > 16383)
        {
            rlow = 16383;
        }
        else if (rlow < -16384)
        {
            rlow = -16384;
        }

        /* Block 2L, INVQAL */
        wd2 = qm4[wd1];
        dlowt = (s->band[0].det*wd2) >> 15;

        /* Block 3L, LOGSCL */
        wd2 = rl42[wd1];
        wd1 = (s->band[0].nb*127) >> 7;
        wd1 += wl[wd2];
        if (wd1 < 0)
        {
            wd1 = 0;
        }
        else if (wd1 > 18432)
        {
            wd1 = 18432;
        }
        s->band[0].nb = wd1;
            
        /* Block 3L, SCALEL */
        wd1 = (s->band[0].nb >> 6) & 31;
        wd2 = 8 - (s->band[0].nb >> 11);
        wd3 = (wd2 < 0)  ?  (ilb[wd1] << -wd2)  :  (ilb[wd1] >> wd2);
        s->band[0].det = wd3 << 2;

        block4(&s->band[0], dlowt);
        
        /* Block 2H, INVQAH */
        wd2 = qm2[ihigh];
        dhigh = (s->band[1].det*wd2) >> 15;
        /* Block 5H, RECONS */
        rhigh = dhigh + s->band[1].s;
        /* Block 6H, LIMIT */

        // ANDREA
        // rhigh=ssat(rhigh,2<<14)

        if (rhigh > 16383)
            rhigh = 16383;
        else if (rhigh < -16384)
            rhigh = -16384;

        /* Block 2H, INVQAH */
        wd2 = rh2[ihigh];
        wd1 = (s->band[1].nb*127) >> 7;
        wd1 += wh[wd2];
        if (wd1 < 0)
            wd1 = 0;
        else if (wd1 > 22528)
            wd1 = 22528;
        s->band[1].nb = wd1;
            
        /* Block 3H, SCALEH */
        wd1 = (s->band[1].nb >> 6) & 31;
        wd2 = 10 - (s->band[1].nb >> 11);
        wd3 = (wd2 < 0)  ?  (ilb[wd1] << -wd2)  :  (ilb[wd1] >> wd2);
        s->band[1].det = wd3 << 2;

        block4(&s->band[1], dhigh);

        /* Apply the receive QMF */
        for (i = 0;  i < 22;  i++)
            s->x[i] = s->x[i + 2];
        s->x[22] = rlow + rhigh;
        s->x[23] = rlow - rhigh;

        // we should get PERF numbers for the following loop 
        xout1 = 0;
        xout2 = 0;
        for (i = 0;  i < 12;  i++)
        {
            xout2 += s->x[2*i]   * qmf_coeffs_even[i];
            xout1 += s->x[2*i+1] * qmf_coeffs_odd[i];
        }
        xout1 = NLDECOMPRESS_PREPROCESS_SAMPLE_WITH_GAIN((int16_t) __ssat16(xout1 >> 11), gain);
        xout2 = NLDECOMPRESS_PREPROCESS_SAMPLE_WITH_GAIN((int16_t) __ssat16(xout2 >> 11), gain);
        if (s->dac_pcm)
        {
            amp[outlen++] = ((int16_t) (xout1 >> 4) + 2048);
            amp[outlen++] = ((int16_t) (xout2 >> 4) + 2048);
        }
        else
        {
            amp[outlen++] = xout1;
            amp[outlen++] = xout2;
        }
    }
    return outlen;
}
/*- End of function --------------------------------------------------------*/
/*- End of file ------------------------------------------------------------*/
