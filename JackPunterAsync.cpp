#if !defined(FCODER_JACK_PUNTER_ASYNC)

function void 
language_do_full_lex_async__inner(Async_Context *actx, Buffer_ID buffer_id)
{    
    Application_Links *app = actx->app;
    ProfileScope(app, "async lex");
    Scratch_Block scratch(app);
    
    String_Const_u8 contents = {};
    {
        ProfileBlock(app, "async lex contents (before mutex)");
        acquire_global_frame_mutex(app);
        ProfileBlock(app, "async lex contents (after mutex)");
        contents = push_whole_buffer(app, scratch, buffer_id);
        release_global_frame_mutex(app);
    }
    
    i32 limit_factor = 10000;
    
    Token_List list = {};
    b32 canceled = false;
    

    acquire_global_frame_mutex(app);
    ProfileBlock(app, "async lex save results (after mutex)");
    Managed_Scope scope = buffer_get_managed_scope(app, buffer_id);
    jp_buffer_data_t *buffer_data = scope_attachment(app, scope, jp_buffer_attachment, jp_buffer_data_t);

    Language *language = buffer_data->language;
    release_global_frame_mutex(app);

    Assert(language);

    Generic_Lexer_State state = {};
    language->lex_init(&state, contents);
    for (;;){
        ProfileBlock(app, "async lex block");
        if(language->lex_breaks(scratch, &list, &state, limit_factor)) {
            break;
        }
        if (async_check_canceled(actx)){
            canceled = true;
            break;
        }
    }
    
    if (!canceled){
        ProfileBlock(app, "async lex save results (before mutex)");
        acquire_global_frame_mutex(app);
        ProfileBlock(app, "async lex save results (after mutex)");
        if (scope != 0){
            Base_Allocator *allocator = managed_scope_allocator(app, scope);
            Token_Array *tokens_ptr = scope_attachment(app, scope, attachment_tokens, Token_Array);
            base_free(allocator, tokens_ptr->tokens);
            Token_Array tokens = {};
            tokens.tokens = base_array(allocator, Token, list.total_count);
            tokens.count = list.total_count;
            tokens.max = list.total_count;
            token_fill_memory_from_list(tokens.tokens, &list);
            block_copy_struct(tokens_ptr, &tokens);
        }
        buffer_mark_as_modified(buffer_id);
        release_global_frame_mutex(app);
    }
}

function void
language_do_full_lex_async(Async_Context *actx, Data data)
{
    if (data.size == sizeof(Buffer_ID)){
        Buffer_ID buffer = *(Buffer_ID*)data.data;
        language_do_full_lex_async__inner(actx, buffer);
    }
}

struct AsyncParserData {
    Buffer_ID buffer_id;
    Async_Task lexer_task;
};

void
jp_trigger_parse_buffer(Async_Context *actx, Data data)
{
    if(data.size == sizeof(AsyncParserData)) {
        Application_Links *app = actx->app;
        AsyncParserData* ParserData = (AsyncParserData*)data.data;
        async_task_wait(actx->app, &global_async_system, ParserData->lexer_task);
        
        acquire_global_frame_mutex(app);
        Managed_Scope buffer_scope = buffer_get_managed_scope(app, ParserData->buffer_id);
        jp_buffer_data_t* buffer_data = scope_attachment(app, buffer_scope, jp_buffer_attachment,
                                                         jp_buffer_data_t);
        buffer_data->parse_contents = true;
        release_global_frame_mutex(app);
    }
}


#define FCODER_JACK_PUNTER_ASYNC 
#endif