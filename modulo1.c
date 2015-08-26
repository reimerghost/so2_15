#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
//Reimer Adolfo Chamalé Linares
//200815270

//Contadores
int tam,tam2;
//Mensaje en memoria
char *msg;

//Simbolo público
EXPORT_SYMBOL(msg);

//Metodo para leer archivo virtual en proc
int read_proc(struct file *filp,char *buf,size_t count,loff_t *offp )
{
	if(count>tam2)
	{
		count=tam2;
	}

	tam2=tam2-count;
	copy_to_user(buf,msg, count); //Da la información al usuario
	if(count==0)
		tam2=tam;
	return count;
}

//Metodo para escribir en el archivo virtual en Proc
int write_proc(struct file *filp,const char *buf,size_t count,loff_t *offp)
{
	copy_from_user(msg,buf,count); //Obtiene información dada por el usuario (desde consola)
	tam=count;
	tam2=tam;
	return count;
}

//Aqui defino las operaciones que tendrá el archivo virtual (y los nombres de los metodos, dentro de este archivo, que llamará para eso)
struct file_operations proc_fops = {
	read: read_proc,
	write: write_proc
};

//Metodo para crear el archivo virtual de proc
void create_new_proc_entry()
{
	//Se crea el archivo virtual, con el nombre "modulo1" y las operaciones dadas (en el struct.)
	proc_create("modulo1",0,NULL,&proc_fops);
	msg=kmalloc(GFP_KERNEL,10*sizeof(char));
}


//Metodo que se llama al cargar el modulo (aqui se llama el metodo que crea el archivo virtual)
int proc_init (void) {
	create_new_proc_entry();
	return 0;
}

void proc_cleanup(void) {
 remove_proc_entry("modulo1",NULL);
}

module_init(proc_init);
module_exit(proc_cleanup);

MODULE_LICENSE("GPL");
