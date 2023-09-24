
def custom_cc_library(name, srcs, **kwargs):
    native.cc_library(
        name = name,
        srcs = srcs,
        linkopts = select({
            "//conditions:default": [],
            ":enable_stacktrace": ["-lstdc++_libbacktrace", "-static"],
        }),
        copts = select({
            "//conditions:default": [],
            ":enable_stacktrace": ["-DENABLE_STACKTRACE", "-no-pie"],
        }),
        **kwargs
    )

def custom_cc_binary(name, srcs, **kwargs):
    native.cc_binary(
        name = name,
        srcs = srcs,
        linkopts = select({
            "//conditions:default": [],
            ":enable_stacktrace": ["-lstdc++_libbacktrace", "-static"],
        }),
        copts = select({
            "//conditions:default": [],
            ":enable_stacktrace": ["-DENABLE_STACKTRACE", "-no-pie"],
        }),
        **kwargs
    )

def custom_cc_test(name, srcs, **kwargs):
    native.cc_test(
        name = name,
        srcs = srcs,
        linkopts = select({
            "//conditions:default": [],
            ":enable_stacktrace": ["-lstdc++_libbacktrace", "-static"],
        }),
        copts = select({
            "//conditions:default": [],
            ":enable_stacktrace": ["-DENABLE_STACKTRACE", "-no-pie"],
        }),
        **kwargs
    )