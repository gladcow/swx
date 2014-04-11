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

#include "time_point.hpp"
#include <chrono>
#include <ctime>
#include <stdexcept>
#include <string>

namespace chrono = std::chrono;

using std::localtime;
using std::mktime;
using std::runtime_error;
using std::string;
using std::tm;
using std::time_t;

namespace swx
{

TimePoint
now()
{
	return chrono::system_clock::now();
}

TimePoint
start_of_day(TimePoint const& p_time_point)
{
	tm time_tm = time_point_to_tm(p_time_point);
	time_tm.tm_hour = time_tm.tm_min = time_tm.tm_sec = 0;
	return tm_to_time_point(time_tm);
}

std::tm
time_point_to_tm(TimePoint const& p_time_point)
{
	time_t const time_time_t = chrono::system_clock::to_time_t(p_time_point);
	tm* const time_tm_ptr = localtime(&time_time_t);
	tm const ret = *time_tm_ptr;
	return ret;
}

TimePoint
tm_to_time_point(tm const& p_tm)
{
	tm time_tm = p_tm;
	time_t const time_time_t = mktime(&time_tm);
	return chrono::system_clock::from_time_t(time_time_t);
}

TimePoint
time_stamp_to_point(string const& p_time_stamp)
{
	// don't make this static - caused odd bug with strptime
	char const format[] = SWX_FORMAT_STRING;

	tm tm;
	bool parse_error = false;
#	ifdef __GNUC__
		if (strptime(p_time_stamp.c_str(), format, &tm) == nullptr)
		{
			parse_error = true;
		}
#	else
		stringstream ss;
		ss << p_time_stamp;
		ss >> get_time(&tm, format);
		if (!ss)
		{
			parse_error = true;
		}
#	endif
	if (parse_error)
	{
		throw runtime_error("Could not parse timestamp.");
	}
	return chrono::system_clock::from_time_t(mktime(&tm));
}

string
time_point_to_stamp(TimePoint const& p_time_point)
{
	// don't make this static - caused odd bug with strptime
	char const format[] = SWX_FORMAT_STRING;
	tm const time_tmp = time_point_to_tm(p_time_point);
	char buf[SWX_FORMATTED_BUF_LEN];
	if (strftime(buf, SWX_FORMATTED_BUF_LEN, format, &time_tmp) == 0)
	{
		throw runtime_error("Error formatting TimePoint.");
	}
	return string(buf);
}

}   // namespace swx
