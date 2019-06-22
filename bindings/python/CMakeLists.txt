if("$ENV{PYTHON}" STREQUAL "")
    # https://cmake.org/cmake/help/v3.12/module/FindPython.html
    find_package(Python COMPONENTS Interpreter)
else()
    set(PYTHON $ENV{PYTHON})
    message(STATUS ${PYTHON})
endif()

if("$ENV{CYTHON}" STREQUAL "")
    find_program(CYTHON
        NAMES cython cython3
    )
else()
    set(CYTHON $ENV{CYTHON})
    message(STATUS ${CYTHON})
endif()

set(EXT_FILES
        _graph_analysis.pxd
        graph_analysis.pyx
)

if(CYTHON)
    message(STATUS "Using python: ${Python_EXECUTABLE}")
    message(STATUS "Detected python version: ${Python_VERSION}")
    message(STATUS "Using cython: ${CYTHON}")

    configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in"
    "${CMAKE_CURRENT_BINARY_DIR}/setup.py")

    add_custom_target(graph_analysis_py-prepare)
    foreach(file ${EXT_FILES})
        add_custom_command(TARGET graph_analysis_py-prepare PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E
                copy ${CMAKE_CURRENT_SOURCE_DIR}/${file} ${CMAKE_CURRENT_BINARY_DIR}
        )
    endforeach()

    # Custom python target that runs 'setup.py build' to build all python
    # components.
    option(PYTHON_SUPPORT "Build and install python wrapper" ON)
    if (PYTHON_SUPPORT)
        set(SETUP_PY "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
        message(STATUS "Building python extension with setup.py: ${SETUP_PY}")

        add_custom_target(graph_analysis_py ALL DEPENDS graph_analysis graph_analysis_py-prepare
            COMMAND ${Python_EXECUTABLE} ${SETUP_PY} build_ext -i
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            )

        add_custom_target(pyext-clean
            COMMAND ${Python_EXECUTABLE} ${SETUP_PY} clean
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            )

        install(
          CODE "execute_process(
            COMMAND ${Python_EXECUTABLE} ${SETUP_PY} install --prefix=${CMAKE_INSTALL_PREFIX}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})"
        )

        # nose is the python unit testing program it will run all python unit tests
        add_test(
          NAME nosetests
          COMMAND nosetests3 ${CMAKE_SOURCE_DIR}/test/bindings/python/test.py
          WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test/bindings/python
        )
    elseif(PYTHON_SUPPORT)
        message(FATAL_ERROR "Python extension will not be built: python not found")
    endif()
else()
    message(WARNING "Python extension will not be built: cython not found")
endif()


