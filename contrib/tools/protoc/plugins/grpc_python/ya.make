# Copy of contrib/libs/grpc/src/compiler/grpc_python_plugin
# Generated by devtools/yamaker.



IF (USE_PREBUILT_TOOLS)
    INCLUDE(${ARCADIA_ROOT}/build/prebuilt/contrib/tools/protoc/plugins/grpc_python/ya.make.prebuilt)
ENDIF()

IF (NOT PREBUILT)
    INCLUDE(${ARCADIA_ROOT}/contrib/tools/protoc/plugins/grpc_python/bin/ya.make)
ENDIF()

RECURSE(
    bin
)
