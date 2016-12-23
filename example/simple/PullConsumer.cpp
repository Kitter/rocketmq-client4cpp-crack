// PullConsumer.cpp : 定义控制台应用程序的入口点。
//

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <map>
#include <set>

#include <rocketmq/OffsetStore.h>
#include <rocketmq/DefaultMQPullConsumer.h>
#include <rocketmq/Message.h>
#include <rocketmq/MessageExt.h>
#include <rocketmq/MessageQueue.h>
#include <rocketmq/PullResult.h>
#include <rocketmq/MQClientException.h>

std::map<MessageQueue, long long> offsetTable;

void putMessageQueueOffset(const MessageQueue& mq, long long offset)
{
	offsetTable[mq]=offset;
}

long long getMessageQueueOffset(const MessageQueue& mq) 
{
	std::map<MessageQueue, long long>::iterator it = offsetTable.find(mq);

	if (it != offsetTable.end())
	{
		return it->second;
	}

	return 0;
}

void PrintResult(PullResult& result)
{
	std::cout<<"[pullStatus="<<result.pullStatus
		<<"][nextBeginOffset="<<result.nextBeginOffset
		<<"][minOffset="<<result.minOffset
		<<"][maxOffset="<<result.maxOffset
		<<"]"<<std::endl;

	std::list<MessageExt*>::iterator it = result.msgFoundList.begin();

	for (;it!=result.msgFoundList.end();it++)
	{
		MessageExt* me = *it;
		std::string str;
		str.assign(me->getBody(),me->getBodyLen());
		std::cout<<str<<std::endl;
	}
}

int main(int argc, char* argv[])
{
	DefaultMQPullConsumer consumer("consumer");
    consumer.setNamesrvAddr("10.255.209.167:9876");
    consumer.setMessageModel(CLUSTERING);
	consumer.start();

    OffsetStore* offsetStore = consumer.getOffsetStore();
	std::set<MessageQueue>* mqs = consumer.fetchSubscribeMessageQueues("TopicTest");
	std::set<MessageQueue>::iterator it = mqs->begin();

	for (;it!=mqs->end();it++)
	{
		MessageQueue mq = *it;
		bool noNewMsg = false;
        long offset = consumer.fetchConsumeOffset(mq, true);
		while (!noNewMsg)
		{
			try
			{
				PullResult* pullResult = consumer.pull(mq, "", offset, 32);
                offset = pullResult->nextBeginOffset;
                std::cout <<offset << std::endl;
				switch (pullResult->pullStatus)
				{
				case FOUND:
					break;
				case NO_MATCHED_MSG:
					break;
				case NO_NEW_MSG:
					noNewMsg = true;
					break;
				case OFFSET_ILLEGAL:
					break;
				default:
					break;
				}

				delete pullResult;
			}
			catch (MQClientException& e)
			{
				std::cout<<e<<std::endl;
			}
		}

        offsetStore-> updateOffset(mq, offset,false);
	}

    offsetStore->persistAll(*mqs);

	consumer.shutdown();
    delete mqs;
	return 0;
}
