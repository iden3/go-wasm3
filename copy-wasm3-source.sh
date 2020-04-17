#!/bin/sh

cp \
    wasm3/source/m3_api_defs.h \
    wasm3/source/m3_api_libc.h \
    wasm3/source/m3_api_wasi.h \
    wasm3/source/m3_code.h \
    wasm3/source/m3_compile.h \
    wasm3/source/m3_config.h \
    wasm3/source/m3_config_platforms.h \
    wasm3/source/m3_core.h \
    wasm3/source/m3_emit.h \
    wasm3/source/m3_env.h \
    wasm3/source/m3_exception.h \
    wasm3/source/m3_exec_defs.h \
    wasm3/source/m3_exec.h \
    wasm3/source/m3.h \
    wasm3/source/m3_info.h \
    wasm3/source/m3_math_utils.h \
    wasm3/source/wasm3.h \
    .

cp \
    wasm3/source/m3_core.c \
    wasm3/source/m3_compile.c \
    wasm3/source/m3_exec.c \
    wasm3/source/m3_env.c \
    wasm3/source/m3_api_libc.c \
    wasm3/source/m3_api_meta_wasi.c \
    wasm3/source/m3_api_wasi.c \
    wasm3/source/m3_bind.c \
    wasm3/source/m3_code.c \
    wasm3/source/m3_emit.c \
    wasm3/source/m3_info.c \
    wasm3/source/m3_module.c \
    wasm3/source/m3_optimize.c \
    wasm3/source/m3_parse.c \
    .
