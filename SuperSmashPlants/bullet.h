#ifndef _BULLET_H_
#define _BULLET_H_

#include"vector2.h"
#include"player_id.h"
#include"camera.h" 
#include<functional>
#include<graphics.h>

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	void set_damege(int val)					// 设置子弹的伤害
	{
		damege = val;
	}
	
	int get_damege()							// 获取子弹可以造成的伤害
	{
		return damege;
	}
	 
	void set_position(float x, float y)			//初始化子弹位置
	{
		position.x = x;
		position.y = y;
	}

	const Vector2& get_position() const			// 获取子弹位置
	{
		return position;
	}

	const Vector2& get_size() const				// 获取子弹尺寸
	{
		return size;
	}

	void set_velocity(float x, float y)			// 设置子弹速度
	{
		velocity.x = x;
		velocity.y = y;
	}

	void set_collide_target(PlayerID target)	// 设置碰撞目标
	{
		target_id = target;
	}

	PlayerID get_collide_target() const			// 获取碰撞目标
	{
		return target_id;
	}

	void set_callback(std::function<void()> callback)	// 保存回调函数
	{
		this->callback = callback;
	}

	void set_valid(bool flag)					// 设置子弹是否可以继续碰撞
	{
		valid = flag;
	}

	bool get_valid() const						// 获取子弹是否可以继续碰撞
	{
		return valid;
	}

	bool check_can_remove() const				// 是否能被移除
	{
		return can_remove;
	}

	virtual void on_collide()					// 虚函数提供重写，当子弹发生碰撞时使用
	{
		if (callback)
			callback();
	}

	virtual bool check_collision(const Vector2& position, const Vector2& size)
		// 子弹是否与指定位置和尺寸的目标发生了碰撞
	{
		return this->position.x + this->size.x / 2 >= position.x
			&& this->position.x + this->size.x / 2 <= position.x + size.x
			&& this->position.y + this->size.y / 2 >= position.y
			&& this->position.y + this->size.y / 2 <= position.y + size.y;
	}

	virtual void on_update(int delta){ }
	virtual void on_draw(const Camera& camera){ }
protected:
	Vector2 size;								// 子弹尺寸
	Vector2 position;							// 子弹位置
	Vector2 velocity;							// 子弹速度
	int damege = 10;							// 子弹伤害
	bool valid = true;							// 子弹是否有效
	bool can_remove = false;					// 子弹是否可以被移除
	std::function<void()> callback;				// 子弹碰撞回调函数
	PlayerID target_id = PlayerID::P1;			// 子弹碰撞目标玩家ID

protected:
	bool check_if_exceeds_screen()				// 检测子弹的矩形边界是否已经位于屏幕矩形之外
	{
		return (position.x + size.x <= 0 || position.x >= getwidth()
			|| position.y + size.x <= 0 || position.y >= getheight());
		// 对四个临界条件进行判断
	}
};

#endif // !_BULLET_H_
