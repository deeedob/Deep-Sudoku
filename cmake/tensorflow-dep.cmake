# tensorflow is just so fucked up to build with cmake ....... yes srsly
# what are the guys at google thinking ?!
function( findPrinter lib )
	if( ${${lib}_FOUND} )
		message( "${lib} FOUND @ " ${${lib}_DIR} )
	else()
		message( "COULD NOT FIND ${lib}!" )
	endif()
endfunction()

build_external_project(
		Abseil
		${THIRD_PARTY_DIR}/abseil-cpp
		-DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_INSTALL_DIR}/abseil
)
build_external_project(
		Eigen
		${THIRD_PARTY_DIR}/eigen
		-DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_INSTALL_DIR}/eigen
		-G "Ninja" -DCMAKE_CXX_STANDARD=17 -DBUILD_TESTING=OFF
)
build_external_project(
		Flatbuffers
		${THIRD_PARTY_DIR}/flatbuffers
		-DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_INSTALL_DIR}/flatbuffers
)
build_external_project(
		Neon2
		${THIRD_PARTY_DIR}/ARM_NEON_2_x86_SSE
		-DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_INSTALL_DIR}/arm_neon
)
build_external_project(
		Cpuinfo
		${THIRD_PARTY_DIR}/cpuinfo
		-DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_INSTALL_DIR}/cpuinfo
)
build_external_project(
		Ruy
		${THIRD_PARTY_DIR}/ruy
		-DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_INSTALL_DIR}/ruy
)
build_external_project(
		XNNPACK
		${THIRD_PARTY_DIR}/XNNPACK
		-DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_INSTALL_DIR}/xnnpack
)