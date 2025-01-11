#include <PreCompiled.h>

#include <Core/Profiler.h>

namespace mlx
{
	Profiler* Profiler::s_instance = nullptr;

	void Profiler::BeginRuntimeSession()
	{
		std::lock_guard lock(m_mutex);
		if(m_runtime_session_began)
			return;
		m_output_stream.open("./runtime_profile.mlx.json", std::ofstream::out | std::ofstream::trunc);

		if(m_output_stream.is_open())
			WriteHeader();
		else
			Error("Profiler: cannot open runtime profile file");
		m_runtime_session_began = true;
	}

	void Profiler::AppendProfileData(ProfileResult&& result)
	{
		std::lock_guard lock(m_mutex);
		auto it = m_profile_data.find(result.name);
		if(it != m_profile_data.end())
		{
			result.elapsed_time = (result.elapsed_time + it->second.second.elapsed_time) / it->second.first;
			m_profile_data[result.name].first++;
			m_profile_data[result.name].second = result;
		}
		else
			m_profile_data[result.name] = std::make_pair(1, result);
	}

	void Profiler::WriteProfile(const ProfileResult& result)
	{
		std::stringstream json;
		json << std::setprecision(9) << std::fixed;
		json << ",\n{\n";
		json << "\t\"type\" : \"function\"," << '\n';
		json << "\t\"name\" : \"" << result.name << "\"," << '\n';
		json << "\t\"thread id\" : " << result.thread_id << "," << '\n';
		json << "\t\"average duration\" : \"" << result.elapsed_time.count() << "ms\"\n";
		json << "}";
		m_output_stream << json.str();
	}

	void Profiler::EndRuntimeSession()
	{
		std::lock_guard lock(m_mutex);
		if(!m_runtime_session_began)
			return;
		for(auto& [_, pair] : m_profile_data)
			WriteProfile(pair.second);
		WriteFooter();
		m_output_stream.close();
		m_profile_data.clear();
		m_runtime_session_began = false;
	}

	Profiler::~Profiler()
	{
		if(!m_runtime_session_began)
			return;
		EndRuntimeSession();
		s_instance = nullptr;
	}
}
