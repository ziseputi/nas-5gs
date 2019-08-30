#include "config.hh"
#include "proto.hh"
#include "field_meta.hh"
#include "tvbuff.hh"
#include "packet_info.hh"
#include <cstdarg>

// struct dissector defined in config.hh

void dissector::add_bits(const field_meta* metas[]) {
    for (auto meta = *metas; meta != nullptr; meta++) {
        add_item(1, meta, enc::be);
    }
}
proto_node* dissector::add_item(int len, const field_meta* meta, uint32_t enc) {
    return tree->add_item(pinfo, tvb, offset, len, meta, enc);
}

proto_node *dissector::add_item(int len, const char*format, ...){
    va_list args;
    va_start(args, format);
    auto txt = vformat(format, args);
    va_end(args);
    return tree->add_subtree(pinfo, tvb, offset, len, txt.c_str());
}

void dissector::extraneous_data_check(int maxlen) {
    if (length < 0) {
        diag("overflow at %d\n", offset);
    }
    if (length > maxlen) {
        tree->add_expert(pinfo,
                         tvb,
                         offset,
                         length - maxlen,
                         "extraneous data (%d) bytes",
                         (length - maxlen));
    }
}

dissector dissector::slice(int len) const {
    auto ret   = *this;
    ret.length = len;
    return ret;
}
const uint8_t* dissector::safe_ptr() const {
    if (tvb->length < 0 || offset >= tvb->length) return nullptr;
    return tvb->data + offset;
}
int dissector::safe_length(int len) const {
    if (length < 0) return 0;
    if (len < 0) len = length - offset;
    if (offset + len <= length) return len;
    return 0;
}

void dissector::set_private(const char* name, uint64_t v) {
    pinfo->i_privates[string(name)] = v;
}
uint64_t dissector::get_private(const char* name, uint64_t v) {
    auto i = pinfo->i_privates.find(string(name));
    if (i != pinfo->i_privates.end()) return i->second;
    return v;
}

dissector dissector::use_elem(void* elem)const {
    return dissector{pinfo, tree, tvb, offset, length, elem};
}
