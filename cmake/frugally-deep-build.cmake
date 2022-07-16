# We need to build some packages before actually building frugally-deep
function(findPrinter lib)
    if (${${lib}_FOUND})
        message("${lib} FOUND @ " ${${lib}_DIR})
    else()
        message("COULD NOT FIND ${lib}!")
    endif ()
endfunction()

build_external_project(
        FunctionalPlus
        ${THIRD_PARTY_DIR}/FunctionalPlus
        -DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_INSTALL_DIR}/FunctionalPlus)

build_external_project(
        Eigen
        ${THIRD_PARTY_DIR}/eigen
        -DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_INSTALL_DIR}/eigen)

build_external_project(
        JSON
        ${THIRD_PARTY_DIR}/json
        "-DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_INSTALL_DIR}/json -DJSON_BuildTests=OFF")

find_package(
        FunctionalPlus
        HINTS ${THIRD_PARTY_INSTALL_DIR}/FunctionalPlus
        NO_DEFAULT_PATH
        REQUIRED
)

find_package(
        Eigen3
        HINTS ${THIRD_PARTY_INSTALL_DIR}/eigen
        NO_DEFAULT_PATH
        REQUIRED
)

find_package(
        nlohmann_json
        HINTS ${THIRD_PARTY_INSTALL_DIR}/json
        NO_DEFAULT_PATH
        REQUIRED
)

findPrinter(FunctionalPlus)
findPrinter(Eigen3)
findPrinter(nlohmann_json)

set(_CMAKE_PREFIX_PATH ${FunctionalPlus_DIR} ${Eigen_Dir} ${nlohmann_json_DIR})
build_external_project(
        Frugally-Deep
        ${THIRD_PARTY_DIR}/frugally-deep
        -DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_INSTALL_DIR}/frugally-deep -DCMAKE_PREFIX_PATH=${_CMAKE_PREFIX_PATH} -Dnlohmann_json_DIR=${nlohmann_json_DIR})
