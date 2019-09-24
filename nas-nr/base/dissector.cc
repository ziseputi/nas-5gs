#include "bufview.hh"
#include "config.hh"
#include "field_meta.hh"
#include "proto.hh"

// struct dissector defined in config.hh

void dissector::add_bits(const field_meta* metas[]) const {
    for (const field_meta** meta = metas; *meta != nullptr; meta = meta+1) {
        (void) add_item(1, *meta, enc::be); // NOLINT
    }
}

proto_node* dissector::add_item(int len, const field_meta* meta, uint32_t enc) const {
    return tree->add_item(pinfo, tvb, offset, len, meta, enc);
}

proto_node* dissector::set_item(int len, const field_meta* meta, uint32_t enc)const{
    return tree->set_item(len, meta, enc);
}

proto_node* dissector::add_expert(int len, const string&name) const{
    return tree->add_expert(pinfo, tvb, offset, len, name.c_str());
}
proto_node *dissector::add_tree(int len, const string&txt) const {
    return tree->add_subtree(pinfo, tvb, offset, len, txt.c_str());
}

void dissector::extraneous_data_check(int maxlen, context* ctx)const {
    if (maxlen < 0) return;
    if (length < 0 || length > maxlen)
        tree->add_expert(pinfo,
                         tvb,
                         offset,
                         length,
                         "extra %d bytes at %d under %s",
                         length - maxlen,
                         offset,
                         tree->name.c_str());
    if (length < 0) {
        diag("underflow at %s %d:%d\n", paths(ctx).c_str(), offset, length);
    }
    if (length > maxlen) {
        diag("extraneous data (%d) bytes at %s %d\n",
             (length - maxlen),
             paths(ctx).c_str(),
             offset);
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

dissector dissector::use_elem(void* elem)const {
    return dissector{pinfo, tree, tvb, offset, length, elem};
}

uint8_t dissector::uint8() const{
    const auto p = safe_ptr();
    if (p) return *p;
    return 0;
}

uint16_t dissector::ntohs() const {
    const auto p = safe_ptr();
    const auto l = safe_length(2);
    if (p && l > 0) return n2uint16(p);
    return 0;
}

uint32_t dissector::uint32() const {
    const auto p = safe_ptr();
    const auto l = safe_length(4);
    if (p && l > 0) return n2uint32(p);
    return 0;
}