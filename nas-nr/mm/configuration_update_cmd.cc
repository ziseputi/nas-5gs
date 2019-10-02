#include "../common/dissector.hh"
#include "../common/use_context.hh"

/* 8.2.19 Configuration update command */
int dissect_configuration_update_command(dissector d, context* ctx) {
    use_context uc(&d, ctx, "configuration-update-command", 3);
    // network to UE
    down_link(d.pinfo);


    return uc.length;
}
