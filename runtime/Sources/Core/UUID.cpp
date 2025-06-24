#include <PreCompiled.h>

#include <Core/UUID.h>

namespace mlx
{
	static std::random_device random_device;
	static std::mt19937_64 engine(random_device());
	static std::uniform_int_distribution<std::uint64_t> uniform_distribution;
	static std::unordered_set<std::uint64_t> registry;

	UUID::UUID()
	{
		do
		{
			m_uuid = uniform_distribution(engine);
		} while(registry.contains(m_uuid));
		registry.emplace(m_uuid);
	}
}
