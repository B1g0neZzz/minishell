#include "parse.h"
#include "externs.h"
#include "init.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * shell主循环
 */

void shell_loop()
{
    while (1)
    {
        printf("[minishell]$ ");
        fflush(stdout);
        init();
        if (read_command() == -1)
            break;
        parse_command();
        execute_command();
    }
    printf("\nexit\n");
    
}

/**
 * 读取命令
 * 成功返回0，失败或者读取到文件结束符(EOF)返回-1
 */
int read_command()
{
    if (fgets(cmdline, MAXLINE, stdin) == NULL)
        return -1;
    return 0;
}

/**
 * 解析命令
 * 成功返回解析到的命令个数，失败返回-1
 */
int parse_command()
{
    /* ls -l */
    /* ls\0-l\0 */
    char *cp = cmdline;
    char *avp = avline;
    int i = 0;

    while (*cp != '\0')
    {
        /* 去除空格 */
        while (*cp == ' ' || *cp == '\t')
            cp ++;
        /* 如果到了行尾，跳出循环*/
        if (*cp == '\0' || *cp == '\n')
            break;
        cmd.args[i] = avp;

        /* 解析参数 */
        while (*cp != '\0' 
            && *cp != ' ' 
            && *cp != '\t' 
            && *cp != '\n')
            *avp++ = *cp++;
        *avp++ = '\0';
        i++;
    }
    return 0;
}

/**
 * 执行命令
 * 成功返回0，失败返回-1
 */
int execute_command()
{
    pid_t pid = fork();
    if (pid == -1)
        ERR_EXIT("fork");

    if (pid == 0)
        execvp(cmd.args[0], cmd.args);
        
    wait(NULL);
    return 0;
}