#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include"scene.h"

extern Scene* menu_scene;
extern Scene* game_scene;
extern Scene* selector_scene;

class SceneManager
{
public:
	// 场景的种类
	enum class SceneType
	{
		Menu,
		Game,
		Selector
	};

public:
	SceneManager() = default;
	~SceneManager() = default;

	// 这个函数只用于初始化
	void set_current_scene(Scene* scene)
	{
		current_scene = scene;
		scene->on_enter();
	}

	// 这个函数用于场景切换
	void switch_to(SceneType type)
	{
		current_scene->on_exit();
		switch (type)
		{
		case SceneType::Menu:
			current_scene = menu_scene;
			break;
		case SceneType::Game:
			current_scene = game_scene;
			break;
		case SceneType::Selector:
			current_scene = selector_scene;
			break;
		default:
			break;
		}
		current_scene->on_enter();
	}

	// 调用当前场景更新方法
	void on_update(int delta)
	{
		current_scene->on_update(delta);
	}

	void on_draw()
	{
		current_scene->on_draw();
	}

	void on_input(const ExMessage& msg)
	{
		current_scene->on_input(msg);
	}

private:
	// 基类指针允许我们可以把它设置为任何派生类
	Scene* current_scene = nullptr;
};

#endif // !_SCENE_MANAGER_H
