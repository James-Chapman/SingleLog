// Copyright(c) 2016-2024, James Chapman
//
// Use of this source code is governed by a BSD -
// style license that can be found in the LICENSE file or
// at https://choosealicense.com/licenses/bsd-3-clause/

#include <thread>

namespace Uplinkzero
{

struct Foo
{
    Foo();
    std::jthread m_thread;
};

} // namespace Uplinkzero