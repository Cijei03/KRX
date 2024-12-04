SoftwareRasterizer is (like title says) software rasterizer based on CPU computing. It can be used by my own C++ API called _teg_. 
![alt text](https://cijei03.github.io/resources/panther3d.png)

As you can see on the image it was supporting depth testing and barycentric interpolation. Additionally it was supporting color blending.
These triangles has been rasterized from vertices in buffers (which also was storing colors per vertex).
All rendering operations was performed on CPU on single thread. It supports only Linux and uses Xlib.
Now this project is outdated, so I will rename it into KRX and I am rewriting it for better performance and better code.
More features planned :)

![alt text](https://cijei03.github.io/resources/krx_logo.png)
