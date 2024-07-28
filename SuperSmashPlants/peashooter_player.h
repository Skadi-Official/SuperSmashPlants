#ifndef _PEASHOOTER_PLAYER_H_
#define _PEASHOOTER_PLAYER_H_

#include "player.h"
#include "pea_bullet.h"
#include "camera.h"
#include<iostream>

extern Atlas atlas_peashooter_idle_left;				// �㶹���ֳ������Ĭ�϶���ͼ��
extern Atlas atlas_peashooter_idle_right;				// �㶹���ֳ����ҵ�Ĭ�϶���ͼ��
extern Atlas atlas_peashooter_run_left;					// �㶹���ֳ�����ı��ܶ���ͼ��
extern Atlas atlas_peashooter_run_right;				// �㶹���ֳ����ҵı��ܶ���ͼ��
extern Atlas atlas_peashooter_attack_ex_left;			
extern Atlas atlas_peashooter_attack_ex_right;
extern Atlas atlas_peashooter_die_left;
extern Atlas atlas_peashooter_die_right;
extern Camera main_camera;


class PeashooterPlayer : public Player
{
public:
	PeashooterPlayer(bool facing_right = true) : Player(facing_right)
	{
		animation_idle_left.set_atlas(&atlas_peashooter_idle_left);
		animation_idle_right.set_atlas(&atlas_peashooter_idle_right);
		animation_run_left.set_atlas(&atlas_peashooter_run_left);
		animation_run_right.set_atlas(&atlas_peashooter_run_right);
		animation_attack_ex_left.set_atlas(&atlas_peashooter_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_peashooter_attack_ex_right);
		animation_die_left.set_atlas(&atlas_peashooter_die_left);
		animation_die_right.set_atlas(&atlas_peashooter_die_right);

		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);
		animation_attack_ex_left.set_interval(75);
		animation_attack_ex_right.set_interval(75);
		animation_die_left.set_interval(150);
		animation_die_right.set_interval(150);

		animation_die_left.set_loop(false);
		animation_die_right.set_loop(false);
		size.x = 96;
		size.y = 96;

		timer_attack_ex.set_wait_time(attack_ex_duration);
		timer_attack_ex.set_one_shot(true);
		timer_attack_ex.set_callback([&]() { is_attacking_ex = false; });

		timer_spawn_pea_ex.set_wait_time(100);
		timer_spawn_pea_ex.set_callback([&]()
			{
				spawn_pea_bullet(speed_pea_ex);
			});

		attack_cd = 100;
	}

	~PeashooterPlayer() = default;

	void on_update(int delta)
	{	
		Player::on_update(delta);

		if (is_attacking_ex)
		{
			main_camera.shake(5, 100);
			timer_attack_ex.on_update(delta);
			timer_spawn_pea_ex.on_update(delta);
		}
	}

	

private:
	void spawn_pea_bullet(float speed)
	{
		Bullet* bullet = new PeaBullet();

		Vector2 bullet_position, bullet_velocity;
		const Vector2& bullet_size = bullet->get_size();
		if (is_facing_right)
		{
			bullet_position.x = position.x + size.x - bullet_size.x / 2;
			bullet_velocity.x = speed;
		}
		else
		{
			bullet_position.x = position.x - bullet_size.x / 2;
			bullet_velocity.x = -speed;
		}
		bullet_position.y = position.y;
		bullet_velocity.y = 0;
		


		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_velocity(bullet_velocity.x, bullet_velocity.y);

		if (id == PlayerID::P1)
		{
			bullet->set_collide_target(PlayerID::P2);
		}
		else
		{
			bullet->set_collide_target(PlayerID::P1);
		}

		bullet->set_callback([&]() {mp += 25; });

		std::cout << bullet_position.x << std::endl;
		std::cout << bullet_position.y << std::endl;
		std::cout << bullet << std::endl;
		std::cout << bullet->get_damege() << std::endl;
		bullet_list.push_back(bullet);
	}

	void on_attack()
	{
		spawn_pea_bullet(speed_pea);

		switch (rand() % 2)
		{
		case 0:
			break;
			mciSendString(_T("play pea_shoot_1 from 0"), NULL, 0, NULL);
		case 1:
			mciSendString(_T("play pea_shoot_2 from 0"), NULL, 0, NULL);
			break;
		default:
			break;
		}
	}

	void on_attack_ex()
	{
		is_attacking_ex = true;
		timer_attack_ex.restart();

		if (is_facing_right)
		{
			animation_attack_ex_right.reset();
		}
		else
		{
			animation_attack_ex_left.reset();
		}
		mciSendString(_T("play pea_shoot_ex from 0"), NULL, 0, NULL);
	}

private:
	const float speed_pea = 0.75f;				// ��ͨ�����㶹�ӵ��ٶ�
	const float speed_pea_ex = 1.5f;			// ���⹥���㶹�ӵ��ٶ�
	const int attack_ex_duration = 2500;		// ���⹥��״̬����ʱ��
private:
	Timer timer_attack_ex;		// ���⹥��״̬��ʱ��
	Timer timer_spawn_pea_ex;	// �㶹�ӵ����䶨ʱ��

};

#endif // !_PEASHOOTER_PLAYER_H_
