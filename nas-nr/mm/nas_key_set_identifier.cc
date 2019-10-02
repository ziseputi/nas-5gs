#include "../common/dissector.hh"
#include "../common/use_context.hh"

// 9.11.3.32	NAS key set identifier
int dissect_nas_ksi(dissector d, context* ctx) {
    use_context uc(&d, ctx, "NAS key set identifier", -1);

    return 1;
}
