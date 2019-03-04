#include "Windows.h"
#include <random>
#include <iostream>
#include <time.h>
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
	void Init(string myHand, string winHand, string loseHand)
	{
		myHandStr += myHand;
		winHandStr += winHand;
		loseHandStr += loseHand;
	}
	string myHandStr = "";
	string winHandStr = "";
	string loseHandStr = "";
	string WinJudgeHand(string hand)
	{
		return ((hand == winHandStr) ? "勝ち！" : "");
	}
	string LoseJudgeHand(string hand)
	{
		return ((hand == loseHandStr) ? "負け..." : "");
	}
	string DrawJudgeHand(string hand)
	{
		return ((hand == myHandStr) ? "引き分け" : "");
	}
};

struct Rand
{
	int GetRandom(int min, int max)
	{
		static int flag;
		if (flag == 0)
		{
			srand((unsigned int)time(NULL));
			flag = 1;
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
	Janken hand[JANKEN_TYPEMAX];
	hand[GUU].Init("グー", "チョキ", "パー");
	hand[TYOKI].Init("チョキ", "パー", "グー");
	hand[PAA].Init("パー", "グー", "チョキ");

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
		Rand rnd;
		// PCの結果
		int pcHandNum = rnd.GetRandom(0, 2);
		cout << "PCは" << hand[pcHandNum].myHandStr.c_str() << "です。\n";
		myHandNum = myHandNum - 1;
		string judge;
		// 勝敗判定
		judge += hand[myHandNum].DrawJudgeHand(hand[pcHandNum].myHandStr);
		judge += hand[myHandNum].WinJudgeHand(hand[pcHandNum].myHandStr);
		judge += hand[myHandNum].LoseJudgeHand(hand[pcHandNum].myHandStr);
		cout << "結果は" << judge.c_str() << "\n";
		num++;
		// リザルト保存
		result.AddResult(num, judge);
	}
}