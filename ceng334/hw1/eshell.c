#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "parser.h"
#include "parser.c"

void execute_command(single_input input);
void execute_pipeline(parsed_input parsed);
void execute_sequential(parsed_input parsed);
void execute_parallel(parsed_input parsed);
void execute_subshell(single_input input);
void execute_pipeline_input(single_input input);
void execute_parallel_with_repeater(parsed_input parsed);
void execute_command(single_input input)
{
    command cmd = input.data.cmd;
    execvp(cmd.args[0], cmd.args);
}
void execute_pipeline(parsed_input parsed)
{
    int num_pipes = parsed.num_inputs - 1;
    int pipefds[num_pipes][2];
    for (int i = 0; i < num_pipes; i++)
    {
        pipe(pipefds[i]);
    }
    for (int i = 0; i < parsed.num_inputs; i++)
    {
        single_input input = parsed.inputs[i];
        if (fork() == 0)
        {
            if (i != 0)
            {
                dup2(pipefds[i - 1][0], 0);
            }
            if (i != num_pipes)
            {
                dup2(pipefds[i][1], 1);
            }
            for (int j = 0; j < num_pipes; j++)
            {
                close(pipefds[j][0]);
                close(pipefds[j][1]);
            }
            if (input.type == INPUT_TYPE_COMMAND)
            {
                execute_command(input);
            }
            else if (input.type == INPUT_TYPE_SUBSHELL)
            {
                execute_subshell(input);
            }
            exit(0);
        }
    }
    for (int i = 0; i < num_pipes; i++)
    {
        close(pipefds[i][0]);
        close(pipefds[i][1]);
    }
    for (int i = 0; i < parsed.num_inputs; i++)
    {
        wait(NULL);
    }
    return;
}
void execute_pipeline_input(single_input input)
{
    int num_pipes = input.data.pline.num_commands - 1;
    int pipefds[num_pipes][2];
    int num_commands = input.data.pline.num_commands;
    for (int i = 0; i < num_pipes; i++)
    {
        pipe(pipefds[i]);
    }
    for (int i = 0; i < num_commands; i++)
    {
        command cmd = input.data.pline.commands[i];
        if (fork() == 0)
        {
            if (i != 0)
            {
                dup2(pipefds[i - 1][0], 0);
            }
            if (i != num_pipes)
            {
                dup2(pipefds[i][1], 1);
            }
            for (int j = 0; j < num_pipes; j++)
            {
                close(pipefds[j][0]);
                close(pipefds[j][1]);
            }
            execvp(cmd.args[0], cmd.args);
            exit(1);
        }
    }
    for (int i = 0; i < num_pipes; i++)
    {
        close(pipefds[i][0]);
        close(pipefds[i][1]);
    }
    for (int i = 0; i < num_commands; i++)
    {
        wait(NULL);
    }
    return;
}
void execute_sequential(parsed_input parsed)
{
    for (int i = 0; i < parsed.num_inputs; i++)
    {
        single_input input = parsed.inputs[i];
        if (fork() == 0)
        {
            if (input.type == INPUT_TYPE_COMMAND)
            {
                execute_command(input);
            }
            else if (input.type == INPUT_TYPE_PIPELINE)
            {
                execute_pipeline_input(input);
            }
            exit(0);
        }
        else
        {
            wait(NULL);
        }
    }
}
void execute_parallel(parsed_input parsed)
{
    for (int i = 0; i < parsed.num_inputs; i++)
    {
        single_input input = parsed.inputs[i];
        if (fork() == 0)
        {
            if (input.type == INPUT_TYPE_COMMAND)
            {
                execute_command(input);
            }
            else if (input.type == INPUT_TYPE_PIPELINE)
            {
                execute_pipeline_input(input);
            }
            exit(0);
        }
    }
    for (int i = 0; i < parsed.num_inputs; i++)
    {
        wait(NULL);
    }
    return;
}
void execute_parallel_with_repeater(parsed_input parsed)
{
    int num_inputs = parsed.num_inputs;
    int pipefds[num_inputs][2];
    for (int i = 0; i < num_inputs; i++)
    {
        pipe(pipefds[i]);
    }
    for (int i = 0; i < num_inputs; i++)
    {
        single_input input = parsed.inputs[i];
        if (fork() == 0)
        {
            dup2(pipefds[i][0], 0);
            for (int j = 0; j < num_inputs; j++)
            {
                close(pipefds[j][0]);
                close(pipefds[j][1]);
            }
            if (input.type == INPUT_TYPE_COMMAND)
            {
                execute_command(input);
            }
            else if (input.type == INPUT_TYPE_PIPELINE)
            {
                execute_pipeline_input(input);
            }
            exit(0);
        }
    }
    if (fork() == 0)
    {
        char c;
        while (read(0, &c, 1) > 0)
        {
            for (int i = 0; i < num_inputs; i++)
            {
                write(pipefds[i][1], &c, 1);
            }
        }
        for (int i = 0; i < num_inputs; i++)
        {
            close(pipefds[i][0]);
            close(pipefds[i][1]);
        }
        exit(0);
    }
    else
    {
        for (int i = 0; i < num_inputs; i++)
        {
            close(pipefds[i][0]);
            close(pipefds[i][1]);
        }
        for (int j = 0; j <= num_inputs; j++)
        {
            wait(NULL);
        }
    }
}
void execute_subshell(single_input input)
{
    parsed_input *parsed = (parsed_input *)malloc(sizeof(parsed_input));
    if (parse_line(input.data.subshell, parsed))
    {
        if (parsed->separator == SEPARATOR_NONE)
        {
            single_input input = parsed->inputs[0];
            if (input.type == INPUT_TYPE_COMMAND)
            {
                execute_command(input);
            }
            else if (input.type == INPUT_TYPE_SUBSHELL)
            {
                execute_subshell(input);
            }
        }
        else if (parsed->separator == SEPARATOR_PIPE)
        {
            execute_pipeline(*parsed);
        }
        else if (parsed->separator == SEPARATOR_SEQ)
        {
            execute_sequential(*parsed);
        }
        else if (parsed->separator == SEPARATOR_PARA)
        {
            execute_parallel_with_repeater(*parsed);
        }
        free_parsed_input(parsed);
    }
    else
    {
        printf("Parsing failed\n");
    }
}
int main()
{
    while (1)
    {
        printf("/> ");
        fflush(stdout);
        const char *exit_command = "quit";
        char inp[INPUT_BUFFER_SIZE];
        fgets(inp, INPUT_BUFFER_SIZE, stdin);

        if (strncmp(inp, exit_command, 4) == 0)
        {
            break;
        }
        else
        {
            parsed_input *parsed = (parsed_input *)malloc(sizeof(parsed_input));
            if (parse_line(inp, parsed))
            {
                if (parsed->separator == SEPARATOR_NONE)
                {
                    single_input input = parsed->inputs[0];
                    if (input.type == INPUT_TYPE_COMMAND)
                    {
                        execute_command(input);
                    }
                    else if (input.type == INPUT_TYPE_SUBSHELL)
                    {
                        execute_subshell(input);
                    }
                }
                else if (parsed->separator == SEPARATOR_PIPE)
                {
                    execute_pipeline(*parsed);
                }
                else if (parsed->separator == SEPARATOR_SEQ)
                {
                    execute_sequential(*parsed);
                }
                else if (parsed->separator == SEPARATOR_PARA)
                {
                    execute_parallel(*parsed);
                }
                free_parsed_input(parsed);
            }
            else
            {
                printf("Parsing failed\n");
            }
        }
    }

    return 0;
}