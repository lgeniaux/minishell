#include "minishell.h"
#include <readline/readline.h>

typedef struct s_toklist
{
    t_token tok;
    struct s_toklist *next;
} t_toklist;

t_toklist *get_tokens(const char *line)
{
    t_toklist *head;
    t_toklist *last;
    t_toklist *tmp;
    t_token tok;
    int n;

    head = NULL;
    while (1)
    {
        n = next_token(&line, &tok);
        if (!n)
            break ;
        tmp = malloc(sizeof(*tmp));
        if (!tmp)
        {
            // TODO free all
            return NULL;
        }
        tmp->tok = tok;
        if (!head)
            head = tmp;
        else
            last->next = tmp;
        last = tmp;
    }
    return head;
}

// void parse_line(const char *line)
// {
//     t_toklist *list;

//     list = get_tokens(line);

//     while (list)
//     {
//         printf("%.*s\n", list->tok.len, list->tok.str);
//         list = list->next;
//     }
// }

typedef struct s_strarray
{
    int n;
    char **strs;
} t_strarray;

void    strarray_init(t_strarray *arr)
{
    arr->strs = NULL;
    arr->n = 0;
}

void *strarray_append(t_strarray *array, char *str)
{
    char    **strings;
    int     i;

    strings = malloc(sizeof(char *) * (array->n + 1));
    if (!strings)
        return (NULL);
    i = 0;
    while (i < array->n)
    {
        strings[i] = array->strs[i];
        ++i;
    }
    strings[i] = str;
    ++array->n;
    free(array->strs);
    array->strs = strings;
    return (array->strs);
}

char *strndup(const char *, size_t);

int parse_redir(struct command *cmd, int type, const char **head)
{
    t_token     tok;
    const char  *ptr;
    int         n;

    ptr = *head;
    n = next_token(&ptr, &tok);
    if (n <= 0)
        return (n);
    if (tok.type != TOKEN_TEXT)
    {
        printf("Parse error\n");
        return (-1);
    }
    /* TODO append redir here */
    printf("Redir %d to %.*s\n", type, tok.len, tok.str);
    *head = ptr;

    struct output_redir r;
    r.is_append = 0;
    r.next = NULL;
    return (1);
}

int parse_command(struct command *cmd, const char **head)
{
    t_token     tok;
    int         n;
    const char  *ptr;
    t_strarray  args;

    strarray_init(&args);
    ptr = *head;
    while (1)
    {
        n = next_token(&ptr, &tok);
        if (n <= 0 || tok.type == TOKEN_PIPE)
            break ;
        if (tok.type == TOKEN_TEXT)
        {
            strarray_append(&args, strndup(tok.str, tok.len));
        }
        else if (tok.type == TOKEN_REDIR_IN || tok.type == TOKEN_REDIR_OUT
                || tok.type == TOKEN_APPEND || tok.type == TOKEN_HEREDOC)
        {
            n = parse_redir(cmd, tok.type, &ptr);
            if (n <= 0)
                break ;
        }
        *head = ptr;
    }
    cmd->argc = args.n;
    cmd->argv = args.strs;

    for (int i = 0; i < cmd->argc; ++i)
    {
        printf("arg %d = %s\n", i, cmd->argv[i]);
    }
    return (n);
}

void parse_line(const char *line)
{
    struct command cmd;
    t_token tok;
    int n;

    while (1)
    {
        n = parse_command(&cmd, &line);
        if (n <= 0)
            break ;

        n = next_token(&line, &tok);
        if (n <= 0 || tok.type != TOKEN_PIPE)
            break ;

        printf("PIPE\n");
    }
    printf("Final return value is %d\n", n);
}

int main(int argc, char *argv[])
{
    char    *line;

    while (1)
    {
        line = readline("Shell> ");
        if (!line)
            break ;
        if (line[0])
        {
            printf("Line: %s\n", line);
            parse_line(line);
            add_history(line);
        }
        free(line);
    }

    return 0;
}
