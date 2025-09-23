#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#define DEBUG_MODE 0

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#ifndef CC_UNLIKELY
#define CC_UNLIKELY(...) unlikely(__VA_ARGS__)
#endif

#ifndef CC_LIKELY
#define CC_LIKELY(...) likely(__VA_ARGS__)
#endif

#if DEBUG_MODE == 1
// if DEBUG_MODE is on print log info to logcat
#define LOG_TAG "GL_CHECKER"
#define PRINT_LOG_WITH_ARG(func_name, macro_name, arg, line) \
    ALOGE("%s => %s arg: 0x%x line %d", func_name, macro_name, (int)arg, line);
#define PRINT_LOG(func_name, macro_name) \
    ALOGE("%s => %s ", func_name, macro_name);
#else
#define PRINT_LOG_WITH_ARG(func_name, macro_name, arg, line)
#define PRINT_LOG(func_name, macro_name)
#endif

// Declare res_ctx first use CHECK_RESOURCE_ID in gl_functions
#define CHECK_RESOURCE_ID(line, res_name, id, error_msg)                \
    ResourceContext *res_ctx =                                         \
        &(((GLThreadContext *)context)->resource_context);              \
    if (CC_UNLIKELY(!res_ctx->res_name##_resource()->has_id(id))) {     \
        PRINT_LOG_WITH_ARG(__FUNCTION__, "CHECK_RESOURCE_ID", id, line) \
        set_gl_error(context, error_msg);                               \
        return;                                                         \
    }

// subsequently CHECK ID without declaring the res_ctx
#define CHECK_RESOURCE_ID_CONTINUELY(line, res_name, id, error_msg)          \
    if (CC_UNLIKELY(!res_ctx->res_name##_resource()->has_id(id))) {          \
        PRINT_LOG_WITH_ARG(__FUNCTION__, "CHECK_RESOURCE_ID_CONTINUELY", id, \
                           line)                                             \
        set_gl_error(context, error_msg);                                    \
        return;                                                              \
    }

// check id if its active
#define CHECK_RESOURCE_ACTIVE(line, res_name, id, error_msg)                \
    ResourceContext *res_ctx =                                             \
        &(((Direct_GPU_Context *)context)->resource_context);               \
    if (CC_UNLIKELY(!res_ctx->res_name##_resource->is_active(id))) {        \
        PRINT_LOG_WITH_ARG(__FUNCTION__, "CHECK_RESOURCE_ACTIVE", id, line) \
        set_gl_error(context, error_msg);                                   \
        return;                                                             \
    }

// check id if its deactive mode
#define CHECK_RESOURCE_DEACTIVE(line, res_name, id, error_msg)                \
    ResourceContext *res_ctx =                                               \
        &(((Direct_GPU_Context *)context)->resource_context);                 \
    if (CC_UNLIKELY(res_ctx->res_name##_resource->is_active(id))) {           \
        PRINT_LOG_WITH_ARG(__FUNCTION__, "CHECK_RESOURCE_DEACTIVE", id, line) \
        set_gl_error(context, error_msg);                                     \
        return;                                                               \
    }

// check if it's greater than or equal to some GL_MAX int, will do glGetIntegerv
// first to get MAX
#define CHECK_INT_MAX(line, tar, max_enum, error_msg)                \
    GLint tar##_max;                                                 \
    d_glGetIntegerv(context, max_enum, &tar##_max);                  \
    if (CC_UNLIKELY(tar >= tar##_max)) {                             \
        PRINT_LOG_WITH_ARG(__FUNCTION__, "CHECK_INT_MAX", tar, line) \
        set_gl_error(context, error_msg);                            \
        return;                                                      \
    }

// check if tar is in a valid range from 0 ~ current active attrib
#define CHECK_PROGRAM_INFO(line, tar, Enum, error_msg)                    \
    GLint para;                                                           \
    d_glGetProgramiv(context, program, Enum, &para);                      \
    if (CC_UNLIKELY(tar >= para)) {                                       \
        PRINT_LOG_WITH_ARG(__FUNCTION__, "CHECK_PROGRAM_INFO", tar, line) \
        set_gl_error(context, error_msg);                                 \
        return;                                                           \
    }

// check if the program has been linked using glLinkProgram, if the program is
// linked it should have program info
#define CHECK_PROGRAM_LINKED                                                 \
    if (CC_UNLIKELY(                                                         \
            ((GLThreadContext *)context)->program_info_map->find(program) == \
            ((GLThreadContext *)context)->program_info_map->end())) {        \
        PRINT_LOG(__FUNCTION__, "CHECK_PROGRAM_LINKED")                      \
        set_gl_error(context, GL_INVALID_OPERATION);                         \
        return;                                                              \
    }

#define CHECK_BUFFER_BITS(mask, N, error_msg, con, ...)                        \
    if (CC_UNLIKELY(!(mask & con)                                              \
                        LOOP_BITS_CONDITIONS_##N(mask, __VA_ARGS__))) {        \
        /*ALOGE("%s => %s mask %x", __FUNCTION__, "CHECK_BUFFER_BITS", mask)*/ \
        set_gl_error(context, error_msg);                                      \
        return;                                                                \
    }

#define CHECK_TARGET_INDEX_MAX(target, special_target, index,    \
                               target_max_index, error_msg)      \
    if (CC_UNLIKELY(target == special_target &&                  \
                    index >= static_values->target_max_index)) { \
        PRINT_LOG(__FUNCTION__, "CHECK_BUFFER_BITS")             \
        set_gl_error(context, error_msg);                        \
        return;                                                  \
    }

#define LOOP_BITS_CONDITIONS_6(mask, con, ...) \
    &&!(mask & con) LOOP_BITS_CONDITIONS_5(mask, __VA_ARGS__)
#define LOOP_BITS_CONDITIONS_5(mask, con, ...) \
    &&!(mask & con) LOOP_BITS_CONDITIONS_4(mask, __VA_ARGS__)
#define LOOP_BITS_CONDITIONS_4(mask, con, ...) \
    &&!(mask & con) LOOP_BITS_CONDITIONS_3(mask, __VA_ARGS__)
#define LOOP_BITS_CONDITIONS_3(mask, con, ...) \
    &&!(mask & con) LOOP_BITS_CONDITIONS_2(mask, __VA_ARGS__)
#define LOOP_BITS_CONDITIONS_2(mask, con, ...) \
    &&!(mask & con) LOOP_BITS_CONDITIONS_1(mask, __VA_ARGS__)
#define LOOP_BITS_CONDITIONS_1(mask, con)

// check target GLenum if it is not one of valid ENUM, N represent the total
// number of ENUMs to check
#define CHECK_VALID_ENUM(line, target, N, con, ...)                            \
    if (CC_UNLIKELY(target != con LOOP_CONDITIONS_##N(target, __VA_ARGS__))) { \
        PRINT_LOG_WITH_ARG(__FUNCTION__, "CHECK_VALID_ENUM", target, line)     \
        set_gl_error(context, GL_INVALID_ENUM);                                \
        return;                                                                \
    }

#define CHECK_VALID_ENUM_WITH_ERROR(line, target, N, error_msg, con, ...)      \
    if (CC_UNLIKELY(target != con LOOP_CONDITIONS_##N(target, __VA_ARGS__))) { \
        PRINT_LOG_WITH_ARG(__FUNCTION__, "CHECK_VALID_ENUM_WITH_ERROR",        \
                           target, line)                                       \
        set_gl_error(context, error_msg);                                      \
        return;                                                                \
    }
#define LOOP_CONDITIONS_95(target, con, ...) \
    &&target != con LOOP_CONDITIONS_94(target, __VA_ARGS__)
#define LOOP_CONDITIONS_94(target, con, ...) \
    &&target != con LOOP_CONDITIONS_93(target, __VA_ARGS__)
#define LOOP_CONDITIONS_93(target, con, ...) \
    &&target != con LOOP_CONDITIONS_92(target, __VA_ARGS__)
#define LOOP_CONDITIONS_92(target, con, ...) \
    &&target != con LOOP_CONDITIONS_91(target, __VA_ARGS__)
#define LOOP_CONDITIONS_91(target, con, ...) \
    &&target != con LOOP_CONDITIONS_90(target, __VA_ARGS__)
#define LOOP_CONDITIONS_90(target, con, ...) \
    &&target != con LOOP_CONDITIONS_89(target, __VA_ARGS__)
#define LOOP_CONDITIONS_89(target, con, ...) \
    &&target != con LOOP_CONDITIONS_88(target, __VA_ARGS__)
#define LOOP_CONDITIONS_88(target, con, ...) \
    &&target != con LOOP_CONDITIONS_87(target, __VA_ARGS__)
#define LOOP_CONDITIONS_87(target, con, ...) \
    &&target != con LOOP_CONDITIONS_86(target, __VA_ARGS__)
#define LOOP_CONDITIONS_86(target, con, ...) \
    &&target != con LOOP_CONDITIONS_85(target, __VA_ARGS__)
#define LOOP_CONDITIONS_85(target, con, ...) \
    &&target != con LOOP_CONDITIONS_84(target, __VA_ARGS__)
#define LOOP_CONDITIONS_84(target, con, ...) \
    &&target != con LOOP_CONDITIONS_83(target, __VA_ARGS__)
#define LOOP_CONDITIONS_83(target, con, ...) \
    &&target != con LOOP_CONDITIONS_82(target, __VA_ARGS__)
#define LOOP_CONDITIONS_82(target, con, ...) \
    &&target != con LOOP_CONDITIONS_81(target, __VA_ARGS__)
#define LOOP_CONDITIONS_81(target, con, ...) \
    &&target != con LOOP_CONDITIONS_80(target, __VA_ARGS__)
#define LOOP_CONDITIONS_80(target, con, ...) \
    &&target != con LOOP_CONDITIONS_79(target, __VA_ARGS__)
#define LOOP_CONDITIONS_79(target, con, ...) \
    &&target != con LOOP_CONDITIONS_78(target, __VA_ARGS__)
#define LOOP_CONDITIONS_78(target, con, ...) \
    &&target != con LOOP_CONDITIONS_77(target, __VA_ARGS__)
#define LOOP_CONDITIONS_77(target, con, ...) \
    &&target != con LOOP_CONDITIONS_76(target, __VA_ARGS__)
#define LOOP_CONDITIONS_76(target, con, ...) \
    &&target != con LOOP_CONDITIONS_75(target, __VA_ARGS__)
#define LOOP_CONDITIONS_75(target, con, ...) \
    &&target != con LOOP_CONDITIONS_74(target, __VA_ARGS__)
#define LOOP_CONDITIONS_74(target, con, ...) \
    &&target != con LOOP_CONDITIONS_73(target, __VA_ARGS__)
#define LOOP_CONDITIONS_73(target, con, ...) \
    &&target != con LOOP_CONDITIONS_72(target, __VA_ARGS__)
#define LOOP_CONDITIONS_72(target, con, ...) \
    &&target != con LOOP_CONDITIONS_71(target, __VA_ARGS__)
#define LOOP_CONDITIONS_71(target, con, ...) \
    &&target != con LOOP_CONDITIONS_70(target, __VA_ARGS__)
#define LOOP_CONDITIONS_70(target, con, ...) \
    &&target != con LOOP_CONDITIONS_69(target, __VA_ARGS__)
#define LOOP_CONDITIONS_69(target, con, ...) \
    &&target != con LOOP_CONDITIONS_68(target, __VA_ARGS__)
#define LOOP_CONDITIONS_68(target, con, ...) \
    &&target != con LOOP_CONDITIONS_67(target, __VA_ARGS__)
#define LOOP_CONDITIONS_67(target, con, ...) \
    &&target != con LOOP_CONDITIONS_66(target, __VA_ARGS__)
#define LOOP_CONDITIONS_66(target, con, ...) \
    &&target != con LOOP_CONDITIONS_65(target, __VA_ARGS__)
#define LOOP_CONDITIONS_65(target, con, ...) \
    &&target != con LOOP_CONDITIONS_64(target, __VA_ARGS__)
#define LOOP_CONDITIONS_64(target, con, ...) \
    &&target != con LOOP_CONDITIONS_63(target, __VA_ARGS__)
#define LOOP_CONDITIONS_63(target, con, ...) \
    &&target != con LOOP_CONDITIONS_62(target, __VA_ARGS__)
#define LOOP_CONDITIONS_62(target, con, ...) \
    &&target != con LOOP_CONDITIONS_61(target, __VA_ARGS__)
#define LOOP_CONDITIONS_61(target, con, ...) \
    &&target != con LOOP_CONDITIONS_60(target, __VA_ARGS__)
#define LOOP_CONDITIONS_60(target, con, ...) \
    &&target != con LOOP_CONDITIONS_59(target, __VA_ARGS__)
#define LOOP_CONDITIONS_59(target, con, ...) \
    &&target != con LOOP_CONDITIONS_58(target, __VA_ARGS__)
#define LOOP_CONDITIONS_58(target, con, ...) \
    &&target != con LOOP_CONDITIONS_57(target, __VA_ARGS__)
#define LOOP_CONDITIONS_57(target, con, ...) \
    &&target != con LOOP_CONDITIONS_56(target, __VA_ARGS__)
#define LOOP_CONDITIONS_56(target, con, ...) \
    &&target != con LOOP_CONDITIONS_55(target, __VA_ARGS__)
#define LOOP_CONDITIONS_55(target, con, ...) \
    &&target != con LOOP_CONDITIONS_54(target, __VA_ARGS__)
#define LOOP_CONDITIONS_54(target, con, ...) \
    &&target != con LOOP_CONDITIONS_53(target, __VA_ARGS__)
#define LOOP_CONDITIONS_53(target, con, ...) \
    &&target != con LOOP_CONDITIONS_52(target, __VA_ARGS__)
#define LOOP_CONDITIONS_52(target, con, ...) \
    &&target != con LOOP_CONDITIONS_51(target, __VA_ARGS__)
#define LOOP_CONDITIONS_51(target, con, ...) \
    &&target != con LOOP_CONDITIONS_50(target, __VA_ARGS__)
#define LOOP_CONDITIONS_50(target, con, ...) \
    &&target != con LOOP_CONDITIONS_49(target, __VA_ARGS__)
#define LOOP_CONDITIONS_49(target, con, ...) \
    &&target != con LOOP_CONDITIONS_48(target, __VA_ARGS__)
#define LOOP_CONDITIONS_48(target, con, ...) \
    &&target != con LOOP_CONDITIONS_47(target, __VA_ARGS__)
#define LOOP_CONDITIONS_47(target, con, ...) \
    &&target != con LOOP_CONDITIONS_46(target, __VA_ARGS__)
#define LOOP_CONDITIONS_46(target, con, ...) \
    &&target != con LOOP_CONDITIONS_45(target, __VA_ARGS__)
#define LOOP_CONDITIONS_45(target, con, ...) \
    &&target != con LOOP_CONDITIONS_44(target, __VA_ARGS__)
#define LOOP_CONDITIONS_44(target, con, ...) \
    &&target != con LOOP_CONDITIONS_43(target, __VA_ARGS__)
#define LOOP_CONDITIONS_43(target, con, ...) \
    &&target != con LOOP_CONDITIONS_42(target, __VA_ARGS__)
#define LOOP_CONDITIONS_42(target, con, ...) \
    &&target != con LOOP_CONDITIONS_41(target, __VA_ARGS__)
#define LOOP_CONDITIONS_41(target, con, ...) \
    &&target != con LOOP_CONDITIONS_40(target, __VA_ARGS__)
#define LOOP_CONDITIONS_40(target, con, ...) \
    &&target != con LOOP_CONDITIONS_39(target, __VA_ARGS__)
#define LOOP_CONDITIONS_39(target, con, ...) \
    &&target != con LOOP_CONDITIONS_38(target, __VA_ARGS__)
#define LOOP_CONDITIONS_38(target, con, ...) \
    &&target != con LOOP_CONDITIONS_37(target, __VA_ARGS__)
#define LOOP_CONDITIONS_37(target, con, ...) \
    &&target != con LOOP_CONDITIONS_36(target, __VA_ARGS__)
#define LOOP_CONDITIONS_36(target, con, ...) \
    &&target != con LOOP_CONDITIONS_35(target, __VA_ARGS__)
#define LOOP_CONDITIONS_35(target, con, ...) \
    &&target != con LOOP_CONDITIONS_34(target, __VA_ARGS__)
#define LOOP_CONDITIONS_34(target, con, ...) \
    &&target != con LOOP_CONDITIONS_33(target, __VA_ARGS__)
#define LOOP_CONDITIONS_33(target, con, ...) \
    &&target != con LOOP_CONDITIONS_32(target, __VA_ARGS__)
#define LOOP_CONDITIONS_32(target, con, ...) \
    &&target != con LOOP_CONDITIONS_31(target, __VA_ARGS__)
#define LOOP_CONDITIONS_31(target, con, ...) \
    &&target != con LOOP_CONDITIONS_30(target, __VA_ARGS__)
#define LOOP_CONDITIONS_30(target, con, ...) \
    &&target != con LOOP_CONDITIONS_29(target, __VA_ARGS__)
#define LOOP_CONDITIONS_29(target, con, ...) \
    &&target != con LOOP_CONDITIONS_28(target, __VA_ARGS__)
#define LOOP_CONDITIONS_28(target, con, ...) \
    &&target != con LOOP_CONDITIONS_27(target, __VA_ARGS__)
#define LOOP_CONDITIONS_27(target, con, ...) \
    &&target != con LOOP_CONDITIONS_26(target, __VA_ARGS__)
#define LOOP_CONDITIONS_26(target, con, ...) \
    &&target != con LOOP_CONDITIONS_25(target, __VA_ARGS__)
#define LOOP_CONDITIONS_25(target, con, ...) \
    &&target != con LOOP_CONDITIONS_24(target, __VA_ARGS__)
#define LOOP_CONDITIONS_24(target, con, ...) \
    &&target != con LOOP_CONDITIONS_23(target, __VA_ARGS__)
#define LOOP_CONDITIONS_23(target, con, ...) \
    &&target != con LOOP_CONDITIONS_22(target, __VA_ARGS__)
#define LOOP_CONDITIONS_22(target, con, ...) \
    &&target != con LOOP_CONDITIONS_21(target, __VA_ARGS__)
#define LOOP_CONDITIONS_21(target, con, ...) \
    &&target != con LOOP_CONDITIONS_20(target, __VA_ARGS__)
#define LOOP_CONDITIONS_20(target, con, ...) \
    &&target != con LOOP_CONDITIONS_19(target, __VA_ARGS__)
#define LOOP_CONDITIONS_19(target, con, ...) \
    &&target != con LOOP_CONDITIONS_18(target, __VA_ARGS__)
#define LOOP_CONDITIONS_18(target, con, ...) \
    &&target != con LOOP_CONDITIONS_17(target, __VA_ARGS__)
#define LOOP_CONDITIONS_17(target, con, ...) \
    &&target != con LOOP_CONDITIONS_16(target, __VA_ARGS__)
#define LOOP_CONDITIONS_16(target, con, ...) \
    &&target != con LOOP_CONDITIONS_15(target, __VA_ARGS__)
#define LOOP_CONDITIONS_15(target, con, ...) \
    &&target != con LOOP_CONDITIONS_14(target, __VA_ARGS__)
#define LOOP_CONDITIONS_14(target, con, ...) \
    &&target != con LOOP_CONDITIONS_13(target, __VA_ARGS__)
#define LOOP_CONDITIONS_13(target, con, ...) \
    &&target != con LOOP_CONDITIONS_12(target, __VA_ARGS__)
#define LOOP_CONDITIONS_12(target, con, ...) \
    &&target != con LOOP_CONDITIONS_11(target, __VA_ARGS__)
#define LOOP_CONDITIONS_11(target, con, ...) \
    &&target != con LOOP_CONDITIONS_10(target, __VA_ARGS__)
#define LOOP_CONDITIONS_10(target, con, ...) \
    &&target != con LOOP_CONDITIONS_9(target, __VA_ARGS__)
#define LOOP_CONDITIONS_9(target, con, ...) \
    &&target != con LOOP_CONDITIONS_8(target, __VA_ARGS__)
#define LOOP_CONDITIONS_8(target, con, ...) \
    &&target != con LOOP_CONDITIONS_7(target, __VA_ARGS__)
#define LOOP_CONDITIONS_7(target, con, ...) \
    &&target != con LOOP_CONDITIONS_6(target, __VA_ARGS__)
#define LOOP_CONDITIONS_4(target, con, ...) \
    &&target != con LOOP_CONDITIONS_3(target, __VA_ARGS__)
#define LOOP_CONDITIONS_6(target, con, ...) \
    &&target != con LOOP_CONDITIONS_5(target, __VA_ARGS__)
#define LOOP_CONDITIONS_5(target, con, ...) \
    &&target != con LOOP_CONDITIONS_4(target, __VA_ARGS__)
#define LOOP_CONDITIONS_4(target, con, ...) \
    &&target != con LOOP_CONDITIONS_3(target, __VA_ARGS__)
#define LOOP_CONDITIONS_3(target, con, ...) \
    &&target != con LOOP_CONDITIONS_2(target, __VA_ARGS__)
#define LOOP_CONDITIONS_2(target, con, ...) \
    &&target != con LOOP_CONDITIONS_1(target, __VA_ARGS__)
#define LOOP_CONDITIONS_1(target, con)

// check if target equal to 0
#define CHECK_ZERO(line, target, error_msg)                          \
    if (CC_UNLIKELY(target == 0)) {                                  \
        PRINT_LOG_WITH_ARG(__FUNCTION__, "CHECK_ZERO", target, line) \
        set_gl_error(context, error_msg);                            \
        return;                                                      \
    }

// check if target is negative
#define CHECK_NEGATIVE(line, target, error_msg)                          \
    if (CC_UNLIKELY(target < 0)) {                                       \
        PRINT_LOG_WITH_ARG(__FUNCTION__, "CHECK_NEGATIVE", target, line) \
        set_gl_error(context, error_msg);                                \
        return;                                                          \
    }

/*GL_INVALID_FRAMEBUFFER_OPERATION is generated if the currently bound
 * framebuffer is not framebuffer complete (i.e. the return value from
 * glCheckFramebufferStatus is not GL_FRAMEBUFFER_COMPLETE). errors that have to
 * send to host
 */
#define CHECK_DRAW_FRAMEBUFFER_COMPLETE(error_msg) \
    set_host_error(context);                       \
    //GLenum fbo_complete;\
//fbo_complete = d_glCheckFramebufferStatus(context,GL_DRAW_FRAMEBUFFER);\
//if(fbo_complete!=GL_FRAMEBUFFER_COMPLETE){PRINT_LOG(__FUNCTION__,"CHECK_DRAW_FRAMEBUFFER_COMPLETE")set_gl_error(context,error_msg);return;}

#define CHECK_READ_FRAMEBUFFER_COMPLETE(error_msg) \
    set_host_error(context);                       \
//GLenum fbo_complete;\
//fbo_complete = d_glCheckFramebufferStatus(context,GL_READ_FRAMEBUFFER);\
//if(fbo_complete!=GL_FRAMEBUFFER_COMPLETE){PRINT_LOG(__FUNCTION__,"CHECK_READ_FRAMEBUFFER_COMPLETE")set_gl_error(context,error_msg);return;}

// check current bound transform feedback, if it's active and not paused error
// occur
#define CHECK_TRANSFORM_FEEDBACK_ACTIVE_BUT_NOT_PAUSED(error_msg)   \
    GLboolean trans_active, trans_paused;                           \
    BindingStatus *bind_status =                                   \
        &(((GLThreadContext *)context)->context_binding_status);    \
    trans_active = bind_status->transform_feedback_active;          \
    trans_paused = bind_status->transform_feedback_paused;          \
    if (CC_UNLIKELY(!trans_paused && trans_active)) {               \
        PRINT_LOG(__FUNCTION__,                                     \
                  "CHECK_TRANSFORM_FEEDBACK_ACTIVE_BUT_NOT_PAUSED") \
        set_gl_error(context, error_msg);                           \
        return;                                                     \
    }

// only use in glv3_guest_texture for simplified the switch case
#define CHECK_TEXTURE_FORMAT(line, check_format, N, ...)                 \
    if (format == check_format) {                                        \
        CHECK_VALID_ENUM_WITH_ERROR(line, type, N, GL_INVALID_OPERATION, \
                                    __VA_ARGS__)                         \
    } else {                                                             \
        set_gl_error(context, GL_INVALID_OPERATION);                     \
        return;                                                          \
    }

#endif  // ERROR_HANDLER_H