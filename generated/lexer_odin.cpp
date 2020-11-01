#if !defined(FCODER_LEX_GEN_HAND_WRITTEN)
#define FCODER_LEX_GEN_HAND_WRITTEN

internal u64
lexeme_hash(u64 seed, u8 *ptr, u64 size){
    u64 result = 0;
    for (u64 i = 0; i < size; i += 1, ptr += 1){
        result ^= ((*ptr) ^ result*59) + seed;
    }
    return(result);
}

internal Lexeme_Table_Lookup
lexeme_table_lookup(u64 *hash_array, String_Const_u8 *key_array, 
                    Lexeme_Table_Value *value_array, i32 slot_count, u64 seed,
                    u8 *ptr, u64 size){
    Lexeme_Table_Lookup result = {};
    u64 hash = lexeme_hash(seed, ptr, size);
    u64 comparison_hash = hash | 1;
    i32 first_index = (hash % slot_count);
    i32 index = first_index;
    for (;;){
        if (hash_array[index] == comparison_hash){
            if (string_match(SCu8(ptr, size), key_array[index])){
                result.found_match = true;
                result.base_kind = value_array[index].base_kind;
                result.sub_kind = value_array[index].sub_kind;
                break;
            }
        }
        else if (hash_array[index] == 0){
            break;
        }
        index += 1;
        if (index == slot_count){
            index = 0;
        }
        if (index == first_index){
            break;
        }
    }
    return(result);
}

#endif
u64 odin_main_keys_hash_array[332] = {
0x0000000000000000,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0xcad2f107145969f5,0xb135ffea018f5a1d,0xef9310125608a561,0xcad2f10714597ad1,
0x8c01c32a62e0df87,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0x2f9b507bc72b28e9,0x0000000000000000,0x4f08b030e4017d2b,0x0000000000000000,
0x0000000000000000,0x4f08b030e4778279,0x3a5939eaf487d9f9,0xf0d23f58a6a664e3,
0x0000000000000000,0x4f08b030e445b105,0xb135ffea3b0c78dd,0xbdf321039d843747,
0xef931153933df143,0x0000000000000000,0x0000000000000000,0x4f08b030e4458a23,
0x4f08b030e455f403,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0xcad2f1071458e9ad,0x4f08b030e4516b55,0x8c6a3f702a384105,0x0000000000000000,
0x0000000000000000,0xb135ffea3e29bb39,0x4f08b030e41dc085,0x384779cac586d7f7,
0x0000000000000000,0x0000000000000000,0xc9c0494e3cfb3523,0xc9bd313fe40b311b,
0x0000000000000000,0x0000000000000000,0x4f08b030fbabcf83,0x8df9decb6af0e4e3,
0xef936e45d5ef252d,0x4de1a3a09b9ee105,0x0000000000000000,0x9eb9400424f5c99b,
0xf22ad21ac6ef099b,0x8ed65b432f86d7f7,0xcad762d773561085,0x4f08b030e4767b59,
0x0000000000000000,0xb135ffea3b383639,0x0000000000000000,0x4f08b030e4767a13,
0xb135ffea3f925049,0x4f08b030e445bcd9,0x0000000000000000,0x0000000000000000,
0xc9bcae1a8425e4f1,0x4140f4a8962ed7d9,0x0000000000000000,0x0000000000000000,
0x0000000000000000,0xef936e7b99348985,0x0000000000000000,0x0000000000000000,
0xef936e366d6a89d9,0xcad2f107145914a1,0xcad2f1071459542d,0x4f08b030e4767a23,
0x7506e7a0d6d44f29,0x0000000000000000,0x0000000000000000,0xb135ffd0bb2f623b,
0x2f9b506e52ca876b,0x2f9b506d28f3ced9,0x0000000000000000,0x0000000000000000,
0x0000000000000000,0xcaec7c80f855d299,0x2f9b5661c6b29fb9,0x0000000000000000,
0x5a9325df5160c3c1,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0x0000000000000000,0x0000000000000000,0xcad2f10714593907,0x41a31b1e89933d4f,
0x0000000000000000,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0x4f08b030e4402d09,0x0000000000000000,0xe325cff99ce118fb,0x0000000000000000,
0x0000000000000000,0x2f9b507bdc73d079,0x4f08b030e47da409,0x0000000000000000,
0x0000000000000000,0xb135ffea3f0aca79,0x0000000000000000,0x4f08b030e46dfb6b,
0xf0cb920852297bbd,0xb135ffea3b394bf9,0x0000000000000000,0x0000000000000000,
0x0000000000000000,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0x11a596631dc50931,0xcad2f10714597b45,0x0000000000000000,0x0000000000000000,
0xcad2f107145914d5,0x0000000000000000,0x0000000000000000,0x4f08b030e4634b13,
0x891e1fd4f73fc99b,0xb135ffea3f13d579,0x0000000000000000,0xcad8e3ff1299a11b,
0x46016e7f99c7e4dd,0x0000000000000000,0xcad2f1071458a557,0x7506e7a0d6d44f63,
0x8c01c32a62e192ff,0x01544e80160bd9f9,0x4f08b030e46df273,0x0000000000000000,
0x11a474af0b905ead,0xb135ffea366a4f3d,0x5c5237b107a96637,0x0000000000000000,
0x0000000000000000,0x4f08b030e44d7c19,0x2f9b566345040f07,0x0000000000000000,
0x0000000000000000,0x4f08b030e455be05,0xef936e45d8088397,0x0000000000000000,
0x0000000000000000,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0xcad2f1071458a56d,0xb135ffea3b39445d,0x0000000000000000,0x0000000000000000,
0x0000000000000000,0x4f08b030e448be49,0x0000000000000000,0x56d78c332f86d7f7,
0xef92cb147ed387e1,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0x0000000000000000,0xef92cb7ecddb64b5,0x4f08b030e45076c3,0x0000000000000000,
0x4f08b030e455e26d,0xb135ffea3ed971dd,0x2f9b5661c6a66d33,0x0000000000000000,
0x0000000000000000,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0x0000000000000000,0x4f08b030e455bcd9,0x0000000000000000,0x0000000000000000,
0xc986ca5e5e982fd5,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0x7506e7a0d6d44d01,0x0000000000000000,0x11a444026a4c78bb,0x0000000000000000,
0xcaecf199cc38f8f1,0x0000000000000000,0x2f9b506f5152d5b3,0x11b2a5647c311987,
0x7506e7a0d6d44d09,0xcaecb03d8c530e39,0x0000000000000000,0x0000000000000000,
0xcad2f1071458a599,0x46016e7f99c7d591,0x0000000000000000,0x0000000000000000,
0x4f08b030e44289c9,0x2f9b507bdc73d0dd,0xef9310a1089220fb,0x0000000000000000,
0x0000000000000000,0xb135ffea3f0acadd,0x0000000000000000,0x50b5e9229bf65f9b,
0xb135ffea39be4fd9,0x0000000000000000,0x2f9b506f511cd27f,0xb135ffea035d8a07,
0x0000000000000000,0xef936e52a0276cd9,0x0000000000000000,0x07d33b3df4a52d1b,
0x0000000000000000,0x0000000000000000,0xcad2f1071458a5af,0x0000000000000000,
0x0000000000000000,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0x0000000000000000,0x2f9b5078f6082a79,0x0000000000000000,0x0000000000000000,
0xb135ffea035e7f35,0x0000000000000000,0x0000000000000000,0x2f9b506f5176419b,
0x0000000000000000,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0x6e274472af6a0d19,0x0000000000000000,0x7506e7a0d6d44d37,0x0000000000000000,
0x0000000000000000,0x933860e05515a905,0xef93102648e74737,0x6531ad965512419b,
0xcad2f1071458e939,0xcad2f1071458a5c9,0x0000000000000000,0x0000000000000000,
0x0000000000000000,0xb135ffea3b0c7879,0x2f9b506e9c4308a5,0x7506e7a0d6d44bf7,
0x0000000000000000,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0x4f08b030e44434ad,0xcaeceea9c5f6e635,0x4f08b030e45785a9,0x1c4cf4d8c066e105,
0x0000000000000000,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0x0000000000000000,0x2f9b507b831f1299,0x2f9b5078c062facb,0xef936d11d9d41f9b,
0xf178edbeba0f746f,0x598bd14f8b680059,0x0000000000000000,0x0000000000000000,
0x0000000000000000,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0x0000000000000000,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0x0000000000000000,0x0000000000000000,0x4f08b030e477f30f,0x4d4c24236cee54e3,
0x0000000000000000,0x0000000000000000,0xcad2f10714589f77,0x11a470eb59f61543,
0x4f08b030e46deddb,0xcad2f107145916c7,0xb135ffea3ed97639,0x0000000000000000,
0x0000000000000000,0xcad2f1071458f661,0x0000000000000000,0x0000000000000000,
0xcad2f10714589289,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0xc986ca5e5e98112d,0xb135ffea3f13d4dd,0x0000000000000000,0x0000000000000000,
0x0000000000000000,0x0000000000000000,0x0000000000000000,0xb2e6faf46986d7f7,
0xcad2f107145911ad,0xcad2f107145916dd,0x0000000000000000,0xb135ffea0636626b,
0x9f6392358b0c211b,0x0000000000000000,0x41a31b1e8992af87,0x5d1ccd85c586d7f7,
0x2f9b5078d9c5eb81,0xcad2f1071458ab41,0xcad2f1071458f14b,0x0000000000000000,
0x0000000000000000,0xb135ffea3b3839dd,0x0000000000000000,0x0000000000000000,
0xcad2f1071458d4c9,0xcad2f1071458a619,0xfde37b51c22e4df5,0x2f9b5078f6082add,
};
u8 odin_main_keys_key_array_4[] = {0x66,0x6f,0x72,};
u8 odin_main_keys_key_array_5[] = {0x64,0x65,0x66,0x65,0x72,};
u8 odin_main_keys_key_array_6[] = {0x74,0x79,0x70,0x65,0x5f,0x6f,0x66,};
u8 odin_main_keys_key_array_7[] = {0x66,0x31,0x36,};
u8 odin_main_keys_key_array_8[] = {0x64,0x65,0x6c,0x65,0x74,0x65,0x5f,0x6d,0x61,0x70,};
u8 odin_main_keys_key_array_12[] = {0x69,0x6d,0x70,0x6f,0x72,0x74,};
u8 odin_main_keys_key_array_14[] = {0x62,0x6f,0x6f,0x6c,};
u8 odin_main_keys_key_array_17[] = {0x63,0x6f,0x70,0x79,};
u8 odin_main_keys_key_array_18[] = {0x6f,0x72,0x64,0x65,0x72,0x65,0x64,0x5f,0x72,0x65,0x6d,0x6f,0x76,0x65,};
u8 odin_main_keys_key_array_19[] = {0x63,0x6f,0x70,0x79,0x5f,0x73,0x6c,0x69,0x63,0x65,};
u8 odin_main_keys_key_array_21[] = {0x75,0x38,0x62,0x65,};
u8 odin_main_keys_key_array_22[] = {0x69,0x33,0x32,0x62,0x65,};
u8 odin_main_keys_key_array_23[] = {0x66,0x61,0x6c,0x6c,0x74,0x68,0x72,0x6f,0x75,0x67,0x68,};
u8 odin_main_keys_key_array_24[] = {0x70,0x61,0x63,0x6b,0x61,0x67,0x65,};
u8 odin_main_keys_key_array_27[] = {0x74,0x72,0x75,0x65,};
u8 odin_main_keys_key_array_28[] = {0x6d,0x61,0x6b,0x65,};
u8 odin_main_keys_key_array_32[] = {0x75,0x33,0x32,};
u8 odin_main_keys_key_array_33[] = {0x70,0x72,0x6f,0x63,};
u8 odin_main_keys_key_array_34[] = {0x65,0x78,0x63,0x6c,0x5f,0x65,0x6c,0x65,0x6d,0x73,};
u8 odin_main_keys_key_array_37[] = {0x77,0x68,0x65,0x72,0x65,};
u8 odin_main_keys_key_array_38[] = {0x62,0x79,0x74,0x65,};
u8 odin_main_keys_key_array_39[] = {0x64,0x65,0x6c,0x65,0x74,0x65,0x5f,0x64,0x79,0x6e,0x61,0x6d,0x69,0x63,0x5f,0x61,0x72,0x72,0x61,0x79,};
u8 odin_main_keys_key_array_42[] = {0x61,0x75,0x74,0x6f,0x5f,0x63,0x61,0x73,0x74,};
u8 odin_main_keys_key_array_43[] = {0x65,0x78,0x63,0x6c,0x5f,0x65,0x6c,0x65,0x6d,};
u8 odin_main_keys_key_array_46[] = {0x72,0x65,0x61,0x6c,};
u8 odin_main_keys_key_array_47[] = {0x64,0x65,0x6c,0x65,0x74,0x65,0x5f,0x73,0x6c,0x69,0x63,0x65,};
u8 odin_main_keys_key_array_48[] = {0x63,0x6f,0x6e,0x74,0x65,0x78,0x74,};
u8 odin_main_keys_key_array_49[] = {0x69,0x6e,0x63,0x6c,0x5f,0x65,0x6c,0x65,0x6d,0x73,};
u8 odin_main_keys_key_array_51[] = {0x63,0x6f,0x70,0x79,0x5f,0x66,0x72,0x6f,0x6d,0x5f,0x73,0x74,0x72,0x69,0x6e,0x67,};
u8 odin_main_keys_key_array_52[] = {0x64,0x65,0x6c,0x65,0x74,0x65,0x5f,0x73,0x74,0x72,0x69,0x6e,0x67,};
u8 odin_main_keys_key_array_53[] = {0x72,0x65,0x73,0x65,0x72,0x76,0x65,0x5f,0x64,0x79,0x6e,0x61,0x6d,0x69,0x63,0x5f,0x61,0x72,0x72,0x61,0x79,};
u8 odin_main_keys_key_array_54[] = {0x62,0x69,0x74,0x5f,0x66,0x69,0x65,0x6c,0x64,};
u8 odin_main_keys_key_array_55[] = {0x63,0x61,0x72,0x64,};
u8 odin_main_keys_key_array_57[] = {0x69,0x36,0x34,0x6c,0x65,};
u8 odin_main_keys_key_array_59[] = {0x63,0x61,0x73,0x65,};
u8 odin_main_keys_key_array_60[] = {0x6e,0x6f,0x74,0x69,0x6e,};
u8 odin_main_keys_key_array_61[] = {0x75,0x38,0x6c,0x65,};
u8 odin_main_keys_key_array_64[] = {0x6f,0x66,0x66,0x73,0x65,0x74,0x5f,0x6f,0x66,};
u8 odin_main_keys_key_array_65[] = {0x71,0x75,0x61,0x74,0x65,0x72,0x6e,0x69,0x6f,0x6e,};
u8 odin_main_keys_key_array_69[] = {0x64,0x79,0x6e,0x61,0x6d,0x69,0x63,};
u8 odin_main_keys_key_array_72[] = {0x66,0x6f,0x72,0x65,0x69,0x67,0x6e,};
u8 odin_main_keys_key_array_73[] = {0x62,0x31,0x36,};
u8 odin_main_keys_key_array_74[] = {0x66,0x36,0x34,};
u8 odin_main_keys_key_array_75[] = {0x63,0x61,0x73,0x74,};
u8 odin_main_keys_key_array_76[] = {0x62,0x38,};
u8 odin_main_keys_key_array_79[] = {0x75,0x73,0x69,0x6e,0x67,};
u8 odin_main_keys_key_array_80[] = {0x6f,0x70,0x61,0x71,0x75,0x65,};
u8 odin_main_keys_key_array_81[] = {0x61,0x70,0x70,0x65,0x6e,0x64,};
u8 odin_main_keys_key_array_85[] = {0x6e,0x6f,0x5f,0x69,0x6e,0x6c,0x69,0x6e,0x65,};
u8 odin_main_keys_key_array_86[] = {0x72,0x65,0x73,0x69,0x7a,0x65,};
u8 odin_main_keys_key_array_88[] = {0x74,0x79,0x70,0x65,0x5f,0x69,0x6e,0x66,0x6f,0x5f,0x6f,0x66,};
u8 odin_main_keys_key_array_94[] = {0x63,0x61,0x70,};
u8 odin_main_keys_key_array_95[] = {0x72,0x65,0x73,0x65,0x72,0x76,0x65,0x5f,0x73,0x6f,0x61,};
u8 odin_main_keys_key_array_100[] = {0x77,0x68,0x65,0x6e,};
u8 odin_main_keys_key_array_102[] = {0x65,0x78,0x63,0x6c,0x5f,0x62,0x69,0x74,0x5f,0x73,0x65,0x74,};
u8 odin_main_keys_key_array_105[] = {0x69,0x31,0x32,0x38,0x6c,0x65,};
u8 odin_main_keys_key_array_106[] = {0x6a,0x6d,0x61,0x67,};
u8 odin_main_keys_key_array_109[] = {0x75,0x33,0x32,0x6c,0x65,};
u8 odin_main_keys_key_array_111[] = {0x65,0x6c,0x73,0x65,};
u8 odin_main_keys_key_array_112[] = {0x63,0x6f,0x6d,0x70,0x6c,0x65,0x78,0x31,0x32,0x38,};
u8 odin_main_keys_key_array_113[] = {0x69,0x31,0x36,0x6c,0x65,};
u8 odin_main_keys_key_array_120[] = {0x61,0x6c,0x69,0x67,0x6e,0x5f,0x6f,0x66,};
u8 odin_main_keys_key_array_121[] = {0x66,0x33,0x32,};
u8 odin_main_keys_key_array_124[] = {0x62,0x33,0x32,};
u8 odin_main_keys_key_array_127[] = {0x66,0x72,0x65,0x65,};
u8 odin_main_keys_key_array_128[] = {0x61,0x70,0x70,0x65,0x6e,0x64,0x5f,0x65,0x6c,0x65,0x6d,0x5f,0x73,0x74,0x72,0x69,0x6e,0x67,};
u8 odin_main_keys_key_array_129[] = {0x75,0x31,0x36,0x6c,0x65,};
u8 odin_main_keys_key_array_131[] = {0x69,0x6e,0x63,0x6c,0x5f,0x65,0x6c,0x65,0x6d,};
u8 odin_main_keys_key_array_132[] = {0x71,0x75,0x61,0x74,0x65,0x72,0x6e,0x69,0x6f,0x6e,0x31,0x32,0x38,};
u8 odin_main_keys_key_array_134[] = {0x6d,0x61,0x70,};
u8 odin_main_keys_key_array_135[] = {0x64,0x6f,};
u8 odin_main_keys_key_array_136[] = {0x64,0x65,0x6c,0x65,0x74,0x65,0x5f,0x6b,0x65,0x79,};
u8 odin_main_keys_key_array_137[] = {0x75,0x6e,0x6f,0x72,0x65,0x64,0x65,0x72,0x65,0x64,0x5f,0x72,0x65,0x6d,0x6f,0x76,0x65,};
u8 odin_main_keys_key_array_138[] = {0x65,0x78,0x63,0x6c,};
u8 odin_main_keys_key_array_140[] = {0x66,0x72,0x65,0x65,0x5f,0x61,0x6c,0x6c,};
u8 odin_main_keys_key_array_141[] = {0x62,0x72,0x65,0x61,0x6b,};
u8 odin_main_keys_key_array_142[] = {0x6d,0x61,0x6b,0x65,0x5f,0x64,0x79,0x6e,0x61,0x6d,0x69,0x63,0x5f,0x61,0x72,0x72,0x61,0x79,0x5f,0x6c,0x65,0x6e,0x5f,0x63,0x61,0x70,};
u8 odin_main_keys_key_array_145[] = {0x72,0x75,0x6e,0x65,};
u8 odin_main_keys_key_array_146[] = {0x72,0x61,0x77,0x70,0x74,0x72,};
u8 odin_main_keys_key_array_149[] = {0x69,0x38,0x62,0x65,};
u8 odin_main_keys_key_array_150[] = {0x63,0x6f,0x6d,0x70,0x6c,0x65,0x78,};
u8 odin_main_keys_key_array_156[] = {0x69,0x33,0x32,};
u8 odin_main_keys_key_array_157[] = {0x69,0x31,0x36,0x62,0x65,};
u8 odin_main_keys_key_array_161[] = {0x6b,0x6d,0x61,0x67,};
u8 odin_main_keys_key_array_163[] = {0x72,0x65,0x73,0x69,0x7a,0x65,0x5f,0x64,0x79,0x6e,0x61,0x6d,0x69,0x63,0x5f,0x61,0x72,0x72,0x61,0x79,};
u8 odin_main_keys_key_array_164[] = {0x73,0x69,0x7a,0x65,0x5f,0x6f,0x66,};
u8 odin_main_keys_key_array_169[] = {0x72,0x65,0x73,0x65,0x72,0x76,0x65,};
u8 odin_main_keys_key_array_170[] = {0x69,0x6e,0x63,0x6c,};
u8 odin_main_keys_key_array_172[] = {0x69,0x31,0x32,0x38,};
u8 odin_main_keys_key_array_173[] = {0x75,0x36,0x34,0x62,0x65,};
u8 odin_main_keys_key_array_174[] = {0x72,0x65,0x74,0x75,0x72,0x6e,};
u8 odin_main_keys_key_array_181[] = {0x69,0x38,0x6c,0x65,};
u8 odin_main_keys_key_array_184[] = {0x63,0x6f,0x6d,0x70,0x6c,0x65,0x78,0x33,0x32,};
u8 odin_main_keys_key_array_188[] = {0x69,0x66,};
u8 odin_main_keys_key_array_190[] = {0x64,0x69,0x73,0x74,0x69,0x6e,0x63,0x74,};
u8 odin_main_keys_key_array_192[] = {0x74,0x79,0x70,0x65,0x69,0x64,0x5f,0x6f,0x66,};
u8 odin_main_keys_key_array_194[] = {0x73,0x74,0x72,0x75,0x63,0x74,};
u8 odin_main_keys_key_array_195[] = {0x6d,0x61,0x6b,0x65,0x5f,0x6d,0x61,0x70,};
u8 odin_main_keys_key_array_196[] = {0x69,0x6e,};
u8 odin_main_keys_key_array_197[] = {0x6e,0x65,0x77,0x5f,0x63,0x6c,0x6f,0x6e,0x65,};
u8 odin_main_keys_key_array_200[] = {0x69,0x31,0x36,};
u8 odin_main_keys_key_array_201[] = {0x71,0x75,0x61,0x74,0x65,0x72,0x6e,0x69,0x6f,0x6e,0x32,0x35,0x36,};
u8 odin_main_keys_key_array_204[] = {0x75,0x69,0x6e,0x74,};
u8 odin_main_keys_key_array_205[] = {0x69,0x31,0x32,0x38,0x62,0x65,};
u8 odin_main_keys_key_array_206[] = {0x62,0x69,0x74,0x5f,0x73,0x65,0x74,};
u8 odin_main_keys_key_array_209[] = {0x75,0x33,0x32,0x62,0x65,};
u8 odin_main_keys_key_array_211[] = {0x61,0x70,0x70,0x65,0x6e,0x64,0x5f,0x73,0x74,0x72,0x69,0x6e,0x67,};
u8 odin_main_keys_key_array_212[] = {0x75,0x6e,0x69,0x6f,0x6e,};
u8 odin_main_keys_key_array_214[] = {0x73,0x77,0x69,0x74,0x63,0x68,};
u8 odin_main_keys_key_array_215[] = {0x63,0x6c,0x61,0x6d,0x70,};
u8 odin_main_keys_key_array_217[] = {0x73,0x77,0x69,0x7a,0x7a,0x6c,0x65,};
u8 odin_main_keys_key_array_219[] = {0x61,0x70,0x70,0x65,0x6e,0x64,0x5f,0x73,0x6f,0x61,0x5f,0x65,0x6c,0x65,0x6d,};
u8 odin_main_keys_key_array_222[] = {0x6d,0x61,0x78,};
u8 odin_main_keys_key_array_229[] = {0x75,0x31,0x32,0x38,0x6c,0x65,};
u8 odin_main_keys_key_array_232[] = {0x63,0x6c,0x65,0x61,0x72,};
u8 odin_main_keys_key_array_235[] = {0x73,0x74,0x72,0x69,0x6e,0x67,};
u8 odin_main_keys_key_array_240[] = {0x6d,0x61,0x6b,0x65,0x5f,0x64,0x79,0x6e,0x61,0x6d,0x69,0x63,0x5f,0x61,0x72,0x72,0x61,0x79,0x5f,0x6c,0x65,0x6e,};
u8 odin_main_keys_key_array_242[] = {0x69,0x38,};
u8 odin_main_keys_key_array_245[] = {0x61,0x70,0x70,0x65,0x6e,0x64,0x5f,0x73,0x6f,0x61,0x5f,0x65,0x6c,0x65,0x6d,0x73,};
u8 odin_main_keys_key_array_246[] = {0x75,0x69,0x6e,0x74,0x70,0x74,0x72,};
u8 odin_main_keys_key_array_247[] = {0x64,0x65,0x6c,0x65,0x74,0x65,0x5f,0x63,0x73,0x74,0x72,0x69,0x6e,0x67,};
u8 odin_main_keys_key_array_248[] = {0x75,0x31,0x36,};
u8 odin_main_keys_key_array_249[] = {0x69,0x36,0x34,};
u8 odin_main_keys_key_array_253[] = {0x69,0x33,0x32,0x6c,0x65,};
u8 odin_main_keys_key_array_254[] = {0x64,0x65,0x6c,0x65,0x74,0x65,};
u8 odin_main_keys_key_array_255[] = {0x75,0x38,};
u8 odin_main_keys_key_array_260[] = {0x75,0x31,0x32,0x38,};
u8 odin_main_keys_key_array_261[] = {0x74,0x72,0x61,0x6e,0x73,0x6d,0x75,0x74,0x65,};
u8 odin_main_keys_key_array_262[] = {0x69,0x6d,0x61,0x67,};
u8 odin_main_keys_key_array_263[] = {0x61,0x70,0x70,0x65,0x6e,0x64,0x5f,0x65,0x6c,0x65,0x6d,0x73,};
u8 odin_main_keys_key_array_269[] = {0x69,0x6e,0x6c,0x69,0x6e,0x65,};
u8 odin_main_keys_key_array_270[] = {0x74,0x79,0x70,0x65,0x69,0x64,};
u8 odin_main_keys_key_array_271[] = {0x63,0x73,0x74,0x72,0x69,0x6e,0x67,};
u8 odin_main_keys_key_array_272[] = {0x61,0x70,0x70,0x65,0x6e,0x64,0x5f,0x73,0x6f,0x61,};
u8 odin_main_keys_key_array_273[] = {0x65,0x78,0x70,0x61,0x6e,0x64,0x5f,0x74,0x6f,0x5f,0x74,0x75,0x70,0x6c,0x65,};
u8 odin_main_keys_key_array_286[] = {0x63,0x6f,0x6e,0x6a,};
u8 odin_main_keys_key_array_287[] = {0x6d,0x61,0x6b,0x65,0x5f,0x73,0x6c,0x69,0x63,0x65,};
u8 odin_main_keys_key_array_290[] = {0x70,0x6f,0x70,};
u8 odin_main_keys_key_array_291[] = {0x63,0x6f,0x6e,0x74,0x69,0x6e,0x75,0x65,};
u8 odin_main_keys_key_array_292[] = {0x65,0x6e,0x75,0x6d,};
u8 odin_main_keys_key_array_293[] = {0x61,0x62,0x73,};
u8 odin_main_keys_key_array_294[] = {0x75,0x36,0x34,0x6c,0x65,};
u8 odin_main_keys_key_array_297[] = {0x6e,0x65,0x77,};
u8 odin_main_keys_key_array_300[] = {0x69,0x6e,0x74,};
u8 odin_main_keys_key_array_304[] = {0x63,0x6f,0x6d,0x70,0x6c,0x65,0x78,0x36,0x34,};
u8 odin_main_keys_key_array_305[] = {0x75,0x31,0x36,0x62,0x65,};
u8 odin_main_keys_key_array_311[] = {0x6d,0x61,0x6b,0x65,0x5f,0x64,0x79,0x6e,0x61,0x6d,0x69,0x63,0x5f,0x61,0x72,0x72,0x61,0x79,};
u8 odin_main_keys_key_array_312[] = {0x62,0x36,0x34,};
u8 odin_main_keys_key_array_313[] = {0x61,0x6e,0x79,};
u8 odin_main_keys_key_array_315[] = {0x66,0x61,0x6c,0x73,0x65,};
u8 odin_main_keys_key_array_316[] = {0x61,0x70,0x70,0x65,0x6e,0x64,0x5f,0x65,0x6c,0x65,0x6d,};
u8 odin_main_keys_key_array_318[] = {0x72,0x65,0x73,0x65,0x72,0x76,0x65,0x5f,0x6d,0x61,0x70,};
u8 odin_main_keys_key_array_319[] = {0x63,0x6c,0x65,0x61,0x72,0x5f,0x64,0x79,0x6e,0x61,0x6d,0x69,0x63,0x5f,0x61,0x72,0x72,0x61,0x79,};
u8 odin_main_keys_key_array_320[] = {0x6e,0x6f,0x74,0x5f,0x69,0x6e,};
u8 odin_main_keys_key_array_321[] = {0x6d,0x69,0x6e,};
u8 odin_main_keys_key_array_322[] = {0x6e,0x69,0x6c,};
u8 odin_main_keys_key_array_325[] = {0x69,0x36,0x34,0x62,0x65,};
u8 odin_main_keys_key_array_328[] = {0x75,0x36,0x34,};
u8 odin_main_keys_key_array_329[] = {0x6c,0x65,0x6e,};
u8 odin_main_keys_key_array_330[] = {0x69,0x6e,0x69,0x74,0x5f,0x67,0x6c,0x6f,0x62,0x61,0x6c,0x5f,0x74,0x65,0x6d,0x70,0x6f,0x72,0x61,0x72,0x79,0x5f,0x61,0x6c,0x6c,0x6f,0x63,0x61,0x74,0x6f,0x72,};
u8 odin_main_keys_key_array_331[] = {0x75,0x31,0x32,0x38,0x62,0x65,};
String_Const_u8 odin_main_keys_key_array[332] = {
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_4, 3},
{odin_main_keys_key_array_5, 5},
{odin_main_keys_key_array_6, 7},
{odin_main_keys_key_array_7, 3},
{odin_main_keys_key_array_8, 10},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_12, 6},
{0, 0},
{odin_main_keys_key_array_14, 4},
{0, 0},
{0, 0},
{odin_main_keys_key_array_17, 4},
{odin_main_keys_key_array_18, 14},
{odin_main_keys_key_array_19, 10},
{0, 0},
{odin_main_keys_key_array_21, 4},
{odin_main_keys_key_array_22, 5},
{odin_main_keys_key_array_23, 11},
{odin_main_keys_key_array_24, 7},
{0, 0},
{0, 0},
{odin_main_keys_key_array_27, 4},
{odin_main_keys_key_array_28, 4},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_32, 3},
{odin_main_keys_key_array_33, 4},
{odin_main_keys_key_array_34, 10},
{0, 0},
{0, 0},
{odin_main_keys_key_array_37, 5},
{odin_main_keys_key_array_38, 4},
{odin_main_keys_key_array_39, 20},
{0, 0},
{0, 0},
{odin_main_keys_key_array_42, 9},
{odin_main_keys_key_array_43, 9},
{0, 0},
{0, 0},
{odin_main_keys_key_array_46, 4},
{odin_main_keys_key_array_47, 12},
{odin_main_keys_key_array_48, 7},
{odin_main_keys_key_array_49, 10},
{0, 0},
{odin_main_keys_key_array_51, 16},
{odin_main_keys_key_array_52, 13},
{odin_main_keys_key_array_53, 21},
{odin_main_keys_key_array_54, 9},
{odin_main_keys_key_array_55, 4},
{0, 0},
{odin_main_keys_key_array_57, 5},
{0, 0},
{odin_main_keys_key_array_59, 4},
{odin_main_keys_key_array_60, 5},
{odin_main_keys_key_array_61, 4},
{0, 0},
{0, 0},
{odin_main_keys_key_array_64, 9},
{odin_main_keys_key_array_65, 10},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_69, 7},
{0, 0},
{0, 0},
{odin_main_keys_key_array_72, 7},
{odin_main_keys_key_array_73, 3},
{odin_main_keys_key_array_74, 3},
{odin_main_keys_key_array_75, 4},
{odin_main_keys_key_array_76, 2},
{0, 0},
{0, 0},
{odin_main_keys_key_array_79, 5},
{odin_main_keys_key_array_80, 6},
{odin_main_keys_key_array_81, 6},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_85, 9},
{odin_main_keys_key_array_86, 6},
{0, 0},
{odin_main_keys_key_array_88, 12},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_94, 3},
{odin_main_keys_key_array_95, 11},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_100, 4},
{0, 0},
{odin_main_keys_key_array_102, 12},
{0, 0},
{0, 0},
{odin_main_keys_key_array_105, 6},
{odin_main_keys_key_array_106, 4},
{0, 0},
{0, 0},
{odin_main_keys_key_array_109, 5},
{0, 0},
{odin_main_keys_key_array_111, 4},
{odin_main_keys_key_array_112, 10},
{odin_main_keys_key_array_113, 5},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_120, 8},
{odin_main_keys_key_array_121, 3},
{0, 0},
{0, 0},
{odin_main_keys_key_array_124, 3},
{0, 0},
{0, 0},
{odin_main_keys_key_array_127, 4},
{odin_main_keys_key_array_128, 18},
{odin_main_keys_key_array_129, 5},
{0, 0},
{odin_main_keys_key_array_131, 9},
{odin_main_keys_key_array_132, 13},
{0, 0},
{odin_main_keys_key_array_134, 3},
{odin_main_keys_key_array_135, 2},
{odin_main_keys_key_array_136, 10},
{odin_main_keys_key_array_137, 17},
{odin_main_keys_key_array_138, 4},
{0, 0},
{odin_main_keys_key_array_140, 8},
{odin_main_keys_key_array_141, 5},
{odin_main_keys_key_array_142, 26},
{0, 0},
{0, 0},
{odin_main_keys_key_array_145, 4},
{odin_main_keys_key_array_146, 6},
{0, 0},
{0, 0},
{odin_main_keys_key_array_149, 4},
{odin_main_keys_key_array_150, 7},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_156, 3},
{odin_main_keys_key_array_157, 5},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_161, 4},
{0, 0},
{odin_main_keys_key_array_163, 20},
{odin_main_keys_key_array_164, 7},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_169, 7},
{odin_main_keys_key_array_170, 4},
{0, 0},
{odin_main_keys_key_array_172, 4},
{odin_main_keys_key_array_173, 5},
{odin_main_keys_key_array_174, 6},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_181, 4},
{0, 0},
{0, 0},
{odin_main_keys_key_array_184, 9},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_188, 2},
{0, 0},
{odin_main_keys_key_array_190, 8},
{0, 0},
{odin_main_keys_key_array_192, 9},
{0, 0},
{odin_main_keys_key_array_194, 6},
{odin_main_keys_key_array_195, 8},
{odin_main_keys_key_array_196, 2},
{odin_main_keys_key_array_197, 9},
{0, 0},
{0, 0},
{odin_main_keys_key_array_200, 3},
{odin_main_keys_key_array_201, 13},
{0, 0},
{0, 0},
{odin_main_keys_key_array_204, 4},
{odin_main_keys_key_array_205, 6},
{odin_main_keys_key_array_206, 7},
{0, 0},
{0, 0},
{odin_main_keys_key_array_209, 5},
{0, 0},
{odin_main_keys_key_array_211, 13},
{odin_main_keys_key_array_212, 5},
{0, 0},
{odin_main_keys_key_array_214, 6},
{odin_main_keys_key_array_215, 5},
{0, 0},
{odin_main_keys_key_array_217, 7},
{0, 0},
{odin_main_keys_key_array_219, 15},
{0, 0},
{0, 0},
{odin_main_keys_key_array_222, 3},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_229, 6},
{0, 0},
{0, 0},
{odin_main_keys_key_array_232, 5},
{0, 0},
{0, 0},
{odin_main_keys_key_array_235, 6},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_240, 22},
{0, 0},
{odin_main_keys_key_array_242, 2},
{0, 0},
{0, 0},
{odin_main_keys_key_array_245, 16},
{odin_main_keys_key_array_246, 7},
{odin_main_keys_key_array_247, 14},
{odin_main_keys_key_array_248, 3},
{odin_main_keys_key_array_249, 3},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_253, 5},
{odin_main_keys_key_array_254, 6},
{odin_main_keys_key_array_255, 2},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_260, 4},
{odin_main_keys_key_array_261, 9},
{odin_main_keys_key_array_262, 4},
{odin_main_keys_key_array_263, 12},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_269, 6},
{odin_main_keys_key_array_270, 6},
{odin_main_keys_key_array_271, 7},
{odin_main_keys_key_array_272, 10},
{odin_main_keys_key_array_273, 15},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_286, 4},
{odin_main_keys_key_array_287, 10},
{0, 0},
{0, 0},
{odin_main_keys_key_array_290, 3},
{odin_main_keys_key_array_291, 8},
{odin_main_keys_key_array_292, 4},
{odin_main_keys_key_array_293, 3},
{odin_main_keys_key_array_294, 5},
{0, 0},
{0, 0},
{odin_main_keys_key_array_297, 3},
{0, 0},
{0, 0},
{odin_main_keys_key_array_300, 3},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_304, 9},
{odin_main_keys_key_array_305, 5},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{odin_main_keys_key_array_311, 18},
{odin_main_keys_key_array_312, 3},
{odin_main_keys_key_array_313, 3},
{0, 0},
{odin_main_keys_key_array_315, 5},
{odin_main_keys_key_array_316, 11},
{0, 0},
{odin_main_keys_key_array_318, 11},
{odin_main_keys_key_array_319, 19},
{odin_main_keys_key_array_320, 6},
{odin_main_keys_key_array_321, 3},
{odin_main_keys_key_array_322, 3},
{0, 0},
{0, 0},
{odin_main_keys_key_array_325, 5},
{0, 0},
{0, 0},
{odin_main_keys_key_array_328, 3},
{odin_main_keys_key_array_329, 3},
{odin_main_keys_key_array_330, 31},
{odin_main_keys_key_array_331, 6},
};
Lexeme_Table_Value odin_main_keys_value_array[332] = {
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_for},
{4, TokenOdinKind_defer},
{4, TokenOdinKind_type_of},
{4, TokenOdinKind_f16},
{4, TokenOdinKind_delete_map},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_import},
{0, 0},
{4, TokenOdinKind_bool},
{0, 0},
{0, 0},
{4, TokenOdinKind_copy},
{4, TokenOdinKind_ordered_remove},
{4, TokenOdinKind_copy_slice},
{0, 0},
{4, TokenOdinKind_u8be},
{4, TokenOdinKind_i32be},
{4, TokenOdinKind_fallthrough},
{4, TokenOdinKind_package},
{0, 0},
{0, 0},
{8, TokenOdinKind_LiteralTrue},
{4, TokenOdinKind_make},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_u32},
{4, TokenOdinKind_proc},
{4, TokenOdinKind_excl_elems},
{0, 0},
{0, 0},
{4, TokenOdinKind_where},
{4, TokenOdinKind_byte},
{4, TokenOdinKind_delete_dynamic_array},
{0, 0},
{0, 0},
{4, TokenOdinKind_auto_cast},
{4, TokenOdinKind_excl_elem},
{0, 0},
{0, 0},
{4, TokenOdinKind_real},
{4, TokenOdinKind_delete_slice},
{4, TokenOdinKind_context},
{4, TokenOdinKind_incl_elems},
{0, 0},
{4, TokenOdinKind_copy_from_string},
{4, TokenOdinKind_delete_string},
{4, TokenOdinKind_reserve_dynamic_array},
{4, TokenOdinKind_bit_field},
{4, TokenOdinKind_card},
{0, 0},
{4, TokenOdinKind_i64le},
{0, 0},
{4, TokenOdinKind_case},
{4, TokenOdinKind_notin},
{4, TokenOdinKind_u8le},
{0, 0},
{0, 0},
{4, TokenOdinKind_offset_of},
{4, TokenOdinKind_quaternion},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_dynamic},
{0, 0},
{0, 0},
{4, TokenOdinKind_foreign},
{4, TokenOdinKind_b16},
{4, TokenOdinKind_f64},
{4, TokenOdinKind_cast},
{4, TokenOdinKind_b8},
{0, 0},
{0, 0},
{4, TokenOdinKind_using},
{4, TokenOdinKind_opaque},
{4, TokenOdinKind_append},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_no_inline},
{4, TokenOdinKind_resize},
{0, 0},
{4, TokenOdinKind_type_info_of},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_cap},
{4, TokenOdinKind_reserve_soa},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_when},
{0, 0},
{4, TokenOdinKind_excl_bit_set},
{0, 0},
{0, 0},
{4, TokenOdinKind_i128le},
{4, TokenOdinKind_jmag},
{0, 0},
{0, 0},
{4, TokenOdinKind_u32le},
{0, 0},
{4, TokenOdinKind_else},
{4, TokenOdinKind_complex128},
{4, TokenOdinKind_i16le},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_align_of},
{4, TokenOdinKind_f32},
{0, 0},
{0, 0},
{4, TokenOdinKind_b32},
{0, 0},
{0, 0},
{4, TokenOdinKind_free},
{4, TokenOdinKind_append_elem_string},
{4, TokenOdinKind_u16le},
{0, 0},
{4, TokenOdinKind_incl_elem},
{4, TokenOdinKind_quaternion128},
{0, 0},
{4, TokenOdinKind_map},
{4, TokenOdinKind_do},
{4, TokenOdinKind_delete_key},
{4, TokenOdinKind_unoredered_remove},
{4, TokenOdinKind_excl},
{0, 0},
{4, TokenOdinKind_free_all},
{4, TokenOdinKind_break},
{4, TokenOdinKind_make_dynamic_array_len_cap},
{0, 0},
{0, 0},
{4, TokenOdinKind_rune},
{4, TokenOdinKind_rawptr},
{0, 0},
{0, 0},
{4, TokenOdinKind_i8be},
{4, TokenOdinKind_complex},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_i32},
{4, TokenOdinKind_i16be},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_kmag},
{0, 0},
{4, TokenOdinKind_resize_dynamic_array},
{4, TokenOdinKind_size_of},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_reserve},
{4, TokenOdinKind_incl},
{0, 0},
{4, TokenOdinKind_i128},
{4, TokenOdinKind_u64be},
{4, TokenOdinKind_return},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_i8le},
{0, 0},
{0, 0},
{4, TokenOdinKind_complex32},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_if},
{0, 0},
{4, TokenOdinKind_distinct},
{0, 0},
{4, TokenOdinKind_typeid_of},
{0, 0},
{4, TokenOdinKind_struct},
{4, TokenOdinKind_make_map},
{4, TokenOdinKind_in},
{4, TokenOdinKind_new_clone},
{0, 0},
{0, 0},
{4, TokenOdinKind_i16},
{4, TokenOdinKind_quaternion256},
{0, 0},
{0, 0},
{4, TokenOdinKind_uint},
{4, TokenOdinKind_i128be},
{4, TokenOdinKind_bit_set},
{0, 0},
{0, 0},
{4, TokenOdinKind_u32be},
{0, 0},
{4, TokenOdinKind_append_string},
{4, TokenOdinKind_union},
{0, 0},
{4, TokenOdinKind_switch},
{4, TokenOdinKind_clamp},
{0, 0},
{4, TokenOdinKind_swizzle},
{0, 0},
{4, TokenOdinKind_append_soa_elem},
{0, 0},
{0, 0},
{4, TokenOdinKind_max},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_u128le},
{0, 0},
{0, 0},
{4, TokenOdinKind_clear},
{0, 0},
{0, 0},
{4, TokenOdinKind_string},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_make_dynamic_array_len},
{0, 0},
{4, TokenOdinKind_i8},
{0, 0},
{0, 0},
{4, TokenOdinKind_append_soa_elems},
{4, TokenOdinKind_uintptr},
{4, TokenOdinKind_delete_cstring},
{4, TokenOdinKind_u16},
{4, TokenOdinKind_i64},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_i32le},
{4, TokenOdinKind_delete},
{4, TokenOdinKind_u8},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_u128},
{4, TokenOdinKind_transmute},
{4, TokenOdinKind_imag},
{4, TokenOdinKind_append_elems},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_inline},
{4, TokenOdinKind_typeid},
{4, TokenOdinKind_cstring},
{4, TokenOdinKind_append_soa},
{4, TokenOdinKind_expand_to_tuple},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_conj},
{4, TokenOdinKind_make_slice},
{0, 0},
{0, 0},
{4, TokenOdinKind_pop},
{4, TokenOdinKind_continue},
{4, TokenOdinKind_enum},
{4, TokenOdinKind_abs},
{4, TokenOdinKind_u64le},
{0, 0},
{0, 0},
{4, TokenOdinKind_new},
{0, 0},
{0, 0},
{4, TokenOdinKind_int},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_complex64},
{4, TokenOdinKind_u16be},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_make_dynamic_array},
{4, TokenOdinKind_b64},
{4, TokenOdinKind_any},
{0, 0},
{8, TokenOdinKind_LiteralFalse},
{4, TokenOdinKind_append_elem},
{0, 0},
{4, TokenOdinKind_reserve_map},
{4, TokenOdinKind_clear_dynamic_array},
{4, TokenOdinKind_not_in},
{4, TokenOdinKind_min},
{8, TokenOdinKind_LiteralNil},
{0, 0},
{0, 0},
{4, TokenOdinKind_i64be},
{0, 0},
{0, 0},
{4, TokenOdinKind_u64},
{4, TokenOdinKind_len},
{4, TokenOdinKind_init_global_temporary_allocator},
{4, TokenOdinKind_u128be},
};
i32 odin_main_keys_slot_count = 332;
u64 odin_main_keys_seed = 0xc73cb4963f9f1ed4;
u64 odin_directives_hash_array[33] = {
0xaa44e174ef27f3f1,0x0000000000000000,0x0000000000000000,0x8c09598b0470f551,
0x0000000000000000,0x0000000000000000,0x1d516fcbd16f7027,0x0000000000000000,
0x1d514a930a9eadcf,0x0000000000000000,0x18f79f1e9f3f3dbd,0x0000000000000000,
0x0000000000000000,0xaa7c7b72d873c507,0x0000000000000000,0x2b9b8bdadf118527,
0x0000000000000000,0x1d516fbb927dda75,0x8c09598b046e8d97,0x51138001ac9bf321,
0x0000000000000000,0xb9509e8ca9120e85,0x37249b78cb0c2dbd,0x2b93055e3ee7d91f,
0x18a381fd217dcceb,0x605b75f9b89bf321,0x0000000000000000,0x8c09598b046e00fd,
0x0000000000000000,0x8c09598b046bb781,0xaa7c09f7f02801b1,0xd14bb29ee2aaaacd,
0x0000000000000000,
};
u8 odin_directives_key_array_0[] = {0x6e,0x6f,0x5f,0x61,0x6c,0x69,0x61,0x73,};
u8 odin_directives_key_array_3[] = {0x74,0x79,0x70,0x65,};
u8 odin_directives_key_array_6[] = {0x6e,0x6f,0x5f,0x6e,0x69,0x6c,};
u8 odin_directives_key_array_8[] = {0x70,0x61,0x63,0x6b,0x65,0x64,};
u8 odin_directives_key_array_10[] = {0x72,0x61,0x77,0x5f,0x75,0x6e,0x69,0x6f,0x6e,};
u8 odin_directives_key_array_13[] = {0x63,0x5f,0x76,0x61,0x72,0x61,0x72,0x67,};
u8 odin_directives_key_array_15[] = {0x64,0x65,0x66,0x69,0x6e,0x65,0x64,};
u8 odin_directives_key_array_17[] = {0x61,0x73,0x73,0x65,0x72,0x74,};
u8 odin_directives_key_array_18[] = {0x6c,0x6f,0x61,0x64,};
u8 odin_directives_key_array_19[] = {0x62,0x6f,0x75,0x6e,0x64,0x73,0x5f,0x63,0x68,0x65,0x63,0x6b,};
u8 odin_directives_key_array_21[] = {0x70,0x72,0x6f,0x63,0x65,0x64,0x75,0x72,0x65,};
u8 odin_directives_key_array_22[] = {0x61,0x6c,0x69,0x67,0x6e,};
u8 odin_directives_key_array_23[] = {0x70,0x61,0x72,0x74,0x69,0x61,0x6c,};
u8 odin_directives_key_array_24[] = {0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x73,};
u8 odin_directives_key_array_25[] = {0x6e,0x6f,0x5f,0x62,0x6f,0x75,0x6e,0x64,0x73,0x5f,0x63,0x68,0x65,0x63,0x6b,};
u8 odin_directives_key_array_27[] = {0x6c,0x69,0x6e,0x65,};
u8 odin_directives_key_array_29[] = {0x66,0x69,0x6c,0x65,};
u8 odin_directives_key_array_30[] = {0x63,0x6f,0x6d,0x70,0x6c,0x65,0x74,0x65,};
u8 odin_directives_key_array_31[] = {0x63,0x61,0x6c,0x6c,0x65,0x72,0x5f,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,};
String_Const_u8 odin_directives_key_array[33] = {
{odin_directives_key_array_0, 8},
{0, 0},
{0, 0},
{odin_directives_key_array_3, 4},
{0, 0},
{0, 0},
{odin_directives_key_array_6, 6},
{0, 0},
{odin_directives_key_array_8, 6},
{0, 0},
{odin_directives_key_array_10, 9},
{0, 0},
{0, 0},
{odin_directives_key_array_13, 8},
{0, 0},
{odin_directives_key_array_15, 7},
{0, 0},
{odin_directives_key_array_17, 6},
{odin_directives_key_array_18, 4},
{odin_directives_key_array_19, 12},
{0, 0},
{odin_directives_key_array_21, 9},
{odin_directives_key_array_22, 5},
{odin_directives_key_array_23, 7},
{odin_directives_key_array_24, 9},
{odin_directives_key_array_25, 15},
{0, 0},
{odin_directives_key_array_27, 4},
{0, 0},
{odin_directives_key_array_29, 4},
{odin_directives_key_array_30, 8},
{odin_directives_key_array_31, 15},
{0, 0},
};
Lexeme_Table_Value odin_directives_value_array[33] = {
{4, TokenOdinKind_no_alias},
{0, 0},
{0, 0},
{4, TokenOdinKind_type},
{0, 0},
{0, 0},
{4, TokenOdinKind_no_nil},
{0, 0},
{4, TokenOdinKind_packed},
{0, 0},
{4, TokenOdinKind_raw_union},
{0, 0},
{0, 0},
{4, TokenOdinKind_c_vararg},
{0, 0},
{4, TokenOdinKind_defined},
{0, 0},
{4, TokenOdinKind_assert},
{4, TokenOdinKind_load},
{4, TokenOdinKind_bounds_check},
{0, 0},
{4, TokenOdinKind_procedure},
{4, TokenOdinKind_align},
{4, TokenOdinKind_partial},
{4, TokenOdinKind_locations},
{4, TokenOdinKind_no_bounds_check},
{0, 0},
{4, TokenOdinKind_line},
{0, 0},
{4, TokenOdinKind_file},
{4, TokenOdinKind_complete},
{4, TokenOdinKind_caller_location},
{0, 0},
};
i32 odin_directives_slot_count = 33;
u64 odin_directives_seed = 0x75061fe98fd008b0;
u64 odin_attributes_hash_array[22] = {
0x4d88d72d3da023db,0x0000000000000000,0x0000000000000000,0x1f6c99bc93ba902b,
0xd52c0fbf514bc219,0x0000000000000000,0xcfc8daf2b213e2fb,0x607109b8148ede29,
0x986ddb2fc8752185,0x0dc1d4de6aa56cdb,0x0000000000000000,0x4d88d7e40c09b873,
0xf5085cb568d95565,0x0000000000000000,0xf4777d7bac063cef,0x16f6f1d2338a804d,
0xb06a08ad373fcf35,0x0000000000000000,0x0000000000000000,0x0000000000000000,
0x1f6c9b1717c5601f,0x0000000000000000,
};
u8 odin_attributes_key_array_0[] = {0x65,0x78,0x70,0x6f,0x72,0x74,};
u8 odin_attributes_key_array_3[] = {0x70,0x72,0x69,0x76,0x61,0x74,0x65,};
u8 odin_attributes_key_array_4[] = {0x64,0x65,0x70,0x72,0x65,0x63,0x61,0x74,0x65,0x64,};
u8 odin_attributes_key_array_6[] = {0x64,0x65,0x66,0x65,0x72,0x72,0x65,0x64,0x5f,0x69,0x6e,};
u8 odin_attributes_key_array_7[] = {0x72,0x65,0x71,0x75,0x69,0x72,0x65,0x5f,0x72,0x65,0x73,0x75,0x6c,0x74,0x73,};
u8 odin_attributes_key_array_8[] = {0x64,0x65,0x66,0x65,0x72,0x72,0x65,0x64,0x5f,0x6f,0x75,0x74,};
u8 odin_attributes_key_array_9[] = {0x64,0x65,0x66,0x65,0x72,0x72,0x65,0x64,0x5f,0x6e,0x6f,0x6e,0x65,};
u8 odin_attributes_key_array_11[] = {0x73,0x74,0x61,0x74,0x69,0x63,};
u8 odin_attributes_key_array_12[] = {0x6c,0x69,0x6e,0x6b,0x5f,0x6e,0x61,0x6d,0x65,};
u8 odin_attributes_key_array_14[] = {0x64,0x65,0x66,0x61,0x75,0x6c,0x74,0x5f,0x63,0x61,0x6c,0x6c,0x69,0x6e,0x67,0x5f,0x63,0x6f,0x6e,0x76,0x65,0x6e,0x74,0x69,0x6f,0x6e,};
u8 odin_attributes_key_array_15[] = {0x6c,0x69,0x6e,0x6b,0x5f,0x70,0x72,0x65,0x66,0x69,0x78,};
u8 odin_attributes_key_array_16[] = {0x74,0x68,0x72,0x65,0x61,0x64,0x5f,0x6c,0x6f,0x63,0x61,0x6c,};
u8 odin_attributes_key_array_20[] = {0x62,0x75,0x69,0x6c,0x74,0x69,0x6e,};
String_Const_u8 odin_attributes_key_array[22] = {
{odin_attributes_key_array_0, 6},
{0, 0},
{0, 0},
{odin_attributes_key_array_3, 7},
{odin_attributes_key_array_4, 10},
{0, 0},
{odin_attributes_key_array_6, 11},
{odin_attributes_key_array_7, 15},
{odin_attributes_key_array_8, 12},
{odin_attributes_key_array_9, 13},
{0, 0},
{odin_attributes_key_array_11, 6},
{odin_attributes_key_array_12, 9},
{0, 0},
{odin_attributes_key_array_14, 26},
{odin_attributes_key_array_15, 11},
{odin_attributes_key_array_16, 12},
{0, 0},
{0, 0},
{0, 0},
{odin_attributes_key_array_20, 7},
{0, 0},
};
Lexeme_Table_Value odin_attributes_value_array[22] = {
{4, TokenOdinKind_export},
{0, 0},
{0, 0},
{4, TokenOdinKind_private},
{4, TokenOdinKind_deprecated},
{0, 0},
{4, TokenOdinKind_deferred_in},
{4, TokenOdinKind_require_results},
{4, TokenOdinKind_deferred_out},
{4, TokenOdinKind_deferred_none},
{0, 0},
{4, TokenOdinKind_static},
{4, TokenOdinKind_link_name},
{0, 0},
{4, TokenOdinKind_default_calling_convention},
{4, TokenOdinKind_link_prefix},
{4, TokenOdinKind_thread_local},
{0, 0},
{0, 0},
{0, 0},
{4, TokenOdinKind_builtin},
{0, 0},
};
i32 odin_attributes_slot_count = 22;
u64 odin_attributes_seed = 0x745a8bee6fcf51a6;
struct Lex_State_Odin{
u32 flags_ZF0;
u32 flags_KF0;
u16 flags_KB0;
u8 *base;
u8 *delim_first;
u8 *delim_one_past_last;
u8 *emit_ptr;
u8 *ptr;
u8 *opl_ptr;
};
internal void
lex_full_input_odin_init(Lex_State_Odin *state_ptr, String_Const_u8 input){
state_ptr->flags_ZF0 = 0;
state_ptr->flags_KF0 = 0;
state_ptr->flags_KB0 = 0;
state_ptr->base = input.str;
state_ptr->delim_first = input.str;
state_ptr->delim_one_past_last = input.str;
state_ptr->emit_ptr = input.str;
state_ptr->ptr = input.str;
state_ptr->opl_ptr = input.str + input.size;
}
internal b32
lex_full_input_odin_breaks(Arena *arena, Token_List *list, Lex_State_Odin *state_ptr, u64 max){
b32 result = false;
u64 emit_counter = 0;
Lex_State_Odin state;
block_copy_struct(&state, state_ptr);
{
state_label_1: // root
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_EOF;
token.kind = 0;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
result = true;
goto end;
}
}
switch (*state.ptr){
case 0x00:case 0x01:case 0x02:case 0x03:case 0x04:case 0x05:case 0x06:
case 0x07:case 0x08:case 0x0e:case 0x0f:case 0x10:case 0x11:case 0x12:
case 0x13:case 0x14:case 0x15:case 0x16:case 0x17:case 0x18:case 0x19:
case 0x1a:case 0x1b:case 0x1c:case 0x1d:case 0x1e:case 0x1f:case 0x5c:
case 0x7f:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LexError;
token.kind = 2;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x09:case 0x0a:case 0x0b:case 0x0c:case 0x0d:case 0x20:
{
state.ptr += 1;
goto state_label_3; // whitespace
}break;
case 0x21:
{
state.ptr += 1;
goto state_label_31; // op stage
}break;
case 0x22:
{
state.ptr += 1;
goto state_label_19; // string
}break;
case 0x23:
{
state.ptr += 1;
state.delim_first = state.ptr;
goto state_label_26; // directive
}break;
default:
{
state.ptr += 1;
goto state_label_2; // identifier
}break;
case 0x25:
{
state.ptr += 1;
goto state_label_37; // op stage
}break;
case 0x26:
{
state.ptr += 1;
goto state_label_32; // op stage
}break;
case 0x27:
{
state.ptr += 1;
state.flags_ZF0 |= 0x10;
goto state_label_19; // string
}break;
case 0x28:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_ParenOp;
token.kind = 13;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x29:
{
if ((HasFlag(state.flags_KB0, 0x1))){
state.ptr += 1;
state.flags_KB0 &= ~(0x1);
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_AttributeClose;
token.kind = 14;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_ParenCl;
token.kind = 14;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x2a:
{
state.ptr += 1;
goto state_label_36; // op stage
}break;
case 0x2b:
{
state.ptr += 1;
goto state_label_34; // op stage
}break;
case 0x2c:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Comma;
token.kind = 15;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x2d:
{
state.ptr += 1;
goto state_label_35; // op stage
}break;
case 0x2e:
{
state.ptr += 1;
goto state_label_4; // operator_or_fnumber_dot
}break;
case 0x2f:
{
state.ptr += 1;
goto state_label_5; // operator_or_comment_slash
}break;
case 0x30:
{
state.ptr += 1;
goto state_label_7; // znumber
}break;
case 0x31:case 0x32:case 0x33:case 0x34:case 0x35:case 0x36:case 0x37:
case 0x38:case 0x39:
{
state.ptr += 1;
goto state_label_6; // number
}break;
case 0x3a:
{
state.ptr += 1;
goto state_label_27; // op stage
}break;
case 0x3b:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Semicolon;
token.kind = 15;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x3c:
{
state.ptr += 1;
goto state_label_28; // op stage
}break;
case 0x3d:
{
state.ptr += 1;
goto state_label_30; // op stage
}break;
case 0x3e:
{
state.ptr += 1;
goto state_label_29; // op stage
}break;
case 0x3f:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Ternary;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x40:
{
state.ptr += 1;
state.delim_first = state.ptr;
goto state_label_24; // attribute_first
}break;
case 0x5b:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_BrackOp;
token.kind = 13;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x5d:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_BrackCl;
token.kind = 14;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x5e:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Carrot;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x60:
{
state.ptr += 1;
state.flags_ZF0 |= 0x20;
goto state_label_19; // string
}break;
case 0x7b:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_BraceOp;
token.kind = 11;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x7c:
{
state.ptr += 1;
goto state_label_33; // op stage
}break;
case 0x7d:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_BraceCl;
token.kind = 12;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x7e:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Tilde;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_2: // identifier
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
if (HasFlag(state.flags_KB0, 0x1)){
Lexeme_Table_Lookup lookup = lexeme_table_lookup(odin_attributes_hash_array, odin_attributes_key_array, odin_attributes_value_array, odin_attributes_slot_count, odin_attributes_seed, state.emit_ptr, token.size);
if (lookup.found_match){
token.kind = lookup.base_kind;
token.sub_kind = lookup.sub_kind;
break;
}
}
Lexeme_Table_Lookup lookup = lexeme_table_lookup(odin_main_keys_hash_array, odin_main_keys_key_array, odin_main_keys_value_array, odin_main_keys_slot_count, odin_main_keys_seed, state.emit_ptr, token.size);
if (lookup.found_match){
token.kind = lookup.base_kind;
token.sub_kind = lookup.sub_kind;
break;
}
token.sub_kind = TokenOdinKind_Identifier;
token.kind = 6;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
case 0x00:case 0x01:case 0x02:case 0x03:case 0x04:case 0x05:case 0x06:
case 0x07:case 0x08:case 0x09:case 0x0a:case 0x0b:case 0x0c:case 0x0d:
case 0x0e:case 0x0f:case 0x10:case 0x11:case 0x12:case 0x13:case 0x14:
case 0x15:case 0x16:case 0x17:case 0x18:case 0x19:case 0x1a:case 0x1b:
case 0x1c:case 0x1d:case 0x1e:case 0x1f:case 0x20:case 0x21:case 0x22:
case 0x23:case 0x25:case 0x26:case 0x27:case 0x28:case 0x29:case 0x2a:
case 0x2b:case 0x2c:case 0x2d:case 0x2e:case 0x2f:case 0x3a:case 0x3b:
case 0x3c:case 0x3d:case 0x3e:case 0x3f:case 0x40:case 0x5b:case 0x5c:
case 0x5d:case 0x5e:case 0x60:case 0x7b:case 0x7c:case 0x7d:case 0x7e:
case 0x7f:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
if (HasFlag(state.flags_KB0, 0x1)){
Lexeme_Table_Lookup lookup = lexeme_table_lookup(odin_attributes_hash_array, odin_attributes_key_array, odin_attributes_value_array, odin_attributes_slot_count, odin_attributes_seed, state.emit_ptr, token.size);
if (lookup.found_match){
token.kind = lookup.base_kind;
token.sub_kind = lookup.sub_kind;
break;
}
}
Lexeme_Table_Lookup lookup = lexeme_table_lookup(odin_main_keys_hash_array, odin_main_keys_key_array, odin_main_keys_value_array, odin_main_keys_slot_count, odin_main_keys_seed, state.emit_ptr, token.size);
if (lookup.found_match){
token.kind = lookup.base_kind;
token.sub_kind = lookup.sub_kind;
break;
}
token.sub_kind = TokenOdinKind_Identifier;
token.kind = 6;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
default:
{
state.ptr += 1;
goto state_label_2; // identifier
}break;
}
}
{
state_label_3: // whitespace
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Whitespace;
token.kind = 1;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Whitespace;
token.kind = 1;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x09:case 0x0a:case 0x0b:case 0x0c:case 0x0d:case 0x20:
{
state.ptr += 1;
goto state_label_3; // whitespace
}break;
}
}
{
state_label_4: // operator_or_fnumber_dot
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Dot;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Dot;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x2e:
{
state.ptr += 1;
goto state_label_40; // op stage
}break;
case 0x30:case 0x31:case 0x32:case 0x33:case 0x34:case 0x35:case 0x36:
case 0x37:case 0x38:case 0x39:
{
state.ptr += 1;
goto state_label_8; // fnumber_decimal
}break;
}
}
{
state_label_5: // operator_or_comment_slash
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Div;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Div;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x2a:
{
state.ptr += 1;
goto state_label_21; // comment_block
}break;
case 0x2f:
{
state.ptr += 1;
goto state_label_23; // comment_line
}break;
case 0x3d:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_DivEq;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_6: // number
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralInteger;
token.kind = 8;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralInteger;
token.kind = 8;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x2e:
{
state.ptr += 1;
goto state_label_8; // fnumber_decimal
}break;
case 0x30:case 0x31:case 0x32:case 0x33:case 0x34:case 0x35:case 0x36:
case 0x37:case 0x38:case 0x39:
{
state.ptr += 1;
goto state_label_6; // number
}break;
case 0x45:case 0x65:
{
state.ptr += 1;
goto state_label_9; // fnumber_exponent
}break;
}
}
{
state_label_7: // znumber
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralInteger;
token.kind = 8;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralInteger;
token.kind = 8;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x2e:
{
state.ptr += 1;
goto state_label_8; // fnumber_decimal
}break;
case 0x42:case 0x62:
{
state.ptr += 1;
state.flags_ZF0 |= 0x8;
goto state_label_14; // number_bin_first
}break;
case 0x45:case 0x65:
{
state.ptr += 1;
goto state_label_9; // fnumber_exponent
}break;
case 0x4f:case 0x6f:
{
state.ptr += 1;
state.flags_ZF0 |= 0x2;
goto state_label_16; // number_oct
}break;
case 0x58:case 0x78:
{
state.ptr += 1;
state.flags_ZF0 |= 0x1;
goto state_label_12; // number_hex_first
}break;
case 0x5a:case 0x7a:
{
state.ptr += 1;
state.flags_ZF0 |= 0x4;
goto state_label_13; // number_doz_first
}break;
}
}
{
state_label_8: // fnumber_decimal
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralFloat;
token.kind = 9;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralFloat;
token.kind = 9;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x30:case 0x31:case 0x32:case 0x33:case 0x34:case 0x35:case 0x36:
case 0x37:case 0x38:case 0x39:
{
state.ptr += 1;
goto state_label_8; // fnumber_decimal
}break;
case 0x45:case 0x65:
{
state.ptr += 1;
goto state_label_9; // fnumber_exponent
}break;
}
}
{
state_label_9: // fnumber_exponent
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralFloat;
token.kind = 9;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralFloat;
token.kind = 9;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x2b:case 0x2d:
{
state.ptr += 1;
goto state_label_10; // fnumber_exponent_sign
}break;
case 0x30:case 0x31:case 0x32:case 0x33:case 0x34:case 0x35:case 0x36:
case 0x37:case 0x38:case 0x39:
{
state.ptr += 1;
goto state_label_11; // fnumber_exponent_digits
}break;
}
}
{
state_label_10: // fnumber_exponent_sign
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralFloat;
token.kind = 9;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralFloat;
token.kind = 9;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x30:case 0x31:case 0x32:case 0x33:case 0x34:case 0x35:case 0x36:
case 0x37:case 0x38:case 0x39:
{
state.ptr += 1;
goto state_label_11; // fnumber_exponent_digits
}break;
}
}
{
state_label_11: // fnumber_exponent_digits
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralFloat;
token.kind = 9;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralFloat;
token.kind = 9;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x30:case 0x31:case 0x32:case 0x33:case 0x34:case 0x35:case 0x36:
case 0x37:case 0x38:case 0x39:
{
state.ptr += 1;
goto state_label_11; // fnumber_exponent_digits
}break;
}
}
{
state_label_12: // number_hex_first
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LexError;
token.kind = 2;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LexError;
token.kind = 2;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x30:case 0x31:case 0x32:case 0x33:case 0x34:case 0x35:case 0x36:
case 0x37:case 0x38:case 0x39:case 0x41:case 0x42:case 0x43:case 0x44:
case 0x45:case 0x46:case 0x61:case 0x62:case 0x63:case 0x64:case 0x65:
case 0x66:
{
state.ptr += 1;
goto state_label_15; // number_hex
}break;
}
}
{
state_label_13: // number_doz_first
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LexError;
token.kind = 2;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LexError;
token.kind = 2;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x30:case 0x31:case 0x32:case 0x33:case 0x34:case 0x35:case 0x36:
case 0x37:case 0x38:case 0x39:case 0x41:case 0x42:case 0x61:case 0x62:
{
state.ptr += 1;
goto state_label_17; // number_doz
}break;
}
}
{
state_label_14: // number_bin_first
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LexError;
token.kind = 2;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LexError;
token.kind = 2;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x30:case 0x31:
{
state.ptr += 1;
goto state_label_18; // number_bin
}break;
case 0x5f:
{
state.ptr += 1;
state.flags_ZF0 |= 0x8;
goto state_label_14; // number_bin_first
}break;
}
}
{
state_label_15: // number_hex
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralIntegerHex;
token.kind = 8;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralIntegerHex;
token.kind = 8;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x30:case 0x31:case 0x32:case 0x33:case 0x34:case 0x35:case 0x36:
case 0x37:case 0x38:case 0x39:case 0x41:case 0x42:case 0x43:case 0x44:
case 0x45:case 0x46:case 0x61:case 0x62:case 0x63:case 0x64:case 0x65:
case 0x66:
{
state.ptr += 1;
goto state_label_15; // number_hex
}break;
}
}
{
state_label_16: // number_oct
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralIntegerOct;
token.kind = 8;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralIntegerOct;
token.kind = 8;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x30:case 0x31:case 0x32:case 0x33:case 0x34:case 0x35:case 0x36:
case 0x37:
{
state.ptr += 1;
state.flags_ZF0 |= 0x2;
goto state_label_16; // number_oct
}break;
}
}
{
state_label_17: // number_doz
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralIntegerDoz;
token.kind = 8;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralIntegerDoz;
token.kind = 8;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x30:case 0x31:case 0x32:case 0x33:case 0x34:case 0x35:case 0x36:
case 0x37:case 0x38:case 0x39:case 0x41:case 0x42:case 0x43:case 0x44:
case 0x45:case 0x46:case 0x61:case 0x62:case 0x63:case 0x64:case 0x65:
case 0x66:
{
state.ptr += 1;
goto state_label_17; // number_doz
}break;
}
}
{
state_label_18: // number_bin
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralIntegerBin;
token.kind = 8;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralIntegerBin;
token.kind = 8;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x30:case 0x31:case 0x5f:
{
state.ptr += 1;
goto state_label_18; // number_bin
}break;
}
}
{
state_label_19: // string
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LexError;
token.kind = 2;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
state.ptr += 1;
goto state_label_19; // string
}break;
case 0x0a:
{
if ((!HasFlag(state.flags_ZF0, 0x20))){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LexError;
token.kind = 2;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
state.ptr += 1;
goto state_label_19; // string
}break;
case 0x22:
{
if ((!HasFlag(state.flags_ZF0, 0x10))){
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralString;
token.kind = 10;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
state.ptr += 1;
goto state_label_19; // string
}break;
case 0x27:
{
if ((HasFlag(state.flags_ZF0, 0x10))){
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralCharacter;
token.kind = 10;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
if ((!HasFlag(state.flags_ZF0, 0x20) && !HasFlag(state.flags_ZF0, 0x10))){
state.ptr += 1;
goto state_label_19; // string
}
state.ptr += 1;
goto state_label_19; // string
}break;
case 0x5c:
{
if ((!HasFlag(state.flags_ZF0, 0x20))){
state.ptr += 1;
goto state_label_20; // string_esc
}
state.ptr += 1;
goto state_label_19; // string
}break;
case 0x60:
{
if ((HasFlag(state.flags_ZF0, 0x20))){
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LiteralStringRaw;
token.kind = 10;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
state.ptr += 1;
goto state_label_19; // string
}break;
}
}
{
state_label_20: // string_esc
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LexError;
token.kind = 2;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_EOF;
token.kind = 0;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
result = true;
goto end;
}
}
switch (*state.ptr){
default:
{
state.ptr += 1;
goto state_label_19; // string
}break;
}
}
{
state_label_21: // comment_block
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_BlockComment;
token.kind = 3;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_EOF;
token.kind = 0;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
result = true;
goto end;
}
}
switch (*state.ptr){
default:
{
state.ptr += 1;
goto state_label_21; // comment_block
}break;
case 0x2a:
{
state.ptr += 1;
goto state_label_22; // comment_block_try_close
}break;
}
}
{
state_label_22: // comment_block_try_close
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_BlockComment;
token.kind = 3;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_EOF;
token.kind = 0;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
result = true;
goto end;
}
}
switch (*state.ptr){
default:
{
state.ptr += 1;
goto state_label_21; // comment_block
}break;
case 0x2a:
{
state.ptr += 1;
goto state_label_22; // comment_block_try_close
}break;
case 0x2f:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_BlockComment;
token.kind = 3;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_23: // comment_line
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LineComment;
token.kind = 3;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
state.ptr += 1;
goto state_label_23; // comment_line
}break;
case 0x0a:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LineComment;
token.kind = 3;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_24: // attribute_first
if (state.ptr == state.opl_ptr){
if ((true)){
goto state_label_25; // attribute
}
}
switch (*state.ptr){
default:
{
goto state_label_25; // attribute
}break;
case 0x28:
{
state.ptr += 1;
state.flags_KB0 |= 0x1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_AttributeOpen;
token.kind = 13;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_25: // attribute
if (state.ptr == state.opl_ptr){
if ((true)){
state.delim_one_past_last = state.ptr;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
Lexeme_Table_Lookup lookup = lexeme_table_lookup(odin_attributes_hash_array, odin_attributes_key_array, odin_attributes_value_array, odin_attributes_slot_count, odin_attributes_seed, state.delim_first, (state.delim_one_past_last - state.delim_first));
if (lookup.found_match){
token.kind = lookup.base_kind;
token.sub_kind = lookup.sub_kind;
break;
}
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
state.delim_one_past_last = state.ptr;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
Lexeme_Table_Lookup lookup = lexeme_table_lookup(odin_attributes_hash_array, odin_attributes_key_array, odin_attributes_value_array, odin_attributes_slot_count, odin_attributes_seed, state.delim_first, (state.delim_one_past_last - state.delim_first));
if (lookup.found_match){
token.kind = lookup.base_kind;
token.sub_kind = lookup.sub_kind;
break;
}
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x41:case 0x42:case 0x43:case 0x44:case 0x45:case 0x46:case 0x47:
case 0x48:case 0x49:case 0x4a:case 0x4b:case 0x4c:case 0x4d:case 0x4e:
case 0x4f:case 0x50:case 0x51:case 0x52:case 0x53:case 0x54:case 0x55:
case 0x56:case 0x57:case 0x58:case 0x59:case 0x5a:case 0x5f:case 0x61:
case 0x62:case 0x63:case 0x64:case 0x65:case 0x66:case 0x67:case 0x68:
case 0x69:case 0x6a:case 0x6b:case 0x6c:case 0x6d:case 0x6e:case 0x6f:
case 0x70:case 0x71:case 0x72:case 0x73:case 0x74:case 0x75:case 0x76:
case 0x77:case 0x78:case 0x79:case 0x7a:
{
state.ptr += 1;
goto state_label_25; // attribute
}break;
}
}
{
state_label_26: // directive
if (state.ptr == state.opl_ptr){
if ((true)){
state.delim_one_past_last = state.ptr;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
Lexeme_Table_Lookup lookup = lexeme_table_lookup(odin_directives_hash_array, odin_directives_key_array, odin_directives_value_array, odin_directives_slot_count, odin_directives_seed, state.delim_first, (state.delim_one_past_last - state.delim_first));
if (lookup.found_match){
token.kind = lookup.base_kind;
token.sub_kind = lookup.sub_kind;
break;
}
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
state.delim_one_past_last = state.ptr;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
Lexeme_Table_Lookup lookup = lexeme_table_lookup(odin_directives_hash_array, odin_directives_key_array, odin_directives_value_array, odin_directives_slot_count, odin_directives_seed, state.delim_first, (state.delim_one_past_last - state.delim_first));
if (lookup.found_match){
token.kind = lookup.base_kind;
token.sub_kind = lookup.sub_kind;
break;
}
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x41:case 0x42:case 0x43:case 0x44:case 0x45:case 0x46:case 0x47:
case 0x48:case 0x49:case 0x4a:case 0x4b:case 0x4c:case 0x4d:case 0x4e:
case 0x4f:case 0x50:case 0x51:case 0x52:case 0x53:case 0x54:case 0x55:
case 0x56:case 0x57:case 0x58:case 0x59:case 0x5a:case 0x5f:case 0x61:
case 0x62:case 0x63:case 0x64:case 0x65:case 0x66:case 0x67:case 0x68:
case 0x69:case 0x6a:case 0x6b:case 0x6c:case 0x6d:case 0x6e:case 0x6f:
case 0x70:case 0x71:case 0x72:case 0x73:case 0x74:case 0x75:case 0x76:
case 0x77:case 0x78:case 0x79:case 0x7a:
{
state.ptr += 1;
goto state_label_26; // directive
}break;
}
}
{
state_label_27: // op stage
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Colon;
token.kind = 15;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Colon;
token.kind = 15;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x3a:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_ColonColon;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x3d:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_ColonEq;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_28: // op stage
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Less;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Less;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x3c:
{
state.ptr += 1;
goto state_label_38; // op stage
}break;
case 0x3d:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LessEq;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_29: // op stage
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Grtr;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Grtr;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x3d:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_GrtrEq;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x3e:
{
state.ptr += 1;
goto state_label_39; // op stage
}break;
}
}
{
state_label_30: // op stage
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Eq;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Eq;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x3d:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_EqEq;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_31: // op stage
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Not;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Not;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x3d:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_NotEq;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_32: // op stage
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_And;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_And;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x26:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_AndAnd;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_33: // op stage
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Or;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Or;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x7c:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_OrOr;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_34: // op stage
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Plus;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Plus;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x3d:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_PlusEq;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_35: // op stage
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Minus;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Minus;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x3d:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_MinusEq;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_36: // op stage
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Star;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Star;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x3d:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_StarEq;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_37: // op stage
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Mod;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_Mod;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x3d:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_ModEq;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_38: // op stage
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LeftLeft;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LeftLeft;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x3d:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_LeftLeftEq;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_39: // op stage
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_RightRight;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_RightRight;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x3d:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_RightRightEq;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
{
state_label_40: // op stage
if (state.ptr == state.opl_ptr){
if ((true)){
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_DotDot;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}
}
switch (*state.ptr){
default:
{
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_DotDot;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
case 0x3c:
{
state.ptr += 1;
{
Token token = {};
token.pos = (i64)(state.emit_ptr - state.base);
token.size = (i64)(state.ptr - state.emit_ptr);
token.flags = state.flags_KB0;
do{
token.sub_kind = TokenOdinKind_DotDotLess;
token.kind = 7;
}while(0);
token_list_push(arena, list, &token);
emit_counter += 1;
state.emit_ptr = state.ptr;
if (emit_counter == max){
goto end;
}
}
state.flags_KF0 |= 0x1;
state.flags_ZF0 = 0;
goto state_label_1; // root
}break;
}
}
end:;
block_copy_struct(state_ptr, &state);
return(result);
}
internal Token_List
lex_full_input_odin(Arena *arena, String_Const_u8 input){
Lex_State_Odin state = {};
lex_full_input_odin_init(&state, input);
Token_List list = {};
lex_full_input_odin_breaks(arena, &list, &state, max_u64);
return(list);
}
