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

	virtual void on_run(float distance)		// ���ܷ���������λ���޸����ˮƽ��������
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
			float distance = direction * run_velocity * delta;	// ��һ֡�У������ˮƽ�������ƶ��ľ���
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

	

	virtual void on_jump()					// ��Ծ����
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
					// ��
					is_left_key_down = true;
					break;
				case VK_RIGHT:
					// ��
					is_right_key_down = true;
					break;
				case VK_UP:
					// ��
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
					// ��
					is_left_key_down = false;
					break;
				case VK_RIGHT:
					// ��
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
		velocity.y += gravity * delta;			// v = g*t(��ֱ������)
		position += velocity * (float)delta;	// s = v*t

		if (velocity.y > 0)		// ע�⣬y�����0����������
		{
			for (const Platform& platform : platform_list)
			{
				const Platform::CollisionShape& shape = platform.shape;

				// ��Ҿ��κ�ƽ̨��x�������Ƿ�����ײ
				bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left)
					<= size.x + (shape.right - shape.left));

				// ��Ҿ��κ�ƽ̨��y�������Ƿ�����ײ����ײ��λ����ҵײ��Ͷ���֮�伴������ײ����
				bool is_collide_y = (shape.y >= position.y && shape.y <= position.y + size.y);

				if (is_collide_x && is_collide_y)
				{	
					float delta_pos_y = velocity.y * delta;	// �����һ֡��y�ƶ��ľ���
					float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;		// ��һ֡��ҽŵ׵�λ��
					if (last_tick_foot_pos_y <= shape.y)	// �����һ֡�ŵ�����ײ���Ϸ�����xy�����϶�������ײ
					{
						position.y = shape.y - size.y;		// ����λ��
						velocity.y = 0;						// ��ֱ�����ٶȵ���Ϊ 0 
						break;
					}
				}
			}
		}
	}

protected:
	const float gravity = 1.6e-3f;		// �������
	const float run_velocity = 0.55f;	// �ܶ��ٶ�
	const float jump_velocity = -0.85f; // ��Ծ�ٶ�

protected:
	int mp = 0;							// ��ɫ����
	int hp = 100;						// ��ɫѪ��
	Vector2 size;						// ��ɫ�ߴ�
	Vector2 position;					// ��ɫλ��
	Vector2 velocity;					// ��ɫ�ٶ�

	Animation animation_idle_left;		// �������Ĭ�϶���
	Animation animation_idle_right;		// �����ҵ�Ĭ�϶���
	Animation animation_run_left;		// ������ı��ܶ���
	Animation animation_run_right;		// �����ҵı��ܶ���
	Animation animation_attack_ex_left;	// ����������⹥������
	Animation animation_attack_ex_right;// �����ҵ����⹥������

	Animation* current_animation = nullptr;	//��ǰ���ڲ��ŵĶ���

	PlayerID id = PlayerID::P1;			// ������ ID

	bool is_left_key_down = false;		// �����ƶ������Ƿ���
	bool is_right_key_down = false;		// �����ƶ������Ƿ���

	bool is_facing_right = true;		// ��ɫ�Ƿ����Ҳ�	

	int attack_cd = 500;				// ��ͨ������ȴʱ�� ms
	bool can_attack = true;				// �Ƿ�����ͷ���ͨ����

	Timer timer_attack_cd;				// ��ͨ������ȴ��ʱ��

	bool is_attacking_ex = false;		// �Ƿ������ͷ����⹥��
};

#endif // !_PLAYER_H_
