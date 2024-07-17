#ifndef _CAMERA_H_
#define _CAMERA_H_

#include"vector2.h"
#include "timer.h"
class Camera
{
public:
	Camera()
	{
		timer_shake.set_one_shot(true);
		timer_shake.set_callback([&]()
			{
				is_shaking = false;
				reset();
			});
	}
	~Camera() = default;

	// 返回摄像机位置的常量引用
	const Vector2& get_position() const
	{
		return position;
	}

	// 重置摄像机位置
	void reset()
	{
		position.x = 0;
		position.y = 0;
	}

	void on_update(int delta)
	{
		timer_shake.on_update(delta);

		if(is_shaking)
		{
			// 将摄像机的坐标在抖动强度为半径的圆内进行随机设置，乘号左边的系数是描述了一个单位圆的范围
			// -1.0 到 1.0 的浮点数
			position.x = (-50 + rand() % 100) / 50.0f * shaking_strength;
			position.y = (-50 + rand() % 100) / 50.0f * shaking_strength;
		}
	}

	void shake(float strength, int duration)
	{
		is_shaking = true;
		shaking_strength = strength;

		timer_shake.set_wait_time(duration);
		timer_shake.restart();
	}

private:
	Vector2 position;			 // 摄像机位置
	Timer timer_shake;			 // 摄像机抖动定时器
	bool is_shaking = false;	 // 摄像机是否正在抖动
	float shaking_strength = 0;	 // 摄像机抖动复读
};

#endif // !_CAMERA_H_
