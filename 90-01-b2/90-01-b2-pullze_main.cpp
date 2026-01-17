/* 2353626 贺胡鸣 大数据 */
#include "90-01-b2-pullze.h"
using namespace std;
/***************************************************************************
  函数名称：to_be_continued
  功    能：显示提示信息
  输入参数：const char* prompt, const int X=0, const int Y=22
  返 回 值：/
  说    明：如果读不懂，暂时放着，等指针和引用学习完成后再阅读
***************************************************************************/
static void to_be_continued(const char* prompt, const int X = state_X, const int Y = state_Y)
{
	cct_setcolor(); //恢复缺省颜色
	cct_gotoxy(X, Y);

	if (prompt)
		cout << prompt << "按回车键继续...";
	else
		cout << "按回车键继续...";

	/* 忽略除回车键外的所有输入（注意：_getch的回车是\r，而getchar是\n）*/
	while (_getch() != '\r')
		;

	cct_cls();

	return;
}

/***************************************************************************
  函数名称：main
  功    能：从菜单读入选择，并调用各自函数
  输入参数：/
  返 回 值：/
  说    明：/
***************************************************************************/
int main()
{
	//通过菜单读入选择
	char choice;
	while (1) {
		choice = menu(pullze_prompts, 2);
		if (choice == 'Q') {
			cct_cls();
			to_be_continued("");
			return 0;
		}
		else if (choice == 'A')
			choiceA();
		else if (choice == 'B') {
			choiceB('B');
			end();
		}
		else if (choice == 'C') {
			choiceB('C');
		}
		else if (choice == 'D') {
			choiceD();
			cout << endl;
			end();
			recover();
		}
		else if (choice == 'E') {
			choiceEI('E');
			cout << endl;
			end();
			recover();
		}
		else if (choice == 'F') {
			choiceEI('F');
			cout << endl;
			end();
			recover();
		}
		else if (choice == 'G') {
			choiceEI('G');
			cout << endl;
			end();
			recover();
		}
		else if (choice == 'H') {
			choiceH();
			cout << endl;
			end();
			recover();
		}
		else if (choice == 'I') {
			choiceEI('I');
			cout << endl;
			end();
			recover();
		}
		else if (choice == 'J') {
			choiceEI('J');
			cout << endl;
			end();
			recover();
		}
		else if (choice == 'K') {
			choiceEI('K');
			cout << endl;
			end();
			recover();
		}
	}
	return 0;
}