#if !defined(FCODER_JACK_PUNTER)
#define FCODER_JACK_PUNTER

#include "../4coder_default_include.cpp"

#define DEBUGGING 1

#if DEBUGGING
#define DEBUG_MSG_LIT(str) print_message(app, string_u8_litexpr(str))
#define DEBUG_MSG_STR(str) print_message(app, str)
#else
#define DEBUG_MSG_LIT(str)
#define DEBUG_MSG_STR(str)
#endif

struct jp_buffer_data_t {
    
};

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

// TODO(jack) Make custom_keywords array dynamic on an arena
// So there is no limit to the number of custom keywords
// global Arena keyword_array_arena;
global Arena keyword_storage_arena;

global const u64 total_custom_keywords = 256;
global String_Const_u8 custom_keywords[total_custom_keywords] = {};
global u64 custom_keywords_end = 0;
function b32
jp_is_custom_keyword(String_Const_u8 keyword) {
    for (size_t i = 0; i < custom_keywords_end; ++i){
        if (string_match(keyword, custom_keywords[i])){
            return true;
        }
    }
    return false;
}
function b32
jp_push_custom_keyword(String_Const_u8 new_keyword) {
    if(!jp_is_custom_keyword(new_keyword)){
        if (custom_keywords_end < total_custom_keywords) {
            custom_keywords[custom_keywords_end++] = push_string_copy(&keyword_storage_arena, new_keyword);
            return true;
        }
        return false;
    }
    return true;
}

global const u64 total_custom_types = 512;
global String_Const_u8 custom_types[total_custom_types] = {};
global u64 custom_types_end = 0;
function b32
jp_is_custom_type(String_Const_u8 type) {
    for (size_t i = 0; i < total_custom_types; ++i){
        if (string_match(type, custom_types[i])){
            return true;
        }
    }
    return false;
}
function b32
jp_push_custom_type(String_Const_u8 new_type) {
    if (!jp_is_custom_type(new_type)) {
        if (custom_types_end < total_custom_types) {
            custom_types[custom_types_end++] = push_string_copy(&keyword_storage_arena, new_type);
            return true;
        }
        return false;
    }
    return true;
}

global bool GlobalIsRecordingMacro = false;

CUSTOM_ID(attachment, jp_buffer_attachment);

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

    keyword_storage_arena = make_arena_system();
    // keyword_array_arena = make_arena_system();

    jp_push_custom_type(string_u8_litexpr("uint8_t"));
    jp_push_custom_type(string_u8_litexpr("uint16_t"));
    jp_push_custom_type(string_u8_litexpr("uint32_t"));
    jp_push_custom_type(string_u8_litexpr("uint64_t"));
    jp_push_custom_type(string_u8_litexpr("int8_t"));
    jp_push_custom_type(string_u8_litexpr("int16_t"));
    jp_push_custom_type(string_u8_litexpr("int32_t"));
    jp_push_custom_type(string_u8_litexpr("int64_t"));
}

#endif // FCODER_JACK_PUNTER

// BOTTOM
