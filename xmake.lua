--------------------------------------------------------------------------------
--                                                                            --
--                                                        :::      ::::::::   --
--   xmake.lua                                          :+:      :+:    :+:   --
--                                                    +:+ +:+         +:+     --
--   By: kbz_8 </var/spool/mail/kbz_8>              +#+  +:+       +#+        --
--                                                +#+#+#+#+#+   +#+           --
--   Created: 2023/12/07 15:21:38 by kbz_8             #+#    #+#             --
--   Updated: 2023/12/07 15:21:38 by kbz_8            ###   ########.fr       --
--                                                                            --
--------------------------------------------------------------------------------

-- Global settings

add_requires("libsdl", "vulkan-headers")

add_rules("mode.debug", "mode.release")
set_languages("cxx17")

set_objectdir("objs/xmake/$(os)_$(arch)")
set_targetdir("./")

set_optimize("fastest")

target("libmlx")
	set_default(true)
	set_license("MIT")
    set_kind("shared")
	add_includedirs("includes", "srcs", "third_party")
	
    add_files("srcs/**.cpp")

	add_packages("libsdl", "vulkan-headers")

	if is_mode("debug") then
		add_defines("DEBUG")
	end
target_end() -- optional but I think the code is cleaner with this -- optional but I think the code is cleaner with this
