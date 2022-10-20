#include "server.hpp"

char	*ft_strjoin(char *s, char c)
{
	char	*str;
	int		i;

	i = 0;
	while (s[i])
		i++;
	str = (char *) malloc(i + 2);
	if (!str)
		return (NULL);
	i = 0;
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = c;
	str[i + 1] = '\0';
	free(s);
	return (str);
}


char	*get_file(int fd){
	char	buffer;
	char	*str_line;
	int		i;
	

	if (fd < 0)
		return (NULL);
	i = 0;
	str_line = strdup("");
	while (read(fd, &buffer, 1) > 0){
		str_line = ft_strjoin(str_line, buffer);
	}
	i = strlen(str_line);
	if (i == 0){
		free(str_line);
		str_line = NULL;
	}
	return (str_line);
}