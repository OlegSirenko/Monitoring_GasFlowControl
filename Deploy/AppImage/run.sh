#!/bin/sh
./linuxdeploy-x86_64.AppImage --executable ../../cmake-build-release/Monitor --appdir AppDir --desktop-file ../../resources/GasFlowControll.desktop --icon-file ../../resources/new_icon.png --output appimage
export LDNP_META_DESCRIPTION="Application for monitoring the gas flow control system"
./linuxdeploy-plugin-native_packages-x86_64.AppImage --appdir ./AppDir/ --build deb --package-version 2.0 --app-name Monitor