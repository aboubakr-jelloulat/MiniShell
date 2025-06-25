/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enums.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajelloul <ajelloul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 09:56:20 by ajelloul          #+#    #+#             */
/*   Updated: 2025/06/21 22:20:12 by ajelloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUMS_H
# define ENUMS_H

typedef enum e_env_type
{
	ENV_NAME,
	ENV_VALUE,
	ENV_OLDPWD,
	ENV_PWD,
	ENV_PATH,
	ENV_EXEC,
	CMD_SIMPLE,
	CMD_COMPLEX,
	REDIRECT,
	HEREDOC,
	ALLOC_TYPE,
	ERROR_TYPE,
	CLEANUP,
	BUILTIN,
	CHILD_PROC,
	PARENT_PROC
}	t_env_type;

typedef enum e_state
{
	N,
	S,
	D,
	NUL,
}	t_state;

typedef enum e_token_type 
{
	TOKEN_WORD = -1,
	TOKEN_SPACE = ' ',
	TOKEN_PIPE = '|',
	TOKEN_ENV = '$',
	TOKEN_REDIR_IN = '<',
	TOKEN_REDIR_OUT = '>',
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_NEWLINE = '\n',
	TOKEN_EOF,
	TOKEN_WHITE_SPACE = 32,
	TOKEN_SINGLE_QUOTE = '\'',
	TOKEN_DOUBLE_QUOTE = '\"'
}	t_token_type;

#endif
