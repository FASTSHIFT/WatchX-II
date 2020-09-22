# WatchX-II 更新日志
> https://github.com/FASTSHIFT/WatchX-II

## [v0.1] - 2020-08-31
* 1.框架搭建完成，UI基于littlevGL v7.4.0，跑分120FPS+
* 2.硬件I2C调试通过，所有设备读写正常
* 3.添加RTC库
* 4.添加AT32F403A文档

## [v0.2] - 2020-09-02
* 1.添加LVGL PDF文档
* 2.添加字体转换工具：LvglFontTool，图片转换工具：Lvgl_image_convert_tool
* 3.添加第一个表盘页面
* 4.添加新字体

## [v0.3] - 2020-09-07
* 1.表盘设计完成
* 2.修改mono主题
* 3.添加新的字体RexBold
* 4.添加label滑动效果库
* 5.添加页面注册宏

## [v0.4] - 2020-09-22
 * 1.调整DISP刷新周期至20ms
 * 2.修改PageManger的event类型为uint8_t，与lv_obj_event_cb兼容
 * 3.更优雅的页面注册宏，使用PAGE_EXPORT和PAGE_IMPORT
 * 4.使用lvgl提供的gestute功能，屏幕的gestute事件会传递给页面事件
 * 5.独立DisplayPage.cpp
 * 6.加入微软雅黑字体
 * 7.添加计算器页面
 * 8.添加主菜单，精致优雅的非线性动画
 * 9.使用宏简化表盘页面繁琐的动画函数
 * 10.添加lv_port_log日志输出接口
 * 11.添加专属主题lv_theme_watchx
 * 12.添加LVGL扩展功能文件组lv_ext
