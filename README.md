# Dishwasher Controller Assessment

> [!IMPORTANT]
> This code is my solution for a job application assessment I've participated in. And has no real life application.

The main goal of this assessment is to write a portable C library meant to be used in a connectivity module of an internet enabled dishwasher.
This connectivity module is separated from the dishwasher controller. These two communicate through a serial connection. The later is being developed in parallel to this connectivity module and therefore its behavior must be mocked.

This repository comprises of two parts: the connectivity module library (in C); And a dishwasher simulator (in Python).

# How to build/use project
## Dishwasher Simulator

1. Install python dependencies:

```
pip3 install -r dishwasher_simulator/requirements.txt
```

2. Run simulator:

```
python3 dishwasher_simulator/main.py
```

## Controller library

1. Initialize git submodules:

```
git submodule init
git submodule update 
```

2. Choose a target (e.g. tests) and run the make command to build it:

```
cd controller
make tests
```