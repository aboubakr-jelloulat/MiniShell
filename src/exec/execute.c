/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:29:21 by ajelloul          #+#    #+#             */
/*   Updated: 2025/06/25 12:32:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	delete_heredoc_files(t_minibash *bash, t_cmd *cmd)
{
	t_heredoc_cleanup	cleanup;

	cleanup.current_cmd = cmd;
	while (cleanup.current_cmd)
	{
		cleanup.current_heredoc = cleanup.current_cmd->heredoc;
		while (cleanup.current_heredoc)
		{
			cleanup.index_str = ft_itoa(cleanup.current_heredoc->index);
			cleanup.temp_path = ft_strjoin_with_null(
					cleanup.current_heredoc->delimiter, cleanup.index_str);
			cleanup.full_filepath = ft_strjoin(
					"/tmp/miniheredoc", cleanup.temp_path);
			if (unlink(cleanup.full_filepath) != 0)
			{
				bash->exit_status = 1;
				perror("Failed to delete heredoc file");
			}
			free(cleanup.index_str);
			free(cleanup.temp_path);
			free(cleanup.full_filepath);
			cleanup.current_heredoc = cleanup.current_heredoc->next;
		}
		cleanup.current_cmd = cleanup.current_cmd->next;
	}
}

/* Case 1: Handle heredoc operations
	1.1
		echo hej << 42  :  print hej
		cd - << 1337  ; file heredoc and go to the old pwd

	Case 3 : line to long >  has_redirections(cmd)
		cd > file.txt -> change cur dir + create file
*/

/*
	echo hej  << 13 > infile
*/

int	process_herdoc_builtins(t_minibash *bash, t_env **env, t_cmd *cmd)
{
	t_cmd	*tmp_cmd;

	tmp_cmd = cmd;
	if (!cmd)
		return (0);
	if (has_herdoc(tmp_cmd))
	{
		if (handle_heredocs(bash, env, tmp_cmd))
			return (-1);
		if (is_builtins(tmp_cmd) && !has_redirections(tmp_cmd)
			&& !has_pipes(tmp_cmd))
		{
			execute_builtins(bash, env, tmp_cmd);
			return (1);
		}
	}
	if (is_builtins(tmp_cmd) && !has_redirections(tmp_cmd)
		&& !has_pipes(tmp_cmd) && !has_herdoc(tmp_cmd))
	{
		execute_builtins(bash, env, tmp_cmd);
		return (1);
	}
	else if (is_parent_builtins(tmp_cmd) && !has_pipes(tmp_cmd))
		execute_parent_builtin(bash, env, tmp_cmd);
	return (0);
}

/* Case 1: SIGINT (Ctrl+C) during heredoc */
/* Case 2: Builtin executed successfully */
/* Case 3: Continue to pipe/fork execution */

int	open_her_execute_builtins(t_minibash *bash, t_cmd *cmd)
{
	int		her_status;
	t_env	**env;

	env = &bash->env;
	her_status = process_herdoc_builtins(bash, env, cmd);
	if (her_status == -1)
	{
		delete_heredoc_files(bash, cmd);
		bash->exit_status = 1;
		return (1);
	}
	if (her_status == 1)
	{
		delete_heredoc_files(bash, cmd);
		return (1);
	}
	return (0);
}

/*
	status is an int set by waitpid()
	contains encoded info about how the child process ended

	WIFSIGNALED(status) : Was the child killed by a signal

	WTERMSIG(status) :gives the signal number that terminated the child
*/

static void	wait_for_child(t_minibash *bash, int pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		bash->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		bash->exit_status = 128 + WTERMSIG(status);
}

/*
	Tells the parent shell to ignore Ctrl+C (SIGINT) during the child exec



	Allow Ctrl+C to kill the child process
		Use the default action for this signal

	Restore the shell’s custom Ctrl+C handler after the command is done
*/

void	execution(t_minibash *bash, t_env **env, t_cmd *cmd)
{
	t_cmd	*tmp_cmd;
	int		pid;

	tmp_cmd = cmd;
	if (open_her_execute_builtins(bash, tmp_cmd))
		return ;
	pid = fork();
	if (!is_fork_succes(bash, pid))
		return ;
	signal(SIGINT, SIG_IGN);
	if (!pid)
	{
		signal(SIGQUIT, handle_sigquit);
		signal(SIGINT, SIG_DFL);
		execute_command(bash, env, tmp_cmd);
	}
	else
	{
		wait_for_child(bash, pid);
		signal(SIGINT, sigint_handler);
	}
	delete_heredoc_files(bash, cmd);
}
