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

#include "yesterday_command.hpp"
#include "today_command.hpp"
#include "command.hpp"
#include "help_line.hpp"
#include "reporting_command.hpp"
#include "time_point.hpp"
#include <ostream>
#include <string>
#include <vector>

using std::ostream;
using std::string;
using std::vector;

namespace swx
{

YesterdayCommand::YesterdayCommand
(	string const& p_command_word,
	vector<string> const& p_aliases,
	TimeLog& p_time_log
):
	ReportingCommand
	(	p_command_word,
		p_aliases,
		"Print summary of yesterday's activities",
		vector<HelpLine>
		{	HelpLine("Print summary of time spent of all activities yesterday"),
			HelpLine
			(	"Print summary of time spent on ACTIVITY yesterday",
				"ACTIVITY"
			)
		},
		p_time_log
	)
{
}

YesterdayCommand::~YesterdayCommand()
{
}

Command::ErrorMessages
YesterdayCommand::do_process
(	ParsedArguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	ErrorMessages ret;
	TimePoint const n = now();
	auto const b = day_begin(n, -1);
	auto const e = day_end(n, -1);
	print_report
	(	p_ordinary_ostream,
		p_args.single_character_flags(),
		p_args.ordinary_args(),
		&b,
		&e
	);
	return ret;
}

}  // namespace swx