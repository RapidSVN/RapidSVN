add_executable(rapidsvn WIN32
    ${RAPIDSVN_DIR}/rapidsvn_app.cpp
    ${RAPIDSVN_DIR}/rapidsvn.rc
)

target_compile_definitions(rapidsvn
    PRIVATE
    ${WX_DEFINITIONS}
)

target_link_libraries(rapidsvn
    svncpp1
    rapidsvn1
    ${APR_LIBRARIES}
    ${APU_LIBRARIES}
    ${SVN_LIBRARY_NAMES}
    ${WX_LIBRARIES}
)
