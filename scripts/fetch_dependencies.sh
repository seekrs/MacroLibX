#!/bin/bash

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

# Update KVF
rm -f ../third_party/kvf.h
git clone https://github.com/Kbz-8/KVF.git ../third_party/KVF/
mv ../third_party/KVF/kvf.h ../third_party/kvf.h
rm -rf ../third_party/KVF
