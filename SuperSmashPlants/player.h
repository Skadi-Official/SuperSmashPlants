#ifndef _PLAYER_H_
#define _PLAYER_H_

#include"camera.h"
#include"vector2.h"
#include"animation.h"
#include"player_id.h"
#include"platform.h"
#include"bullet.h"

#include<graphics.h>

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;

class Player
{
public:
	Player()
	{
		current_animation = &animation_idle_right;

		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]() { can_attack = true; });
	}
	~Player() = default;

	virtual void on_run(float distance)		// 奔跑方法，根据位移修改玩家水平方向坐标
	{
		if (is_attacking_ex)
			return;

		position.x += distance;
	}

	virtual void on_update(int delta)
	{
		int direction = is_right_key_down - is_left_key_down;

		if (direction != 0)
		{
			is_facing_right = direction > 0;
			current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
			float distance = direction * run_velocity * delta;	// 这一帧中，玩家在水平方向上移动的距离
			on_run(distance);
		}
		else
		{
			current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
		}

		current_animation->on_update(delta);

		timer_attack_cd.on_update(delta);

		move_and_collide(delta);
	}

	

	virtual void on_jump()					// 跳跃方法
	{	
		if (velocity.y != 0 || is_attacking_ex)
		{
			return;
		}
		velocity.y += jump_velocity;
	}

	virtual void on_draw(const Camera& camera)
	{
		current_animation->on_draw(camera, (int)position.x, (int)position.y);
	}

	virtual void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
				case 0x41:
					// A
					is_left_key_down = true;
					break;
				case 0x44:
					// D
					is_right_key_down = true;
					break;
				case 0x57:
					// W
					on_jump();
					break;
				case 0x46:
					// F
					if (can_attack)
					{
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}
					break;
				case 0x47:
					// G
					if (mp >= 100)
					{
						on_attack_ex();
						mp = 0;
					}
					break;
				default:
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT:
					// ←
					is_left_key_down = true;
					break;
				case VK_RIGHT:
					// →
					is_right_key_down = true;
					break;
				case VK_UP:
					// ↑
					on_jump();
					break;
				case VK_OEM_PERIOD:
					// .
					if (can_attack)
					{
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}
					break;
				case VK_OEM_2:
					// /
					if (mp >= 100)
					{
						on_attack_ex();
						mp = 0;
					}
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		case WM_KEYUP:
			switch (id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
				case 0x41:
					// A
					is_left_key_down = false;
					break;
				case 0x44:
					// D
					is_right_key_down = false;
					break;
				default:
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT:
					// ←
					is_left_key_down = false;
					break;
				case VK_RIGHT:
					// →
					is_right_key_down = false;
					break;
				default:
					break;
				}
				break;
			}
			break;
		default:
			break;
		}
	}

	void set_id(PlayerID id)
	{
		this->id = id;
	}

	void set_position(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	const Vector2& get_position() const
	{
		return position;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	virtual void on_attack() { }
	virtual void on_attack_ex() { }
protected:
	void move_and_collide(int delta)
	{	
		velocity.y += gravity * delta;			// v = g*t(竖直方向上)
		position += velocity * (float)delta;	// s = v*t

		if (velocity.y > 0)		// 注意，y轴大于0才是在下落
		{
			for (const Platform& platform : platform_list)
			{
				const Platform::CollisionShape& shape = platform.shape;

				// 玩家矩形和平台在x方向上是否有碰撞
				bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left)
					<= size.x + (shape.right - shape.left));

				// 玩家矩形和平台在y方向上是否有碰撞，碰撞线位于玩家底部和顶部之间即满足碰撞条件
				bool is_collide_y = (shape.y >= position.y && shape.y <= position.y + size.y);

				if (is_collide_x && is_collide_y)
				{	
					float delta_pos_y = velocity.y * delta;	// 玩家这一帧在y移动的距离
					float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;		// 上一帧玩家脚底的位置
					if (last_tick_foot_pos_y <= shape.y)	// 如果上一帧脚底在碰撞线上方，且xy方向上都发生碰撞
					{
						position.y = shape.y - size.y;		// 矫正位置
						velocity.y = 0;						// 竖直方向速度调整为 0 
						break;
					}
				}
			}
		}
	}

protected:
	const float gravity = 1.6e-3f;		// 玩家重力
	const float run_velocity = 0.55f;	// 跑动速度
	const float jump_velocity = -0.85f; // 跳跃速度

protected:
	int mp = 0;							// 角色能量
	int hp = 100;						// 角色血量
	Vector2 size;						// 角色尺寸
	Vector2 position;					// 角色位置
	Vector2 velocity;					// 角色速度

	Animation animation_idle_left;		// 朝向左的默认动画
	Animation animation_idle_right;		// 朝向右的默认动画
	Animation animation_run_left;		// 朝向左的奔跑动画
	Animation animation_run_right;		// 朝向右的奔跑动画
	Animation animation_attack_ex_left;	// 朝向左的特殊攻击动画
	Animation animation_attack_ex_right;// 朝向右的特殊攻击动画

	Animation* current_animation = nullptr;	//当前正在播放的动画

	PlayerID id = PlayerID::P1;			// 玩家序号 ID

	bool is_left_key_down = false;		// 向左移动按键是否按下
	bool is_right_key_down = false;		// 向右移动按键是否按下

	bool is_facing_right = true;		// 角色是否朝向右侧	

	int attack_cd = 500;				// 普通攻击冷却时间 ms
	bool can_attack = true;				// 是否可以释放普通攻击

	Timer timer_attack_cd;				// 普通攻击冷却定时器

	bool is_attacking_ex = false;		// 是否正在释放特殊攻击
};

#endif // !_PLAYER_H_
