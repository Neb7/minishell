/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capi <capi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:35:46 by llemmel           #+#    #+#             */
/*   Updated: 2025/03/03 20:39:18 by capi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief	Create a command path from a directory and a command name
 * 			This function join the path and the command name with a '/'
 * 
 * @param	path The path of the directory
 * @param	cmd_name The command name
 * @param	error A pointer to the error flag
 * @return	char* The command path or NULL if an error occurs
 */
static char	*create_path(const char *path, const char *cmd_name, bool *error)
{
	char	*cmd_path;
	char	*tmp;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (perror("minishell"), NULL);
	cmd_path = ft_strjoin(tmp, cmd_name);
	if (!cmd_path)
	{
		free(tmp);
		*error = true;
		return (perror("minishell"), NULL);
	}
	free(tmp);
	return (cmd_path);
}

/**
 * @brief	Search a command in a directory
 * 
 * @param	path The path of the directory
 * @param	dir The directory
 * @param	cmd_name The command name
 * @param	error A pointer to the error flag
 * @return	char* The command path or NULL if the command was not found
 */
static char	*search_in_dir(const char *path, DIR *dir, \
	const char *cmd_name, bool *error)
{
	size_t			cmd_name_len;
	struct dirent	*dirent;

	cmd_name_len = ft_strlen(cmd_name);
	dirent = readdir(dir);
	if (!dirent)
		return (NULL);
	while (dirent)
	{
		if (ft_strncmp(dirent->d_name, cmd_name, cmd_name_len + 1) == 0)
			return (create_path(path, cmd_name, error));
		dirent = readdir(dir);
	}
	return (NULL);
}

/**
 * @brief	Get an array of path from the PATH environment variable
 * 
 * @param	shell The Shell structure
 * @return	char** The array of path or NULL if an error occurs
 */
static char	**get_path_var(t_shell shell)
{
	char	*env_path;
	char	**path;

	env_path = get_envvar_value(shell, "PATH");
	if (!*env_path && shell.save_path)
		env_path = DEFAULT_PATH;
	path = ft_split(env_path, ':');
	if (!path)
		perror("minishell");
	if (!path || !path[0])
		return (free(path), NULL);
	return (path);
}

/**
 * @brief	Add the default path to the command path, 
 * 			if the command was not found
 * 
 * @param	shell The Shell structure 
 * @param	cmd_name The command name
 * @return	char* The command path or NULL if an error occurs
 */
char	*add_default_invalid_path(t_shell shell, char *cmd_name)
{
	char	*cmd_path;
	char	**path;

	cmd_path = NULL;
	path = get_path_var(shell);
	if (!path)
		return (NULL);
	cmd_path = ft_strjoin(path[0], "/");
	if (!cmd_path)
		return (perror("minishell"), free_tab(path), NULL);
	cmd_path = ft_strjoin_free(cmd_path, cmd_name);
	free_tab(path);
	if (!cmd_path)
		return (perror("minishell"), NULL);
	return (cmd_path);
}

/*
	return the path of the command if it exists in the PATH
	return NULL if the command does not exist or if an error occured
	The return path must be freed by the caller

	Example:
		path = search_command("ls");
		result : path = "/bin/ls";
*/
char	*search_command(t_shell shell, const char *cmd_name, bool *error)
{
	char	*cmd_path;
	char	**path;
	ssize_t	i;
	DIR		*dir;

	i = -1;
	cmd_path = NULL;
	path = get_path_var(shell);
	if (!path)
		return (NULL);
	while (path[++i])
	{
		dir = opendir(path[i]);
		if (!dir)
		{
			closedir(dir);
			continue ;
		}
		cmd_path = search_in_dir(path[i], dir, cmd_name, error);
		closedir(dir);
		if (cmd_path)
			break ;
	}
	free_tab(path);
	return (cmd_path);
}
