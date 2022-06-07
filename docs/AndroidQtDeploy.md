## Deploy to Android device

Applications can be packaged in two ways : 
- APK (Application Package)
- AAB (Android App Bundle)
Both are zip files. But *APK* files can be deployed and executed!
*AAB* is interpreted by the Google Play Store to generate different APK device architecture.

> In either case, the application bundle is generated from a specific directory structure that contains the .so file of your code, as well as all Qt's dependencies needed by your application. In addition, any .jar files, Java code, assets, resources and .xml files are included.

-G
Ninja
-DCMAKE_TOOLCHAIN_FILE=/home/dennis/Android/android-sdk/ndk/22.1.7171670/build/cmake/android.toolchain.cmake
-DANDROID_ABI=arm64-v8a
-DANDROID_SDK_ROOT=/home/dennis/Android/android-sdk
-DANDROID_NDK=/home/dennis/Android/android-sdk/ndk/22.1.7171670
-DANDROID_QT_ROOT=/opt/android-qt6-aarch64
-DQT_HOST_PATH=/home/dennis/Qt/6.3.0/gcc_64

