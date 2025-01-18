# Dishwasher Controller Assessment

> [!IMPORTANT]
> This code is my solution for a job application assessment I've participated in. And has no real life application.

The main goal of this assessment is to write a portable C library meant to be used in a connectivity module of an internet enabled dishwasher.
This connectivity module is separated from the dishwasher controller. These two communicate through a serial connection. The later is being developed in parallel to this connectivity module and therefore its behavior must be mocked.

This repository comprises of two parts: the connectivity module library (in C); And a dishwasher simulator (in Python).
