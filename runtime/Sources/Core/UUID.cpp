#include <PreCompiled.h>

#include <Core/UUID.h>

namespace mlx
{
	static std::random_device random_device;
	static std::mt19937_64 engine(random_device());
	static std::uniform_int_distribution<std::uint64_t> uniform_distribution;

	UUID::UUID() : m_uuid(uniform_distribution(engine)) {}
	UUID::UUID(std::uint64_t uuid) : m_uuid(uuid) {}
}
