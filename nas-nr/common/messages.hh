#pragma once
#include <cstdint>

#include "ber.hh"
#include "definitions.hh"
#include "dissectors.hh"
#include "ies.hh"
#include "nas.hh"

using bit4_t   = uint8_t;
using octet_16 = uint8_t[16];

/*
Table 8.2.1.1.1: AUTHENTICATION REQUEST message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Authentication request message identity	Message type	9.7	M	V	1
    ngKSI 	NAS key set identifier	9.11.3.32	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    ABBA	ABBA	9.11.3.10	M	LV	3-n
21	Authentication parameter RAND (5G authentication challenge)	9.11.3.16	O	TV	17
20	Authentication parameter AUTN (5G authenticationchallenge)	9.11.3.15	O	TLV	18
78	EAP message	EAP message	9.11.2.2	O	TLV-E	7-1503
*/
struct authentication_request_t {
    nmm_header_t     header = {};
    bit4_t           nksi   = {}; // 9.11.3.32	M	V
    octet_t          abba   = {}; // ABBA	9.11.3.10	M	LV	3-n
    opt_t< octet_g > rand   = {}; // 21 9.11.3.16 O TV 17
    opt_t< octet_g > autn   = {}; // 20 9.11.3.15	TLV	18
    opt_t< octet_t > eap    = {}; // 78	9.11.2.2 TLVE
};

result_t de_authentication_request(dissector                 d,
                                   context*                  ctx,
                                   authentication_request_t* ret) {
    const use_context uc(&d, ctx, "authentication-request", 0);

    de_nmm_header(d, ctx, &ret->header).step(d);

    /*
    Table 8.2.1.1.1: AUTHENTICATION REQUEST message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Authentication request message identity	Message type	9.7	M	V	1

        // ngKSI 	NAS key set identifier	9.11.3.32	M	V	1/2
        de_nibble(d, ctx, &ret->nksi).step(d);

        // Spare half octet	Spare half octet	9.5	M	V	1/2

        // ABBA	ABBA	9.11.3.10	M	LV	3-n
        de_lv_octet(d, ctx, &ret->abba).step(d);

    // 21	Authentication parameter RAND (5G authentication challenge)	9.11.3.16	O TV
    17 de_t_fixed(d, ctx, 0x21, &ret->rand.v).step(d);

    // 20	Authentication parameter AUTN (5G authenticationchallenge)	9.11.3.15	O
    TLV	18 de_tl_fixed(d, ctx, 0x20, &ret->autn).step(d);

    // 78	EAP message	EAP message	9.11.2.2	O	TLV-E	7-1503
    de_tlve_octet(d, ctx, 0x78, &ret->eap).step(d);
    */

    return {d.offset - uc.offset};
}

/*
Table 8.2.2.1.1: AUTHENTICATION RESPONSE message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Authentication response message identity	Message type	9.7	M	V	1
2D	Authentication response parameter	9.11.3.17 O TLV	18
78	EAP message	9.11.2.2	O	TLV-E	7-1503
*/
struct authentication_response_t {
    nmm_header_t     header    = {};
    opt_t< octet_g > parameter = {}; // 2D 9.11.3.17 TV 18
    opt_t< octet_t > eap       = {}; // 78 9.11.2.2 TLV-E 7+
};

result_t de_authentication_response(dissector                  d,
                                    context*                   ctx,
                                    authentication_response_t* ret) {
    const use_context uc(&d, ctx, "authentication-response", 0);

    de_nmm_header(d, ctx, &ret->header).step(d);
    /*
    Table 8.2.2.1.1: AUTHENTICATION RESPONSE message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Authentication response message identity	Message type	9.7	M	V	1

    // 2D	Authentication response parameter	9.11.3.17 O TLV	18
    de_tl_fixed(d, ctx, 0x2d, &ret->parameter).step(d);

    // 78	EAP message	9.11.2.2	O	TLV-E	7-1503
    de_tlve_octet(d, ctx, 0x78, &ret->eap).step(d);
    */

    return {d.offset - uc.offset};
}

/*
Table 8.2.3.1.1: AUTHENTICATION RESULT message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Authentication result message identity	Message type	9.7	M	V	1
    ngKSI 	NAS key set identifier	9.11.3.32	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    EAP message	EAP message	9.11.2.2	M	LV-E	6-1502
38	ABBA	ABBA	9.11.3.10	O	TLV	4-n
*/
struct authentication_result_t {
    nmm_header_t     header = {};
    bit4_t           nksi   = {}; // 9.11.3.32 M V 1/2
    octet_t          eap    = {}; // 9.11.2.2 LV-E 6+
    opt_t< octet_t > abba   = {}; // 38 9.11.3.10 TLV 4
};

result_t de_authentication_result(dissector                d,
                                  context*                 ctx,
                                  authentication_result_t* ret) {
    const use_context uc(&d, ctx, "authentication-result", 0);

    de_nmm_header(d, ctx, &ret->header).step(d);

    /*
    Table 8.2.3.1.1: AUTHENTICATION RESULT message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Authentication result message identity	Message type	9.7	M	V	1

        // ngKSI 	NAS key set identifier	9.11.3.32	M	V	1/2
        de_nibble(d, ctx, &ret->nksi).step(d);

        // Spare half octet	Spare half octet	9.5	M	V	1/2

        // EAP message	EAP message	9.11.2.2	M	LV-E	6-1502
        de_lve_octet(d, ctx, &ret->eap).step(d);

    // 38	ABBA	ABBA	9.11.3.10	O	TLV	4-n
    de_tlv_octet(d, ctx, 0x38, &ret->abba).step(d);
    */

    return {d.offset - uc.offset};
}

/*
Table 8.2.4.1.1: AUTHENTICATION FAILURE message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Authentication failure message identity	Message type	9.7	M	V	1
    5GMM cause	5GMM cause	9.11.3.2	M	V	1
30	Authentication failure parameter	9.11.3.14 O TLV	16
*/
struct authentication_failure_t {
    nmm_header_t     header    = {};
    uint8_t          cause     = {}; // 9.11.3.2 V 1
    opt_t< octet_e > parameter = {}; // 30 9.11.3.14 TLV 16
};

result_t de_authentication_failure(dissector               d,
                                   context*                ctx,
                                   authentication_failure_t* ret) {
    const use_context uc(&d, ctx, "authentication-failure", 0);

    de_nmm_header(d, ctx, &ret->header).step(d);

    /*
    Table 8.2.4.1.1: AUTHENTICATION FAILURE message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Authentication failure message identity	Message type	9.7	M	V	1

        // 5GMM cause	5GMM cause	9.11.3.2	M	V	1
        de_uint8(d, ctx, &ret->cause).step(d);

    // 30	Authentication failure parameter	9.11.3.14 O TLV	16
    de_tl_fixed(d, ctx, 0x30, &ret->parameter).step(d);
    */

    return {d.offset - uc.offset};
}

/*
Table 8.2.5.1.1: AUTHENTICATION REJECT message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Authentication reject message identity	Message type	9.7	M	V	1
78	EAP message	9.11.2.2	O	TLV-E	7-1503
*/
struct authentication_reject_t {
    nmm_header_t     header = {};
    opt_t< octet_t > eap = {}; // 78 9.11.2.2 TLV-E 7+
};

result_t de_authentication_reject(dissector                d,
                                  context*                 ctx,
                                  authentication_reject_t* ret) {
    const use_context uc(&d, ctx, "authentication-reject", 0);

    de_nmm_header(d, ctx, &ret->header).step(d);
    /*
    Table 8.2.5.1.1: AUTHENTICATION REJECT message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Authentication reject message identity	Message type	9.7	M	V	1

    // 78	EAP message	9.11.2.2	O	TLV-E	7-1503
    de_tlve_octet(d, ctx, 0x78, &ret->eap).step(d);
    */
    de_tl_octet(d, ctx, 0x78, &ret->eap).step(d);

    return {d.offset - uc.offset};
}

/*
Table 8.2.6.1.1: REGISTRATION REQUEST message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended Protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Registration request message identity	Message type	9.7	M	V	1
    5GS registration type	5GS registration type	9.11.3.7	M	V	1/2
    ngKSI	NAS key set identifier	9.11.3.32	M	V	1/2
    5GS mobile identity	5GS mobile identity	9.11.3.4	M	LV-E	6-n
C-	Non-current native NAS key set identifier	9.11.3.32 O	TV	1
10	5GMM capability	5GMM capability	9.11.3.1	O	TLV	3-15
2E	UE security capability	UE security capability	9.11.3.54	O	TLV	4-10
2F	Requested NSSAI	NSSAI	9.11.3.37	O	TLV	4-74
52	Last visited registered TAI	9.11.3.8	O	TV	7
17	S1 UE network capability	9.11.3.48	O	TLV	4-15
40	Uplink data status	9.11.3.57	O	TLV	4-34
50	PDU session status	9.11.3.44	O	TLV	4-34
B-	MICO indication	9.11.3.31	O	TV	1
2B	UE status	UE status	9.11.3.56	O	TLV	3
77	Additional GUTI	5GS mobile identity	9.11.3.4	O	TLV-E	14
25	Allowed PDU session status	9.11.3.13	O	TLV	4-34
18	UE's usage setting	9.11.3.55	O	TLV	3
51	Requested DRX parameters	9.11.3.2A	O	TLV	3
70	EPS NAS message container	9.11.3.24	O	TLV-E	4-n
74	LADN indication	LADN indication	9.11.3.29	O	TLV-E	3-811
8-	Payload container type	9.11.3.40	O	TV	1
7B	Payload container	9.11.3.39	O	TLV-E	4-65538
9-	Network slicing indication	9.11.3.36	O	TV	1
53	5GS update type	9.11.3.9A	O	TLV	3
TBD	Mobile station classmark 2	9.11.3.61	O	TLV	5
TBD	Supported codecs	9.11.3.62	O	TLV	5-n
71	NAS message container	9.11.3.33	O	TLV-E	4-n
60	EPS bearer context status	9.11.3.60	O	TLV	4
XX	Requested extended DRX parameters	9.11.3.60	O	TLV	3
TBD	T3324 value	GPRS timer 3	9.11.2.5	O	TLV	3
*/
struct registration_request_t {
    nmm_header_t      header                            = {};
    bit4_t            nregistration_type                = {}; // 9.11.3.7 V 1/2
    bit4_t            nksi                              = {}; // 9.11.3.32 V 1/2
    octet_t           nmid                              = {}; // 9.11.3.5 LV-E 6+
    opt_t< bit4_t >   native_nksi                       = {}; // C- 9.11.3.32 TV 1
    opt_t< octet_t >  nmm_capability                    = {}; // 10 9.11.3.1 TLV 3+
    opt_t< octet_t >  security_capability               = {}; // 2E 9.11.3.54 TLV 4+
    opt_t< octet_t >  requested_nssai                   = {}; // 2F TLV 4+ 9.11.3.37
    opt_t< octet_6 >  last_visited_tai                  = {}; // 52 TV 7 9.11.3.8
    opt_t< octet_t >  s1_ue_network_capability          = {}; // 17 TLV 4+ 9.11.3.48
    opt_t< octet_t >  uplink_data_status                = {}; // 40 TLV 4+ 9.11.3.57
    opt_t< octet_t >  pdu_session_status                = {}; // 50 TLV 4+ 9.11.3.44
    opt_t< bit4_t >   mico_ind                          = {}; // B- TV 1 9.11.3.31
    opt_t< uint8_t >  ue_status                         = {}; // 2B TLV 3 9.11.3.56
    opt_t< octet_b >  additional_guti_mid               = {}; // 77 TLV-E 14 9.11.3.4
    opt_t< octet_t >  allowed_pdu_session_status        = {}; // 25 TLV 4+ 9.11.3.13
    opt_t< uint8_t >  ue_usage_setting                  = {}; // 18 TLV 3 9.11.3.55
    opt_t< uint8_t >  requested_drx_parameters          = {}; // 51 TLV 3 9.11.3.2A
    opt_t< octet_t >  eps_nas_container                 = {}; // 70 TLV-E 4+
    opt_t< octet_t >  ladn_ind                          = {}; // 74 TLV-E 3+
    opt_t< bit4_t >   payload_container_type            = {}; // 8- TV 1
    opt_t< octet_t >  payload_container                 = {}; // 7B TLV-E
    opt_t< bit4_t >   network_slicing_ind               = {}; // 9- TV 1
    opt_t< uint8_t >  nupdate_type                      = {}; // 53 TLV 3
    opt_t< octet_3 >  mobile_station_classmark2         = {}; // TBD TLV 5
    opt_t< octet_t >  supported_codecs                  = {}; // TBD TLV 5+
    opt_t< octet_t >  nas_message_container             = {}; // 71 TLV-E 4+
    opt_t< uint16_t > eps_bearer_context_status         = {}; // 60 TLV 4
    opt_t< uint8_t >  requested_extended_drx_parameters = {}; // XX TLV 3
    opt_t< uint8_t >  t3324                             = {}; // TBD TLV 3
};

result_t de_registration_request(dissector d, context* ctx, registration_request_t* ret) {
    const use_context uc(&d, ctx, "registration-request", 0);

    de_nmm_header(d, ctx, &ret->header).step(d);

    /*
    Table 8.2.6.1.1: REGISTRATION REQUEST message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended Protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Registration request message identity	Message type	9.7	M	V	1

    */

    // 5GS registration type	5GS registration type	9.11.3.7	M	V	1/2
    de_nibble(d, ctx, &ret->nregistration_type);

    // ngKSI	NAS key set identifier	9.11.3.32	M	V	1/2
    de_nibble(d, ctx, &ret->nksi).step(d);

    // 5GS mobile identity	5GS mobile identity	9.11.3.4	M	LV-E	6-n
    de_le_octet(d, ctx, &ret->nmid).step(d);

    // C-	Non-current native NAS key set identifier	9.11.3.32 O	TV	1
    de_tv_short(d, ctx, 0xc0, &ret->native_nksi).step(d);

    // 10	5GMM capability	5GMM capability	9.11.3.1	O	TLV	3-15
    de_tl_octet(d, ctx, 0x10, &ret->nmm_capability).step(d);

    // 2E	UE security capability	UE security capability	9.11.3.54	O	TLV	4-10
    de_tl_octet(d, ctx, 0x2e, &ret->security_capability).step(d);

    // 2F	Requested NSSAI	NSSAI	9.11.3.37	O	TLV	4-74
    de_tl_octet(d, ctx, 0x2f, &ret->requested_nssai).step(d);

    // 52	Last visited registered TAI	9.11.3.8	O	TV	7
    de_t_fixed(d, ctx, 0x52, &ret->last_visited_tai).step(d);

    // 17	S1 UE network capability	9.11.3.48	O	TLV	4-15
    de_tl_octet(d, ctx,0x17, &ret->s1_ue_network_capability).step(d);

    // 40	Uplink data status	9.11.3.57	O	TLV	4-34
    de_tl_octet(d, ctx, 0x40, &ret->uplink_data_status).step(d);

    // 50	PDU session status	9.11.3.44	O	TLV	4-34
    de_tl_octet(d, ctx, 0x50, &ret->pdu_session_status).step(d);

    // B-	MICO indication	9.11.3.31	O	TV	1
    de_tv_short(d, ctx, 0xb0, &ret->mico_ind).step(d);

    // 2B	UE status	UE status	9.11.3.56	O	TLV	3
    de_tl_uint8(d, ctx, 0x2b, &ret->ue_status).step(d);

    // 77	Additional GUTI	5GS mobile identity	9.11.3.4	O	TLV-E	14
    de_tle_fixed(d, ctx, 0x77, &ret->additional_guti_mid).step(d);

    // 25	Allowed PDU session status	9.11.3.13	O	TLV	4-34
    de_tl_octet(d, ctx, 0x25, &ret->allowed_pdu_session_status).step(d);

    // 18	UE's usage setting	9.11.3.55	O	TLV	3
    de_tl_uint8(d, ctx, 0x18, &ret->ue_usage_setting).step(d);

    // 51	Requested DRX parameters	9.11.3.2A	O	TLV	3
    de_tl_uint8(d, ctx, 0x51, &ret->requested_drx_parameters).step(d);

    // 70	EPS NAS message container	9.11.3.24	O	TLV-E	4-n
    de_tle_octet(d, ctx, 0x70, &ret->eps_nas_container).step(d);

    // 74	LADN indication	LADN indication	9.11.3.29	O	TLV-E	3-811
    de_tle_octet(d, ctx, 0x74, &ret->ladn_ind).step(d);

    // 8-	Payload container type	9.11.3.40	O	TV	1
    de_tv_short(d, ctx, 0x80, &ret->payload_container_type).step(d);

    // 7B	Payload container	9.11.3.39	O	TLV-E	4-65538
    de_tle_octet(d, ctx, 0x7b, &ret->payload_container).step(d);

    // 9-	Network slicing indication	9.11.3.36	O	TV	1
    de_tv_short(d, ctx, 0x90, &ret->network_slicing_ind).step(d);

    // 53	5GS update type	9.11.3.9A	O	TLV	3
    de_tl_uint8(d, ctx, 0x53, &ret->nupdate_type).step(d);

    // TBD	Mobile station classmark 2	9.11.3.61	O	TLV	5
    // TBD	Supported codecs	9.11.3.62	O	TLV	5-n

    // 71	NAS message container	9.11.3.33	O	TLV-E	4-n
    de_tle_octet(d, ctx, 0x71, &ret->nas_message_container).step(d);

    // 60	EPS bearer context status	9.11.3.60	O	TLV	4
    de_tl_uint16(d, ctx, 0x60, &ret->eps_bearer_context_status).step(d);

    // XX	Requested extended DRX parameters	9.11.3.60	O	TLV	3

    // TBD	T3324 value	GPRS timer 3	9.11.2.5	O	TLV	3
    return {d.offset - uc.offset};
}

struct registration_accept_t {
    nmm_header_t      header = {};
    uint8_t           nregistration_result;                        // LV 2
    opt_t< octet_b >  guti_nmid;                                   // 77 TLV-E 14
    opt_t< octet_t >  equivalent_plmns;                            // 4A TLV 5+
    opt_t< octet_t >  tai_list;                                    // 54 TLV 9+
    opt_t< octet_t >  allowed_nssai;                               // 15 TLV 4+
    opt_t< octet_t >  rejected_nssai;                              // 11 TLV 4+
    opt_t< octet_t >  configured_nssai;                            // 31 TLV 4+
    opt_t< octet_t >  nnetwork_feature_support;                    // 21 TLV 3+
    opt_t< octet_t >  pdu_session_status;                          // 50 TLV 4+
    opt_t< octet_t >  pdu_session_reactivation_result;             // 26 TLV 4+
    opt_t< octet_t >  pdu_session_reactivation_result_error_cause; // 72 TLV-E 5+
    opt_t< octet_t >  ladn_information;                            // 79 TLV-E 12+
    opt_t< bit4_t >   mico_ind;                                    // B- TV 1
    opt_t< bit4_t >   network_slicing_ind;                         // 9- TV 1
    opt_t< octet_t >  service_area_list;                           // 27 TLV 6+
    opt_t< uint8_t >  t3512;                                       // 5E TLV 3
    opt_t< uint8_t >  n3_deregistration_timer;                     // 5D TLV 3
    opt_t< uint8_t >  t3502;                                       // 16 TLV 3
    opt_t< octet_t >  emergency_numbers;                           // 34 TLV 5+
    opt_t< octet_t >  extended_emergency_numbers;                  // 7A TLV-E 7+
    opt_t< octet_t >  sor_container;                               // 73 TLV-E 20+
    opt_t< octet_t >  eap;                                         // 78 TLV-E 7+
    opt_t< bit4_t >   nssai_inclusion_mode;                        // A- TV 1
    opt_t< octet_t >  access_categories;                           // 76 TLV-E
    opt_t< uint8_t >  negotiated_drx_parameters;                   // 51 TLV 3
    opt_t< bit4_t >   n3_nw_provided_policies;                     // D- TV 1
    opt_t< uint16_t > eps_bearer_context_status;                   // 60 TLV 4
    opt_t< uint8_t >  negotiated_extended_drx_parameters;          // XX TLV 3
    opt_t< uint8_t >  t3447;                                       // TBD TVL 3
    opt_t< uint8_t >  t3348;                                       // XX TLV 3
    opt_t< uint8_t >  t3324;                                       // TBD TLV 3
};

result_t de_registration_accept(dissector d, context* ctx, registration_accept_t* ret) {
    const use_context uc(&d, ctx, "registration-accept", 0);

    de_nmm_header(d, ctx, &ret->header).step(d);

    /*
    Table 8.2.7.1.1: REGISTRATION ACCEPT message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Registration accept message identity	Message type	9.7	M	V	1
    */
    // 5GS registration result	5GS registration result	9.11.3.6	M	LV	2
    de_l_uint8(d, ctx, &ret->nregistration_result).step(d);

    // 77	5G-GUTI	5GS mobile identity	9.11.3.4	O	TLV-E	14
    de_tle_fixed(d, ctx, 0x77, &ret->guti_nmid).step(d);

    // 4A	Equivalent PLMNs	PLMN list	9.11.3.45	O	TLV	5-47
    de_tl_octet(d, ctx, 0x4a, &ret->equivalent_plmns).step(d);

    // 54	TAI list	5GS tracking area identity list	9.11.3.9	O	TLV	9-114
    de_tl_octet(d, ctx, 0x54, &ret->tai_list).step(d);

    // 15	Allowed NSSAI	NSSAI	9.11.3.37	O	TLV	4-74
    de_tl_octet(d, ctx, 0x15, &ret->allowed_nssai).step(d);

    // 11	Rejected NSSAI	Rejected NSSAI	9.11.3.46	O	TLV	4-42
    de_tl_octet(d, ctx, 0x11, &ret->rejected_nssai).step(d);

    // 31	Configured NSSAI	NSSAI	9.11.3.37	O	TLV	4-146
    de_tl_octet(d, ctx, 0x31, &ret->configured_nssai).step(d);

    // 21	5GS network feature support	9.11.3.5	O	TLV	3-5
    de_tl_octet(d, ctx, 0x21, &ret->nnetwork_feature_support).step(d);

    // 50	PDU session status	PDU session status	9.11.3.44	O	TLV	4-34
    de_tl_octet(d, ctx, 0x50, &ret->pdu_session_status).step(d);

    // 26	PDU session reactivation result	9.11.3.42	O TLV 4-34
    de_tl_octet(d, ctx, 0x26, &ret->pdu_session_reactivation_result).step(d);

    // 72	PDU session reactivation result error cause	9.11.3.43	O	TLV-E	5-515
    de_tle_octet(d, ctx, 0x72, &ret->pdu_session_reactivation_result_error_cause)
        .step(d);

    // 79	LADN information	LADN information	9.11.3.30	O	TLV-E	12-1715
    de_tle_octet(d, ctx, 0x79, &ret->ladn_information).step(d);

    // B-	MICO indication	MICO indication	9.11.3.31	O	TV	1
    de_tv_short(d, ctx, 0xb0, &ret->mico_ind).step(d);

    // 9-	Network slicing indication	9.11.3.36	O	TV	1
    de_tv_short(d, ctx, 0x90, &ret->network_slicing_ind).step(d);

    // 27	Service area list	Service area list	9.11.3.49	O	TLV	6-114
    de_tl_octet(d, ctx, 0x27, &ret->service_area_list).step(d);

    // 5E	T3512 value	GPRS timer 3	9.11.2.5	O	TLV	3
    de_tl_uint8(d, ctx, 0x5e, &ret->t3512).step(d);

    // 5D	Non-3GPP de-registration GPRS timer 2	9.11.2.4	O	TLV	3
    de_tl_uint8(d, ctx, 0x5d, &ret->n3_deregistration_timer).step(d);

    // 16	T3502 value	GPRS timer 2	9.11.2.4	O	TLV	3
    de_tl_uint8(d, ctx, 0x16, &ret->t3502).step(d);

    // 34	Emergency number list	9.11.3.23	O	TLV	5-50
    de_tl_octet(d, ctx, 0x34, &ret->emergency_numbers).step(d);

    // 7A	Extended emergency number list	9.11.3.26	O TLV-E 7-65538
    de_tl_octet(d, ctx, 0x7a, &ret->extended_emergency_numbers).step(d);

    // 73	SOR transparent container 9.11.3.51	O TLV-E 20-n
    de_tle_octet(d, ctx, 0x73, &ret->sor_container).step(d);

    // 78	EAP message	EAP message	9.11.2.2	O	TLV-E	7-1503
    de_tle_octet(d, ctx, 0x78, &ret->eap).step(d);

    // A-	NSSAI inclusion mode	9.11.3.37A	O	TV	1
    de_tv_short(d, ctx, 0xa0, &ret->nssai_inclusion_mode).step(d);

    // 76	Operator-defined access category definitions	9.11.3.38	O	TLV-E	3-n
    de_tle_octet(d, ctx, 0x76, &ret->access_categories).step(d);

    // 51  5GS DRX parameters	9.11.3.2A	O	TLV	3
    de_tl_uint8(d, ctx, 0x51, &ret->negotiated_drx_parameters).step(d);

    // D-	Non-3GPP NW provided policies	9.11.3.58	O	TV	1
    de_tv_short(d, ctx, 0xd0, &ret->n3_nw_provided_policies).step(d);

    // 60	EPS bearer context status	9.11.3.59	O	TLV	4
    de_tl_uint16(d, ctx, 0x60, &ret->eps_bearer_context_status).step(d);

    // xx	Extended DRX parameters	9.11.3.60	O	TLV	3
    // tbd	T3447 value	GPRS timer 3	9.11.2.5	O	TLV	3
    // XX	T3448 value	GPRS timer 3	9.11.2.4	O	TLV	3
    // TBD T3324 value	GPRS timer 3	9.11.2.5	O	TLV	3
    /**/

    return {d.offset - uc.offset};
}

/*
Table 8.2.8.1.1: REGISTRATION COMPLETE message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Registration complete message identity	Message type	9.7	M	V	1
73	SOR transparent container	9.11.3.51	O	TLV-E	20
*/
struct registration_complete_t {
    nmm_header_t     header = {};
    opt_t< octet_t > sor_container={}; // 73 9.11.3.51 TLV-2
};

result_t de_registration_complet(dissector                d,
                                 context*                 ctx,
                                 registration_complete_t* ret) {
    const use_context uc(&d, ctx, "registration-complete", 0);

    de_nmm_header(d, ctx, &ret->header).step(d);
    /*
    Table 8.2.8.1.1: REGISTRATION COMPLETE message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Registration complete message identity	Message type	9.7	M	V	1
    */

    // 73	SOR transparent container	9.11.3.51	O	TLV-E	20
    de_tle_octet(d, ctx, 0x73, &ret->sor_container).step(d);
    return {d.offset - uc.offset};
}
/*
Table 8.2.9.1.1: REGISTRATION REJECT message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Registration reject message identity	Message type	9.7	M	V	1
    5GMM cause	5GMM cause	9.11.3.2	M	V	1
5F	T3346 value	GPRS timer 2	9.11.2.4	O	TLV	3
16	T3502 value	GPRS timer 2	9.11.2.4	O	TLV	3
78	EAP message	EAP message	9.11.2.2	O	TLV-E	7-1503
*/
struct registration_reject_t {
    nmm_header_t     header= {};
    uint8_t          cause= {}; // V 1
    opt_t< uint8_t > t3346= {}; // 5F TLV 3
    opt_t< uint8_t > t3502= {}; // 16 TLV 3
    opt_t< octet_t > eap= {};   // 78 TLV 7+
};

result_t de_registration_reject(dissector d, context* ctx, registration_reject_t* ret) {
    const use_context uc(&d, ctx, "registration-reject", 0);
    de_nmm_header(d, ctx, &ret->header).step(d);

    /*
    Table 8.2.9.1.1: REGISTRATION REJECT message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Registration reject message identity	Message type	9.7	M	V	1
    */

    // 5GMM cause	5GMM cause	9.11.3.2	M	V	1
    de_uint8(d, ctx, &ret->cause).step(d);

    // 5F	T3346 value	GPRS timer 2	9.11.2.4	O	TLV	3
    de_tl_uint8(d, ctx, 0x5f, &ret->t3346).step(d);

    // 16	T3502 value	GPRS timer 2	9.11.2.4	O	TLV	3
    de_tl_uint8(d, ctx, 0x16, &ret->t3502).step(d);

    // 78	EAP message	EAP message	9.11.2.2	O	TLV-E	7-1503
    de_tle_octet(d, ctx, 0x78, &ret->eap).step(d);
    return {uc.consumed()};
}

/*
Table 8.2.10.1.1: UL NAS TRANSPORT message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    UL NAS TRANSPORT message identity	Message type	9.7	M	V	1
    Payload container type	Payload container type	9.11.3.40	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Payload container	Payload container	9.11.3.39	M	LV-E	3-65537
12	PDU session ID	PDU session identity 2	9.11.3.41	C	TV	2
59	Old PDU session ID	PDU session identity 2	9.11.3.41	O	TV	2
8	Request type	Request type	9.11.3.47	O	TV	1
22	S-NSSAI	S-NSSAI	9.11.2.8	O	TLV	3-10
25	DNN	DNN	9.11.2.1A	O	TLV	3-102
24	Additional information	Additional information	9.11.2.1	O	TLV	3-n
Z	MA PDU session information	MA PDU session information	O	TV	1
*/
struct ul_nas_transport_t {
    nmm_header_t     header;
    bit4_t           payload_container_type;     // V 1/2
    octet_t          payload_container;          // LV-E
    uint8_t          pdu_session_id;             // 12 C TV 2
    opt_t< uint8_t > old_pdu_session_id;         // 59 TV 2
    opt_t< bit4_t >  request_type;               // 8- TV 1
    opt_t< octet_t > s_nssai;                    // 22 TLV 3+
    opt_t< octet_t > dnn;                        // 25 TLV 3+
    opt_t< octet_t > additional_information;     // 24 TLV 3+
    opt_t< bit4_t >  ma_pdu_session_information; // Z TV 1
};

result_t de_ul_nas_transport(dissector d, context* ctx, ul_nas_transport_t* ret) {
    const use_context uc(&d, ctx, "ul-nas-transport", 0);
    de_nmm_header(d, ctx, &ret->header).step(d);

    /*
    Table 8.2.10.1.1: UL NAS TRANSPORT message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        UL NAS TRANSPORT message identity	Message type	9.7	M	V	1

    */
    // Payload container type	Payload container type	9.11.3.40	M	V	1/2
    de_nibble(d, ctx, &ret->payload_container_type).step(d);
    // Spare half octet Spare half octet 9.5 M V 1 / 2

    // Payload container	Payload container	9.11.3.39	M	LV-E	3-65537
    de_le_octet(d, ctx, &ret->payload_container).step(d);

    // 12	PDU session ID	PDU session identity 2	9.11.3.41	C	TV	2
    de_t_uint8(d, ctx, 0x12, &ret->pdu_session_id).step(d);

    // 59	Old PDU session ID	PDU session identity 2	9.11.3.41	O	TV	2
    de_t_uint8(d, ctx, 0x59, &ret->old_pdu_session_id).step(d);

    // 8	Request type	Request type	9.11.3.47	O	TV	1
    de_tv_short(d, ctx, 0x80, &ret->request_type).step(d);

    // 22	S-NSSAI	S-NSSAI	9.11.2.8	O	TLV	3-10
    de_tl_octet(d, ctx, 0x22, &ret->s_nssai).step(d);

    // 25	DNN	DNN	9.11.2.1A	O	TLV	3-102
    de_tl_octet(d, ctx, 0x25, &ret->dnn).step(d);

    // 24	Additional information	Additional information	9.11.2.1	O	TLV	3-n
    de_tl_octet(d, ctx, 0x24, &ret->additional_information).step(d);

    // Z	MA PDU session information	MA PDU session information	O	TV	1
    return {uc.consumed()};
}

/*
Table 8.2.11.1.1: DL NAS TRANSPORT message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    DL NAS TRANSPORT message identity	Message type	9.7	M	V	1
    Payload container type	Payload container type	9.11.3.40	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Payload container	Payload container	9.11.3.39	M	LV-E	3-65537
12	PDU session ID	PDU session identity 2	9.11.3.41	C	TV	2
24	Additional information	Additional information	9.11.2.1	O	TLV	3-n
58	5GMM cause	5GMM cause	9.11.3.2	O	TV	2
37	Back-off timer value	GPRS timer 3	9.11.2.5	O	TLV	3
*/
struct dl_nas_transport_t {
    nmm_header_t     header;
    bit4_t           payload_container_type; // 1/2
    octet_t          payload_container;      // LV-E
    opt_t< uint8_t > pdu_session_id;         // 12 C TV 2
    opt_t< octet_t > additional_information;  // 24 TLV 3+
    opt_t< uint8_t > cause;                  // 58 TV 2
    opt_t< uint8_t > backoff_timer;          // 37 TLV 3
};

result_t de_dl_nas_transport(dissector d, context* ctx, dl_nas_transport_t* ret) {
    const use_context uc(&d, ctx, "dl-nas-transport", 0);
    /*
    Table 8.2.11.1.1: DL NAS TRANSPORT message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        DL NAS TRANSPORT message identity	Message type	9.7	M	V	1
    */
    // Payload container type	Payload container type	9.11.3.40	M	V	1/2
    de_nibble(d, ctx, &ret->payload_container_type).step(d);

    // Spare half octet	Spare half octet	9.5	M	V	1/2

    // Payload container	Payload container	9.11.3.39	M	LV-E	3-65537
    de_le_octet(d, ctx, &ret->payload_container).step(d);

    // 12	PDU session ID	PDU session identity 2	9.11.3.41	C	TV	2
    de_t_uint8(d, ctx, 0x12, &ret->pdu_session_id).step(d);

    // 24	Additional information	Additional information	9.11.2.1	O	TLV	3-n
    de_tl_octet(d, ctx, 0x24, &ret->additional_information).step(d);

    // 58	5GMM cause	5GMM cause	9.11.3.2	O	TV	2
    de_t_uint8(d, ctx, 0x58, &ret->cause).step(d);

    // 37	Back-off timer value	GPRS timer 3	9.11.2.5	O	TLV	3
    de_tl_uint8(d, ctx, 0x37, &ret->backoff_timer).step(d);
    return {uc.consumed()};
}

/*
Table 8.2.12.1.1: DEREGISTRATION REQUEST message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    De-registration request message identity	Message type	9.7	M	V	1
    De-registration type	De-registration type	9.11.3.20	M	V	1/2
    ngKSI	NAS key set identifier	9.11.3.32	M	V	1/2
    5GS mobile identity		5GS mobile identity	9.11.3.4	M	LV-E	6-n
*/
struct deregistration_request_ue_orig_t {
    nmm_header_t header;
    bit4_t       type; // 9.11.3.20 1/2
    bit4_t       nksi; // 9.11.3.32 1/2
    octet_t      nmid; // 9.11.3.4 LV-E 6+
};

result_t de_deregistration_request_ue_orig(dissector                         d,
                                           context*                          ctx,
                                           deregistration_request_ue_orig_t* ret) {
    const use_context uc(&d, ctx, "deregistration-request-ue-orig", 0);
    de_nmm_header(d, ctx, &ret->header).step(d);
    /*
    Table 8.2.12.1.1: DEREGISTRATION REQUEST message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        De-registration request message identity	Message type	9.7	M	V	1
    */

    // De-registration type	De-registration type	9.11.3.20	M	V	1/2
    de_nibble(d, ctx, &ret->type);

    // ngKSI	NAS key set identifier	9.11.3.32	M	V	1/2
    de_nibble(d, ctx, &ret->nksi).step(d);

    // 5GS mobile identity		5GS mobile identity	9.11.3.4	M	LV-E	6-n
    de_le_octet(d, ctx, &ret->nmid);
    return {uc.consumed()};
}
/*
Table 8.2.13.1.1: DEREGISTRATION ACCEPT message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    De-registration accept message identity	Message type	9.7	M	V	1
*/
struct deregistration_accept_ue_orig_t {
    nmm_header_t header;
};
result_t de_deregistration_accept_ue_orig(dissector                        d,
                                          context*                         ctx,
                                          deregistration_accept_ue_orig_t* ret) {
    const use_context uc(&d, ctx, "deregistration-accept-ue-orig", 0);
    de_nmm_header(d, ctx, &ret->header).step(d);

    return {uc.consumed()};
}

/*
Table 8.2.14.1.1: DEREGISTRATION REQUEST message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    De-registration request message identity	Message type	9.7	M	V	1
    De-registration type	De-registration type	9.11.3.20	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
58	5GMM cause	5GMM cause	9.11.3.2	O	TV	2
5F	T3346 value	GPRS timer 2	9.11.2.4	O	TLV	3
*/
struct deregistration_request_ue_term_t {
    nmm_header_t     header = {};
    bit4_t           type= {};  // 9.11.3.20
    opt_t< uint8_t > cause= {}; // 58 TV 2
    opt_t< uint8_t > t3346= {}; // 5F TLV 3
};
result_t de_deregistration_request_ue_term(dissector                         d,
                                           context*                          ctx,
                                           deregistration_request_ue_term_t* ret) {
    const use_context uc(&d, ctx, "deregistration-request-ue-term", 0);
    /*
    Table 8.2.14.1.1: DEREGISTRATION REQUEST message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        De-registration request message identity	Message type	9.7	M	V	1

    */

    // De-registration type	De-registration type	9.11.3.20	M	V	1/2
    de_nibble(d, ctx, &ret->type).step(d);

    // Spare half octet	Spare half octet	9.5	M	V	1/2

    // 58	5GMM cause	5GMM cause	9.11.3.2	O	TV	2
    de_t_uint8(d, ctx, 0x58, &ret->cause).step(d);

    // 5F T3346 value GPRS timer 2 9.11.2.4 O TLV 3
    de_tl_uint8(d, ctx, 0x5f, &ret->t3346);
    return {uc.consumed()};
}
/*
Table 8.2.15.1.1.1: DEREGISTRATION ACCEPT message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    De-registration accept message identity	Message type	9.7	M	V	1
*/
struct deregistration_accept_ue_term_t {
    nmm_header_t header;
};

result_t de_deregistration_accept_ue_term(dissector                        d,
                                          context*                         ctx,
                                          deregistration_accept_ue_term_t* ret) {
    const use_context uc(&d, ctx, "deregistration-accept-ue-term", 0);
    de_nmm_header(d, ctx, &ret->header).step(d);
    return {uc.consumed()};
}

/*
Table 8.2.16.1.1: SERVICE REQUEST message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Service request message identity	Message type	9.7	M	V	1
    ngKSI 	NAS key set identifier	9.11.3.32	M	V	1/2
    Service type	Service type	9.11.3.50	M	V	1/2
    5G-S-TMSI	5GS mobile identity	9.11.3.4	M	LV-E	9
40	Uplink data status	Uplink data status	9.11.3.57	O	TLV	4-34
50	PDU session status	PDU session status	9.11.3.44	O	TLV	4-34
25	Allowed PDU session status	Allowed PDU session status	9.11.3.13	O	TLV	4-34
71	NAS message container	NAS message container	9.11.3.33	O	TLV-E	4-n
*/
struct nas_message_t;

struct service_request_t {
    nmm_header_t                          header = {};
    bit4_t                                nksi = {};                       // 9.11.3.32 1/2
    bit4_t                                type = {};                       // 9.11.3.50 1/2
    octet_7                               tmsi_nmid = {};                  // 9.11.3.4 LV-E 9
    opt_t< octet_t >         uplink_data_status = {};         // 40 TLV 4+
    opt_t< octet_t >         pdu_session_status = {};         // 50 TLV 4+
    opt_t< octet_t > allowed_pdu_session_status = {}; // 25 TLV 4+
    opt_t< octet_t >                      message = {};                    // 71 TLV-E 4+
};

result_t de_service_request(dissector d, context* ctx, service_request_t* ret) {
    const use_context uc(&d, ctx, "service-request", 0);
    /*
    Table 8.2.16.1.1: SERVICE REQUEST message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Service request message identity	Message type	9.7	M	V	1

    */
    // ngKSI 	NAS key set identifier	9.11.3.32	M	V	1/2
    de_nibble(d, ctx, &ret->nksi);

    // Service type	Service type	9.11.3.50	M	V	1/2
    de_nibble(d, ctx, &ret->type).step(d);

    // 5G-S-TMSI	5GS mobile identity	9.11.3.4	M	LV-E	9
    de_le_fixed(d, ctx, ret->tmsi_nmid).step(d);

    // 40	Uplink data status	Uplink data status	9.11.3.57	O	TLV	4-34
    de_tl_octet(d, ctx, 0x40, &ret->uplink_data_status).step(d);

    // 50	PDU session status	PDU session status	9.11.3.44	O	TLV	4-34
    de_tl_octet(d, ctx, 0x50, &ret->pdu_session_status).step(d);

    // 25	Allowed PDU session status	9.11.3.13	O	TLV	4-34
    de_tl_octet(d, ctx, 0x25, &ret->allowed_pdu_session_status).step(d);

    // 71	NAS message container	9.11.3.33	O	TLV-E	4-n
    de_tle_octet(d, ctx, 0x71, &ret->message).step(d);
    return {uc.consumed()};
}
/*
Table 8.2.17.1.1: SERVICE ACCEPT message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Service accept message identity	Message type	9.7	M	V	1
50	PDU session status	PDU session status	9.11.3.44	O	TLV	4-34
26	PDU session reactivation result	9.11.3.42	O TLV 4-34
72	PDU session reactivation result error cause	9.11.3.43	O	TLV-E	5-515
78	EAP message	EAP message	9.11.2.2	O	TLV-E 7-1503
XX	T3448 value	GPRS timer 3	9.11.2.4	O	TLV	3
*/
struct service_accept_t {
    nmm_header_t     header;
    opt_t< octet_t > pdu_session_status;              // 50 TLV 4+
    opt_t< octet_t > pdu_session_reactivation_result; // 26 TLV 4+
    opt_t< octet_t > result_error_cause;              // 72 TLV-E 5+
    opt_t< eap_t >   eap;                             // 78 TLV-E 7+
    opt_t< uint8_t > t3348;                           // XX TLV 3
};
result_t de_service_accept(dissector d, context* ctx, service_accept_t* ret) {
    const use_context uc(&d, ctx, "service-accept", 3);
    de_nmm_header(d, ctx, &ret->header).step(d);

    /*
    Table 8.2.17.1.1: SERVICE ACCEPT message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Service accept message identity	Message type	9.7	M	V	1
    */

    // 50	PDU session status	PDU session status	9.11.3.44	O	TLV	4-34
    de_tl_octet(d, ctx, 0x50, &ret->pdu_session_status).step(d);

    // 26	PDU session reactivation result	9.11.3.42	O TLV 4-34
    de_tl_octet(d, ctx, 0x26, &ret->pdu_session_reactivation_result).step(d);

    // 72	PDU session reactivation result error cause	9.11.3.43	O	TLV-E	5-515
    de_tle_octet(d, ctx, 0x72, &ret->result_error_cause).step(d);

    // 78	EAP message	EAP message	9.11.2.2	O	TLV-E 7-1503
    de_tle_octet(d, ctx, 0x78, &ret->eap).step(d);

    // XX	T3448 value	GPRS timer 3	9.11.2.4	O	TLV	3
    return {uc.consumed()};
}

/*
Table 8.2.18.1.1: SERVICE REJECT message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Service reject message identity	Message type	9.7	M	V	1
    5GMM cause	5GMM cause	9.11.3.2	M	V	1
50	PDU session status	PDU session status	9.11.3.44	O	TLV	4-34
5F	T3346 value	GPRS timer 2	9.11.2.4	O	TLV	3
78	EAP message	EAP message	9.11.2.2	O	TLV-E	7-1503
XX	T3448 value	GPRS timer 3	9.11.2.4	O	TLV	3
*/
struct service_reject_t {
    nmm_header_t     header;
    uint8_t          cause;              // 9.11.3.2
    opt_t< octet_t > pdu_session_status; // 50 TLV 4+
    opt_t< uint8_t > t3346;              // 5F TLV 3
    opt_t< eap_t >   eap;                // 78 TLV-E 7+
    opt_t< uint8_t > t3348;              // XX TLV 3
};

result_t de_service_reject(dissector d, context* ctx, service_reject_t* ret) {
    const use_context uc(&d, ctx, "service-reject", 0);
    de_nmm_header(d, ctx, &ret->header).step(d);

    /*
    Table 8.2.18.1.1: SERVICE REJECT message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Service reject message identity	Message type	9.7	M	V	1
    */
    // 5GMM cause	5GMM cause	9.11.3.2	M	V	1
    de_uint8(d, ctx, &ret->cause).step(d);

    // 50	PDU session status	PDU session status	9.11.3.44	O	TLV	4-34
    de_tl_octet(d, ctx, 0x50, &ret->pdu_session_status).step(d);

    // 5F	T3346 value	GPRS timer 2	9.11.2.4	O	TLV	3
    de_tl_uint8(d, ctx, 0x5f, &ret->t3346).step(d);

    // 78	EAP message	EAP message	9.11.2.2	O	TLV-E	7-1503
    de_tle_octet(d, ctx, 0x78, &ret->eap).step(d);

    //    XX	T3448 value	GPRS timer 3	9.11.2.4	O	TLV	3
    return {uc.consumed()};
}

/*
Table 8.2.19.1.1: CONFIGURATION UPDATE COMMAND message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Configuration update command message identity	Message type	9.7	M	V	1
D-	Configuration update indication	9.11.3.18	O	TV	1
77	5G-GUTI	5GS mobile identity	9.11.3.4	O	TLV-E	14
54	TAI list	5GS tracking area identity list	9.11.3.9	O	TLV	9-114
15	Allowed NSSAI	NSSAI	9.11.3.37	O	TLV	4-74
27	Service area list	Service area list	9.11.3.49	O	TLV	6-114
43	Full name for network	Network name	9.11.3.35	O	TLV	3-n
45	Short name for network	Network name	9.11.3.35	O	TLV	3-n
46	Local time zone	Time zone	9.11.3.52	O	TV	2
47	Universal time and local time zone	Time zone and time	9.11.3.53	O	TV	8
49	Network daylight saving time	Daylight saving time	9.11.3.19	O	TLV	3
79	LADN information	LADN information	9.11.3.30	O	TLV-E	3-1715
B-	MICO indication	MICO indication	9.11.3.31	O	TV	1
9-	Network slicing indication	Network slicing indication	9.11.3.36	O	TV	1
31	Configured NSSAI	NSSAI	9.11.3.37	O	TLV	4-146
11	Rejected NSSAI	Rejected NSSAI	9.11.3.46	O	TLV	4-42
76	Operator-defined access category definitions	9.11.3.38	O	TLV-E	3-n
F-	SMS indication	SMS indication	9.11.3.50A O TV	1
Tbd	T3447 value	GPRS timer 3	9.11.2.5	O	TLV	3
*/
struct configuration_update_command_t {
    nmm_header_t                      header = {};
    opt_t< uint8_t >                  ind = {};                  // D- TV 1
    opt_t< octet_b >                  nguti_nmid = {};           // 77 TLVE 14
    opt_t< octet_t > taies = {};                // 54 TLV 9+
    opt_t< octet_t >                  allowed_nssai = {};        // 15 TLV 4+
    opt_t< octet_t >      service_areas = {};        // 27 TLV 6+
    opt_t< octet_t >                  full_name = {};            // 43 TLV 3+
    opt_t< octet_t >                  short_name = {};           // 45 TLV 3+
    opt_t< uint8_t >                  local_time_zone = {};      // 46 TV 2
    opt_t< octet_7 >                  utc = {};                  // 47 TV 8
    opt_t< uint8_t >                  daylight_saving_time = {}; // 49 TLV 3
    opt_t< octet_t >       ladn_information = {};     // 79 TLVE 3+
    opt_t< bit4_t >                   mico_ind = {};             // B- TV 1
    opt_t< bit4_t >                   network_slicing_ind = {};  // 9- TV 1
    opt_t< octet_t >                  configured_nssai = {};     // 31 TLV 4+
    opt_t< octet_t >                  rejected_nssai = {};       // 11 TLV 4+
    opt_t< octet_t >                  access_definitions = {};   // 76 TLVE 3+
    opt_t< bit4_t >                   sms_ind = {};              // F- TV 1
    opt_t< uint8_t >                  t3347 = {};                // TBD TLV 3
};

result_t de_configuration_update_command(dissector                        d,
                                         context*                         ctx,
                                         configuration_update_command_t* ret) {
    const use_context uc(&d, ctx, "configuration-update-command", 0);

    /*
    Table 8.2.19.1.1: CONFIGURATION UPDATE COMMAND message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Configuration update command message identity	Message type	9.7	M	V	1
    */
    // D-	Configuration update indication	9.11.3.18	O	TV	1
    de_tv_short(d, ctx, 0xd0, &ret->ind).step(d);

    // 77	5G-GUTI	5GS mobile identity	9.11.3.4	O	TLV-E	14
    de_tle_fixed(d, ctx, 0x77, &ret->nguti_nmid).step(d);

    // 54	TAI list	5GS tracking area identity list	9.11.3.9	O	TLV	9-114
    de_tl_octet(d, ctx, 0x54, &ret->taies).step(d);

    // 15	Allowed NSSAI	NSSAI	9.11.3.37	O	TLV	4-74
    de_tl_octet(d, ctx, 0x15, &ret->allowed_nssai).step(d);

    // 27	Service area list	Service area list	9.11.3.49	O	TLV	6-114
    de_tl_octet(d, ctx, 0x27, &ret->service_areas).step(d);

    // 43	Full name for network	9.11.3.35	O	TLV	3-n
    de_tl_octet(d, ctx, 0x43, &ret->full_name).step(d);

    // 45	Short name for network	9.11.3.35	O	TLV	3-n
    de_tl_octet(d, ctx, 0x45, &ret->short_name).step(d);

    // 46	Local time zone	9.11.3.52	O	TV	2
    de_t_uint8(d, ctx, 0x46, &ret->local_time_zone).step(d);

    // 47	Universal time and local time zone	9.11.3.53	O	TV	8
    de_t_fixed(d, ctx, 0x47, &ret->utc).step(d);

    // 49	Network daylight saving time	9.11.3.19	O	TLV	3
    de_tl_uint8(d, ctx, 0x49, &ret->daylight_saving_time).step(d);

    // 79	LADN information	9.11.3.30	O	TLV-E	3-1715
    de_tle_octet(d, ctx, 0x79, &ret->ladn_information).step(d);

    // B-	MICO indication	MICO indication	9.11.3.31	O	TV	1
    de_tv_short(d, ctx, 0xb0, &ret->mico_ind).step(d);

    // 9-	Network slicing indication	9.11.3.36	O	TV	1
    de_tv_short(d, ctx, 0x90, &ret->network_slicing_ind).step(d);

    // 31	Configured NSSAI	9.11.3.37	O	TLV	4-146
    de_tl_octet(d, ctx, 0x31, &ret->configured_nssai).step(d);

    // 11	Rejected NSSAI	9.11.3.46	O	TLV	4-42
    de_tl_octet(d, ctx, 0x11, &ret->rejected_nssai).step(d);

    // 76	Operator-defined access category definitions	9.11.3.38	O	TLV-E	3-n
    de_tle_octet(d, ctx, 0x76, &ret->access_definitions).step(d);

    // F-	SMS indication	9.11.3.50A O TV	1
    de_tv_short(d, ctx, 0xF0, &ret->sms_ind).step(d);

    // Tbd	T3447 value	GPRS timer 3	9.11.2.5	O	TLV	3
    return {uc.consumed()};
}

/*
Table 8.2.20.1.1: CONFIGURATION UPDATE COMPLETE message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Configuration update complete message identity	Message type	9.7	M	V	1
*/
struct configuration_update_complete_t {
    nmm_header_t header;
};

result_t de_configuration_update_complete(dissector  d,
                                          context* ctx,
                                          configuration_update_complete_t*ret) {
    const use_context uc(&d, ctx, "configuration-update-complete", 0);
    de_nmm_header(d, ctx, &ret->header).step(d);

    return {uc.consumed()};
}

/*
Table 8.2.21.1.1: IDENTITY REQUEST message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Identity request message identity	Message type	9.7	M	V	1
    Identity type	5GS identity type	9.11.3.3	M	V	1/2
    Spare half octet	Spare half octet 	9.5	M	V	1/2
*/
struct identity_request_t {
    nmm_header_t header;
    bit4_t       nidentity_type; // 9.11.3.3 1/2
};

result_t de_identity_request(dissector d, context* ctx, identity_request_t* ret) {
    const use_context uc(&d, ctx, "identity-request", 0);
    /*
    Table 8.2.21.1.1: IDENTITY REQUEST message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Identity request message identity	Message type	9.7	M	V	1
    */

    // Identity type	5GS identity type	9.11.3.3	M	V	1/2
    de_nibble(d, ctx, &ret->nidentity_type).step(d);

    // Spare half octet Spare half octet 9.5 M V 1 / 2;
    return {uc.consumed()};
}

/*
Table 8.2.22.1.1: IDENTITY RESPONSE message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Identity response message identity	Message type	9.7	M	V	1
    Mobile identity	5GS mobile identity	9.11.3.4	M	LV-E	3-n
*/
struct identity_response_t {
    nmm_header_t header;
    octet_t      nmid; // 9.11.3.4 LV-E 3+
};

result_t de_identity_response(dissector d, context* ctx, identity_response_t* ret) {
    const use_context uc(&d, ctx, "identity-response", 0);
    de_nmm_header(d, ctx, &ret->header).step(d);
    /*
    Table 8.2.22.1.1: IDENTITY RESPONSE message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Identity response message identity	Message type	9.7	M	V	1
    */
    //        Mobile identity	5GS mobile identity	9.11.3.4	M	LV-E	3-n
    de_le_octet(d, ctx, &ret->nmid).step(d);
    return {uc.consumed()};
}

/*
Table 8.2.23.1.1: NOTIFICATION message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Notification message identity	Message type	9.7	M	V	1
    Access type	Access type	9.11.3.11	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
*/
struct notification_t {
    nmm_header_t header;
    bit4_t       access_type; // 9.11.3.11 1/2
};

result_t de_notification(dissector d, context* ctx, notification_t* ret) {
    const use_context uc(&d, ctx, "notification", 0);
    de_nmm_header(d, ctx, &ret->header).step(d);

    /*
    Table 8.2.23.1.1: NOTIFICATION message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Notification message identity	Message type	9.7	M	V	1
    */
    // Access type	Access type	9.11.3.11	M	V	1/2
    de_nibble(d, ctx, &ret->access_type).step(d);
    // Spare half octet	Spare half octet	9.5	M	V	1/2
    return {uc.consumed()};
}

/*
Table 8.2.2341.1: NOTIFICATION RESPONSE message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Notification response message identity	Message type	9.7	M	V	1
50	PDU session status	PDU session status	9.11.3.44	O	TLV	4-34
*/
struct notification_response_t {
    nmm_header_t     header;
    opt_t< octet_t > pdu_session_status; // 50
};

result_t de_notification_response(dissector                d,
                                  context*               ctx,
                                  notification_response_t* ret) {
    const use_context uc(&d, ctx, "notification-response", 0);
    de_nmm_header(d, ctx, &ret->header).step(d);

    /*
    Table 8.2.2341.1: NOTIFICATION RESPONSE message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Notification response message identity	Message type	9.7	M	V	1
    */
    // 50	PDU session status	PDU session status	9.11.3.44	O	TLV	4-34
    de_tl_octet(d, ctx, 0x50, &ret->pdu_session_status).step(d);
    return {uc.consumed()};
}

/*
Table 8.2.25.1.1: SECURITY MODE COMMAND message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Security mode command message identity	Message type	9.7	M	V	1
    Selected NAS security algorithms	NAS security algorithms	9.11.3.34	M	V	1
    ngKSI	NAS key set identifier	9.11.3.32	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Replayed UE security capabilities	UE security capability	9.11.3.54	M	LV	3-9
E-	IMEISV request	IMEISV request	9.11.3.28	O	TV	1
57	Selected EPS NAS security algorithms 9.11.3.25 O	TV	2
36	Additional 5G security information	9.11.3.12	O	TLV	3
78	EAP message	EAP message	9.11.2.2	O	TLV-E 7-1503
38	ABBA	ABBA	9.11.3.10	O	TLV	4-n
19	Replayed S1 UE security capability	9.11.3.48A	O	TLV	4-7
*/
struct security_mode_command_t {
    nmm_header_t                        header;
    uint8_t                             selected_security_algo; // 9.11.3.34
    bit4_t                              nksi;                   // 9.11.3.32 1/2
    octet_t                             replayed_ue_security_capabilities;    // LV 3
    opt_t< bit4_t >                     imeisv_request;                       // E- TV 1
    opt_t< uint8_t >                    selected_eps_security_algo;           // 57 TV 2
    opt_t< uint8_t >                    additional_5g_security_information;   // 36 TLV 3
    opt_t< eap_t >                      eap;                                  // 78 TLVE
    opt_t< abba_t >                     abba;                                 // 38 TLV 4-
    opt_t< octet_t > replayed_s1_ue_security_capabilities; // 19 TLV 4-
};

result_t de_security_mode_command(dissector                 d,
                                  context*                  ctx,
                                  security_mode_command_t* ret) {
    const use_context uc(&d, ctx, "security-mode-command", 0);
    de_nmm_header(d, ctx, &ret->header).step(d);
    /*
    Table 8.2.25.1.1: SECURITY MODE COMMAND message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Security mode command message identity	Message type	9.7	M	V	1
    */
    // Selected NAS security algorithms		9.11.3.34	M	V	1
    de_uint8(d, ctx, &ret->selected_security_algo).step(d);

    // ngKSI	NAS key set identifier	9.11.3.32	M	V	1/2
    // Spare half octet	Spare half octet	9.5	M	V	1/2
    de_nibble(d, ctx, &ret->nksi).step(d);

    // Replayed UE security capabilities	9.11.3.54	M LV 3-9
    de_l_octet(d, ctx, &ret->replayed_ue_security_capabilities).step(d);

    // E-	IMEISV request	IMEISV request	9.11.3.28	O	TV	1
    de_tv_short(d, ctx, 0xe0, &ret->imeisv_request).step(d);

    // 57	Selected EPS NAS   security algorithms 9.11.3.25 O	TV	2
    de_t_uint8(d, ctx, 0x57, &ret->selected_eps_security_algo).step(d);

    // 36	Additional 5G security  information	9.11.3.12	O	TLV	3
    de_tl_uint8(d, ctx, 0x36, &ret->additional_5g_security_information).step(d);

    // 78	EAP message	EAP message	9.11.2.2	O	TLV-E    7-1503
    de_tle_octet(d, ctx, 0x78, &ret->eap).step(d);

    // 38	ABBA	ABBA	9.11.3.10	O	TLV	4-n
    de_tl_octet(d, ctx, 0x38, &ret->abba).step(d);

    // 19	Replayed S1 UE security    capability	9.11.3.48A	O	TLV	4-7
    de_tl_octet(d, ctx, 0x19, &ret->replayed_s1_ue_security_capabilities).step(d);
    return {uc.consumed()};
}

/*
Table 8.2.26.1.1: SECURITY MODE COMPLETE message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Security mode complete message identity	Message type	9.6	M	V	1
77	IMEISV	5G mobile identity	9.11.3.4	O	TLV-E	12
71	NAS message container	9.11.3.33	O	TLV-E	4-n
*/
struct security_mode_complete_t {
    nmm_header_t     header = {};
    opt_t< octet_b > imeisv_nmid= {}; // 77 TLVE
    opt_t< octet_t > message= {};
    // std::shared_ptr< nas_message_t > message; // 71 9.11.3.33 TLV-E
};
result_t de_security_mode_complete(dissector                 d,
                                   context*                  ctx,
                                   security_mode_complete_t* ret) {
    const use_context uc(&d, ctx, "security-mode-complete", 0);
    de_nmm_header(d, ctx, &ret->header).step(d);
    /*
    Table 8.2.26.1.1: SECURITY MODE COMPLETE message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Security mode complete message identity	Message type	9.6	M	V	1
    */
    // 77	IMEISV	5G mobile identity	9.11.3.4	O	TLV-E	12
    de_tle_fixed(d, ctx, 0x77, &ret->imeisv_nmid).step(d);

    // 71	NAS message container	9.11.3.33	O	TLV-E	4-n
    de_tle_octet(d, ctx, 0x71, &ret->message).step(d);
    return {uc.consumed()};
}

/*
Table 8.2.27.1.1: SECURITY MODE REJECT message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Security mode reject message identity	Message type	9.6	M	V	1
    5GMM cause	5GMM cause	9.11.3.2	M	V	1
*/
struct security_mode_reject_t {
    nmm_header_t header;
    uint8_t      cause; // 9.11.3.2 V
};

result_t de_security_mode_reject(dissector d, context* ctx, security_mode_reject_t* ret) {
    const use_context uc(&d, ctx, "security-mode-reject", 0);
    de_nmm_header(d, ctx, &ret->header).step(d);

    /*
    Table 8.2.27.1.1: SECURITY MODE REJECT message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        Security mode reject message identity	Message type	9.6	M	V	1
    */
    //        5GMM cause	5GMM cause	9.11.3.2	M	V	1
    de_uint8(d, ctx, &ret->cause).step(d);

    return {uc.consumed()};
}
/*
Table 8.2.28.1.1: SECURITY PROTECTED 5GS NAS MESSAGE message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    Message authentication code	Message authentication code	9.8	M	V	4
    Sequence number	Sequence number	9.10	M	V	1
    Plain 5GS NAS message	Plain 5GS NAS message	9.9	M	V	3-n
*/
struct security_protected_nas_message_t {
    uint8_t epd; //
    bit4_t  security_header_type;
    bit4_t  spare;
    octet_4 auth_code;
    uint8_t sequence_no; //
    octet_t plain;
};

result_t de_security_protected_message(dissector                d,
                                       context*                 ctx,
                                       security_protected_nas_message_t* ret) {
    const use_context uc(&d, ctx, "nas-security-protected-message", 0);
    /*
    Table 8.2.28.1.1: SECURITY PROTECTED 5GS NAS MESSAGE message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
    */
    //        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V
    //        1
    // ret->epd = d.uint8(true);
    de_uint8(d, ctx, &ret->epd).step(d);

    //        Security header type	Security header type	9.3	M	V	1/2
    de_nibble(d, ctx, &ret->security_header_type).step(d);
    // Spare half octet	Spare half octet	9.5	M	V	1/2

    // Message authentication code	Message authentication code	9.8	M	V	4
    d.octet(ret->auth_code, d.safe_length(sizeof(ret->auth_code)), true);

    // Sequence number	Sequence number	9.10	M	V	1
    ret->sequence_no = d.uint8(true);

    // Plain 5GS NAS message	Plain 5GS NAS message	9.9	M	V	3-n
    ret->plain = octet_t(d.safe_ptr(), d.safe_ptr()+d.safe_length(d.length));

    return {uc.consumed()};
}
/*
Table 8.2.29.1.1: 5GMM STATUS message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    Security header type	Security header type	9.3	M	V	1/2
    Spare half octet	Spare half octet	9.5	M	V	1/2
    5GMM STATUS message identity	Message type	9.7	M	V	1
    5GMM cause	5GMM cause	9.11.3.2	M	V	1
*/
struct nmm_status_t {
    nmm_header_t header;
    uint8_t      cause; // 9.11.3.2 V 1
};

result_t de_nmm_status(dissector d, context* ctx, nmm_status_t* ret) {
    const use_context uc(&d, ctx, "security-mode-reject", 0);
    de_nmm_header(d, ctx, &ret->header).step(d);

    /*
    Table 8.2.29.1.1: 5GMM STATUS message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        Security header type	Security header type	9.3	M	V	1/2
        Spare half octet	Spare half octet	9.5	M	V	1/2
        5GMM STATUS message identity	Message type	9.7	M	V	1
    */
    //        5GMM cause	5GMM cause	9.11.3.2	M	V	1
    de_uint8(d, ctx, &ret->cause).step(d);

    return {uc.consumed()};
}

/*
Table 8.3.1.1.1: PDU SESSION ESTABLISHMENT REQUEST message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    PDU session ID	PDU session identity	9.4	M	V	1
    PTI	Procedure transaction identity	9.6	M	V	1
    PDU SESSION ESTABLISHMENT REQUEST message identity	Message type	9.7	M	V	1
    Integrity protection maximum data rate	 9.11.4.7	M	V	2
9-	PDU session type	PDU session type	9.11.4.11	O	TV	1
A-	SSC mode	SSC mode	9.11.4.16	O	TV	1
28	5GSM capability	5GSM capability	9.11.4.1	O	TLV	3-15
55	Maximum number of supported packet filters	 9.11.4.9	O	TV	3
B-	Always-on PDU session requested	O	TV	1
39	SM PDU DN request container	9.11.4.15	O	TLV	3-255
7B	Extended protocol configuration options 	9.11.4.6	O	TLV-E	4-65538
*/
struct pdu_session_establishment_request_t {
    nsm_header_t      header;
    uint16_t          integrity_max_data_rate;         // 9.11.4.7 V 2
    opt_t< bit4_t >   pdu_session_type;                 // 9-
    opt_t< bit4_t >   ssc_mode;                        // A-
    opt_t< octet_t >  nsm_capabilities;                // 28 TLV
    opt_t< uint16_t > supported_packet_filters_max_n;  // 55 TV 3
    opt_t< bit4_t >   always_on_pdu_session_requested; // B- TV 1
    opt_t< octet_t >  sm_pdu_dn_request_container;     // 39 TLV 3+
    opt_t< octet_t >  extended_pco;                    // 7B TLVE 4+
};

result_t de_pdu_session_establishment_request(dissector                            d,
                                              context*                             ctx,
                                              pdu_session_establishment_request_t* ret) {
    const use_context uc(&d, ctx, "pdu-session-establishment-request", 0);
    de_nsm_header(d, ctx, &ret->header).step(d);
    /*
    Table 8.3.1.1.1: PDU SESSION ESTABLISHMENT REQUEST message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        PDU session ID	PDU session identity	9.4	M	V	1
        PTI	Procedure transaction identity	9.6	M	V	1
        PDU SESSION ESTABLISHMENT REQUEST message identity	Message type	9.7	M	V	1
    */
    //        Integrity protection maximum data rate	 9.11.4.7	M	V	2
    de_uint16(d, ctx, &ret->integrity_max_data_rate).step(d);

    // 9-	PDU session type	PDU session type	9.11.4.11	O	TV	1
    de_tv_short(d, ctx, 0x90, &ret->pdu_session_type).step(d);

    // A-	SSC mode	SSC mode	9.11.4.16	O	TV	1
    de_tv_short(d, ctx, 0xA0, &ret->ssc_mode);

    // 28	5GSM capability	5GSM capability	9.11.4.1	O	TLV	3-15
    de_tl_octet(d, ctx, 0x28, &ret->nsm_capabilities).step(d);

    // 55	Maximum number of supported packet filters	 9.11.4.9	O	TV	3
    de_t_uint16(d, ctx, 0x55, &ret->supported_packet_filters_max_n).step(d);

    // B-	Always-on PDU session requested	O	TV	1
    de_tv_short(d, ctx, 0xb0, &ret->always_on_pdu_session_requested).step(d);

    // 39	SM PDU DN request container	9.11.4.15	O	TLV	3-255
    de_tl_octet(d, ctx, 0x39, &ret->sm_pdu_dn_request_container).step(d);

    // 7B	Extended protocol configuration options 	9.11.4.6	O	TLV-E	4-65538
    de_tle_octet(d, ctx, 0x7b, &ret->extended_pco).step(d);
    return {uc.consumed()};
}

// clang-format off
/*
Table 8.3.2.1.1: PDU SESSION ESTABLISHMENT ACCEPT message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    PDU session ID	PDU session identity	9.4	M	V	1
    PTI	Procedure transaction identity	9.6	M	V	1
    PDU SESSION ESTABLISHMENT ACCEPT message identity	Message type	9.7	M	V	1
    Selected PDU session type	PDU session type	9.11.4.11	M	V	1/2
    Selected SSC mode	SSC mode	9.11.4.16	M	V	1/2
    Authorized QoS rules	QoS rules	9.11.4.13	M	LV-E	6-65538
    Session AMBR	Session-AMBR	9.11.4.14	M	LV	7
59	5GSM cause	5GSM cause	9.11.4.2	O	TV	2
29	PDU address	PDU address	9.11.4.10	O	TLV	7, 11 or 15
56	RQ timer value	GPRS timer	9.11.2.3	O	TV	2
22	S-NSSAI	S-NSSAI	9.11.2.8	O	TLV	3-10
8-	Always-on PDU session indication	9.11.4.3 O TV 1
75	Mapped EPS bearer contexts	9.11.4.8	O	TLV-E 7-65538
78	EAP message	EAP message	9.11.2.2	O	TLV-E	7-1503
79	Authorized QoS flow descriptions	9.11.4.12	O	TLV-E 6-65538
7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
25	DNN	DNN	9.11.2.1A	O	TLV	3-102
xx	5GSM network feature support	9.11.4.18	O	TLV	3-15
xx Session-TMBR	Session-TMBR	9.11.4.19	O	TLV	8
TBD	Serving PLMN rate control Serving PLMN rate control	9.11.4.20	O	TLV	4
XX	ATSSS container	ATSSS container	9.11.4.22	O	TLV-E	3-65538
XX	Control plane only indication	9.11.4.23	O	TV	1
*/
// clang-format on
struct pdu_session_establishment_accept_t {
    nsm_header_t                           header;
    bit4_t                                 selected_pdu_session_type; // 9.11.4.11 V
    bit4_t                                 selected_ssc_mode;         // 9.11.4.16 V 1/2
    octet_t                                authorized_qos_rules;      // 9.11.4.13 LV-E 6+
    octet_6                                session_ambr;              // 9.11.4.14 LV 7
    opt_t< uint8_t >                       cause;                     // 59 TV 2
    opt_t< octet_t >                       pdu_address;               // 29 TLV 7+
    opt_t< uint8_t >                       rq_timer;                  // 56 TV 2
    opt_t< octet_t >                     s_nssai;                   // 22 TLV 3+
    opt_t< bit4_t >                        always_on_pdu_session_ind; // 8- TV 1
    opt_t< octet_t >  mapped_eps_bearer_contexts;  // 75
    opt_t< eap_t >                         eap;                         // 78 TLVE
    opt_t< octet_t >       authorized_qos_flow_descs;   // 79 TLVE
    opt_t< octet_t >                extended_pco;                // 7B TLVE
    opt_t< dnn_t >                         dnn;                         // 25 TLV
    opt_t< nsm_network_feature_support_t > nsm_network_feature_support; // XX TLV
    opt_t< session_tmbr_t >                session_tmbr_t;              // XX TLV
    opt_t< uint16_t >                      serving_plmn_rate_control;   // TBD TLV
    opt_t< atsss_container_t >             atsss_container;             // XX TLVE
    opt_t< bit4_t >                        control_plane_only_indi;     // XX TV 1
};

result_t de_pdu_session_establishment_accept(dissector                           d,
                                             context*                            ctx,
                                             pdu_session_establishment_accept_t* ret) {
    const use_context uc(&d, ctx, "pdu-session-establishment-accept", 0);
    /*
    Table 8.3.2.1.1: PDU SESSION ESTABLISHMENT ACCEPT message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        PDU session ID	PDU session identity	9.4	M	V	1
        PTI	Procedure transaction identity	9.6	M	V	1
        PDU SESSION ESTABLISHMENT ACCEPT message identity	Message type	9.7	M	V	1
    */
    // Selected PDU session type	PDU session type	9.11.4.11	M	V	1/2
    de_nibble(d, ctx, &ret->selected_pdu_session_type);

    // Selected SSC mode	SSC mode	9.11.4.16	M	V	1/2
    de_nibble(d, ctx, &ret->selected_ssc_mode).step(d);

    // Authorized QoS rules	QoS rules	9.11.4.13	M	LV-E	6-65538
    de_le_octet(d, ctx, &ret->authorized_qos_rules).step(d);

    // Session AMBR	Session-AMBR	9.11.4.14	M	LV	7
    de_l_fixed(d, ctx, ret->session_ambr).step(d);

    // 59	5GSM cause	5GSM cause	9.11.4.2	O	TV	2
    de_t_uint8(d, ctx, 0x59, &ret->cause).step(d);

    // 29	PDU address	PDU address	9.11.4.10	O	TLV	7, 11 or 15
    de_tl_octet(d, ctx, 0x29, &ret->pdu_address).step(d);

    // 56	RQ timer value	GPRS timer	9.11.2.3	O	TV	2
    de_t_uint8(d, ctx, 0x56, &ret->rq_timer).step(d);

    // 22	S-NSSAI	S-NSSAI	9.11.2.8	O	TLV	3-10
    de_tl_octet(d, ctx, 0x22, &ret->s_nssai).step(d);

    // 8-	Always-on PDU session indication	9.11.4.3 O TV 1
    de_tv_short(d, ctx, 0x80, &ret->always_on_pdu_session_ind).step(d);

    // 75	Mapped EPS bearer contexts	9.11.4.8	O	TLV-E 7-65538
    de_tle_octet(d, ctx, 0x75, &ret->mapped_eps_bearer_contexts).step(d);

    // 78	EAP message	EAP message	9.11.2.2	O	TLV-E	7-1503
    de_tle_octet(d, ctx, 0x78, &ret->eap).step(d);

    // 79	Authorized QoS flow descriptions	9.11.4.12	O	TLV-E 6-65538
    de_tle_octet(d, ctx, 0x79, &ret->authorized_qos_flow_descs).step(d);

    // 7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
    de_tle_octet(d, ctx, 0x7b, &ret->extended_pco).step(d);

    // 25	DNN	DNN	9.11.2.1A	O	TLV	3-102
    de_tl_octet(d, ctx, 0x25, &ret->dnn).step(d);

    // xx	5GSM network feature support	9.11.4.18	O	TLV	3-15
    // xx Session-TMBR	Session-TMBR	9.11.4.19	O	TLV	8
    // TBD	Serving PLMN rate control Serving PLMN rate control	9.11.4.20	O	TLV	4
    // XX	ATSSS container	ATSSS container	9.11.4.22	O	TLV-E	3-65538
    // XX	Control plane only indication	9.11.4.23	O	TV	1
    return {uc.consumed()};
}
// clang-format off
/*
Table 8.3.3.1.1: PDU SESSION ESTABLISHMENT REJECT message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    PDU session ID	PDU session identity	9.4	M	V	1
    PTI	Procedure transaction identity	9.6	M	V	1
    PDU SESSION ESTABLISHMENT REJECT message identity	Message type	9.7	M	V	1
    5GSM cause	5GSM cause	9.11.4.2	M	V	1
37	Back-off timer value	GPRS timer 3	9.11.2.5	O	TLV	3
F-	Allowed SSC mode	Allowed SSC mode	9.11.4.5	O	TV	1
78	EAP message	EAP message	9.11.2.2	O	TLV-E	7-1503
7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
TBD	Re-attempt indicator	9.11.4.17	O	TLV	3
61	5GSM congestion re-attempt indicator	9.11.4.21	O	TLV	3
*/
// clang-format on
struct pdu_session_establishment_reject_t {
    nsm_header_t            header = {};
    uint8_t                 cause = {};                        // 9.11.4.2
    opt_t< uint8_t >        backoff_timer = {};                // 37 TLV
    opt_t< bit4_t >         allowed_ssc_mode = {};             // F- TV 1
    opt_t< eap_t >          eap = {};                          // 78 TLVE
    opt_t< octet_t > extended_pco = {};                 // 7B TLVE
    opt_t< uint8_t >        reattempt_ind = {};                // TBD TLV 3
    opt_t< uint8_t >        nsm_congestion_reattempt_ind = {}; // 61 TLV 3
};

result_t de_pdu_session_establishment_reject(dissector                           d,
                                             context*                            ctx,
                                             pdu_session_establishment_reject_t* ret) {
    const use_context uc(&d, ctx, "pdu-session-establishment-reject", 0);
    /*
    Table 8.3.3.1.1: PDU SESSION ESTABLISHMENT REJECT message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        PDU session ID	PDU session identity	9.4	M	V	1
        PTI	Procedure transaction identity	9.6	M	V	1
        PDU SESSION ESTABLISHMENT REJECT message identity	Message type	9.7	M	V	1
    */
    // 5GSM cause	5GSM cause	9.11.4.2	M	V	1
    de_uint8(d, ctx, &ret->cause);

    // 37	Back-off timer value	GPRS timer 3	9.11.2.5	O	TLV	3
    de_tl_uint8(d, ctx, 0x37, &ret->backoff_timer).step(d);

    // F-	Allowed SSC mode	Allowed SSC mode	9.11.4.5	O	TV	1
    de_tv_short(d, ctx, 0xf0, &ret->allowed_ssc_mode).step(d);

    // 78	EAP message	EAP message	9.11.2.2	O	TLV-E	7-1503
    de_tle_octet(d, ctx, 0x78, &ret->eap).step(d);

    // 7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
    de_tle_octet(d, ctx, 0x7b, &ret->extended_pco).step(d);

    // TBD	Re-attempt indicator	9.11.4.17	O	TLV	3
    // 61	5GSM congestion re-attempt indicator	9.11.4.21	O	TLV	3
    de_tl_uint8(d, ctx, 0x61, &ret->reattempt_ind).step(d);
    return {uc.consumed()};
}

/*
Table 8.3.4.1.1: PDU SESSION AUTHENTICATION COMMAND message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    PDU session ID	PDU session identity	9.4	M	V	1
    PTI	Procedure transaction identity	9.6	M	V	1
    PDU SESSION AUTHENTICATION COMMAND message identity	Message type	9.7	M	V	1
    EAP message	EAP message	9.11.2.2	M	LV-E	6-1502
7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
*/
struct pdu_session_authentication_command_t {
    nsm_header_t            header;
    eap_t                   eap;          // 9.11.2.2 LV-E 6+
    opt_t< octet_t > extended_pco; // 7B 9.11.4.6 TLV-E
};

result_t de_pdu_session_authentication_command(
    dissector                             d,
    context*                              ctx,
    pdu_session_authentication_command_t* ret) {
    const use_context uc(&d, ctx, "pdu-session-authentication-command", 0);
    /*
    Table 8.3.4.1.1: PDU SESSION AUTHENTICATION COMMAND message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        PDU session ID	PDU session identity	9.4	M	V	1
        PTI	Procedure transaction identity	9.6	M	V	1
        PDU SESSION AUTHENTICATION COMMAND message identity	Message type	9.7	M	V	1
    */
    //  EAP message	EAP message	9.11.2.2	M	LV-E	6-1502
    de_le_octet(d, ctx, &ret->eap).step(d);

    // 7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
    de_tle_octet(d, ctx, 0x7b, &ret->extended_pco).step(d);
    return {uc.consumed()};
}

// clang-format off
/*
Table 8.3.5.1.1: PDU SESSION AUTHENTICATION COMPLETE message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    PDU session ID	PDU session identity	9.4	M	V	1
    PTI	Procedure transaction identity	9.6	M	V	1
    PDU SESSION AUTHENTICATION COMPLETE message identity	Message type	9.7	M	V	1
    EAP message	EAP message	9.11.2.2	M	LV-E	6-1502
7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
*/
// clang-format on
struct pdu_session_authentication_complete_t {
    nsm_header_t            header;
    eap_t                   eap;
    opt_t< octet_t > extended_pco; // 7B 9.11.4.6 TLV-E
};

result_t de_pdu_session_authentication_complete(
    dissector                              d,
    context*                               ctx,
    pdu_session_authentication_complete_t* ret) {
    const use_context uc(&d, ctx, "pdu-session-authentication-complete", 0);
    /*
    Table 8.3.5.1.1: PDU SESSION AUTHENTICATION COMPLETE message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        PDU session ID	PDU session identity	9.4	M	V	1
        PTI	Procedure transaction identity	9.6	M	V	1
        PDU SESSION AUTHENTICATION COMPLETE message identity	Message type	9.7	M V	1
    */
    // EAP message	EAP message	9.11.2.2	M	LV-E	6-1502
    de_le_octet(d, ctx, &ret->eap).step(d);

    // 7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
    de_tle_octet(d, ctx, 0x7b, &ret->extended_pco).step(d);

    return {uc.consumed()};
}

// clang-format off
/*
Table 8.3.6.1.1: PDU SESSION AUTHENTICATION RESULT message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    PDU session ID	PDU session identity	9.4	M	V	1
    PTI	Procedure transaction identity	9.6	M	V	1
    PDU SESSION AUTHENTICATION RESULT message identity	Message type	9.7	M	V	1
78	EAP message	EAP message	9.11.2.2	O	TLV-E	7-1503
7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
*/
// clang-format on
struct pdu_session_authentication_result_t {
    nsm_header_t            header;
    opt_t<eap_t>                   eap;          // 78 TLVE 7+
    opt_t< octet_t > extended_pco; // 7B 9.11.4.6 TLV-E
};

result_t de_pdu_session_authentication_result(dissector                            d,
                                              context*                             ctx,
                                              pdu_session_authentication_result_t* ret) {
    const use_context uc(&d, ctx, "pdu-session-authentication-result", 0);
    /*
    Table 8.3.6.1.1: PDU SESSION AUTHENTICATION RESULT message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        PDU session ID	PDU session identity	9.4	M	V	1
        PTI	Procedure transaction identity	9.6	M	V	1
        PDU SESSION AUTHENTICATION RESULT message identity	Message type	9.7	M	V	1
    */
    // 78	EAP message	EAP message	9.11.2.2	O	TLV-E	7-1503
    de_tle_octet(d, ctx, 0x78, &ret->eap).step(d);

    // 7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
    de_tle_octet(d, ctx, 0x7b, &ret->extended_pco).step(d);

    return {uc.consumed()};
}

// clang-format off
/*
Table 8.3.7.1.1: PDU SESSION MODIFICATION REQUEST message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    PDU session ID	PDU session identity	9.4	M	V	1
    PTI	Procedure transaction identity	9.6	M	V	1
    PDU SESSION MODIFICATION REQUEST message identity	Message type	9.7	M	V	1
28	5GSM capability	5GSM capability	9.11.4.1	O	TLV	3-15
59	5GSM cause	5GSM cause	9.11.4.2	O	TV	2
55	Maximum number of supported packet filters	9.11.4.9	O	TV	3
B-	Always-on PDU session requested	9.11.4.4	O	TV	1
13	Integrity protection maximum data rate	9.11.4.7	O	TV	3
7A	Requested QoS rules	QoS rules	9.11.4.13	O	TLV-E	7-65538
79	Requested QoS flow descriptions	9.11.4.12	O	TLV-E	6-65538
75	Mapped EPS bearer contexts	9.11.4.8	O	TLV-E	7-65538
7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
*/
// clang-format on
struct pdu_session_modification_request_t {
    nsm_header_t                          header = {};
    opt_t< octet_t >             capabilities = {};                    // 28 TLV 3+
    opt_t< uint8_t >                      cause = {};                           // 59 TV 2
    opt_t< uint16_t >                     supported_packet_filters_max_n = {};  // 55 TV 3
    opt_t< bit4_t >                       always_on_pdu_session_requested = {}; // B- TV 1
    opt_t< uint16_t >                     integrity_max_data_rate = {};         // 13 TV 3
    opt_t< octet_t >                  requested_qos_rules = {};             // 7A TLVE
    opt_t< octet_t >      requested_qos_flow_descs = {};        // 79 TLVE
    opt_t< octet_t > mapped_eps_bearer_contexts = {};      // 75 TLVE
    opt_t< octet_t >               extended_pco = {};                    // 7B TLVE
};

result_t de_pdu_session_modification_request(dissector                           d,
                                             context*                            ctx,
                                             pdu_session_modification_request_t* ret) {
    const use_context uc(&d, ctx, "pdu-session-modification-request", 0);
    /*
    Table 8.3.7.1.1: PDU SESSION MODIFICATION REQUEST message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        PDU session ID	PDU session identity	9.4	M	V	1
        PTI	Procedure transaction identity	9.6	M	V	1
        PDU SESSION MODIFICATION REQUEST message identity	Message type	9.7	M	V	1
    */
    // 28	5GSM capability	5GSM capability	9.11.4.1	O	TLV	3-15
    de_tl_octet(d, ctx, 0x28, &ret->capabilities).step(d);

    // 59	5GSM cause	5GSM cause	9.11.4.2	O	TV	2
    de_t_uint8(d, ctx, 0x59, &ret->cause).step(d);

    // 55	Maximum number of supported packet filters	9.11.4.9	O	TV	3
    de_t_uint16(d, ctx, 0x55, &ret->supported_packet_filters_max_n).step(d);

    // B-	Always-on PDU session requested	9.11.4.4	O	TV	1
    de_tv_short(d, ctx, 0xb0, &ret->always_on_pdu_session_requested).step(d);

    // 13	Integrity protection maximum data rate	9.11.4.7	O	TV	3
    de_t_uint16(d, ctx, 0x13, &ret->integrity_max_data_rate).step(d);

    // 7A	Requested QoS rules	QoS rules	9.11.4.13	O	TLV-E	7-65538
    de_tle_octet(d, ctx, 0x7a, &ret->requested_qos_rules).step(d);

    // 79	Requested QoS flow descriptions	9.11.4.12	O	TLV-E	6-65538
    de_tle_octet(d, ctx, 0x79, &ret->requested_qos_flow_descs).step(d);

    // 75	Mapped EPS bearer contexts	9.11.4.8	O	TLV-E	7-65538
    de_tle_octet(d, ctx, 0x75, &ret->mapped_eps_bearer_contexts).step(d);

    // 7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
    de_tle_octet(d, ctx, 0x7b, &ret->extended_pco).step(d);
    return {uc.consumed()};
}

// clang-format off
/*
Table 8.3.8.1.1: PDU SESSION MODIFICATION REJECT message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    PDU session ID	PDU session identity	9.4	M	V	1
    PTI	Procedure transaction identity	9.6	M	V	1
    PDU SESSION MODIFICATION REJECT message identity	Message type	9.7	M	V	1
    5GSM cause	5GSM cause	9.11.4.2	M	V	1
37	Back-off timer value	GPRS timer 3	9.11.2.5	O	TLV	3
7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
TBD	Re-attempt indicator	9.11.4.17	O	TLV	3
61	5GSM congestion re-attempt indicator	9.11.4.21	O	TLV	3
*/
// clang-format on
struct pdu_session_modification_reject_t {
    nsm_header_t            header = {};
    uint8_t                 cause = {};                        // 9.11.4.2 V 1
    opt_t< uint8_t >        backoff_timer = {};                // 37 TLV
    opt_t< extended_pco_t > extended_pco = {};                 // 7B 9.11.4.6 TLV-E
    opt_t< uint8_t >        reattempt_ind = {};                // TBD TLV
    opt_t< uint8_t >        nsm_congestion_reattempt_ind = {}; // 61 TLV
};

/*
Table 8.3.9.1.1: PDU SESSION MODIFICATION COMMAND message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    PDU session ID	PDU session identity	9.4	M	V	1
    PTI	Procedure transaction identity	9.6	M	V	1
    PDU SESSION MODIFICATION COMMAND message identity	Message type	9.7	M	V	1
59	5GSM cause	5GSM cause	9.11.4.2	O	TV	2
2A	Session AMBR	Session-AMBR	9.11.4.14	O	TLV	8
56	RQ timer value	GPRS timer	9.11.2.3	O	TV	2
8-	Always-on PDU session indication	9.11.4.3 O TV 1
7A	Authorized QoS rules	QoS rules	9.11.4.13	O	TLV-E	7-65538
75	Mapped EPS bearer contexts	9.11.4.8	O	TLV-E	7-65538
79 Authorized QoS flow descriptions	9.11.4.12	O	TLV-E	6-65538
7B Extended protocol configuration options	9.11.4.6 O	TLV-E	4-65538
xx	Session-TMBR	Session-TMBR	9.11.4.19	O	TLV	8
*/
struct pdu_session_modification_command_t {
    nsm_header_t                          header = {};
    opt_t< uint8_t >                      cause = {};                      // 59 TV
    opt_t< octet_6 >                      session_ambr = {};               // 2A TLV
    opt_t< uint8_t >                      rq_timer = {};                   // 56 TV 2
    opt_t< bit4_t >                       always_on_pdu_session_ind = {};  // 8- TV 1
    opt_t< qos_rules_t >                  authorized_qos_rules = {};       // 7A TLVE
    opt_t< mapped_eps_bearer_contexts_t > mapped_eps_bearer_contexts = {}; // 75 TLVE
    opt_t< qos_flow_descriptions_t >      authorized_qos_flow_descs = {};  // 79 TLVE
    opt_t< extended_pco_t >               extended_pco = {};               // 7B TLVE
    opt_t< session_tmbr_t >               session_tmbr = {};               // XX TLV
};

/*
Table 8.3.10.1.1: PDU SESSION MODIFICATION COMPLETE message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    PDU session ID	PDU session identity	9.4	M	V	1
    PTI	Procedure transaction identity	9.6	M	V	1
    PDU SESSION MODIFICATION COMPLETE message identity	Message type	9.7	M	V	1
7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
*/
struct pdu_session_modification_complete_t {
    nsm_header_t            header = {};
    opt_t< octet_t > extended_pco = {}; // 7B TLVE
};

result_t de_pdu_session_modification_complete(dissector                            d,
                                              context*                             ctx,
                                              pdu_session_modification_complete_t* ret) {
    const use_context uc(&d, ctx, "pdu-session-modification-complete", 0);
    de_nsm_header(d, ctx, &ret->header);

    de_tle_octet(d, ctx, 0x7b, &ret->extended_pco).step(d);
    return {uc.consumed()};
}

/*
Table 8.3.11.1.1: PDU SESSION MODIFICATION COMMAND REJECT message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    PDU session ID	PDU session identity	9.4	M	V	1
    PTI	Procedure transaction identity	9.6	M	V	1
    PDU SESSION MODIFICATION COMMAND REJECT message identity	Message type	9.7	M V	1
    5GSM cause	5GSM cause	9.11.4.2	M	V	1
7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
*/
struct pdu_session_modification_command_reject_t {
    nsm_header_t            header = {};
    uint8_t                 cause = {};        // 9.11.4.2
    opt_t< octet_t > extended_pco = {}; // 7B TLVE
};

result_t de_pdu_session_modification_command_reject(
    dissector                                  d,
    context*                                   ctx,
    pdu_session_modification_command_reject_t* ret) {
    const use_context uc(&d, ctx, "pdu-session-modification-command-reject", 0);
    de_nsm_header(d, ctx, &ret->header);

    de_uint8(d, ctx, &ret->cause).step(d);
    de_tle_octet(d, ctx, 0x7b, &ret->extended_pco).step(d);
    return {uc.consumed()};
}
/*
Table 8.3.12.1.1: PDU SESSION RELEASE REQUEST message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    PDU session ID	PDU session identity	9.4	M	V	1
    PTI	Procedure transaction identity	9.6	M	V	1
    PDU SESSION RELEASE REQUEST message identity	Message type	9.7	M	V	1
59	5GSM cause	5GSM cause	9.11.4.2	O	TV	2
7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
*/
struct pdu_session_release_request_t {
    nsm_header_t header = {};
    opt_t< uint8_t >        cause = {};        // 59 TV 2
    opt_t< octet_t > extended_pco = {}; // 7B TLVE
};

result_t de_pdu_session_release_request(dissector                      d,
                                        context*                       ctx,
                                        pdu_session_release_request_t* ret) {
    const use_context uc(&d, ctx, "pdu-session-release-rquest", 0);
    de_nsm_header(d, ctx, &ret->header);

    de_t_uint8(d, ctx, 0x59, &ret->cause).step(d);
    de_tle_octet(d, ctx, 0x7b, &ret->extended_pco).step(d);
    return {uc.consumed()};
}

/*
Table 8.3.13.1.1: PDU SESSION RELEASE REJECT message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    PDU session ID	PDU session identity	9.4	M	V	1
    PTI	Procedure transaction identity	9.6	M	V	1
    PDU SESSION RELEASE REJECT message identity	Message type	9.7	M	V	1
    5GSM cause	5GSM cause	9.11.4.2	M	V	1
7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
*/
struct pdu_session_release_reject_t {
    nsm_header_t header = {};
    uint8_t                 cause= {};        // V 1
    opt_t< octet_t > extended_pco= {}; // 7B TLVE
};

result_t de_pdu_session_release_reject(dissector                     d,
                                       context*                      ctx,
                                       pdu_session_release_reject_t* ret) {
    const use_context uc(&d, ctx, "pdu-session-release-reject", 0);
    de_nsm_header(d, ctx, &ret->header);

    de_uint8(d, ctx, &ret->cause).step(d);
    de_tle_octet(d, ctx, 0x7b, &ret->extended_pco).step(d);
    return {uc.consumed()};
}

/*
Table 8.3.14.1.1: PDU SESSION RELEASE COMMAND message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    PDU session ID	PDU session identity	9.4	M	V	1
    PTI	Procedure transaction identity	9.6	M	V	1
    PDU SESSION RELEASE COMMAND message identity	Message type	9.7	M	V	1
    5GSM cause	5GSM cause	9.11.4.2	M	V	1
37	Back-off timer value	GPRS timer 3	9.11.2.5	O	TLV	3
78	EAP message	EAP message	9.11.2.2	O	TLV-E	7-1503
61	5GSM congestion re-attempt indicator	9.11.4.21	O	TLV	3
7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
*/
struct pdu_session_release_command_t {
    uint8_t                 cause= {};                        // 9.11.4.2 V 1
    opt_t< uint8_t >        backoff_timer= {};                // 37 TLV
    opt_t< octet_t >          eap= {};                          // 78 TLVE
    opt_t< uint8_t >        nsm_congestion_reattempt_ind= {}; // 61 TLV
    opt_t< octet_t > extended_pco= {};                 // 7B TLVE
};

result_t de_pdu_session_release_command(dissector                      d,
                                        context*                       ctx,
                                        pdu_session_release_command_t* ret) {
    const use_context uc(&d, ctx, "pdu-session-release-command", 0);
    /*
    Table 8.3.14.1.1: PDU SESSION RELEASE COMMAND message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        PDU session ID	PDU session identity	9.4	M	V	1
        PTI	Procedure transaction identity	9.6	M	V	1
        PDU SESSION RELEASE COMMAND message identity	Message type	9.7	M	V	1
    */
    // 5GSM cause	5GSM cause	9.11.4.2	M	V	1
    de_uint8(d, ctx, &ret->cause).step(d);

    // 37	Back-off timer value	GPRS timer 3	9.11.2.5	O	TLV	3
    de_tl_uint8(d, ctx, 0x37, &ret->backoff_timer).step(d);

    // 78	EAP message	EAP message	9.11.2.2	O	TLV-E	7-1503
    de_tle_octet(d, ctx, 0x78, &ret->eap).step(d);

    // 61	5GSM congestion re-attempt indicator	9.11.4.21	O	TLV	3
    de_tl_uint8(d, ctx, 0x61, &ret->nsm_congestion_reattempt_ind).step(d);

    // 7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
    de_tle_octet(d, ctx, 0x7b, &ret->extended_pco).step(d);
    return {uc.consumed()};
}

/*
Table 8.3.15.1.1: PDU SESSION RELEASE COMPLETE message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    PDU session ID	PDU session identity	9.4	M	V	1
    PTI	Procedure transaction identity	9.6	M	V	1
    PDU SESSION RELEASE COMPLETE message identity	Message type	9.7	M	V	1
59	5GSM cause	5GSM cause	9.11.4.2	O	TV	2
7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
*/
struct pdu_session_release_complete_t {
    nsm_header_t            header= {};
    opt_t< uint8_t >        cause= {};        // 59 TV 2 9.11.4.2
    opt_t< octet_t > extended_pco= {}; // 7B TLVE
};

result_t de_pdu_session_release_complete(dissector                       d,
                                         context*                        ctx,
                                         pdu_session_release_complete_t* ret) {
    const use_context uc(&d, ctx, "pdu-session-release-complete", 0);
    /*
    Table 8.3.15.1.1: PDU SESSION RELEASE COMPLETE message content
    IEI	Information Element	Type/Reference	Presence	Format	Length
        Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
        PDU session ID	PDU session identity	9.4	M	V	1
        PTI	Procedure transaction identity	9.6	M	V	1
        PDU SESSION RELEASE COMPLETE message identity	Message type	9.7	M	V	1
    59	5GSM cause	5GSM cause	9.11.4.2	O	TV	2
    7B	Extended protocol configuration options	9.11.4.6	O	TLV-E	4-65538
    */
    de_nsm_header(d, ctx, &ret->header);

    de_t_uint8(d, ctx, 0x59, &ret->cause).step(d);
    de_tle_octet(d, ctx, 0x7b, &ret->extended_pco).step(d);
    return {uc.consumed()};
}

/*
Table 8.3.16.1.1: 5GSM STATUS message content
IEI	Information Element	Type/Reference	Presence	Format	Length
    Extended protocol discriminator	Extended protocol discriminator	9.2	M	V	1
    PDU session ID	PDU session identity	9.4	M	V	1
    PTI	Procedure transaction identity	9.6	M	V	1
    5GSM STATUS message identity	Message type	9.7	M	V	1
    5GSM cause	5GSM cause	9.11.4.2	M	V	1
*/
struct nsm_status_t {
    nsm_header_t header;
    uint8_t      cause; // V 1 9.11.4.2
};

result_t de_nsm_status(dissector d, context* ctx, nsm_status_t* ret) {
    const use_context uc(&d, ctx, "nsm-status", 0);
    de_nsm_header(d, ctx, &ret->header);

    de_uint8(d, ctx, &ret->cause).step(d);
    return {uc.consumed()};
}
