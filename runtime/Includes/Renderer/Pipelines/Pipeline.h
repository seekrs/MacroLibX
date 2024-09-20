#ifndef __MLX_PIPELINE__
#define __MLX_PIPELINE__

namespace mlx
{
	class Pipeline
	{
		public:
			Pipeline() = default;

			inline virtual bool BindPipeline(VkCommandBuffer command_buffer) noexcept { RenderCore::Get().vkCmdBindPipeline(command_buffer, GetPipelineBindPoint(), GetPipeline()); return true; }
			inline virtual void EndPipeline([[maybe_unused]] VkCommandBuffer command_buffer) noexcept {}

			virtual VkPipeline GetPipeline() const = 0;
			virtual VkPipelineLayout GetPipelineLayout() const = 0;
			virtual VkPipelineBindPoint GetPipelineBindPoint() const = 0;

			virtual ~Pipeline() = default;
	};
}

#endif
