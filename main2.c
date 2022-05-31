#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include "minishell.h"

int builtin_env(int argc, char *argv[], t_shellenv *sh)
{
    t_env *env;

    env = sh->envlist;
    while (env)
    {
        printf("%s=%s\n", env->name, env->val);
        env = env->next;
    }
    return (0);
}

static int ft_strlen(char *p)
{
    int i;

    i = 0;
    while (p[i])
        ++i;
    return (i);
}

static int calc_envp_size(t_shellenv *sh, int *count)
{
    t_env *env;
    int sz;
    int i;

    env = sh->envlist;
    sz = 0;
    i = 0;
    while (env)
    {
        /* '=' + '\0' + sizeof(char *) */
        sz += ft_strlen(env->name) + ft_strlen(env->val) + 2 + sizeof(char *);
        ++i;
        env = env->next;
    }
    *count = i;
    return (sz + sizeof(char *));
}

void fill_envp(t_env *env, int count, char **envp)
{
    char *s;
    char *s1;
    int i;
 
    s = (char *)envp + (count + 1) * sizeof(char *);
    i = 0;
    while (i < count)
    {
        envp[i++] = s;
        s1 = env->name;
        while (*s1)
            *s++ = *s1++;
        *s++ = '=';
        s1 = env->val;
        while (*s1)
            *s++ = *s1++;
        *s++ = '\0';
        env = env->next;
    }
    envp[i] = NULL;
}

char **build_envp(t_shellenv *sh)
{
    int count;
    int size;
    char **envp;
    int i;
    t_env *env;

    size = calc_envp_size(sh, &count);
    envp = malloc(size);
    if (!envp)
        return (NULL);
    fill_envp(sh->envlist, count, envp);
    return (envp);
}

void    skip_spaces(const char **head)
{
    const char  *ptr;

    ptr = *head;
    while (isspace(*ptr))
        ++ptr;
    *head = ptr;
}

int is_valid_text(int ch)
{
    if (isalpha(ch))
        return (1);
    if ((char)ch == '$' || (char)ch == '-' || (char)ch == '?')
        return (1);
    if ((char)ch == '"' || (char)ch == '\'')
        return (1);
    return (0);
}

int get_text_token(const char **head, char *buf, int *len)
{
    const char  *p;
    int         strmode;

    p = *head;
    strmode = 0;
    while (*p)
    {
        if (*p == '"' && strmode != 2)
            strmode = 1 - strmode;
        else if (*p == '\'' && strmode != 1)
            strmode = 2 - strmode;
        else if (!is_valid_text(*p) && !strmode)
            break ;
        if (buf)
            *buf++ = *p;
        ++p;
    }
    if (strmode)
    {
        printf("Unterminated string!\n");
        return -1;
    }
    if (len)
        *len = (int)(p - *head);
    return 1;
}

char    *substitute_vars(const char *token, char *buf, int *len)
{
    int strmode;
    int n;

    strmode = 0;
    n = 0;
    while (*token)
    {
        if (*token == '"' && strmode != 2)
            strmode = 1 - strmode;
        else if (*token == '\'' && strmode != 1)
            strmode = 2 - strmode;
        else
        {
            if (buf)
                buf[n] = *token;
            ++n;
        }
        ++token;
    }
    if (buf)
        buf[n] = '\0';
    if (len)
        *len = n + 1;
    return (buf);
}

int next_token(const char **head)
{
    const char *p;
    char *buf;
    int len;
    int isstr;

    skip_spaces(head);
    p = *head;
    if (*p == '\0')
        return (0);
    if (is_valid_text(*p))
    {
        if (get_text_token(&p, NULL, &len) < 0)
            return (-1);
        buf = malloc(len);
        get_text_token(head, buf, NULL);
        printf("Token: %.*s\n", len, buf);


        char *bab;
        int bob;

        substitute_vars(buf, NULL, &bob);
        bab = malloc(bob);
        substitute_vars(buf, bab, NULL);

        printf("Substitued token: %.*s\n", bob, bab);
        free(bab);

        free(buf);
        *head = p + len;
        return 1;
    }
    else if (*p == '>')
    {
        if (p[1] == '>')
        {
            if (p[2] == '>')
            {
                printf("Parse error\n");
                return -1;
            }
            printf("Token: APPEND\n");
            *head = p + 2;
            return 1;
        }
        printf("Token: REDIR\n");
        *head = p + 1;
        return 1;
    }
    else if (*p == '|')
    {
        if (p[1] == '|')
        {
            if (p[2] == '|')
            {
                printf("Parse error\n");
                return -1;
            }
            printf("Token: AND\n");
            *head = p + 2;
            return 1;
        }
        printf("Token: PIPE\n");
        *head = p + 1;
        return 1;
    }
    *head = p;
    return 0;
}

void parse_line(const char *line)
{
    while (next_token(&line) == 1)
        ;
}

t_env *shell_addenv(t_shellenv *sh, char *name, char *val)
{
    t_env *env;

    env = malloc(sizeof(*env));
    if (!env)
        return (NULL);
    env->name = name;
    env->val = val;
    env->next = sh->envlist;
    sh->envlist = env;
    return (env);
}

int main()
{
    char buf[1024];
    int n;

    t_shellenv sh;

    shell_addenv(&sh, "PATH", "test");
    shell_addenv(&sh, "HOME", "/Users/alavaud");

    char **envp = build_envp(&sh);

    int i;

    for (i = 0; envp[i]; ++i)
        printf("env %s\n", envp[i]);
    free(envp);

    builtin_env(0, NULL, &sh);

    do
    {
        printf("Shell> ");
        fflush(stdout);
        n = read(0, buf, sizeof(buf));
        if (n < 0)
        {
            perror("read");
            return 1;
        }

        buf[n] = '\0';
        parse_line(buf);
    } while (n != 0);

    return 0;
}