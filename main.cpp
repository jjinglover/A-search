
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

struct Point
{
	Point(){}

	Point(int x, int y){
		this->x = x;
		this->y = y;
	}
	int x = 0;
	int y = 0;

	double distance(const Point &pos){
		double dis = sqrt(pow(pos.x + x, 2) + pow(pos.y + y, 2));
		return dis;
	}

	bool operator==(const Point &pos){
		return this->x == pos.x&&this->y == pos.y;
	}
};

struct Data
{
	Data(){}
	Data(Point pos, double h, double g, Data *parent){
		this->pos = pos;
		this->h = h;
		this->g = g;
		this->parent = parent;
	}

	Point pos=Point(0,0);
	double h = 0;
	double g = 0;
	Data *parent = nullptr;

	double f(){
		return this->g + this->h;
	}
};

struct MinHeap 
{
	std::vector<Data*> m_vec;
	std::map<std::string, Data*> m_map;
	int index = 0;

	Data *getMinAndRemove(){
		if (isEmpty()){
			return nullptr;
		}
		sort();
		auto first = m_vec.at(0);
		auto last = m_vec.at(index - 1);
		m_vec[0] = last;
		
		--index;
		return first;
	}

	bool isEmpty(){
		return index <= 0;
	}

	std::string getKey(Point pos){
		char buff[32] = { 0 };
		sprintf(buff, "%d-%d", pos.x, pos.y);
		return buff;
	}

	Data *find(Point pos){
		auto it = m_map.find(getKey(pos));
		if (it!=m_map.end())
		{
			return it->second;
		}
		return nullptr;
	}

	void add(Data *data){
		if (index<m_vec.size())
		{
			m_vec[index] = data;
		}
		else
		{
			m_vec.push_back(data);
		}
		index = m_vec.size();
		m_map[getKey(data->pos)] = data;
	}

	void sort(){
		std::sort(m_vec.begin(), m_vec.end(), [](Data *a, Data *b){return a->f() < b->f(); });
	}

	void release(){
		for (auto it = m_map.begin();it!=m_map.end();)
		{
			Data *tmp = it->second;
			it = m_map.erase(it);
			delete tmp;
		}
	}
};

int main()
{
	const char START = 'S';
	const char END = 'E';
	const char VISITED = '-';
	const char SPACE = '.';
	Point startPos;
	Point endPos;

	const std::string s_arr[10] = {
		"....................",
		"....................",
		"...II.......IIIII...",
		".....I......I....I..",
		".....I......I..E....",
		".....I......I....I..",
		"...S.I......IIIII...",
		"...II...............",
		"....................",
		"....................",
	};

	char arr[30][30] = { 0 };
	int index = 0;
	int max_l = 0;
	for (int i = 0; i < 10;++i)
	{
		std::string str = s_arr[i];
		index = 0;
		for (int j = 0; j < str.size();++j)
		{
			char c = str.at(j);
			if (j!=' ')
			{
				arr[i][j] = c;
				if (c==START)
				{
					startPos = Point(i, index);
				} else if (c==END)
				{
					endPos = Point(i, index);
				}
				++index;
			}
		}
		if (max_l == 0){
			max_l = index;
		}
	}

	int directs[][2] = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };

	MinHeap heap;
	heap.add(new Data(startPos, 0, 0, nullptr));
	bool finish = false;
	Data *lastData = nullptr;
	
	while (!finish&&!heap.isEmpty())
	{
		Data *data = heap.getMinAndRemove();
		if (arr[data->pos.x][data->pos.y]==SPACE)
		{
			arr[data->pos.x][data->pos.y] = VISITED;
		}

		for (auto dir : directs)
		{
			Point pos = Point(data->pos.x+dir[0], data->pos.y + dir[1]);
			if (pos.x>0&&pos.x<10
				&& pos.y>0 && pos.y<max_l)
			{
				char c = arr[pos.x][pos.y];
				if (c==END)
				{
					finish = true;
					lastData = data;
					break;
				}
				if (c!=SPACE)
				{
					continue;
				}

				auto nn = heap.find(pos);
				if (nn)
				{
					if (nn->g>data->g+1)
					{
						nn->g = data->g + 1;
						nn->parent = data;
					}
				} else{
					heap.add(new Data(pos, pos.distance(endPos), data->g + 1,data));
				}
			}
		}
	}

	if (lastData)
	{
		while (lastData->parent)
		{
			arr[lastData->pos.x][lastData->pos.y] = '@';
			lastData = lastData->parent;
		}

		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < max_l; ++j)
			{
				std::cout << arr[i][j];
			}
			std::cout << std::endl;
		}

	} else{
		std::cout << "no path" << std::endl;
	}
	heap.release();

	getchar();
	return 0;
}
