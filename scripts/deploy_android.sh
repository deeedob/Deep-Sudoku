#!/bin/bash

PROJECT_NAME="QSudoku"

echo "====================[ DEPLOY| ${PROJECT_NAME} | ARM ]=================================="

# get binary directory
if [ -z "$1" ]; then
    echo "No argument supplied"
    echo "Usage: ./deploy_android.sh <SettingsDir> <androiddeployqt>"
    exit 1
fi

# get binary directory
if [ -z "$2" ]; then
    echo "No argument supplied"
    echo "Usage: ./deploy_android.sh <SettingsDir> <androiddeployqt>"
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


# execute android deployment
echo ">>>> Starting deployment"
echo ""

$2 --verbose --output "$OUTPUT_DIR" --no-build --input "$ANDROID_DEPLOYSETTINGS" --gradle --install