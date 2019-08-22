#include "../dissect_mm_msg.hh"
#include "../ts24007.hh"

/*
 * 8.2.26 Security mode complete
 */
int mm::sec_mode_comp(dissector d, context* ctx) {

    /* 77    IMEISV    5G mobile identity 9.11.3.4    O    TLV-E    11 */
    // ELEM_OPT_TLV_E(0x77, NAS_5GS_PDU_TYPE_MM, DE_NAS_5GS_MM_5GS_MOBILE_ID, NULL);
    auto consumed = dissect_opt_elem_tlv_e(nullptr, &mobile_id, d);
    d.offset += consumed;
    d.length -= consumed;

    /* 71    NAS message container    NAS message container 9.11.3.33    O    TLV-E    4-n
     */
    // ELEM_OPT_TLV_E(0x71, NAS_5GS_PDU_TYPE_MM, DE_NAS_5GS_MM_NAS_MSG_CONT, NULL);
    consumed = dissect_opt_elem_tlv_e(nullptr, &nas_msg_cont, d);
    d.offset += consumed;
    d.length -= consumed;

    extraneous_data_check(d.pinfo, d.tree, d.tvb, d.offset, d.length, 0);
    return d.tvb->reported_length;
}
