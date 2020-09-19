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

global const i32 custom_highlight_base_size = 32;

enum HighlightType : u64 {
    HighlightType_Function,
    HighlightType_Macro,
    HighlightType_Type,
    HighlightType_Keyword,
    HighlightType_None,
};

struct jp_app_data_t {
    // NOTE(jack): This will store the custom highlight information for all buffers
    Base_Allocator* table_allocator;
    Table_Data_u64 custom_highlight_table;
};

struct jp_buffer_data_t {
    // NOTE(jack): This will store the custom highlight tokens (keys for table lookup)
    // for this buffer
    // cleared on reparse (as well as erased from app table)
    Arena custom_highlights_arena;
    String_Const_u8_Array custom_highlight_array;

    b32 parse_contents = false;
};

CUSTOM_ID(attachment, jp_buffer_attachment);
CUSTOM_ID(attachment, jp_app_attachment);
CUSTOM_ID(attachment, buffer_parse_keywords_types_task);

global i32 type_token_kinds[] = {
    TokenCppKind_Void,
    TokenCppKind_Bool,
    TokenCppKind_Char,
    TokenCppKind_Int,
    TokenCppKind_Float,
    TokenCppKind_Double,
    TokenCppKind_Long,-
    TokenCppKind_Short,
    TokenCppKind_Unsigned,
    TokenCppKind_Signed,
    TokenCppKind_Const,
    TokenCppKind_Volatile
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
jp_insert_custom_highlight(Application_Links *app, String_Const_u8 string,
                           HighlightType type) 
{
    ProfileScope(app, "JP Insert Custom Highlight");
    Managed_Scope global_scope = get_global_managed_scope(app);
    jp_app_data_t* app_data = scope_attachment(app, global_scope, jp_app_attachment, jp_app_data_t);
    
    //String_Const_u8 perm_string = push_string_copy(&app_data->highlight_table_arena, string);
    Data key = {};
    key.data = string.str;
    key.size = string.size;
    return table_insert(&app_data->custom_highlight_table, key, type);
}

function HighlightType
jp_custom_highlight_lookup(Application_Links *app, String_Const_u8 string)
{
    ProfileScope(app, "JP Custom Highlight Lookup");
    HighlightType Result;
    Managed_Scope global_scope = get_global_managed_scope(app);
    jp_app_data_t* app_data = scope_attachment(app, global_scope, jp_app_attachment, jp_app_data_t);

    Data lookup_data = {};
    lookup_data.data = string.str;
    lookup_data.size = string.size;
    if(!table_read(&app_data->custom_highlight_table, lookup_data, (u64 *)&Result)) {
        Result = HighlightType_None;
    }
    return Result;
}

function b32
jp_erase_custom_highlight(Application_Links *app, String_Const_u8 string)
{
    ProfileScope(app, "JP Erase Custom Highlight");
    Managed_Scope global_scope = get_global_managed_scope(app);
    jp_app_data_t* app_data = scope_attachment(app, global_scope, jp_app_attachment, jp_app_data_t);
    Data key = {};
    key.data = string.str;
    key.size = string.size;
    return table_erase(&app_data->custom_highlight_table, key);
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
    set_custom_hook(app, HookID_Tick, jp_tick);
    set_custom_hook(app, HookID_BeginBuffer, jp_begin_buffer);
    set_custom_hook(app, HookID_EndBuffer, jp_end_buffer);
    set_custom_hook(app, HookID_RenderCaller, jp_render_caller);
    set_custom_hook(app, HookID_SaveFile, jp_file_save);
    
    mapping_init(tctx, &framework_mapping);
    
    // NOTE(jack): If None of the custom bindings are working, check that the the config/ file
    // has mapping=""; rather than mapping="choose";
    // 4.1.6 chaned the default setting to ""
    JackPunterBindings(&framework_mapping);

    Managed_Scope global_scope = get_global_managed_scope(app);
    jp_app_data_t* app_data = scope_attachment(app, global_scope, jp_app_attachment, jp_app_data_t);
    app_data->table_allocator = get_base_allocator_system();
    app_data->custom_highlight_table = make_table_Data_u64(app_data->table_allocator,
                                                           custom_highlight_base_size);

    jp_insert_custom_highlight(app, SCu8("uint8_t"), HighlightType_Type);
    jp_insert_custom_highlight(app, SCu8("uint16_t"), HighlightType_Type);
    jp_insert_custom_highlight(app, SCu8("uint32_t"), HighlightType_Type);
    jp_insert_custom_highlight(app, SCu8("uint64_t"), HighlightType_Type);
    jp_insert_custom_highlight(app, SCu8("int8_t"), HighlightType_Type);
    jp_insert_custom_highlight(app, SCu8("int16_t"), HighlightType_Type);
    jp_insert_custom_highlight(app, SCu8("int32_t"), HighlightType_Type);
    jp_insert_custom_highlight(app, SCu8("int64_t"), HighlightType_Type);
    jp_insert_custom_highlight(app, SCu8("size_t"), HighlightType_Type);
    jp_insert_custom_highlight(app, SCu8("auto"), HighlightType_Type);

    jp_insert_custom_highlight(app, SCu8("override"), HighlightType_Keyword);
    jp_insert_custom_highlight(app, SCu8("final"), HighlightType_Keyword);
}

#endif // FCODER_JACK_PUNTER

// BOTTOM
