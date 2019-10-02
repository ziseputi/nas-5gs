#include "../common/dissector.hh"
#include "../common/use_context.hh"

/*  8.2.29 5GMM status */
int dissect_mm_status(dissector d, context* ctx) {
    const use_context uc(&d, ctx, "5gmm-status", -1);

    /* Direction: both*/

    /* 5GMM cause 9.11.3.2    M    V    1 */


    return 1;
}
