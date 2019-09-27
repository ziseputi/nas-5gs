#include "../common/dissector.hh"
#include "../common/use_context.hh"

// 9.11.3.4 5GS mobile identity
int dissect_mobile_id(dissector d, context* ctx) {
    const use_context uc(ctx, "mobile-id", d, 0);

    return uc.length;
}

// type_id = 6, MAC address
int dissect_mobile_id_mac(dissector d, context* ctx) {
    const use_context uc(ctx, "mobile-id-mac", d, 0);

    return 7;
}

// type_id = 0, no identity
int dissect_mobile_id_noid(dissector d, context* ctx) {
    const use_context uc(ctx, "mobile-no-identity", d, -1);


    return 1;
}

// type_id = 4, 5G-S-TMSI
int dissect_mobile_id_5gstmsi(dissector d, context* ctx) {
    const use_context uc(ctx, "mobile-id-5gs-tmis", d, 0);


    return 7;
}

// type_id = 1, SUCI
int dissect_mobile_id_suci(dissector d, context* ctx) {
    const use_context uc(ctx, "mobile-id-suci", d, 0);

    return uc.length;
}

// type_id = 2, 5G-GUTI
int dissect_mobile_id_5gguti(dissector d, context* ctx) {
    const use_context uc(ctx, "mobile-id-5g-guti", d, 0);

    return 11;
}

// type_id = 3, IMEI
int dissect_mobile_id_imei(dissector d, context* ctx) {
    const use_context uc(ctx, "mobile-id-imei", d, 0);

    return uc.length;
}

// type_id = 5, IMEISV
int dissect_mobile_id_imeisv(dissector d, context* ctx) {
    const use_context uc(ctx, "mobile-id-imeisv", d, 0);

    return uc.length;
}