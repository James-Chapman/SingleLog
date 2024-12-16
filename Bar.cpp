// Copyright(c) 2016-2024, James Chapman
//
// Use of this source code is governed by a BSD -
// style license that can be found in the LICENSE file or
// at https://choosealicense.com/licenses/bsd-3-clause/

#include "Bar.hpp"
#include "singlelog.hpp"

namespace Uplinkzero
{

using namespace std::chrono_literals;

Bar::Bar() : m_thread{[]() {
    for (int i = 0; i < 10; ++i) {
        LOG_INFO("Bar");
        std::this_thread::sleep_for(100ms);
    }
}}
{}

} // namespace Uplinkzero