#ifndef _ATLAS_H_
#define _ATLAS_H_

#include<vector>
#include<graphics.h>

// ͼƬ��Դ������
class Atlas
{
public:
	Atlas() = default;
	~Atlas() = default;

	void load_from_file(LPCTSTR path_template, int num)
	{
		// ���ͼƬ�����б�
		// ���б�������Ϊָ����С
		img_list.clear();
		img_list.resize(num);
		TCHAR path_file[256];
		for (int i = 0; i < img_list.size(); i++)
		{
			_stprintf_s(path_file, path_template, i + 1);
			loadimage(&img_list[i], path_file);
		}
	}

	// ���ͼƬ�����б�
	void clear()
	{
		img_list.clear();
	}

	// ��ȡͼ����ͼƬ����
	int get_size()
	{
		return (int)img_list.size();
	}

	// ��ȡʵ����Ⱦ�Ķ���֡
	IMAGE* get_image(int idx)
	{
		if (idx < 0 || idx >= img_list.size())
			return nullptr;

		return &img_list[idx];
	}

	// ��ͼ����������е�ͼƬ����
	void add_image(const IMAGE& img)
	{
		img_list.push_back(img);
	}
private:
	// ͼƬ�����б�
	std::vector<IMAGE> img_list;
};


#endif // !_ATLAS_H_
