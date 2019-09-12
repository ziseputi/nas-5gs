#include "../dissect_mm_msg.hh"

using namespace cmn;
using namespace nas;
using namespace mm;

// 9.11.3.32	NAS key set identifier
int mm::dissect_nas_ksi(dissector d, context* ctx) {
    use_context uc(ctx, "NAS key set identifier", d, -1);

    static const field_meta* flags[] = {
        &hf_tsc,
        hf_nas_eps_nas_ksi,
        nullptr,
    };

    /* NAS key set identifier IEI   TSC     NAS key set identifier */
    d.add_bits(flags);
    d.step(1);

    return 1;
}


/* NAS key set identifier (octet 1) Bits 3  2   1 */
extern const val_string mm::nas_ksi_values[] = {
    {0, "0"},
    {1, "1"},
    {2, "2"},
    {3, "3"},
    {4, "4"},
    {5, "5"},
    {6, "6"},
    {7, "No key is available"},
    {0, nullptr},
};

// only spare half octet
const field_meta mm::hf_ngksi_nas_ksi = {
    "NAS key set identifier - ngKSI",
    "nas.nr.mm.ngksi",
    ft::ft_uint8,
    fd::base_dec,
    nullptr,
    nullptr,
    nullptr,
    0xF0,
};

// NAS key set identifier 9.11.3.32
extern const element_meta mm::nas_ksi = {
    0xff,
    "NAS key set identifier - ngKSI",
    dissect_nas_ksi,
    nullptr,
};

