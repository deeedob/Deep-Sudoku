## Deploy to Android device

Applications can be packaged in two ways : 
- APK (Application Package)
- AAB (Android App Bundle)
Both are zip files. But *APK* files can be deployed and executed!
*AAB* is interpreted by the Google Play Store to generate different APK device architecture.

> In either case, the application bundle is generated from a specific directory structure that contains the .so file of your code, as well as all Qt's dependencies needed by your application. In addition, any .jar files, Java code, assets, resources and .xml files are included.

-G "Unix Makefiles"
-DCMAKE_TOOLCHAIN_FILE=/home/dennis/Android/android-sdk/ndk/22.1.7171670/build/cmake/android.toolchain.cmake
-DQT_ANDROID_QT_ROOT=/home/dennis/Qt/6.3.0/android_arm64_v8a
-DQT_ANDROID_SDK_ROOT=/home/dennis/Android/android-sdk
-DQT_ANDROID_NDK_ROOT=/home/dennis/Android/android-sdk/ndk/22.1.7171670
-DANDROID_ABI=arm64-v8a
-DQT_HOST_PATH=/usr
-DQT_ANDROIDDEPLOYQT=/usr/lib/qt6/bin
-DANDROID_PLATFORM_LEVEL=31


