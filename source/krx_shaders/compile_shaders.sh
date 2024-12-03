#!/bin/bash

function Compile
{
	gcc -shared -std=c17 -fPIC -O3 -o $1 $2
}

Compile "hello_triangle_vert.krxsl" "hello_triangle_vert.c"
Compile "hello_triangle_frag.krxsl" "hello_triangle_frag.c"