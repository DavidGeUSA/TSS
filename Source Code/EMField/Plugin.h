#ifndef __PLUGIN_H__
#define __PLUGIN_H__
/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/
//typedef void (*fnRemovePlugin)(char *name);

class Plugin
{
private:
	char *_className;
public:
	Plugin(void);
	~Plugin(void);
	char *getClassName();
	void setClassName(const char *name);
};

//code to create instance
#define CREATEPLUGININSTANCE(type, count, list)               \
		type *p0 = new type();                                \
		type **pp = (type**)malloc((count+1)*sizeof(type*));  \
		if(list != NULL)                                      \
		{                                                     \
			for(unsigned int i=0;i<count;i++)                 \
			{                                                 \
				pp[i] = list[i];                              \
			}                                                 \
			free(list);                                       \
		}                                                     \
		list = pp;                                            \
		list[count] = p0;                                     \
		count++;                                              \
		p = p0


//code to remove all instances
#define REMOVEALLPLUGINS(type, count, list) \
	if(list != NULL)                        \
	{                                       \
		for(unsigned int i=0;i<(count);i++) \
		{                                   \
			type *p = list[i];              \
			if(p != NULL)                   \
			{                               \
				delete p;                   \
				list[i] = NULL;             \
			}                               \
		}                                   \
		free(list);                         \
		count = 0;                          \
		list = NULL;                        \
	}


#endif
