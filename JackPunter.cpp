#if !defined(FCODER_JACK_PUNTER)
#define FCODER_JACK_PUNTER

#include "../4coder_default_include.cpp"

#define DEBUGGING 0

#if DEBUGGING
#define DEBUG_MSG_LIT(str) print_message(app, string_u8_litexpr(str))
#define DEBUG_MSG_STR(str) print_message(app, str)
#else
#define DEBUG_MSG_LIT(str)
#define DEBUG_MSG_STR(str)
#endif

global const u64 custom_keywords_per_buffer = 16;
global const u64 custom_types_per_buffer = 256;

struct jp_buffer_data_t {
    Arena custom_keyword_type_arena;
    
    String_Const_u8 custom_keywords[custom_keywords_per_buffer] = {};
    u64             custom_keywords_end = 0;
    
    String_Const_u8 custom_types[custom_types_per_buffer] = {};
    u64             custom_types_end = 0;
};
CUSTOM_ID(attachment, jp_buffer_attachment);

global i32 type_token_kinds[] = {
    TokenCppKind_Void,
    TokenCppKind_Bool,
    TokenCppKind_Char,
    TokenCppKind_Int,
    TokenCppKind_Float,
    TokenCppKind_Double,
    TokenCppKind_Long,
    TokenCppKind_Short,
    TokenCppKind_Unsigned,
    TokenCppKind_Signed,
};
function b32
jp_is_type_token(Token* token){
    for (int i = 0; i < ArrayCount(type_token_kinds); ++i) {
        if (token->sub_kind == type_token_kinds[i]) {
            return true;
        }
    }
    return false;
}

function b32
jp_is_custom_keyword(Application_Links *app, String_Const_u8 keyword) {
    // NOTE(jack): Check the global scope array for hardcoded custom keywords / types
    Managed_Scope global_scope = get_global_managed_scope(app);
    jp_buffer_data_t* global_scope_data = scope_attachment(app, global_scope, jp_buffer_attachment, jp_buffer_data_t);
    for (size_t i = 0; i < global_scope_data->custom_keywords_end; ++i){
        if (string_match(keyword, global_scope_data->custom_keywords[i])){
            return true;
        }
    }
    
    Buffer_ID buffer = get_buffer_next(app, 0, Access_Read);
    do {
        Managed_Scope buffer_scope = buffer_get_managed_scope(app, buffer);
        jp_buffer_data_t *buffer_data = scope_attachment(app, buffer_scope, jp_buffer_attachment, 
                                                         jp_buffer_data_t);
        if (buffer_data) {
            for (size_t i = 0; i < buffer_data->custom_keywords_end; ++i){
                if (string_match(keyword, buffer_data->custom_keywords[i])){
                    return true;
                }
            }
        }
        buffer = get_buffer_next(app, buffer, Access_Read);
    } while (buffer);
    return false;
}
function b32
jp_is_custom_type(Application_Links *app, String_Const_u8 type) {
    // NOTE(jack): Check the global scope array for hardcoded custom keywords / types
    Managed_Scope global_scope = get_global_managed_scope(app);
    jp_buffer_data_t* global_scope_data = scope_attachment(app, global_scope, jp_buffer_attachment, jp_buffer_data_t);
    for (size_t i = 0; i < global_scope_data->custom_types_end; ++i){
        if (string_match(type, global_scope_data->custom_types[i])){
            return true;
        }
    }
    
    Buffer_ID buffer = get_buffer_next(app, 0, Access_Read);
    do {
        Managed_Scope buffer_scope = buffer_get_managed_scope(app, buffer);
        jp_buffer_data_t *buffer_data = scope_attachment(app, buffer_scope, jp_buffer_attachment, 
                                                         jp_buffer_data_t);
        if (buffer_data) {
            for (size_t i = 0; i < buffer_data->custom_types_end; ++i){
                if (string_match(type, buffer_data->custom_types[i])){
                    return true;
                }
            }
        }
        // NOTE(jack): returns zero when its the last buffer
        buffer = get_buffer_next(app, buffer, Access_Read);
    } while (buffer);
    return false;
}

function void
jp_push_custom_keyword(Application_Links* app, jp_buffer_data_t* buffer_data, String_Const_u8 new_keyword) {
    b32 result = true;
    if(!jp_is_custom_keyword(app, new_keyword)) {
        if (buffer_data->custom_keywords_end < custom_keywords_per_buffer) {
            buffer_data->custom_keywords[buffer_data->custom_keywords_end++] =
                push_string_copy(&buffer_data->custom_keyword_type_arena, new_keyword);
            result = true;
        } else {
            result = false;
        }
    }
    if (result) {
        DEBUG_MSG_LIT("New custom keyword added: ");
        DEBUG_MSG_STR(new_keyword);
        DEBUG_MSG_LIT("\n");
    } else {
        log_string(app, string_u8_litexpr("Array Full. Couldn't add custom keyword: "));
        log_string(app, new_keyword);
        log_string(app, string_u8_litexpr("\n"));
    }
}

function void
jp_push_custom_keyword(Application_Links* app, Buffer_ID buffer, String_Const_u8 new_keyword) {
    Managed_Scope buffer_scope = buffer_get_managed_scope(app, buffer);
    jp_buffer_data_t* buffer_data = scope_attachment(app, buffer_scope, jp_buffer_attachment, 
                                                     jp_buffer_data_t);
    jp_push_custom_keyword(app, buffer_data, new_keyword);
}

function void
jp_push_custom_type(Application_Links* app, jp_buffer_data_t* buffer_data, String_Const_u8 new_type) {
    b32 result = true;
    if (!jp_is_custom_type(app, new_type)) {
        if (buffer_data->custom_types_end < custom_types_per_buffer) {
            buffer_data->custom_types[buffer_data->custom_types_end++] =
                push_string_copy(&buffer_data->custom_keyword_type_arena, new_type);
        } else {
            result = false;
        }
    }
    if (result) {
        DEBUG_MSG_LIT("New custom type added: ");
        DEBUG_MSG_STR(new_type);
        DEBUG_MSG_LIT("\n");
    } else {
        log_string(app, string_u8_litexpr("Array Full. Couldn't add custom type: "));
        log_string(app, new_type);
        log_string(app, string_u8_litexpr("\n"));
    }
}

function void
jp_push_custom_type(Application_Links* app, Buffer_ID buffer, String_Const_u8 new_type) {
    Managed_Scope buffer_scope = buffer_get_managed_scope(app, buffer);
    jp_buffer_data_t* buffer_data = scope_attachment(app, buffer_scope, jp_buffer_attachment, 
                                                     jp_buffer_data_t);
    jp_push_custom_type(app, buffer_data, new_type);
}

global bool GlobalIsRecordingMacro = false;

#include "JackPunterParsing.cpp"
// NOTE(jack): Custom Command Definitions
#include "JackPunterCommand.cpp"
#include "JackPunterDraw.cpp"
#include "JackPunterHook.cpp"

#include "../generated/managed_id_metadata.cpp"

#include "JackPunterBindings.cpp"

void
custom_layer_init(Application_Links *app)
{
    Thread_Context *tctx = get_thread_context(app);
    
    // NOTE(allen): setup for default framework
    default_framework_init(app);
    
    // NOTE(allen): default hooks and command maps
    set_all_default_hooks(app);
    set_custom_hook(app, HookID_BeginBuffer, jp_begin_buffer);
    set_custom_hook(app, HookID_RenderCaller, jp_render_caller);
    set_custom_hook(app, HookID_SaveFile, jp_file_save);
    
    mapping_init(tctx, &framework_mapping);
    
    // NOTE(jack): If None of the custom bindings are working, check that the the config/ file
    // has mapping=""; rather than mapping="choose";
    // 4.1.6 chaned the default setting to ""
    JackPunterBindings(&framework_mapping);
    
    Managed_Scope global_scope = get_global_managed_scope(app);
    jp_buffer_data_t* global_scope_data = scope_attachment(app, global_scope, jp_buffer_attachment, jp_buffer_data_t);
    global_scope_data->custom_keyword_type_arena = make_arena_system();
    
    jp_push_custom_type(app, global_scope_data, string_u8_litexpr("uint8_t"));
    jp_push_custom_type(app, global_scope_data, string_u8_litexpr("uint16_t"));
    jp_push_custom_type(app, global_scope_data, string_u8_litexpr("uint32_t"));
    jp_push_custom_type(app, global_scope_data, string_u8_litexpr("uint64_t"));
    jp_push_custom_type(app, global_scope_data, string_u8_litexpr("int8_t"));
    jp_push_custom_type(app, global_scope_data, string_u8_litexpr("int16_t"));
    jp_push_custom_type(app, global_scope_data, string_u8_litexpr("int32_t"));
    jp_push_custom_type(app, global_scope_data, string_u8_litexpr("int64_t"));
    jp_push_custom_type(app, global_scope_data, string_u8_litexpr("size_t"));

    jp_push_custom_keyword(app, global_scope_data, string_u8_litexpr("override"));
    jp_push_custom_keyword(app, global_scope_data, string_u8_litexpr("final"));
}

#endif // FCODER_JACK_PUNTER

// BOTTOM
