# 图形学学习

## 参考书籍 《Fundamentals of Computer Graphics 4th》

## 本项目需要依赖 SDL2
需要首先安装 SDL2

```bash 
sudo apt install libsdl2-2.0
sudo apt install libsdl2-dev
sudo apt install libsdl2-mixer-dev
sudo apt install libsdl2-image-dev
sudo apt install libsdl2-ttf-dev
sudo apt install libsdl2-gfx-dev
``` 

## BasicPipline 一个简单的软渲染管线 demo
采用 C 编写，但是由于基础知识掌握得不牢靠，这个项目废弃了，我将新建一个 SimpleRaster 来代替。
## SimpleRayTracing 是一个简单的光线追踪 demo 学习虎书 1~4 章节的成果
基本将虎书 1-4 章所讲解的知识点实现，采用 C++ 编写。
## SimpleRaster
一个简单的软光栅，采用 phong shading 渲染一个棱锥。
## GraphicRender
用 C 语言重写的软渲染器，不定时更新，作为学习《Fundamentals of Computer Graphics 4th》的成果
