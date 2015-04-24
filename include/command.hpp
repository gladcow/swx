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

#ifndef GUARD_command_hpp_08439188501070807
#define GUARD_command_hpp_08439188501070807

#include "config.hpp"
#include "help_line.hpp"
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

namespace swx
{

/**
 * Represents a command that processes arguments received from
 * the command line.
 *
 * To implement a specific command, inherit from this class and implement
 * its virtual functions.
 */
class Command
{
// nested types and typedefs
protected:
    typedef std::vector<std::string> Arguments;
    typedef std::vector<std::string> ErrorMessages;
    typedef std::set<char> Flags;

protected:
    class ParsedArguments
    {
    public:
        friend class Command;
    private:
        ParsedArguments
        (   std::vector<std::string> const& p_raw_args,
            bool p_recognize_double_dash,
            bool p_accept_ordinary_args
        );
    public:
        std::vector<std::string> const& ordinary_args() const;
        Flags const& flags() const;
    private:
        Flags m_flags;
        std::vector<std::string> m_ordinary_args;
    };

// special member functions
protected:
    Command
    (   std::string const& p_command_word,
        std::vector<std::string> const& p_aliases,
        std::string const& p_usage_summary,
        std::vector<HelpLine> const& p_help_lines,
        bool p_accept_ordinary_args = true
    );
public:
    Command(Command const& rhs) = delete;
    Command(Command&& rhs) = delete;
    Command& operator=(Command const& rhs) = delete;
    Command& operator=(Command&& rhs) = delete;
    virtual ~Command();

// ordinary member functions
protected:
    void add_boolean_option(char p_flag, std::string const& p_description);
public:

    /**
     * @returns "Miscellaneous" unless do_get_category() is overridden.
     */
    std::string category() const;

    bool has_boolean_option(char p_flag) const;

    int process
    (   Config const& p_config,
        Arguments const& p_args,
        std::ostream& p_ordinary_ostream,
        std::ostream& p_error_ostream
    );

    std::string usage_summary() const;
    std::string usage_descriptor() const;
    std::string const& command_word() const;
    std::vector<std::string> const& aliases() const;

// virtual functions
private:
    virtual std::string do_get_category() const;

    virtual ErrorMessages do_process
    (   Config const& p_config,
        ParsedArguments const& p_args,
        std::ostream& p_ordinary_ostream
    ) = 0;

    /**
     * If you use placeholders in a derived class, override this to
     * return \e true, so that the help string is modified accordingly.
     */
    virtual bool does_support_placeholders() const;

// member variables
private:
    bool m_accept_ordinary_args;
    std::string const m_command_word;
    std::string const m_usage_summary;
    std::vector<std::string> const m_aliases;
    std::vector<HelpLine> const m_help_lines;
    std::map<char, std::string> m_boolean_options;

};  // class Command

}  // namespace swx

#endif  // GUARD_command_hpp_08439188501070807
