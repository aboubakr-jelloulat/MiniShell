/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajelloul <ajelloul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:40:45 by achoukri          #+#    #+#             */
/*   Updated: 2025/06/24 17:15:43 by ajelloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
	1 - prints a newline
	2 - tells readline We're on a new line now
	3 - This clears the current input line
		and Replace input buffer with an empty string
		0 means don’t keep the old content
	4 - This reprints the shell prompt and displays the cleared line
*/

void	sigint_handler(int signum)
{
	if (signum != SIGINT)
		return ;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	execute_command_pipeline(t_minibash *bash, t_env **env,
	t_token *token, t_cmd **cmd)
{
	if (token && env)
	{
		parse_input_commands(&token, cmd, bash);
		execution(bash, env, *cmd);
	}
	free_cmd_list(cmd);
	free_lexer(&token);
}

/*
	if (!isatty(STDIN_FILENO))
    	rl_outstream = stdin;
*/

void	ft_readline(t_minibash	*bash, t_token *tokens, t_cmd *cmd, t_env **env)
{
	char	*line;

	while (true)
	{
		line = readline("minishell $ ");
		if (!line)
			break ;
		if (line && *line)
			add_history(line);
		if (lexer(line, &tokens))
		{
			ft_putendl_fd("minishell: syntax error", 2);
			bash->exit_status = 258;
			free_lexer(&tokens);
		}
		else
			execute_command_pipeline(bash, env, tokens, &cmd);
		tokens = NULL;
		cmd = NULL;
		free (line);
	}
	rl_clear_history();
	write(1, "exit\n", 5);
}

static void	init_minibash(t_minibash **bash)
{
	*bash = (t_minibash *)malloc(sizeof(t_minibash));
	if (!*bash)
	{
		ft_putstr_fd("minishell: fatal error: cannot allocate memory\n", 2);
		exit(1);
	}
	ft_memset(*bash, 0, sizeof(t_minibash));
	(*bash)->exit_status = 0;
	(*bash)->env = NULL;
}
/*
	SIGQUIT is sent when you press Ctrl + \
*/

int	main(int ac, char **av, char **env)
{
	t_minibash	*bash;
	t_token		*tokens;
	t_cmd		*cmd;
	int			exit_st;

	(void)ac;
	(void)av;
	tokens = NULL;
	cmd = NULL;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	init_minibash(&bash);
	initialize_environment(bash, env);
	using_history();
	ft_readline(bash, tokens, cmd, &bash->env);
	exit_st = bash->exit_status;
	free_minibash(&bash);
	return (exit_st);
}
