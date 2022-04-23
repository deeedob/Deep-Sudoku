#!/bin/bash

PROJECT_NAME="qtsudoku"
ANDROIDDEPLOYQT="$HOME/Qt/6.3.0/gcc_64/bin/androiddeployqt"

# check if QT_ANDROIDDEPLOY_PATH is a valid file
if [ -f "$ANDROIDDEPLOYQT" ]; then
echo "androiddeployqt is valid"
else
echo "androiddeployqt is not valid"
exit 1
fi

# get binary directory
if [ -z "$1" ]; then
    echo "No argument supplied"
    echo "Usage: ./deploy_android.sh <BuildPath>"
    exit 1
fi

# get path to android deployment settings
ANDROID_DEPLOYSETTINGS="${1}/android-$PROJECT_NAME-deployment-settings.json"

if [ -f "$ANDROID_DEPLOYSETTINGS" ]; then
    echo "androiddeploysettings are valid"
    echo "      "${ANDROID_DEPLOYSETTINGS}

else
    echo "${ANDROID_DEPLOYSETTINGS} not found"
    exit 1
fi

OUTPUT_DIR="$1/android-build"
ANDROID_PLATFORM="android-31"

# execute android deployment
echo "Starting  : "
echo "${ANDROIDDEPLOYQT} --input $ANDROID_DEPLOYSETTINGS --output ${OUTPUT_DIR} --android-platform $ANDROID_PLATFORM --jdk $JAVA_HOME --grade --verbose "
$ANDROIDDEPLOYQT --input "$ANDROID_DEPLOYSETTINGS" --output "$OUTPUT_DIR" --android-platform "$ANDROID_PLATFORM"  --jdk "$JAVA_HOME" --gradle --verbose