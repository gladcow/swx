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

#ifndef GUARD_resume_command_hpp_2942334922469377
#define GUARD_resume_command_hpp_2942334922469377

#include "config_fwd.hpp"
#include "recording_command.hpp"
#include "time_log.hpp"
#include <ostream>
#include <string>
#include <vector>

namespace swx
{

class ResumeCommand: public RecordingCommand
{
// special member functions
public:
    ResumeCommand
    (   std::string const& p_command_word,
        std::vector<std::string> const& p_aliases,
        TimeLog& p_time_log
    );
    ResumeCommand(ResumeCommand const& rhs) = delete;
    ResumeCommand(ResumeCommand&& rhs) = delete;
    ResumeCommand& operator=(ResumeCommand const& rhs) = delete;
    ResumeCommand& operator=(ResumeCommand&& rhs) = delete;
    virtual ~ResumeCommand();

// inherited virtual functions
private:
    virtual ErrorMessages do_process
    (   Config const& p_config,
        std::vector<std::string> const& p_ordinary_args,
        std::ostream& p_ordinary_ostream
    ) override;

// member variables
private:
    bool m_time_stamp_provided = false;
    std::string m_time_stamp;

};  // class ResumeCommand

}  // namespace swx

#endif  // GUARD_resume_command_hpp_2942334922469377
