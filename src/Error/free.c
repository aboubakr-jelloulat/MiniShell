/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achoukri <achoukri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 22:36:56 by achoukri          #+#    #+#             */
/*   Updated: 2025/06/20 22:37:04 by achoukri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	display_syntax_error(t_minibash *bash)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putendl_fd("syntax error near unexpected token `newline'", 2);
	bash->exit_status = 1;
	exit(1);
}

void	free_lexer(t_token **token)
{
	t_token	*cur;

	while (*token)
	{
		cur = *token;
		*token = (*token)->next;
		if (cur->value)
		{
			free(cur->value);
			cur->value = NULL;
		}
		free(cur);
		cur = NULL;
	}
}

void	free_2d(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_minibash(t_minibash **bash)
{
	t_env	*current;
	t_env	*next;

	if (!bash || !*bash)
		return ;
	current = (*bash)->env;
	while (current)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
	free(*bash);
	*bash = NULL;
}
