#include "Windows.h"
#include <random>
#include <iostream>
#include <time.h>
#include <map>
#include <stdio.h>
#include <algorithm>
using namespace std;

	enum JANKENTYPE
	{
		GUU,
		TYOKI,
		PAA,
		JANKEN_TYPEMAX,
	};

	enum RESULTTYPE
	{
		WIN,
		LOSE,
		DRAW,
		RESULT_TYPEMAX,
	};

#define RESULT_INFOMAX 10

class JankenKind
{
public:
	JankenKind();
	JankenKind(JANKENTYPE myHand, JANKENTYPE winHand, JANKENTYPE loseHand, string myHandStr)
	{
		m_myHandStr = myHandStr;
		judgeInfo[(int)myHand] = "引き分け";
		judgeInfo[(int)winHand] = "勝ち";
		judgeInfo[(int)loseHand] = "負け";

		judgeInfoType[(int)myHand] = DRAW;
		judgeInfoType[(int)winHand] = WIN;
		judgeInfoType[(int)loseHand] = LOSE;
	}
public:
	map<int, string> judgeInfo;
	map<int, int> judgeInfoType;
	string m_myHandStr;
};

// じゃんけんの出した手
class JankenHand
{
public:
	JankenHand()
	{
		m_myHand = NULL;
	}
	~JankenHand();

	// 出した手を設定
	void SetMyHand(JankenKind *myHand)
	{
		m_myHand = myHand;
	}
	// 出した手を取得
	JankenKind *GetMyHand()
	{
		return m_myHand;
	}

private:
	JankenKind *m_myHand;

};

struct Rand
{
	int GetRandom(int min, int max)
	{
		std::random_device randomSeed;
		std::mt19937_64 mt(randomSeed());
		std::uniform_int_distribution<> randRange(min, max);
		return randRange(mt);
	}
};


class Result
{
private:
	int NowResultCount(int num)
	{
		int count = num;
		count -= m_resultLimitCont;
		count = std::clamp(count, m_minNum, m_minNum + (m_resultLimitCont - 1));
		return  count;
	}
public:
	Result() {
		m_resultType[WIN] = "勝ち";
		m_resultType[LOSE] = "負け";
		m_resultType[DRAW] = "引き分け";
	}
	~Result();
	void Init(int num)
	{
		m_minNum = NowResultCount(num);
	}
	// リザルトの追加
	void AddResult(int num, int _resultType)
	{
		m_results[num] = _resultType;
		m_resultsDetail[_resultType]++;
		m_maxNum = m_results.size() + m_minNum;
	}
	// リザルトの表示
	void ShowResultsDetail()
	{
		for (int i = m_minNum; i < m_maxNum; i++)
		{
			int resultType = m_results[i];
			auto result = m_resultType[resultType];
			cout << i + 1 << "戦目" << result.c_str() << "\n";
		}
	}
	void ShowResults()
	{
		int w = m_resultsDetail[WIN];
		int l = m_resultsDetail[LOSE];
		int d = m_resultsDetail[DRAW];
		cout << "勝ち:" << w << "," << "負け:" << l << "," << "引き分け" << d << "\n";
	}
private:
	map<int, int> m_results;
	int m_resultsDetail[RESULT_TYPEMAX];
	map<int, string> m_resultType;
	const int m_resultLimitCont = RESULT_INFOMAX;
	int m_minNum;
	int m_maxNum;
	int m_ResultLimitArrival;
};

int main()
{
	JankenKind *jankenHand[JANKEN_TYPEMAX];
	jankenHand[GUU] = new JankenKind(GUU, TYOKI, PAA, "グー");
	jankenHand[TYOKI] = new JankenKind(TYOKI, PAA, GUU, "チョキ");
	jankenHand[PAA] = new JankenKind(PAA, GUU, TYOKI, "パー");

	// 戦績
	Result *result = new Result();
	JankenHand *user = new JankenHand();
	JankenHand *pc = new JankenHand();
	Rand rnd;
	int pcHandNum;
	int myHandNum;

	char userHand[256];
	int num = 0;
	// リザルトの初期化
	result->Init(num);
	while (true)
	{
		cout << "0未選択, 1:グー, 2:チョキ, 3:パーのいずれかを入力してください\nRは戦績を表示します\n";
		cin >> userHand;
		// ユーザーの入力
		myHandNum = atoi(userHand);
		if (myHandNum <= 0 || myHandNum > JANKEN_TYPEMAX)
		{
			cout << "1:グー, 2:チョキ, 3:パー以外は戦績につきません。\n";
			if (strcmp(userHand, "R") == 0)
			{
				// リザルトの初期化
				result->Init(num);
				result->ShowResultsDetail();
			}
			continue;
		}
		myHandNum = myHandNum - 1;
		// PCの結果
		pcHandNum = rnd.GetRandom(GUU,PAA);
		// じゃんけんの手を設定
		user->SetMyHand(jankenHand[myHandNum]);
		pc->SetMyHand(jankenHand[pcHandNum]);
		cout << "あなたは" << user->GetMyHand()->m_myHandStr.c_str() << "です。\n";
		cout << "PCは" << pc->GetMyHand()->m_myHandStr.c_str() << "です。\n";
		// 勝敗判定
		string judge = user->GetMyHand()->judgeInfo[pcHandNum];
		cout << "結果は" << judge.c_str() << "\n";

		// リザルト保存
		result->AddResult(num, user->GetMyHand()->judgeInfoType[pcHandNum]);
		// 戦績表示
		result->ShowResults();
		num++;
	}
}