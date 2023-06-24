cxx_library(
   TARGET test_main
   FILENAME test_main.cpp
   LINK Catch2::Catch2
)

cxx_test(
   TARGET sort_descending_test
   FILENAME "sort_descending_test.cpp"
   LINK range-v3 sort_descending
)

cxx_test(
   TARGET is_permutation_test
   FILENAME "is_permutation_test.cpp"
   LINK is_permutation
)
