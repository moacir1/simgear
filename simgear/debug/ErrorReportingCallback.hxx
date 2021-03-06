// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#pragma once

#include <functional>
#include <string>

namespace simgear {

void reportError(const std::string& msg, const std::string& more = {});

void reportFatalError(const std::string& msg, const std::string& more = {});

using ErrorReportCallback = std::function<void(const std::string& msg, const std::string& more, bool isFatal)>;

void setErrorReportCallback(ErrorReportCallback cb);

} // namespace simgear
