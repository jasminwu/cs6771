cxx_library(
   TARGET test_main
   FILENAME test_main.cpp
   LINK Catch2::Catch2
)

cxx_test(
   TARGET lambda_string
   FILENAME "lambda_string.cpp"
)

cxx_test(
   TARGET car_test
   FILENAME "car_test.cpp"
)

cxx_test(
   TARGET dict_test
   FILENAME "dict_test.cpp"
)


