#include "../dissect_sm_msg.hh"

const true_false_string tfs_pdu_always_on_ses_id_apsi = {
    "Always-on PDU session allowed",
    "Always-on PDU session not allowed"};

const field_meta hf_pdu_always_on_ses_ind = {
    "Always-on PDU session indication",
    "nas_5gs.sm.pdu.ses_id",
    ft::ft_uint8,
    fd::base_dec,
    nullptr,
    &tfs_pdu_always_on_ses_id_apsi,
    nullptr,
    0x01,
};

/*  9.11.4.3 Always-on PDU session indication */
int sm::dissect_always_on_pdu_ses_ind(dissector d, context* ctx) {
    const use_context uc(ctx, "always-on-pdu-session-indication", d, -1);

    auto i = d.add_item(1, &hf_pdu_always_on_ses_ind, enc::be);
    unused(i);
    d.step(1);

    return 1;
}


// Always-on PDU session indication 9.11.4.3
const element_meta sm::alwayson_pdu_ses_ind = {
    0x80,
    "Always-on PDU session indication",
    dissect_always_on_pdu_ses_ind,
    nullptr,
};