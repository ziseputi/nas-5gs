#include "../common/dissector.hh"
#include "../common/use_context.hh"


/* 9.11.3.30    LADN information  */
int dissect_ladn_information(dissector d, context* ctx) {
    const use_context uc(&d, ctx, "ladn-information", 0);
    return uc.length;
}
