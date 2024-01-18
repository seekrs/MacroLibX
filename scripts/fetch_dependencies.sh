#!/bin/bash

# Update volk
rm -f ../third_party/volk.c
rm -f ../third_party/volk.h
tag_name=$(curl -sL https://api.github.com/repos/zeux/Volk/releases/latest | jq -r '.tag_name')
wget https://api.github.com/repos/zeux/volk/zipball/$tag_name -O volk.zip
unzip -o volk.zip -d ../third_party/
mv ../third_party/zeux-volk*/volk.h ../third_party
mv ../third_party/zeux-volk*/volk.c ../third_party
rm -rf ../third_party/zeux-volk*
rm volk.zip

# Update VMA
rm -f ../third_party/vma.h
tag_name=$(curl -sL https://api.github.com/repos/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/releases/latest | jq -r '.tag_name')
wget https://api.github.com/repos/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/zipball/$tag_name -O vma.zip
unzip -o vma.zip -d ../third_party/
mv ../third_party/GPUOpen-LibrariesAndSDKs-VulkanMemoryAllocator*/include/vk_mem_alloc.h ../third_party/vma.h
rm -rf ../third_party/GPUOpen-LibrariesAndSDKs-VulkanMemoryAllocator*
rm vma.zip

# Update Vulkan headers
rm -rf ../third_party/vulkan
rm -rf ../third_party/vk_video
wget https://github.com/KhronosGroup/Vulkan-Headers/archive/main.zip -O vulkan-headers.zip
unzip -o vulkan-headers.zip -d ../third_party/
mv ../third_party/Vulkan-Headers-main/include/vulkan ../third_party/
mv ../third_party/Vulkan-Headers-main/include/vk_video ../third_party/
rm -rf ../third_party/Vulkan-Headers-main
rm vulkan-headers.zip
