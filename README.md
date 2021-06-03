# puppetmaster

# !!!WARNING!!!
### This is a real virus targeting Windows 10 - If you run it without precautions it will infect your computer

# What is this?
This is a virus targeting windows 10 that I implemented during my undergrad. It's written in C. It was written in a very tight timeframe and the code could definitely use some work.
### Attack vector
The program uses file channels to move binaries and places itself on top of them. When executed, the virus searches for more binaries to infect, then launches the binary the user intended to run.
### Polymorphism
The virus constantly reencrypts itself with a unique key every time it infects a new file. This makes it virtually impossible to detect with classical antivirus methods.