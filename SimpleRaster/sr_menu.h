#ifndef __SR_MENU_H__
#define __SR_MENU_H__

//-----------------------------------------------------------------------------
// 菜单
//-----------------------------------------------------------------------------

#define MENU_STR_LEN 64

// 选择菜单
#define SELECT_MENU(m) selectMenu(m, sizeof(m)/sizeof(m[0]))

// 标准输入字符串
const char *inputStr(const char *prompt);

// 标准输入整数
int inputInt(const char *prompt);

// 选择菜单
int selectMenu(char (*menu)[MENU_STR_LEN], int count);

//-----------------------------------------------------------------------------

#endif // __SR_MENU_H__
