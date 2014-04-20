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

#include "command.hpp"
#include "info.hpp"
#include "stream_flag_guard.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using std::endl;
using std::left;
using std::ostream;
using std::ostringstream;
using std::setw;
using std::string;
using std::vector;

namespace swx
{

Command::HelpLine::HelpLine
(	string const& p_args_descriptor,
	string const& p_usage_descriptor
):
	args_descriptor(p_args_descriptor),
	usage_descriptor(p_usage_descriptor)
{
}

Command::Command
(	string const& p_command_word,
	vector<string> const& p_aliases
):
	m_command_word(p_command_word),
	m_aliases(p_aliases)
{
}

Command::~Command()
{
}

int
Command::process
(	vector<string> const& p_args,
	ostream& p_ordinary_ostream,
	ostream& p_error_ostream
)
{
	auto const error_messages = do_process(p_args, p_ordinary_ostream);
	for (auto const& message: error_messages)
	{
		p_error_ostream << message << endl;
	}
	if (error_messages.empty())
	{
		return 0;
	}
	assert (error_messages.size() > 0);
	return 1;
}

string
Command::usage_descriptor() const
{
	// TODO LOW PRIORITY This should handle wrapping of the right-hand cell
	// to a reasonable width if necessary.

	auto const help_lines = do_get_help_lines();
	typedef string::size_type ColWidth;
	ColWidth command_word_length = m_command_word.length();
	ColWidth left_col_width = command_word_length;
	for (auto const& line: help_lines)
	{
		ColWidth const left_cell_width =
			line.args_descriptor.length() + command_word_length;
		left_col_width =
		(	(left_cell_width > left_col_width)?
			left_cell_width:
			left_col_width
		);
	}
	left_col_width +=
		Info::application_name().length() + 1 + m_command_word.length() + 2;
	ostringstream oss;
	for (auto const& line: help_lines)
	{
		StreamFlagGuard guard(oss);
		oss << setw(left_col_width)
		    << left
		    << Info::application_name() + ' ' + m_command_word + ' ' +
			     line.args_descriptor
		    << "  ";
		guard.reset();
		oss << line.usage_descriptor
		    << '\n';
	}
	if (!m_aliases.empty())
	{
		oss << "\nAliased as: ";
		auto it = m_aliases.begin();
		oss << *it;
		for (++it; it != m_aliases.end(); ++it)
		{
			oss << ", " << *it;	
		}
	}
	return oss.str();
}

string const&
Command::command_word() const
{
	return m_command_word;
}

vector<string> const&
Command::aliases() const
{
	return m_aliases;
}

}  // namespace swx