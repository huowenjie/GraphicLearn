#ifndef __GR_MENU_H__
#define __GR_MENU_H__

/*===========================================================================*/
/* 菜单 */
/*===========================================================================*/

#define MENU_STR_LEN 64

// 选择菜单
#define SELECT_MENU(m) select_menu(m, sizeof(m)/sizeof(m[0]))

// 标准输入字符串
const char *input_str(const char *prompt);

// 标准输入整数
int input_int(const char *prompt);

// 选择菜单
int select_menu(char (*menu)[MENU_STR_LEN], int count);

/*===========================================================================*/

#endif // __GR_MENU_H__
