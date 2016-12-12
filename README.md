# secure-nc
Simple netcat based on DES which encrypt its secret key with RSA......

## Dependency
- libgmp
- libgmpxx

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
- Only support ipv4 address
- Communication must take turns
- Saving format and permission of RSA pubkey and prikey
- Many other things

---
虽说是snc，但名不副实，说是simple，这东西连simple都算不上，说secure它又不怎么secure...
