# QTDeepSudoku

## Prerequisite
Currently only tested with **arm64-v8a** on archlinux with x86-64

>- Install [Qt >= 6.3.0 for Android](https://doc-snapshots.qt.io/qt6-dev/android-building.html)
>
>- Install Android SDK and NDK 
>
>- on linux create a udev rule for your mobile device
 ```shell
    
    echo "SUBSYSTEM==\"usb\", ATTR{idVendor}==\"<yourIDVendor>\", ATTR{idProduct}==\"<yourIDProduct>\", MODE=\"0666\", GROUP=\"plugdev\""  | sudo tee -a /etc/udev/rules.d/51.android.rules
    # add user to plugdev group
    usermod -a -G plugdev <user>
```

## Building

Clone this repository : 

```git clone --recursive https://github.com/DeepSudoku/QtSudoku.git ```

In the root CMakeLists.txt file :
- specify paths for ndk , qt for android
- set QT_Host_Path for Crosscompiling
- specify abi and api level

## Running

To bundle an apk file use the **build_android.sh** script and specify the build directory.
To deploy the app to your mobile device use the script **deploy_android.sh** and specify the
ID of your android device.
