#ifndef _ATLAS_H_
#define _ATLAS_H_

#include<vector>
#include<graphics.h>

// 图片资源的容器
class Atlas
{
public:
	Atlas() = default;
	~Atlas() = default;

	void load_from_file(LPCTSTR path_template, int num)
	{
		// 清空图片对象列表
		// 将列表长度设置为指定大小
		img_list.clear();
		img_list.resize(num);
		TCHAR path_file[256];
		for (int i = 0; i < img_list.size(); i++)
		{
			_stprintf_s(path_file, path_template, i + 1);
			loadimage(&img_list[i], path_file);
		}
	}

	// 清空图片对象列表
	void clear()
	{
		img_list.clear();
	}

	// 获取图集中图片数量
	int get_size()
	{
		return (int)img_list.size();
	}

	// 获取实际渲染的动画帧
	IMAGE* get_image(int idx)
	{
		if (idx < 0 || idx >= img_list.size())
			return nullptr;

		return &img_list[idx];
	}

	// 向图集中添加已有的图片对象
	void add_image(const IMAGE& img)
	{
		img_list.push_back(img);
	}
private:
	// 图片对象列表
	std::vector<IMAGE> img_list;
};


#endif // !_ATLAS_H_
