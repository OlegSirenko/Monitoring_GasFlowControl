[![CMake on a single platform](https://github.com/OlegSirenko/Monitoring_GasFlowControll/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/OlegSirenko/Monitoring_GasFlowControll/actions/workflows/cmake-single-platform.yml)
# Monitoring for Gas Flow Control System

<img align="middle" src="resources/GasFlowControlIcon_png.png" alt="Logo" width="250" >

![The appearance of the application](resources/img_2.png)
## Overview

The Gas Flow Control System is a comprehensive solution
designed to revolutionize vacuum heat treatment technology.

This project encompasses three key components:
* a desktop application for precise hardware control,
* an embedded system for on-site operations,
* a web interface for real-time data access from any device.

Our aim is to enhance accuracy, efficiency, and user experience
in controlling gas flow, paving the way for the next generation
of vacuum heat treatment processes.
This project is currently in the preparation phase for its
upcoming release. Stay tuned for more updates!

This repository is first part of project: **Desktop application**.

## References
* As a base of application was used [Dear ImGui framework](https://github.com/ocornut/imgui)
* We lightly changed code of [jfoshea's PID-controller with auto-tuning](https://github.com/jfoshea/PID-Controller)
* For creating AppImage for Linux machines we used [LinuxDeploy utility](https://github.com/linuxdeploy/linuxdeploy)
* For creating MSI installer we used [Wix Toolset v3.14](https://github.com/wixtoolset/wix3)
* For showing plots we used [ImPlot lib for ImGui](https://github.com/epezent/implot)
* For network communication we used [Boost](https://github.com/boostorg/boost)

## How to install

### Windows machines:
- [Go to Releases tab](https://github.com/OlegSirenko/Monitoring_GasFlowControll/releases)
- Download the latest MSI installer.
- Run the installer on your machine by double-clicking it. It will install all necessary dll's and create desktop shortcut for executable. (Keep in mind application use DX11 as a system of rendering, so you will need to [install it](https://support.microsoft.com/en-au/topic/how-to-install-the-latest-version-of-directx-d1f5ffa5-dae2-246c-91b1-ee1e973ed8c2))
- There you can run monitoring application by double-clicking on it.

### Linux machines
- [Go to Releases tab](https://github.com/OlegSirenko/Monitoring_GasFlowControll/releases)
- Download the latest AppImage file.
- [Make the downloaded file executable](https://docs.appimage.org/introduction/quickstart.html)
- Run the AppImage file.

# Usage
The first time you opened the application you will see this:
![first start](resources/first_start.png)

When the Microcontroller (or multiple microcontrollers) connected to server application you will see that appeared  
![Plot Window](resources/img_1.png)


**You could chnage theme by clicking `File -> Dark` or `File -> Light` in the `change style` section.**  

![PlotWindow Dark](resources/img_2.png)

Microcontroller sends only one number as an actual Data from one sensor.

**In next iterations will be modified for Real-Time communication in interface:** 
```json
{"actual_time": {"sensor_1": "actual_data_from_sensor", "sensor_2": "actual_data_from_sensor", ...}}
```