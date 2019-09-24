#pragma once
#include "../base/common.hh"
#include "../base/config.hh"
#include "../base/core.hh"
#include "../sm/sm_dissects.hh"
#include "../sm/sm_element_meta.hh"
#include "../sm/sm_field_meta.hh"
#include "dissect_nas5g.hh"

struct context;

/* 5GS session management messages */
namespace sm {
inline const dissect_fnc_t no_dissect = nullptr;

extern const tf_string         tfs_default_qos_rule;
extern const message_meta      msgs[];
extern const v_string      ambr_values[];
extern const v_string      nrsm_cause_values[];
extern const v_string      rule_operation_code_values[];
extern const v_string      packet_filter_component_type_values[];
extern const v_string      packet_filter_direction_values[];
extern const v_string      operation_code_values[];
extern const v_string      deb_bit_values[];
extern const v_string      e_bit_values[];
extern const v_string      e_bit_modify_values[];
extern const v_string      eps_parameter_identity_values[];
extern const v_string      integrity_protection_max_data_rate_values[];
extern const v_string      ssc_mode_values[];

inline const tf_string tfs_allowed_not_allowed = {
    "Allowed",
    "Not Allowed",
};

} // namespace sm