#include "GM.h"
#define LOGIN 0
#define MESSAGE 1
#define LOGOUT 2
#define N 1

/*Recebe uma Mensagem e um codigo com a ação a ser executada, retorna um vetor de Mensagem com notificaçao de entrada/saida do grupo,N mensagens do grupo caso LOGIN, e caso MESSAGE retorna a propria mensagem
Gera um arquivo de texto com os nomes do grupos criados separados por espaco; Caso seja requisitado LOGIN mas o usuario ja ultrapassou o limite, retorna invalid_argument*/

using namespace std;

typedef struct user_l_type{
	string name;
	int clients;
	struct user_l_type *next;
}user_l;

typedef struct group_l_type{
	string name;
	user_l *members;
	struct group_l_type *next;
}group_l;

class group_man{
	group_l *groups;
	user_l *users;
	
	private:
	group_l* exist_grp(string group);							//verifica se nome de grupo ja existe, caso sim retornar ponteiro, caso nao nullptr
	void insert_grp(string group,string user);					//adiciona nova estrutura de grupo para um lista encadeada
	int insert_user(string group,string user);					//adiciona novo usuario a estrutura de grupo
	void remove_user(string group,string user);					// remove usuario da estrutura de grupo
	user_l* user_clients(string user);							//retorna e cria user sem incrementar clients
	
	public:
	vector<Mensagem> handle(Mensagem command,int type);			//Recebe uma Mensagem e um codigo definido acima para LOGIN,LOGOUT e MESSAGE
	group_man();
};

group_man::group_man(){
	string group_name;
	group_l *grp_point;
	int n = 0;
	users = nullptr;
	
	ifstream group_fin("groups.txt");
	if(group_fin.peek() == std::ifstream::traits_type::eof()){
		groups = nullptr;
	}
	else{
		
		while(group_fin >> group_name)
		{
			n++;
		}
		
		group_fin.clear();
		group_fin.seekg (0, ios::beg);
		
		group_l *new_group [n];
		group_l *pointer;
		
		for(int i = 0; i < n; i++){
			new_group[i] = new group_l;
			group_fin >> new_group[i]->name;
			new_group[i]->members = nullptr;
		}
		for(int i = 0; i < (n - 1); i++){
			new_group[i]->next = new_group[i+1];
		}
		new_group[n-1]->next = nullptr;
		groups = new_group[0];
	}
		group_fin.close();
		return;

}

group_l* group_man::exist_grp(string group){
	if (groups == nullptr){
	return nullptr;
	}
	else{
		
		if(groups->name == group)
			return groups;

		group_l *point;
		point = groups->next;
		
		while(point != nullptr){
			if(point->name == group)
			return point;
			point = point->next;
		}
	}
		return nullptr;
			
}

int group_man::insert_user(string group,string user){
	group_l *gpoint;
	
	gpoint = exist_grp(group);
	if (gpoint == nullptr)
		return 0;
		
	user_l *new_user = new user_l;
	new_user->name = user;
	new_user->next = nullptr;
	
	if (gpoint->members == nullptr){
		gpoint->members = new_user;
		return 1;
	}
	
	user_l *upoint;
	upoint = gpoint->members;
		
	while(upoint->next != nullptr){
		upoint = upoint->next;
	}
	upoint->next = new_user;
	
	return 1; 	
	
}


void group_man:: remove_user(string group,string user){
	group_l *gpointer;
	user_l *upointer;
	user_l *upointer2;
	
	gpointer = exist_grp(group);
	
	upointer = gpointer->members;
	
	if(upointer->name == user){
		gpointer->members = upointer->next;
		free(upointer);
		return;
	}
	upointer2 = upointer;
	upointer = upointer->next;
	
	while(upointer != nullptr){
		
		if(upointer->name == user){
			upointer2->next = upointer->next;
			return;
		}
		upointer2 = upointer;
		upointer=upointer->next;
	}
	return;
	
}



void group_man::insert_grp(string group,string user){
	group_l *new_group = new group_l;
	group_l *pointer;
	user_l *new_user = new user_l;
	
	ofstream group_fout("groups.txt",ios::app);
	group_fout<< group + " ";
	group_fout.close();
	
	new_user->name = user;
	new_user->next = nullptr;
	
	new_group->name = group;
	new_group->members = new_user;
	
	
	if(groups == nullptr){
		groups = new_group;
		return;
	}
	
	pointer = groups;
	while(pointer->next != nullptr){
		pointer=pointer->next;
		
	}
	
	pointer->next = new_group;
	
	
	
	return;
}

user_l* group_man:: user_clients(string user){
		user_l *pointer;
		user_l *ptr_aux;
		
		if(users == nullptr){
			users = new user_l;
			users->name = user;
			users->clients = 0;
			users->next = nullptr;
		return users;
		}
		
		pointer = users;
		
		while(pointer != nullptr){
			if(pointer->name == user){
				return pointer;
			}
			ptr_aux = pointer;
			pointer = pointer->next;
		}
		pointer = new user_l;
		pointer->name = user;
		pointer->clients = 0;
		pointer->next = nullptr;
		ptr_aux->next = pointer;
		return pointer;
}	


vector<Mensagem> group_man::handle(Mensagem command, int type){
	user_l *user_ptr;
	vector<Mensagem> message;
	Mensagem notf;

	switch(type)
	{
		case LOGIN :
						user_ptr = user_clients(command.usuario);
						if(user_ptr->clients >= 2){
							throw invalid_argument( "simultaneous clients limit" );
							break;
						}
						user_ptr->clients = user_ptr->clients + 1;
						
					if(exist_grp(command.grupo) != nullptr){
						GMServer gm(command.grupo); 
						message = gm.ReadLastMessages(N);
					}
					else{
						insert_grp(command.grupo,command.usuario);
						GMServer gm(command.grupo);
						message = gm.ReadLastMessages(N);
					}
					notf = command;
					notf.texto = "Entrou no grupo";
					message.insert(message.begin(),notf);
					
					break;
					
					
		case LOGOUT :
						user_ptr = user_clients(command.usuario);
						user_ptr->clients = user_ptr->clients - 1;
						remove_user(command.grupo,command.usuario);
						notf = command;
						notf.texto = "Saiu do grupo";
						message.insert(message.begin(),notf);
					break;
			
			
		case MESSAGE : 
						GMServer gm(command.grupo);
						gm.WriteMessage(command);
						message.push_back(command);
	}
	return message;
}
