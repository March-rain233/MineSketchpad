# 课程设计报告

* 组员： 李昱辉， 王梓萌
* 题材：类似sai2的具有绘图功能的图像处理软件
* 视频地址：[中国海洋大学夏季学期c++实践项目_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1ag411c78V/)
* 仓库地址：[March-rain233/MineSketchpad: c++课程设计项目 (github.com)](https://github.com/March-rain233/MineSketchpad)/[MineSketchpad: c++课程设计项目 (gitee.com)](https://gitee.com/March__rain/MineSketchpad)

### 设计概述

---

​	<s>我不是太会学术性的总结一些东西，这个文档我就口水化啦。</s>首先是显示图片功能的制作，是通过QPainter的DrawImage接口来实现，他需要一个QImage的对象，这个我通过实现Image类的ToQImage来生成一个QImage对象实现。

​	在画板类中维护着一个图层栈，是画板的核心数据，这个图层有自己定义的一个类LayerModel，这个类是画板的模型，具备可视、锁定、以及图像数据与图层效果等属性，画面显示是通过画板从最底层的图层向上传递一张画布（Image）每个图层根据自己的图层效果将图像数据渲染到画布上，当所有图层渲染结束后，画布将被转换为QImage来进行显示。每个图层具备一个源数据以及一个缓冲区，当对图层进行修改时需要调用BeginPaint来通知图层将源数据复制进缓冲区内，让外界修改缓冲区，并在修改结束后调用EndPaint来将缓冲区数据读入源数据内，或是调用CancelPaint来取消这次的修改。

