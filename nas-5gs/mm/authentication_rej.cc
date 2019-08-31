#include "../dissect_mm_msg.hh"
#include "../ts24007.hh"

namespace mm_authentication_rej {

}
using namespace nas;

/*
 * 8.2.5 Authentication reject
 */
int mm::authentication_rej(dissector d, context* ctx) {
    auto        len = d.length;
    use_context uc(ctx, "authentication-reject");

    using namespace mm_authentication_rej;

    /* 78    EAP message    EAP message 9.11.2.2    O    TLV-E    7-1503 */
    // ELEM_OPT_TLV_E(0x78, NAS_5GS_PDU_TYPE_COMMON, DE_NAS_5GS_CMN_EAP_MESSAGE, NULL);
    auto consumed = dissect_opt_elem_tlv_e(nullptr, &eap_msg, d, ctx);
    d.step(consumed);

    d.extraneous_data_check(0);
    return len;
}
namespace mm_authentication_rej {

}
