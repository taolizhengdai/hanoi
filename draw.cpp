#include <easyx.h>
#include <math.h>
#include <stdio.h>
#define WINDOWWIDTH (1800)
#define WINDOWHEIGHT (800)
#define UNDERDISK (110)
#define TOWERY 300
#define TOWERRAD WINDOWWIDTH / 6
#define RECTWIDTH 10
#define XAXIS true
#define YAXIS false
#define POSITIVE 1  // 正方向判断
#define NEGATIVE -1 // 反方向
typedef struct _disk disk;
typedef struct _diskStack diskStack;
typedef struct _tower tower;
typedef struct _diskvalue diskvalue;
struct _diskvalue {
  int basicRad;
  int height;
};
// 圆盘半径与高度
struct _disk {
  int serial;
  disk *next;
};
// 圆盘链
struct _diskStack {
  int num;
  disk *top;
};
// 单座塔的栈
struct _tower {
  int x;
  diskStack stk;
};
diskvalue diskvul;
tower from, via, to;
COLORREF selectColor[3] = {RED, BLUE, YELLOW};
void init_disk_value(int *n) {
  if (*n < 3)
    diskvul.height = ((WINDOWHEIGHT - RECTWIDTH - TOWERY) / (3)) + 1;
  // 圆盘数低于3时，圆盘高度过高，直接按3计算
  else
    diskvul.height = ((WINDOWHEIGHT - RECTWIDTH - TOWERY) / (*n)) + 1;
  diskvul.basicRad = (TOWERRAD) / (*n);
}
// 圆盘数值初始化计算
int get_disk_basicRad() { return diskvul.basicRad; }
// 获取圆盘半径
int get_disk_height() { return diskvul.height; }
// 获取圆盘高度
void init_tower(int *n) {
  from.x = 300;
  via.x = 900;
  to.x = 1500;
  via.stk.num = 0;
  via.stk.top = NULL;
  to.stk.num = 0;
  to.stk.top = NULL;
  from.stk.num = *n;
  from.stk.top = (disk *)(malloc(sizeof(disk)));
  disk *fp = from.stk.top;
  for (int i = 1; i <= (*n); ++i) {
    fp->serial = i;
    fp->next = (disk *)(malloc(sizeof(disk)));
    fp = fp->next;
  }
  fp->next = NULL;
}
// 三座塔的初始化
void create_window() {
  initgraph(WINDOWWIDTH, WINDOWHEIGHT);
  setbkcolor(0xCDDEC2);
  cleardevice();
}
// 窗口创建
void render_text() {
  settextcolor(BLACK);
  settextstyle(30, 18, _T("微软雅黑"));
  outtextxy(
      0, 0,
      _T("按n（next）可以继续动画,动画进行期间再次按n可以直接进入下个状态"));
  outtextxy(0, 31, _T("按u(undo)可以返回上一状态"));
}

void render_one_tower(tower *t) {
  fillrectangle((*t).x - RECTWIDTH / 2, TOWERY, (*t).x + RECTWIDTH / 2,
                WINDOWHEIGHT - RECTWIDTH);
}
// 单座塔的绘制
void render_tower(tower *t1, tower *t2, tower *t3) {
  render_one_tower(t1);
  render_one_tower(t2);
  render_one_tower(t3);
}
// 三座塔的绘制
void render_one_disk(disk *fp, int x, int y, int right, int bottom) {
  int snum = fp->serial;
  setfillcolor(selectColor[(snum) % 3]);
  fillrectangle(x, y, right, bottom);
}
// 单圆盘绘制
void render_basic() {
  setfillcolor(BLACK);
  fillrectangle(0, WINDOWHEIGHT - RECTWIDTH, WINDOWWIDTH, WINDOWHEIGHT);
  render_tower(&from, &via, &to);
  render_text();
}
// 简单的初始界面绘制
void render_onetower_disk(tower *t, int *n) {
  disk *fp = (*t).stk.top;
  int num = (*t).stk.num;
  int basicHeight;
  if (*n < 3)
    basicHeight = TOWERY + (3 - num) * get_disk_height();
  else
    basicHeight = TOWERY + ((*n) - num) * get_disk_height();
  for (int i = 1; i <= num; ++i) {
    if (fp) {
      render_one_disk(fp, (*t).x - (fp->serial) * get_disk_basicRad(),
                      basicHeight + (i - 1) * get_disk_height(),
                      (*t).x + (fp->serial) * get_disk_basicRad(),
                      basicHeight + i * get_disk_height());
      fp = fp->next;
    }
  }
}
// 绘制单座塔的圆盘
void render_disk(tower *from, tower *via, tower *to, int *n) {
  render_onetower_disk(from, n);
  render_onetower_disk(via, n);
  render_onetower_disk(to, n);
}
// 圆盘绘制
void render_unmove(int *n) {
  render_basic();
  render_tower(&from, &via, &to);
  render_disk(&from, &via, &to, n);
}
// 渲染不会动的

void render_onedrection_move(int *o1, int *o2, int *c, bool axis, int direction,
                             disk *fp, int *n) {
  int speed = direction * 20;
  if (abs(*o2 - *o1 - speed) > speed)
    *o1 += speed;
  else
    *o1 = *o2;
  cleardevice();
  render_unmove(n);
  if (axis) {
    render_one_disk(fp, *o1, *c, *o1 + 2 * fp->serial * get_disk_basicRad(),
                    *c + get_disk_height());
  } else {
    render_one_disk(fp, *c, *o1, *c + 2 * fp->serial * get_disk_basicRad(),
                    *o1 + get_disk_height());
  }
}
bool render_horizontal_move(int *x1, int *x2, int *y, disk *fp, int *n) {
  if (*x1 > *x2) {
    render_onedrection_move(x1, x2, y, XAXIS, NEGATIVE, fp, n);
    return true;
  } else if (*x1 < *x2) {
    render_onedrection_move(x1, x2, y, XAXIS, POSITIVE, fp, n);
    return true;
  } else
    return false; // 移动结束
}
// 水平移动，返回false表示移动结束
bool render_vertical_move(int *y1, int *y2, int *x, disk *fp, int *n) {
  if (*y1 > *y2) {
    render_onedrection_move(y1, y2, x, YAXIS, NEGATIVE, fp, n);
    return true;
  } else if (*y1 < *y2) {
    render_onedrection_move(y1, y2, x, YAXIS, POSITIVE, fp, n);
    return true;
  } else
    return false; // 移动结束
}
// 竖直移动，返回false表示移动结束
bool render_disk_move(int *sx, int *sy, int *tx, int *ty, disk *fp, int *n) {
  ExMessage msg = {0};
  int y = UNDERDISK; // 圆盘竖直移动最高点
  BeginBatchDraw();
  while (render_vertical_move(sy, &y, sx, fp, n)) {
    FlushBatchDraw();
    peekmessage(&msg, EX_KEY);
    if (msg.vkcode == 'N')
      return true;
    /*render_vertical_move(sy, &y, sx, fp);*/
    Sleep(30);
    msg.vkcode = 0;
  }
  while (render_horizontal_move(sx, tx, &y, fp, n)) {
    FlushBatchDraw();
    peekmessage(&msg, EX_KEY);
    if (msg.vkcode == 'N')
      return true;
    // msg.vkcode = 0;
    Sleep(30);
  }
  while (render_vertical_move(&y, ty, tx, fp, n)) {
    FlushBatchDraw();
    peekmessage(&msg, EX_KEY);
    if (msg.vkcode == 'N')
      return true;
    // msg.vkcode = 0;
    Sleep(30);
  }
  return false;
  /*EndBatchDraw();*/
}

tower *select_tower(int num, int *n) {
  if ((*n) % 2 == 0) {
    if (num == 0)
      return &from;
    else if (num == 1)
      return &to;
    else if (num == 2)
      return &via;
  } else {
    if (num == 0)
      return &from;
    else if (num == 1)
      return &via;
    else if (num == 2)
      return &to;
  }
}
int source_pegs(int *i) { return (*i & (*i - 1)) % 3; }
int target_pegs(int *i) { return ((*i | (*i - 1)) + 1) % 3; }
tower *source_tower(int *i, int *n) { return select_tower(source_pegs(i), n); }
tower *target_tower(int *i, int *n) { return select_tower(target_pegs(i), n); }
void move_disk(tower *source, tower *target) {
  disk *fp1 = source->stk.top;
  disk *fp2 = target->stk.top;
  if (fp1) {
    source->stk.num--;
    target->stk.num++;
    source->stk.top = fp1->next;
    fp1->next = fp2;
    target->stk.top = fp1;
  } else {
    return;
  } //???
}
void target_source(int *i, int *n, bool reverse) {
  tower *source = source_tower(i, n);
  tower *target = target_tower(i, n);
  if (reverse)
    move_disk(target, source);
  else
    move_disk(source, target);
}
void render_status(int *n) {
  BeginBatchDraw();
  cleardevice();
  render_basic();
  render_disk(&from, &via, &to, n);
  FlushBatchDraw();
  EndBatchDraw();
}
// 渲染圆盘状态
void render_quickmove(int *i, int *n) {
  target_source(i, n, false);
  render_status(n);
}
// 快速移动渲染
void render_slowmove(int *i, int *n) {
  tower *source = source_tower(i, n);
  tower *target = target_tower(i, n);
  // move_disk(source, target);
  disk *fp = source->stk.top;
  if (fp) {
    int sx = source->x - fp->serial * get_disk_basicRad();
    int sy = TOWERY + (*n - source->stk.num) * get_disk_height();
    source->stk.top = fp->next;
    source->stk.num--;
    int tx = target->x - (fp->serial) * get_disk_basicRad();
    int ty = TOWERY + (*n - target->stk.num - 1) * get_disk_height();
    if (render_disk_move(&sx, &sy, &tx, &ty, fp, n)) {
      source->stk.top = fp;
      source->stk.num++;
      render_quickmove(i, n);
      FlushBatchDraw();
      EndBatchDraw();
      return;
    }
    fp->next = target->stk.top;
    target->stk.top = fp;
    target->stk.num++;
  }
}
// 圆盘缓慢移动渲染
void render_undo_move(int *i, int *n) {
  //(*i)--;
  target_source(i, n, true);
  render_status(n);
}
// 取消移动渲染
void init(int *n) {
  create_window();
  init_disk_value(n);
  init_tower(n);
  render_basic();
  render_disk(&from, &via, &to, n);
}
// 初始化
void render_all(int *n) {
  init(n);
  ExMessage msg = {0};
  getmessage(&msg, EX_KEY);
  unsigned long long processnum = pow(2, *n) - 1; // 移动总需步骤数
  // BeginBatchDraw();
  for (int i = 1; i <= processnum + 1;
       ++i) // 为什么最后i<=
            // processnum+1，因为要允许移动结束后可以再进入循环进行撤回等操作
  {
    getmessage(&msg, EX_KEY);
    if (msg.message == WM_KEYDOWN) {
      getmessage(&msg, EX_KEY);
      switch (msg.vkcode) {
      case 'N':
        if (i <= processnum) {
          render_slowmove(&i, n);
        } else
          --i;
        break;
      case 'U':
        if (i > 1) {
          --i;
          render_undo_move(&i, n); // 这里面会使i-1，绘制上一步的状态
        }
        --i;
        break;
      default:
        --i;
        // 无效按键修改i的值，以免偏移
      }
    } else if (msg.message == WM_KEYUP) {
      --i;
    }
    msg.message = 0;
    msg.vkcode = 0;
  }
}
