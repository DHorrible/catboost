# Generated by devtools/yamaker from nixpkgs 28acaac96f0cc203c63a3d50634541feff7fc31c.

LIBRARY()



VERSION(12.0.0-post-2021-05-08-de1843e51a76c5628dc93c0507a4fb8e7be52482)

LICENSE(Apache-2.0 WITH LLVM-exception)

DISABLE(USE_LTO)

ADDINCL(
    contrib/libs/libunwind/include
)

NO_RUNTIME()

NO_UTIL()

NO_SANITIZE()

NO_SANITIZE_COVERAGE()

CFLAGS(
    GLOBAL -D_libunwind_
    -D_LIBUNWIND_IS_NATIVE_ONLY
    -fno-exceptions
    -fno-rtti
    -funwind-tables
)

IF (SANITIZER_TYPE == memory)
    CFLAGS(-fPIC)
ENDIF()

SRCS(
    src/Unwind-EHABI.cpp
    src/Unwind-seh.cpp
    src/Unwind-sjlj.c
    src/UnwindLevel1-gcc-ext.c
    src/UnwindLevel1.c
    src/UnwindRegistersRestore.S
    src/UnwindRegistersSave.S
    src/libunwind.cpp
)

IF (OS_DARWIN OR OS_IOS)
    SRCS(
        src/Unwind_AppleExtras.cpp
    )
ENDIF()

END()

RECURSE_FOR_TESTS(
    ut
)
