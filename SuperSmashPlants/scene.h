#ifndef _SCENE_H_
#define _SCENE_H

#include<graphics.h>

class Scene
{
public:
	Scene() = default;
	~Scene() = default;
	//��Ӧ��Ϸ�����׶�
	virtual void on_enter() { }
	virtual void on_update() { }
	virtual void on_draw() { }
	virtual void on_input(const ExMessage& meg) { }
	virtual void on_exit() { }

private:

};


#endif // !_SCENE_H_