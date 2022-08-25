cmake_minimum_required( VERSION 3.2 )

find_package( Qt6 COMPONENTS Core Qml Quick Multimedia Widgets REQUIRED )
find_package( OpenCV
              HINTS ${THIRD_PARTY_INSTALL_DIR}/opencv
              NO_DEFAULT_PATH
              REQUIRED )

set( FunctionalPlus_DIR ${THIRD_PARTY_INSTALL_DIR}/FunctionalPlus/lib/cmake/FunctionalPlus )
find_package(
		FunctionalPlus
		HINTS ${THIRD_PARTY_INSTALL_DIR}/FunctionalPlus
		NO_DEFAULT_PATH
		REQUIRED
)

set( Eigen3_DIR ${THIRD_PARTY_INSTALL_DIR}/eigen/share/eigen3/cmake )
find_package(
		Eigen3
		HINTS ${THIRD_PARTY_INSTALL_DIR}/eigen
		NO_DEFAULT_PATH
		REQUIRED
)

set( nlohmann_json_DIR ${THIRD_PARTY_INSTALL_DIR}/json/share/cmake/nlohmann_json )
find_package(
		nlohmann_json
		HINTS ${THIRD_PARTY_INSTALL_DIR}/json
		NO_DEFAULT_PATH
		REQUIRED
)

build_external_project(
		Benchmark
		${THIRD_PARTY_DIR}/benchmark
		-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/benchmarks
		-DBENCHMARK_DOWNLOAD_DEPENDENCIES=on
		-DCMAKE_BUILD_TYPE=Release
)

find_package(
		benchmark
		HINTS ${CMAKE_CURRENT_BINARY_DIR}/benchmarks
		REQUIRED
)

add_executable( test_opencv
                test_opencv.cpp
                ../src/img_proc/cv_segmentation.hpp
                ../src/img_proc/cv_segmentation.cpp
                ../src/img_proc/cv_img_mat.cpp
                ../src/img_proc/cv_img_mat.hpp
                )

add_executable( test_custom_hough
                test_custom_hough.cpp
                ../src/img_proc/cv_segmentation.hpp
                ../src/img_proc/cv_segmentation.cpp
                ../src/img_proc/cv_img_mat.cpp
                ../src/img_proc/cv_img_mat.hpp
                ../src/img_proc/cv_hough_finder.cpp
                ../src/img_proc/cv_hough_finder.hpp
                )

add_executable( test_predict_tensor
                test_predict_tensor.cpp
                ../src/img_proc/cv_segmentation.cpp
                ../src/img_proc/cv_segmentation.hpp
                ../src/img_proc/cv_img_mat.cpp
                ../src/img_proc/cv_img_mat.hpp
                ../src/deep_solver.cpp
                ../src/deep_solver.hpp
                )

add_executable( test_solver
                test_solver.cpp
                ../src/img_proc/cv_segmentation.cpp
                ../src/img_proc/cv_segmentation.hpp
                ../src/img_proc/cv_img_mat.cpp
                ../src/img_proc/cv_img_mat.hpp
                ../src/deep_solver.cpp
                ../src/deep_solver.hpp
                )

add_executable( BM_opencv
                BM_opencv.cpp
                ../src/img_proc/cv_segmentation.hpp
                ../src/img_proc/cv_segmentation.cpp
                ../src/img_proc/cv_img_mat.cpp
                ../src/img_proc/cv_img_mat.hpp
                )

target_link_libraries( test_opencv
                       PRIVATE
                       Qt6::Core
                       Qt6::Multimedia
                       ${OpenCV_LIBS}
                       )
target_link_libraries( BM_opencv
                       PRIVATE
                       Qt6::Core
                       Qt6::Multimedia
                       ${OpenCV_LIBS}
                       benchmark::benchmark
                       )

target_link_libraries( test_custom_hough
                       PRIVATE
                       Qt6::Core
                       Qt6::Multimedia
                       ${OpenCV_LIBS}
                       )

target_link_libraries( test_predict_tensor
                       PRIVATE
                       Qt6::Core
                       Qt6::Multimedia
                       ${OpenCV_LIBS}
                       fdeep
                       )

target_link_libraries( test_solver
                       PRIVATE
                       Qt6::Core
                       Qt6::Multimedia
                       ${OpenCV_LIBS}
                       fdeep
                       )