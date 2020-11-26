#include "SyncQueue.h"

#include <thread>
#include <atomic>

#define MAX_READERS 100
#define MAX_WRITERS 100

std::atomic<int> count = MAX_READERS + MAX_WRITERS;

static void PrintException(const std::exception& ex)
{
	std::stringstream str;
	str << ex.what() << std::endl;
	std::cout << str.str();
}


int StartWorkers()
{
	SyncQueue<int> queue;
	
	std::thread writers([&queue]
		{
			for (size_t i = 0; i < MAX_WRITERS; ++i)
			{
				try 
				{
					std::thread([&queue, i]
						{
							try
							{
								queue.Push(i);

							}
							catch (const std::exception& ex)
							{
								PrintException(ex);
							}
							--count;
						}
					).detach();
				}
				catch(const std::exception& ex)
				{
					--count;
					PrintException(ex);
				}
			}
		});
	
	std::thread readers([&queue]
		{
			for (size_t i = 0; i < MAX_READERS; ++i)
			{
				try 
				{
					std::thread([&queue]
						{
							try
							{
								queue.Pop();
							}
							catch (const std::exception& ex)
							{
								PrintException(ex);
							}
							--count;

						}).detach();
				}
				catch(const std::exception& ex)
				{
					PrintException(ex);
					--count;
				}
			}
		});

	
	while(count != 0)
	{
		Sleep(100);
	}
	
	readers.join();
	writers.join();

	return 0;
}


int main()
{
	while (1)
	{
		StartWorkers();
		count = MAX_READERS + MAX_WRITERS;
		Sleep(10);
	}
}