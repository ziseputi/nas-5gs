#include "../dissect_mm_msg.hh"
#include "../ber.hh"


/* 8.2.4 Authentication failure */
int mm::authentication_failure(dissector d, context* ctx) {    
    const use_context uc(ctx, "authentication-failure", d, 0);
    // UE to network
    up_link(d.pinfo);

    /* 5GMM cause   5GMM cause     9.11.3.2  M   V   1 */
    auto consumed = dissect_mm_cause(d, ctx);
    d.step(consumed);

    /* 30    Authentication failure parameter   9.11.3.14    O    TLV    16 */
    consumed = dissect_opt_tlv(nullptr, &auth_fail_param, d, ctx);
    d.step(consumed);
    
    return uc.length;
}
