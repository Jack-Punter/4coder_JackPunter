#if !defined(FCODER_JACK_PUNTER_BINDINGS_CPP)
#define FCODER_DEFAULT_BINDINGS_CPP

#include "../4coder_default_include.cpp"

#if 0
#define DEBUG_MSG(str) print_message(app, string_u8_litexpr(str))
#else
#define DEBUG_MSG(str)
#endif

struct jp_buffer_data_t {
    
};

global const u64 total_custom_keywords = 512;
global String_Const_u8 custom_keywords[total_custom_keywords] = {};
global u64 custom_keywords_end = 0;

global Arena keyword_arena;

function bool
jp_push_custom_keyword(String_Const_u8 new_keyword) {
    if (custom_keywords_end < total_custom_keywords) {
        custom_keywords[custom_keywords_end++] = push_string_copy(&keyword_arena, new_keyword);
        return true;
    }
    return false;
}

function b32
jp_is_custom_keyword(String_Const_u8 keyword) {
    for (size_t i = 0; i < custom_keywords_end; ++i){
        if (string_match(keyword, custom_keywords[i])){
            return true;
        }
    }
    return false;
}

global bool GlobalIsRecordingMacro = false;

CUSTOM_ID(attachment, jp_buffer_attachment);

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

    mapping_init(tctx, &framework_mapping);
    
    // NOTE(jack): If None of the custom bindings are working, check that the the config/ file
    // has mapping=""; rather than mapping="choose";
    // 4.1.6 chaned the default setting to ""
    JackPunterBindings(&framework_mapping);

    keyword_arena = make_arena_system();

    jp_push_custom_keyword(string_u8_litexpr("uint8_t"));
    jp_push_custom_keyword(string_u8_litexpr("uint16_t"));
    jp_push_custom_keyword(string_u8_litexpr("uint32_t"));
    jp_push_custom_keyword(string_u8_litexpr("uint64_t"));

    jp_push_custom_keyword(string_u8_litexpr("int8_t"));
    jp_push_custom_keyword(string_u8_litexpr("int16_t"));
    jp_push_custom_keyword(string_u8_litexpr("int32_t"));
    jp_push_custom_keyword(string_u8_litexpr("int64_t"));

/*
    PushNewCustomKeyword(string_u8_litexpr("u8"));
    PushNewCustomKeyword(string_u8_litexpr("u16"));
    PushNewCustomKeyword(string_u8_litexpr("u32"));
    PushNewCustomKeyword(string_u8_litexpr("u64"));
    PushNewCustomKeyword(string_u8_litexpr("i8"));
    PushNewCustomKeyword(string_u8_litexpr("i16"));
    PushNewCustomKeyword(string_u8_litexpr("i32"));
    PushNewCustomKeyword(string_u8_litexpr("i64"));
    PushNewCustomKeyword(string_u8_litexpr("bool32"));
*/
}

#endif // FCODER_JACK_PUNTER_BINDINGS_CPP

// BOTTOM
