#!/bin/bash

# Update volk
rm ../third_party/volk.*
wget https://api.github.com/repos/zeux/volk/zipball/1.3.270 -O volk.zip
unzip -o volk.zip -d ../third_party/
mv ../third_party/zeux-volk*/volk.h ../third_party
mv ../third_party/zeux-volk*/volk.c ../third_party
rm -rf ../third_party/zeux-volk*
rm volk.zip

# Update Vulkan headers
rm -rf ../third_party/vulkan
wget https://github.com/KhronosGroup/Vulkan-Headers/archive/main.zip -O vulkan-headers.zip
unzip -o vulkan-headers.zip -d ../third_party/
mv ../third_party/Vulkan-Headers-main/include/vulkan ../third_party/
mv ../third_party/Vulkan-Headers-main/include/vk_video ../third_party/
rm -rf ../third_party/Vulkan-Headers-main
rm vulkan-headers.zip
