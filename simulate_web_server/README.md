# Simple web server simulate

----
# What is it?

It is a simple simulation of a web server. We do not intend to handle
connections or any detail; here we just try to simulate main processes that
manage child process by passing it to request it. We use it to better study
MVAS behavior.

----
# How to compile

Compile without MVAS and with a few output

```
make
```

Compile with MVAS

```
make LIBVAS=-lmvas
```

----
# How to run

On a terminal:

```
bin/simulate_web_server
```

In another termina
```
kill -SIGUSR1 <PID-PAI>
``
