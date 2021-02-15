#include "modules/video_capture/video_capture_factory.h"

#include <iostream>

int main()
{
    std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(webrtc::VideoCaptureFactory::CreateDeviceInfo());

    if (info) {
        int num_devices = info->NumberOfDevices();
        std::cout << (num_devices);
    }
    return 0;
}