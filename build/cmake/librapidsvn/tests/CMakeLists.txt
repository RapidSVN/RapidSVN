add_executable(librapidsvn_tests
    ${LIBRAPIDSVN_DIR}/tests/log_dlg_tests.cpp
    ${LIBRAPIDSVN_DIR}/tests/main.cpp
)

target_include_directories(librapidsvn_tests
    PRIVATE
    ${LIBRAPIDSVN_DIR}/include
    ${LIBSVNCPP_DIR}/include
    ${APR_INCLUDE_DIR}
    ${SVN_INCLUDE_DIR}
    ${WX_INCLUDE_DIRS}
)

target_compile_definitions(librapidsvn_tests
    PUBLIC
    ${WX_DEFINITIONS}
)

target_link_libraries(librapidsvn_tests
    librapidsvn1
    ${WX_LIBRARIES}
)
