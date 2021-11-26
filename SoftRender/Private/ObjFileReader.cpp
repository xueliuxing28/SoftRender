#include "ObjFileReader.h"

void StringSplit(string s, char splitchar, vector<string>& vec)

{
	if (vec.size() > 0)//保证vec是空的  
		vec.clear();
	int length = s.length();
	int start = 0;
	for (int i = 0; i < length; i++)
	{
		if (s[i] == splitchar && i == 0)//第一个就遇到分割符  
		{
			start += 1;
		}
		else if (s[i] == splitchar)
		{
			vec.push_back(s.substr(start, i - start));
			start = i + 1;
		}
		else if (i == length - 1)//到达尾部  
		{
			vec.push_back(s.substr(start, i + 1 - start));
		}
	}
}

void ReadObjFile(string path, Mesh * obj)

{
	ifstream in(path);
	//int vertexIndex = 0;
	//int vertexCount = 0;
	string txt = "";

	if (in) // 有该文件  
	{
		while (getline(in, txt)) // line中不包括每行的换行符  
		{
			//顶点坐标
			if (txt[0] == 'v' && txt[1] == ' ')
			{
				vector<string> num;
				txt.erase(0, 2);
				StringSplit(txt, ' ', num);
				Vector3f pos;
				pos = Vector3f((float)atof(num[0].c_str()), (float)atof(num[1].c_str()), (float)atof(num[2].c_str()));
				//pos.Print();
				obj->positionBuffer.push_back(pos);
			}
			//顶点法线
			else if (txt[0] == 'v' && txt[1] == 'n')
			{
				vector<string> num;
				txt.erase(0, 3);
				StringSplit(txt, ' ', num);
				Vector3f n = Vector3f((float)atof(num[0].c_str()), (float)atof(num[1].c_str()), (float)atof(num[2].c_str()), 0.0);
				//n.Print();
				obj->normalBuffer.push_back(n);
			}
			//uv
			else if (txt[0] == 'v' && txt[1] == 't')
			{
				vector<string> num;
				txt.erase(0, 3);
				StringSplit(txt, ' ', num);
				//Vector2((float)atof(num[0].c_str()), (float)atof(num[1].c_str()));
				obj->uvBuffer.push_back(Vector2((float)atof(num[0].c_str()), (float)atof(num[1].c_str())));
				//cout << (float)atof(num[0].c_str()) << " " << (float)atof(num[1].c_str()) << endl;

			}
			//索引编号
			//　"f 7/13/21"这时在面的数据中多了贴图坐标uv点和法线的索引号，索引号分别用左斜线(/)隔开。 
			else if (txt[0] == 'f' && txt[1] == ' ')
			{
				vector<string> num;
				txt.erase(0, 2);
				StringSplit(txt, ' ', num);
				//vector<string> index;
				for (int i = 0; i < num.size(); i++)
				{
					vector<string> threeIndex;
					StringSplit(num[i], '/', threeIndex);
					Vector3i indexes = { atoi(threeIndex[0].c_str()), atoi(threeIndex[1].c_str()), atoi(threeIndex[2].c_str()) };
					//indexes.Print();
					obj->indexBuffer.push_back(indexes);
				}
			}
		}
	}
	else // 没有该文件  
	{
		cout << "no file" << endl;
	}
}

