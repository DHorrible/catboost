

RESOURCES_LIBRARY()

IF (NOT HOST_OS_DARWIN AND NOT HOST_OS_LINUX AND NOT HOST_OS_WINDOWS)
    MESSAGE(FATAL_ERROR Unsupported host platform for GO_TOOLS)
ELSEIF(GOSTD_VERSION == 1.18.4)
    DECLARE_EXTERNAL_HOST_RESOURCES_BUNDLE(
        GO_TOOLS
        sbr:3383386553 FOR DARWIN-ARM64
        sbr:3383397086 FOR DARWIN
        sbr:3383407059 FOR LINUX
        sbr:3392015425 FOR WIN32
    )
ELSEIF(GOSTD_VERSION == 1.19.1)
    DECLARE_EXTERNAL_HOST_RESOURCES_BUNDLE(
        GO_TOOLS
        sbr:3574365459 FOR DARWIN-ARM64
        sbr:3574390680 FOR DARWIN
        sbr:3574341336 FOR LINUX
        sbr:3574325228 FOR WIN32
    )
ELSE()
    MESSAGE(FATAL_ERROR Unsupported version [${GOSTD_VERSION}] of Go Standard Library)
ENDIF()

END()
