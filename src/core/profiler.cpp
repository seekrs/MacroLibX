/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   profiler.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 13:56:21 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/10 18:17:35 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/profiler.h>
#include <core/errors.h>
#include <iostream>

namespace mlx
{
	void Profiler::beginRuntimeSession()
	{
		std::lock_guard lock(_mutex);
		if(_runtime_session_began)
			return;
		_output_stream.open("./runtime_profile.mlx.json", std::ofstream::out | std::ofstream::trunc);

		if(_output_stream.is_open())
			writeHeader();
		else
			core::error::report(e_kind::error, "Profiler : cannot open runtime profile file");
		_runtime_session_began = true;
	}

	void Profiler::appendProfileData(ProfileResult&& result)
	{
		std::lock_guard lock(_mutex);
		auto it = _profile_data.find(result.name);
		if(it != _profile_data.end())
		{
			result.elapsed_time = (result.elapsed_time + it->second.second.elapsed_time) / it->second.first;
			_profile_data[result.name].first++;
			_profile_data[result.name].second = result;
		}
		else
			_profile_data[result.name] = std::make_pair(1, result);
	}

	void Profiler::writeProfile(const ProfileResult& result)
	{
		std::stringstream json;
		json << std::setprecision(9) << std::fixed;
		json << ",\n{\n";
		json << "\t\"type\" : \"function\"," << '\n';
		json << "\t\"name\" : \"" << result.name << "\"," << '\n';
		json << "\t\"thread id\" : " << result.thread_id << "," << '\n';
		json << "\t\"average duration\" : \"" << result.elapsed_time.count() << "ms\"\n";
		json << "}";
		_output_stream << json.str();
	}

	void Profiler::endRuntimeSession()
	{
		std::lock_guard lock(_mutex);
		if(!_runtime_session_began)
			return;
		for(auto& [_, pair] : _profile_data)
			writeProfile(pair.second);
		writeFooter();
		_output_stream.close();
		_profile_data.clear();
		_runtime_session_began = false;
	}

	Profiler::~Profiler()
	{
		if(!_runtime_session_began)
			return;
		endRuntimeSession();
	}
}
