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

char    *shell_get_var(t_shellenv *sh, const char *name)
{
    (void)sh;
    (void)name;
    return "Alexis";
}

int decode_var(const char *token, char *buf)
{
    int i;

    i = 0;
    while (token[i])
    {
        /* TODO better cond */
        if (isdigit(token[i]) && i == 0)
            break ;
        if (!isalpha(token[i]) && !isdigit(token[i]) && token[i] != '_')
            break ;
        if (buf)
            *buf++ = token[i];
        ++i;
    }
    if (buf)
        *buf = '\0';
    return (i);
}

int substitude_var(const char *token, char *buf)
{
    return 0;
}

int substitute_vars(const char *token, char *buf)
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
        else if (*token == '$' && strmode != 2)
        {
        }
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
    return (n + 1);
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