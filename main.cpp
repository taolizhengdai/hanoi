#include"draw.h"
//void honoi(int n)
//{
//	typedef struct _status
//	{
//		int num;
//		char* from;
//		char* via;
//		char* to;
//	}status;
//	struct stautsStack
//	{
//		status* a;
//	};
//	return;
//
//}
int main()
{
	//initgraph(1280, 800);
	//cleardevice();
	//settextstyle(30,10,_T("Kreon"));
	//getchar();
	int n = 1;
	printf("input the disks' number that you need: ");
	scanf_s(" %d", &n);
	render_all(&n);

	//scanf("%d", &n);
	/*render_disk(n);*/
	/*getchar();*/
	/*getchar();*/
	return 0;
}