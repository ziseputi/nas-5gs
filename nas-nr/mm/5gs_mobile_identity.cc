#include "../common/common.hh"
#include "../common/dissect_mm_msg.hh"

using namespace cmn;
using namespace mm;

const element_meta mm::guti_mobile_id = {
    0x77,
    "5GS mobile identity - 5G-GUTI",
    dissect_mobile_id,
    nullptr,
};

// 9.11.3.4 5GS mobile identity
int mm::dissect_mobile_id(dissector d, context* ctx) {
    const use_context uc(ctx, "mobile-id", d, 0);

    const auto oct      = d.uint8();
    const auto type_id  = oct & 0x07u;

    int consumed = 0;
    switch (type_id) {
    case 0:
        // no identity
        consumed = dissect_mobile_id_noid(d, ctx);
        d.step(consumed);
        break;
    case 1:
        // SUCI
        consumed = dissect_mobile_id_suci(d, ctx);
        d.step(consumed);
        break;
    case 2:
        // 5G-GUTI
        consumed = dissect_mobile_id_5gguti(d, ctx);
        d.step(consumed);
        break;
    case 3:
        // IMEI
        dissect_mobile_id_imei(d, ctx);
        d.step(d.length);
        break;
    case 4:
        // 5G-S-TMSI
        consumed = dissect_mobile_id_5gstmsi(d, ctx);
        d.step(consumed);
        break;
    case 5:
        // IMEISV
        dissect_mobile_id_imeisv(d, ctx);
        d.step(d.length);
        break;
    case 6:
        // MAC address
        dissect_mobile_id_mac(d, ctx);
        d.step(7);
        break;
    default:
        (void) d.add_item(d.length, &hf_identity_type, enc::be);
        diag("unknown mobile type id %d\n", type_id);
        break;
    }

    return uc.length;
}

// type_id = 6, MAC address
int mm::dissect_mobile_id_mac(dissector d, context* ctx) {
    const use_context uc(ctx, "mobile-id-mac", d, 0);

    (void) d.add_item(1, &hf_identity_type, enc::be);
    d.step(1);

    (void) d.add_item(6, &hf_mac_address, enc::be);
    d.step(6);

    return 7;
}

static const field_meta* flags_odd_even_id_type[] = {
    &hf_odd_even_indication,
    &hf_identity_type,
    nullptr,
};
// type_id = 0, no identity
int mm::dissect_mobile_id_noid(dissector d, context* ctx) {
    const use_context uc(ctx, "mobile-no-identity", d, -1);

    d.add_bits(flags_odd_even_id_type);
    d.step(1);

    return 1;
}

// type_id = 4, 5G-S-TMSI
int mm::dissect_mobile_id_5gstmsi(dissector d, context* ctx) {
    const use_context uc(ctx, "mobile-id-5gs-tmis", d, 0);

    (void) d.add_item(1, &hf_identity_type, enc::be);
    d.step(1);

    /* AMF Set ID */
    (void) d.add_item(2, &hf_amf_set_id, enc::be);
    d.step(1);

    /* AMF Pointer AMF */
    (void) d.add_item(1, &hf_amf_pointer, enc::be);
    d.step(1);

    (void) d.add_item(4, &hf_5g_tmsi, enc::be);
    d.step(4);

    return 7;
}

// type_id = 1, SUCI
int mm::dissect_mobile_id_suci(dissector d, context* ctx) {
    const use_context uc(ctx, "mobile-id-suci", d, 0);

    const auto oct = d.uint8();

    static const field_meta* flags_supi_fmt_tid[] = {
        // &hf_spare_b7,
        &hf_supi_format,
        // &hf_spare_b3,
        &hf_identity_type,
        nullptr,
    };

    d.add_bits(flags_supi_fmt_tid);
    d.step(1);

    const auto supi_fmt = oct & 0x70u;
    if (supi_fmt != 1) { // supi_fmt == 0
        // IMSI
        const auto consumed = dissect_e212_mcc_mnc(d, ctx);
        d.step(consumed);

        /* Routing indicator octet 8-9 */
        (void) d.add_item(2, &hf_routing_indication, enc::be);
        d.step(2);

        /* Protection scheme id octet 10 */
        const auto scheme_id = d.uint8() & 0x0fu;
        (void) d.add_item(1, &hf_protection_scheme_id, enc::be);
        d.step(1);

        /* Home network public key identifier octet 11 */
        (void) d.add_item(1, &hf_public_key_identifier, enc::be);
        d.step(1);

        /* Scheme output octet 12-x */
        // Null scheme
        if (scheme_id == 0)
            (void) d.add_item(d.length, &hf_msin, enc::be);
        else
            (void) d.add_item(d.length, &hf_mm_scheme_output, enc::na);

        d.step(d.length);
    }
    if (supi_fmt == 1) {
        // nai, network specific identifier
        (void) d.add_item(d.length, &hf_suci_nai, enc::be);
        d.step(d.length);
    }
    return uc.length;
}

// type_id = 2, 5G-GUTI
int mm::dissect_mobile_id_5gguti(dissector d, context* ctx) {
    const use_context uc(ctx, "mobile-id-5g-guti", d, 0);

    (void) d.add_item(1, &hf_identity_type, enc::be);
    d.step(1);

    const auto consumed = dissect_e212_mcc_mnc(d, ctx);
    d.step(consumed);

    /* AMF Region ID octet 7 */
    (void) d.add_item(1, &hf_amf_region_id, enc::be);
    d.step(1);

    /* AMF Set ID octet 8-9 10bits */
    (void) d.add_item(2, &hf_amf_set_id, enc::be);
    d.step(1);

    /* AMF AMF Pointer*/
    (void) d.add_item(1, &hf_amf_pointer, enc::be);
    d.step(1);

    (void) d.add_item(4, &hf_5g_tmsi, enc::be);
    d.step(4);

    return 11;
}

// type_id = 3, IMEI
int mm::dissect_mobile_id_imei(dissector d, context* ctx) {
    const use_context uc(ctx, "mobile-id-imei", d, 0);

    d.add_bits(flags_odd_even_id_type);

    // The format of the IMEI is described in 3GPP TS 23.003
    (void) d.add_item(d.length, &hf_imei, enc::be);
    d.step(d.length);

    return uc.length;
}

// type_id = 5, IMEISV
int mm::dissect_mobile_id_imeisv(dissector d, context* ctx) {
    const use_context uc(ctx, "mobile-id-imeisv", d, 0);

    d.add_bits(flags_odd_even_id_type);

    // The format of the IMEISV is described in 3GPP TS 23.003
    (void) d.add_item(d.length, &hf_imeisv, enc::be);
    d.step(d.length);

    return uc.length;
}

extern const field_meta mm::hf_odd_even_indication = {
    "Odd/even indication",
    "nas.nr.odd.even",
    ft::ft_boolean,
    fd::base_dec,
    nullptr,
    &tfs_odd_even,
    nullptr,
    0x08,
};

// 5GS mobile identity  9.11.3.4
const element_meta mm::mobile_id = {
    0x77,
    "5GS mobile identity",
    dissect_mobile_id,
    nullptr,
};

namespace mm {

const field_meta hf_mac_address = {
    "MAC address",
    "nas.nr.mac",
    ft::ft_bytes,
    fd::sep_colon,
    nullptr,
    nullptr,
    nullptr,
    0,
};
const val_string supi_fmt_values[] = {
    {0x0, "IMSI"},
    {0x1, "Network Specific Identifier"},
    {0, nullptr},
};
const field_meta hf_supi_format = {
    "SUPI format",
    "nas.nr.suci.supi.format",
    ft::ft_uint8,
    fd::base_dec,
    supi_fmt_values,
    nullptr,
    nullptr,
    0x70,
};

const field_meta hf_suci_nai = {
    "NAI",
    "nas.nr.suci.nai",
    ft::ft_bytes,
    fd::base_string,
    nullptr,
    nullptr,
    nullptr,
    0,
};
const field_meta hf_imei = {
    "IMEI",
    "nas.nr.imei",
    ft::ft_bytes,
    fd::imei,
    nullptr,
    nullptr,
    nullptr,
    0,
};

const field_meta hf_imeisv = {
    "IMEISV",
    "nas.nr.imeisv",
    ft::ft_bytes,
    fd::imei,
    nullptr,
    nullptr,
    nullptr,
    0,
};

const field_meta hf_amf_set_id = {
    "AMF Set ID",
    "nas.nr.amf",
    ft::ft_uint16,
    fd::base_dec,
    nullptr,
    nullptr,
    nullptr,
    0xffc0,
};

const field_meta hf_amf_pointer = {
    "AMF Pointer",
    "nas.nr.amf.pointer",
    ft::ft_uint8,
    fd::base_dec,
    nullptr,
    nullptr,
    nullptr,
    0x3f,
};


const field_meta hf_5g_tmsi = {
    "5G-TMSI",
    "nas.nr.5g.tmsi",
    ft::ft_bytes,
    fd::base_hex,
    nullptr,
    nullptr,
    nullptr,
    0x0,
};

const field_meta hf_routing_indication = {
    "Routing indicator",
    "nas.nr.suci.routing.ind",
    ft::ft_bytes,
    fd::bcd,
    nullptr,
    nullptr,
    nullptr,
    0,
};

static const value_string prot_scheme_id_vals[] = {
    {0x0, "NULL scheme"},
    {0x1, "ECIES scheme profile A"},
    {0x2, "ECIES scheme profile B"},
    {0, nullptr},
};

const field_meta hf_protection_scheme_id = {
    "Protection scheme Id",
    "nas.nr.suci.scheme.id",
    ft::ft_uint8,
    fd::base_dec,
    prot_scheme_id_vals,
    nullptr,
    nullptr,
    0x0f,
};
const field_meta hf_public_key_identifier = {
    "Home network public key identifier",
    "nas.nr.suci.pki",
    ft::ft_uint8,
    fd::base_dec,
    nullptr,
    nullptr,
    nullptr,
    0x0,
};

const field_meta hf_msin = {
    "Scheme output - MSIN",
    "nas.nr.scheme.msin",
    ft::ft_bytes,
    fd::bcd,
    nullptr,
    nullptr,
    nullptr,
    0,
};

const field_meta hf_mm_scheme_output = {
    "Scheme output - Network specific identifier",
    "nas.nr.scheme.network.specific.id",
    ft::ft_bytes,
    fd::base_none,
    nullptr,
    nullptr,
    nullptr,
    0x0,
};

const field_meta hf_amf_region_id = {
    "AMF Region ID",
    "nas.nr.amf.region",
    ft::ft_uint8,
    fd::base_dec,
    nullptr,
    nullptr,
    nullptr,
    0x0,
};


} // namespace mm
