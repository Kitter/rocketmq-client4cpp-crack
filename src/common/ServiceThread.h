/**
* Copyright (C) 2013 kangliqiang ,kangliq@163.com
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

#if!defined __SERVICETHREAD_H__
#define __SERVICETHREAD_H__

#include <string>
#include "Thread.h"
#include "Monitor.h"

// 线程回收时间，默认90S
const long JoinTime = 90 * 1000;

/**
* 后台服务线程基类
*
*/
class ServiceThread :public kpr::Thread,public kpr::Monitor
{
public:
	ServiceThread(const char* name=NULL);
	virtual ~ServiceThread();

	virtual std::string  getServiceName() = 0;

	void stop();
	void wakeup();
	void waitForRunning(long interval);

protected:
	volatile bool m_notified;// 是否已经被Notify过
	volatile bool m_stoped; // 线程是否已经停止
};

#endif
