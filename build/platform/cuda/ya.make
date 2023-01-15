RESOURCES_LIBRARY()

# https://wiki.yandex-team.ru/devtools/cuda/



IF (NOT HAVE_CUDA)
    MESSAGE(FATAL_ERROR "No CUDA Toolkit for your build")
ENDIF()

IF (USE_ARCADIA_CUDA)
    IF (HOST_OS_LINUX AND HOST_ARCH_X86_64)
        IF (OS_LINUX AND ARCH_X86_64)
            IF (CUDA_VERSION == "11.1")
                DECLARE_EXTERNAL_RESOURCE(CUDA sbr:1882836946) # CUDA Toolkit 11.1.1 for Linux x86-64
            ELSEIF (CUDA_VERSION == "11.0")
                DECLARE_EXTERNAL_RESOURCE(CUDA sbr:1647896014) # CUDA Toolkit 11.0.2 for Linux x86-64
            ELSEIF (CUDA_VERSION == "10.1")
                DECLARE_EXTERNAL_RESOURCE(CUDA sbr:983615296) # CUDA Toolkit 10.1.168 for Linux x86-64
            ELSEIF (CUDA_VERSION == "10.0")
                DECLARE_EXTERNAL_RESOURCE(CUDA sbr:840560679) # CUDA Toolkit 10.0.130 for Linux x86-64
            ELSE()
                ENABLE(CUDA_NOT_FOUND)
            ENDIF()

        ELSE()
            ENABLE(CUDA_NOT_FOUND)
        ENDIF()

    ELSEIF (HOST_OS_LINUX AND HOST_ARCH_PPC64LE)
        IF (OS_LINUX AND ARCH_PPC64LE)
            IF (CUDA_VERSION == "10.1")
                DECLARE_EXTERNAL_RESOURCE(CUDA sbr:1586537264) # CUDA Toolkit 10.1.168 for Linux ppc64le
            ELSE()
                ENABLE(CUDA_NOT_FOUND)
            ENDIF()

        ELSE()
            ENABLE(CUDA_NOT_FOUND)
        ENDIF()

    ELSEIF (HOST_OS_DARWIN AND HOST_ARCH_X86_64)
        IF (OS_DARWIN AND ARCH_X86_64)
            IF (CUDA_VERSION == "10.1")
                DECLARE_EXTERNAL_RESOURCE(CUDA sbr:978727023) # CUDA Toolkit 10.1.168 for macOS x86-64
            ELSEIF (CUDA_VERSION == "10.0")
                DECLARE_EXTERNAL_RESOURCE(CUDA sbr:840563990) # CUDA Toolkit 10.0.130 for macOS x86-64
            ENDIF()

        ELSE()
            ENABLE(CUDA_NOT_FOUND)
        ENDIF()

    ELSEIF (HOST_OS_WINDOWS AND HOST_ARCH_X86_64)
        IF (OS_WINDOWS AND ARCH_X86_64)
            IF (CUDA_VERSION == "11.1")
                DECLARE_EXTERNAL_RESOURCE(CUDA sbr:1896564605) # CUDA Toolkit 11.1.1 for Windows x86-64
            ELSEIF (CUDA_VERSION == "10.1")
                DECLARE_EXTERNAL_RESOURCE(CUDA sbr:978734165) # CUDA Toolkit 10.1.168 for Windows x86-64
            ELSEIF (CUDA_VERSION == "10.0")
                DECLARE_EXTERNAL_RESOURCE(CUDA sbr:840570409) # CUDA Toolkit 10.0.130 for Windows x86-64
            ELSE()
                ENABLE(CUDA_NOT_FOUND)
            ENDIF()

        ELSE()
            ENABLE(CUDA_NOT_FOUND)
        ENDIF()

    ELSE()
        ENABLE(CUDA_NOT_FOUND)
    ENDIF()
ENDIF()

IF (USE_ARCADIA_CUDA_HOST_COMPILER)
    IF (HOST_OS_LINUX AND HOST_ARCH_X86_64)
        IF (OS_LINUX AND ARCH_X86_64)
            IF (CUDA_VERSION == "11.1")
                DECLARE_EXTERNAL_RESOURCE(CUDA_HOST_TOOLCHAIN sbr:971092418) # Clang 8.0 for Linux x86-64 (may be not latest)
            ELSEIF (CUDA_VERSION == "11.0")
                DECLARE_EXTERNAL_RESOURCE(CUDA_HOST_TOOLCHAIN sbr:971092418) # Clang 8.0 for Linux x86-64 (may be not latest)
            ELSEIF (CUDA_VERSION == "10.1")
                DECLARE_EXTERNAL_RESOURCE(CUDA_HOST_TOOLCHAIN sbr:243907179) # Clang 4.0 for Linux x86-64 (not latest)
            ELSEIF (CUDA_VERSION == "10.0")
                DECLARE_EXTERNAL_RESOURCE(CUDA_HOST_TOOLCHAIN sbr:243907179) # Clang 4.0 for Linux x86-64 (not latest)
            ELSE()
                ENABLE(CUDA_HOST_COMPILER_NOT_FOUND)
            ENDIF()

        ELSE()
            ENABLE(CUDA_HOST_COMPILER_NOT_FOUND)
        ENDIF()

    ELSEIF (HOST_OS_LINUX AND HOST_ARCH_PPC64LE)
        IF (OS_LINUX AND ARCH_PPC64LE)
            IF (CUDA_VERSION == "10.1")
                DECLARE_EXTERNAL_RESOURCE(CUDA_HOST_TOOLCHAIN sbr:1566513994) # Clang 7.0 for Linux ppc64le (not latest)
            ELSE()
                ENABLE(CUDA_HOST_COMPILER_NOT_FOUND)
            ENDIF()

        ELSE()
            ENABLE(CUDA_HOST_COMPILER_NOT_FOUND)
        ENDIF()

    ELSEIF (HOST_OS_DARWIN AND HOST_ARCH_X86_64)
        IF (OS_DARWIN AND ARCH_X86_64)
            SET(__XCODE_RESOURCE_NAME CUDA_HOST_TOOLCHAIN)
            IF (CUDA_VERSION == "10.1")
                SET(__XCODE_TOOLCHAIN_VERSION "9.2") # (not latest)
            ELSEIF (CUDA_VERSION == "10.0")
                SET(__XCODE_TOOLCHAIN_VERSION "9.2") # (not latest)
            ELSE()
                SET(__XCODE_TOOLCHAIN_VERSION "")
                ENABLE(CUDA_HOST_COMPILER_NOT_FOUND)
            ENDIF()
            IF (__XCODE_TOOLCHAIN_VERSION)
                INCLUDE(${ARCADIA_ROOT}/build/platform/xcode/ya.make.inc)
            ENDIF()
        ELSE()
            ENABLE(CUDA_HOST_COMPILER_NOT_FOUND)
        ENDIF()

    ELSEIF (HOST_OS_WINDOWS AND HOST_ARCH_X86_64)
        IF (OS_WINDOWS AND ARCH_X86_64)
            IF (CUDA_HOST_MSVC_VERSION == "14.11.25503")
                DECLARE_EXTERNAL_RESOURCE(CUDA_HOST_TOOLCHAIN sbr:637113754) # Microsoft Visual C++ 14.11.25503
            ELSEIF (CUDA_HOST_MSVC_VERSION == "14.13.26128")
                DECLARE_EXTERNAL_RESOURCE(CUDA_HOST_TOOLCHAIN sbr:631304468) # Microsoft Visual C++ 14.13.26128
            ELSE()
                MESSAGE(FATAL_ERROR "Unexpected or unspecified Microsoft Visual C++ CUDA host compiler version")
            ENDIF()

        ELSE()
            ENABLE(CUDA_HOST_COMPILER_NOT_FOUND)
        ENDIF()

    ELSE()
        ENABLE(CUDA_HOST_COMPILER_NOT_FOUND)
    ENDIF()
ENDIF()

IF (CUDA_NOT_FOUND)
    MESSAGE(FATAL_ERROR "No CUDA Toolkit for the selected platform")
ENDIF()

IF (CUDA_HOST_COMPILER_NOT_FOUND)
    MESSAGE(FATAL_ERROR "No CUDA host compiler for the selected platform and CUDA Toolkit version ${CUDA_VERSION}")
ENDIF()

IF (HOST_OS_WINDOWS)
    SET_APPEND_WITH_GLOBAL(USER_CFLAGS GLOBAL "\"-I${CUDA_ROOT}/include\"")
ELSE()
    CFLAGS(GLOBAL "-I${CUDA_ROOT}/include")
ENDIF()

IF (HOST_OS_WINDOWS)
    SET_APPEND(LDFLAGS_GLOBAL "\"/LIBPATH:${CUDA_ROOT}/lib/x64\"")
ELSEIF(HOST_OS_LINUX)
    LDFLAGS("-L${CUDA_ROOT}/lib64")
ELSE()
    LDFLAGS("-L${CUDA_ROOT}/lib")
ENDIF()

IF (CUDA_REQUIRED)
    IF(HOST_OS_LINUX)
        LDFLAGS("-L${CUDA_ROOT}/lib64/stubs")
        EXTRALIBS(-lcuda)
    ELSEIF(HOST_OS_DARWIN)
        LDFLAGS("-F${CUDA_ROOT}/lib/stubs -framework CUDA")
    ENDIF()
ENDIF()

IF (HOST_OS_WINDOWS)
    LDFLAGS(cudart_static.lib)
ELSE()
    EXTRALIBS(-lcudart_static -lculibos)
ENDIF()

END()
