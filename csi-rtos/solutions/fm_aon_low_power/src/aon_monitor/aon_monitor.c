#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <strings.h>
#include <string.h>
#include <drv/common.h>
#include <drv/pmic.h>
#include <drv/timer.h>
#include <drv/pvt.h>
#include <pmic_rsc_def.h>
#include "aon_monitor.h"
#include <csi_core.h>
#include "syslog.h"
#include "aon_common.h"

#ifndef CONFIG_SIMPLE_CONSOLE
#define HIST_SIZE   5
#define MAXLINE     252
#endif

#define MAXARGS     36
#define NUM_CMD     23
#define BYTE        1
#define WORD        2
#define LONG        4
#define READ        0
#define WRITE       1
#define SET         2
#define DUAL_SET    3
#define GET         4
#define LIST        5
#define READ_TS     6
#define READ_PD     7
#define READ_VM     8
#define GET_PS      9
#define GET_CLKFREQ 10
#define GET_CLKEN   11
#define GET_SOFTRST 12
#define GET_REG		13
#define SET_REG		14
#define LP2_MODE	15
#define LP3_MODE	16

#define ESC         27
#define BS          8
#define DEL         127

typedef struct {
   uint8_t id;
   const char* cmd;
   const char* arg;
   const char* cmd_help;
} cmds_info_t;

#define DELETE_CHAR()         printf(" \b")
#define DISPLAY_PROMPT()      printf(">$ ")
#define ERASE_LINE()          printf("%c[2K\r", 27)
#define CURSOR_FORWARD()      printf("%c[1C", 27)
#define CURSOR_BACK()         printf("%c[1D", 27)

extern void csi_clr_pendc(void);
extern uint8_t csi_getchar(void);
extern void csi_putchar(int c);
//extern csi_pmic_t pmic;
extern csi_pvt_t  pvt;

extern void lpddr4_phy_lp2_enter(int speed);
extern void lpddr4_phy_lp2_exit(int speed);
extern void lpddr4_phy_lp3_enter(int speed);
extern void lpddr4_phy_lp3_exit(int speed);

#ifndef CONFIG_SIMPLE_CONSOLE
static int32_t aon_monitor_getc(char buf[], uint32_t len, uint8_t echo);
static char* aon_monitor_readline(void);
static char* aon_monitor_restore_hist(char hist[]);
static uint8_t aon_monitor_add_to_hist(char hist[]);
static uint8_t aon_monitor_redraw_line(void);
static char* aon_monitor_insert_char(char buf[], char buf_end[], char c);
static char* aon_monitor_remove_char(char buf[]);
static void aon_monitor_restore_cursor_pos(char start[], char end[]);
#endif
static uint8_t aon_dispatch(char* data);
static uint8_t aon_cmd_md(int argc, char* argv[], int len);
static uint8_t aon_cmd_mm(int argc, char* argv[], int len);
static uint8_t aon_cmd_pmic(int argc, char* argv[], int rw);
#ifdef CONFIG_AON_MONITOR_DDR_LP
static uint8_t aon_cmd_ddr(int argc, char* argv[], int lp_mode);
#endif
#ifdef CONFIG_AON_MONITOR_PVT
static uint8_t aon_cmd_pvt(int argc, char* argv[], int rw);
#endif
static uint8_t aon_cmd_lpwr(int argc, char* argv[], int rw);
#ifdef CONFIG_BMU_ENABLE
static uint8_t aon_cmd_bmu_cfg(int argc, char* argv[]);
static uint8_t aon_cmd_bmu_start(int argc, char* argv[]);
static uint8_t aon_cmd_bmu_stop(int argc, char* argv[]);
#endif
static uint8_t aon_cmd_system_reboot();
static uint8_t aon_cmd_wdog_ops(int argc, char* argv[]);
static uint8_t aon_cmd_log_ops(int argc, char* argv[]);
#ifdef  CONFIG_AUDIO_ENABLE
static uint8_t aon_cmd_audio(int argc, char* argv[]);
#endif


static int findstrn(cmds_info_t* list, int max, char* str);
static uint32_t aon_memory_rw(void* addr, void* val, uint8_t width);
#ifdef CONFIG_BMU_ENABLE
static void aon_bmu_init_defaut_cfg(void);
#endif

csi_error_t aon_bmu_init(void);


#define readl(addr) \
    ({ unsigned int __v = (*(volatile uint32_t *) (addr)); __v; })

#define writel(addr,b) ((*(volatile uint32_t *) (addr)) = (b))

#define IS_BLANK(c) ((c) == ' ' || (c) == '\t')
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#define IS_ALPHA(c) ( ((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') )
#define IS_HEX_DIGIT(c) (((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))


#ifdef CONFIG_SIMPLE_CONSOLE

#include "drv/uart.h"

static char g_data[255];
static uint8_t g_recv_len = 0;


static void uart_event_cb(csi_uart_t *handle, csi_uart_event_t event, void *arg)
{
    int recv_num = 0;
    uint32_t time_out = 0x7ffff;
	char rx;
	int err;

    switch (event) {
        case UART_EVENT_RECEIVE_FIFO_READABLE:
            recv_num = csi_uart_receive(handle, &rx , 1, time_out);
        default:
            break;
    }

	if(recv_num <= 0) {
      printf("recv err");
	}

	if(rx == '\n' || rx == '\r' || g_recv_len >= sizeof(g_data) -1) {
		g_data[g_recv_len] = '\0';
		g_recv_len = 0;
		printf("%s\n",g_data);
		err = aon_dispatch(g_data);
			/* Check for non-fatal error */
		if (err == AON_ERR_NOTFOUND)
		{
			printf("Unknown cmd!\n");
		}
		else if (err == AON_ERR_PARM)
		{
			printf("Invalid/missing parm!\n");
		}
		else if (err == AON_ERR_UNAVAILABLE)
		{
			printf("Function not avail!\n");
		}
		/* Display prompt */
		printf("\n>$ ");
	} else {
		//printf("rx:%s\n",rx);
        g_data[g_recv_len++] = rx;
	}

}

int aon_register_callbcak()
{
	extern csi_uart_t console_uart;
	/* Display prompt */
	printf("\n>$ ");
    return  csi_uart_attach_callback(&console_uart, uart_event_cb, NULL);
}
#else

/* Local Variables */
static char line[MAXLINE];
static char hist[HIST_SIZE][MAXLINE];

static int32_t aon_monitor_getc(char* buf, uint32_t len, uint8_t echo)
{
	for (uint32_t i = 0U; i < len; i++)
		buf[i] = csi_getchar();

	if ((*buf != 27U) && (echo == AON_TRUE))
		for (uint32_t i = 0U; i < len; i++)
			csi_putchar(buf[i]);

	for (uint32_t i = 0U; i < len; i++) {
		if (buf[i] == '\r')
			buf[i] = '\n';
	}

	return len;
}

static char* aon_monitor_readline(void)
{
	/* Maintain pointers to the beginning and end of the command buffer */
	char* const buf_start = line;
	char* const buf_end = line + MAXLINE;

	/* Points to end of the current command */
	char* end_of_cmd = line;

	/* Pointer used for inserting characters into command buffer */
	char* buf = line;

	/* Character received from UART */
	char input = 0U;

	/* Initialize seek and insert indices - seek is used for traversing
	 * the history buffer using the up and down arrow keys, and insert
	 * is for indicating the next location to insert the next command */
	static uint8_t cur_hist_size = 0U;
	static int8_t insert_idx = 0U;

	uint8_t seek_cnt = 0;

	/***************************************************************************/

	/* Clear command buffer */
	(void)memset((void*)buf, 0U, (size_t)MAXLINE);

	/* Get first character */
	aon_monitor_getc(&input, 1U, AON_TRUE);

	/* Main processing loop */
	while ((input != '\n') && (input != '\r') && (buf != buf_end))
	{
		if (input == BS) /* Backspace */
		{
			if (buf != buf_start)
			{
				if (buf == end_of_cmd) /* Check if backspacing from the end of the line */
				{
					/* Remove previous character from buffer */
					buf--;
					*buf = 0U;

					/* Update end pointer */
					end_of_cmd--;

					/* Clear character on console */
					DELETE_CHAR();
				}
				else
				{
					/* Remove previous character from buffer */
					buf--;
					aon_monitor_remove_char(buf);

					/* Update end pointer */
					end_of_cmd--;

					/* Print updated command to console */
					printf("%s", buf);

					/* Remove extra character from end of line */
					DELETE_CHAR();

					aon_monitor_restore_cursor_pos(buf, end_of_cmd);
				}

			}
			else /* No more characters to backspace */
			{
				printf(" ");
			}

		}
		else if (input == DEL) /* Delete */
		{
			if ((buf != end_of_cmd) && (*buf != 0U))
			{
				/* Remove character underneath cursor */
				aon_monitor_remove_char(buf);

				/* Update end pointer */
				end_of_cmd--;

				/* Print updated command and remove extra character */
				printf("%s", buf);
				DELETE_CHAR();

				aon_monitor_restore_cursor_pos(buf, end_of_cmd);
			}
		}
		else if (input == ESC) /* Escape character */
		{
			/* Get second character of escape sequence */
			aon_monitor_getc(&input, 1U, AON_FALSE);

			if (input == '[')
			{
				/* Get last character of escape sequence */
				aon_monitor_getc(&input, 1U, AON_FALSE);

				if (input == 'A') /* Up arrow key */
				{
					/* Once the seek counter is equal to the current size of the
					 * history buffer, we have displayed all available commands */
					if (seek_cnt < cur_hist_size)
					{
						uint8_t seek_idx;
						seek_cnt++;

						/* Compute seek position based on offset from insert position */
						seek_idx = ((insert_idx - seek_cnt) + HIST_SIZE) % HIST_SIZE;

						/* Erase contents of current command buffer and
						 * replace with the previous command */
						(void)memset((void*)buf_start, 0U, (size_t)MAXLINE);
						end_of_cmd = aon_monitor_restore_hist(hist[seek_idx]);

						/* Set buffer pointer to end of buffer */
						buf = end_of_cmd;

						/* Update console to display previous command */
						aon_monitor_redraw_line();
					}

				}
				else if (input == 'B') /* Down arrow key */
				{
					if (seek_cnt > 1)
					{
						uint8_t seek_idx;
						seek_cnt--;

						/* Compute seek position based on offset from insert position */
						seek_idx = ((insert_idx - seek_cnt) + HIST_SIZE) % HIST_SIZE;

						/* Erase contents of current command buffer and
						 * replace with the previous command */
						(void)memset((void*)buf_start, 0U, (size_t)MAXLINE);
						end_of_cmd = aon_monitor_restore_hist(hist[seek_idx]);

						/* Set buffer pointer to end of buffer */
						buf = end_of_cmd;

						/* Update console to display previous command */
						aon_monitor_redraw_line();
					}
					else /* No more commands to display, so clear the line */
					{
						/* Update console */
						ERASE_LINE();
						DISPLAY_PROMPT();

						(void)memset((void*)buf_start, 0U, (size_t)MAXLINE);

						/* Update buffer pointers */
						buf = buf_start;
						end_of_cmd = buf_start;

						/* Reset seek counter */
						seek_cnt = 0;
					}

				}
				else if (input == 'C') /* Right arrow key */
				{
					if (buf != end_of_cmd) /* Check if not at EOL */
					{
						buf++;
						CURSOR_FORWARD();
					}
				}
				else if (input == 'D') /* Left arrow key */
				{
					if (buf != buf_start) /* Check if no more characters remain */
					{
						buf--;
						CURSOR_BACK();
					}
				}
				else if (input == 51) /* Delete */
				{
					if ((buf != end_of_cmd) && (*buf != 0))
					{
						/* Remove character underneath cursor */
						aon_monitor_remove_char(buf);

						/* Update end pointer */
						end_of_cmd--;

						/* Print updated command and remove extra character */
						printf("%s", buf);
						DELETE_CHAR();

						aon_monitor_restore_cursor_pos(buf, end_of_cmd);
					}
				}
			}
		}
		else /* Normal character */
		{
			if (buf == end_of_cmd) /* Add character to the end */
			{
				/* Write character to command buffer */
				*buf++ = input;
				end_of_cmd++;
			}
			else /* Insert character */
			{
				aon_monitor_insert_char(buf, end_of_cmd, input);

				/* Update pointers */
				buf++;
				end_of_cmd++;

				/* Print updated command */
				printf("%s", buf);

				aon_monitor_restore_cursor_pos(buf, end_of_cmd);
			}
		}

		/* Get next character from console */
		aon_monitor_getc(&input, 1U, AON_TRUE);
	}

	/* Reset seek counter */
	seek_cnt = 0;

	/* Check that a valid character is stored in the command buffer */
	if ((*buf_start != '\n') && (*buf_start != '\r') && (*buf_start != 0U))
	{
		uint8_t seek_idx = (insert_idx + HIST_SIZE - 1U) % HIST_SIZE;

		/* Don't add command to history if most recent command is identical */
		if (strcasecmp(line, hist[seek_idx]))
		{
			/* Add current command to history buffer */
			aon_monitor_add_to_hist(hist[insert_idx]);

			/* Move to next address in history buffer */
			insert_idx = (insert_idx + 1U) % HIST_SIZE;

			/* Current size of history buffer can never be larger than total
			 * size of buffer */
			if (cur_hist_size < HIST_SIZE)
				cur_hist_size++;
		}
	}

	return hist[insert_idx];
}

static char* aon_monitor_restore_hist(char hist_buf[])
{
	uint8_t idx = 0U;

	/* Null pointer check */
	if (!hist_buf)
		return NULL;

	/* Copy contents of history buffer into
	 * current command buffer */
	while ((idx < MAXLINE) && (hist_buf[idx] != 0U))
	{
		line[idx] = hist_buf[idx];
		idx++;
	}

	/* Return pointer to end of command buffer */
	return &line[idx];
}

static uint8_t aon_monitor_add_to_hist(char hist_buf[])
{
	uint8_t idx = 0U;

	/* Null pointer check */
	if (!hist_buf)
		return AON_ERR_PARM;

	/* Copy contents of current command buffer into
	 * command history buffer */
	while ((*line != '\n') && (*line != '\r') && (*line != 0U) && (idx < MAXLINE))
	{
		hist_buf[idx] = line[idx];
		idx++;
	}

	return AON_ERR_NONE;
}

static uint8_t aon_monitor_redraw_line(void)
{
	/* Erase current line on console */
	ERASE_LINE();

	/* Re-display prompt */
	DISPLAY_PROMPT();

	printf("%s", line);

	return AON_ERR_NONE;
}

static char* aon_monitor_insert_char(char buf[], char buf_end[], char c)
{
	while (buf_end > buf)
	{
		*buf_end = *(buf_end - 1);
		buf_end--;
	}
	*buf_end = c;

	return buf;
}

static char* aon_monitor_remove_char(char buf[])
{
	while (*buf != 0U)
	{
		*buf = *(buf + 1);
		buf++;
	}

	return buf;
}

static void aon_monitor_restore_cursor_pos(char start[], char end[])
{
	for (int idx = (end - start); idx > 0; idx--)
	{
		printf("\b");
	}
}

#endif

void aon_dbg_monitor(void)
{
	uint8_t err = AON_ERR_NONE;
//#ifdef CONFIG_BMU_ENABLE
	csi_error_t ret;
//#endif

	printf("\n*** Debug Monitor ***\n");
#ifdef CONFIG_BMU_ENABLE
	ret = aon_bmu_init();
	if (ret)
		printf("aon_bmu_init failed %d \n", ret);
#endif

#ifdef CONFIG_SIMPLE_CONSOLE
    ret = aon_register_callbcak();
    if(ret)
	    LOG_E("aon console callbcak register faild %d", ret);
#else
	/* Zero initialize command and history buffers */
	(void)memset((void*)line, 0U, (size_t)MAXLINE);
	(void)memset((void*)hist, 0U, (size_t)(MAXLINE * HIST_SIZE));
	/* Command loop */
	while (err != AON_ERR_LAST)
	{
		/* Display prompt */
		printf("\n>$ ");

		/* Read user input from console */
		aon_monitor_readline();
		printf("\n");

		/* Dispatch */
		err = aon_dispatch(line);

		/* Clean up input buffer */
		csi_clr_pendc();

		/* Check for non-fatal error */
		if (err == AON_ERR_NONE)
		{
			continue;
		}

		/* Check for non-fatal error */
		if (err == AON_ERR_NOTFOUND)
		{
			printf("Unknown cmd!\n");
		}
		else if (err == AON_ERR_PARM)
		{
			printf("Invalid/missing parm!\n");
		}
		else if (err == AON_ERR_UNAVAILABLE)
		{
			printf("Function not avail!\n");
		}
		else if (err != AON_ERR_LAST)
		{
			printf("Unknown err (%d)!\n", err);
		}
	}
#endif
    (void)err;

}

static cmds_info_t g_cmds[] =
	{
		{0, "exit", "", "exit the monitor" },
		{1, "help", "", "help" },
		{2, "md.b"," *address* [*count*]", "display count bytes at address"},
		{3, "md.w", " *address* [*count*]", "display count words at address"},
		{4, "md","[.l] *address* [*count*]", "display count long-words at address"},
		{5, "mm.b", " *address value*", "modify byte at address"},
		{6, "mm.w", " *address value*","modify word at address"},
		{7, "mm", "[.l] *address value*","modify long-word at address"},
		{8, "pmic.r", " regu_ext_id", "regulator enable/disable status"},
		{9, "pmic.w", " regu_ext_id 1/0", "regulator enable/disable"},
		{10, "pmic.s", " regu_ext_id val", "regulator voltage set: (unit: uV)"},
		{11, "pmic.d.s", " regu_ext_id dc1 dc2", "dual voltages rail set: (unit: uV)"},
		{12, "pmic.g", " regu_ext_id", "regulator voltage get: (unit: uV)"},
		{13, "pmic.id.list", "", "list all virtual regu_id"},
#ifdef CONFIG_AON_MONITOR_PVT
		{14, "pvt.ts.r", "", "read all ts data"},
		{15, "pvt.pd.r","", "read all pd data"},
		{16, "pvt.vm.r", "", "read all vm data"},
#endif
		{17, "ps.r", "", "read all power switch status"},
		{18, "clkfreq.r", "", "read all clk frequcence (uinit:KHz)"},
		{19, "clken.r", "", "read all clk enable status "},
		{20, "softrst.r", "", "read all soft reset status"},
#ifdef CONFIG_BMU_ENABLE
		{21, "bmu.cfg","", "bus-monitor config bmu param"},
		{22, "bmu.start","","bus-monitor start monitor with all bmu configed"},
		{23, "bmu.stop","","bus-monitor stop all bmu & reset bmu env"},
#endif
		{24, "reboot", "", "reboot the whole system"},
		{25, "wdog", "", "wdog ops"},
		{26, "log", "", "pmic log level ops"},
#ifdef CONFIG_AON_MONITOR_PMIC_REG_OP
		{27, "reg.get", "", "pmic get reg ops"},
		{28, "reg.set", "" ,"pmic set reg ops"},
#endif
#ifdef CONFIG_AON_MONITOR_DDR_LP
		{29, "ddr.lp2", "", "ddr lp2 mode test"},
		{30, "ddr.lp3", "", "ddr lp3 mode test"},
#endif
#ifdef  CONFIG_AUDIO_ENABLE
		{31, "audio", "", "audo str ops"},
#endif
};

#define MAX_CMD_LINE_SIZE 60

static void aon_cmd_help(void)
{
     int block_num = 0;
     for(int i=0; i < ARRAY_SIZE(g_cmds); i++) {
		int cmd_len = strlen(g_cmds[i].cmd) + strlen(g_cmds[i].arg);
		if( cmd_len > MAX_CMD_LINE_SIZE) {
			printf("too long for cmd str %s %s", g_cmds[i].cmd, g_cmds[i].arg);
			return;
		} else {
            block_num = MAX_CMD_LINE_SIZE - cmd_len;
		}
        printf("%s%s",g_cmds[i].cmd, g_cmds[i].arg);
        while(block_num--) {
           printf(" ");
		}
        printf("| %s\n", g_cmds[i].cmd_help);
	 }
}

uint8_t aon_dispatch(char* data)
{
	int argc = 0;
	char* argv[MAXARGS];
	char* p = data;
	int sub;


	/* Parse Line */
	while (*p != 0)
	{
		/* Skip white space */
		while (*p == ' ')
		{
			p++;
		}

		/* Save pointer */
		if ((*p != 0) && (*p != '\n') && (*p != '\r'))
		{
			argv[argc] = p;
			argc++;
		}

		/* Find end */
		while ((*p != ' ') && (*p != 0) && (*p != '\n') && (*p != '\r'))
		{
			p++;
		}

		/* Mark end */
		if (*p != 0)
		{
			*p++ = 0;
		}
	}

	/* Parse command */
	if (argc == 0)
	{
		return AON_ERR_NONE;
	}
	else
	{
		sub = findstrn(g_cmds, ARRAY_SIZE(g_cmds), argv[0]);
	}

	switch (sub)
	{
	case 0: /* exit */
		return AON_ERR_LAST;
	case 1: /* help */
		aon_cmd_help();
		return AON_ERR_NONE;
	case 2: /* md.b */
		return aon_cmd_md(argc - 1, &argv[1], 1);
	case 3: /* md.w */
		return aon_cmd_md(argc - 1, &argv[1], 2);
	case 4: /* md.l */
		return aon_cmd_md(argc - 1, &argv[1], 4);
	case 5: /* mm.b */
		return aon_cmd_mm(argc - 1, &argv[1], 1);
	case 6: /* mm.w */
		return aon_cmd_mm(argc - 1, &argv[1], 2);
	case 7: /* mm.l */
		return aon_cmd_mm(argc - 1, &argv[1], 4);
	case 8: /* pmic.r */
		return aon_cmd_pmic(argc - 1, &argv[1], READ);
	case 9: /* pmic.w */
		return aon_cmd_pmic(argc - 1, &argv[1], WRITE);
	case 10: /* pmic.s */
		return aon_cmd_pmic(argc - 1, &argv[1], SET);
	case 11: /* pmic.d.s */
		return aon_cmd_pmic(argc - 1, &argv[1], DUAL_SET);
	case 12: /* pmic.g */
		return aon_cmd_pmic(argc - 1, &argv[1], GET);
	case 13: /* pmic.id.list */
		return aon_cmd_pmic(argc - 1, &argv[1], LIST);
#ifdef CONFIG_AON_MONITOR_PVT
	case 14: /*pvt.ts.r*/
		return aon_cmd_pvt(argc - 1, &argv[1], READ_TS);
	case 15: /*pvt.pd.r*/
		return aon_cmd_pvt(argc - 1, &argv[1], READ_PD);
	case 16: /*pvt.vm.r*/
		return aon_cmd_pvt(argc - 1, &argv[1], READ_VM);
#endif
	case 17: /*ps.r*/
		return aon_cmd_lpwr(argc - 1, &argv[1], GET_PS);
	case 18: /*clkfreq.r*/
		return aon_cmd_lpwr(argc - 1, &argv[1], GET_CLKFREQ);
	case 19: /*clken.r*/
		return aon_cmd_lpwr(argc - 1, &argv[1], GET_CLKEN);
	case 20: /*softrst.r*/
		return aon_cmd_lpwr(argc - 1, &argv[1], GET_SOFTRST);
#ifdef CONFIG_BMU_ENABLE
	case 21: /*bmu.cfg*/
		return aon_cmd_bmu_cfg(argc - 1, &argv[1]);
	case 22: /*bmu.start*/
		return aon_cmd_bmu_start(argc - 1, &argv[1]);
	case 23: /*bmu.stop*/
		return aon_cmd_bmu_stop(argc - 1, &argv[1]);
#endif
	case 24: /* reboot */
		return aon_cmd_system_reboot();
	case 25: /* wdog */
		return aon_cmd_wdog_ops(argc - 1, &argv[1]);
	case 26: /* log level */
		return aon_cmd_log_ops(argc - 1, &argv[1]);
#ifdef CONFIG_AON_MONITOR_PMIC_REG_OP
	case 27: /* pmic.get */
		return aon_cmd_pmic(argc - 1, &argv[1], GET_REG);
	case 28: /* pmic.set */
		return aon_cmd_pmic(argc - 1, &argv[1], SET_REG);
#endif
#ifdef CONFIG_AON_MONITOR_DDR_LP
	case 29: /* ddr.lp2 */
		return aon_cmd_ddr(argc - 1, &argv[1], LP2_MODE);
	case 30: /* ddr.lp3 */
		return aon_cmd_ddr(argc - 1, &argv[1], LP3_MODE);
#endif
#ifdef  CONFIG_AUDIO_ENABLE
	case 31: /* audio */
	        return aon_cmd_audio(argc - 1, &argv[1]);
#endif
	default:
		return AON_ERR_NOTFOUND;
	}
}

#ifdef CONFIG_BMU_ENABLE
/*********************************************************************************************
 *                      bmu global data structure
 ********************************************************************************************/
typedef enum {
	BMU0,
	BMU1,
	BMU2,
	BMU3,
	BMU4,
	BMU5,
	BMU6,
	BMU_ID_MAX
}bmu_id_t;

typedef enum {
	BMU_MASTER,
	BMU_MODE,
	BMU_PERIOD,
	BMU_DISPLAY,
	BMU_AXI_ID,
	BMU_AXI_ID_MSK,
	BMU_DUA_THR,
	BMU_DUA_THR_COE,
	BMU_FLT_TYPE,
	BMU_FLT_START_ADDR,
	BMU_FLT_END_ADDR,
	BMU_FLT_SIZE,
	BMU_FLT_LEN,
	BMU_FLT_ALIGN,
	BMU_CMD_EXEC,
	BMU_PARAM_ID_MAX,
}bmu_param_id_t;

typedef struct {
	uint16_t  param_id;
	uint16_t  param_val_num;
	char* param_name;
	uint64_t  param_val;
}bmu_param_t;

typedef enum {

	BMU_FSM_IDLE,
	BMU_FSM_CFG,
	BMU_FSM_RUN,
}bmu_fsm_t;

typedef enum {
	BMU_LOG_DEV_UART,
	BMU_LOG_DEV_MEM
}bmu_log_mem_t;

typedef enum {
	BMU_MONITOR_SINGLE,
	BMU_MONITOR_CONTINUOUS
}bmu_monitor_t;


typedef struct {
	uint32_t bmu_fsm_st : 3;
	uint32_t bmu_log_out : 1;
	uint32_t bmu_rev_bits : 4;
	uint32_t bmu_master : 16;
	uint32_t bmu_rev1_bits : 8;
	void* bmu_log_mem_addr;
	uint32_t* bmu_pmem_addr;
	uint32_t bmu_log_mem_sz;
	uint32_t bmu_sample_cnt;
	uint32_t bmu_sample_cur_cnt;
	uint32_t bmu_sample_timer;           ///< unit in ns
	uint32_t bmu_master_bk;              ///< used for uart print
	csi_timer_t              thandle;
}bmu_dev_t;
/*********************************************************************************************
 *                      bmu global data
 ********************************************************************************************/
static uint8_t bmu_status_reg_offset[] = {
	0x48,
	0x44,
	0x40,
	0xa0,
	0x4c,
	0x58,
	0x54,
	0x50,
	0xa4,
	0x5c,
	0x90,
	0x60,
	0x70,
	0x80,
	0x84
};

static uint32_t    g_bmu_temp_monitor_data[(ARRAY_SIZE(bmu_status_reg_offset) + 1) * BMU_ID_MAX];
static bmu_dev_t   g_bmu_dev = {
	.bmu_fsm_st = BMU_FSM_IDLE,
	.bmu_log_out = BMU_LOG_DEV_UART,
	.bmu_master = 0,
	.bmu_sample_cnt = 0xFFFFFFFF,
	.bmu_sample_cur_cnt = 0,
	.bmu_log_mem_addr = (void*)&g_bmu_temp_monitor_data,
};

static bmu_param_t g_bmu_cmd_param_list[] = {
	{BMU_MASTER,		1,	"-bmu_num",		0x1f},
	{BMU_MODE,		1,	"-mode",		0},
	{BMU_PERIOD,		1,	"-period",		0x000f4240},
	{BMU_DISPLAY,		1,	"-display",		0xffffffff},
	{BMU_AXI_ID,		1,	"-axi_id",		0},
	{BMU_AXI_ID_MSK,	1,	"-axi_id_mask",		0},
	{BMU_DUA_THR,		1,	"-dura_threshold",	0x10001},
	{BMU_DUA_THR_COE,	1,	"-dura_threshold_coe",	0x1},
	{BMU_FLT_TYPE,		1,	"-flt_type",		0},
	{BMU_FLT_START_ADDR,	2,	"-flt_start_addr",	0},
	{BMU_FLT_END_ADDR,	2,	"-flt_end_addr",	0},
	{BMU_FLT_SIZE,		1,	"-flt_size",		0},
	{BMU_FLT_LEN,		1,	"-flt_len",		0},
	{BMU_FLT_ALIGN,		1,	"-flt_align",		0x1},
	{BMU_CMD_EXEC,		0,	"-c",			0x0},
};

static void aon_bmu_cfg_param_to_mem(bmu_dev_t* bmu_dev);
static uint8_t _aon_bmu_stop(bmu_dev_t* bmu_dev);
//static void aon_bmu_cfg_param_to_mem(bmu_dev_t *bmu_dev);
static bmu_param_t g_bmu_usr_cmd_param_list[BMU_ID_MAX][BMU_PARAM_ID_MAX];
static void aon_bmu_sample_data_to_mem(bmu_dev_t* bmu_dev);
static void aon_bmu_save_usr_bmu_cfg(bmu_param_t usr_bmu_cmd_cfg[][BMU_PARAM_ID_MAX], uint32_t bmu_sel_msk, bmu_param_t bmu_cfg[])
{
	uint32_t i = 0;
	while (bmu_sel_msk) {
		if (bmu_sel_msk & 0x1) {
			memcpy(&usr_bmu_cmd_cfg[i][0], bmu_cfg, sizeof(g_bmu_cmd_param_list));
		}
		bmu_sel_msk >>= 1;
		i++;
	}
	return;
}

//void aon_bmu_timer_callback(csi_timer_t *timer_handle, void* argv)
//{
//	/*read data from bmu reg status*/
//	bmu_dev_t *pbmu = (bmu_dev_t*)argv;
//	if(pbmu->bmu_sample_cur_cnt < pbmu->bmu_sample_cnt) {
//		aon_bmu_sample_data_to_mem(pbmu);
//		pbmu->bmu_sample_cur_cnt++;
//	}else {
//		csi_timer_stop(&pbmu->thandle);
//		printf("bmu sample data done!!!,please excec [bmu.stop] to dump log\n");
//	}
//}

void aon_bmu_timer_callback(csi_timer_t* timer_handle, void* argv)
{
	/*read data from bmu reg status*/
	bmu_dev_t* pbmu = (bmu_dev_t*)argv;
	if (pbmu->bmu_sample_cur_cnt < pbmu->bmu_sample_cnt) {
		//	  if(pbmu->bmu_sample_cur_cnt ==  0) {
		//		aon_bmu_cfg_param_to_mem(pbmu);
		//          }
		aon_bmu_sample_data_to_mem(pbmu);
		pbmu->bmu_sample_cur_cnt++;
	}
else {
		csi_timer_stop(&pbmu->thandle);
		printf("BMU sample data done!!! If you want to use serial port to display data, please enter [bmu.stop]\n");
		//	aon_bmu_cfg_param_to_mem(pbmu);
		_aon_bmu_stop(pbmu);
	}
}

csi_error_t aon_bmu_init(void)
{
	csi_error_t ret;

	// bmu_dev_t *pbmu = (bmu_dev_t*)argv;

	ret = csi_timer_init(&g_bmu_dev.thandle, AON_BMU_TIMER_IDX);

	if (ret)
		return ret;
	ret = csi_timer_attach_callback(&g_bmu_dev.thandle, aon_bmu_timer_callback, &g_bmu_dev);
	if (ret)
		return ret;
	aon_bmu_init_defaut_cfg();

	return CSI_OK;
}

static inline csi_error_t aon_bmu_get_param_id_byname(char* param_name,
	uint32_t* param_id,
	uint32_t* param_val_num,
	bmu_param_t param_list[],
	uint32_t param_list_sz)
{
	CSI_PARAM_CHK(param_list, CSI_ERROR);
	CSI_PARAM_CHK(param_name, CSI_ERROR);
	csi_error_t ret = CSI_ERROR;

	uint32_t i;
	for (i = 0; i < param_list_sz; i++) {
		if (0 == strncasecmp(param_name, param_list[i].param_name, strlen(param_name))) {
			*param_id = param_list[i].param_id;
			*param_val_num = param_list[i].param_val_num;
			ret = CSI_OK;
			break;
		}
	}

	return ret;
}

static inline csi_error_t aon_bmu_get_param_value(bmu_param_t param[], uint32_t param_id, uint64_t* param_val)
{
	CSI_PARAM_CHK(param, CSI_ERROR);
	csi_error_t ret = CSI_ERROR;

	if (param_id >= BMU_PARAM_ID_MAX)
		return CSI_ERROR;

	uint32_t i;
	for (i = 0; i < BMU_PARAM_ID_MAX; i++) {
		if (param[i].param_id == param_id) {
			*param_val = param[i].param_val;
			ret = CSI_OK;
			break;
		}
	}

	return ret;
}

static inline csi_error_t aon_bmu_set_param_value(bmu_param_t param[], uint32_t param_id, uint64_t param_val)
{
	CSI_PARAM_CHK(param, CSI_ERROR);
	csi_error_t ret = CSI_ERROR;

	if (param_id >= BMU_PARAM_ID_MAX)
		return CSI_ERROR;

	uint32_t i;
	for (i = 0; i < BMU_PARAM_ID_MAX; i++) {
		if (param[i].param_id == param_id) {
			param[i].param_val = param_val;
			ret = CSI_OK;
			break;
		}
	}

	return ret;
}

static void* g_bmu_reg_base[] = {
	(void*)0xff008000,
	(void*)0xff008800,
	(void*)0xff009000,
	(void*)0xff009800,
	(void*)0xff00a000,
	(void*)0xbfc34000,
	(void*)0xcb020000
};

typedef struct {
	volatile uint32_t BMU_CFG0;            ///< offset: 0x00
	volatile uint32_t BMU_CFG1;            ///< offset: 0x04
	volatile uint32_t BMU_CFG2;            ///< offset: 0x08
	volatile uint32_t BMU_CFG3;            ///< offset: 0x0c
	volatile uint32_t BMU_CFG4;            ///< offset: 0x10
	volatile uint32_t BMU_CFG5;            ///< offset: 0x14
	volatile uint32_t BMU_CFG6;            ///< offset: 0x18
	volatile uint32_t BMU_CFG7;            ///< offset: 0x1c
	volatile uint32_t BMU_CFG8;            ///< offset: 0x20
	volatile uint32_t BMU_CFG9;            ///< offset: 0x24
	volatile uint32_t BMU_FLT;             ///< offset: 0x28
	volatile uint32_t BMU_CFG10;           ///< offset: 0x2c
	volatile uint32_t BMU_CFG11;           ///< offset: 0x30
	volatile uint32_t BMU_CFG12;           ///< offset: 0x34
	volatile uint32_t BMU_CFG13;           ///< offset: 0x38
	volatile uint32_t BMU_CFG14;           ///< offset: 0x3c
	volatile uint32_t BMU_STATUS1[10];     ///< offset: 0x40~0x64
	uint32_t          REV1[2];             ///< offset: 0x68~0x6c
	volatile uint32_t BMU_STATUS2[2];      ///< offset: 0x70~0x74
	uint32_t          REV2[2];             ///< offset: 0x78~0x7c
	volatile uint32_t BMU_STATUS3[2];      ///< offset: 0x80~0x84
	uint32_t          REV3[2];             ///< offset: 0x88~0x8c
	volatile uint32_t BMU_STATUS4[2];      ///< offset: 0x90~0x94
	uint32_t          REV4[2];             ///< offset: 0x98~0x9c
}bmu_reg_map_t;

void aon_bmu_default_cfg(bmu_reg_map_t* pbmu)
{
	pbmu->BMU_CFG0 = 0x100;
	pbmu->BMU_CFG1 = 0x10001;
	pbmu->BMU_CFG2 = 0;
	pbmu->BMU_CFG3 = 0x0ee6b280;
	pbmu->BMU_CFG4 = 0;
	pbmu->BMU_CFG5 = 0;
	pbmu->BMU_CFG6 = 0;
	pbmu->BMU_CFG7 = 0;
	pbmu->BMU_CFG8 = 0;
	pbmu->BMU_CFG9 = 0;
	pbmu->BMU_CFG10 = 0;
	pbmu->BMU_CFG11 = 0;
	pbmu->BMU_CFG12 = 0;
	pbmu->BMU_CFG13 = 0;
	pbmu->BMU_CFG14 = 0;
	pbmu->BMU_FLT = 0x300;
	/*BMU_OSTD_CFG:0x94*/
	pbmu->BMU_STATUS4[1] = 0;
};

static void aon_bmu_print_cfg_param(bmu_dev_t* bmu_dev)
{
	uint32_t i = 0;
	uint32_t bmu_sel_msk = bmu_dev->bmu_master;
	while (bmu_sel_msk) {
		if (bmu_sel_msk & 0x1) {
			uint32_t j = 0;
			printf("\n");
			printf("BMU_CFG%d,", i);
			for (j = 0; j < BMU_PARAM_ID_MAX; j++)
				printf("%s=0x%llx,", g_bmu_usr_cmd_param_list[i][j].param_name, g_bmu_usr_cmd_param_list[i][j].param_val);
		}
		bmu_sel_msk >>= 1;
		i++;
	}
	printf("\n");
}

static void aon_bmu_sample_data_to_uart(bmu_dev_t* bmu_dev)
{
	uint32_t i, k;
	uint32_t bmu_sel = bmu_dev->bmu_master;
	uint32_t bmu_sample_cnt = bmu_dev->bmu_sample_cur_cnt;//bmu_dev->bmu_sample_cnt;

	/* print bmu user cfg first*/
	aon_bmu_print_cfg_param(bmu_dev);
	bmu_dev->bmu_pmem_addr = bmu_dev->bmu_log_mem_addr;

	/*print bmu samle data */
	k = 0;
	while (k < bmu_sample_cnt) {
		if (bmu_sample_cnt > 1) {
			bmu_sel = bmu_dev->bmu_master_bk;
		}
		else if (bmu_sample_cnt == 1) {
			bmu_sel = bmu_dev->bmu_master;
		}
		i = 0;
		while (bmu_sel) {
			if (bmu_sel & 0x1) {
				printf("bmu[%d],loop[%d],time[%d],%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n", \
					i, k, \
					bmu_dev->bmu_pmem_addr[0], bmu_dev->bmu_pmem_addr[1], bmu_dev->bmu_pmem_addr[2],
					bmu_dev->bmu_pmem_addr[3], bmu_dev->bmu_pmem_addr[4], bmu_dev->bmu_pmem_addr[5],
					bmu_dev->bmu_pmem_addr[6], bmu_dev->bmu_pmem_addr[7], bmu_dev->bmu_pmem_addr[8],
					bmu_dev->bmu_pmem_addr[9], bmu_dev->bmu_pmem_addr[10], bmu_dev->bmu_pmem_addr[11],
					bmu_dev->bmu_pmem_addr[12], bmu_dev->bmu_pmem_addr[13], bmu_dev->bmu_pmem_addr[14],
					bmu_dev->bmu_pmem_addr[15]);
				bmu_dev->bmu_pmem_addr += 16;

			}
			i++;
			bmu_sel >>= 1;
		}
		k++;
	}
	bmu_dev->bmu_pmem_addr = bmu_dev->bmu_log_mem_addr;
	if (bmu_sample_cnt > 1) {
		bmu_dev->bmu_master_bk = 0;
	}
else if (bmu_sample_cnt == 1) {
		bmu_dev->bmu_master = 0;
	}
}

static void aon_bmu_sample_data_to_mem(bmu_dev_t* bmu_dev)
{
	/* sample data to dedicated mem */
	uint32_t bmu_sel = bmu_dev->bmu_master;
	uint32_t i = 0, j = 0;
	uint32_t temp;
	uint32_t reg_val;

	//bmu_dev->bmu_pmem_addr = bmu_dev->bmu_log_mem_addr;
	//bmu_dev->bmu_pmem_addr += 128;
	//printf("DEBUG:data to mem %08x \n",bmu_dev->bmu_pmem_addr[0]);
	udelay(10);  //period mode remove invalid data
	temp = readl(0xe000bff8);
	while (bmu_sel) {
		if (bmu_sel & 0x1) {
			writel(bmu_dev->bmu_pmem_addr, temp);
			bmu_dev->bmu_pmem_addr++;
			for (j = 0; j < ARRAY_SIZE(bmu_status_reg_offset); j++) {
				writel(bmu_dev->bmu_pmem_addr, readl((uint32_t)g_bmu_reg_base[i] + bmu_status_reg_offset[j]));
				bmu_dev->bmu_pmem_addr++;
			}
			reg_val = readl(((uint32_t)g_bmu_reg_base[i] + 0x80));
			reg_val |= 0x1;
			writel(((uint32_t)g_bmu_reg_base[i] + 0x80), reg_val);
		}
		bmu_sel >>= 1;
		i++;
	}
}

static void aon_bmu_cfg_param_to_mem(bmu_dev_t* bmu_dev)
{
	/* sample cfg parameter to dedicated mem */
	uint32_t bmu_sel = bmu_dev->bmu_master;
	uint32_t i = 0, j = 0, k = 0;
	uint32_t loop = 0;
	uint32_t data_size;



	bmu_dev->bmu_pmem_addr = bmu_dev->bmu_log_mem_addr;
	//printf("parm to mem %08x \n",bmu_dev->bmu_pmem_addr[0]);
	//printf("parm to mem %08x \n",(uint32_t)(bmu_dev->bmu_pmem_addr));


	while (i < 7) {
		if (bmu_sel & 0x1) {
			memset(bmu_dev->bmu_pmem_addr, 0xFF, sizeof(uint32_t));
			if (g_bmu_usr_cmd_param_list[i][1].param_val == 0)
				loop = 1;
			else
				loop = g_bmu_usr_cmd_param_list[i][1].param_val;
			k++;
		}
		else {
			memset(bmu_dev->bmu_pmem_addr, 0x0, sizeof(uint32_t));
		}
		bmu_dev->bmu_pmem_addr++;

		if (bmu_sel & 0x1) {
			for (j = 1; j < 14; j++) {
				writel(bmu_dev->bmu_pmem_addr, (uint32_t)g_bmu_usr_cmd_param_list[i][j].param_val);
				bmu_dev->bmu_pmem_addr++;
				if ((j == 9) || (j == 10)) {
					g_bmu_usr_cmd_param_list[i][j].param_val >>= 32;
					writel(bmu_dev->bmu_pmem_addr, (uint32_t)g_bmu_usr_cmd_param_list[i][j].param_val);
					bmu_dev->bmu_pmem_addr++;
				}
			}
		}
		else {
			for (j = 0; j < 15; j++) {
				writel(bmu_dev->bmu_pmem_addr, 0x0);// Clean the memory when sel inactive
				bmu_dev->bmu_pmem_addr++;
			}
		}
		bmu_sel >>= 1;
		i++;
		//printf("CHK-CFG %d,parm to mem %08x \n",i,(uint32_t)(bmu_dev->bmu_pmem_addr));
	}
	// set clock select
	writel(0xff011300, 0x24); //cpubus clk
	udelay(2200);
	data_size = readl(0xff011304);
	writel(bmu_dev->bmu_pmem_addr, data_size);
	bmu_dev->bmu_pmem_addr++;
	//printf("cpu clkfreq to mem %08x,%08x \n",(uint32_t)(bmu_dev->bmu_pmem_addr),data_size);

	writel(0xff011300, 0x34); //vi aclk
	udelay(2200);
	data_size = readl(0xff011304);
	writel(bmu_dev->bmu_pmem_addr, data_size);
	bmu_dev->bmu_pmem_addr++;
	//printf("npu clkfreq to mem %08x,%08x \n",(uint32_t)(bmu_dev->bmu_pmem_addr),data_size);

	writel(0xff011300, 0x33); //v0 aclk
	udelay(2200);
	data_size = readl(0xff011304);
	writel(bmu_dev->bmu_pmem_addr, data_size);
	bmu_dev->bmu_pmem_addr++;

	writel(0xff011300, 0x2a); //npubus clk
	udelay(2200);
	data_size = readl(0xff011304);
	writel(bmu_dev->bmu_pmem_addr, data_size);
	bmu_dev->bmu_pmem_addr++;

	writel(0xff011300, 0x37); //vp clk
	udelay(2200);
	data_size = readl(0xff011304);
	writel(bmu_dev->bmu_pmem_addr, data_size);
	bmu_dev->bmu_pmem_addr++;

	writel(0xff011300, 0x00); //clk monitor off

	data_size = 0x9;// Version
	writel(bmu_dev->bmu_pmem_addr, data_size);
	bmu_dev->bmu_pmem_addr++;

	//RSVD Region set 0
	for (j = 0; j < 10; j++) {
		writel(bmu_dev->bmu_pmem_addr, 0x0);
		bmu_dev->bmu_pmem_addr++;
	}

	data_size = k * loop * 64 + 512;
	printf("BMU data size = 0x%08x\n", data_size);
}

static void aon_bmu_init_defaut_cfg(void)
{
	uint32_t bmu_num = ARRAY_SIZE(g_bmu_reg_base);
	uint32_t i;

	for (i = 0; i < bmu_num; i++) {
		aon_bmu_default_cfg((bmu_reg_map_t*)g_bmu_reg_base[i]);
	}
}

csi_error_t aon_bmu_init_user_cfg(bmu_reg_map_t* pbmu, bmu_param_t bmu_param[], uint32_t bmu_param_num)
{
	uint32_t i, param_id;
	i = 0;
	uint64_t param_val;
	csi_error_t ret = CSI_OK;

	do {
		param_id = bmu_param[i].param_id;

		ret = aon_bmu_get_param_value(bmu_param, param_id, &param_val);
		if (ret)
			return ret;

		switch (param_id)
		{
		case BMU_MASTER:
		{
			g_bmu_dev.bmu_master |= (param_val & 0xFFFF);
		}
		break;
		case BMU_MODE:
		{
			g_bmu_dev.bmu_sample_cnt = (param_val) ? param_val : 1;

			if (!param_val) {
				pbmu->BMU_CFG0 |= 0x100;
			}
else {
				pbmu->BMU_CFG0 &= ~0x100;
			}
			pbmu->BMU_CFG0 |= 0x1;
		}

		break;
		case BMU_PERIOD:
		{
			pbmu->BMU_CFG3 = (uint32_t)param_val * 250;
			g_bmu_dev.bmu_sample_timer = param_val;
		}
		break;

		case BMU_DISPLAY:
		{
			if (param_val == 0xFFFFFFFF) {
				g_bmu_dev.bmu_log_out = BMU_LOG_DEV_UART;
				g_bmu_dev.bmu_log_mem_addr = (void*)&g_bmu_temp_monitor_data;
				g_bmu_dev.bmu_pmem_addr = g_bmu_dev.bmu_log_mem_addr;
			}
			else {
				g_bmu_dev.bmu_log_out = BMU_LOG_DEV_MEM;
				g_bmu_dev.bmu_log_mem_addr = (void*)((uint32_t)(param_val & 0xFFFFFFFF));
				g_bmu_dev.bmu_pmem_addr = g_bmu_dev.bmu_log_mem_addr;
			}
		}
		break;

		case BMU_AXI_ID:
		{
			pbmu->BMU_CFG2 &= ~0xffff0000;
			pbmu->BMU_CFG2 |= (uint32_t)param_val << 16;
		}
		break;

		case BMU_AXI_ID_MSK:
		{
			pbmu->BMU_CFG2 &= ~0xffff;
			pbmu->BMU_CFG2 |= (uint32_t)param_val;
		}
		break;

		case BMU_DUA_THR:
		{
			pbmu->BMU_CFG1 = (uint32_t)param_val;
		}
		break;

		case BMU_DUA_THR_COE:
		{
			switch (param_val)
			{
			case 1:
				pbmu->BMU_FLT = 0x300;
				break;
			case 2:
				pbmu->BMU_FLT = 0x11;
				break;
			case 4:
				pbmu->BMU_FLT = 0x22;
				break;
			case 8:
				pbmu->BMU_FLT = 0x44;
				break;
			default:
				pbmu->BMU_FLT = 0x300;
				break;
			}
		}
		break;

		case BMU_FLT_TYPE:
		{
			switch (param_val)
			{
			case 1:
			{
				pbmu->BMU_CFG4 |= 0x10;
			}
			break;
			case 2:
			{
				pbmu->BMU_CFG4 |= 1 << 8;
			}
			break;
			case 3:
			{
				pbmu->BMU_CFG4 |= 0x1000;
			}
			break;
			case 4:
			{
				pbmu->BMU_CFG4 |= 0x1;
			}
			break;
			}
		}
		break;

		case BMU_FLT_START_ADDR:
		{
			pbmu->BMU_CFG6 = param_val & 0xffffffff;
			pbmu->BMU_CFG7 = (param_val >> 32) & 0x3;
		}
		break;

		case BMU_FLT_END_ADDR:
		{
			pbmu->BMU_CFG10 = param_val & 0xffffffff;
			pbmu->BMU_CFG11 = (param_val >> 32) & 0x3;
		}
		break;

		case BMU_FLT_SIZE:
		{
			pbmu->BMU_CFG4 &= ~(0x7 << 16);
			pbmu->BMU_CFG4 |= (param_val & 0x7) << 16;
		}
		break;

		case BMU_FLT_LEN:
		{
			pbmu->BMU_CFG5 &= ~0xff;
			pbmu->BMU_CFG5 |= (param_val & 0xff);
		}
		break;

		case BMU_FLT_ALIGN:
		{
			pbmu->BMU_CFG0 &= ~(0xff << 16);
			pbmu->BMU_CFG0 |= (param_val & 0xff) << 16;
		}
		break;

		default:
			break;
		}

		i++;
	} while (i < bmu_param_num);

	return CSI_OK;
}

static void _aon_bmu_stop_master(uint32_t bmu_msk)
{
	uint32_t i = 0;
	uint32_t reg_val;

	while (bmu_msk) {
		if (bmu_msk & 0x1) {
			reg_val = readl(g_bmu_reg_base[i]);
			reg_val &= ~0x2;
			writel(g_bmu_reg_base[i], reg_val);
			reg_val = readl(g_bmu_reg_base[i]);
		}
		i++;
		bmu_msk >>= 1;
	}
}

static void _aon_bmu_start_master(uint32_t bmu_msk)
{
	uint32_t i = 0;
	uint32_t val;

	while (bmu_msk) {
		if (bmu_msk & 0x1) {
			val = readl(g_bmu_reg_base[i]);
			val |= 0x2;
			writel(g_bmu_reg_base[i], val);
		}
		i++;
		bmu_msk >>= 1;
	}
}
static uint8_t _aon_bmu_stop(bmu_dev_t* bmu_dev)
{
	_aon_bmu_stop_master(bmu_dev->bmu_master);
	bmu_dev->bmu_fsm_st = BMU_FSM_IDLE,
		//bmu_dev->bmu_log_out       = BMU_LOG_DEV_UART,
		bmu_dev->bmu_master_bk = bmu_dev->bmu_master;
	bmu_dev->bmu_master = 0,
		//bmu_dev->bmu_sample_cnt    = 0xFFFFFFFF,
		//bmu_dev->bmu_sample_cur_cnt = 0;
		//bmu_dev->bmu_log_mem_addr  = (void*)0xFFFFFFFF,
		//csi_timer_detach_callback(&bmu_dev->thandle);
		csi_timer_stop(&bmu_dev->thandle);

	return AON_ERR_NONE;
}
static uint8_t _aon_bmu_start(bmu_dev_t* bmu_dev)
{
	csi_error_t ret;
	if (bmu_dev->bmu_fsm_st != BMU_FSM_CFG)
		return AON_ERR_UNAVAILABLE;
	/*bmu sample */
	if (bmu_dev->bmu_sample_cnt > 1) {
		aon_bmu_cfg_param_to_mem(&g_bmu_dev); //printf cfg to mem
		bmu_dev->bmu_sample_cur_cnt = 0;
		_aon_bmu_start_master(bmu_dev->bmu_master);
		ret = csi_timer_start(&bmu_dev->thandle, bmu_dev->bmu_sample_timer);
		if (ret)
			return AON_ERR_FAIL;
		bmu_dev->bmu_fsm_st = BMU_FSM_RUN;

	}
	else if (bmu_dev->bmu_sample_cnt == 1) {
		/* start bmu ,then stop at once */
		uint32_t i = 0;
		uint32_t val;
		uint32_t bmu_msk = bmu_dev->bmu_master;
		bmu_dev->bmu_sample_cur_cnt = 0;

		while (bmu_msk) {
			if (bmu_msk & 0x1) {
				val = readl(g_bmu_reg_base[i]);
				val |= (1 << 8);
				writel(g_bmu_reg_base[i], val);
			}
			i++;
			bmu_msk >>= 1;
		}
		_aon_bmu_start_master(bmu_dev->bmu_master);
		_aon_bmu_stop_master(bmu_dev->bmu_master);
		bmu_dev->bmu_sample_cur_cnt = 1;
		udelay(bmu_dev->bmu_sample_timer + 100);
		aon_bmu_sample_data_to_mem(bmu_dev);
		aon_bmu_sample_data_to_uart(bmu_dev);
		bmu_dev->bmu_fsm_st = BMU_FSM_IDLE;
	}
	return AON_ERR_NONE;
}
/*--------------------------------------------------------------------------*/
/* bmu config bmu-param cmd                                                      */
/*--------------------------------------------------------------------------*/
static uint8_t aon_cmd_bmu_cfg(int argc, char* argv[])
{
	////return 0;
	csi_error_t ret;
	bmu_param_t usr_param_list[BMU_PARAM_ID_MAX];

	uint32_t param_idx = 0;
	uint32_t param_id = 0;
	uint32_t bmu_num_sel_msk, tem_msk;
	uint32_t param_val_num;
	uint64_t temp;
	uint64_t param_val = 0;
	uint32_t exec_flag = 1;
	uint32_t i;

	/*set usr param list as default */
	memcpy(usr_param_list, g_bmu_cmd_param_list, sizeof(g_bmu_cmd_param_list));
	aon_bmu_get_param_value(g_bmu_cmd_param_list, BMU_MASTER, &param_val);
	bmu_num_sel_msk = param_val & 0xFFFFFFFF;

	/*parser user cfg cmd*/
	param_idx = 0;
	while (param_idx < argc) {
		/*parser param name*/
		ret = aon_bmu_get_param_id_byname(argv[param_idx],
			&param_id,
			&param_val_num,
			g_bmu_cmd_param_list,
			ARRAY_SIZE(g_bmu_cmd_param_list));
		if (ret) {
			printf("unknow param_name:%d %s\n", param_id, argv[param_idx]);
			return AON_ERR_PARM;
		}
		if (param_val_num > 2) {
			printf("unsupport param value number indicated by[%s]\n", argv[param_idx]);
			return AON_ERR_PARM;
		}
		param_idx++;
		/*parser param value specified by param-name,and save them */
		uint32_t offset = 0;
		param_val = 0;
		while (offset < param_val_num) {

			temp = strtoul(argv[param_idx], NULL, 0);
			param_val |= temp << (32 * offset);
			offset++;
			param_idx++;
		}
		/*swap param_val*/
		if (param_val_num == 2)
			param_val = (param_val << 32) | (param_val >> 32);

		/*modify user param value*/
		if (offset)
			aon_bmu_set_param_value(usr_param_list, param_id, param_val);
		/*whether exist BMU_CMD_EXEC param_id */
		if (param_id == BMU_CMD_EXEC)
			exec_flag = 0;
		/*update by cfg cmd*/
		if (param_id == BMU_MASTER)
			bmu_num_sel_msk = param_val;
	}

	/*set log-mem addr*/
	aon_bmu_get_param_value(usr_param_list, BMU_DISPLAY, &param_val);
	if (param_val == 0xFFFFFFFF) {
		g_bmu_dev.bmu_log_mem_addr = &g_bmu_temp_monitor_data;
		g_bmu_dev.bmu_pmem_addr = (uint32_t*)&g_bmu_temp_monitor_data;
	}

	/*save user cmd param list*/
	aon_bmu_save_usr_bmu_cfg(g_bmu_usr_cmd_param_list, bmu_num_sel_msk, usr_param_list);

	/*set default reg */
	i = 0;
	tem_msk = bmu_num_sel_msk;
	while (tem_msk) {
		if (tem_msk & 0x1)
		{
			aon_bmu_default_cfg((bmu_reg_map_t*)g_bmu_reg_base[i]);
		}
		i++;
		tem_msk >>= 1;
	}
	/*config bmux reg by cmd param list*/
	i = 0;
	tem_msk = bmu_num_sel_msk;
	while (tem_msk) {
		if (tem_msk & 1) {
			ret = aon_bmu_init_user_cfg(g_bmu_reg_base[i], usr_param_list, ARRAY_SIZE(usr_param_list));
			if (ret)
				return AON_ERR_FAIL;
		}
		i++;
		tem_msk >>= 1;
	}

#if 0
	tem_msk = bmu_num_sel_msk;
	i = 0;
	while (tem_msk) {
		if (tem_msk & 1) {
			bmu_reg_map_t* preg = (bmu_reg_map_t*)g_bmu_reg_base[i];
			printf("bmu[%02d]_cfg0:%08x\n", i, preg->BMU_CFG0);
			printf("bmu[%02d]_cfg1:%08x\n", i, preg->BMU_CFG1);
			printf("bmu[%02d]_cfg2:%08x\n", i, preg->BMU_CFG2);
			printf("bmu[%02d]_cfg3:%08x\n", i, preg->BMU_CFG3);
			printf("bmu[%02d]_cfg4:%08x\n", i, preg->BMU_CFG4);
			printf("bmu[%02d]_cfg5:%08x\n", i, preg->BMU_CFG5);
			printf("bmu[%02d]_cfg6:%08x\n", i, preg->BMU_CFG6);
			printf("bmu[%02d]_cfg7:%08x\n", i, preg->BMU_CFG7);
			printf("bmu[%02d]_cfg8:%08x\n", i, preg->BMU_CFG8);
			printf("bmu[%02d]_cfg9:%08x\n", i, preg->BMU_CFG9);
			printf("bmu[%02d]_cfg10:%08x\n", i, preg->BMU_CFG10);
			printf("bmu[%02d]_cfg11:%08x\n", i, preg->BMU_CFG11);
			printf("bmu[%02d]_cfg12:%08x\n", i, preg->BMU_CFG12);
			printf("bmu[%02d]_cfg13:%08x\n", i, preg->BMU_CFG13);
			printf("bmu[%02d]_flt:%08x\n", i, preg->BMU_FLT);
			printf("bmu[%02d]_cfg14:%08x\n", i, preg->BMU_CFG14);
			printf("bmu[%02d]_ostd_cfg:%08x\n", i, preg->BMU_STATUS4[1]);

		}
		i++;
		tem_msk >>= 1;
	}
#endif
	/* param exclusive check */
	uint64_t mode, mem_addr;
	aon_bmu_get_param_value(usr_param_list, BMU_MODE, &mode);
	aon_bmu_get_param_value(usr_param_list, BMU_DISPLAY, &mem_addr);
	if (mode > 0 && (void*)((uint32_t)mem_addr) == (void*)0xffffffff) {
		printf("please set -display with specic mem addr \n");
		return AON_ERR_PARM;
	}
	g_bmu_dev.bmu_fsm_st = BMU_FSM_CFG;
	/*whether to exec bmu cmd at once: todo*/
	if (exec_flag) {
		//aon_bmu_cfg_param_to_mem(&g_bmu_dev); //printf cfg to mem
		ret = _aon_bmu_start((bmu_dev_t*)&g_bmu_dev);
		if (ret) {
			return ret;
		}
	}
	return ret;
}

/*--------------------------------------------------------------------------*/
/* bmu start monitor cmd                                                      */
/*--------------------------------------------------------------------------*/
static uint8_t aon_cmd_bmu_start(int argc, char* argv[])
{
	if (argc > 0)
		return AON_ERR_PARM;
	return _aon_bmu_start((bmu_dev_t*)&g_bmu_dev);
}

/*--------------------------------------------------------------------------*/
/* bmu stop monitor cmd                                                      */
/*--------------------------------------------------------------------------*/
static uint8_t aon_cmd_bmu_stop(int argc, char* argv[])
{
	if (argc > 0)
		return AON_ERR_PARM;
	//_aon_bmu_stop((bmu_dev_t*)&g_bmu_dev);
	aon_bmu_sample_data_to_uart(&g_bmu_dev);
	return AON_ERR_NONE;
}
#endif
/*--------------------------------------------------------------------------*/
/* system reboot cmd                                                        */
/*--------------------------------------------------------------------------*/

extern void system_reset();

static uint8_t aon_cmd_system_reboot()
{
	////return 0;
	uint32_t autoboot_ena;
	int ret;
	csi_pmic_t *pmic  = aon_pmic_get_handler();

	ret = csi_pmic_get_autoboot_flag(pmic, &autoboot_ena);
	if (ret < 0) {
		printf("pmic wdt is not supported\n");
		return CSI_ERROR;
	}

	if (autoboot_ena) {
		printf("the boad AUTO_BOOT OTP configured system restarting......\n");
		csi_pmic_repower(pmic);
	}
	else {
		printf("the boad no AUTO_BOOT OTP configured system restarting......\n");
		mdelay(1000);
		system_reset();
	}

	/* Never go here */
	printf("BUG: should not go here!!!\n");
	__WFI();

	return CSI_ERROR;
}

/*--------------------------------------------------------------------------*/
/* pmic wdog cmd                                                      */
/*--------------------------------------------------------------------------*/
static uint8_t aon_cmd_wdog_ops(int argc, char* argv[])
{
	//return 0;
	int err = 0;
	if (argc < 1)
		return AON_ERR_PARM;

	uint8_t ops = strtoul(argv[0], NULL, 0);
	csi_pmic_t *pmic = aon_pmic_get_handler();

	switch (ops) {
	case 1: { // start wdog
		if (argc < 2)
			return AON_ERR_PARM;
		uint8_t set = strtoul(argv[1], NULL, 0);
		err = csi_pmic_wdt_set_timer(pmic, set * 1000);
		if (err) {
			return err;
		}
		err = csi_pmic_wdt_start(pmic);
		if (err) {
			return err;
		}
	} break;
	case 0: {//stop wdog
		err = csi_pmic_wdt_stop(pmic);
		if (err) {
			return err;
		}
	} break;
	case 2: {//feed
		err = csi_pmic_wdt_feed(pmic);
		if (err) {
			return err;
		}
	} break;
	default:
		return AON_ERR_PARM;
	}
	return AON_ERR_NONE;
}

/*--------------------------------------------------------------------------*/
/* pmic log level cmd                                                      */
/*--------------------------------------------------------------------------*/
static uint8_t aon_cmd_log_ops(int argc, char* argv[])
{
	return 0;
	int err = 0;
	if (argc < 2)
		return AON_ERR_PARM;
    csi_pmic_t *pmic = aon_pmic_get_handler();
	uint8_t pmic_id = strtoul(argv[0], NULL, 0);
	uint8_t level = strtoul(argv[1], NULL, 0);

	err = csi_pmic_set_log_level(pmic, pmic_id, level);
	if (err) {
		return err;
	}
	return AON_ERR_NONE;
}


/*--------------------------------------------------------------------------*/
/* Memory dump command                                                      */
/*--------------------------------------------------------------------------*/
static uint8_t aon_cmd_md(int argc, char* argv[], int len)
{
	////return 0;
	uint32_t addr;
	uint32_t count = 64 / len;

	/* Parse first argument */
	if (argc == 0)
	{
		return AON_ERR_PARM;
	}
	else
	{
		addr = strtoul(argv[0], NULL, 0);
	}

	/* Parse second argument */
	if (argc > 1)
	{
		count = strtoul(argv[1], NULL, 0);
	}
	else {
		count = 1;
	}

	switch (len)
	{
	case BYTE:
	{
		uint8_t* x = (uint8_t*)addr;

		for (uint32_t i = 0U; i < count; i++)
		{
			if ((i % 16U) == 0U)
			{
				printf("%08x: ", (uint32_t)x);
			}

			uint8_t v = 0U;
			if (aon_memory_rw(x, &v, 8U) == 0U)
			{
				printf("%02x ", v);
			}
			else
			{
				printf("?? ");
			}
			x++;

			if (((i + 1U) % 16U) == 0U)
			{
				printf("\n");
			}
		}
	}
	break;
	case WORD:
	{
		uint16_t* x = (uint16_t*)(addr & ~0x1);

		for (uint32_t i = 0U; i < count; i++)
		{
			if ((i % 8U) == 0U)
			{
				printf("%08x: ", (uint32_t)x);
			}

			uint16_t v = 0U;
			if (aon_memory_rw(x, &v, 16U) == 0U)
			{
				printf("%04x ", v);
			}
			else
			{
				printf("???? ");
			}
			x++;

			if (((i + 1U) % 8U) == 0U)
			{
				printf("\n");
			}
		}
	}
	break;
	case LONG:
	{
		uint32_t* x = (uint32_t*)(addr & ~0x3);

		for (uint32_t i = 0U; i < count; i++)
		{
			if ((i % 4U) == 0U)
			{
				printf("%08x: ", (uint32_t)x);
			}

			uint32_t v = 0U;
			if (aon_memory_rw(x, &v, 32U) == 0U)
			{
				printf("%08x ", v);
			}
			else
			{
				printf("???????? ");
			}
			x++;

			if (((i + 1U) % 4U) == 0U)
			{
				printf("\n");
			}
		}
	}
	break;
	default:
		;
		break;
	}
	if ((count % (16 / len)) != 0)
	{
		printf("\n");
	}

	return AON_ERR_NONE;
}

/*--------------------------------------------------------------------------*/
/* Memory/registers modify command                                          */
/*--------------------------------------------------------------------------*/
static uint8_t aon_cmd_mm(int argc, char* argv[], int len)
{
	////return 0;
	uint32_t addr, data;
	uint8_t err = AON_ERR_NONE;

	/* Parse arguments */
	if (argc < 2)
	{
		return AON_ERR_PARM;
	}
	else
	{
		addr = strtoul(argv[0], NULL, 0);
		data = strtoul(argv[1], NULL, 0);
	}
	switch (len)
	{
	case BYTE:
	{
		uint8_t v = 0U;
		if (aon_memory_rw((void*)addr, &v, 8U) == 0U)
		{
			*((uint8_t*)addr) = U8(data);
		}
		else
		{
			err = AON_ERR_FAIL;
		}
	}
	break;
	case WORD:
	{
		uint16_t v = 0U;
		if (aon_memory_rw((void*)addr, &v, 16U) == 0U)
		{
			*((uint16_t*)addr) = U16(data);
		}
		else
		{
			err = AON_ERR_FAIL;
		}
	}
	break;

	case LONG:
	{
		uint32_t v = 0U;
		if (aon_memory_rw((void*)addr, &v, 32U) == 0U)
		{
			*((uint32_t*)addr) = U32(data);
		}
		else
		{
			err = AON_ERR_FAIL;
		}
	}
	break;
	default:
		;
		break;
	}

	return err;
}

#ifdef  CONFIG_AUDIO_ENABLE
/*--------------------------------------------------------------------------*/
/* audio power ctrl cmd                                          */
/*--------------------------------------------------------------------------*/
static uint8_t aon_cmd_audio(int argc, char* argv[])
{
	uint32_t cmd;
	uint8_t err = AON_ERR_NONE;

	/* Parse arguments */
	if (argc < 1)
	{
		return AON_ERR_PARM;
	}
	else
	{
		cmd = strtoul(argv[0], NULL, 0);
	}

	if (cmd == 0) {
       audio_powerdown();
	} else {
	   audio_powerup();
	}

	return err;
}
#endif

#ifdef CONFIG_AON_MONITOR_PVT
/* cmd list:
 *
 * get all pvt-ts sensor data
 * pvt.ts.r
 *
 * get all pvt-pd sensor data,each sensor has six-delay-chain
 * pvt.pd.r
 *
 * get all pvt-vm sensor data,each sensor has 16-channel vm data
 * pvt.vm.r
 *
 */
static uint8_t aon_cmd_pvt(int argc, char* argv[], int rw)
{
	//return 0;
	uint8_t ret = AON_ERR_NONE;
	uint32_t i, j, raw_data, cvt_data;

	if (argc > 0)
		return AON_ERR_PARM;
	UNUSED(argv);

	switch (rw) {
	case READ_PD:
	{
		uint32_t pd_all_data[pvt.sensor_info.pd_num][6];
		memset(pd_all_data, 0, sizeof(pd_all_data));
		for (i = 0; i < 6; i++) {
			/* select delay-chain port */
			ret = csi_pvt_pd_port_select(&pvt, i + 1);
			if (ret)
				return ret;
			/*read pd-sensor data*/
			for (j = 0; j < pvt.sensor_info.pd_num; j++) {
				ret = csi_pvt_read_pd(&pvt, j, &pd_all_data[j][i]);
				if (ret)
					return ret;
			}

		}
		/*display all pd-data */
		for (j = 0; j < pvt.sensor_info.pd_num; j++) {
			printf("pd[%d]: %d %d %d %d %d %d \n", j,
				pd_all_data[j][0],
				pd_all_data[j][1],
				pd_all_data[j][2],
				pd_all_data[j][3],
				pd_all_data[j][4],
				pd_all_data[j][5]);

		}
	}
	break;

	case READ_VM:
	{

		uint32_t ch_all_data[16] = { 0 };
		printf("\n");
		for (i = 0; i < pvt.sensor_info.vm_num; i++) {
			ret = csi_pvt_read_vm_muti_ch(&pvt, i, 16, (uint32_t*)&ch_all_data);
			if (ret)
				return ret;

			for (j = 0; j < 16; j++) {
				csi_pvt_vm_convert_raw_data(&pvt, ch_all_data[j], &cvt_data);
				printf("pvt_vm[%d]_ch[%d]:%d %d\n", i, j, ch_all_data[j], cvt_data);
			}

		}
	}
	break;

	case READ_TS:
	{
		printf("\n");
		for (i = 0; i < pvt.sensor_info.ts_num; i++) {
			ret = csi_pvt_read_ts(&pvt, i, &raw_data);
			if (ret)
				return ret;
			csi_pvt_ts_convert_raw_data(&pvt, raw_data, &cvt_data);
			printf("pvt_ts[%d]:%d %d\n", i, raw_data, cvt_data);

		}
	}
	break;

	default:
		return AON_ERR_PARM;
	}

	return ret;
}
#endif

void aon_get_power_switch(void)
{
	uint32_t temp;
	char* str_temp[] = { "off","on","on","on" };

	printf("dump all power switch as follow \n");
	temp = readl(0xfff48124);
	printf("1.audio:%s\n", str_temp[temp & 0x3]);
	temp >>= 2;
	printf("2.vdec:%s\n", str_temp[temp & 0x3]);
	temp >>= 2;
	printf("3.npu:%s\n", str_temp[temp & 0x3]);
	temp >>= 2;
	printf("4.venc:%s\n", str_temp[temp & 0x3]);
	temp >>= 2;
	printf("5.gpu:%s\n", str_temp[temp & 0x3]);
	temp >>= 2;
	printf("6.dsp0:%s\n", str_temp[temp & 0x3]);
	temp >>= 2;
	printf("7.dsp1:%s\n", str_temp[temp & 0x3]);
	temp >>= 2;
	printf("8.c910-core0:%s\n", str_temp[temp & 0x3]);
	temp >>= 2;
	printf("9.c910-core1:%s\n", str_temp[temp & 0x3]);
	temp >>= 2;
	printf("10.c910-core2:%s\n", str_temp[temp & 0x3]);
	temp >>= 2;
	printf("11.c910-core3:%s\n", str_temp[temp & 0x3]);
	temp >>= 2;
}

static inline void aon_clksrc_sel(uint32_t addr, uint32_t src, uint32_t en_msk, uint32_t src_msk)
{
	uint32_t temp;

	/*disable clk en and set clk src*/
	temp = readl(addr);
	temp &= ~(en_msk | src_msk);
	writel(addr, temp);
	/*enable clk src*/
	temp |= en_msk | (src & src_msk);
	writel(addr, temp);
	mdelay(10);
}

static inline void  aon_clksrc_sel2(uint32_t addr, uint32_t src, uint32_t en_msk, uint32_t src_msk)
{
	uint32_t temp;

	/*disable clk en and set clk src*/
	temp = readl(addr);
	temp &= ~(en_msk | src_msk << 4);
	writel(addr, temp);
	/*enable clk src*/
	temp |= en_msk | ((src & src_msk) << 4);
	writel(addr, temp);
	mdelay(10);
}

void aon_get_clkfreq(void)
{
	uint32_t str_idx = 1;
	const char* clk_src1[] = {
		"NA_c910_cclk",
		"cpusys_int_pclk",
		"cpusys_int_aclk",
		"cpusys_int_hclk",
		"c910_mbus_aclk",
		"sys_cfg_aclk",
		"teesys_hclk",
		"NA",
		"perisys_hclk",
		"perisys_pclk",
		"npu_cclk",
		"cfg_apb_pclk",
		"peri2sys_pclk",
		"venc_cclk",
		"c910_c0_clkdiv",
		"c910_c1_clkdiv",
		"c910_c2_clkdiv",
		"c910_c3_clkdiv",
		"peri_i2s_src_clk",
		"vosys_aclk_m",
		"visys_aclk_m",
		"visys_ahb_hclk",
		"visys_apb_pclk",
		"vpsys_axi_aclk",
		"dpu0_pll_div_clk",
		"dpu1_pll_div_clk",
	};

	const char* clk_src2[] = {
		"share_sram_clk",
		"aonsys_clk",
		"audio_i2s_src_clk",
		"audio_c906_cclk",
		"rc_clk",
		"audio_subsys_aclk",
	};
	const char* clk_src3[] = {
		"ddr_postdiv",
		"NA",
		"fout4",
	};
	const char* clk_src4[] = {
		"visys_dw200_clk_dwe",
		"visys_dw200_clk_vse",
		"visys_isp0_clk",
		"visys_isp1_clk",
		"visys_isp_ry_cclk",
		"visys_mipi_csi0_pixelclk",
		"visys_pclk",
	};
	const char* clk_src5[] = {
		"vosys_dpu_cclk",
		"vosys_pclk",
	};
	const char* clk_src6[] = {
		"vpsys_fce_aclk",
		"vpsys_g2d_cclk",
		"vpsys_vdec_cclk",
		"vpsys_axi_clk",
		"vpsys_apb_clk",
	};
	const char* clk_src7[] = {
		"dsp0_clk_cde",
		"dsp0_mclk_cde",
		"dsp0_sclk_cde",
		"dsp1_clk_cde",
		"dsp1_mclk_cde",
		"dsp1_sclk_cde",
	};

	uint32_t clk_idx;
	uint32_t val;
	printf("dump all clkfreq as follow \n");

	for (clk_idx = 0; clk_idx < ARRAY_SIZE(clk_src1); clk_idx++)
	{
		aon_clksrc_sel(0xff011300, clk_idx, 0x20, 0x1f);
		val = readl(0xff011304);
		printf("%d:%s:%d KHz \n", str_idx++, clk_src1[clk_idx], val);
	}

	for (clk_idx = 0; clk_idx < ARRAY_SIZE(clk_src2); clk_idx++)
	{
		aon_clksrc_sel(0xfff46180, clk_idx, 0x10, 0xf);
		val = readl(0xfff46184);
		printf("%d:%s:%d KHz \n", str_idx++, clk_src2[clk_idx], val);
	}

	for (clk_idx = 0; clk_idx < ARRAY_SIZE(clk_src3); clk_idx++)
	{
		aon_clksrc_sel2(0xff005024, clk_idx, 0x1, 0xf);
		val = readl(0xff005020);
		printf("%d:%s:%d KHz \n", str_idx++, clk_src3[clk_idx], val);
	}

	for (clk_idx = 0; clk_idx < ARRAY_SIZE(clk_src4); clk_idx++)
	{
		aon_clksrc_sel2(0xf404110c, clk_idx, 0x1, 0xf);
		val = readl(0xf4041108);
		printf("%d:%s:%d KHz \n", str_idx++, clk_src4[clk_idx], val);
	}

	for (clk_idx = 0; clk_idx < ARRAY_SIZE(clk_src5); clk_idx++)
	{
		aon_clksrc_sel2(0xff5290c8, clk_idx, 0x1, 0xf);
		val = readl(0xff5290c4);
		printf("%d:%s:%d KHz \n", str_idx++, clk_src5[clk_idx], val);
	}

	for (clk_idx = 0; clk_idx < ARRAY_SIZE(clk_src6); clk_idx++)
	{
		aon_clksrc_sel(0xfcc31050, clk_idx, 0x10, 0xf);
		val = readl(0xfcc31054);
		printf("%d:%s:%d KHz \n", str_idx++, clk_src6[clk_idx], val);
	}
	for (clk_idx = 0; clk_idx < ARRAY_SIZE(clk_src7); clk_idx++)
	{
		aon_clksrc_sel2(0xff041030, clk_idx, 0x1, 0xf);
		val = readl(0xff04102c);
		printf("%d:%s:%d KHz \n", str_idx++, clk_src7[clk_idx], val);
	}
}

typedef struct {
	char* reg_name;
	uint32_t reg_addr;
}reg_dump_t;

void aon_get_clken(void)
{
	reg_dump_t reg_list[] = {
		{"AUDIO_CLK_GATE", 0xfff46114},
		{"AONSYS_CLK_GATE",0xfff46120},
		{"APSYS_CLK_GATE",0xfff46130},
		{"DSP0_CLK_GATE",0xfff46134},
		{"DSP1_CLK_GATE",0xfff46138},
		{"GPU_CLK_GATE",0xfff4613c},
		{"VDEC_CLK_GATE",0xfff46140},
		{"VENC_CLK_GATE",0xfff46144},
		{"C910_CLK_CFG",0xbf010100},
		{"C910_CORECLK_CFG",0xbf010104},
		{"AXI_CPUSYS_ACLK_CFG",0xbf010134},
		{"CFG_AXI_ACLK_CFG",0xbf010138},
		{"PERISYS_AHB_HCLK_CFG",0xbf010140},
		{"NPU_CCLK_CFG",0xbf0101c8},
		{"VISYS_CLK_CFG",0xbf0101d0},
		{"VOSYS_CLK_CFG",0xbf0101dc},
		{"VPSYS_CLK_CFG",0xbf0101e0},
		{"VENC_CCLK_CFG",0xbf0101e4},
		{"DPU0_PLL_DIV_CFG",0xbf0101e8},
		{"DPU1_PLL_DIV_CFG",0xbf0101ec},
		{"PERI_I2S_SRC_CLK_CFG",0xbf0101f0},
		{"PERI_CLK_CFG",0xbf010204},
		{"SRAM_AXI_CLK_CFG",0xbf01020c},
		{"SUBSYS_CLK_CFG",0xbf010220},
		{"DDR_PLL_STS",0xff005018},
		{"MISCSYS_BUS_CLK_CTRL",0xbc02c100},
		{"MISCSYS_USB_CLK_CTRL",0xbc02c104},
		{"MISCSYS_EMMC_CLK_CTRL",0xbc02c108},
		{"MISCSYS_SDIO0_CLK_CTRL",0xbc02c10c},
		{"MISCSYS_SDIO1_CLK_CTRL",0xbc02c110},
		{"DW200_CLK_DWE_CFG",0xb4040014},
		{"DW200_CLK_VSE_CFG",0xb4040018},
		{"ISP0_CLK_CFG",0xb4040024},
		{"ISP1_CLK_CFG",0xb4040028},
		{"ISP_RY_CLK_CFG",0xb404002c},
		{"MIPI_CSI0_PIXELCLK",0xb4040030},
		{"VOSYS_CLK_GATE",0xbf528050},
		{"VOSYS_CLK_GATE1",0xbf528054},
		{"VOSYS_DPU_CCLK_CFG",0xbf528064},
		{"VPSYS_CLK_CFG",0xbcc30020},
		{"VPSYS_VDEC_CCLK_CFG",0xbcc30024},
		{"VPSYS_FCE_CCLK_CFG",0xbcc3002c},
		{"VPSYS_G2D_CCLK_CFG",0xbcc30030},
		{"DSP0_CLK_CFG",0xbf040000},
		{"DSP1_CLK_CFG",0xbf040004},
		{"DSP_CLK_CFG",0xbf040008},
		{"DSPSYS_CLK_GATE_EN1",0xbf040024},
		/*{"IP_CG_REG",0xcb000010},*/
	};

	uint32_t i;
	uint32_t val;
	printf("\n dump all clk en reg as follow \n");
	for (i = 0; i < ARRAY_SIZE(reg_list); i++) {
		val = readl(reg_list[i].reg_addr);
		printf("%d:%s:0x%08X\n", i, reg_list[i].reg_name, val);
	}

}

void  aon_get_softrst(void)
{
	reg_dump_t reg_list[] = {
	{"E902_RST_CFG",0xfff44024},
	{"APSYS_RST_CFG",0xfff44030},
	{"NPUSYS_RST_CFG",0xfff44034},
	{"DDRSYS_RST_CFG",0xfff44038},
	{"AUDIO_RST_CFG",0xfff4403c},
	{"DSP0_RST_CFG",0xfff44054},
	{"DSP1_RST_CFG",0xfff44058},
	{"GPU_RST_CFG",0xfff4405c},
	{"VDEC_RST_CFG",0xfff44060},
	{"VENC_RST_CFG",0xfff44064},
	{"C910_SWRST",0xbf014004},
	{"AXI4_CPUSYS2_SWRST",0xbf014010},
	{"X2X_CPUSYS_SWRST",0xbf014014},
	{"GMAC0_SWRST",0xbf014068},
	{"QSPI0_SWRST",0xbf01408c},
	{"QSPI1_SWRST",0xbf014090},
	{"DDR_CFG0",0xff005000},
	{"EMMC_SWRST",0xbc02c000},
	{"MISC_AXI_SWRST",0xbc02c008},
	{"SDIO0_SWRST",0xbc02c00c},
	{"SDIO1_SWRST",0xbc02c010},
	{"USB3_DRD_SWRST",0xbc02c014},
	{"VISYS_SW_RST",0xb4040100},
	{"VISYS_SW_RST2",0xb4040104},
	{"GPU_RST_CFG",0xbf528000},
	{"DPU_RST_CFG",0xbf528004},
	{"MIPI_DSI0_RST_CFG",0xbf528008},
	{"MIPI_DSI1_RST_CFG",0xbf52800c},
	{"HDMI_RST_CFG",0xbf528014},
	{"AXIBUS_RST_CFG",0xbcc30000},
	{"FCE_RST_CFG",0xbcc30004},
	{"G2D_RST_CFG",0xbcc30008},
	{"VDEC_RST_CFG",0xbcc3000c},
	{"VENC_RST_CFG",0xbcc30010},
	{"DSPSYS_SW_RST",0xbf040000},
	/*{"IP_RST_REG",0xcb000014},*/
	};

	uint32_t i;
	uint32_t val;
	printf("\n dump all reset reg as follow \n");
	for (i = 0; i < ARRAY_SIZE(reg_list); i++) {
		val = readl(reg_list[i].reg_addr);
		printf("%d:%s:0x%08X\n", i, reg_list[i].reg_name, val);
	}

}
static uint8_t aon_cmd_lpwr(int argc, char* argv[], int rw)
{
	return 0;
	if (argc > 0)
		return AON_ERR_PARM;
	UNUSED(argv);

	switch (rw)
	{
	case GET_PS: /* read status */
		aon_get_power_switch();
		break;
	case GET_CLKFREQ: /* enable/disable */
		aon_get_clkfreq();
		break;
	case GET_CLKEN: /* set */
		aon_get_clken();
		break;
	case GET_SOFTRST: /* get vol */
		aon_get_softrst();
		break;
	default:
		return AON_ERR_PARM;
	}

	return AON_ERR_NONE;
}

/* cmd list:
 *
 * list all regulator id
 * pmic.id.list
 *
 * regulator enable/disable status:
 * pmic.r regu_ext_id
 *
 * regulator enable/disable:
 * pmic.w regu_ext_id 1/0
 *
 * regulator voltage set: (unit: uV)
 * pmic.s regu_ext_id val
 *
 * dual voltages rail set: (unit: uV)
 * pmic.d.s regu_ext_id dc1 dc2
 *
 * regulator voltage get: (unit: uV)
 * pmic.g regu_ext_id
 */
static uint8_t aon_cmd_pmic(int argc, char* argv[], int rw)
{
	//return 0;
	uint32_t regu_ext_id = 0xFF;
	uint32_t val = 0;
	uint32_t dc1 = 0, dc2 = 0;
	uint8_t ret = AON_ERR_NONE;
	regu_voltage_param_t regu_vol = {0};
	csi_pmic_t *pmic = aon_pmic_get_handler();

	if (rw == READ || rw == GET) {
		if (argc < 1)
		{
			return AON_ERR_PARM;
		}
		else
		{
			regu_ext_id = strtoul(argv[0], NULL, 0);
		}
	}
	if (rw == WRITE || rw == SET || rw == GET_REG) {
		if (argc < 2)
		{
			return AON_ERR_PARM;
		}
		else
		{
			regu_ext_id = strtoul(argv[0], NULL, 0);
			val = strtoul(argv[1], NULL, 0);
		}
	}
	if (rw == DUAL_SET || rw == SET_REG) {
		if (argc < 3)
		{
			return AON_ERR_PARM;
		}
		else
		{
			regu_ext_id = strtoul(argv[0], NULL, 0);
			dc1 = strtoul(argv[1], NULL, 0);
			dc2 = strtoul(argv[2], NULL, 0);
		}
	}

	if (rw == LIST && argc > 0) {
		return AON_ERR_PARM;
	}


	switch (rw)
	{
	case READ: /* read status */
	{
		uint32_t benable;
		ret = csi_pmic_regulator_is_enable(pmic, regu_ext_id, &benable);
		if (ret) {
			return AON_ERR_FAIL;
		}
		if (benable)
			printf("regu_ext_id[%d]: on\n", regu_ext_id);
		else
			printf("regu_ext_id[%d]: off\n", regu_ext_id);
	}
	break;
	case WRITE: /* enable/disable */
	{
		ret = csi_pmic_regulator_ctrl(pmic, regu_ext_id, val);
		if (ret) {
			return AON_ERR_FAIL;
		}
	}
	break;
	case SET: /* set */
	{
		regu_vol.regu_ext_id = regu_ext_id;
		regu_vol.target_uv[0]= val;
		regu_vol.target_uv[1] = 0;

		ret = csi_pmic_regulator_set_voltage(pmic, regu_vol);
		if (ret) {
			return AON_ERR_FAIL;
		}
	}
	break;
	case DUAL_SET: /* dual vol set */
	{

		regu_vol.regu_ext_id = regu_ext_id;
		regu_vol.target_uv[0]= dc1;
		regu_vol.target_uv[1] = dc2;

		ret = csi_pmic_regulator_set_voltage(pmic, regu_vol);
		if (ret) {
			return AON_ERR_FAIL;
		}
	}
	break;
	case GET: /* get vol */
	{


		regu_vol.regu_ext_id = regu_ext_id;
		ret = csi_pmic_regulator_get_voltage(pmic, &regu_vol);
		if (ret) {
			return AON_ERR_FAIL;
		}
		printf("regu_ext_id[%d],vol1=%d, vol2=%d\n",
			regu_ext_id, regu_vol.target_uv[0], regu_vol.target_uv[1]);
	}
	break;
	case LIST: /* list all pmic id */
	{
		for(int i = 0;i < pmic->regu_num; i++) {
            printf("\t[%d]:%s\n", pmic->regu_list[i].regu_ext_id, pmic->regu_list[i].regu_ext_id_name);
		}
	}
	break;
	case GET_REG: /* read pmic reg */
	{
		/*regu_ext_id actual is pmic_id*/
		int pmic_id = regu_ext_id;
		/*val actaul is reg addr*/
		uint32_t reg = val;
		uint32_t reg_data = 0;
		ret = csi_pmic_get_reg(pmic, pmic_id, reg, &reg_data);
		if (ret) {
			return AON_ERR_FAIL;
		}
		printf("get pmic:%d reg:0x%x val:0x%x\n",pmic_id, reg, reg_data);
	}
	break;
	case SET_REG: /* write pmic reg */
	{
		/*regu_ext_id actual is pmic_id*/
		int pmic_id = regu_ext_id;
		/*dc1 actaul is reg addr*/
		uint32_t reg = dc1;
		/*dc2 actaul is reg data to set*/
		uint32_t reg_data = dc2;
		ret = csi_pmic_set_reg(pmic, pmic_id, reg, dc2);
		if (ret) {
			return AON_ERR_FAIL;
		}
		printf("set pmic:%d reg:0x%x val:0x%x\n",pmic_id,  reg, reg_data);
	}
	break;
	default:
		return AON_ERR_PARM;
	}

	return AON_ERR_NONE;
}

#ifdef CONFIG_AON_MONITOR_DDR_LP
static uint8_t aon_cmd_ddr(int argc, char* argv[], int lp_mode)
{
	if (argc != 2)
	{
		printf("invaild params, ex. ddr.lp2 3733 1");
		return AON_ERR_PARM;
	}

	int  speed = atoi(argv[0]);
	int  enter = atoi(argv[1]);


	if (!((speed == 2133) || (speed == 3200) || (speed == 3733) || (speed == 4266)))
	{
		printf("unsupport speed configuration<%d>!!!", speed);
		return AON_ERR_PARM;
	}

	printf("ddr %s lp%d, speed %d\n", enter ? "enter" : "exit", lp_mode == LP2_MODE ? 2 : 3, speed);

	if (lp_mode == LP2_MODE)
	{
		if (enter)
		{
			lpddr4_phy_lp2_enter(speed);
		}
		else
		{
			lpddr4_phy_lp2_exit(speed);
		}
	}
	else if (lp_mode == LP3_MODE)
	{
		if (enter)
		{
			lpddr4_phy_lp3_enter(speed);
		}
		else
		{
			lpddr4_phy_lp3_exit(speed);
		}
	}

	return AON_ERR_NONE;
}
#endif

static int findstrn(cmds_info_t* list, int max, char* str)
{
	int r;

	for (r = 0; r < max; r++)
	{
		if (strncasecmp(str, list[r].cmd, strlen(list[r].cmd)) == 0)
		{
			return list[r].id;
		}
	}

	return 0xff;
}

static uint32_t aon_memory_rw(void* addr, void* val, uint8_t width)
{
	switch (width)
	{
	case 8:
		*((uint8_t*)val) = *((uint8_t*)addr);
		break;
	case 16:
		*((uint16_t*)val) = *((uint16_t*)addr);
		break;
	case 32:
		*((uint32_t*)val) = *((uint32_t*)addr);
		break;
	default:
		break;
	}

	return 0U;
}
