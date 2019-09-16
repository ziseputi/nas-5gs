#pragma once
#include "config.hh"
#include "tvbuff.hh"
#include "field_meta.hh"
#include "proto.hh"
#include "tree_meta.hh"
#include "packet_info.hh"

struct protocol_meta {
    const char*   name;
    const char*   full_name;
    dissect_fnc_t dissector;
};

// protocol dissect entry
extern int dissect_nas5g(dissector, context*);

NASNR_INLINE const protocol_meta nas_5gs_module = {
    "NAS-5GS",
    "Non-Access-Stratum 5GS (NAS)PDU",
    dissect_nas5g,
};

// Extended protocol discriminator
namespace epd {
NASNR_INLINE const uint8_t MM5G = 0x7e; // TGPP_PD_5GMM
NASNR_INLINE const uint8_t SM5G = 0x2e; // TGPP_PD_5GSM
} // namespace EPD

typedef dissect_fnc_t dissect_msg_fnc_t;

struct message_meta {
    uint8_t           type; // iei
    const char*       name;
    dissect_msg_fnc_t fnc;
    const char*       alias;
};

typedef message_meta element_meta;

inline const char* safe_str(const char* v) { return (v && v[0] != '\n') ? v : ""; }
