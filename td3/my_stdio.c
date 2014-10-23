//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
MY_FILE *my_fopen(char *name, char *mode)
{
	MY_FILE new_file = malloc(sizeof(MY_FILE_s));
	new_file->mode = mode;
	new_file->filename = name;
	if ((fd = open(name, mode)) == -1)
	{
		printf("houga bitches!!\n");
		return NULL;
	}
	return new_file 
}
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
int my_fclose(MY_FILE *f)
{
	if (close(f->fd) = -1)
	{
		free(f);
		return -1;
	}
	else
	{
		free(f);
		return 0;
	}
}
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
int my_fread(void *p, size_t taille, size_t nbelem, MY_FILE *f)
{
	int result=0 , ret =0;
	for(int i=0 ; i<nbelem ; i++)
	{
		if (result = (int)read(f->fd, p, taille) == -1)
			return -1;
		else
		{
			p = (char *)p + result;
			ret += result;
		}
	}
	return ret;
}
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
int my_fwrite(void *p, size_t taille, size_t nbelem, MY_FILE *f)
{
	int result=0 , ret =0;
	for(int i=0 ; i<nbelem ; i++)
	{
		if (result = (int)write(f->fd, p, taille) == -1)
			return -1;
		else
		{
			p = (char *)p + result;
			ret += result;
		}
	}
	return ret;
}
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
int my_feof(MY_FILE *f)
{
	if (read(f) == EOF)
		return 1;
	else
		return 0;
}
//-------------------------------------------------------------------