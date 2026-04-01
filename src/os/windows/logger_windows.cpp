/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "log/logger_output.h"

#include <chrono>
#include <cstdarg>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>

#include <core/namespace.h>

#include "log/logger.h"

CORE_BEGIN_NAMESPACE()
using BASE_NS::string_view;

class LogcatOutput final : public Logger::IOutput {
public:
    void Write(
        ILogger::LogLevel logLevel, const string_view filename, int linenumber, const string_view message) override {}

protected:
    void Destroy() override
    {
        delete this;
    }
};

ILogger::IOutput::Ptr CreateLoggerConsoleOutput()
{
    return ILogger::IOutput::Ptr { new LogcatOutput };
}

ILogger::IOutput::Ptr CreateLoggerDebugOutput()
{
    return {};
}
CORE_END_NAMESPACE()
