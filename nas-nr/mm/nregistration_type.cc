#include "../common/dissector.hh"
#include "../common/use_context.hh"

/* 9.11.3.7    5GS registration type */
int dissect_registration_requestuest_type(dissector d, context* ctx) {
    const use_context uc(ctx, "5gs-registration-type", d, -1);

    return 1;
}