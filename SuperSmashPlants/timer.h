#ifndef _TIMER_H_
#define _TIMER_H_

#include<functional>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	// 重置计时器
	void restart()
	{
		pass_time = 0;
		shotted = false;
	}

	// 设置等待的时间
	void set_wait_time(int val)
	{
		wait_time = val;
	}

	// 是否单次触发
	void set_one_shot(bool flag)
	{
		ont_shot = flag;
	}

	// 
	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}

	// 设置暂停
	void pause()
	{
		paused = true;
	}

	// 设置继续
	void resume()
	{
		paused = false;
	}

	// 
	void on_update(int delta)
	{
		if (paused)
			return;

		pass_time += delta;
		if (pass_time >= wait_time)
		{
			if ((!ont_shot || (ont_shot && !shotted)) && callback)
			// 定时器不是单词触发，或者定时器单次触发并且没有被触发过，则表示可以继续触发
			// 同时还要检查callback是否有效
				callback();
			shotted = true;
			pass_time = 0;
		}
	}

private:
	int pass_time = 0;				// 已经过的时间
	int wait_time = 0;				// 等待时间
	bool paused = false;			// 是否暂停
	bool shotted = false;			// 是否触发
	bool ont_shot = false;			// 单次触发
	std::function<void()> callback; // 触发回调
};
#endif // !_TIMER_H_
