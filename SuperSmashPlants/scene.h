#ifndef _SCENE_H_
#define _SCENE_H_

#include<graphics.h>

class Scene
{
public:
	Scene() = default;
	 ~Scene() = default;

	// 方便子类进行重载
	virtual void on_enter() { } //进入场景
	virtual void on_update(int delta) { } //更新场景
	virtual void on_draw() { } //绘制场景
	virtual void on_input(const ExMessage& meg) { } // 处理输入
	virtual void on_exit() { } //退出场景

private:

};


#endif // !_SCENE_H_