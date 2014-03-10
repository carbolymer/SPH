#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#define _NUMBER_OF_STEPS_ 5e3
#define _TIME_STEP_ 7e-6
// diffraction
//define _NUMBER_OF_STEPS_ 2e4
//define _TIME_STEP_ 1e-6

#define _BENCHMARK_DISPLAY_STEP_ 0.05

// engine config
#define _NG_H_ 1              // smoothing length
#define _NG_GAMMA_ 1e12          // gamma
#define _NG_C_ 2000           // sound velocity
#define _NG_SIGMA_ 1e6        // surface tension
#define _NG_CALCULATE_ENERGY_ 1

//hashtable
#define _HT_P1_ 73856093
#define _HT_P2_ 19349663
#define _HT_NH_ 4861

#endif