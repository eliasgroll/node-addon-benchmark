{
    "targets": [{
        "target_name": "node_addon_benchmark",
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")",
            "<!@(node -p \"require('napi-thread-safe-callback').include\")"
        ],
        "ldflags": [
        ],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'cflags!': [ '-fno-exceptions' ],
        'cflags_cc!': [ '-fno-exceptions' ],
        'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'CLANG_CXX_LIBRARY': 'libc++',
            'MACOSX_DEPLOYMENT_TARGET': '10.7',
        },
        'msvs_settings': {
            'VCCLCompilerTool': { 'ExceptionHandling': 1 },
        },
        "sources" : [
            "module.cc"
        ]
    }]
}