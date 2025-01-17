///////////////////////////////////////////////////////////////////////////////////////
// October 19, 2023 - Modified by José Juan Hernández-Morales
//   Adding mainly assembly instructions for designed RISC-V instruction set extension.
//   Repository: https://github.com/josejuanhm/fpau
///////////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include "params.h"
#include "ntt.h"
#include "reduce.h"

#include "fpau_switches.h"

#ifdef NTT_TESTING_HW
#include "uart.h"
#endif

#ifdef FPAU  // without Montgomery factor
static const int32_t zetas[N] = {
         0, -3572223,  3765607,  3761513, -3201494, -2883726, -3145678, -3201430,  -601683,  3542485,  2682288,  2129892,  3764867, -1005239,   557458, -1221177, 
  -3370349, -4063053,  2663378, -1674615, -3524442,  -434125,   676590, -1335936, -3227876,  1714295,  2453983,  1460718,  -642628, -3585098,  2815639,  2283733, 
   3602218,  3182878,  2740543, -3586446, -3110818,  2101410,  3704823,  1159875,   394148,   928749,  1095468, -3506380,  2071829, -4018989,  3241972,  2156050, 
   3415069,  1759347,  -817536, -3574466,  3756790, -1935799, -1716988, -3950053, -2897314,  3192354,   556856,  3870317,  2917338,  1853806,  3345963,  1858416, 
   3073009,  1277625, -2635473,  3852015,  4183372, -3222807, -3121440,  -274060,  2508980,  2028118,  1937570, -3815725,  2811291, -2983781, -1109516,  4158088, 
   1528066,   482649,  1148858, -2962264,  -565603,   169688,  2462444, -3334383, -4166425, -3488383,  1987814, -3197248,  1736313,   235407, -3250154,  3258457, 
  -2579253,  1787943, -2391089, -2254727,  3482206, -4182915, -1300016, -2362063, -1317678,  2461387,  3035980,   621164,  3901472, -1226661,  2925816,  3374250, 
   1356448, -2775755,  2683270, -2778788, -3467665,  2312838,  -653275,  -459163,   348812,  -327848,  1011223, -2354215, -3818627, -1922253, -2236726,  1744507, 
      1753, -1935420, -2659525, -1455890,  2660408, -1780227,   -59148,  2772600,  1182243,    87208,   636927, -3965306, -3956745, -2296397, -3284915, -3716946, 
    -27812,   822541,  1009365, -2454145, -1979497,  1596822, -3956944, -3759465, -1685153, -3410568,  2678278, -3768948, -3551006,   635956,  -250446, -2455377, 
  -4146264, -1772588,  2192938, -1727088,  2387513, -3611750,  -268456, -3180456,  3747250,  2296099,  1239911, -3838479,  3195676,  2642980,  1254190,   -12417, 
   2998219,   141835,   -89301,  2513018, -1354892,   613238, -1310261, -2218467,  -458740, -1921994,  4040196, -3472069,  2039144, -1879878,  -818761, -2178965, 
  -1623354,  2105286, -2374402, -2033807,   586241, -1179613,   527981, -2743411, -1476985,  1994046,  2491325, -1393159,   507927, -1187885,  -724804, -1834526, 
  -3033742,  -338420,  2647994,  3009748, -2612853,  4148469,   749577, -4022750,  3980599,  2569011, -1615530,  1723229,  1665318,  2028038,  1163598, -3369273, 
   3994671,   -11879, -1370517,  3020393,  3363542,   214880,   545376,  -770441,  3105558, -1103344,   508145,  -553718,   860144,  3430436,   140244, -1514152, 
  -2185084,  3123762,  2358373, -2193087, -3014420, -1716814,  2926054,  -392707,  -303005,  3531229, -3974485, -3773731,  1900052,  -781875,  1054478, -731434
};

#else
static const int32_t zetas[N] = {
         0,    25847, -2608894,  -518909,   237124,  -777960,  -876248,   466468,
   1826347,  2353451,  -359251, -2091905,  3119733, -2884855,  3111497,  2680103,
   2725464,  1024112, -1079900,  3585928,  -549488, -1119584,  2619752, -2108549,
  -2118186, -3859737, -1399561, -3277672,  1757237,   -19422,  4010497,   280005,
   2706023,    95776,  3077325,  3530437, -1661693, -3592148, -2537516,  3915439,
  -3861115, -3043716,  3574422, -2867647,  3539968,  -300467,  2348700,  -539299,
  -1699267, -1643818,  3505694, -3821735,  3507263, -2140649, -1600420,  3699596,
    811944,   531354,   954230,  3881043,  3900724, -2556880,  2071892, -2797779,
  -3930395, -1528703, -3677745, -3041255, -1452451,  3475950,  2176455, -1585221,
  -1257611,  1939314, -4083598, -1000202, -3190144, -3157330, -3632928,   126922,
   3412210,  -983419,  2147896,  2715295, -2967645, -3693493,  -411027, -2477047,
   -671102, -1228525,   -22981, -1308169,  -381987,  1349076,  1852771, -1430430,
  -3343383,   264944,   508951,  3097992,    44288, -1100098,   904516,  3958618,
  -3724342,    -8578,  1653064, -3249728,  2389356,  -210977,   759969, -1316856,
    189548, -3553272,  3159746, -1851402, -2409325,  -177440,  1315589,  1341330,
   1285669, -1584928,  -812732, -1439742, -3019102, -3881060, -3628969,  3839961,
   2091667,  3407706,  2316500,  3817976, -3342478,  2244091, -2446433, -3562462,
    266997,  2434439, -1235728,  3513181, -3520352, -3759364, -1197226, -3193378,
    900702,  1859098,   909542,   819034,   495491, -1613174,   -43260,  -522500,
   -655327, -3122442,  2031748,  3207046, -3556995,  -525098,  -768622, -3595838,
    342297,   286988, -2437823,  4108315,  3437287, -3342277,  1735879,   203044,
   2842341,  2691481, -2590150,  1265009,  4055324,  1247620,  2486353,  1595974,
  -3767016,  1250494,  2635921, -3548272, -2994039,  1869119,  1903435, -1050970,
  -1333058,  1237275, -3318210, -1430225,  -451100,  1312455,  3306115, -1962642,
  -1279661,  1917081, -2546312, -1374803,  1500165,   777191,  2235880,  3406031,
   -542412, -2831860, -1671176, -1846953, -2584293, -3724270,   594136, -3776993,
  -2013608,  2432395,  2454455,  -164721,  1957272,  3369112,   185531, -1207385,
  -3183426,   162844,  1616392,  3014001,   810149,  1652634, -3694233, -1799107,
  -3038916,  3523897,  3866901,   269760,  2213111,  -975884,  1717735,   472078,
   -426683,  1723600, -1803090,  1910376, -1667432, -1104333,  -260646, -3833893,
  -2939036, -2235985,  -420899, -2286327,   183443,  -976891,  1612842, -3545687,
   -554416,  3919660,   -48306, -1362209,  3937738,  1400424,  -846154,  1976782
};
#endif

/*************************************************
* Name:        ntt
*
* Description: Forward NTT, in-place. No modular reduction is performed after
*              additions or subtractions. Output vector is in bitreversed order.
*
* Arguments:   - uint32_t p[N]: input/output coefficient array
**************************************************/
void ntt(int32_t a[N]) {
  #ifdef FPAU
  register uint32_t len asm("s9"), start asm("s1"), j asm("s2"), k asm("s3");
  register int32_t zeta asm("s4");

  register uint32_t coeff0 asm("s6");
  register uint32_t coeff1 asm("s7");
  #else
  unsigned int len, start, j, k;
  int32_t zeta, t;
  #endif

  k = 0;
  for(len = 128; len > 0; len >>= 1) {
    for(start = 0; start < N; start = j + len) {
      zeta = zetas[++k];
      for(j = start; j < start + len; ++j) {
        #ifdef FPAU
        //coeff0 = a[j];
        //coeff1 = a[j + len];

        // load coeff0
        asm volatile("mv t0, s2");
        asm volatile("slli t0, t0, 0x2");
        asm volatile("add t0, a0, t0");
        asm volatile("lw s6, 0(t0)");

        // load coeff1
        asm volatile("add t1, s2, s9");
        asm volatile("slli t1, t1, 0x2");
        asm volatile("add t1, a0, t1");
        asm volatile("lw s7, 0(t1)");

        // Compute butterfly operation
#ifndef STEEL
        asm volatile ("nop"); //ORCA
        asm volatile ("nop"); //ORCA
        asm volatile ("nop"); //ORCA
#endif
        asm volatile("fpau.dil.bf %0, %1,%2\n":"=r"(coeff0):"r"(coeff1),"r"(zeta): );
        
        // Store results in same position
        asm volatile("sw s6, 0(t0)");
        asm volatile("nop"); //ORCA and STEEL (2nd output written in next cycle)
        asm volatile("sw s7, 0(t1)");

        #else
        t = montgomery_reduce((int64_t)zeta * a[j + len]);
        a[j + len] = a[j] - t;
        a[j] = a[j] + t;
        #endif
      }
    }
  }
}

/*************************************************
* Name:        invntt_tomont
*
* Description: Inverse NTT and multiplication by Montgomery factor 2^32.
*              In-place. No modular reductions after additions or
*              subtractions; input coefficients need to be smaller than
*              Q in absolute value. Output coefficient are smaller than Q in
*              absolute value.
*
* Arguments:   - uint32_t p[N]: input/output coefficient array
**************************************************/
void invntt_tomont(int32_t a[N]) {
#ifdef FPAU
  register uint32_t len asm("s9"), start asm("s1"), j asm("s2"), k asm("s3");
  register int32_t zeta asm("s4");

  register uint32_t coeff0 asm("s6");
  register uint32_t coeff1 asm("s7");

  register const uint32_t ninv asm("s8") =  8347681;
#else
  unsigned int start, len, j, k;
  int32_t t, zeta;
  const int32_t f = 41978; // mont^2/256
#endif

  k = 256;
  for(len = 1; len < N; len <<= 1) {
    for(start = 0; start < N; start = j + len) {
      zeta = -zetas[--k];
      for(j = start; j < start + len; ++j) {
#ifdef FPAU
        //coeff0 = a[j];
        //coeff1 = a[j + len];

        // load coeff0
        asm volatile("mv t0, s2");
        asm volatile("slli t0, t0, 0x2");
        asm volatile("add t0, a0, t0");
        asm volatile("lw s6, 0(t0)");

        // load coeff1
        asm volatile("add t1, s2, s9");
        asm volatile("slli t1, t1, 0x2");
        asm volatile("add t1, a0, t1");
        asm volatile("lw s7, 0(t1)");

        // Compute butterfly operation
#ifndef STEEL
        asm volatile ("nop"); //ORCA
        asm volatile ("nop"); //ORCA
        asm volatile ("nop"); //ORCA cached
#endif
        asm volatile("fpau.dil.bfinv %0, %1,%2\n":"=r"(coeff0):"r"(coeff1),"r"(zeta): );

        // Store results in same position
        asm volatile("sw s6, 0(t0)");
        asm volatile("nop"); //ORCA and STEEL (2nd output written in next cycle)
        asm volatile("sw s7, 0(t1)");
#else
        t = a[j];
        a[j] = t + a[j + len];
        a[j + len] = t - a[j + len];
        a[j + len] = montgomery_reduce((int64_t)zeta * a[j + len]);
#endif
      }
    }
  }

#ifdef FPAU
  for(j = 0; j < N; ++j) {
        // load coeff
        asm volatile("mv t0, s2");
        asm volatile("slli t0, t0, 0x2");
        asm volatile("add t0, a0, t0");
        asm volatile("lw s6, 0(t0)");

        // multiply by ninv
        asm volatile("li s7, 0"); // coeff1

#ifndef STEEL
        asm volatile("nop"); //ORCA
        asm volatile("nop"); //ORCA
        asm volatile("nop"); //ORCA
#endif
        asm volatile("fpau.dil.bfinv %0, %1,%2\n":"=r"(coeff0):"r"(coeff1),"r"(ninv): );  //bfinv instead of mac to not overwrite ninv constant
        asm volatile("nop");
        asm volatile("nop"); //ORCA and STEEL (2nd output written in next cycle)

        // store result
        asm volatile("sw s7, 0(t0)");
  }
#else
  for(j = 0; j < N; ++j) {
    a[j] = montgomery_reduce((int64_t)f * a[j]);
  }
#endif
}
