function (build_external_project target file_name)

    set(CMAKELIST_CONTENT "
        cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})
        project(build_external_project)
        include(ExternalProject)
        ExternalProject_add(${target}
            URL \"${file_name}\"
            URL_MD5 \${FILE_HASH}
            CMAKE_GENERATOR \"${CMAKE_GENERATOR}\"
            CMAKE_GENERATOR_PLATFORM \"${CMAKE_GENERATOR_PLATFORM}\"
            CMAKE_GENERATOR_TOOLSET \"${CMAKE_GENERATOR_TOOLSET}\"
            CMAKE_GENERATOR_INSTANCE \"${CMAKE_GENERATOR_INSTANCE}\"
            CMAKE_ARGS ${ARGN})
        add_custom_target(build_external_project)
        add_dependencies(build_external_project ${target})
    ")

    set(TARGET_DIR "${CMAKE_CURRENT_BINARY_DIR}/ExternalProjects/${target}")

    file(WRITE "${TARGET_DIR}/CMakeLists.txt" "${CMAKELIST_CONTENT}")

    file(MAKE_DIRECTORY "${TARGET_DIR}" "${TARGET_DIR}/build")

    execute_process(COMMAND ${CMAKE_COMMAND}
            -G "${CMAKE_GENERATOR}"
            -A "${CMAKE_GENERATOR_PLATFORM}"
            -T "${CMAKE_GENERATOR_TOOLSET}"
            ..
            WORKING_DIRECTORY "${TARGET_DIR}/build")

    execute_process(COMMAND ${CMAKE_COMMAND}
            --build .
            --config ${CMAKE_BUILD_TYPE}
            WORKING_DIRECTORY "${TARGET_DIR}/build")

endfunction()

