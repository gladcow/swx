/*
 * Copyright 2014 Matthew Harvey
 */

#include "command_router.hpp"
#include "activity_command_processor.hpp"
#include "command_processor.hpp"
#include "switch_command_processor.hpp"
#include "time_log.hpp"
#include "version_command_processor.hpp"
#include <cassert>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;
using std::string;
using std::vector;

namespace swx
{

CommandRouter::CommandRouter(TimeLog& p_time_log): m_time_log(p_time_log)
{
	populate_command_processor_map();	
#	ifndef NDEBUG
		for (auto const& entry: m_command_processor_map)
		{
			assert (entry.second);
		}
#	endif
}

void
CommandRouter::populate_command_processor_map()
{
	CommandProcessorPtr version_processor(new VersionCommandProcessor);
	create_command("version", version_processor);	

	CommandProcessorPtr switch_processor
	(	new SwitchCommandProcessor(m_time_log)
	);
	create_command("switch", switch_processor);

	CommandProcessorPtr activity_processor
	(	new ActivityCommandProcessor(m_time_log)
	);
	create_command("print", activity_processor);
		
	return;
}

int
CommandRouter::process_command
(	string const& p_command,
	vector<string> const& p_args
) const
{
	auto const it = m_command_processor_map.find(p_command);
	if (it == m_command_processor_map.end())
	{
		process_unrecognized_command(p_command);
		return 1;
	}
	else
	{
		assert (it->second);
		return it->second->process(p_args, ordinary_ostream(), error_ostream());
	}
}

int
CommandRouter::process_unrecognized_command(string const& p_command) const
{
	error_ostream() << "Unrecognized subcommand: " << p_command << endl;
	return 1;
}

ostream&
CommandRouter::ordinary_ostream() const
{
	return cout;	
}

ostream&
CommandRouter::error_ostream() const
{
	return cerr;
}

void
CommandRouter::create_command
(	string const& p_alias,
	CommandProcessorPtr const& p_cpp
)
{
	m_command_processor_map[p_alias] = p_cpp;
	return;
}

}  // namespace swx