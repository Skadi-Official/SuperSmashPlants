#ifndef _TIMER_H_
#define _TIMER_H_

#include<functional>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	// ���ü�ʱ��
	void restart()
	{
		pass_time = 0;
		shotted = false;
	}

	// ���õȴ���ʱ��
	void set_wait_time(int val)
	{
		wait_time = val;
	}

	// �Ƿ񵥴δ���
	void set_one_shot(bool flag)
	{
		ont_shot = flag;
	}

	// 
	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}

	// ������ͣ
	void pause()
	{
		paused = true;
	}

	// ���ü���
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
			// ��ʱ�����ǵ��ʴ��������߶�ʱ�����δ�������û�б������������ʾ���Լ�������
			// ͬʱ��Ҫ���callback�Ƿ���Ч
				callback();
			shotted = true;
			pass_time = 0;
		}
	}

private:
	int pass_time = 0;				// �Ѿ�����ʱ��
	int wait_time = 0;				// �ȴ�ʱ��
	bool paused = false;			// �Ƿ���ͣ
	bool shotted = false;			// �Ƿ񴥷�
	bool ont_shot = false;			// ���δ���
	std::function<void()> callback; // �����ص�
};
#endif // !_TIMER_H_
