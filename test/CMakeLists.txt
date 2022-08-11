cmake_minimum_required( VERSION 3.2 )

find_package( Qt6 COMPONENTS Core Multimedia REQUIRED )
find_package( OpenCV
              HINTS ${THIRD_PARTY_INSTALL_DIR}/opencv
              NO_DEFAULT_PATH
              REQUIRED )
find_package(
		frugally-deep
		HINTS ${THIRD_PARTY_INSTALL_DIR}/frugally-deep
		NO_DEFAULT_PATH
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

add_executable( test_predictTensor
                test_predictTensor.cpp
                ../src/img_proc/cv_segmentation.cpp
                ../src/img_proc/cv_segmentation.hpp
                ../src/img_proc/cv_img_mat.cpp
                ../src/img_proc/cv_img_mat.hpp
                ../src/img_proc/deep_solver.cpp
                ../src/img_proc/deep_solver.hpp
                )
target_link_libraries( test_opencv
                       PRIVATE
                       Qt6::Core
                       Qt6::Multimedia
                       ${OpenCV_LIBS}
                       )

target_link_libraries( test_custom_hough
                       PRIVATE
                       Qt6::Core
                       Qt6::Multimedia
                       ${OpenCV_LIBS}
                       )

target_link_libraries( test_predictTensor
                       PRIVATE
                       Qt6::Core
                       Qt6::Multimedia
                       ${OpenCV_LIBS}
                       frugally-deep::fdeep
                       )