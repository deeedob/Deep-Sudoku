set( tensorflow_CMake_args
     -G "Ninja"
     -DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_INSTALL_DIR}/tensorflow-lite
     -DCMAKE_BUILD_TYPE=Release
     -DTFLITE_ENABLE_INSTALL=ON
     -DCMAKE_FIND_PACKAGE_PREFER_CONFIG=ON
     -Dabsl_DIR=${THIRD_PARTY_INSTALL_DIR}/abseil/lib/cmake/absl
     -DEigen3_DIR=${THIRD_PARTY_INSTALL_DIR}/eigen/share/eigen3/cmake
     -DFlatbuffers_DIR=${THIRD_PARTY_INSTALL_DIR}/flatbuffers/lib/cmake/flatbuffers
     -DNEON_2_SSE_DIR=${THIRD_PARTY_INSTALL_DIR}/arm_neon/lib/cmake/NEON_2_SSE
     -Dcpuinfo_DIR=${THIRD_PARTY_INSTALL_DIR}/cpuinfo/share/cpuinfo
     -Druy_DIR=${THIRD_PARTY_INSTALL_DIR}/ruy/lib/cmake/ruy
     # General TF options
     -Dtensorflow_BUILD_SHARED_LIB=OFF
     -Dtensorflow_BUILD_ALL_KERNELS=ON
     -Dtensorflow_BUILD_CONTRIB_KERNELS=ON
     -Dtensorflow_ENABLE_GRPC_SUPPORT=OFF
     -Dtensorflow_ENABLE_SSL_SUPPORT=OFF
     -Dtensorflow_BUILD_CC_TESTS=OFF
     -Dtensorflow_BUILD_CC_EXAMPLE=OFF
     -Dtensorflow_BUILD_PYTHON_BINDINGS=OFF
     -Dtensorflow_BUILD_PYTHON_TESTS=OFF
     -Dtensorflow_OPTIMIZE_FOR_NATIVE_ARCH=OFF
     )