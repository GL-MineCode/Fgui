# Fgui --- Fair GUI Library for SDL2

## 注意看这是一个基于 SDL2 的 GUI 库，用于快速开发基于 SDL2 的游戏或应用。  
噢等等，那个是脏区重绘机制吗？没错这款库竟然支持脏区重绘，这使得它在处理复杂场景时能够保持良好的性能（并非，SDL2每次Render Present 都需要交换整个缓冲区，所以性能会受到影响，导致其可能没有其他GUI库比如WinUI占用少。但这起码是SDL2这种蹩脚轮子能写出来的最好的带脏区重绘优化的GUI库了）

## 优点
- 易于做跨平台兼容
- 支持脏区重绘优化，性能比每帧画要好一点（SDL2对性能优化一点也不友好，我已经尽力了）
- 支持自定义控件
- 支持更换配色方案（？我不知道这算不算值得说的优点，但还是加上吧）
- 我已经很努力地去编了，但好像就这些（

## 缺点
- 没排版引擎（这将是硬伤，等我有空和GenFromXML功能一起出）

## 支持的控件

- ScrollBarV --- 垂直滚动条控件
- ScrollBarH --- 水平滚动条控件
- ControlBox --- 控件容器控件
- PushButton --- 按钮控件
- StaticText --- 静态文本控件
- LinkText --- 链接文本控件
- InputBox --- 输入框控件 (Where pain and suffering begins >:( )
- CheckBox --- 复选框控件
- ToggleButton --- 开关控件
- RadioButton --- 单选框控件
- PictureBox --- 图片框控件
- Slider --- 滑动条控件
- ComboBox --- 下拉列表框控件
- ProgressBar --- 进度条控件
- TabControl --- 选项卡控件

### 可选编译控件
- QRCodeBox --- 二维码控件，基于QR-Code-generator库实现

## 未来计划
- 支持排版引擎，用于自动布局控件，属性类似CSS
- 支持GenFromXML功能，用于从XML文件加载并生成控件(It saves your hands!   -   指可以AI生成XML文件，而不是手动写)

## Contributor

### 我(GL)
### D指导(DeepSeek，后来加入的，他修复了我2年来写的屎山代码，他比我更懂写GUI，我是完全没经验纯摸索的)

## 后记

真的有人试吃我的屎山代码吗？这玩意就是我周末抽空写的一个学习项目而已。但是我会试图让它更好的（自我激励）  

请原谅我Commit的注释，它们惨不忍睹

:) GL 2026-08-12  
2067年還在看的請按贊  
67 👍 67 👎        举报