#define _CRT_SECURE_NO_WARNINGS 1
#define START 0
#define DSNAKE 1
#define OPTIONS 2
#define EXIT 3

#include <stdio.h>
#include <conio.h>
#include <vector>
#include <Windows.h>
#include <time.h>
#include <iostream>




#pragma comment(lib, "winmm.lib")

using std::vector;

// 全局方法-控制台-放在全局数据区、就不封装了，方便调用----start

extern class Csnake qing;
extern class CnormalFood nor_food;

// 全局变量
bool break_sign = 0;
bool game_over_sign = 0;
bool time_sign = 0;
short speed = 120;
int score = 0;
unsigned int pre_time = 0;
COORD map_size = { 48,32 };

// 菜单坐标点 基准点
COORD st_menu_0 = { map_size.X + 4, 1 };
COORD st_menu_1 = { map_size.X + 4, 3 };
COORD st_menu_2 = { map_size.X + 4, 6 };
COORD st_menu_3 = { map_size.X + 4, 9 };
COORD st_menu_4 = { map_size.X + 4, 12 };
COORD st_menu_5 = { map_size.X + 4, 15 };
COORD st_menu_6 = { map_size.X + 4, 18 };
COORD st_menu_7 = { map_size.X + 4, 21 };
COORD st_gm_menu_0 = { map_size.X + 3, (map_size.X) / 1.8 };
COORD st_gm_menu_1 = { map_size.X + 9, (map_size.X) / 1.8 };

void SetColor(int color_id) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_id);
}

void LocWrite(COORD st_loc, const char* s_buff) {
	SetColor(14);
	st_loc.X *= 2;
	HANDLE h_output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h_output, st_loc);
	printf(s_buff);
}

void LocWrite(COORD st_loc, const char* s_buff, bool off_on) {
	if (off_on == 1)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_GREEN |
			FOREGROUND_BLUE |
			BACKGROUND_BLUE |
			BACKGROUND_GREEN |
			BACKGROUND_RED);
	st_loc.X *= 2;
	HANDLE h_output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h_output, st_loc);
	printf(s_buff);
}

void BackMusic() {
	PlaySoundA("yoxi.wav", NULL, SND_LOOP | SND_ASYNC);
}

void IntervalSpeed(short o_speed) {
	pre_time = (unsigned int)time(NULL);
	speed = o_speed;
	time_sign = 1;
}

void SpeedResetTimer() {
	if ((time_sign == 1) && ((unsigned int)time(NULL) - pre_time > 3)) {
		time_sign = 0;
		speed = 120;
		pre_time = (unsigned int)time(NULL);
	}
}


// 全局方法-控制台-放在全局数据区、就不封装了，方便调用----over


class CgameDisplay {
	// ***1. 游戏显示：地图、计分板、选项界面
	vector<COORD>clear_loc;
public:
	CgameDisplay(){}
	~CgameDisplay(){
	}

	void MapGen() {
		for (short y = 0; y < map_size.Y; y++) {
			for (short x = 0; x < map_size.X; x++) {
				if (y == 0 || y == map_size.Y - 1) {
					LocWrite({ x,y }, "■");
				}
				if (x == 0 || x == map_size.X - 1) {
					LocWrite({ x,y }, "■");
				}
			}
		}
	}

	void ScoreDisplay() {
		LocWrite(st_menu_1, "Score: ");
		printf("%d   效果时间：5(s)", score);
		clear_loc.push_back(st_menu_1);
		LocWrite(st_menu_2, "★：果实 +长度 +5分数");
		LocWrite(st_menu_3, "□：冰块 +速度 +2分数");
		LocWrite(st_menu_4, "火：火焰 -长度 -1分数 +速度");
		LocWrite(st_menu_5, "■：障碍 碰撞游戏结束");
		LocWrite(st_menu_6, "▓ ：shit -速度");
		LocWrite(st_menu_7, "ESC键：暂停游戏");
		if (score >= 20) {
			LocWrite({ st_menu_7.X, 28 }, "============小彩蛋============");
			LocWrite({ st_menu_7.X, 29 }, "                              ");
			LocWrite({ st_menu_7.X, 30 }, "我又不是贪吃蛇，干嘛说我贪吃？");
			LocWrite({ st_menu_7.X, 31 }, "   (☆ω☆)阿尼亚，哇酷哇酷");
			LocWrite({ st_menu_7.X, 32 }, "                              ");
			LocWrite({ st_menu_7.X, 33 }, "==============================");
		}
	}
	void ClearGameDisplay() {
		for (int i = 0; i < clear_loc.size(); i++){
			LocWrite(clear_loc[i], "                  ");
		}
		score = 0;
	}

	void ClearMenuDisplay() {
		LocWrite(st_menu_1, "                                     ");
		LocWrite(st_menu_2, "                                     ");
		LocWrite(st_menu_3, "                                     ");
		LocWrite(st_menu_4, "                                     ");
		LocWrite(st_menu_5, "                                     ");
		LocWrite(st_menu_6, "                                     ");
		LocWrite(st_menu_7, "                                     ");
	}

	void MainMenuDisplay(bool on_off = 0) {
		LocWrite(st_menu_1, "开始游戏");
		LocWrite(st_menu_2, "双蛇模式");
		LocWrite(st_menu_3, "游戏设置");
		LocWrite(st_menu_4, "退出游戏");
		LocWrite(st_menu_5, "=================");
		LocWrite(st_menu_6, "W A S D  控制蛇");
		LocWrite(st_menu_7, "↑←↓→ 选择选项");
		if (on_off) LocWrite(st_menu_1, "开始游戏", 1);
	}

	void OptMenuDisplay(bool on_off = 0) {
		LocWrite(st_menu_1, "地图障碍物自定义");
		LocWrite(st_menu_2, "果实数量");
		LocWrite(st_menu_3, "shit数量");
		LocWrite(st_menu_4, "冰块数量");
		LocWrite(st_menu_5, "火焰数量");
		LocWrite(st_menu_6, "障碍物数量");
		LocWrite(st_menu_7, "退出设置");
		if (on_off) LocWrite(st_menu_1, "地图障碍物自定义", 1);
	}

};



// 蛇主体
class Csnake {
protected:
	const char* name = "小青";
public:
	// vsta 动态结构体数组
	vector<COORD>vsta_snake;

	// 构造函数初始化：蛇头坐标、蛇头朝向、画头、
	Csnake(COORD head_loc, char orientation) {
		vsta_snake.insert(vsta_snake.begin(), head_loc);
		switch (orientation) {
		case 'w': LocWrite(vsta_snake[0], "△");break;
		case 'a': LocWrite(vsta_snake[0], "＜");break;
		case 's': LocWrite(vsta_snake[0], "▽");break;
		case 'd': LocWrite(vsta_snake[0], "＞");break;
		}
	}
	~Csnake(){}

	void SnakeMove(char move_dir, vector<COORD>food, vector<COORD>fire) {
		bool food_sign = 0, fire_sign = 0;
		for (int i = 0; i < fire.size(); i++) {
			if (fire[i].X == vsta_snake[0].X && fire[i].Y == vsta_snake[0].Y) {
				fire_sign = 1;
				if (vsta_snake.size() == 1) game_over_sign = 1; break;
			}
		}
		if (!fire_sign) vsta_snake.insert(vsta_snake.begin(), vsta_snake[0]);

		switch (move_dir) {
		case 'w': --(vsta_snake[0].Y);LocWrite(vsta_snake[0], "△");break;
		case 'a': --(vsta_snake[0].X);LocWrite(vsta_snake[0], "＜");break;
		case 's': ++(vsta_snake[0].Y);LocWrite(vsta_snake[0], "▽");break;
		case 'd': ++(vsta_snake[0].X);LocWrite(vsta_snake[0], "＞");break;
		default:break;
		}
		CrashDetec();
		for (int i = 0; i < food.size(); i++) {
			if ((food[i].X == vsta_snake[0].X) && (food[i].Y == vsta_snake[0].Y)) {
				food_sign = 1;
				break;
			}
		}
		if (!food_sign) {
			LocWrite(vsta_snake.back(), "  ");
			vsta_snake.pop_back();
		}
		for (int i = 1; i < vsta_snake.size(); i++){
			LocWrite(vsta_snake[i], "●");
		}
	}
	void CrashDetec() {
		if (vsta_snake[0].X >= map_size.X - 1 || vsta_snake[0].X <= 0 ||
			vsta_snake[0].Y >= map_size.Y - 1 || vsta_snake[0].Y <= 0) {
			game_over_sign = 1;
		}
		for (int i = 2; i < vsta_snake.size(); i++) {
			if ((vsta_snake[i].X == vsta_snake[0].X) && (vsta_snake[i].Y == vsta_snake[0].Y)) game_over_sign = 1;
		}
	}

	vector<COORD> GetSnakeLoc() {
		return vsta_snake;
	}

	void SnakeCrashDete(vector<COORD>o_snake) {
		for (int i = 1; i < o_snake.size(); i++){
			if ( (vsta_snake[0].X == o_snake[i].X) && (vsta_snake[0].Y == o_snake[i].Y)){
				COORD center_loc = { (map_size.X / 2),(map_size.Y / 2) - 2 };
				LocWrite(center_loc, "");
				printf("%s 惜败", this->name);
				system("pause > NULL");
				LocWrite(center_loc, "          ");
				game_over_sign = 1;
			}
		}
	}
	
};

class Csnake_bai:public Csnake {
public:
	Csnake_bai(COORD head_loc, char orientation) :Csnake(head_loc, orientation) {
		name = "小白";
	}

	void SnakeMove(char move_dir, vector<COORD>food, vector<COORD>fire) {
		bool food_sign = 0, fire_sign = 0;
		for (int i = 0; i < fire.size(); i++) {
			if (fire[i].X == vsta_snake[0].X && fire[i].Y == vsta_snake[0].Y) {
				fire_sign = 1;
				if (vsta_snake.size() == 1) game_over_sign = 1; break;
			}
		}
		if (!fire_sign) vsta_snake.insert(vsta_snake.begin(), vsta_snake[0]);

		switch (move_dir) {
		case 'i': --(vsta_snake[0].Y);LocWrite(vsta_snake[0], "▲");break;
		case 'j': --(vsta_snake[0].X);LocWrite(vsta_snake[0], "←");break;
		case 'k': ++(vsta_snake[0].Y);LocWrite(vsta_snake[0], "▼");break;
		case 'l': ++(vsta_snake[0].X);LocWrite(vsta_snake[0], "→");break;
		default:break;
		}
		CrashDetec();
		for (int i = 0; i < food.size(); i++) {
			if ((food[i].X == vsta_snake[0].X) && (food[i].Y == vsta_snake[0].Y)) {
				food_sign = 1;
				break;
			}
		}
		if (!food_sign) {
			LocWrite(vsta_snake.back(), "  ");
			vsta_snake.pop_back();
		}
		for (int i = 1; i < vsta_snake.size(); i++) {
			LocWrite(vsta_snake[i], "●");
		}
	}
	void CrashDetec() {
		if (vsta_snake[0].X >= map_size.X - 1 || vsta_snake[0].X <= 0 ||
			vsta_snake[0].Y >= map_size.Y - 1 || vsta_snake[0].Y <= 0) {
			game_over_sign = 1;
		}
		for (int i = 2; i < vsta_snake.size(); i++) {
			if ((vsta_snake[i].X == vsta_snake[0].X) && (vsta_snake[i].Y == vsta_snake[0].Y)) game_over_sign = 1;
		}
	}
};


// 物品父类
class CrandomObj {
protected:
	short num;
	vector<COORD>obj_loc;
	COORD tmp_loc; // 用于装填
	// map_size 生成范围由地图大小决定

public:
	char* s_buff;
	CrandomObj(short o_num) :num(o_num){
		s_buff = new char[4];
	}
	virtual ~CrandomObj() {
		delete[]s_buff;
	}

	virtual void Generate(vector<COORD> snake_loc) {
		for (int i = 0; i < num; i++) {
			tmp_loc.X = (rand() % (map_size.X - 2)) + 1;
			tmp_loc.Y = (rand() % (map_size.Y - 2)) + 1;
			// 判断是否生成在蛇身坐标上，重新生成
			for (int j = 0; j < snake_loc.size(); j++) {
				while (tmp_loc.X == snake_loc[j].X &&
					tmp_loc.Y == snake_loc[j].Y) {
					tmp_loc.X = (rand() % (map_size.X - 2)) + 1;
					tmp_loc.Y = (rand() % (map_size.Y - 2)) + 1;
					j = -1;
					break;
				}
			}
			obj_loc.push_back(tmp_loc);
		}
		for (int i = 0; i < obj_loc.size(); i++)
			LocWrite(obj_loc[i], this->s_buff);
	}

	virtual void Generate(vector<COORD> snake_loc, vector<COORD> snake_loc1) {
		for (int i = 0; i < num; i++) {
			tmp_loc.X = (rand() % (map_size.X - 2)) + 1;
			tmp_loc.Y = (rand() % (map_size.Y - 2)) + 1;
			// 判断是否生成在蛇身坐标上，重新生成
			for (int j = 0; j < snake_loc.size(); j++) {
				while (tmp_loc.X == snake_loc[j].X &&
					tmp_loc.Y == snake_loc[j].Y) {
					tmp_loc.X = (rand() % (map_size.X - 2)) + 1;
					tmp_loc.Y = (rand() % (map_size.Y - 2)) + 1;
					j = -1;
					break;
				}
			}
			for (int j = 0; j < snake_loc1.size(); j++) {
				while (tmp_loc.X == snake_loc1[j].X &&
					tmp_loc.Y == snake_loc1[j].Y) {
					tmp_loc.X = (rand() % (map_size.X - 2)) + 1;
					tmp_loc.Y = (rand() % (map_size.Y - 2)) + 1;
					j = -1;
					break;
				}
			}
			obj_loc.push_back(tmp_loc);
		}
		for (int i = 0; i < obj_loc.size(); i++)
			LocWrite(obj_loc[i], this->s_buff);
	}

	virtual void CrashBuff(vector<COORD> snake_loc) = 0;

	virtual void ClearObj() {
		for (int i = obj_loc.size() - 1; i >= 0 ; --i){
			LocWrite(obj_loc[i], "  ");
			obj_loc.pop_back();
		}
	}

	void AddObj(COORD HACK_FOR_FUN) {
		obj_loc.push_back(HACK_FOR_FUN);
	}

	void PopObj(COORD loc) {
		for (int i = 0; i < obj_loc.size(); i++){
			if (loc.X == obj_loc[i].X && loc.Y == obj_loc[i].Y) {
				obj_loc.erase(obj_loc.begin()+i);
			}
		}
	}

	void SetObjLoc(vector<COORD>loc) {
		obj_loc = loc;
	}

	vector<COORD> GetObjLoc() {
		return this->obj_loc;
	}

};


class CnormalFood : public CrandomObj {
private:
	
public:
	CnormalFood(short o_num = 5) :CrandomObj(o_num) {
		strcpy(s_buff, "★");
	}

	// 虚函数：碰撞后的具体效果 判断被吃重新生成
	void CrashBuff(vector<COORD> snake_loc) {
		for (int i = 0; i < obj_loc.size(); i++) {
			for (int j = 0; j < snake_loc.size(); j++) {
				if (obj_loc[i].X == snake_loc[j].X &&
					obj_loc[i].Y == snake_loc[j].Y) {
					//生成新的
					tmp_loc.X = (rand() % (map_size.X - 2)) + 1;
					tmp_loc.Y = (rand() % (map_size.Y - 2)) + 1;
					// 判断是否生成在蛇身坐标上，重新生成
					while (tmp_loc.X == snake_loc[j].X &&
						tmp_loc.Y == snake_loc[j].Y) {
						tmp_loc.X = (rand() % (map_size.X - 2)) + 1;
						tmp_loc.Y = (rand() % (map_size.Y - 2)) + 1;
					}
					obj_loc[i] = tmp_loc;
					LocWrite(obj_loc[i], this->s_buff);
					// 其他效果
					score += 5;
					return;
				}
			}
		}
	}
};

class CshitGround : public CrandomObj {
private:

public:
	CshitGround(short o_num = 5) :CrandomObj(o_num) {
		strcpy(s_buff, "▓");
	}

	// 虚函数：碰撞后的具体效果：碰撞后重新生成
	void CrashBuff(vector<COORD> snake_loc) {
		for (int i = 0; i < obj_loc.size(); i++) {
			for (int j = 0; j < snake_loc.size(); j++) {
				if (obj_loc[i].X == snake_loc[j].X &&
					obj_loc[i].Y == snake_loc[j].Y) {
					//生成新的
					tmp_loc.X = (rand() % (map_size.X - 2)) + 1;
					tmp_loc.Y = (rand() % (map_size.Y - 2)) + 1;
					// 判断是否生成在蛇身坐标上，重新生成
					while (tmp_loc.X == snake_loc[j].X &&
						tmp_loc.Y == snake_loc[j].Y) {
						tmp_loc.X = (rand() % (map_size.X - 2)) + 1;
						tmp_loc.Y = (rand() % (map_size.Y - 2)) + 1;
					}
					obj_loc[i] = tmp_loc;
					LocWrite(obj_loc[i], this->s_buff);
					// buff 效果 - 大幅减速
					IntervalSpeed(250);
					
					return;
				}
			}
		}
	}
};


class CiceGround : public CrandomObj {
private:

public:
	CiceGround(short o_num = 5) : CrandomObj(o_num) {
		strcpy(s_buff, "□");
	}

	// 虚函数：碰撞后的具体效果：碰撞后重新生成
	void CrashBuff(vector<COORD> snake_loc) {
		for (int i = 0; i < obj_loc.size(); i++) {
			for (int j = 0; j < snake_loc.size(); j++) {
				if (obj_loc[i].X == snake_loc[j].X &&
					obj_loc[i].Y == snake_loc[j].Y) {
					// buff 效果 加速、加分
					IntervalSpeed(60);
					score += 2;
					return;
				}
			}
			LocWrite(obj_loc[i], this->s_buff);
		}
	}
};


class CfireGround : public CrandomObj {
private:

public:
	CfireGround(short o_num = 5) :CrandomObj(o_num) {
		strcpy(s_buff, "火");
	}

	// 虚函数：碰撞后的具体效果：碰撞后重新生成
	void CrashBuff(vector<COORD> snake_loc) {
		for (int i = 0; i < obj_loc.size(); i++) {
			for (int j = 0; j < snake_loc.size(); j++) {
				if (obj_loc[i].X == snake_loc[j].X &&
					obj_loc[i].Y == snake_loc[j].Y) {
					LocWrite(obj_loc[i], this->s_buff);
					// buff 效果 - 加速大幅提升、掉尾巴、减分数
					IntervalSpeed(35);
					score -= 2;
					return;
				}
			}
			LocWrite({ st_menu_1.X, 3 }, ""); // 移走光标
		}
	}
};



class Cbarrier : public CrandomObj {
private:

public:
	Cbarrier(short o_num = 5) :CrandomObj(o_num) {
		strcpy(s_buff, "■");
	}

	// 虚函数：碰撞后的具体效果：碰撞后重新生成
	void CrashBuff(vector<COORD> snake_loc) {
		for (int i = 0; i < obj_loc.size(); i++) {
			for (int j = 0; j < snake_loc.size(); j++) {
				if (obj_loc[i].X == snake_loc[j].X &&
					obj_loc[i].Y == snake_loc[j].Y) {
					game_over_sign = 1;
					return;
				}
			}
		}
	}

};


class CfileOperate {
public:

	static void FileSave(char move_dir, vector<COORD>snake, vector<COORD>food, vector<COORD>shit, vector<COORD>ice, vector<COORD>fire, vector<COORD>barrier) {
		FILE* file = nullptr;
		COORD tmp_coord;
		int tmp = 0;

		if (fopen_s(&file, ".\\snake.txt", "wb+") != 0)	printf("不能正常打开文件");
		else {
			/* 保存信息：
			* 分数
			* 移动方向
			* 蛇对象（整体动态结构体数组坐标、）
			* 物品对象（整体动态结构体数组坐标）：食物、shit、ice、fire、barrier
			*/
			fwrite(&score, sizeof(int), 1, file);
			fwrite(&move_dir, sizeof(char), 1, file);

			tmp = snake.size();
			fwrite(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < snake.size(); i++){
				tmp_coord = snake[i];
				fwrite(&tmp_coord, sizeof(COORD), 1, file);
			}

			tmp = food.size();
			fwrite(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < food.size(); i++) {
				tmp_coord = food[i];
				fwrite(&tmp_coord, sizeof(COORD), 1, file);
			}

			tmp = shit.size();
			fwrite(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < shit.size(); i++) {
				tmp_coord = shit[i];
				fwrite(&tmp_coord, sizeof(COORD), 1, file);
			}

			tmp = ice.size();
			fwrite(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < ice.size(); i++) {
				tmp_coord = ice[i];
				fwrite(&tmp_coord, sizeof(COORD), 1, file);
			}

			tmp = fire.size();
			fwrite(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < fire.size(); i++) {
				tmp_coord = fire[i];
				fwrite(&tmp_coord, sizeof(COORD), 1, file);
			}

			tmp = barrier.size();
			fwrite(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < barrier.size(); i++) {
				tmp_coord = barrier[i];
				fwrite(&tmp_coord, sizeof(COORD), 1, file);
			}
			
			fclose(file);

		}

	}

	static void FileSave(char move_dir, char move_dir1, vector<COORD>snake, vector<COORD>snake1, vector<COORD>food, vector<COORD>shit, vector<COORD>ice, vector<COORD>fire, vector<COORD>barrier) {
		FILE* file = nullptr;
		COORD tmp_coord;
		int tmp = 0;

		if (fopen_s(&file, ".\\snake_double.txt", "wb+") != 0)	printf("不能正常打开文件");
		else {
			/* 保存信息：
			* 分数
			* 移动方向
			* 蛇对象（整体动态结构体数组坐标、）
			* 物品对象（整体动态结构体数组坐标）：食物、shit、ice、fire、barrier
			*/
			fwrite(&score, sizeof(int), 1, file);
			fwrite(&move_dir, sizeof(char), 1, file);
			fwrite(&move_dir1, sizeof(char), 1, file);

			tmp = snake.size();
			fwrite(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < snake.size(); i++) {
				tmp_coord = snake[i];
				fwrite(&tmp_coord, sizeof(COORD), 1, file);
			}

			tmp = snake1.size();
			fwrite(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < snake1.size(); i++) {
				tmp_coord = snake1[i];
				fwrite(&tmp_coord, sizeof(COORD), 1, file);
			}

			tmp = food.size();
			fwrite(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < food.size(); i++) {
				tmp_coord = food[i];
				fwrite(&tmp_coord, sizeof(COORD), 1, file);
			}

			tmp = shit.size();
			fwrite(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < shit.size(); i++) {
				tmp_coord = shit[i];
				fwrite(&tmp_coord, sizeof(COORD), 1, file);
			}

			tmp = ice.size();
			fwrite(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < ice.size(); i++) {
				tmp_coord = ice[i];
				fwrite(&tmp_coord, sizeof(COORD), 1, file);
			}

			tmp = fire.size();
			fwrite(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < fire.size(); i++) {
				tmp_coord = fire[i];
				fwrite(&tmp_coord, sizeof(COORD), 1, file);
			}

			tmp = barrier.size();
			fwrite(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < barrier.size(); i++) {
				tmp_coord = barrier[i];
				fwrite(&tmp_coord, sizeof(COORD), 1, file);
			}

			fclose(file);

		}

	}


	static void FileRead(char& move_dir, Csnake& c_snake, CnormalFood& c_food, CshitGround& c_shit, CiceGround&c_ice, CfireGround&c_fire, Cbarrier&c_barrier) {
		// buffer
		vector<COORD>snake;
		vector<COORD>food;
		vector<COORD>shit;
		vector<COORD>ice;
		vector<COORD>fire;
		vector<COORD>barrier;
		COORD tmp_coord;
		int tmp = 0;

		FILE* file = nullptr;

		if (fopen_s(&file, ".\\snake.txt", "rb+") != 0)	printf("不能正常打开文件");
		else{
			fread(&score, sizeof(int), 1, file);
			fread(&move_dir, sizeof(char), 1, file);

			fread(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < tmp; i++) {
				fread(&tmp_coord, sizeof(COORD), 1, file);
				snake.push_back(tmp_coord);
			}
			c_snake.vsta_snake = snake;
			LocWrite(snake[0], "  ");
			for (int i = 0; i < snake.size(); i++){
				if (i == 0) {
					switch (move_dir) {
					case 'w': LocWrite(snake[0], "△");break;
					case 'a': LocWrite(snake[0], "＜");break;
					case 's': LocWrite(snake[0], "▽");break;
					case 'd': LocWrite(snake[0], "＞");break;
					default:break;
					}
				} 
				else LocWrite(snake[i], "●");
			}

			fread(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < tmp; i++) {
				fread(&tmp_coord, sizeof(COORD), 1, file);
				food.push_back(tmp_coord);
				LocWrite(food[i], c_food.s_buff);
			}


			fread(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < tmp; i++) {
				fread(&tmp_coord, sizeof(COORD), 1, file);
				shit.push_back(tmp_coord);
				LocWrite(shit[i], c_shit.s_buff);
			}

			fread(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < tmp; i++) {
				fread(&tmp_coord, sizeof(COORD), 1, file);
				ice.push_back(tmp_coord);
				LocWrite(ice[i], c_ice.s_buff);
			}

			fread(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < tmp; i++) {
				fread(&tmp_coord, sizeof(COORD), 1, file);
				fire.push_back(tmp_coord);
				LocWrite(fire[i], c_fire.s_buff);
			}

			fread(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < tmp; i++) {
				fread(&tmp_coord, sizeof(COORD), 1, file);
				barrier.push_back(tmp_coord);
				LocWrite(barrier[i], c_barrier.s_buff);
			}

			c_food.SetObjLoc(food);
			c_shit.SetObjLoc(shit);
			c_ice.SetObjLoc(ice);
			c_fire.SetObjLoc(fire);
			c_barrier.SetObjLoc(barrier);
			fclose(file);
		}
	}

	static void FileRead(char& move_dir, char& move_dir1, Csnake& c_snake, Csnake_bai& c_snake1, CnormalFood& c_food, CshitGround& c_shit, CiceGround& c_ice, CfireGround& c_fire, Cbarrier& c_barrier) {
		// buffer
		vector<COORD>snake;
		vector<COORD>snake1;
		vector<COORD>food;
		vector<COORD>shit;
		vector<COORD>ice;
		vector<COORD>fire;
		vector<COORD>barrier;
		COORD tmp_coord;
		int tmp = 0;

		FILE* file = nullptr;

		if (fopen_s(&file, ".\\snake_double.txt", "rb+") != 0)	printf("不能正常打开文件");
		else {
			fread(&score, sizeof(int), 1, file);
			fread(&move_dir, sizeof(char), 1, file);
			fread(&move_dir1, sizeof(char), 1, file);

			fread(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < tmp; i++) {
				fread(&tmp_coord, sizeof(COORD), 1, file);
				snake.push_back(tmp_coord);
			}
			c_snake.vsta_snake = snake;
			LocWrite(snake[0], "  ");
			for (int i = 0; i < snake.size(); i++) {
				if (i == 0) {
					switch (move_dir) {
					case 'w': LocWrite(snake[0], "△");break;
					case 'a': LocWrite(snake[0], "＜");break;
					case 's': LocWrite(snake[0], "▽");break;
					case 'd': LocWrite(snake[0], "＞");break;
					default:break;
					}
				}
				else LocWrite(snake[i], "●");
			}

			fread(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < tmp; i++) {
				fread(&tmp_coord, sizeof(COORD), 1, file);
				snake1.push_back(tmp_coord);
			}
			c_snake1.vsta_snake = snake1;
			LocWrite(snake1[0], "  ");
			for (int i = 0; i < snake1.size(); i++) {
				if (i == 0) {
					switch (move_dir) {
					case 'w': LocWrite(snake1[0], "▲");break;
					case 'a': LocWrite(snake1[0], "←");break;
					case 's': LocWrite(snake1[0], "▼");break;
					case 'd': LocWrite(snake1[0], "→");break;
					default:break;
					}
				}
				else LocWrite(snake1[i], "●");
			}


			fread(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < tmp; i++) {
				fread(&tmp_coord, sizeof(COORD), 1, file);
				food.push_back(tmp_coord);
				LocWrite(food[i], c_food.s_buff);
			}


			fread(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < tmp; i++) {
				fread(&tmp_coord, sizeof(COORD), 1, file);
				shit.push_back(tmp_coord);
				LocWrite(shit[i], c_shit.s_buff);
			}

			fread(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < tmp; i++) {
				fread(&tmp_coord, sizeof(COORD), 1, file);
				ice.push_back(tmp_coord);
				LocWrite(ice[i], c_ice.s_buff);
			}

			fread(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < tmp; i++) {
				fread(&tmp_coord, sizeof(COORD), 1, file);
				fire.push_back(tmp_coord);
				LocWrite(fire[i], c_fire.s_buff);
			}

			fread(&tmp, sizeof(int), 1, file);
			for (int i = 0; i < tmp; i++) {
				fread(&tmp_coord, sizeof(COORD), 1, file);
				barrier.push_back(tmp_coord);
				LocWrite(barrier[i], c_barrier.s_buff);
			}

			c_food.SetObjLoc(food);
			c_shit.SetObjLoc(shit);
			c_ice.SetObjLoc(ice);
			c_fire.SetObjLoc(fire);
			c_barrier.SetObjLoc(barrier);
			fclose(file);
		}
	}
};


// 背景音乐


// 游戏引擎控制类
class CgameSystem {


public:



};

CgameSystem obj_game_system;


// 物品数量
// 食物数量
short num_nor_food = 5;
short num_shit = 5;
short num_ice = 5;
short num_fire = 5;
short num_barrier = 5;




int main() {

	while (true){

		// 局部实例化-食物（结束-析构-销毁）  
		CnormalFood nor_food(num_nor_food);
		CshitGround shit(num_shit);


		// 局部实例化-地形（结束-析构-销毁）
		CiceGround ice(num_ice);
		CfireGround fire(num_fire);
		Cbarrier barrier(num_barrier);

		// 使用父类指针指向子类对象，批量管理调用子类虚函数
		CrandomObj* p_obj[5];
		p_obj[0] = &nor_food;
		p_obj[1] = &shit;
		p_obj[2] = &ice;
		p_obj[3] = &fire;
		p_obj[4] = &barrier;


		// 运行初始化
		system("mode con cols=140 lines=50 ");
		srand((unsigned int)time(NULL));

		CgameDisplay game_display;




		// 1. 主菜单
		game_display.MainMenuDisplay(1);
		char text_num = 0;

		while (char key = _getch()) {
			bool menu_break_sign = 0;
			switch (key) {
			case 72: {
				game_display.MainMenuDisplay();

				if (text_num == 0) text_num = 4;
				--text_num; text_num %= 4;
				switch (text_num) {
				case 0: LocWrite(st_menu_1, "开始游戏", 1); break;
				case 1: LocWrite(st_menu_2, "双蛇模式", 1); break;
				case 2: LocWrite(st_menu_3, "游戏设置", 1); break;
				case 3: LocWrite(st_menu_4, "退出游戏", 1); break;
				}
			
			}; break;
			case 80: {
				game_display.MainMenuDisplay();

				if (text_num == 0) text_num = 4;
				++text_num; text_num %= 4;
				switch (text_num) {
				case 0: LocWrite(st_menu_1, "开始游戏", 1); break;
				case 1: LocWrite(st_menu_2, "双蛇模式", 1); break;
				case 2: LocWrite(st_menu_3, "游戏设置", 1); break;
				case 3: LocWrite(st_menu_4, "退出游戏", 1); break;
				}

			}; break;
			case 13: menu_break_sign = 1; break; // 带着结果跑路
			}
			if (menu_break_sign) break;
		}
		game_display.ClearMenuDisplay();



		// 进入主菜单
		switch (text_num) {

		case START: {
			
			// 游戏开始 - 地图生成，音乐播放
			BackMusic();
			game_display.MapGen();
			// 计分板初始化
			game_display.ScoreDisplay();
			// 定时器初始化
			pre_time = (unsigned int)time(NULL);

			// 局部实例化-蛇体（结束-析构-销毁）  蛇生成
			Csnake qing({ 3,3 }, 's');

			// local var
			char move_dir;

			// 分支结构 - 读取文件开始/重新生成开始
			// 食物生成、地形生成
			COORD center_left_loc = { (map_size.X / 2) - 7,(map_size.Y / 2) - 2 };
			COORD center_right_loc = { (map_size.X / 2) + 1,(map_size.Y / 2) - 2 };
			LocWrite(center_left_loc, "继续上次游戏", 1);
			LocWrite(center_right_loc, "重新开始游戏");
			char load_text_num = 0;

			while (char key = _getch()) {
				bool menu_break_sign = 0;
				switch (key) {
				case 75: {// 去色、浮动、添色
					LocWrite(center_left_loc, "继续上次游戏");
					LocWrite(center_right_loc, "重新开始游戏");

					if (load_text_num == 0) load_text_num = 2;
					--load_text_num; load_text_num %= 2;
					switch (load_text_num) {
					case 0: LocWrite(center_left_loc, "继续上次游戏", 1); break;
					case 1: LocWrite(center_right_loc, "重新开始游戏", 1); break;
					}
				}break;
				case 77: {// 去色、浮动、添色
					LocWrite(center_left_loc, "继续上次游戏");
					LocWrite(center_right_loc, "重新开始游戏");

					if (load_text_num == 0) load_text_num = 2;
					++load_text_num;
					load_text_num %= 2;
					switch (load_text_num) {
					case 0: LocWrite(center_left_loc, "继续上次游戏", 1); break;
					case 1: LocWrite(center_right_loc, "重新开始游戏", 1); break;
					}
				}break;
				case 13:menu_break_sign = 1; break; // 带着结果跑路
				}
				if (menu_break_sign) break;
			}

			LocWrite(center_left_loc, "              ");
			LocWrite(center_right_loc, "              ");
			switch (load_text_num){
			case 0: {
				// 进行文件读取
				CfileOperate::FileRead(move_dir, qing, nor_food, shit, ice, fire, barrier);

			}break;
			case 1: {
				for (int i = 0; i < 5; i++) {
					p_obj[i]->Generate(qing.GetSnakeLoc());
				}
			}break;
			}

			do move_dir = _getch(); while (move_dir != 'w' && move_dir != 'a' && move_dir != 's' && move_dir != 'd');

			while (true) {
				// 检测暂停
				static char pre_dir;
				if (move_dir == 'w' || move_dir == 'a' || move_dir == 's' || move_dir == 'd') pre_dir = move_dir;
				if (move_dir == 27) {
					move_dir = pre_dir;
					char game_text_num = 0;
					bool game_menu_break_sign = 0;
					// 游戏内菜单 - 返回、取消
					LocWrite(st_menu_0, "=========STOPING=========");
					LocWrite(st_gm_menu_0, "返回主菜单", 1);
					LocWrite(st_gm_menu_1, "取消暂停");

					while (char game_key = _getch()) {
						switch (game_key) {
						case 75: {// 去色、浮动、添色
							LocWrite(st_gm_menu_0, "返回主菜单");
							LocWrite(st_gm_menu_1, "取消暂停");

							if (game_text_num == 0) game_text_num = 2;
							--game_text_num; game_text_num %= 2;
							switch (game_text_num) {
							case 0: LocWrite(st_gm_menu_0, "返回主菜单", 1); break;
							case 1: LocWrite(st_gm_menu_1, "取消暂停", 1); break;
							}
						}break;
						case 77: {// 去色、浮动、添色
							LocWrite(st_gm_menu_0, "返回主菜单");
							LocWrite(st_gm_menu_1, "取消暂停");

							if (game_text_num == 0) game_text_num = 2;
							++game_text_num;
							game_text_num %= 2;
							switch (game_text_num) {
							case 0: LocWrite(st_gm_menu_0, "返回主菜单", 1); break;
							case 1: LocWrite(st_gm_menu_1, "取消暂停", 1); break;
							}
						}break;
						case 13:game_menu_break_sign = 1; break; // 带着结果跑路
						}
						if (game_menu_break_sign) {
							game_menu_break_sign = 0;
							break;
						}
					}

					LocWrite(st_menu_0, "                            ");
					LocWrite(st_gm_menu_0, "                  ");
					LocWrite(st_gm_menu_1, "                  ");
					if (game_text_num == 0) game_over_sign = 1;
				}

				// 显示
				// 计分板显示
				game_display.ScoreDisplay();

				// 移动
				qing.SnakeMove(move_dir, nor_food.GetObjLoc(), fire.GetObjLoc());

				// 食物地形碰撞检测-效果
				for (int i = 0; i < 5; i++) {
					p_obj[i]->CrashBuff(qing.GetSnakeLoc());
				}

				// 到点清除效果
				SpeedResetTimer();

				// 杂项：玛卡巴卡时间
				Sleep(speed);

				if (_kbhit()) move_dir = _getch();


				if (game_over_sign) {
					speed = 120;
					game_over_sign = 0;

					// 文件自动保存操作
					CfileOperate::FileSave(pre_dir, qing.GetSnakeLoc(), nor_food.GetObjLoc(), shit.GetObjLoc(), ice.GetObjLoc(), fire.GetObjLoc(), barrier.GetObjLoc());
					for (int i = 0; i < 5; i++){
						p_obj[i]->ClearObj();
					}

					game_display.ClearGameDisplay();
					COORD tmp_center_loc = { (map_size.X / 2),(map_size.Y / 2) - 2 };
					LocWrite(tmp_center_loc, "游戏结束");
					system("pause > NULL");
					LocWrite(tmp_center_loc, "        ");
					break;
				}
			}

		} break;
		case DSNAKE: {
			// 2. 双蛇模式

			// 游戏开始 - 地图生成，音乐播放
			BackMusic();
			game_display.MapGen();
			// 计分板初始化
			game_display.ScoreDisplay();
			// 定时器初始化
			pre_time = (unsigned int)time(NULL);

			// 局部实例化-蛇体（结束-析构-销毁）  蛇生成
			Csnake qing({ 3,3 }, 's');
			Csnake_bai bai({ 40,20 }, 'w');

			// local var
			char move_dir;
			char move_dir1;

			// 分支结构 - 读取文件开始/重新生成开始
			// 食物生成、地形生成
			COORD center_left_loc = { (map_size.X / 2) - 7,(map_size.Y / 2) - 2 };
			COORD center_right_loc = { (map_size.X / 2) + 1,(map_size.Y / 2) - 2 };
			LocWrite(center_left_loc, "继续上次游戏", 1);
			LocWrite(center_right_loc, "重新开始游戏");
			char load_text_num = 0;

			while (char key = _getch()) {
				bool menu_break_sign = 0;
				switch (key) {
				case 75: {// 去色、浮动、添色
					LocWrite(center_left_loc, "继续上次游戏");
					LocWrite(center_right_loc, "重新开始游戏");

					if (load_text_num == 0) load_text_num = 2;
					--load_text_num; load_text_num %= 2;
					switch (load_text_num) {
					case 0: LocWrite(center_left_loc, "继续上次游戏", 1); break;
					case 1: LocWrite(center_right_loc, "重新开始游戏", 1); break;
					}
				}break;
				case 77: {// 去色、浮动、添色
					LocWrite(center_left_loc, "继续上次游戏");
					LocWrite(center_right_loc, "重新开始游戏");

					if (load_text_num == 0) load_text_num = 2;
					++load_text_num;
					load_text_num %= 2;
					switch (load_text_num) {
					case 0: LocWrite(center_left_loc, "继续上次游戏", 1); break;
					case 1: LocWrite(center_right_loc, "重新开始游戏", 1); break;
					}
				}break;
				case 13:menu_break_sign = 1; break; // 带着结果跑路
				}
				if (menu_break_sign) break;
			}
			LocWrite(center_left_loc, "              ");
			LocWrite(center_right_loc, "              ");

			switch (load_text_num) {
			case 0: {
				// 进行文件读取
				CfileOperate::FileRead(move_dir, move_dir1, qing, bai, nor_food, shit, ice, fire, barrier);

			}break;
			case 1: {
				for (int i = 0; i < 5; i++) {
					p_obj[i]->Generate(qing.GetSnakeLoc(), bai.GetSnakeLoc());
				}
			}break;
			}

			do move_dir = _getch(); while (move_dir != 'w' && move_dir != 'a' && move_dir != 's' && move_dir != 'd');
			do move_dir1 = _getch(); while (move_dir1 != 'j' && move_dir1 != 'k' && move_dir1 != 'l' && move_dir1 != 'i');

			while (true) {
				// 检测暂停
				static char pre_dir;
				static char pre_dir1;
				if (move_dir == 'w' || move_dir == 'a' || move_dir == 's' || move_dir == 'd') pre_dir = move_dir;
				if (move_dir1 == 'j' || move_dir1 == 'k' || move_dir1 == 'l' || move_dir1 == 'i') pre_dir1 = move_dir1;
				if (move_dir == 27) {
					move_dir = pre_dir;
					move_dir1 = pre_dir1;
					char game_text_num = 0;
					bool game_menu_break_sign = 0;
					// 游戏内菜单 - 返回、取消
					LocWrite(st_menu_0, "=========STOPING=========");
					LocWrite(st_gm_menu_0, "返回主菜单", 1);
					LocWrite(st_gm_menu_1, "取消暂停");

					while (char game_key = _getch()) {
						switch (game_key) {
						case 75: {// 去色、浮动、添色
							LocWrite(st_gm_menu_0, "返回主菜单");
							LocWrite(st_gm_menu_1, "取消暂停");

							if (game_text_num == 0) game_text_num = 2;
							--game_text_num; game_text_num %= 2;
							switch (game_text_num) {
							case 0: LocWrite(st_gm_menu_0, "返回主菜单", 1); break;
							case 1: LocWrite(st_gm_menu_1, "取消暂停", 1); break;
							}
						}break;
						case 77: {// 去色、浮动、添色
							LocWrite(st_gm_menu_0, "返回主菜单");
							LocWrite(st_gm_menu_1, "取消暂停");

							if (game_text_num == 0) game_text_num = 2;
							++game_text_num;
							game_text_num %= 2;
							switch (game_text_num) {
							case 0: LocWrite(st_gm_menu_0, "返回主菜单", 1); break;
							case 1: LocWrite(st_gm_menu_1, "取消暂停", 1); break;
							}
						}break;
						case 13:game_menu_break_sign = 1; break; // 带着结果跑路
						}
						if (game_menu_break_sign) {
							game_menu_break_sign = 0;
							break;
						}
					}

					LocWrite(st_menu_0, "                            ");
					LocWrite(st_gm_menu_0, "                  ");
					LocWrite(st_gm_menu_1, "                  ");
					if (game_text_num == 0) game_over_sign = 1;
				}

				// 显示
				// 计分板显示
				game_display.ScoreDisplay();

				// 移动
				qing.SnakeMove(pre_dir, nor_food.GetObjLoc(), fire.GetObjLoc());
				bai.SnakeMove(pre_dir1, nor_food.GetObjLoc(), fire.GetObjLoc());

				// 食物地形碰撞检测-效果
				for (int i = 0; i < 5; i++) {
					p_obj[i]->CrashBuff(qing.GetSnakeLoc());
					p_obj[i]->CrashBuff(bai.GetSnakeLoc());
				}

				// 碰撞结果判定
				qing.SnakeCrashDete(bai.GetSnakeLoc());
				bai.SnakeCrashDete(qing.GetSnakeLoc());

				// 到点清除效果
				SpeedResetTimer();

				// 杂项：玛卡巴卡时间
				Sleep(speed);

				if (_kbhit()) move_dir = _getch();
				move_dir1 = move_dir;

				if (game_over_sign) {
					speed = 120;
					game_over_sign = 0;

					// 文件自动保存操作
					CfileOperate::FileSave(pre_dir, pre_dir1, qing.GetSnakeLoc(), bai.GetSnakeLoc(), nor_food.GetObjLoc(), shit.GetObjLoc(), ice.GetObjLoc(), fire.GetObjLoc(), barrier.GetObjLoc());
					for (int i = 0; i < 5; i++) {
						p_obj[i]->ClearObj();
					}

					game_display.ClearGameDisplay();
					COORD tmp_center_loc = { ( map_size.X / 2 ), (map_size.Y / 2) - 2 };
					LocWrite(tmp_center_loc, "游戏结束");
					system("pause > NULL");
					LocWrite(tmp_center_loc, "        ");
					break;
				}
			}
		} break;
		case OPTIONS: {

			// 3. 子菜单 - 地图设置
			game_display.OptMenuDisplay(1);
			char text_num = 0;
			bool menu_break_sign = 0;

			while (char key = _getch()) {
				switch (key) {
				case 72: {
					game_display.OptMenuDisplay();

					if (text_num == 0) text_num = 7;
					--text_num; text_num %= 7;
					switch (text_num) {
					case 0: LocWrite(st_menu_1, "地图障碍物自定义", 1); break;
					case 1: LocWrite(st_menu_2, "果实数量", 1); break;
					case 2: LocWrite(st_menu_3, "shit数量", 1); break;
					case 3: LocWrite(st_menu_4, "冰块数量", 1); break;
					case 4: LocWrite(st_menu_5, "火焰数量", 1); break;
					case 5: LocWrite(st_menu_6, "障碍物数量", 1); break;
					case 6: LocWrite(st_menu_7, "退出设置", 1); break;
					}

				}; break;
				case 80: {
					game_display.OptMenuDisplay();

					if (text_num == 0) text_num = 7;
					++text_num; text_num %= 7;
					switch (text_num) {
					case 0: LocWrite(st_menu_1, "地图障碍物自定义", 1); break;
					case 1: LocWrite(st_menu_2, "果实数量", 1); break;
					case 2: LocWrite(st_menu_3, "shit数量", 1); break;
					case 3: LocWrite(st_menu_4, "冰块数量", 1); break;
					case 4: LocWrite(st_menu_5, "火焰数量", 1); break;
					case 5: LocWrite(st_menu_6, "障碍物数量", 1); break;
					case 6: LocWrite(st_menu_7, "退出设置", 1); break;
					}

				}; break;
				case 13: {// 选择
					switch (text_num) {
					case 0: {

						HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
						INPUT_RECORD stcRecord = { 0 };
						DWORD dwRead;
						SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
						short sign = 0;

						while (char break_sign = 1) {
							ReadConsoleInput(hStdin, &stcRecord, 1, &dwRead);

							if (stcRecord.EventType == MOUSE_EVENT) {
								MOUSE_EVENT_RECORD mer = stcRecord.Event.MouseEvent;

								switch (sign){
								case 0: {
									if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
										mer.dwMousePosition.X /= 2;
										nor_food.AddObj(mer.dwMousePosition);
										LocWrite(mer.dwMousePosition, nor_food.s_buff);
									}
									else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
										mer.dwMousePosition.X /= 2;
										nor_food.PopObj(mer.dwMousePosition);
										LocWrite(mer.dwMousePosition, "  ");
									}
								}break;
								case 1: {
									if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
										mer.dwMousePosition.X /= 2;
										shit.AddObj(mer.dwMousePosition);
										LocWrite(mer.dwMousePosition, shit.s_buff);
									}
									else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
										mer.dwMousePosition.X /= 2;
										shit.PopObj(mer.dwMousePosition);
										LocWrite(mer.dwMousePosition, "  ");
									}
								}break;
								case 2: {
									if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
										mer.dwMousePosition.X /= 2;
										ice.AddObj(mer.dwMousePosition);
										LocWrite(mer.dwMousePosition, ice.s_buff);
									}
									else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
										mer.dwMousePosition.X /= 2;
										ice.PopObj(mer.dwMousePosition);
										LocWrite(mer.dwMousePosition, "  ");
									}
								}break;
								case 3: {
									if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
										mer.dwMousePosition.X /= 2;
										fire.AddObj(mer.dwMousePosition);
										LocWrite(mer.dwMousePosition, fire.s_buff);
									}
									else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
										mer.dwMousePosition.X /= 2;
										fire.PopObj(mer.dwMousePosition);
										LocWrite(mer.dwMousePosition, "  ");
									}
								}break;
								case 4: {
									if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
										mer.dwMousePosition.X /= 2;
										barrier.AddObj(mer.dwMousePosition);
										LocWrite(mer.dwMousePosition, barrier.s_buff);
									}
									else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
										mer.dwMousePosition.X /= 2;
										barrier.PopObj(mer.dwMousePosition);
										LocWrite(mer.dwMousePosition, "  ");
									}
								}break;
								default:break;
								}
								
								
							}
							else if (stcRecord.EventType == KEY_EVENT) {
								KEY_EVENT_RECORD ker = stcRecord.Event.KeyEvent;
								if (ker.bKeyDown) {

									switch (ker.wVirtualKeyCode){
									case VK_ESCAPE: break_sign = 0; break;
									case 0x31: sign = 0; break;
									case 0x32: sign = 1; break;
									case 0x33: sign = 2; break;
									case 0x34: sign = 3; break;
									case 0x35: sign = 4; break;
									}
									
									
								}
							}

							if (!break_sign) break;

						}

					}break;
					case 1: {
						short tmp = st_menu_2.X + 5;
						LocWrite({ tmp, st_menu_2.Y }, ": ");
						std::cin >> num_nor_food;
						LocWrite({ tmp, st_menu_2.Y }, "设置成功");
						system("pause > NULL");
						LocWrite({ tmp, st_menu_2.Y }, "        ");
						LocWrite(st_menu_2, "果实数量", 1);
					} break;
					case 2: {
						short tmp = st_menu_3.X + 5;
						LocWrite({ tmp, st_menu_3.Y }, ": ");
						std::cin >> num_shit;
						LocWrite({ tmp, st_menu_3.Y }, "设置成功");
						system("pause > NULL");
						LocWrite({ tmp, st_menu_3.Y }, "        ");
						LocWrite(st_menu_3, "shit数量", 1);

					} break;
					case 3: {
						short tmp = st_menu_4.X + 5;
						LocWrite({ tmp, st_menu_4.Y }, ": ");
						std::cin >> num_ice;
						LocWrite({ tmp, st_menu_4.Y }, "设置成功");
						system("pause > NULL");
						LocWrite({ tmp, st_menu_4.Y }, "        ");
						LocWrite(st_menu_4, "冰块数量", 1);

					} break;
					case 4: {
						short tmp = st_menu_5.X + 5;
						LocWrite({ tmp, st_menu_5.Y }, ": ");
						std::cin >> num_fire;
						LocWrite({ tmp, st_menu_5.Y }, "设置成功");
						system("pause > NULL");
						LocWrite({ tmp, st_menu_5.Y }, "        ");
						LocWrite(st_menu_5, "火焰数量", 1);

					} break;
					case 5: {
						short tmp = st_menu_6.X + 5;
						LocWrite({ tmp, st_menu_6.Y }, ": ");
						std::cin >> num_barrier;
						LocWrite({ tmp, st_menu_6.Y }, "设置成功");
						system("pause > NULL");
						LocWrite({ tmp, st_menu_6.Y }, "        ");
						LocWrite(st_menu_6, "障碍物数量", 1);

					} break;
					case 6: menu_break_sign = 1; break;
					} break;
				}
				}
				if (menu_break_sign) {
					game_display.ClearMenuDisplay();
					break;
				}

			} break;
		}break;
		case EXIT: break_sign = 1; break; // 4. 退出

		}
		 if (break_sign) break;
	}
	return 0;
}
