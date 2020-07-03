#include <libtelegram/libtelegram.h>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string/replace.hpp>
#include "dungeon.hpp"

#define _DEV			//Seta o bot no modo de desenvolvimento. Assim, o bot que responde é o bot de desenvolvimento

const string _CARACTER_DE_ESPACO("澱");
const string _BOT_TOKEN("*****");
const string _DEV_TOKEN("*****");
const int 	 _ERRO_NUMERO = -17237882;		//Um numero qualquer

using namespace std;

void salvarPersonagem (Personagem&);
int carregarPersonagem (Personagem&);
void setStatusInicial(Personagem&);
inline int getNumero(string);
inline int getTextoEquip(string,string&,int&,int&,string&);
inline int getTextoEquip(string,string&);

int main()
{
	Personagem Melliandre	("Melliandre","Elfo","Bardo");
	Personagem Hyggorth		("Hyggorth","Elfo","Druida");
	Personagem Violetta		("Violetta","Humana","Maga");
	Personagem Dominnus		("Dominnus","Humano","Paladino");

	if(carregarPersonagem(Melliandre) != 0)		//Caso nao haja arquivo do personagem em disco
	{
		cout << "Nao existe arquivo para o personagem " + Melliandre.getNome() << endl;
		setStatusInicial(Melliandre);
		salvarPersonagem (Melliandre);
	}

	if(carregarPersonagem(Hyggorth) != 0)
	{
		cout << "Nao existe arquivo para o personagem " + Hyggorth.getNome() << endl;
		setStatusInicial(Hyggorth);
		salvarPersonagem (Hyggorth);
	}

	if(carregarPersonagem(Violetta) != 0)
	{
		cout << "Nao existe arquivo para o personagem " + Violetta.getNome() << endl;
		setStatusInicial(Violetta);
		salvarPersonagem (Violetta);
	}

	if(carregarPersonagem(Dominnus) != 0)
	{
		cout << "Nao existe arquivo para o personagem " + Dominnus.getNome() << endl;
		setStatusInicial(Dominnus);
		salvarPersonagem (Dominnus);
	}

	#ifdef _DEV
  	string const token(_DEV_TOKEN);     	//TOKEN do bot do Telegram
	#else
	string const token(_BOT_TOKEN);     	//TOKEN do bot do Telegram
	#endif	
  	
	telegram::sender sender(token);                                               
  	telegram::listener::poll listener(sender);                                   
  	listener.set_callback_message_json([&](nlohmann::json const &input)
	{  
		auto const &message(telegram::types::message::from_json(input));

    //-------------------------------------------------HELP----------------------------------------------------------------------//

	if(message.text && (*message.text == "/ajuda" || *message.text == "/ajuda@dungeon_worldbot"))
		{
      		sender.send_message(message.chat.id,
				"Não posso te ajudar na quest, mas é assim que eu funciono\n\"/personagem\" - exibe ficha básica\n\"/personagem stats\" - exibe os stats do personagem\n\"/personagem equip\" - exibe inventario do personagem\n\"/personagem mov\" - exibe movimentos iniciais do personagem\n\"/personagem +[-]xp N\" - adiciona[retira] N XPs\n\"/personagem +[-]pv N\" - adiciona[retira] N XPs\n\"/personagem +[-]ouro N\" - adiciona[retira] N Dinheiros\n\"/personagem +[-]equip\" - adiciona um item no inventario do personagem\n\"/personagem STATS= N\" - Faz \"STATS\" do personagem ser igual a N\n\"/personagem STATS(mod)= N\" - Faz o modificador de \"STATS\" do personagem ser igual a N\n\n(os possíveis \"STATS\" são: FOR,DES,CON,INT,SAB,CAR,Armadura e Dano)"
			);
    	}


	if(message.text && (*message.text == "/sobre"|| *message.text == "/sobre@dungeon_worldbot"))
		{
      		sender.send_message(message.chat.id,
				"(8/1/2018)Versão 1.7 - Principal mudanca é que foi adicionado controle total a todos os stats de todos os personagens. Esse comando é feito pelo novo comando /personagem STATS=, e funciona para todos os stats listados. Adicionado também dinheiro ao personagem, e o comando /personagem +[-]ouro. Além disso, os comandos de +/-XP e +/-PV agora aceitam quaisquer numeros (nao precisa mais estar limitado entre 0 e 9). Também concertado o stat inicial de carga, que não iniciava em 0.\n\n(7/1/2018)Versão 1.6 - Agora o bot está em uma fase beta de gerenciamento de equipamentos (inventario). São disponibilizados 3 comandos, +equip, -equip e equip. Alem disso, ele ficará 24h por dia ligado, e não mais será ligado e desligado o tempo todo (tenho um servidor rodando em casa)\n\n(6/1/2018)Versão 1.4 - Pequena atualização, agora também está disponível as opções +pv e -pv\n\n(5/1/2018)Versão 1.3 - Agora o bot esta implementado em classes! Pronto para receber novas atualizacoes, que farao todos os parametros serem administrados pelo chat. No momento so a opcao de XP ta aqui para teste\n\n(28/12/2017)Versão 1.0 - Primeira versão! Não tem nada implementado, são só vários textos"
			);
    	}
	
	//---------------------------------------------------------------------------------------------------------------------------//


	//-------------------------------------------------MENSAGENS-PADRÃO-DOS-PERSONAGENS---------------------------------------------//
	 if(message.text && (*message.text == "/melliandre" || *message.text == "/mel" || *message.text == "/melliandre@dungeon_worldbot"))
		{
      		sender.send_message(message.chat.id,
				"Nome: "  + Melliandre.getNome() 																	+"\n"+
				"Classe: "+ Melliandre.getClasse() 																	+"\n"+
				"PV: "+ to_string(Melliandre.PV.get()) +"/"+ to_string(Melliandre.PV.getMax())+"(6+CON)"			+"\n"+	
				"Armadura: "+ to_string(Melliandre.Armadura.get())+"("+to_string(Melliandre.Armadura.getMod())+")"	+"\n"+
				"Dano: d"+to_string(Melliandre.Dano.get())+"("+to_string(Melliandre.Dano.getMod())+")"				+"\n"+
				"Nível: "+to_string(Melliandre.Nivel.get())+														+"\n"+
				"XP: "+to_string(Melliandre.XP.get())+"/"+to_string(Melliandre.Nivel.get()+7)+"(nível atual+7)"		+"\n"+
				"Carga: "+to_string(Melliandre.Carga.get())+"/"+to_string(Melliandre.Carga.getMax())+"(9+FOR)"		+"\n"+
				"Raça: "+Melliandre.getRaca()+"(Pode perguntar a história do lugar que visitar)."					+"\n"+
				"Ouro: "+to_string(Melliandre.Ouro.get())															+"\n"
			);
    	}

	if(message.text && (*message.text == "/hyggorth" || *message.text == "/hyg" || *message.text == "/hyggorth@dungeon_worldbot"))
		{
      		sender.send_message(message.chat.id,
				"Nome: "  + Hyggorth.getNome() 																		+"\n"+
				"Classe: "+ Hyggorth.getClasse() 																	+"\n"+
				"PV: "+ to_string(Hyggorth.PV.get()) +"/"+ to_string(Hyggorth.PV.getMax())+"(6+CON)"				+"\n"+	
				"Armadura: "+ to_string(Hyggorth.Armadura.get())+"("+to_string(Hyggorth.Armadura.getMod())+")"	+"\n"+
				"Dano: d"+to_string(Hyggorth.Dano.get())+"("+to_string(Hyggorth.Dano.getMod())+")"					+"\n"+
				"Nível: "+to_string(Hyggorth.Nivel.get())+															+"\n"+
				"XP: "+to_string(Hyggorth.XP.get())+"/"+to_string(Hyggorth.Nivel.get()+7)+"(nível atual+7)"			+"\n"+
				"Carga: "+to_string(Hyggorth.Carga.get())+"/"+to_string(Hyggorth.Carga.getMax())+"(9+FOR)"			+"\n"+
				"Raça: "+Hyggorth.getRaca()+"(Pode se transformar em qualquer animal da floresta)."					+"\n"+
				"Ouro: "+to_string(Hyggorth.Ouro.get())																+"\n"

			);
    	}

	if(message.text && (*message.text == "/violetta" || *message.text == "/vio" || *message.text == "/violetta@dungeon_worldbot"))
		{
      		sender.send_message(message.chat.id,
				"Nome: "  + Violetta.getNome() 																		+"\n"+
				"Classe: "+ Violetta.getClasse() 																	+"\n"+
				"PV: "+ to_string(Violetta.PV.get()) +"/"+ to_string(Violetta.PV.getMax())+"(6+CON)"				+"\n"+	
				"Armadura: "+ to_string(Violetta.Armadura.get())+"("+to_string(Violetta.Armadura.getMod())+")"		+"\n"+
				"Dano: d"+to_string(Violetta.Dano.get())+"("+to_string(Violetta.Dano.getMod())+")"					+"\n"+
				"Nível: "+to_string(Violetta.Nivel.get())+															+"\n"+
				"XP: "+to_string(Violetta.XP.get())+"/"+to_string(Violetta.Nivel.get()+7)+"(nível atual+7)"			+"\n"+
				"Carga: "+to_string(Violetta.Carga.get())+"/"+to_string(Violetta.Carga.getMax())+"(9+FOR)"			+"\n"+
				"Raça: "+Violetta.getRaca()+"(Sabe usar um feitiço de clérigo: falar com mortos)."					+"\n"+
				"Ouro: "+to_string(Violetta.Ouro.get())																+"\n"

			);
    	}		

	if(message.text && (*message.text == "/dominnus" || *message.text == "/dom" || *message.text == "/dominnus@dungeon_worldbot"))
		{
      		sender.send_message(message.chat.id,
				"Nome: "  + Dominnus.getNome() 																				+"\n"+
				"Classe: "+ Dominnus.getClasse() 																			+"\n"+
				"PV: "+ to_string(Dominnus.PV.get()) +"/"+ to_string(Dominnus.PV.getMax())+"(6+CON)"						+"\n"+	
				"Armadura: "+ to_string(Dominnus.Armadura.get())+"("+to_string(Dominnus.Armadura.getMod())+")"				+"\n"+
				"Dano: d"+to_string(Dominnus.Dano.get())+"("+to_string(Dominnus.Dano.getMod())+")"							+"\n"+
				"Nível: "+to_string(Dominnus.Nivel.get())+																	+"\n"+
				"XP: "+to_string(Dominnus.XP.get())+"/"+to_string(Dominnus.Nivel.get()+7)+"(nível atual+7)"					+"\n"+
				"Carga: "+to_string(Dominnus.Carga.get())+"/"+to_string(Dominnus.Carga.getMax())+"(9+FOR)"					+"\n"+
				"Raça: "+Dominnus.getRaca()+"(Pode descobrir se há algo maligno nas redondezas através de uma oração)."		+"\n"+
				"Ouro: "+to_string(Dominnus.Ouro.get())																		+"\n"

			);
    	}
	//---------------------------------------------------------------------------------------------------------------------------//

	//--------------------------------------------------------------STATUS-------------------------------------------------------//

	 if(message.text && (*message.text == "/melliandre stats" || *message.text == "/mel stats" || *message.text == "/melliandre@dungeon_worldbot stats"))
		{
			sender.send_message(message.chat.id,
				"FOR: "+ to_string(Melliandre.FOR.get())+"("+to_string(Melliandre.FOR.getMod())+")"						+"\n"+
				"DES: "+ to_string(Melliandre.DES.get())+"("+to_string(Melliandre.DES.getMod())+")"						+"\n"+
				"CON: "+ to_string(Melliandre.CON.get())+"("+to_string(Melliandre.CON.getMod())+")"						+"\n"+	
				"INT: "+ to_string(Melliandre.INT.get())+"("+to_string(Melliandre.INT.getMod())+")"						+"\n"+
				"SAB: "+ to_string(Melliandre.SAB.get())+"("+to_string(Melliandre.SAB.getMod())+")"						+"\n"+
				"CAR: "+ to_string(Melliandre.CAR.get())+"("+to_string(Melliandre.CAR.getMod())+")"						+"\n"
			);
    	}

	if(message.text && (*message.text == "/hyggorth stats" || *message.text == "/hyg stats" || *message.text == "/hyggorth@dungeon_worldbot stats"))
		{
			sender.send_message(message.chat.id,
				"FOR: "+ to_string(Hyggorth.FOR.get())+"("+to_string(Hyggorth.FOR.getMod())+")"							+"\n"+
				"DES: "+ to_string(Hyggorth.DES.get())+"("+to_string(Hyggorth.DES.getMod())+")"							+"\n"+
				"CON: "+ to_string(Hyggorth.CON.get())+"("+to_string(Hyggorth.CON.getMod())+")"							+"\n"+	
				"INT: "+ to_string(Hyggorth.INT.get())+"("+to_string(Hyggorth.INT.getMod())+")"							+"\n"+
				"SAB: "+ to_string(Hyggorth.SAB.get())+"("+to_string(Hyggorth.SAB.getMod())+")"							+"\n"+
				"CAR: "+ to_string(Hyggorth.CAR.get())+"("+to_string(Hyggorth.CAR.getMod())+")"							+"\n"
			);
    	}

	if(message.text && (*message.text == "/violetta stats" || *message.text == "/vio stats" || *message.text == "/violetta@dungeon_worldbot stats"))
		{
			sender.send_message(message.chat.id,
				"FOR: "+ to_string(Violetta.FOR.get())+"("+to_string(Violetta.FOR.getMod())+")"							+"\n"+
				"DES: "+ to_string(Violetta.DES.get())+"("+to_string(Violetta.DES.getMod())+")"							+"\n"+
				"CON: "+ to_string(Violetta.CON.get())+"("+to_string(Violetta.CON.getMod())+")"							+"\n"+	
				"INT: "+ to_string(Violetta.INT.get())+"("+to_string(Violetta.INT.getMod())+")"							+"\n"+
				"SAB: "+ to_string(Violetta.SAB.get())+"("+to_string(Violetta.SAB.getMod())+")"							+"\n"+
				"CAR: "+ to_string(Violetta.CAR.get())+"("+to_string(Violetta.CAR.getMod())+")"							+"\n"
			);
    	}

	if(message.text && (*message.text == "/dominnus stats" || *message.text == "/dom stats" || *message.text == "/dominnus@dungeon_worldbot stats"))
		{
			sender.send_message(message.chat.id,
				"FOR: "+ to_string(Dominnus.FOR.get())+"("+to_string(Dominnus.FOR.getMod())+")"							+"\n"+
				"DES: "+ to_string(Dominnus.DES.get())+"("+to_string(Dominnus.DES.getMod())+")"							+"\n"+
				"CON: "+ to_string(Dominnus.CON.get())+"("+to_string(Dominnus.CON.getMod())+")"							+"\n"+	
				"INT: "+ to_string(Dominnus.INT.get())+"("+to_string(Dominnus.INT.getMod())+")"							+"\n"+
				"SAB: "+ to_string(Dominnus.SAB.get())+"("+to_string(Dominnus.SAB.getMod())+")"							+"\n"+
				"CAR: "+ to_string(Dominnus.CAR.get())+"("+to_string(Dominnus.CAR.getMod())+")"							+"\n"
			);
    	}

	//---------------------------------------------------------------------------------------------------------------------------//

	//---------------------------------------------------GERENCIAMENTO DOS PERSONAGENS-------------------------------------------//

	  if(
		((*message.text).find("/melliandre +xp") != string::npos)					||
		((*message.text).find("/mel +xp") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot +xp") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{
				Melliandre.XP.set(Melliandre.XP.get()+getNumero(*message.text));
				sender.send_message(message.chat.id,"+"+to_string(getNumero(*message.text))+"XP para " + Melliandre.getNome() +"\n");

				while((Melliandre.Nivel.get()+7) <= Melliandre.XP.get())				//Se eu ter xp suficiente para subir de nivel
				{
					Melliandre.Nivel.set(Melliandre.Nivel.get()+1);
					Melliandre.XP.set(Melliandre.XP.get()-(Melliandre.Nivel.get()+7-1));
					sender.send_message(message.chat.id,Melliandre.getNome()+" subiu de Nível! Nível "+to_string(Melliandre.Nivel.get())+"\n");		
				}

				salvarPersonagem(Melliandre);
			}				
    	}	
	  if(
		((*message.text).find("/hyggorth +xp") != string::npos)						||
		((*message.text).find("/hyg +xp") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot +xp") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{
				Hyggorth.XP.set(Hyggorth.XP.get()+getNumero(*message.text));
				sender.send_message(message.chat.id,"+"+to_string(getNumero(*message.text))+"XP para " + Hyggorth.getNome() +"\n");

					while((Hyggorth.Nivel.get()+7) <= Hyggorth.XP.get())
					{
						Hyggorth.Nivel.set(Hyggorth.Nivel.get()+1);
						Hyggorth.XP.set(Hyggorth.XP.get()-(Hyggorth.Nivel.get()+7-1));
						sender.send_message(message.chat.id,Hyggorth.getNome()+" subiu de Nível! Nível "+to_string(Hyggorth.Nivel.get())+"\n");		
					}
				salvarPersonagem(Hyggorth);
			}				
    	}
	  if(
		((*message.text).find("/violetta +xp") != string::npos)						||
		((*message.text).find("/vio +xp") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot +xp") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{
				Violetta.XP.set(Violetta.XP.get()+getNumero(*message.text));
				sender.send_message(message.chat.id,"+"+to_string(getNumero(*message.text))+"XP para " + Violetta.getNome() +"\n");

					while((Violetta.Nivel.get()+7) <= Violetta.XP.get())
					{
						Violetta.Nivel.set(Violetta.Nivel.get()+1);
						Violetta.XP.set(Violetta.XP.get()-(Violetta.Nivel.get()+7-1));
						sender.send_message(message.chat.id,Violetta.getNome()+" subiu de Nível! Nível "+to_string(Violetta.Nivel.get())+"\n");		
					}
				salvarPersonagem(Violetta);
			}				
    	}
	  if(
		((*message.text).find("/dominnus +xp") != string::npos)						||
		((*message.text).find("/dom +xp") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot +xp") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{
				Dominnus.XP.set(Dominnus.XP.get()+getNumero(*message.text));
				sender.send_message(message.chat.id,"+"+to_string(getNumero(*message.text))+"XP para " + Dominnus.getNome() +"\n");
					while((Dominnus.Nivel.get()+7) <= Dominnus.XP.get())
					{
						Dominnus.Nivel.set(Dominnus.Nivel.get()+1);
						Dominnus.XP.set(Dominnus.XP.get()-(Dominnus.Nivel.get()+7-1));
						sender.send_message(message.chat.id,Dominnus.getNome()+" subiu de Nível! Nível "+to_string(Dominnus.Nivel.get())+"\n");		
					}
				salvarPersonagem(Dominnus);
			}				
    	}
	  if(
		((*message.text).find("/melliandre -xp") != string::npos)					||
		((*message.text).find("/mel -xp") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot -xp") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{

				Melliandre.XP.set(Melliandre.XP.get()-getNumero(*message.text));
				sender.send_message(message.chat.id,"-"+to_string(getNumero(*message.text))+"XP para " + Melliandre.getNome() +"\n");
			
				while(Melliandre.XP.get() < 0)								//Se eu perder xp demais...
				{
					if (Melliandre.Nivel.get() > 1)							//& meu nivel for maior que 1
					{
						Melliandre.Nivel.set(Melliandre.Nivel.get()-1);		//perco um nivel

						Melliandre.XP.set(Melliandre.Nivel.get()+7+Melliandre.XP.get());

						sender.send_message(message.chat.id,Melliandre.getNome()+" perdeu um Nível! Nível "+to_string(Melliandre.Nivel.get())+"\n");
					}
					else
					{
						Melliandre.XP.set(0);			//se nao apenas perco todo xp (nunca fica negativo)
					}
				}
				salvarPersonagem(Melliandre);
			}
		}
	  if(
		((*message.text).find("/hyggorth -xp") != string::npos)						||
		((*message.text).find("/hyg -xp") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot -xp") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{

				Hyggorth.XP.set(Hyggorth.XP.get()-getNumero(*message.text));
				sender.send_message(message.chat.id,"-"+to_string(getNumero(*message.text))+"XP para " + Hyggorth.getNome() +"\n");
			
				while(Hyggorth.XP.get() < 0)								//Se eu perder xp demais...
				{
					if (Hyggorth.Nivel.get() > 1)							//& meu nivel for maior que 1
					{
						Hyggorth.Nivel.set(Hyggorth.Nivel.get()-1);		//perco um nivel

						Hyggorth.XP.set(Hyggorth.Nivel.get()+7+Hyggorth.XP.get());

						sender.send_message(message.chat.id,Hyggorth.getNome()+" perdeu um Nível! Nível "+to_string(Hyggorth.Nivel.get())+"\n");
					}
					else
					{
						Hyggorth.XP.set(0);			//se nao apenas perco todo xp (nunca fica negativo)
					}
				}
				salvarPersonagem(Hyggorth);
			}
		}
	  if(
		((*message.text).find("/violetta -xp") != string::npos)						||
		((*message.text).find("/vio -xp") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot -xp") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{

				Violetta.XP.set(Violetta.XP.get()-getNumero(*message.text));
				sender.send_message(message.chat.id,"-"+to_string(getNumero(*message.text))+"XP para " + Violetta.getNome() +"\n");
			
				while(Violetta.XP.get() < 0)								//Se eu perder xp demais...
				{
					if (Violetta.Nivel.get() > 1)							//& meu nivel for maior que 1
					{
						Violetta.Nivel.set(Violetta.Nivel.get()-1);		//perco um nivel

						Violetta.XP.set(Violetta.Nivel.get()+7+Violetta.XP.get());

						sender.send_message(message.chat.id,Violetta.getNome()+" perdeu um Nível! Nível "+to_string(Violetta.Nivel.get())+"\n");
					}
					else
					{
						Violetta.XP.set(0);			//se nao apenas perco todo xp (nunca fica negativo)
					}
				}
				salvarPersonagem(Violetta);
			}
		}
	  if(
		((*message.text).find("/dominnus -xp") != string::npos)						||
		((*message.text).find("/dom -xp") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot -xp") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{

				Dominnus.XP.set(Dominnus.XP.get()-getNumero(*message.text));
				sender.send_message(message.chat.id,"-"+to_string(getNumero(*message.text))+"XP para " + Dominnus.getNome() +"\n");
			
				while(Dominnus.XP.get() < 0)								//Se eu perder xp demais...
				{
					if (Dominnus.Nivel.get() > 1)							//& meu nivel for maior que 1
					{
						Dominnus.Nivel.set(Dominnus.Nivel.get()-1);		//perco um nivel

						Dominnus.XP.set(Dominnus.Nivel.get()+7+Dominnus.XP.get());

						sender.send_message(message.chat.id,Dominnus.getNome()+" perdeu um Nível! Nível "+to_string(Dominnus.Nivel.get())+"\n");
					}
					else
					{
						Dominnus.XP.set(0);			//se nao apenas perco todo xp (nunca fica negativo)
					}
				}
				salvarPersonagem(Dominnus);
			}
		}




	  if(
		((*message.text).find("/melliandre +pv") != string::npos)					||
		((*message.text).find("/mel +pv") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot +pv") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{
				Melliandre.PV.set(Melliandre.PV.get()+getNumero(*message.text));
				if (Melliandre.PV.get()-getNumero(*message.text) == 0)
					sender.send_message(message.chat.id,Melliandre.getNome()+" renasceu das cinzas!"+"\n");

				if (Melliandre.PV.get() > Melliandre.PV.getMax())		//se eu ganhar mais pv do que meu maximo de vida
					{
						Melliandre.PV.set(Melliandre.PV.getMax());
						sender.send_message(message.chat.id,Melliandre.getNome()+" Esta com PV completo."+"\n");		
					}
					else
					{
						sender.send_message(message.chat.id,"+"+to_string(getNumero(*message.text))+"PV para " + Melliandre.getNome() +"\n");
					}
				salvarPersonagem(Melliandre);
			}				
    	}
	  if(
		((*message.text).find("/hyggorth +pv") != string::npos)						||
		((*message.text).find("/hyg +pv") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot +pv") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{
				Hyggorth.PV.set(Hyggorth.PV.get()+getNumero(*message.text));
				if (Hyggorth.PV.get()-getNumero(*message.text) == 0)
					sender.send_message(message.chat.id,Hyggorth.getNome()+" renasceu das cinzas!"+"\n");

				if (Hyggorth.PV.get() > Hyggorth.PV.getMax())		//se eu ganhar mais pv do que meu maximo de vida
					{
						Hyggorth.PV.set(Hyggorth.PV.getMax());
						sender.send_message(message.chat.id,Hyggorth.getNome()+" Esta com PV completo."+"\n");		
					}
					else
					{
						sender.send_message(message.chat.id,"+"+to_string(getNumero(*message.text))+"PV para " + Hyggorth.getNome() +"\n");
					}
				salvarPersonagem(Hyggorth);
			}				
    	}
	  if(
		((*message.text).find("/violetta +pv") != string::npos)						||
		((*message.text).find("/vio +pv") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot +pv") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{
				Violetta.PV.set(Violetta.PV.get()+getNumero(*message.text));
				if (Violetta.PV.get()-getNumero(*message.text) == 0)
					sender.send_message(message.chat.id,Violetta.getNome()+" renasceu das cinzas!"+"\n");

				if (Violetta.PV.get() > Violetta.PV.getMax())		//se eu ganhar mais pv do que meu maximo de vida
					{
						Violetta.PV.set(Violetta.PV.getMax());
						sender.send_message(message.chat.id,Violetta.getNome()+" Esta com PV completo."+"\n");		
					}
					else
					{
						sender.send_message(message.chat.id,"+"+to_string(getNumero(*message.text))+"PV para " + Violetta.getNome() +"\n");
					}
				salvarPersonagem(Violetta);
			}				
    	}
	  if(
		((*message.text).find("/dominnus +pv") != string::npos)						||
		((*message.text).find("/dom +pv") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot +pv") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{
				Dominnus.PV.set(Dominnus.PV.get()+getNumero(*message.text));
				if (Dominnus.PV.get()-getNumero(*message.text) == 0)
					sender.send_message(message.chat.id,Dominnus.getNome()+" renasceu das cinzas!"+"\n");

				if (Dominnus.PV.get() > Dominnus.PV.getMax())		//se eu ganhar mais pv do que meu maximo de vida
					{
						Dominnus.PV.set(Dominnus.PV.getMax());
						sender.send_message(message.chat.id,Dominnus.getNome()+" Esta com PV completo."+"\n");		
					}
					else
					{
						sender.send_message(message.chat.id,"+"+to_string(getNumero(*message.text))+"PV para " + Dominnus.getNome() +"\n");
					}
				salvarPersonagem(Dominnus);
			}				
    	}
	  if(
		((*message.text).find("/melliandre -pv") != string::npos)					||
		((*message.text).find("/mel -pv") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot -pv") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{

				Melliandre.PV.set(Melliandre.PV.get()-getNumero(*message.text));			
				if (Melliandre.PV.get() <= 0)								//Se eu perder PV demais...
				{
					Melliandre.PV.set(0);									//Eu morro!
					sender.send_message(message.chat.id,Melliandre.getNome()+" morreu! ¡oh que mal!"+"\n");
				}
				else
				{
					sender.send_message(message.chat.id,"-"+to_string(getNumero(*message.text))+"PV para " + Melliandre.getNome() +"\n");
				}
				salvarPersonagem(Melliandre);
			}
		}
	  if(
		((*message.text).find("/hyggorth -pv") != string::npos)						||
		((*message.text).find("/hyg -pv") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot -pv") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{

				Hyggorth.PV.set(Hyggorth.PV.get()-getNumero(*message.text));			
				if (Hyggorth.PV.get() <= 0)								//Se eu perder PV demais...
				{
					Hyggorth.PV.set(0);									//Eu morro!
					sender.send_message(message.chat.id,Hyggorth.getNome()+" morreu! ¡oh que mal!"+"\n");
				}
				else
				{
					sender.send_message(message.chat.id,"-"+to_string(getNumero(*message.text))+"PV para " + Hyggorth.getNome() +"\n");
				}
				salvarPersonagem(Hyggorth);
			}
		}
	  if(
		((*message.text).find("/violetta -pv") != string::npos)						||
		((*message.text).find("/vio -pv") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot -pv") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{

				Violetta.PV.set(Violetta.PV.get()-getNumero(*message.text));			
				if (Violetta.PV.get() <= 0)								//Se eu perder PV demais...
				{
					Violetta.PV.set(0);									//Eu morro!
					sender.send_message(message.chat.id,Violetta.getNome()+" morreu! ¡oh que mal!"+"\n");
				}
				else
				{
					sender.send_message(message.chat.id,"-"+to_string(getNumero(*message.text))+"PV para " + Violetta.getNome() +"\n");
				}
				salvarPersonagem(Violetta);
			}
		}
	  if(
		((*message.text).find("/dominnus -pv") != string::npos)						||
		((*message.text).find("/dom -pv") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot -pv") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{

				Dominnus.PV.set(Dominnus.PV.get()-getNumero(*message.text));			
				if (Dominnus.PV.get() <= 0)								//Se eu perder PV demais...
				{
					Dominnus.PV.set(0);									//Eu morro!
					sender.send_message(message.chat.id,Dominnus.getNome()+" morreu! ¡oh que mal!"+"\n");
				}
				else
				{
					sender.send_message(message.chat.id,"-"+to_string(getNumero(*message.text))+"PV para " + Dominnus.getNome() +"\n");
				}
				salvarPersonagem(Dominnus);
			}
		}

	  if(
		((*message.text).find("/melliandre +ouro") != string::npos)						||
		((*message.text).find("/mel +ouro") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot +ouro") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{
				Melliandre.Ouro.set(Melliandre.Ouro.get()+getNumero(*message.text));
				sender.send_message(message.chat.id,"+"+to_string(getNumero(*message.text))+"Ouro para " + Melliandre.getNome() +"\n");

				salvarPersonagem(Melliandre);
			}				
    	}
	  if(
		((*message.text).find("/melliandre -ouro") != string::npos)						||
		((*message.text).find("/mel -ouro") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot -ouro") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{
				Melliandre.Ouro.set(Melliandre.Ouro.get()-getNumero(*message.text));
				sender.send_message(message.chat.id,"-"+to_string(getNumero(*message.text))+"Ouro para " + Melliandre.getNome() +"\n");

				salvarPersonagem(Melliandre);
			}				
    	}
	  if(
		((*message.text).find("/hyggorth +ouro") != string::npos)						||
		((*message.text).find("/hyg +ouro") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot +ouro") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{
				Hyggorth.Ouro.set(Hyggorth.Ouro.get()+getNumero(*message.text));
				sender.send_message(message.chat.id,"+"+to_string(getNumero(*message.text))+"Ouro para " + Hyggorth.getNome() +"\n");

				salvarPersonagem(Hyggorth);
			}				
    	}
	  if(
		((*message.text).find("/hyggorth -ouro") != string::npos)						||
		((*message.text).find("/hyg -ouro") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot -ouro") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{
				Hyggorth.Ouro.set(Hyggorth.Ouro.get()-getNumero(*message.text));
				sender.send_message(message.chat.id,"-"+to_string(getNumero(*message.text))+"Ouro para " + Hyggorth.getNome() +"\n");

				salvarPersonagem(Hyggorth);
			}				
    	}
	  if(
		((*message.text).find("/violetta +ouro") != string::npos)						||
		((*message.text).find("/vio +ouro") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot +ouro") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{
				Violetta.Ouro.set(Violetta.Ouro.get()+getNumero(*message.text));
				sender.send_message(message.chat.id,"+"+to_string(getNumero(*message.text))+"Ouro para " + Violetta.getNome() +"\n");

				salvarPersonagem(Violetta);
			}				
    	}
	  if(
		((*message.text).find("/violetta -ouro") != string::npos)						||
		((*message.text).find("/vio -ouro") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot -ouro") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{
				Violetta.Ouro.set(Violetta.Ouro.get()-getNumero(*message.text));
				sender.send_message(message.chat.id,"-"+to_string(getNumero(*message.text))+"Ouro para " + Violetta.getNome() +"\n");

				salvarPersonagem(Violetta);
			}				
    	}
	  if(
		((*message.text).find("/dominnus +ouro") != string::npos)						||
		((*message.text).find("/dom +ouro") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot +ouro") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{
				Dominnus.Ouro.set(Dominnus.Ouro.get()+getNumero(*message.text));
				sender.send_message(message.chat.id,"+"+to_string(getNumero(*message.text))+"Ouro para " + Dominnus.getNome() +"\n");

				salvarPersonagem(Dominnus);
			}				
    	}
	  if(
		((*message.text).find("/dominnus -ouro") != string::npos)						||
		((*message.text).find("/dom -ouro") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot -ouro") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem +[-]funcao\nNumero\n");
			else
			{
				Dominnus.Ouro.set(Dominnus.Ouro.get()-getNumero(*message.text));
				sender.send_message(message.chat.id,"-"+to_string(getNumero(*message.text))+"Ouro para " + Dominnus.getNome() +"\n");

				salvarPersonagem(Dominnus);
			}				
    	}
	
	//---------------------------------------------------------------------------------------------------------------------------//



	//-------------------------------------------------EQUIPAMENTOS--------------------------------------------------------------//

	  if(
		((*message.text).find("/melliandre +equip") != string::npos)						||
		((*message.text).find("/mel +equip") != string::npos)								||
		((*message.text).find("/melliandre@dungeon_worldbot +equip") != string::npos)
		)
		{
			string nome;
			int peso;
			int quantidade;
			string descricao;
			if (getTextoEquip(*message.text,nome,peso,quantidade,descricao) != 0)
				sender.send_message(message.chat.id,"Digite algo no formado:\n\n/personagem +equip\nNome do Equipamento\nPeso do equipamento (apenas numeros)\nQuantidade (apenas numeros)\nDescricao\n");
			else
			{
				if (Melliandre.Carga.get()+peso > Melliandre.Carga.getMax())
						sender.send_message(message.chat.id,Melliandre.getNome()+" não pode carregar esse equipamento (Excede limite de carga)\n");
				else
				{   
					boost::replace_all(nome, " ",_CARACTER_DE_ESPACO);			//As strings que sao salvas em arquivos nao podem ter espaços
					boost::replace_all(descricao, " ",_CARACTER_DE_ESPACO);		//

					Item *item = new Item(nome,peso);
					item->setDescricao(descricao);
                    
					if(Melliandre.equipamento.adicionarEquipamento(*item) != 0)
						sender.send_message(message.chat.id,Melliandre.getNome()+" já possui esse item\n");
					else
					{
						Melliandre.Carga.set(Melliandre.Carga.get()+peso);
						try{
							Melliandre.equipamento.acessarItemNome(nome).setQuantidade(quantidade);
						}
						catch(...){
							cout << "Erro Desconhecido\n";
							sender.send_message(message.chat.id,
								"Erro deconhecido ao tentar acessar:\nMelliandre.equipamento.acessarItemNome(nome).setQuantidade(quantidade);");
						}
						boost::replace_all(nome,_CARACTER_DE_ESPACO," ");
						sender.send_message(message.chat.id,nome+" adicionado ao inventário\n");
					}
				}

			}

			salvarPersonagem(Melliandre);
		}
	  if(
		((*message.text).find("/hyggorth +equip") != string::npos)						||
		((*message.text).find("/hyg +equip") != string::npos)								||
		((*message.text).find("/hyggorth@dungeon_worldbot +equip") != string::npos)
		)
		{
			string nome;
			int peso;
			int quantidade;
			string descricao;
			if (getTextoEquip(*message.text,nome,peso,quantidade,descricao) != 0)
				sender.send_message(message.chat.id,"Digite algo no formado:\n\n/personagem +equip\nNome do Equipamento\nPeso do equipamento (apenas numeros)\nQuantidade (apenas numeros)\nDescricao\n");
			else
			{
				if (Hyggorth.Carga.get()+peso > Hyggorth.Carga.getMax())
						sender.send_message(message.chat.id,Hyggorth.getNome()+" não pode carregar esse equipamento (Excede limite de carga)\n");
				else
				{   
					boost::replace_all(nome, " ",_CARACTER_DE_ESPACO);			//As strings que sao salvas em arquivos nao podem ter espaços
					boost::replace_all(descricao, " ",_CARACTER_DE_ESPACO);		//

					Item *item = new Item(nome,peso);
					item->setDescricao(descricao);
                    
					if(Hyggorth.equipamento.adicionarEquipamento(*item) != 0)
						sender.send_message(message.chat.id,Hyggorth.getNome()+" já possui esse item\n");
					else
					{
						Hyggorth.Carga.set(Hyggorth.Carga.get()+peso);
						try{
							Hyggorth.equipamento.acessarItemNome(nome).setQuantidade(quantidade);
						}
						catch(...){
							cout << "Erro Desconhecido\n";
							sender.send_message(message.chat.id,
								"Erro deconhecido ao tentar acessar:\nHyggorth.equipamento.acessarItemNome(nome).setQuantidade(quantidade);");
						}
						boost::replace_all(nome,_CARACTER_DE_ESPACO," ");
						sender.send_message(message.chat.id,nome+" adicionado ao inventário\n");
					}
				}

			}

			salvarPersonagem(Hyggorth);
		}
	  if(
		((*message.text).find("/violetta +equip") != string::npos)						||
		((*message.text).find("/vio +equip") != string::npos)								||
		((*message.text).find("/violetta@dungeon_worldbot +equip") != string::npos)
		)
		{
			string nome;
			int peso;
			int quantidade;
			string descricao;
			if (getTextoEquip(*message.text,nome,peso,quantidade,descricao) != 0)
				sender.send_message(message.chat.id,"Digite algo no formado:\n\n/personagem +equip\nNome do Equipamento\nPeso do equipamento (apenas numeros)\nQuantidade (apenas numeros)\nDescricao\n");
			else
			{
				if (Violetta.Carga.get()+peso > Violetta.Carga.getMax())
						sender.send_message(message.chat.id,Violetta.getNome()+" não pode carregar esse equipamento (Excede limite de carga)\n");
				else
				{   
					boost::replace_all(nome, " ",_CARACTER_DE_ESPACO);			//As strings que sao salvas em arquivos nao podem ter espaços
					boost::replace_all(descricao, " ",_CARACTER_DE_ESPACO);		//

					Item *item = new Item(nome,peso);
					item->setDescricao(descricao);
                    
					if(Violetta.equipamento.adicionarEquipamento(*item) != 0)
						sender.send_message(message.chat.id,Violetta.getNome()+" já possui esse item\n");
					else
					{
						Violetta.Carga.set(Violetta.Carga.get()+peso);
						try{
							Violetta.equipamento.acessarItemNome(nome).setQuantidade(quantidade);
						}
						catch(...){
							cout << "Erro Desconhecido\n";
							sender.send_message(message.chat.id,
								"Erro deconhecido ao tentar acessar:\nVioletta.equipamento.acessarItemNome(nome).setQuantidade(quantidade);");
						}
						boost::replace_all(nome,_CARACTER_DE_ESPACO," ");
						sender.send_message(message.chat.id,nome+" adicionado ao inventário\n");
					}
				}

			}

			salvarPersonagem(Violetta);
		}
	  if(
		((*message.text).find("/dominnus +equip") != string::npos)						||
		((*message.text).find("/dom +equip") != string::npos)								||
		((*message.text).find("/dominnus@dungeon_worldbot +equip") != string::npos)
		)
		{
			string nome;
			int peso;
			int quantidade;
			string descricao;
			if (getTextoEquip(*message.text,nome,peso,quantidade,descricao) != 0)
				sender.send_message(message.chat.id,"Digite algo no formado:\n\n/personagem +equip\nNome do Equipamento\nPeso do equipamento (apenas numeros)\nQuantidade (apenas numeros)\nDescricao\n");
			else
			{
				if (Dominnus.Carga.get()+peso > Dominnus.Carga.getMax())
						sender.send_message(message.chat.id,Dominnus.getNome()+" não pode carregar esse equipamento (Excede limite de carga)\n");
				else
				{   
					boost::replace_all(nome, " ",_CARACTER_DE_ESPACO);			//As strings que sao salvas em arquivos nao podem ter espaços
					boost::replace_all(descricao, " ",_CARACTER_DE_ESPACO);		//

					Item *item = new Item(nome,peso);
					item->setDescricao(descricao);
                    
					if(Dominnus.equipamento.adicionarEquipamento(*item) != 0)
						sender.send_message(message.chat.id,Dominnus.getNome()+" já possui esse item\n");
					else
					{
						Dominnus.Carga.set(Dominnus.Carga.get()+peso);
						try{
							Dominnus.equipamento.acessarItemNome(nome).setQuantidade(quantidade);
						}
						catch(...){
							cout << "Erro Desconhecido\n";
							sender.send_message(message.chat.id,
								"Erro deconhecido ao tentar acessar:\nDominnus.equipamento.acessarItemNome(nome).setQuantidade(quantidade);");
						}
						boost::replace_all(nome,_CARACTER_DE_ESPACO," ");
						sender.send_message(message.chat.id,nome+" adicionado ao inventário\n");
					}
				}

			}

			salvarPersonagem(Dominnus);
		}

	  if(
		((*message.text).find("/melliandre -equip") != string::npos)						||
		((*message.text).find("/mel -equip") != string::npos)								||
		((*message.text).find("/melliandre@dungeon_worldbot -equip") != string::npos)
		)
		{
			string nome;
			int peso;

			if (getTextoEquip(*message.text,nome) != 0)
				sender.send_message(message.chat.id,"Digite algo no formado:\n\n/personagem -equip\nNome do Equipamento a");
			else
			{

				boost::replace_all(nome," ",_CARACTER_DE_ESPACO);			//As strings que sao salvas em arquivos nao podem ter espaços

				try{
					peso = Melliandre.equipamento.acessarItemNome(nome).getPeso();
				}
				catch(erro){
					peso = 0;		//objeto nao existe no inventario
				}
				catch(...){
					peso = 0;
				}

                    
				if(Melliandre.equipamento.retirarEquipamento(nome) != 0)
					sender.send_message(message.chat.id,"Item nao encontrado,nada modificado\n");
				else
				{
					Melliandre.Carga.set(Melliandre.Carga.get()-peso);
					boost::replace_all(nome,_CARACTER_DE_ESPACO," ");
					sender.send_message(message.chat.id,nome+" foi retirado do inventario\n");
				}
			}

			salvarPersonagem(Melliandre);
		}
	  if(
		((*message.text).find("/hyggorth -equip") != string::npos)							||
		((*message.text).find("/hyg -equip") != string::npos)								||
		((*message.text).find("/hyggorth@dungeon_worldbot -equip") != string::npos)
		)
		{
			string nome;
			int peso;

			if (getTextoEquip(*message.text,nome) != 0)
				sender.send_message(message.chat.id,"Digite algo no formado:\n\n/personagem -equip\nNome do Equipamento a");
			else
			{

				boost::replace_all(nome," ",_CARACTER_DE_ESPACO);			//As strings que sao salvas em arquivos nao podem ter espaços

				try{
					peso = Hyggorth.equipamento.acessarItemNome(nome).getPeso();
				}
				catch(erro){
					peso = 0;		//objeto nao existe no inventario
				}
				catch(...){
					peso = 0;
				}

                    
				if(Hyggorth.equipamento.retirarEquipamento(nome) != 0)
					sender.send_message(message.chat.id,"Item nao encontrado,nada modificado\n");
				else
				{
					Hyggorth.Carga.set(Hyggorth.Carga.get()-peso);
					boost::replace_all(nome,_CARACTER_DE_ESPACO," ");
					sender.send_message(message.chat.id,nome+" foi retirado do inventario\n");
				}
			}

			salvarPersonagem(Hyggorth);
		}
	  if(
		((*message.text).find("/violetta -equip") != string::npos)							||
		((*message.text).find("/vio -equip") != string::npos)								||
		((*message.text).find("/violetta@dungeon_worldbot -equip") != string::npos)
		)
		{
			string nome;
			int peso;
			
			if (getTextoEquip(*message.text,nome) != 0)
				sender.send_message(message.chat.id,"Digite algo no formado:\n\n/personagem -equip\nNome do Equipamento a");
			else
			{

				boost::replace_all(nome," ",_CARACTER_DE_ESPACO);			//As strings que sao salvas em arquivos nao podem ter espaços

				try{
					peso = Violetta.equipamento.acessarItemNome(nome).getPeso();
				}
				catch(erro){
					peso = 0;		//objeto nao existe no inventario
				}
				catch(...){
					peso = 0;
				}

                    
				if(Violetta.equipamento.retirarEquipamento(nome) != 0)
					sender.send_message(message.chat.id,"Item nao encontrado,nada modificado\n");
				else
				{
					Violetta.Carga.set(Violetta.Carga.get()-peso);
					boost::replace_all(nome,_CARACTER_DE_ESPACO," ");
					sender.send_message(message.chat.id,nome+" foi retirado do inventario\n");
				}
			}

			salvarPersonagem(Violetta);
		}
	  if(
		((*message.text).find("/dominnus -equip") != string::npos)							||
		((*message.text).find("/dom -equip") != string::npos)								||
		((*message.text).find("/dominnus@dungeon_worldbot -equip") != string::npos)
		)
		{
			string nome;
			int peso;

			if (getTextoEquip(*message.text,nome) != 0)
				sender.send_message(message.chat.id,"Digite algo no formado:\n\n/personagem -equip\nNome do Equipamento a");
			else
			{

				boost::replace_all(nome," ",_CARACTER_DE_ESPACO);			//As strings que sao salvas em arquivos nao podem ter espaços

				try{
					peso = Dominnus.equipamento.acessarItemNome(nome).getPeso();
				}
				catch(erro){
					peso = 0;		//objeto nao existe no inventario
				}
				catch(...){
					peso = 0;
				}

                    
				if(Dominnus.equipamento.retirarEquipamento(nome) != 0)
					sender.send_message(message.chat.id,"Item nao encontrado,nada modificado\n");
				else
				{
					Dominnus.Carga.set(Dominnus.Carga.get()-peso);
					boost::replace_all(nome,_CARACTER_DE_ESPACO," ");
					sender.send_message(message.chat.id,nome+" foi retirado do inventario\n");
				}
			}

			salvarPersonagem(Dominnus);
		}

	 if(message.text && (*message.text == "/melliandre equip" || *message.text == "/mel equip" || *message.text == "/melliandre@dungeon_worldbot equip"))
		{

			sender.send_message(message.chat.id,"Equipamentos de "+Melliandre.getNome()+"\n");
			for(int i = 1;i <= Melliandre.equipamento.getNumeroItens();i++)
			{
				string nome = Melliandre.equipamento.acessarItemIndice(i).getNome();
				string descricao = Melliandre.equipamento.acessarItemIndice(i).getDescricao();

				boost::replace_all(nome,_CARACTER_DE_ESPACO," ");			//troco o caracter de espaco por " " para exibicao da menssagem
				boost::replace_all(descricao,_CARACTER_DE_ESPACO," ");

				sender.send_message(message.chat.id,
					"-"+nome+"\n"+
					+"\tPeso: "+to_string(Melliandre.equipamento.acessarItemIndice(i).getPeso())+"\n"+
					+"\tQnt: "+to_string(Melliandre.equipamento.acessarItemIndice(i).getQuantidade())+"\n"+
//					+"\tUsos: "+to_string(Melliandre.equipamento.acessarItemIndice(i).getUso())+"/"+to_string(Melliandre.equipamento.acessarItemIndice(i).getUsoMax())+"\n"+
//					+"\tCondicao: "+to_string(Melliandre.equipamento.acessarItemIndice(i).getCondicao())+"%\n"+
					+"\t"+descricao+"\n"
				);
			}
    	}
	 if(message.text && (*message.text == "/hyggorth equip" || *message.text == "/hyg equip" || *message.text == "/hyggorth@dungeon_worldbot equip"))
		{

			sender.send_message(message.chat.id,"Equipamentos de "+Hyggorth.getNome()+"\n");
			for(int i = 1;i <= Hyggorth.equipamento.getNumeroItens();i++)
			{
				string nome = Hyggorth.equipamento.acessarItemIndice(i).getNome();
				string descricao = Hyggorth.equipamento.acessarItemIndice(i).getDescricao();

				boost::replace_all(nome,_CARACTER_DE_ESPACO," ");			//troco o caracter de espaco por " " para exibicao da menssagem
				boost::replace_all(descricao,_CARACTER_DE_ESPACO," ");

				sender.send_message(message.chat.id,
					"-"+nome+"\n"+
					+"\tPeso: "+to_string(Hyggorth.equipamento.acessarItemIndice(i).getPeso())+"\n"+
					+"\tQnt: "+to_string(Hyggorth.equipamento.acessarItemIndice(i).getQuantidade())+"\n"+
//					+"\tUsos: "+to_string(Hyggorth.equipamento.acessarItemIndice(i).getUso())+"/"+to_string(Hyggorth.equipamento.acessarItemIndice(i).getUsoMax())+"\n"+
//					+"\tCondicao: "+to_string(Hyggorth.equipamento.acessarItemIndice(i).getCondicao())+"%\n"+
					+"\t"+descricao+"\n"
				);
			}
    	}
	 if(message.text && (*message.text == "/violetta equip" || *message.text == "/vio equip" || *message.text == "/violetta@dungeon_worldbot equip"))
		{

			sender.send_message(message.chat.id,"Equipamentos de "+Violetta.getNome()+"\n");
			for(int i = 1;i <= Violetta.equipamento.getNumeroItens();i++)
			{
				string nome = Violetta.equipamento.acessarItemIndice(i).getNome();
				string descricao = Violetta.equipamento.acessarItemIndice(i).getDescricao();

				boost::replace_all(nome,_CARACTER_DE_ESPACO," ");			//troco o caracter de espaco por " " para exibicao da menssagem
				boost::replace_all(descricao,_CARACTER_DE_ESPACO," ");

				sender.send_message(message.chat.id,
					"-"+nome+"\n"+
					+"\tPeso: "+to_string(Violetta.equipamento.acessarItemIndice(i).getPeso())+"\n"+
					+"\tQnt: "+to_string(Violetta.equipamento.acessarItemIndice(i).getQuantidade())+"\n"+
//					+"\tUsos: "+to_string(Violetta.equipamento.acessarItemIndice(i).getUso())+"/"+to_string(Violetta.equipamento.acessarItemIndice(i).getUsoMax())+"\n"+
//					+"\tCondicao: "+to_string(Violetta.equipamento.acessarItemIndice(i).getCondicao())+"%\n"+
					+"\t"+descricao+"\n"
				);
			}
    	}
	 if(message.text && (*message.text == "/dominnus equip" || *message.text == "/dom equip" || *message.text == "/dominnus@dungeon_worldbot equip"))
		{

			sender.send_message(message.chat.id,"Equipamentos de "+Dominnus.getNome()+"\n");
			for(int i = 1;i <= Dominnus.equipamento.getNumeroItens();i++)
			{
				string nome = Dominnus.equipamento.acessarItemIndice(i).getNome();
				string descricao = Dominnus.equipamento.acessarItemIndice(i).getDescricao();

				boost::replace_all(nome,_CARACTER_DE_ESPACO," ");			//troco o caracter de espaco por " " para exibicao da menssagem
				boost::replace_all(descricao,_CARACTER_DE_ESPACO," ");

				sender.send_message(message.chat.id,
					"-"+nome+"\n"+
					+"\tPeso: "+to_string(Dominnus.equipamento.acessarItemIndice(i).getPeso())+"\n"+
					+"\tQnt: "+to_string(Dominnus.equipamento.acessarItemIndice(i).getQuantidade())+"\n"+
//					+"\tUsos: "+to_string(Dominnus.equipamento.acessarItemIndice(i).getUso())+"/"+to_string(Dominnus.equipamento.acessarItemIndice(i).getUsoMax())+"\n"+
//					+"\tCondicao: "+to_string(Dominnus.equipamento.acessarItemIndice(i).getCondicao())+"%\n"+
					+"\t"+descricao+"\n"
				);
			}
    	}

	//---------------------------------------------------------------------------------------------------------------------------//

	//----------------------------------------------------------OUTROS-STATS-----------------------------------------------------//

   	  if(
		((*message.text).find("/melliandre FOR=") != string::npos)					||
		((*message.text).find("/mel FOR=") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot FOR=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Melliandre.FOR.set(getNumero(*message.text));
				Melliandre.Carga.setMax(getNumero(*message.text)+9);				//FOR altera carga maxima
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Melliandre);
			}				
    	}
   	  if(
		((*message.text).find("/melliandre DES=") != string::npos)					||
		((*message.text).find("/mel DES=") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot DES=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Melliandre.DES.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Melliandre);
			}				
    	}
   	  if(
		((*message.text).find("/melliandre CON=") != string::npos)					||
		((*message.text).find("/mel CON=") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot CON=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Melliandre.CON.set(getNumero(*message.text));
				Melliandre.PV.setMax(getNumero(*message.text)+6);			//Altera o PV maximo do personagem
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Melliandre);
			}				
    	}
   	  if(
		((*message.text).find("/melliandre INT=") != string::npos)					||
		((*message.text).find("/mel INT=") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot INT=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Melliandre.INT.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Melliandre);
			}				
    	}
   	  if(
		((*message.text).find("/melliandre SAB=") != string::npos)					||
		((*message.text).find("/mel SAB=") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot SAB=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Melliandre.SAB.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Melliandre);
			}				
    	}
   	  if(
		((*message.text).find("/melliandre CAR=") != string::npos)					||
		((*message.text).find("/mel CAR=") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot CAR=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Melliandre.CAR.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Melliandre);
			}				
    	}
   	  if(
		((*message.text).find("/melliandre FOR(mod)=") != string::npos)					||
		((*message.text).find("/mel FOR(mod)=") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot FOR(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Melliandre.FOR.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Melliandre);
			}				
    	}
   	  if(
		((*message.text).find("/melliandre DES(mod)=") != string::npos)					||
		((*message.text).find("/mel DES(mod)=") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot DES(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Melliandre.DES.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Melliandre);
			}				
    	}
   	  if(
		((*message.text).find("/melliandre CON(mod)=") != string::npos)					||
		((*message.text).find("/mel CON(mod)=") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot CON(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Melliandre.CON.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Melliandre);
			}				
    	}
   	  if(
		((*message.text).find("/melliandre INT(mod)=") != string::npos)					||
		((*message.text).find("/mel INT(mod)=") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot INT(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Melliandre.INT.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Melliandre);
			}				
    	}
   	  if(
		((*message.text).find("/melliandre SAB(mod)=") != string::npos)					||
		((*message.text).find("/mel SAB(mod)=") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot SAB(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Melliandre.SAB.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Melliandre);
			}				
    	}
   	  if(
		((*message.text).find("/melliandre CAR(mod)=") != string::npos)					||
		((*message.text).find("/mel CAR(mod)=") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot CAR(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Melliandre.CAR.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Melliandre);
			}				
    	}
   	  if(
		((*message.text).find("/hyggorth FOR=") != string::npos)					||
		((*message.text).find("/hyg FOR=") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot FOR=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Hyggorth.FOR.set(getNumero(*message.text));
				Hyggorth.Carga.setMax(getNumero(*message.text)+9);					//FOR altera carga maxima
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Hyggorth);
			}				
    	}
   	  if(
		((*message.text).find("/hyggorth DES=") != string::npos)					||
		((*message.text).find("/hyg DES=") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot DES=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Hyggorth.DES.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Hyggorth);
			}				
    	}
   	  if(
		((*message.text).find("/hyggorth CON=") != string::npos)					||
		((*message.text).find("/hyg CON=") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot CON=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Hyggorth.CON.set(getNumero(*message.text));
				Hyggorth.PV.setMax(getNumero(*message.text)+6);			//Altera o PV maximo do personagem
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Hyggorth);
			}				
    	}
   	  if(
		((*message.text).find("/hyggorth INT=") != string::npos)					||
		((*message.text).find("/hyg INT=") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot INT=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Hyggorth.INT.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Hyggorth);
			}				
    	}
   	  if(
		((*message.text).find("/hyggorth SAB=") != string::npos)					||
		((*message.text).find("/hyg SAB=") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot SAB=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Hyggorth.SAB.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Hyggorth);
			}				
    	}
   	  if(
		((*message.text).find("/hyggorth CAR=") != string::npos)					||
		((*message.text).find("/hyg CAR=") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot CAR=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Hyggorth.CAR.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Hyggorth);
			}				
    	}
   	  if(
		((*message.text).find("/hyggorth FOR(mod)=") != string::npos)					||
		((*message.text).find("/hyg FOR(mod)=") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot FOR(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Hyggorth.FOR.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Hyggorth);
			}				
    	}
   	  if(
		((*message.text).find("/hyggorth DES(mod)=") != string::npos)					||
		((*message.text).find("/hyg DES(mod)=") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot DES(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Hyggorth.DES.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Hyggorth);
			}				
    	}
   	  if(
		((*message.text).find("/hyggorth CON(mod)=") != string::npos)					||
		((*message.text).find("/hyg CON(mod)=") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot CON(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Hyggorth.CON.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Hyggorth);
			}				
    	}
   	  if(
		((*message.text).find("/hyggorth INT(mod)=") != string::npos)					||
		((*message.text).find("/hyg INT(mod)=") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot INT(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Hyggorth.INT.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Hyggorth);
			}				
    	}
   	  if(
		((*message.text).find("/hyggorth SAB(mod)=") != string::npos)					||
		((*message.text).find("/hyg SAB(mod)=") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot SAB(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Hyggorth.SAB.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Hyggorth);
			}				
    	}
   	  if(
		((*message.text).find("/hyggorth CAR(mod)=") != string::npos)					||
		((*message.text).find("/hyg CAR(mod)=") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot CAR(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Hyggorth.CAR.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Hyggorth);
			}				
    	}
   	  if(
		((*message.text).find("/violetta FOR=") != string::npos)					||
		((*message.text).find("/vio FOR=") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot FOR=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Violetta.FOR.set(getNumero(*message.text));
				Violetta.Carga.setMax(getNumero(*message.text)+9);					//FOR altera carga maxima
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Violetta);
			}				
    	}
   	  if(
		((*message.text).find("/violetta DES=") != string::npos)					||
		((*message.text).find("/vio DES=") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot DES=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Violetta.DES.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Violetta);
			}				
    	}
   	  if(
		((*message.text).find("/violetta CON=") != string::npos)					||
		((*message.text).find("/vio CON=") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot CON=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Violetta.CON.set(getNumero(*message.text));
				Violetta.PV.setMax(getNumero(*message.text)+6);			//Altera o PV maximo do personagem
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Violetta);
			}				
    	}
   	  if(
		((*message.text).find("/violetta INT=") != string::npos)					||
		((*message.text).find("/vio INT=") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot INT=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Violetta.INT.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Violetta);
			}				
    	}
   	  if(
		((*message.text).find("/violetta SAB=") != string::npos)					||
		((*message.text).find("/vio SAB=") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot SAB=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Violetta.SAB.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Violetta);
			}				
    	}
   	  if(
		((*message.text).find("/violetta CAR=") != string::npos)					||
		((*message.text).find("/vio CAR=") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot CAR=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Violetta.CAR.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Violetta);
			}				
    	}
   	  if(
		((*message.text).find("/violetta FOR(mod)=") != string::npos)					||
		((*message.text).find("/vio FOR(mod)=") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot FOR(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Violetta.FOR.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Violetta);
			}				
    	}
   	  if(
		((*message.text).find("/violetta DES(mod)=") != string::npos)					||
		((*message.text).find("/vio DES(mod)=") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot DES(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Violetta.DES.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Violetta);
			}				
    	}
   	  if(
		((*message.text).find("/violetta CON(mod)=") != string::npos)					||
		((*message.text).find("/vio CON(mod)=") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot CON(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Violetta.CON.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Violetta);
			}				
    	}
   	  if(
		((*message.text).find("/violetta INT(mod)=") != string::npos)					||
		((*message.text).find("/vio INT(mod)=") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot INT(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Violetta.INT.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Violetta);
			}				
    	}
   	  if(
		((*message.text).find("/violetta SAB(mod)=") != string::npos)					||
		((*message.text).find("/vio SAB(mod)=") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot SAB(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Violetta.SAB.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Violetta);
			}				
    	}
   	  if(
		((*message.text).find("/violetta CAR(mod)=") != string::npos)					||
		((*message.text).find("/vio CAR(mod)=") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot CAR(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Violetta.CAR.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Violetta);
			}				
    	}
   	  if(
		((*message.text).find("/dominnus FOR=") != string::npos)					||
		((*message.text).find("/dom FOR=") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot FOR=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Dominnus.FOR.set(getNumero(*message.text));
				Dominnus.Carga.setMax(getNumero(*message.text)+9);					//FOR altera carga maxima
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Dominnus);
			}				
    	}
   	  if(
		((*message.text).find("/dominnus DES=") != string::npos)					||
		((*message.text).find("/dom DES=") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot DES=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Dominnus.DES.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Dominnus);
			}				
    	}
   	  if(
		((*message.text).find("/dominnus CON=") != string::npos)					||
		((*message.text).find("/dom CON=") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot CON=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Dominnus.CON.set(getNumero(*message.text));
				Dominnus.PV.setMax(getNumero(*message.text)+6);			//Altera o PV maximo do personagem
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Dominnus);
			}				
    	}
   	  if(
		((*message.text).find("/dominnus INT=") != string::npos)					||
		((*message.text).find("/dom INT=") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot INT=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Dominnus.INT.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Dominnus);
			}				
    	}
   	  if(
		((*message.text).find("/dominnus SAB=") != string::npos)					||
		((*message.text).find("/dom SAB=") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot SAB=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Dominnus.SAB.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Dominnus);
			}				
    	}
   	  if(
		((*message.text).find("/dominnus CAR=") != string::npos)					||
		((*message.text).find("/dom CAR=") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot CAR=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Dominnus.CAR.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Dominnus);
			}				
    	}
   	  if(
		((*message.text).find("/dominnus FOR(mod)=") != string::npos)					||
		((*message.text).find("/dom FOR(mod)=") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot FOR(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Dominnus.FOR.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Dominnus);
			}				
    	}
   	  if(
		((*message.text).find("/dominnus DES(mod)=") != string::npos)					||
		((*message.text).find("/dom DES(mod)=") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot DES(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Dominnus.DES.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Dominnus);
			}				
    	}
   	  if(
		((*message.text).find("/dominnus CON(mod)=") != string::npos)					||
		((*message.text).find("/dom CON(mod)=") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot CON(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Dominnus.CON.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Dominnus);
			}				
    	}
   	  if(
		((*message.text).find("/dominnus INT(mod)=") != string::npos)					||
		((*message.text).find("/dom INT(mod)=") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot INT(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Dominnus.INT.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Dominnus);
			}				
    	}
   	  if(
		((*message.text).find("/dominnus SAB(mod)=") != string::npos)					||
		((*message.text).find("/dom SAB(mod)=") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot SAB(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Dominnus.SAB.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Dominnus);
			}				
    	}
   	  if(
		((*message.text).find("/dominnus CAR(mod)=") != string::npos)					||
		((*message.text).find("/dom CAR(mod)=") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot CAR(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Dominnus.CAR.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Dominnus);
			}				
    	}
	  if(
		((*message.text).find("/melliandre Armadura=") != string::npos)						||
		((*message.text).find("/mel Armadura=") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot Armadura=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Melliandre.Armadura.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Melliandre);
			}				
    	}
	  if(
		((*message.text).find("/melliandre Armadura(mod)=") != string::npos)					||
		((*message.text).find("/mel Armadura(mod)=") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot Armadura(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Melliandre.Armadura.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Melliandre);
			}				
    	}
	  if(
		((*message.text).find("/hyggorth Armadura=") != string::npos)						||
		((*message.text).find("/hyg Armadura=") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot Armadura=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Hyggorth.Armadura.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Hyggorth);
			}				
    	}
	  if(
		((*message.text).find("/hyggorth Armadura(mod)=") != string::npos)						||
		((*message.text).find("/hyg Armadura(mod)=") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot Armadura(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Hyggorth.Armadura.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Hyggorth);
			}				
    	}
	  if(
		((*message.text).find("/violetta Armadura=") != string::npos)						||
		((*message.text).find("/vio Armadura=") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot Armadura=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Violetta.Armadura.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Violetta);
			}				
    	}
	  if(
		((*message.text).find("/violetta Armadura(mod)=") != string::npos)						||
		((*message.text).find("/vio Armadura(mod)=") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot Armadura(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Violetta.Armadura.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Violetta);
			}				
    	}
	  if(
		((*message.text).find("/dominnus Armadura=") != string::npos)						||
		((*message.text).find("/dom Armadura=") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot Armadura=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Dominnus.Armadura.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Dominnus);
			}				
    	}
	  if(
		((*message.text).find("/dominnus Armadura(mod)=") != string::npos)						||
		((*message.text).find("/dom Armadura(mod)=") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot Armadura(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Dominnus.Armadura.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Dominnus);
			}				
    	}
	  if(
		((*message.text).find("/melliandre Dano=") != string::npos)						||
		((*message.text).find("/mel Dano=") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot Dano=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Melliandre.Dano.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Melliandre);
			}				
    	}
	  if(
		((*message.text).find("/melliandre Dano(mod)=") != string::npos)					||
		((*message.text).find("/mel Dano(mod)=") != string::npos)							||
		((*message.text).find("/melliandre@dungeon_worldbot Dano(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Melliandre.Dano.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Melliandre);
			}				
    	}
	  if(
		((*message.text).find("/hyggorth Dano=") != string::npos)						||
		((*message.text).find("/hyg Dano=") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot Dano=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Hyggorth.Dano.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Hyggorth);
			}				
    	}
	  if(
		((*message.text).find("/hyggorth Dano(mod)=") != string::npos)						||
		((*message.text).find("/hyg Dano(mod)=") != string::npos)							||
		((*message.text).find("/hyggorth@dungeon_worldbot Dano(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Hyggorth.Dano.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Hyggorth);
			}				
    	}
	  if(
		((*message.text).find("/violetta Dano=") != string::npos)						||
		((*message.text).find("/vio Dano=") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot Dano=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Violetta.Dano.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Violetta);
			}				
    	}
	  if(
		((*message.text).find("/violetta Dano(mod)=") != string::npos)						||
		((*message.text).find("/vio Dano(mod)=") != string::npos)							||
		((*message.text).find("/violetta@dungeon_worldbot Dano(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Violetta.Dano.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Violetta);
			}				
    	}
	  if(
		((*message.text).find("/dominnus Dano=") != string::npos)						||
		((*message.text).find("/dom Dano=") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot Dano=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT=\nNúmero(apenas digitos)\n");
			else
			{
				Dominnus.Dano.set(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Dominnus);
			}				
    	}
	  if(
		((*message.text).find("/dominnus Dano(mod)=") != string::npos)						||
		((*message.text).find("/dom Dano(mod)=") != string::npos)							||
		((*message.text).find("/dominnus@dungeon_worldbot Dano(mod)=") != string::npos)
		)
		{
			if(getNumero(*message.text) == _ERRO_NUMERO)
				sender.send_message(message.chat.id,"Digite algo no formato:\n\n/personagem STAT(mod)=\nNúmero(apenas digitos)\n");
			else
			{
				Dominnus.Dano.setMod(getNumero(*message.text));
				sender.send_message(message.chat.id,"stats alterado\n");
				salvarPersonagem(Dominnus);
			}				
    	}




	//---------------------------------------------------------------------------------------------------------------------------//

	//--------------------------------------------------------Movimentos Iniciais------------------------------------------------//

	 if(message.text && (*message.text == "/melliandre mov" || *message.text == "/mel mov" || *message.text == "/melliandre@dungeon_worldbot mov"))
		{
      		sender.send_message(message.chat.id,
				"Movimentos Iniciais:\n- ARTE ARCANA(CAR): Quando tecer um feitiço básico a partir de uma performance, escolha um aliado e um efeito:\n- Curar 1d8 PV;- +1d4 adiante para o dano;\n- Remover um encantamento que esteja afetando sua mente;\n- Na próxima vez que alguém Ajudar o alvo, ele receberá +2, e não +1;\n\nE depois role+CAR. Com 10+, o aliado recebe o efeito selecionado. Com 7-9,  sua magia funciona, mas você atrai atenção indesejável, ou a sua magia reverbera para outros alvos à escolha do MJ, afetando-os também.\n- CONHECIMENTO DE BARDO: você possui conhecimento do Bestiário de Criaturas Incomuns. Quando  encontrar pela primeira vez uma criatura, local ou item importante que esteja ligado ao seu conhecimento de bardo,  você pode fazer uma pergunta qualquer ao MJ a respeito daquilo, que ele deve responder honestamente.\n- CHARMOSO E RECEPTIVO: Quando  conversar francamente com alguém, você pode fazer ao seu jogador uma pergunta da lista abaixo, que deve ser respondida com honestidade. Aquele jogador então poderá também lhe fazer uma pergunta da lista abaixo (que você também precisa responder honestamente):\n- A quem você serve?\n- O que você quer que eu faça?\n- Como  faço  para  conseguir  que  você  ________________?\n- O que você realmente está sentindo agora?\n- O que você mais deseja?\n\n- UM PORTO NA TEMPESTADE: Quando  retornar a um local civilizado que já htenha visitado previamente, diga ao MJ quando esteve aqui pela última vez. Ele lhe responderá honestamente quais foram as mudanças ocorridas desde aquela época.\n"
			);
    	}

	if(message.text && (*message.text == "/hyggorth mov" || *message.text == "/hyg mov" || *message.text == "/hyggorth@dungeon_worldbot mov"))
		{
      		sender.send_message(message.chat.id,
				"Movimentos Iniciais:\n- NASCIDO DO SOLO: Você pode assumir a forma de qualquer animal de sua terra de origem(Norte Gelado).\n\n- NUTRIDO PELA NATUREZA: Você não precisa comer ou beber nada. Se um movimento exigir que você marque uma ração, simplesmente ignore isso.\n\n- LINGUAGEM DOS ESPÍRITOS: Os  grunhidos, latidos, guinchos e  chamados das criaturas selvagens são  uma forma de linguagem para você. Você é capaz de compreender qualquer animal nativo da sua terra ou que seja similar a algum cuja essência tenha estudado.\n\n- METAMORFOSE(SAB): Quando chamar os  espíritos  para mudar sua  forma, role+SAB. Com 10+, domínio 3. Com 7-9, domínio 2. Com 6-, domínio 1 além de qualquer outra coisa definida pelo MJ. Você consegue assumir a forma  de qualquer espécie cuja essência tenha estudado ou que viva em sua terra, fundindo sua própria forma  e seus  pertences em uma cópia perfeita da espécie desejada. Com  isso, você adquire quaisquer habilidades  e fraquezas inatas do animal: garras, asas, guelras, respirar na água e não no  ar, etc. Continue usando suas próprias  características, mas alguns movimentos  podem se tornar mais complicados de serem iniciados  – um gato doméstico  terá dificuldades em combater um ogro.  O MJ também lhe informará a respeito  de um ou  mais movimentos associados à sua nova forma. Gaste 1 domínio para realizar aquele movimento.  Assim  que  seu  domínio  se  esgotar,  você retorna  à sua  forma natural.  É  possível fazer isso também gastando todo o seu domínio.\n\n- ESSÊNCIA ESTUDADA: Quando passar algum  tempo contemplando um espírito animal, você pode adicioná-lo às espécies cuja forma é capaz de assumir através de metamorfose.\n"
			);
    	}

	if(message.text && (*message.text == "/violetta mov" || *message.text == "/vio mov" || *message.text == "/violetta@dungeon_worldbot mov"))
		{
      		sender.send_message(message.chat.id,
				"Movimentos Iniciais:\n- GRIMÓRIO: Item de peso 1 que contém todas as magias que você conhece.\n\n- PREPARAR FEITIÇOS: Quando  passar algum tempo contemplando silenciosamente seu grimório (uma hora, aproximadamente) sem interrupções, você:\n\t- Perde todos os feitiços que havia preparado anteriormente;\n\t- Prepara  novos feitiços  que  estejam anotados  em seu  grimório à  sua escolha, cujo total de níveis não supere seu próprio nível + 1;\n\t- Prepara todos os seus truques, que nunca contam para o limite acima.\n\n- CONJURAR FEITIÇOS(INT):Quando lançar  um  feitiço  preparado, role+INT. Com 10+, o feitiço é conjurado com sucesso  e  não  é  esquecido  –  logo,  você  poderá  conjurá-lo  novamente  mais  tarde.  Com 7-9, o feitiço é conjurado, mas escolha um:\n\t- Você atrai atenção indesejada ou se coloca em alguma situação complicada. O MJ descreverá como;\n\t- Sua conjuração perturba a trama da realidade – receba -1 constante para conjurar feitiços até a próxima vez que você Preparar Feitiços;\n\t- Você esquece o feitiço após conjurá-lo, e não conseguirá conjurá-lo de novo até a próxima vez que preparar feitiços. Repare que manter ativos feitiços com efeitos contínuos pode lhe ocasionar uma penalidade na rolagem de conjurar feitiços.\n\n- DEFESA MÁGICA: Você pode desfazer um feitiço contínuo imediatamente, e utilizar a energia envolvida em sua dissipação para defletir um ataque. O feitiço é encerrado e você subtrai o nível dele do dano recebido.\n\n- RITUAL: Quando drenar energia de um local de poder para criar um efeito mágico, diga ao MJ o que está tentando realizar. Efeitos advindos de rituais são sempre possíveis, mas o MJ lhe dará de uma a quatro das condições abaixo:\n\t- O ritual vai demorar dias/semanas/meses.\n\t- Primeiro você tem que ______.\n\t- Você precisará da ajuda de ______.\n\t- O ritual vai requerer uma enorme quantia em dinheiro.\n\t- O melhor que consegue fazer é uma versão inferior, pouco confiável e limitada.\n\t- Você e seus aliados correrão o perigo de __________.\n\t- Você precisará desencantar ______  para fazê-lo\n\nMovimentos Avançados:\n\n-TRUQUES\n\t-LUZ: Um item tocado por você brilha com luz arcana, com a mesma intensidade de uma  tocha.  Essa luz  não  emite  calor  ou  som  e  nem  precisa  de combustível  para queimar, mas funciona  como uma  tocha  comum para  todos os outros  efeitos. Você possui total controle sobre a cor da luz. O feitiço dura enquanto o item estiver em sua presença.\n\n\t-SERVO INVISÍVEL(contínuo): Este feitiço conjura um construto invisível simples que não é capaz de fazer nada a não ser carregar itens. Ele possui Carga 3 e consegue carregar qualquer coisa que você entregar a ele, mas não é capaz de ir buscar objetos por conta própria. Qualquer coisa carregada por um servo invisível parece flutuar no ar alguns passos atrás do mago. Caso o servo receba dano ou saia de sua presença, ele se desfaz imediatamente. De outra forma, ele o serve até que o feitiço seja encerrado.\n\n\t-PRESTIDIGITAÇÃO: Você é capaz de fazer algumas peripécias com sua magia. Se tocar um item durante a conjuração deste truque, você poderá fazer alterações cosméticas a ele: limpá-lo, sujá-lo, resfriá-lo, aquecê-lo, mudar seu sabor ou sua cor. Se conjurar este feitiço sem tocar um item, poderá invocar pequenas ilusões, cujo tamanho não ultrapasse o seu. As imagens criadas por Prestidigitação são simples e obviamente ilusórias – elas não enganarão ninguém, mas poderão entreter as pessoas.\n\n\n-FEITIÇOS:\n\t-ENCANTAR PESSOA(n1, contínuo): A pessoa (não funciona com animais ou monstros) que você tocar durante a conjuração deste feitiço lhe considerará como um amigo até que sofra algum dano ou que você a prove do contrário.\n\n\t-MÍSSEIS MÁGICOS(n1): Projéteis  de  pura  magia são disparados  das  pontas  de  seus  dedos.  Cause  2d4  de  dano a um alvo.\n\n\t-ALARME(n1, contínuo): Ande  ao redor  de  uma área  circular. Até  que  você prepare feitiços novamente, este feitiço irá alertá-lo caso uma criatura entre no círculo. Mesmo que esteja dormindo, o feitiço lhe despertará de seu sono.\n\n\t-FALAR COM MORTOS(n1): Um cadáver dialoga rapidamente com você, respondendo a até três perguntas com todo o conhecimento que ele tinha em vida, e aquele que adquiriu após a morte.\n"
			);
    	}

	if(message.text && (*message.text == "/dominnus mov" || *message.text == "/dom mov" || *message.text == "/dominnus@dungeon_worldbot mov"))
		{
      		sender.send_message(message.chat.id,
				"Movimentos Iniciais:\n- IMPOR AS MÃOS (CAR): Quando  tocar alguém, pele com pele, e rezar por  seu bem estar,  role+CAR.  Com  10+, você cura 1d8 de dano ou remove uma doença.  Com  7-9, a cura acontece, mas o dano ou a doença é transferido para você. Com 6- nada acontece.\n\n- FORTIFICADO: Ignore o rótulo “desajeitada” em qualquer armadura/arma que você usar.\n\n- EU SOU A LEI (CAR): Quando der uma ordem a um PNJ baseado em sua autoridade divina,  role+CAR. Com  7-9, ele escolhe uma das opções abaixo, dependendo da situação:\n\t- Faz o que foi mandado;\n\t- Recua cuidadosamente, depois foge;\nCom  10+, você também recebe +1 adiante contra o PNJ.  Com  6-, o alvo faz o que bem entender, e você recebe -1 adiante contra ele.\n\n- BUSCA GLORIOSA: Quando você se dedicar a uma missão através da oração e da purificação espiritual, diga o que pretende fazer:\n\t- Destruir  __________,  um  terrível  mal que  se  abate  sobre  a  terra.\n\t- Defender  ________ de um dos  males que  lhe  afligem.\n\t- Descobrir a  verdade a respeito  de __________.\n\nDepois, escolha duas dádivas:\n\t- Um senso de direção infalível  que  me  aponta  para _________.\n\t- Invulnerabilidade contra______________.(por exemplo, armas afiadas, fogo, encantamentos, etc.)\n\t- Uma marca de autoridade divina.\n\t- Sentidos que atravessem qualquer mentira.\n\t- Uma voz que transcenda a linguagem.\n\t- Ver-se livre da fome, sede e sono\n\nO MJ lhe dirá qual voto ou quais votos deverão ser mantidos para que essas dádivas lhe sejam concedidas:\n\t- Honra (proibido: truques e táticas covardes)\n\t- Temperança (proibido: comilança, bebedeira e os prazeres da carne)\n\t- Piedade (requer: realizar seus rituais sagrados diariamente)\n\t- Valor (proibido: permitir que uma criatura maligna viva)\n\t- Verdade (proibido: mentiras)\n\t- Hospitalidade  (requer: dar conforto àqueles que necessitem,  não importa quem sejam).\n"
			);
    	}
	//---------------------------------------------------------------------------------------------------------------------------//

    return;
	});

  listener.set_num_threads(4);// configure how many threads to run with
  listener.run();                                                              
  return EXIT_SUCCESS;
};

void salvarPersonagem (Personagem& personagem)	
{
	std::ofstream arquivo;

	arquivo.open(personagem.getNome()+".dat");

	personagem.PV.serialize(arquivo);
	personagem.Armadura.serialize(arquivo);
	personagem.Dano.serialize(arquivo);
	personagem.Nivel.serialize(arquivo);
	personagem.XP.serialize(arquivo);
	personagem.Carga.serialize(arquivo);
	personagem.FOR.serialize(arquivo);
	personagem.DES.serialize(arquivo);
	personagem.CON.serialize(arquivo);
	personagem.INT.serialize(arquivo);
	personagem.SAB.serialize(arquivo);
	personagem.CAR.serialize(arquivo);
	personagem.Ouro.serialize(arquivo);

	personagem.equipamento.serialize(arquivo);

	arquivo.close();
}

int carregarPersonagem (Personagem& personagem)	
{
	std::ifstream arquivo(personagem.getNome()+".dat");

	if (arquivo.is_open())
    {
        /* Read from file and assign values to Object data*/
        personagem.PV.deserialize(arquivo);
		personagem.Armadura.deserialize(arquivo);
		personagem.Dano.deserialize(arquivo);
		personagem.Nivel.deserialize(arquivo);
		personagem.XP.deserialize(arquivo);
		personagem.Carga.deserialize(arquivo);
		personagem.FOR.deserialize(arquivo);
		personagem.DES.deserialize(arquivo);
		personagem.CON.deserialize(arquivo);
		personagem.INT.deserialize(arquivo);
		personagem.SAB.deserialize(arquivo);
		personagem.CAR.deserialize(arquivo);
		personagem.Ouro.deserialize(arquivo);

		personagem.equipamento.deserialize(arquivo);

		arquivo.close();
	return 0;
    }
    else cout << "Unable to open file.";
	return 1;
}

void setStatusInicial(Personagem& personagem)	//Funcao bem especifica para este RPG
{
	if (personagem.getNome() == "Melliandre")		//Nao eh o melhor mas nao quis perder muito tempo com isso
	{
		personagem.PV.set(18).setMax(18).setMod(0);
		personagem.Armadura.set(0).setMax(0).setMod(0);
		personagem.Dano.set(6).setMax(0).setMod(0);
		personagem.Nivel.set(1).setMax(0).setMod(0);
		personagem.XP.set(0).setMax(0).setMod(0);
		personagem.Carga.set(0).setMax(18).setMod(0);
		personagem.FOR.set(9).setMax(0).setMod(0);
		personagem.DES.set(13).setMax(0).setMod(1);
		personagem.CON.set(12).setMax(0).setMod(0);
		personagem.INT.set(8).setMax(0).setMod(-1);
		personagem.SAB.set(15).setMax(0).setMod(1);
		personagem.CAR.set(16).setMax(0).setMod(2);
		personagem.Ouro.set(0).setMax(0).setMod(0);

		cout << "Personagem " + personagem.getNome() + " inicializado!" << endl;		
	}	
	if (personagem.getNome() == "Hyggorth")
	{
		personagem.PV.set(18).setMax(18).setMod(0);
		personagem.Armadura.set(1).setMax(0).setMod(0);
		personagem.Dano.set(6).setMax(0).setMod(0);
		personagem.Nivel.set(1).setMax(0).setMod(0);
		personagem.XP.set(0).setMax(0).setMod(0);
		personagem.Carga.set(0).setMax(21).setMod(0);
		personagem.FOR.set(15).setMax(0).setMod(1);
		personagem.DES.set(8).setMax(0).setMod(-1);
		personagem.CON.set(12).setMax(0).setMod(0);
		personagem.INT.set(9).setMax(0).setMod(0);
		personagem.SAB.set(16).setMax(0).setMod(2);
		personagem.CAR.set(13).setMax(0).setMod(1);
		personagem.Ouro.set(0).setMax(0).setMod(0);

		cout << "Personagem " + personagem.getNome() + " inicializado!" << endl;		
	}
	if (personagem.getNome() == "Violetta")
	{
		personagem.PV.set(16).setMax(16).setMod(0);
		personagem.Armadura.set(0).setMax(0).setMod(0);
		personagem.Dano.set(4).setMax(0).setMod(0);
		personagem.Nivel.set(1).setMax(0).setMod(0);
		personagem.XP.set(0).setMax(0).setMod(0);
		personagem.Carga.set(0).setMax(16).setMod(0);
		personagem.FOR.set(9).setMax(0).setMod(0);
		personagem.DES.set(8).setMax(0).setMod(-1);
		personagem.CON.set(13).setMax(0).setMod(1);
		personagem.INT.set(16).setMax(0).setMod(2);
		personagem.SAB.set(15).setMax(0).setMod(1);
		personagem.CAR.set(12).setMax(0).setMod(0);
		personagem.Ouro.set(0).setMax(0).setMod(0);

		cout << "Personagem " + personagem.getNome() + " inicializado!" << endl;		
	}
	if (personagem.getNome() == "Dominnus")
	{
		personagem.PV.set(25).setMax(25).setMod(0);
		personagem.Armadura.set(2).setMax(0).setMod(0);
		personagem.Dano.set(10).setMax(0).setMod(1);
		personagem.Nivel.set(1).setMax(0).setMod(0);
		personagem.XP.set(0).setMax(0).setMod(0);
		personagem.Carga.set(0).setMax(28).setMod(0);
		personagem.FOR.set(16).setMax(0).setMod(2);
		personagem.DES.set(9).setMax(0).setMod(0);
		personagem.CON.set(15).setMax(0).setMod(1);
		personagem.INT.set(8).setMax(0).setMod(-1);
		personagem.SAB.set(12).setMax(0).setMod(0);
		personagem.CAR.set(13).setMax(0).setMod(1);
		personagem.Ouro.set(0).setMax(0).setMod(0);

		cout << "Personagem " + personagem.getNome() + " inicializado!" << endl;		
	}	
}

inline int getNumero(string menssagem)
{
	string valor;
	int numero;
	size_t achei = menssagem.find("\n");		//Encontro o primeiro pula-linha

	if (achei==string::npos)
    	return _ERRO_NUMERO;								//Nao encontrei
		
	menssagem.replace(0,achei+1,"");			//Apago a menssagem que nao interessa

	valor = menssagem;							//Separo o nome do numero

	try{
		numero = stoi(valor);					//Transforma para inteiro
	}
	catch(std::invalid_argument& e){
		return _ERRO_NUMERO;
	}
	catch(...){
		return _ERRO_NUMERO;
	}
	
	return numero;
}

inline int getTextoEquip(string menssagem,string& equipamento,int& peso,int& quantidade,string& descricao)				
{
	size_t achei = menssagem.find("\n");		//Encontro o primeiro pula-linha
	descricao = "";

	if (achei==string::npos)
    	return -1;
		
	menssagem.replace(0,achei+1,"");				//Apago a menssagem que nao interessa

	achei = menssagem.find("\n");				//Encontro o segundo pulalinha

	if (achei==string::npos)
    	return -1;

	equipamento = menssagem.substr				//Separo nome do equipamento
			(
				0,
				(int)achei
			);
	
	menssagem.replace(0,achei+1,"");				//Apago nome do equipamento
	
	achei = menssagem.find("\n");				//Encontro o terceiro pulalinha
	
	if (achei==string::npos)
    	return -1;
	
	string numero = menssagem.substr			//Separo o peso do equipamento
			(
				0,
				(int)achei
			);
	
	try{
		peso = stoi(numero);					//Transforma para inteiro
	}
	catch(std::invalid_argument& e){
		return -1;
	}
	catch(...){
		return -1;
	}

	menssagem.replace(0,achei+1,"");				//Apago peso do equipamento

	achei = menssagem.find("\n");				//Encontro o quarto pulalinha
	
	if (achei==string::npos)
    	return -1;
	
	numero = menssagem.substr					//Separo a quantidade
			(
				0,
				(int)achei
			);
	
	try{
		quantidade = stoi(numero);					//Transforma para inteiro
	}
	catch(std::invalid_argument& e){
		return -1;
	}
	catch(...){
		return -1;
	}

	menssagem.replace(0,achei+1,"");

	descricao = menssagem.substr				//Separo a descricao equipamento
			(
				0,
				menssagem.length()
			);	
	return 0;
}

inline int getTextoEquip(string menssagem,string& equipamento)				
{
	size_t achei = menssagem.find("\n");		//Encontro o primeiro pula-linha
	
	if (achei==string::npos)
    	return -1;
		
	menssagem.replace(0,achei+1,"");			//Apago a menssagem que nao interessa

	equipamento = menssagem;					//Separo nome do equipamento

	return 0;
}
