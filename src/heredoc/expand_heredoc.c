/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:13:35 by ajelloul          #+#    #+#             */
/*   Updated: 2025/06/25 11:04:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_exit_status(t_expand_info *info, t_env_var *var)
{
	var->str = ft_itoa(info->bash->exit_status);
	(*info->index)++;
}

static void	init_var(t_env_var *var)
{
	ft_memset(var, 0, sizeof(t_env_var));
}

/*
	$?
	$""
	prevents invalid expansion like : $' $1 $!
*/

char	*ft__expand(t_minibash *bash, t_env **env, char *str)
{
	t_expand_heredoc	ex;
	char				*result;

	if (!str || !*str)
		return (ft_strdup(""));
	if (ft_strcmp(str, "?") == 0)
		return (ft_itoa(bash->exit_status));
	if (search("$\"\"", str))
		return (ft_strdup(""));
	if (str[0] && !str[1] && (!ft_isalnum(str[0]) || ft_isdigit(str[0])
			|| str[0] == '"' || str[0] == '\''))
		return (ft_strdup(""));
	ft_memset(&ex, 0, sizeof(ex));
	lookup_env_var(env, str, &result, &ex.index);
	if (result)
		return (result);
	else
		return (ft_strdup(""));
}

/*

	loops : $MY_VAR

	This function handles variable names after '$' inside a heredoc line

	Loop to calculate the length of a valid variable name
*/

static void	handle_env_name(t_expand_info *info, t_env_var *var)
{
	var->j = *info->index;
	while (ft_isalnum(info->line[*info->index])
		|| info->line[*info->index] == '_')
	{
		(*info->index)++;
		var->len++;
	}
	if (var->len == 0)
		var->str = ft_strdup("$");
	else
	{
		var->sub = ft_substr(info->line, var->j, var->len);
		var->str = ft__expand(info->bash, &info->env, var->sub);
		free(var->sub);
	}
}

char	*expand_env_var_her(t_expand_info *info)
{
	t_env_var	var;
	char		*temp;
	char		*to_join;

	init_var(&var);
	(*info->index)++;
	if (info->line[*info->index] == '?')
		handle_exit_status(info, &var);
	else
		handle_env_name(info, &var);
	temp = *info->expanded_line;
	if (var.str)
		to_join = var.str;
	else
		to_join = "";
	*info->expanded_line = ft_strjoin(temp, to_join);
	free(temp);
	free(var.str);
	(*info->index)--;
	return (*info->expanded_line);
}
