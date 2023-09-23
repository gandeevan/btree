
cc_library(
    name="btree", 
    srcs=glob(["src/**/*.cpp"]),
    hdrs=glob(["src/**/*.hpp"]),
    deps=[
        "@cpputilities//:logger",
    ],
)

cc_test(
    name="btree_test", 
    srcs=["tests/btree_test.cpp"], 
    deps=["@com_google_googletest//:gtest", "//:btree"],
)

cc_test(
    name="internal_node_test", 
    srcs=["tests/internal_node_test.cpp"], 
    deps=["@com_google_googletest//:gtest", "//:btree"],
)

cc_test(
    name="sorted_array_test", 
    srcs=["tests/sorted_array_test.cpp"], 
    deps=["@com_google_googletest//:gtest", "//:btree"],
)