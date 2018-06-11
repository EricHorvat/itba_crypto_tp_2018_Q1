## TPE Criptografia y seguridad ~ ITBA ~ 2018 1er Cuatrimestre  
### Compilacion  
  
Utilizamos la herramienta _cmake_ para compilar. Para correr el mismo se debe hacer lo siguiente:  
  
```bash
cd DIR_PATH #Donde DIR_PATH es el directorio donde se creara el ejecutable  
cmake PROJECT_PATH  #Donde PROJECT_PATH apunta a la raiz del proyecto  
make
```  
  
Esto complilara el proyecto creando el ejecutable __cripto_tp__  
  
### Ejecucion
  
Nuestro stegobmp se llama **cripto_tp**, y se ejecuta de la siguiente manera:  
  
```bash
cripto_tp <-extract | -embed -in INPUT_FILE> -p PORTER_FILE -out OUTPUT_FILE\
	-steg LSB1|LSB4|LSB8|LSBE [-pass PASSWORD [-a aes128|aes192|aes256|des]	\
	[-m cbc|cfb|ecb|ofb]]  
```
Los posibles argumentos son los siguientes:

```bash
The required arguments are:  
	-extract:	Set the program to get a inner file from the porter file.  
	-embed:		Set the program to get a inner file from the porter file.	\
		So the -in argument is required.  
	-p:			Set the program to get PORTER_FILE as porter file.  
	-out:		Set the program to get OUTPUT_FILE as output file.  
	-steg:		Set the stenography strategy. There are options LSB1, LSB4,	\
		LSB8 and LSBE.  
  
The special arguments are:  
	-in: Set the program to get INPUT_FILE as output file. Is required if	\
	 	the -extract argument is passed.  

The optional arguments are:  
	-pass: Set PASSWORD as the password to encrypt  
	-a: Set the stream cipher function. There are options aes128, aes192,	\
	 	aes256 and des. If -pass passed the default is aes128.  
	-m: Set the stream cipher strategy. There are options cbc, cfb, ecb and	\
		ofb. If -pass passed the default is cbc.
```