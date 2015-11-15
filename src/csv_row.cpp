/*
 * Copyright 2015 Matthew Harvey
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

#include "csv_row.hpp"
#include <iostream>
#include <ostream>
#include <string>

using std::ostream;
using std::endl;
using std::string;

namespace swx
{

CsvRow::CsvRow()
{
    enable_exceptions(m_contents);
}

string
CsvRow::str() const
{
    return m_contents.str();
}

ostream&
operator<<(ostream& p_os, CsvRow const& p_csv_row)
{
    return p_os << p_csv_row.str() << std::endl;
}

}  // namespace swx
