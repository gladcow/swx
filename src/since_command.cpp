/*
 * Copyright 2014 Matthew Harvey
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "since_command.hpp"
#include "command.hpp"
#include "help_line.hpp"
#include "string_utilities.hpp"
#include "time_log.hpp"
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using std::ostream;
using std::ostringstream;
using std::runtime_error;
using std::string;
using std::unique_ptr;
using std::vector;

namespace swx
{

SinceCommand::SinceCommand
(	string const& p_command_word,
	vector<string> const& p_aliases,
	TimeLog& p_time_log
):
	Command
	(	p_command_word,
		p_aliases,
		vector<HelpLine>
		{	HelpLine
			(	"Print summary of time spent on all activities since "
					"TIMESTAMP",
				"TIMESTAMP"
			),
			HelpLine
			(	"Print summary of time spent on ACTIVITY since TIMESTAMP",
				"TIMESTAMP ACTIVITY"
			)
		}
	),
	m_time_log(p_time_log)
{
}

SinceCommand::~SinceCommand()
{
}

Command::ErrorMessages
SinceCommand::do_process
(	Arguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	ErrorMessages ret;
	if (p_args.empty())
	{
		ret.push_back("Too few arguments passed to this subcommand.");
	}
	else
	{
		unique_ptr<TimePoint> time_point_ptr;
		try
		{
			time_point_ptr.reset
			(	new TimePoint(time_stamp_to_point(p_args[0]))
			);
		}
		catch (runtime_error&)
		{
			ostringstream oss;
			oss << "Could not parse timestamp: " << p_args[0];
			ret.push_back(oss.str());
		}
		if (p_args.size() == 1)
		{
			p_ordinary_ostream <<
				m_time_log.get_stints(nullptr, time_point_ptr.get());
		}
		else
		{
			string const activity = squish(p_args.begin() + 1, p_args.end());
			p_ordinary_ostream <<
				m_time_log.get_stints(&activity, time_point_ptr.get());
		}
	}
	return ret;
}

}  // namespace swx