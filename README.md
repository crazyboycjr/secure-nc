# secure-nc
simple netcat base on DES which encrypt its secret with RSA......

## Dependency
- gmp
- gmpxx

## Compile
Firstly, you should execute
```
autoreconf -i
```
to check environments and generate `./configure` script, then
```
./configure && make
```
to compile.

## Usage
```
./snc server|client <addr> <port>
```
You should first run
```
./gen-key
```
to generate a pair of RSA public key and private key.
Then open two terminal and type respectively
```
./snc server 127.0.0.1 12346
./snc client 127.0.0.1 12346
```
Then you can take turns to send messages.

## TODO
- <addr> only support ipv4 address
- Communication must take turns
- Saving format and permission of RSA pubkey and prikey
- Many other things
