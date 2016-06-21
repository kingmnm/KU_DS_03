#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct HashMap* find_key(struct HashMap* map, int key);
struct User{
	char id[10];
	struct User_Profile* profile;
	struct friend* friend;
	struct User* next;
	struct Tweet* tweet;
};


struct HashMap{
	int key;
	struct HashMap* next;
	struct User* user;
};


struct friend{
	char friend_id[10];
	struct friend* next;
};

void friend_init(struct friend* self)
{
	self->next = NULL;
}

void add_friend(struct HashMap* map, int hash_key, char my_id[] , char friend_id[])
{
	struct HashMap* p = find_key(map,hash_key);
	struct User* user;
	
	struct friend * friend = (struct friend*)malloc(sizeof(struct friend));
	friend_init(friend);
	sscanf(friend_id, "%s", friend->friend_id);
	
	for( user = p->user ; user != NULL ; user = user->next)
	{
		if( strcmp(user->id,my_id) == 0 )
		{
			if(user->friend == NULL)
			{
				user->friend = friend;
			}
			else
			{
				friend->next = user->friend;
				user->friend = friend;
			}
		}
	}


}



struct User_Profile{
	char sign_up_date[50];
	char screen_name[20];
};

struct User* alloc_user(){

	struct User* user = (struct User*)malloc(sizeof(struct User));
	struct User_Profile* user_profile = (struct User_Profile*)malloc(sizeof(struct User_Profile));
	user->profile = user_profile;
	user->next = NULL;
	user->friend = NULL;
	user->tweet = NULL;
	return user;
}

struct User* create_user( char id[], char sign_up_date[], char screen_name[] ){

	struct User* user = alloc_user();
	
	sscanf(id, "%s", user->id);
	sscanf(screen_name,"%s",user->profile->screen_name);
	strcpy(user->profile->sign_up_date,sign_up_date);
	
	return user;
}





struct HashMap* alloc_HashMap()
{
	struct HashMap* hashmap = (struct HashMap*)malloc(sizeof(struct HashMap));
	hashmap->next = NULL;
	hashmap->user = NULL;
	return hashmap;
}

struct HashMap* insert_HashMap(struct HashMap* map, int key)
{
	struct HashMap* hashmap = alloc_HashMap();
	hashmap->key = key;
	hashmap->next = map;
	return hashmap;
}

int key_of_HashMap(char id[])
{
	int id_for_hash;
	int i;
	for(i=0;i<strlen(id);i++)
	{
		if(i%2==0)
		{
			id_for_hash  = id_for_hash + (id[i]-48)*10;
		}
		else
		{
			id_for_hash  = id_for_hash + id[i]-48;
		}
	}
	return id_for_hash%100;
}

struct HashMap* find_key(struct HashMap* map, int key)
{
	if(key>99)
	{
		printf("범위를 넘어섰습니다\n");
		exit(1);
	}
	
	struct HashMap* p = map;
	for( ; p != NULL ; p=p->next)
	{
		if( p->key == key )
			return p;
	}	
}

void add_user_in_HashMap(struct HashMap* key_place,struct User* user)
{
	if( key_place->user == NULL)
	{
		key_place->user = user;
	}
	else
	{
		user->next = key_place->user;
		key_place->user = user;
	}
}




void print_user(struct HashMap* hashmap) 
{	
	struct HashMap *p = hashmap;
	struct friend* friend;
	for( ; p != NULL; p= p->next)
	{
		printf("Hash Key = %d \n\n",p->key);
		struct User* q = p->user;
		for( ; q!= NULL; q=q->next)
		{
			printf("%s\n",q->id);	
			printf("%s",q->profile->sign_up_date);
			printf("%s\n\n",q->profile->screen_name);	
			printf("friend list\n");
			
			if(q->friend != NULL)
			{
				for(friend = q->friend ; friend != NULL ; friend = friend->next)
				{
					printf("%s\n",friend->friend_id);
				}
			}
			printf("\n\n");
		}
		printf("==================================\n");
	}
}





void main()
{


	int i;
	struct HashMap* hashmap= NULL;
	for(i=0;i<100;i++) 
	{
		hashmap = insert_HashMap(hashmap,i);
	}


	FILE *fa;

	char tstr[1000];
	char screen_name[50];
	char sign_up_date[50];
	char id[10];
	int id_for_hash;

	fa = fopen("user.txt","r");
	
	int num=0;
	

	while( fgets(tstr,50,fa) != NULL )
	{
		
		if(num%4 == 0)
		{
			id_for_hash = 0;
			sscanf(tstr,"%s",id);
			id_for_hash = key_of_HashMap(id);
		}
		else if(num%4 == 1)
		{
			strcpy(sign_up_date,tstr);
		}
			
		else if(num%4 == 2)
		{
			
			sscanf(tstr,"%s",screen_name);
			struct HashMap* p = find_key(HashMap,id_for_hash);
			
			struct User* user = create_user(id,sign_up_date,screen_name);

			add_user_in_HashMap(p,user);
			
		} 
		
		
		num = num + 1;
			
		
	}
	fclose(fa);


	
	fa = fopen("friend.txt","r");
	
	num = 0;
	char my_id[10];
	char friend_id[10];
	int hash_key_my_id;

	while( fgets(tstr,10,fa) != NULL )
	{
		if(num%3 == 0)
		{	
			hash_key_my_id = 0;
			sscanf(tstr,"%s",my_id);
			hash_key_my_id = key_of_HashMap(my_id);
		}
		else if(num%3 == 1)
		{
			sscanf(tstr,"%s",friend_id);
			add_friend(hashmap,hash_key_my_id,my_id,friend_id);
		}
		num = num + 1;
	
	}
	
	fclose(fa);


	print_user(hashmap);
}