#pragma once
#include "dissect_nas_5gs.hh"

/* 9.7  Message type */

/* 5GS mobility management messages */
namespace mm {
__declspec(selectany) extern const dissect_fnc_t no_dissect = nullptr;

int registration_req(dissector d, context* ctx);
int registration_accept(dissector d, context* ctx);
int registration_complete(dissector d, context* ctx);
int registration_rej(dissector d, context* ctx);
int dissect_reg_req_ue_orig(dissector d, context* ctx);
int dissect_registration_req_ue_term(dissector d, context* ctx);
int service_req(dissector d, context* ctx);
int service_rej(dissector d, context* ctx);
int service_acc(dissector d, context* ctx);

int conf_upd_cmd(dissector d, context* ctx);
int conf_update_comp(dissector d, context* ctx);
int authentication_req(dissector d, context* ctx);
int authentication_resp(dissector d, context* ctx);
int authentication_rej(dissector d, context* ctx);
int authentication_failure(dissector d, context* ctx);
int authentication_result(dissector d, context* ctx);
int id_req(dissector d, context* ctx);
int id_resp(dissector d, context* ctx);
int sec_mode_cmd(dissector d, context* ctx);
int sec_mode_comp(dissector d, context* ctx);
int sec_mode_rej(dissector d, context* ctx);

int mm_status(dissector d, context* ctx);
int notification(dissector d, context* ctx);
int notification_resp(dissector d, context* ctx);
int ul_nas_transp(dissector d, context* ctx);
int dl_nas_transp(dissector d, context* ctx);

__declspec(selectany) extern const message_meta msgs[] = {
    {0x41, "Registration request",                    registration_req},
    {0x42, "Registration accept",                     registration_accept},
    {0x43, "Registration complete",                   registration_complete},
    {0x44, "Registration reject",                     registration_rej},
    {0x45, "Deregistration request (UE originating)", dissect_reg_req_ue_orig},
    {0x46, "Deregistration accept (UE originating)",  nullptr},
    {0x47, "Deregistration request (UE terminated)",  dissect_registration_req_ue_term},
    {0x48, "Deregistration accept (UE terminated)",   nullptr},

    {0x49, "Not used in current version",             no_dissect},
    {0x4a, "Not used in current version",             no_dissect},
    {0x4b, "Not used in current version",             no_dissect},

    {0x4c, "Service request",                         service_req},
    {0x4d, "Service reject",                          service_rej},
    {0x4e, "Service accept",                          service_acc},

    {0x4f, "Not used in current version",             no_dissect},
    {0x50, "Not used in current version", no_dissect},
    {0x51, "Not used in current version", no_dissect},
    {0x52, "Not used in current version", no_dissect},
    {0x53, "Not used in current version", no_dissect},

    {0x54, "Configuration update command", conf_upd_cmd},
    {0x55, "Configuration update complete", conf_update_comp},
    {0x56, "Authentication request", authentication_req},
    {0x57, "Authentication response", authentication_resp},
    {0x58, "Authentication reject", authentication_rej},
    {0x59, "Authentication failure", authentication_failure},

    {0x5a, "Authentication result", authentication_result},
    {0x5b, "Identity request", id_req},
    {0x5c, "Identity response", id_resp},
    {0x5d, "Security mode command", sec_mode_cmd},
    {0x5e, "Security mode complete", sec_mode_comp},
    {0x5f, "Security mode reject", sec_mode_rej},

    {0x60, "Not used in current version", no_dissect},
    {0x61, "Not used in current version", no_dissect},
    {0x62, "Not used in current version", no_dissect},
    {0x63, "Not used in current version", no_dissect},
    {0x64, "5GMM status", mm_status},
    {0x65, "Notification", notification},
    {0x66, "Notification response", notification_resp},
    {0x67, "UL NAS transport", ul_nas_transp},
    {0x68, "DL NAS transport", dl_nas_transp},
    {0, nullptr, nullptr},
};

__declspec(selectany) extern const field_meta hf_nas_5gs_spare_b7 = {
    "Spare",
    "nas_5gs.spare_b7",
    ft::ft_uint8,
    fd::base_dec,
    nullptr,
    nullptr,
    nullptr,
    0x80,
};
__declspec(selectany) extern const field_meta hf_nas_5gs_spare_b3 = {
    "Spare",
    "nas_5gs.spare_b3",
    ft::ft_uint8,
    fd::base_dec,
    nullptr,
    nullptr,
    nullptr,
    0x04,
};
__declspec(selectany) extern const field_meta hf_nas_5gs_spare_b2 = {
    "Spare",
    "nas_5gs.spare_b2",
    ft::ft_uint8,
    fd::base_dec,
    nullptr,
    nullptr,
    nullptr,
    0x02,
};

__declspec(selectany) extern const true_false_string tfs_requested_not_requested = {
    "Requested",
    "Not Requested",
};

int dissect_allowed_nssai(dissector d, context* ctx);

__declspec(selectany) extern const element_meta allowed_nssai = {
    0x15,
    "Allowed NSSAI",
    dissect_allowed_nssai,
};

int dissect_ta_id_list(dissector d, context* ctx);

__declspec(selectany) extern const element_meta ta_id_list = {
    0x54,
    "TAI list",
    dissect_ta_id_list,
};

int dissect_e212_mcc_mnc(dissector d, context* ctx);

extern const field_meta hf_tac;
extern const field_meta hf_tal_num_e;
extern const field_meta hf_tal_t_li;

__declspec(selectany) extern const field_meta hf_mm_length = {
    "Length",
    "nas_5gs.mm.length",
    ft::ft_uint8,
    fd::base_dec,
    nullptr,
    nullptr,
    nullptr,
    0x0,
};
int dissect_dnn(dissector d, context* ctx);
int dissect_nssai(dissector d, context* ctx);

extern const field_meta* hf_nas_5gs_mm_raai_b0;

//  9.11.3.46    Rejected NSSAI
int                       dissect_rej_nssai(dissector d, context* ctx);
__declspec(selectany) extern const element_meta rej_nssai = {
    0x11,
    "Rejected NSSAI",
    dissect_rej_nssai,
};

int dissect_configured_nssai(dissector d, context* ctx);
__declspec(selectany) extern const element_meta configured_nssai = {
    0x31,
    "Configured NSSAI",
    dissect_configured_nssai,
};

int                dissect_pdu_ses_react_res(dissector d, context* ctx = nullptr);
__declspec(selectany) extern const element_meta pdu_ses_react_res = {
    0x26,
    "PDU session reactivation result",
    dissect_pdu_ses_react_res,
};

/* 9.11.3.44    PDU session status */
int dissect_pdu_ses_status(dissector d, context* ctx = nullptr);

__declspec(selectany) extern const element_meta pdu_ses_status = {
    0x50,
    "PDU session status",
    dissect_pdu_ses_status,
};
extern const val_string values_cause[];
__declspec(selectany) extern const field_meta hfm_mm_cause = {
    "5GMM cause",
    "nas_5gs.mm.5gmm_cause",
    ft::ft_uint8,
    fd::base_dec,
    values_cause,
    nullptr,
    nullptr,
    0x0,
};
__declspec(selectany)  extern const field_meta* hf_mm_cause = &hfm_mm_cause;
int dissect_mm_cause(dissector d, context* ctx);

int dissect_ladn_inf(dissector d, context* ctx);
int dissect_mico_ind(dissector d, context* ctx);
int dissect_sal(dissector d, context* ctx);

extern const true_false_string tfs_nas_5gs_sal_al_t;
extern const val_string        nas_5gs_mm_sal_t_li_values[];
} // namespace mm


__declspec(selectany) extern const element_meta mm_cause = {
    0xff,
    "5GMM cause",
    mm::dissect_mm_cause,
};



int                       dissect_nas_ksi(dissector d, context* ctx);
__declspec(selectany) extern const element_meta nas_ksi = {
    0xff,
    "NAS key set identifier",
    dissect_nas_ksi,
};

int dissect_mobile_id(dissector d, context* ctx);

__declspec(selectany) extern const element_meta mobile_id = {
    0xff,
    "Mobile identity",
    dissect_mobile_id,
};


int dissect_nas_msg_cont(dissector d, context* ctx = nullptr);

__declspec(selectany) extern const element_meta nas_msg_cont = {
    0x71,
    "NAS message container",
    dissect_nas_msg_cont,
};

int dissect_abba(dissector d, context* ctx);

__declspec(selectany) extern const element_meta abba = {
    0x38,
    "ABBA",
    dissect_abba,
};

