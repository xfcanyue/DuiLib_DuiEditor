c

### 1. 开源说明


```c++
本项目是针对DuiLib开发的可视化界面设计工具，可实际工作中使用
如有想参与本工具的开发欢迎加入：
							duilib设计器交流群：819272442
```

### 2.  项目帮助	

```c++
本项目基于以下：
	https://gitee.com/Liqs99/DuiLib_DuiEditor
	https://github.com/xfcanyue/DuiLib_DuiEditor
-------------------------------------------------------------------------
相关教程与说明：	
duilib设计器 DuiEditor简易教程 (DuiDesigner) （一）
https://blog.csdn.net/mmcanyu/article/details/106758857

duilib设计器 DuiEditor简易教程 (DuiDesigner) （二）
https://blog.csdn.net/mmcanyu/article/details/106799975

duilib设计器 DuiEditor简易教程 (DuiDesigner) （三）
https://blog.csdn.net/mmcanyu/article/details/106843748

DuiEditor高级教程
https://blog.csdn.net/mmcanyu/article/details/106976013

duilib仿百度网盘界面
https://blog.csdn.net/mmcanyu/article/details/108905986

DuiLib中图片属性的编辑
https://blog.csdn.net/mmcanyu/article/details/111148886

正式发布DuiLib脚本系统 结合angelscript 
https://blog.csdn.net/mmcanyu/article/details/112685524
```
### 3. 项目编译说明：

DuiEditor2010.sln	VS2010解决方案  
DuiEditor2019.sln	VS2019解决方案  

编译选项说明：  
Debug				多字节 - 调试  
Release				多字节 - 发行  
UnicodeDebug		Unicode - 调试  
UnicodeRelease		Unicode - 发行  
xsDebug				静态库 - 多字节 - 调试  
xsRelease			静态库 - 多字节 - 发行  
xsUnicodeDebug		静态库 - Unicode - 调试  
xsUnicodeRelease	静态库 - Unicode - 发行  

除非为了兼容老项目，新项目优先使用Unicode版本。 

    
   
   ### 4. 重要说明

设计器创建控件和修改属性依据`bin\DuiLib.xml`，理论上只要遵循这个约定的duilib版本都可以使用这款设计器。