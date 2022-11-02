/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ctype.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:02:47 by alavaud           #+#    #+#             */
/*   Updated: 2022/10/23 04:55:36 by alavaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isspace(int ch)
{
	return ((char)ch == ' ' || (char)ch == '\t' || (char)ch == '\n'
		|| (char)ch == '\r' || (char)ch == '\v' || (char)ch == '\f');
}

int	ft_islower(int ch)
{
	return (ch >= 'a' && ch <= 'z');
}

int	ft_isupper(int ch)
{
	return (ch >= 'A' && ch <= 'Z');
}

int	ft_isalpha(int ch)
{
	return (ft_islower(ch) || ft_isupper(ch));
}

int	ft_isprint(int ch)
{
	return (ch >= 32 && ch < 127);
}
