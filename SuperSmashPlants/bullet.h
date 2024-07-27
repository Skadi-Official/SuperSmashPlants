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

	void set_damege(int val)					// �����ӵ����˺�
	{
		damege = val;
	}
	
	int get_damege()							// ��ȡ�ӵ�������ɵ��˺�
	{
		return damege;
	}
	 
	void set_position(float x, float y)			//��ʼ���ӵ�λ��
	{
		position.x = x;
		position.y = y;
	}

	const Vector2& get_position() const			// ��ȡ�ӵ�λ��
	{
		return position;
	}

	const Vector2& get_size() const				// ��ȡ�ӵ��ߴ�
	{
		return size;
	}

	void set_velocity(float x, float y)			// �����ӵ��ٶ�
	{
		velocity.x = x;
		velocity.y = y;
	}

	void set_collide_target(PlayerID target)	// ������ײĿ��
	{
		target_id = target;
	}

	PlayerID get_collide_target() const			// ��ȡ��ײĿ��
	{
		return target_id;
	}

	void set_callback(std::function<void()> callback)	// ����ص�����
	{
		this->callback = callback;
	}

	void set_valid(bool flag)					// �����ӵ��Ƿ���Լ�����ײ
	{
		valid = flag;
	}

	bool get_valid() const						// ��ȡ�ӵ��Ƿ���Լ�����ײ
	{
		return valid;
	}

	bool check_can_remove() const				// �Ƿ��ܱ��Ƴ�
	{
		return can_remove;
	}

	virtual void on_collide()					// �麯���ṩ��д�����ӵ�������ײʱʹ��
	{
		if (callback)
			callback();
	}

	virtual bool check_collision(const Vector2& position, const Vector2& size)
		// �ӵ��Ƿ���ָ��λ�úͳߴ��Ŀ�귢������ײ
	{
		return this->position.x + this->size.x / 2 >= position.x
			&& this->position.x + this->size.x / 2 <= position.x + size.x
			&& this->position.y + this->size.y / 2 >= position.y
			&& this->position.y + this->size.y / 2 <= position.y + size.y;
	}

	virtual void on_update(int delta){ }
	virtual void on_draw(const Camera& camera){ }
protected:
	Vector2 size;								// �ӵ��ߴ�
	Vector2 position;							// �ӵ�λ��
	Vector2 velocity;							// �ӵ��ٶ�
	int damege = 10;							// �ӵ��˺�
	bool valid = true;							// �ӵ��Ƿ���Ч
	bool can_remove = false;					// �ӵ��Ƿ���Ա��Ƴ�
	std::function<void()> callback;				// �ӵ���ײ�ص�����
	PlayerID target_id = PlayerID::P1;			// �ӵ���ײĿ�����ID

protected:
	bool check_if_exceeds_screen()				// ����ӵ��ľ��α߽��Ƿ��Ѿ�λ����Ļ����֮��
	{
		return (position.x + size.x <= 0 || position.x >= getwidth()
			|| position.y + size.x <= 0 || position.y >= getheight());
		// ���ĸ��ٽ����������ж�
	}
};

#endif // !_BULLET_H_
