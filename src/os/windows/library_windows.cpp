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

#include "library_windows.h"

#pragma warning(push)
#pragma warning(disable : 5039)
#include <windows.h>
#pragma warning(pop)

#include <base/containers/iterator.h>
#include <base/containers/string.h>
#include <base/containers/string_view.h>
#include <base/namespace.h>
#include <core/log.h>
#include <core/namespace.h>

CORE_BEGIN_NAMESPACE()
using BASE_NS::string;
using BASE_NS::string_view;

LibraryWindows::LibraryWindows(const string_view filename)
{
    string tmp(filename);
	for (auto& t : tmp) {
		if (t == '/') {
			t = '\\';
		}
	}
	
	const int wStringLength = ::MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), static_cast<int>(tmp.size()), nullptr, 0);
	BASE_NS::basic_string<wchar_t> wString(static_cast<const size_t>(wStringLength), 0);
	::MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), static_cast<int>(tmp.size()), wString.data(), wStringLength);
	
    libraryHandle_ = LoadLibraryExW(wString.c_str(), nullptr, LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
    if (!libraryHandle_) {
        DWORD errorCode = GetLastError();
		CORE_LOG_E("Loading dynamic library '%s' failed: 0x%lx", tmp.c_str(), errorCode);
    }
}

LibraryWindows::~LibraryWindows()
{
    if (libraryHandle_) {
        FreeLibrary(libraryHandle_);
        libraryHandle_ = nullptr;
    }
}

IPlugin* LibraryWindows::GetPlugin() const
{
    if (!libraryHandle_) {
        return nullptr;
    }

    return reinterpret_cast<IPlugin*>(GetProcAddress(libraryHandle_, "gPluginData"));
}

void LibraryWindows::Destroy()
{
    delete this;
}

ILibrary::Ptr ILibrary::Load(const string_view filePath)
{
    return ILibrary::Ptr { new LibraryWindows(filePath) };
}

string_view ILibrary::GetFileExtension()
{
    return ".dll";
}
CORE_END_NAMESPACE()