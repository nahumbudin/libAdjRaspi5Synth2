/**
* @file		safeQueues.h
*	@author		https://stackoverflow.com/a/16075550
*	@date		10-Oct-2019 (Updated)
*	@version	1.0
*
*	@brief		Thread safe queses based on std::queue, mutex and conditional-variable
*		Use: SafeQueue<message> qname;
*/
/**
			Timeout is not working - returns immidiatlly. For blocking use waifor_msec = 0.
*/

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template <class T>
class SafeQueue
{
public:
	SafeQueue(void)
		: q()	// The Q
		, m()	// Mutex
		, c()	// condition_variable
	{}

	~SafeQueue(void)
	{}

	// Add an element to the queue.
	void enqueue(T t)
	{
		std::lock_guard<std::mutex> lock(m);
		q.push(t);
		c.notify_one();
		msec = std::chrono::milliseconds(1);
	}

	// Get the "front"-element.
	// If blocking = false - return NULL if Q is enpty.
	// If blocking = true - if the queue is empty, wait till an element is avaiable (waitfor_msec <= 0) or timeout expired.
	T dequeue(int waitfor_msec = -1, bool* timeoutexit = NULL, bool* blocking = NULL)
	{
		std::unique_lock<std::mutex> lock(m);
		std::cv_status stat;
		T val;

		if ((blocking != NULL) && (*blocking == false))
		{
			// A non blocking call
			if (q.empty())
			{
				return NULL;
			}
			else
			{
				*timeoutexit = false;
				val = q.front();
				q.pop();
				return val;
			}
		}
		else
		{
			// A blocking call
			while (q.empty())
			{
				// release lock as long as the wait and reaquire it afterwards.
				if ((waitfor_msec <= 0) && (timeoutexit != NULL))
				{
					c.wait(lock);
				}
				else
				{
					stat = c.wait_for(lock, waitfor_msec * msec);
					if (stat == std::cv_status::timeout)
					{
						// Timeout exit - return NULL
						*timeoutexit = true;
						return val;
					}
				}
			}

			*timeoutexit = false;
			val = q.front();
			q.pop();
			return val;
		}
	}

private:
	std::queue<T> q;
	mutable std::mutex m;
	std::condition_variable c;
	std::chrono::milliseconds msec;
};

