# StorytellGame
 A voice storytelling choices matter survival game.

## Compilação e instalação do driver
 1. Abra o terminal.
 2. No terminal, vá para a pasta onde está o código do driver e Digite :
 `
 $ make
 `

 3. Depois :
 ```
 $ sudo insmod altera_driver.ko
 ```

 4. Digite :
 ```
 $ dmesg
 ```
 Para mostrar mensagens do kernel e confirmar que o driver da altera foi inserido com sucesso. É necessário que apareçam mensagens parecidas com as seguintes:
 ``` 
 altera_driver: Found Vendor Id: 41172
 altera_driver: Resource Start at bar 0: 80200000
 altera_driver: char+pci drivers registered
```
Aparecendo todas essas informações indica que a gravação do .sof foi realizada com sucesso na placa. Caso só apareça "altera_driver: char+pci drivers registered" procure reiniciar a placa ou até mesmo gravar a placa novamente.

5. Criando arquivo de link entre driver e aplicação. Crie um arquivo com o seguinte comando: 
 ```
 $ sudo mknod /dev/de2i150_altera c 91 1 
 ```

6. Para remover o driver digite : 
```
$ sudo rmmod altera_driver
```

## Compilação da aplicação
1. No terminal, vá para a pasta onde está o código do arquivo e digite :
```
$ make clean
```

2. Depois:
```
$ make
```

3. E, por fim :
```
$ sudo ./main
```
