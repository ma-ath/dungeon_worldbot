#include <iostream>
#include <string>
#include <vector>


using namespace std;

class erro
	{
		public:
		erro(string s) : err(s) {}
		string getErro(){return err;}
		private:
			const string err;
	};

class Serializable
{
public:
    Serializable(){}
    virtual ~Serializable(){}

    virtual void serialize(std::ostream& stream) = 0;
    virtual void deserialize(std::istream& stream) = 0;
};

template <class T> class Atributo : public Serializable 	//Um atributo é algo como XP, NIVEL, FOR, INT, etc... Pode ser qualquer stats do personagem,ou stats de equipamento. Possui valor,valor max e modificador.
{
	public:
		
		Atributo(string n) : nome(n) {};

		Atributo& set(T valor)						//Seta o valor do atributo
			{
				this->valor = valor;
				return *this;
			}

		Atributo& setMax(T max)						//Seta o valor do atributo
			{
				this->max = max;
				return *this;
			}

		Atributo& setMod(T modificador)				//Seta o valor do modificador do atributo
			{
				this->modificador = modificador;
				return *this;
			}

		T get()										//Pega o valor do atributo
			{
				return this->valor;
			}

		T getMax()										//Pega o valor do atributo
			{
				return this->max;
			}

		T getMod()									//Pega o valor do modificador do atributo
			{
				return this->modificador;
			}

		string getNome()							//Pega o nome do atributo
			{
				return this->nome;
			}


    virtual void serialize(std::ostream& stream)
    {
        // Serialization code
        stream << valor << " " << max << " " << modificador << " ";
    }

	virtual void deserialize(std::istream& stream)
    {
        // Deserialization code
        stream >> valor >> max >> modificador;
    }

	private:
		T valor;
		T max;
		T modificador;
		const string nome;								//guarda apenas o nome do atributo. ex: "XP", "FOR" ... Só é escrito uma vez (const).

};

class Item : public Serializable 	//Um Item é qualquer coisa que o pesonagem carrega em sua bolsa de equipamentos.
{
	public:
		Item(string n,int p) : 		//Inicializo o Item com "nome" e "peso".
			nome(n),
			peso(p),
			condicao("Condicao"),	//Quao bom o Item esta para ser usado.
			uso("Uso")				//Quantos usos eu ainda possuo do Item. 
				{
					quantidade = 1;			//Default
					descricao = ".";		//Default
					uso.set(10);			//Default
					uso.setMax(10);			//Default
					condicao.set(100);		//Default


					uso.setMod(0);			//Nao uso
					condicao.setMod(0);		//Nao uso
					condicao.setMax(100);	//A melhor condicao possivel do Item eh sempre 100%
				};

		Item& operator=(Item& outro)
			{
				if(&outro != this)  
				{
					nome = outro.getNome();
					peso = outro.getPeso();
					descricao = outro.getDescricao();
					quantidade = outro.getQuantidade();
	
					condicao.set(outro.getCondicao());	
					uso.set(outro.getUso());			
					uso.setMax(outro.getUsoMax());
				}
				return *this;
			}

		Item& setDescricao(string descricao)						//Seta uma possivel descricao do Item
			{
				this->descricao = descricao;
				return *this;
			}

		Item& setCondicao(int condicao)							//Seta a condicao do Item. So eh possivel setar condicoes entre 0 e 100.
			{
				if ((condicao <= 100)&&(condicao >= 0))
					{
						this->condicao.set(condicao);
						return *this;
					}
			}

		Item& setUso(int uso)										//Seta numero de uso que ainda possuo do Item
			{
				this->uso.set(uso);
				return *this;
			}

		Item& setUsoMax(int uso)									//Seta numero de uso que o Item possui no maximo
			{
				this->uso.setMax(uso);
				return *this;
			}
		
		Item& setQuantidade(int quantidade)						//Seta a quantidade que possuo do Item
			{
				this->quantidade = quantidade;
				return *this;
			}


		int getCondicao()											//Pega a condicao do Item
			{
				return this->condicao.get();
			}

		int getUso()												//Pega quantos usos o Item tem
			{
				return this->uso.get();
			}

		int getUsoMax()											//Pega os usos max do Item
			{
				return this->uso.getMax();
			}

		string getDescricao()										//Pega a descricao
			{
				return this->descricao;
			}

		string getNome()											//Pega o nome do Item
			{
				return this->nome;
			}

		int getPeso()												//Pega o peso do Item
			{
				return this->peso;
			}

		int getQuantidade()											//pega a qantidade do Item
			{
				return this->quantidade;
			}

  	virtual void serialize(std::ostream& stream)
		   	{
	    	    stream << nome << " " << peso << " " << descricao << " " << quantidade << " ";
				condicao.serialize(stream);
				uso.serialize(stream);
		   	}

	virtual void deserialize(std::istream& stream)
	    	{
	    	    stream >> nome >> peso >> descricao >> quantidade;
				condicao.deserialize(stream);
				uso.deserialize(stream);
		   	}

	private:
		string nome;				//guarda apenas o nome do Item. ex: "Faca", "Poção" ... Só é escrito uma vez ('const').
		int peso;
				
		Atributo <int> condicao;
		Atributo <int> uso;
		string descricao;
		int quantidade;
};


class Equipamento : public Serializable
{
	public:
		Equipamento()	:	equipamento(1,Item("Bolsa-de-Equipamentos",0))	//O primeiro equipamento (equipamento.at(0))
																			//eh chamado simplesmente de bolsa de equipamentos, e possui peso 0
			{}


		bool adicionarEquipamento(Item item)	//Adiciona um Item a bolsa de equipamentos.
												//Só é possivel adicionar equipamentos com nomes diferentes.
												//Retorna 1 caso não seja possivel adicionar, 0 caso foi adicionado
			{
				unsigned int i = 0;
				do
				{
					if (equipamento.at(i).getNome() == item.getNome())		//Caso ja exista o equipamento na bolsa
						return 1;											//Nao adiciona e retorna erro
				i++;
				}while(i < equipamento.size());

				this->equipamento.push_back(item);	//Caso nao encontre um equipamento com mesmo nome, adiciona o equipamento a bolsa
				return 0;
			}

		bool retirarEquipamento(string nome)		//retira um Item da bolsa, dado o nome dele
			{
				unsigned int i = 0;
				do
				{
					if (equipamento.at(i).getNome() == nome)		//Encontrei
					{
						for(unsigned int j = i; j < equipamento.size()-1;j++)	//Shifta todos os elementos
						{
							equipamento.at(j) = equipamento.at(j+1);
						}
						equipamento.pop_back();
						return 0;									//E retorna sucesso
					}
				i++;
				}while(i < equipamento.size());
				return 1;											//Caso nao encontre o nome do Item, retorna erro
			}

		Item& acessarItemNome(string nome) noexcept(false)				//Obter acesso a um item dentro da bolsa a partir de seu nome.
			{
				unsigned int i = 0;
				do
				{
					if (equipamento.at(i).getNome() == nome)		//Caso eu encontre o equipamento na bolsa
						return equipamento.at(i);					//Retorna o equipamento
				i++;
				}while(i < equipamento.size());

				throw erro("Item Inexistente");						//Caso nao encontre um equipamento com mesmo nome, retorna uma essecao
			}

		Item& acessarItemIndice(int i)								//Obter acesso a um item dentro da bolsa a partir de seu indice.
			{
				if ((i >= 0)&&((unsigned)i<equipamento.size())) 				//Caso seja um indice valido
					return equipamento.at(i);						//Retorna o equipamento
				return equipamento.at(0);							//Caso contrario retorna o item 0, default
			}

		int getNumeroItens()
			{
				return equipamento.size()-1;
			}

    	virtual void serialize(std::ostream& stream)
			{	cout << "iniciando serializacao!"<<endl;
				stream << equipamento.size()-1 << " ";				//Salva quantos itens eu tenho no inventario
				
				for(unsigned i = 0;i <= equipamento.size()-1;i++)	//Salva itens
				{
					equipamento.at(i).serialize(stream);
				}
				
			}

    	virtual void deserialize(std::istream& stream)
			{
				int numeroDeItens;

				stream >> numeroDeItens;							//Carrega quantos itens eu tinha no inventario
				
				for(int i = 0;i < numeroDeItens;i++)				//Crio itens genericos para preencher inventario
				{
					Item *item = new Item(".",0);
					equipamento.push_back(*item);
				}
				
				for(int i = 0;i <= numeroDeItens;i++)				//Carrego os itens do inventario
				{
					equipamento.at(i).deserialize(stream);
				}
			}
	private:
		vector <Item> equipamento;
	
};


class Movimento
{
	public:
		Movimento(string mov)
		{
			this->mov = mov;
		}

	private:
		string mov;
};

class Personagem{

	public:
		
		Personagem(string nome,string raca,string classe) : //Eu inicializo o personagem com nome, raca e classe;
		
		Nome(nome),
		Raca(raca),
		Classe(classe),
		PV("PV"),					//Pontos de Vida	
		Armadura("Armadura"),		//	.	.	.
		Dano("Dano"),
		Nivel("Nivel"),
		XP("XP"),
		Carga("Carga"),
		FOR("Forca"),
		DES("Destreza"),
		CON("Controle"),
		INT("Inteligencia"),
		SAB("Sabedoria"),
		CAR("Carisma"),
		Ouro("Ouro"),
		equipamento(),
		movimento("")
			{}

		string getNome()
		{
			return this-> Nome;
		}
		string getRaca()
		{
			return this-> Raca;
		}
		string getClasse()
		{
			return this-> Classe;
		}


	private:
		
		const string Nome;
		const string Raca;
		const string Classe;

	public:

		Atributo <int> PV;	
		Atributo <int> Armadura;
		Atributo <int> Dano;
		Atributo <int> Nivel;
		Atributo <int> XP;
		Atributo <int> Carga;
		Atributo <int> FOR;
		Atributo <int> DES;
		Atributo <int> CON;
		Atributo <int> INT;
		Atributo <int> SAB;
		Atributo <int> CAR;
		Atributo <int> Ouro;

		Equipamento equipamento;
		Movimento movimento;
};
