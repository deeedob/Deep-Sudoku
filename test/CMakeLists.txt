cmake_minimum_required( VERSION 3.2 )

find_package( Qt6 COMPONENTS Core Multimedia REQUIRED )
find_package( OpenCV
              HINTS ${THIRD_PARTY_INSTALL_DIR}/opencv
              NO_DEFAULT_PATH
              REQUIRED )

add_executable( test_opencv
                test_opencv.cpp
                ../src/cv_segmentation.hpp
                ../src/cv_segmentation.cpp
                ../src/cv_img_mat.cpp
                ../src/cv_img_mat.hpp
                )
target_link_libraries( test_opencv
                       PRIVATE
                       Qt6::Core
                       Qt6::Multimedia
                       ${OpenCV_LIBS}
                       )