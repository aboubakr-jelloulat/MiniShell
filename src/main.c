/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajelloul <ajelloul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:48:10 by ajelloul          #+#    #+#             */
/*   Updated: 2025/06/29 00:26:55 by ajelloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	rl_catch_signals = 0;
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
