#pragma once
#include <cstdint>

struct packet;
struct dissector {
    packet*        pinfo       = nullptr;
    const uint8_t* view        = nullptr;
    int            view_length = 0;
    int            offset      = 0;
    int            length      = 0;

    dissector& step(int consumed);
    void       uplink();
    void       downlink();
    auto       safe_ptr(int skip = 0) const -> const uint8_t*;
    auto       safe_length(int len, int skip = 0) const -> int;
    auto       slice(int len) const -> dissector;
    auto       uint8(bool step = true, int skip = 0) -> uint8_t;
    auto       uint16(bool step = true, int skip = 0) -> uint16_t;
    auto       octet(uint8_t* to, int len, bool step = true) -> int;
};
struct context;
