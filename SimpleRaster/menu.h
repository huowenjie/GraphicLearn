#ifndef __MENU_H__
#define __MENU_H__

//-----------------------------------------------------------------------------
// 菜单
//-----------------------------------------------------------------------------

#define MENU_STR_LEN 64

// 选择菜单
#define SELECT_MENU(m) SelectMenu(m, sizeof(m)/sizeof(m[0]))

// 标准输入字符串
const char *InputStr(const char *prompt);

// 标准输入整数
int InputInt(const char *prompt);

// 选择菜单
int SelectMenu(char (*menu)[MENU_STR_LEN], int count);

//-----------------------------------------------------------------------------

#endif // __MENU_H__
