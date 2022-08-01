set( opencv_CMake_args
     -G "Ninja"
     -DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_INSTALL_DIR}/opencv
     -DBUILD_SHARED_LIBS=OFF # build static lib
     -DCMAKE_BUILD_TYPE=Release
     -DENABLE_PROFILING=ON
     -DOPENCV_GENERATE_PKGCONFIG=OFF
     -DBUILD_TESTS=OFF
     -DBUILD_PERF_TESTS=OFF
     -DBUILD_EXAMPLES=OFF
     -DBUILD_opencv_apps=OFF
     -DINSTALL_PYTHON_EXAMPLES=OFF
     -DINSTALL_C_EXAMPLES=OFF
     )
if( UNIX )
	set( opencv_CMake_args ${opencv_CMake_args} -DCMAKE_CXX_FLAGS=-fPIC )
endif()