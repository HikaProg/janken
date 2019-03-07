#include "Windows.h"
#include <random>
#include <iostream>
#include <time.h>
#include <map>
using namespace std;

	enum JANKENTYPE
	{
		GUU,
		TYOKI,
		PAA,
		JANKEN_TYPEMAX
	};

struct Janken
{
	Janken();
	Janken(JANKENTYPE myHand, JANKENTYPE winHand, JANKENTYPE loseHand, string myHandStr)
	{
		m_myHandStr = myHandStr;
		judgeInfo[(int)myHand] = "引き分け";
		judgeInfo[(int)winHand] = "勝ち";
		judgeInfo[(int)loseHand] = "負け";
	}
public:
	map<int, string> judgeInfo;
	string m_myHandStr;
};

struct Rand
{
	bool flag;
	int GetRandom(int min, int max)
	{
		if (!flag)
		{
			srand((unsigned int)time(NULL));
			flag = true;
		}
		return min + (int)rand()*(max - min + 1.0) / (1.0 + RAND_MAX);
	}
};

struct Result
{
	string result[30];
	// リザルトの追加
	void AddResult(int num, string _result)
	{
		result[num] = _result;
	}
	// リザルトの表示
	void ShowResult()
	{
		int resultMax = sizeof(result) / sizeof(string);
		for (int i = 1; i <= resultMax; i++)
		{
			cout << i << "戦目" << result[i].c_str() << "\n";
		}
	}
};

int main()
{
	Janken *jankenHand[JANKEN_TYPEMAX];
	jankenHand[GUU] = new Janken(GUU, TYOKI, PAA, "グー");
	jankenHand[TYOKI] = new Janken(TYOKI, PAA, GUU, "チョキ");
	jankenHand[PAA] = new Janken(PAA, GUU, TYOKI, "パー");

	char userHand[256];
	// 戦績
	Result result;
	int num = 0;
	while (true)
	{
		cout << "1:グー, 2:チョキ, 3:パーのいずれかを入力してください\nRは戦績を表示します\n";
		cin >> userHand;
		// ユーザーの入力
		int myHandNum = atoi(userHand);
		
		if (myHandNum <= 0 || myHandNum > JANKEN_TYPEMAX)
		{
			cout << "1,2,3以外は戦績につきません。\n";
			if(strcmp(userHand, "R") == 0)
				result.ShowResult();
			continue;
		}
		myHandNum = myHandNum - 1;
		Rand rnd;
		// PCの結果
		int pcHandNum = rnd.GetRandom(GUU, PAA);
		cout << "PCは" << jankenHand[pcHandNum]->m_myHandStr.c_str() << "です。\n";
		string judge;
		// 勝敗判定
		judge = jankenHand[myHandNum]->judgeInfo[pcHandNum];
		cout << "結果は" << judge.c_str() << "\n";
		num++;
		// リザルト保存
		result.AddResult(num, judge);
	}
}