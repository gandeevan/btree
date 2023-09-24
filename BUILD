load("//:defines.bzl", "custom_cc_library", "custom_cc_test")

config_setting(
    name = "enable_stacktrace",
    values = {"define": "enable_stacktrace=true"},
)

custom_cc_library(
    name="btree", 
    srcs=glob(["src/**/*.cpp"]),
    hdrs=glob(["src/**/*.hpp"]),
    deps=[
        "@cpputilities//:logger",
        "@cpputilities//:utilities",
    ],
)

custom_cc_test(
    name="btree_test", 
    srcs=["tests/btree_test.cpp"], 
    deps=["@com_google_googletest//:gtest", "//:btree"],
)

custom_cc_test(
    name="internal_node_test", 
    srcs=["tests/internal_node_test.cpp"], 
    deps=["@com_google_googletest//:gtest", "//:btree"],
)

custom_cc_test(
    name="sorted_array_test", 
    srcs=["tests/sorted_array_test.cpp"], 
    deps=["@com_google_googletest//:gtest", "//:btree"],
)