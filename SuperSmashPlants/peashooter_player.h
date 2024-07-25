#ifndef _PEASHOOTER_PLAYER_H_
#define _PEASHOOTER_PLAYER_H_

#include "player.h"

#include<iostream>

extern Atlas atlas_peashooter_idle_left;				// �㶹���ֳ������Ĭ�϶���ͼ��
extern Atlas atlas_peashooter_idle_right;				// �㶹���ֳ����ҵ�Ĭ�϶���ͼ��
extern Atlas atlas_peashooter_run_left;					// �㶹���ֳ�����ı��ܶ���ͼ��
extern Atlas atlas_peashooter_run_right;				// �㶹���ֳ����ҵı��ܶ���ͼ��



class PeashooterPlayer : public Player
{
public:
	PeashooterPlayer()
	{
		animation_idle_left.set_atlas(&atlas_peashooter_idle_left);
		animation_idle_right.set_atlas(&atlas_peashooter_idle_right);
		animation_run_left.set_atlas(&atlas_peashooter_run_left);
		animation_run_right.set_atlas(&atlas_peashooter_run_right);

		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);

		size.x = 96;
		size.y = 96;
	}

	~PeashooterPlayer() = default;

	void on_update(int delta)
	{	
		Player::on_update(delta);
	}

	

private:

};

#endif // !_PEASHOOTER_PLAYER_H_
