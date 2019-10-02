#include "../common/dissector.hh"
#include "../common/use_context.hh"

/*  9.11.3.13    Allowed PDU session status */
int dissect_allowed_pdu_session_status(dissector d, context* ctx) {
    const use_context uc(&d, ctx, "allowed-pdu-session-status", 30);
    return uc.length;
}
