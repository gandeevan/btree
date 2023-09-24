# BTree
A BTree implementation in C++. 

Features:
- [x] Insert
- [x] Delete
- [x] Search
- [] Range search
- [] Multi-threaded
- [] Disk based



## Build
```bash
bazel build //:btree 
```

## Test
Run all tests
```bash
bazel test //:btree_test 
```

Enable stack traces (if the compiler supports it). The implementation use the std::stacktrace which is a c++23 feature.
```bash
bazel test //:btree_test  --define enable_stacktrace=true --dynamic_mode=off  
```