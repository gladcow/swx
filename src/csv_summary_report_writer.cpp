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

#include "csv_summary_report_writer.hpp"
#include "activity_stats.hpp"
#include "csv_utilities.hpp"
#include "stint.hpp"
#include "summary_report_writer.hpp"
#include "time_point.hpp"
#include <map>
#include <ostream>
#include <string>
#include <vector>

using std::map;
using std::ostream;
using std::string;
using std::vector;

namespace swx
{

CsvSummaryReportWriter::CsvSummaryReportWriter
(   vector<Stint> const& p_stints,
    Options const& p_options,
    bool p_include_beginning,
    bool p_include_ending,
    bool p_succinct
):
    SummaryReportWriter(p_stints, p_options),
    m_include_beginning(p_include_beginning),
    m_include_ending(p_include_ending),
    m_succinct(p_succinct)
{
}

CsvSummaryReportWriter::~CsvSummaryReportWriter() = default;

void
CsvSummaryReportWriter::do_write_summary
(   ostream& p_os,
    map<string, ActivityStats> const& p_activity_stats_map
)
{
    if (m_succinct)
    {
        ActivityStats total_info;
        for (auto const& pair: p_activity_stats_map) total_info += pair.second;
        if (m_include_beginning && m_include_ending)
        {
            output_csv_row
            (   p_os,
                seconds_to_rounded_hours(total_info.seconds),
                time_point_to_stamp(total_info.beginning, time_format(), formatted_buf_len()),
                time_point_to_stamp(total_info.ending, time_format(), formatted_buf_len())
            );
        }
        else if (m_include_beginning)
        {
            output_csv_row
            (   p_os,
                seconds_to_rounded_hours(total_info.seconds),
                time_point_to_stamp(total_info.beginning, time_format(), formatted_buf_len())
            );
        }
        else if (m_include_ending)
        {
            output_csv_row
            (   p_os,
                seconds_to_rounded_hours(total_info.seconds),
                time_point_to_stamp(total_info.ending, time_format(), formatted_buf_len())
            );
        }
        else
        {
            output_csv_row
            (   p_os,
                seconds_to_rounded_hours(total_info.seconds)
            );
        }
        return;
    }
    for (auto const& pair: p_activity_stats_map)
    {
        auto const& activity = pair.first;
        auto const& info = pair.second;
        if (m_include_beginning && m_include_ending)
        {
            output_csv_row
            (   p_os,
                activity,
                seconds_to_rounded_hours(info.seconds),
                time_point_to_stamp(info.beginning, time_format(), formatted_buf_len()),
                time_point_to_stamp(info.ending, time_format(), formatted_buf_len())
            );
        }
        else if (m_include_beginning)
        {
            output_csv_row
            (   p_os,
                activity,
                seconds_to_rounded_hours(info.seconds),
                time_point_to_stamp(info.beginning, time_format(), formatted_buf_len())
            );
        }
        else if (m_include_ending)
        {
            output_csv_row
            (   p_os,
                activity,
                seconds_to_rounded_hours(info.seconds),
                time_point_to_stamp(info.ending, time_format(), formatted_buf_len())
            );
        }
        else
        {
            output_csv_row
            (   p_os,
                activity,
                seconds_to_rounded_hours(info.seconds)
            );
        }
    }
    return;
}

}  // namespace swx
