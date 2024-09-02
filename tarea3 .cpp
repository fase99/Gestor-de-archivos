#include <iostream>
#include <filesystem>
#include <fstream>
#include <cmath>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>
using namespace std;
namespace fs = filesystem;

void listar(){
	cout << "Lista de archivos: "<<endl;
	cout<<"\nRuta actual: "<<fs::current_path()<<endl<<endl;

	int a = 1;
	//auto entry_length{3UZ};

	for(auto const& dir_entry : fs::recursive_directory_iterator(fs::current_path())){
		cout<< a <<" -> "<<dir_entry<<endl;
		a++;
	}

	cout<<endl<<endl;
}
void navegacionDir(){
	string directorio;
	int op;
	
	cout<<"\n\n-------------NAVEGACION DIRECTORIOS---------------"<<endl;
	cout<<endl<<endl;
	
	while(op != 0){
		cout<<"1 -> Entrar a un directorio"<<endl;
		cout<<"2 -> Directorio padre"<<endl;
		cout<<"0 -> [SALIR]"<<endl;
		cout<<"SELECCIONE UNA OPCION: ";
		cin>>op;
		if(op == 1){
			cout<<"\nENTRAR AL DIRECTORIO: ";
			system("ls -l");
			cin.ignore();
			getline(cin, directorio);

			if(!fs::exists(directorio)){
				//fs::current_path(directorio);
				cout<<"\nEL DIRECTORIO NO EXISTE"<<endl;
				return;
			}	
			//fs::path directoryActual{directorio};
			fs::current_path(directorio);
			cout<<"Ruta actual: "<< fs::current_path()<<endl;
		}
		else if (op==2) {
			//salir al directorio padre
		
			fs::path pathPadre = fs::current_path().parent_path();
			fs::current_path(pathPadre);
			system("ls -l");
		}
		else{
			cout<<"ESCOJA UNA OPCION VALIDA"<<endl;
		}

	}

	//entrar en directorio
	/*fs::directory_iterator itr;
	for (const auto& entry : fs::recursive_directory_iterator()) {
		cout << entry.path().filename() << '\n';
		}	
	*/
}

void createDirectory(){
	cout<<"\n\n-------------CREAR DIRECTORIO---------------"<<endl;

	listar();

	string nombreDir;
	// Create a directory named "MyFolder" in the current path.
	cout<<fs::current_path()<<endl;
	cout<<"Ingrese nombre del directorio: "<<endl;
	cin.ignore();
	getline(cin, nombreDir);
	if(fs::exists(nombreDir)){
		cout<<"Esta carpeta ya existe!"<<endl;
		return;
	}

	fs::create_directory(nombreDir);
	system("ls -l ");

}

void crearArchivo(){
	cout<<"\n\n-------------CREAR ARCHIVO---------------"<<endl;


	ofstream archivo;
    cout<<"Escribir Nombre del archivo: ";
    string nombreArchivo;
    cin.ignore();
    getline(cin, nombreArchivo);
    
	if(fs::exists(nombreArchivo)){
		cout << "El archivo con el mismo nombre ya existe." << endl;
		return;
	}
    cout<<endl<<endl;
	archivo.open(nombreArchivo.c_str(), ios::out);
	if(archivo.fail()){
		cout<<"ERROR AL ABRIR ARCHIVO"<<endl;
		exit(1);
	}

	cout<<"ARCHIVO CREADO"<<endl;

	archivo.close();

	//system("ls -l");

}

void renombrar(){
	cout<<"\n\n-------------RENOMBRAR---------------"<<endl;

	fs::path p = fs::current_path();
	fs::current_path(p);

	int a;
	string original, nuevo;
	cout<<"\n1-> Renombrar archivo"<<endl;
	cout<<"\n2-> Renombrar directorio"<<endl;
	cout<<"\n\nSeleccione una opcion: ";
	cin>>a;
	
	switch (a)
	{
	case 1:
		
		cout<<"\n Nombre original del archivo: ";
		cin.ignore();
		getline(cin, original);
		if(!fs::exists(original)){
			cout<<"No se encontro el archivo para renombrar";
			return;
		}
		cout<<"\nNuevo nombre del archivo: ";
		cin.ignore();
		getline(cin, nuevo);

		fs::rename(original, nuevo);

		//system("ls -l");
		break;
	case 2:
		cout<<"\nNombre del directorio original: ";
		cin.ignore();
		getline(cin,original);

		if(!fs::exists(original)){
			cout<<"El directorio no existe";
			return;
		}

		cout<<"\nNuevo nombre del directorio: ";
		cin.ignore();
		getline(cin, nuevo);

		fs::rename(original, nuevo);
		//system("ls -l");
		break;
	default:
		break;
	}
}



void permisos(fs::perms p){

	auto show = [=](char op, fs::perms perm){
		if((p & perm) == perm) cout <<op; else cout << "-";
	};

	show('r', fs::perms::owner_read);
    show('w', fs::perms::owner_write);
    show('x', fs::perms::owner_exec);
    show('r', fs::perms::group_read);
    show('w', fs::perms::group_write);
    show('x', fs::perms::group_exec);
    show('r', fs::perms::others_read);
    show('w', fs::perms::others_write);
    show('x', fs::perms::others_exec);
	cout<<endl;

}

void cambiarPermisos(){

	cout<<"\n-------------GESTOR DE PERMISOS-------------"<<endl<<endl;

	cout<<"WARNING! Los permisos del archivo que selecciona seran removidos."<<endl<<endl;
	string a;

	system("ls -l");
		
		cout<<"\n[Ingrese 'salir' para cancelar la operacion]"<<endl;
		cout<<"\nSeleccionar archivo para modificar permisos: ";
		cin.ignore();
		getline(cin,a);

		if(a == "salir"){
			return;
		}

		if(!fs::exists(a)){
			cout<<"EL ARHIVO NO EXISTE"<<endl;
			return;
		}

	cout<<"Permisos actuales del archivo: "<< endl;

	permisos(fs::status(a).permissions());

				int n;
				cout<<"\nNuevos permisos: \n1.- Owner read\t2.- Owner write\
				\t3.- Owner execute\n4.- Group read\t5.- Group write\
				\t6.- Group execute\n7.- Other's read\t8.- Other's write\
				\t9.- Other's execute\n0.- Terminar"<<endl;
				fs::permissions(a,fs::perms::none);
				do{

					cin>>n;

						if(n == 1){
							fs::permissions(a,fs::perms::owner_read, fs::perm_options::add); 
							
						}
						else if(n == 2){
							fs::permissions(a,fs::perms::owner_write,fs::perm_options::add); 
							
						}
						else if(n==3){
							fs::permissions(a,fs::perms::owner_exec,fs::perm_options::add); 
							
						}
						else if(n==4){
							fs::permissions(a,fs::perms::group_read,fs::perm_options::add);
							
						}
						else if(n==5){
							fs::permissions(a,fs::perms::group_write, fs::perm_options::add); 
							
						}
						else if(n==6){
							fs::permissions(a,fs::perms::group_exec, fs::perm_options::add); 
							
						}
						else if(n==7){
							fs::permissions(a,fs::perms::others_read, fs::perm_options::add); 

						}
						else if(n==8){
							fs::permissions(a,fs::perms::others_write, fs::perm_options::add); 
							
						}
						else if(n==9){
							fs::permissions(a,fs::perms::others_exec, fs::perm_options::add); 
							
						}
						else{
							cout<<"Cancelado."<<endl;
							break;
						}
		
						cout<<"Agregue los que necesite, luego ingrese 0 para terminar"<<endl;
						}while(n!=0);
						
							cout<<"Permisos nuevos del archivo: "<< endl;
							permisos(fs::status(a).permissions());

					cout<<fs::current_path()<<endl;
						
}

struct HumanReadable
{
    std::uintmax_t size{};
 
private:
    friend std::ostream& operator<<(std::ostream& os, HumanReadable hr)
    {
        int o{};
        double mantissa = hr.size;
        for (; mantissa >= 1024.; mantissa /= 1024., ++o);
        os << std::ceil(mantissa * 10.) / 10. << "BKMGTPE"[o];
        return o ? os << "B (" << hr.size << ')' : os;
    }
};

void tipoArchivo(const fs::path& p, fs::file_status s){
	cout<<p<<": ";
	if (s.type() == fs::file_type::not_found) cout << "No existe";
	else if (s.type() == fs::file_type::none) cout<< "Aun sin evaluar";
	else if (s.type() == fs::file_type::directory) cout << "Directorio";
	else if (s.type() == fs::file_type::regular) cout << "Regular";
	else if (s.type() == fs::file_type::socket) cout << "Socket";
	else if (s.type() == fs::file_type::symlink) cout << "Enlace simbolico";
	else if (s.type() == fs::file_type::block) cout << "Block special file";
	else if (s.type() == fs::file_type::character) cout << "Character special file";
	else if (s.type() == fs::file_type::fifo) cout << "FIFO/pipe";
	else if (s.type() == fs::file_type::unknown) cout << "Unknown type";
	cout<<endl;

	cout<<endl;
}
void verMetadata(){
	cout<<"\n-------------METADATA-------------"<<endl<<endl;

	listar();

	string a;
	cout<<fs::current_path()<<endl;
	cout<<"\nSELECCIONE UN ARCHIVO: ";
	cin.ignore();
	getline(cin,a);

	if(!fs::exists(a)){
		cout<<"No existe el archivo"<<endl;
		return;
	}

	cout<<"\nMETADATA DE "<<a<<endl;

	cout<<"\nPERMISOS: ";
	permisos(fs::status(a).permissions());
	cout<<"\nTAMAÑO: "<<HumanReadable{fs::file_size(a)};
	
	cout<<"\nTIPO DE ARCHIVO: "; tipoArchivo(a, fs::status(a));

	cout<<endl<<endl;
}

void eliminarArchivo(){
	cout<<"\n--------------ELIMINAR ARCHIVO / CARPETA------------------"<<endl<<endl;

	cout<<"\nDirectorio actual: "<<fs::current_path()<<endl;

	listar();

	int op;

	cout<<"1. Eliminar un archivo"<<endl;
	cout<<"2. Eliminar una carpeta"<<endl;

	cout<<"Seleccione una opcion: ";cin>>op;

	if(op == 1){

		string a;
		cout<<"\nSeleccione un archivo o carpeta para borrar: ";
		cin.ignore();
		getline(cin, a);
		if (!fs::exists(a)) {
			cout<<"El archivo no existe."<<endl;
			return;
		}
		fs::remove(a);

		cout<<a<<" Ha sido eliminado"<<endl<<endl;

		listar();
	}
	else if(op == 2){
		string c;
		cout<<"[WARNING] Si se borra un directorio, se elimina todo su contenido"<<endl;
		cout<<"\nSeleccione la carpeta que desea eliminar: ";
		cin.ignore();
		getline(cin,c);

		if (!fs::exists(c)) {
			cout<<"La carpeta no existe"<<endl;
			return;
		}

		fs::remove_all(c);

		cout<<c<<" SE HA ELIMINADO JUNTO CON TODO SU CONTENIDO"<<endl<<endl;
	}
	else{
		cout<<"Opción incorrecta";
		return;
	}

	listar();
}

void moverArchivo(){
	cout<<"\n------------MOVER ARCHIVO / CARPETA------------\n"<<endl;

	cout<<fs::current_path()<<endl<<endl;

	listar();

	int op;

	cout<<"1. Mover un archivo"<<endl;
	cout<<"2. Mover una carpeta"<<endl;

	cout<<"Seleccione una opcion: ";cin>>op;

	if(op == 1){
		string a;
		cout<<"Archivo que desea mover: "<<endl;
		cin.ignore();
		getline(cin,a);
		if(!fs::exists(a)){
			cout<<"El archivo no existe"<<endl;
			return;
		}
		string ruta;

		cout<<"Ingrese ruta donde desea mover el archivo: "<<endl;
		cin.ignore();
		getline(cin,ruta);

		string rel = "./"+ruta;
		fs::copy(a, ruta);

		const auto copyOptions = fs::copy_options::update_existing
                           | fs::copy_options::recursive
                           | fs::copy_options::directories_only
                           ;

		fs::copy(a,ruta,copyOptions);
		
		

		listar();
	}
	else if(op == 2){

		string c;
		cout<<"Carpeta que desea mover: "<<endl;
		cin.ignore();

		if(!fs::exists(c)){
			cout<<"La carpeta no existe"<<endl;
			return;
		}
		
		string ruta;
		cout<<"Ingrese ruta donde desea mover la carpeta: "<<endl;
		cin.ignore();
		getline(cin,ruta);

		string rel = "./"+ruta;
		fs::copy(c, rel);

		//fs::remove_all(c); 

		listar();
	}
	else{
		cout<<"Opción incorrecta";
		return;
	}
}

int main(){

int s;

	cout<<"\n-------------BIENVENIDO AL GESTOR ARCHIVOS-------------"<<endl<<endl;

	while(s != 0){

    	cout<<"1. LISTAR"<<endl;
    	cout<<"2. NAVEGAR ENTRE DIRECTORIOS"<<endl;
	    cout<<"3. CREAR DIRECTORIO"<<endl;
		cout<<"4. CREAR ARCHIVO"<<endl;
    	cout<<"5. RENOMBRAR ARCHIVO / DIRECTORIO"<<endl;
		cout<<"6. CAMBIAR PERMISOS"<<endl;
		cout<<"7. VER METADATA DE UN ARCHIVO"<<endl;
		cout<<"8. ELIMINAR UN ARCHIVO / DIRECTORIO"<<endl;
		cout<<"9. MOVER ARCHIVO / DIRECTORIO"<<endl;	
		cout<<"[0] SALIR"<<endl;
		
    	cout<<"\nSELECCIONE UNA OPCION: ";
    	cin>>s;


    	switch (s)
    	{
    	case 1:
    	    listar();
    	    break;
    	case 2:
			navegacionDir();
			cout<<"\n\n"<<fs::current_path()<<endl<<endl;
    		break;
    	case 3:
			createDirectory();

    		break;
    	case 4:
			crearArchivo();
    		break;
		case 5:
			renombrar();
			break;
		case 6:
			cambiarPermisos();
			break;
		case 7:
			verMetadata();
			break;
		case 8:
			eliminarArchivo();
			break;
		case 9:
			moverArchivo();
    	default:
    	    cout<<"SELECCION INVALIDA"<<endl;
        	break;
    	}

	}

	return 0;


}
