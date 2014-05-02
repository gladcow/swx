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

#include "help_command.hpp"
#include "help_line.hpp"
#include "command.hpp"
#include "command_manager.hpp"
#include "info.hpp"
#include <cassert>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using std::endl;
using std::ostream;
using std::ostringstream;
using std::pair;
using std::runtime_error;
using std::string;
using std::vector;

namespace swx
{

HelpCommand::HelpCommand
(	string const& p_command_word,
	vector<string> const& p_aliases,
	CommandManager const& p_command_manager
):
	Command
	(	p_command_word,
		p_aliases,
		vector<HelpLine>
		{	HelpLine("Print general help information."),
			HelpLine("Print help information for COMMAND", "COMMAND")
		}
	),
	m_command_manager(p_command_manager)
{
}

HelpCommand::~HelpCommand()
{
}

Command::ErrorMessages
HelpCommand::do_process
(	Arguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	ErrorMessages ret;
	if (p_args.empty())
	{
		vector<pair<string, vector<string>>> commands =
			m_command_manager.available_commands();
		ostringstream oss;
		for (auto const& pair: commands)
		{
			oss << pair.first;
			if (!pair.second.empty())
			{
				for (auto const& alias: pair.second)
				{
					oss << ", " << alias;
				}
			}
			oss << '\n';
		}
		p_ordinary_ostream << "Usage: "
		                   << Info::application_name() << ' '
						   << "COMMAND [ARGS..]" << endl
						   << "\nAvailable commands are:\n\n"
		                   << oss.str() << "\n"
						   << "For more information on a particular command,"
						   << " enter '"
						   << Info::application_name() << ' '
						   << command_word() << " COMMAND'." << endl;
		return ret;
	}
	else if (p_args.size() == 1)
	{
		try
		{
			p_ordinary_ostream << m_command_manager.help_information(p_args[0])
							   << endl;			
		}
		catch (runtime_error&)
		{
			ostringstream oss;
			oss << CommandManager::
					  error_message_for_unrecognized_command(p_args[0]);
			ret.push_back(oss.str());
		}
	}
	else
	{
		assert (p_args.size() > 1);
		ret.push_back("Too many arguments passed to this command.");
	}
	return ret;	
}

}  // namespace swx
