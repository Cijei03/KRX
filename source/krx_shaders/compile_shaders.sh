#!/bin/bash

KRXSL_OPTIMIZATION_ENABLED=true

function Compile
{
	if [ $KRXSL_OPTIMIZATION_ENABLED ]
	then
		gcc -shared -std=c17 -fPIC -O3 -o $1 $2
	else
		gcc -shared -std=c17 -fPIC -o $1 $2
	fi
}

Compile "hello_triangle_vert.krxsl" "hello_triangle_vert.c"
Compile "hello_triangle_frag.krxsl" "hello_triangle_frag.c"
Compile "depth_and_blending_vert.krxsl" "depth_and_blending_vert.c"
Compile "depth_and_blending_frag.krxsl" "depth_and_blending_frag.c"