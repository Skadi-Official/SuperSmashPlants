#ifndef _PLAYER_H_
#define _PLAYER_H_

#include"camera.h"
#include"vector2.h"
#include"animation.h"
#include"player_id.h"
#include"platform.h"
#include"bullet.h"
#include"particle.h"

#include<graphics.h>
#include<iostream>
extern bool is_debug;

extern IMAGE img_1P_cursor;
extern IMAGE img_2P_cursor;

extern Atlas atlas_run_effect;
extern Atlas atlas_jump_effect;
extern Atlas atlas_land_effect;

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;

class Player
{
public:
	Player(bool facing_right = true) : is_facing_right(facing_right)
	{
		if (is_facing_right)
		{
			current_animation = &animation_idle_right;
		}
		else
		{
			current_animation = &animation_idle_left;
		}

		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]() { can_attack = true; });

		timer_invulnerable.set_wait_time(750);
		timer_invulnerable.set_one_shot(true);
		timer_invulnerable.set_callback([&]() { is_invulnerable = false; });

		timer_invulnerable_blink.set_wait_time(75);
		timer_invulnerable_blink.set_callback([&]()
			{
				is_showing_sketch_frame = !is_showing_sketch_frame;
			});

		timer_run_effect_generation.set_wait_time(75);
		timer_run_effect_generation.set_callback([&]() 
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 45);
			});

		timer_die_effect_generation.set_wait_time(35);
		timer_die_effect_generation.set_callback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 150);
			});

		animation_jump_effect.set_atlas(&atlas_jump_effect);
		animation_jump_effect.set_interval(25);
		animation_jump_effect.set_loop(false);
		animation_jump_effect.set_callback([&]()
			{
				is_jump_effect_visible = false;
			});

		animation_land_effect.set_atlas(&atlas_land_effect);
		animation_land_effect.set_interval(50);
		animation_land_effect.set_loop(false);
		animation_land_effect.set_callback([&]()
			{
				is_land_effect_visible = false;
			});

		timer_cursor_visibility.set_wait_time(2500);
		timer_cursor_visibility.set_one_shot(true);
		timer_cursor_visibility.set_callback([&]()
			{
				is_cursor_visible = false;
			});
	}


	~Player() = default;

	virtual void on_run(float distance)		// 奔跑方法，根据位移修改玩家水平方向坐标
	{
		if (is_attacking_ex)
			return;

		position.x += distance;
		timer_run_effect_generation.resume();
	}

	virtual void on_update(int delta)
	{
		int direction = is_right_key_down - is_left_key_down;

		if (direction != 0)
		{
			if (is_attacking_ex == false)
			{
				is_facing_right = direction > 0;
			}
			current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
			float distance = direction * run_velocity * delta;	// 这一帧中，玩家在水平方向上移动的距离
			on_run(distance);
		}
		else
		{
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
			timer_run_effect_generation.pause();
		}

		if (is_attacking_ex)
		{
			if (is_facing_right)
			{
				current_animation = &animation_attack_ex_right;
			}
			else
			{
				current_animation = &animation_attack_ex_left;
			}
		}

		current_animation->on_update(delta);
		animation_jump_effect.on_update(delta);
		animation_land_effect.on_update(delta);

		timer_attack_cd.on_update(delta);
		timer_invulnerable.on_update(delta);
		timer_invulnerable_blink.on_update(delta);
		timer_run_effect_generation.on_update(delta);
		timer_cursor_visibility.on_update(delta);

		if (hp <= 0)
		{
			timer_die_effect_generation.on_update(delta);
			if (last_hurt_direction.x < 0)
			{
				current_animation = &animation_die_left;
			}
			else
			{
				current_animation = &animation_die_right;
			}
		}

		particle_list.erase(std::remove_if(
			particle_list.begin(), particle_list.end(),
			[](const Particle& particle)
			{
				return !particle.check_valid();
			}),
			particle_list.end());

		for (Particle& particle : particle_list)
		{	
			//std::cout << "particle update" << std::endl;
			particle.on_update(delta);
		}

		if (is_showing_sketch_frame == true)
		{	
			// 判断当前是否需要显示剪影图片，如果有则把当前帧图片抓取出来处理为剪影效果保存
			sketch_image(current_animation->get_frame(), &img_sketch);
		}

		move_and_collide(delta);
	}

	

	virtual void on_jump()					// 跳跃方法
	{	
		if (velocity.y != 0 || is_attacking_ex)
		{
			return;
		}
		velocity.y += jump_velocity;
		is_jump_effect_visible = true;
		animation_jump_effect.reset();

		IMAGE* effect_frame = animation_jump_effect.get_frame();
		position_jump_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
		position_jump_effect.y = position.y + size.y - effect_frame->getheight();
	}

	virtual void on_land()
	{
		is_land_effect_visible = true;
		animation_land_effect.reset();

		IMAGE* effect_frame = animation_jump_effect.get_frame();
		position_land_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
		position_land_effect.y = position.y + size.y - effect_frame->getheight();
	}

	virtual void on_draw(const Camera& camera)
	{
		if (is_jump_effect_visible)
		{
			animation_jump_effect.on_draw(camera, (int)position_jump_effect.x, (int)position_jump_effect.y);
		}

		if (is_land_effect_visible)
		{
			animation_land_effect.on_draw(camera, (int)position_land_effect.x, (int)position_land_effect.y);
		}

		for (const Particle& particle : particle_list)
		{
			//std::cout << "particle on draw" << std::endl;
			particle.on_draw(camera);
		}

		if (hp > 0 && is_invulnerable && is_showing_sketch_frame)
		{
			putimage_alpha(camera, (int)position.x, (int)position.y, &img_sketch);
		}
		else
		{
			current_animation->on_draw(camera, (int)position.x, (int)position.y);
		}

		if (is_cursor_visible)
		{
			switch (id)
			{
			case PlayerID::P1:
				putimage_alpha(camera, (int)(position.x + (size.x - img_1P_cursor.getwidth()) / 2),
					(int)(position.y - img_1P_cursor.getheight()), &img_1P_cursor);
				break;
			case PlayerID::P2:
				putimage_alpha(camera, (int)(position.x + (size.x - img_2P_cursor.getwidth()) / 2),
					(int)(position.y - img_2P_cursor.getheight()), &img_2P_cursor);
				break;
			default:
				break;
			}
		}
		
		if (is_debug)
		{
			setlinecolor(RGB(0, 155, 155));
			rectangle((int)position.x, (int)position.y, (int)(position.x + size.x), (int)(position.y + size.y));
		}
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

	void set_hp(int val)
	{
		hp = val;
	}

	int get_hp() const
	{
		return hp;
	}

	int get_mp() const
	{
		return mp;
	}

	void make_invulnerable()
	{
		is_invulnerable = true;
		timer_invulnerable.restart();
	}


	virtual void on_attack() { }
	virtual void on_attack_ex() { }
protected:
	void move_and_collide(int delta)
	{	
		float last_velocity_y = velocity.y;

		velocity.y += gravity * delta;			// v = g*t(竖直方向上)
		position += velocity * (float)delta;	// s = v*t

		// 血量小于 0 的时候不与任何物体发生互动
		if (hp <= 0)
		{
			return;
		}

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

						if (last_velocity_y != 0)
						{
							on_land();
						}

						break;
					}
				}
			}
		}

		if (is_invulnerable == false)
		{
			for (Bullet* bullet : bullet_list)
			{
				if (bullet->get_valid() == false || bullet->get_collide_target() != id)
				{
					// 如果当前子弹无效，或者碰撞目标的id和当前角色id不匹配，直接跳过
					continue;
				}

				// 没有跳过则需要判断是否发生碰撞
				if (bullet->check_collision(position, size))
				{
					// 发生碰撞则调用对应方法，并且设置它为不可继续发生碰撞
					make_invulnerable();
					bullet->on_collide();
					bullet->set_valid(false);
					hp -= bullet->get_damege();

					// 作差得到最后一次收到攻击的方向
					last_hurt_direction = bullet->get_position() - position;
					if (hp <= 0)
					{	
						// x 轴方向的值要与收到攻击方向相反
						// 小于 0 则表示攻击来自左边，大于 0 则表示攻击来自右边
						if (last_hurt_direction.x < 0)
						{
							velocity.x = 0.35f;
						}
						else
						{
							velocity.x = -0.35f;
						}
						velocity.y = -1.0f;
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
	Animation animation_jump_effect;	// 跳跃特效动画
	Animation animation_land_effect;	// 落地特效动画
	Animation animation_die_left;		// 朝向左的死亡动画
	Animation animation_die_right;		// 朝向右的死亡动画

	bool is_jump_effect_visible = false;// 跳跃动画是否可见
	bool is_land_effect_visible = false;// 落地动画是否可见

	Vector2 position_jump_effect;		// 跳跃动画播放位置
	Vector2 position_land_effect;		// 落地动画播放位置

	Animation* current_animation = nullptr;	//当前正在播放的动画

	PlayerID id = PlayerID::P1;			// 玩家序号 ID

	bool is_left_key_down = false;		// 向左移动按键是否按下
	bool is_right_key_down = false;		// 向右移动按键是否按下

	bool is_facing_right = true;		// 角色是否朝向右侧	

	int attack_cd = 500;				// 普通攻击冷却时间 ms
	bool can_attack = true;				// 是否可以释放普通攻击

	Timer timer_attack_cd;				// 普通攻击冷却定时器

	bool is_attacking_ex = false;		// 是否正在释放特殊攻击

	IMAGE img_sketch;						// 动画帧剪影图片
	bool is_invulnerable = false;			// 角色是否处于无敌状态
	bool is_showing_sketch_frame = false;	// 当前帧是否应该显示剪影
	Timer timer_invulnerable;				// 无敌状态定时器
	Timer timer_invulnerable_blink;			// 无敌状态闪烁定时器

	std::vector<Particle> particle_list;	// 粒子对象列表
	Timer timer_run_effect_generation;		// 跑动特效粒子定时发射器
	Timer timer_die_effect_generation;		// 死亡特效粒子定时发射器

	bool is_cursor_visible = true;			// 玩家光标指示器是否显示
	Timer timer_cursor_visibility;			// 玩家光标指示器可见性定时器

	Vector2 last_hurt_direction;			// 最后一次收到攻击的方向
};

#endif // !_PLAYER_H_
